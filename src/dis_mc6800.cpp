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

DisMc6800::DisMc6800() : Disassembler(_hexFormatter, TableMc6800::TABLE, '*') {
    reset();
}

Error DisMc6800::decodeDirectPage(DisMemory &memory, InsnMc6800 &insn, StrBuffer &out) {
    const uint8_t dir = insn.readByte(memory);
    const auto label = lookup(dir);
    if (label) {
        out.letter('<').rtext(label);
    } else {
        outAbsAddr(out, dir, 8);
    }
    return OK;
}

Error DisMc6800::decodeExtended(DisMemory &memory, InsnMc6800 &insn, StrBuffer &out) {
    const Config::uintptr_t addr = insn.readUint16(memory);
    const auto label = lookup(addr);
    if (label) {
        out.letter('>').rtext(label);
    } else {
        if (addr < 0x100)
            out.letter('>');
        outAbsAddr(out, addr);
    }
    return OK;
}

Error DisMc6800::decodeRelative(DisMemory &memory, InsnMc6800 &insn, StrBuffer &out) {
    const auto delta = static_cast<int8_t>(insn.readByte(memory));
    const auto base = insn.address() + insn.length();
    const auto target = branchTarget(base, delta);
    outRelAddr(out, target, insn.address(), 8);
    return OK;
}

static int8_t bitNumber(uint8_t val) {
    for (uint8_t pos = 0, mask = 0x01; pos < 8; pos++, mask <<= 1) {
        if (val == mask)
            return pos;
    }
    return -1;
}

Error DisMc6800::decodeBitNumber(DisMemory &memory, InsnMc6800 &insn, StrBuffer &out) {
    const uint8_t val8 = insn.readByte(memory);
    const bool aim = (insn.opCode() & 0xF) == 1;
    const int8_t bitNum = bitNumber(aim ? ~val8 : val8);
    if (bitNum >= 0) {
        if (TableMc6800::TABLE.searchOpCodeAlias(insn, out))
            return setError(insn);
        outHex(out, bitNum, 3);
    } else {
        outHex(out.letter('#'), val8, 8);
    }
    return OK;
}

Error DisMc6800::decodeOperand(DisMemory &memory, InsnMc6800 &insn, StrBuffer &out, AddrMode mode) {
    const auto gn = insn.opCode() & 0x30;
    switch (mode) {
    case M_DIR:
    dir:
        decodeDirectPage(memory, insn, out);
        break;
    case M_GMEM:
        if ((insn.opCode() & 0xF0) == 0x60)
            goto idx;
        // Fall-through
    case M_EXT:
    ext:
        decodeExtended(memory, insn, out);
        break;
    case M_IDX:
    case M_IDY:
    idx:
        outRegName(
                outDec(out, insn.readByte(memory), 8).letter(','), mode == M_IDY ? REG_Y : REG_X);
        break;
    case M_REL:
        decodeRelative(memory, insn, out);
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
        outHex(out.letter('#'), insn.readByte(memory), 8);
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
        outHex(out.letter('#'), insn.readUint16(memory), 16);
        break;
    case M_BMM:
        decodeBitNumber(memory, insn, out);
        break;
    default:
        return OK;
    }
    return setErrorIf(insn);
}

Error DisMc6800::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnMc6800 insn(_insn);
    auto opCode = insn.readByte(memory);
    insn.setOpCode(opCode);
    if (TableMc6800::TABLE.isPrefix(opCode)) {
        const auto prefix = opCode;
        opCode = insn.readByte(memory);
        insn.setOpCode(opCode, prefix);
    }
    if (setError(insn))
        return getError();

    if (TableMc6800::TABLE.searchOpCode(insn, out))
        return setError(insn);

    const auto mode1 = insn.mode1();
    if (mode1 == M_NONE)
        return setOK();
    if (decodeOperand(memory, insn, out, mode1))
        return getError();

    const auto mode2 = insn.mode2();
    if (mode2 == M_NONE)
        return setOK();
    out.comma();
    if (decodeOperand(memory, insn, out, mode2))
        return getError();

    const auto mode3 = insn.mode3();
    if (mode3 == M_NONE)
        return setOK();
    out.comma();
    return decodeOperand(memory, insn, out, mode3);
}

}  // namespace mc6800
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
