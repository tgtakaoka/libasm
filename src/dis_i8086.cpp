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

#include "dis_i8086.h"

#include "reg_i8086.h"
#include "table_i8086.h"

namespace libasm {
namespace i8086 {

using namespace reg;

static const char OPT_BOOL_SEGMENT_INSN[] PROGMEM = "segment-insn";
static const char OPT_DESC_SEGMENT_INSN[] PROGMEM = "segment override as instruction";
static const char OPT_BOOL_STRING_INSN[] PROGMEM = "string-insn";
static const char OPT_DESC_STRING_INSN[] PROGMEM = "string instruction as repeat operand";

DisI8086::DisI8086() : Disassembler(_formatter, TableI8086::TABLE, '$'), _formatter() {
    reset();
}

void DisI8086::reset() {
    Disassembler::reset();
    _segOverrideInsn = true;
    _repeatHasStringInst = false;
}

DisI8086::OptStringInsn::OptStringInsn(bool &var)
    : BoolOption(OPT_BOOL_STRING_INSN, OPT_DESC_STRING_INSN, var) {}

DisI8086::OptSegmentInsn::OptSegmentInsn(bool &var, const OptionBase &next)
    : BoolOption(OPT_BOOL_SEGMENT_INSN, OPT_DESC_SEGMENT_INSN, var, next) {}

StrBuffer &DisI8086::outRegister(StrBuffer &out, RegName name, const char prefix) {
    if (name == REG_UNDEF)
        return out;
    if (prefix)
        out.letter(prefix);
    return outRegName(out, name);
}

RegName DisI8086::decodeRegister(const InsnI8086 &insn, AddrMode mode, OprPos pos) {
    uint8_t num = 0;
    switch (pos) {
    case P_OREG:
        num = insn.opCode();
        break;
    case P_OSEG:
        num = insn.opCode() >> 3;
        break;
    case P_MOD:
    case P_OMOD:
        num = insn.modReg();
        break;

    case P_REG:
        num = insn.modReg() >> 3;
        break;
    default:
        return REG_UNDEF;
    }
    switch (mode) {
    case M_BREG:
        return decodeByteReg(num);
    case M_WREG:
        return decodeWordReg(num);
    case M_SREG:
        return decodeSegReg(num);
    default:
        return REG_UNDEF;
    }
}

Error DisI8086::decodeRelative(DisMemory &memory, InsnI8086 &insn, StrBuffer &out, AddrMode mode) {
    int16_t delta;
    if (mode == M_REL8) {
        delta = static_cast<int8_t>(insn.readByte(memory));
    } else {
        delta = static_cast<int16_t>(insn.readUint16(memory));
    }
    const auto base = insn.address() + insn.length();
    const auto target = branchTarget(base, delta);
    outRelAddr(out, target, insn.address(), mode == M_REL8 ? 8 : 16);
    return setError(insn);
}

Error DisI8086::decodeImmediate(DisMemory &memory, InsnI8086 &insn, StrBuffer &out, AddrMode mode) {
    if (mode == M_IMM && insn.size() == SZ_WORD) {
        outHex(out, insn.readUint16(memory), 16);
    } else if ((mode == M_IMM && insn.size() == SZ_BYTE) || mode == M_IOA) {
        outHex(out, insn.readByte(memory), 8);
    } else if (mode == M_IMM8) {
        outHex(out, insn.readByte(memory), -8);
    } else if (mode == M_UI16) {
        outDec(out, insn.readUint16(memory), 16);
    } else if (mode == M_UI8) {
        outDec(out, insn.readByte(memory), 8);
    } else if (mode == M_BIT) {
        const auto bit = insn.readByte(memory);
        if (bit >= 16 || (insn.size() == SZ_BYTE && bit >= 8))
            return setError(OVERFLOW_RANGE);
        outDec(out, bit, 4);
    } else {
        // M_FAR
        const uint16_t offset = insn.readUint16(memory);
        const uint16_t segment = insn.readUint16(memory);
        outAbsAddr(out, segment, 16).letter(':');
        outAbsAddr(out, offset, 16);
    }
    return setError(insn);
}

static RegName getBaseReg(uint8_t mod, uint8_t r_m) {
    if (r_m <= 1 || r_m == 7)
        return REG_BX;
    if (r_m <= 3)
        return REG_BP;
    if (r_m == 6 && mod != 0)
        return REG_BP;
    return REG_UNDEF;
}

static RegName getIndexReg(uint8_t r_m) {
    if (r_m >= 6)
        return REG_UNDEF;
    return r_m % 2 == 0 ? REG_SI : REG_DI;
}

static OprSize operandSize(const InsnI8086 &insn, AddrMode mode) {
    switch (mode) {
    case M_AL:
    case M_BREG:
        return SZ_BYTE;
    case M_AX:
    case M_WREG:
    case M_SREG:
        return SZ_WORD;
    case M_BMOD:
    case M_WMOD:
        return (insn.modReg() >> 6) == 3 ? insn.size() : SZ_NONE;
    default:
        return SZ_NONE;
    }
}

static OprSize operandSize(const InsnI8086 &insn) {
    if (insn.stringInst())
        return insn.size();
    OprSize size;
    if ((size = operandSize(insn, insn.dst())) != SZ_NONE)
        return size;
    if ((size = operandSize(insn, insn.src())) != SZ_NONE)
        return size;
    return SZ_NONE;
}

static OprSize pointerSize(const InsnI8086 &insn, AddrMode mode) {
    const OprSize size = insn.size();
    if (size == SZ_NONE)
        return size;
    switch (mode) {
    case M_BMOD:
    case M_BMEM:
        return SZ_BYTE;
    case M_WMOD:
    case M_WMEM:
        return SZ_WORD;
    default:
        return size;
    }
}

static RegName pointerReg(const InsnI8086 &insn) {
    OprSize size;
    if ((size = pointerSize(insn, insn.dst())) == SZ_NONE &&
            (size = pointerSize(insn, insn.src())) == SZ_NONE)
        return REG_UNDEF;
    return size == SZ_BYTE ? REG_BYTE : REG_WORD;
}

Error DisI8086::outMemReg(
        DisMemory &memory, InsnI8086 &insn, StrBuffer &out, RegName seg, uint8_t mod, uint8_t r_m) {
    if (operandSize(insn) == SZ_NONE) {
        const RegName ptr = pointerReg(insn);
        if (ptr != REG_UNDEF) {
            outRegister(out, ptr);
            outRegister(out, REG_PTR, ' ').letter(' ');
        }
    }
    if (seg != REG_UNDEF)
        outRegister(out, seg).letter(':');
    const auto base = getBaseReg(mod, r_m);
    const auto index = getIndexReg(r_m);
    out.letter('[');
    char sep = 0;
    outRegister(out, base, sep);
    if (base != REG_UNDEF)
        sep = '+';
    outRegister(out, index, sep);
    if (index != REG_UNDEF)
        sep = '+';
    if (mod == 1) {
        const int8_t disp8 = static_cast<int8_t>(insn.readByte(memory));
        if (disp8 >= 0)
            out.letter(sep);
        outDec(out, disp8, -8);
    }
    if (mod == 2 || (mod == 0 && r_m == 6)) {
        if (sep)
            out.letter(sep);
        outAbsAddr(out, insn.readUint16(memory), 16);
    }
    out.letter(']');
    return setError(insn);
}

Error DisI8086::decodeMemReg(
        DisMemory &memory, InsnI8086 &insn, StrBuffer &out, AddrMode mode, OprPos pos) {
    const uint8_t mod = insn.modReg() >> 6;
    if (mod == 3) {
        if (mode == M_BMEM || mode == M_WMEM)
            return setError(ILLEGAL_OPERAND);
        const auto regMode = (mode == M_BMOD ? M_BREG : M_WREG);
        outRegister(out, decodeRegister(insn, regMode, pos));
        return OK;
    }
    const uint8_t r_m = insn.modReg() & 07;
    return outMemReg(memory, insn, out, TableI8086::TABLE.overrideSeg(insn.segment()), mod, r_m);
}

Error DisI8086::decodeRepeatStr(DisMemory &memory, InsnI8086 &rep, StrBuffer &out) {
    if (_repeatHasStringInst) {
        Insn _istr(0);
        InsnI8086 istr(_istr);
        const auto opc = rep.readByte(memory);
        istr.setOpCode(opc, 0);
        if (TableI8086::TABLE.searchOpCode(istr, out))
            return setError(istr);
        if (!istr.stringInst())
            return setError(UNKNOWN_INSTRUCTION);
        out.text(istr.name());
    }
    return OK;
}

Error DisI8086::decodeOperand(
        DisMemory &memory, InsnI8086 &insn, StrBuffer &out, AddrMode mode, OprPos pos) {
    RegName name;
    switch (mode) {
    case M_NONE:
        return OK;
    case M_AL:
        outRegister(out, REG_AL);
        break;
    case M_CL:
        outRegister(out, REG_CL);
        break;
    case M_AX:
        outRegister(out, REG_AX);
        break;
    case M_DX:
        outRegister(out, REG_DX);
        break;
    case M_CS:
    case M_BREG:
    case M_WREG:
    case M_SREG:
        name = decodeRegister(insn, mode, pos);
        if (mode == M_CS && pos == P_NONE)  // POP CS
            return setError(REGISTER_NOT_ALLOWED);
        outRegister(out, name);
        break;
    case M_BMOD:
    case M_WMOD:
    case M_BMEM:
    case M_WMEM:
        return decodeMemReg(memory, insn, out, mode, pos);
    case M_VAL1:
        outHex(out, 1, 3);
        break;
    case M_VAL3:
        outHex(out, 3, 3);
        break;
    case M_IMM:
    case M_UI16:
    case M_UI8:
    case M_IMM8:
    case M_FAR:
    case M_IOA:
    case M_BIT:
        return decodeImmediate(memory, insn, out, mode);
    case M_BDIR:
    case M_WDIR:
        return outMemReg(memory, insn, out, TableI8086::TABLE.overrideSeg(insn.segment()), 0, 6);
    case M_REL:
    case M_REL8:
        return decodeRelative(memory, insn, out, mode);
    case M_ISTR:
        return decodeRepeatStr(memory, insn, out);
    default:
        return setError(INTERNAL_ERROR);
    }
    return OK;
}

static bool validSegOverride(AddrMode mode, uint8_t mod) {
    switch (mode) {
    case M_BDIR:
    case M_WDIR:
        return true;
    case M_BMOD:
    case M_WMOD:
    case M_BMEM:
    case M_WMEM:
        return mod != 3;
    default:
        return false;
    }
}

static bool validSegOverride(const InsnI8086 &insn) {
    if (insn.segment() == 0)
        return true;
    if (insn.stringInst())
        return true;
    const uint8_t mod = insn.modReg() >> 6;
    return validSegOverride(insn.dst(), mod) || validSegOverride(insn.src(), mod);
}

Error DisI8086::decodeStringInst(DisMemory &memory, InsnI8086 &insn, StrBuffer &out) {
    if (insn.segment()) {
        const auto seg = TableI8086::TABLE.overrideSeg(insn.segment());
        switch (insn.opCode() & ~1) {
        case 0xA4:  // MOVS ES:[DI],DS:[SI]
        case 0x20:  // ADD4S ES:[DI],DS:[SI]
        case 0x22:  // SUB4S ES:[DI],DS:[SI]
        case 0x26:  // CMP4S ES:[DI],DS:[SI]
            outMemReg(memory, insn, out, REG_ES, 0, 5);
            out.comma();
            /* Fall-through */
        case 0xAC:  // LODS DS:[SI]
        case 0x6E:  // OUTS DS:[SI]
            outMemReg(memory, insn, out, seg, 0, 4);
            break;
        case 0xA6:  // CMPS DS:[SI],ES:[DI]
            outMemReg(memory, insn, out, seg, 0, 4);
            out.comma();
            outMemReg(memory, insn, out, REG_ES, 0, 5);
            break;
        case 0xAA:  // STOS ES:[DI]
        case 0xAE:  // SCAS ES:[DI]
        case 0x6C:  // INS ES:[DI]
            return setError(ILLEGAL_SEGMENT);
        }
    }
    return setOK();
}

Error DisI8086::readCodes(DisMemory &memory, InsnI8086 &insn) {
    auto opCode = insn.readByte(memory);
    while (!_segOverrideInsn && TableI8086::TABLE.isSegmentPrefix(opCode)) {
        if (insn.segment())
            return setError(ILLEGAL_SEGMENT);
        insn.setSegment(opCode);
        opCode = insn.readByte(memory);
    }
    auto prefix = 0;
    if (TableI8086::TABLE.isPrefix(opCode)) {
        prefix = opCode;
        opCode = insn.readByte(memory);
    }
    insn.setOpCode(opCode, prefix);
    return setError(insn);
}

static bool imulHasSameDstSrc(const InsnI8086 &insn) {
    const auto modreg = insn.modReg();
    const auto reg = (modreg >> 3) & 7;
    return (insn.opCode() & ~2) == 0x69 &&  // IMUL
           (modreg >> 6) == 3 &&            // regsiter
           (modreg & 7) == reg;             // dst==src
}

Error DisI8086::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnI8086 insn(_insn);
    if (readCodes(memory, insn))
        return getError();
    if (TableI8086::TABLE.searchOpCode(insn, out))
        return setError(insn);

    insn.readModReg(memory);
    if (setError(insn))
        return getError();
    if (!validSegOverride(insn))
        return setError(ILLEGAL_SEGMENT);

    if (insn.stringInst())
        return decodeStringInst(memory, insn, out);
    if (decodeOperand(memory, insn, out, insn.dst(), insn.dstPos()))
        return getError();

    if (insn.src() == M_NONE)
        return setOK();
    if (!imulHasSameDstSrc(insn)) {
        out.comma();
        if (decodeOperand(memory, insn, out, insn.src(), insn.srcPos()))
            return getError();
    }

    if (insn.ext() == M_NONE)
        return setOK();
    out.comma();
    if (decodeOperand(memory, insn, out, insn.ext(), insn.extPos()))
        return getError();
    return setOK();
}

}  // namespace i8086
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
