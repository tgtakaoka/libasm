/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "asm_tlcs900.h"
#include "reg_tlcs900.h"
#include "table_tlcs900.h"
#include "text_common.h"
#include "text_tlcs900.h"

namespace libasm {
namespace tlcs900 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::tlcs900;

namespace {

constexpr char OPT_BOOL_COMPLEX_INDIR[] PROGMEM = "complex-indir";
constexpr char OPT_DESC_COMPLEX_INDIR[] PROGMEM =
        "use complex encoding for (-xrr) and (xrr+) addressing";

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_DB,      &Assembler::defineDataConstant, Assembler::DATA_BYTE },
    { TEXT_DL,      &Assembler::defineDataConstant, Assembler::DATA_LONG },
    { TEXT_DS,      &Assembler::allocateSpaces,     Assembler::DATA_BYTE },
    { TEXT_DW,      &Assembler::defineDataConstant, Assembler::DATA_WORD },
};
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};
// clang-format on

}  // namespace

constexpr AsmTlcs900::PseudoTlcs900 AsmTlcs900::PSEUDO_TLCS900_TABLE[] PROGMEM = {
        {TEXT_MAXMODE, &AsmTlcs900::processMaxMode},
};
PROGMEM constexpr AsmTlcs900::PseudosTlcs900 AsmTlcs900::PSEUDOS_TLCS900{
        ARRAY_RANGE(PSEUDO_TLCS900_TABLE)};

AsmTlcs900::AsmTlcs900(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_complexIndir),
      Config(TABLE),
      _opt_complexIndir(
              this, &AsmTlcs900::setComplexIndir, OPT_BOOL_COMPLEX_INDIR, OPT_DESC_COMPLEX_INDIR) {
    reset();
}

void AsmTlcs900::reset() {
    Assembler::reset();
    setComplexIndir(false);
    // Reset register mode to the CPU's natural default (handled by setCpuType).
    setCpuType(cpuType());
}

Error AsmTlcs900::setComplexIndir(bool enable) {
    _complexIndir = enable;
    return OK;
}

Error AsmTlcs900::processPseudo(StrScanner &scan, Insn &insn) {
    const auto *p = PSEUDOS_TLCS900.search(insn.name());
    return p ? p->invoke(this, scan, insn) : Assembler::processPseudo(scan, insn);
}

Error AsmTlcs900::processMaxMode(StrScanner &scan, Insn &insn, uint16_t) {
    auto p = scan.skipSpaces();
    StrScanner arg = p;
    while (arg.size() && !isspace(*arg) && *arg != ',')
        ++arg;
    const auto len = arg.str() - p.str();
    bool on;
    if (len == 2 && (p[0] == 'O' || p[0] == 'o') && (p[1] == 'N' || p[1] == 'n')) {
        on = true;
    } else if (len == 3 && (p[0] == 'O' || p[0] == 'o') && (p[1] == 'F' || p[1] == 'f') &&
               (p[2] == 'F' || p[2] == 'f')) {
        on = false;
    } else {
        return insn.setError(p, UNKNOWN_OPERAND);
    }
    scan = arg;
    if (setMaxMode(on) != OK)
        return insn.setError(p, OPERAND_NOT_ALLOWED);
    return OK;
}

const ValueParser::Plugins &AsmTlcs900::defaultPlugins() {
    return ValueParser::Plugins::intel();
}

static uint8_t memPmSimpleBase(PrefixMode pm) {
    switch (pm) {
    case PM_MEM8:
        return 0x80;
    case PM_MEM16:
        return 0x90;
    case PM_MEM32:
        return 0xA0;
    case PM_MEMD:
        return 0xB0;
    default:
        return 0x80;
    }
}

static uint8_t memPmComplexBase(PrefixMode pm) {
    return memPmSimpleBase(pm) | 0x40;
}

static uint8_t pmSizeCode(PrefixMode pm) {
    switch (pm) {
    case PM_MEM8:
        return 0;
    case PM_MEM16:
        return 1;
    default:
        return 2;
    }
}

