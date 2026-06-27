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

#include "asm_h16.h"
#include "reg_h16.h"
#include "table_h16.h"
#include "text_common.h"
#include "text_h16.h"

using namespace libasm::text::common;
using namespace libasm::text::h16;

namespace libasm {
namespace h16 {

using namespace pseudo;
using namespace reg;

namespace {

// "d" prefix marks the leading-dot mnemonics (avoids colliding with TEXT_DATA).
constexpr char TEXT_dDATA[]   PROGMEM = ".DATA";
constexpr char TEXT_dDATA_B[] PROGMEM = ".DATA.B";
constexpr char TEXT_dDATA_W[] PROGMEM = ".DATA.W";
constexpr char TEXT_dDATA_L[] PROGMEM = ".DATA.L";
constexpr char TEXT_dSDATA[]  PROGMEM = ".SDATA";
constexpr char TEXT_dRES[]    PROGMEM = ".RES";
constexpr char TEXT_dRES_B[]  PROGMEM = ".RES.B";
constexpr char TEXT_dRES_W[]  PROGMEM = ".RES.W";
constexpr char TEXT_dRES_L[]  PROGMEM = ".RES.L";

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_dDATA,   &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING },
    { TEXT_dDATA_B, &Assembler::defineDataConstant, Assembler::DATA_BYTE_NO_STRING },
    { TEXT_dDATA_L, &Assembler::defineDataConstant, Assembler::DATA_LONG           },
    { TEXT_dDATA_W, &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING },
    { TEXT_dRES,    &Assembler::allocateSpaces,     Assembler::DATA_WORD           },
    { TEXT_dRES_B,  &Assembler::allocateSpaces,     Assembler::DATA_BYTE           },
    { TEXT_dRES_L,  &Assembler::allocateSpaces,     Assembler::DATA_LONG           },
    { TEXT_dRES_W,  &Assembler::allocateSpaces,     Assembler::DATA_WORD           },
    { TEXT_dSDATA,  &Assembler::defineString,       0                              },
};
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};
// clang-format on

// Parse an explicit size qualifier ":N" where N is the field width 8/16/32.
// Returns 0 (|scan| unchanged) if absent or not one of those widths.
uint8_t parseSizeQual(StrScanner &scan) {
    auto p = scan;
    if (!p.expect(':'))
        return 0;
    Value::unsigned_t n;
    if (Value::read(p, RADIX_10, n) == OK && (n == 8 || n == 16 || n == 32)) {
        scan = p;
        return uint8_t(n);
    }
    return 0;
}

// Match "PC" as a two-letter token, rewinding on failure (so "PR.." doesn't
// leak a partial advance).
bool expectPC(StrScanner &scan) {
    auto save = scan;
    if (scan.iexpect('P') && scan.iexpect('C'))
        return true;
    scan = save;
    return false;
}

// Parse a scale factor "*N" where N is 1/2/4/8.  Returns 0 (|scan| unchanged)
// if absent or not a valid scale.
uint8_t parseScale(StrScanner &scan) {
    auto p = scan;
    if (!p.expect('*'))
        return 0;
    p.skipSpaces();
    Value::unsigned_t n;
    if (Value::read(p, RADIX_10, n) == OK && (n == 1 || n == 2 || n == 4 || n == 8)) {
        scan = p;
        return uint8_t(n);
    }
    return 0;
}

// Parse ".W" or ".L" (index size suffix on Xm); returns 0=word, 1=long, or
// -1 if absent / unrecognised.
int8_t parseIndexSize(StrScanner &scan) {
    auto p = scan;
    if (!p.expect('.')) return -1;
    if (p.expect('W') || p.expect('w')) { scan = p; return 0; }
    if (p.expect('L') || p.expect('l')) { scan = p; return 1; }
    return -1;
}

// True if |value| fits a signed |bits| (8/16/32) field.  Disp/imm/abs fields
// are all sign-extended (manual), so an explicit :N that can't hold the value
// as signed is an overflow (e.g. #$FF:8 reads back as -0x38).
bool fitsSize(int32_t value, uint8_t bits) {
    switch (bits) {
    case 8:  return value >= -0x80 && value <= 0x7F;
    case 16: return value >= -0x8000 && value <= 0x7FFF;
    default: return true;  // 32-bit: always fits an int32_t
    }
}

// Emit a 1/2/4-byte (|bits| = 8/16/32) disp/imm/abs value, flagging
// OVERFLOW_RANGE when too small.  An undefined forward ref is skipped: its
// first-pass value is a placeholder, meaningful only once the label resolves.
void emitValue(AsmInsn &insn, const Operand &op, uint8_t bits, int32_t value) {
    if (!op.val.isUndefined() && !fitsSize(value, bits))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    switch (bits) {
    case 8:  insn.emitOperand8(uint8_t(value));    break;
    case 16: insn.emitOperand16(uint16_t(value));  break;
    case 32: insn.emitOperand32(uint32_t(value));  break;
    }
}

// Smallest frame size (byte/word/long) holding |value|.  LINK/RTD auto-size
// their frame this way when no suffix is given (the manual shows none).
OprSize pickFrameSize(int32_t value) {
    if (fitsSize(value, 8))
        return SZ_BYTE;
    if (fitsSize(value, 16))
        return SZ_WORD;
    return SZ_LONG;
}

// A fixed-width unsigned quick field (TRAP vector = 4 bits, static shift/bit
// count = 5 bits) has no wider encoding, so an overflow is an error, not a
// silent wrap.  Flags OVERFLOW_RANGE and returns |value| masked to |bits|.
uint8_t fitQuickField(AsmInsn &insn, const Operand &op, uint8_t bits) {
    const auto mask = (1u << bits) - 1;
    if (!op.val.isUndefined() && op.val.getUnsigned() > mask)
        insn.setErrorIf(op, OVERFLOW_RANGE);
    return uint8_t(op.val.getUnsigned() & mask);
}

// Emit a displacement/extension value whose width comes from the Sd code
// (1=byte, 2=word, 3=long).  Sd==0 emits nothing.
void emitSdValue(AsmInsn &insn, const Operand &op, uint8_t sd, const Value &value) {
    if (sd)
        emitValue(insn, op, sd == 1 ? 8 : sd == 2 ? 16 : 32, value.getSigned());
}

// Returns the Sd field (0/1/2/3) for a displacement size of 0/8/16/32 bits.
uint8_t sdFromSize(uint8_t size) {
    switch (size) {
    case 0:  return 0;
    case 8:  return 1;
    case 16: return 2;
    case 32: return 3;
    default: return 0;
    }
}

