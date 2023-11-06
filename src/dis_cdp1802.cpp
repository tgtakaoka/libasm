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
#include "text_common.h"

namespace libasm {
namespace cdp1802 {

using namespace reg;
using namespace text::common;

namespace {

const char OPT_BOOL_USE_REGISTER[] PROGMEM = "use-register";
const char OPT_DESC_USE_REGISTER[] PROGMEM = "use register name Rn";

}  // namespace

const ValueFormatter::Plugins &DisCdp1802::defaultPlugins() {
    static const struct final : ValueFormatter::Plugins {
        const HexFormatter &hex() const override { return _hex; }
        char locationSymbol() const override { return '$'; }
        const /*PROGMEM*/ char *lineComment_P() const override { return PSTR_DOT_DOT; }
        const SurroundHexFormatter _hex{PSTR_X_DASH, '\''};
    } PLUGINS{};
    return PLUGINS;
}

DisCdp1802::DisCdp1802(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_useReg),
      Config(TABLE),
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

namespace {

Config::uintptr_t page(Config::uintptr_t addr) {
    return addr & ~0xFF;
}

Config::uintptr_t inpage(Config::uintptr_t base, uint8_t offset) {
    return page(base) | offset;
}

}  // namespace

void DisCdp1802::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto opCode = insn.opCode();
    switch (mode) {
    case M_REG1:
    case M_REGN:
        if (_useReg) {
            outRegName(out, RegName(opCode & 0x0F));
        } else {
            outDec(out, opCode & 0xF, 4);
        }
        break;
    case M_IMM8:
        outHex(out, insn.readByte(), 8);
        break;
    case M_IOAD:
        outHex(out, opCode & 7, 3);
        break;
    case M_ADDR:
    case M_LONG:
        outAbsAddr(out, insn.readUint16());
        break;
    case M_PAGE:
    case M_SHRT:
        outAbsAddr(out, inpage(insn.address() + 2, insn.readByte()));
        break;
    default:
        break;
    }
}

Error DisCdp1802::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readByte();
    insn.setOpCode(opc);
    if (TABLE.isPrefix(cpuType(), opc)) {
        insn.setOpCode(insn.readByte(), opc);
        if (insn.getError())
            return _insn.setError(insn);
    }
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    const auto mode1 = insn.mode1();
    if (mode1 != M_NONE) {
        decodeOperand(insn, out, mode1);
        const auto mode2 = insn.mode2();
        if (mode2 != M_NONE)
            decodeOperand(insn, out.comma(), mode2);
    }
    return _insn.setError(insn);
}

}  // namespace cdp1802
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
