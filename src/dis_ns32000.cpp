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
#include "reg_ns32000.h"
#include "table_ns32000.h"
#include "text_common.h"

namespace libasm {
namespace ns32000 {

using namespace reg;
using namespace text::common;
using namespace text::option;

namespace {

const char OPT_BOOL_EXTERNAL_PAREN[] PROGMEM = "external-paren";
const char OPT_DESC_EXTERNAL_PAREN[] PROGMEM = "disp2(disp(ext)) as external addressing";

}  // namespace

const ValueFormatter::Plugins &DisNs32000::defaultPlugins() {
    static const struct final : ValueFormatter::Plugins {
        const HexFormatter &hex() const override { return NationalHexFormatter::singleton(); }
        char locationSymbol() const override { return '.'; }
        const /*PROGMEM*/ char *lineComment_P() const override { return PSTR_SHARP; }
    } PLUGINS{};
    return PLUGINS;
}

DisNs32000::DisNs32000(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_gnuAs),
      Config(TABLE),
      _opt_gnuAs(
              this, &DisNs32000::setGnuAs, OPT_BOOL_GNU_AS, OPT_DESC_GNU_AS, &_opt_externalParen),
      _opt_externalParen(this, &DisNs32000::setExternalParen, OPT_BOOL_EXTERNAL_PAREN,
              OPT_DESC_EXTERNAL_PAREN) {
    reset();
}

void DisNs32000::reset() {
    Disassembler::reset();
    setGnuAs(false);
    setExternalParen(false);
}

Error DisNs32000::setGnuAs(bool enable) {
    _gnuAs = enable;
    setCStyle(enable);
    setCurSym(enable ? '.' : 0);
    return OK;
}

Error DisNs32000::setExternalParen(bool enable) {
    _externalParen = enable;
    return OK;
}

namespace {

bool isGenMode(AddrMode mode) {
    return mode == M_GENR || mode == M_GENW || mode == M_GENC || mode == M_GENA || mode == M_FENR ||
           mode == M_FENW;
}

uint8_t getOprField(const DisInsn &insn, OprPos pos) {
    if (pos == P_GEN1)
        return (insn.hasPostfix() ? insn.postfix() : insn.opCode()) >> 3;
    if (pos == P_GEN2 && insn.hasPostfix()) {
        const uint8_t post = (insn.postfix() << 2) & 0x1f;
        return post | (insn.opCode() >> 6);
    }
    if (pos == P_REG)
        return (insn.opCode() >> 3) & 0x7;
    if (pos == P_SHORT && insn.hasPostfix()) {
        uint8_t val8 = (insn.postfix() & 7) << 1;
        if (insn.opCode() & 0x80)
            val8 |= 1;
        // Sign extends 4-bit number.
        // val8 = (val8 & 7) - (val8 & 8);
        val8 = ConfigBase::signExtend(val8, 4);
        return val8;
    }
    return 0;
}

bool isScaledIndex(uint8_t gen) {
    return gen >= 0x1c;
}

}  // namespace

Error DisNs32000::readIndexByte(DisInsn &insn, AddrMode mode, OprPos pos) const {
    if (isGenMode(mode) && isScaledIndex(getOprField(insn, pos)))
        insn.setIndexByte(insn.readByte(), pos);
    return insn.getError();
}

StrBuffer &DisNs32000::outDisplacement(StrBuffer &out, const Displacement &disp) const {
    if (disp.bits == 7)
        return outDec(out, static_cast<int8_t>(disp.val32), -8);
    if (disp.bits == 14)
        return outHex(out, static_cast<int16_t>(disp.val32), -16);
    // disp.bits == 30
    return outHex(out, disp.val32, -30);
}

Error DisNs32000::readDisplacement(DisInsn &insn, Displacement &disp) const {
    const auto head = insn.readByte();
    if ((head & 0x80) == 0) {
        // 0xxx|xxxx
        // Sign extends 7-bit number as 0x40 is a sign bit.
        disp.val32 = signExtend(head, 7);
        disp.bits = 7;
    } else {
        // Sign extends 14-bit number as 0x2000 is a sign bit.
        const auto lsb = insn.readByte();
        const auto val16 = (static_cast<uint16_t>(head) << 8) | lsb;
        if ((head & 0x40) == 0) {
            // 10xx|xxxx
            disp.val32 = signExtend(val16, 14);
            disp.bits = 14;
        } else {
            // 11xx|xxxx
            const auto lsw = insn.readUint16Be();
            if (head == 0xE0)
                disp.setErrorIf(ILLEGAL_CONSTANT);  // 1110|0000 is reserved
            const auto val32 = (static_cast<uint32_t>(val16) << 16) | lsw;
            disp.val32 = signExtend(val32, 30);
            disp.bits = 30;
            if (disp.val32 < -0x1F000000L)
                disp.setErrorIf(OVERFLOW_RANGE);
        }
    }
    return disp.getError();
}

void DisNs32000::decodeLength(DisInsn &insn, StrBuffer &out) const {
    Displacement disp;
    if (readDisplacement(insn, disp))
        insn.setErrorIf(out, disp);
    if (disp.val32 < 0) {
        insn.setErrorIf(out, ILLEGAL_CONSTANT);
    } else {
        uint8_t len = disp.val32;
        if (len >= 16)
            insn.setErrorIf(out, OVERFLOW_RANGE);
        if (insn.size() == SZ_QUAD) {
            if (len % 4) {
                insn.setErrorIf(out, ILLEGAL_CONSTANT);
            } else {
                len /= 4;
                len++;
            }
        } else if (insn.size() == SZ_WORD) {
            if (len % 2) {
                insn.setErrorIf(out, ILLEGAL_CONSTANT);
            } else {
                len /= 2;
                len++;
            }
        } else if (insn.size() == SZ_BYTE) {
            len++;
        }
        outDec(out, len, 5);
        return;
    }
    outDisplacement(out, disp);
}

void DisNs32000::decodeBitField(DisInsn &insn, StrBuffer &out) const {
    const auto data = insn.readByte();
    const auto len = (data & 0x1F) + 1;
    const auto off = (data >> 5);
    if (off + len > 32)
        insn.setError(out, OVERFLOW_RANGE);
    outHex(out, off, 3);  // M_BFOFF
    out.comma();
    outDec(out, len, 6);  // M_BFLEN
}

void DisNs32000::decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto size = insn.size();
    if (mode == M_GENC) {
        outDec(out, static_cast<int8_t>(insn.readByte()), -8);
    } else if (mode != M_FENR && mode != M_FENW) {
        if (size == SZ_BYTE) {
            outHex(out, insn.readByte(), 8);
        } else if (size == SZ_WORD) {
            outHex(out, insn.readUint16Be(), 16);
        } else if (size == SZ_QUAD) {
            outHex(out, insn.readUint32Be(), 32);
        }
#if !defined(LIBASM_NS32000_NOFPU)
    } else {
        if (size == SZ_QUAD) {
#if defined(LIBASM_DIS_NOFLOAT)
            insn.setErrorIf(out, FLOAT_NOT_SUPPORTED);
            outHex(out, insn.readUint32(), 32, false);
#else
            const auto float32 = insn.readFloat32Be();
            if (_gnuAs)
                out.letter('0').letter('f');
            out.float32(float32);
#endif
        } else if (size == SZ_OCTA) {
#if defined(LIBASM_DIS_NOFLOAT)
            insn.setErrorIf(out, FLOAT_NOT_SUPPORTED);
            const auto lsw = insn.readUint32();
            outHex(out, insn.readUint32(), 32, false);
            out.hex(lsw, 8);
#else
            const auto float64 = insn.readFloat64Be();
            if (_gnuAs)
                out.letter('0').letter('l');
            out.float64(float64);
#endif
        }
#endif
    }
}

void DisNs32000::decodeDisplacement(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    Displacement disp;
    if (readDisplacement(insn, disp))
        insn.setErrorIf(out, disp);
    if (mode == M_LEN32) {
        if (disp.val32 <= 0)
            insn.setErrorIf(out, ILLEGAL_CONSTANT);
        if (disp.val32 > 32)
            insn.setErrorIf(out, OVERFLOW_RANGE);
        outDec(out, disp.val32, 6);
    } else if (mode == M_DISP) {
        outDisplacement(out, disp);
    }
}

void DisNs32000::decodeRelative(DisInsn &insn, StrBuffer &out) const {
    Displacement disp;
    if (readDisplacement(insn, disp))
        insn.setErrorIf(out, disp);
    const auto target = insn.address() + disp.val32;
    insn.setErrorIf(out, checkAddr(target));
    outRelAddr(out, target, insn.address(), disp.bits);
}

void DisNs32000::decodeConfig(DisInsn &insn, StrBuffer &out, OprPos pos) const {
    const auto configs = getOprField(insn, pos);
    outConfigNames(out, configs);
}

void DisNs32000::decodeStrOpt(DisInsn &insn, StrBuffer &out, OprPos pos) const {
    auto strOpts = getOprField(insn, pos);
    if (_gnuAs)
        out.letter('[');
    char sep = 0;
    if (strOpts & uint8_t(STROPT_B)) {
        out.letter('B');
        sep = ',';
    }
    strOpts &= 0xC;
    if (strOpts) {
        if (sep)
            out.letter(sep);
        if (strOpts == STROPT_W) {
            out.letter('W');
        } else if (strOpts == STROPT_U) {
            out.letter('U');
        } else {
            insn.setErrorIf(ILLEGAL_OPERAND_MODE);
        }
    }
    if (_gnuAs)
        out.letter(']');
}

void DisNs32000::decodeRegisterList(DisInsn &insn, StrBuffer &out) const {
    auto list = insn.readByte();
    if (list == 0)
        insn.setErrorIf(out, OPCODE_HAS_NO_EFFECT);
    const auto push = insn.size() == SZ_NONE;
    const auto mask = push ? 0x01 : 0x80;
    out.letter('[');
    char sep = 0;
    for (uint8_t reg = 0; list; reg++) {
        if (list & mask) {
            if (sep)
                out.letter(sep);
            outRegName(out, decodeRegName(reg));
            sep = ',';
        }
        if (push)
            list >>= 1;
        else
            list <<= 1;
    }
    out.letter(']');
}

void DisNs32000::decodeGeneric(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos,
        OprSize size, Error idxError) const {
    const auto base = getOprField(insn, pos);
    const auto scaledIndex = isScaledIndex(base);
    const auto gen = scaledIndex ? (insn.indexByte(pos) >> 3) : base;
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
            insn.setErrorIf(out, REGISTER_NOT_ALLOWED);
        reg = decodeRegName(gen, !scaledIndex && (mode == M_FENR || mode == M_FENW));
        if (size == SZ_OCTA && !isRegPair(reg))
            insn.setErrorIf(out, REGISTER_NOT_ALIGNED);
        if (scaledIndex)
            out.letter('0').letter('(');
        outRegName(out, reg);
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
        reg = decodeRegName(gen);
        if (readDisplacement(insn, disp))
            insn.setErrorIf(out, disp);
        outDisplacement(out, disp);
        outRegName(out.letter('('), reg).letter(')');
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
    {
        const auto dispError = readDisplacement(insn, disp);
        if (readDisplacement(insn, disp2))
            insn.setErrorIf(out, disp);
        if (reg != REG_EXT || disp2.val32)
            outDisplacement(out, disp2).letter('(');
        if (dispError)
            insn.setErrorIf(out, dispError);
        outDisplacement(out, disp);
        outRegName(out.letter('('), reg).letter(')');
        if (reg != REG_EXT || disp2.val32)
            out.letter(')');
    } break;
    case 0x14:  // M_IMM
        if (mode == M_GENW || mode == M_GENA || mode == M_FENW || scaledIndex)
            insn.setErrorIf(out, OPERAND_NOT_ALLOWED);
        decodeImmediate(insn, out, mode);
        break;
    case 0x15:  // M_ABS
        if (readDisplacement(insn, disp))
            insn.setErrorIf(out, disp);
        // Check address is in 24bit signed integer range.
        if (disp.val32 < INT32_C(-0x80'0000) || disp.val32 > INT32_C(0x00FF'FFFF))
            insn.setErrorIf(out, OVERFLOW_RANGE);
        outAbsAddr(out.letter('@'), disp.val32, insn.getError() == OVERFLOW_RANGE ? -32 : 0);
        break;
    case 0x16:  // M_EXT
        if (_externalParen && !_gnuAs) {
            reg = REG_EXT;
            goto m_mrel;
        }
        {
            outRegName(out, REG_EXT);
            if (readDisplacement(insn, disp))
                insn.setErrorIf(out, disp);
            outDisplacement(out.letter('('), disp).letter(')');
            out.letter('+');
            const auto disp2Error = readDisplacement(insn, disp2);
            if (disp2.val32 < 0)
                out.letter('(');
            if (disp2Error)
                insn.setErrorIf(out, disp2Error);
            outDisplacement(out, disp2);
            if (disp2.val32 < 0)
                out.letter(')');
        }
        break;
    case 0x17:  // M_TOS
        outRegName(out, REG_TOS);
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
        if (readDisplacement(insn, disp))
            insn.setErrorIf(out, disp);
        if (reg == REG_PC) {
            const auto target = insn.address() + disp.val32;
            insn.setErrorIf(out, checkAddr(target));
            if (_gnuAs) {
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
        outRegName(out.letter('('), reg).letter(')');
        break;
    default:  // 0x13
        insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
        break;
    }
    if (scaledIndex) {
        const auto indexByte = insn.indexByte(pos);
        const auto indexSize = decodeIndexSize(base);
        const auto index = decodeRegName(indexByte);
        if (idxError)
            insn.setErrorIf(out, idxError);
        outRegName(out.letter('['), index).letter(':').letter(indexSizeChar(indexSize)).letter(']');
    }
}

void DisNs32000::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos,
        OprSize size, Error idxError) const {
    const auto field = getOprField(insn, pos);
    switch (mode) {
    case M_GREG:
        outRegName(out, decodeRegName(field));
        break;
    case M_PREG:
        if (decodePregName(field) == PREG_UNDEF)
            insn.setErrorIf(out, UNKNOWN_REGISTER);
        outPregName(out, decodePregName(field));
        break;
    case M_MREG:
        if (decodeMregName(field) == MREG_UNDEF)
            insn.setErrorIf(out, UNKNOWN_REGISTER);
        outMregName(out, decodeMregName(field));
        break;
    case M_CONF:
        decodeConfig(insn, out, pos);
        break;
    case M_SOPT:
        decodeStrOpt(insn, out, pos);
        break;
    case M_RLST:
        decodeRegisterList(insn, out);
        break;
    case M_FENW:
    case M_FENR:
    case M_GENR:
    case M_GENW:
    case M_GENC:
    case M_GENA:
        decodeGeneric(insn, out, mode, pos, size, idxError);
        break;
    case M_INT4:
        outDec(out, static_cast<int8_t>(getOprField(insn, pos)), -4);
        break;
    case M_BFOFF:
    case M_BFLEN:
        decodeBitField(insn, out);
        break;
    case M_REL:
        decodeRelative(insn, out);
        break;
    case M_DISP:
    case M_LEN32:
        decodeDisplacement(insn, out, mode);
        break;
    case M_LEN16:
        decodeLength(insn, out);
        break;
    default:
        break;
    }
}

Error DisNs32000::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readByte();
    insn.setOpCode(opc);
    if (isPrefixCode(_cpuSpec, opc)) {
        insn.setPrefix(opc);
        insn.setOpCode(insn.readByte());
        if (insn.getError())
            return _insn.setError(insn);
    }
    if (searchOpCode(_cpuSpec, insn, out))
        return _insn.setError(insn);

