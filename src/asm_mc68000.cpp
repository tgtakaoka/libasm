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

#include "asm_mc68000.h"
#include <stdlib.h>
#include "table_mc68000.h"
#include "text_mc68000.h"

namespace libasm {
namespace mc68000 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::option;

using text::mc68000::TEXT_FMOVE;
using text::mc68000::TEXT_FPU_68881;
using text::mc68000::TEXT_FPU_MC68881;

namespace {

// clang-format off
constexpr char TEXT_DC_B[] PROGMEM = "dc.b";
constexpr char TEXT_DC_D[] PROGMEM = "dc.d";
constexpr char TEXT_DC_L[] PROGMEM = "dc.l";
constexpr char TEXT_DC_P[] PROGMEM = "dc.p";
constexpr char TEXT_DC_S[] PROGMEM = "dc.s";
constexpr char TEXT_DC_W[] PROGMEM = "dc.w";
constexpr char TEXT_DC_X[] PROGMEM = "dc.x";
constexpr char TEXT_DS_B[] PROGMEM = "ds.b";
constexpr char TEXT_DS_L[] PROGMEM = "ds.l";
constexpr char TEXT_DS_W[] PROGMEM = "ds.w";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_dALIGN, &Assembler::alignOrigin},
    {TEXT_DC,     &Assembler::defineDataConstant, Assembler::DATA_WORD|Assembler::DATA_ALIGN2},
    {TEXT_DC_B,   &Assembler::defineDataConstant, Assembler::DATA_BYTE},
#if !defined(LIBASM_MC68000_NOFPU)
    {TEXT_DC_D,   &Assembler::defineDataConstant, Assembler::DATA_FLOAT64|Assembler::DATA_ALIGN2},
#endif
    {TEXT_DC_L,   &Assembler::defineDataConstant, Assembler::DATA_LONG|Assembler::DATA_ALIGN2},
#if !defined(LIBASM_MC68000_NOFPU)
    {TEXT_DC_S,   &Assembler::defineDataConstant, Assembler::DATA_FLOAT32|Assembler::DATA_ALIGN2},
#endif
    {TEXT_DC_W,   &Assembler::defineDataConstant, Assembler::DATA_WORD|Assembler::DATA_ALIGN2},
    {TEXT_DS,     &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DS_B,   &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DS_L,   &Assembler::allocateSpaces,     Assembler::DATA_LONG|Assembler::DATA_ALIGN2},
    {TEXT_DS_W,   &Assembler::allocateSpaces,     Assembler::DATA_WORD|Assembler::DATA_ALIGN2},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmMc68000::defaultPlugins() {
    static const struct final : ValueParser::MotorolaPlugins {
        const SymbolParser &symbol() const override { return Mc68000SymbolParser::singleton(); }
        const LetterParser &letter() const override { return Plugins::letter(); }
        const OperatorParser &operators() const override {
            return CStyleOperatorParser::singleton();
        }
    } PLUGINS{};
    return PLUGINS;
}

AsmMc68000::AsmMc68000(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_fpu),
      Config(TABLE),
      _opt_fpu(this, &Config::setFpuName, OPT_TEXT_FPU, OPT_DESC_FPU) {
    reset();
}

void AsmMc68000::reset() {
    Assembler::reset();
    setFpuType(FPU_NONE);
}

namespace {

int8_t modePos(OprPos pos) {
    // clang-format off
    static constexpr int8_t BITNO[] PROGMEM = {
        3, 6, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1,
    };
    // clang-format on
    return static_cast<int8_t>(pgm_read_byte(&BITNO[pos]));
}

int8_t regPos(OprPos pos) {
    // clang-format off
    static constexpr int8_t BITNO[] PROGMEM = {
        0, 9, 0, 9, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1,
    };
    // clang-format on
    return static_cast<int8_t>(pgm_read_byte(&BITNO[pos]));
}

int8_t postPos(OprPos pos) {
    // clang-format off
    static constexpr int8_t BITNO[] PROGMEM = {
        -1, -1, -1, -1, -1,
        10, 7, 7, 0, 4, 0, 4, 12, 0,
    };
    // clang-format on
    return static_cast<int8_t>(pgm_read_byte(&BITNO[pos]));
}

bool checkFloatControlMode(AsmInsn &insn, const Operand &op) {
    const auto src = insn.src();
    const auto &ea = (src == op.mode || src == M_FCMLT) ? insn.dstOp : insn.srcOp;
    if (op.mode != M_FPIAR && ea.mode == M_AREG)
        insn.setErrorIf(ea, ILLEGAL_OPERAND_MODE);
    return src == M_FCMLT || insn.dst() == M_FCMLT;
}

}  // namespace

void emitOprSize(AsmInsn &insn, InsnSize isize) {
    const auto osize = insn.oprSize();
    if (osize == SZ_DATA) {
        switch (isize) {
        case ISZ_NONE:
        case ISZ_WORD:
            insn.embed(1 << 6);
            break;
        case ISZ_LONG:
            insn.embed(2 << 6);
            break;
        default:  // ISZ_BYTE
            break;
        }
        return;
    }
    if (osize == SZ_ADDR) {
        if (isize == ISZ_LONG)
            insn.embed(1 << 6);
        return;
    }
    if (osize == SZ_ADR8) {
        if (isize == ISZ_LONG)
            insn.embed(1 << 8);
        return;
    }
    if (osize == SZ_FDAT) {
        // clang-format off
        static constexpr uint8_t FSIZES[] PROGMEM = {
            // B  W  L  _  _  _  _  S  D  X  P  _
            0, 6, 4, 0, 0, 0, 0, 0, 1, 5, 2, 3, 0,
        };
        // clang-format on
        insn.embedPostfix(static_cast<Config::opcode_t>(pgm_read_byte(&FSIZES[isize])) << 10);
        return;
    }
}

Error AsmMc68000::checkAlignment(AsmInsn &insn, OprSize size, const Operand &op) const {
    const auto val32 = op.val.getUnsigned();
    if (size == SZ_WORD && (val32 % 2))
        return insn.setError(op, OPERAND_NOT_ALIGNED);
    if (size == SZ_LONG && (val32 % 2))
        return insn.setError(op, OPERAND_NOT_ALIGNED);
    return OK;
}

void AsmMc68000::encodeBriefExtension(
        AsmInsn &insn, const Operand &op, Config::ptrdiff_t disp) const {
    if (overflowDelta(disp, 8))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    uint16_t ext = static_cast<uint8_t>(disp);
    ext |= encodeGeneralRegNo(op.indexReg) << 12;
    if (isAddrReg(op.indexReg))
        ext |= (1 << 15);
    if (op.indexSize == ISZ_LONG)
        ext |= (1 << 11);
    insn.emitOperand16(ext);
}

void AsmMc68000::encodeDisplacement(
        AsmInsn &insn, const Operand &op, Config::ptrdiff_t disp) const {
    if (overflowDelta(disp, 16))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.emitOperand16(disp);
}

void AsmMc68000::encodeRelativeAddr(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    // FDxx has different base address
    const auto base = insn.address() + (insn.hasPostVal() ? 4 : 2);
    const auto target = op.getError() ? base : op.val.getUnsigned();
    insn.setErrorIf(op, checkAddr(target, true));
    ErrorAt error;
    // the branchDelta() uses addressWidth(), which is 24 bit, to
    // check the range of |delta|. It is not suitable for
    // M_REL32. Record errors in |error| then copy it to |insn| if
    // any.
    const auto delta = branchDelta(base, target, error, op);
    const auto insnSize = insn.insnSize();
    if (mode == M_REL8) {
        if (insnSize == ISZ_NONE) {
            if (overflowDelta(delta, 8))
                goto rel16;
            insn.embed(static_cast<uint8_t>(delta));
        } else if (insnSize == ISZ_BYTE || insnSize == ISZ_SNGL) {
            if (overflowDelta(delta, 8))
                error.setErrorIf(op, OPERAND_TOO_FAR);
            insn.embed(static_cast<uint8_t>(delta));
        } else if (insnSize == ISZ_WORD || insnSize == ISZ_LONG) {
            goto rel16;
        } else {
            error.setErrorIf(ILLEGAL_SIZE);
        }
    } else if (mode == M_REL16) {
        if (insnSize == ISZ_NONE || insnSize == ISZ_WORD || insnSize == ISZ_LONG) {
        rel16:
            if (overflowDelta(delta, 16))
                error.setErrorIf(op, OPERAND_TOO_FAR);
        } else {
            error.setErrorIf(ILLEGAL_SIZE);
        }
        insn.emitOperand16(delta);
    } else if (mode == M_REL32) {
        if (insnSize == ISZ_NONE) {
            if (!overflowDelta(delta, 16))
                goto rel32_16;
            goto rel32;
        } else if (insnSize == ISZ_WORD || insnSize == ISZ_LONG) {
            if (overflowDelta(delta, 16))
                error.setErrorIf(op, OPERAND_TOO_FAR);
        rel32_16:
            insn.setOpCode(insn.opCode() & ~(1 << 6));  // M_REL16
            insn.emitOperand16(static_cast<uint16_t>(delta));
        } else if (insnSize == ISZ_LONG || insnSize == ISZ_XTND) {
        rel32:
            error.setOK();                      // M_REL32 never overflow.
            insn.embed(1 << 6);                 // M_REL32
            insn.emitOperand32(target - base);  // 32 bit delta
        } else {
            error.setErrorIf(ILLEGAL_SIZE);
            goto rel32_16;
        }
    }
    // Copy error if any
    insn.setErrorIf(error);
}

void AsmMc68000::encodeImmediate(AsmInsn &insn, const Operand &op, OprSize size) const {
    const auto val32 = op.val.getUnsigned();
    switch (size) {
    case SZ_LONG:
        if (op.val.overflowUint32())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand32(val32);
        break;
    case SZ_WORD:
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(val32);
        break;
    case SZ_BYTE:
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(static_cast<uint8_t>(val32));
        break;
#if defined(LIBASM_ASM_NOFLOAT) || defined(LIBASM_MC68000_NOFPU)
    case SZ_SNGL:
        insn.setErrorIf(op, FLOAT_NOT_SUPPORTED);
        insn.emitUint32(0, insn.operandPos());
        break;
    case SZ_DUBL:
        insn.setErrorIf(op, FLOAT_NOT_SUPPORTED);
        insn.emitUint64(0, insn.operandPos());
        break;
    case SZ_XTND:
    case SZ_PBCD:
        insn.setErrorIf(op, FLOAT_NOT_SUPPORTED);
        insn.emitUint32(0, insn.operandPos());
        insn.emitUint64(0, insn.operandPos());
        break;
#else
    case SZ_SNGL:
        insn.emitFloat32(op.val.getFloat(), insn.operandPos());
        break;
    case SZ_DUBL:
        insn.emitFloat64(op.val.getFloat(), insn.operandPos());
        break;
    case SZ_XTND:
        insn.emitExtendedReal(op.val.getFloat(), insn.operandPos());
        break;
    case SZ_PBCD: {
        insn.emitDecimalString(op.val.getFloat(), insn.operandPos());
        break;
    }
#endif
    default:
        break;
    }
}

Config::uintptr_t Operand::offset(const AsmInsn &insn) const {
    if (getError())
        return 0;
    auto len = insn.length();
    if (len == 0)
        len = sizeof(Config::opcode_t);
    if (insn.hasPostVal() && len < 4)
        len = 4;
    return val.getUnsigned() - (insn.address() + len);
}

namespace {
bool inFloatOperand(const AsmInsn &insn, AddrMode mode) {
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

Error AsmMc68000::encodeOperand(
        AsmInsn &insn, OprSize size, const Operand &op, AddrMode mode, OprPos pos) const {
    if (mode == M_NONE) {
        if (op.mode != M_NONE)
            insn.setErrorIf(op, UNKNOWN_OPERAND);
        return insn.getError();
    }

    const auto mode_gp = modePos(pos);
    if (mode_gp >= 0) {
        const auto m = EaMc68000::encodeMode(op.mode);
        insn.embed(m << mode_gp);
    }

    const auto reg_gp = regPos(pos);
    if (reg_gp >= 0) {
        const auto r = EaMc68000::encodeRegNo(op.mode, op.reg);
        insn.embed(r << reg_gp);
    }

    const auto post_gp = postPos(pos);
    if (pos == EX_DL) {
        if (op.mode == M_DREG)
            insn.embedPostfix(encodeGeneralRegNo(op.reg) << post_gp);
        if (insn.dstPos() == EX_DL || EaMc68000::encodeMode(insn.dstOp.mode) != M_PDEC)
            insn.embedPostfix(0x1000);
    }

    const auto val32 = op.val.getUnsigned();
    switch (op.mode) {
    case M_DREG:
        if (inFloatOperand(insn, mode)) {
            if (size == SZ_DUBL || size == SZ_XTND || size == SZ_PBCD)
                insn.setErrorIf(op, ILLEGAL_SIZE);
        }
        if (post_gp >= 0)
            insn.embedPostfix(encodeGeneralRegNo(op.reg) << post_gp);
        break;
    case M_AREG:
        if (post_gp >= 0) {
            // MOVES.B An is OK
            insn.embedPostfix(encodeGeneralRegNo(op.reg) << post_gp);
        } else if (size == SZ_BYTE) {
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        }
        break;
    case M_INDX:
        encodeBriefExtension(insn, op, static_cast<Config::ptrdiff_t>(val32));
        break;
    case M_PCIDX:
        encodeBriefExtension(insn, op, op.offset(insn));
        break;
    case M_DISP:
        encodeDisplacement(insn, op, static_cast<Config::ptrdiff_t>(val32));
        break;
    case M_PCDSP:
        insn.setErrorIf(op, checkAlignment(insn, size, op));
        encodeDisplacement(insn, op, op.offset(insn));
        break;
    case M_AWORD:
    case M_ALONG:
        insn.setErrorIf(op, checkAlignment(insn, size, op));
        if (op.mode == M_AWORD) {
            insn.emitOperand16(val32);
        } else {
            insn.emitOperand32(val32);
        }
        break;
    case M_IMDAT:
        if (mode == M_IMBIT)
            break;
        if (mode == M_IM3) {
            // "Zero means 2^3" unsigned 3-bit.
            if (op.val.overflow(8))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            if (op.val.isZero())
                insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
            const auto count = (val32 & 7);  // 8 is encoded to 0.
            insn.embed(count << 9);
            break;
        }
        if (mode == M_IM8) {
            // Signed 8-bit.
            if (op.val.overflowInt8())
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.embed(static_cast<uint8_t>(val32));
            break;
        }
        if (mode == M_IMROM) {
            if (op.val.overflow(INT8_MAX))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.embedPostfix(val32 & 0x7F);
            break;
        }
        if (mode == M_IMVEC) {
            if (op.val.overflow(15))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.embed(static_cast<uint8_t>(val32 & 0xF));
            break;
        }
        if (mode == M_IMDSP) {
            if (op.val.overflowInt16())
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand16(static_cast<uint16_t>(val32));
            break;
        }
        /* Fall-through */
    case M_IMFLT:
        encodeImmediate(insn, op, size);
        break;
    case M_LABEL:
        if (size == SZ_BYTE && mode == M_REL16)
            insn.setErrorIf(op, ILLEGAL_SIZE);
        if (size == SZ_WORD && mode == M_REL8)
            mode = M_REL16;
        encodeRelativeAddr(insn, mode, op);
        break;
    case M_FPREG:
        if (mode == M_FPMLT) {
            encodeFloatRegisterList(insn, op);
        } else if (post_gp >= 0) {
            insn.embedPostfix(encodeFloatRegNo(op.reg) << post_gp);
            if (insn.dst() == M_NONE)
                insn.embedPostfix(encodeFloatRegNo(op.reg) << postPos(EX_RY));
        }
        break;
    case M_FSICO:
        insn.embedPostfix(encodeFloatRegNo(op.reg));                         // FPc
        insn.embedPostfix(encodeFloatRegNo(op.indexReg) << postPos(EX_SC));  // FPs
        break;
    case M_FPMLT:
        encodeFloatRegisterList(insn, op);
        break;
    case M_FPSR:
    case M_FPCR:
    case M_FPIAR:
        if (!checkFloatControlMode(insn, op))
            break;
        /* Fall-through */
    case M_FCMLT:
        encodeFloatControlList(insn, op);
        break;
    case M_USP:
    case M_CREG:
        if (post_gp == 0)
            insn.embedPostfix(encodeControlRegNo(op.reg));
        break;
    default:
        break;
    }
    if (op.kMode == M_KFACT) {
        insn.embedPostfix(op.kFact & 0x7F);
    } else if (op.kMode == M_KDREG) {
        const auto kdreg = encodeGeneralRegNo(op.kDreg);
        insn.embedPostfix(0x1000 | (kdreg << 4));
    }
    return insn.getError();
}

void AsmMc68000::encodeFloatControlList(AsmInsn &insn, const Operand &op) const {
    auto p = op.list;
    uint8_t n = 0;
    auto reg = REG_UNDEF;
    for (;;) {
        auto a = p;
        reg = parseRegName(a, parser());
        if (!isFloatControlReg(reg))
            insn.setErrorIf(p, REGISTER_NOT_ALLOWED);
        insn.embedPostfix(1 << encodeFloatControlRegPos(reg));
        ++n;
        if (a.skipSpaces().expect('/')) {
            p = a;
            continue;
        }
        if (*a != ',' && !endOfLine(a))
            insn.setErrorIf(a, UNKNOWN_OPERAND);
        break;
    }
    const auto &ea = insn.src() == M_FCMLT ? insn.dstOp : insn.srcOp;
    if (ea.mode == M_DREG || ea.mode == M_AREG || ea.mode == M_IMDAT)
        insn.setError(ea, ILLEGAL_OPERAND_MODE);
}

namespace {

void swapRegPos(uint8_t &s, uint8_t &e) {
    const auto t = s;
    s = e;
    e = t;
}

uint8_t reverseUint8(uint8_t bits) {
    bits = (bits & 0x55) << 1 | (bits & 0xAA) >> 1;
    bits = (bits & 0x33) << 2 | (bits & 0xCC) >> 2;
    return bits << 4 | bits >> 4;
}

uint16_t reverseUint16(uint16_t bits) {
    bits = (bits & 0x5555) << 1 | (bits & 0xAAAA) >> 1;
    bits = (bits & 0x3333) << 2 | (bits & 0xCCCC) >> 2;
    bits = (bits & 0x0F0F) << 4 | (bits & 0xF0F0) >> 4;
    return bits << 8 | bits >> 8;
}

}  // namespace

void AsmMc68000::encodeFloatRegisterList(AsmInsn &insn, const Operand &op) const {
    uint8_t bits = 0;
    if (op.mode == M_FPREG) {
        bits = 1 << encodeFloatRegPos(op.reg);
    } else {
        auto p = op.list;
        for (;;) {
            auto a = p;
            const auto start = parseRegName(a, parser());
            if (!isFloatReg(start))
                insn.setErrorIf(p, REGISTER_NOT_ALLOWED);
            auto s = encodeFloatRegPos(start);
            auto e = s;
            if (a.skipSpaces().expect('-')) {
                p = a.skipSpaces();
                const auto last = parseRegName(a, parser());
                if (!isFloatReg(last))
                    insn.setErrorIf(p, REGISTER_NOT_ALLOWED);
                e = encodeFloatRegPos(last);
            }
            if (s > e)
                swapRegPos(s, e);
            for (auto i = s; i <= e; i++) {
                const uint8_t bm = 1 << i;
                if (bits & bm)
                    insn.setErrorIf(p, DUPLICATE_REGISTER);
                bits |= bm;
            }
            if (a.skipSpaces().expect('/')) {
                p = a;
                continue;
            }
            if (*a != ',' && !endOfLine(a))
                insn.setErrorIf(a, UNKNOWN_OPERAND);
            break;
        }
    }
    const auto dstMode = EaMc68000::encodeMode(insn.dstOp.mode);
    if (insn.src() == M_FPMLT && dstMode == M_PDEC)
        bits = reverseUint8(bits);
    insn.embedPostfix(bits);
    if (insn.dst() == M_FPMLT || dstMode != M_PDEC)
        insn.embedPostfix(0x1000);
}

void AsmMc68000::encodeRegisterList(AsmInsn &insn, const Operand &op, bool reverse) const {
    auto p = op.list;
    uint16_t bits = 0;
    for (;;) {
        auto a = p;
        const auto start = parseRegName(a, parser());
        if (!isGeneralReg(start))
            insn.setErrorIf(p, REGISTER_NOT_ALLOWED);
        auto s = encodeGeneralRegPos(start);
        auto e = s;
        if (a.skipSpaces().expect('-')) {
            p = a.skipSpaces();
            const auto last = parseRegName(a, parser());
            if (!isGeneralReg(last))
                insn.setErrorIf(p, REGISTER_NOT_ALLOWED);
            e = encodeGeneralRegPos(last);
        }
        if (s > e)
            swapRegPos(s, e);
        for (auto i = s; i <= e; i++) {
            const uint16_t bm = 1 << i;
            if (bits & bm)
                insn.setErrorIf(p, DUPLICATE_REGISTER);
            bits |= bm;
        }
        if (a.skipSpaces().expect('/')) {
            p = a;
            continue;
        }
        if (*a != ',' && !endOfLine(a))
            insn.setErrorIf(a, UNKNOWN_OPERAND);
        break;
    }
    if (reverse)
        bits = reverseUint16(bits);
    insn.emitOperand16(bits);
}

Error AsmMc68000::parseKFactor(StrScanner &scan, Operand &op) const {
    auto p = scan;
    if (p.skipSpaces().expect('{')) {
        if (p.skipSpaces().expect('#')) {
            auto kfact = parseInteger(p, op);
            if (op.hasError())
                return op.getError();
            if (kfact.overflow(63, -64)) {
                op.setErrorIf(scan, OVERFLOW_RANGE);
                op.kFact = 17;
            } else {
                op.kFact = kfact.isUndefined() ? 17 : kfact.getSigned();
            }
            op.kMode = M_KFACT;
        } else {
            op.kDreg = parseRegName(p, parser());
            if (isDataReg(op.kDreg)) {
                op.kMode = M_KDREG;
            } else {
                op.setErrorIf(scan, REGISTER_NOT_ALLOWED);
            }
        }
        if (!p.skipSpaces().expect('}'))
            return op.setErrorIf(p, MISSING_CLOSING_PAREN);
        scan = p;
    } else {
        op.kMode = M_KFACT;
        op.kFact = 17;
    }
    return OK;
}

Error AsmMc68000::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(op.list = p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#')) {
        op.val = parseExpr(p, op);
        if (op.hasError())
            return op.getError();
        if (op.val.isFloat()) {
            op.mode = M_IMFLT;
            scan = p;
            return op.setOK();
        }
        op.mode = M_IMDAT;
        scan = p;
        return OK;
    }
    auto a = p;
    const auto pdec = (*a++ == '-' && *a == '(');
    if (pdec)
        p = a;
    if (p.expect('(')) {
        op.reg = parseRegName(p.skipSpaces(), parser());
        if (isAddrReg(op.reg)) {
            if (!p.skipSpaces().expect(')'))
                return op.setError(MISSING_CLOSING_PAREN);
            if (pdec) {
                op.mode = M_PDEC;
            } else if (p.expect('+')) {
                op.mode = M_PINC;
            } else {
                op.mode = M_AIND;
            }
            scan = p;
            return OK;
        }
        op.val = parseInteger(p, op, ')');
        if (op.hasError())
            return op.getError();
        const auto val32 = op.val.getUnsigned();
        if (p.skipSpaces().expect(')')) {
            const auto size = parseSize(p.skipSpaces());
            auto over16 = op.val.overflowInt16();
            if (over16) {
                // check if it is near the end of address space.
                const auto max = 1UL << uint8_t(addressWidth());
                if (val32 <= max - 1 && val32 >= (max - 0x8000))
                    over16 = false;
            }
            if (over16 && size == ISZ_WORD)
                op.setErrorIf(OVERFLOW_RANGE);
            op.mode = (size == ISZ_WORD || (size == ISZ_NONE && !over16)) ? M_AWORD : M_ALONG;
            scan = p;
            return OK;
        }
        if (p.expect(',')) {
            a = p.skipSpaces();
            op.reg = parseRegName(a, parser());
            if (!isAddrReg(op.reg) && op.reg != REG_PC)
                return op.setError(a, REGISTER_NOT_ALLOWED);
            if (a.skipSpaces().expect(')')) {
                op.mode = (op.reg == REG_PC) ? M_PCDSP : M_DISP;
                scan = a;
                return OK;
            }
            if (!a.expect(','))
                return op.setError(a, MISSING_COMMA);
            p = a.skipSpaces();
            op.indexReg = parseRegName(p, parser());
            if (!isGeneralReg(op.indexReg))
                return op.setError(UNKNOWN_OPERAND);
            op.indexSize = parseSize(p);
            if (op.indexSize == ISZ_ERROR)
                return op.setError(UNKNOWN_OPERAND);
            if (op.indexSize == ISZ_NONE)
                op.indexSize = ISZ_WORD;
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            op.mode = (op.reg == REG_PC) ? M_PCIDX : M_INDX;
            scan = p;
            return OK;
        }
        return op.setError(UNKNOWN_OPERAND);
    }

    op.reg = parseRegName(a = p, parser());
    if (op.reg != REG_UNDEF) {
        a.skipSpaces();
        if ((*a == '/' || *a == '-') &&
                (isGeneralReg(op.reg) || isFloatReg(op.reg) || isFloatControlReg(op.reg))) {
            while (*a != ',' && !endOfLine(a))
                ++a;
            if (isGeneralReg(op.reg)) {
                op.mode = M_MULT;
            } else if (isFloatReg(op.reg)) {
                op.mode = M_FPMLT;
            } else {
                op.mode = M_FCMLT;
            }
            scan = a;
            return op.getError();
        }
        if (isAddrReg(op.reg)) {
            op.mode = M_AREG;
        } else if (isDataReg(op.reg)) {
            op.mode = M_DREG;
        } else if (isFloatReg(op.reg)) {
            if (a.skipSpaces().expect(':')) {
                op.indexReg = parseRegName(a, parser());
                if (!isFloatReg(op.indexReg))
                    return op.setErrorIf(p, UNKNOWN_OPERAND);
                op.mode = M_FSICO;
            } else {
                op.mode = M_FPREG;
            }
        } else if (op.reg == REG_USP) {
            op.mode = M_USP;
        } else if (op.reg == REG_CCR) {
            op.mode = M_CCR;
        } else if (op.reg == REG_SR) {
            op.mode = M_SR;
        } else if (op.reg == REG_FPCR) {
            op.mode = M_FPCR;
        } else if (op.reg == REG_FPSR) {
            op.mode = M_FPSR;
        } else if (op.reg == REG_FPIAR) {
            op.mode = M_FPIAR;
        } else if (isControlReg(op.reg)) {
            op.mode = M_CREG;
        } else {
            return op.setError(p, REGISTER_NOT_ALLOWED);
        }
        scan = a;
        return OK;
    }
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_LABEL;
    scan = p;
    return OK;
}

InsnSize AsmInsn::parseInsnSize() {
    StrScanner p{name()};
    p.trimStart([](char c) { return c != '.'; });
    auto eos = const_cast<char *>(p.str());
    const auto isize = parseSize(p);
    *eos = 0;
    return _isize = isize;
}

#if !defined(LIBASM_ASM_NOFLOAT) && !defined(LIBASM_MC68000_NOFPU)

namespace {

uint64_t convert2pbcd(uint64_t bin, uint8_t digits) {
    uint64_t pbcd = 0;
    uint8_t shift = 0;
    while (digits-- > 0) {
        pbcd |= (bin % 10) << shift;
        bin /= 10;
        shift += 4;
    }
    return pbcd;
}

}  // namespace

Error AsmInsn::emitExtendedReal(const float80_t &value, uint8_t pos) {
    emitUint16(value.tag(), pos);
    emitUint16(0, pos + 2);
    return emitUint64(value.significand(), pos + 4);
}

Error AsmInsn::emitDecimalString(const float80_t &value, uint8_t pos) {
    if (value.isInf()) {
    infinity:
        const auto tag = (value.tag() & float80_t::SGN_MASK) | float80_t::EXP_MASK;
        emitUint16(tag, pos);
        emitUint16(0, pos += 2);
        emitUint64(0, pos += 2);
    } else if (value.isNan()) {
    not_a_number:
        const auto tag = (value.tag() & float80_t::SGN_MASK) | float80_t::EXP_MASK;
        emitUint16(tag, pos);
        emitUint16(0, pos += 2);
        emitUint64(UINT64_C(0xC000000000000000), pos += 2);
    } else {
        uint_fast16_t sign = value.isNegative() ? 0x8000 : 0x0000;
        int_fast16_t exp10;
        fixed64_t sig;
        auto exp = value.decompose(sig, exp10);
        const auto ndigits = 17;
        uint8_t digits[ndigits];
        if (sig.decompose(exp, digits, ndigits))
            ++exp10;
        if (exp >= 1000) {
            setErrorIf(OVERFLOW_RANGE);
            goto infinity;
        }
        if (exp <= -1000) {
            setErrorIf(UNDERFLOW_RANGE);
            goto not_a_number;
        }
        if (exp10 < 0) {
            sign |= 0x4000;
            exp10 = -exp10;
        }
        emitUint16(sign | convert2pbcd(exp10, 3), pos);
        emitByte(0, pos += 2);
        uint_fast8_t data = 0;
        for (auto i = 0; i < ndigits; ++i) {
            if (i % 2 == 0) {
                data |= digits[i];
                emitByte(data, ++pos);
            } else {
                data = digits[i] << 4;
            }
        }
    }
    return getError();
}

#endif

Error AsmMc68000::defineDataConstant(
        AsmInsn &insn, StrScanner &scan, Mc68881Type type, ErrorAt &_error) const {
    insn.insnBase().align(2);
    ErrorAt error;
    do {
        scan.skipSpaces();
        ErrorAt exprErr;
        auto p = scan;
        const auto val = parseExpr(p, exprErr);
        if (!endOfLine(p) && *p != ',') {
            error.setErrorIf(scan, ILLEGAL_CONSTANT);
            break;
        }
        if (exprErr.hasError()) {
            error.setErrorIf(exprErr);
            break;
        }
#if defined(LIBASM_MC68000_NOFPU)
        (void)val;
#elif defined(LIBASM_ASM_NOFLOAT)
        (void)val;
        exprErr.setErrorIf(scan, FLOAT_NOT_SUPPORTED);
        insn.emitUint32(0, insn.length());
        insn.emitUint64(0, insn.length());
#else
        if (type == DATA_DCX)
            exprErr.setErrorIf(scan, insn.emitExtendedReal(val.getFloat(), insn.length()));
        if (type == DATA_DCP)
            exprErr.setErrorIf(scan, insn.emitDecimalString(val.getFloat(), insn.length()));
#endif
        scan = p;
        if (error.setErrorIf(exprErr) == NO_MEMORY)
            break;
    } while (scan.skipSpaces().expect(',') && error.getError() != NO_MEMORY);
    return _error.setError(error);
}

Error AsmMc68000::processPseudo(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    const auto at = scan;
    if (strcasecmp_P(insn.name(), TEXT_FPU) == 0) {
        const auto error = _opt_fpu.set(scan);
        return error ? _insn.setErrorIf(at, error) : OK;
    }
#if !defined(LIBASM_MC68000_NOFPU)
    if (strcasecmp_P(insn.name(), TEXT_DC_X) == 0)
        return defineDataConstant(insn, scan, DATA_DCX, _insn);
    if (strcasecmp_P(insn.name(), TEXT_DC_P) == 0)
        return defineDataConstant(insn, scan, DATA_DCP, _insn);
#endif
    return Assembler::processPseudo(scan, _insn);
}

namespace {
bool hasKFactor(const AsmInsn &insn, InsnSize size) {
    // FMOVE.P FPn, ea
    if (strcasecmp_P(insn.name(), TEXT_FMOVE) == 0 && size == ISZ_PBCD &&
            insn.srcOp.mode == M_FPREG) {
        const auto mode = insn.dstOp.mode;
        return (mode >= M_AIND && mode <= M_INDX) || mode == M_AWORD || mode == M_ALONG;
    }
    return false;
}
}  // namespace

Error AsmMc68000::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    const auto isize = insn.parseInsnSize();
    if (isize == ISZ_ERROR)
        return _insn.setError(scan, ILLEGAL_SIZE);

