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

#include "asm_tms320f.h"
#include "table_tms320f.h"
#include "text_tms320f.h"
#include "ti_float.h"

namespace libasm {
namespace tms320f {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::tms320f;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_dINT,    &Assembler::defineDataConstant, Assembler::DATA_LONG},
    {TEXT_dLONG,   &Assembler::defineDataConstant, Assembler::DATA_LONG},
    {TEXT_dORG,    &Assembler::defineOrigin},
    {TEXT_dSPACE,  &Assembler::allocateSpaces,     Assembler::DATA_LONG},
    {TEXT_dSTRING, &Assembler::defineDataConstant, Assembler::DATA_LONG},
    {TEXT_dWORD,   &Assembler::defineDataConstant, Assembler::DATA_LONG},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmTms320f::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return IntelNumberParser::singleton(); }
        const CommentParser &comment() const override { return Tms320CommentParser::singleton(); }
        const SymbolParser &symbol() const override { return Tms320fSymbolParser::singleton(); }
        const LetterParser &letter() const override { return Tms320LetterParser::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmTms320f::AsmTms320f(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE), _prevInsn(0), _prev(_prevInsn) {
    reset();
}

Error AsmTms320f::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('@')) {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_DIR;
        scan = p;
        return OK;
    }

    if (p.expect('*')) {
        auto s = p.skipSpaces();
        if (p.expect('+')) {
            op.indexSign = false;
            if (p.expect('+')) {
                op.subMode = SUB_PRE;
            } else {
                op.subMode = SUB_DISP;
            }
        } else if (p.expect('-')) {
            op.indexSign = true;
            if (p.expect('-')) {
                op.subMode = SUB_PRE;
            } else {
                op.subMode = SUB_DISP;
            }
        }
        s = p.skipSpaces();
        op.reg = parseRegName(p, parser());
        if (op.reg == REG_UNDEF)
            return op.setErrorIf(s, UNKNOWN_OPERAND);
        s = p.skipSpaces();
        if (p.expect('+')) {
            if (op.subMode == SUB_NONE && p.expect('+')) {
                op.indexSign = false;
                op.subMode = SUB_POST;
            } else {
                return op.setErrorIf(s, UNKNOWN_OPERAND);
            }
        } else if (p.expect('-')) {
            if (op.subMode == SUB_NONE && p.expect('-')) {
                op.indexSign = true;
                op.subMode = SUB_POST;
            } else {
                return op.setErrorIf(s, UNKNOWN_OPERAND);
            }
        }
        if (p.skipSpaces().expect('(')) {
            op.dispAt = p;
            op.index = parseRegName(p, parser());
            if (op.index == REG_UNDEF) {
                op.val = parseInteger(p, op, ')');
                if (op.hasError())
                    return op.getError();
                op.hasDisp = true;
            }
            if (!p.skipSpaces().expect(')'))
                return op.setErrorIf(p, MISSING_CLOSING_PAREN);
        }
        if (p.skipSpaces().expect('%')) {
            op.subMode = SubMode(op.subMode | SUB_CIRC);
        } else if (p.iexpect('B')) {
            op.subMode = SubMode(op.subMode | SUB_BITR);
        }
        op.mode = M_IDIR;
        scan = p;
        return op.getError();
    }

    op.reg = parseRegName(p, parser());
    if (op.reg != REG_UNDEF) {
        if (isFloatReg(op.reg)) {
            op.mode = M_FREG;
        } else if (op.reg == REG_DP) {
            op.mode = R_DP;
        } else {
            op.mode = M_IREG;
        }
        scan = p;
        return OK;
    }

    op.val = parseExpr(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_IMM;
    scan = p;
    return OK;
}

Error AsmTms320f::encodeRelative(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    if (op.mode == M_FREG || op.mode == M_IREG || op.mode == R_DP) {
        insn.embed(encodeRegName(op.reg));
        return OK;
    }
    const auto base = insn.address() + (mode == M_DREL ? 3 : 1);
    const auto target = op.getError() ? base : op.val.getUnsigned();
    const auto delta = branchDelta(base, target, insn, op);
    if (overflowDelta(delta, 16))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.embed(UINT32_C(1) << 25);
    insn.embed(delta & UINT16_MAX);
    return insn.getError();
}

