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

#include "table_mc6800.h"

namespace libasm {
namespace mc6800 {

StrBuffer &DisMc6800::outRegister(StrBuffer &out, RegName regName) {
    return _regs.outRegName(out, regName);
}

Error DisMc6800::decodeDirectPage(DisMemory &memory, InsnMc6800 &insn, StrBuffer &out) {
    const uint8_t dir = insn.readByte(memory);
    const char *label = lookup(dir);
    if (label) {
        out.letter('<').text(label);
    } else {
        outAbsAddr(out, dir, 8);
    }
    return OK;
}

Error DisMc6800::decodeExtended(DisMemory &memory, InsnMc6800 &insn, StrBuffer &out) {
    const Config::uintptr_t addr = insn.readUint16(memory);
    const char *label = lookup(addr);
    if (label) {
        out.letter('>').text(label);
    } else {
        if (addr < 0x100)
            out.letter('>');
        outAbsAddr(out, addr);
    }
    return OK;
}

Error DisMc6800::decodeRelative(DisMemory &memory, InsnMc6800 &insn, StrBuffer &out) {
    const int8_t delta8 = static_cast<int8_t>(insn.readByte(memory));
    const Config::uintptr_t base = insn.address() + insn.length();
    const Config::uintptr_t target = base + delta8;
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
        if (TableMc6800.searchOpCodeAlias(insn))
            return setError(TableMc6800.getError());
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
        outRegister(outDec(out, insn.readByte(memory), 8).letter(','), mode == M_IDY ? REG_Y : REG_X);
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
    if (isOK())
        setError(insn);
    return getError();
}

Error DisMc6800::decode(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnMc6800 insn(_insn);
    Config::opcode_t opCode = insn.readByte(memory);
    insn.setOpCode(opCode);
    if (TableMc6800.isPrefix(opCode)) {
        const Config::opcode_t prefix = opCode;
        opCode = insn.readByte(memory);
        insn.setOpCode(opCode, prefix);
    }
    if (setError(insn))
        return getError();

    if (TableMc6800.searchOpCode(insn))
        return setError(TableMc6800.getError());

    const AddrMode mode1 = insn.mode1();
    if (mode1 == M_NO)
        return setOK();
    if (decodeOperand(memory, insn, out, mode1))
        return getError();
    const AddrMode mode2 = insn.mode2();

    if (mode2 == M_NO)
        return setOK();
    out.comma();
    if (decodeOperand(memory, insn, out, mode2))
        return getError();

    const AddrMode mode3 = insn.mode3();
    if (mode3 == M_NO)
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
