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

#include "table_ins8070.h"
#include "text_common.h"

namespace libasm {
namespace ins8070 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_dequal, &Assembler::defineOrigin},
    {TEXT_dASCII, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_dBYTE,  &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_dDBYTE, &Assembler::defineDataConstant, Assembler::DATA_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

struct Ins8070FunctionTable final : FunctionTable, Singleton<Ins8070FunctionTable> {
    const Functor *lookupFunction(const StrScanner &) const override;
};

}  // namespace

const ValueParser::Plugins &AsmIns8070::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return Ins80xxNumberParser::singleton(); }
        const SymbolParser &symbol() const override { return Ins80xxSymbolParser::singleton(); }
        const LocationParser &location() const override {
            return DotDollarLocationParser::singleton();
        }
        const FunctionTable &function() const override { return Ins8070FunctionTable::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmIns8070::AsmIns8070(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

namespace {

const struct : Functor {
    int_fast8_t nargs() const override { return 1; }
    Error eval(ValueStack &stack, ParserContext &, uint_fast8_t) const override {
        stack.pushUnsigned((stack.pop().getUnsigned() >> 8) & 0xFF);
        return OK;
    }
} FN_HIGH;

const struct : Functor {
    int_fast8_t nargs() const override { return 1; }
    Error eval(ValueStack &stack, ParserContext &, uint_fast8_t) const override {
        stack.pushUnsigned(stack.pop().getUnsigned() & 0xFF);
        return OK;
    }
} FN_LOW;

const struct : Functor {
    int_fast8_t nargs() const override { return 1; }
    Error eval(ValueStack &stack, ParserContext &, uint_fast8_t) const override {
        stack.pushUnsigned((stack.pop().getUnsigned() - 1) & 0xFFFF);
        return OK;
    }
} FN_ADDR;

const Functor *Ins8070FunctionTable::lookupFunction(const StrScanner &name) const {
    if (name.iequals_P(TEXT_FN_H))
        return &FN_HIGH;
    if (name.iequals_P(TEXT_FN_L))
        return &FN_LOW;
    if (name.iequals_P(TEXT_FN_ADDR))
        return &FN_ADDR;
    return nullptr;
}

}  // namespace

void AsmIns8070::emitAbsolute(AsmInsn &insn, const Operand &op) const {
    // PC will be +1 before fetching instruction.
    const auto target = op.getError() ? 0 : op.val.getUnsigned() - 1;
    insn.emitOperand16(target);
}

void AsmIns8070::emitImmediate(AsmInsn &insn, const Operand &op) const {
    if (insn.oprSize() == SZ_WORD) {
        insn.emitOperand16(op.val.getUnsigned());
    } else {
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val.getUnsigned());
    }
}

void AsmIns8070::emitRelative(AsmInsn &insn, const Operand &op) const {
    const auto base = insn.address() + 1;
    // PC will be +1 before feting instruction
    const auto fetch = insn.execute() ? 1 : 0;
    const auto target = (op.getError() ? base + fetch : op.val.getUnsigned()) - fetch;
    const auto offset = branchDelta(base, target, insn, op);
    if (overflowDelta(offset, 8))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand8(offset);
}

void AsmIns8070::emitGeneric(AsmInsn &insn, const Operand &op) const {
    if (op.mode == M_IMM) {
        insn.embed(4);
        emitImmediate(insn, op);
        return;
    }
    if ((op.mode == M_ADR || op.mode == M_VEC) && op.reg == REG_UNDEF) {
        const auto target = op.getError() ? 0xFF00 : op.val.getUnsigned();
        if (target < 0xFF00) {
            emitRelative(insn, op);
            return;
        }
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
    if (op.val.overflowInt8())
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.emitOperand8(op.val.getSigned());
}

void AsmIns8070::emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_P23:
    case M_PTR:
        insn.embed(encodePointerReg(op.reg));
        break;
    case M_VEC:
        insn.embed(op.val.getUnsigned() & 0x0F);
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
        op.val = parseInteger(p, op);
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
    op.val = parseInteger(p, op);
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
        if (ptr == REG_UNDEF)
            return op.setError(ptrp, UNKNOWN_OPERAND);
        if (!isPointerReg(ptr))
            return op.setError(ptrp, REGISTER_NOT_ALLOWED);
        if (autoIndex && (ptr == REG_PC || ptr == REG_SP))
            return op.setError(ptrp, REGISTER_NOT_ALLOWED);
        op.reg = ptr;
        op.autoIndex = autoIndex;
        op.mode = (autoIndex || ptr == REG_SP || ptr == REG_PC) ? M_GEN : M_IDX;
        scan = p;
        return OK;
    }

    op.mode = op.val.overflow(15) ? M_ADR : M_VEC;
    scan = p;
    return OK;
}

Error AsmIns8070::defineAddrConstant(StrScanner &scan, Insn &insn) {
    do {
        auto p = scan.skipSpaces();
        const auto value = parseInteger(p, insn);
        if (insn.hasError())
            break;
        const auto v = value.getUnsigned();
        insn.emitUint16Le((v - 1) & 0xFFFF);
        scan = p;
    } while (scan.skipSpaces().expect(',') && insn.isOK());
    return insn.getError();
}

Error AsmIns8070::processPseudo(StrScanner &scan, Insn &insn) {
    if (strcasecmp_P(insn.name(), TEXT_dADDR) == 0)
        return defineAddrConstant(scan, insn);
    return Assembler::processPseudo(scan, insn);
}

Error AsmIns8070::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
        return _insn.setError(insn.dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
            return _insn.setError(insn.srcOp);
        scan.skipSpaces();
    }

    if (_insn.setErrorIf(insn.dstOp, TABLE.searchName(cpuType(), insn)))
        return _insn.getError();

    emitOperand(insn, insn.dst(), insn.dstOp);
    emitOperand(insn, insn.src(), insn.srcOp);
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace ins8070
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
