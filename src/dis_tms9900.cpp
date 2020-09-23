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

#include "dis_tms9900.h"
#include "table_tms9900.h"

namespace libasm {
namespace tms9900 {

Error DisTms9900::checkPostWord(InsnTms9900 &insn) {
    const Config::opcode_t post = insn.post();
    const uint8_t srcMode = (post >> 4 & 3);
    switch (insn.dstMode()) {
    case M_DST2:
        if ((post & 0xF000) == 0x4000) return OK;
        break;
    case M_CNT2:
        if ((post & 0xFC00) == 0x4000) return OK;
        break;
    case M_BIT2:
        // no auto increment mode.
        if ((post & 0xFC00) == 0x0000 && srcMode != 3) return OK;
        break;
    default:
        return OK;
    }
    return decodeMactoInstructionDetect(insn);
}

static const char TEXT_MID[]  PROGMEM = "MID";

Error DisTms9900::decodeMactoInstructionDetect(InsnTms9900 &insn) {
    insn.setName_P(TEXT_MID);
    return setError(UNKNOWN_INSTRUCTION);
}

Error DisTms9900::decodeModeReg(
    DisMemory &memory, InsnTms9900 &insn, char *out, uint8_t mode, uint8_t reg) {
    switch (mode &= 3) {
    case 1: case 3:
        *out++ = '*';
        /* Fall-through */
    case 0:
        out = _regs.outRegName(out, reg);
        if (mode == 3)
            *out++ = '+';
        break;
    default:
        *out++ = '@';
        out = outHex(out, insn.readUint16(memory), 16);
        if (reg & 0xF) {
            *out++ = '(';
            out = _regs.outRegName(out, reg);
            *out++ = ')';
        }
        break;
    }
    *out = 0;
    return setError(insn);
}

Error DisTms9900::decodeRelative(InsnTms9900 &insn, char *out) {
    int16_t delta = static_cast<int8_t>(insn.opCode() & 0xff);
    delta <<= 1;
    const Config::uintptr_t target = insn.address() + 2 + delta;
    outRelAddr(out, target, insn.address(), 9);
    return OK;
}

Error DisTms9900::decodeOperand(
    DisMemory &memory, InsnTms9900 &insn, char *out, AddrMode mode) {
    const Config::opcode_t opc = insn.opCode();
    const Config::opcode_t post = insn.post();
    uint8_t val8;
    switch (mode) {
    case M_IMM:
        outHex(out, insn.readUint16(memory), 16);
        return setError(insn);
    case M_REG:
        _regs.outRegName(out, opc);
        return OK;
    case M_DREG:
        _regs.outRegName(out, opc >> 6);
        return OK;
    case M_SRC2:
        if (checkPostWord(insn)) return getError();
        /* Fall-through */
    case M_SRC:
        val8 = (mode == M_SRC) ? opc : post;
        return decodeModeReg(memory, insn, out, val8 >> 4, val8);
    case M_DST2:
    case M_DST:
        val8 = ((mode == M_DST) ? opc : post) >> 6;
        return decodeModeReg(memory, insn, out, val8 >> 4, val8);
    case M_CNT2:
    case M_XOP:
    case M_CNT:
        val8 = (((mode == M_CNT2) ? post : opc) >> 6) & 0xF;
        if (mode == M_CNT2 && val8 == 0) {
            _regs.outRegName(out, 0);
            return OK;
        }
        if (mode == M_CNT && val8 == 0) val8 = 16;
        outDec(out, val8, 5);
        return OK;
    case M_BIT2:
        val8 = (post >> 6) & 0xF;
        outDec(out, val8, 4);
        return OK;
    case M_REL:
        return decodeRelative(insn, out);
    case M_SCNT:
        val8 = (opc >> 4) & 0xF;
        if (val8 == 0) _regs.outRegName(out, 0);
        else outDec(out, val8, 4);
        return OK;
    case M_CRU:
        val8 = opc & 0xFF;
        outDec(out, val8, -8);
        return OK;
    case M_RTWP:
        val8 = opc & 7;
        if (val8 == 0 || val8 == 1 || val8 == 2 || val8 == 4) {
            if (val8) outHex(out, val8, 7);
            return OK;
        }
        return decodeMactoInstructionDetect(insn);
    default:
        return OK;
    }
}

Error DisTms9900::decode(DisMemory &memory, Insn &_insn, char *out) {
    InsnTms9900 insn(_insn);
    Config::opcode_t opCode = insn.readUint16(memory);

    insn.setOpCode(opCode);
    if (TableTms9900.searchOpCode(insn))
        return decodeMactoInstructionDetect(insn);
    insn.readPost(memory);
    if (setError(insn)) return getError();

    const AddrMode src = insn.srcMode();
    if (src == M_NO) return OK;
    if (decodeOperand(memory, insn, out, src))
        return getError();
    const AddrMode dst = insn.dstMode();
    if (dst == M_NO) return OK;
    out += strlen(out);
    *out++ = ',';
    return decodeOperand(memory, insn, out, dst);
}

} // namespace tms9900
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
