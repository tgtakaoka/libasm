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

#include "asm_h8500.h"
#include "reg_h8500.h"
#include "table_h8500.h"
#include "text_common.h"
#include "text_h8500.h"

using namespace libasm::text::common;
using namespace libasm::text::h8500;

namespace libasm {
namespace h8500 {

using namespace pseudo;
using namespace reg;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_FCB, &Assembler::defineDataConstant, Assembler::DATA_BYTE_NO_STRING },
    { TEXT_FDB, &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING },
    { TEXT_RMB, &Assembler::allocateSpaces,     Assembler::DATA_BYTE           },
};
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};
// clang-format on

// Return size qualifier parsed after current scanner position (advances scan on match).
static uint8_t parseSizeQual(StrScanner &scan) {
    auto p = scan;
    if (!p.expect(':')) return 0;
    if (p.expect('8')) { scan = p; return 8; }
    if (p.expect('1')) {
        if (p.expect('6')) { scan = p; return 16; }
        return 0;
    }
    if (p.expect('2')) {
        if (p.expect('4')) { scan = p; return 24; }
        return 0;
    }
    return 0;
}


// Build the EA byte for the given addressing mode, base register, and Sz bit.
static uint8_t makeEaByte(AddrMode mode, RegName reg, EaSz sz) {
    const uint8_t szBit = static_cast<uint8_t>(sz) << 3;
    switch (mode) {
    case M_REG:     return 0xA0 | szBit | encodeReg(reg);
    case M_PREDEC:  return 0xB0 | szBit | encodeReg(reg);
    case M_POSTINC: return 0xC0 | szBit | encodeReg(reg);
    case M_IND:     return 0xD0 | szBit | encodeReg(reg);
    case M_DISP8:
    case M_DISP8F:  return 0xE0 | szBit | encodeReg(reg);
    case M_DISP16:  return 0xF0 | szBit | encodeReg(reg);
    case M_IMM8:    return 0x04;
    case M_IMM16:   return 0x0C;
    case M_ABS8:    return 0x05 | szBit;
    case M_ABS16:   return 0x15 | szBit;
    default:        return 0;
    }
}

// Emit the EA extension byte(s) following the EA byte.
static void emitEaExt(AsmInsn &insn, const Operand &op) {
    const auto v = op.val.getUnsigned();
    switch (op.mode) {
    case M_DISP8:
    case M_DISP8F:
        insn.setErrorIf(op, op.val.overflowInt8() ? OVERFLOW_RANGE : OK);
        insn.emitByte(static_cast<uint8_t>(v));
        break;
    case M_DISP16:
        insn.setErrorIf(op, op.val.overflowInt16() ? OVERFLOW_RANGE : OK);
        insn.emitUint16Be(static_cast<uint16_t>(v));
        break;
    case M_IMM8:
    case M_ABS8:
        insn.setErrorIf(op, op.val.overflowUint8() ? OVERFLOW_RANGE : OK);
        insn.emitByte(static_cast<uint8_t>(v));
        break;
    case M_IMM16:
    case M_ABS16:
        insn.emitUint16Be(static_cast<uint16_t>(v));
        break;
    default:
        break;
    }
}

}  // namespace

const ValueParser::Plugins &AsmH8500::defaultPlugins() {
    return ValueParser::Plugins::motorola();
}

void AsmH8500::emitPcRel8(AsmInsn &insn, const Operand &op) const {
    const auto base = insn.address() + insn.length() + 1;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    const auto delta = branchDelta(base, target, insn, op);
    if (overflowDelta(delta, 8))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitByte(static_cast<uint8_t>(delta));
}

void AsmH8500::emitPcRel16(AsmInsn &insn, const Operand &op) const {
    const auto base = insn.address() + insn.length() + 2;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    const auto delta = branchDelta(base, target, insn, op);
    if (overflowDelta(delta, 16))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitUint16Be(static_cast<uint16_t>(delta));
}

