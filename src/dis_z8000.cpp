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

#include "dis_z8000.h"
#include "table_z8000.h"

namespace libasm {
namespace z8000 {

void DisZ8000::outConditionCode(uint8_t num) {
    const CcName cc = _regs.decodeCcNum(num);
    _operands = _regs.outCcName(_operands, cc);
}

void DisZ8000::outImmediate(uint8_t data, AddrMode mode) {
    uint8_t val = data;
    if (mode == M_CNT)
        val = data + 1;
    if (mode == M_QCNT)
        val = (data & 2) ? 2 : 1;
    *_operands++ = '#';
    outConstant(val, 16);
}

Error DisZ8000::decodeImmediate(
    DisMemory &memory, InsnZ8000 &insn, AddrMode mode, OprSize size) {
    *_operands++ = '#';
    if (mode == M_SCNT || mode == M_NCNT) {
        uint16_t data;
        if (insn.readUint16(memory, data)) return setError(NO_MEMORY);
        if (size == SZ_BYTE && (data & 0xFF00) != 0)
            return setError(ILLEGAL_OPERAND);
        const int16_t count = (size == SZ_BYTE)
            ? static_cast<int8_t>(data)
            : static_cast<int16_t>(data);
        if (count < 0) {
            if (TableZ8000.searchOpCodeAlias(insn, memory))
                return setError(TableZ8000.getError());
            data = -count;
        }
        if (size == SZ_BYTE && data > 8)
            return setError(ILLEGAL_OPERAND);
        if (size == SZ_WORD && data > 16)
            return setError(ILLEGAL_OPERAND);
        if (size == SZ_LONG && data > 32)
            return setError(ILLEGAL_OPERAND);
        outConstant(data, 10);
        return OK;
    }
    if (mode == M_IO) {
        uint16_t data;
        if (insn.readUint16(memory, data)) return setError(NO_MEMORY);
        outConstant(data, 16);
        return OK;
    }
    if (size == SZ_BYTE) {
        uint16_t data;
        if (insn.readUint16(memory, data)) return setError(NO_MEMORY);
        outConstant(static_cast<uint8_t>(data), 16, true);
        return OK;
    }
    if (size == SZ_WORD) {
        uint16_t data;
        if (insn.readUint16(memory, data)) return setError(NO_MEMORY);
        outConstant(data, 16, true);
        return OK;
    }
    // SZ_LONG
    uint32_t data;
    if (insn.readUint32(memory, data)) return setError(NO_MEMORY);
    outConstant(data, 16, true);
    return OK;
}

Error DisZ8000::decodeFlags(uint8_t flags) {
    flags &= 0xF;
    if (flags == 0) return setError(OPCODE_HAS_NO_EFFECT);
    _operands = _regs.outFlagNames(_operands, flags);
    return OK;
}

Error DisZ8000::decodeGeneralRegister(
    uint8_t num, OprSize size, bool indirect) {
    const RegName reg = _regs.decodeRegNum(num, size);
    if (reg == REG_ILLEGAL) return setError(ILLEGAL_REGISTER);
    if (indirect) *_operands++ = '@';
    _operands = _regs.outRegName(_operands, reg);
    return OK;
}

Error DisZ8000::decodeDoubleSizedRegister(
        uint8_t num, OprSize size) {
    if (size == SZ_BYTE)
        return decodeGeneralRegister(num, SZ_WORD);
    if (size == SZ_WORD)
        return decodeGeneralRegister(num, SZ_LONG);
    if (size == SZ_LONG)
        return decodeGeneralRegister(num, SZ_QUAD);
    return setError(INTERNAL_ERROR);
}

Error DisZ8000::decodeControlRegister(uint8_t ctlNum, OprSize size) {
    const RegName reg = _regs.decodeCtlReg(ctlNum);
    if (reg == REG_ILLEGAL) return setError(ILLEGAL_REGISTER);
    if (size == SZ_BYTE && reg != REG_FLAGS) return setError(ILLEGAL_SIZE);
    if (size == SZ_WORD && reg == REG_FLAGS) return setError(ILLEGAL_SIZE);
    _operands = _regs.outRegName(_operands, reg);
    return OK;
}

Error DisZ8000::decodeBaseAddressing(
    DisMemory &memory, InsnZ8000 &insn, AddrMode mode, uint8_t num) {
    num &= 0xf;
    if (num == 0) return setError(REGISTER_NOT_ALLOWED);
    if (decodeGeneralRegister(num, SZ_ADDR)) return getError();
    *_operands++ = '(';
    if (mode == M_BA) {
        uint16_t disp;
        if (insn.readUint16(memory, disp)) return setError(NO_MEMORY);
        *_operands++ = '#';
        outConstant(disp, -16, true);
    } else { // M_BX
        decodeGeneralRegister(insn.post() >> 8, SZ_WORD);
    }
    *_operands++ = ')';
    *_operands = 0;
    return OK;
}

Error DisZ8000::decodeGenericAddressing(
    DisMemory &memory, InsnZ8000 &insn, AddrMode mode, uint8_t num) {
    num &= 0xF;
    const uint8_t addressing = insn.opCode() >> 14;
    if (addressing == 0 && num == 0 && mode == M_GENI) { // M_IM
        return decodeImmediate(memory, insn, M_IM, insn.oprSize());
    }
    if (addressing == 2 && num && (mode == M_GENI || mode == M_GEND)) { // M_R
        return decodeGeneralRegister(num, insn.oprSize());
    }
    if (addressing == 0 && num) { // M_IR
        return decodeGeneralRegister(num, SZ_ADDR, true);
    }
    if (addressing == 1) { // M_DA/M_X
        if (decodeDirectAddress(memory, insn)) return getError();
        if (num) {
            *_operands++ = '(';
            if (decodeGeneralRegister(num, SZ_WORD)) return getError();
            *_operands++ = ')';
        }
        *_operands = 0;
        return OK;
    }
    return setError(INTERNAL_ERROR);
}

Error DisZ8000::decodeDirectAddress(DisMemory &memory, InsnZ8000 &insn) {
    uint16_t addr;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    if (TableZ8000.isSegmentModel()) {
        const uint32_t seg = static_cast<uint32_t>(addr & 0x7F00) << 8;
        uint16_t off = static_cast<uint8_t>(addr);
        if (addr & 0x8000) {
            if (addr & 0x00FF) setError(ILLEGAL_OPERAND);
            if (insn.readUint16(memory, off)) return setError(NO_MEMORY);
        }
        const uint32_t linear = seg | off;
        outConstant(linear, 16, false, true, addressBits());
    } else {
        outConstant(addr, 16, false, true, addressBits());
    }
    return OK;
}

Error DisZ8000::decodeRelativeAddressing(
    DisMemory &memory, InsnZ8000 &insn, AddrMode mode) {
    int16_t delta = 0;
    if (mode == M_RA) {
        uint16_t disp;
        if (insn.readUint16(memory, disp)) return setError(NO_MEMORY);
        delta = static_cast<int16_t>(disp);
    }
    if (mode == M_RA12) {
        uint16_t ra12 = insn.opCode() & 0xFFF;
        if (ra12 & 0x800) ra12 |= 0xF000;
        delta = static_cast<int16_t>(ra12) * 2;
    }
    if (mode == M_RA8) {
        uint16_t ra8 = insn.opCode() & 0xFF;
        if (ra8 & 0x80) ra8 |= 0xFF00;
        delta = static_cast<int16_t>(ra8) * 2;
    }
    if (mode == M_RA7) {
        const uint16_t ra7 = insn.opCode() & 0x7F;
        delta = -static_cast<int16_t>(ra7) * 2;
    }
    const Config::uintptr_t base = insn.address() + insn.length();
    const Config::uintptr_t target = base + delta;
    if (mode == M_RA12 && (target % 2) != 0)
        return setError(OPERAND_NOT_ALIGNED);
    outRelativeAddr(target, insn.address(), mode == M_RA ? 16 : 13);
    return OK;
}


static uint8_t modeField(const InsnZ8000 &insn, ModeField field) {
    switch (field) {
    case MF_C0: return (insn.opCode() & 0xF);
    case MF_C4: return (insn.opCode() >> 4) & 0xF;
    case MF_C8: return (insn.opCode() >> 8) & 0xF;
    case MF_P0: return (insn.post() & 0xF);
    case MF_P4: return (insn.post() >> 4) & 0xF;
    case MF_P8: return (insn.post() >> 8) & 0xF;
    default: return 0;
    }
}

Error DisZ8000::decodeOperand(
        DisMemory &memory, InsnZ8000 &insn, AddrMode mode, ModeField field) {
    uint8_t num = modeField(insn, field);
    switch (mode) {
    case M_NO:
        return OK;
    case M_BA: case M_BX:
        return decodeBaseAddressing(memory, insn, mode, num);
    case M_CC:
        outConditionCode(num);
        return OK;
    case M_CTL:
        return decodeControlRegister(num, insn.oprSize());
    case M_FLAG:
        return decodeFlags(num);
    case M_IM8:
        outImmediate(insn.opCode(), mode);
        return OK;
    case M_IO:
        return decodeImmediate(memory, insn, mode, SZ_WORD);
    case M_IRIO:
        if (num == 0) return setError(REGISTER_NOT_ALLOWED);
        return decodeGeneralRegister(num, SZ_WORD, true);
    case M_IR:
        if (num == 0) return setError(REGISTER_NOT_ALLOWED);
        return decodeGeneralRegister(num, SZ_ADDR, true);
    case M_INTT:
        num &= 3;
        if (num == 3) return setError(OPCODE_HAS_NO_EFFECT);
        _operands = _regs.outIntrNames(_operands, num);
        return OK;
    case M_RA: case M_RA12: case M_RA8: case M_RA7:
        return decodeRelativeAddressing(memory, insn, mode);
    case M_DR:
        return decodeDoubleSizedRegister(num, insn.oprSize());
    case M_R:
        return decodeGeneralRegister(num, insn.oprSize());
    case M_WR07:
        if (num >= 8) return setError(REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case M_WR:
        return decodeGeneralRegister(num, SZ_WORD);
    case M_DA:
        return decodeDirectAddress(memory, insn);
    case M_GENA:
    case M_GEND:
    case M_GENI:
        return decodeGenericAddressing(memory, insn, mode, num);
    case M_BIT:
        if (insn.oprSize() == SZ_BYTE && num >= 8)
            return setError(ILLEGAL_BIT_NUMBER);
        /* Fall-through */
    case M_CNT:
    case M_QCNT:
        outImmediate(num, mode);
        return OK;
    case M_IM:
    case M_SCNT:
    case M_NCNT:
        return decodeImmediate(memory, insn, mode, insn.oprSize());
    case M_X:
        return decodeGenericAddressing(memory, insn, mode, num);
    default:
        return setError(UNKNOWN_INSTRUCTION);
    }
}

Error DisZ8000::checkPostWord(const InsnZ8000 &insn) {
    if (insn.hasPost()) {
        uint16_t mask;
        switch (insn.postMode()) {
        case P_0X0X: mask = 0x0F0F; break;
        case P_0X00: mask = 0x0F00; break;
        case P_0XXX: mask = 0x0FFF; break;
        default:     mask = 0x0FF0; break;
        }
        if ((insn.post() & ~mask) == insn.postVal())
            return OK;
        return setError(UNKNOWN_POSTBYTE);
    }
    return OK;
}

void DisZ8000::outComma(
    const InsnZ8000 &insn, AddrMode mode, ModeField field) {
    if (mode == M_CC && _regs.decodeCcNum(modeField(insn, field)) == CC_T)
        return;
    *_operands++ = ',';
}

Error DisZ8000::decode(DisMemory &memory, Insn &_insn) {
    InsnZ8000 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readUint16(memory, opCode)) return setError(NO_MEMORY);
    insn.setOpCode(opCode);
    if (TableZ8000.searchOpCode(insn, memory))
        return setError(TableZ8000.getError());
    if (checkPostWord(insn)) return getError();
    const AddrMode dst = insn.dstMode();
    if (dst == M_NO) return setOK();
    if (decodeOperand(memory, insn, dst, insn.dstField()))
        return getError();
    const AddrMode src = insn.srcMode();
    if (src == M_NO) return setOK();
    outComma(insn, dst, insn.dstField());
    if (decodeOperand(memory, insn, src, insn.srcField()))
        return getError();
    const AddrMode ex1 = insn.ex1Mode();
    if (ex1 == M_NO) return setOK();
    const ModeField ex1Field = (ex1 == M_CNT ? MF_P0 : MF_P8);
    outComma(insn, ex1, ex1Field);
    if (decodeOperand(memory, insn, ex1, ex1Field))
        return getError();
    const AddrMode ex2 = insn.ex2Mode();
    if (ex2 == M_NO) return setOK();
    outComma(insn, ex2, MF_P0);
    return decodeOperand(memory, insn, ex2, MF_P0);
}

} // namespace z8000
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
