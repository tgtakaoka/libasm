/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "dis_h8300.h"
#include "reg_h8300.h"
#include "table_h8300.h"
#include "text_h8300.h"

namespace libasm {
namespace h8300 {

using namespace reg;

const ValueFormatter::Plugins &DisH8300::defaultPlugins() {
    return ValueFormatter::Plugins::hitachi();
}

DisH8300::DisH8300(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_spAlias),
      Config(TABLE),
      _opt_spAlias(this, &Config::setSpAlias,
              ::libasm::text::h8300::OPT_BOOL_SP_ALIAS,
              ::libasm::text::h8300::OPT_DESC_SP_ALIAS) {
    reset();
}

void DisH8300::decodeRelative(DisInsn &insn, StrBuffer &out) const {
    const auto delta = static_cast<int8_t>(insn.opCode() & UINT8_MAX);
    const auto base = insn.address() + insn.length();
    const auto target = static_cast<Config::uintptr_t>(base + delta);
    insn.setErrorIf(out, checkAddr(target, true));
    outRelAddr(out, target, insn.address(), 8);
}

Config::uintptr_t readAddress(DisInsn &insn, OprPos pos, uint_fast8_t bits, bool indirect) {
    if (bits == 16)
        return insn.readUint16();
    const auto code = (pos == POS_PRX) ? insn.prefix() : insn.opCode();
    const auto addr8 = code & UINT8_MAX;
    return (indirect ? 0x0000 : 0xFF00) | addr8;
}

void DisH8300::decodeAbsolute(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos) const {
    const auto bits = (mode == M_MIND8 || mode == M_ABS8) ? 8 : 16;
    const auto addr = readAddress(insn, pos, bits, mode == M_MIND8);
    insn.setErrorIf(out, checkAddr(addr, insn.oprSize() == SZ_WORD));
    if (mode == M_MIND8)
        out.letter('@');
    const auto *label = lookup(addr, addressWidth());
    if (label) {
        out.rtext(label).letter(':').int16(bits);
    } else {
        outHex(out, addr, 16, false);
        if (mode == M_ABS8 || (mode == M_ABS16 && addr >= 0xFF00))
            out.letter(':').int16(bits);
    }
}

void DisH8300::decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode, uint16_t val) const {
    const auto bits = (mode == M_IMM8) ? 8 : 16;
    out.letter('#');
    if (bits == addressWidth()) {
        const auto *label = lookup(val, addressWidth());
        if (label) {
            out.rtext(label);
            return;
        }
    }
    outHex(out, val, bits);
}

RegName DisH8300::decodeOprReg8(DisInsn &insn, OprPos pos) const {
    const auto code = (pos == POS_PRX) ? insn.prefix() : insn.opCode();
    if (pos == POS_PRX)
        pos = insn.prefixPos;
    return decodeReg8(Entry::decodeOperand(code, pos));
}

RegName DisH8300::decodeOprReg16(
        DisInsn &insn, const StrBuffer &out, AddrMode mode, OprPos pos) const {
    const auto code = (pos == POS_PRX) ? insn.prefix() : insn.opCode();
    if (pos == POS_PRX)
        pos = insn.prefixPos;
    const auto regno = Entry::decodeOperand(code, pos);
    // H8/300 has only R0-R7 (16-bit). The 4-bit register field is shared with
    // future H8/300H E-register encodings (bit 3 = 1), which are illegal here.
    if (regno & 0x08)
        insn.setErrorIf(UNKNOWN_INSTRUCTION);
    if (regno == 7 && mode != M_REG16) {
        if (insn.oprSize() == SZ_BYTE && (mode == M_PINC || mode == M_PDEC))
            insn.setErrorIf(out, ILLEGAL_SIZE);
        return spAlias() ? REG_SP : REG_R7;
    }
    return decodeReg16(regno);
}

void DisH8300::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos) const {
    const auto opc = insn.opCode();
    switch (mode) {
    case M_REG8:
        outRegName(out, decodeOprReg8(insn, pos));
        break;
    case M_REG16:
    case M_ADREG:
        outRegName(out, decodeOprReg16(insn, out, mode, pos));
        break;
    case M_INDIR:
    case M_PINC:
    case M_PDEC: {
        const auto regName = decodeOprReg16(insn, out, mode, pos);
        out.letter('@');
        if (mode == M_PDEC)
            out.letter('-');
        outRegName(out, regName);
        if (mode == M_PINC)
            out.letter('+');
        break;
    }
    case M_IDX16:
        out.letter('@').letter('(');
        decodeAbsolute(insn, out, mode, pos);
        out.letter(',');
        outRegName(out, decodeOprReg16(insn, out, mode, pos)).letter(')');
        break;
    case M_ABS8:
    case M_MIND8:
    case M_ABS16:
        decodeAbsolute(insn, out.letter('@'), mode, pos);
        break;
    case M_REL8:
        decodeRelative(insn, out);
        break;
    case M_IMM8:
        decodeImmediate(insn, out, mode, opc & UINT8_MAX);
        break;
    case M_IMM16:
        decodeImmediate(insn, out, mode, insn.readUint16());
        break;
    case M_IMM3:
        decodeImmediate(insn, out, mode, (opc >> 4) & 7);
        break;
    case M_VAL1:
        decodeImmediate(insn, out, mode, 1);
        break;
    case M_VAL2:
        decodeImmediate(insn, out, mode, 2);
        break;
    case M_CCR:
        outRegName(out, REG_CCR);
        break;
    case M_NONE:
    default:
        break;
    }
}

Error DisH8300::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readUint16();
    insn.setOpCode(opc);
    if (isPrefix(cpuType(), opc)) {
        insn.setPrefix(opc);
        insn.setOpCode(insn.readUint16());
        if (insn.getError())
            return _insn.setError(insn);
    }

    if (searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    const auto size = (insn.insnSize() == ISZ_DATA) ? insn.oprSize() : SZ_NONE;
    const auto suffix = sizeSuffix(size);
    if (suffix) {
        insn.nameBuffer().letter('.');
        insn.appendName(out, suffix);
    }

    decodeOperand(insn, out, insn.src(), insn.srcPos());
    const auto dst = insn.dst();
    if (dst != M_NONE)
        decodeOperand(insn, out.comma(), insn.dst(), insn.dstPos());

    if (insn.getError() == UNKNOWN_INSTRUCTION) {
        insn.nameBuffer().reset();
        out.reset();
    }
    return _insn.setError(insn);
}

}  // namespace h8300
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
