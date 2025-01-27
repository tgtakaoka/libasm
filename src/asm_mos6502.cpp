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

#include "asm_mos6502.h"
#include "reg_mos6502.h"
#include "table_mos6502.h"
#include "text_mos6502.h"

namespace libasm {
namespace mos6502 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::mos6502;
using namespace text::option;

namespace {
constexpr char TEXT_aequal[] PROGMEM = "*=";

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_aequal, &Assembler::defineOrigin},
    {TEXT_dALIGN, &Assembler::alignOrigin},
    {TEXT_dBYTE,  &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_dWORD,  &Assembler::defineDataConstant, Assembler::DATA_WORD},
    {TEXT_FCB,    &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_FCC,    &Assembler::defineString},
    {TEXT_FDB,    &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING},
    {TEXT_RMB,    &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmMos6502::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return MotorolaNumberParser::singleton(); }
        const CommentParser &comment() const override { return MostekCommentParser::singleton(); }
        const SymbolParser &symbol() const override { return MostekSymbolParser::singleton(); }
        const LetterParser &letter() const override { return MostekLetterParser::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmMos6502::AsmMos6502(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE),
      Config(TABLE),
      _opt_longa(this, &AsmMos6502::setLongAccumulator, OPT_BOOL_LONGA),
      _opt_longi(this, &AsmMos6502::setLongIndex, OPT_BOOL_LONGI) {
    reset();
}

void AsmMos6502::reset() {
    Assembler::reset();
    setLongAccumulator(false);
    setLongIndex(false);
}

Error AsmMos6502::setLongAccumulator(bool enable) {
    _longAccumulator = enable;
    if (cpuType() == W65C816)
        return OK;
    _longAccumulator = false;
    return enable ? OPERAND_NOT_ALLOWED : OK;
}

Error AsmMos6502::setLongIndex(bool enable) {
    _longIndex = enable;
    if (cpuType() == W65C816)
        return OK;
    _longIndex = false;
    return enable ? OPERAND_NOT_ALLOWED : OK;
}

namespace {

constexpr Config::uintptr_t bank(Config::uintptr_t addr) {
    return addr & ~0xFFFF;
}

constexpr Config::opcode_t BRA = 0x80;
constexpr Config::opcode_t BRL = 0x82;

bool maySmartBranch(const AsmInsn &insn, CpuType cpuType) {
    const auto opc = insn.opCode();
    return cpuType == W65C816 && (opc == BRA || opc == BRL);
}

void shortBranch(AsmInsn &insn) {
    insn.setOpCode(BRA);
}

void longBranch(AsmInsn &insn, const Operand &op) {
    insn.setOpCode(BRL);
    insn.setError(op);
}

}  // namespace

void AsmMos6502::encodeRelative(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    const auto len = insn.length();
    const auto base = insn.address() + (len ? len : 1) + 1;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    if (cpuType() == W65C816)
        insn.setErrorIf(op, checkAddr(target, insn.address(), 16));
    const auto smartBranch = _smartBranch && maySmartBranch(insn, cpuType());
    if (mode == M_REL && !smartBranch) {
    short_branch:
        const auto delta = branchDelta(base, target, insn, op);
        if (overflowDelta(delta, 8))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
        return;
    }
    if (mode == M_LREL && !smartBranch) {
    long_branch:
        const auto base = insn.address() + 3;
        const auto target = op.getError() ? base : op.val.getUnsigned();
        if (cpuType() == W65C816)
            insn.setErrorIf(op, checkAddr(target, insn.address(), 16));
        const auto delta = branchDelta(base, target, insn, op);
        insn.emitOperand16(delta);
        return;
    }
    const auto delta = branchDelta(base, target, insn, op);
    if (op.getError() || overflowDelta(delta, 8)) {
        longBranch(insn, op);
        goto long_branch;
    }
    shortBranch(insn);
    goto short_branch;
}

void AsmMos6502::emitImmediate(AsmInsn &insn, const Operand &op, bool imm16) const {
    if (imm16) {
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val.getUnsigned());
    } else {
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val.getUnsigned());
    }
}

void AsmMos6502::encodeOperand(AsmInsn &insn, AddrMode modeAndFlags, const Operand &op) const {
    const auto mode = AsmInsn::baseMode(modeAndFlags);
    switch (mode) {
    case M_IMA:
        emitImmediate(insn, op, _longAccumulator);
        break;
    case M_IMX:
        emitImmediate(insn, op, _longIndex);
        break;
    case M_ABS:
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        emitImmediate(insn, op, true);
        break;
    case M_ABSL:
        if (checkAddr(op.val.getUnsigned()))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val.getUnsigned());
        insn.emitOperand8(op.val.getUnsigned() >> 16);
        break;
    case M_DPG:
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        /* Fall-through */
    case M_IM8:
        emitImmediate(insn, op, false);
        break;
    case M_REL:
    case M_LREL:
        encodeRelative(insn, mode, op);
        break;
    default:
        break;
    }
}

Error AsmMos6502::selectMode(
        char size, Operand &op, AddrMode zp, AddrMode abs, AddrMode labs) const {
    if (size == '}') {
        if (labs == M_NONE)
            return op.setError(UNKNOWN_OPERAND);
        op.mode = labs;
    } else if (size == '>') {
        op.mode = abs;
    } else if (size == '<') {
        op.mode = zp;
    } else if (op.val.overflow(UINT16_MAX) && op.isOK()) {
        op.mode = labs;
    } else {
        op.mode = op.val.overflow(UINT8_MAX) ? abs : zp;
    }
    return OK;
}

