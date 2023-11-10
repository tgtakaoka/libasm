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

namespace {
StrBuffer &outOprSize(StrBuffer &out, OprSize size) {
    const auto suffix = sizeSuffix(size);
    if (suffix)
        out.letter('.').letter(suffix);
    return out;
}
}  // namespace

void DisMc68000::decodeImmediateData(DisInsn &insn, StrBuffer &out, OprSize size) const {
    out.letter('#');
    uint32_t val;
    uint8_t bits;
    if (size == SZ_QUAD) {
        val = insn.readUint32();
        bits = 32;
    } else {
        val = insn.readUint16();
        bits = 16;
        if (size == SZ_BYTE || insn.src() == M_CCR || insn.dst() == M_CCR) {
            val &= 0xFF;
            bits = 8;
        }
    }
    outHex(out, val, bits);
}

void DisMc68000::decodeEffectiveAddr(DisInsn &insn, StrBuffer &out, const EaMc68000 &ea) const {
    const auto mode = ea.mode;
    if (mode == M_DREG || mode == M_AREG) {
        outRegName(out, ea.reg);
        return;
    }
    if (mode == M_IMDAT) {
        decodeImmediateData(insn, out, ea.size);
        return;
    }

    if (mode == M_PDEC)
        out.letter('-');
    if (mode == M_DISP || mode == M_PCDSP) {
        const auto base = (mode == M_DISP) ? ea.reg : REG_PC;
        const auto val16 = insn.readUint16();
        if (mode == M_PCDSP) {
            const Config::uintptr_t target =
                    insn.address() + insn.length() - 2 + static_cast<int16_t>(val16);
            if (ea.size != SZ_BYTE && (target % 2) != 0)
                insn.setErrorIf(out, OPERAND_NOT_ALIGNED);
            outRelAddr(out.letter('('), target, insn.address(), 16);
        } else {
            outHex(out.letter('('), val16, -16);
        }
        out.letter(',');
        outRegName(out, base);
    } else if (mode == M_AIND || mode == M_PINC || mode == M_PDEC) {
        outRegName(out.letter('('), ea.reg);
    } else if (mode == M_AWORD || mode == M_ALONG) {
        const Config::uintptr_t target =
                (mode == M_AWORD) ? static_cast<int16_t>(insn.readUint16()) : insn.readUint32();
        if (ea.size != SZ_BYTE && (target % 2) != 0)
            insn.setErrorIf(out, OPERAND_NOT_ALIGNED);
        outAbsAddr(out.letter('('), target);
    } else if (mode == M_INDX || mode == M_PCIDX) {
        const auto base = (mode == M_INDX) ? ea.reg : REG_PC;
        BriefExt ext;
        ext.word = insn.readUint16();
        const uint8_t val8 = ext.disp();
        out.letter('(');
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
        outOprSize(out, SZ_QUAD);
    if (mode == M_PINC)
        out.letter('+');
}

void DisMc68000::decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto base = insn.address() + 2;
    const auto rel8 = (mode == M_REL8) ? static_cast<int8_t>(insn.opCode() & 0xFF) : 0;
    const auto delta = rel8 ? static_cast<int8_t>(rel8) : static_cast<int16_t>(insn.readUint16());
    if (mode == M_REL8 && rel8 == 0 && !overflowInt8(delta)) {
        auto save{out};
        insn.nameBuffer().over(out);
        out.letter('.').letter('W').over(insn.nameBuffer());
        save.over(out);
    }
    const auto target = base + delta;
    insn.setErrorIf(out, checkAddr(target, true));
    outRelAddr(out, target, insn.address(), rel8 ? 8 : 16);
}

