/*
 * Copyright 2024 Tadashi G. Takaoka
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

#include "asm_pdp11.h"
#include "dec_float.h"
#include "reg_pdp11.h"
#include "table_pdp11.h"
#include "text_common.h"

namespace libasm {
namespace pdp11 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::option;

namespace {

// clang-format off
constexpr char TEXT_dEVEN[]  PROGMEM = ".EVEN";
constexpr char TEXT_dODD[]   PROGMEM = ".ODD";
constexpr char TEXT_dFLT2[]  PROGMEM = ".FLT2";
constexpr char TEXT_dFLT4[]  PROGMEM = ".FLT4";
constexpr char TEXT_dRADIX[] PROGMEM = ".RADIX";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_dequal, &Assembler::defineOrigin},
    {TEXT_dBLKB,  &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_dBLKW,  &Assembler::allocateSpaces,     Assembler::DATA_WORD | Assembler::DATA_ALIGN2},
    {TEXT_dBYTE,  &Assembler::defineDataConstant, Assembler::DATA_BYTE_NO_STRING},
    {TEXT_dEVEN,  &Assembler::alignOrigin,        Assembler::ALIGN_EVEN},
    {TEXT_dODD,   &Assembler::alignOrigin,        Assembler::ALIGN_ODD},
    {TEXT_dWORD,  &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING | Assembler::DATA_ALIGN2},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmPdp11::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return Pdp11NumberParser::singleton(); }
        const SymbolParser &symbol() const override { return Pdp11SymbolParser::singleton(); }
        const LetterParser &letter() const override { return MotorolaLetterParser::singleton(); }
        const OperatorParser &operators() const override {
            return Pdp11OperatorParser::singleton();
        }
    } PLUGINS{};
    return PLUGINS;
}

AsmPdp11::AsmPdp11(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_implicitWord),
      Config(TABLE),
      _opt_implicitWord(
              this, &AsmPdp11::setImplicitWord, OPT_BOOL_IMPLICIT_WORD, OPT_DESC_IMPLICIT_WORD) {
    reset();
}

void AsmPdp11::reset() {
    Assembler::reset();
    setInputRadix(RADIX_8);
    setListRadix(RADIX_8);
    setImplicitWord(true);
}

Error AsmPdp11::setImplicitWord(bool enable) {
    _implicitWord = enable;
    return OK;
}

RegName AsmPdp11::parseRegExpr(StrScanner &scan, Operand &op, char delim) const {
    auto p = scan;
    if (p.expect('%')) {
        ErrorAt error;
        auto regno = parseInteger(p, error, delim);
        if (error.getError() == UNDEFINED_SYMBOL) {
            regno.setUnsigned(0);  // default to 0
            op.setErrorIf(error);
        }
        if (error.hasError() || regno.overflow(7)) {
            op.setErrorIf(scan, UNKNOWN_REGISTER);
            return REG_UNDEF;
        }
        scan = p;
        return decodeGeneralReg(regno.getUnsigned());
    }
    return parseRegName(scan);
}

// Check register deferred mode: -(Rn), (Rn), (Rn)+
Error AsmPdp11::isRegDeferred(StrScanner &scan, Operand &op) const {
    auto p = scan;
    const auto decrement = p.skipSpaces().expect('-');
    if (!p.skipSpaces().expect('('))
        return NOT_AN_EXPECTED;
    const auto regat = p.skipSpaces();
    const auto reg = parseRegExpr(p, op, ')');
    if (reg == REG_UNDEF)
        return NOT_AN_EXPECTED;
    if (p.skipSpaces().expect(')')) {
        const auto increment = p.skipSpaces().expect('+');
        if (!isGeneralReg(reg))
            return op.setErrorIf(regat, REGISTER_NOT_ALLOWED);
        if (decrement && increment)
            return op.setErrorIf(scan, UNKNOWN_OPERAND);
        op.reg = reg;
        op.mode = decrement ? M_DEC : (increment ? M_INC : M_REGD);
        scan = p;
        return OK;
    }
    return NOT_AN_EXPECTED;
}

// Parse register mode; (Rn), -(Rn), (Rn)+, nn(PC)
Error AsmPdp11::parseRegMode(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    if (isRegDeferred(p, op) == OK) {
        scan = p;
        return OK;
    }
    ErrorAt error;
    auto val = parseInteger(p, error);
    if (error.hasError())
        return NOT_AN_EXPECTED;
    op.setErrorIf(error);  // undefined symbol
    if (isRegDeferred(p, op) == OK && op.mode == M_REGD) {
        op.mode = M_IDX;
        op.val = val;
        scan = p;
        return OK;
    }
    return NOT_AN_EXPECTED;
}

Error AsmPdp11::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p)) {
        op.mode = M_NONE;
        scan = p;
        return OK;
    }

    const auto deferred = p.expect('@');
    auto regat = p.skipSpaces();
    const auto reg = parseRegExpr(p, op);
    if (reg != REG_UNDEF) {
        if (isFloatReg(reg)) {
            if (deferred)
                return op.setErrorIf(regat, REGISTER_NOT_ALLOWED);
            op.mode = M_FREG;
        } else {
            op.mode = deferred ? M_REGD : M_REG;
        }
        op.reg = reg;
        scan = p;
        return OK;
    }

    const auto immediate = p.expect('#');
    if (deferred && immediate) {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_ABS;
        scan = p;
        return OK;
    }

    regat = p.skipSpaces();
    if (parseRegMode(p, op) == OK) {
        if (immediate)
            return UNKNOWN_OPERAND;
        if (deferred)
            op.mode = static_cast<AddrMode>(op.mode + 1);
        scan = p;
        return OK;
    }

    if (immediate) {
        op.val = parseExpr(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_IMM;
        scan = p;
        return OK;
    }

    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = deferred ? M_RELD : M_REL;  // PC-relative
    scan = p;
    return OK;
}

Error AsmInsn::emitOperand32(uint32_t u32, uint8_t pos) {
    emitUint16(u32 >> 16, pos + 0);
    emitUint16(u32 >> 0, pos + 2);
    return getError();
}

Error AsmInsn::emitOperand64(uint64_t u64, uint8_t pos) {
    emitOperand32(u64 >> 32, pos + 0);
    emitOperand32(u64 >> 0, pos + 4);
    return getError();
}

#if !defined(LIBASM_ASM_NOFLOAT)

Error AsmInsn::emitDecFloat32(const float80_t &f80, uint8_t pos) {
    dec_float32_t f32{0};
    setErrorIf(f32.set(f80));
    emitOperand32(f32.bits(), pos);
    return getError();
}

Error AsmInsn::emitDecFloat64(const float80_t &f80, uint8_t pos) {
    dec_float64_t f64{0};
    setErrorIf(f64.set(f80));
    emitOperand64(f64.bits(), pos);
    return getError();
}

#endif

void AsmInsn::embed(Config::opcode_t data, OprPos pos) {
    if (pos == P_7700 || pos == P_0700 || pos == P_0300)
        data <<= 6;
    EntryInsnBase::embed(data & Entry::insnMask(pos));
}

uint_fast8_t AsmInsn::operandPos() const {
    uint_fast8_t pos = length();
    return pos ? pos : sizeof(Config::opcode_t);
}

namespace {
Config::opcode_t encodeMode(AddrMode mode, RegName reg) {
    if (mode == M_FREG)
        mode = M_REG;
    const auto m = mode - M_base;
    const auto r = encodeRegNo(reg);
    return (m << 3) | r;
}
}  // namespace

void AsmPdp11::encodeRelative(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const {
    const auto base = insn.address() + 2;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    insn.setErrorIf(op, checkAddr(target, true));
    const auto delta = branchDelta(base, target, insn, op);
    if (mode == M_REL8) {
        if (overflowDelta(delta, 9))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.embed(delta / 2, pos);
    } else if (mode == M_REL6) {
        if (delta > 0 || delta <= -128)
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.embed(-delta / 2, pos);
    }
}

void AsmPdp11::encodeImmediate(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const {
    const auto size = insn.size();
    insn.embed(encodeMode(M_INC, REG_PC), pos);
    if (mode == M_GENS || mode == M_GEND || mode == M_GENA) {
        if (op.val.isFloat())
            insn.setErrorIf(op, INTEGER_REQUIRED);
        if (size == SZ_BYTE) {
            if (op.val.overflowUint8())
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand16(op.val.getUnsigned());
        } else if (size == SZ_WORD) {
            if (op.val.overflowUint16())
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand16(op.val.getUnsigned());
        } else if (size == SZ_INTG) {
            if (op.val.overflowInt16())
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand16(op.val.getUnsigned());
        } else if (size == SZ_LONG) {
            if (op.val.overflowInt32())
                insn.setErrorIf(op, OVERFLOW_RANGE);
            const auto val = op.val.getInteger();
            insn.emitOperand32(val, insn.operandPos());
        }
    }
    if (mode == M_GENF || mode == M_GENG) {
        if (size == SZ_FLOT) {
#if defined(LIBASM_ASM_NOFLOAT)
            insn.setErrorIf(op, FLOAT_NOT_SUPPORTED);
            insn.emitOperand32(0, insn.operandPos());
#else
            insn.emitDecFloat32(op.val.getFloat());
#endif
        } else if (size == SZ_DUBL) {
#if defined(LIBASM_ASM_NOFLOAT)
            insn.setErrorIf(op, FLOAT_NOT_SUPPORTED);
            insn.emitOperand64(0, insn.operandPos());
#else
            insn.emitDecFloat64(op.val.getFloat());
#endif
        }
    }
}

void AsmPdp11::encodeRegister(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const {
    if (mode == M_AC03 && !isAc03Reg(op.reg))
        insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
    const auto no = encodeRegNo(op.reg);
    insn.embed(no, pos);
}

void AsmPdp11::encodeGeneral(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const {
    if (op.mode == M_REG && mode == M_GENA)
        insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
    if (op.mode == M_IMM && (mode == M_GEND || mode == M_GENA || mode == M_GENG))
        insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
    if (op.mode == M_REG && (mode == M_GENF || mode == M_GENG))
        insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
    if (op.mode == M_FREG && (mode == M_GENS || mode == M_GENA))
        insn.setErrorIf(op, REGISTER_NOT_ALLOWED);

    if (op.mode == M_REL || op.mode == M_RELD) {
        const auto base = insn.address() + insn.operandPos() + 2;
        const auto target = op.getError() ? base : op.val.getUnsigned();
        insn.setErrorIf(op, checkAddr(target, insn.size() != SZ_BYTE));
        const auto delta = branchDelta(base, target, insn, op);
        if (overflowDelta(delta, 16))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.embed(encodeMode(op.mode == M_REL ? M_IDX : M_IDXD, REG_PC), pos);
        if (delta % 2 != 0 && op.mode == M_RELD)
            insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
        insn.emitOperand16(delta);
        return;
    }
    if (op.mode == M_IMM) {
        encodeImmediate(insn, op, mode, pos);
        return;
    }
    if (op.mode == M_ABS) {
        const auto addr = op.val.getUnsigned();
        insn.setErrorIf(op, checkAddr(addr, insn.size() != SZ_BYTE));
        insn.embed(encodeMode(M_INCD, REG_PC), pos);
        insn.emitOperand16(addr);
        return;
    }
    insn.embed(encodeMode(op.mode, op.reg), pos);
    if (op.reg == REG_PC) {
        if (op.mode == M_INC || op.mode == M_INCD) {
            insn.setErrorIf(op, NOT_AN_EXPECTED);
            insn.emitOperand16(0);
        }
        if (op.mode == M_DEC || op.mode == M_DECD)
            insn.setErrorIf(op, NOT_AN_EXPECTED);
    }
    if (op.mode == M_IDX || op.mode == M_IDXD) {
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        const auto disp = op.val.getUnsigned();
        if (op.reg == REG_PC && disp % 2 != 0 && (op.mode == M_IDXD || insn.size() != SZ_BYTE))
            insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
        insn.emitOperand16(disp);
    }
}

void AsmPdp11::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const {
    switch (mode) {
    case M_GENR:
    case M_AC03:
        encodeRegister(insn, op, mode, pos);
        break;
    case M_GEND:
    case M_GENG:
    case M_GENA:
    case M_GENF:
    case M_GENS:
        encodeGeneral(insn, op, mode, pos);
        break;
    case M_REL8:
    case M_REL6:
        encodeRelative(insn, op, mode, pos);
        break;
    case M_IMM3:
        if (op.val.overflow(7))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        /* Fall-through */
    case M_IMM6:
        if (op.val.overflow(63))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        /* Fall-through */
    case M_IMM8:
        if (op.val.overflow(UINT8_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embed(op.val.getUnsigned(), pos);
        break;
    default:
        break;
    }
}

