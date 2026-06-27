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

#include "asm_superh.h"

#include "option_base.h"
#include "reg_superh.h"
#include "table_superh.h"

namespace libasm {
namespace superh {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::option;

namespace {
// clang-format off
constexpr char TEXT_dDATA[]    PROGMEM = ".data";
// Float data constants are a floating-point feature (guarded by LIBASM_ASM_NOFLOAT
// in defineDataConstant), independent of the FPU *instruction* set: IEEE bytes can
// be emitted without the FPU, so these are not gated by LIBASM_SUPERH_NOFPU.
constexpr char TEXT_dFDATA_S[] PROGMEM = ".fdata.s";
constexpr char TEXT_dFDATA_D[] PROGMEM = ".fdata.d";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_dDATA,    &Assembler::defineDataConstant, Assembler::DATA_WORD    },
    { TEXT_dFDATA_D, &Assembler::defineDataConstant, Assembler::DATA_FLOAT64 },
    { TEXT_dFDATA_S, &Assembler::defineDataConstant, Assembler::DATA_FLOAT32 },
};
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};
// clang-format on

// SH mnemonics embed '.' (size suffix: MOV.B, AND.B, ...) and '/' (condition
// suffix: CMP/EQ, BF/S, BT/S) directly in the name token. Extend the default
// SymbolParser so the tokenizer doesn't split the mnemonic at those chars. The
// location counter is '$' (the inherited default); per the SuperH Assembler
// manual '*' is the multiplication operator, not the location counter.
struct SuperHSymbolParser final : SymbolParser {
    bool instructionLetter(char c) const override {
        return SymbolParser::instructionLetter(c) || c == '/' || c == '.';
    }
};

}  // namespace

const ValueParser::Plugins &AsmSuperH::defaultPlugins() {
    static const struct final : ValueParser::HitachiPlugins {
        const SymbolParser &symbol() const override {
            static const SuperHSymbolParser SYMBOL;
            return SYMBOL;
        }
    } PLUGINS{};
    return PLUGINS;
}

AsmSuperH::AsmSuperH(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_fpu),
      Config(TABLE),
      _opt_fpu(this, &Config::setFpuName, OPT_TEXT_FPU, OPT_DESC_FPU) {
    reset();
}

void AsmSuperH::reset() {
    Assembler::reset();
    setFpuType(FPU_NONE);  // assembler requires the FPU to be enabled explicitly
}

Error AsmSuperH::processPseudo(StrScanner &scan, Insn &insn) {
    // "[.]fpu on/off" toggles the optional SH-2A FPU. Accept an optional leading
    // dot like the base directive lookup does.
    auto name = insn.name();
    if (*name == '.')
        ++name;
    if (strcasecmp_P(name, TEXT_FPU) == 0) {
        const auto at = scan;
        const auto error = _opt_fpu.set(scan);
        return error ? insn.setErrorIf(at, error) : OK;
    }
    return Assembler::processPseudo(scan, insn);
}

