/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "asm_h8500.h"
#include "reg_h8500.h"
#include "table_h8500.h"
#include "text_common.h"
#include "text_h8500.h"

using namespace libasm::text::common;
using namespace libasm::text::h8500;

namespace libasm {
namespace h8500 {

using namespace pseudo;
using namespace reg;

namespace {

// clang-format off
// H8/500 shares H8/300's data/space directives. The dotted names are not in
// text_common, so (as in asm_h8300.cpp) they are defined locally; the "d"
// prefix marks the dotted spelling (".DATA", distinct from common "data").
constexpr char TEXT_dDATA[]   PROGMEM = ".DATA";
constexpr char TEXT_dDATA_B[] PROGMEM = ".DATA.B";
constexpr char TEXT_dDATA_W[] PROGMEM = ".DATA.W";
constexpr char TEXT_dDATA_L[] PROGMEM = ".DATA.L";
constexpr char TEXT_dSDATA[]  PROGMEM = ".SDATA";
constexpr char TEXT_dRES[]    PROGMEM = ".RES";
constexpr char TEXT_dRES_B[]  PROGMEM = ".RES.B";
constexpr char TEXT_dRES_W[]  PROGMEM = ".RES.W";
constexpr char TEXT_dRES_L[]  PROGMEM = ".RES.L";

// H8/500-specific directives, dotted to match libasm's other directives. The
// common TEXT_SET / TEXT_MAXMODE are the unrelated bare "SET" / "MAXMODE", so
// the dotted spellings are defined locally with the "d" prefix.
constexpr char TEXT_dSET[]     PROGMEM = ".set";
constexpr char TEXT_dMAXMODE[] PROGMEM = ".maxmode";

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

// Reuse the Hitachi location symbols ('$', '*', '.'); H8/500 mnemonics also
// include ':' (MOV:G, ADD:Q, CMP:E, ...) and '/' (SCB/EQ, TRAP/VS), plus the
// '.b'/'.w' size suffix, as part of the mnemonic token.
struct H8500SymbolParser final : HitachiSymbolParser {
    bool instructionLetter(char c) const override {
        return HitachiSymbolParser::instructionLetter(c) || c == ':' || c == '/' || c == '.';
    }
};

}  // namespace

const ValueParser::Plugins &AsmH8500::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return HitachiNumberParser::singleton(); }
        const CommentParser &comment() const override {
            return SemicolonCommentParser::singleton();
        }
        const SymbolParser &symbol() const override {
            static const H8500SymbolParser SYMBOL;
            return SYMBOL;
        }
        const LetterParser &letter() const override { return HitachiLetterParser::singleton(); }
        const OperatorParser &operators() const override {
            return HitachiOperatorParser::singleton();
        }
    } PLUGINS;
    return PLUGINS;
}

constexpr AsmH8500::PseudoH8500 AsmH8500::PSEUDO_H8500_TABLE[] PROGMEM = {
        {TEXT_dMAXMODE, &AsmH8500::processMaxMode},  // ".maxmode"
        {TEXT_dSET, &AsmH8500::processSet},          // ".set"
};
PROGMEM constexpr AsmH8500::PseudosH8500 AsmH8500::PSEUDOS_H8500{ARRAY_RANGE(PSEUDO_H8500_TABLE)};

AsmH8500::AsmH8500(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_maxMode),
      Config(TABLE),
      _opt_maxMode(this, &Config::setMaxMode, OPT_BOOL_MAX_MODE, OPT_DESC_MAX_MODE) {
    reset();
}

void AsmH8500::reset() {
    Assembler::reset();
    setMaxMode(false);
    resetPageRegisters();  // page registers start UNDEFINED
    // H8/500 has no explicit d:16 branch mnemonic, so default smart branch on.
    setSmartBranch(true);
}

Error AsmH8500::processPseudo(StrScanner &scan, Insn &insn) {
    const auto *p = PSEUDOS_H8500.search(insn.name());
    return p ? p->invoke(this, scan, insn) : Assembler::processPseudo(scan, insn);
}

Error AsmH8500::processMaxMode(StrScanner &scan, Insn &insn, uint16_t) {
    auto p = scan.skipSpaces();
    const auto at = p;
    bool on;
    if (p.expectTrue()) {
        on = true;
    } else if (p.expectFalse()) {
        on = false;
    } else {
        return insn.setError(at, UNKNOWN_OPERAND);
    }
    scan = p;
    if (setMaxMode(on) != OK)
        return insn.setError(at, OPERAND_NOT_ALLOWED);
    return OK;
}

