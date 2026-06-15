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

constexpr char TEXT_DC_B[] PROGMEM = "dc.b";
constexpr char TEXT_DC_W[] PROGMEM = "dc.w";
constexpr char TEXT_DC_L[] PROGMEM = "dc.l";

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_DC_B, &Assembler::defineDataConstant, Assembler::DATA_BYTE_NO_STRING },
    { TEXT_DC_L, &Assembler::defineDataConstant, Assembler::DATA_LONG },
    { TEXT_DC_W, &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING },
    { TEXT_FCB,  &Assembler::defineDataConstant, Assembler::DATA_BYTE_NO_STRING },
    { TEXT_FDB,  &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING },
    { TEXT_RMB,  &Assembler::allocateSpaces,     Assembler::DATA_BYTE           },
};
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};
// clang-format on

// Parse an explicit size qualifier ":N" where N is 8/16/32.  Returns 0 if
// absent or unrecognised; advances scan past the qualifier on success.
uint8_t parseSizeQual(StrScanner &scan) {
    auto p = scan;
    if (!p.expect(':')) return 0;
    if (p.expect('8'))               { scan = p; return 8;  }
    if (p.expect('1') && p.expect('6')) { scan = p; return 16; }
    {
        auto q = scan;
        if (q.expect(':') && q.expect('3') && q.expect('2')) { scan = q; return 32; }
    }
    return 0;
}

// Match "PC" as a two-letter token; advance on success, rewind on failure.
// (Plain iexpect('P') + iexpect('C') leaks a partial advance for "PR..".)
bool expectPC(StrScanner &scan) {
    auto save = scan;
    if (scan.iexpect('P') && scan.iexpect('C'))
        return true;
    scan = save;
    return false;
}

