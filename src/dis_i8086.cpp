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

namespace {

const char OPT_BOOL_SEGMENT_INSN[] PROGMEM = "segment-insn";
const char OPT_DESC_SEGMENT_INSN[] PROGMEM = "segment override as instruction";
const char OPT_BOOL_STRING_INSN[] PROGMEM = "string-insn";
const char OPT_DESC_STRING_INSN[] PROGMEM = "string instruction as repeat operand";

}  // namespace

const ValueFormatter::Plugins &DisI8086::defaultPlugins() {
    return ValueFormatter::Plugins::intel();
}

DisI8086::DisI8086(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_segmentInsn),
      Config(TABLE),
      _opt_segmentInsn(this, &DisI8086::setSegmentInsn, OPT_BOOL_SEGMENT_INSN,
              OPT_DESC_SEGMENT_INSN, &_opt_stringInsn),
      _opt_stringInsn(this, &DisI8086::setStringInsn, OPT_BOOL_STRING_INSN, OPT_DESC_STRING_INSN) {
    reset();
}

void DisI8086::reset() {
    Disassembler::reset();
    setSegmentInsn(true);
    setStringInsn(false);
}

Error DisI8086::setSegmentInsn(bool enable) {
    _segOverrideInsn = enable;
    return OK;
}

Error DisI8086::setStringInsn(bool enable) {
    _repeatHasStringInst = enable;
    return OK;
}

StrBuffer &DisI8086::outRegister(StrBuffer &out, RegName name, const char prefix) const {
    if (name != REG_UNDEF) {
        if (prefix)
            out.letter(prefix);
        outRegName(out, name);
    }
    return out;
}

RegName DisI8086::decodeRegister(const DisInsn &insn, AddrMode mode, OprPos pos) const {
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

void DisI8086::decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto delta = (mode == M_REL8) ? static_cast<int8_t>(insn.readByte())
                                        : static_cast<int16_t>(insn.readUint16());
    const auto base = insn.address() + insn.length();
    const auto target = base + delta;
    insn.setErrorIf(out, checkAddr(target, insn.address(), 16));
    outRelAddr(out, target, insn.address(), mode == M_REL8 ? 8 : 16);
}

void DisI8086::decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    if (mode == M_IMM && insn.size() == SZ_WORD) {
        outHex(out, insn.readUint16(), 16);
    } else if ((mode == M_IMM && insn.size() == SZ_BYTE) || mode == M_IOA) {
        outHex(out, insn.readByte(), 8);
    } else if (mode == M_IMM8) {
        outHex(out, insn.readByte(), -8);
    } else if (mode == M_UI16) {
        outDec(out, insn.readUint16(), 16);
    } else if (mode == M_UI8) {
        outDec(out, insn.readByte(), 8);
    } else if (mode == M_BIT) {
        const auto bit = insn.readByte();
        if (bit >= 16 || (insn.size() == SZ_BYTE && bit >= 8))
            insn.setErrorIf(OVERFLOW_RANGE);
        outDec(out, bit, 4);
    } else {
        // M_FAR
        const uint16_t offset = insn.readUint16();
        const auto offsetError = insn.getError();
        const uint16_t segment = insn.readUint16();
        outAbsAddr(out, segment, 16).letter(':');
        if (offsetError)
            insn.setErrorIf(out, offsetError);
        outAbsAddr(out, offset, 16);
    }
}

namespace {

RegName getBaseReg(uint8_t mod, uint8_t r_m) {
    if (r_m <= 1 || r_m == 7)
        return REG_BX;
    if (r_m <= 3)
        return REG_BP;
    if (r_m == 6 && mod != 0)
        return REG_BP;
    return REG_UNDEF;
}

RegName getIndexReg(uint8_t r_m) {
    if (r_m >= 6)
        return REG_UNDEF;
    return r_m % 2 == 0 ? REG_SI : REG_DI;
}

OprSize operandSize(const DisInsn &insn, AddrMode mode) {
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

OprSize operandSize(const DisInsn &insn) {
    if (insn.stringInst())
        return insn.size();
    OprSize size;
    if ((size = operandSize(insn, insn.dst())) != SZ_NONE)
        return size;
    if ((size = operandSize(insn, insn.src())) != SZ_NONE)
        return size;
    return SZ_NONE;
}

OprSize pointerSize(const DisInsn &insn, AddrMode mode) {
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

RegName pointerReg(const DisInsn &insn) {
    OprSize size;
    if ((size = pointerSize(insn, insn.dst())) == SZ_NONE &&
            (size = pointerSize(insn, insn.src())) == SZ_NONE)
        return REG_UNDEF;
    return size == SZ_BYTE ? REG_BYTE : REG_WORD;
}

}  // namespace

void DisI8086::outMemReg(
        DisInsn &insn, StrBuffer &out, RegName seg, uint8_t mod, uint8_t r_m) const {
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
        const int8_t disp8 = static_cast<int8_t>(insn.readByte());
        if (disp8 >= 0)
            out.letter(sep);
        outDec(out, disp8, -8);
    } else if (mod == 2 || (mod == 0 && r_m == 6)) {
        if (sep)
            out.letter(sep);
        outAbsAddr(out, insn.readUint16(), 16);
    }
    out.letter(']');
}

void DisI8086::decodeMemReg(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos) const {
    const uint8_t mod = insn.modReg() >> 6;
    if (mod == 3) {
        if (mode == M_BMEM || mode == M_WMEM)
            insn.setErrorIf(ILLEGAL_OPERAND);
        const auto regMode = (mode == M_BMOD ? M_BREG : M_WREG);
        outRegister(out, decodeRegister(insn, regMode, pos));
    } else {
        const uint8_t r_m = insn.modReg() & 07;
        outMemReg(insn, out, TABLE.overrideSeg(insn.segment()), mod, r_m);
    }
}

void DisI8086::decodeRepeatStr(DisInsn &insn, StrBuffer &out) const {
    if (_repeatHasStringInst) {
        Insn _istr(0);
        DisInsn istr(_istr, insn, out);
        const auto opc = insn.readByte();
        if (insn.getError())
            return;
        istr.setOpCode(opc, 0);
        if (TABLE.searchOpCode(cpuType(), istr, out))
            insn.setErrorIf(istr);
        if (!istr.stringInst()) {
            istr.nameBuffer().reset();
            insn.setErrorIf(out, UNKNOWN_INSTRUCTION);
        }
        out.text(istr.name());
    }
}

void DisI8086::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos) const {
    RegName name;
    switch (mode) {
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
        if (mode == M_CS && pos == P_NONE) {  // POP CS
            name = REG_CS;
            insn.setErrorIf(out, REGISTER_NOT_ALLOWED);
        }
        outRegister(out, name);
        break;
    case M_BMOD:
    case M_WMOD:
    case M_BMEM:
    case M_WMEM:
        decodeMemReg(insn, out, mode, pos);
        break;
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
        decodeImmediate(insn, out, mode);
        break;
    case M_BDIR:
    case M_WDIR:
        outMemReg(insn, out, TABLE.overrideSeg(insn.segment()), 0, 6);
        break;
    case M_REL:
    case M_REL8:
        decodeRelative(insn, out, mode);
        break;
    case M_ISTR:
        decodeRepeatStr(insn, out);
        break;
    default:
        break;
    }
}

