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

#include "asm_i8051.h"

#include "table_i8051.h"
#include "text_common.h"

namespace libasm {
namespace i8051 {

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

const ValueParser::Plugins &AsmI8051::defaultPlugins() {
    return ValueParser::Plugins::intel();
}

AsmI8051::AsmI8051(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

Error AsmI8051::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#')) {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_IMM16;
        scan = p;
        return OK;
    }

    const auto indir = p.expect('@');
    if (indir && isspace(*p))
        return op.setError(UNKNOWN_OPERAND);

    const auto regp = p;
    op.reg = parseRegName(p);
    if (op.reg != REG_UNDEF) {
        if (indir && op.reg == REG_A && p.expect('+')) {
            const auto base = parseRegName(p);
            if (base == REG_DPTR || base == REG_PC) {
                op.mode = (base == REG_DPTR) ? M_INDXD : M_INDXP;
                scan = p;
                return OK;
            }
            return op.setError(UNKNOWN_OPERAND);
        }
        scan = p;
        if (indir) {
            if (op.reg == REG_R0 || op.reg == REG_R1) {
                op.mode = M_IDIRR;
                return OK;
            }
            if (op.reg == REG_DPTR) {
                op.mode = M_IDIRD;
                return OK;
            }
            return op.setError(UNKNOWN_OPERAND);
        }
        if (isRReg(op.reg)) {
            op.mode = M_RREG;
            return OK;
        }
        switch (op.reg) {
        case REG_A:
            op.mode = M_AREG;
            return OK;
        case REG_C:
            op.mode = M_CREG;
            return OK;
        case REG_DPTR:
            op.mode = M_DREG;
            return OK;
        case REG_AB:
            op.mode = M_ABREG;
            return OK;
        default:
            return op.setError(regp, UNKNOWN_OPERAND);
        }
    }
    if (indir)
        return op.setError(UNKNOWN_OPERAND);

    const auto addrp = p;
    const auto bitNot = p.expect('/');
    op.val = parseInteger(p.skipSpaces(), op, '.');
    if (op.hasError())
        return op.getError();
    if (p.expect('.')) {
        if (op.getError())
            op.val.setUnsigned(0x20);
        const auto bitp = p;
        ErrorAt bitOp;
        auto bitNo = parseInteger(p, bitOp);
        if (bitOp.getError())
            op.setErrorIf(bitOp);
        if (bitNo.overflow(7))
            return op.setError(bitp, ILLEGAL_BIT_NUMBER);
        auto bitAddr = op.val.getUnsigned();
        if ((bitAddr & ~0x0F) == 0x20 || (bitAddr & ~0x78) == 0x80) {
            op.mode = bitNot ? M_NOTAD : M_BITAD;
            if ((bitAddr & 0x80) == 0)
                bitAddr = (bitAddr & 0xF) << 3;
            op.val.setUnsigned(bitAddr | bitNo.getUnsigned());
            scan = p;
            return OK;
        }
        return op.setError(addrp, NOT_BIT_ADDRESSABLE);
    }
    op.mode = bitNot ? M_NOTAD : M_ADR16;
    scan = p;
    return OK;
}

void AsmI8051::encodeRelative(AsmInsn &insn, const Operand &op) const {
    const auto len = insn.length();
    const auto base = insn.address() + (len ? len : 1) + 1;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    const auto delta = branchDelta(base, target, insn, op);
    if (overflowDelta(delta, 8))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand8(delta);
}

namespace {

constexpr Config::uintptr_t block(Config::uintptr_t addr) {
    return addr & ~0x7FF;
}
constexpr Config::uintptr_t page(Config::uintptr_t addr) {
    return addr & 0x7FF;
}

constexpr Config::opcode_t AJMP = 0x01;
constexpr Config::opcode_t ACALL = 0x11;
constexpr Config::opcode_t LJMP = 0x02;
constexpr Config::opcode_t LCALL = 0x12;

bool maySmartBranch(const AsmInsn &insn) {
    const auto opc = insn.opCode();
    return opc == AJMP || opc == ACALL || opc == LJMP || opc == LCALL;
}

void shortJump(AsmInsn &insn) {
    const auto opc = insn.opCode();
    if (opc == LJMP)
        insn.setOpCode(AJMP);
    if (opc == LCALL)
        insn.setOpCode(ACALL);
}

void longJump(AsmInsn &insn) {
    const auto opc = insn.opCode();
    if (opc == AJMP)
        insn.setOpCode(LJMP);
    if (opc == ACALL)
        insn.setOpCode(LCALL);
}

}  // namespace

void AsmI8051::encodeJump(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    const auto smartBranch = _smartBranch && maySmartBranch(insn);
    const auto base = insn.address() + 2;
    const auto target = op.getError() ? block(base) : op.val.getUnsigned();
    if (mode == M_ADR11 && !smartBranch) {
        if (block(base) != block(target))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
    short_jump:
        insn.embed((page(target) >> (8 - 5)) & 0xE0);
        insn.emitOperand8(target);
        return;
    }
    if (mode == M_ADR16 && !smartBranch) {
    long_jump:
        insn.emitOperand16(op.val.getUnsigned());
        return;
    }
    if (op.getError() || block(base) != block(target)) {
        longJump(insn);
        goto long_jump;
    }
    shortJump(insn);
    goto short_jump;
}

void AsmI8051::encodeOperand(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_REL:
        encodeRelative(insn, op);
        break;
    case M_RREG:
    case M_IDIRR:
        insn.embed(encodeRRegName(op.reg));
        break;
    case M_IMM8:
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_ADR11:
    case M_ADR16:
        encodeJump(insn, mode, op);
        break;
    case M_IMM16:
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_ADR8:
    case M_BITAD:
    case M_NOTAD:
        if (op.val.overflow(UINT8_MAX))
            insn.setErrorIf(op, mode == M_ADR8 ? OVERFLOW_RANGE : NOT_BIT_ADDRESSABLE);
        if (TABLE.invalidDirect(insn.opCode(), op.val.getUnsigned()))
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        insn.emitOperand8(op.val.getUnsigned());
        break;
    default:
        break;
    }
}

Error AsmI8051::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
        return _insn.setError(insn.dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
            return _insn.setError(insn.srcOp);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, insn.extOp) && insn.extOp.hasError())
            return _insn.setError(insn.extOp);
        scan.skipSpaces();
    }

    if (_insn.setErrorIf(insn.dstOp, TABLE.searchName(cpuType(), insn)))
        return _insn.getError();

    const auto dst = insn.dst();
    const auto src = insn.src();
    if (dst == M_ADR8 && src == M_ADR8) {
        insn.setErrorIf(insn.dstOp);
        encodeOperand(insn, src, insn.srcOp);
        encodeOperand(insn, dst, insn.dstOp);
    } else {
        encodeOperand(insn, dst, insn.dstOp);
        encodeOperand(insn, src, insn.srcOp);
        encodeOperand(insn, insn.ext(), insn.extOp);
    }
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace i8051
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