// `.set <page register>, <value>`: record the assumed value (0-255) of a page
// register (DP/EP/TP/CP/BR) so the assembler can shorten absolute operands. A
// negative value restores the undefined state.
Error AsmH8500::processSet(StrScanner &scan, Insn &insn, uint16_t) {
    auto p = scan.skipSpaces();
    const auto at = p;
    const auto reg = parsePageReg(p, parser());
    if (reg == PAGE_ENUM_END)
        return insn.setError(at, UNKNOWN_REGISTER);
    if (!p.skipSpaces().expect(','))
        return insn.setError(p, MISSING_COMMA);
    p.skipSpaces();
    const auto valueAt = p;
    ErrorAt error;
    const auto value = parseInteger(p, error);
    if (error.hasError())
        return insn.setError(error);
    if (setPageRegister(reg, value.getSigned()) != OK)
        return insn.setError(valueAt, OVERFLOW_RANGE);
    scan = p;
    return OK;
}

// Return the ":8"/":16"/":24" size qualifier (advancing scan), or 0 if absent.
static uint8_t parseSizeQual(StrScanner &scan) {
    auto p = scan;
    if (!p.expect(':'))
        return 0;
    Value size;
    if (size.read(p) != OK)
        return 0;
    const auto bits = size.getUnsigned();
    if (bits != 8 && bits != 16 && bits != 24)
        return 0;
    scan = p;
    return static_cast<uint8_t>(bits);
}

