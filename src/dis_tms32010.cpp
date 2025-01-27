/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "dis_tms32010.h"
#include "reg_tms32010.h"
#include "table_tms32010.h"
#include "text_common.h"

namespace libasm {
namespace tms32010 {

using namespace reg;
using namespace text::common;

const ValueFormatter::Plugins &DisTms32010::defaultPlugins() {
    static const struct final : ValueFormatter::Plugins {
        const HexFormatter &hex() const override { return _hex; }
        char locationSymbol() const override { return '$'; }
        const /*PROGMEM*/ char *lineComment_P() const override { return PSTR_STAR; }
        const SuffixHexFormatter _hex{'h'};
    } PLUGINS{};
    return PLUGINS;
}

DisTms32010::DisTms32010(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins), Config(TABLE) {
    reset();
}

StrBuffer &DisTms32010::outDirect(StrBuffer &out, DisInsn &insn) const {
    const auto dma = toDmAddr(insn.opCode());
    if (!validDmAddr(insn.opCode(), dma))
        insn.setErrorIf(out, OVERFLOW_RANGE);
    outAbsAddr(out, dma, 8);
    return out;
}

StrBuffer &DisTms32010::outIndirect(StrBuffer &out, uint8_t mam) const {
    const auto modify = (mam >> 4) & 7;
    out.letter('*');
    if (modify == 4 || modify == 7)
        out.text_P(PSTR("BR0"));
    if (modify == 5 || modify == 6)
        out.letter('0');
    if (modify == 1 || modify == 4 || modify == 5)
        return out.letter('-');
    if (modify == 2 || modify == 6 || modify == 7)
        return out.letter('+');
    return out;
}

StrBuffer &DisTms32010::outModifyAR(StrBuffer &out, uint8_t mam) const {
    const auto modify = (mam >> 4) & 7;
    if (modify)
        out.comma();
    return modify == 0 ? out : outIndirect(out, mam);
}

namespace {
bool hasNarp(uint_fast8_t mam) {
    if ((mam & 0x80) == 0)
        return false;
    const auto narp = mam & 8;
    return narp == 0;
}
}  // namespace

StrBuffer &DisTms32010::outNextArp(StrBuffer &out, uint8_t mam) const {
    if (hasNarp(mam))
        outRegName(out.comma(), decodeAR(mam & maxAR()));
    return out;
}

StrBuffer &DisTms32010::outShiftCount(StrBuffer &out, uint8_t count, uint8_t mam) const {
    if (count || hasNarp(mam))
        outDec(out.comma(), count, 4);
    return out;
}

StrBuffer &DisTms32010::outProgramAddress(StrBuffer &out, DisInsn &insn) const {
    const auto pma = insn.readUint16();
    const auto error = insn.setErrorIf(out, checkAddr(pma));
    outAbsAddr(out, pma, error ? ADDRESS_16BIT : 0);
    return out;
}

void DisTms32010::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto opc = insn.opCode();
    switch (mode) {
    case M_MAM:
        if (opc & 0x80) {
            outIndirect(out, opc);
        } else {
            outDirect(out, insn);
        }
        break;
    case M_NARP:
        outNextArp(out, opc);
        break;
    case M_LS4:
        outShiftCount(out, (opc >> 8) & 0xF, opc);
        break;
    case M_LS3:
    case M_LS0:
        outShiftCount(out, (opc >> 8) & 7, opc);
        break;
    case M_AR:
        outRegName(out, decodeAR((opc >> 8) & maxAR()));
        break;
    case M_PA:
        outRegName(out, decodePA((opc >> 8) & maxPA()));
        break;
    case M_ARK:
        outRegName(out, decodeAR(opc & maxAR()));
        break;
    case M_IM1:
        out.letter((opc & 1) == 0 ? '0' : '1');
        break;
    case M_IM8:
        outDec(out, static_cast<uint8_t>(opc), 8);
        break;
    case M_IM13:
        // Sign extends 13-bit number as 0x1000 is a sign bit.
        outDec(out, signExtend(opc, 13), -13);
        break;
    case M_PM12:
        outProgramAddress(out, insn);
        break;
    default:
        break;
    }
}

Error DisTms32010::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    insn.setOpCode(insn.readUint16());
    if (searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    decodeOperand(insn, out, insn.mode1());
    const auto mode2 = insn.mode2();
    if (mode2 != M_NONE) {
        if (!(mode2 == M_LS4 || mode2 == M_LS3 || mode2 == M_LS0 || mode2 == M_NARP))
            out.comma();
        decodeOperand(insn, out, mode2);
        const auto mode3 = insn.mode3();
        if (mode3 != M_NONE)
            decodeOperand(insn, out, mode3);
    }
    return _insn.setError(insn);
}

}  // namespace tms32010
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
