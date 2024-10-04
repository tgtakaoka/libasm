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

#include "asm_cdp1802.h"

#include "reg_cdp1802.h"
#include "table_cdp1802.h"
#include "text_common.h"

namespace libasm {
namespace cdp1802 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr char OPT_BOOL_USE_REGISTER[] PROGMEM = "use-register";
constexpr char OPT_DESC_USE_REGISTER[] PROGMEM = "enable register name Rn";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_DC, &Assembler::defineDataConstant, Assembler::DATA_BYTE_OR_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

/**
 * RCA style numbers are the same as IBM plus '#hh' for hexadecimal.
 */
struct RcaNumberParser final : NumberParser, Singleton<RcaNumberParser> {
    Error parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const override {
        auto p = scan;
        if (*p == '#' && isxdigit(p[1])) {
            const auto error = val.read(++p, RADIX_16);
            if (error == OK)
                scan = p;
            return error;
        }
        return _ibm.parseNumber(scan, val, defaultRadix);
    }

private:
    const IbmNumberParser _ibm{'X', 'B', 0, 'D'};
};

struct RcaCommentParser final : CommentParser, Singleton<RcaCommentParser> {
    bool commentLine(StrScanner &scan) const override {
        static constexpr char TEXT_DOTDOT[] PROGMEM = "..";
        return scan.iexpectText_P(TEXT_DOTDOT) || endOfLine(scan);
    }
    bool endOfLine(StrScanner &scan) const override {
        return SemicolonCommentParser::singleton().endOfLine(scan);
    }
};

struct RcaSymbolParser final : SymbolParser, Singleton<RcaSymbolParser> {
    bool functionNameLetter(char c) const override { return symbolLetter(c) || c == '.'; }
    bool instructionLetter(char c) const override {
        return SymbolParser::instructionLetter(c) || c == '=';
    }
    bool instructionTerminator(char c) const override { return c == '='; }
};

struct RcaLetterParser final : LetterParser, Singleton<RcaLetterParser> {
    bool letterPrefix(StrScanner &scan) const override {
        scan.iexpect('T');  // optional
        return true;
    }
    bool stringPrefix(StrScanner &scan) const override {
        scan.iexpect('T');  // optional
        return true;
    }
};

struct RcaFunctionTable final : FunctionTable, Singleton<RcaFunctionTable> {
    const Functor *lookupFunction(const StrScanner &name) const override;
};

}  // namespace

