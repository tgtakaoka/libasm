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

#include "asm_mc68hc12.h"
#include "table_mc68hc12.h"
#include "text_common.h"

namespace libasm {
namespace mc68hc12 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_FCB, &Assembler::defineDataConstant, Assembler::DATA_BYTE_NO_STRING },
    { TEXT_FCC, &Assembler::defineString       },
    { TEXT_FDB, &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING },
    { TEXT_RMB, &Assembler::allocateSpaces,     Assembler::DATA_BYTE           },
};
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};
// clang-format on

}  // namespace

const ValueParser::Plugins &AsmMc68HC12::defaultPlugins() {
    return ValueParser::Plugins::motorola();
}

AsmMc68HC12::AsmMc68HC12(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

Config::ptrdiff_t AsmMc68HC12::calculateDisplacement(
        AsmInsn &insn, const Operand &op, AddrMode mode) const {
    if (mode == M_RL8) {
        auto len = insn.length();
        if (len == 0) {
            len = 2;  // Bcc
        } else {
            len += 1;  // BRxxx
        }
        const auto base = insn.address() + len;
        const auto target = op.getError() ? base : op.val.getUnsigned();
        return branchDelta(base, target, insn, op);
    }
    if (mode == M_RL9) {
        const auto base = insn.address() + 3;  // DBcc/IBcc/TBcc
        const auto target = op.getError() ? base : op.val.getUnsigned();
        return branchDelta(base, target, insn, op);
    }
    const auto base = insn.address() + 4;  // LBccc
    const auto target = op.getError() ? base : op.val.getUnsigned();
    return branchDelta(base, target, insn, op);
}

namespace {
bool maySmartBranch(const AsmInsn &insn) {
    const auto opc = insn.opCode();
    if ((opc & 0xF0) == 0x20) {
        const auto cc = opc & 0x0F;
        return cc != 1;  // exclude BRN/LBRN
    }
    return false;
}
}  // namespace

void AsmMc68HC12::encodeRelative(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    const auto smartBranch = _smartBranch && maySmartBranch(insn);
    if (mode == M_RL8 && !smartBranch) {
    short_branch:
        const auto delta = calculateDisplacement(insn, op, M_RL8);
        if (overflowDelta(delta, 8))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
        return;
    }
    if (mode == M_RL16 && !smartBranch) {
    long_branch:
        const auto delta = calculateDisplacement(insn, op, M_RL16);
        insn.emitOperand16(delta);
        return;
    }
    if (mode == M_RL9) {
        const auto delta = calculateDisplacement(insn, op, M_RL9);
        if (delta < 0)
            insn.embed(0x10);
        insn.emitOperand8(delta);
        return;
    }
    const auto delta = calculateDisplacement(insn, op, M_RL8);
    if (op.getError() || overflowDelta(delta, 8)) {
        insn.setPrefix(0x18);
        goto long_branch;
    }
    insn.setPrefix(0);
    goto short_branch;
}

uint_fast8_t encodeBaseHigh(AsmInsn &insn, const Operand &op) {
    static constexpr uint8_t BASE_HIGH[] PROGMEM = {
            0xE0,  // REG_X
            0xE8,  // REG_Y
            0xF0,  // REG_SP
            0xF8,  // REG_PC
    };
    if (op.base < REG_X) {
        insn.setErrorIf(op.baseAt, REGISTER_NOT_ALLOWED);
        return 0xE0;
    }
    return pgm_read_byte(BASE_HIGH + op.base - REG_X);
}

void emitPostByte(AsmInsn &insn, Config::opcode_t post, AddrMode mode) {
    if (mode == M_CIDX) {
        insn.setOpCode(post);
    } else {
        insn.emitOperand8(post);
    }
}

void AsmMc68HC12::encodeAccumIndexed(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    uint8_t post = 6;
    switch (op.index) {
    case REG_A:
        if (op.indir)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        post = 4;
        break;
    case REG_B:
        if (op.indir)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        post = 5;
        break;
    case REG_D:
        post = op.indir ? 7 : 6;
        break;
    default:
        insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        break;
    }
    post |= encodeBaseHigh(insn, op);
    emitPostByte(insn, post, mode);
}

uint_fast8_t encodeBaseLow(AsmInsn &insn, const Operand &op) {
    static constexpr uint8_t BASE_LOW[] PROGMEM = {
            0x00,  // REG_X
            0x40,  // REG_Y
            0x80,  // REG_SP
            0xC0,  // REG_PC
    };
    if (op.base < REG_X) {
        insn.setErrorIf(op.baseAt, REGISTER_NOT_ALLOWED);
        return 0x00;
    }
    return pgm_read_byte(BASE_LOW + op.base - REG_X);
}

void AsmMc68HC12::encodePcRelative(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    auto len = insn.length();
    if (len == 0)
        len = 1;  // opcode
    if (insn.hasPrefix())
        len++;  // prefix byte
    if (mode != M_CIDX)
        len++;  // post byte
    const auto mode1 = insn.mode1();
    if (mode1 == M_IM16 || mode1 == M_EXT) {
        len = 7;  // MOVW #,ix/MOVx ext,ix
    } else if (mode1 == M_IM8 || (mode == M_NIDX && op.pos == 2)) {
        len = 5;  // MOVB #,ix/MOVx ix,ix2
    }
    const auto mode2 = insn.mode2();
    if (mode2 == M_IM8 || mode2 == M_PAG) {
        len++;  // BCLR/BSET/CALL
        if (insn.mode3() == M_RL8)
            len++;  // BRCLR/BRSET
    }
    // Try 5-bit offset
    ErrorAt error;
    auto post = encodeBaseLow(insn, op);
    auto base = insn.address() + len;
    auto target = op.getError() ? base : op.val.getUnsigned();
    auto delta = branchDelta(base, target, error, op);
    if (!op.indir && op.size == 0 && !overflowDelta(delta, 5)) {
        post |= delta & 0x1F;
        emitPostByte(insn, post, mode);
        return;
    }
    // Try 9-bit offset
    post = encodeBaseHigh(insn, op);
    if (op.indir)
        post |= 0x01;
    if (mode == M_NIDX || (op.indir && mode == M_DIDX)) {
        insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        emitPostByte(insn, post | 0x02, mode);
        return;
    }
    base = insn.address() + len + 1;
    target = op.getError() ? base : op.val.getUnsigned();
    delta = branchDelta(base, target, error, op);
    if (!op.indir && op.size != 16 && !overflowDelta(delta, 9)) {
        if (delta < 0)
            post |= 0x01;
        emitPostByte(insn, post, mode);
        insn.emitOperand8(delta);
        return;
    }
    // 16-bit offset
    base = insn.address() + len + 2;
    target = op.getError() ? base : op.val.getUnsigned();
    delta = branchDelta(base, target, error, op);
    emitPostByte(insn, post | 0x02, mode);
    insn.emitOperand16(delta);
}

void AsmMc68HC12::encodeIndexed(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    if (op.indir && (mode == M_DIDX || mode == M_NIDX))
        insn.setErrorIf(op, OPERAND_NOT_ALLOWED);

    if (op.mode == M_PAIR || op.index != REG_UNDEF) {
        encodeAccumIndexed(insn, op, mode);
        return;
    }

    // Constant indexed
    if (op.update < 0) {
        if (op.base == REG_PC) {
            encodePcRelative(insn, op, mode);
            return;
        }
        if (op.indir || op.size == 16 || op.val.overflow(0xFF, -0x100)) {
            auto post = encodeBaseHigh(insn, op) | 0x02;
            if (op.indir)
                post |= 0x01;
            if (op.val.overflowUint16())
                insn.setErrorIf(op, OVERFLOW_RANGE);
            emitPostByte(insn, post, mode);
            if (mode == M_NIDX) {
                insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
            } else {
                insn.emitOperand16(op.val.getUnsigned());
            }
        } else if (op.size == 8 || op.val.overflow(15, -16)) {
            auto post = encodeBaseHigh(insn, op);
            if (op.val.isNegative())
                post |= 0x01;
            emitPostByte(insn, post, mode);
            if (mode == M_NIDX) {
                insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
            } else {
                insn.emitOperand8(op.val.getUnsigned());
            }
        } else {
            auto post = encodeBaseLow(insn, op);
            post |= op.val.getUnsigned() & 0x1F;
            emitPostByte(insn, post, mode);
        }
        return;
    }

    // Auto Pre/Post Decrement/Increment Indexed
    auto post = encodeBaseLow(insn, op) | 0x20 | (op.update << 3);
    if (op.base == REG_PC) {
        insn.setErrorIf(op.baseAt, REGISTER_NOT_ALLOWED);
        post = 0;
    }
    auto disp = op.val.getSigned();
    if (disp <= 0 || disp > 8) {
        disp = 1;
        insn.setErrorIf(op, OVERFLOW_RANGE);
    }
    if (op.update == 0 || op.update == 2) {
        post |= (disp - 1);
    } else {
        post |= (16 - disp);
    }
    emitPostByte(insn, post, mode);
}

void AsmMc68HC12::encodeRegisterPair(AsmInsn &insn, const Operand &op) const {
    if (insn.opCode() == 0x04) {  // SEX M_R8H,M_R16L
        if (regSize(op.index) != 8)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        if (regSize(op.base) != 16)
            insn.setErrorIf(op.baseAt, REGISTER_NOT_ALLOWED);
    }
    if (op.index == REG_PC)
        insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
    if (op.base == REG_PC)
        insn.setErrorIf(op.baseAt, REGISTER_NOT_ALLOWED);
    insn.embed(encodeRegNum(op.index) << 4);
    insn.embed(encodeRegNum(op.base));
}

void AsmMc68HC12::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_RL8:
    case M_RL9:
    case M_RL16:
        encodeRelative(insn, op, mode);
        break;
    case M_GN8:
    case M_GN16:
        if (op.mode == M_DIR) {
            insn.embed(0x10);
            insn.emitOperand8(op.val.getUnsigned());
            break;
        }
        if (op.mode == M_IDX || op.mode == M_PAIR) {
            insn.embed(0x20);
            encodeIndexed(insn, op, mode);
            break;
        }
        if (op.mode == M_EXT) {
            insn.embed(0x30);
            insn.emitOperand16(op.val.getUnsigned());
            break;
        }
        if (op.mode == M_IM16) {
            if (mode == M_GN8) {
            imm8:
                insn.emitOperand8(op.val.getUnsigned());
            } else {
            imm16:
                insn.emitOperand16(op.val.getUnsigned());
            }
        }
        break;
    case M_IM8:
        goto imm8;
    case M_IM16:
        goto imm16;
    case M_PAG:
        goto imm8;
    case M_GMEM:
        if (op.mode == M_IDX || op.mode == M_PAIR) {
            insn.embed(0x60);
            encodeIndexed(insn, op, mode);
            break;
        }
        if (op.mode == M_EXT) {
            insn.embed(0x70);
            insn.emitOperand16(op.val.getUnsigned());
            break;
        }
        // Fall-through
    case M_DIR:
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_EXT:
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_IDX:
    case M_DIDX:
    case M_NIDX:
    case M_CIDX:
        encodeIndexed(insn, op, mode);
        break;
    case M_R8L: {
        auto opc = encodeRegNum(op.index);
        if (opc >= 2) {
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
            opc = 0;
        }
        insn.embed(opc);
        break;
    }
    case M_R16L: {
        auto opc = encodeRegNum(op.index);
        if (opc < 4) {
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
            opc = 0;
        }
        insn.embed(opc);
        break;
    }
    case M_PAIR:
        encodeRegisterPair(insn, op);
        break;
    case M_TRAP:
        if (op.val.overflowUint8()) {
            insn.setErrorIf(op, OVERFLOW_RANGE);
        } else {
            const auto opc = op.val.getUnsigned();
            if ((opc >= 0x30 && opc < 0x3A) || opc >= 0x40) {
                insn.setOpCode(op.val.getUnsigned());
            } else {
                insn.setErrorIf(op, OVERFLOW_RANGE);
            }
        }
        break;
    default:
        break;
    }
}