// Returns Sf bits (0..3) for a scale factor of 1/2/4/8.
uint8_t sfFromScale(uint8_t scale) {
    switch (scale) {
    case 2: return 1;
    case 4: return 2;
    case 8: return 3;
    default: return 0;
    }
}

// Pick the smallest displacement size that holds |val|: 8, 16, or 32 bits.
// |hint| if non-zero forces the user's requested size.
uint8_t pickDispSize(int32_t val, uint8_t hint) {
    if (hint) return hint;
    if (val >= -0x80 && val <= 0x7F) return 8;
    if (val >= -0x8000 && val <= 0x7FFF) return 16;
    return 32;
}

// Smallest branch disp size (.B/.W/.L) reaching |op|'s target; |opcodeBytes| is
// the opcode length before the disp (1 plain branch, 2 for Bcc:G/SCB [Rn|cc]).
// An undefined target falls back to .W.
OprSize pickBranchSize(const AsmInsn &insn, const Operand &op, uint8_t opcodeBytes) {
    if (op.val.isUndefined())
        return SZ_WORD;
    const Config::uintptr_t target = op.val.getUnsigned();
    const Config::uintptr_t base = insn.address() + opcodeBytes;
    if (!Config::overflowDelta(Config::ptrdiff_t(target - (base + 1)), 8))
        return SZ_BYTE;
    if (!Config::overflowDelta(Config::ptrdiff_t(target - (base + 2)), 16))
        return SZ_WORD;
    return SZ_LONG;
}

// Fixed operand size of an instruction with no opcode size field, used to
// accept only the matching suffix (manual's "Operand Size" line).  ISZ_FIXED
// ops carry it in the table; everything else takes no suffix.
OprSize impliedSize(const AsmInsn &insn) {
    return insn.insnSize() == ISZ_FIXED ? insn.fixedSize() : SZ_NONE;
}

// Sd code for an indirect displacement: explicit :N wins; an undefined or zero
// disp encodes Sd=0 (no disp byte); else the smallest size holding the value.
uint8_t dispSdFor(const Operand &op) {
    if (op.dispSize)
        return sdFromSize(op.dispSize);
    if (op.val.isUndefined() || !op.val.getSigned())
        return 0;
    return sdFromSize(pickDispSize(op.val.getSigned(), 0));
}

// Sd code for a PC-relative displacement.  Explicit :N wins; undefined/zero
// target encodes Sd=0.  Else the smallest size whose signed range holds the
// delta from target to |base| (each wider size pushes base one byte further:
// byte form base+1, word form base+2).
uint8_t pcRelSdFor(const Operand &op, Config::uintptr_t base) {
    if (op.dispSize)
        return sdFromSize(op.dispSize);
    if (op.val.isUndefined() || !op.val.getSigned())
        return 0;
    const Config::uintptr_t target = op.val.getUnsigned();
    if (!Config::overflowDelta(Config::ptrdiff_t(target - (base + 1)), 8))
        return 1;
    if (!Config::overflowDelta(Config::ptrdiff_t(target - (base + 2)), 16))
        return 2;
    return 3;
}

// A register run is a comma-separated sequence of register-direct operands
// (M_DREG) and ranges (Rn-Rm, M_RLIST); it backs the STM/LDM/PGBN lists and
// the BFMOV/string/bit-field tuples.
bool isRunReg(const Operand &op) {
    return op.mode == M_DREG || op.mode == M_RLIST;
}

// Add a register-run operand's register(s) to insn.regList (a range is expanded
// to its individual registers).
void gatherRunReg(AsmInsn &insn, const Operand &op) {
    if (op.mode == M_DREG) {
        insn.addReg(op.reg);
        if (!isGlobalReg(op.reg))
            insn.regGlobal = false;
    } else if (op.mode == M_RLIST) {  // a "Rn-Rm" range
        const auto mask = uint16_t(op.val.getUnsigned());
        for (uint8_t n = 0; n < 16; ++n)
            if (mask & (1u << n))
                insn.addRegNum(n);
        insn.regRange = true;
    }
}

}  // namespace

// The mnemonic token is STEM[/cc][:CLASS][.SIZE] (e.g. ADD:RQ.W, SCB/EQ.B).
// Strip "/cc" into ccSuffix, ":CLASS" into classSuffix, ".SIZE" into
// sizeSuffix, truncating name() to the bare stem for table lookup.
void AsmInsn::parseClassAndSize(const ValueParser &parser) {
    classSuffix = IC_N;
    sizeSuffix = SZ_NONE;
    ccSuffix = reg::CC_UNDEF;
    dirSuffix = DIR_NONE;
    StrScanner p{name()};
    p.trimStart([](char c) { return c != ':' && c != '.' && c != '/'; });
    auto stemEnd = const_cast<char *>(p.str());
    // "/X" suffixes: a /cc condition and/or a /F|/B string direction.  "/B" is
    // always a direction; a lone "/F" is ambiguous (records both CC_F and DIR_F,
    // resolved at encode time).  A second direction after that "/F" means the
    // first "/F" was the cc -- tracked by dirFromF.
    bool dirFromF = false;
    while (p.expect('/')) {
        auto save = p;
        const auto tok = parser.readRegName(p);
        if (tok.size() == 1 && (tok[0] == 'B' || tok[0] == 'b')) {
            if (dirSuffix != DIR_NONE && !dirFromF) {  // at most one /F|/B direction
                setErrorIf(p, UNKNOWN_INSTRUCTION);
                return;
            }
            dirSuffix = DIR_B;
            dirFromF = false;
        } else if (tok.size() == 1 && (tok[0] == 'F' || tok[0] == 'f')) {
            if (dirSuffix == DIR_NONE) {
                dirSuffix = DIR_F;
                dirFromF = true;
                if (ccSuffix == reg::CC_UNDEF)
                    ccSuffix = reg::CC_F;
            } else if (dirFromF) {
                dirFromF = false;  // the earlier ambiguous "/F" was the cc
            } else {
                setErrorIf(p, UNKNOWN_INSTRUCTION);
                return;
            }
        } else {
            if (ccSuffix != reg::CC_UNDEF) {  // at most one /cc condition
                setErrorIf(p, UNKNOWN_INSTRUCTION);
                return;
            }
            auto t = save;
            ccSuffix = reg::parseCcName(t, parser);
            if (ccSuffix == reg::CC_UNDEF) {
                setErrorIf(p, UNKNOWN_INSTRUCTION);
                return;
            }
        }
    }
    // Record whether the cc came from a lone, tentative "/F" serving as the
    // direction: a cc-only op (SCB/SET/TRAP) reads it as cc=F; a string op
    // (SMOV/SSTR) treats it as the direction and ignores the tentative cc.
    ccFromLoneF = dirFromF && ccSuffix == reg::CC_F;
    if (p.expect(':')) {
        // Match the class as a whole token, so ADD:GX is rejected, not matched
        // as a prefix.
        const auto cls = parser.readRegName(p);
        if (cls.iequals_P(PSTR("G")))       classSuffix = IC_G;
        else if (cls.iequals_P(PSTR("Q")))  classSuffix = IC_Q;
        else if (cls.iequals_P(PSTR("R")))  classSuffix = IC_R;
        else if (cls.iequals_P(PSTR("RQ"))) classSuffix = IC_RQ;
        else {
            setErrorIf(p, UNKNOWN_INSTRUCTION);
            return;
        }
    }
    if (p.expect('.')) {
        if (p.iexpect('B'))      sizeSuffix = SZ_BYTE;
        else if (p.iexpect('W')) sizeSuffix = SZ_WORD;
        else if (p.iexpect('L')) sizeSuffix = SZ_LONG;
        if (sizeSuffix == SZ_NONE || *p) {
            setErrorIf(p, ILLEGAL_SIZE);
            return;
        }
    }
    if (*p) {
        setErrorIf(p, UNKNOWN_INSTRUCTION);
        return;
    }
    *stemEnd = 0;
}