// Parse the disp/size part of "@(disp,Rn)" / "@(R0,Rn)" / "@(disp,GBR)" /
// "@(disp,PC)" / "@(R0,GBR)". The leading '@(' has been consumed.
//
// For "@(disp,PC)" the value is a raw byte displacement (manual table 3-1); the
// M_PCREL encoder scales it by the access size. The target-as-symbol form
// (MOV.W label,Rn) is a bare operand handled in parseOperand, not here.
Error AsmSuperH::parseAtParen(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    auto s = p;
    // Case 1: starts with R0 -> @(R0,Rn) or @(R0,GBR)
    const auto first = parseRegName(p, parser());
    if (first == REG_R0) {
        if (!p.skipSpaces().expect(','))
            return op.setError(p, MISSING_COMMA);
        auto s2 = p.skipSpaces();
        const auto second = parseRegName(p, parser());
        if (second == REG_GBR) {
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            op.mode = M_IGBR;
            scan = p;
            return OK;
        }
        if (isGpr(second)) {
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            op.reg = second;
            op.mode = M_IDXN;
            scan = p;
            return OK;
        }
        return op.setError(s2, UNKNOWN_OPERAND);
    }
    if (first != REG_UNDEF)
        return op.setError(s, UNKNOWN_OPERAND);

    // Case 2: starts with a value -> @(disp,Rn) or @(disp,GBR) or @(disp,PC)
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    if (!p.skipSpaces().expect(','))
        return op.setError(p, MISSING_COMMA);
    auto s2 = p.skipSpaces();
    const auto base = parseRegName(p, parser());
    if (!p.skipSpaces().expect(')'))
        return op.setError(p, MISSING_CLOSING_PAREN);
    if (base == REG_GBR) {
        op.mode = M_D8;  // GBR-relative; encoder scales by the access size
        scan = p;
        return OK;
    }
    if (base == REG_PC) {
        op.mode = M_PCREL;  // @(disp,PC): raw displacement, scaled by access size
        scan = p;
        return OK;
    }
    if (isGpr(base)) {
        op.reg = base;
        op.mode = M_D4N;  // generic Rn-displaced; encoder picks B/W/L scale
        scan = p;
        return OK;
    }
    return op.setError(s2, UNKNOWN_OPERAND);
}

Error AsmSuperH::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p) || *p == ',') {
        op.mode = M_NONE;
        return OK;
    }

    auto s = p;
    if (p.expect('#')) {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_IMM8;
        scan = p;
        return OK;
    }

    if (p.expect('@')) {
        if (p.expect('-')) {
            // @-Rn
            const auto reg = parseRegName(p, parser());
            if (!isGpr(reg))
                return op.setError(s, REGISTER_NOT_ALLOWED);
            op.reg = reg;
            op.mode = M_DECN;
            scan = p;
            return OK;
        }
        if (p.expect('(')) {
            const auto err = parseAtParen(p, op);
            if (err)
                return err;
            scan = p;
            return OK;
        }
        // @Rn or @Rn+
        const auto reg = parseRegName(p, parser());
        if (!isGpr(reg))
            return op.setError(s, REGISTER_NOT_ALLOWED);
        op.reg = reg;
        if (p.expect('+')) {
            op.mode = M_INCN;
        } else {
            op.mode = M_IRN;
        }
        scan = p;
        return OK;
    }

    // Try register / special register
    const auto reg = parseRegName(p, parser());
    if (reg != REG_UNDEF) {
        op.reg = reg;
        if (isGpr(reg)) {
            op.mode = M_RN;
        } else if (isFr(reg)) {
            op.mode = M_FRN;
        } else if (isDr(reg)) {
            op.mode = M_DRN;
        } else {
            switch (reg) {
            case REG_SR:
                op.mode = M_SR;
                break;
            case REG_GBR:
                op.mode = M_GBR;
                break;
            case REG_VBR:
                op.mode = M_VBR;
                break;
            case REG_MACH:
                op.mode = M_MACH;
                break;
            case REG_MACL:
                op.mode = M_MACL;
                break;
            case REG_PR:
                op.mode = M_PR;
                break;
            case REG_DSR:
                op.mode = M_DSR;
                break;
            case REG_A0:
                op.mode = M_A0;
                break;
            case REG_X0:
                op.mode = M_X0;
                break;
            case REG_X1:
                op.mode = M_X1;
                break;
            case REG_Y0:
                op.mode = M_Y0;
                break;
            case REG_Y1:
                op.mode = M_Y1;
                break;
            case REG_MOD:
                op.mode = M_MOD;
                break;
            case REG_RS:
                op.mode = M_RS;
                break;
            case REG_RE:
                op.mode = M_RE;
                break;
            case REG_FPUL:
                op.mode = M_FPUL;
                break;
            case REG_FPSCR:
                op.mode = M_FPSCR;
                break;
            default:
                return op.setError(s, REGISTER_NOT_ALLOWED);
            }
        }
        scan = p;
        return OK;
    }

    // Plain expression -> branch target (will be matched against M_REL8/M_REL12)
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_REL8;
    scan = p;
    return OK;
}

