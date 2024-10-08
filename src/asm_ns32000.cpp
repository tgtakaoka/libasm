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

#include "asm_ns32000.h"
#include <stdlib.h>
#include "table_ns32000.h"
#include "text_ns32000.h"

namespace libasm {
namespace ns32000 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::option;

using text::ns32000::TEXT_FPU_NS32081;
using text::ns32000::TEXT_MMU_NS32082;
using text::ns32000::TEXT_PMMU;

namespace {

// clang-format off
constexpr char OPT_TEXT_PMMU[] PROGMEM = "pmmu";
constexpr char OPT_DESC_PMMU[] PROGMEM = "memory management unit";

constexpr char TEXT_dBLKB[]   PROGMEM = ".blkb";
constexpr char TEXT_dBLKD[]   PROGMEM = ".blkd";
constexpr char TEXT_dBLKW[]   PROGMEM = ".blkw";
constexpr char TEXT_dDOUBLE[] PROGMEM = ".double";
constexpr char TEXT_dFLOAT[]  PROGMEM = ".float";
constexpr char TEXT_dLONG[]   PROGMEM = ".long";
constexpr char TEXT_dSPACE[]  PROGMEM = ".space";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_dALIGN,  &Assembler::alignOrigin},
    {TEXT_dASCII,  &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_dBLKB,   &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_dBLKD,   &Assembler::allocateSpaces,     Assembler::DATA_LONG},
    {TEXT_dBLKW,   &Assembler::allocateSpaces,     Assembler::DATA_WORD},
    {TEXT_dBYTE,   &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_dDOUBLE, &Assembler::defineDataConstant, Assembler::DATA_LONG},
#if !defined(LIBASM_NS32000_NOFPU)
    {TEXT_dFLOAT,  &Assembler::defineDataConstant, Assembler::DATA_FLOAT32},
    {TEXT_dLONG,   &Assembler::defineDataConstant, Assembler::DATA_FLOAT64},
#endif
    {TEXT_dORG,    &Assembler::defineOrigin},
    {TEXT_dSPACE,  &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_dWORD,   &Assembler::defineDataConstant, Assembler::DATA_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

struct Ns32000NumberParser final : NationalNumberParser, Singleton<Ns32000NumberParser> {
    Ns32000NumberParser() : NationalNumberParser(/* 'X','H' */ 0, 'B', /* 'O' or */ 'Q', 'D') {}
};

struct Ns32000SymbolParser final : SimpleSymbolParser, Singleton<Ns32000SymbolParser> {
    Ns32000SymbolParser() : SimpleSymbolParser(PSTR_UNDER_DOT) {}
    bool instructionLetter(char c) const override {
        return SimpleSymbolParser::instructionLetter(c) || c == '.';
    }
};

struct Ns32000LetterParser final : LetterParser, Singleton<Ns32000LetterParser> {
    char stringDelimiter(StrScanner &scan) const override { return scan.expect('"'); }
    char readLetter(StrScanner &scan, ErrorAt &error, char delim) const override {
        auto c = *scan++;
        if (c == '\\' && *scan) {
            c = *scan++;
        } else if (c == delim) {
            error.setErrorIf(ILLEGAL_CONSTANT);
        }
        return c;
    }
};

}  // namespace

const ValueParser::Plugins &AsmNs32000::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return Ns32000NumberParser::singleton(); }
        const CommentParser &comment() const override { return SharpCommentParser::singleton(); }
        const SymbolParser &symbol() const override { return Ns32000SymbolParser::singleton(); }
        const LetterParser &letter() const override { return Ns32000LetterParser::singleton(); }
        const LocationParser &location() const override {
            return DotStarLocationParser::singleton();
        }
    } PLUGINS{};
    return PLUGINS;
}

AsmNs32000::AsmNs32000(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_fpu),
      Config(TABLE),
      _opt_fpu(this, &Assembler::setFpu, OPT_TEXT_FPU, OPT_DESC_FPU, &_opt_pmmu),
      _opt_pmmu(this, &AsmNs32000::setPmmu, OPT_TEXT_PMMU, OPT_DESC_PMMU) {
    reset();
}

void AsmNs32000::reset() {
    Assembler::reset();
    setFpuType(FPU_NONE);
    setMmuType(MMU_NONE);
}

