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

#include "reg_mos6502.h"
#include "table_mos6502.h"
#include "text_mos6502.h"

namespace libasm {
namespace mos6502 {

using namespace reg;
using namespace text::mos6502;

namespace {

const char OPT_BOOL_INDIRECT_LONG[] PROGMEM = "indirect-long";
const char OPT_DESC_INDIRECT_LONG[] PROGMEM = "[] for indirect long operand";

}  // namespace

const ValueFormatter::Plugins &DisMos6502::defaultPlugins() {
    static const struct fianl : ValueFormatter::Plugins {
        const HexFormatter &hex() const override { return _hex; }
        char locationSymbol() const override { return '*'; }
        const PrefixHexFormatter _hex{HexFormatter::DOLLAR};
    } PLUGINS{};
    return PLUGINS;
}

DisMos6502::DisMos6502(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_longa),
      Config(TABLE),
      _opt_longa(this, &DisMos6502::setLongAccumulator, OPT_BOOL_LONGA, OPT_DESC_LONGA, _opt_longi),
      _opt_longi(
              this, &DisMos6502::setLongIndex, OPT_BOOL_LONGI, OPT_DESC_LONGI, _opt_indirectLong),
      _opt_indirectLong(this, &DisMos6502::setUseIndirectLong, OPT_BOOL_INDIRECT_LONG,
              OPT_DESC_INDIRECT_LONG) {
    reset();
}

void DisMos6502::reset() {
    Disassembler::reset();
    setLongAccumulator(false);
    setLongIndex(false);
    setUseIndirectLong(true);
}

Error DisMos6502::setLongAccumulator(bool enable) {
    _longAccumulator = enable;
    if (cpuType() == W65C816)
        return OK;
    return enable ? OPERAND_NOT_ALLOWED : OK;
}

Error DisMos6502::setLongIndex(bool enable) {
    _longIndex = enable;
    if (cpuType() == W65C816)
        return OK;
    return enable ? OPERAND_NOT_ALLOWED : OK;
}

Error DisMos6502::setUseIndirectLong(bool enable) {
    _useIndirectLong = enable;
    if (cpuType() == W65C816)
        return OK;
    return enable ? OPERAND_NOT_ALLOWED : OK;
}

bool DisMos6502::longImmediate(AddrMode mode) const {
    if (cpuType() == W65C816) {
        if (mode == M_IMA)
            return _longAccumulator;
        if (mode == M_IMX)
            return _longIndex;
    }
    return false;
}

Error DisMos6502::decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode) {
    out.letter('#');
    if (longImmediate(mode)) {
        outHex(out, insn.readUint16(), 16);
    } else {
        outHex(out, insn.readByte(), 8);
    }
    if (insn.opCode() == TableMos6502::WDM)
        return setError(UNKNOWN_INSTRUCTION);
    return setError(insn);
}

Error DisMos6502::decodeAbsoluteLong(DisInsn &insn, StrBuffer &out) {
    uint32_t target = insn.readUint16();
    const auto bank = insn.readByte();
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
            out.letter('>').letter('>').rtext(label);
        } else {
            if (target < 0x10000)
                out.letter('>').letter('>');
            outAbsAddr(out, target);
        }
    }
    return setErrorIf(insn);
}

Error DisMos6502::decodeAbsolute(DisInsn &insn, StrBuffer &out) {
    const auto addr = insn.readUint16();
    const auto label = lookup(addr);
    if (label) {
        out.letter('>').rtext(label);
    } else {
        if (addr < 0x100)
            out.letter('>');
        outAbsAddr(out, addr, 16);
    }
    return setError(insn);
}

Error DisMos6502::decodeDirectPage(DisInsn &insn, StrBuffer &out) {
    const auto zp = insn.readByte();
    const auto label = lookup(zp);
    if (label) {
        out.letter('<').rtext(label);
    } else {
        outAbsAddr(out, zp, 8);
    }
    return setError(insn);
}

Error DisMos6502::decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) {
    int16_t delta;
    if (mode == M_RELL) {
        delta = static_cast<int16_t>(insn.readUint16());
    } else {
        delta = static_cast<int8_t>(insn.readByte());
    }
    const auto base = insn.address() + insn.length();
    const auto target = branchTarget(base, delta);
    if ((target >> 16) != (insn.address() >> 16))
        setErrorIf(OPERAND_TOO_FAR);
    outRelAddr(out, target, insn.address(), mode == M_REL ? 8 : 16);
    return setErrorIf(insn);
}

Error DisMos6502::decodeBlockMove(DisInsn &insn, StrBuffer &out) {
    const uint8_t dbank = insn.readByte();
    const uint8_t sbank = insn.readByte();
    const uint32_t dst = static_cast<uint32_t>(dbank) << 16;
    const uint32_t src = static_cast<uint32_t>(sbank) << 16;
    outAbsAddr(out, src).comma();
    outAbsAddr(out, dst);
    return setOK();
}

Error DisMos6502::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode modeAndFlags) {
    const auto mode = DisInsn::baseMode(modeAndFlags);
    switch (mode) {
    case M_REGA:
        outRegName(out, REG_A);
        break;
    case M_REGX:
        outRegName(out, REG_X);
        break;
    case M_REGY:
        outRegName(out, REG_Y);
        break;
    case M_REGS:
        outRegName(out, REG_S);
        break;
    case M_IMA:
    case M_IMX:
    case M_IM8:
        return decodeImmediate(insn, out, mode);
    case M_ABS:
        return decodeAbsolute(insn, out);
    case M_ABSL:
        return decodeAbsoluteLong(insn, out);
    case M_DPG:
        return decodeDirectPage(insn, out);
    case M_REL:
    case M_RELL:
        return decodeRelative(insn, out, mode);
    default:
        break;
    }
    return setErrorIf(insn);
}

Error DisMos6502::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    DisInsn insn(_insn, memory);
    const Config::opcode_t opCode = insn.readByte();
    if (setError(insn))
        return getError();
    insn.setOpCode(opCode);

    insn.setAllowIndirectLong(_useIndirectLong);
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return setError(insn);

    const auto mode1 = insn.mode1();
    if (mode1 == M_BANK && insn.mode2() == M_BANK)
        return decodeBlockMove(insn, out);
    const auto indir1 = DisInsn::indirect(mode1);
    const auto longi1 = DisInsn::longIndirect(mode1);
    if (mode1 == M_NONE)
        return setOK();
    if (indir1)
        out.letter('(');
    if (longi1)
        out.letter('[');
    if (decodeOperand(insn, out, mode1))
        return getError();

    const auto mode2 = insn.mode2();
    const auto indir2 = DisInsn::indirect(mode2);
    const auto longi2 = DisInsn::longIndirect(mode2);
    if (indir1 && !indir2)
        out.letter(')');
    if (longi1 && !longi2)
        out.letter(']');
    if (mode2 == M_NONE)
        return setOK();
    out.letter(',');
    if (decodeOperand(insn, out, mode2))
        return getError();

    const auto mode3 = insn.mode3();
    if (indir2)
        out.letter(')');
    if (longi2)
        out.letter(']');
    if (mode3 == M_NONE)
        return setOK();
    out.letter(',');
    return decodeOperand(insn, out, mode3);
}

}  // namespace mos6502
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
