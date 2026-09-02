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

#include "asm_i960.h"

#include "reg_i960.h"
#include "table_i960.h"
#include "text_i960.h"

#include "parsers.h"

namespace libasm {
namespace i960 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::i960;

namespace {

// The i960 Processor Assembler User's Guide spells every directive with a
// leading dot, and .equ and .set take the symbol name as their first operand
// rather than as a line label; the driver's I960Directive registers those two.
constexpr char TEXT_dDOUBLE[]   PROGMEM = ".double";
constexpr char TEXT_dEXTENDED[] PROGMEM = ".extended";
constexpr char TEXT_dFPU[]      PROGMEM = ".fpu";
constexpr char TEXT_dFLOAT[]    PROGMEM = ".float";
constexpr char TEXT_dHWORD[]    PROGMEM = ".hword";
constexpr char TEXT_dINT[]      PROGMEM = ".int";
constexpr char TEXT_dIPOPERAND[] PROGMEM = ".ipoperand";
constexpr char TEXT_dMODEBITS[] PROGMEM = ".modebits";
constexpr char TEXT_dSHORT[]    PROGMEM = ".short";
constexpr char TEXT_dSINGLE[]   PROGMEM = ".single";
constexpr char TEXT_dSPACE[]    PROGMEM = ".space";

// .short and .hword are half-word aligned, and .int, .long, .word, .float,
// .single and .double word aligned; see the Guide, page 5-5.
constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_dASCII,  &Assembler::defineDataConstant, Assembler::DATA_BYTE    },
    { TEXT_dBYTE,   &Assembler::defineDataConstant, Assembler::DATA_BYTE    },
    { TEXT_dDOUBLE, &Assembler::defineDataConstant, Assembler::DATA_FLOAT64|Assembler::DATA_ALIGN2 },
    { TEXT_dFLOAT,  &Assembler::defineDataConstant, Assembler::DATA_FLOAT32|Assembler::DATA_ALIGN2 },
    { TEXT_dHWORD,  &Assembler::defineDataConstant, Assembler::DATA_WORD|Assembler::DATA_ALIGN2    },
    { TEXT_dINT,    &Assembler::defineDataConstant, Assembler::DATA_LONG|Assembler::DATA_ALIGN2    },
    { TEXT_dLONG,   &Assembler::defineDataConstant, Assembler::DATA_LONG|Assembler::DATA_ALIGN2    },
    { TEXT_dSHORT,  &Assembler::defineDataConstant, Assembler::DATA_WORD|Assembler::DATA_ALIGN2    },
    { TEXT_dSINGLE, &Assembler::defineDataConstant, Assembler::DATA_FLOAT32|Assembler::DATA_ALIGN2 },
    { TEXT_dSPACE,  &Assembler::allocateSpaces,     Assembler::DATA_BYTE    },
    { TEXT_dWORD,   &Assembler::defineDataConstant, Assembler::DATA_LONG|Assembler::DATA_ALIGN2    },
};

PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const AsmI960::PseudoI960 AsmI960::PSEUDO_I960_TABLE[] PROGMEM = {
        {TEXT_dEXTENDED, &AsmI960::defineExtended},
        {TEXT_dFPU, &AsmI960::processFpu},
        {TEXT_dIPOPERAND, &AsmI960::processIpOperand},
        {TEXT_dMODEBITS, &AsmI960::processModeBits},
};

PROGMEM constexpr AsmI960::PseudosI960 AsmI960::PSEUDOS_I960{
        ARRAY_RANGE(AsmI960::PSEUDO_I960_TABLE)};

