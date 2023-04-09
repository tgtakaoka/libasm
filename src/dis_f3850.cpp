/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "dis_f3850.h"

#include "reg_f3850.h"
#include "table_f3850.h"

namespace libasm {
namespace f3850 {

using namespace reg;

static const char OPT_BOOL_USE_SCRATCHPAD[] PROGMEM = "use-scratchpad";
static const char OPT_DESC_USE_SCRATCHPAD[] PROGMEM = "use name for scratchpad";

DisF3850::DisF3850()
    : Disassembler(_formatter, TableF3850::TABLE, '$', &_opt_useScratchpad),
      _formatter(),
      _opt_useScratchpad(this, &DisF3850::setUseScratchpadName, OPT_BOOL_USE_SCRATCHPAD,
              OPT_DESC_USE_SCRATCHPAD) {
    reset();
}

void DisF3850::reset() {
    Disassembler::reset();
    setUseScratchpadName(false);
}

Error DisF3850::setUseScratchpadName(bool enable) {
    _useScratchpad = enable;
    return OK;
}

Error DisF3850::decodeRelative(DisMemory& memory, InsnF3850& insn, StrBuffer& out) {
    const auto delta = static_cast<int8_t>(insn.readByte(memory));
    const auto base = insn.address() + 1;
    const auto target = branchTarget(base, delta);
    outRelAddr(out, target, insn.address(), 8);
    return OK;
}

Error DisF3850::decodeOperand(DisMemory& memory, InsnF3850& insn, StrBuffer& out, AddrMode mode) {
    const auto opCode = insn.opCode();
    switch (mode) {
    case M_REG:
        if (_useScratchpad) {
            const auto name = decodeRegName(opCode);
            if (name != REG_UNDEF) {
                outRegName(out, name);
                break;
            }
        }
        outDec(out, opCode & 0xF, 4);
        return OK;
    case M_C1:
        outDec(out, 1, 1);
        return OK;
    case M_C4:
        outDec(out, 4, 3);
        return OK;
    case M_IM3:
        outHex(out, opCode & 7, 3);
        return OK;
    case M_IM4:
        outHex(out, opCode & 0xF, 4);
        return OK;
    case M_IM8:
        outHex(out, insn.readByte(memory), 8);
        break;
    case M_ADDR:
        outAbsAddr(out, insn.readUint16(memory));
        break;
    case M_REL:
        return decodeRelative(memory, insn, out);
    case M_NONE:
        return OK;
    case M_J:
        outRegName(out, REG_J);
        return OK;
    default:
        if (uint8_t(mode) < uint8_t(M_REG))
            outRegName(out, RegName(uint8_t(mode)));
        return OK;
    }
    return setError(insn);
}

Error DisF3850::decodeImpl(DisMemory& memory, Insn& _insn, StrBuffer& out) {
    InsnF3850 insn(_insn);
    auto opCode = insn.readByte(memory);
    insn.setOpCode(opCode);
    if (setError(insn))
        return getError();

    if (TableF3850::TABLE.searchOpCode(insn, out))
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

}  // namespace f3850
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
