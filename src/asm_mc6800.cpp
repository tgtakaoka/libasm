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

#include "asm_mc6800.h"
#include "table_mc6800.h"

namespace libasm {
namespace mc6800 {

bool AsmMc6800::adjustAccumulator(Insn &insn) {
    const InsnAdjust iAdjust = insn.insnAdjust();
    if (iAdjust == ADJ_ZERO) return false;
    const char *line = _scan;
    const RegName accum = _regs.parseRegName(line);
    line = skipSpaces(line + _regs.regNameLen(accum));
    if (accum == REG_A) {
        _scan = line;
        return true;
    } else if (accum == REG_B) {
        switch (iAdjust) {
        case ADJ_AB01: insn.embed(1); break;
        case ADJ_AB16: insn.embed(0x10); break;
        case ADJ_AB64: insn.embed(0x40); break;
        default: break;
        }
        _scan = line;
        return true;
    }
    return false;
}

Error AsmMc6800::encodeInherent(Insn &insn) {
    adjustAccumulator(insn);
    insn.emitInsn();
    return checkLineEnd();
 }

Error AsmMc6800::encodeDirect(Insn &insn) {
    if (adjustAccumulator(insn)) {
        if (*_scan != ',') return setError(UNKNOWN_OPERAND);
        _scan = skipSpaces(_scan + 1);
    }
    if (*_scan == '<') _scan++;
    insn.emitInsn();
    target::uintptr_t dir;
    if (getOperand16(dir)) return getError();
    // TODO: Warning if dir isn't on _direct_page.
    insn.emitByte(static_cast<uint8_t>(dir));
    return checkLineEnd();
}

Error AsmMc6800::encodeExtended(Insn &insn) {
    if (adjustAccumulator(insn)) {
        if (*_scan != ',') return setError(UNKNOWN_OPERAND);
        _scan = skipSpaces(_scan + 1);
    }
    if (*_scan == '>') _scan++;
    insn.emitInsn();
    target::uintptr_t addr;
    if (getOperand16(addr)) return getError();
    insn.emitUint16(addr);
    return checkLineEnd();
}

Error AsmMc6800::encodeIndexed(Insn &insn) {
    if (adjustAccumulator(insn)) {
        if (*_scan != ',') return setError(UNKNOWN_OPERAND);
        _scan = skipSpaces(_scan + 1);
    }
    insn.emitInsn();
    uint8_t disp8 = 0;          // accept ",X" as "0,X"
    if (*_scan != ',') {
        if (getOperand8(disp8)) return getError();
        _scan = skipSpaces(_scan);
    }
    insn.emitByte(disp8);
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan = skipSpaces(_scan + 1);
    if (_regs.parseRegName(_scan) != REG_X)
        return setError(UNKNOWN_OPERAND);
    _scan += _regs.regNameLen(REG_X);
    return checkLineEnd();
}

Error AsmMc6800::encodeRelative(Insn &insn) {
    target::uintptr_t addr;
    if (getOperand16(addr)) return getError();
    if (getError() == UNDEFINED_SYMBOL) addr = insn.address();
    const target::uintptr_t base = insn.address() + 2;
    const target::ptrdiff_t delta = addr - base;
    insn.emitInsn();
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.emitByte(static_cast<uint8_t>(delta));
    return checkLineEnd();
}

Error AsmMc6800::encodeImmediate(Insn &insn) {
    if (adjustAccumulator(insn)) {
        if (*_scan != ',') return setError(UNKNOWN_OPERAND);
        _scan = skipSpaces(_scan + 1);
    }
    if (*_scan != '#') return setError(UNKNOWN_OPERAND);
    _scan++;
    insn.emitInsn();
    if (insn.oprSize() == SZ_BYTE) {
        uint8_t val8;
        if (getOperand8(val8)) return getError();
        insn.emitByte(val8);
    } else if (insn.oprSize() == SZ_WORD) {
        uint16_t val16;
        if (getOperand16(val16)) return getError();
        insn.emitUint16(val16);
    } else {
        return setError(UNKNOWN_OPERAND);
    }
    return checkLineEnd();
}

Error AsmMc6800::determineAddrMode(const char *line, Insn &insn) {
    RegName reg = _regs.parseRegName(line);
    if (reg == REG_A || reg == REG_B) {
        line = skipSpaces(line + _regs.regNameLen(reg));
        if (*line != ',') {
            insn.setAddrMode(ACC);
            return OK;
        }
        line = skipSpaces(line + 1);
    }
    if (*line == 0 || *line == ';') {
        insn.setAddrMode(INH);
        return OK;
    }
    if (*line == '#') {
        insn.setAddrMode(IMM);
        return OK;
    }
    host::int_t size = -1;
    if (*line == '<') {
        size = 8;
        line++;
    } else if (*line == '>') {
        size = 16;
        line++;
    }
    uint16_t val16 = 0;         // accept ",X" as "0,X"
    if (*line != ',') {
        const char *saved_scan = _scan;
        _scan = line;
        const Error error = getOperand16(val16);
        line = _scan;
        _scan = saved_scan;
        if (error != OK) return error;
    }
    if (*line == ',' && _regs.parseRegName(skipSpaces(line + 1)) == REG_X) {
        insn.setAddrMode(IDX);
        return OK;
    }
    if (size == 8 || (size == -1 && val16 < 0x100)) {
        insn.setAddrMode(DIR);
    } else {
        insn.setAddrMode(EXT);
    }
    return OK;
}

Error AsmMc6800::encode(Insn &insn) {
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    if (TableMc6800.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpaces(endName);

    if (insn.addrMode() == REL)
        return encodeRelative(insn);

    if (determineAddrMode(_scan, insn)) return getError();

    if (TableMc6800.searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);
    switch (insn.addrMode()) {
    case INH:
    case ACC: return encodeInherent(insn);
    case DIR: return encodeDirect(insn);
    case EXT: return encodeExtended(insn);
    case IDX: return encodeIndexed(insn);
    case IMM: return encodeImmediate(insn);
    default:  return setError(UNKNOWN_OPERAND);
    }
}

} // namespace m6800
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
