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

#include "asm_mc68hc16.h"
#include "table_mc68hc16.h"
#include "text_common.h"

namespace libasm {
namespace mc68hc16 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_FCB, &Assembler::defineDataConstant, Assembler::DATA_BYTE_NO_STRING},
    {TEXT_FCC, &Assembler::defineString},
    {TEXT_FDB, &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING},
    {TEXT_RMB, &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmMc68HC16::defaultPlugins() {
    return ValueParser::Plugins::motorola();
}

AsmMc68HC16::AsmMc68HC16(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

Config::ptrdiff_t AsmMc68HC16::calculateDisplacement(AsmInsn &insn, const Operand &op) const {
    const auto base = insn.address() + PC_OFFSET;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    return branchDelta(base, target, insn, op);
}

namespace {
bool isBsr(const AsmInsn &insn) {
    const auto prefix = insn.prefix();
    if (prefix == 0)
        return insn.opCode() == 0x36;                // BSR
    return prefix == 0x27 && insn.opCode() == 0xF9;  // LBSR
}

bool isBcc(const AsmInsn &insn) {
    const auto opc = insn.opCode();
    const auto bcc = opc & 0xF0;
    const auto brn = (opc & 0xF) == 1;
    const auto prefix = insn.prefix();
    if (prefix == 0)
        return bcc == 0xB0 && !brn;
    return prefix == 0x37 && bcc == 0x80 && !brn;
}

bool isBrxxx(const AsmInsn &insn) {
    if (insn.prefix() == 0 && insn.op1.size == 0) {
        const auto opc = insn.opCode();
        const auto brxxx = opc & 0xB0;
        return (opc & 0xF) == 0xB && (brxxx >= 0x80 && brxxx <= 0xA0);
    }
    return false;
}

void shortBranch(AsmInsn &insn) {
    if (isBcc(insn)) {
        constexpr uint8_t Bcc = 0xB0;
        const auto cc = insn.opCode() & 0xF;
        insn.setPrefix(0);
        insn.setOpCode(Bcc | cc);
    }
    if (isBsr(insn)) {
        insn.setPrefix(0x00);
        insn.setOpCode(0x36);
        return;
    }
}

void longBranch(AsmInsn &insn) {
    if (isBcc(insn)) {
        constexpr uint8_t LBcc = 0x80;
        const auto cc = insn.opCode() & 0xF;
        insn.setPrefix(0x37);
        insn.setOpCode(LBcc | cc);
        return;
    }
    if (isBsr(insn)) {
        insn.setPrefix(0x27);
        insn.setOpCode(0xF9);
        return;
    }
    if (isBrxxx(insn)) {
        // BRxxx  ix8,x, #im8, r8:  opc im8 ix8  r8
        // BRxxx ix16,x, #im8, r16: opc im8 ix16 ix16 r16 r16
        const auto im8 = insn.bytes()[1];
        const auto ix8 = insn.bytes()[2];
        const auto brxxx = insn.opCode();
        const auto opc = (brxxx >= 0xC0 ? 0x0A : 0x0B) | (brxxx & 0x30);
        insn.resetBytes();
        insn.setOpCode(opc);
        insn.emitOperand8(im8);
        insn.emitOperand16(ix8);
        return;
    }
}
}  // namespace

void AsmMc68HC16::encodeRelative(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    const auto smartBranch = _smartBranch && (isBcc(insn) || isBsr(insn) || isBrxxx(insn));
    if (mode == M_RL8 && !smartBranch) {
    short_branch:
        const auto delta = calculateDisplacement(insn, op);
        const auto target = insn.address() + PC_OFFSET + delta;
        insn.setErrorIf(op, checkAddr(target, true));
        if (overflowDelta(delta, 8))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
        return;
    }
    if (mode == M_RL16 && !smartBranch) {
    long_branch:
        const auto delta = calculateDisplacement(insn, op);
        const auto target = insn.address() + PC_OFFSET + delta;
        insn.setErrorIf(op, checkAddr(target, true));
        if (overflowDelta(delta, 16))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand16(delta);
        return;
    }
    const auto delta = calculateDisplacement(insn, op);
    if (op.getError() || overflowDelta(delta, 8)) {
        longBranch(insn);
        goto long_branch;
    }
    shortBranch(insn);
    goto short_branch;
}

uint_fast8_t registerMask(RegName reg, bool push) {
    static constexpr RegName MASK[] PROGMEM = {
            REG_D,    // PSHM push D first.
            REG_E,    //
            REG_X,    //
            REG_Y,    //
            REG_Z,    //
            REG_K,    //
            REG_CCR,  // PULM pull D first
    };
    for (uint_fast8_t i = 0; i < 7; ++i) {
        if (RegName(pgm_read_byte(MASK + i)) == reg) {
            return push ? (1 << i) : (0x40 >> i);
        }
    }
    return 0;
}

void AsmMc68HC16::encodeRegisterList(AsmInsn &insn, const Operand &op) const {
    const auto push = (insn.opCode() == 0x34);
    uint_fast8_t post = 0;
    auto list = op.list;
    while (true) {
        const auto at = list.skipSpaces();
        const auto reg = parseRegName(list, parser());
        const auto mask = registerMask(reg, push);
        if (mask == 0)
            insn.setErrorIf(at, REGISTER_NOT_ALLOWED);
        if (post & mask)
            insn.setErrorIf(at, DUPLICATE_REGISTER);
        post |= mask;
        auto end = list;
        if (list.skipSpaces().expect(','))
            continue;
        if (endOfLine(end))
            break;
    }
    insn.emitOperand8(post);
}

void AsmMc68HC16::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_RL8:
    case M_RL16:
        encodeRelative(insn, op, mode);
        break;
    case M_IXX8:
        if (op.base != REG_X) {
            insn.setErrorIf(op.baseAt, REGISTER_NOT_ALLOWED);
            break;
        }
        // Fall-through
    case M_IX8:
        insn.emitOperand8(op.val.getUnsigned());
        // Fall-through
    case M_IXE:
        if (insn.hasList()) {
            encodeRegisterList(insn, op);
        } else {
            insn.embed(encodeIndexNum(op.base));
        }
        break;
    case M_IX16:
        if (op.val.overflowInt16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val.getUnsigned());
        insn.embed(encodeIndexNum(op.base));
        break;
    case M_IX20:
        if (op.val.overflow(0x7FFFF, -0x80000))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8((op.val.getUnsigned() >> 16) & 0xF);
        insn.emitOperand16(op.val.getUnsigned());
        insn.embed(encodeIndexNum(op.base));
        break;
    case M_IM8:
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_IM16:
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_EX16:
        if (op.val.overflow(UINT16_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_EX20:
        if (op.val.overflow(0xFFFFF))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8((op.val.getUnsigned() >> 16) & 0xF);
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_IM4H:
        if (op.val.overflow(7, -8))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.opr = (op.val.getUnsigned() & 0xF) << 4;
        break;
    case M_IM4L:
        if (op.val.overflow(7, -8))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.opr |= (op.val.getUnsigned() & 0xF);
        insn.emitOperand8(insn.opr);
        break;
    case M_LIST:
        // An operand may be E,ix,...
        if (insn.mode1() != M_IXE)
            encodeRegisterList(insn, op);
        break;
    default:
        break;
    }
}

Error AsmMc68HC16::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#')) {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = !op.val.isUndefined() && op.val.overflowUint8() ? M_IM16 : M_IM8;
        scan = p;
        return OK;
    }

    op.list = p.skipSpaces();
    const auto index = parseRegName(p, parser());
    if (index == REG_E) {
        const auto save = p.skipSpaces();
        if (p.expect(',')) {
            op.baseAt = p.skipSpaces();
            op.base = parseRegName(p, parser());
            if (isIndexReg(op.base)) {
                op.mode = M_IXE;
                scan = p;
                return OK;
            }
        }
        p = save;
    }
    if (index != REG_UNDEF) {
        while (p.skipSpaces().expect(',')) {
            if (parseRegName(p.skipSpaces(), parser()) == REG_UNDEF)
                return op.setErrorIf(p, UNKNOWN_OPERAND);
        }
        if (endOfLine(p)) {
            op.mode = M_LIST;
            scan = p;
            return OK;
        }
        return op.setErrorIf(p, UNKNOWN_OPERAND);
    }

    if (p.expect('>')) {
        op.size = 16;  // >nn
    } else if (p.expect('<')) {
        op.size = 8;  // <nn
    }
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    const auto save = p.skipSpaces();
    if (p.expect(',')) {
        op.baseAt = p.skipSpaces();
        op.base = parseRegName(p, parser());
        if (op.base != REG_UNDEF) {
            if (!isIndexReg(op.base))
                return op.setErrorIf(op.baseAt, REGISTER_NOT_ALLOWED);
            auto sz = op.size;
            if (sz == 0)
                sz = !op.val.isUndefined() && op.val.overflow(UINT8_MAX) ? 16 : 8;
            op.mode = (sz == 8) ? M_IX8 : M_IX16;
            scan = p;
            return OK;
        }
        p = save;
    }
    op.mode = M_EX16;
    scan = p;
    return OK;
}