Error AsmNs32000::setFpu(StrScanner &scan) {
    if (scan.iequals_P(TEXT_none)) {
        setFpuType(FPU_NONE);
#if !defined(LIBASM_NS32000_NOFPU)
    } else if (scan.iequals_P(TEXT_FPU_NS32081)) {
        setFpuType(FPU_NS32081);
#endif
    } else {
        return UNKNOWN_OPERAND;
    }
    return OK;
}

Error AsmNs32000::setPmmu(StrScanner &scan) {
    if (scan.iequals_P(TEXT_none)) {
        setMmuType(MMU_NONE);
#if !defined(LIBASM_NS32000_NOMMU)
    } else if (scan.iequals_P(TEXT_MMU_NS32082)) {
        setMmuType(MMU_NS32082);
#endif
    } else {
        return UNKNOWN_OPERAND;
    }
    return OK;
}

Error AsmNs32000::parseStrOptNames(StrScanner &scan, Operand &op, bool braket) const {
    auto p = scan;
    uint8_t strOpt = 0;
    while (true) {
        const auto name = parseStrOptName(p);
        if (name == STROPT_UNDEF)
            return UNKNOWN_OPERAND;
        if (strOpt & uint8_t(name))
            return op.setError(ILLEGAL_OPERAND);
        strOpt |= uint8_t(name);
        if (p.skipSpaces().expect(']') || (!braket && endOfLine(p)))
            break;
        if (p.expect(',') || (!braket && p.expect('/'))) {
            p.skipSpaces();
            continue;
        }
        return UNKNOWN_OPERAND;
    }
    op.val.setUnsigned(strOpt);
    op.mode = M_SOPT;
    scan = p;
    return OK;
}

Error AsmNs32000::parseConfigNames(StrScanner &scan, Operand &op) const {
    auto p = scan;
    uint8_t configs = 0;
    while (true) {
        if (p.expect(']'))
            break;
        const auto name = parseConfigName(p);
        if (name == CONFIG_UNDEF)
            return UNKNOWN_OPERAND;
        configs |= uint8_t(name);
        if (p.skipSpaces().expect(']'))
            break;
        if (!p.expect(','))
            return UNKNOWN_OPERAND;
        p.skipSpaces();
    }
    op.val.setUnsigned(configs);
    op.mode = configs ? M_CONF : M_NONE;
    scan = p;
    return OK;
}

Error AsmNs32000::parseRegisterList(StrScanner &scan, Operand &op) const {
    auto p = scan;
    uint8_t list = 0;
    uint8_t n = 0;
    while (true) {
        const auto name = parseRegName(p);
        if (!isGeneric(name))
            return UNKNOWN_OPERAND;
        list |= 1U << encodeRegName(name);
        n++;
        if (p.skipSpaces().expect(']'))
            break;
        if (!p.expect(','))
            return UNKNOWN_OPERAND;
        p.skipSpaces();
    }
    op.val.setUnsigned(list);
    op.mode = M_RLST;
    scan = p;
    return OK;
}

