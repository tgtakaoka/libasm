/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "asm_cp1600.h"
#include "comment_parser.h"
#include "number_parser.h"
#include "symbol_parser.h"
#include "table_cp1600.h"
#include "text_cp1600.h"

namespace libasm {
namespace cp1600 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::cp1600;

namespace {

constexpr char TEXT_WORD[] PROGMEM = "WORD";
constexpr char TEXT_ZERO[] PROGMEM = "ZERO";

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_ORG,  &Assembler::defineOrigin                                                   },
    { TEXT_RES,  &Assembler::allocateSpaces,     Assembler::DATA_WORD                       },
    { TEXT_WORD, &Assembler::defineDataConstant, Assembler::DATA_WORD                       },
};
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};
// clang-format on

// CP1600 number literals:
//   X'hh'  hexadecimal
//   B'bb'  binary
//   .dd    decimal (dot prefix)
//   nn     octal (bare digits at the default radix)
struct Cp1600NumberParser final : IbmNumberParser, Singleton<Cp1600NumberParser> {
    Cp1600NumberParser() : IbmNumberParser('X', 'B', 0, 0) {}
    Error parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const override {
        auto p = scan;
        if (p.expect('.') && isdigit(*p)) {
            const auto error = val.read(p, RADIX_10);
            if (error != NOT_AN_EXPECTED)
                scan = p;
            return error;
        }
        return IbmNumberParser::parseNumber(scan, val, defaultRadix);
    }
};

// CP1600 symbol syntax:
//   symbolLetter:      alphanumeric + '_' + '$' + '?' + '&'   (no '@')
//   instructionLetter: symbolLetter + '@'                     (MVO@, MVI@, ...)
//   locationSymbol:    '!', '*' (fallback), '$' (fallback)
constexpr char EXTRA_SYMBOL_LETTERS[] PROGMEM = "$?&";
struct Cp1600SymbolParser final : SimpleSymbolParser, Singleton<Cp1600SymbolParser> {
    Cp1600SymbolParser() : SimpleSymbolParser(EXTRA_SYMBOL_LETTERS) {}
    bool instructionLetter(char c) const override {
        return SimpleSymbolParser::symbolLetter(c) || c == '@';
    }
    bool locationSymbol(StrScanner &scan) const override {
        return SymbolParser::locationSymbol(scan, '!') || SymbolParser::locationSymbol(scan, '*') ||
               SymbolParser::locationSymbol(scan, '$');
    }
};

}  // namespace

const ValueParser::Plugins &AsmCp1600::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return Cp1600NumberParser::singleton(); }
        const SymbolParser &symbol() const override { return Cp1600SymbolParser::singleton(); }
        const CommentParser &comment() const override {
            return SemicolonCommentParser::singleton();
        }
    } PLUGINS{};
    return PLUGINS;
}

