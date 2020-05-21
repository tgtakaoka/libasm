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

Error DisCdp1802::outDecimal(uint8_t val) {
    const char *label = lookup(val);
    if (label) {
        outText(label);
    } else {
        outConstant(val, 10);
    }
    return setOK();
}

Error DisCdp1802::outImm8(uint8_t val) {
    const char *label = lookup(val);
    if (label) {
        outText(label);
    } else {
        outConstant(val, 16);
    }
    return setOK();
}

Error DisCdp1802::outAddr(Config::uintptr_t addr) {
    const char *label = lookup(addr);
    if (label) {
        outText(label);
    } else {
        outConstant(addr, 16, false);
    }
    return setOK();
}

Error DisCdp1802::decode(
    DisMemory &memory, Insn &_insn) {
    InsnCdp1802 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
    insn.setOpCode(opCode);
    if (TableCdp1802.searchOpCode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    uint8_t val;
    Config::uintptr_t addr;
    switch (insn.addrMode()) {
    case REGN:
    case REG1:
        outDecimal(insn.opCode() & 0xF);
        break;
    case IMM8:
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        return outImm8(val);
    case IOAD:
        outDecimal(insn.opCode() & 7);
        break;
    case ADDR:
        if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
        return outAddr(addr);
    case PAGE:
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        addr = (insn.address() + 2) & ~0xFF;
        return outAddr(addr | val);
    case IMPL:
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    return setOK();
}

} // namespace cdp1802
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