Error AsmH8500::parseOperand(StrScanner &scan, Operand &op) const {
    op.setAt(scan.skipSpaces());
    auto p = scan;

    if (p.expect('@')) {
        p.skipSpaces();

        // @-Rn pre-decrement
        if (p.expect('-')) {
            const auto at = p.skipSpaces();
            const auto reg = parseRegName(p, parser());
            if (reg != REG_UNDEF) {
                op.mode = M_PDEC;
                op.reg = reg;
                scan = p;
                return OK;
            }
            return op.setError(at, UNKNOWN_REGISTER);
        }

        // @(d,Rn)
        if (p.expect('(')) {
            const auto disp = parseInteger(p.skipSpaces(), op);
            if (op.hasError())
                return op.getError();
            if (!p.skipSpaces().expect(','))
                return op.setError(p, MISSING_COMMA);
            const auto at = p.skipSpaces();
            const auto reg = parseRegName(p, parser());
            if (reg == REG_UNDEF)
                return op.setError(at, UNKNOWN_REGISTER);
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            op.val = disp;
            op.reg = reg;
            const auto d = disp.getSigned();
            if (reg == REG_FP && d >= -128 && d <= 127) {
                op.mode = M_FPX8;
            } else if (d >= -128 && d <= 127) {
                op.mode = M_IDX8;
            } else {
                op.mode = M_IDX16;
            }
            scan = p;
            return OK;
        }

        // @Rn or @Rn+
        {
            auto q = p;
            const auto reg = parseRegName(q, parser());
            if (reg != REG_UNDEF) {
                if (q.skipSpaces().expect('+')) {
                    op.mode = M_PINC;
                } else {
                    op.mode = M_IND;
                }
                op.reg = reg;
                scan = q;
                return OK;
            }
        }

        // @addr with optional :8/:16/:24 qualifier
        {
            const auto addr = parseInteger(p, op);
            if (op.hasError())
                return op.getError();
            op.val = addr;
            const auto sz = parseSizeQual(p);
            op.absSize = sz;  // 0 when no :size suffix was written
            if (sz == 8) {
                op.mode = M_ABS8;
            } else if (sz == 24) {
                op.mode = M_ABS24;
            } else {
                const auto v = addr.getUnsigned();
                op.mode = (sz == 16 || v <= 0xFFFF) ? M_ABS16 : M_ABS24;
            }
            scan = p;
            return OK;
        }
    }

    if (p.expect('#')) {
        const auto val = parseInteger(p.skipSpaces(), op);
        if (op.hasError())
            return op.getError();
        op.val = val;
        const auto sz = parseSizeQual(p);
        if (sz == 8) {
            op.mode = M_IMM8;
        } else if (sz == 16) {
            op.mode = M_IMM16;
        } else {
            op.mode = val.overflowUint8() ? M_IMM16 : M_IMM8;
        }
        scan = p;
        return OK;
    }

    // (Rn,Rm-Rl,...) register list for LDM/STM
    if (p.expect('(')) {
        uint8_t mask = 0;
        do {
            auto at = p.skipSpaces();
            const auto lo = parseRegName(p, parser());
            if (lo == REG_UNDEF)
                return op.setError(at, UNKNOWN_REGISTER);
            auto hi = lo;
            if (p.skipSpaces().expect('-')) {
                at = p.skipSpaces();
                hi = parseRegName(p, parser());
                if (hi == REG_UNDEF)
                    return op.setError(at, UNKNOWN_REGISTER);
                if (hi < lo)
                    return op.setError(at, ILLEGAL_REGISTER);
            }
            for (auto r = static_cast<int>(lo); r <= static_cast<int>(hi); r++)
                mask |= static_cast<uint8_t>(1 << r);
        } while (p.skipSpaces().expect(','));
        if (!p.skipSpaces().expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        op.mode = M_REGLIST;
        op.val.setUnsigned(mask);
        scan = p;
        return OK;
    }

    // Control register (CCR, SR, BR, EP, DP, TP)
    {
        auto q = p;
        const auto cr = parseCrName(q, parser());
        if (cr != CR_UNDEF) {
            op.mode = M_CR;
            op.creg = cr;
            scan = q;
            return OK;
        }
    }

    // General register Rn (or FP/SP)
    {
        auto q = p;
        const auto reg = parseRegName(q, parser());
        if (reg != REG_UNDEF) {
            op.mode = M_REG;
            op.reg = reg;
            scan = q;
            return OK;
        }
    }

    // Bare address -- branch target (assume short form)
    {
        const auto addr = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        op.val = addr;
        op.mode = M_REL8;
        scan = p;
        return OK;
    }
}

void AsmH8500::emitPcRel8(AsmInsn &insn, const Operand &op) const {
    const auto base = insn.address() + insn.length() + 1;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    const auto delta = branchDelta(base, target, insn, op);
    if (overflowDelta(delta, 8))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitByte(static_cast<uint8_t>(delta));
}

void AsmH8500::emitPcRel16(AsmInsn &insn, const Operand &op) const {
    const auto base = insn.address() + insn.length() + 2;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    const auto delta = branchDelta(base, target, insn, op);
    if (overflowDelta(delta, 16))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitUint16Be(static_cast<uint16_t>(delta));
}

// Emit the bytes for a data operand that follows the OP byte, at operandPos
// (mirrors the disassembler's decodeOperand). Operands carried in the OP byte
// (register/CR/bit/quick-immediate) or the EA byte emit nothing here.
// Low 16 bits of an @aa:16 operand; the high byte comes from |page| (DP for
// data, CP for code) at run time -- undefined drops it, a defined mismatch errors.
uint16_t AsmH8500::pageLow16(AsmInsn &insn, const Operand &op, PageReg page) const {
    const auto v = op.val.getUnsigned();
    if (v > 0xFFFF) {
        // Only maximum mode pages the high byte; min mode can't reach >64K.
        const auto assumed = getPageRegister(page);
        const bool ok = maxMode() && (assumed < 0 || static_cast<uint32_t>(assumed) == (v >> 16));
        if (!ok)
            insn.setErrorIf(op, OVERFLOW_RANGE);
    }
    return static_cast<uint16_t>(v);
}

// Low 8 bits of an @aa:8 short-absolute operand; its address is page 0 || BR ||
// aa8, so a defined BR requires 0x00_BR_xx, an undefined one drops the high bits.
uint8_t AsmH8500::pageLow8(AsmInsn &insn, const Operand &op) const {
    const auto v = op.val.getUnsigned();
    if (v > 0xFF) {
        // bits 23-16 must be zero; bits 15-8 must match BR when defined.
        const auto br = getPageRegister(PAGE_BR);
        const bool ok =
                (v >> 16) == 0 && (br < 0 || ((v >> 8) & 0xFF) == static_cast<uint32_t>(br));
        if (!ok)
            insn.setErrorIf(op, OVERFLOW_RANGE);
    }
    return static_cast<uint8_t>(v);
}

Error AsmH8500::emitOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    switch (mode) {
    case M_REL8:
        emitPcRel8(insn, op);
        break;
    case M_REL16:
        emitPcRel16(insn, op);
        break;
    case M_IMM8:
        insn.setErrorIf(op, op.val.overflowUint8() ? OVERFLOW_RANGE : OK);
        insn.emitOperandUint8(static_cast<uint8_t>(op.val.getUnsigned()));
        break;
    case M_ABS8:
        insn.emitOperandUint8(pageLow8(insn, op));
        break;
    case M_IMM16:
        insn.setErrorIf(op, op.val.overflowUint16() ? OVERFLOW_RANGE : OK);
        insn.emitOperandUint16(static_cast<uint16_t>(op.val.getUnsigned()));
        break;
    case M_ABS16:
        // JMP/JSR @aa:16 are the only @aa:16 operands routed here (code, CP).
        insn.emitOperandUint16(pageLow16(insn, op, PAGE_CP));
        break;
    case M_IDX8:
    case M_FPX8:
        insn.setErrorIf(op, op.val.overflowInt8() ? OVERFLOW_RANGE : OK);
        insn.emitOperandUint8(static_cast<uint8_t>(op.val.getUnsigned()));
        break;
    case M_IDX16:
        insn.setErrorIf(op, op.val.overflowInt16() ? OVERFLOW_RANGE : OK);
        insn.emitOperandUint16(static_cast<uint16_t>(op.val.getUnsigned()));
        break;
    case M_ABS24:
        // @aa:24 (PJMP/PJSR) is max-mode only, so checkAddr uses physicalWidth()
        // (24-bit on H8/500, 20-bit on the pin-limited H8/520/H8/530).
        insn.setErrorIf(op, checkAddr(op.val.getUnsigned()));
        insn.emitOperandUint24(static_cast<uint32_t>(op.val.getUnsigned()));
        break;
    case M_NONE:
    case M_REG:
    case M_FP:
    case M_CR:
    case M_BIT:
    case M_IMM2:
    case M_IND:
    case M_REGP:
    case M_INDP:
        break;  // carried in the OP/EA byte, no trailing bytes
    default:
        return insn.setError(OPERAND_NOT_ALLOWED);
    }
    return insn.getError();
}