namespace {

bool notDot(char c) {
    return c != '.';
}

// Displacement scale for the size-bearing displacement modes (M_D4M/M_D8/
// M_PCREL): .B=1, .W=2, .L=4. ISZ_NONE maps to 4 so MOVA's suffix-less
// PC-relative operand scales by 4 like a long access.
unsigned dispScale(InsnSize sz) {
    switch (sz) {
    case ISZ_BYTE:
        return 1;
    case ISZ_WORD:
        return 2;
    default:
        return 4;
    }
}

}  // namespace

InsnSize AsmInsn::parseInsnSize() {
    StrScanner p{name()};
    p.trimStart(notDot);
    auto eos = const_cast<char *>(p.str());
    auto sz = ISZ_NONE;
    if (p.expect('.')) {
        if (p.iexpect('B')) {
            sz = ISZ_BYTE;
        } else if (p.iexpect('W')) {
            sz = ISZ_WORD;
        } else if (p.iexpect('L')) {
            sz = ISZ_LONG;
        }
        // Only a genuine .B/.W/.L with nothing trailing is a size suffix; a
        // non-size dot (FMOV.S) or trailing garbage stays in the name and is
        // resolved by the table lookup.
        if (sz != ISZ_NONE && *p == 0) {
            *eos = 0;
        } else {
            sz = ISZ_NONE;
        }
    }
    _parsedSize = sz;
    return sz;
}