Error AsmNs32000::parseBaseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('@')) {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_ABS;
        scan = p;
        return OK;
    }

    if (p.expect('[')) {
        p.skipSpaces();
        // parseConfigName can handle empty list "[]".
        if (parseConfigNames(p, op) != UNKNOWN_OPERAND ||
                parseRegisterList(p, op) != UNKNOWN_OPERAND ||
                parseStrOptNames(p, op, true) != UNKNOWN_OPERAND) {
            scan = p;
            return op.getError();
        }
        return op.setError(p, MISSING_CLOSING_BRACKET);
    }

    if (parseStrOptNames(p, op) == OK) {
        scan = p;
        return OK;
    }

    auto l = p;
    if (parser().locationSymbol(l)) {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_REL;
        op.reg = REG_PC;
        scan = p;
        return OK;
    }

    const auto preg = parsePregName(p);
    if (preg != PREG_UNDEF) {
        op.val.setUnsigned(encodePregName(preg));
        op.mode = M_PREG;
        scan = p;
        return OK;
    }

    const auto mreg = parseMregName(p);
    if (mreg != MREG_UNDEF) {
        op.val.setUnsigned(encodeMregName(mreg));
        op.mode = M_MREG;
        scan = p;
        return OK;
    }

    auto reg = parseRegName(p);
    if (reg != REG_UNDEF) {
        if (isGeneric(reg)) {
            op.reg = reg;
            op.mode = M_GREG;
            scan = p;
            return OK;
        }

        if (isFloat(reg)) {
            op.reg = reg;
            op.mode = M_FREG;
            scan = p;
            return OK;
        }

        if (reg == REG_TOS) {
            op.mode = M_TOS;
            scan = p;
            return OK;
        }
        if (reg == REG_EXT) {
            if (!p.expect('('))
                return op.setError(p, UNKNOWN_OPERAND);
            op.val = parseInteger(p, op, ')');
            if (op.hasError())
                return op.getError();
            if (!p.skipSpaces().expect(')'))
                return op.setErrorIf(p, MISSING_CLOSING_PAREN);
            if (*p.skipSpaces() == '+' || *p == '-') {
                ErrorAt error;
                op.disp2 = parseInteger(p, error);
                op.setErrorIf(error);
                if (op.hasError())
                    return op.getError();
                p.skipSpaces();
            }
            if (*p == '[' || endOfLine(p) || *p == ',') {
                op.mode = M_EXT;
                scan = p;
                return OK;
            }
            return op.setError(UNKNOWN_OPERAND);
        }
        return op.setError(scan, UNKNOWN_REGISTER);
    }

    op.val = parseExpr(p, op);
    if (op.hasError())
        return op.getError();
    if (op.val.isFloat()) {
        op.mode = M_IMM;
        op.size = SZ_OCTA;
        scan = p;
        return op.setOK();
    }
    op.size = SZ_QUAD;
    if (endOfLine(p.skipSpaces()) || *p == ',') {
        op.mode = M_IMM;  // M_REL
        scan = p;
        return OK;
    }

    if (!p.expect('('))
        return op.setError(p, UNKNOWN_OPERAND);
    const auto r = p;
    reg = parseRegName(p);
    if (reg != REG_UNDEF) {
        if (!p.expect(')'))
            return op.setErrorIf(p, MISSING_CLOSING_PAREN);
        if (isGeneric(reg)) {
            op.reg = reg;
            op.mode = M_RREL;
            scan = p;
            return OK;
        }
        if (reg == REG_FP || reg == REG_SP || reg == REG_SB || reg == REG_PC || reg == REG_EXT) {
            op.reg = reg;
            op.mode = (reg == REG_EXT) ? M_EXT : M_MEM;
            scan = p;
            return OK;
        }
        return op.setErrorIf(r, UNKNOWN_OPERAND);
    }

    op.disp2 = op.val;
    ErrorAt error;
    op.val = parseInteger(p, error, ')');
    op.setErrorIf(error);
    if (op.hasError())
        return op.getError();
    if (!p.skipSpaces().expect('('))
        return op.setErrorIf(p, UNKNOWN_OPERAND);
    const auto x = p;
    reg = parseRegName(p);
    if (reg != REG_UNDEF) {
        if (!p.expect(')'))
            return op.setErrorIf(p, MISSING_CLOSING_PAREN);
        if (!p.skipSpaces().expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        if (reg == REG_FP || reg == REG_SP || reg == REG_SB || reg == REG_EXT) {
            op.reg = reg;
            op.mode = (reg == REG_EXT) ? M_EXT : M_MREL;
            scan = p;
            return OK;
        }
    }

    return op.setErrorIf(x, UNKNOWN_OPERAND);
}

Error AsmNs32000::parseOperand(StrScanner &scan, Operand &op) const {
    if (parseBaseOperand(scan, op))
        return op.getError();
    if (op.mode == M_GREG || op.mode == M_RREL || op.mode == M_MREL || op.mode == M_ABS ||
            op.mode == M_EXT || op.mode == M_TOS || op.mode == M_MEM || op.mode == M_REL) {
        auto p = scan;
        if (!p.skipSpaces().expect('['))
            return OK;
        const auto indexp = p;
        const auto index = parseRegName(p);
        if (!isGeneric(index))
            return op.setError(indexp, UNKNOWN_OPERAND);
        if (!p.skipSpaces().expect(':'))
            return op.setError(p, UNKNOWN_OPERAND);
        const auto sizep = p;
        const auto indexSize = parseIndexSize(p);
        if (indexSize == SZ_NONE)
            return op.setError(sizep, UNKNOWN_OPERAND);
        if (!p.skipSpaces().expect(']'))
            return op.setError(p, MISSING_CLOSING_BRACKET);
        scan = p;
        op.index = index;
        op.size = indexSize;
    }
    return OK;
}

