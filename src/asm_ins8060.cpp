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

#include "reg_ins8060.h"
#include "table_ins8060.h"
#include "text_common.h"

namespace libasm {
namespace ins8060 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

constexpr Pseudo PSEUDOS[] PROGMEM = {
        Pseudo{TEXT_dequal, &Assembler::defineOrigin},
        Pseudo{TEXT_dASCII, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
        Pseudo{TEXT_dBYTE, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
        Pseudo{TEXT_dDBYTE, &Assembler::defineDataConstant, Assembler::DATA_WORD},
};

struct Ins8060SymbolParser final : PrefixSymbolParser {
    Ins8060SymbolParser() : PrefixSymbolParser(SymbolParser::DOLLAR) {}
    bool instructionLetter(char c) const override {
        return PrefixSymbolParser::instructionLetter(c) || c == '=' || c == '.';
    }
    bool instructionTerminator(char c) const override { return c == '='; }
};

struct Ins8060FunctionTable final : FunctionTable {
    const Functor *lookupFunction(const StrScanner &) const override;
};

}  // namespace

struct AsmIns8060::Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    RegName index;
    uint16_t val16;
    Operand() : mode(M_NONE), reg(REG_UNDEF), index(REG_UNDEF), val16(0) {}
};

const ValueParser::Plugins &AsmIns8060::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return NationalNumberParser::singleton(); }
        const SymbolParser &symbol() const override { return _symbol; }
        const LocationParser &location() const override {
            return NationalLocationParser::singleton();
        }
        const FunctionTable &function() const override { return _function; }
        const Ins8060SymbolParser _symbol{};
        const Ins8060FunctionTable _function{};
    } PLUGINS{};
    return PLUGINS;
}

AsmIns8060::AsmIns8060(const ValueParser::Plugins &plugins)
    : Assembler(plugins, ARRAY_RANGE(PSEUDOS)), Config(TABLE) {
    reset();
}

namespace {

Config::uintptr_t page(Config::uintptr_t addr) {
    return addr & ~0xFFF;
}

Config::uintptr_t offset(Config::uintptr_t addr) {
    return addr & 0xFFF;
}

const struct : Functor {
    int8_t nargs() const override { return 1; }
    Error eval(ValueStack &stack, uint8_t) const override {
        stack.pushUnsigned((stack.pop().getUnsigned() >> 8) & 0xFF);
        return OK;
    }
} FN_HIGH;

const struct : Functor {
    int8_t nargs() const override { return 1; }
    Error eval(ValueStack &stack, uint8_t) const override {
        stack.pushUnsigned(stack.pop().getUnsigned() & 0xFF);
        return OK;
    }
} FN_LOW;

const struct : Functor {
    int8_t nargs() const override { return 1; }
    Error eval(ValueStack &stack, uint8_t) const override {
        const auto v = stack.pop().getUnsigned();
        stack.pushUnsigned(page(v) | offset(v - 1));
        return OK;
    }
} FN_ADDR;

const Functor *Ins8060FunctionTable::lookupFunction(const StrScanner &name) const {
    if (name.iequals_P(TEXT_FN_H))
        return &FN_HIGH;
    if (name.iequals_P(TEXT_FN_L))
        return &FN_LOW;
    if (name.iequals_P(TEXT_FN_ADDR))
        return &FN_ADDR;
    return nullptr;
}

}  // namespace

void AsmIns8060::encodeRel8(AsmInsn &insn, const Operand &op) {
    Config::ptrdiff_t delta;
    if (op.mode == M_DISP) {
        if (op.index == REG_E) {
            delta = -128;
        } else {
            delta = static_cast<Config::ptrdiff_t>(op.val16);
            if (overflowInt8(delta) || delta == -128)
                setErrorIf(op, OVERFLOW_RANGE);
        }
        insn.embed(encodePointerReg(op.reg));
    } else {
        // PC points the last byte of instruction.
        const Config::uintptr_t base = insn.address() + 1;
        // PC will be incremented before fetching next instruction.
        const uint8_t fetch = (insn.addrMode() == M_REL8) ? 1 : 0;
        const Config::uintptr_t target = op.getError() ? base : op.val16;
        // Program space is paged by 4kB.
        if (page(target) != page(base))
            setErrorIf(op, OVERWRAP_PAGE);
        const auto diff = offset(target - fetch) - offset(base);
        // Sign extends 12-bit number.
        delta = signExtend(diff, 12);
        // delta -128 is for E reg.
        if (overflowInt8(delta) || delta == -128)
            setErrorIf(op, OPERAND_TOO_FAR);
        if (op.getError())
            delta = 0;
        insn.embed(encodePointerReg(REG_PC));
    }
    insn.emitInsn();
    insn.emitByte(delta);
}

void AsmIns8060::encodeIndx(AsmInsn &insn, const Operand &op) {
    if (op.mode == M_REL8) {
        encodeRel8(insn, op);
        return;
    }
    insn.embed(encodePointerReg(op.reg));
    if (op.mode == M_INDX) {  // auto displacement mode
        if (insn.addrMode() != M_INDX)
            setErrorIf(op, OPERAND_NOT_ALLOWED);
        insn.embed(4);
    }
    auto disp = static_cast<Config::ptrdiff_t>(op.val16);
    if (op.index == REG_E) {
        disp = -128;
    } else if (overflowInt8(disp) || disp == -128) {
        setErrorIf(op, OVERFLOW_RANGE);
    }
    insn.emitInsn();
    insn.emitByte(disp);
}

Error AsmIns8060::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p)) {
        op.mode = M_NONE;
        return OK;
    }

    const auto autoDisp = p.expect('@');
    const auto reg = parseRegName(p);
    if (reg == REG_E) {
        op.index = REG_E;
    } else if (reg != REG_UNDEF) {
        op.mode = M_PNTR;
        op.reg = reg;
        scan = p;
        return OK;
    } else {
        op.val16 = parseExpr16(p, op);
        if (op.hasError())
            return op.getError();
    }
    if (p.skipSpaces().expect('(')) {
        const auto base = parseRegName(p);
        if (!isPointerReg(base))
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

Error AsmIns8060::defineAddrConstant(StrScanner &scan, Insn &insn) {
    do {
        auto p = scan.skipSpaces();
        ErrorAt error;
        const auto value = parseExpr(p, error);
        if (error.getError())
            return setError(error);
        const auto v = value.getUnsigned();
        insn.emitUint16Le(page(v) | offset(v - 1));
        scan = p;
    } while (scan.skipSpaces().expect(','));

    if (!endOfLine(scan.skipSpaces()))
        return setError(scan, GARBAGE_AT_END);
    return OK;
}

Error AsmIns8060::processPseudo(StrScanner &scan, Insn &insn) {
    if (strcasecmp_P(insn.name(), TEXT_dADDR) == 0)
        return defineAddrConstant(scan, insn);
    return Assembler::processPseudo(scan, insn);
}

Error AsmIns8060::encodeImpl(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    Operand op;
    if (parseOperand(scan, op) && op.hasError())
        return setError(op);
    if (!endOfLine(scan.skipSpaces()))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(op);

    insn.setAddrMode(op.mode);
    const auto error = TABLE.searchName(cpuType(), insn);
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
        insn.embed(encodePointerReg(op.reg));
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