AsmCp1600::AsmCp1600(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

void AsmCp1600::reset() {
    Assembler::reset();
    setInputRadix(RADIX_8);
    _sdbdPrefix = false;
}

namespace {
constexpr char TEXT_RADIX[] PROGMEM = "RADIX";
}  // namespace

// RADIX n -- set the assembler's input radix.  The argument is parsed with
// RADIX_10 so the directive is unambiguous even while the current radix is
// octal (otherwise "RADIX 10" would be read as octal 10 = decimal 8).
// Mirrors the PDP-11 .RADIX pattern (asm_pdp11.cpp::setRadix).
Error AsmCp1600::setRadix(StrScanner &scan, Insn &, uint16_t) {
    const auto saved = _inputRadix;
    setInputRadix(RADIX_10);
    int32_t radix;
    auto p = scan;
    const auto error = parseIntOption(p, radix);
    if (error == OK && (radix == 2 || radix == 8 || radix == 10 || radix == 16)) {
        scan = p;
    } else {
        radix = saved;
    }
    setInputRadix(static_cast<Radix>(radix));
    return error;
}

constexpr AsmCp1600::PseudoCp1600 AsmCp1600::PSEUDO_CP1600_TABLE[] PROGMEM = {
        {TEXT_BYTE, &AsmCp1600::defineByte},
        {TEXT_RADIX, &AsmCp1600::setRadix},
        {TEXT_TEXT, &AsmCp1600::defineText},
        {TEXT_ZERO, &AsmCp1600::defineZero},
};
PROGMEM constexpr AsmCp1600::PseudosCp1600 AsmCp1600::PSEUDOS_CP1600{
        ARRAY_RANGE(PSEUDO_CP1600_TABLE)};

Error AsmCp1600::processPseudo(StrScanner &scan, Insn &insn) {
    const auto *p = PSEUDOS_CP1600.search(insn.name());
    return p ? p->invoke(this, scan, insn) : Assembler::processPseudo(scan, insn);
}

// BYTE val[,val...] -- emit each 16-bit value as two storage words: low byte
// in the first word, high byte in the second word.  Matches the SDBD-prefixed
// immediate layout (CPU reads two consecutive words and takes only the low 8
// bits of each).  Negative signed values get sign-extended in the high-half
// storage word to match ASL's convention.
Error AsmCp1600::defineByte(StrScanner &scan, Insn &insn, uint16_t) {
    ErrorAt error;
    do {
        scan.skipSpaces();
        ErrorAt exprErr;
        auto p = scan;
        const auto val = parseExpr(p, exprErr);
        if (!endOfLine(p) && *p != ',') {
            error.setErrorIf(scan, ILLEGAL_CONSTANT);
            break;
        }
        if (exprErr.hasError()) {
            error.setErrorIf(exprErr);
            break;
        }
        const auto v = val.getUnsigned();
        const auto lo = static_cast<uint16_t>(v & 0xFF);
        auto hi = static_cast<uint16_t>((v >> 8) & 0xFF);
        if (val.isNegative())
            hi |= 0xFF00;
        if (insn.emitUint16Be(lo) || insn.emitUint16Be(hi)) {
            error.setErrorIf(scan, NO_MEMORY);
            break;
        }
        scan = p;
    } while (scan.skipSpaces().expect(','));
    return insn.setError(error);
}

// ZERO N -- emit N zero-valued 16-bit storage words.
Error AsmCp1600::defineZero(StrScanner &scan, Insn &insn, uint16_t) {
    scan.skipSpaces();
    ErrorAt exprErr;
    const auto val = parseExpr(scan, exprErr);
    if (exprErr.hasError())
        return insn.setError(exprErr);
    const auto count = val.getSigned();
    if (count < 0)
        return insn.setError(scan, OPERAND_NOT_ALLOWED);
    for (int32_t i = 0; i < count; i++) {
        if (insn.emitUint16Be(0))
            return insn.setError(scan, NO_MEMORY);
    }
    return OK;
}

// TEXT n,'string' -- mode 1: each char in its own word's low byte.
//                    mode 2: chars packed two per word, first in low byte.
Error AsmCp1600::defineText(StrScanner &scan, Insn &insn, uint16_t) {
    scan.skipSpaces();
    ErrorAt exprErr;
    const auto modeVal = parseExpr(scan, exprErr);
    if (exprErr.hasError())
        return insn.setError(exprErr);
    const auto mode = modeVal.getUnsigned();
    if (mode != 1 && mode != 2)
        return insn.setError(scan, OPERAND_NOT_ALLOWED);
    if (!scan.skipSpaces().expect(','))
        return insn.setError(scan, MISSING_COMMA);
    const auto open = scan.skipSpaces();
    const char delim = *open;
    if (delim != '\'' && delim != '"')
        return insn.setError(scan, ILLEGAL_CONSTANT);
    auto p = open;
    ++p;
    if (mode == 1) {
        // One char per word, char in low byte.
        while (*p && *p != delim) {
            const auto c = static_cast<uint16_t>(*p++) & 0xFF;
            if (insn.emitUint16Be(c))
                return insn.setError(scan, NO_MEMORY);
        }
    } else {
        // Two chars per word, first in low byte.  Pad odd tail with zero.
        while (*p && *p != delim) {
            const auto lo = static_cast<uint16_t>(*p++) & 0xFF;
            uint16_t hi = 0;
            if (*p && *p != delim)
                hi = static_cast<uint16_t>(*p++) & 0xFF;
            if (insn.emitUint16Be(lo | (hi << 8)))
                return insn.setError(scan, NO_MEMORY);
        }
    }
    if (*p != delim)
        return insn.setError(scan, MISSING_CLOSING_QUOTE);
    ++p;
    scan = p;
    return OK;
}

// True if |opc| is an instruction whose data fetch is affected by SDBD.
// Mirrors the same check in dis_cp1600.cpp.
static bool isValidSdbdTarget(Config::opcode_t opc) {
    if (opc < 0x0280 || opc > 0x03FF)
        return false;
    const auto mmm = (opc >> 3) & 7;
    return mmm != 0 && mmm != 6;
}

Error AsmCp1600::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);

    // Try register name first.  SP/PC are accepted as aliases for R6/R7; the
    // alias is collapsed here so the rest of the encoder only deals with R0-R7.
    auto reg = parseRegName(p);
    if (reg != REG_UNDEF) {
        if (reg == REG_SP)
            reg = REG_R6;
        else if (reg == REG_PC)
            reg = REG_R7;
        op.reg = reg;
        op.mode = M_REG;
        scan = p;
        return OK;
    }

    // Try a value (address or immediate)
    op.val = parseInteger(p, op);
    if (!op.hasError()) {
        op.mode = M_DADDR;
        scan = p;
        return OK;
    }

    return op.getError();
}