void AsmSuperH::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    // Carry the operand's own error (notably UNDEFINED_SYMBOL) onto the
    // instruction first, so a later range/distance check -- which only fires
    // via setErrorIf when the instruction is still OK -- can't mask it. An
    // undefined symbol has an undefined value (read as 0); the PC-relative
    // modes below substitute the base address for it to avoid a spurious
    // OPERAND_TOO_FAR and to emit a clean zero displacement.
    insn.setErrorIf(op);
    auto opc = insn.opCode();
    const auto regnum = encodeRegNum(op.reg);
    switch (mode) {
    case M_NONE:
    case M_R0:  // implicit R0; nothing to embed
    case M_SR:
    case M_GBR:
    case M_VBR:
    case M_MACH:
    case M_MACL:
    case M_PR:
    case M_IGBR:
    case M_DSR:
    case M_A0:
    case M_X0:
    case M_X1:
    case M_Y0:
    case M_Y1:
    case M_MOD:
    case M_RS:
    case M_RE:
    case M_FPUL:
    case M_FPSCR:
    case M_BANK:
    case M_PUSH:
    case M_PULL:
        break;
    case M_FRN:
        opc |= static_cast<Config::opcode_t>(encodeRegNum(op.reg) & 0xF) << 8;
        insn.setOpCode(opc);
        break;
    case M_FRM:
        opc |= static_cast<Config::opcode_t>(encodeRegNum(op.reg) & 0xF) << 4;
        insn.setOpCode(opc);
        break;
    case M_DRN:
        // DR registers are even-numbered FR pairs; encoded as 3-bit field
        // in bits[11:9] (value = DR_index/2, but encodeRegNum already
        // returns 0..7 for DR0..DR14).
        opc |= static_cast<Config::opcode_t>(encodeRegNum(op.reg) & 0x7) << 9;
        insn.setOpCode(opc);
        break;
    case M_DRM:
        opc |= static_cast<Config::opcode_t>(encodeRegNum(op.reg) & 0x7) << 5;
        insn.setOpCode(opc);
        break;
    case M_IMM3: {
        const auto v = op.val.getUnsigned();
        if (v > 7)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        opc |= static_cast<Config::opcode_t>(v & 0x7) << 1;  // bits[3:1]
        insn.setOpCode(opc);
        break;
    }
    case M_IMM20: {
        // 20-bit signed: hi nibble in bits[7:4] of word1, lo 16 in word2.
        const auto v = static_cast<int32_t>(op.val.getSigned());
        if (v < -0x80000 || v > 0x7FFFF)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        opc |= static_cast<Config::opcode_t>((v >> 16) & 0xF) << 4;
        insn.setOpCode(opc);
        insn.setOpCode2(static_cast<Config::opcode_t>(v & 0xFFFF));
        break;
    }
    case M_IMM20S: {
        // 28-bit signed shifted-left-8: top 12 bits stored as (hi:lo).
        auto v = static_cast<int32_t>(op.val.getSigned());
        if (v & 0xFF)
            insn.setErrorIf(op, OVERFLOW_RANGE);  // low 8 bits must be 0
        if (v < -0x8000000 || v > 0x7FFFFFF)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        v >>= 8;
        opc |= static_cast<Config::opcode_t>((v >> 16) & 0xF) << 4;
        insn.setOpCode(opc);
        insn.setOpCode2(static_cast<Config::opcode_t>(v & 0xFFFF));
        break;
    }
    case M_D12N:
    case M_D12M:
        // The disp12 forms are deferred -- the assembler still emits an
        // error if the table reaches them. Reserved for future commits.
        insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        break;
    case M_RN:
    case M_IRN:
    case M_INCN:
    case M_DECN:
    case M_IDXN:
        opc |= static_cast<Config::opcode_t>(regnum & 0xF) << 8;
        break;
    case M_RM:
    case M_IRM:
    case M_INCM:
    case M_IDXM:
        opc |= static_cast<Config::opcode_t>(regnum & 0xF) << 4;
        break;
    case M_D4N: {
        // @(disp,Rn) with Rn in bits[11:8]; disp in bits[3:0], scaled by 4
        // (MOV.L only mode that uses M_D4N).
        const auto disp = op.val.getUnsigned();
        if (disp & 3) {
            insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
        } else if (disp > 60) {
            insn.setErrorIf(op, OVERFLOW_RANGE);
        }
        opc |= static_cast<Config::opcode_t>(regnum & 0xF) << 8;
        opc |= (disp >> 2) & 0xF;
        break;
    }
    case M_D4M: {
        // @(disp,Rn/Rm) with register in bits[7:4]; disp in bits[3:0], scale
        // depends on the access size (.B=1, .W=2, .L=4).
        const auto scale = dispScale(insn.effectiveSize());
        const auto disp = op.val.getUnsigned();
        if ((disp % scale) != 0) {
            insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
        } else if (disp / scale > 15) {
            insn.setErrorIf(op, OVERFLOW_RANGE);
        }
        opc |= static_cast<Config::opcode_t>(regnum & 0xF) << 4;
        opc |= (disp / scale) & 0xF;
        break;
    }
    case M_D8: {
        // @(disp,GBR); disp in bits[7:0]; scale by access size (B=1, W=2, L=4).
        const auto scale = dispScale(insn.effectiveSize());
        const auto disp = op.val.getUnsigned();
        if ((disp % scale) != 0) {
            insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
        } else if (disp / scale > 255) {
            insn.setErrorIf(op, OVERFLOW_RANGE);
        }
        opc |= (disp / scale) & 0xFF;
        break;
    }
    case M_PCREL: {
        // .W scales the displacement by 2 (base PC+4); .L and MOVA scale by 4
        // (base (PC+4)&~3). d is the 8-bit field in [0, 255].
        const Config::ptrdiff_t scale = dispScale(insn.effectiveSize());
        Config::ptrdiff_t delta;
        if (op.mode == M_PCREL) {
            // @(disp,PC): disp is a raw byte displacement (manual table 3-1).
            delta = op.getError() ? 0 : static_cast<Config::ptrdiff_t>(op.val.getUnsigned());
        } else {
            // Bare symbol/address: the operand is the target ("symbol" mode);
            // branchDelta validates the target address (incl. word alignment).
            const Config::uintptr_t base = (scale == 2) ? (insn.address() + 4)
                                                        : ((insn.address() + 4) & ~Config::uintptr_t(3));
            const Config::uintptr_t target = op.getError() ? base : op.val.getUnsigned();
            insn.setErrorIf(op, checkAddr(target, true));
            delta = branchDelta(base, target, insn, op);
        }
        // The field is an unsigned 8-bit displacement, so overflowDelta (signed)
        // does not apply; the displacement must also be a multiple of the scale.
        if (delta % scale) {
            insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
        } else if (delta < 0 || delta > 255 * scale) {
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        } else {
            opc |= (delta / scale) & 0xFF;
        }
        break;
    }
    case M_IMM8: {
        // Signed 8-bit immediate.
        const auto val = op.val.getSigned();
        if (val < -128 || val > 255)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        opc |= val & 0xFF;
        break;
    }
    case M_TVEC: {
        // TRAPA #imm, unsigned 8-bit.
        const auto val = op.val.getUnsigned();
        if (val > 255)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        opc |= val & 0xFF;
        break;
    }
    case M_REL8:
    case M_REL8P: {
        // 8-bit signed PC-relative, scale 2. BF/BT (M_REL8) take a bare branch
        // target; LDRS/LDRE (M_REL8P) take a bare symbol/address target or, via
        // @(disp,PC), a raw byte displacement.
        Config::ptrdiff_t delta;
        if (op.mode == M_PCREL) {
            delta = op.getError() ? 0 : static_cast<Config::ptrdiff_t>(op.val.getUnsigned());
        } else {
            const Config::uintptr_t base = insn.address() + 4;
            const Config::uintptr_t target = op.getError() ? base : op.val.getUnsigned();
            insn.setErrorIf(op, checkAddr(target, true));
            delta = branchDelta(base, target, insn, op);
        }
        if ((delta & 1) || overflowDelta(delta / 2, 8))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        opc |= (delta >> 1) & 0xFF;
        break;
    }
    case M_REL12: {
        // 12-bit signed PC-relative branch: d = (target - (PC+4)) / 2.
        const Config::uintptr_t base = insn.address() + 4;
        const Config::uintptr_t target = op.getError() ? base : op.val.getUnsigned();
        insn.setErrorIf(op, checkAddr(target, true));
        const Config::ptrdiff_t delta = branchDelta(base, target, insn, op);
        if ((delta & 1) || overflowDelta(delta / 2, 12))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        opc |= (delta >> 1) & 0xFFF;
        break;
    }
    }
    insn.setOpCode(opc);
}

