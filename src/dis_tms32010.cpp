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
    // Store Status Register can access Data Page 1 only.
    static constexpr uint8_t SST = 0x7C;
    const auto opc = insn.opCode();
    uint8_t dma = opc & 0x7F;
    if ((opc >> 8) == SST) {
        dma |= (1 << 7);
        if (dma > dataMemoryLimit())
            insn.setErrorIf(out, OVERFLOW_RANGE);
    }
    outAbsAddr(out, dma, 8);
    return out;
}

StrBuffer &DisTms32010::outIndirect(StrBuffer &out, uint8_t mam) const {
    out.letter('*');
    if (mam & 0x20)
        return out.letter('+');
    if (mam & 0x10)
        return out.letter('-');
    return out;
}

StrBuffer &DisTms32010::outNextArp(StrBuffer &out, uint8_t mam) const {
    if ((mam & 0x80) && (mam & 8) == 0) {
        const RegName arp = (mam & 1) == 0 ? REG_AR0 : REG_AR1;
        outRegName(out.comma(), arp);
    }
    return out;
}

StrBuffer &DisTms32010::outShiftCount(StrBuffer &out, uint8_t count, uint8_t mam) const {
    const bool indir = mam & (1 << 7);
    const bool nar = (mam & (1 << 3)) == 0;
    if (count || (indir && nar)) {
        out.comma();
        outDec(out, count, 4);
    }
    return out;
}

StrBuffer &DisTms32010::outProgramAddress(StrBuffer &out, DisInsn &insn) const {
    const auto pma = insn.readUint16();
    const auto error = insn.setErrorIf(out, checkAddr(pma));
    outAbsAddr(out, pma, error ? ADDRESS_16BIT : 0);
    return out;
}

void DisTms32010::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const Config::opcode_t opc = insn.opCode();
    switch (mode) {
    case M_MAM:
        if (opc & (1 << 7)) {
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
        outRegName(out, (opc & (1 << 8)) == 0 ? REG_AR0 : REG_AR1);
        break;
    case M_PA:
        outRegName(out, RegName(((opc >> 8) & 7) + int8_t(REG_PA0)));
        break;
    case M_ARK:
        outRegName(out, (opc & 1) == 0 ? REG_AR0 : REG_AR1);
        break;
    case M_DPK:
        out.letter((opc & 1) == 0 ? '0' : '1');
        break;
    case M_IM8:
        outDec(out, static_cast<uint8_t>(opc), 8);
        break;
    case M_IM13:
        // Sign extends 13-bit number as 0x1000 is a sign bit.
        outDec(out, signExtend(opc, 13), -13);
        break;
    case M_PMA:
        outProgramAddress(out, insn);
        break;
    default:
        break;
    }
}

Error DisTms32010::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    DisInsn insn(_insn, memory, out);
    insn.setOpCode(insn.readUint16());
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return setError(insn);

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
    return setError(insn);
}

}  // namespace tms32010
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