// Parse "*N" where N is 1/2/4/8; returns 0 if absent.
uint8_t parseScale(StrScanner &scan) {
    auto p = scan;
    if (!p.expect('*')) return 0;
    p.skipSpaces();
    if (p.expect('1')) { scan = p; return 1; }
    if (p.expect('2')) { scan = p; return 2; }
    if (p.expect('4')) { scan = p; return 4; }
    if (p.expect('8')) { scan = p; return 8; }
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

// Encode a 1, 2, or 4-byte value at the current operand position; used for
// disp / imm / abs extensions.
void emitValue(AsmInsn &insn, uint8_t size, uint32_t value) {
    switch (size) {
    case 8:  insn.emitOperand8(uint8_t(value));   break;
    case 16: insn.emitOperand16(uint16_t(value)); break;
    case 32: insn.emitOperand32(value);           break;
    }
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

// Emit the EA byte and any extension bytes for |op|.  The high bit of the
// EA byte (accumulator-form marker) is supplied by the caller via |accBit|.
void emitEa(AsmInsn &insn, const Operand &op, uint8_t accBit) {
    // Bank prefix: prepend 0x70 / 0x74 when the register is CRn / PRn.
    // The accumulator-form marker (bit 7) goes on the FIRST byte of the EA
    // field (Manual 4.5.2, Figure 4-41 and Note 4 to Table 4-7); when a
    // bank prefix is present, that's the prefix byte and the inner EA byte
    // carries acc=0.
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
        uint8_t sd = 0;
        if (op.dispSize)
            sd = sdFromSize(op.dispSize);
        else if (op.val.isUndefined() || !op.val.getSigned())
            sd = 0;
        else
            sd = sdFromSize(pickDispSize(op.val.getSigned(), 0));
        insn.emitOperand8(accBit | (sd << 4) | rn);
        if (sd)
            emitValue(insn, sd == 1 ? 8 : (sd == 2 ? 16 : 32),
                    uint32_t(op.val.getSigned()));
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
        emitValue(insn, size, uint32_t(op.val.getSigned()));
        return;
    }
    case M_ABS: {
        const uint8_t size = op.absSize ? op.absSize :
                pickDispSize(op.val.getSigned(), 0);
        const uint8_t sa = sdFromSize(size);
        insn.emitOperand8(accBit | 0x74 | sa);
        emitValue(insn, size, uint32_t(op.val.getSigned()));
        return;
    }
    case M_SCALE: {
        const uint8_t sd = op.dispSize ? sdFromSize(op.dispSize) :
                (op.val.getSigned() ? sdFromSize(pickDispSize(op.val.getSigned(), 0)) : 0);
        insn.emitOperand8(accBit | 0x78 | sd);
        const uint8_t ext = (sfFromScale(op.scale) << 4) | regNum(op.reg);
        insn.emitOperand8(ext);
        if (sd)
            emitValue(insn, sd == 1 ? 8 : (sd == 2 ? 16 : 32),
                    uint32_t(op.val.getSigned()));
        return;
    }
    case M_INDEX: {
        // @([disp,] Xm[.Sz][*Sf], Rn)
        const uint8_t sd = op.dispSize ? sdFromSize(op.dispSize) :
                (op.val.getSigned() ? sdFromSize(pickDispSize(op.val.getSigned(), 0)) : 0);
        insn.emitOperand8(accBit | 0x7C);
        const uint8_t ext1 = (op.indexSize ? 0x40 : 0x00) | (sd << 4) | regNum(op.reg);
        const uint8_t ext2 = (sfFromScale(op.scale) << 4) | regNum(op.indexReg);
        insn.emitOperand8(ext1);
        insn.emitOperand8(ext2);
        if (sd)
            emitValue(insn, sd == 1 ? 8 : (sd == 2 ? 16 : 32),
                    uint32_t(op.val.getSigned()));
        return;
    }
    case M_PCREL: {
        // @PC or @(target, PC): disp = target - PC, where PC = address of
        // the next instruction (= start of EAd + 2 + dispBytes).  Pick disp
        // size from the delta so distant targets get word/long encodings.
        // pcAtPick = (current emit pos) + (EAd byte) + (ext byte).
        const uint32_t pcAtPick = insn.address() + insn.length() + 2;
        uint8_t sd;
        if (op.dispSize) {
            sd = sdFromSize(op.dispSize);
        } else if (op.val.isUndefined() || !op.val.getSigned()) {
            sd = 0;
        } else {
            const int32_t delta1 = int32_t(op.val.getUnsigned()) - int32_t(pcAtPick + 1);
            const int32_t delta2 = int32_t(op.val.getUnsigned()) - int32_t(pcAtPick + 2);
            if (delta1 >= -0x80 && delta1 <= 0x7F) {
                sd = 1;
            } else if (delta2 >= -0x8000 && delta2 <= 0x7FFF) {
                sd = 2;
            } else {
                sd = 3;
            }
        }
        insn.emitOperand8(accBit | 0x7D);
        insn.emitOperand8(0x80 | (sd << 4));
        if (sd) {
            const auto dispBytes = (sd == 1 ? 1 : sd == 2 ? 2 : 4);
            const auto pc = insn.address() + insn.length() + dispBytes;
            const int32_t delta = int32_t(op.val.getUnsigned()) - int32_t(pc);
            // When the source forced a size with :8 / :16, the actual delta
            // must fit in that signed range.  Otherwise the encoded bytes
            // would silently point at a different target.
            if ((sd == 1 && (delta < -0x80 || delta > 0x7F)) ||
                    (sd == 2 && (delta < -0x8000 || delta > 0x7FFF)))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            emitValue(insn, sd == 1 ? 8 : (sd == 2 ? 16 : 32), uint32_t(delta));
        }
        return;
    }
    case M_PCIDX: {
        // @([target,] Xm, PC): same delta computation as M_PCREL but with
        // an extra ext2 byte (so EAd + 3 bytes before the disp).
        const uint32_t pcAtPick = insn.address() + insn.length() + 3;
        uint8_t sd;
        if (op.dispSize) {
            sd = sdFromSize(op.dispSize);
        } else if (op.val.isUndefined() || !op.val.getSigned()) {
            sd = 0;
        } else {
            const int32_t delta1 = int32_t(op.val.getUnsigned()) - int32_t(pcAtPick + 1);
            const int32_t delta2 = int32_t(op.val.getUnsigned()) - int32_t(pcAtPick + 2);
            if (delta1 >= -0x80 && delta1 <= 0x7F) {
                sd = 1;
            } else if (delta2 >= -0x8000 && delta2 <= 0x7FFF) {
                sd = 2;
            } else {
                sd = 3;
            }
        }
        insn.emitOperand8(accBit | 0x7D);
        const uint8_t ext1 = (op.indexSize ? 0x40 : 0x00) | (sd << 4);
        const uint8_t ext2 = (sfFromScale(op.scale) << 4) | regNum(op.indexReg);
        insn.emitOperand8(ext1);
        insn.emitOperand8(ext2);
        if (sd) {
            const auto dispBytes = (sd == 1 ? 1 : sd == 2 ? 2 : 4);
            const auto pc = insn.address() + insn.length() + dispBytes;
            const int32_t delta = int32_t(op.val.getUnsigned()) - int32_t(pc);
            if ((sd == 1 && (delta < -0x80 || delta > 0x7F)) ||
                    (sd == 2 && (delta < -0x8000 || delta > 0x7FFF)))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            emitValue(insn, sd == 1 ? 8 : (sd == 2 ? 16 : 32), uint32_t(delta));
        }
        return;
    }
    case M_DIND: {
        // @@Rn or @(ds2, @(ds1, Rn)); legal (S1,S2) per Table 16-7.
        const uint8_t s1 = sdFromSize(op.ds1Size);  // 0, 1 (byte), or 3 (long)
        const uint8_t s2 = sdFromSize(op.ds2Size);
        insn.emitOperand8(accBit | 0x7E);
        insn.emitOperand8((s2 << 6) | (s1 << 4) | regNum(op.reg));
        if (op.ds1Size)
            emitValue(insn, op.ds1Size, uint32_t(op.val.getSigned()));
        if (op.ds2Size)
            emitValue(insn, op.ds2Size, uint32_t(op.val2.getSigned()));
        return;
    }
    default:
        return;
    }
}

}  // namespace