Error AsmMc68HC16::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (hasOperand(cpuType(), insn)) {
        if (parseOperand(scan, insn.op1) && insn.op1.hasError())
            return _insn.setError(insn.op1);
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.op2) && insn.op2.hasError())
                return _insn.setError(insn.op2);
        }
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.op3) && insn.op3.hasError())
                return _insn.setError(insn.op3);
        }
    }
    scan.skipSpaces();

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.op1, insn);

    if (insn.mode2() == M_IM8 || insn.mode2() == M_IXX8) {
        // Special operand order for BCLR/BSET/BRCLR/BRSET/MOVB/MOVW
        encodeOperand(insn, insn.op2, insn.mode2());
        encodeOperand(insn, insn.op1, insn.mode1());
    } else {
        encodeOperand(insn, insn.op1, insn.mode1());
        encodeOperand(insn, insn.op2, insn.mode2());
    }
    encodeOperand(insn, insn.op3, insn.mode3());
    insn.emitInsn();
    return _insn.setError(insn);
}

void AsmInsn::emitInsn() {
    uint_fast8_t pos = 0;
    const auto pre = prefix();
    if (pre > UINT8_MAX)
        emitByte((pre >> 8), pos++);
    if (pre)
        emitByte(pre, pos++);
    emitByte(opCode(), pos++);
    if (hasPost())
        emitByte(post(), pos);
}

uint_fast8_t AsmInsn::operandPos() const {
    uint_fast8_t pos = length();
    if (pos == 0) {
        const auto pre = prefix();
        if (pre > UINT8_MAX)
            pos++;
        if (pre)
            pos++;
        pos++;
    }
    return pos;
};

}  // namespace mc68hc16
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
