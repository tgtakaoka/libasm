/*
 * Copyright 2020 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dis_ns32000.h"

#include "table_ns32000.h"

namespace libasm {
namespace ns32000 {

const char DisNs32000::OPT_BOOL_STROPT_BRACKET[] PROGMEM = "stropt-bracket";
const char DisNs32000::OPT_DESC_STROPT_BRACKET[] PROGMEM = "string instruction operand in []";
const char DisNs32000::OPT_BOOL_PCREL_PAREN[] PROGMEM = "pcrel-paren";
const char DisNs32000::OPT_DESC_PCREL_PAREN[] PROGMEM = "addr(pc) as program counter relative";
const char DisNs32000::OPT_BOOL_EXTERNAL_PAREN[] PROGMEM = "external-paren";
const char DisNs32000::OPT_DESC_EXTERNAL_PAREN[] PROGMEM = "disp2(disp(ext)) as extenal addressing";
const char DisNs32000::OPT_BOOL_FLOAT_PREFIX[] PROGMEM = "float-prefix";
const char DisNs32000::OPT_DESC_FLOAT_PREFIX[] PROGMEM = "float constant prefix 0f (default none)";

static bool isGenMode(AddrMode mode) {
    return mode == M_GENR || mode == M_GENW || mode == M_GENC || mode == M_GENA || mode == M_FENR ||
           mode == M_FENW;
}

static uint8_t getOprField(const InsnNs32000 &insn, OprPos pos) {
    if (pos == P_GEN1)
        return (insn.hasPost() ? insn.post() : insn.opCode()) >> 3;
    if (pos == P_GEN2 && insn.hasPost()) {
        const uint8_t post = (insn.post() << 2) & 0x1f;
        return post | (insn.opCode() >> 6);
    }
    if (pos == P_REG)
        return (insn.opCode() >> 3) & 0x7;
    if (pos == P_SHORT && insn.hasPost()) {
        uint8_t val8 = (insn.post() & 7) << 1;
        if (insn.opCode() & 0x80)
            val8 |= 1;
        // Sign extends 4-bit number.
        val8 = (val8 & 7) - (val8 & 8);
        return val8;
    }
    return 0;
}

static bool isScaledIndex(uint8_t gen) {
    return gen >= 0x1c;
}

Error DisNs32000::readIndexByte(DisMemory &memory, InsnNs32000 &insn, AddrMode mode, OprPos pos) {
    if (isGenMode(mode) && isScaledIndex(getOprField(insn, pos)))
        insn.setIndexByte(insn.readByte(memory), pos);
    return setError(insn);
}

StrBuffer &DisNs32000::outDisplacement(StrBuffer &out, const Displacement &disp) {
    switch (disp.bits) {
    case 7:
        return outDec(out, static_cast<int8_t>(disp.val32), -8);
    case 14:
        return outHex(out, static_cast<int16_t>(disp.val32), -16);
    default:
        return outHex(out, disp.val32, -30);
    }
}

Error DisNs32000::readDisplacement(DisMemory &memory, InsnNs32000 &insn, Displacement &disp) {
    const uint8_t head = insn.readByte(memory);
    if ((head & 0x80) == 0) {
        // 0xxx|xxxx
        // Sign extends 7-bit number as 0x40 is a sign bit.
        const int8_t val8 = (head & 0x3F) - (head & 0x40);
        disp.val32 = val8;
        disp.bits = 7;
    } else {
        // Sign extends 14-bit number as 0x2000 is a sign bit.
        const int8_t val8 = (head & 0x1F) - (head & 0x20);
        const int16_t val16 = (static_cast<int16_t>(val8) << 8) | insn.readByte(memory);
        if ((head & 0x40) == 0) {
            // 10xx|xxxx
            disp.val32 = static_cast<int32_t>(val16);
            disp.bits = 14;
        } else {
            // 11xx|xxxx
            if (head == 0xE0)
                return setError(ILLEGAL_CONSTANT);  // 1110|0000 is reserved
            const int32_t val32 = (static_cast<int32_t>(val16) << 16) | insn.readUint16(memory);
            if (val32 < -0x1F000000L || val32 >= 0x20000000L)
                return setError(OVERFLOW_RANGE);
            disp.val32 = val32;
            disp.bits = 30;
        }
    }
    return setError(insn);
}

Error DisNs32000::decodeLength(
        DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode) {
    Displacement disp;
    if (readDisplacement(memory, insn, disp))
        return getError();
    if (disp.val32 < 0)
        return setError(ILLEGAL_CONSTANT);
    if (disp.val32 >= 16)
        return setError(OVERFLOW_RANGE);
    uint8_t len = disp.val32;
    if (mode == M_LEN8) {
        if (len % 2)
            return setError(ILLEGAL_CONSTANT);
        len /= 2;
    }
    if (mode == M_LEN4) {
        if (len % 4)
            return setError(ILLEGAL_CONSTANT);
        len /= 4;
    }
    len++;
    outDec(out, len, 5);
    return OK;
}

Error DisNs32000::decodeBitField(
        DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode) {
    if (mode == M_BFLEN)
        return OK;
    const uint8_t data = insn.readByte(memory);
    const uint8_t len = (data & 0x1F) + 1;
    const uint8_t off = (data >> 5);
    outHex(out, off, 3);  // M_BFOFF
    out.comma();
    outDec(out, len, 6);  // M_BFLEN
    return setError(insn);
}

Error DisNs32000::decodeImmediate(
        DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode) {
    const OprSize size = (mode == M_GENC) ? SZ_BYTE : insn.oprSize();
    switch (size) {
    case SZ_BYTE:
        if (mode == M_GENC) {
            outDec(out, static_cast<int8_t>(insn.readByte(memory)), -8);
        } else {
            outHex(out, insn.readByte(memory), 8);
        }
        break;
    case SZ_WORD:
        outHex(out, insn.readUint16(memory), 16);
        break;
    case SZ_DOUBLE:
        outHex(out, insn.readUint32(memory), 32);
        break;
    case SZ_FLOAT:
        if (_floatPrefix)
            out.letter('0').letter('f');
        out.format_P(PSTR("%.8g"), insn.readFloat32(memory));
        break;
    case SZ_LONG:
        if (_floatPrefix)
            out.letter('0').letter('f');
        out.format_P(PSTR("%.16lg"), insn.readFloat64(memory));
        break;
    default:
        break;
    }
    return setError(insn);
}

Error DisNs32000::decodeDisplacement(
        DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode) {
    Displacement disp;
    if (readDisplacement(memory, insn, disp))
        return getError();
    if (mode == M_LEN32) {
        if (disp.val32 <= 0)
            return setError(ILLEGAL_CONSTANT);
        if (disp.val32 > 32)
            return setError(OVERFLOW_RANGE);
        outDec(out, disp.val32, 6);
    }
    if (mode == M_DISP)
        outDisplacement(out, disp);
    return OK;
}

Error DisNs32000::decodeRelative(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out) {
    Displacement disp;
    if (readDisplacement(memory, insn, disp))
        return getError();
    const Config::uintptr_t target = insn.address() + disp.val32;
    const Config::uintptr_t max = 1UL << uint8_t(addressWidth());
    if (target >= max)
        return setError(OVERFLOW_RANGE);
    outRelAddr(out, target, insn.address(), disp.bits);
    return OK;
}

Error DisNs32000::decodeConfig(const InsnNs32000 &insn, StrBuffer &out, OprPos pos) {
    const uint8_t configs = getOprField(insn, pos);
    _regs.outConfigNames(out, configs);
    return OK;
}

Error DisNs32000::decodeStrOpt(const InsnNs32000 &insn, StrBuffer &out, OprPos pos) {
    const uint8_t strOpts = getOprField(insn, pos);
    if (_stringOptionBracket)
        out.letter('[');
    _regs.outStrOptNames(out, strOpts);
    if (_stringOptionBracket)
        out.letter(']');
    return OK;
}

Error DisNs32000::decodeRegisterList(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out) {
    uint8_t list = insn.readByte(memory);
    if (setError(insn))
        return getError();
    if (list == 0)
        return setError(OPCODE_HAS_NO_EFFECT);
    const bool push = insn.oprSize() == SZ_NONE;
    const uint8_t mask = push ? 0x01 : 0x80;
    out.letter('[');
    char sep = 0;
    for (uint8_t reg = 0; list; reg++) {
        if (list & mask) {
            if (sep)
                out.letter(sep);
            _regs.outRegName(out, RegNs32000::decodeRegName(reg));
            sep = ',';
        }
        if (push)
            list >>= 1;
        else
            list <<= 1;
    }
    out.letter(']');
    return OK;
}

Error DisNs32000::decodeGeneric(
        DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode, OprPos pos) {
    uint8_t gen = getOprField(insn, pos);
    RegName index = REG_UNDEF;
    OprSize size = SZ_NONE;
    const bool scaledIndex = isScaledIndex(gen);
    if (scaledIndex) {
        size = OprSize(gen & 0x3);
        const uint8_t indexByte = insn.indexByte(pos);
        index = RegNs32000::decodeRegName(indexByte);
        gen = indexByte >> 3;
    }
    Displacement disp, disp2;
    RegName reg;
    switch (gen) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:  // M_GREG, M_FREG
        if (mode == M_GENA)
            return setError(REGISTER_NOT_ALLOWED);
        reg = RegNs32000::decodeRegName(gen, !scaledIndex && (mode == M_FENR || mode == M_FENW));
        if (scaledIndex)
            out.letter('0').letter('(');
        _regs.outRegName(out, reg);
        if (scaledIndex)
            out.letter(')');
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:  // M_RREL
        reg = RegNs32000::decodeRegName(gen);
        if (readDisplacement(memory, insn, disp))
            return getError();
        outDisplacement(out, disp);
        _regs.outRegName(out.letter('('), reg).letter(')');
        break;
    case 0x10:
        reg = REG_FP;
        goto m_mrel;
    case 0x11:
        reg = REG_SP;
        goto m_mrel;
    case 0x12:
        reg = REG_SB;
        goto m_mrel;
    m_mrel:  // M_MREL
        if (readDisplacement(memory, insn, disp))
            return getError();
        if (readDisplacement(memory, insn, disp2))
            return getError();
        if (reg != REG_EXT || disp2.val32)
            outDisplacement(out, disp2).letter('(');
        outDisplacement(out, disp);
        _regs.outRegName(out.letter('('), reg).letter(')');
        if (reg != REG_EXT || disp2.val32)
            out.letter(')');
        break;
    case 0x13:
        return setError(ILLEGAL_OPERAND_MODE);
    case 0x14:  // M_IMM
        if (mode == M_GENW || mode == M_GENA || mode == M_FENW || scaledIndex)
            return setError(OPERAND_NOT_ALLOWED);
        return decodeImmediate(memory, insn, out, mode);
    case 0x15:  // M_ABS
        if (readDisplacement(memory, insn, disp)) {
            return getError();
        } else {
            // Check absolute address is in 24bit integer range.
            const Config::uintptr_t max = 1UL << uint8_t(addressWidth());
            if (static_cast<uint32_t>(disp.val32 + (max >> 1)) > max - 1)
                return setError(OVERFLOW_RANGE);
            outAbsAddr(out.letter('@'), disp.val32);
        }
        break;
    case 0x16:  // M_EXT
        if (_externalParen) {
            reg = REG_EXT;
            goto m_mrel;
        }
        if (readDisplacement(memory, insn, disp))
            return getError();
        if (readDisplacement(memory, insn, disp2))
            return getError();
        _regs.outRegName(out, REG_EXT);
        outDisplacement(out.letter('('), disp).letter(')');
        out.letter('+');
        if (disp2.val32 < 0)
            out.letter('(');
        outDisplacement(out, disp2);
        if (disp2.val32 < 0)
            out.letter(')');
        break;
    case 0x17:  // M_TOS
        _regs.outRegName(out, REG_TOS);
        break;
    case 0x18:
        reg = REG_FP;
        goto m_mem;
    case 0x19:
        reg = REG_SP;
        goto m_mem;
    case 0x1A:
        reg = REG_SB;
        goto m_mem;
    case 0x1B:
        reg = REG_PC;
    m_mem:  // M_MEM
        if (readDisplacement(memory, insn, disp))
            return getError();
        if (reg == REG_PC) {
            const Config::uintptr_t target = insn.address() + disp.val32;
            const Config::uintptr_t max = 1UL << uint8_t(addressWidth());
            if (target >= max)
                return setError(OVERFLOW_RANGE);
            if (_pcRelativeParen) {
                outAbsAddr(out, target);
            } else {
                out.letter(_curSym);
                if (disp.val32 > 0)
                    out.letter('+');
                if (disp.val32)
                    outDisplacement(out, disp);
                break;
            }
        } else {
            outDisplacement(out, disp);
        }
        _regs.outRegName(out.letter('('), reg).letter(')');
        break;
    default:
        return setError(ILLEGAL_OPERAND_MODE);
    }
    if (index != REG_UNDEF) {
        _regs.outRegName(out.letter('['), index)
                .letter(':')
                .letter(_regs.indexSizeChar(size))
                .letter(']');
    }
    return OK;
}

Error DisNs32000::decodeOperand(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode,
        OprPos pos, OprSize size) {
    const uint8_t field = getOprField(insn, pos);
    switch (mode) {
    case M_GREG:
        _regs.outRegName(out, RegNs32000::decodeRegName(field));
        break;
    case M_PREG: {
        const PregName preg = RegNs32000::decodePregName(field);
        if (preg == PREG_UNDEF)
            return setError(UNKNOWN_REGISTER);
        _regs.outPregName(out, preg);
        break;
    }
    case M_MREG: {
        const MregName mreg = RegNs32000::decodeMregName(field);
        if (mreg == MREG_UNDEF)
            return setError(UNKNOWN_REGISTER);
        _regs.outMregName(out, mreg);
        break;
    }
    case M_CONF:
        return decodeConfig(insn, out, pos);
    case M_SOPT:
        return decodeStrOpt(insn, out, pos);
    case M_RLST:
        return decodeRegisterList(memory, insn, out);
    case M_FENW:
    case M_FENR:
        if (field < 8 && field % 2 != 0 && (size == SZ_LONG || size == SZ_QUAD))
            return setError(REGISTER_NOT_ALLOWED);
        goto decode_generic;
    case M_GENR:
    case M_GENW:
        if (field < 8 && field % 2 != 0 && size == SZ_QUAD)
            return setError(REGISTER_NOT_ALLOWED);
        goto decode_generic;
    case M_GENC:
    case M_GENA:
    decode_generic:
        return decodeGeneric(memory, insn, out, mode, pos);
    case M_INT4:
        outDec(out, static_cast<int8_t>(getOprField(insn, pos)), -4);
        break;
    case M_BFOFF:
    case M_BFLEN:
        return decodeBitField(memory, insn, out, mode);
    case M_REL:
        return decodeRelative(memory, insn, out);
    case M_DISP:
    case M_LEN32:
        return decodeDisplacement(memory, insn, out, mode);
    case M_LEN16:
    case M_LEN8:
    case M_LEN4:
        return decodeLength(memory, insn, out, mode);
    default:
        return setError(INTERNAL_ERROR);
    }
    return OK;
}

Error DisNs32000::decode(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnNs32000 insn(_insn);
    Config::opcode_t opCode = insn.readByte(memory);
    insn.setOpCode(opCode);
    if (TableNs32000::TABLE.isPrefixCode(opCode)) {
        const Config::opcode_t prefix = opCode;
        opCode = insn.readByte(memory);
        insn.setOpCode(opCode, prefix);
    }
    if (setError(insn))
        return getError();

    if (TableNs32000::TABLE.searchOpCode(insn, memory))
        return setError(TableNs32000::TABLE.getError());
    if (readIndexByte(memory, insn, insn.srcMode(), insn.srcPos()))
        return getError();
    if (readIndexByte(memory, insn, insn.dstMode(), insn.dstPos()))
        return getError();
    if (readIndexByte(memory, insn, insn.ex1Mode(), insn.ex1Pos()))
        return getError();

    const AddrMode src = insn.srcMode();
    const AddrMode dst = insn.dstMode();
    const AddrMode ex1 = insn.ex1Mode();
    const AddrMode ex2 = insn.ex2Mode();
    const OprSize size = insn.oprSize();
    if (src == M_NONE)
        return setOK();
    const OprSize srcSize = (ex1 == M_NONE && insn.ex1Pos() != P_NONE) ? SZ_QUAD : size;
    if (decodeOperand(memory, insn, out, src, insn.srcPos(), srcSize))
        return getError();
    if (dst == M_NONE)
        return setOK();
    out.comma();
    const OprSize dstSize = (ex2 == M_NONE && insn.ex2Pos() != P_NONE) ? SZ_QUAD : size;
    if (decodeOperand(memory, insn, out, dst, insn.dstPos(), dstSize))
        return getError();
    if (ex1 == M_NONE)
        return setOK();
    out.comma();
    if (decodeOperand(memory, insn, out, ex1, insn.ex1Pos(), size))
        return getError();
    if (ex2 == M_NONE || ex2 == M_BFLEN)
        return setOK();
    out.comma();
    if (decodeOperand(memory, insn, out, ex2, insn.ex2Pos(), size))
        return getError();
    return setOK();
}

}  // namespace ns32000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