Error AsmTms320f::encodeIndirect(
        AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const {
    if (op.mode == M_FREG || op.mode == M_IREG || op.mode == R_DP) {
        if (mode == M_FIDR && !isFloatReg(op.reg))
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        const auto reg = encodeRegName(op.reg) | 0xE0;
        if (pos == P_00FF) {
            insn.embed(reg);
        } else {  // P_FF00
            insn.embed(reg << 8);
        }
        return insn.getError();
    }
    if (!isAuxiliaryReg(op.reg))
        return insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
    const auto noDisp = (mode == M_IDAT || mode == M_FDAT || mode == M_IDIR || mode == M_IIDR ||
                         mode == M_FIDR);
    const auto cf = insn.codeFormat();
    const auto opr3 = (cf == CF_TT) || (cf == CF_T0);
    const auto baseMode = op.subMode & SUB_BASE_MASK;
    Config::opcode_t disp = 1;
    if (op.hasDisp) {
        if (noDisp || opr3) {
            if (baseMode == SUB_PRE || baseMode == SUB_POST)
                if (op.val.getUnsigned() != 1)
                    return insn.setErrorIf(op.dispAt, OVERFLOW_RANGE);
            if (baseMode == SUB_DISP && !op.val.isZero())
                return insn.setErrorIf(op.dispAt, OVERFLOW_RANGE);
        }
        if (op.val.overflow(UINT8_MAX))
            return insn.setErrorIf(op.dispAt, OVERFLOW_RANGE);
        disp = op.val.getUnsigned();
    } else if (op.index != REG_UNDEF && !isIndexReg(op.index)) {
        return insn.setErrorIf(op.dispAt, REGISTER_NOT_ALLOWED);
    }
    Config::opcode_t mod = op.indexSign ? 0x01 : 0x00;
    const auto modifyMode = op.subMode & ~SUB_BASE_MASK;
    if (modifyMode != SUB_NONE && baseMode != SUB_POST)
        return insn.setErrorIf(op, UNKNOWN_OPERAND);
    switch (baseMode) {
    case SUB_DISP:
        if (op.index == REG_UNDEF) {
            if ((noDisp || opr3) && op.val.isZero()) {
                mod = 0x18;
            } else if (!noDisp) {
                insn.embed(disp & UINT8_MAX);
            }
        } else if (op.index == REG_IR0) {
            mod |= 0x08;
        } else if (op.index == REG_IR1) {
            mod |= 0x10;
        }
        break;
    case SUB_PRE:
        if (op.index == REG_UNDEF) {
            mod |= 0x02;
            if (!noDisp)
                insn.embed(disp & UINT8_MAX);
        } else if (op.index == REG_IR0) {
            mod |= 0x0A;
        } else if (op.index == REG_IR1) {
            mod |= 0x12;
        }
        break;
    case SUB_POST:
        if (op.index == REG_UNDEF) {
            mod |= 0x04;
            if (!noDisp)
                insn.embed(disp & UINT8_MAX);
        } else if (op.index == REG_IR0) {
            mod |= 0x0C;
        } else if (op.index == REG_IR1) {
            mod |= 0x14;
        }
        if (modifyMode == SUB_CIRC)
            mod |= 0x06;
        if (modifyMode == SUB_BITR) {
            if (op.index != REG_IR0)
                return insn.setErrorIf(op.dispAt, REGISTER_NOT_ALLOWED);
            mod = 0x19;
        }
        break;
    default:  // SUB_NONE
        mod = 0x18;
        break;
    }
    const auto ar = encodeAuxiliaryReg(op.reg);
    if (opr3) {
        if (op.pos == 1) {
            insn.embed(UINT32_C(1) << 22);
            insn.embed(mod << 3);
            insn.embed(ar);
        } else {  // op.pos == 2
            insn.embed(UINT32_C(1) << 21);
            insn.embed(mod << 11);
            insn.embed(ar << 8);
        }
    } else {
        switch (pos) {
        case P_00FF:
            insn.embed(mod << 3);
            insn.embed(ar);
            break;
        case P_FF00:
            insn.embed(mod << 11);
            insn.embed(ar << 8);
            break;
        default:
            insn.embed(UINT32_C(1) << 22);
            insn.embed(mod << 11);
            insn.embed(ar << 8);
        }
    }
    return insn.getError();
}

Error AsmTms320f::encodeInteger(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const {
    const auto cf = insn.codeFormat();
    const auto opr3 = (cf == CF_TT) || (cf == CF_T0);
    switch (op.mode) {
    case M_FREG:
    case M_IREG:
    case R_DP:
        if (mode == M_MEM)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        if (opr3 && op.pos == 2) {
            insn.embed(encodeRegName(op.reg) << 8);
        } else {
            insn.embed(encodeRegName(op.reg));
        }
        break;
    case M_DIR:
        if (mode == M_MREG || mode == M_IDAT || opr3)
            return insn.setErrorIf(op, ILLEGAL_OPERAND_MODE);
        if (op.val.overflow(MAX_ADDRESS))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embed(UINT32_C(1) << 21);
        insn.embed(op.val.getUnsigned() & UINT16_MAX);
        break;
    case M_IDIR:
        encodeIndirect(insn, op, mode, pos);
        break;
    case M_IMM:
        if (mode == M_MEM || mode == M_MREG || mode == M_IDAT || opr3)
            return insn.setErrorIf(op, ILLEGAL_OPERAND_MODE);
        if (op.val.isFloat())
            return insn.setErrorIf(op, FLOAT_NOT_SUPPORTED);
        insn.embed(UINT32_C(3) << 21);
        if (mode == M_GCNT) {
            if (op.val.overflow(63, -64))
                insn.setErrorIf(op, OVERFLOW_RANGE);
        } else if (mode == M_IGEN) {
            if (op.val.overflowInt16())
                insn.setErrorIf(op, OVERFLOW_RANGE);
        } else if (mode == M_UGEN) {
            if (op.val.overflow(UINT16_MAX))
                insn.setErrorIf(op, OVERFLOW_RANGE);
        }
        insn.embed(op.val.getInteger() & UINT16_MAX);
        break;
    default:
        break;
    }
    return insn.getError();
}

Error AsmTms320f::encodeFloat(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const {
    const auto cf = insn.codeFormat();
    const auto opr3 = (cf == CF_TT || cf == CF_T0);
    ti_float16_t f16;
    switch (op.mode) {
    case M_FREG:
    case M_IREG:
    case R_DP:
        if (!isFloatReg(op.reg) || mode == M_MEM)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        if (opr3 && op.pos == 2) {
            insn.embed(encodeRegName(op.reg) << 8);
        } else {
            insn.embed(encodeRegName(op.reg));
        }
        return OK;
    case M_DIR:
        if (mode == M_FDAT || opr3)
            return insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        if (op.val.overflow(MAX_ADDRESS))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embed(UINT32_C(1) << 21);
        insn.embed(op.val.getUnsigned() & UINT16_MAX);
        break;
    case M_IDIR:
        encodeIndirect(insn, op, mode, pos);
        break;
    case M_IMM:
        if (mode == M_FDAT || mode == M_MEM || opr3)
            return insn.setErrorIf(op, ILLEGAL_OPERAND_MODE);
        insn.embed(UINT32_C(3) << 21);
        f16.set(op.val.getFloat());
        if (mode == M_FLDM) {
            insn.embed(f16.bits() & 0xFFF);
        } else {
            insn.embed(f16.bits());
        }
        break;
    default:
        break;
    }
    return OK;
}

Error AsmTms320f::encodeOperand(
        AsmInsn &out, const AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const {
    switch (mode) {
    case M_IGEN:
    case M_UGEN:
    case M_GCNT:
    case M_MEM:
    case M_MREG:
    case M_IDAT:
        return encodeInteger(out, op, mode, pos);
    case M_FGEN:
    case M_FLDM:
    case M_FDAT:
        return encodeFloat(out, op, mode, pos);
    case R_R01:
        if (op.reg == REG_R0 || op.reg == REG_R1) {
            if (pos == P_0007)
                out.embed(encodeRegName(op.reg) << 16);
            if (pos == P_0038)
                out.embed(encodeRegName(op.reg) << 19);
            if (pos == P_0080 || (insn.maybeUnary() && insn.pos3() == P_0080))
                out.embed(static_cast<uint32_t>(op.reg == REG_R1) << 23);
        } else {
            out.setErrorIf(op, REGISTER_NOT_ALLOWED);
        }
        break;
    case R_R23:
        if (op.reg == REG_R2 || op.reg == REG_R3) {
            if (pos == P_0007)
                out.embed(encodeRegName(op.reg) << 16);
            if (pos == P_0038)
                out.embed(encodeRegName(op.reg) << 19);
            if (pos == P_0040 || (insn.maybeUnary() && insn.pos3() == P_0040))
                out.embed(static_cast<uint32_t>(op.reg == REG_R3) << 22);
        } else {
            out.setErrorIf(op, REGISTER_NOT_ALLOWED);
        }
        break;
    case M_FREG:
        if (!isFloatReg(op.reg))
            out.setErrorIf(op, REGISTER_NOT_ALLOWED);
        if (pos == P_0007) {
            out.embed((encodeRegName(op.reg) & 7) << 16);
            break;
        } else if (pos == P_0038) {
            out.embed((encodeRegName(op.reg) & 7) << 19);
            if (insn.maybeUnary() && insn.pos3() == P_01C0)
                out.embed((encodeRegName(op.reg) & 7) << 22);
            break;
        } else if (pos == P_01C0) {
            out.embed((encodeRegName(op.reg) & 7) << 22);
            break;
        }
        // Fall-through
    case M_IREG:
        out.embed(encodeRegName(op.reg) << 16);
        if (out.maybeUnary() && out.op2.mode == M_NONE)
            out.embed(encodeRegName(op.reg));
        break;
    case M_MSBA:
        if (op.val.overflow(MAX_ADDRESS))
            out.setErrorIf(op, OVERFLOW_RANGE);
        out.embed(op.val.getUnsigned() >> 16);
        break;
    case M_IREL:
    case M_DREL:
        encodeRelative(out, op, mode);
        break;
    case M_AD24:
        if (op.val.overflow(MAX_ADDRESS))
            out.setErrorIf(op, OVERFLOW_RANGE);
        out.embed(op.val.getUnsigned() & MAX_ADDRESS);
        break;
    case R_ARN:
        if (isAuxiliaryReg(op.reg)) {
            out.embed(encodeAuxiliaryReg(op.reg) << 22);
        } else {
            out.setErrorIf(op, REGISTER_NOT_ALLOWED);
        }
        break;
    case M_TVEC:
        if (op.val.overflow(31)) {
            out.setErrorIf(op, OVERFLOW_RANGE);
        } else {
            out.embed(0x20 | (op.val.getUnsigned() & 0x1F));
        }
        break;
    case M_IDIR:
    case M_IIDR:
    case M_FIDR:
        encodeIndirect(out, op, mode, pos);
        break;
    default:
        break;
    }
    return out.getError();
}

Error AsmTms320f::alignOrigin(StrScanner &scan, Insn &insn) {
    const auto start = insn.address();
    if ((start & 0x1F) == 0)
        return OK;
    auto n = 0x20 - (start & 0x1F);
    if (n < 4) {
        static constexpr auto NOP = UINT32_C(0x0C800000);
        while (n--)
            insn.emitUint32Le(NOP);
    } else {
        static constexpr auto BU = UINT32_C(0x68000000);
        insn.emitUint32Le(BU | (n - 1));
    }
    return OK;
}

Error AsmTms320f::defineInteger(StrScanner &scan, Insn &insn, uint32_t max, int32_t min) {
    ErrorAt error;
    do {
        scan.skipSpaces();
        auto p = scan;
        ErrorAt at;
        const auto val = parseInteger(p, at);
        if (!endOfLine(p) && *p != ',') {
            error.setErrorIf(scan, ILLEGAL_CONSTANT);
            break;
        }
        if (at.getError())
            error.setErrorIf(scan, at);
        if (val.overflow(max, min))
            error.setErrorIf(scan, OVERFLOW_RANGE);
        error.setErrorIf(scan, insn.emitUint32Le(val.getUnsigned() & max));
        scan = p;
    } while (scan.skipSpaces().expect(','));
    return insn.setErrorIf(error);
}

Error AsmTms320f::defineFloat(StrScanner &scan, Insn &insn, uint_fast8_t bits) {
    ErrorAt error;
    do {
        scan.skipSpaces();
        auto p = scan;
        ErrorAt at;
        const auto val = parseExpr(p, at);
        if (!endOfLine(p) && *p != ',') {
            error.setErrorIf(scan, ILLEGAL_CONSTANT);
            break;
        }
        if (at.getError())
            error.setErrorIf(scan, at);
        if (bits == 16) {
#if !defined(LIBASM_ASM_NOFLOAT)
            ti_float16_t f16;
            f16.set(val.getFloat());
            insn.emitUint32Le(f16.bits());
#else
            error.setErrorIf(scam, FLOAT_NOT_SUPPORTED);
            insn.emitUint32Le(0);
#endif
        } else if (bits == 32) {
#if !defined(LIBASM_ASM_NOFLOAT)
            ti_float32_t f32;
            f32.set(val.getFloat());
            insn.emitUint32Le(f32.bits());
#else
            error.setErrorIf(scam, FLOAT_NOT_SUPPORTED);
            insn.emitUint32Le(0);
#endif
        } else {  // bits == 40) {
#if !defined(LIBASM_ASM_NOFLOAT)
            ti_float40_t f40;
            f40.set(val.getFloat());
            insn.emitUint32Le(f40.bits());
            insn.emitUint32Le(f40.bits() >> 8);
#else
            error.setErrorIf(scam, FLOAT_NOT_SUPPORTED);
            insn.emitUint32Le(0);
            insn.emitUint32Le(0);
#endif
        }
        scan = p;
    } while (scan.skipSpaces().expect(','));
    return insn.setErrorIf(error);
}

Error AsmTms320f::processPseudo(StrScanner &scan, Insn &insn) {
    if (strcasecmp_P(insn.name(), TEXT_dALIGN) == 0)
        return alignOrigin(scan, insn);
    if (strcasecmp_P(insn.name(), TEXT_dBYTE) == 0)
        return defineInteger(scan, insn, UINT8_MAX, INT8_MIN);
    if (strcasecmp_P(insn.name(), TEXT_dHWORD) == 0)
        return defineInteger(scan, insn, UINT16_MAX, INT16_MIN);
    if (strcasecmp_P(insn.name(), TEXT_dFLOAT) == 0)
        return defineFloat(scan, insn, 32);
    if (strcasecmp_P(insn.name(), TEXT_dLDOUBLE) == 0)
        return defineFloat(scan, insn, 40);
    if (strcasecmp_P(insn.name(), TEXT_dSFLOAT) == 0)
        return defineFloat(scan, insn, 16);
    if (strcmp_P(insn.name(), TEXT_PARALLEL) == 0) {
        insn.setContinueMark_P(TEXT_PARALLEL);
        StrScanner symbol;
        _parser.readInstruction(scan, symbol);
        insn.nameBuffer().reset().text(symbol);
        return encodeImpl(scan.skipSpaces(), insn);
    }
    return Assembler::processPseudo(scan, insn);
}

void AsmInsn::copyFrom(const AsmInsn &from) {
    this->nameBuffer().reset().rtext(from.name());
    this->resetAddress(from.address());
    this->op1 = from.op1;
    this->op2 = from.op2;
    this->op3 = from.op3;
    this->setOpCode(from.opCode());
    this->setFlags(from.flags());
}

Error AsmTms320f::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.op1) && insn.op1.hasError())
        return _insn.setError(insn.op1);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.op2) && insn.op2.hasError())
            return _insn.setError(insn.op2);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, insn.op3) && insn.op3.hasError())
            return _insn.setError(insn.op3);
        scan.skipSpaces();
    }

    auto &search = _insn.hasContinue() ? _prev : insn;
    if (_insn.hasContinue()) {
        search.para = &insn;
    } else {
        search.para = nullptr;
    }
    if (searchName(cpuType(), search))
        return _insn.setError(insn.op1, search);

    encodeOperand(insn, search, search.op1, search.mode1(), search.pos1());
    encodeOperand(insn, search, search.op2, search.mode2(), search.pos2());
    encodeOperand(insn, search, search.op3, search.mode3(), search.pos3());
    if (search.para) {
        auto &para = *search.para;
        para.resetAddress(_prev.address());
        encodeOperand(insn, para, para.op1, para.mode1(), para.pos1());
        encodeOperand(insn, para, para.op2, para.mode2(), para.pos2());
        encodeOperand(insn, para, para.op3, para.mode3(), para.pos3());
    }
    insn.emitInsn();
    if (insn.hasContinue()) {
        insn.clearContinueMark();
    } else {
        _prev.copyFrom(insn);
    }
    return _insn.setErrorIf(insn);
}

}  // namespace tms320f
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
