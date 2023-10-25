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

using text::ns32000::TEXT_FPU;
using text::ns32000::TEXT_FPU_NS32081;
using text::ns32000::TEXT_MMU_NS32082;
using text::ns32000::TEXT_none;
using text::ns32000::TEXT_PMMU;

namespace {

// clang-format off
constexpr char OPT_TEXT_FPU[]  PROGMEM = "fpu";
constexpr char OPT_DESC_FPU[]  PROGMEM = "floating point co-processor";
constexpr char OPT_TEXT_PMMU[] PROGMEM = "pmmu";
constexpr char OPT_DESC_PMMU[] PROGMEM = "memory management unit";

constexpr char TEXT_dBLKB[]   PROGMEM = ".blkb";
constexpr char TEXT_dBLKD[]   PROGMEM = ".blkd";
constexpr char TEXT_dBLKW[]   PROGMEM = ".blkw";
constexpr char TEXT_dDOUBLE[] PROGMEM = ".double";
constexpr char TEXT_dSPACE[]  PROGMEM = ".space";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_dALIGN,  &Assembler::alignOrigin},
    {TEXT_dASCII,  &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_dBLKB,   &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_dBLKD,   &Assembler::allocateSpaces,     Assembler::DATA_LONG},
    {TEXT_dBLKW,   &Assembler::allocateSpaces,     Assembler::DATA_WORD},
    {TEXT_dBYTE,   &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_dDOUBLE, &Assembler::defineDataConstant, Assembler::DATA_LONG},
    {TEXT_dORG,    &Assembler::defineOrigin},
    {TEXT_dSPACE,  &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_dWORD,   &Assembler::defineDataConstant, Assembler::DATA_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

struct Ns32000SymbolParser final : SimpleSymbolParser {
    Ns32000SymbolParser() : SimpleSymbolParser(PSTR_UNDER_DOT) {}
    bool instructionLetter(char c) const override {
        return SimpleSymbolParser::instructionLetter(c) || c == '.';
    }
};

struct Ns32000LetterParser final : LetterParser {
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
        const NumberParser &number() const override { return _number; }
        const CommentParser &comment() const override { return SharpCommentParser::singleton(); }
        const SymbolParser &symbol() const override { return _symbol; }
        const LetterParser &letter() const override { return _letter; }
        const LocationParser &location() const override { return _location; }
        const NationalNumberParser _number{/* 'X','H' */ 0, 'B', /* 'O' or */ 'Q', 'D'};
        const Ns32000SymbolParser _symbol{};
        const SimpleLocationParser _location{PSTR_DOT_STAR};
        const Ns32000LetterParser _letter{};
    } PLUGINS{};
    return PLUGINS;
}

AsmNs32000::AsmNs32000(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_fpu),
      Config(TABLE),
      _opt_fpu(this, &AsmNs32000::setFpuName, OPT_TEXT_FPU, OPT_DESC_FPU, _opt_pmmu),
      _opt_pmmu(this, &AsmNs32000::setPmmuName, OPT_TEXT_PMMU, OPT_DESC_PMMU) {
    reset();
}

void AsmNs32000::reset() {
    Assembler::reset();
    setFpuType(FPU_NONE);
    setMmuType(MMU_NONE);
}

Error AsmNs32000::setFpuName(const StrScanner &scan) {
    if (scan.iequals_P(TEXT_FPU_NS32081)) {
        setFpuType(FPU_NS32081);
    } else if (scan.iequals_P(TEXT_none)) {
        setFpuType(FPU_NONE);
    } else {
        return UNKNOWN_OPERAND;
    }
    return OK;
}

