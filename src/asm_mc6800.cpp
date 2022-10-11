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

Error AsmMc6800::parseOperand(StrScanner &scan, Operand &op) const {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p) || *p == ',') {
        op.mode = M_NO;
        scan = p;
        return OK;
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
        return op.getError();
    if (immediate) {
        op.mode = M_IM16;
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
    if (op.size == 0) {
        if (op.val16 < 8)
            op.mode = M_BIT;
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

Error AsmMc6800::emitRelative(InsnMc6800 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + (insn.length() == 0 ? 2 : insn.length() + 1);
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    const Config::ptrdiff_t delta = target - base;
    if (overflowRel8(delta))
        return setError(op, OPERAND_TOO_FAR);
    insn.emitOperand8(delta);
    return OK;
}

Error AsmMc6800::emitImmediate(InsnMc6800 &insn, AddrMode mode, const Operand &op) {
    if (mode == M_GN16 || mode == M_IM16) {
        insn.emitOperand16(op.val16);
    } else {
        if (overflowUint8(op.val16))
            return setError(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
    }
    return OK;
}

Error AsmMc6800::emitBitNumber(InsnMc6800 &insn, const Operand &op) {
    const uint8_t imm = 1 << op.val16;
    const bool aim = (insn.opCode() & 0xF) == 1;
    insn.emitOperand8(aim ? ~imm : imm);
    return OK;
}

Error AsmMc6800::emitOperand(InsnMc6800 &insn, AddrMode mode, const Operand &op) {
    switch (mode) {
    case M_GN8:
    case M_GN16:
        if (op.mode == M_DIR || op.mode == M_BIT) {
            insn.embed(0x10);
            insn.emitOperand8(op.val16);
        } else if (op.mode == M_IDX || op.mode == M_IDY) {
            if (op.val16 >= 256)
                return setError(op, OVERFLOW_RANGE);
            insn.embed(0x20);
            insn.emitOperand8(op.val16);
        } else if (op.mode == M_EXT) {
            insn.embed(0x30);
            insn.emitOperand16(op.val16);
        } else {
            emitImmediate(insn, mode, op);
        }
        return OK;
    case M_GMEM:
        if (op.mode == M_IDX) {
            if (op.val16 >= 256)
                return setError(op, OVERFLOW_RANGE);
            insn.emitOperand8(op.val16);
        } else {
            insn.embed(0x70);
            insn.emitOperand16(op.val16);
        }
        return OK;
    case M_DIR:
    case M_IDX:
    case M_IDY:
        if (op.val16 >= 256)
            return setError(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
        return OK;
    case M_EXT:
        insn.emitOperand16(op.val16);
        return OK;
    case M_REL:
        return emitRelative(insn, op);
    case M_IM8:
    case M_IM16:
    case M_BMM:
        return emitImmediate(insn, mode, op);
    case M_BIT:
        return emitBitNumber(insn, op);
    default:
        return OK;
    }
}

Error AsmMc6800::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnMc6800 insn(_insn);
    insn.nameBuffer().text(_parser.readSymbol(scan));

    Operand op1, op2, op3;
    if (parseOperand(scan, op1) && op1.hasError())
        return setError(op1);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, op2) && op2.hasError())
            return setError(op2);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, op3) && op3.hasError())
            return setError(op3);
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(op1);
    setErrorIf(op2);
    setErrorIf(op3);

    insn.setAddrMode(op1.mode, op2.mode, op3.mode);
    const auto error = TableMc6800::TABLE.searchName(insn);
    if (error)
        return setError(op1, error);

    if (emitOperand(insn, insn.mode1(), op1))
        goto error;
    if (emitOperand(insn, insn.mode2(), op2))
        goto error;
    if (emitOperand(insn, insn.mode3(), op3)) {
    error:
        insn.reset();
        return getError();
    }
    insn.emitInsn();
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
