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

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_DB, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DS, &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DW, &Assembler::defineDataConstant, Assembler::DATA_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmI8080::defaultPlugins() {
    static struct final : ValueParser::Plugins {
        const NumberParser &number() const { return IntelNumberParser::singleton(); }
        const SymbolParser &symbol() const { return _symbol; }
        const OperatorParser &operators() const { return IntelOperatorParser::singleton(); }
        const PrefixSymbolParser _symbol{PSTR_AT_QUESTION};
    } PLUGINS;
    return PLUGINS;
}

AsmI8080::AsmI8080(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

void AsmI8080::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_IOA:
        if (op.val16 >= 0x100)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        /* Fall-through */
    case M_IM8:
        if (overflowUint8(op.val16))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
        return;
    case M_IM16:
    case M_ABS:
        insn.emitOperand16(op.val16);
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
    case M_REG:
        insn.embed(encodeDataReg(op.reg));
        return;
    case M_DST:
        insn.embed(encodeDataReg(op.reg) << 3);
        return;
    case M_VEC:
        if (op.val16 >= 8)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embed((op.val16 & 7) << 3);
        return;
    default:
        return;
    }
}

Error AsmI8080::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    op.reg = parseRegName(p);
    if (op.reg != REG_UNDEF) {
        switch (op.reg) {
        case REG_H:
            op.mode = M_REGH;
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
            op.mode = M_REG;
            break;
        }
        scan = p;
        return OK;
    }
    op.val16 = parseExpr16(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_IM16;
    scan = p;
    return OK;
}

Error AsmI8080::encodeImpl(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
        return setError(insn.dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.srcOp) && insn.hasError())
            return setError(insn.srcOp);
        scan.skipSpaces();
    }

    if (setErrorIf(insn.dstOp, TABLE.searchName(cpuType(), insn)))
        return getError();

    encodeOperand(insn, insn.dstOp, insn.dst());
    encodeOperand(insn, insn.srcOp, insn.src());
    insn.emitInsn();
    return setError(insn);
}

}  // namespace i8080
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
