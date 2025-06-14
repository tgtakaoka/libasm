/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "asm_tms320.h"
#include "table_tms320.h"
#include "text_common.h"

namespace libasm {
namespace tms320 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_dALIGN,  &Assembler::alignOrigin},
    {TEXT_dBSS,    &Assembler::allocateSpaces,     Assembler::DATA_WORD},
    {TEXT_dBYTE,   &Assembler::defineDataConstant, Assembler::DATA_BYTE_IN_WORD},
    {TEXT_dEVEN,   &Assembler::alignOrigin,        Assembler::ALIGN_EVEN},
    {TEXT_dLONG,   &Assembler::defineDataConstant, Assembler::DATA_LONG},
    {TEXT_dORG,    &Assembler::defineOrigin},
    {TEXT_dSTRING, &Assembler::defineDataConstant, Assembler::DATA_WORD},
    {TEXT_dWORD,   &Assembler::defineDataConstant, Assembler::DATA_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmTms320::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return IntelNumberParser::singleton(); }
        const CommentParser &comment() const override { return Tms320CommentParser::singleton(); }
        const SymbolParser &symbol() const override { return Tms320SymbolParser::singleton(); }
        const LetterParser &letter() const override { return Tms320LetterParser::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmTms320::AsmTms320(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

void AsmTms320::encodeIndirect(AsmInsn &insn, const Operand &op) const {
    static constexpr uint8_t MAR[] PROGMEM = {
            0x80,  // M_ARP: *
            0xA0,  // M_INC: *+
            0x90,  // M_DEC: *-
            0xE0,  // M_INC0: *0+
            0xD0,  // M_DEC0: *0-
            0xF0,  // M_IBR0: *BR0+
            0xC0,  // M_DBR0: *BR0-
    };
    if (op.mode >= M_ARP)
        insn.embed(pgm_read_byte(&MAR[op.mode - M_ARP]));
}

void AsmTms320::encodeDirect(AsmInsn &insn, const Operand &op) const {
    const auto dma = op.val.getUnsigned();
    if (op.val.isNegative() || !validDmAddr(insn.opCode(), dma))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.embed(dma & 0x7F);
}

void AsmTms320::encodeNextAR(AsmInsn &insn, const Operand &op) const {
    if ((insn.opCode() & 0x80) == 0)
        return;
    if (op.mode == M_NONE) {
        if (is3201x())
            insn.embed(8);
        return;
    }
    auto val = op.val.getUnsigned();
    if (!isAR(decodeAR(val))) {
        val &= maxAR();
        insn.setErrorIf(op, UNKNOWN_REGISTER);
    }
    if (is3202x())
        insn.embed(8);
    insn.embed(val);
}

void AsmTms320::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    insn.setErrorIf(op);
    auto val = op.val.getUnsigned();
    auto max = UINT16_MAX;
    auto error = OVERFLOW_RANGE;
    switch (mode) {
    case M_MAM:
        if (op.mode == M_UI16 || op.mode == M_LS16) {
            encodeDirect(insn, op);
        } else {
            encodeIndirect(insn, op);
        }
        break;
    case M_IND:
    case M_MAR:
        encodeIndirect(insn, op);
        break;
    case M_NARP:
        encodeNextAR(insn, op);
        break;
    case M_LS0:
        if (val)
            insn.setErrorIf(op, ILLEGAL_CONSTANT);
        break;
    case M_LS3:
        max = 7;
        if (!is320Cx() && !(val == 0 || val == 1 || val == 4))
            insn.setErrorIf(op, ILLEGAL_CONSTANT);
        goto check_hi8;
    case M_XCN:
        max = 2;
        goto check_int;
    case M_LS4Z:
        max = 16;
    check_int:
        if (op.val.overflow(max, 1)) {
            val &= (max - 1);
            insn.setErrorIf(op, OVERFLOW_RANGE);
        }
        val -= 1;
        if (mode == M_XCN)
            val <<= 12;
        insn.embed(val);
        break;
    case M_BIT:
    case M_LS4:
        max = 15;
    check_hi8:
        if (op.val.overflow(max)) {
            val &= max;
            insn.setErrorIf(op, error);
        }
        insn.embed(val << 8);
        break;
    case M_PA:
        max = maxPA();
        goto check_hi8;
    case M_AR:
        max = maxAR();
        error = UNKNOWN_REGISTER;
        goto check_hi8;
    case M_ARK:
        max = maxAR();
        error = UNKNOWN_REGISTER;
        goto check_lo8;
    case M_STN:
    case M_UI1:
        max = 1;
    check_lo8:
        if (op.val.overflow(max)) {
            val &= max;
            insn.setErrorIf(op, error);
        }
        if (mode == M_STN)
            val <<= 8;
        insn.embed(val);
        break;
    case M_UI2:
        max = 3;
        goto check_lo8;
    case M_LS4L:
        max = 15;
        goto check_lo8;
    case M_UI5:
        max = 31;
        goto check_lo8;
    case M_UI8:
    case M_IMU8:
        max = UINT8_MAX;
        goto check_lo8;
    case M_UI9:
    case M_IMU9:
        max = 0x1FF;
        goto check_lo8;
    case M_SI13:
    case M_IM13:
        if (op.val.overflow(0x0FFF, -0x1000))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embed(val & 0x1FFF);
        break;
    case M_PM12:
        if (op.val.overflow(0x0FFF)) {
            val &= 0x0FFF;
            insn.setErrorIf(op, OVERFLOW_RANGE);
        }
        // Fall-through
    case M_PM16:
    case M_PA16:
        if (op.val.overflow(UINT16_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        // Fall-through
    case M_UI16:
    case M_IMU16:
        if (op.val.overflow(UINT16_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(val);
        break;
    case M_SI16:
    case M_IM16:
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(val);
        break;
    case M_CTL:
        if (op.reg == REG_HM && is320C20x())
            insn.setErrorIf(op, ILLEGAL_REGISTER);
        insn.embed(encodeControlName(op.reg, is320C2x()));
        break;
    case M_CC:
        encodeConditionCode(insn, op);
        break;
    default:
        break;
    }
}

void AsmTms320::encodeConditionCode(AsmInsn &insn, const Operand &op) const {
    if (is320C2x()) {
        static constexpr uint8_t CC[] PROGMEM = {
                0xFF,  // CC_UNC = 0,   Unconditional
                0xF9,  // CC_TC = 13,   TC=1:   BBNZ
                0xF8,  // CC_NTC = 12,  TC=0:   BBZ
                0xFA,  // CC_BIO = 11,  BIO=0:  BIOZ
                0xF6,  // CC_EQ = 1,    Z=1:    BZ
                0xF5,  // CC_NEQ = 2,   Z=0:    BNZ
                0xF3,  // CC_LT = 3,    ACC<0:  BLZ
                0xF4,  // CC_GEQ = 6,   ACC>=0: BGEZ
                0xF0,  // CC_OV = 8,    V=1:    BV
                0xF7,  // CC_NOV = 7,   V=0:    BNV
                0x5E,  // CC_C = 10,    C=1:    BC
                0x5F,  // CC_NC = 9,    C=0:    BNC
                0xF2,  // CC_LEQ = 4,   ACC<=0: BLEZ
                0xF1,  // CC_GT = 5,    ACC>0:  BGZ
        };
        insn.embed(pgm_read_byte(CC + op.cc) << 8);
        return;
    }
    constexpr auto TP = 0x300;
    constexpr auto MASK = 0x00F;
    static constexpr Config::opcode_t TABLE[] PROGMEM = {
            0x000,  // CC_UNC = 0   uncondition
            0x100,  // CC_TC = 1    TC=1
            0x200,  // CC_NTC = 2   TC=0
            0x300,  // CC_BIO = 3   BIO=0
            0x088,  // CC_EQ = 4    Z=1
            0x008,  // CC_NEQ = 5   Z=0
            0x044,  // CC_LT = 6    L=1
            0x08C,  // CC_GEQ = 7   Z=1, L=0
            0x022,  // CC_OV = 8    V=1
            0x002,  // CC_NOV = 9   V=0
            0x011,  // CC_C = 10    C=1
            0x001,  // CC_NC = 11   C=0
            0x0CC,  // CC_LEQ = 12  Z=1, L=1
            0x004,  // CC_GT = 13   L=0
    };
    auto uncondition = false;
    Config::opcode_t code = 0;
    auto p = op.ccAt;
    do {
        const auto at = p.skipSpaces();
        const auto cc = parseCcName(p, parser());
        const auto item = pgm_read_word(TABLE + cc);
        if (cc == CC_UNC)
            uncondition = true;
        if (uncondition && item)
            insn.setErrorIf(at, ILLEGAL_COMBINATION);
        if (item & TP) {
            if ((code & TP) == 0) {
                code |= item;
            } else {
                insn.setErrorIf(at, ILLEGAL_COMBINATION);
            }
        }
        if (item & MASK) {
            const auto both = (code & item & MASK) << 4;
            if (both == 0) {
                code |= item;
            } else if ((code & both) != (item & both)) {
                insn.setErrorIf(at, ILLEGAL_COMBINATION);
            }
        }
    } while (p.skipSpaces().expect(','));
    if ((code & TP) == 0 || (code & TP) == TP)
        code ^= TP;  // flip BIO and UNC
    insn.embed(code);
}

Error AsmTms320::parseConditionCode(StrScanner &scan, Operand &op) const {
    auto p = scan;
    op.cc = parseCcName(p, parser());
    if (op.cc == CC_UNDEF)
        return NOT_AN_EXPECTED;
    op.ccAt = scan;
    // C and TC may bre condition code or control bit
    if (op.cc == CC_C || op.cc == CC_TC) {
        op.mode = M_CCCTL;
        op.reg = (op.cc == CC_C) ? REG_C : REG_TC;
    } else {
        op.mode = M_CC;
    }
    if (is320C20x() || is320C5x()) {
        while (p.skipSpaces().expect(',')) {
            if (parseCcName(p.skipSpaces(), parser()) == CC_UNDEF) {
                op.setErrorIf(p, UNKNOWN_OPERAND);
                return OK;
            }
            op.mode = M_CC;
        }
    }
    scan = p;
    return OK;
}

Error AsmTms320::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#')) {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        if (op.val.isNegative()) {
            op.mode = M_IM16;
        } else {
            op.mode = op.val.overflow(UINT8_MAX) ? M_IMU16 : M_IMU8;
        }
        scan = p;
        return OK;
    }

    if (p.expect('*')) {
        auto s = p;
        const auto br = p.iexpect('B') && p.iexpect('R');
        if (!br)
            p = s;
        const auto ar0 = p.expect('0');
        if (br && !ar0)
            return op.setErrorIf(UNKNOWN_OPERAND);
        if (p.expect('+')) {
            op.mode = ar0 ? (br ? M_IBR0 : M_INC0) : M_INC;
        } else if (p.expect('-')) {
            op.mode = ar0 ? (br ? M_DBR0 : M_DEC0) : M_DEC;
        } else {
            if (br || ar0)
                return op.setErrorIf(UNKNOWN_OPERAND);
            op.mode = M_ARP;
        }
        scan = p;
        return OK;
    }

    if (parseConditionCode(p, op) == OK) {
        if (op.hasError())
            return op.getError();
        scan = p;
        return OK;
    }

    op.reg = parseRegName(p, parser());
    if (op.reg != REG_UNDEF) {
        if (isControlName(op.reg)) {
            op.mode = M_CTL;
        } else if (op.reg == REG_BMAR) {
            op.mode = M_BMAR;
        } else {
            op.mode = isAR(op.reg) ? M_AR : M_PA;
            op.val.setUnsigned(encodeRegName(op.reg));
        }
        scan = p;
        return OK;
    }

    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    if (op.val.isNegative()) {
        op.mode = M_SI16;
    } else {
        op.mode = (op.val.getUnsigned() == 16) ? M_LS16 : M_UI16;
    }
    scan = p;
    return OK;
}

Error AsmTms320::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.op1) && insn.op1.hasError())
        return _insn.setError(insn.op1);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.op2) && insn.op2.hasError())
            return _insn.setError(insn.op2);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, insn.op3) && insn.op3.hasError())
            return _insn.setError(insn.op3);
        scan.skipSpaces();
    }

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.op1, insn);

    encodeOperand(insn, insn.op1, insn.mode1());
    encodeOperand(insn, insn.op2, insn.mode2());
    encodeOperand(insn, insn.op3, insn.mode3());
    insn.emitInsn();
    return _insn.setErrorIf(insn);
}

}  // namespace tms320
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
