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

StrBuffer &DisMc68000::outRegName(StrBuffer &out, RegName regName) {
    return _regs.outRegName(out, regName);
}

StrBuffer &DisMc68000::outOprSize(StrBuffer &out, OprSize size) {
    const char suffix = _regs.sizeSuffix(size);
    if (suffix)
        out.letter('.').letter(suffix);
    return out;
}

Error DisMc68000::decodeImmediateData(
        DisMemory &memory, InsnMc68000 &insn, StrBuffer &out, OprSize size) {
    if (size == SZ_BYTE) {
        outHex(out, insn.readUint16(memory) & 0xFF, 8);
    } else if (size == SZ_WORD) {
        if (insn.srcMode() == M_CCR || insn.dstMode() == M_CCR) {
            outHex(out, insn.readUint16(memory) & 0xFF, 8);
        } else {
            outHex(out, insn.readUint16(memory), 16);
        }
    } else if (size == SZ_LONG) {
        outHex(out, insn.readUint32(memory), 32);
    }
    return setError(insn);
}

Error DisMc68000::decodeEffectiveAddr(
        DisMemory &memory, InsnMc68000 &insn, StrBuffer &out, const EaMc68000 &ea) {
    const AddrMode mode = ea.mode;
    if (mode == M_ERROR)
        return setError(ILLEGAL_OPERAND);
    if (mode == M_DREG || mode == M_AREG) {
        outRegName(out, ea.reg);
        return setOK();
    }
    if (mode == M_IMDAT)
        return decodeImmediateData(memory, insn, out.letter('#'), ea.size);

    if (mode == M_PDEC)
        out.letter('-');
    out.letter('(');
    if (mode == M_DISP || mode == M_PCDSP) {
        const RegName base = (mode == M_DISP) ? ea.reg : REG_PC;
        const uint16_t val16 = insn.readUint16(memory);
        if (mode == M_PCDSP) {
            const Config::uintptr_t target =
                    insn.address() + insn.length() - 2 + static_cast<int16_t>(val16);
            if (ea.size == SZ_WORD && (target % 2) != 0)
                return setError(OPERAND_NOT_ALIGNED);
            if (ea.size == SZ_LONG && (target % 4) != 0)
                return setError(OPERAND_NOT_ALIGNED);
            outRelAddr(out, target, insn.address(), 16);
        } else {
            outHex(out, val16, -16);
        }
        out.letter(',');
        outRegName(out, base);
    }
    if (mode == M_AIND || mode == M_PINC || mode == M_PDEC)
        outRegName(out, ea.reg);
    if (mode == M_AWORD || mode == M_ALONG) {
        Config::uintptr_t target;
        if (mode == M_AWORD) {
            target = static_cast<int16_t>(insn.readUint16(memory));
        } else {
            target = insn.readUint32(memory);
        }
        if (ea.size == SZ_WORD && (target % 2) != 0)
            return setError(OPERAND_NOT_ALIGNED);
        if (ea.size == SZ_LONG && (target % 4) != 0)
            return setError(OPERAND_NOT_ALIGNED);
        outAbsAddr(out, target);
    }
    if (mode == M_INDX || mode == M_PCIDX) {
        const RegName base = (mode == M_INDX) ? ea.reg : REG_PC;
        BriefExt ext;
        ext.word = insn.readUint16(memory);
        const uint8_t val8 = ext.disp();
        if (mode == M_PCIDX) {
            const Config::uintptr_t target =
                    insn.address() + insn.length() - 2 + static_cast<int8_t>(val8);
            outRelAddr(out, target, insn.address(), 8);
        } else {
            outHex(out, val8, -8);
        }
        out.letter(',');
        outRegName(out, base).letter(',');
        outRegName(out, ext.index());
        outOprSize(out, ext.indexSize());
    }
    out.letter(')');
    if (mode == M_AWORD)
        outOprSize(out, SZ_WORD);
    if (mode == M_ALONG)
        outOprSize(out, SZ_LONG);
    if (mode == M_PINC)
        out.letter('+');
    return setError(insn);
}

