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

void outBcdDigit(StrBuffer &out, uint8_t digit, ErrorAt &error) {
    if (digit < 10) {
        out.letter(digit + '0');
    } else {
        out.letter('0');
        error.setErrorIf(out, ILLEGAL_CONSTANT);
    }
}

}  // namespace

void DisMc68000::decodeImmediateData(DisInsn &insn, StrBuffer &out, OprSize size) const {
    out.letter('#');
    if (size == SZ_BYTE || insn.src() == M_CCR || insn.dst() == M_CCR) {
        outHex(out, insn.readUint16() & 0xFF, 8);
    } else if (size == SZ_WORD) {
        outHex(out, insn.readUint16(), 16);
    } else if (size == SZ_LONG) {
        outHex(out, insn.readUint32(), 32);
    } else if (size == SZ_SNGL) {
        out.float32(insn.readFloat32());
    } else if (size == SZ_DUBL) {
        out.float64(insn.readFloat64());
    } else if (size == SZ_XTND) {
        outFloat96(out, insn.readFloat96(), insn);
    } else if (size == SZ_PBCD) {
        outPackedBcd96(out, insn.readFloat96(), insn);
    }
}

StrBuffer &DisMc68000::outFloat96(StrBuffer &out, const Float96 &v, ErrorAt &error) const {
#if FLOAT96_STRICT
    if (v.digit != 0) {
        error.setErrorIf(out, ILLEGAL_CONSTANT);
        return out;
    }
#endif
    if (v.exp & INT16_MIN)
        out.letter('-');
    const auto exp = v.exp & INT16_MAX;
    if (exp == INT16_MAX) {
        // MSB of siginificand is don't care.
        return (v.significand & INT64_MAX) == 0 ? out.text_P(TEXT_INF) : out.text_P(TEXT_NAN);
    }
    const auto exponent = static_cast<int16_t>(exp) - 0x3FFF - 63;
    return out.float80(exponent, v.significand);
}

StrBuffer &DisMc68000::outPackedBcd96(StrBuffer &out, const Float96 &v, ErrorAt &error) const {
#if FLOAT96_STRICT
    if ((v.digit & ~0xF) != 0) {
        error.setErrorIf(out, ILLEGAL_CONSTANT);
        return out;
    }
#endif
    if (v.exp & 0x8000)
        out.letter('-');
    const auto exp = v.exp & INT16_MAX;
    if (exp == INT16_MAX)
        return v.significand == 0 ? out.text_P(TEXT_INF) : out.text_P(TEXT_NAN);
#if FLOAT96_STRICT
    if (v.exp & 0x3000)
        error.setErrorIf(out, ILLEGAL_CONSTANT);
#endif
    outBcdDigit(out, v.digit & 0xF, error);
    if (v.significand)
        out.letter('.');
    auto significand = v.significand;
    while (significand) {
        outBcdDigit(out, significand >> 60, error);
        significand <<= 4;
    }
    if (v.exp & 0x0FFF) {
        out.letter('E');
        if (v.exp & 0x4000)
            out.letter('-');
        auto exponent = v.exp << 4;
        auto suppress = true;
        for (auto i = 0; i < 3; ++i, exponent <<= 4) {
            const auto digit = (exponent >> 12) & 0xF;
            if (digit || !suppress) {
                outBcdDigit(out, digit, error);
                suppress = false;
            }
        }
    }
    return out;
}

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
        outAbsAddr(out.letter('('), target);
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
    const auto base = insn.address() + 2;
    if (mode == M_REL32 && (insn.opCode() & (1 << 6)) == 0)
        mode = M_REL16;
    if (mode == M_REL32) {
        const auto delta = static_cast<int32_t>(insn.readUint32());
        if (!overflowInt16(delta)) {
            auto save{out};
            insn.nameBuffer().over(out);
            out.letter('.').letter('L').over(insn.nameBuffer());
            save.over(out);
        }
        const auto target = base + delta;
        insn.setErrorIf(out, checkAddr(target, true));
        outRelAddr(out, target, insn.address(), insn.isOK() ? addressWidth() : 32);
        return;
    }
    const auto rel8 = (mode == M_REL8) ? static_cast<int8_t>(insn.opCode() & 0xFF) : 0;
    const auto delta = rel8 ? static_cast<int8_t>(rel8) : static_cast<int16_t>(insn.readUint16());
    if (mode == M_REL8 && rel8 == 0 && !overflowInt8(delta)) {
        auto save{out};
        insn.nameBuffer().over(out);
        out.letter('.').letter('W').over(insn.nameBuffer());
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
    default:
        return 0;
    }
}

bool isFloatOpOnSameFpreg(const DisInsn &insn) {
    return insn.src() == M_FPREG && insn.dst() == M_FPREG &&
           regVal(insn, insn.srcPos()) == regVal(insn, insn.dstPos());
}

}  // namespace

void DisMc68000::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos,
        OprSize size, uint16_t opr16, Error opr16Error) const {
    if (mode == M_KDREG || mode == M_KFACT)
        pos = OP_10;
    auto m = modeVal(insn, pos);
    auto r = regVal(insn, pos);
    EaMc68000 ea(size, m, r);
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
        if (insn.src() == M_FPREG && ea.mode == M_DREG) {
            if (size != SZ_BYTE && size != SZ_WORD && size != SZ_LONG && size != SZ_SNGL)
                insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
        }
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
    if (TABLE.searchOpCode(_cpuSpec, insn, out))
        return _insn.setError(insn);

    const auto osize = decodeSize(insn);
    const auto suffix = decodeInsnSize(insn, osize);
    if (suffix) {
        auto save{out};
        insn.nameBuffer().over(out);
        out.letter('.').letter(suffix).over(insn.nameBuffer());
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

Float96 DisInsn::readFloat96() {
    /** Float96
     * | |15-bit exponent|16-bit zeroes|64-bit significand|
     *  ^- sign of significand
     */
    /** PackedBcd96
     * | | |  |3-digit exponent|12-bit zeroes|1-digit integer|16-digit significand|
     *  ^ ^ ^-- infinity and nan
     *  | +---- sign of exponent
     *  +------ sign of significand
     */
    const auto exp = readUint16();
    const auto digit = readUint16();
    const auto significand = readUint64();
    return {exp, digit, significand};
}

}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
