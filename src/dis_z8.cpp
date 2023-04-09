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

#include "dis_z8.h"

#include "reg_z8.h"
#include "table_z8.h"

namespace libasm {
namespace z8 {

using namespace reg;

static const char OPT_BOOL_WORK_REGISTER[] PROGMEM = "work-register";
static const char OPT_DESC_WORK_REGISTER[] PROGMEM = "prefer work register name than alias address";

DisZ8::DisZ8() : Disassembler(_formatter, TableZ8::TABLE, '$', &_opt_workRegister), _formatter() {
    reset();
}

void DisZ8::reset() {
    Disassembler::reset();
    _useWorkRegister = true;
}

DisZ8::OptWorkRegister::OptWorkRegister(bool &var)
    : BoolOption(OPT_BOOL_WORK_REGISTER, OPT_DESC_WORK_REGISTER, var) {}

StrBuffer &DisZ8::outConditionCode(StrBuffer &out, Config::opcode_t opCode) {
    const auto cc = decodeCcNum(opCode >> 4);
    if (cc != CC_T)
        outCcName(out, cc).comma();
    return out;
}

static StrBuffer &outWorkReg(StrBuffer &out, uint8_t num, bool indir = false) {
    const auto reg = decodeRegNum(num);
    if (indir)
        out.letter('@');
    return outRegName(out, reg);
}

static StrBuffer &outPairReg(StrBuffer &out, uint8_t num, bool indir = false) {
    const auto reg = decodePairRegNum(num);
    if (indir)
        out.letter('@');
    return outRegName(out, reg);
}

StrBuffer &DisZ8::outRegAddr(StrBuffer &out, uint8_t addr, bool indir) {
    if (_useWorkRegister && isWorkRegAlias(addr))
        return outWorkReg(out, addr & 0xF, indir);
    if (indir)
        out.letter('@');
    const auto label = lookup(addr);
    if (label)
        return out.letter('>').text(label);
    if (addr < 16 && !indir)
        out.letter('>');
    return outAbsAddr(out, addr, 8);
}

StrBuffer &DisZ8::outPairAddr(StrBuffer &out, uint8_t addr, bool indir) {
    if (_useWorkRegister && isWorkRegAlias(addr))
        return outPairReg(out, addr & 0xF, indir);
    if (indir)
        out.letter('@');
    const auto label = lookup(addr);
    if (label)
        return out.letter('>').text(label);
    if (addr < 16 && !indir)
        out.letter('>');
    return outAbsAddr(out, addr, 8);
}

StrBuffer &DisZ8::outBitPos(StrBuffer &out, uint8_t bitPos) {
    bitPos &= 7;
    out.comma();
    return outHex(out.letter('#'), bitPos, 3);
}

Error DisZ8::decodeOperand(DisMemory &memory, InsnZ8 &insn, StrBuffer &out, AddrMode mode) {
    const auto post = insn.postFormat();
    uint8_t val = post ? insn.post() : insn.readByte(memory);
    if (setError(insn))
        return getError();
    if (mode == M_R || mode == M_IR || mode == M_RR || mode == M_IRR) {
        const auto pair = (mode == M_RR || mode == M_IRR);
        const auto indir = (mode == M_IR || mode == M_IRR);
        if (pair) {
            if (val % 2)
                return setError(ILLEGAL_REGISTER);
            outPairAddr(out, val, indir);
        } else {
            outRegAddr(out, val, indir);
        }
    }
    if (mode == M_IM) {
        if (post == PF2_0 || post == PF2_1 || post == PF2_2)
            val &= ~3;
        if (post == PF_NONE && insn.opCode() == TableZ8::SRP && (val & ~0xF0) != 0)
            return setError(OPERAND_NOT_ALLOWED);
        outHex(out.letter('#'), val, 8);
    }
    return setOK();
}

Error DisZ8::decodeAbsolute(DisMemory &memory, InsnZ8 &insn, StrBuffer &out, Endian endian) {
    const Config::uintptr_t addr =
            (endian == ENDIAN_LITTLE) ? insn.readUint16Le(memory) : insn.readUint16Be(memory);
    outAbsAddr(out, addr);
    return setError(insn);
}

Error DisZ8::decodeRelative(DisMemory &memory, InsnZ8 &insn, StrBuffer &out) {
    const auto delta = static_cast<int8_t>(insn.readByte(memory));
    const auto base = insn.address() + insn.length();
    const auto target = branchTarget(base, delta);
    outRelAddr(out, target, insn.address(), 8);
    return setError(insn);
}

StrBuffer &DisZ8::outIndexed(StrBuffer &out, uint16_t base, RegName idx, AddrMode mode) {
    if (mode == M_XL) {
        outAbsAddr(out, base);
    } else if (mode == M_XS) {
        const int8_t disp = static_cast<int8_t>(base);
        if (disp > 0)
            out.letter('+');
        outDec(out, disp, -8);
    } else {  // M_X
        outHex(out, base, 8);
    }
    out.letter('(');
    if (mode == M_X) {
        outWorkReg(out, idx);
    } else {
        outPairReg(out, idx);
    }
    return out.letter(')');
}

Error DisZ8::decodeIndexed(DisMemory &memory, InsnZ8 &insn, StrBuffer &out, uint8_t opr1) {
    const auto dst = insn.dst();
    const auto src = insn.src();
    const auto pair = !(dst == M_X || src == M_X);
    uint16_t base16;
    if (dst == M_XL || src == M_XL) {
        base16 = insn.readUint16Le(memory);
    } else {
        base16 = insn.readByte(memory);
    }
    const auto idx = pair ? decodePairRegNum(opr1) : decodeRegNum(opr1);
    if (idx == REG_UNDEF)
        return setError(ILLEGAL_REGISTER);
    if (dst == M_r) {
        outWorkReg(out, opr1 >> 4);
    } else {
        outIndexed(out, base16, idx, dst);
    }
    out.comma();
    if (src == M_r) {
        outWorkReg(out, opr1 >> 4);
    } else {
        outIndexed(out, base16, idx, src);
    }
    return setError(insn);
}

Error DisZ8::decodeIndirectRegPair(DisMemory &memory, InsnZ8 &insn, StrBuffer &out) {
    const uint8_t opr = insn.readByte(memory);
    const uint8_t reg1 = opr & 0xF;
    const uint8_t reg2 = opr >> 4;
    if (reg1 % 2)
        return setError(ILLEGAL_REGISTER);
    const auto dst = insn.dst();
    if (dst == M_Irr) {
        outPairReg(out, reg1, true);
    } else if (dst == M_r || dst == M_Ir) {
        outWorkReg(out, reg2, dst == M_Ir);
    }
    out.comma();
    const auto src = insn.src();
    if (src == M_Irr) {
        outPairReg(out, reg1, true);
    } else if (src == M_r || src == M_Ir) {
        outWorkReg(out, reg2, src == M_Ir);
    }
    return setError(insn);
}

Error DisZ8::decodeInOpCode(DisMemory &memory, InsnZ8 &insn, StrBuffer &out) {
    const auto dst = insn.dst();
    const auto src = insn.src();
    if (dst == M_r) {
        outWorkReg(out, insn.opCode() >> 4);
    } else {
        if (decodeOperand(memory, insn, out, dst))
            return getError();
    }
    if (src == M_NONE)
        return setOK();
    out.comma();
    if (src == M_r) {
        outWorkReg(out, insn.opCode() >> 4);
        return setOK();
    }
    if (src == M_R) {
        outRegAddr(out, insn.readByte(memory));
        return setError(insn);
    }
    return decodeOperand(memory, insn, out, src);
}

Error DisZ8::decodeTwoOperands(DisMemory &memory, InsnZ8 &insn, StrBuffer &out) {
    const auto opr1 = insn.readByte(memory);
    if (setError(insn))
        return getError();
    const auto dst = insn.dst();
    const auto src = insn.src();
    if (src == M_Ir && insn.ext() == M_RA) {
        outWorkReg(out, opr1 & 0xF).comma();
        outWorkReg(out, opr1 >> 4, true).comma();
        return decodeRelative(memory, insn, out);
    }
    if (dst == M_RR && src == M_IML) {
        const auto val16 = insn.readUint16(memory);
        if (opr1 % 2)
            return setError(ILLEGAL_REGISTER);
        outPairAddr(out, opr1).comma().letter('#');
        outHex(out, val16, 16);
        return setError(insn);
    }
    if (dst == M_r || dst == M_Ir) {
        outWorkReg(out, opr1 >> 4, dst == M_Ir).comma();
        outWorkReg(out, opr1 & 0xF, src == M_Ir);
        return setOK();
    }
    const auto opr2 = insn.readByte(memory);
    if (setError(insn))
        return getError();
    const auto dstFirst = insn.dstFirst();
    const auto dstReg = dstFirst ? opr1 : opr2;
    const auto srcReg = dstFirst ? opr2 : opr1;
    if (dst == M_RR) {
        if (dstReg % 2)
            return setError(ILLEGAL_REGISTER);
        outPairAddr(out, dstReg);
    } else {
        outRegAddr(out, dstReg, dst == M_IR);
    }
    out.comma();
    if (src == M_IM) {
        outHex(out.letter('#'), srcReg, 8);
        return setOK();
    }
    if (src == M_RR) {
        if (srcReg % 2)
            return setError(ILLEGAL_REGISTER);
        outPairAddr(out, srcReg);
    } else {
        outRegAddr(out, srcReg, src == M_IR);
    }
    return setOK();
}

Error DisZ8::decodePostByte(DisMemory &memory, InsnZ8 &insn, StrBuffer &out) {
    const auto dst = insn.dst();
    const auto src = insn.src();
    const auto post = insn.post();
    if (insn.opCode() == TableZ8::SRP) {
        const auto format = insn.postFormat();
        if (format == PF2_0 && (post & ~0xF0) != 0x00)
            return setError(OPERAND_NOT_ALLOWED);
        if (format == PF2_1 && (post & ~0xF8) != 0x01)
            return setError(OPERAND_NOT_ALLOWED);
        if (format == PF2_2 && (post & ~0xF8) != 0x02)
            return setError(OPERAND_NOT_ALLOWED);
        return decodeOperand(memory, insn, out, dst);
    }
    if (dst == M_DA || src == M_DA) {  // P4: LDC, LDE
        const uint8_t regNum = post >> 4;
        if (dst == M_DA) {
            if (decodeAbsolute(memory, insn, out, ENDIAN_LITTLE))
                return getError();
        } else {
            outWorkReg(out, regNum);
        }
        out.comma();
        if (src == M_DA) {
            if (decodeAbsolute(memory, insn, out, ENDIAN_LITTLE))
                return getError();
        } else {
            outWorkReg(out, regNum);
        }
        return setOK();
    }
    if (dst == M_Irr || src == M_Irr) {  // P1: LDCxx, LDExx
        if (dst == M_Irr) {
            outPairReg(out, post & 0x0E, true);
        } else {
            outWorkReg(out, post >> 4);
        }
        out.comma();
        if (src == M_Irr) {
            outPairReg(out, post & 0x0E, true);
        } else {
            outWorkReg(out, post >> 4);
        }
        return setOK();
    }
    if (dst == M_XL || src == M_XL || dst == M_XS || src == M_XS)  // P1: LDC, LDE
        return decodeIndexed(memory, insn, out, post & ~1);
    if (dst == M_RA) {  // P1: BTJRF, BTJRT
        if (decodeRelative(memory, insn, out))
            return getError();
        out.comma();
        outWorkReg(out, post >> 4);
        outBitPos(out, post >> 1);
        return setOK();
    }
    const auto ext = insn.ext();
    if (ext == M_NONE) {  // P1: BITC, BITR, BITS
        outWorkReg(out, post >> 4);
        outBitPos(out, post >> 1);
        return setOK();
    }
    // P1: LDB, BAND, BOR, BXOR
    const uint8_t addr = insn.readByte(memory);
    if (dst == M_r) {
        outWorkReg(out, post >> 4);
    } else {
        outRegAddr(out, addr);
    }
    if (src == M_IMb)
        outBitPos(out, post >> 1);
    else {
        out.comma();
        outRegAddr(out, addr);
    }
    if (ext == M_IMb)
        outBitPos(out, post >> 1);
    else {
        out.comma();
        outWorkReg(out, post >> 4);
    }
    return setError(insn);
}

Error DisZ8::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnZ8 insn(_insn);
    const Config::opcode_t opCode = insn.readByte(memory);
    if (setError(insn))
        return getError();
    insn.setOpCode(opCode);