Error AsmPdp11::defineDecFloat(
        StrScanner &scan, AsmInsn &insn, DecType type, ErrorAt &_error) const {
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
        if (type == DATA_FLT2) {
#if defined(LIBASM_ASM_NOFLOAT)
            (void)val;
            exprErr.setErrorIf(scan, FLOAT_NOT_SUPPORTED);
            insn.emitOperand32(0, insn.length());
#else
            exprErr.setErrorIf(scan, insn.emitDecFloat32(val.getFloat(), insn.length()));
#endif
        } else if (type == DATA_FLT4) {
#if defined(LIBASM_ASM_NOFLOAT)
            (void)val;
            exprErr.setErrorIf(scan, FLOAT_NOT_SUPPORTED);
            insn.emitOperand64(0, insn.length());
#else
            exprErr.setErrorIf(scan, insn.emitDecFloat64(val.getFloat(), insn.length()));
#endif
        }
        scan = p;
        if (error.setErrorIf(exprErr) == NO_MEMORY)
            break;
    } while (scan.skipSpaces().expect(','));
    return _error.setError(error);
}

Error AsmPdp11::defineAscii(StrScanner &scan, Insn &insn) const {
    ErrorAt error;
    do {
        auto p = scan;
        if (p.expect('<')) {
            const auto val = parseInteger(p, error, '>');
            if (error.hasError())
                break;
            if (val.overflowUint8())
                error.setErrorIf(scan, OVERFLOW_RANGE);
            error.setErrorIf(p, insn.emitByte(val.getUnsigned()));
            ++p;
        } else {
            const auto delim = *p++;
            while (!p.expect(delim)) {
                const auto c = *p;
                if (c == 0)
                    return insn.setErrorIf(scan, MISSING_CLOSING_DELIMITER);
                error.setErrorIf(p, insn.emitByte(c));
                ++p;
            }
        }
        scan = p.skipSpaces();
    } while (!endOfLine(scan) && !error.hasError());
    return insn.setError(error);
}

