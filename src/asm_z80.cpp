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

#include "asm_z80.h"
#include "table_z80.h"
#include "text_common.h"

namespace libasm {
namespace z80 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::option;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_DB,   &Assembler::defineDataConstant, Assembler::DATA_BYTE },
    { TEXT_DEFB, &Assembler::defineDataConstant, Assembler::DATA_BYTE },
    { TEXT_DEFM, &Assembler::defineDataConstant, Assembler::DATA_BYTE },
    { TEXT_DEFS, &Assembler::allocateSpaces,     Assembler::DATA_BYTE },
    { TEXT_DEFW, &Assembler::defineDataConstant, Assembler::DATA_WORD },
    { TEXT_DL,   &Assembler::defineDataConstant, Assembler::DATA_LONG },
    { TEXT_DS,   &Assembler::allocateSpaces,     Assembler::DATA_BYTE },
    { TEXT_DW,   &Assembler::defineDataConstant, Assembler::DATA_WORD },
};
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};
// clang-format on

}  // namespace

const ValueParser::Plugins &AsmZ80::defaultPlugins() {
    static const struct final : ValueParser::IntelPlugins {
        const OperatorParser &operators() const override {
            return ZilogOperatorParser::singleton();
        }
    } PLUGINS{};
    return PLUGINS;
}

