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

#include "dis_mc6800.h"
#include "reg_mc6800.h"
#include "table_mc6800.h"

namespace libasm {
namespace mc6800 {

using namespace reg;

const ValueFormatter::Plugins &DisMc6800::defaultPlugins() {
    return ValueFormatter::Plugins::motorola();
}

DisMc6800::DisMc6800(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins), Config(TABLE) {
    reset();
}

void DisMc6800::decodeDirectPage(DisInsn &insn, StrBuffer &out) const {
    const uint8_t dir = insn.readByte();
    const auto label = lookup(dir);
    if (label) {
        out.letter('<').rtext(label);
    } else {
        outAbsAddr(out, dir, 8);
    }
}

void DisMc6800::decodeExtended(DisInsn &insn, StrBuffer &out) const {
    const Config::uintptr_t addr = insn.readUint16();
    const auto label = lookup(addr);
    if (label) {
        out.letter('>').rtext(label);
    } else {
        if (addr < 0x100)
            out.letter('>');
        outAbsAddr(out, addr);
    }
}

void DisMc6800::decodeRelative(DisInsn &insn, StrBuffer &out) const {
    const auto delta = static_cast<int8_t>(insn.readByte());
    const auto base = insn.address() + insn.length();
    const auto target = base + delta;
    insn.setErrorIf(out, checkAddr(target));
    outRelAddr(out, target, insn.address(), 8);
}

namespace {

int8_t bitNumber(uint8_t val) {
    for (uint8_t pos = 0, mask = 0x01; pos < 8; pos++, mask <<= 1) {
        if (val == mask)
            return pos;
    }
    return -1;
}

}  // namespace

void DisMc6800::decodeBitNumber(DisInsn &insn, StrBuffer &out) const {
    const uint8_t val8 = insn.readByte();
    const bool aim = (insn.opCode() & 0xF) == 1;
    const int8_t bitNum = bitNumber(aim ? ~val8 : val8);
    if (bitNum >= 0) {
        searchOpCodeAlias(cpuType(), insn, out);
        outHex(out, bitNum, 3);
    } else {
        outHex(out.letter('#'), val8, 8);
    }
}

void DisMc6800::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto gn = insn.opCode() & 0x30;
    switch (mode) {
    case M_DIR:
    dir:
        decodeDirectPage(insn, out);
        break;
    case M_GMEM:
        if ((insn.opCode() & 0xF0) == 0x60)
            goto idx;
        // Fall-through
    case M_EXT:
    ext:
        decodeExtended(insn, out);
        break;
    case M_IDX:
    case M_IDY:
    idx:
        outRegName(outDec(out, insn.readByte(), 8).letter(','), mode == M_IDY ? REG_Y : REG_X);
        break;
    case M_REL:
        decodeRelative(insn, out);
        break;
    case M_GN8:
        if (gn == 0x10)
            goto dir;
        if (gn == 0x20)
            goto idx;
        if (gn == 0x30)
            goto ext;
        // Fall-through
    case M_IM8:
        out.letter('#');
        outHex(out, insn.readByte(), 8);
        break;
    case M_GN16:
        if (gn == 0x10)
            goto dir;
        if (gn == 0x20)
            goto idx;
        if (gn == 0x30)
            goto ext;
        // Fall-through
    case M_IM16:
        out.letter('#');
        outHex(out, insn.readUint16(), 16);
        break;
    case M_BMM:
        return decodeBitNumber(insn, out);
    default:
        break;
    }
}

Error DisMc6800::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
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

    decodeOperand(insn, out, insn.mode1());
    const auto mode2 = insn.mode2();
    if (mode2 != M_NONE) {
        decodeOperand(insn, out.comma(), mode2);
        const auto mode3 = insn.mode3();
        if (mode3 != M_NONE)
            decodeOperand(insn, out.comma(), mode3);
    }
    return _insn.setError(insn);
}

}  // namespace mc6800
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
