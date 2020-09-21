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

Error DisTms9900::decodeOperand(
    DisMemory &memory, InsnTms9900 &insn, char *out, const uint8_t opr) {
    const uint8_t regno = opr & 0xf;
    const uint8_t mode = (opr >> 4) & 0x3;
    if (mode == 1 || mode == 3) *out++ = '*';
    if (mode == 2) {
        *out++ = '@';
        out = outHex(out, insn.readUint16(memory), 16);
        if (regno) {
            *out++ = '(';
            out = _regs.outRegName(out, regno);
            *out++ = ')';
        }
    } else {
        out = _regs.outRegName(out, regno);
        if (mode == 3)
            *out++ = '+';
    }
    *out = 0;
    return setError(insn);
}

Error DisTms9900::decodeImmediate(
    DisMemory& memory, InsnTms9900 &insn, char *out) {
    outHex(out, insn.readUint16(memory), 16);
    return setError(insn);
}

Error DisTms9900::decodeRelative(InsnTms9900 &insn, char *out) {
    int16_t delta = static_cast<int8_t>(insn.opCode() & 0xff);
    delta <<= 1;
    const Config::uintptr_t target = insn.address() + 2 + delta;
    outRelAddr(out, target, insn.address(), 9);
    return setOK();
}

static char *outComma(char *out) {
    out += strlen(out);
    *out++ = ',';
    return out;
}

static const char TEXT_MID[]  PROGMEM = "MID";

Error DisTms9900::decode(DisMemory &memory, Insn &_insn, char *out) {
    InsnTms9900 insn(_insn);
    Config::opcode_t opCode = insn.readUint16(memory);
    if (setError(insn)) return getError();

    insn.setOpCode(opCode);
    if (TableTms9900.searchOpCode(insn)) {
    mid:
        insn.setName_P(TEXT_MID);
        return setError(UNKNOWN_INSTRUCTION);
    }

    uint8_t count;
    switch (insn.addrMode()) {
    case IMM_MOD:
        count = insn.opCode() & 7;
        if (count == 1 || count == 2 || count == 4)
            outHex(out, count, 3);
        break;
    case INH:
        break;
    case IMM:
        return decodeImmediate(memory, insn, out);
    case REG:
        _regs.outRegName(out, opCode);
        break;
    case REG_IMM:
        out = _regs.outRegName(out, opCode);
        out = outComma(out);
        return decodeImmediate(memory, insn, out);
    case CNT_REG:
        out = _regs.outRegName(out, opCode);
        out = outComma(out);
        count = (opCode >> 4) & 0xF;
        if (count == 0) _regs.outRegName(out, 0);
        else outDec(out, count, 4);
        break;
    case DW_BIT_SRC:
        opCode = insn.readUint16(memory);
        count = (opCode >> 6) & 0xF;
        if ((opCode & 0xFC00) != 0x0000) goto mid;
        if ((opCode & 0x0030) == 0x0030) goto mid; // no auto increment mode
        if (decodeOperand(memory, insn, out, opCode)) return getError();
        out = outComma(out);
        outDec(out, count, 4);
        break;
    case SRC:
        return decodeOperand(memory, insn, out, opCode);
    case REG_SRC:
        if (decodeOperand(memory, insn, out, opCode))
            return getError();
        out = outComma(out);
        _regs.outRegName(out, opCode >> 6);
        break;
    case DW_CNT_SRC:
        opCode = insn.readUint16(memory);
        if ((opCode & 0xFC00) != 0x4000) goto mid;
        if (decodeOperand(memory, insn, out, opCode))
            return getError();
        out = outComma(out);
        count = (opCode >> 6) & 0xF;
        if (count == 0) _regs.outRegName(out, 0);
        else outDec(out, count, 4);
        break;
    case CNT_SRC:
    case XOP_SRC:
        if (decodeOperand(memory, insn, out, opCode))
            return getError();
        out = outComma(out);
        count = (opCode >> 6) & 0xF;
        if (insn.addrMode() == CNT_SRC && count == 0)
            count = 16;
        outDec(out, count, 5);
        break;
    case DW_DST_SRC:
        opCode = insn.readUint16(memory);
        if ((opCode & 0xF000) != 0x4000) goto mid;
        /* Fall-through */
    case DST_SRC:
        if (decodeOperand(memory, insn, out, opCode))
            return getError();
        out = outComma(out);
        decodeOperand(memory, insn, out, opCode >> 6);
        return getError();
    case REL:
        decodeRelative(insn, out);
        break;
    case CRU_OFF: {
        const int8_t offset = static_cast<int8_t>(opCode & 0xff);
        outDec(out, offset, -8);
        break;
    }
    default:
        return setError(INTERNAL_ERROR);
    }
    return setError(insn);
}

} // namespace tms9900
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