namespace {

uint16_t reverseBits(uint8_t bits) {
    bits = (bits & 0x55) << 1 | (bits & 0xAA) >> 1;
    bits = (bits & 0x33) << 2 | (bits & 0xCC) >> 2;
    return bits << 4 | bits >> 4;
}

uint8_t encodeScaledIndex(OprSize indexSize) {
    switch (indexSize) {
    case SZ_BYTE:
        return 0x1C;
    case SZ_WORD:
        return 0x1D;
    case SZ_QUAD:
        return 0x1E;
    case SZ_OCTA:
        return 0x1F;
    default:
        return 0;
    }
}

void embedOprField(AsmInsn &insn, OprPos pos, uint8_t opr) {
    if (pos == P_GEN1) {
        opr &= 0x1F;
        opr <<= 3;
        if (insn.hasPostfix())
            insn.embedPostfix(opr);
        else
            insn.embed(opr);
    }
    if (pos == P_GEN2) {
        opr &= 0x1F;
        insn.embedPostfix(opr >> 2);
        insn.embed(opr << 6);
    }
    if (pos == P_REG) {
        opr &= 0x7;
        insn.embed(opr << 3);
    }
    if (pos == P_SHORT) {
        opr &= 0xF;
        insn.embedPostfix(opr >> 1);
        insn.embed(opr << 7);
    }
}

}  // namespace