Error AsmMc68HC12::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#')) {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_IM16;
        scan = p;
        return OK;
    }

    if ((op.indir = p.expect('[')))
        p.skipSpaces();
    op.index = parseRegName(p, parser());
    if (op.index != REG_UNDEF) {
        auto a = p.skipSpaces();
        if (!p.expect(','))
            return op.setErrorIf(p, UNKNOWN_OPERAND);
        op.baseAt = p.skipSpaces();
        op.base = parseRegName(p, parser());
        if (op.base == REG_UNDEF) {
            op.mode = (op.index == REG_A || op.index == REG_B) ? M_R8L : M_R16L;
            scan = a;
            return OK;
        }
        if (op.indir && !p.skipSpaces().expect(']'))
            return op.setErrorIf(p, MISSING_CLOSING_BRACKET);
        op.mode = op.indir ? M_IDX : M_PAIR;
        scan = p;
        return OK;
    }
    if (p.expect('>')) {
        op.size = 16;  // >nn
    } else if (p.expect('<')) {
        op.size = 8;  // <nn
    }
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    auto a = p.skipSpaces();
    if (p.expect(',')) {
        op.baseAt = p.skipSpaces();
        if (p.expect('+')) {
            op.update = 0;  // +X
        } else if (p.expect('-')) {
            op.update = 1;  // -X
        }
        op.base = parseRegName(p, parser());
        if (op.base < REG_X) {
        absolute:
            if (op.indir)
                return op.setErrorIf(scan, UNKNOWN_OPERAND);
            if (op.size == 0)
                op.size = op.val.isUndefined() || op.val.overflowUint8() ? 16 : 8;
            op.mode = (op.size == 8) ? M_DIR : M_EXT;
            scan = a;
            return OK;
        }

        auto u = p;
        if (p.expect('+')) {
            if (op.update >= 0)
                return op.setErrorIf(u, UNKNOWN_OPERAND);
            op.update = 2;  // X+
        } else if (p.expect('-')) {
            if (op.update >= 0)
                return op.setErrorIf(u, UNKNOWN_OPERAND);
            op.update = 3;  // X-
        }
        if (op.indir && !p.skipSpaces().expect(']'))
            return op.setErrorIf(p, MISSING_CLOSING_BRACKET);
        op.mode = M_IDX;
        scan = p;
        return OK;
    }
    goto absolute;
}

Error AsmMc68HC12::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (hasOperand(cpuType(), insn)) {
        insn.op1.pos = 1;
        if (parseOperand(scan, insn.op1) && insn.op1.hasError())
            return _insn.setError(insn.op1);
        if (scan.skipSpaces().expect(',')) {
            insn.op2.pos = 2;
            if (parseOperand(scan, insn.op2) && insn.op2.hasError())
                return _insn.setError(insn.op2);
        }
        if (scan.skipSpaces().expect(',')) {
            insn.op3.pos = 3;
            if (parseOperand(scan, insn.op3) && insn.op3.hasError())
                return _insn.setError(insn.op3);
        }
    }
    scan.skipSpaces();

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.op1, insn);

    if (insn.mode1() != M_NIDX && insn.mode2() == M_NIDX) {
        // Special index post byte order for MOVW/MOVB
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

}  // namespace mc68hc12
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
