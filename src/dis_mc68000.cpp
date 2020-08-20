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

#include "dis_mc68000.h"
#include "table_mc68000.h"

namespace libasm {
namespace mc68000 {

void DisMc68000::outRegName(RegName regName) {
    _operands = _regs.outRegName(_operands, regName);
}

void DisMc68000::outOprSize(OprSize size) {
    _operands = _regs.outOprSize(_operands, size);
}

Error DisMc68000::decodeImmediateData(
    DisMemory &memory, InsnMc68000 &insn, OprSize size) {
    if (size == SZ_BYTE) {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        outConstant(static_cast<uint8_t>(val16));
        return OK;
    }
    if (size == SZ_WORD) {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        outConstant(val16);
        return OK;
    }
    if (size == SZ_LONG) {
        uint32_t val32;
        if (insn.readUint32(memory, val32)) return setError(NO_MEMORY);
        outConstant(val32);
        return OK;
    }
    return setError(ILLEGAL_SIZE);
}

Error DisMc68000::decodeEffectiveAddr(
    DisMemory &memory, InsnMc68000 &insn,
    const EaMc68000 &ea) {
    const AddrMode mode = ea.mode;
    if (mode == M_ERROR)
        return setError(ILLEGAL_OPERAND);
    if (mode == M_DREG || mode == M_AREG) {
        outRegName(ea.reg);
        return setOK();
    }
    if (mode == M_IMDAT) {
        *_operands++ = '#';
        return decodeImmediateData(memory, insn, ea.size);
    }

    if (mode == M_PDEC) *_operands++ = '-';
    *_operands++ = '(';
    if (mode == M_DISP || mode == M_PCDSP) {
        const RegName base = (mode == M_DISP) ? ea.reg : REG_PC;
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        if (mode == M_PCDSP) {
            const Config::uintptr_t target =
                insn.address() + 2 + static_cast<int16_t>(val16);
            if (ea.size == SZ_WORD && (target % 2) != 0)
                return setError(OPERAND_NOT_ALIGNED);
            if (ea.size == SZ_LONG && (target % 4) != 0)
                return setError(OPERAND_NOT_ALIGNED);
            outRelativeAddr(target, insn.address(), 16);
        } else {
            if (val16 & 0x8000) {
                const uint16_t disp16 = 0x10000 - val16;
                *_operands++ = '-';
                outConstant(disp16);
            } else {
                outConstant(val16);
            }
        }
        *_operands++ = ',';
        outRegName(base);
    }
    if (mode == M_AIND || mode == M_PINC || mode == M_PDEC) {
        outRegName(ea.reg);
    }
    if (mode == M_AWORD || mode == M_ALONG) {
        Config::uintptr_t target;
        if (mode == M_AWORD) {
            uint16_t val16;
            if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
            const int16_t signed16 = static_cast<int16_t>(val16);
            target = static_cast<int32_t>(signed16);
        } else {
            uint32_t val32;
            if (insn.readUint32(memory, val32)) return setError(NO_MEMORY);
            target = val32;
        }
        if (ea.size == SZ_WORD && (target % 2) != 0)
            return setError(OPERAND_NOT_ALIGNED);
        if (ea.size == SZ_LONG && (target % 4) != 0)
            return setError(OPERAND_NOT_ALIGNED);
        outConstant(target, 16, false, true, addressBits());
    }
    if (mode == M_INDX || mode == M_PCIDX) {
        const RegName base = (mode == M_INDX) ? ea.reg : REG_PC;
        BriefExt ext;
        if (insn.readUint16(memory, ext.word)) return setError(NO_MEMORY);
        const uint8_t val8 = ext.disp();
        if (mode == M_PCIDX) {
            const Config::uintptr_t target =
                insn.address() + 2 + static_cast<int8_t>(val8);
            outRelativeAddr(target, insn.address(), 8);
        } else {
            if (val8 & 0x80) {
                const uint8_t disp8 = 0x100 - val8;
                *_operands++ = '-';
                outConstant(disp8);
            } else {
                outConstant(val8);
            }
        }
        *_operands++ = ',';
        outRegName(base);
        *_operands++ = ',';
        outRegName(ext.index());
        outOprSize(ext.indexSize());
    }
    *_operands++ = ')';
    if (mode == M_AWORD) outOprSize(SZ_WORD);
    if (mode == M_ALONG)  outOprSize(SZ_LONG);
    if (mode == M_PINC) *_operands++ = '+';
    *_operands = 0;
    return setOK();
}

Error DisMc68000::decodeRelative(
    DisMemory &memory, InsnMc68000 &insn, uint8_t rel8) {
    Config::uintptr_t target = insn.address() + 2;
    if (rel8) {
        target += static_cast<int8_t>(rel8);
    } else {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        target += static_cast<int16_t>(val16);
    }
    if (target % 2) return setError(OPERAND_NOT_ALIGNED);
    outRelativeAddr(target, insn.address(), rel8 ? 8 : 16);
    return setOK();
}

static RegName decodeMoveMltReg(int8_t regno) {
    return (regno < 8)
        ? RegMc68000::decodeDataReg(regno)
        : RegMc68000::decodeAddrReg(regno - 8);
}

void DisMc68000::decodeMoveMltRegList(
    uint16_t list, bool push,
    void (DisMc68000::*outRegs)(RegName, RegName, char)) {
    int8_t start = -1;
    int8_t last = 0;
    uint16_t mask = push ? 0x8000 : 0x0001;
    for (int8_t i = 0; i < 16; i++) {
        if (list & mask) {
            if (start < 0) {
                start = last = i;
            } else if (i == last + 1) {
                last = i;
            } else {
                (this->*outRegs)(
                    decodeMoveMltReg(start), decodeMoveMltReg(last), '/');
                start = last = i;
            }
        }
        if (push) mask >>= 1;
        else mask <<= 1;
    }
    if (start >= 0)
        (this->*outRegs)(
            decodeMoveMltReg(start), decodeMoveMltReg(last), 0);
}

void DisMc68000::outMoveMltRegs(RegName start, RegName last, char suffix) {
    outRegName(start);
    if (start != last) {
        *_operands++ = '-';
        outRegName(last);
    }
    if (suffix) *_operands++ = suffix;
    *_operands = 0;
}

Error DisMc68000::decodeOperand(
    DisMemory &memory, InsnMc68000 &insn, AddrMode mode,
    uint8_t m, uint8_t r, OprSize s, uint16_t opr16) {
    EaMc68000 ea(s, m, r);
    switch (mode) {
    case M_AREG:
    case M_PDEC:
    case M_PINC:
    case M_DISP:
        ea.size = s;
        ea.mode = mode;
        ea.reg = _regs.decodeAddrReg(r);
        /* Fall-through */
    case M_RADDR:
    case M_WADDR:
    case M_DADDR:
    case M_IADDR:
    case M_JADDR:
    case M_WDATA:
    case M_WMEM:
    case M_RDATA:
    case M_RMEM:
        return decodeEffectiveAddr(memory, insn, ea);
    case M_DREG:
        ea.size = s;
        ea.mode = M_DREG;
        ea.reg = _regs.decodeDataReg(r);
        return decodeEffectiveAddr(memory, insn, ea);
    case M_IM3:
        r = (insn.opCode() >> 9) & 7;
        if (r == 0) r = 8;
        *_operands++ = '#';
        outConstant(r, 10);
        return OK;
    case M_IM8:
        *_operands++ = '#';
        outConstant(static_cast<uint8_t>(insn.opCode()), -16);
        return OK;
    case M_IMBIT:
        if (s == SZ_BYTE && opr16 >= 8) return setError(ILLEGAL_BIT_NUMBER);
        if (s == SZ_WORD && opr16 >= 16) return setError(ILLEGAL_BIT_NUMBER);
        if (s == SZ_LONG && opr16 >= 32) return setError(ILLEGAL_BIT_NUMBER);
        *_operands++ = '#';
        outConstant(static_cast<uint8_t>(opr16), 10);
        break;
    case M_IMDAT:
        *_operands++ = '#';
        return decodeImmediateData(memory, insn, s);
    case M_IMVEC:
        *_operands++ = '#';
        outConstant(static_cast<uint8_t>(insn.opCode() & 0xF), 10);
        return OK;
    case M_IMDSP:
        *_operands++ = '#';
        outConstant(opr16, -16);
        return OK;
    case M_MULT:
        if (opr16 == 0) return setError(OPCODE_HAS_NO_EFFECT);
        decodeMoveMltRegList(
            opr16, AddrMode((insn.opCode() >> 3) & 7) == M_PDEC,
            &DisMc68000::outMoveMltRegs);
        return OK;
    case M_NONE:
        return OK;
    case M_SR:
        outRegName(REG_SR);
        return OK;
    case M_CCR:
        outRegName(REG_CCR);
        return OK;
    case M_USP:
        outRegName(REG_USP);
        return OK;
    case M_REL8:
        return decodeRelative(
            memory, insn, static_cast<uint8_t>(insn.opCode()));
    case M_REL16:
        return decodeRelative(memory, insn, 0);
    default:
        break;
    }
    return OK;
}

Error DisMc68000::checkOperand(
        AddrMode mode, uint8_t m, uint8_t r, OprSize s) {
    EaMc68000 ea(s, m, r);
    switch (mode) {
    case M_WDATA:
        if (ea.mode == M_AREG
            || ea.mode == M_PCDSP || ea.mode == M_PCIDX || ea.mode == M_IMDAT)
            return setError(OPERAND_NOT_ALLOWED);
        break;
    case M_WMEM:
        if (ea.mode == M_DREG || ea.mode == M_AREG
            || ea.mode == M_PCDSP || ea.mode == M_PCIDX || ea.mode == M_IMDAT)
            return setError(OPERAND_NOT_ALLOWED);
        break;
    case M_RMEM:
        if (ea.mode == M_AREG || ea.mode == M_IMDAT)
            return setError(OPERAND_NOT_ALLOWED);
        break;
    case M_RDATA:
        if (ea.mode == M_AREG)
            return setError(OPERAND_NOT_ALLOWED);
        break;
    case M_JADDR:
        if (ea.mode == M_DREG || ea.mode == M_AREG
            || ea.mode == M_PINC || ea.mode == M_PDEC
            || ea.mode == M_IMDAT)
            return setError(OPERAND_NOT_ALLOWED);
        break;
    case M_IADDR:
        if (ea.mode == M_DREG || ea.mode == M_AREG || ea.mode == M_PDEC
            || ea.mode == M_IMDAT)
            return setError(OPERAND_NOT_ALLOWED);
        break;
    case M_DADDR:
        if (ea.mode == M_DREG || ea.mode == M_AREG || ea.mode == M_PINC
            || ea.mode == M_PCDSP || ea.mode == M_PCIDX
            || ea.mode == M_IMDAT)
            return setError(OPERAND_NOT_ALLOWED);
        break;
    case M_WADDR:
        if (ea.mode == M_IMDAT || ea.mode == M_PCDSP || ea.mode == M_PCIDX)
            return setError(OPERAND_NOT_ALLOWED);
        /* Fall-through */
    case M_RADDR:
        if (ea.mode == M_AREG && s == SZ_BYTE)
            return setError(OPERAND_NOT_ALLOWED);
        break;
    default:
        break;
    }
    return OK;
}

static uint8_t modeVal(Config::opcode_t opc, OprPos pos) {
    switch (pos) {
    case OP_10: return (opc >> 3) & 7;
    case OP_23: return (opc >> 6) & 7;
    default: return 0;
    }
}
static uint8_t regVal(Config::opcode_t opc, OprPos pos) {
    switch (pos) {
    case OP_10:
    case OP__0:
        return (opc >> 0) & 7;
    case OP_23:
    case OP__3:
        return (opc >> 9) & 7;
    default:
        return 0;
    }
}

static OprSize sizeVal(const InsnMc68000 &insn) {
    const OprSize size = insn.oprSize();
    const Config::opcode_t opc = insn.opCode();
    if (size == SZ_DATA) {
        switch ((opc >> 6) & 3) {
        case 0: return SZ_BYTE;
        case 1: return SZ_WORD;
        case 2: return SZ_LONG;
        default: return SZ_ERROR;
        }
    }
    if (size == SZ_ADR6)
        return (opc & (1 << 6)) ? SZ_LONG : SZ_WORD;
    if (size == SZ_ADR8)
        return (opc & (1 << 8)) ? SZ_LONG : SZ_WORD;
    return size;
}

Error DisMc68000::decode(
    DisMemory &memory, Insn &_insn) {
    InsnMc68000 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readUint16(memory, opCode)) return setError(NO_MEMORY);
    insn.setOpCode(opCode);
    if (TableMc68000.searchOpCode(insn))
        return setError(TableMc68000.getError());