// Mnemonic carries the size as ".B" / ".W" / ".L"; strip it and store the
// extracted size for later use during encoding.
OprSize AsmInsn::parseSizeSuffix() {
    StrScanner p{name()};
    p.trimStart([](char c) { return c != '.'; });
    auto eos = const_cast<char *>(p.str());
    sizeSuffix = SZ_NONE;
    if (p.expect('.')) {
        if (p.iexpect('B'))      sizeSuffix = SZ_BYTE;
        else if (p.iexpect('W')) sizeSuffix = SZ_WORD;
        else if (p.iexpect('L')) sizeSuffix = SZ_LONG;
        if (sizeSuffix == SZ_NONE || *p) {
            setErrorIf(p, ILLEGAL_SIZE);
            return SZ_NONE;
        }
        *eos = 0;
    }
    return sizeSuffix;
}

namespace {

// H16 mnemonics embed ':' (e.g. ADD:G, MOV:RQ) and '.' (e.g. ADD:G.W).
// '*' is the current-location symbol in Hitachi syntax.
struct H16SymbolParser final : SymbolParser {
    bool instructionLetter(char c) const override {
        return SymbolParser::instructionLetter(c) || c == ':' || c == '.';
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
        // @@Rn or @@(...) -- double indirect short form
        if (p.expect('@')) {
            // @@Rn is the short form for @(0:8, @(0:8, Rn)); per Table 16-7
            // legal (S1,S2) combos are (01,01), (01,11), (11,01), (11,11),
            // so the shorthand picks the smallest -- 8-bit, 8-bit, both 0.
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
            auto sp = p.skipSpaces();
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
                op.val = firstExpr;
                op.dispSize = firstSize;
                op.indexReg = reg1;
                op.indexSize = (idxSize1 < 0) ? 1 : uint8_t(idxSize1);
                op.scale = scale1 ? scale1 : 1;
                op.reg = reg2;
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

    // Register direct: Rn, CRn, PRn
    auto rSave = p;
    const auto reg = parseRegName(p, parser());
    if (reg != REG_UNDEF) {
        op.reg = reg;
        op.mode = M_DREG;
        scan = p;
        return OK;
    }
    p = rSave;

    // Bare expression -- could be a branch target (PC-relative computed) or
    // an immediate without #.  The acceptMode layer will sort out which.
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_DISP;
    scan = p;
    return OK;
}

void AsmH16::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    // Accumulator form: when the table expects M_EAD and the parsed source
    // was M_EA, and the destination is R0 (M_DREG, REG_R0), the caller sets
    // bit 7 of the source EA and omits the destination EAd byte.  We track
    // that here by peeking at op2 for 2-EA encodings.
    switch (mode) {
    case M_NONE:
        return;
    case M_EA: {
        // When called for the source operand (insn.src() == M_EA): emit the
        // source EA byte, setting bit 7 if the destination is register-
        // direct R0 (accumulator form).
        // When called for the destination operand (insn.dst() == M_EA, e.g.
        // CMP:G / CMPS): skip if accumulator form already set the bit; else
        // emit the destination EA byte.
        if (&op == &insn.op2) {
            if (op.mode == M_DREG && op.reg == REG_R0 && insn.src() == M_EA)
                return;
            emitEa(insn, op, 0x00);
            return;
        }
        const auto dst = insn.dst();
        const uint8_t accBit =
                ((dst == M_EAD || dst == M_EA) &&
                        insn.op2.mode == M_DREG && insn.op2.reg == REG_R0)
                        ? 0x80
                        : 0x00;
        emitEa(insn, op, accBit);
        return;
    }
    case M_EAD:
        // Accumulator form suppresses the dst byte (already in src bit 7).
        if (op.mode == M_DREG && op.reg == REG_R0 && insn.src() == M_EA)
            return;
        emitEa(insn, op, 0x00);
        return;
    case M_IQ:
        // 8-bit signed immediate; emit as a single operand byte.
        insn.emitOperand8(uint8_t(op.val.getSigned()));
        return;
    case M_IRQ:
        // ADD:RQ #imm4, Rnd -- one byte [Rnd(4)|Imm4(4)] composed from op1
        // (the immediate) and op2 (the destination register).
        insn.emitOperand8(uint8_t(
                (regNum(insn.op2.reg) << 4) |
                (uint8_t(insn.op1.val.getUnsigned()) & 0x0F)));
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
    case M_DISP: {
        // Branch disp = target - (address past the disp bytes).  Sd comes
        // from the size suffix; for unprefixed branches (BRA/BEQ/BNE/BSR)
        // it lives in the opcode byte's low 2 bits, for Bcc:G it lives in
        // the prefix.  Either way insn.sizeSuffix already carries the size.
        const int32_t target = static_cast<int32_t>(op.val.getUnsigned());
        const auto sd = uint8_t(insn.sizeSuffix);
        const auto dispBytes = (sd == 0) ? 1 : (sd == 1) ? 2 : 4;
        const auto opcodeBytes = insn.hasPrefix() ? 2 : 1;
        // Bcc:G has an extra cc byte between prefix and disp.
        const auto ccBytes = insn.hasPrefix() ? 0 : 0;  // cc is the opcode itself for Bcc:G
        const auto pc = insn.address() + opcodeBytes + ccBytes + dispBytes;
        const int32_t delta = target - int32_t(pc);
        switch (sd) {
        case 0: insn.emitOperand8(uint8_t(int8_t(delta))); break;
        case 1: insn.emitOperand16(uint16_t(int16_t(delta))); break;
        case 2: insn.emitOperand32(uint32_t(delta)); break;
        }
        return;
    }
    case M_FRMSZ: {
        const auto sd = insn.opCode() & 0x03;
        const int32_t v = op.val.getSigned();
        switch (sd) {
        case 0: insn.emitOperand8(uint8_t(int8_t(v))); break;
        case 1: insn.emitOperand16(uint16_t(int16_t(v))); break;
        case 2: insn.emitOperand32(uint32_t(v)); break;
        }
        return;
    }
    case M_ITRPN:
        insn.emitOperand8(uint8_t(op.val.getUnsigned()) & 0x0F);
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
        insn.embed(uint8_t(op.val.getUnsigned()) & 0x1F);
        return;
    default:
        insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        return;
    }
}

Error AsmH16::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    insn.parseSizeSuffix();

