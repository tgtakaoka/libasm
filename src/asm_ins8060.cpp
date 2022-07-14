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
        return OK;
    }
} functionParser;

AsmIns8060::AsmIns8060() : Assembler(_parser, TableIns8060::TABLE), _parser('$') {
    _parser.setFuncParser(&functionParser);
}

Error AsmIns8060::encodeRel8(InsnIns8060 &insn, const Operand &op) {
    Config::ptrdiff_t delta;
    if (op.mode == M_DISP) {
        delta = static_cast<Config::ptrdiff_t>(op.val16);
        if (overflowRel8(delta))
            return setError(op, OVERFLOW_RANGE);
        insn.embed(RegIns8060::encodePointerReg(op.reg));
    } else {
        // PC points the last byte of instruction.
        const auto base = insn.address() + 1;
        // PC will be incremented before fetching next instruction.
        const uint8_t fetch = (insn.addrMode() == M_REL8) ? 1 : 0;
        const auto target = op.getError() ? base : op.val16;
        // Program space is paged by 4kB.
        if (page(target) != page(base))
            return setError(op, OVERWRAP_PAGE);
        const int16_t diff = offset(target - fetch) - offset(base);
        // Sign extends 12-bit number.
        delta = (diff & 0x7FF) - (diff & 0x800);
        // delta -128 is for E reg.
        if (delta == -128 || overflowRel8(delta))
            return setError(op, OPERAND_TOO_FAR);
        if (op.getError())
            delta = 0;
        insn.embed(RegIns8060::encodePointerReg(REG_PC));
    }
    insn.emitInsn();
    insn.emitByte(delta);
    return getError();
}

Error AsmIns8060::encodeIndx(InsnIns8060 &insn, const Operand &op) {
    if (op.mode == M_REL8)
        return encodeRel8(insn, op);
    insn.embed(RegIns8060::encodePointerReg(op.reg));
    if (op.mode == M_INDX) {  // auto displacement mode
        if (insn.addrMode() != M_INDX)
            return setError(op, OPERAND_NOT_ALLOWED);
        insn.embed(4);
    }
    if (overflowRel8(static_cast<int16_t>(op.val16)))
        return setError(op, OVERFLOW_RANGE);
    insn.emitInsn();
    insn.emitByte(op.val16);
    return getError();
}

Error AsmIns8060::parseOperand(StrScanner &scan, Operand &op) {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p)) {
        op.mode = M_INHR;
        return OK;
    }

    const bool autoDisp = p.expect('@');
    const RegName reg = RegIns8060::parseRegName(p);
    if (reg == REG_E) {
        op.val16 = -128;
    } else if (reg != REG_UNDEF) {
        op.mode = M_PNTR;
        op.reg = reg;
        scan = p;
        return OK;
    } else {
        op.val16 = parseExpr16(p, op);
        if (parserError())
            return getError();
    }
    if (p.skipSpaces().expect('(')) {
        const RegName base = RegIns8060::parseRegName(p);
        if (!RegIns8060::isPointerReg(base))
            return setError(p, UNKNOWN_OPERAND);
        if (!p.expect(')'))
            return setError(p, MISSING_CLOSING_PAREN);
        scan = p;
        op.reg = base;
        op.mode = autoDisp ? M_INDX : M_DISP;
        return OK;
    }

    if (autoDisp || reg == REG_E)
        return setError(scan, UNKNOWN_OPERAND);
    scan = p;
    op.mode = M_REL8;  // May be M_IMM8 too
    return OK;
}

Error AsmIns8060::encode(StrScanner &scan, Insn &_insn) {
    InsnIns8060 insn(_insn);
    insn.setName(_parser.readSymbol(scan));

    Operand op;
    if (parseOperand(scan, op))
        return getError();
    if (!endOfLine(*scan.skipSpaces()))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(op);

    insn.setAddrMode(op.mode);
    if (TableIns8060::TABLE.searchName(insn))
        return setError(TableIns8060::TABLE.getError());

    switch (insn.addrMode()) {
    case M_INHR:
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
            return setError(op, OVERFLOW_RANGE);
        insn.emitInsn();
        insn.emitByte(op.val16);
        break;
    default:
        break;
    }
    if (insn.length() > 0 && page(insn.address()) != page(insn.address() + insn.length() - 1)) {
        setError(OVERWRAP_PAGE);
        insn.reset();
    }
    return getError();
}

}  // namespace ins8060
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