const ValueParser::Plugins &AsmI960::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const SymbolParser &symbol() const override { return I960SymbolParser::singleton(); }
        const LetterParser &letter() const override { return CStyleLetterParser::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmI960::AsmI960(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_modeBits),
      Config(TABLE),
      _opt_modeBits(this, &AsmI960::setModeBitsOption, OPT_BOOL_MODE_BITS, OPT_DESC_MODE_BITS,
              &_opt_ipOperand),
      _opt_ipOperand(this, &Config::setIpOperand, OPT_TEXT_IP_OPERAND, OPT_DESC_IP_OPERAND,
              &_opt_fpu),
      _opt_fpu(this, &AsmI960::setFpuOption, OPT_BOOL_FPU, OPT_DESC_FPU) {
    reset();
}

void AsmI960::reset() {
    Assembler::reset();
    // Otherwise the settings leak from one assembly into the next.
    setModeBits(MODE_BITS_SET);
    setIpMode(IP_OFFSET);
    // The base 80960 instruction set; .fpu on adds the 80960KB extensions.
    setFpu(false);
}

Error AsmI960::setFpuOption(bool enable) {
    return setFpu(enable);
}

Error AsmI960::processFpu(StrScanner &scan, Insn &insn, uint16_t) {
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
    return insn.setErrorIf(at, setFpu(on));
}

Error AsmI960::setModeBitsOption(int32_t value) {
    return setModeBits(value);
}

// The directive names which reading the operand of the IP-relative mode
// takes: its offset from IP + 8, or the address to be reached.
Error AsmI960::processIpOperand(StrScanner &scan, Insn &insn, uint16_t) {
    auto p = scan.skipSpaces();
    const auto at = p;
    StrScanner name;
    if (parser().readSymbol(p, name) != OK || !endOfLine(p.skipSpaces()))
        return insn.setError(at, UNKNOWN_OPERAND);
    if (setIpOperand(name) != OK)
        return insn.setError(at, UNKNOWN_OPERAND);
    scan = p;
    return OK;
}

// The directive takes the value the bit is to carry, 0 or 1.
Error AsmI960::processModeBits(StrScanner &scan, Insn &insn, uint16_t) {
    auto p = scan.skipSpaces();
    const auto at = p;
    Value value;
    if (value.read(p) != OK || !endOfLine(p.skipSpaces()))
        return insn.setError(at, UNKNOWN_OPERAND);
    scan = p;
    return insn.setErrorIf(at, setModeBits(value.getUnsigned()));
}

// The Guide, page 5-25, gives .extended an 80-bit floating point operand.  It
// is the same datum the i8086 assembler writes for dt, and GNU as emits the
// ten significand and sign-exponent bytes with nothing after them.
Error AsmI960::defineExtended(StrScanner &scan, Insn &insn, uint16_t) {
#if defined(LIBASM_ASM_NOFLOAT)
    insn.setErrorIf(scan, FLOAT_NOT_SUPPORTED);
    return insn.getError();
#else
    ErrorAt error;
    do {
        auto p = scan.skipSpaces();
        ErrorAt exprErr;
        const auto val = parseExpr(p, exprErr);
        if (!endOfLine(p) && *p != ',') {
            error.setErrorIf(scan, ILLEGAL_CONSTANT);
            break;
        }
        if (exprErr.hasError()) {
            error.setErrorIf(exprErr);
            break;
        }
        const auto &val80 = val.getFloat();
        insn.emitUint64Le(val80.significand());
        insn.emitUint16Le(val80.tag());
        scan = p;
    } while (scan.skipSpaces().expect(','));
    return insn.setError(error);
#endif
}

Error AsmI960::processPseudo(StrScanner &scan, Insn &insn) {
    const auto *p = PSEUDOS_I960.search(insn.name());
    return p ? p->invoke(this, scan, insn) : Assembler::processPseudo(scan, insn);
}

// The base and index of a memory operand, written (abase)[index*scale] after
// any displacement, or (ip) for the IP-relative form.
Error AsmI960::parseMemory(StrScanner &scan, Operand &op) const {
    auto p = scan;
    if (p.expect('(')) {
        auto a = p.skipSpaces();
        // Match the whole symbol, so that (iparm) is an unknown operand rather
        // than an unclosed (ip).
        if (parser().readRegName(a).iequals_P(TEXT_REG_IP)) {
            op.ip = true;
            p = a;
        } else {
            op.base = parseRegName(p, parser());
            if (op.base == NO_REG)
                return op.setError(p, UNKNOWN_OPERAND);
        }
        if (!p.skipSpaces().expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        p.skipSpaces();
    }
    if (p.expect('[')) {
        op.index = parseRegName(p.skipSpaces(), parser());
        if (op.index == NO_REG)
            return op.setError(p, UNKNOWN_OPERAND);
        if (p.skipSpaces().expect('*')) {
            ErrorAt scaleErr;
            const auto at = p.skipSpaces();
            const auto val = parseInteger(p, scaleErr, ']');
            if (scaleErr.hasError())
                return op.setError(scaleErr);
            // The scale is encoded as the power of two, and the hardware
            // defines only the five values up to sixteen.
            const auto factor = val.getUnsigned();
            uint8_t scale = 0;
            while (scale <= MAX_SCALE && (UINT32_C(1) << scale) != factor)
                scale++;
            if (scale > MAX_SCALE)
                return op.setError(at, OPERAND_NOT_ALLOWED);
            op.scale = scale;
        }
        if (!p.skipSpaces().expect(']'))
            return op.setError(p, MISSING_CLOSING_BRACKET);
    }
    scan = p;
    return OK;
}

Error AsmI960::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    auto r = p;
    const auto regno = parseRegName(r, parser());
    if (regno != NO_REG && (endOfLine(r.skipSpaces()) || *r == ',')) {
        op.mode = M_REG;
        op.regno = regno;
        scan = r;
        return OK;
    }
    auto f = p;
    const auto freg = parseFloatRegName(f, parser());
    if (freg != NO_REG && (endOfLine(f.skipSpaces()) || *f == ',')) {
        op.mode = M_FREG;
        op.floatField = freg;
        scan = f;
        return OK;
    }
    // The only real literals the encoding has are +0.0 and +1.0 (Table B-1).
    auto d = p;
    const auto one = *d == '1';
    if (d.expect('0') || d.expect('1')) {
        if (d.expect('.') && d.expect('0') && (endOfLine(d.skipSpaces()) || *d == ',')) {
            op.mode = M_FREG;
            op.floatField = one ? FLOAT_ONE : FLOAT_ZERO;
            scan = d;
            return OK;
        }
    }
    if (*p != '(' && *p != '[') {
        op.val = parseInteger(p, op, '(');
        if (op.hasError())
            return op.getError();
        op.hasDisp = true;
        p.skipSpaces();
    }
    // A base or an index register makes this a memory operand.  A bare
    // expression may still be a literal, a branch target or a MEMA offset, so
    // it is reported as a value and the chosen entry decides.
    if (*p == '(' || *p == '[') {
        if (parseMemory(p, op))
            return op.getError();
        op.mode = M_MEM;
    } else {
        op.mode = M_VALUE;
    }
    scan = p;
    return OK;
}

void AsmI960::encodeRegister(
        AsmInsn &insn, const Operand &op, AddrMode mode, uint_fast8_t pos) const {
    const auto regs = operandRegs(mode);
    if (op.regno % regs)
        insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
    insn.embed(static_cast<Config::opcode_t>(op.regno) << pos);
}

void AsmI960::encodeLiteral(
        AsmInsn &insn, const Operand &op, Config::opcode_t modeBit, uint_fast8_t pos) const {
    const auto val = op.val.getUnsigned();
    if (op.val.overflow(31))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.embed(modeBit);
    insn.embed(static_cast<Config::opcode_t>(val & 0x1F) << pos);
}

// A branch names the target address itself and the assembler works out the
// displacement, which is a byte count with its low two bits zero.
void AsmI960::encodeDisplacement(AsmInsn &insn, const Operand &op, uint_fast8_t bits) const {
    const auto base = insn.address();
    const auto target = op.getError() ? base : op.val.getUnsigned();
    if (target & 3) {
        insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
        return;
    }
    const auto delta = branchDelta(base, target, insn, op);
    if (overflowDelta(delta, bits))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    // The field is bits |bits|-1 down to 2; the low two are always zero.
    const auto mask = ((UINT32_C(1) << bits) - 1) & ~UINT32_C(3);
    insn.embed(static_cast<Config::opcode_t>(delta) & mask);
}

void AsmI960::encodeMemory(AsmInsn &insn, const Operand &op) const {
    if (op.base != NO_REG)
        insn.embed(static_cast<Config::opcode_t>(op.base) << 14);

    if (op.ip) {
        // The operand is the offset itself; the processor adds the IP and a
        // constant of 8 to it (Manual Table 5-1 and 5-8).  A bare (ip) carries
        // no expression and so addresses IP + 8.  The ip-operand option set to
        // target reads it the way ASL does, as the address to be reached.
        insn.embedMemMode(MEM_IP);
        if (ipOffset()) {
            insn.displacement = op.val.getUnsigned();
        } else {
            const auto base = insn.address() + 8;
            insn.displacement = (op.getError() ? base : op.val.getUnsigned()) - base;
        }
        insn.hasDisplacement = true;
        return;
    }

    const auto disp = op.val.getUnsigned();
    if (op.index == NO_REG) {
        // MEMA reaches a twelve-bit unsigned offset.  A bare (abase) carries
        // no offset at all and is MEMB; 0(abase) is the MEMA form.
        if (op.hasDisp && disp <= 0xFFF) {
            if (op.base != NO_REG)
                insn.embed(MEMA_ABASE);
            insn.embed(disp);
            return;
        }
        if (!op.hasDisp) {
            insn.embedMemMode(MEM_ABASE);
            return;
        }
        insn.embedMemMode(op.base == NO_REG ? MEM_DISP : MEM_DISP_ABASE);
        insn.displacement = disp;
        insn.hasDisplacement = true;
        return;
    }

    insn.embed(static_cast<Config::opcode_t>(op.scale) << 7);
    insn.embed(op.index);
    // An index with an address base and no displacement is the one indexed
    // mode with no second word.
    if (!op.hasDisp && op.base != NO_REG) {
        insn.embedMemMode(MEM_ABASE_IX);
        return;
    }
    insn.embedMemMode(op.base == NO_REG ? MEM_DISP_IX : MEM_DISP_ABASE_IX);
    insn.displacement = op.hasDisp ? disp : 0;
    insn.hasDisplacement = true;
}

// A floating-point operand: a global or local register leaves the mode bit
// clear, a floating-point register or a real literal raises it.
void AsmI960::encodeFloat(AsmInsn &insn, const Operand &op, AddrMode mode,
        Config::opcode_t modeBit, uint_fast8_t pos) const {
    if (op.mode == M_FREG) {
        insn.embed(modeBit);
        insn.embed(static_cast<Config::opcode_t>(op.floatField) << pos);
        return;
    }
    encodeRegister(insn, op, mode, pos);
}

void AsmI960::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    switch (mode) {
    case M_NONE:
        return;
    case M_SRC1:
    case M_SRC1L:
    case M_SRC1Q:
        if (op.mode == M_REG) {
            encodeRegister(insn, op, mode, 0);
        } else {
            encodeLiteral(insn, op, BIT_M1, 0);
        }
        return;
    case M_REG1:
        encodeRegister(insn, op, mode, 0);
        return;
    case M_SRC2:
    case M_SRC2L:
        if (op.mode == M_REG) {
            encodeRegister(insn, op, mode, 14);
        } else {
            encodeLiteral(insn, op, BIT_M2, 14);
        }
        return;
    case M_REG2:
    case M_CSRC2:
        encodeRegister(insn, op, mode, 14);
        return;
    case M_DST:
    case M_DSTL:
    case M_DSTQ:
    case M_MDST:
    case M_MDSTL:
    case M_MDSTQ:
    case M_CREG1:
        encodeRegister(insn, op, mode, 19);
        return;
    case M_CSRC1:
        if (op.mode == M_REG) {
            encodeRegister(insn, op, mode, 19);
        } else {
            encodeLiteral(insn, op, BIT_COBR_M1, 19);
        }
        return;
    case M_CDISP:
        encodeDisplacement(insn, op, 13);
        return;
    case M_TDISP:
        encodeDisplacement(insn, op, 24);
        return;
    case M_FREG:
    case M_REG:
    case M_VALUE:
        // Reported by the parser only; no table entry names them.
        return;
    case M_MEM:
        encodeMemory(insn, op);
        return;
    case M_FSRC1:
    case M_FSRC1L:
    case M_FSRC1Q:
        encodeFloat(insn, op, mode, BIT_M1, 0);
        return;
    case M_FSRC2:
    case M_FSRC2L:
    case M_FSRC2Q:
        encodeFloat(insn, op, mode, BIT_M2, 14);
        return;
    case M_FDST:
    case M_FDSTL:
    case M_FDSTQ:
        // A destination takes no real literal, only a register.
        if (op.mode == M_FREG && op.floatField >= FLOAT_REGS)
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        encodeFloat(insn, op, mode, BIT_M3, 19);
        return;
    }
}

Error AsmI960::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    for (uint_fast8_t pos = 0; pos < MAX_OPERANDS; pos++) {
        auto &op = insn.operands[pos];
        if (parseOperand(scan, op) && op.hasError())
            return _insn.setError(op);
        if (op.mode == M_NONE)
            break;
        if (!scan.skipSpaces().expect(','))
            break;
    }
    scan.skipSpaces();

    if (searchName(cpuType(), modeBits(), hasFpu(), insn))
        return _insn.setError(insn.operands[0], insn);

    for (uint_fast8_t pos = 0; pos < MAX_OPERANDS; pos++) {
        const auto &op = insn.operands[pos];
        encodeOperand(insn, op, insn.mode(pos));
        // Carry over an error the operand tolerated while parsing, such as a
        // reference which stayed undefined.
        insn.setErrorIf(op);
    }
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace i960
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
