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

namespace libasm {
namespace cdp1802 {

using namespace reg;

namespace {

const char OPT_BOOL_USE_REGISTER[] PROGMEM = "use-register";
const char OPT_DESC_USE_REGISTER[] PROGMEM = "enable register name Rn";
const char OPT_BOOL_SMART_BRANCH[] PROGMEM = "smart-branch";
const char OPT_DESC_SMART_BRANCH[] PROGMEM = "enable optimizing to short branch";

struct Cdp1802FunctionParser final : FunctionParser {
    const Functor *parseFunction(
            StrScanner &, ErrorAt &, const SymbolParser &, const SymbolTable *) const override;
};

}  // namespace

struct AsmCdp1802::Operand final : ErrorAt {
    AddrMode mode;
    uint16_t val16;
    Operand() : mode(M_NONE), val16(0) {}
};

const ValueParser::Plugins &AsmCdp1802::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return RcaNumberParser::singleton(); }
        const CommentParser &comment() const override { return RcaCommentParser::singleton(); }
        const LetterParser &letter() const override { return _letter; }
        const LocationParser &location() const override {
            return AsteriskLocationParser::singleton();
        }
        const FunctionParser &function() const override { return _function; }
        const IbmLetterParser _letter{'T'};
        const Cdp1802FunctionParser _function{};
    } PLUGINS{};
    return PLUGINS;
}

AsmCdp1802::AsmCdp1802(const ValueParser::Plugins &plugins)
    : Assembler(&_opt_useReg, plugins),
      Config(TABLE),
      _opt_useReg(this, &AsmCdp1802::setUseReg, OPT_BOOL_USE_REGISTER, OPT_DESC_USE_REGISTER,
              _opt_smartBranch),
      _opt_smartBranch(
              this, &AsmCdp1802::setSmartBranch, OPT_BOOL_SMART_BRANCH, OPT_DESC_SMART_BRANCH) {
    reset();
}

void AsmCdp1802::reset() {
    setUseReg(false);
    setSmartBranch(false);
}

Error AsmCdp1802::setUseReg(bool enable) {
    _useReg = enable;
    return OK;
}

Error AsmCdp1802::setSmartBranch(bool enable) {
    _smartBranch = enable;
    return OK;
}

namespace {

Config::uintptr_t page(Config::uintptr_t addr) {
    return addr & ~0xFF;
}

const struct : Functor {
    int8_t nargs() const override { return 1; }
    Error eval(ValueStack &stack, uint8_t) const override {
        // Mark that this is 2 bytes value.
        stack.pushUnsigned(stack.pop().getUnsigned() | 0x10000);
        return OK;
    }
} FN_A;

const struct : Functor {
    int8_t nargs() const override { return 1; }
    Error eval(ValueStack &stack, uint8_t) const override {
        stack.pushUnsigned(stack.pop().getUnsigned() & 0xFF);
        return OK;
    }
} FN_A0;

const struct : Functor {
    int8_t nargs() const override { return 1; }
    Error eval(ValueStack &stack, uint8_t) const override {
        stack.pushUnsigned((stack.pop().getUnsigned() >> 8) & 0xFF);
        return OK;
    }
} FN_A1;

const Functor *Cdp1802FunctionParser::parseFunction(StrScanner &scan, ErrorAt &error,
        const SymbolParser &symParser, const SymbolTable *symtab) const {
    auto p = scan;
    if (p.iexpect('A')) {
        const Functor *fn = nullptr;
        if (p.expect('.')) {
            if (p.expect('0')) {
                fn = &FN_A0;
            } else if (p.expect('1')) {
                fn = &FN_A1;
            }
        } else {
            fn = &FN_A;
        }
        if (*p.skipSpaces() == '(' && fn) {
            scan = p;
            return fn;
        }
    }
    return FunctionParser::parseFunction(scan, error, symParser, symtab);
}

}  // namespace

void AsmCdp1802::encodePage(AsmInsn &insn, AddrMode mode, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    if (mode == M_PAGE) {
        if (page(target) != page(base))
            setErrorIf(op, OVERWRAP_PAGE);
        insn.emitInsn();
        insn.emitByte(target);
        return;
    }
    if (_smartBranch && page(target) == page(base)) {
        const auto opc = insn.opCode();
        insn.setOpCode(0x30 | (opc & 0xF));  // convert to in-page branch
        insn.emitInsn();
        insn.emitByte(target);
        return;
    }
    insn.emitInsn();
    insn.emitUint16(op.val16);
}

void AsmCdp1802::emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op) {
    auto val16 = op.val16;
    switch (mode) {
    case M_REG1:
        if (op.getError())
            val16 = 7;  // default work register.
        if (val16 == 0) {
            setErrorIf(op, REGISTER_NOT_ALLOWED);
            val16 = 7;
        }
        /* Fall-through */
    case M_REGN:
        if (op.getError())
            val16 = 7;  // default work register.
        if (val16 >= 16) {
            setErrorIf(op, ILLEGAL_REGISTER);
            val16 = 7;
        }
        insn.embed(val16);
        insn.emitInsn();
        break;
    case M_IMM8:
        if (overflowUint8(val16))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitInsn();
        insn.emitByte(val16);
        break;
    case M_PAGE:
    case M_ADDR:
        encodePage(insn, mode, op);
        break;
    case M_IOAD:
        if (op.getError())
            val16 = 1;  // default IO address
        if (val16 == 0 || val16 >= 8) {
            setErrorIf(op, OPERAND_NOT_ALLOWED);
            val16 = 1;
        }
        insn.embed(val16);
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
            op.val16 = int8_t(reg);
            op.mode = M_REGN;
            scan = p;
            return OK;
        }
    }
    op.val16 = parseExpr(p, op).getUnsigned();
    if (op.hasError())
        return op.getError();
    op.mode = M_ADDR;
    scan = p;
    return OK;
}

Error AsmCdp1802::encodeImpl(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    Operand op1, op2;
    if (parseOperand(scan, op1) && op1.hasError())
        return setError(op1);
    const auto op1p(scan);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, op2) && op2.hasError())
            return setError(op2);
        scan.skipSpaces();
    }
    if (!endOfLine(scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(op1);
    setErrorIf(op2);

    insn.setAddrMode(op1.mode, op2.mode);
    const auto error = TABLE.searchName(cpuType(), insn);
    if (error)
        return setError(op1, error);

    emitOperand(insn, insn.mode1(), op1);
    if (insn.mode2() == M_ADDR)
        insn.emitUint16(op2.val16);
    return setErrorIf(insn);
}

}  // namespace cdp1802
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