// In PM_MEMD context, size comes from the source operand, not the prefix mode.
static uint8_t opdSizeCode(const AsmInsn &insn, PrefixMode pm) {
    if (pm != PM_MEMD)
        return pmSizeCode(pm);
    switch (insn.src()) {
    case M_REG8:
    case M_IMM8:
        return 0;
    case M_REG16:
    case M_IMM16:
        return 1;
    default:
        return 2;
    }
}

// Selects which user operand drives the operand-dependent prefix slot for
// the current entry. The mode in the entry's dst position tells us whether
// the prefix is keyed off dstOp or off srcOp.
static const Operand &prefixOperand(const AsmInsn &insn) {
    const auto pm = insn.pre();
    const auto d = insn.dst();
    const bool dstIsPre =
            (d == M_SRC) || (d == M_DST && pm == PM_MEMD) || d == M_ABSDST || d == M_R32SRC;
    return dstIsPre ? insn.dstOp : insn.srcOp;
}

static uint8_t encodeIdxRegSfr(RegName reg) {
    switch (reg) {
    case REG_A:
        return 0xE0;
    case REG_W:
        return 0xE1;
    case REG_C:
        return 0xE4;
    case REG_B:
        return 0xE5;
    case REG_E:
        return 0xE8;
    case REG_D:
        return 0xE9;
    case REG_L:
        return 0xEC;
    case REG_H:
        return 0xED;
    default:
        return 0;
    }
}

// Bytes the prefix area occupies before the opcode, derived from PrefixMode,
// prefix() and the prefix operand's AddrMode (not from current emit length).
uint_fast8_t AsmInsn::prefixSize() const {
    const auto pm = pre();
    const auto p = prefix();
    if (p) {
        const uint_fast8_t base = p > UINT8_MAX ? 2 : 1;
        // PM_LDAR layout: F3 13 rel16 opc -- rel16 sits in the prefix area.
        return pm == PM_LDAR ? base + 2 : base;
    }
    if (pm == PM_NONE)
        return 0;
    const auto &op = prefixOperand(*this);
    switch (op.mode) {
    case R_C:
    case M_REG8:
    case M_REG16:
    case M_REG32:
    case M_IND:
    case M_IMM8:
        return 1;
    case M_ABREG8:
    case M_ABREG16:
    case M_ABREG32:
    case M_IDX8:
    case M_ABS8:
        return 2;
    case M_ABS16:
        return 3;
    case M_IDX16:
    case M_IDXR:
    case M_ABS24:
        return 4;
    case M_PRDC: {
        const uint8_t compact = memPmSimpleBase(pm) | encodeIndReg(op.reg);
        return (!complexIndir && compact != 0x83 && compact != 0xB0) ? 1 : 2;
    }
    case M_PINC:
        return complexIndir ? 2 : 1;
    default:
        return 0;
    }
}

uint_fast8_t AsmInsn::operandPos() const {
    const uint_fast8_t after = prefixSize() + 1;
    return length() < after ? after : length();
}

void AsmInsn::emitInsn() {
    // Skip the back-patch when encodeOperand reset bytes for a structural
    // error: length drops below prefixSize() and emitting the opcode alone
    // would leave garbage with no prefix.
    if (length() >= prefixSize())
        emitByte(opCode(), prefixSize());
}

