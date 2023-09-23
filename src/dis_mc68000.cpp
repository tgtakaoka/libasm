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

#include "reg_mc68000.h"
#include "table_mc68000.h"

namespace libasm {
namespace mc68000 {

using namespace reg;

const ValueFormatter::Plugins &DisMc68000::defaultPlugins() {
    return ValueFormatter::Plugins::motorola();
}

DisMc68000::DisMc68000(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins), Config(TABLE) {
    reset();
}

StrBuffer &DisMc68000::outOprSize(StrBuffer &out, OprSize size) {
    const auto suffix = sizeSuffix(size);
    if (suffix)
        out.letter('.').letter(suffix);
    return out;
}

Error DisMc68000::decodeImmediateData(DisInsn &insn, StrBuffer &out, OprSize size) {
    if (size == SZ_BYTE) {
        outHex(out, insn.readUint16() & 0xFF, 8);
    } else if (size == SZ_WORD) {
        if (insn.src() == M_CCR || insn.dst() == M_CCR) {
            outHex(out, insn.readUint16() & 0xFF, 8);
        } else {
            outHex(out, insn.readUint16(), 16);
        }
    } else if (size == SZ_LONG) {
        outHex(out, insn.readUint32(), 32);
    }
    return setError(insn);
}

Error DisMc68000::decodeEffectiveAddr(DisInsn &insn, StrBuffer &out, const EaMc68000 &ea) {
    const auto mode = ea.mode;
    if (mode == M_ERROR)
        return setError(ILLEGAL_OPERAND);
    if (mode == M_DREG || mode == M_AREG) {
        outRegName(out, ea.reg);
        return setOK();
    }
    if (mode == M_IMDAT)
        return decodeImmediateData(insn, out.letter('#'), ea.size);

    if (mode == M_PDEC)
        out.letter('-');
    out.letter('(');
    if (mode == M_DISP || mode == M_PCDSP) {
        const auto base = (mode == M_DISP) ? ea.reg : REG_PC;
        const uint16_t val16 = insn.readUint16();
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
            target = static_cast<int16_t>(insn.readUint16());
        } else {
            target = insn.readUint32();
        }
        if (ea.size == SZ_WORD && (target % 2) != 0)
            return setError(OPERAND_NOT_ALIGNED);
        if (ea.size == SZ_LONG && (target % 4) != 0)
            return setError(OPERAND_NOT_ALIGNED);
        outAbsAddr(out, target);
    }
    if (mode == M_INDX || mode == M_PCIDX) {
        const auto base = (mode == M_INDX) ? ea.reg : REG_PC;
        BriefExt ext;
        ext.word = insn.readUint16();
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

Error DisMc68000::decodeRelative(DisInsn &insn, StrBuffer &out, uint8_t rel8) {
    const auto base = insn.address() + 2;
    int16_t delta;
    if (rel8) {
        delta = static_cast<int8_t>(rel8);
    } else {
        delta = static_cast<int16_t>(insn.readUint16());
    }
    const auto target = branchTarget(base, delta);
    outRelAddr(out, target, insn.address(), rel8 ? 8 : 16);
    return setErrorIf(insn);
}

namespace {

RegName decodeMoveMltReg(int8_t regno) {
    return (regno < 8) ? decodeDataReg(regno) : decodeAddrReg(regno - 8);
}

}  // namespace

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

Error DisMc68000::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, uint8_t m, uint8_t r,
        OprSize s, uint16_t opr16) {
    EaMc68000 ea(s, m, r);
    switch (mode) {
    case M_AREG:
    case M_PDEC:
    case M_PINC:
    case M_DISP:
        ea.size = s;
        ea.mode = mode;
        ea.reg = decodeAddrReg(r);
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
        return decodeEffectiveAddr(insn, out, ea);
    case M_DREG:
        ea.size = s;
        ea.mode = M_DREG;
        ea.reg = decodeDataReg(r);
        return decodeEffectiveAddr(insn, out, ea);
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
        return decodeImmediateData(insn, out.letter('#'), s);
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
        return decodeRelative(insn, out, static_cast<uint8_t>(insn.opCode()));
    case M_REL16:
        return decodeRelative(insn, out, 0);
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

namespace {

uint8_t modeVal(Config::opcode_t opc, OprPos pos) {
    switch (pos) {
    case OP_10:
        return (opc >> 3) & 7;
    case OP_23:
        return (opc >> 6) & 7;
    default:
        return 0;
    }
}

uint8_t regVal(Config::opcode_t opc, OprPos pos) {
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

OprSize sizeVal(const DisInsn &insn) {
    const auto size = insn.oprSize();
    const auto opc = insn.opCode();
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

}  // namespace

Error DisMc68000::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    DisInsn insn(_insn, memory);
    const auto opCode = insn.readUint16();
    if (setError(insn))
        return getError();
    insn.setOpCode(opCode);

    if (TABLE.searchOpCode(cpuType(), insn, out))
        return setError(insn);

    const auto src = insn.src();
    const auto dst = insn.dst();
    const auto size = sizeVal(insn);
    if (size == SZ_ERROR)
        return setError(ILLEGAL_SIZE);
    const auto srcPos = insn.srcPos();
    const auto srcModeVal = modeVal(opCode, srcPos);
    const auto srcReg = regVal(opCode, srcPos);
    if (checkOperand(src, srcModeVal, srcReg, size))
        return getError();
    const auto dstPos = insn.dstPos();
    const auto dstModeVal = modeVal(opCode, dstPos);
    const auto dstReg = regVal(opCode, dstPos);
    if (checkOperand(dst, dstModeVal, dstReg, size))
        return getError();

    uint16_t opr16 = 0;
    if (src == M_IMBIT || src == M_MULT || dst == M_MULT || dst == M_IMDSP)
        opr16 = insn.readUint16();

    const auto iSize = insn.insnSize();
    const auto oSize = (iSize == ISZ_DATA || insn.hasSize()) ? size : OprSize(iSize);
    const auto suffix = sizeSuffix(oSize);
    if (suffix) {
        StrBuffer save(out);
        insn.nameBuffer().over(out);
        out.letter('.').letter(suffix).over(insn.nameBuffer());
        save.over(out);
    }

    if (src == M_NONE)
        return setOK();
    if (decodeOperand(insn, out, src, srcModeVal, srcReg, size, opr16))
        return getError();

    if (dst == M_NONE)
        return setOK();
    out.comma();
    return decodeOperand(insn, out, dst, dstModeVal, dstReg, size, opr16);
}

}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
