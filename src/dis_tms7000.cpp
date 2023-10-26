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

#include "dis_tms7000.h"

#include "reg_tms7000.h"
#include "table_tms7000.h"
#include "text_tms7000.h"

namespace libasm {
namespace tms7000 {

using namespace reg;
using namespace text::tms7000;

namespace {
const char TEXT_IDXB[] PROGMEM = "(B)";
}

const ValueFormatter::Plugins &DisTms7000::defaultPlugins() {
    return ValueFormatter::Plugins::texas();
}

DisTms7000::DisTms7000(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins), Config(TABLE) {
    reset();
}

void DisTms7000::reset() {
    Disassembler::reset();
}

void DisTms7000::decodeRegister(DisInsn &insn, StrBuffer &out) const {
    const auto regno = insn.readByte();
    if (insn.isOK() && regno < 2) {
        out.letter('A' + regno);
    } else {
        outRegName(out, toRegName(regno));
    }
}

void DisTms7000::decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    out.letter('%');
    const uint16_t val = (mode == M_IM8) ? insn.readByte() : insn.readUint16();
    outHex(out, val, (mode == M_IM8) ? 8 : 16);
    if (mode == M_BIMM)
        out.text_P(TEXT_IDXB);
}

void DisTms7000::decodeAbsolute(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    out.letter('@');
    outAbsAddr(out, insn.readUint16());
    if (mode == M_BIDX)
        out.text_P(TEXT_IDXB);
}

void DisTms7000::decodeRelative(DisInsn &insn, StrBuffer &out) const {
    const auto delta = static_cast<int8_t>(insn.readByte());
    const auto base = insn.address() + insn.length();
    Error error;
    const auto target = branchTarget(base, delta, error);
    if (error)
        insn.setErrorIf(out, error);
    outRelAddr(out, target, insn.address(), 8);
}

void DisTms7000::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    switch (mode) {
    case M_A:
        outRegName(out, REG_A);
        break;
    case M_B:
        outRegName(out, REG_B);
        break;
    case M_ST:
        outRegName(out, REG_ST);
        break;
    case M_RN:
        decodeRegister(insn, out);
        break;
    case M_PN:
        outRegName(out, toPortName(insn.readByte()));
        break;
    case M_IM8:
    case M_IM16:
    case M_BIMM:
        decodeImmediate(insn, out, mode);
        break;
    case M_ABS:
    case M_BIDX:
        decodeAbsolute(insn, out, mode);
        break;
    case M_IDIR:
        out.letter('*');
        outRegName(out, toRegName(insn.readByte()));
        break;
    case M_REL:
        decodeRelative(insn, out);
        break;
    case M_TRAP:
        outDec(out, 0x1F - (insn.opCode() & 0x1F), 5);
        break;
    default:
        break;
    }
}

Error DisTms7000::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto opCode = insn.readByte();
    insn.setOpCode(opCode);
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    decodeOperand(insn, out, insn.src());
    const auto dst = insn.dst();
    if (dst != M_NONE) {
        decodeOperand(insn, out.comma(), dst);
        const auto ext = insn.ext();
        if (ext != M_NONE)
            decodeOperand(insn, out.comma(), ext);
    }
    return _insn.setError(insn);
}

}  // namespace tms7000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