    const AddrMode src = insn.srcMode();
    const AddrMode dst = insn.dstMode();
    const OprSize size = sizeVal(insn);
    if (size == SZ_ERROR) return setError(ILLEGAL_SIZE);
    const OprPos srcPos = insn.srcPos();
    if (checkOperand(src, modeVal(opCode, srcPos), regVal(opCode, srcPos), size))
        return getError();
    const OprPos dstPos = insn.dstPos();
    if (checkOperand(dst, modeVal(opCode, dstPos), regVal(opCode, dstPos), size))
        return getError();
    uint16_t opr16 = 0;
    if (src == M_IMBIT || src == M_MULT || dst == M_MULT || dst == M_IMDSP) {
        if (insn.readUint16(memory, opr16)) return setError(NO_MEMORY);
    }
    if (insn.insnSize() != ISZ_NONE)
        insn.appendOprSize(size, _regs);
    if (src == M_NONE) return setOK();
    if (decodeOperand(memory, insn, src, modeVal(opCode, srcPos), regVal(opCode, srcPos), size, opr16))
        return getError();
    if (dst == M_NONE) return setOK();
    *_operands++ = ',';
    return decodeOperand(memory, insn, dst, modeVal(opCode, dstPos), regVal(opCode, dstPos), size, opr16);
}

} // namespace mc68000
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
