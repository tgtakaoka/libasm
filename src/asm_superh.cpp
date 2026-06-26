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
constexpr char TEXT_dDATA[] PROGMEM = ".data";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_dDATA, &Assembler::defineDataConstant, Assembler::DATA_WORD },
};
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};
// clang-format on

// SH mnemonics embed '.' (size suffix: MOV.B, AND.B, ...) and '/' (condition
// suffix: CMP/EQ, BF/S, BT/S) directly in the name token. Extend the default
// SymbolParser so the tokenizer doesn't split the mnemonic at those chars.
// Also accept '*' (Hitachi convention) as the current-PC location symbol so
// idioms like "BF *+4" and "MOV.W @(*+H'40,PC),R0" assemble.
struct SuperHSymbolParser final : SymbolParser {
    bool instructionLetter(char c) const override {
        return SymbolParser::instructionLetter(c) || c == '/' || c == '.';
    }
    bool locationSymbol(StrScanner &scan) const override {
        return SymbolParser::locationSymbol(scan, '*');
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

// Parse the disp/size part of "@(disp,Rn)" / "@(R0,Rn)" / "@(disp,GBR)" /
// "@(target,PC)" / "@(R0,GBR)". The leading '@(' has been consumed.
//
// For "@(target,PC)" the operand is the absolute target address. The user's
// idiom "@(*+N,PC)" works naturally because '*' is registered as the
// location symbol -- parseExpr evaluates *+N to current_PC + N, which is
// exactly the target the M_PCL encoder consumes.
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

    // Case 2: starts with a value -> @(disp,Rn) or @(disp,GBR) or @(target,PC)
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
        op.mode = M_D8B;  // generic GBR-relative; encoder picks B/W/L scale
        scan = p;
        return OK;
    }
    if (base == REG_PC) {
        op.mode = M_PCL;  // absolute target; encoder picks W/L scale
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

// Extract the mnemonic size suffix: 'B', 'W', 'L' from the trailing
// ".B"/".W"/".L" of the name. Returns 0 if no suffix.
char nameSizeSuffix(const char *name) {
    const auto len = strlen(name);
    if (len < 3)
        return 0;
    if (name[len - 2] != '.')
        return 0;
    const auto last = name[len - 1];
    if (last == 'B' || last == 'W' || last == 'L')
        return last;
    return 0;
}

unsigned sizeScale(char suffix) {
    switch (suffix) {
    case 'B':
        return 1;
    case 'W':
        return 2;
    case 'L':
        return 4;
    default:
        return 1;
    }
}

}  // namespace

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
    case M_DECR15:
    case M_INCR15:
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
        // depends on mnemonic suffix (.B=1, .W=2, .L=4).
        const auto scale = sizeScale(nameSizeSuffix(insn.name()));
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
    case M_D8B:
    case M_D8W:
    case M_D8L: {
        // @(disp,GBR); disp in bits[7:0]; scale by mode (B=1, W=2, L=4).
        const auto scale = (mode == M_D8B) ? 1u : (mode == M_D8W) ? 2u : 4u;
        const auto disp = op.val.getUnsigned();
        if ((disp % scale) != 0) {
            insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
        } else if (disp / scale > 255) {
            insn.setErrorIf(op, OVERFLOW_RANGE);
        }
        opc |= (disp / scale) & 0xFF;
        break;
    }
    case M_PCW: {
        // Operand is the absolute target address. The CPU computes
        // target = (PC+4) + d*2, so d = (target - (PC+4)) / 2 in [0, 255].
        const auto pc4 = insn.address() + 4;
        const auto target = op.getError() ? pc4 : op.val.getUnsigned();
        const auto delta = static_cast<int32_t>(target - pc4);
        if (delta < 0 || delta > 510 || (delta & 1)) {
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        } else {
            opc |= (delta >> 1) & 0xFF;
        }
        break;
    }
    case M_PCL: {
        // Operand is the absolute target address. The CPU computes
        // target = ((PC+4) & ~3) + d*4, so d = (target - ((PC+4) & ~3)) / 4.
        // PC may be any even value (the mask handles non-4-aligned PC);
        // target itself must be 4-aligned for the encoding to round-trip.
        const auto pc4 = (insn.address() + 4) & ~Config::uintptr_t(3);
        const auto target = op.getError() ? pc4 : op.val.getUnsigned();
        if (target & 3) {
            insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
            break;
        }
        const auto delta = static_cast<int32_t>(target - pc4);
        if (delta < 0 || delta > 1020) {
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        } else {
            opc |= (delta >> 2) & 0xFF;
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
    case M_TNUM: {
        // TRAPA #imm, unsigned 8-bit.
        const auto val = op.val.getUnsigned();
        if (val > 255)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        opc |= val & 0xFF;
        break;
    }
    case M_REL8:
    case M_REL8P: {
        // 8-bit signed PC-relative: d = (target - (PC+4)) / 2, [-128, 127]
        const auto pc4 = insn.address() + 4;
        const auto target = op.getError() ? pc4 : op.val.getUnsigned();
        const auto delta = static_cast<int32_t>(target - pc4);
        if (delta < -256 || delta > 254 || (delta & 1))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        opc |= (delta >> 1) & 0xFF;
        break;
    }
    case M_REL12: {
        // 12-bit signed PC-relative branch: d = (target - (PC+4)) / 2, [-2048, 2047]
        const auto pc4 = insn.address() + 4;
        const auto target = op.getError() ? pc4 : op.val.getUnsigned();
        const auto delta = static_cast<int32_t>(target - pc4);
        if (delta < -4096 || delta > 4094 || (delta & 1))
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

    if (searchName(cpuType(), fpuType(), insn))
        return _insn.setError(insn.srcOp, insn);

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