const ValueParser::Plugins &AsmCdp1802::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return RcaNumberParser::singleton(); }
        const CommentParser &comment() const override { return RcaCommentParser::singleton(); }
        const SymbolParser &symbol() const override { return RcaSymbolParser::singleton(); }
        const LetterParser &letter() const override { return RcaLetterParser::singleton(); }
        const LocationParser &location() const override { return StarLocationParser::singleton(); }
        const FunctionTable &function() const override { return RcaFunctionTable::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmCdp1802::AsmCdp1802(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_useReg),
      Config(TABLE),
      _opt_useReg(this, &AsmCdp1802::setUseReg, OPT_BOOL_USE_REGISTER, OPT_DESC_USE_REGISTER) {
    reset();
}

void AsmCdp1802::reset() {
    Assembler::reset();
    setUseReg(false);
}

Error AsmCdp1802::setUseReg(bool enable) {
    _useReg = enable;
    return OK;
}

namespace {

const struct : Functor {
    int_fast8_t nargs() const override { return 1; }
    Error eval(ValueStack &stack, ParserContext &, uint_fast8_t) const override {
        // Mark that this is 2 bytes value.
        stack.pushUnsigned((stack.pop().getUnsigned() & UINT16_MAX) | UINT32_C(0x1000'0000));
        return OK;
    }
} FN_A;

const struct : Functor {
    int_fast8_t nargs() const override { return 1; }
    Error eval(ValueStack &stack, ParserContext &, uint_fast8_t) const override {
        stack.pushUnsigned(stack.pop().getUnsigned() & UINT8_MAX);
        return OK;
    }
} FN_A0;

const struct : Functor {
    int_fast8_t nargs() const override { return 1; }
    Error eval(ValueStack &stack, ParserContext &, uint_fast8_t) const override {
        stack.pushUnsigned((stack.pop().getUnsigned() >> 8) & UINT8_MAX);
        return OK;
    }
} FN_A1;

const Functor *RcaFunctionTable::lookupFunction(const StrScanner &name) const {
    if (name.iequals_P(PSTR("A.0")))
        return &FN_A0;
    if (name.iequals_P(PSTR("A.1")))
        return &FN_A1;
    if (name.iequals_P(PSTR("A")))
        return &FN_A;
    return nullptr;
}

constexpr Config::opcode_t BR = 0x30;
constexpr Config::opcode_t LBR = 0xC0;

void intraBranch(AsmInsn &insn) {
    const auto cc = insn.opCode() & 0xF;
    insn.setOpCode(BR | cc);  // Bxx, convert to intra-page branch
}

void interBranch(AsmInsn &insn) {
    const auto cc = insn.opCode() & 0xF;
    insn.setOpCode(LBR | cc);  // LBxx, convert to inter-page branch
}

}  // namespace

void AsmCdp1802::encodePage(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    const auto base = insn.address() + 2;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    if (mode == M_PAGE || (mode == M_SHRT && !_smartBranch)) {
        insn.setErrorIf(op, checkAddr(target, base, 8));
    intra_page:
        insn.emitInsn();
        insn.emitByte(target);
        return;
    }
    if (mode == M_ADDR || (mode == M_LONG && !_smartBranch)) {
    inter_page:
        const auto target = op.val.getUnsigned();
        insn.setErrorIf(op, checkAddr(target));
        insn.emitInsn();
        insn.emitUint16(target);
        return;
    }
    if (op.getError() || checkAddr(target, base, 8)) {
        interBranch(insn);
        goto inter_page;
    }
    intraBranch(insn);
    goto intra_page;
}

void AsmCdp1802::emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    insn.setErrorIf(op);
    auto val = op.val;
    switch (mode) {
    case M_REG1:
        if (op.getError())
            val.setUnsigned(7);  // default work register.
        if (val.isZero()) {
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
            val.setUnsigned(7);
        }
        /* Fall-through */
    case M_REGN:
        if (op.getError())
            val.setUnsigned(7);  // default work register.
        if (val.overflow(15)) {
            insn.setErrorIf(op, ILLEGAL_REGISTER);
            val.setUnsigned(7);
        }
        insn.embed(val.getUnsigned());
        insn.emitInsn();
        break;
    case M_IMM8:
        if (val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitInsn();
        insn.emitByte(val.getUnsigned());
        break;
    case M_PAGE:
    case M_ADDR:
    case M_SHRT:
    case M_LONG:
        encodePage(insn, mode, op);
        break;
    case M_IOAD:
        if (op.getError())
            val.setUnsigned(1);  // default IO address
        if (val.isZero() || val.overflow(7)) {
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
            val.setUnsigned(1);
        }
        insn.embed(val.getUnsigned());
        insn.emitInsn();
        break;
    default:
        insn.emitInsn();
        break;
    }
}

Error AsmCdp1802::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (_useReg) {
        const auto reg = parseRegName(p);
        if (reg != REG_UNDEF) {
            op.val.setUnsigned(static_cast<uint8_t>(reg));
            op.mode = M_REGN;
            scan = p;
            return OK;
        }
    }
    op.val = parseInteger(p, op);
    if (!op.hasError()) {
        op.mode = M_ADDR;
        scan = p;
    }
    return op.getError();
}

Error AsmCdp1802::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.op1) && insn.op1.hasError())
        return _insn.setError(insn.op1);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.op2) && insn.op2.hasError())
            return _insn.setError(insn.op2);
        scan.skipSpaces();
    }

    if (_insn.setErrorIf(insn.op1, TABLE.searchName(cpuType(), insn)))
        return _insn.getError();

    emitOperand(insn, insn.mode1(), insn.op1);
    if (insn.mode2() == M_ADDR) {
        const auto &op = insn.op2;
        insn.setErrorIf(op);
        insn.emitUint16(op.val.getUnsigned());
    }
    return _insn.setError(insn);
}

}  // namespace cdp1802
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
