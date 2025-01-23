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

#include "table_ins8060.h"
#include "text_common.h"

namespace libasm {
namespace ins8060 {

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

}  // namespace

const ValueParser::Plugins &AsmIns8060::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return Ins80xxNumberParser::singleton(); }
        const SymbolParser &symbol() const override { return Ins80xxSymbolParser::singleton(); }
        const FunctionTable &function() const override { return Ins8060FunctionTable::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmIns8060::AsmIns8060(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

void AsmIns8060::encodeRel8(AsmInsn &insn, const Operand &op) const {
    Config::ptrdiff_t delta;
    if (op.mode == M_DISP) {
        const auto pcdisp = (op.reg == REG_PC || op.reg == REG_P0);
        if (op.index == REG_E) {
            if (pcdisp)
                insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
            delta = -128;
        } else {
            delta = op.val.getSigned();
            if (overflowDelta(delta, 8) || (!pcdisp && delta == -128))
                insn.setErrorIf(op, OVERFLOW_RANGE);
        }
        insn.embed(encodePointerReg(op.reg));
    } else {
        // PC points the last byte of instruction.
        const Config::uintptr_t base = insn.address() + 1;
        // PC will be incremented before fetching next instruction.
        const uint8_t fetch = (insn.addrMode() == M_REL8) ? 1 : 0;
        const Config::uintptr_t target = op.getError() ? base : op.val.getUnsigned();
        // Program space is paged by 4kB.
        insn.setErrorIf(op, checkAddr(target, insn.address(), 12));
        const auto diff = offset(target - fetch) - offset(base);
        // Sign extends 12-bit number.
        delta = signExtend(diff, 12);
        if (overflowDelta(delta, 8))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        if (op.getError())
            delta = 0;
        insn.embed(encodePointerReg(REG_PC));
    }
    insn.emitInsn();
    insn.emitByte(delta);
}

void AsmIns8060::encodeIndx(AsmInsn &insn, const Operand &op) const {
    if (op.mode == M_REL8) {
        encodeRel8(insn, op);
        return;
    }
    insn.embed(encodePointerReg(op.reg));
    if (op.mode == M_INDX) {  // auto displacement mode
        if (insn.addrMode() != M_INDX)
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        insn.embed(4);
    }
    auto disp = op.val.getSigned();
    const auto pcdisp = (op.reg == REG_PC || op.reg == REG_P0);
    if (op.index == REG_E) {
        if (pcdisp)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        disp = -128;
    } else if (overflowDelta(disp, 8) || (!pcdisp && disp == -128)) {
        insn.setErrorIf(op, OVERFLOW_RANGE);
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
    const auto reg = parseRegName(p, parser());
    if (reg == REG_E) {
        op.index = REG_E;
    } else if (reg != REG_UNDEF) {
        op.mode = M_PNTR;
        op.reg = reg;
        scan = p;
        return OK;
    } else {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
    }
    if (p.skipSpaces().expect('(')) {
        const auto base = parseRegName(p, parser());
        if (!isPointerReg(base))
            return op.setError(scan, UNKNOWN_OPERAND);
        if (!p.expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        scan = p;
        op.reg = base;
        op.mode = autoDisp ? M_INDX : M_DISP;
        return OK;
    }

    if (autoDisp || reg == REG_E)
        return op.setError(scan, UNKNOWN_OPERAND);
    scan = p;
    op.mode = M_REL8;  // May be M_IMM8 too
    return OK;
}

Error AsmIns8060::defineAddrConstant(StrScanner &scan, Insn &insn) {
    do {
        auto p = scan.skipSpaces();
        const auto value = parseInteger(p, insn);
        if (!insn.hasError()) {
            const auto v = value.getUnsigned();
            insn.emitUint16Le(page(v) | offset(v - 1));
        }
        scan = p;
    } while (scan.skipSpaces().expect(',') && !insn.hasError());
    return insn.getError();
}

Error AsmIns8060::processPseudo(StrScanner &scan, Insn &insn) {
    if (strcasecmp_P(insn.name(), TEXT_dADDR) == 0)
        return defineAddrConstant(scan, insn);
    return Assembler::processPseudo(scan, insn);
}

Error AsmIns8060::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.op) && insn.op.hasError())
        return _insn.setError(insn.op);
    scan.skipSpaces();

    if (_insn.setErrorIf(insn.op, TABLE.searchName(cpuType(), insn)))
        return _insn.getError();

    insn.setErrorIf(insn.op);
    switch (insn.addrMode()) {
    case M_NONE:
        insn.emitInsn();
        break;
    case M_REL8:
        encodeRel8(insn, insn.op);
        break;
    case M_PNTR:
        insn.embed(encodePointerReg(insn.op.reg));
        insn.emitInsn();
        break;
    case M_DISP:
    case M_INDX:
        encodeIndx(insn, insn.op);
        break;
    case M_IMM8:
        if (insn.op.val.overflowUint8())
            insn.setErrorIf(insn.op, OVERFLOW_RANGE);
        insn.emitInsn();
        insn.emitByte(insn.op.val.getUnsigned());
        break;
    default:
        break;
    }
    if (insn.length() > 0 && page(insn.address()) != page(insn.address() + insn.length() - 1))
        insn.setErrorIf(insn.op, OVERWRAP_SEGMENT);
    return _insn.setError(insn);
}

}  // namespace ins8060
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