Error AsmNs32000::setPmmuName(const StrScanner &scan) {
    if (scan.iequals_P(TEXT_MMU_NS32082)) {
        setMmuType(MMU_NS32082);
    } else if (scan.iequals_P(TEXT_none)) {
        setMmuType(MMU_NONE);
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
    op.val32 = strOpt;
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
    op.val32 = configs;
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
    op.val32 = list;
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
        op.val32 = parseExpr32(p, op);
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
        op.val32 = parseExpr32(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_REL;
        op.reg = REG_PC;
        scan = p;
        return OK;
    }

    const auto preg = parsePregName(p);
    if (preg != PREG_UNDEF) {
        op.val32 = encodePregName(preg);
        op.mode = M_PREG;
        scan = p;
        return OK;
    }

    const auto mreg = parseMregName(p);
    if (mreg != MREG_UNDEF) {
        op.val32 = encodeMregName(mreg);
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
            op.val32 = parseExpr32(p, op, ')');
            if (op.hasError())
                return op.getError();
            if (!p.skipSpaces().expect(')'))
                return op.setErrorIf(p, MISSING_CLOSING_PAREN);
            if (*p.skipSpaces() == '+' || *p == '-') {
                ErrorAt error;
                op.disp2 = parseExpr32(p, error);
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

    const auto val = parseExpr(p, op);
    if ((op.isOK() && (*p == '.' || *p == 'e' || *p == 'E')) || op.getError() == OVERFLOW_RANGE ||
            op.getError() == UNDEFINED_SYMBOL) {
        char *end;
        op.float64 = strtod(scan.str(), &end);
        StrScanner e(end);
        if (end != scan.str() && (endOfLine(e.skipSpaces()) || *e == ',')) {
            op.mode = M_IMM;
            op.size = SZ_LONG;
            scan = e;
            return op.setOK();
        }
    }
    if (op.hasError())
        return op.getError();
    op.val32 = val.getUnsigned();
    op.size = SZ_DOUBLE;
    if (val.isSigned()) {
        op.float64 = val.getSigned();
    } else {
        op.float64 = val.getUnsigned();
    }
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

    op.disp2 = op.val32;
    ErrorAt error;
    op.val32 = parseExpr32(p, error, ')');
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
    case SZ_DOUBLE:
        return 0x1E;
    case SZ_QUAD:
        return 0x1F;
    default:
        return 0;
    }
}

void embedOprField(AsmInsn &insn, OprPos pos, uint8_t opr) {
    if (pos == P_GEN1) {
        opr &= 0x1F;
        opr <<= 3;
        if (insn.hasPost())
            insn.embedPost(opr);
        else
            insn.embed(opr);
    }
    if (pos == P_GEN2) {
        opr &= 0x1F;
        insn.embedPost(opr >> 2);
        insn.embed(opr << 6);
    }
    if (pos == P_REG) {
        opr &= 0x7;
        insn.embed(opr << 3);
    }
    if (pos == P_SHORT) {
        opr &= 0xF;
        insn.embedPost(opr >> 1);
        insn.embed(opr << 7);
    }
}

}  // namespace

void AsmNs32000::emitDisplacement(
        AsmInsn &insn, const Operand &op, int32_t val32, Error error) const {
    if (overflowInt(val32, 30) || val32 < -0x1F000000L) {
        insn.setErrorIf(op, error);
        val32 = val32 < 0 ? -0x1F000000 : 0x1FFFFFFF;
    }
    if (overflowInt(val32, 14)) {
        insn.emitOperand32((val32 & 0x3FFFFFFFL) | 0xC0000000L);
    } else if (overflowInt(val32, 7)) {
        insn.emitOperand16((val32 & 0x3FFF) | 0x8000);
    } else {
        insn.emitOperand8(val32 & 0x7F);
    }
}

void AsmNs32000::emitLength(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    uint8_t len = op.getError() ? 0 : op.val32;
    if (op.isOK()) {
        const auto val = static_cast<int32_t>(op.val32);
        if (val <= 0) {
            insn.setErrorIf(op, ILLEGAL_CONSTANT);
        } else if (insn.size() == SZ_DOUBLE) {
            if (len > 4)
                insn.setErrorIf(op, OVERFLOW_RANGE);
            len -= 1;
            len *= 4;
        } else if (insn.size() == SZ_WORD) {
            if (len > 8)
                insn.setErrorIf(op, OVERFLOW_RANGE);
            len -= 1;
            len *= 2;
        } else if (insn.size() == SZ_BYTE) {
            if (len > 16)
                insn.setErrorIf(op, OVERFLOW_RANGE);
            len -= 1;
        }
    }
    emitDisplacement(insn, op, len);
}

void AsmNs32000::emitBitField(
        AsmInsn &insn, AddrMode mode, const Operand &off, const Operand &len) const {
    if (mode == M_BFOFF)
        return;
    if (off.val32 >= 8)
        insn.setErrorIf(off, ILLEGAL_BIT_NUMBER);
    if (len.isOK() && len.val32 == 0)
        insn.setErrorIf(len, ILLEGAL_CONSTANT);
    if (len.val32 > 32)
        insn.setErrorIf(len, OVERFLOW_RANGE);
    const auto length = len.getError() ? 0 : len.val32 - 1;
    const auto data = (static_cast<uint8_t>(off.val32) << 5) | (length & 0x1F);
    insn.emitOperand8(data);
}

void AsmNs32000::emitImmediate(AsmInsn &insn, const Operand &op, OprSize size) const {
    switch (size) {
    case SZ_BYTE:
        insn.emitOperand8(op.val32);
        break;
    case SZ_WORD:
        insn.emitOperand16(op.val32);
        break;
    case SZ_DOUBLE:
        insn.emitOperand32(op.val32);
        break;
    case SZ_FLOAT:
        insn.emitOpFloat32(op.float64);
        break;
    case SZ_LONG:
        insn.emitOpFloat64(op.float64);
        break;
    default:
        break;
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
        if (checkAddr(op.val32))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        // Sign extends 24 bit address into 32 bit.
        const auto val32 = signExtend(op.val32, addressWidth());
        emitDisplacement(insn, op, val32);
        break;
    }
    case M_RREL:
    case M_MEM:
        if (op.mode == M_MEM && op.reg == REG_PC) {
            emitRelative(insn, op);
        } else {
            emitDisplacement(insn, op, op.val32);
        }
        break;
    case M_REL:
        emitRelative(insn, op);
        break;
    case M_MREL:
    case M_EXT:
        emitDisplacement(insn, op, op.val32);
        emitDisplacement(insn, op, op.disp2);
        break;
    case M_IMM:
        emitImmediate(insn, op, mode == M_GENC ? SZ_BYTE : insn.size());
        break;
    default:
        break;
    }
}

