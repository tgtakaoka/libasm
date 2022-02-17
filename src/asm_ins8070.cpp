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

#include "asm_ins8070.h"

namespace libasm {
namespace ins8070 {

static constexpr ValueParser::FuncParser::FuncId FUNC_ADDR{
        ValueParser::FuncParser::EXTENDED_ID_BASE};

struct Ins8070FuncParser : ValueParser::FuncParser {
    ValueParser::FuncParser::FuncId isFunc(const StrScanner &symbol) const override {
        if (symbol.iequals_P(PSTR("addr")))
            return FUNC_ADDR;
        return ValueParser::FuncParser::isFunc(symbol);
    }

    Error parseFunc(ValueParser &parser, const FuncId id, StrScanner &scan, Value &val) override {
        if (id == FUNC_ADDR) {
            const auto arg = parseArg(parser, scan);
            const auto v = arg.getUnsigned();
            const auto a = v - 1;
            val.setValue(a & 0xFFFF);
            return getError();
        }
        return ValueParser::FuncParser::parseFunc(parser, id, scan, val);
    }
};

AsmIns8070::AsmIns8070() : Assembler(_parser, TableIns8070) {
    static Ins8070FuncParser functionParser;
    reset();
    _parser.setFuncParser(&functionParser);
}

Error AsmIns8070::emitAbsolute(InsnIns8070 &insn, const Operand &op) {
    // PC will be +1 before fetching instruction.
    const Config::uintptr_t target = op.getError() ? 0 : op.val16 - 1;
    insn.emitOperand16(target);
    return OK;
}

Error AsmIns8070::emitImmediate(InsnIns8070 &insn, const Operand &op) {
    if (insn.oprSize() == SZ_WORD)
        insn.emitOperand16(op.val16);
    if (insn.oprSize() == SZ_BYTE) {
        if (overflowUint8(op.val16))
            return setError(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
    }
    return OK;
}

Error AsmIns8070::emitRelative(InsnIns8070 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 1;
    // PC will be +1 before feting instruction
    const uint8_t fetch = (insn.addrMode() == RELATIVE) ? 1 : 0;
    const Config::uintptr_t target = (op.getError() ? base + fetch : op.val16) - fetch;
    const Config::ptrdiff_t offset = target - base;
    if (overflowRel8(offset))
        return setError(op, OPERAND_TOO_FAR);
    insn.emitOperand8(static_cast<uint8_t>(offset));
    return OK;
}

Error AsmIns8070::emitGeneric(InsnIns8070 &insn, const Operand &op) {
    if (op.format == OPR_IM) {
        insn.embed(4);
        return emitImmediate(insn, op);
    }
    if ((op.format == OPR_16 || op.format == OPR_4) && op.reg == REG_UNDEF) {
        const Config::uintptr_t target = op.getError() ? 0xFF00 : op.val16;
        if (target < 0xFF00)
            return setError(op, OVERFLOW_RANGE);
        insn.embed(5);
        insn.emitOperand8(static_cast<uint8_t>(target));
        return OK;
    }
    if (op.reg == REG_PC)
        return emitRelative(insn, op);

    const Config::ptrdiff_t offset = static_cast<Config::uintptr_t>(op.val16);
    if (overflowRel8(offset))
        return setError(op, OVERFLOW_RANGE);
    insn.embed(RegIns8070::encodePointerReg(op.reg));
    if (op.autoIndex)
        insn.embed(4);
    insn.emitOperand8(static_cast<uint8_t>(offset));
    return OK;
}

Error AsmIns8070::emitOperand(InsnIns8070 &insn, OprFormat format, const Operand &op) {
    switch (format) {
    case OPR_PN:
    case OPR_BR:
        insn.embed(RegIns8070::encodePointerReg(op.reg));
        break;
    case OPR_4:
        insn.embed(op.val16 & 0x0F);
        break;
    case OPR_IM:
        return emitImmediate(insn, op);
    case OPR_16:
        return emitAbsolute(insn, op);
    case OPR_PR:
    case OPR_GN:
        return emitGeneric(insn, op);
    case OPR_RL:
        return emitRelative(insn, op);
    default:
        break;
    }
    return OK;
}

Error AsmIns8070::parseOperand(StrScanner &scan, Operand &op) {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p))
        return OK;

    if (p.expect('#') || p.expect('=')) {
        op.val16 = parseExpr16(p, op);
        if (parserError())
            return getError();
        op.format = OPR_IM;
        scan = p;
        return OK;
    }

    const RegName reg = RegIns8070::parseRegName(p);
    if (reg != REG_UNDEF) {
        OprFormat opr;
        switch (reg) {
        case REG_A:
            opr = OPR_A;
            break;
        case REG_E:
            opr = OPR_E;
            break;
        case REG_S:
            opr = OPR_S;
            break;
        case REG_EA:
            opr = OPR_EA;
            break;
        case REG_T:
            opr = OPR_T;
            break;
        case REG_SP:
            opr = OPR_SP;
            break;
        case REG_PC:
            opr = OPR_BR;
            break;
        default:
            opr = OPR_PN;
            break;
        }
        op.reg = reg;
        op.format = opr;
        scan = p;
        return OK;
    }

    bool autoIndex = p.expect('@');
    op.val16 = parseExpr16(p, op);
    if (parserError())
        return getError();

    if (*p.skipSpaces() == '(')  // SC/MP style
        return setError(p, MISSING_COMMA);
    if (p.expect(',')) {
        p.skipSpaces();
        if (!autoIndex && p.expect('@'))
            autoIndex = true;
        const RegName ptr = RegIns8070::parseRegName(p);
        if (!RegIns8070::isPointerReg(ptr))
            setError(p, UNKNOWN_OPERAND);
        if (autoIndex && (ptr == REG_PC || ptr == REG_SP))
            return setError(p, REGISTER_NOT_ALLOWED);
        op.reg = ptr;
        op.autoIndex = autoIndex;
        op.format = (autoIndex || ptr == REG_SP || ptr == REG_PC) ? OPR_GN : OPR_PR;
        scan = p;
        return OK;
    }

    op.format = (op.val16 < 0x10) ? OPR_4 : OPR_16;
    scan = p;
    return OK;
}

Error AsmIns8070::encode(StrScanner &scan, Insn &_insn) {
    InsnIns8070 insn(_insn);
    insn.setName(_parser.readSymbol(scan));

    Operand dst, src;
    if (parseOperand(scan, dst))
        return getError();
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, src))
            return getError();
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(dst);
    setErrorIf(src);

    insn.setOprFormats(dst.format, src.format);
    if (TableIns8070.searchName(insn))
        return setError(TableIns8070.getError());

    if (emitOperand(insn, insn.dstOpr(), dst))
        return getError();
    if (emitOperand(insn, insn.srcOpr(), src))
        return getError();
    insn.emitInsn();
    return getError();
}

}  // namespace ins8070
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