namespace {

// H16 mnemonics embed ':' (ADD:G), '.' (ADD:G.W) and '/' (SCB/EQ, SMOV/F).
// '*' is the current-location symbol in Hitachi syntax.
struct H16SymbolParser final : SymbolParser {
    // '.' is deliberately NOT a symbol letter: it is overloaded as a size
    // separator (".B/.W/.L", "Rn.W/Rn.L"), so admitting it would break "Rn.W"
    // and "SCB/EQ.B".  Hence a dotted label like "loop.1" (ASL accepts it) is
    // unsupported; use '_'.
    bool instructionLetter(char c) const override {
        return SymbolParser::instructionLetter(c) || c == ':' || c == '.' || c == '/';
    }
    bool locationSymbol(StrScanner &scan) const override {
        return SymbolParser::locationSymbol(scan, '$') ||
               SymbolParser::locationSymbol(scan, '*');
    }
};

}  // namespace

const ValueParser::Plugins &AsmH16::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return MotorolaNumberParser::singleton(); }
        const CommentParser &comment() const override { return SemicolonCommentParser::singleton(); }
        const SymbolParser &symbol() const override {
            static const H16SymbolParser SYMBOL;
            return SYMBOL;
        }
        const LetterParser &letter() const override { return MotorolaLetterParser::singleton(); }
        const OperatorParser &operators() const override {
            return MotorolaOperatorParser::singleton();
        }
    } PLUGINS;
    return PLUGINS;
}

AsmH16::AsmH16(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

Error AsmH16::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    // Immediate: #imm[:size]
    if (p.expect('#')) {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        op.immSize = parseSizeQual(p);
        op.mode = M_IMM;
        scan = p;
        return OK;
    }

    // @-prefixed forms
    if (p.expect('@')) {
        scan = p;
        return parseIndirect(scan, op);
    }

    // Control register: CCR, SR, CBNR, etc.
    auto crSave = p;
    const auto creg = parseCrName(p, parser());
    if (creg != CR_UNDEF) {
        op.creg = creg;
        op.mode = M_CR;
        scan = p;
        return OK;
    }
    p = crSave;

    // Register direct: Rn, CRn, PRn -- or a single range "Rn-Rm" (STM/LDM/PGBN)
    // when a '-' follows.  A multi-range list is comma-separated (ASL spells it
    // "R0, R3-R12, R15"); encodeImpl ORs the per-operand masks.
    auto rSave = p;
    const auto reg = parseRegName(p, parser());
    if (reg != REG_UNDEF) {
        if (p.expect('-')) {
            const auto hi = parseRegName(p, parser());
            if (hi == REG_UNDEF || !isGlobalReg(hi) || !isGlobalReg(reg))
                return op.setError(UNKNOWN_OPERAND);
            auto lo = regNum(reg);
            auto up = regNum(hi);
            if (lo > up) {
                const auto t = lo;
                lo = up;
                up = t;
            }
            uint16_t mask = 0;
            for (auto n = lo; n <= up; ++n)
                mask |= uint16_t(1u << n);
            op.val = Value().setUnsigned(mask);
            op.mode = M_RLIST;
            scan = p;
            return OK;
        }
        op.reg = reg;
        op.mode = M_DREG;
        scan = p;
        return OK;
    }
    p = rSave;

    // Bare expression: a branch target or an immediate without #; acceptMode
    // sorts out which.
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_DISP;
    scan = p;
    return OK;
}

// Parse an @-prefixed EA, with |scan| positioned just past the '@':
// @@Rn / @-Rn / @(...) indirect-index-pcrel-scale-doubleindirect /
// @Rn[+|*Sf] / @PC / @addr[:size].
Error AsmH16::parseIndirect(StrScanner &scan, Operand &op) const {
    auto p = scan;
    // @@Rn or @@(...) -- double indirect short form
    if (p.expect('@')) {
        // @@Rn is short for @(0:8, @(0:8, Rn)) -- the smallest legal (S1,S2)
        // combo per Table 16-7.
        const auto reg = parseRegName(p, parser());
        if (reg == REG_UNDEF)
            return op.setError(UNKNOWN_OPERAND);
        op.reg = reg;
        op.mode = M_DIND;
        op.ds1Size = 8;
        op.ds2Size = 8;
        op.val = Value();
        op.val2 = Value();
        scan = p;
        return OK;
    }
    // @-Rn auto-decrement
    if (p.expect('-')) {
        const auto reg = parseRegName(p, parser());
        if (reg == REG_UNDEF)
            return op.setError(UNKNOWN_OPERAND);
        op.reg = reg;
        op.mode = M_PDEC;
        scan = p;
        return OK;
    }
    // @(...) -- indirect / index / PC-relative / scale / double-indirect
    if (p.expect('(')) {
        scan = p;
        return parseParen(scan, op);
    }
    // @Rn / @Rn+ / @Rn*Sf / @PC (no-paren forms) or @addr
    auto qSave = p;
    const auto reg = parseRegName(p, parser());
    if (reg != REG_UNDEF) {
        if (p.expect('+')) {
            op.reg = reg;
            op.mode = M_PINC;
        } else if (const auto sf = parseScale(p)) {
            op.reg = reg;
            op.scale = sf;
            op.mode = M_SCALE;
        } else {
            op.reg = reg;
            op.mode = M_INDIR;
            op.dispSize = 0;
            op.val = Value();
        }
        scan = p;
        return OK;
    }
    if (expectPC(p)) {
        op.mode = M_PCREL;
        op.val = Value();
        scan = p;
        return OK;
    }
    p = qSave;
    // @addr[:size] absolute
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.absSize = parseSizeQual(p);
    op.mode = M_ABS;
    scan = p;
    return OK;
}

