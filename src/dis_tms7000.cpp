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

DisTms7000::DisTms7000() : Disassembler(_hexFormatter, '$'), Config(TABLE) {
    reset();
}

void DisTms7000::reset() {
    Disassembler::reset();
}

Error DisTms7000::decodeRegister(DisInsn &insn, StrBuffer &out) {
    const auto regno = insn.readByte();
    if (regno < 2) {
        out.letter('A' + regno);
    } else {
        outRegName(out, toRegName(regno));
    }
    return setError(insn);
}

Error DisTms7000::decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode) {
    out.letter('%');
    if (mode == M_IM8) {
        outHex(out, insn.readByte(), 8);
    } else if (mode == M_IM16) {
        outHex(out, insn.readUint16(), 16);
    } else {
        outHex(out, insn.readUint16(), 16).text_P(TEXT_IDXB);
    }
    return setError(insn);
}

Error DisTms7000::decodeAbsolute(DisInsn &insn, StrBuffer &out, AddrMode mode) {
    const auto addr = insn.readUint16();
    const auto label = lookup(addr);
    if (label) {
        out.letter('@').rtext(label);
    } else {
        outAbsAddr(out.letter('@'), addr, 16);
    }
    if (mode == M_BIDX)
        out.text_P(TEXT_IDXB);
    return setError(insn);
}

Error DisTms7000::decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) {
    int16_t delta = static_cast<int8_t>(insn.readByte());
    const auto base = insn.address() + insn.length();
    const auto target = branchTarget(base, delta);
    outRelAddr(out, target, insn.address(), 8);
    return setErrorIf(insn);
}

Error DisTms7000::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) {
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
        return decodeRegister(insn, out);
    case M_PN:
        outRegName(out, toPortName(insn.readByte()));
        break;
    case M_IM8:
    case M_IM16:
    case M_BIMM:
        return decodeImmediate(insn, out, mode);
    case M_ABS:
    case M_BIDX:
        return decodeAbsolute(insn, out, mode);
    case M_IDIR:
        outRegName(out.letter('*'), toRegName(insn.readByte()));
        break;
    case M_REL:
        return decodeRelative(insn, out, mode);
    case M_TRAP:
        outDec(out, 0x1F - (insn.opCode() & 0x1F), 5);
        break;
    default:
        break;
    }
    return setErrorIf(insn);
}

Error DisTms7000::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    DisInsn insn(_insn, memory);
    const Config::opcode_t opCode = insn.readByte();
    if (setError(insn))
        return getError();
    insn.setOpCode(opCode);

    if (TABLE.searchOpCode(cpuType(), insn, out))
        return setError(insn);

    const auto src = insn.src();
    if (src == M_NONE)
        return setOK();
    if (decodeOperand(insn, out, src))
        return getError();

    const auto dst = insn.dst();
    if (dst == M_NONE)
        return setOK();
    out.comma();
    if (decodeOperand(insn, out, dst))
        return getError();

    const auto ext = insn.ext();
    if (ext == M_NONE)
        return setOK();
    out.comma();
    return decodeOperand(insn, out, ext);
}

}  // namespace tms7000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
