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

#include "asm_z380.h"
#include "table_z380.h"
#include "text_common.h"

namespace libasm {
namespace z380 {

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

const ValueParser::Plugins &AsmZ380::defaultPlugins() {
    static const struct final : ValueParser::IntelPlugins {
        const OperatorParser &operators() const override {
            return ZilogOperatorParser::singleton();
        }
    } PLUGINS{};
    return PLUGINS;
}

AsmZ380::AsmZ380(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_extmode),
      Config(TABLE),
      _opt_extmode(
              this, &AsmZ380::setExtendedMode, OPT_BOOL_EXTMODE, OPT_DESC_EXTMODE, &_opt_lwordmode),
      _opt_lwordmode(this, &AsmZ380::setLongWordMode, OPT_BOOL_LWORDMODE, OPT_DESC_LWORDMODE) {
    reset();
}

void AsmZ380::reset() {
    Assembler::reset();
    setExtendedMode(false);
    setLongWordMode(false);
}

bool AsmZ380::wordMode() const {
    return (!_lwordmode && !_ddir.lwordMode()) || (_lwordmode && _ddir.wordMode());
}

bool AsmZ380::lwordMode() const {
    return (_lwordmode && !_ddir.wordMode()) || (!_lwordmode && _ddir.lwordMode());
}

int32_t AsmZ380::calcDeltaZ380(
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

void AsmZ380::encodeRelative(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    const auto deltaLen = (mode == M_REL8) ? 1 : 2;
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

void AsmZ380::encodeAbsolute(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    if (mode == M_XABS || mode == M_JABS) {
        if (_extmode) {
            if (_ddir) {
                if (_ddir.noImmediate()) {
                    insn.setError(op, PREFIX_HAS_NO_EFFECT);
                } else if (!_ddir.noMode()) {
                    insn.setError(op, SUBOPTIMAL_INSTRUCTION);
                }
            }
        } else {
            if (_ddir)
                insn.setErrorIf(op, PREFIX_HAS_NO_EFFECT);
            mode = M_ABS;
        }
    }
    if (mode != M_ABS || mode == M_IO16) {
        auto fixup = DD_UNDEF;
        if (!_ddir) {
        do_fixup:
            if (op.val.overflow(UINT32_C(0xFFFFFF))) {
                if (insn.ddir.setMode(fixup)) {
                    insn.resetAddress(_ddir.addr());
                    _ddir.clear();
                }
                insn.ddir.setImmediate(DD_IW);
            emit32:
                insn.emitOperand32(op.val.getUnsigned());
                return;
            }
            if (op.val.overflow(UINT16_MAX)) {
                if (insn.ddir.setMode(fixup)) {
                    insn.resetAddress(_ddir.addr());
                    _ddir.clear();
                }
                insn.ddir.setImmediate(DD_IB);
            emit24:
                insn.emitOperand24(op.val.getUnsigned());
                return;
            }
        } else {
            if (_ddir.byteImmediate()) {
                if (op.val.overflow(UINT32_C(0xFFFFFF)))
                    insn.setErrorIf(op, OVERFLOW_RANGE);
                goto emit24;
            } else if (_ddir.wordImmediate()) {
                if (op.val.overflowUint32())
                    insn.setErrorIf(op, OVERFLOW_RANGE);
                goto emit32;
            } else if (insn.lmCapable()) {
                fixup = _ddir.lwordMode() ? DD_LW : DD_W;
                goto do_fixup;
            } else if (_ddir.noImmediate()) {
                if (insn.imCapable())
                    insn.setErrorIf(op, PREFIX_HAS_NO_EFFECT);
            }
        }
    }
    if (op.val.overflow(UINT16_MAX))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.emitOperand16(op.val.getUnsigned());
}

namespace {
void encodeIndexReg(AsmInsn &insn, RegName ixReg) {
    const auto ix = Entry::encodeIndex(ixReg);
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

void AsmZ380::encodeShortIndex(AsmInsn &insn, const Operand &op) const {
    if (z380()) {
        auto fixup = DD_UNDEF;
        if (!_ddir) {
        do_fixup:
            if (op.val.overflow(UINT32_C(0x7FFFFF), INT32_C(-0x800000))) {
                insn.setErrorIf(op, OVERFLOW_RANGE);
                insn.emitOperand8(op.val.getUnsigned());
                return;
            }
            if (op.val.overflowInt16()) {
                if (insn.ddir.setMode(fixup)) {
                    insn.resetAddress(_ddir.addr());
                    _ddir.clear();
                }
                insn.ddir.setImmediate(DD_IW);
            disp24:
                insn.emitOperand24(op.val.getSigned());
                return;
            }
            if (op.val.overflowInt8()) {
                if (insn.ddir.setMode(fixup)) {
                    insn.resetAddress(_ddir.addr());
                    _ddir.clear();
                }
                insn.ddir.setImmediate(DD_IB);
            disp16:
                insn.emitOperand16(op.val.getSigned());
                return;
            }
        } else {
            if (_ddir.byteImmediate()) {
                if (op.val.overflowInt16())
                    insn.setErrorIf(op, OVERFLOW_RANGE);
                goto disp16;
            } else if (_ddir.wordImmediate()) {
                if (op.val.overflow(UINT32_C(0x7FFFFF), INT32_C(-0x800000)))
                    insn.setErrorIf(op, OVERFLOW_RANGE);
                goto disp24;
            } else if (insn.lmCapable()) {
                fixup = _ddir.lwordMode() ? DD_LW : DD_W;
                goto do_fixup;
            }
        }
    }
    if (op.val.overflowInt8())
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.emitOperand8(op.val.getUnsigned());
}

void AsmZ380::encodeImmediate16(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    if (mode == M_XM16) {
        encodeAbsolute(insn, op, M_XABS);
        return;
    }
    if (mode == M_LM16) {
        if (!z380()) {
            mode = M_IM16;
        } else if (!lwordMode()) {
            if (_ddir)
                insn.setErrorIf(op, PREFIX_HAS_NO_EFFECT);
            mode = M_IM16;
        }
    }
    if (mode != M_IM16) {
        auto fixup = DD_UNDEF;
        if (!_ddir) {
        do_fixup:
            if (op.val.overflow(UINT32_C(0xFFFFFF), INT32_C(-0x800000))) {
                if (mode == M_LM16 && !lwordMode())
                    insn.ddir.setMode(DD_LW);
                if (insn.ddir.setMode(fixup)) {
                    insn.resetAddress(_ddir.addr());
                    _ddir.clear();
                }
                insn.ddir.setImmediate(DD_IW);
            emit32:
                insn.emitOperand32(op.val.getUnsigned());
                return;
            }
            if (op.val.overflow(UINT16_MAX, INT16_MIN)) {
                if (mode == M_LM16 && !lwordMode())
                    insn.ddir.setMode(DD_LW);
                if (insn.ddir.setMode(fixup)) {
                    insn.resetAddress(_ddir.addr());
                    _ddir.clear();
                }
                insn.ddir.setImmediate(DD_IB);
            emit24:
                insn.emitOperand24(op.val.getUnsigned());
                return;
            }
        } else {
            if (_ddir.byteImmediate()) {
                if (op.val.overflow(UINT32_C(0xFFFFFF), INT32_C(-0x800000)))
                    insn.setErrorIf(op, OVERFLOW_RANGE);
                goto emit24;
            } else if (_ddir.wordImmediate()) {
                if (op.val.overflowUint32())
                    insn.setErrorIf(op, OVERFLOW_RANGE);
                goto emit32;
            } else if (insn.lmCapable()) {
                fixup = _ddir.lwordMode() ? DD_LW : DD_W;
                goto do_fixup;
            }
        }
    }
    if (op.val.overflowUint16())
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.emitOperand16(op.val.getUnsigned());
}

void AsmZ380::encodeOperand(
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
        encodeAbsolute(insn, op, mode);
        break;
    case M_SPX:
    case M_IDX:
    case M_IDX8:
        encodeShortIndex(insn, op);
        /* Fall-through */
    case R_IDX:
    case I_IDX:
        if (mode != M_SPX)
            encodeIndexReg(insn, op.reg);
        break;
    case R_IDXL:
        insn.embed(op.reg == REG_IX ? 0 : 1);
        break;
    case R_DXY:
    case R_SXY:
        if (op.reg == REG_IXL || op.reg == REG_IYL)
            insn.embed(mode == R_DXY ? 8 : 1);
        encodeIndexReg(insn, (op.reg == REG_IXL || op.reg == REG_IXU) ? REG_IX : REG_IY);
        break;
    case R_PTRL:
    case I_PTRL:
        insn.embed(op.reg == REG_BC ? 0 : 1);
        break;
    case R_PTRH:
        insn.embed(op.reg == REG_BC ? 0x00 : 0x10);
        break;
    case M_IM16:
    case M_DM16:
    case M_XM16:
    case M_LM16:
        encodeImmediate16(insn, op, mode);
        break;
    case M_ABS:
    case M_JABS:
    case M_DABS:
    case M_XABS:
        encodeAbsolute(insn, op, mode);
        break;
    case M_REL8:
    case M_REL16:
        encodeRelative(insn, op, mode);
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
    case M_SR8X:
        if (op.reg == REG_H || op.reg == REG_L)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        // Fall-through
    case M_SR8:
    case M_SRC:
        insn.embed(encodeDataReg(op.reg));
        break;
    case M_DR8X:
        if (op.reg == REG_H || op.reg == REG_L)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        // Fall-through
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
            if (z380()) {
                insn.embed(1 << 3);
                break;
            }
            // Fall-through
        default:
            insn.setErrorIf(op, ILLEGAL_OPERAND);
            break;
        }
        break;
    case M_LW:
    case M_LCK:
    case M_XM:
        insn.setPrefix(encodeCtlName(CtlName(op.mode - M_LW + CTL_LW)));
        break;
    case M_DD:
        insn.setErrorIf(op, insn.embedDd(DdName(op.val.getUnsigned())));
        break;
    default:
        break;
    }
}

Error AsmZ380::parseOperand(StrScanner &scan, Operand &op) const {
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
        if (op.mode == M_LW)
            op.val.setUnsigned(DD_LW);
        scan = p;
        return OK;
    }

    const auto ddir = parseDdName(p, parser());
    if (ddir != DD_UNDEF) {
        op.mode = M_DD;
        op.val.setUnsigned(ddir);
        scan = p;
        return OK;
    }

    op.reg = parseRegName(p, parser());
    if (op.reg != REG_UNDEF) {
        switch (op.reg) {
        case REG_IXU:
        case REG_IYU:
        case REG_IXL:
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
            } else if (op.reg == REG_SP) {
                op.mode = M_SPX;
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

Error AsmInsn::fixup(const Ddir &ddir) {
    if (!lmCapable() && !imCapable())
        return PREFIX_HAS_NO_EFFECT;
    if (lmCapable() && !imCapable() && ddir.noMode())
        return PREFIX_HAS_NO_EFFECT;
    return OK;
}

Error AsmInsn::embedDd(DdName dd) {
    if (dd == DD_IB || dd == DD_IW) {
        if (_dd & 0x03)
            return ILLEGAL_COMBINATION;
        _dd |= dd;
    } else if (_dd & 0xC0) {
        return ILLEGAL_COMBINATION;
    } else if (dd == DD_LW) {
        setPrefix(0xFD);
        _dd |= 0xC0;
    } else if (dd == DD_W) {
        setPrefix(0xDD);
        _dd |= 0xC0;
    }
    return OK;
}

void AsmInsn::fixupDd() {
    if (_dd) {
        if ((_dd & 0xC0) == 0) {
            setPrefix((_dd == DD_IB) ? 0xDD : 0xFD);
            _dd = 0xC3;  // DDIR IB/DDIR IW
        }
        setOpCode(_dd);
    }
}

void AsmInsn::emitInsn() {
    fixupDd();  // encoding DDIR operands
    uint_fast8_t pos = 0;
    if (ddir) {
        emitByte(ddir.prefix(), pos++);
        emitByte(ddir.opc(), pos++);
    }
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
    uint_fast8_t len = ddir ? 2 : 0;
    if (ixBit())
        return len + 2;
    len += !hasPrefix() ? 1 : (prefix() < 0x100 ? 2 : 3);
    const auto pos = length();
    return pos < len ? len : pos;
}

Error AsmZ380::processPseudo(StrScanner &scan, Insn &insn) {
    const auto at = scan;
    if (z380()) {
        if (strcasecmp_P(insn.name(), OPT_BOOL_EXTMODE) == 0) {
            const auto error = _opt_extmode.set(scan);
            return error ? insn.setErrorIf(at, error) : OK;
        }
        if (strcasecmp_P(insn.name(), OPT_BOOL_LWORDMODE) == 0) {
            const auto error = _opt_lwordmode.set(scan);
            return error ? insn.setErrorIf(at, error) : OK;
        }
    }
    return Assembler::processPseudo(scan, insn);
}

void Ddir::operator=(const AsmInsn &insn) {
    _prefix = insn.prefix();
    _opc = insn.opCode();
    _addr = insn.address();
}

void Ddir::setImmediate(DdName dd) {
    if (_prefix == 0) {
        _prefix = (dd == DD_IB) ? 0xDD : 0xFD;
        _opc = 0xC3;
    } else {
        _opc |= dd;
    }
}

bool Ddir::setMode(DdName dd) {
    if (dd != DD_UNDEF && _prefix == 0) {
        _prefix = (dd == DD_W) ? 0xDD : 0xFD;
        _opc = 0xC0;
    }
    return dd != DD_UNDEF;
}

Error AsmZ380::encodeImpl(StrScanner &scan, Insn &_insn) const {
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

    if (_ddir) {
        insn.setErrorIf(_insn.errorAt(), insn.fixup(_ddir));
        if (insn.lmCapable() && !_ddir.noMode() && !_ddir.noImmediate() && !insn.imCapable()) {
            insn.setErrorIf(_insn.errorAt(), SUBOPTIMAL_INSTRUCTION);
            insn.resetAddress(_ddir.addr());
            insn.ddir.setMode(_ddir.lwordMode() ? DD_LW : DD_W);
            _ddir.clear();
        }
    }
    encodeOperand(insn, insn.dstOp, insn.dst(), insn.srcOp);
    encodeOperand(insn, insn.srcOp, insn.src(), insn.dstOp);
    insn.emitInsn();
    if (insn.dst() == M_DD && insn.isOK()) {
        _ddir = insn;
    } else {
        _ddir.clear();
    }
    return _insn.setError(insn);
}

}  // namespace z380
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
