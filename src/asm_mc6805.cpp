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

#include "asm_mc6805.h"

namespace libasm {
namespace mc6805 {

Error AsmMc6805::checkAddressRange(Config::uintptr_t addr) {
    const uint8_t pc_bits = (_pc_bits > 0 && _pc_bits <= 16) ? _pc_bits : 13;
    const Config::uintptr_t max = (1 << pc_bits);
    if (max && addr >= max)
        return setError(OVERFLOW_RANGE);
    return OK;
}

Error AsmMc6805::parseOperand(StrScanner &scan, Operand &op) {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p)) {
        op.mode = M_NO;
        return OK;
    }

    if (p.expect(',')) {
        const RegName reg = RegMc6805::parseRegName(p);
        if (reg == REG_X) {
            op.mode = M_IX0;
            scan = p;
            return OK;
        }
        return op.setError(scan, UNKNOWN_OPERAND);
    }

    const bool immediate = p.expect('#');
    op.size = 0;
    if (p.expect('<')) {
        op.size = 8;
    } else if (p.expect('>')) {
        op.size = 16;
    }
    op.val16 = parseExpr16(p, op);
    if (parserError())
        return getError();
    if (immediate) {
        op.mode = M_IMM;
        scan = p;
        return OK;
    }

    StrScanner a(p);
    if (a.skipSpaces().expect(',')) {
        const RegName reg = RegMc6805::parseRegName(a.skipSpaces());
        if (reg == REG_X) {
            if (op.size == 8) {
                op.mode = M_IDX;
            } else if (op.size == 16) {
                op.mode = M_IX2;
            } else if (op.val16 == 0) {
                op.mode = M_IX0;
            } else {
                op.mode = (op.val16 < 0x100) ? M_IDX : M_IX2;
            }
            scan = a;
            return OK;
        }
    }
    if (op.size == 0) {
        if (op.val16 < 8)
            op.mode = M_BNO;
        else if (op.val16 < 0x100)
            op.mode = M_DIR;
        else
            op.mode = M_EXT;
    } else {
        op.mode = (op.size == 8) ? M_DIR : M_EXT;
    }
    scan = p;
    return OK;
}

Error AsmMc6805::emitRelative(InsnMc6805 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + insn.length() + 1;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    if (checkAddressRange(target))
        return getError();
    const Config::ptrdiff_t delta = target - base;
    if (overflowRel8(delta))
        return setError(op, OPERAND_TOO_FAR);
    insn.emitByte(static_cast<uint8_t>(delta));
    return OK;
}

Error AsmMc6805::emitBitNumber(InsnMc6805 &insn, const Operand &op) {
    const uint8_t imm = 1 << op.val16;
    const bool aim = (insn.opCode() & 0xF) == 1;
    insn.emitByte(aim ? ~imm : imm);
    return OK;
}

Error AsmMc6805::emitOperand(InsnMc6805 &insn, AddrMode mode, const Operand &op) {
    switch (mode) {
    case M_DIR:
    case M_IDX:
        if (op.val16 >= 256)
            return setError(op, OVERFLOW_RANGE);
        insn.emitByte(static_cast<uint8_t>(op.val16));
        return OK;
    case M_IX2:
        insn.emitUint16(op.val16);
        return OK;
    case M_EXT:
        if (checkAddressRange(op.val16))
            return getError();
        insn.emitUint16(op.val16);
        return OK;
    case M_REL:
        return emitRelative(insn, op);
    case M_IMM:
        if (overflowUint8(op.val16))
            return setError(op, OVERFLOW_RANGE);
        insn.emitByte(op.val16);
        return OK;
    case M_BNO:  // handled in encode(Insn)
    default:
        return OK;
    }
}

Error AsmMc6805::encode(StrScanner &scan, Insn &_insn) {
    InsnMc6805 insn(_insn);
    insn.setName(_parser.readSymbol(scan));

    Operand op1, op2, op3;
    if (parseOperand(scan, op1))
        return getError();
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, op2))
            return getError();
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, op3))
            return getError();
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(op1);
    setErrorIf(op2);
    setErrorIf(op3);

    insn.setAddrMode(op1.mode, op2.mode, op3.mode);
    if (TableMc6805.searchName(insn))
        return setError(TableMc6805.getError());

    if (insn.mode1() == M_BNO)
        insn.embed((op1.val16 & 7) << 1);
    insn.emitInsn();
    if (emitOperand(insn, insn.mode1(), op1))
        goto error;
    if (emitOperand(insn, insn.mode2(), op2))
        goto error;
    if (emitOperand(insn, insn.mode3(), op3)) {
    error:
        insn.reset();
        return getError();
    }
    return getError();
}

}  // namespace mc6805
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