// Parse the @(...) form, with |scan| positioned just past the (:
// @(Rn)/@(Rn*Sf) indirect, @(disp,Rn) indexed, @(Xm,Rn)/@(disp,Xm,Rn)
// register-indexed, @(disp,PC)/@(Xm,PC) PC-relative, and @(ds2,@(ds1,Rn))
// double-indirect.
Error AsmH16::parseParen(StrScanner &scan, Operand &op) const {
    auto sp = scan.skipSpaces();
    // Check if first item is a register (no leading disp).
    auto regSave = sp;
    const auto firstReg = parseRegName(sp, parser());
    // @(PC) -- PC-relative with no displacement
    {
        auto pcSave = sp;
        if (expectPC(sp) && sp.skipSpaces().expect(')')) {
            op.mode = M_PCREL;
            op.val = Value();
            op.dispSize = 0;
            scan = sp;
            return OK;
        }
        sp = pcSave;
    }
    if (firstReg != REG_UNDEF) {
        // @(Rn) / @(Rn*Sf) / @(Xm[.Sz][*Sf], Rn) / @(Xm, PC)
        const auto idxSize1 = parseIndexSize(sp);
        const auto scale1 = parseScale(sp);
        if (sp.skipSpaces().expect(',')) {
            sp.skipSpaces();
            if (expectPC(sp)) {
                // @(Xm, PC) -- the lone register is always the index.
                op.indexReg = firstReg;
                // No .Sz on Xm defaults to long (L=1) per ASL.
                op.indexSize = (idxSize1 < 0) ? 1 : uint8_t(idxSize1);
                op.scale = scale1 ? scale1 : 1;
                op.mode = M_PCIDX;
            } else {
                const auto reg2 = parseRegName(sp, parser());
                if (reg2 == REG_UNDEF)
                    return op.setError(UNKNOWN_OPERAND);
                const auto idxSize2 = parseIndexSize(sp);
                const auto scale2 = parseScale(sp);
                // Index is the register that carries size/scale
                // modifiers; if neither does, ASL convention is
                // first=base, second=index.
                const auto firstIsIndex = (idxSize1 >= 0) || scale1;
                if (firstIsIndex) {
                    op.indexReg = firstReg;
                    // No .Sz on Xm defaults to long (L=1) per ASL.
                    op.indexSize = (idxSize1 < 0) ? 1 : uint8_t(idxSize1);
                    op.scale = scale1 ? scale1 : 1;
                    op.reg = reg2;
                } else {
                    op.indexReg = reg2;
                    // No .Sz on Xm defaults to long (L=1) per ASL.
                    op.indexSize = (idxSize2 < 0) ? 1 : uint8_t(idxSize2);
                    op.scale = scale2 ? scale2 : 1;
                    op.reg = firstReg;
                }
                op.mode = M_INDEX;
            }
            if (!sp.skipSpaces().expect(')'))
                return op.setError(MISSING_CLOSING_PAREN);
            scan = sp;
            return OK;
        }
        if (!sp.skipSpaces().expect(')'))
            return op.setError(MISSING_CLOSING_PAREN);
        op.reg = firstReg;
        if (scale1) {
            // Any explicit *N (even *1) selects scale-mode EA.
            op.scale = scale1;
            op.mode = M_SCALE;
        } else {
            op.mode = M_INDIR;
        }
        scan = sp;
        return OK;
    }
    // Not a register; first item is a displacement.
    sp = regSave;
    auto firstExpr = parseInteger(sp, op);
    if (op.hasError())
        return op.getError();
    uint8_t firstSize = parseSizeQual(sp);
    if (!sp.skipSpaces().expect(','))
        return op.setError(MISSING_COMMA);
    sp.skipSpaces();
    // After comma: @(...) for double-indirect, else a register
    if (sp.expect('@')) {
        if (!sp.expect('('))
            return op.setError(UNKNOWN_OPERAND);
        op.val2 = firstExpr;
        // ds2 size: explicit suffix, else auto-pick 8 or 32 (only
        // 1-byte and 4-byte sizes are legal per Table 16-7).
        if (firstSize) {
            op.ds2Size = firstSize;
        } else {
            const auto v2 = firstExpr.getSigned();
            op.ds2Size = (v2 >= -0x80 && v2 <= 0x7F) ? 8 : 32;
        }
        sp.skipSpaces();
        op.val = parseInteger(sp, op);
        if (op.hasError())
            return op.getError();
        op.ds1Size = parseSizeQual(sp);
        if (!op.ds1Size) {
            const auto v1 = op.val.getSigned();
            op.ds1Size = (v1 >= -0x80 && v1 <= 0x7F) ? 8 : 32;
        }
        if (!sp.skipSpaces().expect(','))
            return op.setError(MISSING_COMMA);
        sp.skipSpaces();
        const auto reg = parseRegName(sp, parser());
        if (reg == REG_UNDEF)
            return op.setError(UNKNOWN_OPERAND);
        op.reg = reg;
        if (!sp.skipSpaces().expect(')') || !sp.skipSpaces().expect(')'))
            return op.setError(MISSING_CLOSING_PAREN);
        op.mode = M_DIND;
        scan = sp;
        return OK;
    }
    // Parse a register; it might be Xm (with .Sz/*Sf) or Rn (base)
    const auto reg1 = parseRegName(sp, parser());
    if (reg1 == REG_UNDEF) {
        // Check for PC
        if (expectPC(sp)) {
            op.val = firstExpr;
            op.dispSize = firstSize;
            op.mode = M_PCREL;
            if (!sp.skipSpaces().expect(')'))
                return op.setError(MISSING_CLOSING_PAREN);
            scan = sp;
            return OK;
        }
        return op.setError(UNKNOWN_OPERAND);
    }
    // Optional .Sz on reg1
    const auto idxSize1 = parseIndexSize(sp);
    // Optional *Sf on reg1
    const auto scale1 = parseScale(sp);
    // If a comma follows, reg1 was Xm and another reg follows
    if (sp.skipSpaces().expect(',')) {
        sp.skipSpaces();
        // Could be Rn or PC
        if (expectPC(sp)) {
            op.val = firstExpr;
            op.dispSize = firstSize;
            op.indexReg = reg1;
            op.indexSize = (idxSize1 < 0) ? 1 : uint8_t(idxSize1);
            op.scale = scale1 ? scale1 : 1;
            op.mode = M_PCIDX;
            if (!sp.skipSpaces().expect(')'))
                return op.setError(MISSING_CLOSING_PAREN);
            scan = sp;
            return OK;
        }
        const auto reg2 = parseRegName(sp, parser());
        if (reg2 == REG_UNDEF)
            return op.setError(UNKNOWN_OPERAND);
        const auto idxSize2 = parseIndexSize(sp);
        const auto scale2 = parseScale(sp);
        op.val = firstExpr;
        op.dispSize = firstSize;
        // The register carrying .Sz/*Sf is the index (Xm); with neither,
        // first=base, second=index (matches the no-disp form and ASL).
        const auto firstIsIndex = (idxSize1 >= 0) || scale1;
        if (firstIsIndex) {
            op.indexReg = reg1;
            op.indexSize = (idxSize1 < 0) ? 1 : uint8_t(idxSize1);
            op.scale = scale1 ? scale1 : 1;
            op.reg = reg2;
        } else {
            op.indexReg = reg2;
            op.indexSize = (idxSize2 < 0) ? 1 : uint8_t(idxSize2);
            op.scale = scale2 ? scale2 : 1;
            op.reg = reg1;
        }
        op.mode = M_INDEX;
        if (!sp.skipSpaces().expect(')'))
            return op.setError(MISSING_CLOSING_PAREN);
        scan = sp;
        return OK;
    }
    // No second operand: reg1 is the base; may have *Sf for scale
    if (!sp.skipSpaces().expect(')'))
        return op.setError(MISSING_CLOSING_PAREN);
    op.val = firstExpr;
    op.dispSize = firstSize;
    op.reg = reg1;
    if (scale1) {
        op.scale = scale1;
        op.mode = M_SCALE;
    } else {
        op.mode = M_INDIR;
    }
    scan = sp;
    return OK;
}