// True if |mode| is the EA-operand slot of a GEN/EXT entry (the side encoded in
// the EA byte + extensions). The immediate slot (ANDC/ORC/XORC) counts too.
static bool isEaSlot(AddrMode mode) {
    return mode == M_EASRC || mode == M_EADST || mode == M_EAREG || mode == M_EAMEM ||
           mode == M_IMM8 || mode == M_IMM16;
}

// Encode an FMT_GEN/FMT_EXT non-EA operand into the OP byte's low bits.
// Returns OK with |field| set; returns UNKNOWN_REGISTER if a CR operand
// has no resolvable control register, or OPERAND_NOT_ALLOWED for unhandled
// modes. M_NONE passes through with field=0.
static Error encodeOpField(const Operand &op, AddrMode mode, uint8_t &field) {
    switch (mode) {
    case M_NONE:
        field = 0;
        return OK;
    case M_REG:
    case M_FP:
    case M_REGP:
        field = encodeReg(op.reg);
        return OK;
    case M_CR:
        if (op.creg == CR_UNDEF)
            return UNKNOWN_REGISTER;
        field = encodeCr(op.creg);
        return OK;
    case M_BIT:
    case M_IMM8:
    case M_IMM16:
        // M_BIT: bit number 0-15 (BSET/BCLR/BNOT/BTST). An immediate here is the
        // ANDC/ORC/XORC EA source or the MOV:G #xx,<EAd> trailing data; either
        // way it contributes no OP-byte field bits.
        field = (mode == M_BIT) ? (op.val.getUnsigned() & 0x0F) : 0;
        return OK;
    default:
        return OPERAND_NOT_ALLOWED;
    }
}

// Build the EA byte for the given addressing mode, base register, and Sz bit
// (|sz| is 0 for byte, 1 for word).
static uint8_t makeEaByte(AddrMode mode, RegName reg, uint8_t sz) {
    const uint8_t szBit = sz << 3;
    switch (mode) {
    case M_REG:
        return 0xA0 | szBit | encodeReg(reg);
    case M_PDEC:
        return 0xB0 | szBit | encodeReg(reg);
    case M_PINC:
        return 0xC0 | szBit | encodeReg(reg);
    case M_IND:
        return 0xD0 | szBit | encodeReg(reg);
    case M_IDX8:
    case M_FPX8:
        return 0xE0 | szBit | encodeReg(reg);
    case M_IDX16:
        return 0xF0 | szBit | encodeReg(reg);
    case M_IMM8:
        return 0x04;
    case M_IMM16:
        return 0x0C;
    case M_ABS8:
        return 0x05 | szBit;
    case M_ABS16:
        return 0x15 | szBit;
    default:
        return 0;
    }
}

// Number of extension bytes an EA mode appends after the EA byte.
static uint8_t eaExtLen(AddrMode mode) {
    switch (mode) {
    case M_IDX8:
    case M_FPX8:
    case M_ABS8:
    case M_IMM8:
        return 1;
    case M_IDX16:
    case M_ABS16:
    case M_IMM16:
        return 2;
    default:
        return 0;
    }
}

