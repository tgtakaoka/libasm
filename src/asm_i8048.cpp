/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "asm_i8048.h"

#include <ctype.h>

#include "reg_i8048.h"
#include "table_i8048.h"

namespace libasm {
namespace i8048 {

using namespace reg;

struct AsmI8048::Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    uint16_t val16;
    Operand() : mode(M_NONE), reg(REG_UNDEF), val16(0) {}
};

const ValueParser::Plugins &AsmI8048::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return IntelNumberParser::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmI8048::AsmI8048(const ValueParser::Plugins &plugins)
    : Assembler(nullptr, plugins), Config(TABLE) {
    reset();
}

Error AsmI8048::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#')) {
        op.val16 = parseExpr16(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_IMM8;
        scan = p;
        return OK;
    }

    const auto indir = p.expect('@');
    if (indir && isspace(*p))
        return op.setError(UNKNOWN_OPERAND);

    const auto regp = p;
    op.reg = parseRegName(p);
    if (op.reg != REG_UNDEF) {
        if (indir) {
            switch (op.reg) {
            case REG_A:
                op.mode = M_IA;
                break;
            case REG_R0:
            case REG_R1:
                op.mode = M_IR;
                break;
            case REG_R3:
                op.mode = M_IR3;
                break;
            default:
                return op.setError(regp, REGISTER_NOT_ALLOWED);
            }
            scan = p;
            return OK;
        }
        switch (op.reg) {
        case REG_BUS:
            op.mode = M_BUS;
            break;
        case REG_P:
            op.mode = M_P;
            break;
        case REG_P1:
            op.mode = M_P1;
            break;
        case REG_P2:
            op.mode = M_P2;
            break;
        case REG_P4:
        case REG_P5:
        case REG_P6:
        case REG_P7:
            op.mode = M_PEXT;
            break;
        case REG_A:
            op.mode = M_A;
            break;
        case REG_PSW:
            op.mode = M_PSW;
            break;
        case REG_F0:
        case REG_F1:
            op.mode = M_F;
            break;
        case REG_C:
            op.mode = M_C;
            break;
        case REG_I:
            op.mode = M_I;
            break;
        case REG_RB0:
        case REG_RB1:
            op.mode = M_RB;
            break;
        case REG_MB0:
        case REG_MB1:
            op.mode = M_MB;
            break;
        case REG_T:
            op.mode = M_T;
            break;
        case REG_CLK:
            op.mode = M_CLK;
            break;
        case REG_CNT:
            op.mode = M_CNT;
            break;
        case REG_TCNT:
            op.mode = M_TCNT;
            break;
        case REG_TCNTI:
            op.mode = M_TCNTI;
            break;
        default:
            if (isRReg(op.reg)) {
                op.mode = M_R;
            } else {
                return op.setError(UNKNOWN_OPERAND);
            }
        }
        scan = p;
        return OK;
    }
    if (indir)
        return op.setError(UNKNOWN_OPERAND);

    op.val16 = parseExpr16(p, op);
    if (op.hasError())
        return op.getError();
    scan = p;
    op.mode = M_AD11;
    return OK;
}

void AsmI8048::encodeAddress(InsnI8048 &insn, const AddrMode mode, const Operand &op) {
    if (mode == M_AD08) {
        const Config::uintptr_t page = (insn.address() + 1) & ~0xFF;
        if ((op.val16 & ~0xFF) != page)
            setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(op.val16);
        return;
    }
    if (checkAddr(op.val16))
        setErrorIf(op, OVERFLOW_RANGE);
    insn.embed((op.val16 >> 3) & 0xE0);
    insn.emitOperand8(op.val16);
}

void AsmI8048::encodeOperand(InsnI8048 &insn, const AddrMode mode, const Operand &op) {
    switch (mode) {
    case M_IR:
    case M_IR3:
    case M_R:
        insn.embed(encodeRRegName(op.reg));
        return;
    case M_P12:
    case M_PEXT:
        insn.embed(encodePortName(op.reg));
        return;
    case M_AD08:
    case M_AD11:
        encodeAddress(insn, mode, op);
        return;
    case M_IMM8:
    case M_BIT8:
        if (overflowUint8(op.val16))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
        return;
    case M_BITN:
        if (op.val16 >= 8)
            setErrorIf(op, ILLEGAL_BIT_NUMBER);
        insn.embed((op.val16 & 7) << 5);
        return;
    case M_F:
        insn.embed(op.reg == REG_F1 ? 0x20 : 0);
        return;
    case M_RB:
        insn.embed(op.reg == REG_RB1 ? 0x10 : 0);
        return;
    case M_MB:
        insn.embed(op.reg == REG_MB1 ? 0x10 : 0);
        return;
    default:
        return;
    }
}

Error AsmI8048::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnI8048 insn(_insn);
    Operand dstOp, srcOp;
    if (parseOperand(scan, dstOp) && dstOp.hasError())
        return setError(dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, srcOp) && srcOp.hasError())
            return setError(srcOp);
        scan.skipSpaces();
    }
    if (!endOfLine(scan))
        return setError(GARBAGE_AT_END);
    setErrorIf(dstOp);
    setErrorIf(srcOp);

    insn.setAddrMode(dstOp.mode, srcOp.mode);
    const auto error = TABLE.searchName(cpuType(), insn);
    if (error)
        return setError(dstOp, error);

    encodeOperand(insn, insn.dst(), dstOp);
    encodeOperand(insn, insn.src(), srcOp);
    insn.emitInsn();
    return setErrorIf(insn);
}

}  // namespace i8048
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
