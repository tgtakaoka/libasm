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
    {TEXT_DB,   &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DEFB, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DEFM, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DEFS, &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DEFW, &Assembler::defineDataConstant, Assembler::DATA_WORD},
    {TEXT_DL,   &Assembler::defineDataConstant, Assembler::DATA_LONG},
    {TEXT_DS,   &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DW,   &Assembler::defineDataConstant, Assembler::DATA_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

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

int32_t AsmZ80::calcDeltaZ380(
        AsmInsn &insn, const ErrorAt &at, AddrMode &mode, int32_t delta) const {
    if (!overflowDelta(delta, 8))
        return delta;
    auto diff = delta;
    diff--;
    if (insn.prefix() == 0)
        diff--;
    if (!overflowDelta(diff, 16)) {
        mode = M_REL16;
        insn.setPrefix(0xDD);
        return diff;
    }
    diff--;
    if (!overflowDelta(diff, 24)) {
        mode = M_REL24;
        insn.setPrefix(0xFD);
        return diff;
    }
    insn.setErrorIf(at, OPERAND_TOO_FAR);
    return delta;
}

void AsmZ80::encodeRelative(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    const auto deltaLen = (mode == M_REL8) ? 1 : 2;
    uint32_t base = insn.address() + insn.operandPos() + deltaLen;
    if (insn.src() == M_IM8) {
        base += 1;
    } else if (insn.src() == M_IM16) {
        base += 2;
    } else if (insn.src() == M_EPU) {
        base += 4;
    }
    uint32_t target = 0;
    int32_t delta = 0;
    if (mode == M_PCDX) {
        delta = op.val.getSigned();
        target = base + delta;
        mode = M_REL16;
    } else {
        target = op.getError() ? base : op.val.getUnsigned();
        delta = branchDelta(base, target, insn, op);
    }
    if (z380())
        delta = calcDeltaZ380(insn, op, mode, delta);
    if (z380() && _extmode) {
        if ((delta >= 0 && target < base) || (delta < 0 && target >= base))
            insn.setErrorIf(op, OVERFLOW_RANGE);
    } else {
        if ((base & ~UINT16_MAX) != (target & ~UINT16_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
    }
    if (mode == M_REL8) {
        if (overflowDelta(delta, 8))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
    } else if (mode == M_REL16) {
        if (overflowDelta(delta, 16))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand16(delta);
    } else if (mode == M_REL24) {
        insn.emitOperand16(delta);
        insn.emitOperand8(delta >> 16);
    }
}

void AsmZ80::encodeAbsolute(AsmInsn &insn, const Operand &op) const {
    if (op.val.overflow(UINT16_MAX))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.emitOperand16(op.val.getUnsigned());
}

void AsmZ80::encodeMemoryPointer(AsmInsn &insn, const Operand &op) const {
    if (op.mode == M_ABS) {
        insn.embed(0x10);
        encodeAbsolute(insn, op);
    }
}

namespace {
void encodeIndexReg(AsmInsn &insn, RegName ixReg) {
    const auto ix = (ixReg == REG_IX) ? TableZ80::IX : TableZ80::IY;
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

void AsmZ80::encodeLongIndex(AsmInsn &insn, const Operand &op) const {
    if (op.val.overflowInt16())
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.embed(op.reg == REG_IX ? 0x00 : 0x10);
    insn.emitOperand16(op.val.getUnsigned());
}

void AsmZ80::encodeBaseIndex(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    if (op.reg == REG_SP) {
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val.getUnsigned());
    } else {
        const auto type = (op.reg == REG_HL) ? (op.idx == REG_IX ? 1 : 2) : 3;
        if (mode == M_BIXH) {
            insn.embed(type << 3);
        } else {
            insn.embed(type);
        }
    }
}

void AsmZ80::encodePointerIndex(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    if (op.mode == M_REL16 || op.mode == M_PCDX) {
        encodeRelative(insn, op, op.mode);
    } else {
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        const auto type = (op.reg == REG_HL) ? 3 : (op.reg == REG_IX ? 1 : 2);
        if (mode == M_PIXH) {
            insn.embed(type << 3);
        } else {
            insn.embed(type);
        }
        insn.emitOperand16(op.val.getUnsigned());
    }
}

void AsmZ80::encodeFullIndex(AsmInsn &insn, const Operand &op) const {
    if (op.mode == M_BDX || op.mode == M_SPDX) {
        encodeBaseIndex(insn, op, M_BIXH);
    } else if (op.mode == M_IDX8 || op.mode == M_IDX16 || op.mode == M_PDX || op.mode == M_REL16 ||
               op.mode == M_PCDX) {
        insn.embed(0x20);
        encodePointerIndex(insn, op, M_PIXH);
    }
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
    case M_IO16:
        if (op.val.overflow(UINT16_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(val16);
        break;
    case M_SPX:
        if (op.val.overflowInt8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(val16);
        break;
    case M_IDX:
    case M_IDX8:
        if (op.val.overflowInt8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(val16);
        /* Fall-through */
    case R_IDX:
    case I_IDX:
        encodeIndexReg(insn, op.reg);
        break;
    case R_IDXL:
        insn.embed(op.reg == REG_IX ? 0 : 1);
        break;
    case R_DXY:
    case R_SXY:
        if (op.reg == REG_IXL || op.reg == REG_IYL)
            insn.embed(mode == R_DXY ? 8 : 1);
        encodeIndexReg(insn,
                (op.reg == REG_IXH || op.reg == REG_IXL || op.reg == REG_IXU) ? REG_IX : REG_IY);
        break;
    case M_IDX16:
        encodeLongIndex(insn, op);
        break;
    case M_MPTR:
        encodeMemoryPointer(insn, op);
        break;
    case R_PTRL:
    case I_PTRL:
        insn.embed(op.reg == REG_BC ? 0 : 1);
        break;
    case R_PTRH:
        insn.embed(op.reg == REG_BC ? 0x00 : 0x10);
        break;
    case M_BIXH:
    case M_BIXL:
        encodeBaseIndex(insn, op, mode);
        break;
    case M_PIXH:
    case M_PIXL:
        encodePointerIndex(insn, op, mode);
        break;
    case M_FIDX:
        encodeFullIndex(insn, op);
        break;
    case M_IM16:
    case M_DM16:
    case M_XM16:
    case M_LM16:
    case M_ABS:
    case M_JABS:
    case M_DABS:
    case M_XABS:
        insn.emitOperand16(val16);
        break;
    case M_REL8:
    case M_REL16:
        return encodeRelative(insn, op, mode);
    case M_CC4:
    case M_CC8:
        insn.embed(static_cast<uint8_t>(val16) << 3);
        break;
    case M_R16:
    case M_NOHL:
        insn.embed(encodePointerReg(op.reg) << 4);
        break;
    case M_R16X:
        if (op.reg == REG_HL)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        insn.embed(encodePointerRegIx(op.reg, other.reg) << 4);
        break;
    case M_STK:
        insn.embed(encodeStackReg(op.reg) << 4);
        break;
    case I_PTR:
        insn.embed(encodeIndirectBase(op.reg) << 4);
        break;
    case M_SR8X:
        if (op.reg == REG_H || op.reg == REG_L)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        // Fall-though
    case M_SR8:
    case M_SRC:
        insn.embed(encodeDataReg(op.reg));
        break;
    case M_DR8X:
        if (op.reg == REG_H || op.reg == REG_L)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        // Fall-though
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
            if (z280() || z380()) {
                insn.embed(1 << 3);
                break;
            }
            // Fall-through
        default:
            insn.setErrorIf(op, ILLEGAL_OPERAND);
            break;
        }
        break;
    case M_EPU:
        insn.emitOperand32(op.val.getUnsigned());
        break;
    case M_LW:
    case M_LCK:
    case M_XM:
        insn.setPrefix(encodeCtlName(CtlName(op.mode - M_LW + CTL_LW)));
        break;
    default:
        break;
    }
}

Error AsmZ80::parseOperand(StrScanner &scan, Operand &op, const AsmInsn &insn) const {
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
    } else if (op.reg == REG_CP) {
        op.mode = R_ALT;
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

    const auto ctl = parseCtlName(p, parser());
    if (ctl != CTL_UNDEF) {
        op.mode = AddrMode((ctl - CTL_LW) + M_LW);
        scan = p;
        return OK;
    }

    op.reg = parseRegName(p, parser());
    if (op.reg != REG_UNDEF) {
        switch (op.reg) {
        case REG_IXU:
        case REG_IYU:
        case REG_IXH:
        case REG_IXL:
        case REG_IYH:
        case REG_IYL:
            op.mode = R_DXY;
            break;
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
    if (z280() && p.expect('<')) {
        op.val = parseInteger(p.skipSpaces(), op, '>');
        if (!p.skipSpaces().expect('>'))
            return op.setErrorIf(p, MISSING_CLOSING_DELIMITER);
        scan = p;
        op.mode = M_REL16;
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
            auto idxp = p;
            idxp++;
            op.idx = parseRegName(idxp.skipSpaces(), parser());
            if (op.idx == REG_UNDEF) {
                op.val = parseInteger(p, op, ')');
                if (op.hasError())
                    return op.getError();
                if (isIndexReg(op.reg)) {
                    op.mode = z80() ? M_IDX8 : (op.val.overflowInt8() ? M_IDX16 : M_IDX8);
                } else if (op.reg == REG_HL) {
                    op.mode = M_PDX;
                } else if (op.reg == REG_SP) {
                    op.mode = M_SPDX;
                } else if (op.reg == REG_PC) {
                    op.mode = M_PCDX;
                } else {
                    return op.setErrorIf(regp, REGISTER_NOT_ALLOWED);
                }
            } else {
                if (*p == '-')
                    return op.setErrorIf(p, UNKNOWN_OPERAND);
                if (op.reg == REG_HL && isIndexReg(op.idx)) {
                    op.mode = M_BDX;
                } else if (op.reg == REG_IX && op.idx == REG_IY) {
                    op.mode = M_BDX;
                } else if (op.reg == REG_HL || op.reg == REG_IX) {
                    return op.setErrorIf(idxp, REGISTER_NOT_ALLOWED);
                } else {
                    return op.setErrorIf(regp, REGISTER_NOT_ALLOWED);
                }
                p = idxp;
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
    if (ixBit())
        return 2;
    const auto opcodeLen = !hasPrefix() ? 1 : (prefix() < 0x100 ? 2 : 3);
    const auto pos = length();
    return pos < opcodeLen ? opcodeLen : pos;
}

Error AsmZ80::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.dstOp, insn) && insn.dstOp.hasError())
        return _insn.setError(insn.dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.srcOp, insn) && insn.srcOp.hasError())
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