// Emit the EA extension byte(s) at their fixed slot right after the EA byte
// (position 1); the EA byte itself and the OP byte are placed by emitInsn().
// |mode| may differ from op.mode when a word operation widened an 8-bit
// immediate EA (M_IMM8 -> M_IMM16), so the size follows |mode|, not op.mode.
void AsmH8500::emitEaExt(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    const auto v = op.val.getUnsigned();
    constexpr uint8_t pos = 1;
    switch (mode) {
    case M_IDX8:
    case M_FPX8:
        insn.setErrorIf(op, op.val.overflowInt8() ? OVERFLOW_RANGE : OK);
        insn.emitByte(static_cast<uint8_t>(v), pos);
        break;
    case M_IDX16:
        insn.setErrorIf(op, op.val.overflowInt16() ? OVERFLOW_RANGE : OK);
        insn.emitUint16Be(static_cast<uint16_t>(v), pos);
        break;
    case M_IMM8:
        insn.setErrorIf(op, op.val.overflowUint8() ? OVERFLOW_RANGE : OK);
        insn.emitByte(static_cast<uint8_t>(v), pos);
        break;
    case M_ABS8:
        insn.emitByte(pageLow8(insn, op), pos);
        break;
    case M_IMM16:
        insn.setErrorIf(op, op.val.overflowUint16() ? OVERFLOW_RANGE : OK);
        insn.emitUint16Be(static_cast<uint16_t>(v), pos);
        break;
    case M_ABS16:
        // GEN-format data @aa:16: the high byte comes from DP at run time.
        insn.emitUint16Be(pageLow16(insn, op, PAGE_DP), pos);
        break;
    default:
        break;
    }
}

// Encode GEN (Format A) or EXT (Format C) instruction.
Error AsmH8500::encodeGenFmt(AsmInsn &insn) const {
    const auto flags = insn.flags();
    const auto esrc = flags.src();
    const auto edst = flags.dst();

    // Pick the EA-side operand (the one that goes into the EA byte +
    // extensions) and let the other side contribute the OP-byte field. Prefer
    // the dst as the EA: MOV:G #xx,<EAd> has dst=M_EADST and an immediate src
    // that trails the OP byte. ANDC/ORC/XORC have no EA placeholder, so their
    // immediate src is itself the EA (byte 0x04/0x0C).
    const bool dstIsEa = (edst == M_EASRC || edst == M_EADST || edst == M_EAMEM);
    const bool srcIsEa = !dstIsEa && isEaSlot(esrc);
    const Operand &eaOp = srcIsEa ? insn.op1 : insn.op2;
    const Operand &otherOp = srcIsEa ? insn.op2 : insn.op1;
    const auto otherMode = srcIsEa ? edst : esrc;

    // EA-byte Sz bit. SZ_BYTE/SZ_WORD are fixed; SZ_DATA takes the .B/.W suffix
    // if written, else MOV:G-style ops (ISZ_DATA) default to word while the
    // size-by-context ops (bit ops, LDC/STC; ISZ_NONE) default to word for a
    // register EA and byte for memory.
    uint8_t eaSz;
    if (flags.oprSize() == SZ_WORD) {
        eaSz = 1;
    } else if (flags.oprSize() == SZ_DATA) {
        if (insn.insnSz != SZ_NONE)
            eaSz = (insn.insnSz == SZ_BYTE) ? 0 : 1;
        else if (flags.insnSize() == ISZ_DATA)
            eaSz = 1;
        else
            eaSz = (eaOp.mode == M_REG) ? 1 : 0;
    } else {
        eaSz = 0;  // SZ_BYTE / SZ_NONE
    }

    // LDC/STC/ANDC/ORC/XORC <op>,CR: the operand size must match the control
    // register width (SR is word; CCR/BR/EP/DP/TP are byte). The immediate forms
    // carry their size in the EA mode (M_IMM8/M_IMM16); a mismatch is a size
    // error. A register/memory EA simply follows the CR width.
    const auto cr = (edst == M_CR) ? insn.op2.creg : (esrc == M_CR) ? insn.op1.creg : CR_UNDEF;
    if (cr != CR_UNDEF) {
        const uint8_t crSz = (cr == CR_SR) ? 1 : 0;
        const uint8_t immSz = (eaOp.mode == M_IMM16) ? 1 : (eaOp.mode == M_IMM8) ? 0 : crSz;
        if (immSz != crSz)
            return insn.setError(eaOp, ILLEGAL_SIZE);
        eaSz = crSz;
    }

    // BSET/BCLR/BNOT/BTST #xx: a byte-size target has bits 0-7, a word one 0-15.
    if (otherMode == M_BIT && otherOp.val.getUnsigned() > (eaSz == 0 ? 7u : 15u))
        return insn.setError(otherOp, ILLEGAL_BIT_NUMBER);

    uint8_t regField = 0;
    auto opcode = insn.opCode();
    if (otherMode == M_IMM2) {
        // ADD:Q #xx,<EAd>: the quick value (+/-1, +/-2) selects the OP byte.
        switch (otherOp.val.getSigned()) {
        case 1:
            opcode = 0x08;
            break;
        case 2:
            opcode = 0x09;
            break;
        case -1:
            opcode = 0x0C;
            break;
        case -2:
            opcode = 0x0D;
            break;
        default:
            return insn.setError(otherOp, OVERFLOW_RANGE);
        }
    } else {
        const auto err = encodeOpField(otherOp, otherMode, regField);
        if (err != OK)
            return insn.setError(err);
    }

    // MULXU.W/DIVXU.W place the 32-bit result in the register pair Rd:Rd+1, so
    // the destination register must be even. The byte forms use a single
    // register (any number). Odd is an encodable but misaligned register: flag
    // it (REGISTER_NOT_ALIGNED) without aborting the encode.
    if (otherMode == M_REGP && eaSz == 1 && (regField & 1))
        insn.setErrorIf(otherOp, REGISTER_NOT_ALIGNED);

    // An immediate used as the EA takes the 8-bit form (0x04) for a byte
    // operation and the 16-bit form (0x0C) for a word one. So a word op -- the
    // unsized default, an explicit .W, or a word CR target -- widens a small
    // immediate EA (ANDC/ORC/XORC and the ALU #imm,Rd forms).
    auto eaMode = eaOp.mode;
    if (eaMode == M_IMM8 && eaSz == 1)
        eaMode = M_IMM16;
    // Resolve the absolute EA form: a 24-bit data address (M_ABS24) has no
    // @aa:24 data form, so it shortens to @aa:16; a no-suffix @aa:16 prefers the
    // shorter @aa:8 when the value lies in BR's page. An explicit :size is kept.
    if (eaMode == M_ABS24) {
        eaMode = M_ABS16;
    } else if (eaMode == M_ABS16 && eaOp.absSize == 0) {
        const auto br = getPageRegister(PAGE_BR);
        const auto v = eaOp.val.getUnsigned();
        if (br >= 0 && (v >> 16) == 0 && ((v >> 8) & 0xFF) == static_cast<uint32_t>(br))
            eaMode = M_ABS8;
    }

    // [EA byte][ext 0-2][(00)][OP byte]. The EA byte is the prefix (the base
    // Insn prefix slot is reused to carry it); emitInsn places it and the OP
    // byte around the extension (emitEaExt, at pos 1).
    insn.setErrorIf(eaOp);
    insn.setPrefix(makeEaByte(eaMode, eaOp.reg, eaSz));
    insn.eaExt = eaExtLen(eaMode);
    insn.setOpCode(opcode | regField);
    emitEaExt(insn, eaOp, eaMode);
    insn.emitInsn();

    // MOV:G #xx,<EAd>: the immediate source trails the OP byte; the other non-EA
    // operands (register/CR/bit) are already embedded in it.
    return emitOperand(insn, otherOp, otherMode);
}

