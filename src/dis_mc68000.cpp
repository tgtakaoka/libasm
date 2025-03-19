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
#include "text_mc68000.h"

namespace libasm {
namespace mc68000 {

using namespace reg;
using namespace text::common;
using namespace text::option;
using namespace text::mc68000;

const ValueFormatter::Plugins &DisMc68000::defaultPlugins() {
    return ValueFormatter::Plugins::motorola();
}

DisMc68000::DisMc68000(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_fpu),
      Config(TABLE),
      _opt_fpu(this, &Config::setFpuName, OPT_TEXT_FPU, OPT_DESC_FPU) {
    reset();
}

void DisMc68000::reset() {
    Disassembler::reset();
#if defined(LIBASM_MC68000_NOFPU)
    setFpuType(FPU_NONE);
#else
    setFpuType(FPU_ON);
#endif
}

namespace {

StrBuffer &outOprSize(StrBuffer &out, OprSize size) {
    const auto suffix = sizeSuffix(size);
    if (suffix)
        out.letter('.').letter(suffix);
    return out;
}

}  // namespace

void DisMc68000::outBitField(DisInsn &insn, StrBuffer &out) const {
    const auto post = insn.postfix();
    out.letter('{');
    if (post & 0x800) {
        if (post & 0x600)
            insn.setErrorIf(out, UNKNOWN_POSTBYTE);
        outRegName(out, decodeDataReg(post >> 6));
    } else {
        outDec(out, (post >> 6) & 0x1F, 5);
    }
    out.letter(':');
    if (post & 0x20) {
        if (post & 0x18)
            insn.setErrorIf(out, UNKNOWN_POSTBYTE);
        outRegName(out, decodeDataReg(post));
    } else {
        // 0 means 32
        outDec(out, ((post + 0x1F) & 0x1F) + 1, 6);
    }
    out.letter('}');
}

void DisMc68000::outDataRegPair(DisInsn &insn, StrBuffer &out, OprPos pos) const {
    auto reg1 = REG_UNDEF;
    auto reg2 = REG_UNDEF;
    if (pos == EX_QR) {
        reg1 = decodeDataReg(insn.postfix());
        reg2 = decodeDataReg(insn.postfix() >> 12);
    } else if (pos == EX_DCP) {
        reg1 = decodeDataReg(insn.postfix());
        reg2 = decodeDataReg(insn.post2 = insn.readUint16());
    } else if (pos == EX_DUP) {
        reg1 = decodeDataReg(insn.postfix() >> 6);
        reg2 = decodeDataReg(insn.post2 >> 6);
    }
    outRegName(outRegName(out, reg1).letter(':'), reg2);
}

void DisMc68000::outPointerPair(DisInsn &insn, StrBuffer &out) const {
    const auto reg1 = decodeGeneralReg(insn.postfix() >> 12);
    const auto reg2 = decodeGeneralReg(insn.post2 >> 12);
    outRegName(outRegName(out.letter('('), reg1).text_P(PSTR("):(")), reg2).letter(')');
}

void DisMc68000::outControlReg(DisInsn &insn, StrBuffer &out) const {
    const auto reg = decodeControlReg(insn.postfix());
    if (reg == REG_UNDEF)
        insn.setErrorIf(out, ILLEGAL_REGISTER);
    if (firstGen() && reg > REG_VBR)
        insn.setErrorIf(out, UNKNOWN_REGISTER);
    outRegName(out, reg);
}

void DisMc68000::outImmediateData(DisInsn &insn, StrBuffer &out, OprSize size) const {
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

void DisMc68000::decodeBriefExtension(
        DisInsn &insn, StrBuffer &out, uint16_t ext, RegName base) const {
    if (firstGen() && (ext & 0x0700))
        insn.setErrorIf(out, UNKNOWN_POSTBYTE);
    const auto disp8 = static_cast<int8_t>(ext & 0xFF);
    out.letter('(');
    if (base == REG_PC) {
        const Config::uintptr_t target =
                insn.address() + insn.length() - 2 + static_cast<int8_t>(disp8);
        outRelAddr(out, target, insn.address(), 8);
    } else {
        outHex(out, disp8, -8);
    }
    outRegName(out.letter(','), base);
    outRegName(out.letter(','), decodeGeneralReg(ext >> 12));
    outOprSize(out, (ext & 0x800) ? SZ_LONG : SZ_WORD);
    const auto indexScale = (ext >> 9) & 3;
    if (indexScale && !firstGen()) {
        const auto scale = 1 << indexScale;
        out.letter('*').int16(scale);
    }
}

const char *DisMc68000::outIndexReg(StrBuffer &out, uint16_t ext, const char *delim) const {
    if (ext & 0x40)
        return delim;
    outRegName(out.text(delim), decodeGeneralReg(ext >> 12));
    outOprSize(out, (ext & 0x800) ? SZ_LONG : SZ_WORD);
    const auto indexScale = (ext >> 9) & 3;
    if (indexScale && !firstGen()) {
        const auto scale = 1 << indexScale;
        out.letter('*').int16(scale);
    }
    return PSTR_COMMA;
}

Config::uintptr_t DisMc68000::readDisplacement(
        DisInsn &insn, RegName base, uint_fast8_t bdSize) const {
    if (base == REG_PC) {
        Config::uintptr_t target = insn.address() + insn.length() - 2;
        if (bdSize == 2) {
            target += static_cast<int16_t>(insn.readUint16());
        } else if (bdSize == 3) {
            target += static_cast<int32_t>(insn.readUint32());
        }
        return target;
    }
    if (bdSize == 2)
        return insn.readUint16();
    if (bdSize == 3)
        return insn.readUint32();
    return 0;
}

const char *DisMc68000::outDisplacement(DisInsn &insn, StrBuffer &out, RegName base,
        Config::uintptr_t disp, uint_fast8_t bdSize, const char *delim) const {
    if (base == REG_PC) {
        uint_fast8_t bits = 8;
        if (bdSize == 2) {
            bits = 16;
        } else if (bdSize == 3) {
            bits = 32;
        }
        outRelAddr(out.text(delim), disp, insn.address(), bits);
        delim = PSTR_COMMA;
    } else {
        if (bdSize == 2) {
            outHex(out.text(delim), disp, -16);
            delim = PSTR_COMMA;
        } else if (bdSize == 3) {
            outHex(out.text(delim), disp, -32);
            delim = PSTR_COMMA;
        }
    }
    return delim;
}

void DisMc68000::decodeFullExtension(
        DisInsn &insn, StrBuffer &out, uint16_t ext, RegName base, OprSize size) const {
    if (ext & 8)
        insn.setErrorIf(out, UNKNOWN_POSTBYTE);
    const auto bdSize = (ext >> 4) & 3;
    if (bdSize == 0)
        insn.setErrorIf(out, UNKNOWN_POSTBYTE);
    if (bdSize == 1) {
        const auto bsis = (ext >> 6) & 3;
        if (bsis == 3)
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
        if (bsis == 2 && (ext & 7) >= 5)
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
    }
    if ((ext & 7) == 0) {
        const auto disp = readDisplacement(insn, base, bdSize);
        if ((ext & 0xC0) == 0xC0 || (base == REG_PC && (ext & 0x40) != 0)) {
            if (size != SZ_BYTE && disp % 2)
                insn.setErrorIf(out, OPERAND_NOT_ALIGNED);
        }
        out.letter('(');
        auto delim = outDisplacement(insn, out, base, disp, bdSize, TEXT_null);
        if ((ext & 0x80) == 0) {
            outRegName(out.text(delim), base);
            delim = PSTR_COMMA;
        }
        outIndexReg(out, ext, delim);
    } else {
        const auto iis = (ext & 0107);
        if (iis == 4 || iis >= 0104)
            insn.setErrorIf(out, UNKNOWN_POSTBYTE);
        const auto disp = readDisplacement(insn, base, bdSize);
        if ((ext & 0xC0) == 0xC0 || (base == REG_PC && (ext & 0x40) != 0)) {
            if (disp % 2)
                insn.setErrorIf(out, OPERAND_NOT_ALIGNED);
        }
        out.letter('(').letter('[');
        auto delim = outDisplacement(insn, out, base, disp, bdSize, TEXT_null);
        if ((ext & 0x80) == 0) {
            outRegName(out.text(delim), base);
            delim = PSTR_COMMA;
        }
        if (iis < 4)
            delim = outIndexReg(out, ext, delim);
        out.letter(']');
        delim = PSTR_COMMA;
        if (iis >= 4 && iis < 8)
            delim = outIndexReg(out, ext, delim);
        const auto outer = readDisplacement(insn, REG_UNDEF, ext & 3);
        outDisplacement(insn, out, REG_UNDEF, outer, ext & 3, delim);
    }
}

void DisMc68000::outEffectiveAddr(
        DisInsn &insn, StrBuffer &out, AddrMode mode, RegName reg, OprSize size) const {
    if (mode == M_DREG || mode == M_AREG) {
        outRegName(out, reg);
        return;
    }
    if (mode == M_IMDAT) {
        outImmediateData(insn, out, size);
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
        const auto ext = insn.readUint16();
        if (firstGen() || (ext & 0x0100) == 0) {
            decodeBriefExtension(insn, out, ext, base);
        } else {
            decodeFullExtension(insn, out, ext, base, size);
        }
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
    auto type = mode;
    if (mode == M_REL32) {
        // FBcc
        const auto size16 = (insn.opCode() & 0x40) == 0;
        if (size16)
            type = M_REL16;
    }
    if (mode == M_REL8) {
        const auto rel8 = insn.opCode() & 0xFF;
        if (rel8 == 0x00)
            type = M_REL16;
        if (rel8 == 0xFF && hasLongBranch())
            type = M_REL32;
    }
    // FDBcc has different base address
    const auto FDBcc = insn.hasPostVal();
    const Config::uintptr_t base = insn.address() + (FDBcc ? 4 : 2);
    auto bits = 8;
    Config::ptrdiff_t delta = 0;
    if (type == M_REL32) {
        bits = addressWidth();
        delta = static_cast<int32_t>(insn.readUint32());
        if (_gnuAs) {
            insn.appendName(out, 'L');
        } else if (!overflowDelta(delta, 16)) {
            insn.appendName(out, '.');
            insn.appendName(out, 'X');
        }
    } else if (type == M_REL16) {
        bits = 16;
        delta = static_cast<int16_t>(insn.readUint16());
        if (!_gnuAs && mode == M_REL8 && !overflowDelta(delta, 8)) {
            insn.appendName(out, '.');
            insn.appendName(out, 'W');
        }
    } else {  // type == M_REL8
        delta = static_cast<int8_t>(insn.opCode() & 0xFF);
        if (_gnuAs)
            insn.appendName(out, 'S');
    }
    const Config::uintptr_t target = base + delta;
    insn.setErrorIf(out, checkAddr(target, true));
    // Cactch overwrap case
    if ((delta < 0 && target >= base) || (delta >= 0 && target < base))
        insn.setErrorIf(out, OVERFLOW_RANGE);
    if (insn.getError() == OVERFLOW_RANGE)
        bits = 32;
    outRelAddr(out, target, insn.address(), bits);
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
    const auto mode = decodeAddrMode(insn.opCode() >> 3, 0);
    const auto push = mode == M_PDEC;
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

void checkFmovemMode(DisInsn &insn, AddrMode mode, StrBuffer &out) {
    const auto src = insn.src();
    const auto dst = insn.dst();
    if (needsFmoveXtendCheck(src, insn.srcPos()) || needsFmoveXtendCheck(dst, insn.dstPos())) {
        const auto decrement = (insn.postfix() & 0x1000) == 0;
        if (decrement && mode != M_PDEC)
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
        if (!decrement && mode == M_PDEC)
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
    } else if (needsFmoveLongCheck(src) || needsFmoveLongCheck(dst)) {
        const auto list = (insn.postfix() >> 10) & 7;
        const auto single = (list == 4 || list == 2 || list == 1);
        // Single FPCR/FPSR/FPIAR can move to Dn
        if (mode == M_DREG && !single)
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
        // Single FPIAR can move to An
        if (mode == M_AREG && list != 1)
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
        // Immediate can move to a single register
        if (mode == M_IMDAT && !single)
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
    case EX_SC:  // Ds:Dc
        return ((post & 0x380) >> 3) | (post & 7);
    case EX_DL:
    case EX_DK:
        return (post >> 4) & 7;
    case EX_GR:
        return (post >> 12) & 0xF;
    case EX_DR:
    case EX_QQ:
        return (post >> 12) & 7;
    case EX_DC:
        return post & 7;
    case EX_DU:
        return (post >> 6) & 7;
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

bool inFloatOperand(const DisInsn &insn, AddrMode type) {
    if (type == M_RDATA) {
        const auto dst = insn.dst();
        // Dn,FPn || Dn,FPc:FPs || FTST Dn
        return dst == M_FPREG || dst == M_FSICO || dst == M_NONE;
    }
    if (type == M_WDATA) {
        // FPn,Dn
        return insn.src() == M_FPREG;
    }
    return false;
}

}  // namespace

void DisMc68000::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode type, OprPos pos,
        OprSize size, uint16_t opr16, Error opr16Error) const {
    const auto m = modeVal(insn, pos);
    const auto r = regVal(insn, pos);
    const auto mode = decodeAddrMode(m, r);
    if (mode == M_DREG && inFloatOperand(insn, type)) {
        if (size == SZ_DUBL || size == SZ_XTND || size == SZ_PBCD)
            insn.setErrorIf(out, ILLEGAL_SIZE);
    }
    RegName reg;
    switch (type) {
    case M_DREG:
        outEffectiveAddr(insn, out, type, decodeDataReg(r), size);
        break;
    case M_KDREG:
        reg = decodeDataReg((insn.postfix() >> 4) & 7);
        outRegName(out.letter('{'), reg).letter('}');
        break;
    case M_AREG:
    case M_PDEC:
    case M_PINC:
    case M_DISP:
        outEffectiveAddr(insn, out, type, decodeAddrReg(r), size);
        break;
    case M_GREG:
        outRegName(out, decodeGeneralReg(r));
        break;
    case M_RADDR:
    case M_WADDR:
    case M_IADDR:
    case M_DADDR:
        checkFmovemMode(insn, mode, out);
        /* Fall-through */
    case M_RDATA:
    case M_WDATA:
    case M_RMEM:
    case M_WMEM:
    case M_JADDR:
    case M_BITFR:
    case M_BITFW:
        outEffectiveAddr(insn, out, mode, decodeRegNo(m, r), size);
        break;
    case M_IM3:
        if (pos == OP__3) {
            // ADDQ/SUBQ/shift/rotate
            const auto im3 = (r == 0) ? 8 : r;
            outDec(out.letter('#'), im3, 4);
        } else if (pos == OP__0) {
            // BKPT
            outDec(out.letter('#'), r, 3);
        }
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
        outImmediateData(insn, out, size);
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
        decodeRelative(insn, out, type);
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
    case M_KFACT:
        if (insn.isOK()) {
            const auto kfact = insn.postfix() & 0x7F;
            outDec(out.letter('{').letter('#'), kfact, -7).letter('}');
        }
        break;
    case M_IMROM:
        outHex(out.letter('#'), insn.postfix() & 0x7F, 7, false);
        break;
    case M_CREG:
        outControlReg(insn, out);
        break;
    case M_BITOW:
        outBitField(insn, out);
        break;
    case M_DPAIR:
        outDataRegPair(insn, out, pos);
        break;
    case M_PPAIR:
        outPointerPair(insn, out);
        break;
    default:
        break;
    }
}

OprSize decodeSize(DisInsn &insn) {
    static constexpr OprSize INT_SIZES[] PROGMEM = {
            SZ_BYTE,
            SZ_WORD,
            SZ_LONG,
            SZ_WORD,
    };
    const auto osize = insn.oprSize();
    const auto opc = insn.opCode();
    if (osize == SZ_DATA)
        return pgm_read_byte(&INT_SIZES[(opc >> 6) & 3]);
    if (osize == SZ_DATH)
        return pgm_read_byte(&INT_SIZES[(opc >> 9) & 3]);
    if (osize == SZ_FDAT) {
        static constexpr OprSize FLT_SIZES[] PROGMEM = {
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
        return pgm_read_byte(&FLT_SIZES[fmt]);
    }
    if (osize == SZ_ADDR)
        return pgm_read_byte(&INT_SIZES[((opc >> 6) & 1) + 1]);
    if (osize == SZ_ADR8)
        return pgm_read_byte(&INT_SIZES[((opc >> 8) & 1) + 1]);
    if (osize == SZ_CAS1 || osize == SZ_CAS2)
        return pgm_read_byte(&INT_SIZES[((opc >> 9) & 3) - 1]);
    return osize;
}

char decodeInsnSize(const DisInsn &insn, OprSize osize) {
    const auto isize = insn.insnSize();
    const auto size =
            (isize == ISZ_DATA || isize == ISZ_FIXD || isize == ISZ_FDAT) ? osize : OprSize(isize);
    return sizeSuffix(size);
}

void DisInsn::appendName(StrBuffer &out, char c) {
    auto save{out};
    nameBuffer().over(out);
    out.letter(c).over(nameBuffer());
    save.over(out);
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
        if (!_gnuAs)
            insn.appendName(out, '.');
        insn.appendName(out, suffix);
    }

    const auto dst = insn.dst();
    const auto opr16 = (dst == M_MULT || dst == M_IMDSP) ? insn.readUint16() : 0;
    const auto opr16Error = insn.getError();

    decodeOperand(insn, out, insn.src(), insn.srcPos(), osize);
    if (dst != M_NONE && !isFloatOpOnSameFpreg(insn)) {
        if (dst != M_BITOW)
            out.comma();
        decodeOperand(insn, out, dst, insn.dstPos(), osize, opr16, opr16Error);
    }
    const auto ext = insn.ext();
    if (ext != M_NONE) {
        if (ext != M_KDREG && ext != M_KFACT && ext != M_BITOW)
            out.comma();
        decodeOperand(insn, out, ext, insn.extPos(), osize);
    }
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
