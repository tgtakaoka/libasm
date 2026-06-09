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

#include "asm_h8300.h"
#include "reg_h8300.h"
#include "table_h8300.h"
#include "text_h8300.h"

namespace libasm {
namespace h8300 {

using namespace pseudo;
using namespace reg;

namespace {

// clang-format off
constexpr char TEXT_DATA[]   PROGMEM = ".DATA";
constexpr char TEXT_DATA_B[] PROGMEM = ".DATA.B";
constexpr char TEXT_DATA_W[] PROGMEM = ".DATA.W";
constexpr char TEXT_DATA_L[] PROGMEM = ".DATA.L";
constexpr char TEXT_SDATA[]  PROGMEM = ".SDATA";
constexpr char TEXT_RES[]    PROGMEM = ".RES";
constexpr char TEXT_RES_B[]  PROGMEM = ".RES.B";
constexpr char TEXT_RES_W[]  PROGMEM = ".RES.W";
constexpr char TEXT_RES_L[]  PROGMEM = ".RES.L";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_DATA,   &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING },
    { TEXT_DATA_B, &Assembler::defineDataConstant, Assembler::DATA_BYTE_NO_STRING },
    { TEXT_DATA_L, &Assembler::defineDataConstant, Assembler::DATA_LONG           },
    { TEXT_DATA_W, &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING },
    { TEXT_RES,    &Assembler::allocateSpaces,     Assembler::DATA_WORD           },
    { TEXT_RES_B,  &Assembler::allocateSpaces,     Assembler::DATA_BYTE           },
    { TEXT_RES_L,  &Assembler::allocateSpaces,     Assembler::DATA_LONG           },
    { TEXT_RES_W,  &Assembler::allocateSpaces,     Assembler::DATA_WORD           },
    { TEXT_SDATA,  &Assembler::defineString,       0                              },
};
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};
// clang-format on

}  // namespace

const ValueParser::Plugins &AsmH8300::defaultPlugins() {
    return ValueParser::Plugins::hitachi();
}

AsmH8300::AsmH8300(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_advancedMode),
      Config(TABLE),
      _opt_advancedMode(this, &Config::setAdvancedMode,
              ::libasm::text::h8300::OPT_BOOL_ADVANCED_MODE,
              ::libasm::text::h8300::OPT_DESC_ADVANCED_MODE) {
    reset();
}

RegName AsmH8300::parseRegOperand(StrScanner &scan) const {
    const auto reg = parseRegName(scan, parser());
    if (reg == REG_SP)
        return hasReg32() ? REG_ER7 : REG_R7;
    return reg;
}

Error AsmH8300::parseBitSuffix(StrScanner &scan, Operand &op) const {
    auto p = scan;
    if (p.expect(':')) {
        if (p.skipSpaces().iexpectWord_P(PSTR("8"))) {
            op.bitSuffix = 8;
        } else if (p.iexpectWord_P(PSTR("16"))) {
            op.bitSuffix = 16;
        } else if (p.iexpectWord_P(PSTR("24"))) {
            op.bitSuffix = 24;
        } else {
            return op.setErrorIf(scan, ILLEGAL_SIZE);
        }
        scan = p;
    }
    return OK;
}