// Encode SPC (Format B) instruction: [OP byte][operand bytes].
Error AsmH8500::encodeSpcFmt(AsmInsn &insn) const {
    const auto esrc = insn.src();
    const auto edst = insn.dst();

    // TRAPA #vec (0-15): [08][10|vec]
    if (esrc == M_TRAPV) {
        const auto vec = insn.op1.val.getUnsigned();
        if (vec > 15)
            insn.setErrorIf(insn.op1, OVERFLOW_RANGE);
        insn.emitInsn();
        return insn.emitOperandUint8(0x10 | (vec & 0x0F));
    }

    // SCB/EQ|NE|F Rc, rel8: [base][B8|Rc][rel8]
    if (esrc == M_SCB) {
        insn.emitInsn();
        insn.emitOperandUint8(0xB8 | encodeReg(insn.op1.reg));
        emitPcRel8(insn, insn.op2);
        return insn.getError();
    }

    // LDM @SP+,(list) / STM (list),@-SP: the list is op1 for STM (src=M_REGLIST),
    // op2 for LDM; the stack operand emits no bytes. Enforce direction -- LDM
    // pops via @SP+ (post-inc), STM pushes via @-SP (pre-dec).
    if (esrc == M_REGLIST || edst == M_REGLIST) {
        const bool listIsSrc = (esrc == M_REGLIST);
        const auto &list = listIsSrc ? insn.op1 : insn.op2;
        const auto &sp = listIsSrc ? insn.op2 : insn.op1;
        if (sp.mode != (listIsSrc ? M_PDEC : M_PINC))
            return insn.setError(sp, REGISTER_NOT_ALLOWED);
        insn.emitInsn();
        return insn.emitOperandUint8(static_cast<uint8_t>(list.val.getUnsigned()));
    }

    // PJMP/PJSR @aa:24 live on a MAXMODE-only page filtered out by searchName in
    // minimum mode, so addressWidth() here is the chip's physical width (24-bit
    // on the H8/500, 20-bit on the pin-limited H8/520/H8/530).
    if (esrc == M_ABS24) {
        insn.setErrorIf(insn.op1, checkAddr(insn.op1.val.getUnsigned()));
        insn.emitInsn();
        return insn.emitOperandUint24(static_cast<uint32_t>(insn.op1.val.getUnsigned()));
    }

    // No-operand instructions (NOP, RTE, RTS, SLEEP, TRAP/VS).
    if (esrc == M_NONE && edst == M_NONE) {
        insn.emitInsn();
        return insn.getError();
    }

    // General format: embed the register field into the OP byte. LINK/UNLK leave
    // FP implicit (no embedding).
    if (edst == M_REG)
        insn.embed(encodeReg(insn.op2.reg));
    else if (esrc == M_REG && (edst == M_ABS8 || edst == M_FPX8))
        insn.embed(encodeReg(insn.op1.reg));  // MOV:S Rs,@aa:8 / MOV:F Rs,@(d,FP)
    // MOV:L/MOV:S/MOV:F carry the size bit (bit 3); unsuffixed defaults to word.
    if (insn.oprSize() == SZ_DATA && insn.insnSize() == ISZ_DATA && insn.insnSz != SZ_BYTE)
        insn.embed(0x08);
    insn.emitInsn();

    // The data operand follows the OP byte.
    const Operand *dataOp;
    AddrMode dataMode;
    if (esrc == M_REG || esrc == M_FP) {
        dataOp = &insn.op2;
        dataMode = edst;
    } else {
        dataOp = &insn.op1;
        dataMode = esrc;
    }

    return emitOperand(insn, *dataOp, dataMode);
}

