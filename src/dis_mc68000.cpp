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

#include "dis_mc68000.h"
#include <math.h>
#include "reg_mc68000.h"
#include "table_mc68000.h"
#include "text_common.h"

namespace libasm {
namespace mc68000 {

using namespace reg;
using namespace text::common;

const ValueFormatter::Plugins &DisMc68000::defaultPlugins() {
    return ValueFormatter::Plugins::motorola();
}

DisMc68000::DisMc68000(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins), Config(TABLE) {
    reset();
}

namespace {

StrBuffer &outOprSize(StrBuffer &out, OprSize size) {
    const auto suffix = sizeSuffix(size);
    if (suffix)
        out.letter('.').letter(suffix);
    return out;
}

}  // namespace

void DisMc68000::decodeImmediateData(DisInsn &insn, StrBuffer &out, OprSize size) const {
#if defined(LIBASM_DIS_NOFLOAT) && !defined(LIBASM_MC68000_NOFPU)
    const auto *at = out.mark();
#endif
    out.letter('#');
    if (size == SZ_BYTE || insn.src() == M_CCR || insn.dst() == M_CCR) {
        outHex(out, insn.readUint16() & 0xFF, 8);
    } else if (size == SZ_WORD) {
        outHex(out, insn.readUint16(), 16);
    } else if (size == SZ_LONG) {
        outHex(out, insn.readUint32(), 32);
#if !defined(LIBASM_MC68000_NOFPU)
    } else if (_gnuAs) {
        outHex(out, insn.readUint32(), 32, false);
        if (size == SZ_DUBL) {
            out.hex(insn.readUint32(), 8);
        } else if (size == SZ_XTND || size == SZ_PBCD) {
            out.hex(insn.readUint64(), 16);
        }
    } else {
#if defined(LIBASM_DIS_NOFLOAT)
        insn.setErrorIf(at, FLOAT_NOT_SUPPORTED);
#endif
        if (size == SZ_SNGL) {
#if defined(LIBASM_DIS_NOFLOAT)
            outHex(out, insn.readUint32(), 32, false);
#else
            out.float32(insn.readFloat32Be());
#endif
        } else if (size == SZ_DUBL) {
#if defined(LIBASM_DIS_NOFLOAT)
            outHex(out, insn.readUint32(), 32, false);
            out.hex(insn.readUint32(), 8);
#else
            out.float64(insn.readFloat64Be());
#endif
        } else if (size == SZ_XTND) {
            outExtendedReal(out, insn.readExtendedReal());
        } else if (size == SZ_PBCD) {
            outDecimalString(out, insn.readDecimalString());
        }
#endif
    }
}

#if !defined(LIBASM_MC68000_NOFPU)

bool ExtendedReal::isValid() const {
    /**
     * Extended Binary Real
     * |s|15-bit exponent|16-bit zero|j|63-bit significand|
     *  s - sign of significand
     *  j - integer part of significand
     */
    const auto exp = tag & 0x7FFF;
    if (exp != 0x7FFF) {
        // Zero, Normalized, and Denormalized
        const auto j = static_cast<uint_fast8_t>(sig >> 63);
        return (exp == 0 && j == 0) || (exp != 0 && j != 0);
    }
    return true;
}

ExtendedReal DisInsn::readExtendedReal() {
    const auto tag = readUint16();
    const auto pad = readUint16();  // don't care for input
    const auto sig = readUint64();
    const ExtendedReal value{tag, pad, sig};
    if (!value.isValid())
        setErrorIf(_out, ILLEGAL_CONSTANT);
    return value;
}

StrBuffer &DisMc68000::outExtendedReal(StrBuffer &out, const ExtendedReal &v) const {
#if defined(LIBASM_DIS_NOFLOAT)
    outHex(out, v.tag, 16, false);
    out.hex(v.pad, 4);
    return out.hex(v.sig, 16);
#else
    if (v.tag & 0x8000)
        out.letter('-');
    const auto bexp = v.tag & 0x7FFF;
    if (!v.isValid())
        return out.text_P(TEXT_NAN);
    if (bexp == 0x7FFF) {
        // Inf or Nan, don't care bit j.
        const auto inf = (v.sig << 1) == 0;
        return out.text_P(inf ? TEXT_INF : TEXT_NAN);
    }
    const auto exp = static_cast<int16_t>(bexp) - 0x3FFF;
    fixed64_t sig{v.sig};
    return out.float80(float80_t::compose(false, exp, sig));
#endif
}

bool DecimalString::isValid() const {
    /**
     * Decimal String Type
     * |s|e|in|3-digit exponent|12-bit zero|1-digit integer|16-digit significand|
     *  s - sign of significand
     *  e - sign of exponent
     *  in - 11: infinity or nan, exp=FFF
     */
    if ((tag & 0x7FFF) != 0x7FFF) {
        for (auto bcd = tag & 0xFFF; bcd; bcd >>= 4) {
            if ((bcd & 0xF) >= 10)
                return false;
        }
        if (integ >= 10)
            return false;
        for (auto bcd = sig; bcd; bcd >>= 4) {
            if ((bcd & 0xF) >= 10)
                return false;
        }
    }
    return true;
}

DecimalString DisInsn::readDecimalString() {
    const auto tag = readUint16();
    const auto integ = readUint16();
    const auto sig = readUint64();
    const DecimalString value{tag, integ, sig};
    if (!value.isValid())
        setErrorIf(_out, ILLEGAL_CONSTANT);
    return DecimalString{tag, integ, sig};
}

StrBuffer &DisMc68000::outDecimalString(StrBuffer &out, const DecimalString &v) const {
#if defined(LIBASM_DIS_NOFLOAT)
    outHex(out, v.tag, 16, false);
    out.hex(v.integ, 4);
    return out.hex(v.sig, 16);
#else
    if (v.tag & 0x8000)
        out.letter('-');
    if ((v.tag & 0x7FFF) == 0x7FFF)
        return out.text_P(v.sig == 0 ? TEXT_INF : TEXT_NAN);
    fixed64_t sig;
    // nondecimal digit is ignored and converted as is.
    int_fast16_t exp = sig.dec_digit(0, v.integ & 0xF);
    auto sigbcd = v.sig;
    for (int_fast8_t i = 0; i < 16; ++i) {
        const auto digit = static_cast<uint_fast8_t>(sigbcd >> 60);
        sigbcd <<= 4;
        exp = sig.dec_digit(exp, digit);
    }
    int_fast16_t exp10 = 0;
    uint16_t expbcd = v.tag << 4;
    for (auto i = 0; i < 3; ++i) {
        const auto digit = expbcd >> 12;
        expbcd <<= 4;
        exp10 *= 10;
        exp10 += digit;
    }
    if (v.tag & 0x4000)
        exp10 = -exp10;
    exp += sig.power10(exp10 - 16);
    return out.float80(float80_t::compose(false, exp, sig), 17);
#endif
}

#endif

void DisMc68000::decodeEffectiveAddr(
        DisInsn &insn, StrBuffer &out, uint8_t mode, RegName reg, OprSize size) const {
    if (mode == M_DREG || mode == M_AREG) {
        outRegName(out, reg);
        return;
    }
    if (mode == M_IMDAT) {
        decodeImmediateData(insn, out, size);
        return;
    }

    if (mode == M_PDEC)
        out.letter('-');
    if (mode == M_DISP || mode == M_PCDSP) {
        const auto base = (mode == M_DISP) ? reg : REG_PC;
        const auto val16 = insn.readUint16();
        if (mode == M_PCDSP) {
            const Config::uintptr_t target =
                    insn.address() + insn.length() - 2 + static_cast<int16_t>(val16);
            if (size != SZ_BYTE && (target % 2) != 0)
                insn.setErrorIf(out, OPERAND_NOT_ALIGNED);
            outRelAddr(out.letter('('), target, insn.address(), 16);
        } else {
            outHex(out.letter('('), val16, -16);
        }
        out.letter(',');
        outRegName(out, base);
    } else if (mode == M_AIND || mode == M_PINC || mode == M_PDEC) {
        outRegName(out.letter('('), reg);
    } else if (mode == M_AWORD || mode == M_ALONG) {
        const Config::uintptr_t target =
                (mode == M_AWORD) ? static_cast<int16_t>(insn.readUint16()) : insn.readUint32();
        if (size != SZ_BYTE && (target % 2) != 0)
            insn.setErrorIf(out, OPERAND_NOT_ALIGNED);
        outAbsAddr(out.letter('('), target, _gnuAs ? 16 : 0);
    } else if (mode == M_INDX || mode == M_PCIDX) {
        const auto base = (mode == M_INDX) ? reg : REG_PC;
        BriefExt ext;
        ext.word = insn.readUint16();
        const auto val8 = ext.disp();
        out.letter('(');
        if (mode == M_PCIDX) {
            const Config::uintptr_t target =
                    insn.address() + insn.length() - 2 + static_cast<int8_t>(val8);
            outRelAddr(out, target, insn.address(), 8);
        } else {
            outHex(out, val8, -8);
        }
        out.letter(',');
        outRegName(out, base).letter(',');
        outRegName(out, ext.index());
        outOprSize(out, ext.indexSize());
    }
    out.letter(')');
    if (mode == M_AWORD)
        outOprSize(out, SZ_WORD);
    if (mode == M_ALONG)
        outOprSize(out, SZ_LONG);
    if (mode == M_PINC)
        out.letter('+');
}

void DisMc68000::decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    // FDxx has different base address
    const auto base = insn.address() + (insn.hasPostVal() ? 4 : 2);
    if (mode == M_REL32 && (insn.opCode() & (1 << 6)) == 0)
        mode = M_REL16;
    if (mode == M_REL32) {
        const auto delta = static_cast<int32_t>(insn.readUint32());
        auto save{out};
        insn.nameBuffer().over(out);
        if (!overflowDelta(delta, 16)) {
            if (!_gnuAs)
                out.letter('.').letter('L');
        }
        if (_gnuAs)
            out.letter('L');
        out.over(insn.nameBuffer());
        save.over(out);
        const auto target = base + delta;
        insn.setErrorIf(out, checkAddr(target, true));
        outRelAddr(out, target, insn.address(), insn.isOK() ? addressWidth() : 32);
        return;
    }
    const auto rel8 = (mode == M_REL8) ? static_cast<int8_t>(insn.opCode() & 0xFF) : 0;
    const auto delta = rel8 ? static_cast<int8_t>(rel8) : static_cast<int16_t>(insn.readUint16());
    if (mode == M_REL8) {
        auto save{out};
        insn.nameBuffer().over(out);
        if (_gnuAs) {
            if (rel8)
                out.letter('S');
        } else {
            if (rel8 == 0 && !overflowDelta(delta, 8))
                out.letter('.').letter('W');
        }
        out.over(insn.nameBuffer());
        save.over(out);
    }
    const auto target = base + delta;
    insn.setErrorIf(out, checkAddr(target, true));
    outRelAddr(out, target, insn.address(), rel8 ? 8 : 16);
}

namespace {

RegName decodeMoveMltReg(int8_t regno) {
    return (regno < 8) ? decodeDataReg(regno) : decodeAddrReg(regno - 8);
}

StrBuffer &outMoveMltRegs(StrBuffer &out, RegName start, RegName last, char suffix) {
    outRegName(out, start);
    if (start != last)
        outRegName(out.letter('-'), last);
    if (suffix)
        out.letter(suffix);
    return out;
}

StrBuffer &outMoveMltRegList(StrBuffer &out, uint16_t list, DisInsn &insn) {
    const auto push = AddrMode((insn.opCode() >> 3) & 7) == M_PDEC;
    if (list == 0)
        insn.setErrorIf(out, OPCODE_HAS_NO_EFFECT);
    int8_t start = -1;
    int8_t last = 0;
    uint16_t mask = push ? 0x8000 : 0x0001;
    for (auto i = 0; i < 16; i++) {
        if (list & mask) {
            if (start < 0) {
                start = last = i;
            } else if (i == last + 1) {
                last = i;
            } else {
                outMoveMltRegs(out, decodeMoveMltReg(start), decodeMoveMltReg(last), '/');
                start = last = i;
            }
        }
        if (push)
            mask >>= 1;
        else
            mask <<= 1;
    }
    if (start >= 0)
        outMoveMltRegs(out, decodeMoveMltReg(start), decodeMoveMltReg(last), 0);
    return out;
}

StrBuffer &outFmoveMltRegs(StrBuffer &out, int8_t start, int8_t last, char suffix) {
    outRegName(out, RegName(start + REG_FP0));
    if (start != last)
        outRegName(out.letter('-'), RegName(last + REG_FP0));
    if (suffix)
        out.letter(suffix);
    return out;
}

StrBuffer &outFmoveMltRegList(StrBuffer &out, DisInsn &insn) {
    const auto post = insn.postfix();
    const auto push = (post & 0x3800) == 0x2000;
    const auto list = post & 0xFF;
    if (list == 0)
        insn.setErrorIf(out, OPCODE_HAS_NO_EFFECT);
    int8_t start = -1;
    int8_t last = 0;
    uint8_t mask = push ? 0x01 : 0x80;
    for (auto i = 0; i < 8; i++) {
        if (list & mask) {
            if (start < 0) {
                start = last = i;
            } else if (i == last + 1) {
                last = i;
            } else {
                outFmoveMltRegs(out, start, last, '/');
                start = last = i;
            }
        }
        if (push)
            mask <<= 1;
        else
            mask >>= 1;
    }
    if (start >= 0)
        outFmoveMltRegs(out, start, last, 0);
    return out;
}

bool needsFmoveXtendCheck(AddrMode mode, OprPos pos) {
    return mode == M_FPMLT || pos == EX_DL;
}

bool needsFmoveLongCheck(AddrMode mode) {
    return mode == M_FCMLT || mode == M_FPCR || mode == M_FPSR || mode == M_FPIAR;
}

void checkFmovemMode(DisInsn &insn, const EaMc68000 &ea, StrBuffer &out) {
    const auto src = insn.src();
    const auto dst = insn.dst();
    if (needsFmoveXtendCheck(src, insn.srcPos()) || needsFmoveXtendCheck(dst, insn.dstPos())) {
        const auto decrement = (insn.postfix() & 0x1000) == 0;
        if (decrement && ea.mode != M_PDEC)
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
        if (!decrement && ea.mode == M_PDEC)
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
    } else if (needsFmoveLongCheck(src) || needsFmoveLongCheck(dst)) {
        const auto list = (insn.postfix() >> 10) & 7;
        const auto single = (list == 4 || list == 2 || list == 1);
        // Single FPCR/FPSR/FPIAR can move to Dn
        if (ea.mode == M_DREG && !single)
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
        // Single FPIAR can move to An
        if (ea.mode == M_AREG && list != 1)
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
        // Immediate can move to a single register
        if (ea.mode == M_IMDAT && !single)
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
    }
}

StrBuffer &outFmoveControlRegList(StrBuffer &out, DisInsn &insn) {
    const auto list = (insn.postfix() >> 10) & 7;
    if (list == 0)
        insn.setErrorIf(out, OPCODE_HAS_NO_EFFECT);
    if (list & 4)
        outRegName(out, REG_FPCR);
    if (list & 2) {
        if (list & 4)
            out.letter('/');
        outRegName(out, REG_FPSR);
    }
    if (list & 1) {
        if (list & 6)
            out.letter('/');
        outRegName(out, REG_FPIAR);
    }
    return out;
}

uint8_t modeVal(const DisInsn &insn, OprPos pos) {
    const auto opc = insn.opCode();
    switch (pos) {
    case OP_10:
        return (opc >> 3) & 7;
    case OP_23:
        return (opc >> 6) & 7;
    default:
        return 0;
    }
}

uint8_t regVal(const DisInsn &insn, OprPos pos) {
    const auto opc = insn.opCode();
    const auto post = insn.postfix();
    switch (pos) {
    case OP_10:
    case OP__0:
        return (opc >> 0) & 7;
    case OP_23:
    case OP__3:
        return (opc >> 9) & 7;
    case EX_RX:
        return (post >> 10) & 7;
    case EX_RY:
        return (post >> 7) & 7;
    case EX_SC:
        return ((post & 0x380) >> 3) | (post & 7);
    case EX_DL:
    case EX_DK:
        return (post >> 4) & 7;
    case EX_RR:
        return (post >> 12) & 7;
    default:
        return 0;
    }
}

bool isFloatOpOnSameFpreg(const DisInsn &insn) {
    const auto postVal = insn.postVal();
    const auto may1opr = insn.hasPostVal() && postVal >= 0x0001 && postVal < 0x0020;
    return may1opr && insn.src() == M_FPREG && insn.dst() == M_FPREG &&
           regVal(insn, insn.srcPos()) == regVal(insn, insn.dstPos());
}

bool inFloatOperand(const DisInsn &insn, AddrMode mode) {
    if (mode == M_RDATA) {
        const auto dst = insn.dst();
        // Dn,FPn || Dn,FPc:FPs || FTST Dn
        return dst == M_FPREG || dst == M_FSICO || dst == M_NONE;
    }
    if (mode == M_WDATA || mode == M_KFACT || mode == M_KDREG) {
        // FPn,Dn
        return insn.src() == M_FPREG;
    }
    return false;
}

}  // namespace

void DisMc68000::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos,
        OprSize size, uint16_t opr16, Error opr16Error) const {
    if (mode == M_KDREG || mode == M_KFACT)
        pos = OP_10;
    auto m = modeVal(insn, pos);
    auto r = regVal(insn, pos);
    EaMc68000 ea(size, m, r);
    if (ea.mode == M_DREG && inFloatOperand(insn, mode)) {
        if (size == SZ_DUBL || size == SZ_XTND || size == SZ_PBCD)
            insn.setErrorIf(out, ILLEGAL_SIZE);
    }
    RegName reg;
    switch (mode) {
    case M_AREG:
    case M_PDEC:
    case M_PINC:
    case M_DISP:
        decodeEffectiveAddr(insn, out, mode, decodeAddrReg(r), size);
        break;
    case M_DADDR:
    case M_IADDR:
    case M_RADDR:
    case M_WADDR:
        checkFmovemMode(insn, ea, out);
        /* Fall-through */
    case M_JADDR:
    case M_WDATA:
    case M_WMEM:
    case M_RDATA:
    case M_RMEM:
    case M_KDREG:
    case M_KFACT:
        decodeEffectiveAddr(insn, out, ea.mode, ea.reg, size);
        if (insn.getError())
            break;
        if (mode == M_KDREG) {
            const auto dreg = decodeDataReg((insn.postfix() >> 4) & 7);
            outRegName(out.letter('{'), dreg).letter('}');
        } else if (mode == M_KFACT) {
            const auto kfactor = insn.postfix() & 0x7F;
            outDec(out.letter('{').letter('#'), kfactor, -7).letter('}');
        }
        break;
    case M_DREG:
        decodeEffectiveAddr(insn, out, M_DREG, decodeDataReg(r), size);
        break;
    case M_IM3:
        r = (insn.opCode() >> 9) & 7;
        if (r == 0)
            r = 8;
        outDec(out.letter('#'), r, 4);
        break;
    case M_IM8:
        outHex(out.letter('#'), insn.opCode(), -8);
        break;
    case M_IMBIT:
        out.letter('#');
        if (insn.src() == M_IMBIT)
            opr16 = insn.readUint16();
        if (size == SZ_BYTE && opr16 >= 8) {
            insn.setErrorIf(out, ILLEGAL_BIT_NUMBER);
        } else if (size == SZ_WORD && opr16 >= 16) {
            insn.setErrorIf(out, ILLEGAL_BIT_NUMBER);
        } else if (size == SZ_LONG && opr16 >= 32) {
            insn.setErrorIf(out, ILLEGAL_BIT_NUMBER);
        }
        outDec(out, opr16, 16);
        break;
    case M_IMDAT:
        decodeImmediateData(insn, out, size);
        break;
    case M_IMVEC:
        outDec(out.letter('#'), insn.opCode() & 0xF, 4);
        break;
    case M_IMDSP:
        out.letter('#');
        if (opr16Error == NO_MEMORY)
            insn.setError(out, opr16Error);
        outHex(out, opr16, -16);
        break;
    case M_MULT:
        if (insn.src() == M_MULT) {
            opr16 = insn.readUint16();
        } else if (opr16Error == NO_MEMORY) {
            insn.setError(out, opr16Error);
        }
        outMoveMltRegList(out, opr16, insn);
        break;
    case M_FPMLT:
        outFmoveMltRegList(out, insn);
        break;
    case M_FCMLT:
        outFmoveControlRegList(out, insn);
        break;
    case M_NONE:
        break;
    case M_SR:
        outRegName(out, REG_SR);
        break;
    case M_CCR:
        outRegName(out, REG_CCR);
        break;
    case M_USP:
        outRegName(out, REG_USP);
        break;
    case M_REL8:
    case M_REL16:
    case M_REL32:
        decodeRelative(insn, out, mode);
        break;
    case M_FPREG:
        outRegName(out, RegName(REG_FP0 + r));
        break;
    case M_FSICO:
        outRegName(out, RegName(REG_FP0 + (r & 7)));
        outRegName(out.letter(':'), RegName(REG_FP0 + (r >> 4)));
        break;
    case M_FPCR:
        outRegName(out, REG_FPCR);
        break;
    case M_FPSR:
        outRegName(out, REG_FPSR);
        break;
    case M_FPIAR:
        outRegName(out, REG_FPIAR);
        break;
    case M_IMROM:
        outHex(out.letter('#'), insn.postfix() & 0x7F, 7, false);
        break;
    case M_CREG:
        if ((reg = decodeControlReg(insn.postfix())) == REG_UNDEF)
            insn.setErrorIf(out, ILLEGAL_REGISTER);
        outRegName(out, reg);
    default:
        break;
    }
}

