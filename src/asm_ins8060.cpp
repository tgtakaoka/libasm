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

#include "asm_ins8060.h"

namespace libasm {
namespace ins8060 {

static Config::uintptr_t page(Config::uintptr_t addr) {
    return addr & ~0xFFF;
}

static Config::uintptr_t offset(Config::uintptr_t addr) {
    return addr & 0xFFF;
}

static struct : public ValueParser::FuncParser {
    Error parseFunc(ValueParser &parser, const StrScanner &name, StrScanner &scan, Value &val,
            const SymbolTable *symtab) override {
        const auto v = parser.eval(scan, symtab).getUnsigned();
        if (name.iequals_P(PSTR("h"))) {
            val.setValue((v >> 8) & 0xFF);
        } else if (name.iequals_P(PSTR("l"))) {
            val.setValue(v & 0xFF);
        } else if (name.iequals_P(PSTR("addr"))) {
            val.setValue(page(v) | offset(v - 1));
        } else {
            return setError(UNKNOWN_FUNCTION);
        }
        return setOK();
    }
} functionParser;

AsmIns8060::AsmIns8060() : Assembler(_parser, TableIns8060::TABLE), _parser('$') {
    _parser.setFuncParser(&functionParser);
}

void AsmIns8060::encodeRel8(InsnIns8060 &insn, const Operand &op) {
    Config::ptrdiff_t delta;
    if (op.mode == M_DISP) {
        if (op.index == REG_E) {
            delta = -128;
        } else {
            delta = static_cast<Config::ptrdiff_t>(op.val16);
            if (overflowRel8(delta) || delta == -128)
                setErrorIf(op, OVERFLOW_RANGE);
        }
        insn.embed(RegIns8060::encodePointerReg(op.reg));
    } else {
        // PC points the last byte of instruction.
        const Config::uintptr_t base = insn.address() + 1;
        // PC will be incremented before fetching next instruction.
        const uint8_t fetch = (insn.addrMode() == M_REL8) ? 1 : 0;
        const Config::uintptr_t target = op.getError() ? base : op.val16;
        // Program space is paged by 4kB.
        if (page(target) != page(base))
            setErrorIf(op, OVERWRAP_PAGE);
        auto diff = offset(target - fetch) - offset(base);
        // Sign extends 12-bit number.
        delta = static_cast<Config::ptrdiff_t>((diff & 0x7FF) - (diff & 0x800));
        // delta -128 is for E reg.
        if (overflowRel8(delta) || delta == -128)
            setErrorIf(op, OPERAND_TOO_FAR);
        if (op.getError())
            delta = 0;
        insn.embed(RegIns8060::encodePointerReg(REG_PC));
    }
    insn.emitInsn();
    insn.emitByte(delta);
}

void AsmIns8060::encodeIndx(InsnIns8060 &insn, const Operand &op) {
    if (op.mode == M_REL8) {
        encodeRel8(insn, op);
        return;
    }
    insn.embed(RegIns8060::encodePointerReg(op.reg));
    if (op.mode == M_INDX) {  // auto displacement mode
        if (insn.addrMode() != M_INDX)
            setErrorIf(op, OPERAND_NOT_ALLOWED);
        insn.embed(4);
    }
    auto disp = static_cast<Config::ptrdiff_t>(op.val16);
    if (op.index == REG_E) {
        disp = -128;
    } else if (overflowRel8(disp) || disp == -128) {
        setErrorIf(op, OVERFLOW_RANGE);
    }
    insn.emitInsn();
    insn.emitByte(disp);
}

Error AsmIns8060::parseOperand(StrScanner &scan, Operand &op) const {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p)) {
        op.mode = M_NONE;
        return OK;
    }

    const auto autoDisp = p.expect('@');
    const auto reg = RegIns8060::parseRegName(p);
    if (reg == REG_E) {
        op.index = REG_E;
    } else if (reg != REG_UNDEF) {
        op.mode = M_PNTR;
        op.reg = reg;
        scan = p;
        return OK;
    } else {
        op.val16 = parseExpr16(p, op);
        if (parserError())
            return op.getError();
    }
    if (p.skipSpaces().expect('(')) {
        const auto base = RegIns8060::parseRegName(p);
        if (!RegIns8060::isPointerReg(base))
            return op.setError(UNKNOWN_OPERAND);
        if (!p.expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        scan = p;
        op.reg = base;
        op.mode = autoDisp ? M_INDX : M_DISP;
        return OK;
    }

    if (autoDisp || reg == REG_E)
        return op.setError(UNKNOWN_OPERAND);
    scan = p;
    op.mode = M_REL8;  // May be M_IMM8 too
    return OK;
}

Error AsmIns8060::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnIns8060 insn(_insn);
    Operand op;
    if (parseOperand(scan, op) && op.hasError())
        return setError(op);
    if (!endOfLine(*scan.skipSpaces()))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(op);

    insn.setAddrMode(op.mode);
    const auto error = TableIns8060::TABLE.searchName(insn);
    if (error)
        return setError(op, error);

    switch (insn.addrMode()) {
    case M_NONE:
        insn.emitInsn();
        break;
    case M_REL8:
        encodeRel8(insn, op);
        break;
    case M_PNTR:
        insn.embed(RegIns8060::encodePointerReg(op.reg));
        insn.emitInsn();
        break;
    case M_DISP:
    case M_INDX:
        encodeIndx(insn, op);
        break;
    case M_IMM8:
        if (overflowUint8(op.val16))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitInsn();
        insn.emitByte(op.val16);
        break;
    default:
        break;
    }
    if (insn.length() > 0 && page(insn.address()) != page(insn.address() + insn.length() - 1))
        setErrorIf(OVERWRAP_PAGE);
    return setErrorIf(insn);
}

}  // namespace ins8060
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