    const auto srcIdxError = readIndexByte(insn, insn.src(), insn.srcPos());
    const auto dstIdxError = readIndexByte(insn, insn.dst(), insn.dstPos());
    const auto ex1IdxError = readIndexByte(insn, insn.ex1(), insn.ex1Pos());

    decodeOperand(insn, out, insn.src(), insn.srcPos(), insn.srcSize(), srcIdxError);
    const auto dst = insn.dst();
    if (dst == M_ZERO) {
        const auto field = getOprField(insn, insn.dstPos());
        if (field != 0) {
            out.reset();
            insn.nameBuffer().reset();
            insn.setError(UNKNOWN_INSTRUCTION);
        }
    } else if (dst != M_NONE) {
        decodeOperand(insn, out.comma(), dst, insn.dstPos(), insn.dstSize(), dstIdxError);
        const auto ex1 = insn.ex1();
        if (ex1 != M_NONE) {
            decodeOperand(insn, out.comma(), ex1, insn.ex1Pos(), insn.size(), ex1IdxError);
            const auto ex2 = insn.ex2();
            if (ex2 != M_NONE && ex2 != M_BFLEN)
                decodeOperand(insn, out.comma(), ex2, insn.ex2Pos(), insn.size());
        }
    }
    return _insn.setError(insn);
}

}  // namespace ns32000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