OprSize decodeSize(DisInsn &insn) {
    const auto osize = insn.oprSize();
    const auto opc = insn.opCode();
    if (osize == SZ_DATA) {
        static constexpr OprSize SIZES[] PROGMEM = {
                SZ_BYTE,
                SZ_WORD,
                SZ_LONG,
                SZ_WORD,
        };
        return pgm_read_byte(&SIZES[(opc >> 6) & 3]);
    }
    if (osize == SZ_FDAT) {
        static constexpr OprSize SIZES[] PROGMEM = {
                SZ_LONG,
                SZ_SNGL,
                SZ_XTND,
                SZ_PBCD,
                SZ_WORD,
                SZ_DUBL,
                SZ_BYTE,
                SZ_NONE,
        };
        const auto fmt = (insn.postfix() >> 10) & 7;
        if (fmt == 7)
            insn.setErrorIf(ILLEGAL_SIZE);
        return pgm_read_byte(&SIZES[fmt]);
    }
    static constexpr OprSize SIZES[] PROGMEM = {
            SZ_WORD,
            SZ_LONG,
    };
    if (osize == SZ_ADDR)
        return pgm_read_byte(&SIZES[(opc >> 6) & 1]);
    if (osize == SZ_ADR8)
        return pgm_read_byte(&SIZES[(opc >> 8) & 1]);
    return osize;
}

