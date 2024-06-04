/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "asm_tms7000.h"

#include "reg_tms7000.h"
#include "table_tms7000.h"
#include "text_common.h"

namespace libasm {
namespace tms7000 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_AORG, &Assembler::defineOrigin},
    {TEXT_BSS,  &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_BYTE, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DATA, &Assembler::defineDataConstant, Assembler::DATA_WORD},
    {TEXT_TEXT, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmTms7000::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return TexasNumberParser::singleton(); }
        const CommentParser &comment() const override { return AsteriskCommentParser::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmTms7000::AsmTms7000(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

bool AsmTms7000::hasIndexB(StrScanner &scan, ErrorAt &error) const {
    auto p = scan;
    if (p.skipSpaces().expect('(')) {
        const auto at = p.skipSpaces();
        const auto reg = parseRegName(p);
        if (reg == REG_B) {
            if (p.skipSpaces().expect(')')) {
                scan = p;
                return true;
            }
            error.setError(p, MISSING_CLOSING_PAREN);
        } else if (reg == REG_UNDEF) {
            error.setError(at, UNKNOWN_OPERAND);
        } else {
            error.setError(at, REGISTER_NOT_ALLOWED);
        }
    }
    return false;
}

namespace {
AddrMode toAddrMode(RegName name, uint16_t &regno) {
    switch (name) {
    case REG_A:
        regno = 0;
        return M_A;
    case REG_B:
        regno = 1;
        return M_B;
    case REG_ST:
        return M_ST;
    default:
        if (isRegName(name)) {
            regno = toRegNum(name);
            return M_RN;
        }
        if (isPortName(name)) {
            regno = toPortNum(name);
            return M_PN;
        }
        return M_NONE;
    }
}
}  // namespace

Error AsmTms7000::parseOperand(StrScanner &scan, Operand &op) const {
    op.setAt(scan.skipSpaces());

    auto p = scan;
    if (p.expect('%')) {
        const auto val = parseExpr(p, op);
        if (op.hasError())
            return op.getError();
        op.val16 = val.getUnsigned();
        if (hasIndexB(p, op)) {
            op.mode = M_BIMM;
            scan = p;
            return OK;
        }
        if (op.hasError())
            return op.getError();
        op.mode = val.overflowUint8() ? M_IM16 : M_IM8;
        scan = p;
        return OK;
    }

    auto indir = p.expect('*');
    const auto reg = parseRegName(p.skipSpaces());
    if (op.hasError())
        return op.getError();
    if (reg != REG_UNDEF) {
        if (!indir) {
            op.mode = toAddrMode(reg, op.val16);
        } else if (isRegName(reg)) {
            op.mode = M_IDIR;
            op.val16 = toRegNum(reg);
        } else {
            return op.setError(UNKNOWN_OPERAND);
        }
        scan = p;
        return OK;
    }

    const auto abs = p.expect('@');
    if (abs && !indir)
        indir = p.skipSpaces().expect('*');
    if (abs && indir)
        return op.setError(UNKNOWN_OPERAND);
    const auto val = parseExpr(p.skipSpaces(), op);
    if (op.hasError())
        return op.getError();
    op.val16 = val.getUnsigned();
    if (hasIndexB(p, op)) {
        if (!indir && abs) {
            op.mode = M_BIDX;
        } else {
            return op.setError(UNKNOWN_OPERAND);
        }
    } else if (op.hasError()) {
        return op.getError();
    } else if (indir) {
        if (val.getUnsigned() < 0x100) {
            op.mode = M_IDIR;
        } else {
            return op.setError(OVERFLOW_RANGE);
        }
    } else if (abs) {
        op.mode = M_ABS;
    } else {
        const auto v = val.getUnsigned();
        if (v < 0x100) {
            op.mode = M_ADRR;
        } else if (v < 0x200) {
            op.mode = M_ADRP;
        } else {
            op.mode = M_REL;
        }
    }
    scan = p;
    return OK;
}

void AsmTms7000::emitRelative(AsmInsn &insn, const Operand &op) const {
    const auto len = insn.length();
    const auto base = insn.address() + (len ? len + 1 : 2);
    const auto target = op.getError() ? base : op.val16;
    const auto delta = branchDelta(base, target, insn, op);
    if (overflowInt8(delta))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand8(delta);
}

void AsmTms7000::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_RN:
    case M_PN:
    case M_IM8:
    case M_IDIR:
        insn.emitOperand8(op.val16);
        break;
    case M_REL:
        emitRelative(insn, op);
        break;
    case M_ABS:
    case M_BIDX:
    case M_IM16:
    case M_BIMM:
        insn.emitOperand16(op.val16);
        break;
    case M_TRAP:
        if (op.val16 >= 24) {
            insn.setError(op, OVERFLOW_RANGE);
        } else {
            insn.setOpCode(insn.opCode() + 23 - op.val16);
        }
        break;
    default:
        break;
    }
}

Error AsmTms7000::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (TABLE.hasOperand(cpuType(), insn)) {
        if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
            return _insn.setError(insn.srcOp);
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
                return _insn.setError(insn.dstOp);
        }
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.extOp) && insn.extOp.hasError())
                return _insn.setError(insn.extOp);
        }
    }
    scan.skipSpaces();

    if (_insn.setErrorIf(insn.srcOp, TABLE.searchName(cpuType(), insn)))
        return _insn.getError();

    encodeOperand(insn, insn.srcOp, insn.src());
    encodeOperand(insn, insn.dstOp, insn.dst());
    encodeOperand(insn, insn.extOp, insn.ext());
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace tms7000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
