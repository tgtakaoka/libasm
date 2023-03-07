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

const char AsmMc6805::OPT_INT_PCBITS[] = "pc-bits";
const char AsmMc6805::OPT_DESC_PCBITS[] = "program counter width in bit, default 13";

AddressWidth AsmMc6805::addressWidth() const {
    return AddressWidth(_pc_bits == 0 ? 13 : _pc_bits);
}

Error AsmMc6805::parseOperand(StrScanner &scan, Operand &op) const {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p)) {
        op.mode = M_NONE;
        return OK;
    }

    if (p.expect('#')) {
        op.val16 = parseExpr16(p, op);
        if (parserError())
            return op.getError();
        op.mode = M_IMM;
        scan = p;
        return OK;
    }

    if (p.expect(',')) {
        const auto reg = RegMc6805::parseRegName(p);
        if (reg == REG_X) {
            op.mode = M_IX0;
            scan = p;
            return OK;
        }
        return op.setError(scan, UNKNOWN_OPERAND);
    }

    op.size = 0;
    if (p.expect('<')) {
        op.size = 8;
    } else if (p.expect('>')) {
        op.size = 16;
    }
    op.val16 = parseExpr16(p, op);
    if (parserError())
        return op.getError();

    StrScanner a(p);
    if (a.skipSpaces().expect(',')) {
        const auto reg = RegMc6805::parseRegName(a.skipSpaces());
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

void AsmMc6805::emitRelative(InsnMc6805 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + insn.length() + (insn.length() == 0 ? 2 : 1);
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    checkAddress(target, op);
    const Config::ptrdiff_t delta = target - base;
    if (overflowRel8(delta))
        setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand8(delta);
}

void AsmMc6805::emitBitNumber(InsnMc6805 &insn, const Operand &op) {
    const uint8_t imm = 1 << (op.val16 & 7);
    const auto aim = (insn.opCode() & 0xF) == 1;
    insn.emitOperand8(aim ? ~imm : imm);
}

void AsmMc6805::emitOperand(InsnMc6805 &insn, AddrMode mode, const Operand &op) {
    switch (mode) {
    case M_GEN:
        insn.setOpCode(insn.opCode() & 0x0F);
        switch (op.mode) {
        case M_IMM:
            insn.embed(0xA0);
            goto imm;
        case M_DIR:
        case M_BNO:
            insn.embed(0xB0);
            goto dir;
        case M_EXT:
            insn.embed(0xC0);
            goto ext;
        case M_IX2:
            insn.embed(0xD0);
            goto ix2;
        case M_IDX:
            insn.embed(0xE0);
            goto idx;
        default:  // M_IX0
            insn.embed(0xF0);
            break;
        }
        break;
    case M_MEM:
        insn.setOpCode(insn.opCode() & 0x0F);
        switch (op.mode) {
        case M_DIR:
        case M_BNO:
            insn.embed(0x30);
            goto dir;
        case M_IDX:
            insn.embed(0x60);
            goto idx;
        default:  // M_IX0
            insn.embed(0x70);
            break;
        }
        break;
    case M_DIR:
    dir:
    case M_IDX:
    idx:
        if (op.val16 >= 256)
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
        break;
    case M_IX2:
    ix2:
        insn.emitOperand16(op.val16);
        break;
    case M_EXT:
    ext:
        checkAddress(op.val16, op);
        insn.emitOperand16(op.val16);
        break;
    case M_REL:
        return emitRelative(insn, op);
    case M_IMM:
    imm:
        if (overflowUint8(op.val16))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
        break;
    case M_BNO:  // handled in encodeImpl(Insn)
    default:
        break;
    }
}

Error AsmMc6805::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnMc6805 insn(_insn);
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
    const auto error = TableMc6805::TABLE.searchName(insn);
    if (error)
        return setError(op1, error);

    if (insn.mode1() == M_BNO)
        insn.embed((op1.val16 & 7) << 1);
    emitOperand(insn, insn.mode1(), op1);
    emitOperand(insn, insn.mode2(), op2);
    emitOperand(insn, insn.mode3(), op3);
    insn.emitInsn();
    return setErrorIf(insn);
}

}  // namespace mc6805
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
