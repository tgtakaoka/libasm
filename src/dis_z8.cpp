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

void DisZ8::outCcName(Config::opcode_t opCode) {
    const CcName cc = _regs.decodeCcNum(opCode >> 4);
    if (cc != CC_T) {
        _operands = _regs.outCcName(_operands, cc);
        *_operands++ = ',';
    }
}

void DisZ8::outIndexed(uint16_t base, RegName idx, AddrMode mode) {
    if (mode == M_XL) {
        outConstant(base, 16, false);
    } else if (mode == M_XS) {
        const int8_t disp = static_cast<int8_t>(base);
        if (disp > 0) *_operands++ = '+';
        outConstant(disp, 10);
    } else { // M_X
        outConstant(static_cast<uint8_t>(base), 16, false);
    }
    *_operands++ = '(';
    outWorkReg(idx, false, mode != M_X);
    *_operands++ = ')';
    *_operands = 0;
}

Error DisZ8::outWorkReg(uint8_t regNum, bool indir, bool pair) {
    const RegName reg = _regs.decodeRegNum(regNum, pair);
    if (reg == REG_UNDEF) return setError(ILLEGAL_REGISTER);
    if (indir) *_operands++ = '@';
    _operands = _regs.outRegName(_operands, reg);
    return setOK();
}

Error DisZ8::outRegAddr(uint8_t regAddr, bool indir, bool pair) {
    if (pair && (regAddr & 1) != 0) return setError(ILLEGAL_REGISTER);
    if (_preferWorkRegister && _regs.isWorkRegAlias(regAddr))
        return outWorkReg(regAddr & 0xF, indir, pair);
    if (indir) *_operands++ = '@';
    const char *label = lookup(regAddr);
    if (label) outText(label);
    else {
        if (regAddr < 16 && !indir) *_operands++ = '>';
        outConstant(regAddr, 16, false);
    }
    return setOK();
}

Error DisZ8::outBitPos(uint8_t bitPos) {
    bitPos &= 7;
    *_operands++ = ',';
    *_operands++ = '#';
    outConstant(bitPos);
    return setOK();
}

Error DisZ8::decodeOperand(DisMemory &memory, InsnZ8 &insn, AddrMode mode) {
    uint8_t val;
    const PostFormat post = insn.postFormat();
    if (post) {
        val = insn.post();
    } else if (insn.readByte(memory, val))
        return setError(NO_MEMORY);
    if (mode == M_R || mode == M_IR || mode == M_RR || mode == M_IRR) {
        const bool pair = (mode == M_RR || mode == M_IRR);
        if (pair && (val & 1) != 0) return setError(ILLEGAL_REGISTER);
        const bool indir = (mode == M_IR || mode == M_IRR);
        return outRegAddr(val, indir, pair);
    }
    if (mode == M_IM) {
        if (post == P2_0 || post == P2_1 || post == P2_2) val &= ~3;
        *_operands++ = '#';
        outConstant(val, 16);
        return setOK();
    }
    return setError(ILLEGAL_OPERAND);
}

Error DisZ8::decodeAbsolute(
    DisMemory &memory, InsnZ8 &insn, Endian endian) {
    Config::uintptr_t addr = 0;
    if (endian == ENDIAN_BIG && insn.readUint16Be(memory, addr))
        return setError(NO_MEMORY);
    if (endian == ENDIAN_LITTLE && insn.readUint16Le(memory, addr))
        return setError(NO_MEMORY);
    outConstant(addr, 16, false);
    return setOK();
}

Error DisZ8::decodeRelative(DisMemory &memory, InsnZ8 &insn) {
    uint8_t disp;
    if (insn.readByte(memory, disp)) return setError(NO_MEMORY);
    const Config::uintptr_t target =
        insn.address() + insn.length() + static_cast<int8_t>(disp);
    outRelativeAddr(target, insn.address(), 8);
    return setOK();
}