namespace {

RegName decodeMoveMltReg(int8_t regno) {
    return (regno < 8) ? decodeDataReg(regno) : decodeAddrReg(regno - 8);
}

StrBuffer &outMoveMltRegs(StrBuffer &out, RegName start, RegName last, char suffix) {
    outRegName(out, start);
    if (start != last)
        outRegName(out.letter('-'), last);
    if (suffix)
        out.letter(suffix);
    return out;
}

StrBuffer &outMoveMltRegList(StrBuffer &out, uint16_t list, bool push) {
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
                outMoveMltRegs(out, decodeMoveMltReg(start), decodeMoveMltReg(last), '/');
                start = last = i;
            }
        }
        if (push)
            mask >>= 1;
        else
            mask <<= 1;
    }
    if (start >= 0)
        outMoveMltRegs(out, decodeMoveMltReg(start), decodeMoveMltReg(last), 0);
    return out;
}

}  // namespace

void DisMc68000::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, uint8_t m, uint8_t r,
        OprSize s, uint16_t opr16, Error opr16Error) const {
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
        decodeEffectiveAddr(insn, out, ea);
        break;
    case M_DREG:
        ea.size = s;
        ea.mode = M_DREG;
        ea.reg = decodeDataReg(r);
        decodeEffectiveAddr(insn, out, ea);
        break;
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
        out.letter('#');
        if (insn.src() == M_IMBIT)
            opr16 = insn.readUint16();
        if (s == SZ_BYTE && opr16 >= 8) {
            insn.setErrorIf(out, ILLEGAL_BIT_NUMBER);
        } else if (s == SZ_WORD && opr16 >= 16) {
            insn.setErrorIf(out, ILLEGAL_BIT_NUMBER);
        } else if (s == SZ_QUAD && opr16 >= 32) {
            insn.setErrorIf(out, ILLEGAL_BIT_NUMBER);
        }
        outDec(out, opr16, 16);
        break;
    case M_IMDAT:
        decodeImmediateData(insn, out, s);
        break;
    case M_IMVEC:
        outDec(out.letter('#'), insn.opCode() & 0xF, 4);
        break;
    case M_IMDSP:
        out.letter('#');
        if (opr16Error == NO_MEMORY)
            insn.setError(out, opr16Error);
        outHex(out, opr16, -16);
        break;
    case M_MULT:
        if (insn.src() == M_MULT) {
            opr16 = insn.readUint16();
        } else if (opr16Error == NO_MEMORY) {
            insn.setError(out, opr16Error);
        }
        if (opr16 == 0)
            insn.setErrorIf(out, OPCODE_HAS_NO_EFFECT);
        outMoveMltRegList(out, opr16, AddrMode((insn.opCode() >> 3) & 7) == M_PDEC);
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
    case M_REL16:
        return decodeRelative(insn, out, mode);
    default:
        break;
    }
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
            return SZ_QUAD;
        default:
            return SZ_OCTA;
        }
    }
    if (size == SZ_ADDR)
        return (opc & (1 << 6)) ? SZ_QUAD : SZ_WORD;
    if (size == SZ_ADR8)
        return (opc & (1 << 8)) ? SZ_QUAD : SZ_WORD;
    return size;
}

}  // namespace

Error DisMc68000::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readUint16();
    insn.setOpCode(opc);
    TABLE.searchOpCode(cpuType(), insn, out);
    if (insn.getError())
        return _insn.setError(insn);

    const auto size = sizeVal(insn);
    const auto insnSize = insn.insnSize();
    const auto oprSize = (insnSize == ISZ_DATA || insn.hasSize()) ? size : OprSize(insnSize);
    const auto suffix = sizeSuffix(oprSize);
    if (suffix) {
        auto save{out};
        insn.nameBuffer().over(out);
        out.letter('.').letter(suffix).over(insn.nameBuffer());
        save.over(out);
    }

    const auto dst = insn.dst();
    const auto opr16 = (dst == M_MULT || dst == M_IMDSP) ? insn.readUint16() : 0;
    const auto opr16Error = insn.getError();

    const auto srcPos = insn.srcPos();
    decodeOperand(insn, out, insn.src(), modeVal(opc, srcPos), regVal(opc, srcPos), size);
    const auto dstPos = insn.dstPos();
    if (dst != M_NONE)
        decodeOperand(insn, out.comma(), dst, modeVal(opc, dstPos), regVal(opc, dstPos), size,
                opr16, opr16Error);
    return _insn.setError(insn);
}

}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