namespace {

bool validSegOverride(AddrMode mode, uint8_t mod) {
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

bool validSegOverride(const DisInsn &insn) {
    if (insn.segment() == 0)
        return true;
    if (insn.stringInst())
        return true;
    const uint8_t mod = insn.modReg() >> 6;
    return validSegOverride(insn.dst(), mod) || validSegOverride(insn.src(), mod);
}

}  // namespace

void DisI8086::decodeStringInst(DisInsn &insn, StrBuffer &out) const {
    if (insn.segment()) {
        const auto seg = TABLE.overrideSeg(insn.segment());
        switch (insn.opCode() & ~1) {
        case 0xA4:  // MOVS ES:[DI],DS:[SI]
        case 0x20:  // ADD4S ES:[DI],DS:[SI]
        case 0x22:  // SUB4S ES:[DI],DS:[SI]
        case 0x26:  // CMP4S ES:[DI],DS:[SI]
            outMemReg(insn, out, REG_ES, 0, 5);
            out.comma();
            /* Fall-through */
        case 0xAC:  // LODS DS:[SI]
        case 0x6E:  // OUTS DS:[SI]
            outMemReg(insn, out, seg, 0, 4);
            break;
        case 0xA6:  // CMPS DS:[SI],ES:[DI]
            outMemReg(insn, out, seg, 0, 4);
            out.comma();
            outMemReg(insn, out, REG_ES, 0, 5);
            break;
        case 0xAA:  // STOS ES:[DI]
        case 0xAE:  // SCAS ES:[DI]
        case 0x6C:  // INS ES:[DI]
            insn.setErrorIf(out, ILLEGAL_SEGMENT);
            outMemReg(insn, out, seg, 0, 5);
        }
    }
}

Error DisI8086::readCodes(DisInsn &insn) const {
    auto opc = insn.readByte();
    while (insn.isOK() && !_segOverrideInsn && TABLE.isSegmentPrefix(opc)) {
        if (insn.segment()) {
            insn.setError(ILLEGAL_SEGMENT);
            return insn.getError();
        }
        insn.setSegment(opc);
        opc = insn.readByte();
    }
    auto prefix = 0;
    if (TABLE.isPrefix(cpuType(), opc)) {
        prefix = opc;
        opc = insn.readByte();
    }
    insn.setOpCode(opc, prefix);
    return insn.getError();
}

namespace {

bool imulHasSameDstSrc(const DisInsn &insn) {
    const auto modreg = insn.modReg();
    const auto reg = (modreg >> 3) & 7;
    return (insn.opCode() & ~2) == 0x69 &&  // IMUL
           (modreg >> 6) == 3 &&            // regsiter
           (modreg & 7) == reg;             // dst==src
}

}  // namespace

Error DisI8086::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    if (readCodes(insn))
        return _insn.setError(insn);
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    insn.readModReg();
    if (_insn.setErrorIf(insn))
        return _insn.getError();
    if (!validSegOverride(insn))
        return _insn.setErrorIf(insn, ILLEGAL_SEGMENT);

    if (insn.stringInst()) {
        decodeStringInst(insn, out);
    } else {
        decodeOperand(insn, out, insn.dst(), insn.dstPos());
        if (insn.src() != M_NONE && !imulHasSameDstSrc(insn))
            decodeOperand(insn, out.comma(), insn.src(), insn.srcPos());
        if (insn.ext() != M_NONE)
            decodeOperand(insn, out.comma(), insn.ext(), insn.extPos());
    }
    return _insn.setError(insn);
}

}  // namespace i8086
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
