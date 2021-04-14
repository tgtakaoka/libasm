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

#include "dis_cdp1802.h"

#include "table_cdp1802.h"

namespace libasm {
namespace cdp1802 {

static Config::uintptr_t pageAddr(Config::uintptr_t base, uint8_t val) {
    return (base & ~0xFF) | val;
}

Error DisCdp1802::decodeOperand(DisMemory &memory, InsnCdp1802 &insn, char *out, AddrMode mode) {
    const Config::opcode_t opCode = insn.opCode();
    switch (mode) {
    case REG1:
    case REGN:
        outDec(out, opCode & 0xF, 4);
        return OK;
    case IMM8:
        outHex(out, insn.readByte(memory), 8);
        break;
    case IOAD:
        outHex(out, opCode & 7, 3);
        return OK;
    case ADDR:
        outAbsAddr(out, insn.readUint16(memory));
        break;
    case PAGE:
        outAbsAddr(out, pageAddr(insn.address() + 2, insn.readByte(memory)));
        break;
    default:
        return OK;
    }
    return setError(insn);
}

Error DisCdp1802::decode(DisMemory &memory, Insn &_insn, char *out) {
    InsnCdp1802 insn(_insn);
    Config::opcode_t opCode = insn.readByte(memory);
    insn.setOpCode(opCode);
    if (TableCdp1802.isPrefix(opCode)) {
        const Config::opcode_t prefix = opCode;
        opCode = insn.readByte(memory);
        insn.setOpCode(opCode, prefix);
    }
    if (setError(insn))
        return getError();

    if (TableCdp1802.searchOpCode(insn))
        return setError(TableCdp1802.getError());

    const AddrMode mode1 = insn.mode1();
    if (mode1 == NONE)
        return OK;
    if (decodeOperand(memory, insn, out, mode1))
        return getError();
    const AddrMode mode2 = insn.mode2();
    if (mode2 == NONE)
        return OK;
    out += strlen(out);
    *out++ = ',';
    return decodeOperand(memory, insn, out, mode2);
}

}  // namespace cdp1802
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
