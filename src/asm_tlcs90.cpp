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

#include "asm_tlcs90.h"
#include "table_tlcs90.h"
#include "text_common.h"

namespace libasm {
namespace tlcs90 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_DB,   &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DL,   &Assembler::defineDataConstant, Assembler::DATA_LONG},
    {TEXT_DS,   &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DW,   &Assembler::defineDataConstant, Assembler::DATA_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

AsmTlcs90::AsmTlcs90(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

const ValueParser::Plugins &AsmTlcs90::defaultPlugins() {
    return ValueParser::Plugins::intel();
}

namespace {

constexpr Config::opcode_t JRL = 0x1B;
constexpr Config::opcode_t JR = 0xC8;

bool maySmartBranch(const AsmInsn &insn) {
    const auto opc = insn.opCode();
    return opc == JRL || opc == JR;
}

void shortBranch(AsmInsn &insn) {
    insn.setOpCode(JR);
}

void longBranch(AsmInsn &insn) {
    insn.setOpCode(JRL);
}

}  // namespace

void AsmTlcs90::encodeRelative(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    const auto base = insn.address() + 2;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    const auto delta = branchDelta(base, target, insn, op);
    const auto smartBranch = _smartBranch && maySmartBranch(insn);
    if (mode == M_REL8 && !smartBranch) {
        if (overflowDelta(delta, 8))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
    short_branch:
        insn.emitInsn(insn.opCode());
        insn.emitByte(delta);
        return;
    }
    if (mode == M_REL16 && !smartBranch) {
    long_branch:
        insn.emitInsn(insn.opCode());
        insn.emitUint16(delta);
        return;
    }
    if (op.getError() || overflowDelta(delta, 8)) {
        longBranch(insn);
        goto long_branch;
    }
    shortBranch(insn);
    goto short_branch;
}

void AsmTlcs90::encodeOperand(
        AsmInsn &insn, AddrMode mode, const Operand &op, Config::opcode_t opc) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_IMM8:
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        /* Fall-through */
    case M_DIR:
        insn.emitInsn(opc);
        insn.emitByte(op.val.getUnsigned());
        return;
    case M_IMM16:
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        /* Fall-through */
    case M_EXT:
        insn.emitInsn(opc);
        insn.emitUint16(op.val.getUnsigned());
        return;
    case M_BIT:
        if (op.val.overflow(7))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitInsn(opc | (op.val.getUnsigned() & 7));
        return;
    case M_REL8:
    case M_REL16:
        insn.setOpCode(opc);
        encodeRelative(insn, mode, op);
        return;
    case M_IND:
        insn.emitInsn(opc | encodeReg16(op.reg));
        return;
    case M_IDX:
    case M_IXPD:
        if (op.val.overflowInt8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitInsn(opc | encodeIndexReg(op.reg));
        insn.emitByte(op.val.getUnsigned());
        return;
    case M_CC:
        insn.emitInsn(opc | encodeCcName(op.cc));
        return;
    case M_STACK:
        insn.emitInsn(opc | encodeStackReg(op.reg));
        return;
    case M_REG8:
        insn.emitInsn(opc | encodeReg8(op.reg));
        return;
    case M_REG16:
        insn.emitInsn(opc | encodeReg16(op.reg));
        return;
    case M_REGIX:
        insn.emitInsn(opc | encodeIndexReg(op.reg));
        return;
    case R_A:
    case R_HL:
        return;
    default:
        insn.emitInsn(opc);
        return;
    }
}

AddrMode reg2mode(RegName reg) {
    static constexpr AddrMode MODE[] PROGMEM = {
            R_BC,     // REG_BC=0
            R_DE,     // REG_DE=1
            R_HL,     // REG_HL=2
            M_NONE,   // 3
            M_REGIX,  // REG_IX=4
            M_REGIX,  // REG_IY=5
            R_SP,     // REG_SP=6
            R_AF,     // REG_AF=7
            M_REG8,   // REG_B=8
            R_C,      // REG_C=9
            M_REG8,   // REG_D=10
            M_REG8,   // REG_E=11
            M_REG8,   // REG_H=12
            M_REG8,   // REG_L=13
            R_A,      // REG_A=14
            R_AFP,    // REG_AFP=15
    };
    return AddrMode(pgm_read_byte(&MODE[reg]));
}

Error AsmTlcs90::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    const auto cc = parseCcName(p, parser());
    if (cc != CC_UNDEF) {
        op.mode = M_CC;
        op.cc = cc;
        if (cc == CC_C) {
            // 'C' is either REG_C or CC_C
            op.mode = R_C;
            op.reg = REG_C;
        }
        scan = p;
        return OK;
    }

    const auto paren = p.expect('(') ? ')' : 0;
    const auto regAt = p.skipSpaces();
    const auto reg = parseRegName(p, parser());
    if (reg == REG_UNDEF) {
        op.val = parseInteger(p, op, paren);
        if (op.hasError())
            return op.getError();
        if (paren && !p.skipSpaces().expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        if (paren) {  // (nnnn)
            op.mode = op.getError() ? M_SYM : (op.val.getUnsigned() >= 0xFF00 ? M_DIR : M_EXT);
        } else {  // nnnn
            op.mode = M_IMM16;
        }
        scan = p;
        return OK;
    }
    p.skipSpaces();
    op.reg = reg;
    if (paren && p.expect(')')) {  // (rr)
        if (isReg16(reg)) {
            op.mode = M_IND;
            scan = p;
            return OK;
        }
        return op.setError(regAt, REGISTER_NOT_ALLOWED);
    }
    const auto dispAt = p;
    const auto disp = p.expect([](char c) { return c == '+' || c == '-'; });
    if (disp) {
        if (reg == REG_HL && disp == '+') {
            const auto idxAt = p.skipSpaces();
            const auto idx = parseRegName(p, parser());
            if (idx != REG_UNDEF) {
                if (idx != REG_A)
                    return op.setError(idxAt, REGISTER_NOT_ALLOWED);
                if (paren && !p.skipSpaces().expect(')'))
                    return op.setError(p, MISSING_CLOSING_PAREN);
                scan = p;
                op.mode = paren ? M_BASE : M_HLPA;  // (HL+A), HL+A
                return OK;
            }
        }
        if (isRegIndex(reg)) {
            p = dispAt;
            op.val = parseInteger(p, op, paren);
            if (op.hasError())
                return op.getError();
            if (paren && !p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            scan = p;
            op.mode = paren ? M_IDX : M_IXPD;  // (ix+d), ix+d
            return OK;
        }
        return op.setError(regAt, REGISTER_NOT_ALLOWED);
    }
    op.mode = reg2mode(reg);
    scan = p;
    return OK;
}

Error AsmTlcs90::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
        return _insn.setError(insn.dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
            return _insn.setError(insn.srcOp);
        scan.skipSpaces();
    }

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.dstOp, insn);

    const auto pre = insn.pre();
    const auto dst = insn.dst();
    const auto src = insn.src();
    const auto prefix = insn.prefix();
    if (prefix) {
        insn.setEmitInsn();
        if (pre == M_DST)
            encodeOperand(insn, dst, insn.dstOp, prefix);
        if (pre == M_SRC)
            encodeOperand(insn, src, insn.srcOp, prefix);
        if (pre == M_NONE)
            insn.emitByte(prefix);
    }
    const auto opc = insn.opCode();
    insn.setEmitInsn();
    if (pre != M_DST)
        encodeOperand(insn, dst, insn.dstOp, opc);
    if (pre != M_SRC)
        encodeOperand(insn, src, insn.srcOp, opc);
    insn.emitInsn(opc);
    return _insn.setError(insn);
}

}  // namespace tlcs90
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