Error DisZ8::decodeIndexed(
    DisMemory &memory, InsnZ8 &insn, uint8_t opr1) {
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    uint16_t base16;
    if (dst == M_XL || src == M_XL) {
        if (insn.readUint16Le(memory, base16)) return setError(NO_MEMORY);
    }  else {
        uint8_t base8;
        if (insn.readByte(memory, base8)) return setError(NO_MEMORY);
        base16 = base8;
    }
    const RegName idx = _regs.decodeRegNum(opr1 & 0xF);
    if (dst == M_r) {
        outWorkReg(opr1 >> 4);
    } else {
        outIndexed(base16, idx, dst);
    }
    *_operands++ = ',';
    if (src == M_r) {
        outWorkReg(opr1 >> 4);
    } else {
        outIndexed(base16, idx, src);
    }
    return setOK();
}

Error DisZ8::decodeIndirectRegPair(DisMemory &memory, InsnZ8 &insn) {
    uint8_t opr;
    if (insn.readByte(memory, opr)) return setError(NO_MEMORY);
    const AddrMode dst = insn.dstMode();
    if (dst == M_Irr) {
        if (outWorkReg(opr & 0xF, true, true)) return getError();
    } else if (dst == M_r || dst == M_Ir) {
        if (outWorkReg(opr >> 4, dst == M_Ir)) return getError();
    } else return setError(INTERNAL_ERROR);
    *_operands++ = ',';
    const AddrMode src = insn.srcMode();
    if (src == M_Irr) {
        if (outWorkReg(opr & 0xF, true, true)) return getError();
    } else if (src == M_r || src == M_Ir) {
        if (outWorkReg(opr >> 4, src == M_Ir)) return getError();
    } else return setError(INTERNAL_ERROR);
    return setOK();
}

Error DisZ8::decodeInOpCode(DisMemory &memory, InsnZ8 &insn) {
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    if (dst == M_r) {
        outWorkReg(insn.opCode() >> 4);
    } else {
        if (decodeOperand(memory, insn, dst)) return getError();
    }
    if (src == M_NO) return setOK();
    *_operands++ = ',';
    if (src == M_r)
        return outWorkReg(insn.opCode() >> 4);
    if (src == M_R) {
        uint8_t val = 0;
        if (insn.readByte(memory, val)) setError(NO_MEMORY);
        return outRegAddr(val);
    }
    return decodeOperand(memory, insn, src);
}

Error DisZ8::decodeTwoOperands(DisMemory &memory, InsnZ8 &insn) {
    uint8_t opr1;
    if (insn.readByte(memory, opr1)) return setError(NO_MEMORY);
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    if (src == M_Ir && insn.extMode() == M_RA) {
        if (outWorkReg(opr1 & 0xF)) return getError();
        *_operands++ = ',';
        if (outWorkReg(opr1 >> 4, true)) return getError();
        *_operands++ = ',';
        return decodeRelative(memory, insn);
    }
    if (dst == M_RR && src == M_IML) {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        if (outRegAddr(opr1, false, true)) return getError();
        *_operands++ = ',';
        *_operands++ = '#';
        outConstant(val16);
        return setOK();
    }
    if (dst == M_r || dst == M_Ir) {
        outWorkReg(opr1 >> 4, dst == M_Ir);
        *_operands++ = ',';
        return outWorkReg(opr1 & 0xF, src == M_Ir);
    }
    uint8_t opr2;
    if (insn.readByte(memory, opr2)) return setError(NO_MEMORY);
    const bool dstSrc = ((dst == M_R || dst == M_IR) && src == M_IM);
    const uint8_t dstReg = dstSrc ? opr1 : opr2;
    const uint8_t srcReg = dstSrc ? opr2 : opr1;
    if (outRegAddr(dstReg, dst == M_IR, dst == M_RR)) return getError();
    *_operands++ = ',';
    if (src == M_IM) {
        *_operands++ = '#';
        outConstant(srcReg, 16);
        return setOK();
    }
    return setError(outRegAddr(srcReg, src == M_IR, src == M_RR));
}

