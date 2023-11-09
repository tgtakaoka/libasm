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

#include "dis_ins8060.h"

#include "reg_ins8060.h"
#include "table_ins8060.h"

namespace libasm {
namespace ins8060 {

using namespace reg;

const ValueFormatter::Plugins &DisIns8060::defaultPlugins() {
    return ValueFormatter::Plugins::national();
}

DisIns8060::DisIns8060(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins), Config(TABLE) {
    reset();
}

void DisIns8060::decodePntr(DisInsn &insn, StrBuffer &out) const {
    outRegName(out, decodePointerReg(insn.opCode()));
}

void DisIns8060::decodeImm8(DisInsn &insn, StrBuffer &out) const {
    outHex(out, insn.readByte(), 8);
}

void DisIns8060::decodeIndx(DisInsn &insn, StrBuffer &out, bool hasMode) const {
    if (hasMode && (insn.opCode() & 4) != 0)
        out.letter('@');
    const auto reg = decodePointerReg(insn.opCode());
    const auto opr = insn.readByte();
    if (reg == REG_PC && opr != 0x80) {  // PC relative
        // PC points the last byte of instruction.
        const auto base = insn.address() + 1;
        const auto disp = static_cast<int8_t>(opr);
        // PC will be incremented before fetching next instruction.
        const int8_t fetch = insn.addrMode() == M_REL8 ? 1 : 0;
        // Program space is paged by 4kB.
        const auto toff = offset(offset(base) + disp + fetch);
        const Config::uintptr_t target = page(base) | toff;
        const Config::ptrdiff_t diff = target - base - fetch;
        if (diff >= -127 && diff < 128) {
            outRelAddr(out, target, insn.address(), 8);
        } else {
            outAbsAddr(out, target);
        }
    } else {
        if (opr == 0x80) {  // E(Pn)
            outRegName(out, REG_E);
        } else {
            const int8_t disp = static_cast<int8_t>(opr);
            outDec(out, disp, -8);
        }
        outRegName(out.letter('('), reg).letter(')');
    }
}

Error DisIns8060::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    insn.setOpCode(insn.readByte());
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    switch (insn.addrMode()) {
    case M_PNTR:
        decodePntr(insn, out);
        break;
    case M_IMM8:
        decodeImm8(insn, out);
        break;
    case M_REL8:
    case M_DISP:
        decodeIndx(insn, out, false);
        break;
    case M_INDX:
        decodeIndx(insn, out, true);
        break;
    default:
        break;
    }
    if (page(insn.address()) != page(insn.address() + insn.length() - 1))
        insn.setErrorIf(OVERWRAP_SEGMENT);
    return _insn.setError(insn);
}

}  // namespace ins8060
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