Error AsmH8300::parseOperand(StrScanner &scan, Operand &op) const {
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
        if (parseBitSuffix(p, op))
            return op.getError();
        if (op.bitSuffix == 8) {
            if (op.val.overflow(UINT8_MAX))
                op.setErrorIf(s, OVERFLOW_RANGE);
            op.mode = M_IMM8;
        } else if (op.bitSuffix == 16) {
            if (op.val.overflow(UINT16_MAX))
                op.setErrorIf(s, OVERFLOW_RANGE);
            op.mode = M_IMM16;
        } else if (!op.val.overflow(UINT8_MAX)) {
            switch (op.val.getUnsigned()) {
            case 1:
                op.mode = M_VAL1;
                break;
            case 2:
                op.mode = M_VAL2;
                break;
            case 4:
                op.mode = M_VAL4;
                break;
            default:
                op.mode = M_IMM8;
                break;
            }
        } else if (!op.val.overflowUint16()) {
            op.mode = M_IMM16;
        } else {
            op.mode = M_IMM32;
        }
        scan = p;
        return OK;
    }

    if (p.expect('@')) {
        if (p.expect('@')) {
            s = p;
            // Memory indirect: @@abs8
            op.val = parseInteger(p, op);
            if (op.hasError())
                return op.getError();
            auto z = p;
            if (parseBitSuffix(p, op) || op.bitSuffix == 16)
                return op.setErrorIf(z, ILLEGAL_SIZE);
            if (op.val.overflow(UINT8_MAX))
                op.setErrorIf(s, OVERFLOW_RANGE);
            op.mode = M_MIND8;
            scan = p;
            return OK;
        }
        if (p.expect('-')) {
            s = p;
            // Pre-decrement: @-Rn
            op.reg = parseRegOperand(p);
            if (isAddrReg(op.reg, hasReg32())) {
                op.mode = M_PDEC;
                scan = p;
                return OK;
            }
            if (op.reg != REG_UNDEF)
                return op.setErrorIf(s, REGISTER_NOT_ALLOWED);
            return op.setErrorIf(s, UNKNOWN_OPERAND);
        }
        if (p.expect('(')) {
            s = p;
            // Indexed: @(disp16,Rn) or @(d:24,ERn)
            op.val = parseInteger(p, op);
            if (op.hasError())
                return op.getError();
            auto z = p;
            if (parseBitSuffix(p, op) || op.bitSuffix == 8)
                return op.setErrorIf(z, ILLEGAL_SIZE);
            const bool idx24 = (op.bitSuffix == 24);
            // d:16 is sign-extended (manual section 1.7). In advanced mode
            // it must be a strict signed 16-bit value [-32768,32767];
            // in normal mode either signed or unsigned [0,65535] is fine
            // because only the low 16 bits of the address are observable.
            // d:24 stays unsigned (high reserved byte must be zero).
            const bool overflow = idx24 ? op.val.overflow(UINT24_MAX)
                                        : advancedMode() ? op.val.overflowInt16()
                                                         : op.val.overflowUint16();
            if (overflow)
                op.setErrorIf(s, OVERFLOW_RANGE);
            if (!p.skipSpaces().expect(','))
                return op.setError(MISSING_COMMA);
            s = p;
            op.reg = parseRegOperand(p.skipSpaces());
            if (isAddrReg(op.reg, hasReg32())) {
                if (p.skipSpaces().expect(')')) {
                    op.mode = idx24 ? M_IDX24 : M_IDX16;
                    scan = p;
                    return OK;
                }
                return op.setError(MISSING_CLOSING_PAREN);
            }
            if (op.reg != REG_UNDEF)
                return op.setErrorIf(s, REGISTER_NOT_ALLOWED);
            return op.setErrorIf(s, UNKNOWN_OPERAND);
        }
        // Try register indirect: @Rn or @Rn+
        op.reg = parseRegOperand(p);
        if (isAddrReg(op.reg, hasReg32())) {
            op.mode = p.expect('+') ? M_PINC : M_INDIR;
            scan = p;
            return OK;
        } else if (op.reg != REG_UNDEF) {
            return op.setErrorIf(s, REGISTER_NOT_ALLOWED);
        }
        // Absolute address: @abs:16 or @abs:8 or @abs (default 16)
        s = p;
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        if (parseBitSuffix(p, op))
            return op.setErrorIf(p, ILLEGAL_SIZE);
        if (op.bitSuffix == 24) {
            if (op.val.overflow(UINT24_MAX))
                op.setErrorIf(s, OVERFLOW_RANGE);
            op.mode = M_ABS24;
            scan = p;
            return OK;
        }
        if (op.bitSuffix == 16) {
            if (overflowAbs16(op.val))
                op.setErrorIf(s, OVERFLOW_RANGE);
            op.mode = M_ABS16;
            scan = p;
            return OK;
        }
        if (op.bitSuffix == 8) {
            if (overflowAbs8(op.val))
                op.setErrorIf(s, OVERFLOW_RANGE);
            op.val.setUnsigned(op.val.getUnsigned() | 0xFF00);
            op.mode = M_ABS8;
            scan = p;
            return OK;
        }
        if (op.val.overflow(UINT16_MAX))
            op.setErrorIf(s, OVERFLOW_RANGE);
        if (!op.val.overflow(0xFFFF, 0xFF00)) {
            op.mode = M_ABS8;
        } else {
            op.mode = M_ABS16;
        }
        scan = p;
        return OK;
    }

    // Try register
    const auto reg = parseRegOperand(p);
    if (reg != REG_UNDEF) {
        op.reg = reg;
        if (isReg8(reg)) {
            op.mode = M_REG8;
        } else if (isReg32(reg)) {
            op.mode = M_REG32;
        } else if (isReg16(reg)) {
            op.mode = M_REG16;
        } else if (reg == REG_CCR) {
            op.mode = M_CCR;
        } else {
            return op.setError(p, UNKNOWN_OPERAND);
        }
        scan = p;
        return OK;
    }

    // Plain number → branch target address (M_REL8)
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_REL8;
    scan = p;
    return OK;
}