namespace {

AddrMode regName2AddrMode(RegName reg) {
    switch (reg) {
    case REG_X:
        return M_REGX;
    case REG_Y:
        return M_REGY;
    case REG_S:
        return M_REGS;
    default:
        return M_REGA;
    }
}

}  // namespace

Error AsmMos6502::parseOpenIndirect(StrScanner &scan, Operand &op, char &indirect) const {
    if (scan.expect('(')) {
        if (indirect)
            return op.setError(UNKNOWN_OPERAND);
        indirect = ')';
        op.mode = I_FLAG;
    } else if (scan.expect('[')) {
        if (indirect)
            return op.setError(UNKNOWN_OPERAND);
        indirect = ']';
        op.mode = L_FLAG;
    } else if (indirect) {
        op.mode = indirect == ')' ? I_FLAG : L_FLAG;
    }
    return OK;
}

Error AsmMos6502::parseCloseIndirect(StrScanner &scan, Operand &op, char &indirect) const {
    if (scan.skipSpaces().expect(')')) {
        if (indirect != ')')
            return op.setError(UNKNOWN_OPERAND);
        indirect = 0;
    } else if (scan.expect(']')) {
        if (indirect != ']')
            return op.setError(UNKNOWN_OPERAND);
        indirect = 0;
    }
    return OK;
}

namespace {

char parseSizeOverride(StrScanner &p) {
    if (p.expect('<'))
        return '<';
    if (p.expect('>')) {
        if (p.expect('>'))
            return '}';  // >>
        return '>';
    }
    return 0;
}

}  // namespace

Error AsmMos6502::parseOperand(StrScanner &scan, Operand &op, char &indirect) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#')) {  // #nn
        const auto bop = p.expect([](char c) { return c == '<' || c == '>'; });
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        if (bop == '<')
            op.val.setUnsigned(op.val.getUnsigned() & 0xFF);
        if (bop == '>')
            op.val.setUnsigned((op.val.getUnsigned() >> 8) & 0xFF);
        op.mode = M_IMA;
        scan = p;
        return OK;
    }

    if (parseOpenIndirect(p, op, indirect))
        return op.getError();
    const auto indirectFlags = op.mode;

    const auto reg = parseRegName(p.skipSpaces(), parser());
    if (reg != REG_UNDEF) {
        op.mode = regName2AddrMode(reg);
    } else {
        const auto size = parseSizeOverride(p);
        op.val = parseInteger(p, op, indirect);
        if (op.hasError())
            return op.getError();
        if (selectMode(size, op, M_DPG, M_ABS, M_ABSL))
            return op.getError();
    }
    op.embed(indirectFlags);
    if (indirect && parseCloseIndirect(p, op, indirect))
        return op.getError();
    scan = p;
    return OK;
}

Error AsmMos6502::processPseudo(StrScanner &scan, Insn &insn) {
    const auto at = scan;
    if (strcasecmp_P(insn.name(), OPT_BOOL_LONGA) == 0) {
        const auto error = _opt_longa.set(scan);
        return error ? insn.setErrorIf(at, error) : OK;
    }
    if (strcasecmp_P(insn.name(), OPT_BOOL_LONGI) == 0) {
        const auto error = _opt_longi.set(scan);
        return error ? insn.setErrorIf(at, error) : OK;
    }
    return Assembler::processPseudo(scan, insn);
}

namespace {

bool hasRegister(AddrMode mode) {
    const auto base = uint8_t(AsmInsn::baseMode(mode));
    return base >= uint8_t(M_REGA) && base <= uint8_t(M_REGS);
}

bool maybeStackRelativeIndirect(CpuType cpuType, AddrMode mode3) {
    return cpuType == W65C816 && hasRegister(mode3);
}

}  // namespace

Error AsmMos6502::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    char indirect = 0;
    if (hasOperand(cpuType(), insn)) {
        if (parseOperand(scan, insn.op1, indirect) && insn.op1.hasError())
            return _insn.setError(insn.op1);
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.op2, indirect) && insn.op2.hasError())
                return _insn.setError(insn.op2);
        }
        if (indirect)
            return _insn.setError(
                    scan, indirect == '(' ? MISSING_CLOSING_PAREN : MISSING_CLOSING_BRACKET);
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.op3, indirect) && insn.op3.hasError())
                return _insn.setError(insn.op3);
        }
    }
    scan.skipSpaces();

    const auto error = searchName(cpuType(), insn);
    if (error == OPERAND_NOT_ALLOWED) {
        if (hasRegister(insn.op1.mode))
            return _insn.setError(insn.op1, REGISTER_NOT_ALLOWED);
        if (hasRegister(insn.op2.mode) && !maybeStackRelativeIndirect(cpuType(), insn.op3.mode))
            return _insn.setError(insn.op2, REGISTER_NOT_ALLOWED);
        if (hasRegister(insn.op3.mode))
            return _insn.setError(insn.op3, REGISTER_NOT_ALLOWED);
    }
    if (error)
        return _insn.setError(insn.op1, error);

    insn.setErrorIf(insn.op1);
    insn.setErrorIf(insn.op2);
    // W65C816 block move instructions
    if (insn.mode1() == M_BANK && insn.mode2() == M_BANK) {
        insn.emitOperand8(insn.op2.val.getUnsigned() >> 16);  // destination
        insn.emitOperand8(insn.op1.val.getUnsigned() >> 16);  // source
    } else {
        encodeOperand(insn, insn.mode1(), insn.op1);
        encodeOperand(insn, insn.mode2(), insn.op2);
    }
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace mos6502
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