    if (hasOperand(_cpuSpec, insn)) {
        if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
            return _insn.setError(insn.srcOp);
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
                return _insn.setError(insn.dstOp);
            if (hasKFactor(insn, isize)) {
                if (parseKFactor(scan, insn.dstOp) && insn.dstOp.hasError())
                    return _insn.setError(insn.dstOp);
            }
        }
    }
    scan.skipSpaces();

    if (searchName(_cpuSpec, insn))
        return _insn.setError(insn.srcOp, insn);

    insn.setErrorIf(insn.srcOp);
    insn.setErrorIf(insn.dstOp);
    const auto src = insn.src();
    const auto dst = insn.dst();
    if (src == M_MULT)
        encodeRegisterList(insn, insn.srcOp, insn.dstOp.mode == M_PDEC);
    if (dst == M_MULT)
        encodeRegisterList(insn, insn.dstOp);
    if (src == M_IMBIT) {
        auto bitno = insn.srcOp.val.getUnsigned();
        if (insn.srcOp.mode != M_IMDAT) {
            insn.setErrorIf(insn.srcOp, OPERAND_NOT_ALLOWED);
            bitno = 0;
        }
        if (insn.oprSize() == SZ_BYTE && bitno >= 8) {
            insn.setErrorIf(insn.srcOp, ILLEGAL_BIT_NUMBER);
            bitno &= 7;
        }
        if (insn.oprSize() == SZ_LONG && bitno >= 32) {
            insn.setErrorIf(insn.srcOp, ILLEGAL_BIT_NUMBER);
            bitno &= 0x1F;
        }
        insn.emitOperand16(bitno);
    }
    const auto osize = (isize == ISZ_NONE) ? insn.oprSize() : OprSize(isize);
    emitOprSize(insn, isize);
    encodeOperand(insn, osize, insn.srcOp, src, insn.srcPos());
    encodeOperand(insn, osize, insn.dstOp, dst, insn.dstPos());
    insn.emitInsn();
    if (insn.getError() == ILLEGAL_SIZE)
        insn.setAt(_insn.errorAt());
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
