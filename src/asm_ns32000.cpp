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
#include "reg_ns32000.h"
#include "table_ns32000.h"

namespace libasm {
namespace ns32000 {

Error AsmNs32000::parseStrOptNames(const char *p, Operand &op, bool braket) {
    uint8_t strOpt = 0;
    while (true) {
        const StrOptName name = RegNs32000::parseStrOptName(p);
        if (name == STROPT_UNDEF)
            return UNKNOWN_OPERAND;
        p += RegNs32000::strOptNameLen(name);
        if (strOpt & uint8_t(name))
            return setError(ILLEGAL_OPERAND);
        strOpt |= uint8_t(name);
        p = skipSpaces(p);
        if (*p == ',' || (!braket && *p == '/')) {
            p++;
        } else if (*p == ']') {
            p++;
            break;
        } else if (!braket && endOfLine(p)) {
            break;
        } else {
            return UNKNOWN_OPERAND;
        }
        p = skipSpaces(p);
    }
    _scan = p;
    op.val32 = strOpt;
    op.mode = M_SOPT;
    return setOK();
}

Error AsmNs32000::parseConfigNames(const char *p, Operand &op) {
    uint8_t configs = 0;
    while (true) {
        if (*p == ']') {
            p++;
            break;
        }
        const ConfigName name = RegNs32000::parseConfigName(p);
        if (name == CONFIG_UNDEF)
            return UNKNOWN_OPERAND;
        p += RegNs32000::configNameLen(name);
        configs |= uint8_t(name);
        p = skipSpaces(p);
        if (*p == ',') {
            p++;
        } else if (*p == ']') {
            p++;
            break;
        } else {
            return UNKNOWN_OPERAND;
        }
        p = skipSpaces(p);
    }
    _scan = p;
    op.val32 = configs;
    op.mode = configs ? M_CONF : M_NONE;
    return setOK();
}

Error AsmNs32000::parseRegisterList(const char *p, Operand &op) {
    uint8_t list = 0;
    uint8_t n = 0;
    while (true) {
        const RegName name = RegNs32000::parseRegName(p);
        if (!RegNs32000::isGeneric(name))
            return UNKNOWN_OPERAND;
        list |= (1 << RegNs32000::encodeRegName(name));
        n++;
        p = skipSpaces(p + RegNs32000::regNameLen(name));
        if (*p == ',') {
            p++;
        } else if (*p == ']') {
            p++;
            break;
        } else {
            return UNKNOWN_OPERAND;
        }
        p = skipSpaces(p);
    }
    _scan = p;
    op.val32 = list;
    op.mode = M_RLST;
    return setOK();
}

Error AsmNs32000::parseBaseOperand(const char *scan, Operand &op) {
    const char *p = scan;
    _scan = p;
    if (endOfLine(p))
        return OK;
    if (*p == '@') {
        op.val32 = parseExpr32(p + 1);
        if (parserError())
            return getError();
        op.setError(getError());
        op.mode = M_ABS;
        return OK;
    }

    if (*p == '[') {
        p = skipSpaces(p + 1);
        // parseConfigName can handle empty list "[]".
        if (parseConfigNames(p, op) != UNKNOWN_OPERAND)
            return getError();
        if (parseRegisterList(p, op) != UNKNOWN_OPERAND)
            return getError();
        if (parseStrOptNames(p, op, true) != UNKNOWN_OPERAND)
            return getError();
        return setError(MISSING_CLOSING_PAREN);
    }
    if (parseStrOptNames(p, op) == OK)
        return OK;

    if (*p == '*' || *p == '.') {
        const char *t = skipSpaces(p + 1);
        if (*t == '+' || *t == '-' || endOfLine(t) || *t == '[' || *t == ',') {
            const char *end = scanExpr(t, '[');
            op.val32 = parseExpr32(p, end);
            if (parserError() == OK) {
                op.setError(getError());
                op.mode = M_REL;
                op.reg = REG_PC;
                return OK;
            }
            return setError(UNKNOWN_OPERAND);
        }
    }

    const PregName preg = RegNs32000::parsePregName(p);
    if (preg != PREG_UNDEF) {
        _scan = p + RegNs32000::pregNameLen(preg);
        op.val32 = RegNs32000::encodePregName(preg);
        op.mode = M_PREG;
        return OK;
    }

    const MregName mreg = RegNs32000::parseMregName(p);
    if (mreg != MREG_UNDEF) {
        _scan = p + RegNs32000::mregNameLen(mreg);
        op.val32 = RegNs32000::encodeMregName(mreg);
        op.mode = M_MREG;
        return OK;
    }

    RegName reg = RegNs32000::parseRegName(p);
    if (reg != REG_UNDEF) {
        p += RegNs32000::regNameLen(reg);
        if (RegNs32000::isGeneric(reg)) {
            _scan = p;
            op.reg = reg;
            op.mode = M_GREG;
            return OK;
        }

        if (RegNs32000::isFloat(reg)) {
            _scan = p;
            op.reg = reg;
            op.mode = M_FREG;
            return OK;
        }

        if (reg == REG_TOS) {
            _scan = p;
            op.mode = M_TOS;
            return OK;
        }
        if (reg == REG_EXT) {
            if (*p != '(')
                return setError(UNKNOWN_OPERAND);
            op.val32 = parseExpr32(p + 1);
            if (parserError())
                return getError();
            op.setError(getError());
            p = skipSpaces(_scan);
            if (*p != ')')
                return setError(MISSING_CLOSING_PAREN);
            p = skipSpaces(p + 1);
            if (*p == '+' || *p == '-') {
                op.disp2 = parseExpr32(p);
                if (parserError())
                    return getError();
                op.setErrorIf(getError());
                p = skipSpaces(_scan);
            }
            if (*p == '[' || endOfLine(p) || *p == ',') {
                op.mode = M_EXT;
                _scan = p;
                return OK;
            }
            return setError(UNKNOWN_OPERAND);
        }
        return setError(UNKNOWN_REGISTER);
    }

    op.val32 = parseExpr32(p);
    const char *t = _scan;
    if (*t == '.' || *t == 'e' || *t == 'E' || parserError() == OVERFLOW_RANGE ||
            getError() == UNDEFINED_SYMBOL) {
        char *e;
        op.float64 = strtod(p, &e);
        t = skipSpaces(e);
        if (p != e && (*t == ',' || endOfLine(t))) {
            _scan = e;
            op.mode = M_IMM;
            op.size = SZ_LONG;
            return setOK();
        }
    }
    if (parserError())
        return getError();
    op.setError(getError());
    p = skipSpaces(_scan);
    op.size = SZ_DOUBLE;
    if (endOfLine(p) || *p == ',') {
        _scan = p;
        op.mode = M_IMM;  // M_REL
        return OK;
    }
    if (*p++ != '(')
        return setError(UNKNOWN_OPERAND);
    reg = RegNs32000::parseRegName(p);
    if (reg != REG_UNDEF) {
        p += RegNs32000::regNameLen(reg);
        if (*p++ != ')')
            return setError(MISSING_CLOSING_PAREN);
        if (RegNs32000::isGeneric(reg)) {
            _scan = p;
            op.reg = reg;
            op.mode = M_RREL;
            return OK;
        }
        if (reg == REG_FP || reg == REG_SP || reg == REG_SB || reg == REG_PC || reg == REG_EXT) {
            _scan = p;
            op.reg = reg;
            op.mode = (reg == REG_EXT) ? M_EXT : M_MEM;
            return OK;
        }
        return setError(UNKNOWN_OPERAND);
    }

    op.disp2 = op.val32;
    op.val32 = parseExpr32(p);
    if (parserError())
        return getError();
    op.setErrorIf(getError());
    p = skipSpaces(_scan);
    if (*p++ != '(')
        return setError(UNKNOWN_OPERAND);
    reg = RegNs32000::parseRegName(p);
    if (reg != REG_UNDEF) {
        p += RegNs32000::regNameLen(reg);
        if (*p++ != ')')
            return setError(MISSING_CLOSING_PAREN);
        if (*(p = skipSpaces(p)) != ')')
            return setError(MISSING_CLOSING_PAREN);
        if (reg == REG_FP || reg == REG_SP || reg == REG_SB || reg == REG_EXT) {
            _scan = p + 1;
            op.reg = reg;
            op.mode = (reg == REG_EXT) ? M_EXT : M_MREL;
            return OK;
        }
        return setError(UNKNOWN_OPERAND);
    }

    return setError(UNKNOWN_OPERAND);
}

Error AsmNs32000::parseOperand(const char *scan, Operand &op) {
    if (parseBaseOperand(skipSpaces(scan), op))
        return getError();
    if (op.mode == M_GREG || op.mode == M_RREL || op.mode == M_MREL || op.mode == M_ABS ||
            op.mode == M_EXT || op.mode == M_TOS || op.mode == M_MEM || op.mode == M_REL) {
        const char *p = skipSpaces(_scan);
        if (*p++ != '[')
            return OK;
        const RegName index = RegNs32000::parseRegName(p);
        if (!RegNs32000::isGeneric(index))
            return setError(UNKNOWN_OPERAND);
        p += RegNs32000::regNameLen(index);
        if (*p++ != ':')
            return setError(UNKNOWN_OPERAND);
        const OprSize indexSize = RegNs32000::parseIndexSize(p);
        if (indexSize == SZ_NONE)
            return setError(UNKNOWN_OPERAND);
        p += RegNs32000::indexSizeLen(indexSize);
        if (*p++ != ']')
            return setError(MISSING_CLOSING_PAREN);
        _scan = p;
        op.index = index;
        op.size = indexSize;
    }
    return OK;
}

static uint16_t reverseBits(uint8_t bits) {
    uint8_t reverse = 0;
    for (uint8_t i = 0;; i++) {
        if (bits & 1)
            reverse |= 1;
        if (i == 7)
            break;
        bits >>= 1;
        reverse <<= 1;
    }
    return reverse;
}

static uint8_t encodeScaledIndex(OprSize indexSize) {
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

static void embedOprField(InsnNs32000 &insn, OprPos pos, uint8_t opr) {
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

Error AsmNs32000::emitDisplacement(InsnNs32000 &insn, uint32_t val32) {
    const int32_t val = static_cast<int32_t>(val32);
    if (val >= -0x40 && val < 0x40) {
        const uint8_t disp = static_cast<uint8_t>(val) & 0x7F;
        insn.emitOperand8(disp);
        return OK;
    }
    if (val >= -0x2000 && val < 0x2000) {
        const uint16_t disp = (static_cast<uint16_t>(val) & 0x3FFF) | 0x8000;
        insn.emitOperand16(disp);
        return OK;
    }
    if (val >= -0x1F000000L && val < 0x20000000L) {
        const uint32_t disp = (static_cast<uint32_t>(val) & 0x3FFFFFFFL) | 0xC0000000L;
        insn.emitOperand32(disp);
        return OK;
    }
    return setError(OVERFLOW_RANGE);
}

Error AsmNs32000::emitLength(InsnNs32000 &insn, AddrMode mode, const Operand &op) {
    uint8_t len = op.getError() ? 0 : op.val32;
    if (op.getError() == OK) {
        const int32_t val = static_cast<int32_t>(op.val32);
        if (val <= 0)
            return setError(ILLEGAL_CONSTANT);
        if (val > 16)
            return setError(OVERFLOW_RANGE);
        if (mode == M_LEN4) {
            if (len > 4)
                return setError(OVERFLOW_RANGE);
            len -= 1;
            len *= 4;
        }
        if (mode == M_LEN8) {
            if (len > 8)
                return setError(OVERFLOW_RANGE);
            len -= 1;
            len *= 2;
        }
        if (mode == M_LEN16) {
            if (len > 16)
                return setError(OVERFLOW_RANGE);
            len -= 1;
        }
    }
    return emitDisplacement(insn, len);
}

Error AsmNs32000::emitBitField(
        InsnNs32000 &insn, AddrMode mode, const Operand *offOp, const Operand &lenOp) {
    if (mode == M_BFOFF)
        return OK;
    if (offOp->val32 >= 8)
        return setError(ILLEGAL_BIT_NUMBER);
    if (lenOp.getError() == OK && lenOp.val32 == 0)
        return setError(ILLEGAL_CONSTANT);
    if (lenOp.val32 > 32)
        return setError(OVERFLOW_RANGE);
    const uint8_t len = lenOp.getError() ? 0 : lenOp.val32 - 1;
    const uint8_t data = (static_cast<uint8_t>(offOp->val32) << 5) | len;
    insn.emitOperand8(data);
    return OK;
}

Error AsmNs32000::emitImmediate(InsnNs32000 &insn, const Operand &op, OprSize size) {
    switch (size) {
    case SZ_BYTE:
        insn.emitOperand8(static_cast<uint8_t>(op.val32));
        break;
    case SZ_WORD:
        insn.emitOperand16(static_cast<uint16_t>(op.val32));
        break;
    case SZ_DOUBLE:
        insn.emitOperand32(op.val32);
        break;
    case SZ_FLOAT:
        insn.emitOpFloat32(
                op.size == SZ_LONG ? static_cast<float>(op.float64) : static_cast<float>(op.val32));
        break;
    case SZ_LONG:
        insn.emitOpFloat64(op.size == SZ_LONG ? op.float64 : static_cast<double>(op.val32));
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    return OK;
}

uint8_t AsmNs32000::encodeGenericField(AddrMode mode, RegName reg) const {
    switch (mode) {
    case M_GREG:
    case M_FREG:
        return RegNs32000::encodeRegName(reg);
    case M_RREL:
        return RegNs32000::encodeRegName(reg) | 0x08;
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

Error AsmNs32000::emitIndexByte(InsnNs32000 &insn, const Operand &op) const {
    if (op.index == REG_UNDEF)
        return OK;
    const uint8_t indexByte =
            (encodeGenericField(op.mode, op.reg) << 3) | RegNs32000::encodeRegName(op.index);
    insn.emitOperand8(indexByte);
    return OK;
}

Error AsmNs32000::emitGeneric(InsnNs32000 &insn, AddrMode mode, const Operand &op, OprPos pos) {
    const uint8_t field = (op.index == REG_UNDEF) ? encodeGenericField(op.mode, op.reg)
                                                  : encodeScaledIndex(op.size);
    embedOprField(insn, pos, field);
    switch (op.mode) {
    case M_ABS:
        if (op.val32 < uint32_t(1) << uint8_t(addressWidth())) {
            // Convert negative 24 bit address into negative 32bit value.
            uint32_t val32 = op.val32;
            if (val32 & (uint32_t(1) << uint8_t(addressWidth() - 1))) {
                val32 |= ~((uint32_t(1) << uint8_t(addressWidth())) - 1);
            }
            return emitDisplacement(insn, val32);
        }
        return setError(OVERFLOW_RANGE);
    case M_RREL:
    case M_MEM:
        if (op.mode == M_MEM && op.reg == REG_PC)
            return emitRelative(insn, op);
        return emitDisplacement(insn, op.val32);
    case M_REL:
        return emitRelative(insn, op);
    case M_MREL:
    case M_EXT:
        if (emitDisplacement(insn, op.val32))
            return getError();
        return emitDisplacement(insn, op.disp2);
    case M_IMM:
        return emitImmediate(insn, op, mode == M_GENC ? SZ_BYTE : insn.oprSize());
    default:
        return OK;
    }
}

Error AsmNs32000::emitRelative(InsnNs32000 &insn, const Operand &op) {
    const Config::uintptr_t target = op.getError() ? insn.address() : op.val32;
    const Config::ptrdiff_t disp = target - insn.address();
    if (emitDisplacement(insn, disp))
        return setError(OPERAND_TOO_FAR);
    return OK;
}

Error AsmNs32000::emitOperand(InsnNs32000 &insn, AddrMode mode, OprSize size, const Operand &op,
        OprPos pos, const Operand *prevOp) {
    switch (mode) {
    case M_GREG:
        embedOprField(insn, pos, RegNs32000::encodeRegName(op.reg));
        break;
    case M_PREG:
    case M_MREG:
    case M_CONF:
    case M_SOPT:
        embedOprField(insn, pos, op.val32);
        break;
    case M_RLST:
        if (op.val32 == 0)
            return setError(OPCODE_HAS_NO_EFFECT);
        if (size == SZ_NONE) {  // PUSH
            insn.emitOperand8(op.val32);
        } else {  // POP
            insn.emitOperand8(reverseBits(op.val32));
        }
        break;
    case M_FENR:
    case M_FENW:
        if (op.mode == M_FREG && (size == SZ_LONG || size == SZ_QUAD) &&
                !RegNs32000::isRegPair(op.reg)) {
            insn.resetAddress(insn.address());
            return setError(REGISTER_NOT_ALLOWED);
        }
        goto emit_generic;
    case M_GENR:
    case M_GENW:
        if (op.mode == M_GREG && size == SZ_QUAD && !RegNs32000::isRegPair(op.reg)) {
            insn.resetAddress(insn.address());
            return setError(REGISTER_NOT_ALLOWED);
        }
        goto emit_generic;
    case M_GENC:
    case M_GENA:
    emit_generic:
        return emitGeneric(insn, mode, op, pos);
    case M_INT4: {
        const int32_t val = static_cast<int32_t>(op.val32);
        if (val < -8 || val >= 8)
            return setError(OVERFLOW_RANGE);
        embedOprField(insn, pos, op.val32);
        break;
    }
    case M_BFOFF:
    case M_BFLEN:
        return emitBitField(insn, mode, prevOp, op);
    case M_REL:
        return emitRelative(insn, op);
    case M_LEN32: {
        const int32_t val = static_cast<int32_t>(op.val32);
        if (val < 0)
            return setError(ILLEGAL_CONSTANT);
        if (op.getError() == OK && val == 0)
            setError(ILLEGAL_CONSTANT);
        if (val > 32)
            return setError(OVERFLOW_RANGE);
        return emitDisplacement(insn, val);
    }
    case M_DISP:
        return emitDisplacement(insn, op.val32);
    case M_LEN4:
    case M_LEN8:
    case M_LEN16:
        return emitLength(insn, mode, op);
    default:
        return setError(INTERNAL_ERROR);
    }
    return OK;
}

static const char TEXT_FPU[] PROGMEM = "FPU";
static const char TEXT_PMMU[] PROGMEM = "PMMU";
static const char TEXT_FPU_NS32081[] PROGMEM = "NS32081";
static const char TEXT_MMU_NS32082[] PROGMEM = "NS32082";
static const char TEXT_NONE[] PROGMEM = "NONE";

Error AsmNs32000::processPseudo(const char *scan, const InsnNs32000 &insn) {
    const char *p = skipSpaces(scan);
    const char *end = _parser.scanSymbol(p);
    const bool none = strcasecmp_P(p, TEXT_NONE) == 0;
    if (strcasecmp_P(insn.name(), TEXT_FPU) == 0) {
        if (strcasecmp_P(p, TEXT_FPU_NS32081) == 0 || none) {
            setFpu(none ? FPU_NONE : FPU_NS32081);
            _scan = end;
        } else {
            setError(UNKNOWN_OPERAND);
        }
        return OK;
    }
    if (strcasecmp_P(insn.name(), TEXT_PMMU) == 0) {
        if (strcasecmp_P(p, TEXT_MMU_NS32082) == 0 || none) {
            setMmu(none ? MMU_NONE : MMU_NS32082);
            _scan = end;
        } else {
            setError(UNKNOWN_OPERAND);
        }
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

Error AsmNs32000::encode(Insn &_insn) {
    InsnNs32000 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    if (processPseudo(endName, insn) == OK)
        return getError();

    Operand srcOp, dstOp, ex1Op, ex2Op;
    if (parseOperand(endName, srcOp))
        return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        if (parseOperand(p + 1, dstOp))
            return getError();
        p = skipSpaces(_scan);
    }
    if (*p == ',') {
        if (parseOperand(p + 1, ex1Op))
            return getError();
        p = skipSpaces(_scan);
    }
    if (*p == ',') {
        if (parseOperand(p + 1, ex2Op))
            return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setErrorIf(srcOp.getError());
    setErrorIf(dstOp.getError());
    setErrorIf(ex1Op.getError());
    setErrorIf(ex2Op.getError());

    insn.setAddrMode(srcOp.mode, dstOp.mode, ex1Op.mode, ex2Op.mode);
    if (TableNs32000.searchName(insn))
        return setError(TableNs32000.getError());
    const AddrMode src = insn.srcMode();
    const AddrMode dst = insn.dstMode();
    const AddrMode ex1 = insn.ex1Mode();
    const AddrMode ex2 = insn.ex2Mode();
    if (emitIndexByte(insn, srcOp))
        return getError();
    if (emitIndexByte(insn, dstOp))
        return getError();
    if (emitIndexByte(insn, ex1Op))
        return getError();
    const OprSize size = insn.oprSize();
    if (src != M_NONE) {
        const OprSize srcSize = (ex1 == M_NONE && insn.ex1Pos() != P_NONE) ? SZ_QUAD : size;
        if (emitOperand(insn, src, srcSize, srcOp, insn.srcPos()))
            return getError();
    }
    if (dst != M_NONE) {
        const OprSize dstSize = (ex2 == M_NONE && insn.ex2Pos() != P_NONE) ? SZ_QUAD : size;
        if (emitOperand(insn, dst, dstSize, dstOp, insn.dstPos()))
            return getError();
    }
    if (ex1 != M_NONE) {
        if (emitOperand(insn, ex1, size, ex1Op, insn.ex1Pos()))
            return getError();
    }
    if (ex2 != M_NONE) {
        const Operand *prevOp = (ex2 == M_BFLEN) ? &ex1Op : nullptr;
        if (emitOperand(insn, ex2, size, ex2Op, insn.ex2Pos(), prevOp))
            return getError();
    }
    insn.emitInsn();
    return getError();
}

}  // namespace ns32000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
