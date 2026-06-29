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
    : Assembler(plugins, PSEUDO_TABLE, &_opt_maxMode),
      Config(TABLE),
      _opt_maxMode(this, &Config::setMaxMode, OPT_BOOL_MAXIMUM_MODE, OPT_DESC_MAXIMUM_MODE) {
    reset();
}

void AsmTlcs900::reset() {
    Assembler::reset();
    // Reset register mode to the CPU's natural default (handled by setCpuType).
    setCpuType(cpuType());
}

Error AsmTlcs900::processPseudo(StrScanner &scan, Insn &insn) {
    const auto *p = PSEUDOS_TLCS900.search(insn.name());
    return p ? p->invoke(this, scan, insn) : Assembler::processPseudo(scan, insn);
}

Error AsmTlcs900::processMaxMode(StrScanner &scan, Insn &insn, uint16_t) {
    auto p = scan.skipSpaces();
    const auto at = p;
    bool on;
    if (p.expectTrue()) {
        on = true;
    } else if (p.expectFalse()) {
        on = false;
    } else {
        return insn.setError(at, UNKNOWN_OPERAND);
    }
    scan = p;
    if (setMaxMode(on) != OK)
        return insn.setError(at, OPERAND_NOT_ALLOWED);
    return OK;
}

const ValueParser::Plugins &AsmTlcs900::defaultPlugins() {
    return ValueParser::Plugins::intel();
}

// Parse a 32-bit memory base register into its register code: current bank
// XWA..XSP, previous bank XWA'..XHL', or bank-N XWAn..XHLn. The abs-bank
// parser handles the suffixed forms; plain current-bank names come from the
// register table. Advances |p| and returns true on success.
static bool parseMemBaseReg(StrScanner &p, const ValueParser &parser, uint8_t &code) {
    auto q = p;
    uint8_t size;
    if (reg::parseAbsRegCode(q, parser, code, size)) {
        if (size != 2 || !reg::isMemBaseCode(code))
            return false;
        p = q;
        return true;
    }
    auto r = p;
    const auto reg = reg::parseRegName(r, parser);
    if (reg != REG_UNDEF && reg::isReg32(reg)) {
        code = reg::reg32BaseCode(reg);
        p = r;
        return true;
    }
    return false;
}

