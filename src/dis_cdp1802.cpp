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

#include "reg_cdp1802.h"
#include "table_cdp1802.h"

namespace libasm {
namespace cdp1802 {

using namespace reg;

static const char OPT_BOOL_USE_REGISTER[] PROGMEM = "use-register";
static const char OPT_DESC_USE_REGISTER[] PROGMEM = "use register name Rn";

DisCdp1802::DisCdp1802()
    : Disassembler(_hexFormatter, TableCdp1802::TABLE, '$', &_opt_useReg),
      _opt_useReg(
              this, &DisCdp1802::setUseRegsterName, OPT_BOOL_USE_REGISTER, OPT_DESC_USE_REGISTER) {
    reset();
}

void DisCdp1802::reset() {
    Disassembler::reset();
    setUseRegsterName(false);
}

Error DisCdp1802::setUseRegsterName(bool enable) {
    _useReg = enable;
    return OK;
}

static Config::uintptr_t page(Config::uintptr_t addr) {
    return addr & ~0xFF;
}

static Config::uintptr_t inpage(Config::uintptr_t base, uint8_t offset) {
    return page(base) | offset;
}

Error DisCdp1802::decodeOperand(
        DisMemory &memory, InsnCdp1802 &insn, StrBuffer &out, AddrMode mode) {
    const auto opCode = insn.opCode();
    switch (mode) {
    case M_REG1:
    case M_REGN:
        if (_useReg) {
            outRegName(out, RegName(opCode & 0x0F));
        } else {
            outDec(out, opCode & 0xF, 4);
        }
        return OK;
    case M_IMM8:
        outHex(out, insn.readByte(memory), 8);
        break;
    case M_IOAD:
        outHex(out, opCode & 7, 3);
        return OK;
    case M_ADDR:
        outAbsAddr(out, insn.readUint16(memory));
        break;
    case M_PAGE:
        outAbsAddr(out, inpage(insn.address() + 2, insn.readByte(memory)));
        break;
    default:
        return OK;
    }
    return setError(insn);
}

Error DisCdp1802::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnCdp1802 insn(_insn);
    auto opCode = insn.readByte(memory);
    insn.setOpCode(opCode);
    if (TableCdp1802::TABLE.isPrefix(opCode)) {
        const auto prefix = opCode;
        opCode = insn.readByte(memory);
        insn.setOpCode(opCode, prefix);
    }
    if (setError(insn))
        return getError();

    if (TableCdp1802::TABLE.searchOpCode(insn, out))
        return setError(insn);

    const auto mode1 = insn.mode1();
    if (mode1 == M_NONE)
        return OK;
    if (decodeOperand(memory, insn, out, mode1))
        return getError();
    const auto mode2 = insn.mode2();
    if (mode2 == M_NONE)
        return OK;
    out.comma();
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
