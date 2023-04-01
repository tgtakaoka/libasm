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

#include "dis_mos6502.h"

#include "table_mos6502.h"

namespace libasm {
namespace mos6502 {

static const char OPT_BOOL_INDIRECT_LONG[] PROGMEM = "indirect-long";
static const char OPT_DESC_INDIRECT_LONG[] PROGMEM = "[] for indirect long operand";
static const char OPT_BOOL_LONGA[] PROGMEM = "longa";
static const char OPT_DESC_LONGA[] PROGMEM = "enable 16-bit accumulator";
static const char OPT_BOOL_LONGI[] PROGMEM = "longi";
static const char OPT_DESC_LONGI[] PROGMEM = "enable 16-bit index registers";

DisMos6502::OptIndirectLong::OptIndirectLong()
    : BoolOptionBase(OPT_BOOL_INDIRECT_LONG, OPT_DESC_INDIRECT_LONG) {}

Error DisMos6502::OptIndirectLong::set(bool value) const {
    return TableMos6502::TABLE.useIndirectLong(value) ? OK : OPERAND_NOT_ALLOWED;
}

DisMos6502::OptLongI::OptLongI(const OptionBase &next)
    : BoolOptionBase(OPT_BOOL_LONGI, OPT_DESC_LONGI, next) {}

Error DisMos6502::OptLongI::set(bool value) const {
    return TableMos6502::TABLE.setLongIndex(value) ? OK : OPERAND_NOT_ALLOWED;
}

DisMos6502::OptLongA::OptLongA(const OptionBase &next)
    : BoolOptionBase(OPT_BOOL_LONGA, OPT_DESC_LONGA, next) {}

Error DisMos6502::OptLongA::set(bool value) const {
    return TableMos6502::TABLE.setLongAccumulator(value) ? OK : OPERAND_NOT_ALLOWED;
}

Error DisMos6502::decodeImmediate(
        DisMemory &memory, InsnMos6502 &insn, StrBuffer &out, AddrMode mode) {
    out.letter('#');
    if (TableMos6502::TABLE.longImmediate(mode)) {
        outHex(out, insn.readUint16(memory), 16);
    } else {
        outHex(out, insn.readByte(memory), 8);
    }
    if (insn.opCode() == TableMos6502::WDM)
        return setError(UNKNOWN_INSTRUCTION);
    return setError(insn);
}

Error DisMos6502::decodeAbsoluteLong(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out) {
    uint32_t target = insn.readUint16(memory);
    const auto bank = insn.readByte(memory);
    target |= static_cast<uint32_t>(bank) << 16;
    const auto err = config().checkAddr(target);
    if (err)
        setErrorIf(err);
    // JSL has only ABS_LONG addressing
    if (insn.opCode() == TableMos6502::JSL) {
        outAbsAddr(out, target);
    } else {
        const auto label = lookup(target);
        if (label) {
            out.letter('>').letter('>').text(label);
        } else {
            if (target < 0x10000)
                out.letter('>').letter('>');
            outAbsAddr(out, target);
        }
    }
    return setErrorIf(insn);
}

Error DisMos6502::decodeAbsolute(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out) {
    const auto addr = insn.readUint16(memory);
    const auto label = lookup(addr);
    if (label) {
        out.letter('>').text(label);
    } else {
        if (addr < 0x100)
            out.letter('>');
        outAbsAddr(out, addr, 16);
    }
    return setError(insn);
}

Error DisMos6502::decodeDirectPage(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out) {
    const auto zp = insn.readByte(memory);
    const auto label = lookup(zp);
    if (label) {
        out.letter('<').text(label);
    } else {
        outAbsAddr(out, zp, 8);
    }
    return setError(insn);
}

Error DisMos6502::decodeRelative(
        DisMemory &memory, InsnMos6502 &insn, StrBuffer &out, AddrMode mode) {
    int16_t delta;
    if (mode == M_RELL) {
        delta = static_cast<int16_t>(insn.readUint16(memory));
    } else {
        delta = static_cast<int8_t>(insn.readByte(memory));
    }
    const auto base = insn.address() + insn.length();
    const auto target = branchTarget(base, delta);
    if ((target >> 16) != (insn.address() >> 16))
        setErrorIf(OPERAND_TOO_FAR);
    outRelAddr(out, target, insn.address(), mode == M_REL ? 8 : 16);
    return setErrorIf(insn);
}

Error DisMos6502::decodeBlockMove(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out) {
    const uint8_t dbank = insn.readByte(memory);
    const uint8_t sbank = insn.readByte(memory);
    const uint32_t dst = static_cast<uint32_t>(dbank) << 16;
    const uint32_t src = static_cast<uint32_t>(sbank) << 16;
    outAbsAddr(out, src).comma();
    outAbsAddr(out, dst);
    return setOK();
}

Error DisMos6502::decodeOperand(
        DisMemory &memory, InsnMos6502 &insn, StrBuffer &out, AddrMode modeAndFlags) {
    const auto mode = InsnMos6502::baseMode(modeAndFlags);
    switch (mode) {
    case M_REGA:
        _regs.outRegName(out, REG_A);
        break;
    case M_REGX:
        _regs.outRegName(out, REG_X);
        break;
    case M_REGY:
        _regs.outRegName(out, REG_Y);
        break;
    case M_REGS:
        _regs.outRegName(out, REG_S);
        break;
    case M_IMA:
    case M_IMX:
    case M_IM8:
        return decodeImmediate(memory, insn, out, mode);
    case M_ABS:
        return decodeAbsolute(memory, insn, out);
    case M_ABSL:
        return decodeAbsoluteLong(memory, insn, out);
    case M_DPG:
        return decodeDirectPage(memory, insn, out);
    case M_REL:
    case M_RELL:
        return decodeRelative(memory, insn, out, mode);
    default:
        break;
    }
    return setErrorIf(insn);
}

Error DisMos6502::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnMos6502 insn(_insn);
    const Config::opcode_t opCode = insn.readByte(memory);
    if (setError(insn))
        return getError();
    insn.setOpCode(opCode);

    if (TableMos6502::TABLE.searchOpCode(insn))
        return setError(insn);

    const auto mode1 = insn.mode1();
    if (mode1 == M_BANK && insn.mode2() == M_BANK)
        return decodeBlockMove(memory, insn, out);
    const auto indir1 = InsnMos6502::indirect(mode1);
    const auto longi1 = InsnMos6502::longIndirect(mode1);
    if (mode1 == M_NONE)
        return setOK();
    if (indir1)
        out.letter('(');
    if (longi1)
        out.letter('[');
    if (decodeOperand(memory, insn, out, mode1))
        return getError();

    const auto mode2 = insn.mode2();
    const auto indir2 = InsnMos6502::indirect(mode2);
    const auto longi2 = InsnMos6502::longIndirect(mode2);
    if (indir1 && !indir2)
        out.letter(')');
    if (longi1 && !longi2)
        out.letter(']');
    if (mode2 == M_NONE)
        return setOK();
    out.letter(',');
    if (decodeOperand(memory, insn, out, mode2))
        return getError();

    const auto mode3 = insn.mode3();
    if (indir2)
        out.letter(')');
    if (longi2)
        out.letter(']');
    if (mode3 == M_NONE)
        return setOK();
    out.letter(',');
    return decodeOperand(memory, insn, out, mode3);
}

}  // namespace mos6502
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
