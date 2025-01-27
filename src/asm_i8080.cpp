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

#include "asm_i8080.h"
#include "table_i8080.h"
#include "text_common.h"

namespace libasm {
namespace i8080 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::option;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_DB,   &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DEFB, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DEFM, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DEFS, &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DEFW, &Assembler::defineDataConstant, Assembler::DATA_WORD},
    {TEXT_DS,   &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DW,   &Assembler::defineDataConstant, Assembler::DATA_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

struct I8080SymbolParser final : PrefixSymbolParser, Singleton<I8080SymbolParser> {
    I8080SymbolParser() : PrefixSymbolParser(PSTR_AT_QUESTION) {}
};

}  // namespace

const ValueParser::Plugins &AsmI8080::defaultPlugins() {
    static struct final : ValueParser::IntelPlugins {
        const SymbolParser &symbol() const { return I8080SymbolParser::singleton(); }
    } PLUGINS;
    return PLUGINS;
}

AsmI8080::AsmI8080(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_zilog),
      Config(TABLE),
      _opt_zilog(this, &AsmI8080::setZilogSyntax, OPT_BOOL_ZILOG_SYNTAX, OPT_DESC_ZILOG_SYNTAX) {
    reset();
}

void AsmI8080::reset() {
    Assembler::reset();
    setZilogSyntax(false);
}

Error AsmI8080::setZilogSyntax(bool enable) {
    _zilogSyntax = enable;
    return OK;
}

void AsmI8080::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    insn.setErrorIf(op);
    uint16_t val16 = op.val.getUnsigned();
    switch (mode) {
    case M_IOA:
        if (op.val.overflow(UINT8_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        /* Fall-through */
    case M_IM8:
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(val16);
        return;
    case M_IM16:
    case M_ABS:
        insn.emitOperand16(val16);
        return;
    case M_PTR:
        insn.embed(encodePointerReg(op.reg) << 4);
        return;
    case M_STK:
        insn.embed(encodeStackReg(op.reg) << 4);
        return;
    case M_IDX:
        insn.embed(encodeIndexReg(op.reg) << 4);
        return;
    case M_SRC:
        insn.embed(encodeDataReg(op.reg));
        return;
    case M_DST:
        insn.embed(encodeDataReg(op.reg) << 3);
        return;
    case M_VEC:
        if (_zilogSyntax) {
            if ((val16 & ~0x38) != 0) {
                val16 &= ~0x38;
                insn.setErrorIf(op, ILLEGAL_OPERAND);
            }
            insn.embed(val16);
        } else {
            if (op.val.overflow(7))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.embed((val16 & 7) << 3);
        }
        return;
    case M_CC:
        insn.embed(val16 << 3);
        return;
    default:
        return;
    }
}

Error AsmI8080::parseZilogOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    // 'C' is either C-reg or C-condition
    auto a = p;
    const auto reg = parseRegName(a, true, parser());
    if (reg == REG_C) {
        op.mode = R_C;
        op.reg = REG_C;
        op.val.setUnsigned(encodeCcName(CC_C));
        scan = a;
        return OK;
    }

    const auto cc = parseCcName(p, parser());
    if (cc != CC_UNDEF) {
        op.mode = M_CC;
        op.val.setUnsigned(encodeCcName(cc));
        scan = p;
        return OK;
    }

    op.reg = parseRegName(p, true, parser());
    if (op.reg != REG_UNDEF) {
        switch (op.reg) {
        case REG_B:
        case REG_D:
        case REG_E:
        case REG_H:
        case REG_L:
            op.mode = M_SRC;
            break;
        default:
            op.mode = AddrMode(op.reg + R_BASE);
            break;
        }
        scan = p;
        return OK;
    }
    if (p.expect('(')) {
        const auto regp = p.skipSpaces();
        op.reg = parseRegName(p, true, parser());
        if (op.reg == REG_UNDEF) {
            op.val = parseInteger(p, op, ')');
            if (op.hasError())
                return op.getError();
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            op.mode = M_ABS;
            scan = p;
            return OK;
        }
        if (p.skipSpaces().expect(')')) {
            switch (op.reg) {
            case REG_BC:
            case REG_DE:
                op.mode = M_IDX;
                break;
            case REG_HL:
            case REG_SP:
                op.mode = AddrMode(op.reg + I_BASE);
                break;
            default:
                return op.setError(regp, REGISTER_NOT_ALLOWED);
            }
            scan = p;
            return OK;
        }
        return op.setError(UNKNOWN_OPERAND);
    }
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_IM16;
    scan = p;
    return OK;
}

Error AsmI8080::parseIntelOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    op.reg = parseRegName(p, _zilogSyntax, parser());
    if (op.reg != REG_UNDEF) {
        switch (op.reg) {
        case REG_H:
            op.mode = R_H;
            break;
        case REG_SP:
            op.mode = M_PTR;
            break;
        case REG_PSW:
            op.mode = M_STK;
            break;
        case REG_B:
        case REG_D:
            op.mode = M_IDX;
            break;
        default:
            op.mode = M_SRC;
            break;
        }
        scan = p;
        return OK;
    }
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_IM16;
    scan = p;
    return OK;
}

Error AsmI8080::parseOperand(StrScanner &scan, Operand &op) const {
    return _zilogSyntax ? parseZilogOperand(scan, op) : parseIntelOperand(scan, op);
}

Error AsmI8080::processPseudo(StrScanner &scan, Insn &insn) {
    const auto at = scan;
    if (strcasecmp_P(insn.name(), PSTR("Z80SYNTAX")) == 0) {
        const auto error = _opt_zilog.set(scan);
        return error ? insn.setErrorIf(at, error) : OK;
    }
    return Assembler::processPseudo(scan, insn);
}

Error AsmI8080::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
        return _insn.setError(insn.dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.srcOp) && insn.hasError())
            return _insn.setError(insn.srcOp);
        scan.skipSpaces();
    }

    if (searchName(cpuType(), insn, _zilogSyntax))
        return _insn.setError(insn.dstOp, insn);

    encodeOperand(insn, insn.dstOp, insn.dst());
    encodeOperand(insn, insn.srcOp, insn.src());
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace i8080
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