Error DisMc68000::decodeRelative(
        DisMemory &memory, InsnMc68000 &insn, StrBuffer &out, uint8_t rel8) {
    Config::uintptr_t target = insn.address() + 2;
    if (rel8) {
        target += static_cast<int8_t>(rel8);
    } else {
        target += static_cast<int16_t>(insn.readUint16(memory));
    }
    if (target % 2)
        return setError(OPERAND_NOT_ALIGNED);
    outRelAddr(out, target, insn.address(), rel8 ? 8 : 16);
    return setError(insn);
}

static RegName decodeMoveMltReg(int8_t regno) {
    return (regno < 8) ? RegMc68000::decodeDataReg(regno) : RegMc68000::decodeAddrReg(regno - 8);
}

StrBuffer &DisMc68000::outMoveMltRegList(StrBuffer &out, uint16_t list, bool push,
        StrBuffer &(DisMc68000::*outRegs)(StrBuffer &, RegName, RegName, char)) {
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
                (this->*outRegs)(out, decodeMoveMltReg(start), decodeMoveMltReg(last), '/');
                start = last = i;
            }
        }
        if (push)
            mask >>= 1;
        else
            mask <<= 1;
    }
    if (start >= 0)
        (this->*outRegs)(out, decodeMoveMltReg(start), decodeMoveMltReg(last), 0);
    return out;
}

StrBuffer &DisMc68000::outMoveMltRegs(StrBuffer &out, RegName start, RegName last, char suffix) {
    outRegName(out, start);
    if (start != last)
        outRegName(out.letter('-'), last);
    if (suffix)
        out.letter(suffix);
    return out;
}

Error DisMc68000::decodeOperand(DisMemory &memory, InsnMc68000 &insn, StrBuffer &out, AddrMode mode,
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
        return decodeEffectiveAddr(memory, insn, out, ea);
    case M_DREG:
        ea.size = s;
        ea.mode = M_DREG;
        ea.reg = _regs.decodeDataReg(r);
        return decodeEffectiveAddr(memory, insn, out, ea);
    case M_IM3:
        r = (insn.opCode() >> 9) & 7;
        if (r == 0)
            r = 8;
        outDec(out.letter('#'), r, 4);
        break;
    case M_IM8:
        outHex(out.letter('#'), insn.opCode(), -8);
        break;
    case M_IMBIT:
        if (s == SZ_BYTE && opr16 >= 8)
            return setError(ILLEGAL_BIT_NUMBER);
        if (s == SZ_WORD && opr16 >= 16)
            return setError(ILLEGAL_BIT_NUMBER);
        if (s == SZ_LONG && opr16 >= 32)
            return setError(ILLEGAL_BIT_NUMBER);
        outDec(out.letter('#'), opr16, 5);
        break;
    case M_IMDAT:
        return decodeImmediateData(memory, insn, out.letter('#'), s);
    case M_IMVEC:
        outDec(out.letter('#'), insn.opCode() & 0xF, 4);
        break;
    case M_IMDSP:
        outHex(out.letter('#'), opr16, -16);
        break;
    case M_MULT:
        if (opr16 == 0)
            return setError(OPCODE_HAS_NO_EFFECT);
        outMoveMltRegList(out, opr16, AddrMode((insn.opCode() >> 3) & 7) == M_PDEC,
                &DisMc68000::outMoveMltRegs);
        break;
    case M_NONE:
        break;
    case M_SR:
        outRegName(out, REG_SR);
        break;
    case M_CCR:
        outRegName(out, REG_CCR);
        break;
    case M_USP:
        outRegName(out, REG_USP);
        break;
    case M_REL8:
        return decodeRelative(memory, insn, out, static_cast<uint8_t>(insn.opCode()));
    case M_REL16:
        return decodeRelative(memory, insn, out, 0);
    default:
        break;
    }
    return OK;
}