void AsmNs32000::emitRelative(AsmInsn &insn, const Operand &op) const {
    const auto base = insn.address();
    const auto target = op.getError() ? base : op.val32;
    insn.setErrorIf(op, checkAddr(target));
    const auto delta = static_cast<int32_t>(target - base);
    if ((delta >= 0 && target < base) || (delta < 0 && target >= base))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    emitDisplacement(insn, op, delta, OPERAND_TOO_FAR);
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
        embedOprField(insn, pos, op.val32);
        break;
    case M_RLST:
        if (op.val32 == 0 && (insn.opCode() == SAVE || insn.opCode() == RESTORE))
            insn.setErrorIf(op, OPCODE_HAS_NO_EFFECT);
        if (size == SZ_NONE) {  // PUSH
            insn.emitOperand8(op.val32);
        } else {  // POP
            insn.emitOperand8(reverseBits(op.val32));
        }
        break;
    case M_FENR:
    case M_FENW:
        if (op.mode == M_FREG && (size == SZ_LONG || size == SZ_QUAD) && !isRegPair(op.reg))
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        goto emit_generic;
    case M_GENR:
    case M_GENW:
        if (op.mode == M_GREG && size == SZ_QUAD && !isRegPair(op.reg))
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        goto emit_generic;
    case M_GENC:
    case M_GENA:
    emit_generic:
        emitGeneric(insn, mode, op, pos);
        break;
    case M_INT4: {
        const int32_t val = static_cast<int32_t>(op.val32);
        if (val < -8 || val >= 8)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        embedOprField(insn, pos, op.val32);
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
        const int32_t val = static_cast<int32_t>(op.val32);
        if (val < 0)
            insn.setErrorIf(op, ILLEGAL_CONSTANT);
        if (op.isOK() && val == 0)
            insn.setErrorIf(op, ILLEGAL_CONSTANT);
        if (val > 32)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        emitDisplacement(insn, op, val);
        break;
    }
    case M_DISP:
        emitDisplacement(insn, op, op.val32);
        break;
    case M_LEN16:
        emitLength(insn, mode, op);
        break;
    default:
        break;
    }
}

Error AsmNs32000::processPseudo(StrScanner &scan, Insn &insn) {
    auto p = scan.skipSpaces();
    StrScanner opr;
    parser().readSymbol(p, opr);
    if (strcasecmp_P(insn.name(), TEXT_FPU) == 0) {
        auto error = setFpuName(opr);
        if (error)
            return setError(scan, error);
        scan = p;
        return OK;
    }
    if (strcasecmp_P(insn.name(), TEXT_PMMU) == 0) {
        auto error = setPmmuName(opr);
        if (error)
            return setError(scan, error);
        scan = p;
        return OK;
    }
    return Assembler::processPseudo(scan, insn);
}

Error AsmNs32000::encodeImpl(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
        return setError(insn.srcOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
            return setError(insn.dstOp);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, insn.ex1Op) && insn.ex1Op.hasError())
            return setError(insn.ex1Op);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, insn.ex2Op) && insn.ex2Op.hasError())
            return setError(insn.ex2Op);
        scan.skipSpaces();
    }

    if (setErrorIf(insn.srcOp, TABLE.searchName(_cpuSpec, insn)))
        return getError();

    insn.setErrorIf(insn.srcOp);
    insn.setErrorIf(insn.dstOp);
    insn.setErrorIf(insn.ex1Op);
    insn.setErrorIf(insn.ex2Op);
    emitIndexByte(insn, insn.srcOp);
    emitIndexByte(insn, insn.dstOp);
    emitIndexByte(insn, insn.ex1Op);
    const AddrMode src = insn.src();
    const AddrMode dst = insn.dst();
    const AddrMode ex1 = insn.ex1();
    const AddrMode ex2 = insn.ex2();
    emitOperand(insn, src, insn.srcSize(), insn.srcOp, insn.srcPos(), insn.srcOp);
    emitOperand(insn, dst, insn.dstSize(), insn.dstOp, insn.dstPos(), insn.srcOp);
    emitOperand(insn, ex1, insn.size(), insn.ex1Op, insn.ex1Pos(), insn.dstOp);
    emitOperand(insn, ex2, insn.size(), insn.ex2Op, insn.ex2Pos(), insn.ex1Op);
    insn.emitInsn();
    return setError(insn);
}

}  // namespace ns32000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
