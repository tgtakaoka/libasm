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
#include "table_z8.h"

namespace libasm {
namespace z8 {

char *DisZ8::outCcName(char *out, Config::opcode_t opCode) {
    const CcName cc = _regs.decodeCcNum(opCode >> 4);
    if (cc != CC_T) {
        out = _regs.outCcName(out, cc);
        *out++ = ',';
    }
    return out;
}

char *DisZ8::outWorkReg(char *out, uint8_t num, bool indir) {
    const RegName reg = _regs.decodeRegNum(num);
    if (indir) *out++ = '@';
    return _regs.outRegName(out, reg);
}

char *DisZ8::outPairReg(char *out, uint8_t num, bool indir) {
    const RegName reg = _regs.decodePairRegNum(num);
    if (indir) *out++ = '@';
    return _regs.outRegName(out, reg);
}

char *DisZ8::outRegAddr(char *out, uint8_t addr, bool indir) {
    if (_preferWorkRegister && _regs.isWorkRegAlias(addr))
        return outWorkReg(out, addr & 0xF, indir);
    if (indir) *out++ = '@';
    return outAbsAddr(out, addr, 8, PSTR(">"), addr < 16 && !indir);
}

char *DisZ8::outPairAddr(char *out, uint8_t addr, bool indir) {
    if (_preferWorkRegister && _regs.isWorkRegAlias(addr))
        return outPairReg(out, addr & 0xF, indir);
    if (indir) *out++ = '@';
    return outAbsAddr(out, addr, 8, PSTR(">"), addr < 16 && !indir);
}

char *DisZ8::outBitPos(char *out, uint8_t bitPos) {
    bitPos &= 7;
    *out++ = ',';
    *out++ = '#';
    return outHex(out, bitPos, 3);
}

Error DisZ8::decodeOperand(
    DisMemory &memory, InsnZ8 &insn, char *out, AddrMode mode) {
    const PostFormat post = insn.postFormat();
    uint8_t val = post ? insn.post() : insn.readByte(memory);
    if (setError(insn)) return getError();
    if (mode == M_R || mode == M_IR || mode == M_RR || mode == M_IRR) {
        const bool pair = (mode == M_RR || mode == M_IRR);
        const bool indir = (mode == M_IR || mode == M_IRR);
        if (pair) {
            if (val % 2) return setError(ILLEGAL_REGISTER);
            outPairAddr(out, val, indir);
        } else {
            outRegAddr(out, val, indir);
        }
    } if (mode == M_IM) {
        if (post == P2_0 || post == P2_1 || post == P2_2) val &= ~3;
        *out++ = '#';
        outHex(out, val, 8);
    }
    return setOK();
}

Error DisZ8::decodeAbsolute(
    DisMemory &memory, InsnZ8 &insn, char *out, Endian endian) {
    const Config::uintptr_t addr = (endian == ENDIAN_LITTLE)
        ? insn.readUint16Le(memory) : insn.readUint16(memory);
    outAbsAddr(out, addr);
    return setError(insn);
}

Error DisZ8::decodeRelative(DisMemory &memory, InsnZ8 &insn, char *out) {
    const int8_t disp = static_cast<int8_t>(insn.readByte(memory));
    const Config::uintptr_t target = insn.address() + insn.length() + disp;
    outRelAddr(out, target, insn.address(), 8);
    return setError(insn);
}

static char *outComma(char *out) {
    out += strlen(out);
    *out++ = ',';
    return out;
}

char *DisZ8::outIndexed(char *out, uint16_t base, RegName idx, AddrMode mode) {
    if (mode == M_XL) {
        out = outAbsAddr(out, base);
    } else if (mode == M_XS) {
        const int8_t disp = static_cast<int8_t>(base);
        if (disp > 0) *out++ = '+';
        out = outDec(out, disp, -8);
    } else { // M_X
        out = outHex(out, base, 8);
    }
    *out++ = '(';
    out = (mode == M_X) ? outWorkReg(out, idx) : outPairReg(out, idx);
    *out++ = ')';
    *out = 0;
    return out;
}

Error DisZ8::decodeIndexed(
    DisMemory &memory, InsnZ8 &insn, char *out, uint8_t opr1) {
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    const bool pair = !(dst == M_X || src == M_X);
    uint16_t base16;
    if (dst == M_XL || src == M_XL) {
        base16 = insn.readUint16Le(memory);
    }  else {
        base16 = insn.readByte(memory);
    }
    const RegName idx = pair ? _regs.decodePairRegNum(opr1)
        : _regs.decodeRegNum(opr1);
    if (idx == REG_UNDEF) return setError(ILLEGAL_REGISTER);
    out = (dst == M_r) ? outWorkReg(out, opr1 >> 4)
        : outIndexed(out, base16, idx, dst);
    out = outComma(out);
    out = (src == M_r) ? outWorkReg(out, opr1 >> 4)
        : outIndexed(out, base16, idx, src);
    return setError(insn);
}

Error DisZ8::decodeIndirectRegPair(
    DisMemory &memory, InsnZ8 &insn, char *out) {
    const uint8_t opr = insn.readByte(memory);
    const uint8_t reg1 = opr & 0xF;
    const uint8_t reg2 = opr >> 4;
    if (reg1 % 2) return setError(ILLEGAL_REGISTER);
    const AddrMode dst = insn.dstMode();
    if (dst == M_Irr) {
        out = outPairReg(out, reg1, true);
    } else if (dst == M_r || dst == M_Ir) {
        out = outWorkReg(out, reg2, dst == M_Ir);
    }
    out = outComma(out);
    const AddrMode src = insn.srcMode();
    if (src == M_Irr) {
        outPairReg(out, reg1, true);
    } else if (src == M_r || src == M_Ir) {
        outWorkReg(out, reg2, src == M_Ir);
    }
    return setError(insn);
}

Error DisZ8::decodeInOpCode(DisMemory &memory, InsnZ8 &insn, char *out) {
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    if (dst == M_r) {
        out = outWorkReg(out, insn.opCode() >> 4);
    } else {
        if (decodeOperand(memory, insn, out, dst)) return getError();
    }
    if (src == M_NO) return setOK();
    out = outComma(out);
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

Error DisZ8::decodeTwoOperands(DisMemory &memory, InsnZ8 &insn, char *out) {
    const uint8_t opr1 = insn.readByte(memory);
    if (setError(insn)) return getError();
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    if (src == M_Ir && insn.extMode() == M_RA) {
        out = outWorkReg(out, opr1 & 0xF);
        *out++ = ',';
        out = outWorkReg(out, opr1 >> 4, true);
        *out++ = ',';
        return decodeRelative(memory, insn, out);
    }
    if (dst == M_RR && src == M_IML) {
        const uint16_t val16 = insn.readUint16(memory);
        if (opr1 % 2) return setError(ILLEGAL_REGISTER);
        out = outPairAddr(out, opr1);
        *out++ = ',';
        *out++ = '#';
        outHex(out, val16, 16);
        return setError(insn);
    }
    if (dst == M_r || dst == M_Ir) {
        out = outWorkReg(out, opr1 >> 4, dst == M_Ir);
        *out++ = ',';
        outWorkReg(out, opr1 & 0xF, src == M_Ir);
        return setOK();
    }
    const uint8_t opr2 = insn.readByte(memory);
    if (setError(insn)) return getError();
    const bool dstSrc = ((dst == M_R || dst == M_IR) && src == M_IM);
    const uint8_t dstReg = dstSrc ? opr1 : opr2;
    const uint8_t srcReg = dstSrc ? opr2 : opr1;
    if (dst == M_RR) {
        if (dstReg % 2) return setError(ILLEGAL_REGISTER);
        out = outPairAddr(out, dstReg);
    } else {
        out = outRegAddr(out, dstReg, dst == M_IR);
    }
    *out++ = ',';
    if (src == M_IM) {
        *out++ = '#';
        outHex(out, srcReg, 8);
        return setOK();
    }
    if (src == M_RR) {
        if (srcReg % 2) return setError(ILLEGAL_REGISTER);
        outPairAddr(out, srcReg);
    } else {
        outRegAddr(out, srcReg, src == M_IR);
    }
    return setOK();
}

Error DisZ8::decodePostByte(DisMemory &memory, InsnZ8 &insn, char *out) {
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    const uint8_t post = insn.post();
    if (dst == M_IM && src == M_NO) { // P2: SRP, SPR0, SRP1
        // TODO: Warning
        return decodeOperand(memory, insn, out, dst);
    }
    if (dst == M_DA || src == M_DA) { // P4: LDC, LDE
        const uint8_t regNum = post >> 4;
        if (dst == M_DA) {
            if (decodeAbsolute(memory, insn, out, ENDIAN_LITTLE))
                return getError();
        } else {
            out = outWorkReg(out, regNum);
        }
        out = outComma(out);
        if (src == M_DA) {
            if (decodeAbsolute(memory, insn, out, ENDIAN_LITTLE))
                return getError();
        } else {
            outWorkReg(out, regNum);
        }
        return setOK();
    }
    if (dst == M_Irr || src == M_Irr) { // P1: LDCxx, LDExx
        out = (dst == M_Irr) ? outPairReg(out, post & 0x0E, true)
            : outWorkReg(out, post >> 4);
        out = outComma(out);
        out = (src == M_Irr) ? outPairReg(out, post & 0x0E, true)
            : outWorkReg(out, post >> 4);
        return setOK();
    }
    if (dst == M_XL || src == M_XL || dst == M_XS || src == M_XS) // P1: LDC, LDE
        return decodeIndexed(memory,  insn, out,  post & ~1);
    if (dst == M_RA) {          // P1: BTJRF, BTJRT
        if (decodeRelative(memory, insn, out)) return getError();
        out = outComma(out);
        out = outWorkReg(out, post >> 4);
        outBitPos(out, post >> 1);
        return setOK();
    }
    const AddrMode ext = insn.extMode();
    if (ext == M_NO) {          // P1: BITC, BITR, BITS
        out = outWorkReg(out, post >> 4);
        outBitPos(out, post >> 1);
        return setOK();
    }
    // P1: LDB, BAND, BOR, BXOR
    const uint8_t addr = insn.readByte(memory);
    out = (dst == M_r) ? outWorkReg(out, post >> 4) : outRegAddr(out, addr);
    if (src == M_IMb) out = outBitPos(out, post >> 1);
    else {
        *out++ = ',';
        out = outRegAddr(out, addr);
    }
    if (ext == M_IMb) out = outBitPos(out, post >> 1);
    else {
        *out++ = ',';
        outWorkReg(out, post >> 4);
    }
    return setError(insn);
}

Error DisZ8::decode(DisMemory &memory, Insn &_insn, char *out) {
    InsnZ8 insn(_insn);
    const Config::opcode_t opCode = insn.readByte(memory);
    if (setError(insn)) return getError();
    insn.setOpCode(opCode);

    if (TableZ8.searchOpCode(insn, memory))
        return setError(TableZ8.getError());

    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    if (dst == M_NO)
        return setOK();         // No operand
    if (insn.postFormat() != P0)
        return decodePostByte(memory, insn, out);
    if (dst == M_DA || src == M_DA) {
        if (dst == M_cc) out = outCcName(out, opCode);
        return decodeAbsolute(memory, insn, out);
    }
    if (dst == M_RA || src == M_RA) {
        if (dst == M_cc) out = outCcName(out, opCode);
        else if (dst == M_r)  {
            out = outWorkReg(out, opCode >> 4);
            *out++ = ',';
        }
        return decodeRelative(memory, insn, out);
    }
    if (dst == M_X || src == M_X) {
        const uint8_t opr1 = insn.readByte(memory);
        return decodeIndexed(memory, insn, out, opr1);
    }
    if (dst == M_Irr || src == M_Irr)
        return decodeIndirectRegPair(memory, insn, out);
    if (InsnZ8::operandInOpCode(opCode))
        return decodeInOpCode(memory, insn, out);
    if (src == M_NO)
        return decodeOperand(memory, insn, out, dst);
    return decodeTwoOperands(memory, insn, out);
}

} // namespace z8
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