// Consume the remaining register run -- the comma-separated Rn / Rn-Rm operands
// starting at |scan| -- into insn.regList, stopping at (and not consuming) the
// first non-register operand, which the caller parses as a trailing memory EA.
Error AsmH16::parseRegList(StrScanner &scan, AsmInsn &insn) const {
    for (;;) {
        auto save = scan;
        Operand op;
        if (parseOperand(scan, op) && op.hasError())
            return op.getError();
        if (!isRunReg(op)) {
            scan = save;  // not a register; leave it for the trailing-EA parse
            return OK;
        }
        gatherRunReg(insn, op);
        if (!scan.skipSpaces().expect(','))
            return OK;  // the run -- and the operand list -- ends here
    }
}

// Emit the EA byte and any extension bytes for |op|.  The high bit of the
// EA byte (accumulator-form marker) is supplied by the caller via |accBit|.
void AsmH16::emitEa(AsmInsn &insn, const Operand &op, uint8_t accBit) const {
    // Bank prefix: prepend 0x70 / 0x74 when the register is CRn / PRn.  The
    // accumulator-form marker (bit 7) goes on the FIRST byte of the EA field
    // (Manual 4.5.2, Figure 4-41 and Note 4 to Table 4-7); with a bank prefix
    // that is the prefix byte, and the inner EA byte carries acc=0.
    if (isCurBankReg(op.reg)) {
        insn.emitOperand8(accBit | 0x70);
        accBit = 0;
    } else if (isPrevBankReg(op.reg)) {
        insn.emitOperand8(accBit | 0x74);
        accBit = 0;
    }

    const uint8_t rn = regNum(op.reg);
    switch (op.mode) {
    case M_DREG:
        insn.emitOperand8(accBit | 0x40 | rn);
        return;
    case M_INDIR: {
        // @Rn (no disp) or @(disp, Rn) where disp size is in op.dispSize.
        const auto sd = dispSdFor(op);
        insn.emitOperand8(accBit | (sd << 4) | rn);
        emitSdValue(insn, op, sd, op.val);
        return;
    }
    case M_PINC:
        insn.emitOperand8(accBit | 0x50 | rn);
        return;
    case M_PDEC:
        insn.emitOperand8(accBit | 0x60 | rn);
        return;
    case M_IMM: {
        const uint8_t size = op.immSize ? op.immSize :
                pickDispSize(op.val.getSigned(), 0);
        const uint8_t si = sdFromSize(size);
        insn.emitOperand8(accBit | 0x70 | si);
        emitValue(insn, op, size, op.val.getSigned());
        return;
    }
    case M_ABS: {
        const uint8_t size = op.absSize ? op.absSize :
                pickDispSize(op.val.getSigned(), 0);
        const uint8_t sa = sdFromSize(size);
        insn.emitOperand8(accBit | 0x74 | sa);
        emitValue(insn, op, size, op.val.getSigned());
        return;
    }
    case M_SCALE: {
        const auto sd = dispSdFor(op);
        insn.emitOperand8(accBit | 0x78 | sd);
        const uint8_t ext = (sfFromScale(op.scale) << 4) | regNum(op.reg);
        insn.emitOperand8(ext);
        emitSdValue(insn, op, sd, op.val);
        return;
    }
    case M_INDEX: {
        // @([disp,] Xm[.Sz][*Sf], Rn)
        const auto sd = dispSdFor(op);
        insn.emitOperand8(accBit | 0x7C);
        const uint8_t ext1 = (op.indexSize ? 0x40 : 0x00) | (sd << 4) | regNum(op.reg);
        const uint8_t ext2 = (sfFromScale(op.scale) << 4) | regNum(op.indexReg);
        insn.emitOperand8(ext1);
        insn.emitOperand8(ext2);
        emitSdValue(insn, op, sd, op.val);
        return;
    }
    case M_PCREL: {
        // @PC or @(target, PC): disp = target - PC.  Pick the disp size from
        // the delta; base = emit pos + EAd byte + ext byte.
        const auto sd = pcRelSdFor(op, insn.address() + insn.length() + 2);
        insn.emitOperand8(accBit | 0x7D);
        insn.emitOperand8(0x80 | (sd << 4));
        emitPcRelDisp(insn, op, sd);
        return;
    }
    case M_PCIDX: {
        // @([target,] Xm, PC): like M_PCREL but with an extra ext2 byte
        // (EAd + 3 bytes before the disp).
        const auto sd = pcRelSdFor(op, insn.address() + insn.length() + 3);
        insn.emitOperand8(accBit | 0x7D);
        const uint8_t ext1 = (op.indexSize ? 0x40 : 0x00) | (sd << 4);
        const uint8_t ext2 = (sfFromScale(op.scale) << 4) | regNum(op.indexReg);
        insn.emitOperand8(ext1);
        insn.emitOperand8(ext2);
        emitPcRelDisp(insn, op, sd);
        return;
    }
    case M_DIND: {
        // @@Rn or @(ds2, @(ds1, Rn)); legal (S1,S2) per Table 16-7.
        const uint8_t s1 = sdFromSize(op.ds1Size);  // 0, 1 (byte), or 3 (long)
        const uint8_t s2 = sdFromSize(op.ds2Size);
        insn.emitOperand8(accBit | 0x7E);
        insn.emitOperand8((s2 << 6) | (s1 << 4) | regNum(op.reg));
        if (op.ds1Size)
            emitValue(insn, op, op.ds1Size, op.val.getSigned());
        if (op.ds2Size)
            emitValue(insn, op, op.ds2Size, op.val2.getSigned());
        return;
    }
    default:
        return;
    }
}