// Emit the prefix bytes that precede the opcode position. For PM_LDAR, also
// emit the src rel16 here (it sits between the prefix and the opcode).
void AsmTlcs900::encodePrefixAddr(AsmInsn &insn) const {
    const auto pm = insn.pre();
    const auto prefix = insn.prefix();
    if (prefix) {
        if (prefix > UINT8_MAX)
            insn.emitByte(static_cast<uint8_t>(prefix >> 8));
        insn.emitByte(static_cast<uint8_t>(prefix));
        if (pm == PM_LDAR)
            encodeOperand(insn, insn.srcOp, insn.src());
        return;
    }
    if (pm == PM_NONE)
        return;
    const Operand &op = prefixOperand(insn);
    insn.setErrorIf(op);
    switch (op.mode) {
    case R_C:
    case M_REG8:
        insn.emitByte(0xC8 | encodeReg8(op.reg));
        break;
    case M_REG16:
        insn.emitByte(0xD8 | encodeReg16(op.reg));
        break;
    case M_REG32:
        if (pm == PM_REG16) {
            // MULA xrr32 alias: 32-bit register expressed as PM_REG16 prefix.
            // encodeReg32(XWA..XHL) returns 0..3, matching encodeReg16(WA..HL).
            insn.emitByte(0xD8 | encodeReg32(op.reg));
        } else {
            insn.emitByte(0xE8 | encodeReg32(op.reg));
        }
        break;
    case M_ABREG8:
        insn.emitByte(0xC7);
        insn.emitByte(static_cast<uint8_t>(op.val.getUnsigned()));
        break;
    case M_ABREG16:
        insn.emitByte(0xD7);
        insn.emitByte(static_cast<uint8_t>(op.val.getUnsigned()));
        break;
    case M_ABREG32:
        insn.emitByte(0xE7);
        insn.emitByte(static_cast<uint8_t>(op.val.getUnsigned()));
        break;
    case M_IND:
        insn.emitByte(memPmSimpleBase(pm) | 0x04 | encodeIndReg(op.reg));
        break;
    case M_IDX8: {
        insn.emitByte(memPmSimpleBase(pm) | 0x0C | encodeIndReg(op.reg));
        const auto disp = static_cast<int8_t>(op.val.getSigned());
        insn.emitByte(static_cast<uint8_t>(disp));
        break;
    }
    case M_IDX16:
        insn.emitByte(memPmComplexBase(pm) | 0x03);
        insn.emitByte(0xF0 | (encodeIndReg(op.reg) << 2) | 0x01);
        insn.emitUint16(static_cast<uint16_t>(op.val.getSigned()));
        break;
    case M_IDXR:
        insn.emitByte(memPmComplexBase(pm) | 0x03);
        insn.emitByte(0x03);
        insn.emitByte(0xF0 | (encodeIndReg(op.reg) << 2));
        insn.emitByte(encodeIdxRegSfr(op.idx));
        break;
    case M_ABS8:
        insn.emitByte(memPmComplexBase(pm) | 0x00);
        insn.emitByte(static_cast<uint8_t>(op.val.getUnsigned()));
        break;
    case M_ABS16:
        insn.emitByte(memPmComplexBase(pm) | 0x01);
        insn.emitUint16(static_cast<uint16_t>(op.val.getUnsigned()));
        break;
    case M_ABS24:
        insn.emitByte(memPmComplexBase(pm) | 0x02);
        insn.emitUint24(op.val.getUnsigned() & 0xFFFFFF);
        break;
    case M_PRDC: {
        const uint8_t compact = memPmSimpleBase(pm) | encodeIndReg(op.reg);
        if (!_complexIndir && compact != 0x83 && compact != 0xB0) {
            insn.emitByte(compact);
        } else {
            insn.emitByte(memPmComplexBase(pm) | 0x04);
            insn.emitByte(0xF0 | (encodeIndReg(op.reg) << 2) | opdSizeCode(insn, pm));
        }
        break;
    }
    case M_PINC:
        if (!_complexIndir) {
            insn.emitByte(memPmSimpleBase(pm) | 0x08 | encodeIndReg(op.reg));
        } else {
            insn.emitByte(memPmComplexBase(pm) | 0x05);
            insn.emitByte(0xF0 | (encodeIndReg(op.reg) << 2) | opdSizeCode(insn, pm));
        }
        break;
    case M_IMM8:
        // MUL/MULS WA,#n8: register-context prefix with A (0xC9) as placeholder
        if (pm == PM_REG8) {
            insn.emitByte(0xC9);
            break;
        }
        insn.setErrorIf(op, UNKNOWN_OPERAND);
        break;
    default:
        insn.setErrorIf(op, UNKNOWN_OPERAND);
        break;
    }
}

