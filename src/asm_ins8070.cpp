
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

#include "asm_ins8070.h"

namespace libasm {
namespace ins8070 {

static struct : public ValueParser::FuncParser {
    Error parseFunc(ValueParser &parser, const StrScanner &name, StrScanner &scan, Value &val,
            const SymbolTable *symtab) override {
        const auto v = parser.eval(scan, symtab).getUnsigned();
        if (name.iequals_P(PSTR("h"))) {
            val.setValue((v >> 8) & 0xFF);
        } else if (name.iequals_P(PSTR("l"))) {
            val.setValue(v & 0xFF);
        } else if (name.iequals_P(PSTR("addr"))) {
            val.setValue((v - 1) & 0xFFFF);
        } else {
            return setError(UNKNOWN_FUNCTION);
        }
        return setOK();
    }
} functionParser;

AsmIns8070::AsmIns8070() : Assembler(_parser, TableIns8070::TABLE), _parser('$') {
    _parser.setFuncParser(&functionParser);
}

void AsmIns8070::emitAbsolute(InsnIns8070 &insn, const Operand &op) {
    // PC will be +1 before fetching instruction.
    const auto target = op.getError() ? 0 : op.val16 - 1;
    insn.emitOperand16(target);
}

void AsmIns8070::emitImmediate(InsnIns8070 &insn, const Operand &op) {
    if (insn.oprSize() == SZ_WORD) {
        insn.emitOperand16(op.val16);
    } else {
        if (overflowUint8(op.val16))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
    }
}

void AsmIns8070::emitRelative(InsnIns8070 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 1;
    // PC will be +1 before feting instruction
    const uint8_t fetch = insn.execute() ? 1 : 0;
    const Config::uintptr_t target = (op.getError() ? base + fetch : op.val16) - fetch;
    const Config::ptrdiff_t offset = target - base;
    if (overflowRel8(offset))
        setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand8(offset);
}

void AsmIns8070::emitGeneric(InsnIns8070 &insn, const Operand &op) {
    if (op.mode == M_IMM) {
        insn.embed(4);
        emitImmediate(insn, op);
        return;
    }
    if ((op.mode == M_ADR || op.mode == M_VEC) && op.reg == REG_UNDEF) {
        const auto target = op.getError() ? 0xFF00 : op.val16;
        if (target < 0xFF00)
            setErrorIf(op, OVERFLOW_RANGE);
        insn.embed(5);
        insn.emitOperand8(target);
        return;
    }
    if (op.reg == REG_PC) {
        emitRelative(insn, op);
        return;
    }

    insn.embed(RegIns8070::encodePointerReg(op.reg));
    if (op.autoIndex)
        insn.embed(4);
    const auto offset = static_cast<Config::ptrdiff_t>(op.val16);
    if (overflowRel8(offset))
        setErrorIf(op, OVERFLOW_RANGE);
    insn.emitOperand8(offset);
}

void AsmIns8070::emitOperand(InsnIns8070 &insn, AddrMode mode, const Operand &op) {
    switch (mode) {
    case M_P23:
    case M_PTR:
        insn.embed(RegIns8070::encodePointerReg(op.reg));
        break;
    case M_VEC:
        insn.embed(op.val16 & 0x0F);
        break;
    case M_IMM:
        emitImmediate(insn, op);
        break;
    case M_ADR:
        emitAbsolute(insn, op);
        break;
    case M_IDX:
    case M_GEN:
        emitGeneric(insn, op);
        break;
    case M_PCR:
        emitRelative(insn, op);
        break;
    default:
        break;
    }
}

Error AsmIns8070::parseOperand(StrScanner &scan, Operand &op) const {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p))
        return OK;

    if (p.expect('#') || p.expect('=')) {
        op.val16 = parseExpr16(p, op);
        if (parserError())
            return op.getError();
        op.mode = M_IMM;
        scan = p;
        return OK;
    }

    const RegName reg = RegIns8070::parseRegName(p);
    if (reg != REG_UNDEF) {
        AddrMode mode;
        switch (reg) {
        case REG_A:
            mode = M_AR;
            break;
        case REG_E:
            mode = M_ER;
            break;
        case REG_S:
            mode = M_SR;
            break;
        case REG_EA:
            mode = M_EA;
            break;
        case REG_T:
            mode = M_TR;
            break;
        case REG_SP:
            mode = M_SP;
            break;
        case REG_PC:
            mode = M_PTR;
            break;
        default:
            mode = M_P23;
            break;
        }
        op.reg = reg;
        op.mode = mode;
        scan = p;
        return OK;
    }

    auto autoIndex = p.expect('@');
    op.val16 = parseExpr16(p, op);
    if (parserError())
        return op.getError();

    if (*p.skipSpaces() == '(')  // SC/MP style
        return op.setError(MISSING_COMMA);
    if (p.expect(',')) {
        p.skipSpaces();
        if (!autoIndex)
            autoIndex = p.expect('@');
        const auto ptrp = p;
        const auto ptr = RegIns8070::parseRegName(p);
        if (!RegIns8070::isPointerReg(ptr))
            op.setError(UNKNOWN_OPERAND);
        if (autoIndex && (ptr == REG_PC || ptr == REG_SP))
            return op.setError(ptrp, REGISTER_NOT_ALLOWED);
        op.reg = ptr;
        op.autoIndex = autoIndex;
        op.mode = (autoIndex || ptr == REG_SP || ptr == REG_PC) ? M_GEN : M_IDX;
        scan = p;
        return OK;
    }

    op.mode = (op.val16 < 0x10) ? M_VEC : M_ADR;
    scan = p;
    return OK;
}

Error AsmIns8070::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnIns8070 insn(_insn);
    Operand dst, src;
    if (parseOperand(scan, dst) && dst.hasError())
        return setError(dst);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, src) && src.hasError())
            return setError(src);
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(dst);
    setErrorIf(src);

    insn.setAddrMode(dst.mode, src.mode);
    const auto error = TableIns8070::TABLE.searchName(insn);
    if (error)
        return setError(dst, error);

    emitOperand(insn, insn.dst(), dst);
    emitOperand(insn, insn.src(), src);
    insn.emitInsn();
    return setErrorIf(insn);
}

}  // namespace ins8070
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
