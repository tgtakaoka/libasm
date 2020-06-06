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

bool DisMc6800::outAccumulator(InsnMc6800 &insn) {
    Config::opcode_t opc = insn.opCode();

    if (insn.insnAdjust() == ADJ_AB01) {
        opc &= 1;
    } else if (insn.insnAdjust() == ADJ_AB16) {
        opc &= 0x10;
    } else if (insn.insnAdjust() == ADJ_AB64) {
        opc &= 0x40;
    } else {
        return false;
    }
    const RegName regName = (opc == 0) ? REG_A : REG_B;
    if (_accDelim) {
        outRegister(regName);
        return true;
    } else {
        insn.appendRegister(regName, _regs);
        return false;
    }
}

Error DisMc6800::decodeInherent(InsnMc6800& insn) {
    outAccumulator(insn);
    return setOK();
}

Error DisMc6800::decodeDirectPage(
    DisMemory &memory, InsnMc6800& insn) {
    uint8_t dir;
    if (insn.readByte(memory, dir)) return setError(NO_MEMORY);
    if (outAccumulator(insn)) *_operands++ = _accDelim;
    const char *label = lookup(dir);
    if (label) {
        *_operands ++ = '<';
        outText(label);
    } else {
        outConstant(dir, 16, false);
    }
    return setOK();
}

Error DisMc6800::decodeExtended(
    DisMemory &memory, InsnMc6800 &insn) {
    Config::uintptr_t addr;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    if (outAccumulator(insn)) *_operands++ = _accDelim;
    if (addr < 0x100) *_operands++ = '>';
    outConstant(addr, 16, false);
    return setOK();
}

Error DisMc6800::decodeIndexed(
    DisMemory &memory, InsnMc6800 &insn) {
    uint8_t disp8;
    if (insn.readByte(memory, disp8)) return setError(NO_MEMORY);
    if (outAccumulator(insn)) *_operands++ = _accDelim;
    outConstant(disp8, 10);
    *_operands++ = ',';
    const AddrMode mode = insn.addrMode();
    if (mode == IDY || mode == IDY_IMM || mode == IDY_IMM_REL) {
        outRegister(REG_Y);
    } else {
        outRegister(REG_X);
    }
    return setOK();
}

Error DisMc6800::decodeRelative(
    DisMemory &memory, InsnMc6800 &insn) {
    uint8_t delta8;
    if (insn.readByte(memory, delta8)) return setError(NO_MEMORY);
    const Config::uintptr_t target =
        insn.address() + insn.length() + static_cast<int8_t>(delta8);
    outRelativeAddr(target, insn.address());
    return setOK();
}

Error DisMc6800::decodeImmediate(
    DisMemory &memory, InsnMc6800 &insn) {
    if (outAccumulator(insn)) *_operands++ = _accDelim;
    *_operands++ = '#';
    if (insn.oprSize() == SZ_BYTE) {
        uint8_t val8;
        if (insn.readByte(memory, val8)) return setError(NO_MEMORY);
        outConstant(val8);
    } else if (insn.oprSize() == SZ_WORD) {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        outConstant(val16);
    } else {
        return setError(UNKNOWN_INSTRUCTION);
    }
    return setOK();
}

static int8_t decodeBitNumber(uint8_t val, Config::opcode_t opCode) {
    if ((opCode & 0xF) == 1) val = ~val; // AIM
    for (uint8_t pos = 0, mask = 0x01; pos < 8; pos++, mask <<= 1) {
        if (val == mask) return pos;
    }
    return -1;
}

Error DisMc6800::decodeBitOperation(
    DisMemory &memory, InsnMc6800 &insn) {
    const AddrMode mode = insn.addrMode();
    if (mode == DIR_IMM || mode == DIR_IMM_REL) {
        decodeDirectPage(memory, insn);
    } else if (mode == IDX_IMM || mode == IDY_IMM
               || mode == IDX_IMM_REL || mode == IDY_IMM_REL) {
        decodeIndexed(memory, insn);
    } else {
        uint8_t val8;
        if (insn.readByte(memory, val8)) return setError(NO_MEMORY);
        const int8_t bitNum = decodeBitNumber(val8, insn.opCode());
        if (bitNum >= 0) {
            val8 = bitNum;
            if (TableMc6800.searchOpCodeAlias(insn))
                return setError(TableMc6800.getError());
        } else {
            *_operands++ = '#';
        }
        outConstant(val8);
    }
    *_operands++ = ',';
    if (mode == IMM_DIR || mode == BIT_DIR) {
        return decodeDirectPage(memory, insn);
    } else if (mode == IMM_IDX) {
        return decodeIndexed(memory, insn);
    } else {
        decodeImmediate(memory, insn);
        if (mode == DIR_IMM || mode == IDX_IMM || mode == IDY_IMM)
            return OK;
        *_operands++ = ',';
        return decodeRelative(memory, insn);
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

    switch (insn.addrMode()) {
    case INH: return decodeInherent(insn);
    case DIR: return decodeDirectPage(memory, insn);
    case EXT: return decodeExtended(memory, insn);
    case IDX:
    case IDY:
        return decodeIndexed(memory, insn);
    case REL: return decodeRelative(memory, insn);
    case IMM: return decodeImmediate(memory, insn);
    case IMM_DIR:
    case IMM_IDX:
    case BIT_DIR:
    case BIT_IDX:
    case DIR_IMM:
    case IDX_IMM:
    case IDY_IMM:
    case DIR_IMM_REL:
    case IDX_IMM_REL:
    case IDY_IMM_REL:
        return decodeBitOperation(memory, insn);
    default: break;
    }
    return setError(INTERNAL_ERROR);
}

} // namespace mc6800
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