void AsmNs32000::emitDisplacement(
        AsmInsn &insn, const Operand &op, const Value &disp, Error error) const {
    constexpr auto INT7_MIN = INT32_C(-0x40);
    constexpr auto INT7_MAX = INT32_C(0x3F);
    constexpr auto INT14_MIN = INT32_C(-0x2000);
    constexpr auto INT14_MAX = INT32_C(0x1FFF);
    constexpr auto INT30_MIN = INT32_C(-0x1F00'0000);
    constexpr auto INT30_MAX = INT32_C(0x1FFF'FFFF);
    if (!disp.overflow(INT7_MAX, INT7_MIN)) {
        insn.emitOperand8(disp.getUnsigned() & UINT8_C(0x7F));
    } else if (!disp.overflow(INT14_MAX, INT14_MIN)) {
        const auto mark = UINT16_C(0x8000);
        insn.emitOperand16((disp.getUnsigned() & UINT16_C(0x3FFF)) | mark);
    } else {
        auto val = disp.getSigned();
        if (val < INT30_MIN) {
            insn.setErrorIf(op, error);
            val = INT30_MIN;
        } else if (val > INT30_MAX) {
            insn.setErrorIf(op, error);
            val = INT30_MAX;
        }
        const auto mark = UINT32_C(0xC000'0000);
        insn.emitOperand32((val & UINT32_C(0x3FFF'FFFF)) | mark);
    }
}

void AsmNs32000::emitLength(AsmInsn &insn, const Operand &op) const {
    auto len = 0;
    if (op.val.isNegative() || op.val.isZero()) {
        insn.setErrorIf(op, ILLEGAL_CONSTANT);
    } else if (op.isOK()) {
        len = op.val.getUnsigned() - 1;
        if (insn.size() == SZ_QUAD) {
            if (op.val.overflow(4))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            len *= 4;
        } else if (insn.size() == SZ_WORD) {
            if (op.val.overflow(8))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            len *= 2;
        } else {
            if (op.val.overflow(16))
                insn.setErrorIf(op, OVERFLOW_RANGE);
        }
    }
    Value disp;
    emitDisplacement(insn, op, disp.setUnsigned(len));
}

void AsmNs32000::emitBitField(
        AsmInsn &insn, AddrMode mode, const Operand &off, const Operand &len) const {
    if (mode == M_BFOFF)
        return;
    if (off.val.overflow(7))
        insn.setErrorIf(off, ILLEGAL_BIT_NUMBER);
    const auto offset = off.val.getUnsigned();
    auto length = len.val.getUnsigned();
    if (len.val.isNegative() || len.val.isZero()) {
        insn.setErrorIf(len, ILLEGAL_CONSTANT);
        length = 1;
    } else if (len.getError()) {
        length = 1;  // undefined symbol
    }
    if (offset + length > 32)
        insn.setErrorIf(off, OVERFLOW_RANGE);
    const auto data = (offset << 5) | ((length - 1) & 0x1F);
    insn.emitOperand8(data);
}

void AsmNs32000::emitImmediate(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    const auto size = insn.size();
    if (mode != M_FENR) {
        if (size == SZ_BYTE || mode == M_GENC) {
            insn.emitOperand8(op.val.getUnsigned());
        } else if (size == SZ_WORD) {
            insn.emitOperand16(op.val.getUnsigned());
        } else if (size == SZ_QUAD) {
            insn.emitOperand32(op.val.getUnsigned());
        }
#if !defined(LIBASM_NS32000_NOFPU)
    } else {
        if (size == SZ_QUAD) {
#if defined(LIBASM_ASM_NOFLOAT)
            insn.setErrorIf(op, FLOAT_NOT_SUPPORTED);
            insn.emitOperand32(0);
#else
            insn.emitOpFloat32(op.val.getFloat());
#endif
        } else if (size == SZ_OCTA) {
#if defined(LIBASM_ASM_NOFLOAT)
            insn.setErrorIf(op, FLOAT_NOT_SUPPORTED);
            insn.emitOperand32(0);
            insn.emitOperand32(0);
#else
            insn.emitOpFloat64(op.val.getFloat());
#endif
        }
#endif
    }
}

uint8_t AsmNs32000::encodeGenericField(AddrMode mode, RegName reg) const {
    switch (mode) {
    case M_GREG:
    case M_FREG:
        return encodeRegName(reg);
    case M_RREL:
        return encodeRegName(reg) | 0x08;
    case M_MREL:
        if (reg == REG_FP)
            return 0x10;
        if (reg == REG_SP)
            return 0x11;
        if (reg == REG_SB)
            return 0x12;
        break;
    case M_IMM:
        return 0x14;
    case M_ABS:
        return 0x15;
    case M_EXT:
        return 0x16;
    case M_TOS:
        return 0x17;
    case M_REL:
    case M_MEM:
        if (reg == REG_FP)
            return 0x18;
        if (reg == REG_SP)
            return 0x19;
        if (reg == REG_SB)
            return 0x1A;
        if (reg == REG_PC)
            return 0x1B;
    default:
        break;
    }
    return 0;
}

void AsmNs32000::emitIndexByte(AsmInsn &insn, const Operand &op) const {
    if (op.index == REG_UNDEF)
        return;
    const auto indexByte = (encodeGenericField(op.mode, op.reg) << 3) | encodeRegName(op.index);
    insn.emitOperand8(indexByte);
}

void AsmNs32000::emitGeneric(AsmInsn &insn, AddrMode mode, const Operand &op, OprPos pos) const {
    const auto field = (op.index == REG_UNDEF) ? encodeGenericField(op.mode, op.reg)
                                               : encodeScaledIndex(op.size);
    embedOprField(insn, pos, field);
    switch (op.mode) {
    case M_ABS: {
        if (checkAddr(op.val.getUnsigned()))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        Value disp;
        // Sign extends 24 bit address into 32 bit.
        disp.setSigned(signExtend(op.val.getUnsigned(), addressWidth()));
        emitDisplacement(insn, op, disp);
        break;
    }
    case M_RREL:
    case M_MEM:
        if (op.mode == M_MEM && op.reg == REG_PC) {
            emitRelative(insn, op);
        } else {
            emitDisplacement(insn, op, op.val);
        }
        break;
    case M_REL:
        emitRelative(insn, op);
        break;
    case M_MREL:
    case M_EXT:
        emitDisplacement(insn, op, op.val);
        emitDisplacement(insn, op, op.disp2);
        break;
    case M_IMM:
        // emitImmediate(insn, op, mode == M_GENC ? SZ_BYTE : insn.size());
        emitImmediate(insn, mode, op);
        break;
    default:
        break;
    }
}

void AsmNs32000::emitRelative(AsmInsn &insn, const Operand &op) const {
    const auto base = insn.address();
    const auto target = op.getError() ? base : op.val.getUnsigned();
    insn.setErrorIf(op, checkAddr(target));
    Value delta;
    emitDisplacement(insn, op, delta.setSigned(target - base), OPERAND_TOO_FAR);
}

void AsmNs32000::emitOperand(AsmInsn &insn, AddrMode mode, OprSize size, const Operand &op,
        OprPos pos, const Operand &prevOp) const {
    constexpr uint8_t SAVE = 0x62;
    constexpr uint8_t RESTORE = 0x72;
    switch (mode) {
    case M_GREG:
        embedOprField(insn, pos, encodeRegName(op.reg));
        break;
    case M_PREG:
    case M_MREG:
    case M_CONF:
    case M_SOPT:
        embedOprField(insn, pos, op.val.getUnsigned());
        break;
    case M_RLST:
        if (op.val.isZero() && (insn.opCode() == SAVE || insn.opCode() == RESTORE))
            insn.setErrorIf(op, OPCODE_HAS_NO_EFFECT);
        if (size == SZ_NONE) {  // PUSH
            insn.emitOperand8(op.val.getUnsigned());
        } else {  // POP
            insn.emitOperand8(reverseBits(op.val.getUnsigned()));
        }
        break;
    case M_FENR:
    case M_FENW:
        if (op.mode == M_FREG && size == SZ_OCTA && !isRegPair(op.reg))
            insn.setErrorIf(op, REGISTER_NOT_ALIGNED);
        goto emit_generic;
    case M_GENR:
    case M_GENW:
        if (op.mode == M_GREG && size == SZ_OCTA && !isRegPair(op.reg))
            insn.setErrorIf(op, REGISTER_NOT_ALIGNED);
        goto emit_generic;
    case M_GENC:
    case M_GENA:
    emit_generic:
        emitGeneric(insn, mode, op, pos);
        break;
    case M_INT4: {
        if (op.val.overflow(7, -8))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        embedOprField(insn, pos, op.val.getUnsigned());
        break;
    }
    case M_BFOFF:
    case M_BFLEN:
        emitBitField(insn, mode, prevOp, op);
        break;
    case M_REL:
        emitRelative(insn, op);
        break;
    case M_LEN32: {
        if (op.val.isNegative() || op.val.isZero())
            insn.setErrorIf(op, ILLEGAL_CONSTANT);
        if (op.val.overflow(32))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        emitDisplacement(insn, op, op.val);
        break;
    }
    case M_DISP:
        emitDisplacement(insn, op, op.val);
        break;
    case M_LEN16:
        emitLength(insn, op);
        break;
    default:
        break;
    }
}

Error AsmNs32000::processPseudo(StrScanner &scan, Insn &insn) {
    const auto at = scan;
    if (strcasecmp_P(insn.name(), TEXT_FPU) == 0) {
        const auto error = _opt_fpu.set(scan);
        return error ? insn.setErrorIf(at, error) : OK;
    }
    if (strcasecmp_P(insn.name(), TEXT_PMMU) == 0) {
        const auto error = _opt_pmmu.set(scan);
        return error ? insn.setErrorIf(at, error) : OK;
    }
    return Assembler::processPseudo(scan, insn);
}

Error AsmNs32000::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
        return _insn.setError(insn.srcOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
            return _insn.setError(insn.dstOp);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, insn.ex1Op) && insn.ex1Op.hasError())
            return _insn.setError(insn.ex1Op);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, insn.ex2Op) && insn.ex2Op.hasError())
            return _insn.setError(insn.ex2Op);
        scan.skipSpaces();
    }

    if (_insn.setErrorIf(insn.srcOp, TABLE.searchName(_cpuSpec, insn)))
        return _insn.getError();

    insn.setErrorIf(insn.srcOp);
    insn.setErrorIf(insn.dstOp);
    insn.setErrorIf(insn.ex1Op);
    insn.setErrorIf(insn.ex2Op);
    emitIndexByte(insn, insn.srcOp);
    emitIndexByte(insn, insn.dstOp);
    emitIndexByte(insn, insn.ex1Op);
    emitOperand(insn, insn.src(), insn.srcSize(), insn.srcOp, insn.srcPos(), insn.srcOp);
    emitOperand(insn, insn.dst(), insn.dstSize(), insn.dstOp, insn.dstPos(), insn.srcOp);
    emitOperand(insn, insn.ex1(), insn.size(), insn.ex1Op, insn.ex1Pos(), insn.dstOp);
    emitOperand(insn, insn.ex2(), insn.size(), insn.ex2Op, insn.ex2Pos(), insn.ex1Op);
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace ns32000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