// Map prefix mode to the register-size encoder for opcode-bit insertion.
static uint8_t encodePrefixSizedReg(const Operand &op, PrefixMode pm) {
    switch (pm) {
    case PM_REG8:
    case PM_MEM8:
    case PM_ABREG8:
        return encodeReg8(op.reg);
    case PM_REG16:
    case PM_MEM16:
    case PM_ABREG16:
        return encodeReg16(op.reg);
    case PM_REG32:
    case PM_MEM32:
    case PM_ABREG32:
        return encodeReg32(op.reg);
    default:
        return 0;
    }
}

// Generic per-AddrMode operand encoder: OR-embeds opcode low bits via
// insn.embed() and/or emits trailing bytes via emitOperand* at operandPos().
// Called once per operand from encodeImpl after setEmitInsn() has reserved
// the opcode slot; emitInsn() back-patches the final opcode at the end.
void AsmTlcs900::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    insn.setErrorIf(op);
    const auto pm = insn.pre();
    switch (mode) {
    case M_NONE:
    case M_SRC:
    case M_ABSDST:
    case M_STEP1:
    case M_STEP2:
    case M_STEP4:
    case R_A:
    case R_WA:
    case R_SR:
    case R_F:
    case R_FP:
    case R_C:
    case M_R32SRC:
        break;
    case M_DST:
        if (pm != PM_MEMD)
            insn.embed(encodePrefixSizedReg(op, pm));
        break;
    case M_REG8:
        insn.embed(encodeReg8(op.reg));
        break;
    case M_REG16:
        insn.embed(encodeReg16(op.reg));
        break;
    case M_REG32:
        insn.embed(encodeReg32(op.reg));
        break;
    case M_CC:
        insn.embed(encodeCcName(op.cc));
        break;
    case M_SWI:
        insn.embed(op.val.getUnsigned() & 7);
        break;
    case M_ZERO:
        insn.emitOperand8(0);
        break;
    case M_DISUF:
        insn.emitOperand8(0x07);
        break;
    case M_CREG: {
        const auto creg = op.reg;
        const bool isBase = (cpuType() == TLCS900);
        // Structural errors (CPU mismatch, size mismatch) drop any bytes
        // already emitted for this instruction so the failure is reported
        // with no encoded output.
        if ((creg == REG_NSP || creg == REG_XNSP) && !isBase) {
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
            insn.resetBytes();
            break;
        }
        if (creg == REG_INTNEST && isBase) {
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
            insn.resetBytes();
            break;
        }
        const Size expected = pm == PM_REG8 ? SZ_BYTE : pm == PM_REG16 ? SZ_WORD : SZ_QUAD;
        if (ctrlRegSize(creg) != expected) {
            // Attribute size mismatch to the gen-reg operand (the one whose
            // size dictates PM via the prefix), matching the test expectation.
            const Operand &genOp = (&op == &insn.dstOp) ? insn.srcOp : insn.dstOp;
            insn.setErrorIf(genOp, ILLEGAL_SIZE);
            insn.resetBytes();
            break;
        }
        insn.emitOperand8(ctrlRegCode(creg));
        break;
    }
    case M_LDARREG:
        if (op.mode == M_REG16)
            insn.emitOperand8(0x20 | encodeReg16(op.reg));
        else if (op.mode == M_REG32)
            insn.emitOperand8(0x30 | encodeReg32(op.reg));
        else
            insn.setErrorIf(op, UNKNOWN_OPERAND);
        break;
    case M_LDARREL: {
        // PM_LDAR's rel16 sits BETWEEN the prefix bytes and the opcode, so
        // it's emitted by appending while encodePrefixAddr is still running;
        // operandPos() (past the opcode slot) would be the wrong target.
        const auto base = static_cast<uint32_t>(insn.address() + 4);
        const auto target = op.getError() ? base : op.val.getUnsigned();
        const auto delta = branchDelta(base, target, insn, op);
        if (overflowDelta(delta, 16))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitUint16(static_cast<uint16_t>(delta));
        break;
    }
    case M_IMM8:
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_IMM16:
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_IMM32:
        insn.emitOperand32(op.val.getUnsigned());
        break;
    case M_IMMX:
        switch (pm) {
        case PM_REG8:
        case PM_MEM8:
        case PM_ABREG8:
            if (op.val.overflowUint8())
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand8(op.val.getUnsigned());
            break;
        case PM_REG16:
        case PM_MEM16:
        case PM_ABREG16:
            if (op.val.overflowUint16())
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand16(op.val.getUnsigned());
            break;
        case PM_REG32:
        case PM_MEM32:
        case PM_ABREG32:
            insn.emitOperand32(op.val.getUnsigned());
            break;
        default:
            insn.emitOperand8(op.val.getUnsigned());
            break;
        }
        break;
    case M_BIT:
        if (pm == PM_MEMD) {
            insn.embed(op.val.getUnsigned() & 7);
        } else {
            if (op.val.overflow(7))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand8(op.val.getUnsigned() & 7);
        }
        break;
    case M_REL8: {
        const auto base = static_cast<uint32_t>(insn.address() + insn.operandPos() + 1);
        const auto target = op.getError() ? base : op.val.getUnsigned();
        const auto delta = branchDelta(base, target, insn, op);
        if (overflowDelta(delta, 8))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(static_cast<uint8_t>(delta));
        break;
    }
    case M_REL16: {
        const auto base = static_cast<uint32_t>(insn.address() + insn.operandPos() + 2);
        const auto target = op.getError() ? base : op.val.getUnsigned();
        const auto delta = branchDelta(base, target, insn, op);
        if (overflowDelta(delta, 16))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand16(static_cast<uint16_t>(delta));
        break;
    }
    case M_ABS8:
        insn.emitOperand8(static_cast<uint8_t>(op.val.getUnsigned()));
        break;
    case M_ABS16:
        insn.emitOperand16(static_cast<uint16_t>(op.val.getUnsigned()));
        break;
    case M_ABS24:
        insn.emitOperand24(op.val.getUnsigned() & 0xFFFFFF);
        break;
    case M_LDF: {
        // RFP range depends on register mode: MIN = 0-7 (3 bits), MAX = 0-3 (2 bits).
        const uint8_t max = maxMode() ? 3 : 7;
        if (op.val.overflow(max))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val.getUnsigned() & max);
        break;
    }
    case M_INTLVL:
        // EI n: interrupt level 0-7.
        if (op.val.overflow(7))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val.getUnsigned() & 7);
        break;
    case M_RCOUNT:
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_LDXDST:
        insn.emitOperand8(0x00);
        insn.emitOperand16(static_cast<uint16_t>(op.val.getUnsigned()));
        break;
    default:
        break;
    }
}

