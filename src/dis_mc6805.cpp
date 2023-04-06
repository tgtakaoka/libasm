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

#include "dis_mc6805.h"

#include "reg_mc6805.h"
#include "table_mc6805.h"

namespace libasm {
namespace mc6805 {

using namespace reg;

static const char OPT_INT_PCBITS[] PROGMEM = "pc-bits";
static const char OPT_DESC_PCBITS[] = "program counter width in bit, default 13";

DisMc6805::DisMc6805() : Disassembler(_formatter, TableMc6805::TABLE, '*'), _formatter() {
    reset();
}

AddressWidth DisMc6805::addressWidth() const {
    return AddressWidth(_pc_bits == 0 ? 13 : _pc_bits);
}

void DisMc6805::reset() {
    Disassembler::reset();
    _pc_bits = 0;
}

DisMc6805::OptPcBits::OptPcBits(uint8_t &var)
    : IntOptionBase(OPT_INT_PCBITS, OPT_DESC_PCBITS), _var(var) {}

Error DisMc6805::OptPcBits::check(int32_t value) const {
    return value >= 0 && value <= 16 ? OK : OVERFLOW_RANGE;
}

void DisMc6805::OptPcBits::set(int32_t value) const {
    _var = value;
}

Error DisMc6805::decodeDirectPage(DisMemory &memory, InsnMc6805 &insn, StrBuffer &out) {
    const uint8_t dir = insn.readByte(memory);
    const auto label = lookup(dir);
    if (label) {
        out.letter('<').rtext(label);
    } else {
        outAbsAddr(out, dir, 8);
    }
    return setError(insn);
}

Error DisMc6805::decodeExtended(DisMemory &memory, InsnMc6805 &insn, StrBuffer &out) {
    const Config::uintptr_t addr = insn.readUint16(memory);
    if (checkAddr(addr))
        setErrorIf(OVERFLOW_RANGE);
    const auto label = lookup(addr);
    if (label) {
        out.letter('>').rtext(label);
    } else {
        if (addr < 0x100)
            out.letter('>');
        outAbsAddr(out, addr, isOK() ? 0 : ADDRESS_16BIT);
    }
    return setErrorIf(insn);
}

Error DisMc6805::decodeIndexed(DisMemory &memory, InsnMc6805 &insn, StrBuffer &out, AddrMode mode) {
    if (mode == M_IX0) {
        outRegName(out.letter(','), REG_X);
    } else if (mode == M_IX2) {
        const uint16_t disp16 = insn.readUint16(memory);
        if (disp16 < 0x100)
            out.letter('>');
        outDec(out, disp16, 16).letter(',');
        outRegName(out, REG_X);
    } else {
        const uint8_t disp8 = insn.readByte(memory);
        if (disp8 == 0)
            out.letter('<');
        outDec(out, disp8, 8).letter(',');
        outRegName(out, REG_X);
    }
    return setErrorIf(insn);
}

Error DisMc6805::decodeRelative(DisMemory &memory, InsnMc6805 &insn, StrBuffer &out) {
    const auto delta = static_cast<int8_t>(insn.readByte(memory));
    const auto base = insn.address() + insn.length();
    const auto target = branchTarget(base, delta);
    outRelAddr(out, target, insn.address(), 8);
    return setErrorIf(insn);
}

Error DisMc6805::decodeOperand(DisMemory &memory, InsnMc6805 &insn, StrBuffer &out, AddrMode mode) {
    if (mode == M_GEN || mode == M_MEM) {
        switch (insn.opCode() & 0xF0) {
        case 0xA0:
            outHex(out.letter('#'), insn.readByte(memory), 8);
            return OK;
        case 0x30:
        case 0xB0:
            return decodeDirectPage(memory, insn, out);
        case 0xC0:
            return decodeExtended(memory, insn, out);
        case 0xD0:
            return decodeIndexed(memory, insn, out, M_IX2);
        case 0x60:
        case 0xE0:
            return decodeIndexed(memory, insn, out, M_IDX);
        default:
            return decodeIndexed(memory, insn, out, M_IX0);
        }
    }
    switch (mode) {
    case M_DIR:
        return decodeDirectPage(memory, insn, out);
    case M_EXT:
        return decodeExtended(memory, insn, out);
    case M_IDX:
    case M_IX0:
    case M_IX2:
        return decodeIndexed(memory, insn, out, mode);
    case M_REL:
        return decodeRelative(memory, insn, out);
    case M_IMM:
        outHex(out.letter('#'), insn.readByte(memory), 8);
        return OK;
    case M_BNO:
        outHex(out, (insn.opCode() >> 1) & 7, 3);
        return OK;
    default:
        return OK;
    }
}

Error DisMc6805::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnMc6805 insn(_insn);
    auto opCode = insn.readByte(memory);
    insn.setOpCode(opCode);
    if (setError(insn))
        return getError();

    if (TableMc6805::TABLE.searchOpCode(insn, out))
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

}  // namespace mc6805
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