char decodeInsnSize(const DisInsn &insn, OprSize osize) {
    const auto isize = insn.insnSize();
    const auto size =
            (isize == ISZ_DATA || isize == ISZ_FIXD || isize == ISZ_FDAT) ? osize : OprSize(isize);
    return sizeSuffix(size);
}

Error DisMc68000::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readUint16();
    insn.setOpCode(opc);
    if (searchOpCode(_cpuSpec, insn, out))
        return _insn.setError(insn);

    const auto osize = decodeSize(insn);
    const auto suffix = decodeInsnSize(insn, osize);
    if (suffix) {
        auto save{out};
        insn.nameBuffer().over(out);
        if (!_gnuAs)
            out.letter('.');
        out.letter(suffix).over(insn.nameBuffer());
        save.over(out);
    }

    const auto dst = insn.dst();
    const auto opr16 = (dst == M_MULT || dst == M_IMDSP) ? insn.readUint16() : 0;
    const auto opr16Error = insn.getError();

    decodeOperand(insn, out, insn.src(), insn.srcPos(), osize);
    if (dst != M_NONE && !isFloatOpOnSameFpreg(insn))
        decodeOperand(insn, out.comma(), dst, insn.dstPos(), osize, opr16, opr16Error);
    return _insn.setError(insn);
}

}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