Error AsmTlcs900::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    // Try absolute bank register first (A0, QBC3, A', IXL, etc.)
    uint8_t abCode, abSize;
    if (parseAbsRegCode(p, parser(), abCode, abSize)) {
        op.val.setUnsigned(abCode);
        op.mode = abSize == 0 ? M_ABREG8 : abSize == 1 ? M_ABREG16 : M_ABREG32;
        scan = p;
        return OK;
    }

    // Try LDC control register (NSP/XNSP/DMA*) before general registers.
    const auto creg = parseCtrlRegName(p, parser());
    if (creg != REG_UNDEF) {
        op.reg = creg;
        op.mode = M_CREG;
        scan = p;
        return OK;
    }

    // Try condition code first; 'C' is dual-use (REG_C or CC_C), use R_C for it.
    // F followed by apostrophe is the alternate flag register F' (EX F,F').
    const auto cc = parseCcName(p, parser());
    if (cc != CC_UNDEF) {
        op.cc = cc;
        if (cc == CC_C) {
            op.mode = R_C;
            op.reg = REG_C;
        } else if (cc == CC_F && p.expect('\'')) {
            op.mode = R_FP;
        } else {
            op.mode = M_CC;
        }
        scan = p;
        return OK;
    }

    // Try register name
    const auto regAt = p;
    const auto reg = parseRegName(p, parser());
    if (reg != REG_UNDEF) {
        op.reg = reg;
        if (isReg8(reg)) {
            op.mode = M_REG8;
        } else if (isReg16(reg)) {
            op.mode = M_REG16;
        } else if (isReg32(reg)) {
            op.mode = M_REG32;
        } else if (reg == REG_SR) {
            op.mode = R_SR;
        } else {
            op.setError(regAt, REGISTER_NOT_ALLOWED);
        }
        scan = p;
        return OK;
    }

    // immediate or jump traget
    if (!p.expect('(')) {
        // Immediate value
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        if (op.val.overflowUint8()) {
            op.mode = op.val.overflowUint16() ? M_IMM32 : M_IMM16;
        } else {
            op.mode = M_IMM8;
        }
        scan = p;
        return OK;
    }

    p.skipSpaces();
    // Check for pre-decrement: (-XIx)
    if (p.expect('-')) {
        const auto idxAt = p.skipSpaces();
        const auto idxReg = parseRegName(p, parser());
        if (!isIndexReg(idxReg))
            return op.setError(idxAt, REGISTER_NOT_ALLOWED);
        if (!p.skipSpaces().expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        op.reg = idxReg;
        op.mode = M_PRDC;
        scan = p;
        return OK;
    }

    // Try to parse a register
    const auto innerAt = p;
    const auto innerReg = parseRegName(p, parser());
    if (innerReg != REG_UNDEF) {
        if (!isIndexReg(innerReg))
            return op.setError(innerAt, REGISTER_NOT_ALLOWED);
        p.skipSpaces();
        const auto signAt = p;
        if (p.expect('+')) {
            // (XIx+) post-increment
            if (p.expect(')')) {
                op.reg = innerReg;
                op.mode = M_PINC;
                scan = p;
                return OK;
            }
            // (XIx+r8) register-indexed
            const auto idxAt = p.skipSpaces();
            const auto idxReg = parseRegName(p, parser());
            if (idxReg != REG_UNDEF) {
                if (!isReg8(idxReg))
                    return op.setError(idxAt, REGISTER_NOT_ALLOWED);
                if (!p.skipSpaces().expect(')'))
                    return op.setError(p, MISSING_CLOSING_PAREN);
                op.reg = innerReg;
                op.idx = idxReg;
                op.mode = M_IDXR;
                scan = p;
                return OK;
            }
            // (XIx+disp) falls through to the signed-displacement path
            // below; parseInteger handles '+' after we backtrack to signAt.
        } else if (p.expect(')')) {
            // (XIx) indirect
            op.reg = innerReg;
            op.mode = M_IND;
            scan = p;
            return OK;
        }
        // Signed displacement: (XIx+disp) or (XIx-disp). parseInteger accepts
        // either leading sign starting from signAt.
        p = signAt;
        op.val = parseInteger(p, op, ')');
        if (op.hasError())
            return op.getError();
        if (!p.skipSpaces().expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        op.reg = innerReg;
        op.mode = op.val.overflowInt8() ? M_IDX16 : M_IDX8;
        scan = p;
        return OK;
    }

    // Absolute address
    op.val = parseInteger(p, op, ')');
    if (op.hasError())
        return op.getError();
    if (!p.skipSpaces().expect(')'))
        return op.setError(p, MISSING_CLOSING_PAREN);
    if (!op.val.overflow(UINT8_MAX))
        op.mode = M_ABS8;
    else if (!op.val.overflow(UINT16_MAX))
        op.mode = M_ABS16;
    else
        op.mode = M_ABS24;
    scan = p;
    return OK;
}

Error AsmTlcs900::encodeImpl(StrScanner &scan, Insn &_insn) const {
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

    insn.complexIndir = _complexIndir;
    encodePrefixAddr(insn);
    encodeOperand(insn, insn.dstOp, insn.dst());
    if (insn.pre() != PM_LDAR)
        encodeOperand(insn, insn.srcOp, insn.src());
    insn.emitInsn();

    return _insn.setError(insn);
}

}  // namespace tlcs900
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
