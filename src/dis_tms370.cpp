/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "dis_tms370.h"
#include "reg_tms370.h"
#include "table_tms370.h"
#include "text_tms370.h"

namespace libasm {
namespace tms370 {

using namespace reg;
using namespace text::tms370;

namespace {
const char PSTR_BIDX[] PROGMEM = "(B)";
}

const ValueFormatter::Plugins &DisTms370::defaultPlugins() {
    return ValueFormatter::Plugins::intel();
}

DisTms370::DisTms370(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins), Config(TABLE) {
    reset();
}

void DisTms370::decodeRegister(DisInsn &insn, StrBuffer &out) const {
    const auto regno = insn.readByte();
    if (insn.isOK() && regno < 2) {
        out.letter('A' + regno);
    } else {
        outRegName(out, toRegName(regno));
    }
}

void DisTms370::decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    out.letter('#');
    if (mode == M_IM8) {
        outHex(out, insn.readByte(), 8);
    } else if (mode == M_SIM8) {
        outDec(out, insn.readByte(), -8);
    } else if (mode == M_IM16 || mode == M_BIMM) {
        outHex(out, insn.readUint16(), 16);
    }
    if (mode == M_BIMM)
        out.text_P(PSTR_BIDX);
}

void DisTms370::decodeIndexed(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    outDec(out, insn.readByte(), -8);
    out.letter('(');
    if (mode == M_SIDX) {
        outRegName(out, REG_SP);
    } else if (mode == M_RIDX || mode == M_RIMM) {
        const auto reg = insn.readByte();
        outRegName(out, RegName(REG_RN + reg));
    }
    out.letter(')');
}

void DisTms370::decodeAbsolute(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    outAbsAddr(out, insn.readUint16());
    if (mode == M_BIDX)
        out.text_P(PSTR_BIDX);
}

void DisTms370::decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const Config::ptrdiff_t delta = (mode == M_REL) ? static_cast<int8_t>(insn.readByte())
                                                    : static_cast<int16_t>(insn.readUint16());
    const auto base = insn.address() + insn.length();
    const auto target = base + delta;
    if (mode == M_REL)
        insn.setErrorIf(out, checkAddr(target));
    outRelAddr(out, target, insn.address(), (mode == M_REL) ? 8 : 16);
    if (mode == M_BREL)
        out.text_P(PSTR_BIDX);
}

void DisTms370::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
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
        if (insn.opCode() == 0xF0) {
            const auto opr = insn.readByte();
            insn.setPrefix(0xF0);
            insn.setOpCode(opr);
            if (searchOpCode(cpuType(), insn, out)) {
                insn.setOK();
                outHex(out.letter('#'), opr, 8, false);
            }
            break;
        }
        // Fall-through
    case M_SIM8:
    case M_IM16:
    case M_BIMM:
        decodeImmediate(insn, out, mode);
        break;
    case M_ABS:
    case M_BIDX:
        decodeAbsolute(insn, out, mode);
        break;
    case M_RIMM:
        out.letter('#');
        // Fall-through
    case M_SIDX:
    case M_RIDX:
        decodeIndexed(insn, out, mode);
        break;
    case M_IDIR:
        out.letter('@');
        outRegName(out, toRegName(insn.readByte()));
        break;
    case M_REL:
    case M_LREL:
    case M_BREL:
        decodeRelative(insn, out, mode);
        break;
    case M_TVEC:
        outDec(out, 0xF - (insn.opCode() & 0xF), 4);
        break;
    default:
        break;
    }
}

Error DisTms370::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readByte();
    insn.setOpCode(opc);
    if (isPrefix(cpuType(), opc)) {
        insn.setPrefix(opc);
        insn.setOpCode(insn.readByte());
        if (insn.getError())
            return _insn.setError(insn);
    }

    if (searchOpCode(cpuType(), insn, out))
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

}  // namespace tms370
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
