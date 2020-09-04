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

#include <stdio.h>

#include "dis_i8086.h"
#include "table_i8086.h"

namespace libasm {
namespace i8086 {

Error DisI8086::outRegister(RegName name, const char prefix) {
    if (name == REG_UNDEF) return OK;
    if (prefix) *_operands++ = prefix;
    _operands = _regs.outRegName(_operands, name);
    *_operands = 0;
    return OK;
}

RegName DisI8086::decodeRegister(
    const InsnI8086 &insn, AddrMode mode, OprPos pos) {
    uint8_t num = 0;
    switch (pos) {
    case P_OREG: num = insn.opCode(); break;
    case P_OSEG: num = insn.opCode() >> 3; break;
    case P_MOD:
    case P_OMOD: num = insn.modReg(); break;

    case P_REG:  num = insn.modReg() >> 3; break;
    default: return REG_UNDEF;
    }
    switch (mode) {
    case M_BREG: return _regs.decodeByteReg(num);
    case M_WREG: return _regs.decodeWordReg(num);
    case M_SREG: return _regs.decodeSegReg(num);
    default: return REG_UNDEF;
    }
}

Error DisI8086::decodeRelative(
    DisMemory &memory, InsnI8086 &insn, AddrMode mode) {
    int16_t disp;
    if (mode == M_REL8) {
        uint8_t disp8;
        if (insn.readByte(memory, disp8)) return setError(NO_MEMORY);
        disp = static_cast<int8_t>(disp8);
    } else {
        uint16_t disp16;
        if (insn.readUint16(memory, disp16)) return setError(NO_MEMORY);
        disp = static_cast<int16_t>(disp16);
    }
    const Config::uintptr_t target = insn.address() + insn.length() + disp;
    outRelativeAddr(target, insn.address(), mode == M_REL8 ? 8 : 16);
    return OK;
}

Error DisI8086::decodeImmediate(
    DisMemory &memory, InsnI8086 &insn, AddrMode mode) {
    if (mode == M_IMM && insn.oprSize() == SZ_WORD) {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        outConstant(val16);
        return OK;
    }
    if ((mode == M_IMM && insn.oprSize() == SZ_BYTE) || mode == M_IOA) {
        uint8_t val8;
        if (insn.readByte(memory, val8)) return setError(NO_MEMORY);
        outConstant(val8);
        return OK;
    }
    if (mode == M_IMM8) {
        uint8_t val8;
        if (insn.readByte(memory, val8)) return setError(NO_MEMORY);
        outConstant(val8, -16);
        return OK;
    }
    // M_FAR
    uint16_t segment, offset;
    if (insn.readUint16(memory, offset)) return setError(NO_MEMORY);
    if (insn.readUint16(memory, segment)) return setError(NO_MEMORY);
    outAddress(segment);
    *_operands++ = ':';
    outAddress(offset);
    return OK;
}

static RegName getBaseReg(uint8_t mod, uint8_t r_m) {
    if (r_m <= 1 || r_m == 7) return REG_BX;
    if (r_m <= 3) return REG_BP;
    if (r_m == 6 && mod != 0) return REG_BP;
    return REG_UNDEF;
}

static RegName getIndexReg(uint8_t r_m) {
    if (r_m >= 6) return REG_UNDEF;
    return r_m % 2 == 0 ? REG_SI : REG_DI;
}

static OprSize operandSize(const InsnI8086 &insn, AddrMode mode) {
    switch (mode) {
    case M_AL: case M_BREG:
        return SZ_BYTE;
    case M_AX: case M_WREG: case M_SREG:
        return SZ_WORD;
    case M_BMOD:
    case M_WMOD:
        return (insn.modReg() >> 6) == 3 ? insn.oprSize() : SZ_NONE;
    default:
        return SZ_NONE;
    }
}

static OprSize operandSize(const InsnI8086 &insn) {
    if (insn.stringInst()) return insn.oprSize();
    OprSize size;
    if ((size = operandSize(insn, insn.dstMode())) != SZ_NONE)
        return size;
    if ((size = operandSize(insn, insn.srcMode())) != SZ_NONE)
        return size;
    return SZ_NONE;
}

static OprSize pointerSize(const InsnI8086 &insn, AddrMode mode) {
    const OprSize size = insn.oprSize();
    if (size == SZ_NONE) return size;
    switch (mode) {
    case M_BMOD: case M_BMEM:
        return SZ_BYTE;
    case M_WMOD: case M_WMEM:
        return SZ_WORD;
    default:
        return size;
    }
}

static RegName pointerReg(const InsnI8086 &insn) {
    OprSize size;
    if ((size = pointerSize(insn, insn.dstMode())) == SZ_NONE
        && (size = pointerSize(insn, insn.srcMode())) == SZ_NONE)
        return REG_UNDEF;
    return size == SZ_BYTE ? REG_BYTE : REG_WORD;
}

Error DisI8086::outMemReg(
    DisMemory &memory, InsnI8086 &insn,
    RegName seg, uint8_t mod, uint8_t r_m) {
    if (operandSize(insn) == SZ_NONE) {
        const RegName ptr = pointerReg(insn);
        if (ptr != REG_UNDEF) {
            outRegister(ptr);
            outRegister(REG_PTR, ' ');
            *_operands++ = ' ';
        }
    }
    if (seg != REG_UNDEF) {
        outRegister(seg);
        *_operands++ = ':';
    }
    const RegName base = getBaseReg(mod, r_m);
    const RegName index = getIndexReg(r_m);
    *_operands++ = '[';
    char sep = 0;
    outRegister(base, sep);
    if (base != REG_UNDEF) sep = '+';
    outRegister(index, sep);
    if (index != REG_UNDEF) sep = '+';
    if (mod == 1) {
        uint8_t disp;
        if (insn.readByte(memory, disp)) return setError(NO_MEMORY);
        const int8_t disp8 = static_cast<int8_t>(disp);
        if (disp8 >= 0) *_operands++ = sep;
        outConstant(disp8, 10);
    }
    if (mod == 2 || (mod == 0 && r_m == 6)) {
        uint16_t disp;
        if (insn.readUint16(memory, disp)) return setError(NO_MEMORY);
        if (sep) *_operands++ = sep;
        outAddress(disp);
    }
    *_operands++ = ']';
    *_operands = 0;
    return OK;
}

Error DisI8086::decodeMemReg(
    DisMemory &memory, InsnI8086 &insn, AddrMode mode, OprPos pos) {
    const uint8_t mod = insn.modReg() >> 6;
    if (mod == 3) {
        if (mode == M_BMEM || mode == M_WMEM) return setError(ILLEGAL_OPERAND);
        const AddrMode regMode = (mode == M_BMOD ? M_BREG : M_WREG);
        return outRegister(decodeRegister(insn, regMode, pos));
    }
    const uint8_t r_m = insn.modReg() & 07;
    return outMemReg(
        memory, insn, TableI8086.overrideSeg(insn.segment()), mod, r_m);
}

Error DisI8086::decodeRepeatStr(DisMemory &memory, InsnI8086 &rep) {
    if (_repeatHasStringInst) {
        Config::opcode_t opc;
        Insn _istr;
        InsnI8086 istr(_istr);
        if (rep.readByte(memory, opc)) return setError(NO_MEMORY);
        istr.setOpCode(opc, 0);
        if (TableI8086.searchOpCode(istr))
            setError(TableI8086.getError());
        if (!istr.stringInst())
            return setError(UNKNOWN_INSTRUCTION);
        outText(istr.name());
    }
    return OK;
}

Error DisI8086::decodeOperand(
    DisMemory &memory, InsnI8086 &insn, AddrMode mode, OprPos pos) {
    RegName name;
    switch (mode) {
    case M_NONE: return OK;
    case M_AL:  return outRegister(REG_AL);
    case M_CL:  return outRegister(REG_CL);
    case M_AX:  return outRegister(REG_AX);
    case M_DX:  return outRegister(REG_DX);
    case M_BREG:
    case M_WREG:
    case M_SREG:
        name = decodeRegister(insn, mode, pos);
        if (pos == P_OSEG && name == REG_CS) // PUSH/POP CS
            return setError(REGISTER_NOT_ALLOWED);
        return outRegister(name);
    case M_BMOD:
    case M_WMOD:
    case M_BMEM:
    case M_WMEM:
        return decodeMemReg(memory, insn, mode, pos);
    case M_VAL1: outConstant(static_cast<uint8_t>(1)); return OK;
    case M_VAL3: outConstant(static_cast<uint8_t>(3)); return OK;
    case M_IMM:
    case M_IMM8:
    case M_FAR:
    case M_IOA: return decodeImmediate(memory, insn, mode);
    case M_BDIR:
    case M_WDIR:
        return outMemReg(
                memory, insn, TableI8086.overrideSeg(insn.segment()), 0, 6);
    case M_REL:
    case M_REL8:
        return decodeRelative(memory, insn, mode);
    case M_ISTR:
        return decodeRepeatStr(memory, insn);
    default:
        return setError(INTERNAL_ERROR);
    }
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
    if (insn.segment() == 0) return true;
    if (insn.stringInst()) return true;
    const uint8_t mod = insn.modReg() >> 6;
    return validSegOverride(insn.dstMode(), mod)
        || validSegOverride(insn.srcMode(), mod);
}

Error DisI8086::readCodes(DisMemory &memory, InsnI8086 &insn) {
    while (true) {
        Config::opcode_t opCode;
        if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
        if (TableI8086.isSegmentPrefix(opCode)) {
            if (insn.segment()) return setError(ILLEGAL_SEGMENT);
            insn.setSegment(opCode);
            continue;
        }
        Config::opcode_t firstByte = 0;
        if (TableI8086.isFirstByte(opCode)) {
            firstByte = opCode;
            if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
        }
        insn.setOpCode(opCode, firstByte);
        return OK;
    }
}

Error DisI8086::decodeStringInst(DisMemory &memory, InsnI8086 &insn) {
    if (insn.segment()) {
        const RegName seg = TableI8086.overrideSeg(insn.segment());
        switch (insn.opCode() & ~1) {
        case 0xA4: // MOVS ES:[DI],DS:[SI]
            outMemReg(memory, insn, REG_ES, 0, 5);
            *_operands++ = ',';
            /* Fall-through */
        case 0xAC: // LODS DS:[SI]
            outMemReg(memory, insn, seg, 0, 4);
            break;
        case 0xA6: // CMPS DS:[SI],ES:[DI]
            outMemReg(memory, insn, seg, 0, 4);
            *_operands++ = ',';
            outMemReg(memory, insn, REG_ES, 0, 5);
            break;
        case 0xAA: // STDS ES:[DI]
        case 0xAE: // SCAS ES:[DI]
            return setError(ILLEGAL_SEGMENT);
        }
    }
    return setOK();
}

Error DisI8086::decode(DisMemory &memory, Insn &_insn) {
    InsnI8086 insn(_insn);
    if (readCodes(memory, insn)) return getError();
#if 0
    printf("@@@@@ search: opc=%02X first=%02X seg=%02X\n", insn.opCode(), insn.first(), insn.segment());
#endif
    if (TableI8086.searchOpCode(insn))
        return setError(TableI8086.getError());
    if (insn.readModReg(memory)) return setError(NO_MEMORY);

#if 0
    printf("@@@@@  found: opc=%02X first=%02X seg=%02X name=%s dst=%d:%d src=%d:%d modReg=%02X\n", insn.opCode(), insn.first(), insn.segment(), insn.name(), insn.dstMode(), insn.dstPos(), insn.srcMode(), insn.srcPos(), insn.modReg());
#endif
    if (!validSegOverride(insn)) return setError(ILLEGAL_SEGMENT);

    if (insn.stringInst())
        return decodeStringInst(memory, insn);
    if (decodeOperand(memory, insn, insn.dstMode(), insn.dstPos()))
        return getError();
    if (insn.srcMode() == M_NONE)
        return setOK();
    *_operands++ = ',';
    if (decodeOperand(memory, insn, insn.srcMode(), insn.srcPos()))
        return getError();
    return setOK();
}

} // namespace i8086
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