    if (TableZ8::TABLE.searchOpCode(insn, out, memory))
        return setError(insn);

    const auto dst = insn.dst();
    const auto src = insn.src();
    if (dst == M_NONE)
        return setOK();  // No operand
    if (insn.postFormat() != PF_NONE)
        return decodePostByte(memory, insn, out);
    if (dst == M_DA || src == M_DA) {
        if (dst == M_cc)
            outConditionCode(out, opCode);
        return decodeAbsolute(memory, insn, out);
    }
    if (dst == M_RA || src == M_RA) {
        if (dst == M_cc)
            outConditionCode(out, opCode);
        else if (dst == M_r) {
            outWorkReg(out, opCode >> 4).comma();
        }
        return decodeRelative(memory, insn, out);
    }
    if (dst == M_X || src == M_X) {
        const auto opr1 = insn.readByte(memory);
        return decodeIndexed(memory, insn, out, opr1);
    }
    if (dst == M_Irr || src == M_Irr)
        return decodeIndirectRegPair(memory, insn, out);
    if (InsnZ8::operandInOpCode(opCode))
        return decodeInOpCode(memory, insn, out);
    if (src == M_NONE)
        return decodeOperand(memory, insn, out, dst);
    return decodeTwoOperands(memory, insn, out);
}

}  // namespace z8
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