AsmH8500::AsmH8500(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

Error AsmH8500::parseOperand(StrScanner &scan, Operand &op) const {
    op.setAt(scan.skipSpaces());
    auto p = scan;

    if (p.expect('@')) {
        p.skipSpaces();

        // @-Rn pre-decrement
        if (p.expect('-')) {
            const auto reg = parseRegName(p.skipSpaces(), parser());
            if (reg != REG_UNDEF) {
                op.mode = M_PREDEC;
                op.reg = reg;
                scan = p;
                return OK;
            }
            return op.setError(UNKNOWN_REGISTER);
        }

        // @(d,Rn)
        if (p.expect('(')) {
            const auto disp = parseInteger(p.skipSpaces(), op);
            if (op.hasError()) return op.getError();
            if (!p.skipSpaces().expect(',')) return op.setError(MISSING_COMMA);
            const auto reg = parseRegName(p.skipSpaces(), parser());
            if (reg == REG_UNDEF) return op.setError(UNKNOWN_REGISTER);
            if (!p.skipSpaces().expect(')')) return op.setError(MISSING_CLOSING_PAREN);
            op.val = disp;
            op.reg = reg;
            const auto d = disp.getSigned();
            if (reg == REG_FP && d >= -128 && d <= 127) {
                op.mode = M_DISP8F;
            } else if (d >= -128 && d <= 127) {
                op.mode = M_DISP8;
            } else {
                op.mode = M_DISP16;
            }
            scan = p;
            return OK;
        }

        // @Rn or @Rn+
        {
            auto q = p;
            const auto reg = parseRegName(q, parser());
            if (reg != REG_UNDEF) {
                if (q.skipSpaces().expect('+')) {
                    op.mode = M_POSTINC;
                } else {
                    op.mode = M_IND;
                }
                op.reg = reg;
                scan = q;
                return OK;
            }
        }

        // @addr with optional :8/:16/:24 qualifier
        {
            const auto addr = parseInteger(p, op);
            if (op.hasError()) return op.getError();
            op.val = addr;
            const auto sz = parseSizeQual(p);
            if (sz == 8) {
                op.mode = M_ABS8;
            } else if (sz == 24) {
                op.mode = M_ABS24;
            } else {
                const auto v = addr.getUnsigned();
                op.mode = (sz == 16 || v <= 0xFFFF) ? M_ABS16 : M_ABS24;
            }
            scan = p;
            return OK;
        }
    }

    if (p.expect('#')) {
        const auto val = parseInteger(p.skipSpaces(), op);
        if (op.hasError()) return op.getError();
        op.val = val;
        const auto sz = parseSizeQual(p);
        if (sz == 8) {
            op.mode = M_IMM8;
        } else if (sz == 16) {
            op.mode = M_IMM16;
        } else {
            op.mode = val.overflowUint8() ? M_IMM16 : M_IMM8;
        }
        scan = p;
        return OK;
    }

    // {Rn,...} register list for LDM/STM
    if (p.expect('{')) {
        uint8_t mask = 0;
        do {
            p.skipSpaces();
            const auto reg = parseRegName(p, parser());
            if (reg == REG_UNDEF) return op.setError(UNKNOWN_REGISTER);
            mask |= static_cast<uint8_t>(1 << static_cast<int>(reg));
        } while (p.skipSpaces().expect(','));
        if (!p.skipSpaces().expect('}')) return op.setError(MISSING_CLOSING_BRACKET);
        op.mode = M_REGLIST;
        op.val.setUnsigned(mask);
        scan = p;
        return OK;
    }

    // Control register (CCR, SR, BR, EP, DP, TP)
    {
        auto q = p;
        const auto cr = parseCrName(q, parser());
        if (cr != CR_UNDEF) {
            op.mode = M_CR;
            op.creg = cr;
            scan = q;
            return OK;
        }
    }

    // General register Rn (or FP/SP)
    {
        auto q = p;
        const auto reg = parseRegName(q, parser());
        if (reg != REG_UNDEF) {
            op.mode = M_REG;
            op.reg = reg;
            scan = q;
            return OK;
        }
    }

    // Bare address — branch target (assume short form)
    {
        const auto addr = parseInteger(p, op);
        if (op.hasError()) return op.getError();
        op.val = addr;
        op.mode = M_PCREL8;
        scan = p;
        return OK;
    }
}

// Encode GEN (Format A) or EXT (Format C) instruction.
Error AsmH8500::encodeGenFmt(AsmInsn &insn) const {
    const auto flags = insn.flags();
    const auto esrc = flags.src();
    const auto edst = flags.dst();
    const auto eaSz = flags.eaSz();
    const auto fmt = flags.fmt();

    const Operand *eaOp = nullptr;
    uint8_t regField = 0;

    if (esrc == M_IMM8 && edst == M_CR) {
        // ANDC/ORC/XORC: EA=imm8 (op1), CR (op2)
        eaOp = &insn.op1;
        const auto cr = insn.op2.creg;
        if (cr == CR_UNDEF) return insn.setError(UNKNOWN_REGISTER);
        regField = encodeCr(cr);
    } else if (esrc == M_BIT && edst == M_EA) {
        // BSET/BCLR/BNOT/BTST with bit# (op1) and EA (op2)
        eaOp = &insn.op2;
        regField = insn.op1.val.getUnsigned() & 0x0F;
    } else if (esrc == M_CR && edst == M_EA) {
        // STC: CR (op1) → EA (op2)
        eaOp = &insn.op2;
        const auto cr = insn.op1.creg;
        if (cr == CR_UNDEF) return insn.setError(UNKNOWN_REGISTER);
        regField = encodeCr(cr);
    } else if (esrc == M_REG && edst == M_EA) {
        // MOV:G Rs,EA / XCH Rs,EA
        eaOp = &insn.op2;
        regField = encodeReg(insn.op1.reg);
    } else if (esrc == M_EA && edst == M_NONE) {
        // Unary: CLR, NEG, NOT, EXTS, EXTU, SWAP, TAS, TST, ROTL/R, SHAL/R, SHLL/R, ADD:Q
        eaOp = &insn.op1;
    } else if (esrc == M_EA && edst == M_REG) {
        // Binary: ADD:G, SUB, MOV:G, ADDX, SUBX, MULXU, DIVXU, CMP:G, AND, OR, XOR, DADD, DSUB
        eaOp = &insn.op1;
        regField = encodeReg(insn.op2.reg);
    } else if (esrc == M_EA && edst == M_CR) {
        // LDC
        eaOp = &insn.op1;
        const auto cr = insn.op2.creg;
        if (cr == CR_UNDEF) return insn.setError(UNKNOWN_REGISTER);
        regField = encodeCr(cr);
    } else {
        return insn.setError(OPERAND_NOT_ALLOWED);
    }

    insn.setErrorIf(*eaOp);
    insn.emitByte(makeEaByte(eaOp->mode, eaOp->reg, eaSz));
    emitEaExt(insn, *eaOp);
    if (fmt == FMT_EXT)
        insn.emitByte(0x00);
    insn.emitByte(insn.opCode() | regField);
    return insn.getError();
}

// Encode SPC (Format B) instruction.
Error AsmH8500::encodeSpcFmt(AsmInsn &insn) const {
    const auto flags = insn.flags();
    const auto esrc = flags.src();
    const auto edst = flags.dst();
    const auto base = insn.opCode();

    // ADD:Q #imm2, Rn — encoded as GEN format (EA=Rn word, OP=opcode-for-const)
    if (esrc == M_IMM2 && edst == M_REG) {
        const auto imm = insn.op1.val.getSigned();
        uint8_t opByte;
        switch (imm) {
        case 1:  opByte = 0x08; break;
        case -1: opByte = 0x09; break;
        case 2:  opByte = 0x0C; break;
        case -2: opByte = 0x0D; break;
        default:
            return insn.setError(OPERAND_NOT_ALLOWED);
        }
        insn.emitByte(0xA8 | encodeReg(insn.op2.reg));  // M_REG EA_WORD
        insn.emitByte(opByte);
        return insn.getError();
    }

    // TRAPA #vec (0-3)
    if (esrc == M_TRAPV) {
        const auto vec = insn.op1.val.getUnsigned();
        if (vec > 3)
            insn.setErrorIf(insn.op1, OVERFLOW_RANGE);
        insn.emitByte(0x08);
        insn.emitByte(0x10 | (vec & 0x03));
        return insn.getError();
    }

    // SCB/EQ|NE|F Rc, rel8
    if (esrc == M_SCB) {
        insn.emitByte(base);
        insn.emitByte(0xB8 | encodeReg(insn.op1.reg));
        emitPcRel8(insn, insn.op2);
        return insn.getError();
    }

    // LDM/STM {reglist}
    if (esrc == M_REGLIST) {
        insn.emitByte(base);
        insn.emitByte(static_cast<uint8_t>(insn.op1.val.getUnsigned()));
        return insn.getError();
    }

    // PJMP/PJSR @aa:24
    if (esrc == M_ABS24) {
        const auto addr = insn.op1.val.getUnsigned();
        insn.emitByte(base);
        insn.emitByte(static_cast<uint8_t>((addr >> 16) & 0xFF));
        insn.emitUint16Be(static_cast<uint16_t>(addr & 0xFFFF));
        return insn.getError();
    }

    // No-operand instructions (NOP, RTE, RTS, SLEEP, TRAP/VS)
    if (esrc == M_NONE && edst == M_NONE) {
        insn.emitByte(base);
        return insn.getError();
    }

    // Determine register bits to embed in opcode byte
    uint8_t regBits = 0;
    if (edst == M_REG) {
        // dst is register, embedded in base opcode
        regBits = encodeReg(insn.op2.reg);
    } else if (esrc == M_REG && (edst == M_ABS8 || edst == M_DISP8F)) {
        // MOV:S Rs,@aa:8 or MOV:F Rs,@(d,FP): src register embedded
        regBits = encodeReg(insn.op1.reg);
    }
    // LINK/UNLK: src=M_REG but FP implied, no embedding (regBits stays 0)

    insn.emitByte(base | regBits);

    // Determine data operand to emit after opcode byte
    const Operand *dataOp;
    AddrMode dataMode;
    if (esrc == M_REG) {
        dataOp = &insn.op2;
        dataMode = edst;
    } else {
        dataOp = &insn.op1;
        dataMode = esrc;
    }

    switch (dataMode) {
    case M_PCREL8:
        emitPcRel8(insn, *dataOp);
        break;
    case M_PCREL16:
        emitPcRel16(insn, *dataOp);
        break;
    case M_IMM8:
    case M_ABS8:
        insn.setErrorIf(*dataOp, dataOp->val.overflowUint8() ? OVERFLOW_RANGE : OK);
        insn.emitByte(static_cast<uint8_t>(dataOp->val.getUnsigned()));
        break;
    case M_IMM16:
    case M_ABS16:
        insn.emitUint16Be(static_cast<uint16_t>(dataOp->val.getUnsigned()));
        break;
    case M_DISP8F:
        insn.setErrorIf(*dataOp, dataOp->val.overflowInt8() ? OVERFLOW_RANGE : OK);
        insn.emitByte(static_cast<uint8_t>(dataOp->val.getUnsigned()));
        break;
    case M_NONE:
        break;
    default:
        return insn.setError(OPERAND_NOT_ALLOWED);
    }

    return insn.getError();
}

Error AsmH8500::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);

    // Parse up to two operands
    if (!endOfLine(scan.skipSpaces())) {
        if (parseOperand(scan, insn.op1) && insn.op1.hasError())
            return _insn.setError(insn.op1);
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.op2) && insn.op2.hasError())
                return _insn.setError(insn.op2);
        }
    }
    scan.skipSpaces();

    // Handle JMP/JSR @Rn or @(d,Rn) — indirect and displacement forms (0x11 prefix)
    if (strcasecmp_P(insn.name(), TEXT_JMP) == 0 ||
            strcasecmp_P(insn.name(), TEXT_JSR) == 0) {
        const auto isJsr = (strcasecmp_P(insn.name(), TEXT_JSR) == 0);
        const auto &op1 = insn.op1;
        if (op1.mode == M_IND) {
            insn.emitByte(0x11);
            insn.emitByte((isJsr ? 0xD8u : 0xD0u) | encodeReg(op1.reg));
            return _insn.setError(insn);
        }
        if (op1.mode == M_DISP8 || op1.mode == M_DISP8F) {
            insn.setErrorIf(op1, op1.val.overflowInt8() ? OVERFLOW_RANGE : OK);
            insn.emitByte(0x11);
            insn.emitByte((isJsr ? 0xE8u : 0xE0u) | encodeReg(op1.reg));
            insn.emitByte(static_cast<uint8_t>(op1.val.getUnsigned()));
            return _insn.setError(insn);
        }
        if (op1.mode == M_DISP16) {
            insn.emitByte(0x11);
            insn.emitByte((isJsr ? 0xF8u : 0xF0u) | encodeReg(op1.reg));
            insn.emitUint16Be(static_cast<uint16_t>(op1.val.getUnsigned()));
            return _insn.setError(insn);
        }
        // Fall through for JMP/JSR @aa:16 (handled by normal table lookup)
    }

    // Handle PJMP/PJSR @Rn (H8/532 indirect form, 0x11 prefix)
    if ((strcasecmp_P(insn.name(), TEXT_PJMP) == 0 ||
                strcasecmp_P(insn.name(), TEXT_PJSR) == 0) &&
            insn.op1.mode == M_IND) {
        if (cpuType() != H8_532)
            return _insn.setError(UNKNOWN_INSTRUCTION);
        const auto isPjsr = (strcasecmp_P(insn.name(), TEXT_PJSR) == 0);
        insn.emitByte(0x11);
        insn.emitByte((isPjsr ? 0xC8u : 0xC0u) | encodeReg(insn.op1.reg));
        return _insn.setError(insn);
    }

    // Handle PRTS (H8/532, 0x11 secondary)
    if (strcasecmp_P(insn.name(), TEXT_PRTS) == 0) {
        if (cpuType() != H8_532)
            return _insn.setError(UNKNOWN_INSTRUCTION);
        insn.emitByte(0x11);
        insn.emitByte(0x19);
        return _insn.setError(insn);
    }

    // Handle PRTD (H8/532, 0x11 secondary)
    if (strcasecmp_P(insn.name(), TEXT_PRTD) == 0) {
        if (cpuType() != H8_532)
            return _insn.setError(UNKNOWN_INSTRUCTION);
        insn.emitByte(0x11);
        if (insn.op1.mode == M_IMM8) {
            insn.emitByte(0x14);
            insn.emitByte(static_cast<uint8_t>(insn.op1.val.getUnsigned()));
        } else if (insn.op1.mode == M_IMM16) {
            insn.emitByte(0x1C);
            insn.emitUint16Be(static_cast<uint16_t>(insn.op1.val.getUnsigned()));
        } else {
            return _insn.setError(OPERAND_NOT_ALLOWED);
        }
        return _insn.setError(insn);
    }

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.op1, insn);

    const auto fmt = insn.fmt();
    if (fmt == FMT_GEN || fmt == FMT_EXT) {
        encodeGenFmt(insn);
    } else {
        encodeSpcFmt(insn);
    }

    return _insn.setError(insn);
}

}  // namespace h8500
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