// Encode SEC (0x11 prefix) instruction. Page-jump entries (PJMP/PJSR/PRTS/PRTD)
// live in a separate MAXMODE-only page that searchName already filters out
// when MIN-mode is active, so no per-instruction check is needed here.
Error AsmH8500::encodeSecFmt(AsmInsn &insn) const {
    const auto esrc = insn.src();

    // [0x11][OP byte][operand]. JMP/JSR/PJMP/PJSR @Rn|@(d,Rn) embed the register
    // into the OP byte; PRTS/PRTD/@aa take none.
    if (esrc == M_IND || esrc == M_INDP || esrc == M_IDX8 || esrc == M_IDX16)
        insn.embed(encodeReg(insn.op1.reg));
    // PJMP/PJSR @Rn copy Rn->CP and Rn+1->PC, so Rn must be the even base of a
    // register pair. Odd is encodable but misaligned: flag, don't abort.
    if (esrc == M_INDP && (encodeReg(insn.op1.reg) & 1))
        insn.setErrorIf(insn.op1, REGISTER_NOT_ALIGNED);
    insn.emitInsn();
    return emitOperand(insn, insn.op1, esrc);
}

static bool notDot(char c) {
    return c != '.';
}

static bool notColon(char c) {
    return c != ':';
}

// Split a trailing .B/.W operand-size suffix off the mnemonic (e.g. MOV:G.B),
// recording it on |insn| and truncating the name so searchName sees the base.
static void parseInsnSize(AsmInsn &insn) {
    StrScanner p{insn.name()};
    p.trimStart(notDot);
    if (*p != '.')
        return;
    auto eos = const_cast<char *>(p.str());
    const auto c = *++p;
    // The suffix must end the mnemonic (word boundary): ".BX" is not a size.
    if (p[1] != 0)
        return;
    if (c == 'B' || c == 'b') {
        insn.insnSz = SZ_BYTE;
        *eos = 0;
    } else if (c == 'W' || c == 'w') {
        insn.insnSz = SZ_WORD;
        *eos = 0;
    }
}

static InsnClass classOfLetter(char c) {
    switch (c) {
    case 'G':
    case 'g':
        return IC_G;
    case 'E':
    case 'e':
        return IC_E;
    case 'F':
    case 'f':
        return IC_F;
    case 'I':
    case 'i':
        return IC_I;
    case 'L':
    case 'l':
        return IC_L;
    case 'S':
    case 's':
        return IC_S;
    case 'Q':
    case 'q':
        return IC_Q;
    default:
        return IC_N;
    }
}

// Split a trailing ":x" class suffix off the mnemonic (run after parseInsnSize,
// so MOV:G.B is already trimmed to MOV:G). A bare stem leaves insnCls IC_N
// and acceptOperands selects the optimal class.
static void parseInsnClass(AsmInsn &insn) {
    StrScanner p{insn.name()};
    p.trimStart(notColon);
    if (*p != ':')
        return;
    auto eos = const_cast<char *>(p.str());
    const auto cls = classOfLetter(*++p);
    // The suffix must end the mnemonic (word boundary): ":GX" is not a class.
    if (cls != IC_N && p[1] == 0) {
        insn.insnCls = cls;
        *eos = 0;
    }
}