Error AsmPdp11::processPseudo(StrScanner &scan, Insn &_insn) {
    if (strcasecmp_P(_insn.name(), TEXT_dRADIX) == 0) {
        const auto saved = _inputRadix;
        setInputRadix(RADIX_10);  // argument is always parsed with RADIX 10
        int32_t radix;
        auto p = scan;
        const auto error = parseIntOption(p, radix);
        if (error == OK && (radix == 2 || radix == 8 || radix == 10)) {
            scan = p;
        } else {
            radix = saved;
        }
        setInputRadix(static_cast<Radix>(radix));
        return error;
    }
    if (strcasecmp_P(_insn.name(), TEXT_dASCII) == 0)
        return defineAscii(scan, _insn);
    AsmInsn insn(_insn);
    if (strcasecmp_P(insn.name(), TEXT_dFLT2) == 0)
        return defineDecFloat(scan, insn, DATA_FLT2, _insn);
    if (strcasecmp_P(insn.name(), TEXT_dFLT4) == 0)
        return defineDecFloat(scan, insn, DATA_FLT4, _insn);
    return Assembler::processPseudo(scan, _insn);
}

Error AsmPdp11::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    const auto error = TABLE.hasName(cpuType(), insn);
    if (_implicitWord && error != OK) {
        StrScanner p = _insn.errorAt();
        insn.setOK();  // clear UNKNOWN_INSTRUCTION
        const auto opc = parseInteger(p, insn);
        if (insn.hasError())
            return _insn.setError(insn);
        if (opc.overflowUint16())
            insn.setError(_insn.errorAt(), OVERFLOW_RANGE);
        insn.setOpCode(opc.getUnsigned());
        scan = p;
        insn.emitInsn();
    } else if (error == OK) {
        if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
            return _insn.setError(insn.srcOp);
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
                return _insn.setError(insn.dstOp);
            scan.skipSpaces();
        }

        if (_insn.setErrorIf(insn.srcOp, TABLE.searchName(cpuType(), insn)))
            return _insn.getError();
        insn.setErrorIf(insn.srcOp);
        insn.setErrorIf(insn.dstOp);
        encodeOperand(insn, insn.srcOp, insn.src(), insn.srcPos());
        encodeOperand(insn, insn.dstOp, insn.dst(), insn.dstPos());
        insn.emitInsn();
    }
    return _insn.setError(insn);
}

}  // namespace pdp11
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