Error DisMc68000::checkOperand(AddrMode mode, uint8_t m, uint8_t r, OprSize s) {
    EaMc68000 ea(s, m, r);
    switch (mode) {
    case M_WDATA:
        if (ea.mode == M_AREG || ea.mode == M_PCDSP || ea.mode == M_PCIDX || ea.mode == M_IMDAT)
            return setError(OPERAND_NOT_ALLOWED);
        break;
    case M_WMEM:
        if (ea.mode == M_DREG || ea.mode == M_AREG || ea.mode == M_PCDSP || ea.mode == M_PCIDX ||
                ea.mode == M_IMDAT)
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
        if (ea.mode == M_DREG || ea.mode == M_AREG || ea.mode == M_PINC || ea.mode == M_PDEC ||
                ea.mode == M_IMDAT)
            return setError(OPERAND_NOT_ALLOWED);
        break;
    case M_IADDR:
        if (ea.mode == M_DREG || ea.mode == M_AREG || ea.mode == M_PDEC || ea.mode == M_IMDAT)
            return setError(OPERAND_NOT_ALLOWED);
        break;
    case M_DADDR:
        if (ea.mode == M_DREG || ea.mode == M_AREG || ea.mode == M_PINC || ea.mode == M_PCDSP ||
                ea.mode == M_PCIDX || ea.mode == M_IMDAT)
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
    case OP_10:
        return (opc >> 3) & 7;
    case OP_23:
        return (opc >> 6) & 7;
    default:
        return 0;
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
        case 0:
            return SZ_BYTE;
        case 1:
            return SZ_WORD;
        case 2:
            return SZ_LONG;
        default:
            return SZ_ERROR;
        }
    }
    if (size == SZ_ADR6)
        return (opc & (1 << 6)) ? SZ_LONG : SZ_WORD;
    if (size == SZ_ADR8)
        return (opc & (1 << 8)) ? SZ_LONG : SZ_WORD;
    return size;
}

Error DisMc68000::decode(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnMc68000 insn(_insn);
    const Config::opcode_t opCode = insn.readUint16(memory);
    if (setError(insn))
        return getError();
    insn.setOpCode(opCode);

    if (TableMc68000::TABLE.searchOpCode(insn))
        return setError(TableMc68000::TABLE.getError());

    const AddrMode src = insn.srcMode();
    const AddrMode dst = insn.dstMode();
    const OprSize size = sizeVal(insn);
    if (size == SZ_ERROR)
        return setError(ILLEGAL_SIZE);
    const OprPos srcPos = insn.srcPos();
    const uint8_t srcMode = modeVal(opCode, srcPos);
    const uint8_t srcReg = regVal(opCode, srcPos);
    if (checkOperand(src, srcMode, srcReg, size))
        return getError();
    const OprPos dstPos = insn.dstPos();
    const uint8_t dstMode = modeVal(opCode, dstPos);
    const uint8_t dstReg = regVal(opCode, dstPos);
    if (checkOperand(dst, dstMode, dstReg, size))
        return getError();

    uint16_t opr16 = 0;
    if (src == M_IMBIT || src == M_MULT || dst == M_MULT || dst == M_IMDSP)
        opr16 = insn.readUint16(memory);

    const InsnSize iSize = insn.insnSize();
    const OprSize oSize = (iSize == ISZ_DATA || insn.withSize()) ? size : OprSize(iSize);
    const char suffix = _regs.sizeSuffix(oSize);
    if (suffix)
        insn.nameBuffer().letter('.').letter(suffix);

    if (src == M_NONE)
        return setOK();
    if (decodeOperand(memory, insn, out, src, srcMode, srcReg, size, opr16))
        return getError();

    if (dst == M_NONE)
        return setOK();
    out.comma();
    return decodeOperand(memory, insn, out, dst, dstMode, dstReg, size, opr16);
}

}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