Error AsmH8500::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);

    // Parse up to two operands
    if (!endOfLine(scan.skipSpaces())) {
        if (parseOperand(scan, insn.op1) && insn.op1.hasError())
            return _insn.setError(insn.op1);
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.op2) && insn.op2.hasError())
                return _insn.setError(insn.op2);
        }
    }
    // A third operand or any other trailing token (a comment ends the line) is
    // an error; without this, e.g. "MOV:G R0,R1,R2" would silently ignore R2.
    if (!endOfLine(scan.skipSpaces()))
        return _insn.setError(scan, GARBAGE_AT_END);

    // Smart branch: upgrade M_REL8 to M_REL16 when the byte delta would
    // overflow. This lets users write BRA target without specifying d:8/d:16.
    if (_smartBranch && insn.op1.mode == M_REL8 && !insn.op1.hasError()) {
        const auto target = insn.op1.val.getUnsigned();
        const auto base = _insn.address() + 2;
        const auto delta = static_cast<int32_t>(target) - static_cast<int32_t>(base);
        if (delta < -128 || delta > 127)
            insn.op1.mode = M_REL16;
    }

    parseInsnSize(insn);
    parseInsnClass(insn);
    // A bare ANDC/ORC/XORC/LDC #imm,<CR> (no suffix) takes its size from the
    // control register: SR is word, CCR/BR/EP/DP/TP are byte. Default the size
    // here so the immediate widens (below) and the encoder's CR-width check
    // (encodeGenFmt) is satisfied without an explicit .B/.W.
    if (insn.insnSz == SZ_NONE) {
        const auto cr = (insn.op1.mode == M_CR)   ? insn.op1.creg
                        : (insn.op2.mode == M_CR) ? insn.op2.creg
                                                  : CR_UNDEF;
        if (cr != CR_UNDEF)
            insn.insnSz = (cr == CR_SR) ? SZ_WORD : SZ_BYTE;
    }
    // A .W suffix sizes an immediate-EA operand to 16 bits so the table search
    // and EA byte (0x04 -> 0x0C) select the word variant.
    if (insn.insnSz == SZ_WORD) {
        if (insn.op1.mode == M_IMM8)
            insn.op1.mode = M_IMM16;
        if (insn.op2.mode == M_IMM8)
            insn.op2.mode = M_IMM16;
    }

    insn.maxMode = maxMode();
    if (searchName(cpuType(), insn))
        return _insn.setError(insn.op1, insn);

    // A fixed-size (ISZ_NONE) mnemonic needs no suffix and the disassembler emits
    // none, but a suffix is still accepted when it matches the effective operand
    // size and rejected otherwise: byte/word ops take the matching letter, LDC/STC
    // follow the control register (SR=word, else byte), and sizeless ops (NOP/
    // branches) take none. An ISZ_DATA mnemonic with no suffix defaults to word.
    if (insn.insnSz != SZ_NONE && insn.insnSize() != ISZ_DATA) {
        auto expect = insn.oprSize();
        if (expect == SZ_DATA) {
            // LDC/STC fix the size by the control register; the bit ops accept
            // either .B or .W (the size selects the target width).
            if (insn.src() == M_CR || insn.dst() == M_CR) {
                const auto cr = (insn.src() == M_CR) ? insn.op1.creg : insn.op2.creg;
                expect = (cr == CR_SR) ? SZ_WORD : SZ_BYTE;
            } else {
                expect = insn.insnSz;
            }
        }
        if (insn.insnSz != expect) {
            StrScanner p{_insn.errorAt()};
            p.trimStart(notDot);
            return _insn.setErrorIf(p, ILLEGAL_SIZE);
        }
    }

    const auto pm = insn.prefixMode();
    if (pm == PM_GEN || pm == PM_EXT) {
        encodeGenFmt(insn);
    } else if (pm == PM_SEC) {
        encodeSecFmt(insn);
    } else {
        encodeSpcFmt(insn);
    }

    // Surface an undefined-symbol (or other parse) error carried by either
    // operand: an undefined value parses cleanly (mode set, parseOperand OK), so
    // it must be reported here. The SPC/SEC/trailing-data emit paths -- unlike
    // the EA path -- have no other place that propagates it.
    insn.setErrorIf(insn.op1);
    insn.setErrorIf(insn.op2);
    return _insn.setError(insn);
}

}  // namespace h8500
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