    if (parseOperand(scan, insn.op1) && insn.op1.hasError())
        return _insn.setError(insn.op1);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.op2) && insn.op2.hasError())
            return _insn.setError(insn.op2);
    }
    scan.skipSpaces();

    if (searchName(cpuType(), insn))
        return _insn.setError(insn);

    // Sized instructions require a .B/.W/.L suffix (Sz=11 is reserved).
    // Fixed-size instructions reject any size suffix.
    const auto isz = insn.insnSize();
    if (isz == ISZ_DATA) {
        if (insn.sizeSuffix == SZ_NONE)
            return _insn.setError(scan, ILLEGAL_SIZE);
        insn.embed(uint8_t(insn.sizeSuffix));
    } else if (isz == ISZ_AUTO) {
        // Suffix optional; if missing, auto-pick Sd from the FRMSZ operand
        // value (RTD #imm / LINK Rn,#imm).
        OprSize sz = insn.sizeSuffix;
        if (sz == SZ_NONE) {
            const auto v = insn.op2.mode == M_IMM ? insn.op2.val.getSigned()
                                                  : insn.op1.val.getSigned();
            if (v >= -0x80 && v <= 0x7F)        sz = SZ_BYTE;
            else if (v >= -0x8000 && v <= 0x7FFF) sz = SZ_WORD;
            else                                  sz = SZ_LONG;
        }
        insn.embed(uint8_t(sz));
    } else if (isz == ISZ_EXTU) {
        if (insn.sizeSuffix == SZ_NONE)
            return _insn.setError(scan, ILLEGAL_SIZE);
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
        if (insn.sizeSuffix == SZ_NONE)
            return _insn.setError(scan, ILLEGAL_SIZE);
        insn.embedPrefix(uint8_t(insn.sizeSuffix));
    } else if (isz == ISZ_OBIT4) {
        // MUL/DIV: size bit is bit 4 of the mode byte.
        if (insn.sizeSuffix == SZ_NONE)
            return _insn.setError(scan, ILLEGAL_SIZE);
        if (insn.sizeSuffix == SZ_LONG)
            return _insn.setError(scan, ILLEGAL_SIZE);
        if (insn.sizeSuffix == SZ_WORD)
            insn.embed(0x10);
    } else if (isz == ISZ_WORD || isz == ISZ_FIXB || isz == ISZ_FIXL) {
        // Fixed-size instruction (MOVTP/MOVTPE/SWAP word; DNEG byte; XCH long).
        const auto expected =
                (isz == ISZ_WORD) ? SZ_WORD :
                (isz == ISZ_FIXL) ? SZ_LONG : SZ_BYTE;
        if (insn.sizeSuffix != SZ_NONE && insn.sizeSuffix != expected)
            return _insn.setError(scan, ILLEGAL_SIZE);
    } else if (isz == ISZ_NONE) {
        if (insn.sizeSuffix != SZ_NONE)
            return _insn.setError(scan, ILLEGAL_SIZE);
    }

    // ANDC/ORC/XORC/LDC/STC byte order is [opcode][CR-code][EA].  Emit the
    // CR-code byte right after the opcode (before any EA operand bytes).
    if (insn.src() == M_CR || insn.dst() == M_CR) {
        const auto cr = (insn.src() == M_CR) ? insn.op1.creg : insn.op2.creg;
        insn.emitOperand8(crCode(cr));
    }

    encodeOperand(insn, insn.op1, insn.src());
    encodeOperand(insn, insn.op2, insn.dst());

    insn.emitInsn();
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