void AsmH8300::encodeOprReg8(AsmInsn &insn, const Operand &op, OprPos pos) const {
    if (!isReg8(op.reg)) {
        insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        return;
    }
    Config::opcode_t regno = encodeReg8(op.reg);
    if (pos == POS_PRX) {
        regno <<= Entry::bitGroupPos(insn.prefixPos);
        insn.embedPrefix(regno);
        return;
    }
    regno <<= Entry::bitGroupPos(pos);
    insn.embed(regno);
}

void AsmH8300::encodeOprReg16(AsmInsn &insn, const Operand &op, OprPos pos) const {
    if (!isReg16(op.reg)) {
        insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        return;
    }
    Config::opcode_t regno = encodeReg16(op.reg);
    if (pos == POS_PRX) {
        regno <<= Entry::bitGroupPos(insn.prefixPos);
        insn.embedPrefix(regno);
        return;
    }
    regno <<= Entry::bitGroupPos(pos);
    insn.embed(regno);
}

void AsmH8300::encodeOprReg32(AsmInsn &insn, const Operand &op, OprPos pos) const {
    if (!isReg32(op.reg)) {
        insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        return;
    }
    Config::opcode_t regno = encodeReg32(op.reg);
    if (pos == POS_PRX) {
        regno <<= Entry::bitGroupPos(insn.prefixPos);
        insn.embedPrefix(regno);
        return;
    }
    regno <<= Entry::bitGroupPos(pos);
    insn.embed(regno);
}

void AsmH8300::encodeOprAddrReg(AsmInsn &insn, const Operand &op, OprPos pos) const {
    if (!isAddrReg(op.reg, insn.hasReg32())) {
        insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        return;
    }
    Config::opcode_t regno = encodeAddrReg(op.reg);
    if (pos == POS_PRX) {
        regno <<= Entry::bitGroupPos(insn.prefixPos);
        insn.embedPrefix(regno);
        return;
    }
    regno <<= Entry::bitGroupPos(pos);
    insn.embed(regno);
}