// 0x83 (block transfer) and 0xB0 (conditional return) are reserved prefix
// bytes, so the single-byte register-indirect forms that would land on them
// -- (XHL) byte-source and (XWA) destination -- must use the complex (r32)
// encoding instead.
static bool isReservedPrefixByte(uint8_t b) {
    return b == 0x83 || b == 0xB0;
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

// Single-byte register-indirect prefix byte for a current-bank base register,
// or 0 when the single-byte form is unavailable (non-current-bank base, or a
// reserved prefix byte) and the complex (r32) form must be used instead.
static uint8_t memIndSingleByte(PrefixMode pm, uint8_t base) {
    if (!reg::isCurrentBankBaseCode(base))
        return 0;
    const uint8_t b = memPmSimpleBase(pm) | reg::memBaseIndex(base);
    return isReservedPrefixByte(b) ? 0 : b;
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
    const bool dstIsPre = (d == M_SRC) || (d == M_DST && pm == PM_MEMD) || d == M_ABSDST ||
                          d == M_R32SRC || d == M_SRCB || d == M_SRCW || d == M_SRCWL ||
                          d == M_SRCBW || d == M_MULDSTP;
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
    case M_IMM8:
        return 1;
    case M_IND:
        // single byte (current bank) or C3 + r32' sub-byte (complex)
        return memIndSingleByte(pm, op.base) ? 1 : 2;
    case M_ABREG8:
    case M_ABREG16:
    case M_ABREG32:
    case M_ABS8:
        return 2;
    case M_IDX8:
        // single byte + d8 (current bank) or C3 + r32' sub-byte + d16 (complex)
        return reg::isCurrentBankBaseCode(op.base) ? 2 : 4;
    case M_ABS16:
        return 3;
    case M_IDX16:
    case M_IDXR:
    case M_ABS24:
        return 4;
    case M_PRDC:
    case M_PINC:
        // C4/C5 + r32' sub-byte: the two-byte form is the only encoding.
        return 2;
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
    if (insn.dst() == M_MULDSTP) {
        // MUL/DIV rr,#: the dest is carried in the prefix, doubled -- a 16-bit
        // pair (C8 + 2*idx+1) for a byte op or a 32-bit register (D8 + idx) for
        // a word op.
        insn.emitByte(pm == PM_REG8 ? (0xC8 | (2 * encodeReg16(op.reg) + 1))
                                    : (0xD8 | encodeReg32(op.reg)));
        return;
    }
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
        if (const uint8_t single = memIndSingleByte(pm, op.base)) {
            // single-byte register-indirect (XWA)..(XSP): 0x80-0x87
            insn.emitByte(single);
        } else {
            // complex (r32): C3 + r32' sub-byte (low 2 bits 0)
            insn.emitByte(memPmComplexBase(pm) | 0x03);
            insn.emitByte(op.base);
        }
        break;
    case M_IDX8: {
        const auto disp = static_cast<int8_t>(op.val.getSigned());
        if (reg::isCurrentBankBaseCode(op.base)) {
            // single-byte indexed (Xrr+d8): 0x88-0x8F + d8
            insn.emitByte(memPmSimpleBase(pm) | 0x08 | reg::memBaseIndex(op.base));
            insn.emitByte(static_cast<uint8_t>(disp));
        } else {
            // no single-byte +d8 form for non-current-bank base: use (r32+d16)
            insn.emitByte(memPmComplexBase(pm) | 0x03);
            insn.emitByte(op.base | 0x01);
            insn.emitUint16(static_cast<uint16_t>(disp));
        }
        break;
    }
    case M_IDX16:
        insn.emitByte(memPmComplexBase(pm) | 0x03);
        insn.emitByte(op.base | 0x01);
        insn.emitUint16(static_cast<uint16_t>(op.val.getSigned()));
        break;
    case M_IDXR:
        insn.emitByte(memPmComplexBase(pm) | 0x03);
        insn.emitByte(0x03);
        insn.emitByte(op.base);
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
    case M_PRDC:
        // (-r32): C4 + r32' sub-byte with the increment-size (zz) field.
        insn.emitByte(memPmComplexBase(pm) | 0x04);
        insn.emitByte(op.base | opdSizeCode(insn, pm));
        break;
    case M_PINC:
        // (r32+): C5 + r32' sub-byte with the increment-size (zz) field.
        insn.emitByte(memPmComplexBase(pm) | 0x05);
        insn.emitByte(op.base | opdSizeCode(insn, pm));
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
    case M_SRCB:
    case M_SRCW:
    case M_SRCWL:
    case M_SRCBW:
    case M_CCT:      // matcher only: opcode already carries the unconditional form
    case M_MULDSTP:  // emitted as the prefix byte by encodePrefixAddr
        break;
    case M_DST:
        if (pm != PM_MEMD)
            insn.embed(encodePrefixSizedReg(op, pm));
        break;
    case M_MULDST: {
        // dest in opcode low bits, double the source(prefix) size: a 16-bit
        // register for a byte-op (R=2*idx+1) or a 32-bit one for a word-op
        // (R=idx). The source may be a register, absolute-bank reg, or memory.
        const bool byteOp = pm == PM_REG8 || pm == PM_ABREG8 || pm == PM_MEM8;
        insn.embed(byteOp ? (2 * encodeReg16(op.reg) + 1) : encodeReg32(op.reg));
        break;
    }
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
        // Control sub-byte 0x3C is the normal stack pointer (NSP/XNSP) on every
        // variant except /L, where it is the interrupt-nesting counter INTNEST.
        const bool isL = (cpuType() == TLCS900L);
        // Structural errors (CPU mismatch, size mismatch) drop any bytes
        // already emitted for this instruction so the failure is reported
        // with no encoded output.
        if ((creg == REG_NSP || creg == REG_XNSP) && isL) {
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
            insn.resetBytes();
            break;
        }
        if (creg == REG_INTNEST && !isL) {
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
        const Config::uintptr_t base = insn.address() + 4;
        const Config::uintptr_t target = op.getError() ? base : op.val.getUnsigned();
        const Config::ptrdiff_t delta = branchDelta(base, target, insn, op);
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
        const Config::uintptr_t base = insn.address() + insn.operandPos() + 1;
        const Config::uintptr_t target = op.getError() ? base : op.val.getUnsigned();
        const Config::ptrdiff_t delta = branchDelta(base, target, insn, op);
        if (overflowDelta(delta, 8))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(static_cast<uint8_t>(delta));
        break;
    }
    case M_REL16: {
        const Config::uintptr_t base = insn.address() + insn.operandPos() + 2;
        const Config::uintptr_t target = op.getError() ? base : op.val.getUnsigned();
        const Config::ptrdiff_t delta = branchDelta(base, target, insn, op);
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
    case M_BUF: {
        const auto step = 1u << (insn.opCode() & 3);
        const auto bufSize = op.val.getUnsigned();
        // Buffer size must be 2^n (manual: n = (opc&3)+1..15), i.e. a power
        // of two with 2*step <= bufSize <= 0x8000.
        if ((bufSize & (bufSize - 1)) != 0 || bufSize < step * 2 || bufSize > 0x8000)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(static_cast<uint16_t>(bufSize - step));
        break;
    }
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
            // XWA/XBC/XDE/XHL cannot be used in minimum mode.
            if (!maxMode() && reg::isReg32General(reg))
                op.setError(regAt, REGISTER_NOT_ALLOWED);
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
    // Check for pre-decrement: (-r32)
    if (p.expect('-')) {
        const auto idxAt = p.skipSpaces();
        uint8_t code;
        if (!parseMemBaseReg(p, parser(), code))
            return op.setError(idxAt, REGISTER_NOT_ALLOWED);
        // Only XIX/XIY/XIZ/XSP are usable as a base register in minimum mode.
        if (!maxMode() && !reg::isIndexBaseCode(code))
            return op.setError(idxAt, REGISTER_NOT_ALLOWED);
        if (!p.skipSpaces().expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        op.base = code;
        op.mode = M_PRDC;
        scan = p;
        return OK;
    }

    // Try to parse a 32-bit memory base register
    const auto innerAt = p;
    uint8_t baseCode;
    if (parseMemBaseReg(p, parser(), baseCode)) {
        // Only XIX/XIY/XIZ/XSP are usable as a base register in minimum mode.
        if (!maxMode() && !reg::isIndexBaseCode(baseCode))
            return op.setError(innerAt, REGISTER_NOT_ALLOWED);
        p.skipSpaces();
        const auto signAt = p;
        if (p.expect('+')) {
            // (r32+) post-increment
            if (p.expect(')')) {
                op.base = baseCode;
                op.mode = M_PINC;
                scan = p;
                return OK;
            }
            // (r32+r8) register-indexed
            const auto idxAt = p.skipSpaces();
            const auto idxReg = parseRegName(p, parser());
            if (idxReg != REG_UNDEF) {
                if (!isReg8(idxReg))
                    return op.setError(idxAt, REGISTER_NOT_ALLOWED);
                if (!p.skipSpaces().expect(')'))
                    return op.setError(p, MISSING_CLOSING_PAREN);
                op.base = baseCode;
                op.idx = idxReg;
                op.mode = M_IDXR;
                scan = p;
                return OK;
            }
            // (r32+disp) falls through to the signed-displacement path
            // below; parseInteger handles '+' after we backtrack to signAt.
        } else if (p.expect(')')) {
            // (r32) indirect
            op.base = baseCode;
            op.mode = M_IND;
            scan = p;
            return OK;
        }
        // Signed displacement: (r32+disp) or (r32-disp). parseInteger accepts
        // either leading sign starting from signAt.
        p = signAt;
        op.val = parseInteger(p, op, ')');
        if (op.hasError())
            return op.getError();
        if (!p.skipSpaces().expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        op.base = baseCode;
        op.mode = op.val.overflowInt8() ? M_IDX16 : M_IDX8;
        scan = p;
        return OK;
    }
    // A register name that isn't a valid 32-bit base is rejected here rather
    // than mis-parsed as an absolute symbol.
    {
        auto probe = innerAt;
        if (parseRegName(probe, parser()) != REG_UNDEF)
            return op.setError(innerAt, REGISTER_NOT_ALLOWED);
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
