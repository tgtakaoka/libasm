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

namespace libasm {
namespace mc6800 {

Error AsmMc6800::parseOperand(StrScanner &scan, Operand &op) {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p) || *p == ',') {
        op.mode = M_NO;
        scan = p;
        return OK;
    }

    const bool immediate = p.expect('#');
    op.size = SZ_NONE;
    if (p.expect('<')) {
        op.size = SZ_BYTE;
    } else if (p.expect('>')) {
        op.size = SZ_WORD;
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
        const RegName reg = RegMc6800::parseRegName(a.skipSpaces());
        if (reg != REG_UNDEF) {
            op.mode = (reg == REG_X) ? M_IDX : M_IDY;
            scan = a;
            return OK;
        }
    }
    if (op.size == SZ_NONE) {
        if (op.val16 < 8)
            op.mode = M_BIT;
        else if (op.val16 < 0x100)
            op.mode = M_DIR;
        else
            op.mode = M_EXT;
    } else {
        op.mode = (op.size == SZ_BYTE) ? M_DIR : M_EXT;
    }
    scan = p;
    return OK;
}

Error AsmMc6800::emitRelative(InsnMc6800 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + insn.length() + 1;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    const Config::ptrdiff_t delta = target - base;
    if (overflowRel8(delta))
        return setError(op, OPERAND_TOO_FAR);
    insn.emitByte(static_cast<uint8_t>(delta));
    return OK;
}

Error AsmMc6800::emitImmediate(InsnMc6800 &insn, const Operand &op) {
    if (insn.size() == SZ_BYTE) {
        if (overflowUint8(op.val16))
            return setError(op, OVERFLOW_RANGE);
        insn.emitByte(static_cast<uint8_t>(op.val16));
    }
    if (insn.size() == SZ_WORD)
        insn.emitUint16(op.val16);
    return OK;
}

Error AsmMc6800::emitBitNumber(InsnMc6800 &insn, const Operand &op) {
    const uint8_t imm = 1 << op.val16;
    const bool aim = (insn.opCode() & 0xF) == 1;
    insn.emitByte(aim ? ~imm : imm);
    return OK;
}

Error AsmMc6800::emitOperand(InsnMc6800 &insn, AddrMode mode, const Operand &op) {
    switch (mode) {
    case M_DIR:
    case M_IDX:
    case M_IDY:
        if (op.val16 >= 256)
            return setError(op, OVERFLOW_RANGE);
        insn.emitByte(static_cast<uint8_t>(op.val16));
        return OK;
    case M_EXT:
        insn.emitUint16(op.val16);
        return OK;
    case M_REL:
        return emitRelative(insn, op);
    case M_IMM:
    case M_BMM:
        return emitImmediate(insn, op);
    case M_BIT:
        return emitBitNumber(insn, op);
    default:
        return OK;
    }
}

Error AsmMc6800::encode(StrScanner &scan, Insn &_insn) {
    InsnMc6800 insn(_insn);
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
    if (TableMc6800.searchName(insn))
        return setError(TableMc6800.getError());

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

}  // namespace mc6800
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