void AsmH8300::encodeAbsolute(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const {
    const auto addr = op.val.getUnsigned();
    if (mode == M_ABS8 || mode == M_MIND8) {
        const auto addr8 = addr & UINT8_MAX;
        if (pos == POS_PRX) {
            insn.embedPrefix(addr8);
        } else {
            insn.embed(addr8);
        }
        return;
    }
    insn.emitOperand16(addr);
}

void AsmH8300::encodeRelative(AsmInsn &insn, const Operand &op) const {
    const auto base = insn.address() + 2;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    insn.setErrorIf(op, checkAddr(target, true));
    const auto delta = branchDelta(base, target, insn, op);
    if (!overflowDelta(delta, 8)) {
        insn.embed(delta & UINT8_MAX);
        return;
    }
    // 8-bit delta doesn't fit. On H8/300H, promote to the matching :16
    // long-branch form (0x40+cc -> 0x5800 | cc<<4, BSR 0x55 -> 0x5C00).
    // On H8/300 there is no long branch, so flag the error.
    const auto opcHi = insn.opCode() >> 8;
    const bool hasLong = hasReg32() &&
            ((opcHi >= 0x40 && opcHi <= 0x4F) || opcHi == 0x55);
    if (!hasLong) {
        insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.embed(delta & UINT8_MAX);
        return;
    }
    const uint16_t longOpc = (opcHi == 0x55) ? 0x5C00
                                             : 0x5800 | ((opcHi - 0x40) << 4);
    insn.setOpCode(longOpc);
    const auto base16 = insn.address() + 4;
    const auto target16 = op.getError() ? base16 : op.val.getUnsigned();
    const auto delta16 = branchDelta(base16, target16, insn, op);
    if (overflowDelta(delta16, 16))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand16(delta16);
}

void AsmH8300::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const {
    // Propagate any error left on the operand by parseOperand (range
    // overflows, invalid registers, etc.) before encoding so the insn
    // surfaces it even if the encoding path doesn't consult op.
    insn.setErrorIf(op);
    switch (mode) {
    case M_REG8:
        encodeOprReg8(insn, op, pos);
        break;
    case M_REG16:
        encodeOprReg16(insn, op, pos);
        break;
    case M_INDIR:
    case M_PDEC:
    case M_PINC:
    case M_IDX16:
        encodeOprAddrReg(insn, op, pos);
        if (mode == M_IDX16)
            insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_IDX24: {
        // ERn goes into the prefix word at insn.prefixPos; disp24 follows
        // in a 4-byte slot with the top byte reserved as zero.
        encodeOprAddrReg(insn, op, pos);
        insn.emitOperand32(op.val.getUnsigned() & UINT24_MAX);
        break;
    }
    case M_REG32:
        encodeOprReg32(insn, op, pos);
        break;
    case M_ADREG:
        encodeOprAddrReg(insn, op, pos);
        break;
    case M_ABS8:
    case M_ABS16:
    case M_MIND8:
        encodeAbsolute(insn, op, mode, pos);
        break;
    case M_ABS24: {
        const auto addr = op.val.getUnsigned() & UINT24_MAX;
        if (pos == POS__FF) {
            insn.embed(static_cast<uint8_t>(addr >> 16));
            insn.emitOperand16(static_cast<uint16_t>(addr));
        } else {
            insn.emitOperand32(addr);
        }
        break;
    }
    case M_REL8:
        encodeRelative(insn, op);
        break;
    case M_IMM8:
        insn.embed(op.val.getUnsigned() & UINT8_MAX);
        break;
    case M_IMM16:
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_IMM32:
        insn.emitOperand32(op.val.getUnsigned());
        break;
    case M_IMM3:
        if (op.val.overflow(7))
            insn.setErrorIf(op, ILLEGAL_BIT_NUMBER);
        insn.embed((op.val.getUnsigned() & 7) << 4);  // bit number in high nibble
        break;
    case M_VAL1:
    case M_VAL2:
    case M_VAL4:
        break;
    case M_NONE:
    case M_CCR:
    default:
        break;
    }
}

OprSize AsmInsn::parseSizeSuffix() {
    StrScanner p{name()};
    p.trimStart([](char c) { return c != '.'; });
    auto eos = const_cast<char *>(p.str());
    sizeSuffix = SZ_NONE;
    if (p.expect('.')) {
        if (p.iexpect('B')) {
            sizeSuffix = SZ_BYTE;
        } else if (p.iexpect('W')) {
            sizeSuffix = SZ_WORD;
        } else if (p.iexpect('L')) {
            sizeSuffix = SZ_LONG;
        }
        if (sizeSuffix == SZ_NONE || *p) {
            setErrorIf(p, ILLEGAL_SIZE);
            return SZ_NONE;
        }
        *eos = 0;
    }
    return sizeSuffix;
}

uint_fast8_t AsmInsn::operandPos() const {
    uint_fast8_t pos = 2;
    if (superPrefix != SPRX_NONE)
        pos += 2;
    if (hasPrefix())
        pos += 2;
    const auto len = length();
    return len < pos ? pos : len;
}

void AsmInsn::emitInsn() {
    uint_fast8_t pos = 0;
    if (superPrefix != SPRX_NONE) {
        emitUint16(fromSuperPrefix(superPrefix), pos);
        pos += 2;
    }
    if (hasPrefix()) {
        emitUint16(prefix(), pos);
        pos += 2;
    }
    emitUint16(opCode(), pos);
}

Error AsmH8300::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    insn.setHasReg32(hasReg32());
    insn.parseSizeSuffix();

    if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
        return _insn.setError(insn.srcOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
            return _insn.setError(insn.dstOp);
    }
    scan.skipSpaces();

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.srcOp, insn);

    if (insn.sizeSuffix != SZ_NONE) {
        const auto expected = (insn.insnSize() == ISZ_ADDR)
                                      ? (insn.hasReg32() ? SZ_LONG : SZ_WORD)
                                      : insn.oprSize();
        // ISZ_ADDR on H8/300H also tolerates .W for back-compat with H8/300
        // assembly: ADDS/SUBS share the same encoding regardless of suffix.
        const bool addrFallback =
                insn.insnSize() == ISZ_ADDR && insn.sizeSuffix == SZ_WORD;
        if (insn.sizeSuffix != expected && !addrFallback) {
            StrScanner p{_insn.errorAt()};
            p.trimStart([](char c) { return c != '.'; });
            return _insn.setErrorIf(p, ILLEGAL_SIZE);
        }
    }

    encodeOperand(insn, insn.srcOp, insn.src(), insn.srcPos());
    encodeOperand(insn, insn.dstOp, insn.dst(), insn.dstPos());
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace h8300
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