Error DisZ8::decodePostByte(DisMemory &memory, InsnZ8 &insn) {
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    const uint8_t post = insn.post();
    if (dst == M_IM && src == M_NO) { // P2: SRP, SPR0, SRP1
        // TODO: Warning
        return decodeOperand(memory, insn, dst);
    }
    if (dst == M_DA || src == M_DA) { // P4: LDC, LDE
        const uint8_t regNum = post >> 4;
        const Error error = dst == M_DA
            ? decodeAbsolute(memory, insn, ENDIAN_LITTLE) : outWorkReg(regNum);
        if (error) return getError();
        *_operands++ = ',';
        return src == M_DA
            ? decodeAbsolute(memory, insn, ENDIAN_LITTLE) : outWorkReg(regNum);
    }
    if (dst == M_Irr || src == M_Irr) { // P1: LDCxx, LDExx
        if (dst == M_Irr) outWorkReg(post & 0x0E, true, true);
        if (dst == M_r) outWorkReg(post >> 4);
        *_operands++ = ',';
        if (src == M_Irr) return outWorkReg(post & 0x0E, true, true);
        return outWorkReg(post >> 4);
    }
    if (dst == M_XL || src == M_XL || dst == M_XS || src == M_XS) // P1: LDC, LDE
        return decodeIndexed(memory,  insn, post & ~1);
    if (dst == M_RA) {          // P1: BTJRF, BTJRT
        if (decodeRelative(memory, insn)) return getError();
        *_operands++ = ',';
        outWorkReg(post >> 4);
        return outBitPos(post >> 1);
    }
    const AddrMode ext = insn.extMode();
    if (ext == M_NO) {          // P1: BITC, BITR, BITS
        outWorkReg(post >> 4);
        outBitPos(post >> 1);
        return setOK();
    }
    // P1: LDB, BAND, BOR, BXOR
    uint8_t regAddr;
    if (insn.readByte(memory, regAddr)) return setError(NO_MEMORY);
    if (dst == M_r) outWorkReg(post >> 4);
    else outRegAddr(regAddr);
    if (src == M_IMb) outBitPos(post >> 1);
    else {
        *_operands++ = ',';
        outRegAddr(regAddr);
    }
    if (ext == M_IMb) outBitPos(post >> 1);
    else {
        *_operands++ = ',';
        outWorkReg(post >> 4);
    }
    return setOK();
}

Error DisZ8::decode(DisMemory &memory, Insn &_insn) {
    InsnZ8 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
    insn.setOpCode(opCode);
    if (TableZ8.searchOpCode(insn, memory))
        return setError(TableZ8.getError());
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    if (dst == M_NO)
        return setOK();         // No operand
    if (insn.postFormat() != P0)
        return decodePostByte(memory, insn);
    if (dst == M_DA || src == M_DA) {
        if (dst == M_cc) outCcName(insn.opCode());
        return decodeAbsolute(memory, insn);
    }
    if (dst == M_RA || src == M_RA) {
        if (dst == M_cc) outCcName(insn.opCode());
        else if (dst == M_r)  {
            outWorkReg(insn.opCode() >> 4);
            *_operands++ = ',';
        }
        return decodeRelative(memory, insn);
    }
    if (dst == M_X || src == M_X) {
        uint8_t opr1;
        if (insn.readByte(memory, opr1)) return setError(NO_MEMORY);
        return decodeIndexed(memory, insn, opr1);
    }
    if (dst == M_Irr || src == M_Irr)
        return decodeIndirectRegPair(memory, insn);
    if (InsnZ8::operandInOpCode(opCode))
        return decodeInOpCode(memory, insn);
    if (src == M_NO)
        return decodeOperand(memory, insn, dst);
    return decodeTwoOperands(memory, insn);
}

} // namespace z8
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
