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

Error DisCdp1802::decode(DisMemory &memory, Insn &_insn, char *out) {
    InsnCdp1802 insn(_insn);
    Config::opcode_t opCode = insn.readByte(memory);
    if (setError(insn))
        return getError();
    insn.setOpCode(opCode);

    if (TableCdp1802.searchOpCode(insn))
        return setError(TableCdp1802.getError());

    switch (insn.addrMode()) {
    case REG1:
    case REGN:
        outDec(out, opCode & 0xF, 4);
        break;
    case IMM8:
        outHex(out, insn.readByte(memory), 8);
        break;
    case IOAD:
        outHex(out, opCode & 7, 3);
        break;
    case ADDR:
        outAbsAddr(out, insn.readUint16(memory));
        break;
    case PAGE: {
        const uint8_t val = insn.readByte(memory);
        const Config::uintptr_t addr = ((insn.address() + 2) & ~0xFF) | val;
        outAbsAddr(out, addr);
        break;
    }
    default:
        break;
    }
    return setError(insn);
}

}  // namespace cdp1802
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