Error AsmSuperH::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);

    if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
        return _insn.setError(insn.srcOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
            return _insn.setError(insn.dstOp);
        // FMAC FR0,FRm,FRn -- consume the optional implicit FR0 prefix and
        // shift the remaining two operands into the (srcOp, dstOp) slots.
        if (scan.skipSpaces().expect(',')) {
            if (insn.srcOp.mode != M_FRN || insn.srcOp.reg != REG_FR0)
                return _insn.setError(insn.srcOp, REGISTER_NOT_ALLOWED);
            insn.srcOp = insn.dstOp;
            if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
                return _insn.setError(insn.dstOp);
        }
    }
    scan.skipSpaces();

    // Try the full mnemonic first so names whose '.' suffix is part of the name
    // (FMOV.S, MAC.W, MAC.L) match directly; otherwise strip a .B/.W/.L size
    // suffix and search again for the base mnemonic.
    if (searchName(cpuSpec(), insn)) {
        if (insn.parseInsnSize() == ISZ_NONE || searchName(cpuSpec(), insn))
            return _insn.setError(insn.srcOp, insn);
    }

    // For ISZ_DATA, an explicit suffix replaces the default size bits in the
    // matched opcode template; a bare mnemonic keeps the stored default.
    if (insn.insnSize() == ISZ_DATA && insn.parsedSize() != ISZ_NONE) {
        const auto sf = sizeFieldOf(insn.src(), insn.dst());
        for (uint8_t i = 0; i < 4; i++) {
            if (sf.sizes[i] == insn.parsedSize()) {
                insn.setOpCode((insn.opCode() & ~sf.mask) |
                        (static_cast<Config::opcode_t>(i) << sf.shift));
                break;
            }
        }
    }

    encodeOperand(insn, insn.srcOp, insn.src());
    encodeOperand(insn, insn.dstOp, insn.dst());
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace superh
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
