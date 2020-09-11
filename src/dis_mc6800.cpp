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

Error DisMc6800::decodeDirectPage(DisMemory &memory, InsnMc6800& insn) {
    uint8_t dir;
    if (insn.readByte(memory, dir)) return setError(NO_MEMORY);
    outAddress(dir, PSTR("<"));
    return setOK();
}

Error DisMc6800::decodeExtended(DisMemory &memory, InsnMc6800 &insn) {
    Config::uintptr_t addr;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    outAddress(addr, PSTR(">"), addr < 0x100);
    return setOK();
}

Error DisMc6800::decodeIndexed(DisMemory &memory, InsnMc6800 &insn, AddrMode mode) {
    uint8_t disp8;
    if (insn.readByte(memory, disp8)) return setError(NO_MEMORY);
    outConstant(disp8, 10);
    *_operands++ = ',';
    outRegister(mode == M_IDY ? REG_Y : REG_X);
    return setOK();
}

Error DisMc6800::decodeRelative(DisMemory &memory, InsnMc6800 &insn) {
    uint8_t delta8;
    if (insn.readByte(memory, delta8)) return setError(NO_MEMORY);
    const Config::uintptr_t base = insn.address() + insn.length();
    const Config::uintptr_t target = base + static_cast<int8_t>(delta8);
    outRelativeAddr(target, insn.address(), 8);
    return setOK();
}

Error DisMc6800::decodeImmediate(DisMemory &memory, InsnMc6800 &insn) {
    *_operands++ = '#';
    if (insn.size() == SZ_BYTE) {
        uint8_t val8;
        if (insn.readByte(memory, val8)) return setError(NO_MEMORY);
        outConstant(val8);
        return setOK();
    }
    // SZ_WORD
    uint16_t val16;
    if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
    outConstant(val16);
    return setOK();
}

static int8_t bitNumber(uint8_t val) {
    for (uint8_t pos = 0, mask = 0x01; pos < 8; pos++, mask <<= 1) {
        if (val == mask) return pos;
    }
    return -1;
}

Error DisMc6800::decodeBitNumber(DisMemory &memory, InsnMc6800 &insn) {
    uint8_t val8;
    if (insn.readByte(memory, val8)) return setError(NO_MEMORY);
    const bool aim = (insn.opCode() & 0xF) == 1;
    const int8_t bitNum = bitNumber(aim ? ~val8 : val8);
    if (bitNum >= 0) {
        val8 = bitNum;
        if (TableMc6800.searchOpCodeAlias(insn))
            return setError(TableMc6800.getError());
    } else {
        *_operands++ = '#';
    }
    outConstant(val8);
    return setOK();
}

Error DisMc6800::decodeOperand(DisMemory &memory, InsnMc6800 &insn, AddrMode mode) {
    switch (mode) {
    case M_DIR: return decodeDirectPage(memory, insn);
    case M_EXT: return decodeExtended(memory, insn);
    case M_IDX:
    case M_IDY: return decodeIndexed(memory, insn, mode);
    case M_REL: return decodeRelative(memory, insn);
    case M_IMM: return decodeImmediate(memory, insn);
    case M_BMM: return decodeBitNumber(memory, insn);
    default:    return OK;
    }
}

Error DisMc6800::decode(
    DisMemory &memory, Insn &_insn) {
    InsnMc6800 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
    insn.setOpCode(opCode);
    if (TableMc6800.isPrefixCode(opCode)) {
        const Config::opcode_t prefix = opCode;
        if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
        insn.setOpCode(opCode, prefix);
    }
    if (TableMc6800.searchOpCode(insn))
        return setError(TableMc6800.getError());

    const AddrMode mode1 = insn.mode1();
    if (mode1 == M_NO) return setOK();
    if (decodeOperand(memory, insn, mode1)) return getError();
    const AddrMode mode2 = insn.mode2();
    if (mode2 == M_NO) return setOK();
    *_operands++ = ',';
    if (decodeOperand(memory, insn, mode2)) return getError();
    const AddrMode mode3 = insn.mode3();
    if (mode3 == M_NO) return setOK();
    *_operands++ = ',';
    return decodeOperand(memory, insn, mode3);
}

} // namespace mc6800
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
