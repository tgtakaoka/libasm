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

void DisMc6800::outRegister(RegName regName) {
    _operands = _regs.outRegName(_operands, regName);
}

bool DisMc6800::outAccumulator(const InsnMc6800 &insn) {
    const Config::opcode_t opc = insn.insnCode();
    switch (insn.insnAdjust()) {
    case ADJ_AB01: outRegister((opc & 1) == 0 ? REG_A : REG_B); break;
    case ADJ_AB16: outRegister((opc & 0x10) == 0 ? REG_A : REG_B); break;
    case ADJ_AB64: outRegister((opc & 0x40) == 0 ? REG_A : REG_B); break;
    default: return false;
    }
    return true;
}

Error DisMc6800::decodeInherent(
    DisMemory &memory, InsnMc6800& insn) {
    outAccumulator(insn);
    return setError(OK);
}

Error DisMc6800::decodeDirectPage(
    DisMemory &memory, InsnMc6800& insn) {
    uint8_t dir;
    if (insn.readByte(memory, dir)) return setError(NO_MEMORY);
    if (outAccumulator(insn)) *_operands++ = ',';
    const char *label = lookup(dir);
    if (label) {
        *_operands ++ = '<';
        outText(label);
    } else {
        outConstant(dir, 16, false);
    }
    return setError(OK);
}

Error DisMc6800::decodeExtended(
    DisMemory &memory, InsnMc6800 &insn) {
    Config::uintptr_t addr;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    if (outAccumulator(insn)) *_operands++ = ',';
    const char *label = lookup(addr);
    if (addr < 0x100) *_operands++ = '>';
    if (label) {
        outText(label);
    } else {
        outConstant(addr, 16, false);
    }
    return setError(OK);
}

Error DisMc6800::decodeIndexed(
    DisMemory &memory, InsnMc6800 &insn) {
    uint8_t disp8;
    if (insn.readByte(memory, disp8)) return setError(NO_MEMORY);
    if (outAccumulator(insn)) *_operands++ = ',';
    const char *label = lookup(disp8);
    if (label) {
        outText(label);
    } else {
        outConstant(disp8, 10);
    }
    *_operands++ = ',';
    outRegister(REG_X);
    return setError(OK);
}

Error DisMc6800::decodeRelative(
    DisMemory &memory, InsnMc6800 &insn) {
    uint8_t delta8;
    if (insn.readByte(memory, delta8)) return setError(NO_MEMORY);
    const Config::uintptr_t addr =
        insn.address() + insn.length() + static_cast<int8_t>(delta8);
    const char *label = lookup(addr);
    if (label) {
        outText(label);
    } else {
        outConstant(addr, 16, false);
    }
    return setError(OK);
}

Error DisMc6800::decodeImmediate(
    DisMemory &memory, InsnMc6800 &insn) {
    if (outAccumulator(insn)) *_operands++ = ',';
    *_operands++ = '#';
    if (insn.oprSize() == SZ_BYTE) {
        uint8_t val8;
        if (insn.readByte(memory, val8)) return setError(NO_MEMORY);
        const char *label = lookup(val8);
        if (label) {
            outText(label);
        } else {
            outConstant(val8);
        }
    } else if (insn.oprSize() == SZ_WORD) {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        const char *label = lookup(val16);
        if (label) {
            outText(label);
        } else {
            outConstant(val16);
        }
    } else {
        return setError(UNKNOWN_INSTRUCTION);
    }
    return setError(OK);
}

Error DisMc6800::decode(
    DisMemory &memory, Insn &_insn) {
    InsnMc6800 insn(_insn);
    Config::insn_t insnCode;
    if (insn.readByte(memory, insnCode)) return setError(NO_MEMORY);
    insn.setInsnCode(insnCode);
    if (TableMc6800.searchInsnCode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case INH:
    case ACC: return decodeInherent(memory, insn);
    case DIR: return decodeDirectPage(memory, insn);
    case EXT: return decodeExtended(memory, insn);
    case IDX: return decodeIndexed(memory, insn);
    case REL: return decodeRelative(memory, insn);
    case IMM: return decodeImmediate(memory, insn);
    default: break;
    }
    return setError(INTERNAL_ERROR);
}

} // namespace m6502
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
