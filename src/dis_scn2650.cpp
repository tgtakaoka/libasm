/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "dis_scn2650.h"

#include "table_scn2650.h"

namespace libasm {
namespace scn2650 {

static constexpr Config::uintptr_t page(const Config::uintptr_t addr) {
    return addr & ~0x1FFF;  // 8k bytes per page
}

static constexpr Config::uintptr_t offset(const Config::uintptr_t addr) {
    return addr & 0x1FFF;
}

static constexpr Config::uintptr_t inpage(
        const Config::uintptr_t addr, const Config::ptrdiff_t delta) {
    return page(addr) | offset(addr + delta);
}

StrBuffer &DisScn2650::appendRegName(InsnScn2650 &insn, StrBuffer &out, RegName name) const {
    auto save = out;
    _regs.outRegName(insn.nameBuffer().over(out).letter(','), name);
    out.over(insn.nameBuffer());
    return save.over(out);
}

StrBuffer &DisScn2650::appendCcName(InsnScn2650 &insn, StrBuffer &out, CcName name) const {
    auto save = out;
    _regs.outCcName(insn.nameBuffer().over(out).letter(','), name);
    out.over(insn.nameBuffer());
    return save.over(out);
}

Error DisScn2650::decodeAbsolute(
        DisMemory &memory, InsnScn2650 &insn, StrBuffer &out, AddrMode mode) {
    const auto opr = insn.readUint16(memory);
    if (opr & 0x8000)
        out.letter('*');
    outAbsAddr(out, opr & ~0x8000);
    if (mode == M_IX15) {
        out.comma();
        _regs.outRegName(out, REG_R3);
    }
    return OK;
}

Error DisScn2650::decodeIndexed(DisMemory &memory, InsnScn2650 &insn, StrBuffer &out) {
    const auto opr = insn.readUint16(memory);
    if (opr & 0x8000)
        out.letter('*');
    const auto base = inpage(insn.address(), insn.length());
    const auto target = page(base) | offset(opr);
    outAbsAddr(out, target);
    auto dst = RegScn2650::decodeRegName(insn.opCode());
    const auto idxMode = opr & 0x6000;
    if (idxMode) {
        out.comma();
        _regs.outRegName(out, dst);
        dst = REG_R0;
        if (idxMode == 0x2000) {
            out.comma().letter('+');
        } else if (idxMode == 0x4000) {
            out.comma().letter('-');
        }
    }
    appendRegName(insn, out, dst);
    return OK;
}

Error DisScn2650::decodeRelative(
        DisMemory &memory, InsnScn2650 &insn, StrBuffer &out, AddrMode mode) {
    const auto opr = insn.readByte(memory);
    if (opr & 0x80)
        out.letter('*');
    // Sign extends 7-bit number
    const auto delta = signExtend(opr, 7);
    if (mode == M_REL7) {
        const auto base = inpage(insn.address(), insn.length());
        const auto target = inpage(base, delta);
        outRelAddr(out, target, insn.address(), 7);
    } else if (mode == M_ABS7) {
        const auto target = offset(delta);
        outAbsAddr(out, target);
    }
    return OK;
}

Error DisScn2650::decodeOperand(
        DisMemory &memory, InsnScn2650 &insn, StrBuffer &out, const AddrMode mode) {
    switch (mode) {
    case M_REG0:
    case M_R123:
    case M_REGN:
        // destination register R0 will be handled at |decodeIndexed|.
        if (insn.mode2() == M_IX13)
            break;
        appendRegName(insn, out, RegScn2650::decodeRegName(insn.opCode()));
        break;
    case M_C012:
    case M_CCVN:
        appendCcName(insn, out, RegScn2650::decodeCcName(insn.opCode()));
        break;
    case M_IMM8:
        outHex(out, insn.readByte(memory), 8);
        break;
    case M_REL7:
    case M_ABS7:
        return decodeRelative(memory, insn, out, mode);
    case M_IX13:
    case M_AB13:
        return decodeIndexed(memory, insn, out);
    case M_IX15:
    case M_AB15:
        return decodeAbsolute(memory, insn, out, mode);
    default:
        break;
    }
    return setOK();
}

Error DisScn2650::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnScn2650 insn(_insn);
    const Config::opcode_t opCode = insn.readByte(memory);
    insn.setOpCode(opCode);
    if (setError(insn))
        return getError();

    if (TableScn2650::TABLE.searchOpCode(insn, out))
        return setError(insn);

    if (decodeOperand(memory, insn, out, insn.mode1()))
        return getError();
    if (decodeOperand(memory, insn, out, insn.mode2()))
        return getError();
    return setError(insn);
}

}  // namespace scn2650
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