// Emit a PC-relative displacement of the Sd-coded size; the delta runs from the
// target to the PC just past the disp bytes (branchDelta() computes it).  An
// undefined forward ref uses base as the target so the field is zero until the
// label resolves.  A source-forced :8/:16 that the delta won't fit is an
// OVERFLOW_RANGE, else the bytes would silently point elsewhere.
void AsmH16::emitPcRelDisp(AsmInsn &insn, const Operand &op, uint8_t sd) const {
    if (!sd)
        return;
    const auto dispBytes = (sd == 1) ? 1 : (sd == 2) ? 2 : 4;
    const Config::uintptr_t base = insn.address() + insn.length() + dispBytes;
    const Config::uintptr_t target = op.val.isUndefined() ? base : op.val.getUnsigned();
    const Config::ptrdiff_t delta = branchDelta(base, target, insn, op);
    if ((sd == 1 && overflowDelta(delta, 8)) || (sd == 2 && overflowDelta(delta, 16)))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    emitSdValue(insn, op, sd, Value().setSigned(delta));
}

void AsmH16::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    // Accumulator form: when the destination is R0, bit 7 of the source EA is
    // set and the destination EAd byte is omitted (tracked here by peeking at
    // op2 for 2-EA encodings).
    switch (mode) {
    case M_NONE:
        return;
    case M_EASRC:
    case M_EAPERI: {
        // Source EA (or MOVFP/MOVFPE peripheral source): emit the EA byte,
        // setting bit 7 for accumulator form (dst R0).  As op2 (a CMP:G dst or
        // MOVTP peripheral EAd): skip if accumulator form set the bit, else
        // emit the destination EA byte.
        if (&op == &insn.op2) {
            if (op.mode == M_DREG && op.reg == REG_R0 && insn.mode1() == M_EASRC)
                return;
            emitEa(insn, op, 0x00);
            return;
        }
        const auto dst = insn.mode2();
        const uint8_t accBit =
                ((dst == M_EADST || dst == M_EASRC) &&
                        insn.op2.mode == M_DREG && insn.op2.reg == REG_R0)
                        ? 0x80
                        : 0x00;
        emitEa(insn, op, accBit);
        return;
    }
    case M_EADST:
        // Accumulator form suppresses the dst byte (it is in the source EA's
        // bit 7); the source may be M_EASRC or the MOVFP peripheral M_EAPERI.
        if (op.mode == M_DREG && op.reg == REG_R0 &&
                (insn.mode1() == M_EASRC || insn.mode1() == M_EAPERI))
            return;
        emitEa(insn, op, 0x00);
        return;
    case M_EAMEM:
        // JMP/JSR target: a plain EA byte (no accumulator form).
        emitEa(insn, op, 0x00);
        return;
    case M_IQ:
        // 8-bit signed immediate; emit as a single operand byte.
        insn.emitOperand8(uint8_t(op.val.getSigned()));
        return;
    case M_IRQ:
    case M_SIRQ:
        // :RQ #imm4, Rnd -- one byte [Rnd(4)|Imm4(4)] from op1 (immediate) and
        // op2 (dest reg).  Same field for unsigned (M_IRQ) and signed (M_SIRQ);
        // a signed value is already two's complement, so masking suffices.
        insn.emitOperand8(uint8_t(
                (regNum(insn.op2.reg) << 4) |
                (uint8_t(insn.op1.val.getSigned()) & 0x0F)));
        return;
    case M_RR:
        // ADD:R / XCH Rns, Rnd -- one byte [Rns(4)|Rnd(4)] composed from
        // op1 (source) and op2 (destination).
        insn.emitOperand8(uint8_t(
                (regNum(insn.op1.reg) << 4) | regNum(insn.op2.reg)));
        return;
    case M_RNNO:
        // Register number in low 4 bits; upper 4 zero.
        insn.emitOperand8(regNum(op.reg));
        return;
    case M_RCC:
        // SCB: [Rn(4)|cc(4)] -- op1's register plus the /cc suffix.
        insn.emitOperand8(uint8_t((regNum(op.reg) << 4) | (uint8_t(insn.ccSuffix) & 0x0F)));
        return;
    case M_STROP:
        // String op: opcode = [selector|Rnc] (direction already in selector);
        // then [Rns|Rnd]; for 4-register forms (opcode bit 7) also [Rnf|cc].
        // The register tuple is regList = Rns, Rnd, Rnc[, Rnf].
        insn.embed(insn.regList[2]);  // Rnc
        insn.emitOperand8(uint8_t((insn.regList[0] << 4) | insn.regList[1]));  // [Rns|Rnd]
        if (insn.opCode() & 0x80)
            insn.emitOperand8(
                    uint8_t((insn.regList[3] << 4) | (uint8_t(insn.ccSuffix) & 0x0F)));
        return;
    case M_DISP: {
        // Branch disp = target - (address past the disp bytes).  Sd comes from
        // sizeSuffix (the opcode's low 2 bits for plain branches, the prefix
        // for Bcc:G).
        const auto sd = uint8_t(insn.sizeSuffix);
        const auto dispBytes = (sd == 0) ? 1 : (sd == 1) ? 2 : 4;
        // base = past the disp = opcode/prefix + operand bytes already emitted
        // (e.g. SCB's [Rn|cc] byte) + the disp.  An undefined forward ref uses
        // base as the target so the field is zero until the label resolves.
        const Config::uintptr_t base = insn.address() + insn.operandPos() + dispBytes;
        const Config::uintptr_t target = op.val.isUndefined() ? base : op.val.getUnsigned();
        const Config::ptrdiff_t delta = branchDelta(base, target, insn, op);
        // An explicit .B/.W branch must reach the target, else the disp would
        // silently truncate (smart-branch picks a fitting size, so never trips).
        if ((sd == 0 && overflowDelta(delta, 8)) || (sd == 1 && overflowDelta(delta, 16)))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        switch (sd) {
        case 0: insn.emitOperand8(uint8_t(int8_t(delta))); break;
        case 1: insn.emitOperand16(uint16_t(int16_t(delta))); break;
        case 2: insn.emitOperand32(uint32_t(delta)); break;
        }
        return;
    }
    case M_FRMSZ: {
        // Frame size width follows the .B/.W/.L suffix (SZ_BYTE/WORD/LONG =
        // 0/1/2); emitValue range-checks it.
        const auto sd = uint8_t(insn.sizeSuffix);
        emitValue(insn, op, sd == 0 ? 8 : sd == 1 ? 16 : 32, op.val.getSigned());
        return;
    }
    case M_TRAPV:
        insn.emitOperand8(fitQuickField(insn, op, 4));
        return;
    case M_CR:
        // CR-code byte is emitted before EA byte by the caller.
        return;
    case M_SCNTR:
    case M_BNUMR:
        // Dynamic shift/bit count: embed Rn into low 4 bits of opcode byte.
        insn.embed(regNum(op.reg));
        return;
    case M_SCNTI:
    case M_BNUMI:
        // Static shift/bit count: embed Imm5 into low 5 bits of opcode byte.
        insn.embed(fitQuickField(insn, op, 5));
        return;
    default:
        insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        return;
    }
}

