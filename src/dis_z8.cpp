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

void DisZ8::outIndexed(uint8_t disp, RegName idx) {
    outConstant(disp, 16, false);
    *_operands++ = '(';
    decodeWorkReg(idx);
    *_operands++ = ')';
    *_operands = 0;
}

Error DisZ8::decodeWorkReg(uint8_t regNum, bool indir, bool pair) {
    const RegName reg = _regs.decodeRegNum(regNum, pair);
    if (reg == REG_UNDEF) return setError(ILLEGAL_REGISTER);
    if (indir) *_operands++ = '@';
    _operands = _regs.outRegName(_operands, reg);
    return setOK();
}

Error DisZ8::decodeRegister(uint8_t regAddr, bool indir, bool pair) {
    if (pair && (regAddr & 1) != 0) return setError(ILLEGAL_REGISTER);
    if (_preferWorkRegister && (regAddr & 0xF0) == 0xE0)
        return decodeWorkReg(regAddr & 0xF, indir, pair);
    if (indir) *_operands++ = '@';
    const char *label = lookup(regAddr);
    if (label) outText(label);
    else {
        if (regAddr < 16 && !indir) *_operands++ = '>';
        outConstant(regAddr, 16, false);
    }
    return setOK();
}

Error DisZ8::decodeOperand(DisMemory &memory, InsnZ8 &insn, AddrMode mode) {
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    if (mode == M_R || mode == M_IR || mode == M_IRR) {
        const bool pair = (mode == M_IRR);
        if (pair && (val & 1) != 0) return setError(ILLEGAL_REGISTER);
        const bool indir = (mode == M_IR || mode == M_IRR);
        return decodeRegister(val, indir, pair);
    }
    if (mode == M_IM) {
        *_operands++ = '#';
        outConstant(val, 16);
        return setOK();
    }
    return setError(ILLEGAL_OPERAND);
}

Error DisZ8::decodeAbsolute(DisMemory &memory, InsnZ8 &insn) {
    if (insn.dstMode() == M_cc)
        outCcName(insn.opCode());
    Config::uintptr_t addr;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    outConstant(addr, 16, false);
    return setOK();
}

Error DisZ8::decodeRelative(DisMemory &memory, InsnZ8 &insn) {
    if (insn.dstMode() == M_cc)
        outCcName(insn.opCode());
    if (insn.dstMode() == M_r) {
        decodeWorkReg(insn.opCode() >> 4);
        *_operands++ = ',';
    }
    uint8_t disp;
    if (insn.readByte(memory, disp)) return setError(NO_MEMORY);
    const Config::uintptr_t target =
        insn.address() + insn.length() + static_cast<int8_t>(disp);
    outRelativeAddr(target, insn.address());
    return setOK();
}

Error DisZ8::decodeIndexed(DisMemory &memory, InsnZ8 &insn) {
    uint8_t opr1, base;
    if (insn.readByte(memory, opr1) || insn.readByte(memory, base))
        return setError(NO_MEMORY);
    const RegName idx = _regs.decodeRegNum(opr1 & 0xF);
    const AddrMode dst = insn.dstMode();
    if (dst == M_X) {
        outIndexed(base, idx);
    } else if (dst == M_r) {
        decodeWorkReg(opr1 >> 4);
    } else return setError(INTERNAL_ERROR);
    *_operands++ = ',';
    const AddrMode src = insn.srcMode();
    if (src == M_X) {
        outIndexed(base, idx);
    } else if (src == M_r) {
        decodeWorkReg(opr1 >> 4);
    } else return setError(INTERNAL_ERROR);
    return setOK();
}

Error DisZ8::decodeIndirectRegPair(DisMemory &memory, InsnZ8 &insn) {
    uint8_t opr;
    if (insn.readByte(memory, opr)) return setError(NO_MEMORY);
    const AddrMode dst = insn.dstMode();
    if (dst == M_Irr) {
        if (decodeWorkReg(opr & 0xF, true, true)) return getError();
    } else if (dst == M_r || dst == M_Ir) {
        if (decodeWorkReg(opr >> 4, dst == M_Ir)) return getError();
    } else return setError(INTERNAL_ERROR);
    *_operands++ = ',';
    const AddrMode src = insn.srcMode();
    if (src == M_Irr) {
        if (decodeWorkReg(opr & 0xF, true, true)) return getError();
    } else if (src == M_r || src == M_Ir) {
        if (decodeWorkReg(opr >> 4, src == M_Ir)) return getError();
    } else return setError(INTERNAL_ERROR);
    return setOK();
}

Error DisZ8::decodeInOpCode(DisMemory &memory, InsnZ8 &insn) {
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    if (dst == M_r) {
        decodeWorkReg(insn.opCode() >> 4);
    } else {
        if (decodeOperand(memory, insn, dst)) return getError();
    }
    if (src == M_NO) return setOK();
    *_operands++ = ',';
    if (src == M_r)
        return decodeWorkReg(insn.opCode() >> 4);
    if (src == M_R) {
        uint8_t val = 0;
        if (insn.readByte(memory, val)) setError(NO_MEMORY);
        return decodeRegister(val);
    }
    return decodeOperand(memory, insn, src);
}

Error DisZ8::decodeTwoOperands(DisMemory &memory, InsnZ8 &insn) {
    uint8_t opr1;
    if (insn.readByte(memory, opr1)) return setError(NO_MEMORY);
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    if (dst == M_r || dst == M_Ir) {
        decodeWorkReg(opr1 >> 4, dst == M_Ir);
        *_operands++ = ',';
        return decodeWorkReg(opr1 & 0xF, src == M_Ir);
    }
    uint8_t opr2;
    if (insn.readByte(memory, opr2)) return setError(NO_MEMORY);
    decodeRegister(src == M_IM ? opr1 : opr2, dst == M_IR);
    *_operands++ = ',';
    if (src == M_IM) {
        *_operands++ = '#';
        outConstant(opr2, 16);
        return setOK();
    }
    return decodeRegister(opr1, src == M_IR);
}

Error DisZ8::decode(DisMemory &memory, Insn &_insn) {
    InsnZ8 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
    insn.setOpCode(opCode);
    if (TableZ8.searchOpCode(insn))
        return setError(TableZ8.getError());
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    if (dst == M_NO)
        return setOK();         // No operand
    if (dst == M_DA || src == M_DA)
        return decodeAbsolute(memory, insn);
    if (dst == M_RA || src == M_RA)
        return decodeRelative(memory, insn);
    if (dst == M_X || src == M_X)
        return decodeIndexed(memory, insn);
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
