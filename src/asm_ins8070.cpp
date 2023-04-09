
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

#include "reg_ins8070.h"
#include "table_ins8070.h"

namespace libasm {
namespace ins8070 {

using namespace reg;

struct AsmIns8070::Operand : public OperandBase {
    AddrMode mode;
    RegName reg;
    bool autoIndex;
    uint16_t val16;
    Operand() : mode(M_NONE), reg(REG_UNDEF), autoIndex(false), val16(0) {}
};

AsmIns8070::AsmIns8070()
    : Assembler(TableIns8070::TABLE, nullptr, _number, _comment, _symbol, _letter, _location,
              nullptr, &_function) {
    reset();
}

static const struct : Functor {
    int8_t nargs() const override { return 1; }
    Error eval(ValueStack &stack, uint8_t argc) const override {
        stack.pushUnsigned((stack.pop().getUnsigned() >> 8) & 0xFF);
        return OK;
    }
} FN_HIGH;

static const struct : Functor {
    int8_t nargs() const override { return 1; }
    Error eval(ValueStack &stack, uint8_t argc) const override {
        stack.pushUnsigned(stack.pop().getUnsigned() & 0xFF);
        return OK;
    }
} FN_LOW;

static const struct : Functor {
    int8_t nargs() const override { return 1; }
    Error eval(ValueStack &stack, uint8_t argc) const override {
        stack.pushUnsigned((stack.pop().getUnsigned() - 1) & 0xFFFF);
        return OK;
    }
} FN_ADDR;

const Functor *AsmIns8070::Ins8070FunctionParser::parseFunction(
        StrScanner &scan, ErrorAt &error) const {
    auto p = scan;
    p.trimStart([](char c) { return c != '(' && !isspace(c); });
    const auto name = StrScanner(scan.str(), p.str());
    const Functor *fn = nullptr;
    if (name.iequals_P(PSTR("H"))) {
        fn = &FN_HIGH;
    } else if (name.iequals_P(PSTR("L"))) {
        fn = &FN_LOW;
    } else if (name.iequals_P(PSTR("ADDR"))) {
        fn = &FN_ADDR;
    }
    if (fn)
        scan = p;
    return fn;
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
    const auto base = insn.address() + 1;
    // PC will be +1 before feting instruction
    const auto fetch = insn.execute() ? 1 : 0;
    const auto target = (op.getError() ? base + fetch : op.val16) - fetch;
    const auto offset = branchDelta(base, target, op);
    if (overflowInt8(offset))
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

    insn.embed(encodePointerReg(op.reg));
    if (op.autoIndex)
        insn.embed(4);
    const auto offset = static_cast<Config::ptrdiff_t>(op.val16);
    if (overflowInt8(offset))
        setErrorIf(op, OVERFLOW_RANGE);
    insn.emitOperand8(offset);
}

void AsmIns8070::emitOperand(InsnIns8070 &insn, AddrMode mode, const Operand &op) {
    switch (mode) {
    case M_P23:
    case M_PTR:
        insn.embed(encodePointerReg(op.reg));
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
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#') || p.expect('=')) {
        op.val16 = parseExpr16(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_IMM;
        scan = p;
        return OK;
    }

    const RegName reg = parseRegName(p);
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
    if (op.hasError())
        return op.getError();

    if (*p.skipSpaces() == '(')  // SC/MP style
        return op.setError(MISSING_COMMA);
    if (p.expect(',')) {
        p.skipSpaces();
        if (!autoIndex)
            autoIndex = p.expect('@');
        const auto ptrp = p;
        const auto ptr = parseRegName(p);
        if (!isPointerReg(ptr))
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
    if (!endOfLine(scan))
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