AsmZ80::AsmZ80(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

void AsmZ80::encodeRelative(AsmInsn &insn, const Operand &op) const {
    const auto deltaLen = 1;
    uint32_t base = insn.address() + insn.operandPos() + deltaLen;
    if (insn.src() == M_IM8) {
        base += 1;
    } else if (insn.src() == M_IM16) {
        base += 2;
    }
    uint32_t target = 0;
    int32_t delta = 0;
    target = op.getError() ? base : op.val.getUnsigned();
    delta = branchDelta(base, target, insn, op);
    if ((base & ~UINT16_MAX) != (target & ~UINT16_MAX))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    if (overflowDelta(delta, 8))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand8(delta);
}

void AsmZ80::encodeAbsolute(AsmInsn &insn, const Operand &op) const {
    if (op.val.overflow(UINT16_MAX))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.emitOperand16(op.val.getUnsigned());
}

namespace {
void encodeIndexReg(AsmInsn &insn, RegName ixReg) {
    const auto ix = (ixReg == REG_IX) ? Entry::IX : Entry::IY;
    auto prefix = insn.prefix();
    if (prefix < 0x100) {
        prefix = ix;
    } else {
        prefix &= 0xFF;
        prefix |= (ix << 8);
    }
    insn.setPrefix(prefix);
}
}  // namespace

void AsmZ80::encodeShortIndex(AsmInsn &insn, const Operand &op) const {
    if (op.val.overflowInt8())
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.emitOperand8(op.val.getUnsigned());
}

void AsmZ80::encodeImmediate16(AsmInsn &insn, const Operand &op) const {
    if (op.val.overflowUint16())
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.emitOperand16(op.val.getUnsigned());
}

void AsmZ80::encodeOperand(
        AsmInsn &insn, const Operand &op, AddrMode mode, const Operand &other) const {
    insn.setErrorIf(op);
    if (mode >= R_BASE && other.mode == R_ALT) {
        const auto base = RegName(mode - R_BASE);
        if (base != alt2BaseReg(other.reg))
            insn.setErrorIf(other, REGISTER_NOT_ALLOWED);
    }
    auto val16 = op.val.getUnsigned();
    switch (mode) {
    case M_IM8:
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(val16);
        break;
    case M_IOA:
        if (op.val.overflow(UINT8_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(val16);
        break;
    case M_IDX:
    case M_IDX8:
        encodeShortIndex(insn, op);
        /* Fall-through */
    case R_IDX:
    case I_IDX:
        encodeIndexReg(insn, op.reg);
        break;
    case R_PTRH:
        insn.embed(op.reg == REG_BC ? 0x00 : 0x10);
        break;
    case M_IM16:
    case M_LM16:
        encodeImmediate16(insn, op);
        break;
    case M_ABS:
    case M_JABS:
    case M_DABS:
        encodeAbsolute(insn, op);
        break;
    case M_REL8:
        encodeRelative(insn, op);
        break;
    case M_CC4:
    case M_CC8:
        insn.embed(static_cast<uint8_t>(val16) << 3);
        break;
    case M_R16:
        insn.embed(encodePointerReg(op.reg) << 4);
        break;
    case M_R16X:
        if (op.reg == REG_HL)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        insn.embed(encodePointerRegIx(op.reg, other.reg) << 4);
        break;
    case I_PTR:
        insn.embed(encodeIndirectBase(op.reg) << 4);
        break;
    case M_SR8:
    case M_SRC:
        insn.embed(encodeDataReg(op.reg));
        break;
    case M_DR8:
    case M_DST:
        insn.embed(encodeDataReg(op.reg) << 3);
        break;
    case R_R:
        insn.embed(0x08);  // no need for R_I
        break;
    case M_VEC:
        if ((val16 & ~0x38) != 0) {
            val16 &= ~0x38;
            insn.setErrorIf(op, ILLEGAL_OPERAND);
        }
        insn.embed(val16);
        break;
    case M_BIT:
        if (op.val.overflow(7))
            insn.setErrorIf(op, ILLEGAL_BIT_NUMBER);
        insn.embed(static_cast<uint8_t>(val16 & 7) << 3);
        break;
    case M_IMMD:
        switch (val16) {
        case 0:
            break;
        case 1:
            insn.embed(2 << 3);
            break;
        case 2:
            insn.embed(3 << 3);
            break;
        case 3:
            // Fall-through
        default:
            insn.setErrorIf(op, ILLEGAL_OPERAND);
            break;
        }
        break;
    default:
        break;
    }
}

Error AsmZ80::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    // 'C' is either C-reg or C-condition
    auto a = p;
    op.reg = parseRegName(a, parser());
    if (op.reg == REG_C) {
        op.mode = R_C;
        op.val.setUnsigned(encodeCcName(CC_C));
        scan = a;
        return OK;
    }

    const auto cc = parseCcName(p, parser());
    if (cc != CC_UNDEF) {
        op.mode = isCc4Name(cc) ? M_CC4 : M_CC8;
        op.val.setUnsigned(encodeCcName(cc));
        scan = p;
        return OK;
    }

    op.reg = parseRegName(p, parser());
    if (op.reg != REG_UNDEF) {
        switch (op.reg) {
        case REG_I:
            op.mode = R_I;
            break;
        case REG_R:
            op.mode = R_R;
            break;
        case REG_B:
        case REG_D:
        case REG_E:
            op.mode = M_SR8;
            break;
        default:
            if (isAlternateReg(op.reg)) {
                op.mode = R_ALT;
            } else {
                op.mode = AddrMode(op.reg + R_BASE);
            }
            break;
        }
        scan = p;
        return OK;
    }
    if (p.expect('(')) {
        const auto regp = p.skipSpaces();
        op.reg = parseRegName(p, parser());
        if (op.reg == REG_UNDEF) {
            op.val = parseInteger(p, op, ')');
            if (op.hasError())
                return op.getError();
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            op.mode = M_ABS;
            scan = p;
            return OK;
        }
        if (p.skipSpaces().expect(')')) {
            switch (op.reg) {
            case REG_BC:
            case REG_DE:
                op.mode = I_PTR;
                break;
            case REG_IX:
            case REG_IY:
                op.mode = I_IDX;
                break;
            case REG_HL:
            case REG_SP:
            case REG_C:
                op.mode = AddrMode(op.reg + I_BASE);
                break;
            default:
                return op.setError(regp, REGISTER_NOT_ALLOWED);
            }
            scan = p;
            return OK;
        }
        if (*p == '+' || *p == '-') {
            op.val = parseInteger(p, op, ')');
            if (op.hasError())
                return op.getError();
            if (isIndexReg(op.reg)) {
                op.mode = M_IDX8;
            } else {
                return op.setErrorIf(regp, REGISTER_NOT_ALLOWED);
            }
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            scan = p;
            return OK;
        }
        return op.setError(UNKNOWN_OPERAND);
    }
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_IM16;
    scan = p;
    return OK;
}

void AsmInsn::emitInsn() {
    uint_fast8_t pos = 0;
    if (hasPrefix()) {
        const auto pre = prefix();
        if (pre >= 0x100)
            emitByte(pre >> 8, pos++);
        emitByte(pre, pos++);
    }
    if (ixBit()) {
        emitByte(opCode());
    } else {
        emitByte(opCode(), pos);
    }
}

uint_fast8_t AsmInsn::operandPos() const {
    uint_fast8_t len = 0;
    if (ixBit())
        return len + 2;
    len += !hasPrefix() ? 1 : (prefix() < 0x100 ? 2 : 3);
    const auto pos = length();
    return pos < len ? len : pos;
}

Error AsmZ80::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
        return _insn.setError(insn.dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
            return _insn.setError(insn.srcOp);
        scan.skipSpaces();
    }

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.dstOp, insn);

    encodeOperand(insn, insn.dstOp, insn.dst(), insn.srcOp);
    encodeOperand(insn, insn.srcOp, insn.src(), insn.dstOp);
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
