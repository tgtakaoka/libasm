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

namespace libasm {
namespace ns32000 {

const char AsmNs32000::OPT_TEXT_FPU[] PROGMEM = "fpu";
const char AsmNs32000::OPT_DESC_FPU[] PROGMEM = "floating point co-processor";
const char AsmNs32000::OPT_TEXT_PMMU[] PROGMEM = "pmmu";
const char AsmNs32000::OPT_DESC_PMMU[] PROGMEM = "memory management unit";

Error AsmNs32000::parseStrOptNames(StrScanner &scan, Operand &op, bool braket) {
    StrScanner p(scan);
    uint8_t strOpt = 0;
    while (true) {
        const StrOptName name = RegNs32000::parseStrOptName(p);
        if (name == STROPT_UNDEF)
            return UNKNOWN_OPERAND;
        if (strOpt & uint8_t(name))
            return setError(op, ILLEGAL_OPERAND);
        strOpt |= uint8_t(name);
        if (p.skipSpaces().expect(']') || (!braket && endOfLine(*p)))
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
    return setOK();
}

Error AsmNs32000::parseConfigNames(StrScanner &scan, Operand &op) {
    StrScanner p(scan);
    uint8_t configs = 0;
    while (true) {
        if (p.expect(']'))
            break;
        const ConfigName name = RegNs32000::parseConfigName(p);
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
    return setOK();
}

Error AsmNs32000::parseRegisterList(StrScanner &scan, Operand &op) {
    StrScanner p(scan);
    uint8_t list = 0;
    uint8_t n = 0;
    while (true) {
        const RegName name = RegNs32000::parseRegName(p);
        if (!RegNs32000::isGeneric(name))
            return UNKNOWN_OPERAND;
        list |= (1 << RegNs32000::encodeRegName(name));
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
    return setOK();
}

Error AsmNs32000::parseBaseOperand(StrScanner &scan, Operand &op) {
    StrScanner p(scan.skipSpaces());
    if (endOfLine(*p))
        return OK;
    if (p.expect('@')) {
        op.val32 = parseExpr32(p, op);
        if (parserError())
            return getError();
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
            return getError();
        }
        return setError(p, MISSING_CLOSING_BRACKET);
    }
    if (parseStrOptNames(p, op) == OK) {
        scan = p;
        return OK;
    }

    if (*p == '*' || *p == '.') {
        StrScanner t(p);
        ++t;
        if (*t.skipSpaces() == '+' || *t == '-' || endOfLine(*t) || *t == '[' || *t == ',') {
            StrScanner base = _parser.scanExpr(p, '[');
            const auto size = base.size();
            if (size) {
                op.val32 = parseExpr32(base, op);
                p += size - base.size();
            } else {
                op.val32 = parseExpr32(p, op);
            }
            if (parserError())
                return getError();
            op.mode = M_REL;
            op.reg = REG_PC;
            scan = p;
            return OK;
        }
    }

    const PregName preg = RegNs32000::parsePregName(p);
    if (preg != PREG_UNDEF) {
        op.val32 = RegNs32000::encodePregName(preg);
        op.mode = M_PREG;
        scan = p;
        return OK;
    }

    const MregName mreg = RegNs32000::parseMregName(p);
    if (mreg != MREG_UNDEF) {
        op.val32 = RegNs32000::encodeMregName(mreg);
        op.mode = M_MREG;
        scan = p;
        return OK;
    }

    StrScanner a(p);
    RegName reg = RegNs32000::parseRegName(p);
    if (reg != REG_UNDEF) {
        if (RegNs32000::isGeneric(reg)) {
            op.reg = reg;
            op.mode = M_GREG;
            scan = p;
            return OK;
        }

        if (RegNs32000::isFloat(reg)) {
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
                return setError(p, UNKNOWN_OPERAND);
            op.val32 = parseExpr32(p, op);
            if (parserError())
                return getError();
            if (!p.skipSpaces().expect(')'))
                return setError(p, MISSING_CLOSING_PAREN);
            if (*p.skipSpaces() == '+' || *p == '-') {
                op.disp2 = parseExpr32(p, op);
                if (parserError())
                    return getError();
                p.skipSpaces();
            }
            if (*p == '[' || endOfLine(*p) || *p == ',') {
                op.mode = M_EXT;
                scan = p;
                return OK;
            }
            return setError(p, UNKNOWN_OPERAND);
        }
        return setError(a, UNKNOWN_REGISTER);
    }

    const Value val = parseExpr(p, op);
    StrScanner t(p);
    if (*t == '.' || *t == 'e' || *t == 'E' || parserError() == OVERFLOW_RANGE ||
            op.getError() == UNDEFINED_SYMBOL) {
        char *e;
        op.float64 = strtod(a.str(), &e);
        StrScanner end(e);
        if (a.str() != e && (*end == ',' || endOfLine(*end))) {
            op.mode = M_IMM;
            op.size = SZ_LONG;
            scan = end;
            return setError(op.setOK());
        }
    }
    if (parserError())
        return getError();
    op.val32 = val.getUnsigned();
    op.size = SZ_DOUBLE;
    if (val.isSigned()) {
        op.float64 = val.getSigned();
    } else {
        op.float64 = val.getUnsigned();
    }
    if (*p.skipSpaces() == ',' || endOfLine(*p)) {
        op.mode = M_IMM;  // M_REL
        scan = p;
        return OK;
    }
    if (!p.expect('('))
        return setError(p, UNKNOWN_OPERAND);
    const StrScanner r(p);
    reg = RegNs32000::parseRegName(p);
    if (reg != REG_UNDEF) {
        if (!p.expect(')'))
            return setError(p, MISSING_CLOSING_PAREN);
        if (RegNs32000::isGeneric(reg)) {
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
        return setError(r, UNKNOWN_OPERAND);
    }

    op.disp2 = op.val32;
    op.val32 = parseExpr32(p, op);
    if (parserError())
        return getError();
    if (!p.skipSpaces().expect('('))
        return setError(p, UNKNOWN_OPERAND);
    const StrScanner x(p);
    reg = RegNs32000::parseRegName(p);
    if (reg != REG_UNDEF) {
        if (!p.expect(')'))
            return setError(p, MISSING_CLOSING_PAREN);
        if (!p.skipSpaces().expect(')'))
            return setError(p, MISSING_CLOSING_PAREN);
        if (reg == REG_FP || reg == REG_SP || reg == REG_SB || reg == REG_EXT) {
            op.reg = reg;
            op.mode = (reg == REG_EXT) ? M_EXT : M_MREL;
            scan = p;
            return OK;
        }
    }

    return setError(x, UNKNOWN_OPERAND);
}

Error AsmNs32000::parseOperand(StrScanner &scan, Operand &op) {
    if (parseBaseOperand(scan, op))
        return getError();
    if (op.mode == M_GREG || op.mode == M_RREL || op.mode == M_MREL || op.mode == M_ABS ||
            op.mode == M_EXT || op.mode == M_TOS || op.mode == M_MEM || op.mode == M_REL) {
        StrScanner p(scan);
        if (!p.skipSpaces().expect('['))
            return OK;
        const RegName index = RegNs32000::parseRegName(p);
        if (!RegNs32000::isGeneric(index))
            return setError(scan, UNKNOWN_OPERAND);
        if (!p.skipSpaces().expect(':'))
            return setError(scan, UNKNOWN_OPERAND);
        const OprSize indexSize = RegNs32000::parseIndexSize(p);
        if (indexSize == SZ_NONE)
            return setError(scan, UNKNOWN_OPERAND);
        if (!p.skipSpaces().expect(']'))
            return setError(p, MISSING_CLOSING_BRACKET);
        scan = p;
        op.index = index;
        op.size = indexSize;
    }
    return OK;
}

static uint16_t reverseBits(uint8_t bits) {
    bits = (bits & 0x55) << 1 | (bits & 0xAA) >> 1;
    bits = (bits & 0x33) << 2 | (bits & 0xCC) >> 2;
    return bits << 4 | bits >> 4;
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

Error AsmNs32000::emitDisplacement(InsnNs32000 &insn, const Operand &op, uint32_t val32) {
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
    return setError(op, OVERFLOW_RANGE);
}

Error AsmNs32000::emitLength(InsnNs32000 &insn, AddrMode mode, const Operand &op) {
    uint8_t len = op.getError() ? 0 : op.val32;
    if (op.isOK()) {
        const int32_t val = static_cast<int32_t>(op.val32);
        if (val <= 0)
            return setError(op, ILLEGAL_CONSTANT);
        if (val > 16)
            return setError(op, OVERFLOW_RANGE);
        if (mode == M_LEN4) {
            if (len > 4)
                return setError(op, OVERFLOW_RANGE);
            len -= 1;
            len *= 4;
        }
        if (mode == M_LEN8) {
            if (len > 8)
                return setError(op, OVERFLOW_RANGE);
            len -= 1;
            len *= 2;
        }
        if (mode == M_LEN16) {
            if (len > 16)
                return setError(op, OVERFLOW_RANGE);
            len -= 1;
        }
    }
    return emitDisplacement(insn, op, len);
}

Error AsmNs32000::emitBitField(
        InsnNs32000 &insn, AddrMode mode, const Operand &off, const Operand &len) {
    if (mode == M_BFOFF)
        return OK;
    if (off.val32 >= 8)
        return setError(off, ILLEGAL_BIT_NUMBER);
    if (len.isOK() && len.val32 == 0)
        return setError(len, ILLEGAL_CONSTANT);
    if (len.val32 > 32)
        return setError(len, OVERFLOW_RANGE);
    const uint8_t length = len.getError() ? 0 : len.val32 - 1;
    const uint8_t data = (static_cast<uint8_t>(off.val32) << 5) | length;
    insn.emitOperand8(data);
    return OK;
}

Error AsmNs32000::emitImmediate(InsnNs32000 &insn, const Operand &op, OprSize size) {
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
    case M_ABS: {
        const Config::uintptr_t max = 1UL << uint8_t(addressWidth());
        if (max && op.val32 >= max)
            return setError(op, OVERFLOW_RANGE);
        // Sign extends 24 bit address into 32 bit.
        const uint32_t sign = max >> 1;
        const int32_t val32 = (op.val32 & (sign - 1)) - (op.val32 & sign);
        return emitDisplacement(insn, op, val32);
    }
    case M_RREL:
    case M_MEM:
        if (op.mode == M_MEM && op.reg == REG_PC)
            return emitRelative(insn, op);
        return emitDisplacement(insn, op, op.val32);
    case M_REL:
        return emitRelative(insn, op);
    case M_MREL:
    case M_EXT:
        if (emitDisplacement(insn, op, op.val32))
            return getError();
        return emitDisplacement(insn, op, op.disp2);
    case M_IMM:
        return emitImmediate(insn, op, mode == M_GENC ? SZ_BYTE : insn.oprSize());
    default:
        return OK;
    }
}

Error AsmNs32000::emitRelative(InsnNs32000 &insn, const Operand &op) {
    const Config::uintptr_t target = op.getError() ? insn.address() : op.val32;
    const Config::ptrdiff_t disp = target - insn.address();
    if (emitDisplacement(insn, op, disp))
        return setError(op, OPERAND_TOO_FAR);
    return OK;
}

Error AsmNs32000::emitOperand(InsnNs32000 &insn, AddrMode mode, OprSize size, const Operand &op,
        OprPos pos, const Operand &prevOp) {
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
            return setError(op, OPCODE_HAS_NO_EFFECT);
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
            insn.reset();
            return setError(op, REGISTER_NOT_ALLOWED);
        }
        goto emit_generic;
    case M_GENR:
    case M_GENW:
        if (op.mode == M_GREG && size == SZ_QUAD && !RegNs32000::isRegPair(op.reg)) {
            insn.reset();
            return setError(op, REGISTER_NOT_ALLOWED);
        }
        goto emit_generic;
    case M_GENC:
    case M_GENA:
    emit_generic:
        return emitGeneric(insn, mode, op, pos);
    case M_INT4: {
        const int32_t val = static_cast<int32_t>(op.val32);
        if (val < -8 || val >= 8)
            return setError(op, OVERFLOW_RANGE);
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
            return setError(op, ILLEGAL_CONSTANT);
        if (op.isOK() && val == 0)
            setError(op, ILLEGAL_CONSTANT);
        if (val > 32)
            return setError(op, OVERFLOW_RANGE);
        return emitDisplacement(insn, op, val);
    }
    case M_DISP:
        return emitDisplacement(insn, op, op.val32);
    case M_LEN4:
    case M_LEN8:
    case M_LEN16:
        return emitLength(insn, mode, op);
    default:
        break;
    }
    return OK;
}

Error AsmNs32000::setFpu(const StrScanner &scan) {
    if (scan.iequals_P(PSTR("ns32081"))) {
        TableNs32000::TABLE.setFpu(FPU_NS32081);
        return OK;
    }
    if (scan.iequals_P(PSTR("none"))) {
        TableNs32000::TABLE.setFpu(FPU_NONE);
        return OK;
    }
    return UNKNOWN_OPERAND;
}

Error AsmNs32000::setPmmu(const StrScanner &scan) {
    if (scan.iequals_P(PSTR("ns32082"))) {
        TableNs32000::TABLE.setMmu(MMU_NS32082);
        return OK;
    }
    if (scan.iequals_P(PSTR("none"))) {
        TableNs32000::TABLE.setMmu(MMU_NONE);
        return OK;
    }
    return UNKNOWN_OPERAND;
}

Error AsmNs32000::processPseudo(StrScanner &scan, const char *name) {
    StrScanner p(scan.skipSpaces());
    if (strcasecmp_P(name, OPT_TEXT_FPU) == 0) {
        const StrScanner option = _parser.readSymbol(p);
        const auto error = setFpu(option);
        if (error) {
            setError(scan, error);
        } else {
            scan = p;
        }
        return OK;
    }
    if (strcasecmp_P(name, OPT_TEXT_PMMU) == 0) {
        const StrScanner option = _parser.readSymbol(p);
        const auto error = setPmmu(option);
        if (error) {
            setError(scan, error);
        } else {
            scan = p;
        }
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

Error AsmNs32000::encode(StrScanner &scan, Insn &_insn) {
    InsnNs32000 insn(_insn);
    insn.nameBuffer().text(_parser.readSymbol(scan));

    if (processPseudo(scan, insn.name()) == OK)
        return getError();

    Operand srcOp, dstOp, ex1Op, ex2Op;
    if (parseOperand(scan, srcOp))
        return getError();
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, dstOp))
            return getError();
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, ex1Op))
            return getError();
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, ex2Op))
            return getError();
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(srcOp);
    setErrorIf(dstOp);
    setErrorIf(ex1Op);
    setErrorIf(ex2Op);

    insn.setAddrMode(srcOp.mode, dstOp.mode, ex1Op.mode, ex2Op.mode);
    if (TableNs32000::TABLE.searchName(insn))
        return setError(TableNs32000::TABLE.getError());
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
        if (emitOperand(insn, src, srcSize, srcOp, insn.srcPos(), srcOp))
            return getError();
    }
    if (dst != M_NONE) {
        const OprSize dstSize = (ex2 == M_NONE && insn.ex2Pos() != P_NONE) ? SZ_QUAD : size;
        if (emitOperand(insn, dst, dstSize, dstOp, insn.dstPos(), srcOp))
            return getError();
    }
    if (ex1 != M_NONE) {
        if (emitOperand(insn, ex1, size, ex1Op, insn.ex1Pos(), dstOp))
            return getError();
    }
    if (ex2 != M_NONE) {
        if (emitOperand(insn, ex2, size, ex2Op, insn.ex2Pos(), ex1Op))
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