static Config::uintptr_t branchTarget(const AsmInsn &insn, const Operand &op) {
    const Config::uintptr_t pc_after = insn.address() + 2;
    return op.getError() ? pc_after : op.val.getUnsigned();
}

void AsmCp1600::encodeBranch(AsmInsn &insn, const Operand &op) const {
    // Set S bit in opcode (this is called in the "opcode bits" phase)
    const Config::uintptr_t pc_after = insn.address() + 2;
    const auto target = branchTarget(insn, op);
    if (target < pc_after)
        insn.setOpCode(insn.opCode() | 0x0020);  // backward: set S bit
}

void AsmCp1600::emitBranch(AsmInsn &insn, const Operand &op) const {
    // Emit displacement word (this is called in the "extra data" phase after emitInsn)
    const Config::uintptr_t pc_after = insn.address() + 2;
    const auto target = branchTarget(insn, op);
    Config::uintptr_t disp;
    if (target >= pc_after) {
        disp = target - pc_after;
    } else {
        disp = pc_after - target - 1;
    }
    if (disp > 0x03FF)
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand16(static_cast<uint16_t>(disp));
}

// Encode an operand: set the appropriate opcode bits AND/OR emit the trailing
// data word(s).  emitInsn writes the prefix+opcode header into the reserved
// slot, so emitOperand16 (via operandPos) safely emits into the right place
// regardless of call order.
void AsmCp1600::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    const auto opc = insn.opCode();
    const auto reg = encodeRegNumber(op.reg);
    switch (mode) {
    case M_DREG:
        if (reg > 7)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        insn.setOpCode(opc | (reg & 7));
        break;
    case M_SREG:
        if (reg > 7)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        insn.setOpCode(opc | ((reg & 7) << 3));
        break;
    case M_LREG:
        if (reg > 3)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        insn.setOpCode(opc | (reg & 3));
        break;
    case M_XREG:
        // SSS = DDD = reg: TSTR / CLRR aliases set both source and dest fields.
        if (reg > 7)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        insn.setOpCode(opc | ((reg & 7) << 3) | (reg & 7));
        break;
    case M_SHCNT: {
        const auto count = op.mode == M_NONE ? 1 : op.val.getUnsigned();
        if (count == 2)
            insn.setOpCode(opc | 0x04);
        else if (count != 1)
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        break;
    }
    case M_BIT0: {
        // NOP/SIN optional "2": absent or 1 -> bit0=0; "2" -> bit0=1.
        const auto count = op.mode == M_NONE ? 1 : op.val.getUnsigned();
        if (count == 2)
            insn.setOpCode(opc | 0x01);
        else if (count != 1)
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        break;
    }
    case M_INDIR:
        if (reg < 1 || reg > 6)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        insn.setOpCode(opc | ((reg & 7) << 3));
        break;
    case M_BDISP:
        encodeBranch(insn, op);  // S bit
        emitBranch(insn, op);    // displacement word
        break;
    case M_BCOND: {
        const auto cond = op.val.getUnsigned();
        if (cond > 15)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.setOpCode(opc | (cond & 0x0F));
        break;
    }
    case M_BREG:
        // JSR/JSRE/JSRD return register: R4->BB=0, R5->1, R6->2.  Wraps to
        // 0xFF on R3 / 0x03 on R7 so the listing still shows the user input.
        if (reg < 4 || reg > 6)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        insn.setOpCode(opc | static_cast<Config::opcode_t>((reg - 4) & 0xFF) << 8);
        break;
    case M_JADDR: {
        // Jump address: bits 15:10 -> bits 7:2 of word2; low 10 bits emitted
        // as the trailing word3.
        const auto addr = op.val.getUnsigned();
        insn.setOpCode(opc | static_cast<Config::opcode_t>((addr & 0xFC00U) >> 8));
        insn.emitOperand16(addr & 0x03FFU);
        break;
    }
    case M_DADDR:
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_IMM16: {
        // Validate the SDBD prefix here, before the SDBD-aware emit: if it has
        // no effect on this instruction (e.g. MVOI), the double-byte form must
        // not be used and PREFIX_HAS_NO_EFFECT is flagged.
        if (_sdbdPrefix && (insn.hasPrefix() || !isValidSdbdTarget(insn.opCode()))) {
            insn.setErrorIf(PREFIX_HAS_NO_EFFECT);
            _sdbdPrefix = false;
        }
        const auto val = op.val.getUnsigned();
        if (_sdbdPrefix) {
            insn.emitOperand16(val & UINT8_MAX);
            uint16_t hi = (val >> 8) & UINT8_MAX;
            if (op.val.isNegative())
                hi |= 0xFF00;
            insn.emitOperand16(hi);
        } else {
            insn.emitOperand16(val);
        }
        break;
    }
    case M_NOPP:
        // NOPP emits an extra always-zero displacement word after the opcode.
        insn.emitOperand16(0);
        break;
    default:
        break;
    }
}

Error AsmCp1600::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);

    if (!endOfLine(scan.skipSpaces())) {
        if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
            return _insn.setError(insn.srcOp);
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
                return _insn.setError(insn.dstOp);
        }
    }
    scan.skipSpaces();

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.srcOp, insn);

    // Set opcode bits AND emit operand data words; M_NONE is a no-op.
    encodeOperand(insn, insn.srcOp, insn.src());
    encodeOperand(insn, insn.dstOp, insn.dst());

    // SDBD continuation: flag if the prefix has no effect on this instruction.
    // Any prefix-table entry (Jump) never consumes SDBD.
    if (_sdbdPrefix && (insn.hasPrefix() || !isValidSdbdTarget(insn.opCode())))
        insn.setErrorIf(PREFIX_HAS_NO_EFFECT);

    // Write the prefix+opcode header into the reserved leading slot.
    insn.emitInsn();

    _sdbdPrefix = insn.isSdbd();

    return _insn.setError(insn);
}

}  // namespace cp1600
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