Error AsmH16::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    insn.parseClassAndSize(parser());
    // Bail on a malformed :class / .size / /cc suffix before parsing operands,
    // so the precise error survives instead of a later UNKNOWN_INSTRUCTION.
    if (insn.hasError())
        return _insn.setError(insn);

    // Parse the operands.  A register run (STM/LDM/PGBN list or a
    // BFMOV/string/bit-field tuple) is gathered into insn.regList; op1..op3 hold
    // at most three slots, a run folding into one M_RLIST slot.  op1/op2 parse
    // first; a third operand marks a list/tuple form, where the leading run
    // collapses to M_RLIST, parseRegList consumes further run registers, and the
    // trailing memory EAs fill the remaining slots.
    if (!endOfLine(scan.skipSpaces())) {
        if (parseOperand(scan, insn.op1) && insn.op1.hasError())
            return _insn.setError(insn.op1);
        gatherRunReg(insn, insn.op1);
    }
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.op2) && insn.op2.hasError())
            return _insn.setError(insn.op2);
        gatherRunReg(insn, insn.op2);
    }
    if (scan.skipSpaces().expect(',')) {
        Operand *const slot[] = {&insn.op1, &insn.op2, &insn.op3};
        int ea;  // first slot index for a trailing memory EA
        if (isRunReg(insn.op1)) {       // leading run (STM/PGBN/BFx/string)
            insn.op1.mode = M_RLIST;
            if (isRunReg(insn.op2)) {   // op2 was a further run register
                insn.op2 = Operand();
                ea = 1;
            } else {                    // op2 already holds the first EA
                ea = 2;
            }
        } else {                        // leading EA then run (LDM)
            insn.op2.mode = M_RLIST;
            ea = 2;
        }
        if (const auto error = parseRegList(scan, insn))
            return _insn.setError(scan, error);
        // The first non-register operand and any after it are trailing EAs.
        if (!endOfLine(scan.skipSpaces())) {
            for (;;) {
                if (ea > 2)
                    return _insn.setError(scan, OPERAND_NOT_ALLOWED);
                if (parseOperand(scan, *slot[ea]) && slot[ea]->hasError())
                    return _insn.setError(*slot[ea]);
                ++ea;
                if (!scan.skipSpaces().expect(','))
                    break;
            }
        }
    }
    scan.skipSpaces();

    if (searchName(cpuType(), insn))
        return _insn.setError(insn);

    // Trailing text is not an implicit comment in H16, so reject it rather than
    // silently ignore it (e.g. "ADD:G.W R0, R1 junk").
    if (!endOfLine(scan))
        return _insn.setError(scan, GARBAGE_AT_END);

    // A /cc or /F|/B suffix is only valid on ops that consume one: TRAP/SET
    // (IC_CC), SCB (M_RCC), string ops (M_STROP).  Reject it elsewhere, so
    // "ADD/EQ" is an unknown instruction, not a silent ADD.
    const auto takesSuffix =
            insn.insnClass() == IC_CC || insn.mode1() == M_RCC || insn.mode1() == M_STROP;
    if (!takesSuffix && (insn.ccSuffix != CC_UNDEF || insn.dirSuffix != DIR_NONE))
        return _insn.setError(insn, UNKNOWN_INSTRUCTION);
    // A string op without the cc selector (SMOV/SSTR, opcode bit 7 clear) takes
    // a direction but no cc.  A lone "/F" is the direction (tentative cc
    // ignored); a confirmed cc (a real /cc, or SMOV/F/B) is an unknown form.
    if (insn.mode1() == M_STROP && !(insn.opCode() & 0x80) && insn.ccSuffix != CC_UNDEF &&
            !insn.ccFromLoneF)
        return _insn.setError(insn, UNKNOWN_INSTRUCTION);

    // Reject extra operands for instructions that aren't register-tuple ops
    // (string / bit-field) or register-list ops (STM/LDM/PGBN).
    if (insn.mode1() != M_STROP && insn.mode1() != M_BITF && insn.mode1() != M_RLIST &&
            insn.mode2() != M_RLIST && insn.op3.mode != M_NONE)
        return _insn.setError(insn.op3, OPERAND_NOT_ALLOWED);

    // A missing size suffix on a sized instruction defaults to .W (matching
    // ASL); branches instead auto-size by distance under the smart-branch
    // option.
    const auto isz = insn.insnSize();
    if (insn.sizeSuffix == SZ_NONE &&
            (isz == ISZ_DATA || isz == ISZ_OBIT0 || isz == ISZ_EXTU ||
                    isz == ISZ_PRSZ || isz == ISZ_OBIT4 || isz == ISZ_OWL)) {
        if (insn.mode1() == M_DISP && _smartBranch)
            // Plain branch / Bcc:G: target is op1, disp follows the opcode
            // (1 byte) or the prefix+opcode (2 bytes for Bcc:G).
            insn.sizeSuffix = pickBranchSize(insn, insn.op1, isz == ISZ_PRSZ ? 2 : 1);
        else if (insn.mode2() == M_DISP && _smartBranch)
            // SCB: target is op2; disp follows the opcode + [Rn|cc] byte.
            insn.sizeSuffix = pickBranchSize(insn, insn.op2, 2);
        else if (insn.mode1() == M_FRMSZ || insn.mode2() == M_FRMSZ) {
            // LINK/RTD: the manual shows no suffix, so auto-size the frame to
            // the smallest form that holds it (.W for an undefined forward ref).
            const auto &fop = insn.mode1() == M_FRMSZ ? insn.op1 : insn.op2;
            insn.sizeSuffix =
                    fop.val.isUndefined() ? SZ_WORD : pickFrameSize(fop.val.getSigned());
        } else
            insn.sizeSuffix = SZ_WORD;
    }

    if (isz == ISZ_DATA) {
        // 2-bit Sz in the opcode low bits (Sz=11 reserved is unreachable here).
        insn.embed(uint8_t(insn.sizeSuffix));
    } else if (isz == ISZ_OBIT0) {
        // SWAP: 1-bit byte/word size at opcode bit 0 (no long).
        if (insn.sizeSuffix == SZ_LONG)
            return _insn.setError(scan, ILLEGAL_SIZE);
        insn.embed(uint8_t(insn.sizeSuffix));
    } else if (isz == ISZ_OWL) {
        // MOVTP/MOVFP: opcode bit 0 selects word(0) or long(1); no byte.
        if (insn.sizeSuffix == SZ_BYTE)
            return _insn.setError(scan, ILLEGAL_SIZE);
        if (insn.sizeSuffix == SZ_LONG)
            insn.embed(0x01);
    } else if (isz == ISZ_EXTU) {
        // EXTU/EXTS Sz mapping: W=00, L=01, B=10
        uint8_t sz = 0;
        switch (insn.sizeSuffix) {
        case SZ_WORD: sz = 0; break;
        case SZ_LONG: sz = 1; break;
        case SZ_BYTE: sz = 2; break;
        default: break;
        }
        insn.embed(sz);
    } else if (isz == ISZ_PRSZ) {
        // Multi-byte family (SFT/BIT/Bcc:G): Sz lives in the prefix byte.
        // String ops (SMOV/SSTR/SCMP/SSCH) are byte/word only -- a .L would
        // bump the prefix past its 0x94/0x95 range into an undecodable byte.
        if (insn.mode1() == M_STROP && insn.sizeSuffix == SZ_LONG)
            return _insn.setError(scan, ILLEGAL_SIZE);
        insn.embedPrefix(uint8_t(insn.sizeSuffix));
    } else if (isz == ISZ_OBIT4) {
        // MUL/DIV: size bit is bit 4 of the mode byte (only .B/.W are legal).
        if (insn.sizeSuffix == SZ_LONG)
            return _insn.setError(scan, ILLEGAL_SIZE);
        if (insn.sizeSuffix == SZ_WORD)
            insn.embed(0x10);
    } else if (isz == ISZ_FIXED || isz == ISZ_NONE) {
        // No opcode size field: accept only the suffix matching the manual's
        // operand size (ISZ_FIXED carries it), and none for ISZ_NONE.
        const auto natural = impliedSize(insn);
        if (insn.sizeSuffix != SZ_NONE && insn.sizeSuffix != natural)
            return _insn.setError(scan, ILLEGAL_SIZE);
    }

    // ANDC/ORC/XORC/LDC/STC byte order is [opcode][CR-code][EA]: emit the
    // CR-code byte right after the opcode.
    if (insn.mode1() == M_CR || insn.mode2() == M_CR) {
        const auto cr = (insn.mode1() == M_CR) ? insn.op1.creg : insn.op2.creg;
        insn.emitOperand8(crCode(cr));
    }

    // TRAP/SET/SCB require the /cc suffix.  TRAP/SET (IC_CC) emit a standalone
    // [*|cc] byte right after the opcode; SCB packs [Rn|cc] via M_RCC.
    if (insn.insnClass() == IC_CC || insn.mode1() == M_RCC) {
        if (insn.ccSuffix == CC_UNDEF)
            return _insn.setError(scan, OPERAND_NOT_ALLOWED);
    }
    if (insn.insnClass() == IC_CC)
        insn.emitOperand8(uint8_t(insn.ccSuffix) & 0x0F);

    if (insn.mode1() == M_RLIST || insn.mode2() == M_RLIST) {
        // STM/LDM/PGBN: regMask() folds regList to a 16-bit word.  STM/LDM carry
        // a positional EA (STM: list..., EAd; LDM: EAs, list...); byte order is
        // opcode, EA, reglist.
        if (insn.mode2() == M_EADST) {  // STM: the EA is the last operand
            const Operand &ea = insn.op3.mode != M_NONE ? insn.op3 : insn.op2;
            emitEa(insn, ea, 0x00);
        } else if (insn.mode1() == M_EASRC) {  // LDM: the EA is the first operand
            emitEa(insn, insn.op1, 0x00);
        }
        insn.emitOperand16(insn.regMask());
    } else if (insn.mode1() == M_BFMOV) {
        // BFMOV: six registers regList as [Rnx|Rnb][Rny|Rno][Rns|Rnd].
        insn.emitOperand8(uint8_t((insn.regList[0] << 4) | insn.regList[1]));
        insn.emitOperand8(uint8_t((insn.regList[2] << 4) | insn.regList[3]));
        insn.emitOperand8(uint8_t((insn.regList[4] << 4) | insn.regList[5]));
    } else if (insn.mode1() == M_BITF) {
        // BFEXT/BFINS/BFSCH: regList[0..1]=Rnb,Rnf, then EAs and EAd --
        // register-direct EAs fold into regList[2..], memory EAs are op2/op3.
        insn.emitOperand8(uint8_t((insn.regList[0] << 4) | insn.regList[1]));
        Operand *const eaOps[] = {&insn.op2, &insn.op3};
        uint8_t ri = 2;  // next register-direct EA in regList
        int oi = 0;      // next memory EA operand
        for (int e = 0; e < 2; ++e) {  // EAs then EAd
            if (ri < insn.regCount)
                insn.emitOperand8(uint8_t(0x40 | insn.regList[ri++]));  // reg-direct EA
            else
                emitEa(insn, *eaOps[oi++], 0x00);
        }
    } else {
        encodeOperand(insn, insn.op1, insn.mode1());
        encodeOperand(insn, insn.op2, insn.mode2());
    }

    insn.emitInsn();
    // Propagate a deferred operand error (e.g. UNDEFINED_SYMBOL, which
    // hasError() ignores for forward refs) so it surfaces on the final pass.
    insn.setErrorIf(insn.op1);
    insn.setErrorIf(insn.op2);
    insn.setErrorIf(insn.op3);
    return _insn.setError(insn);
}

}  // namespace h16
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
