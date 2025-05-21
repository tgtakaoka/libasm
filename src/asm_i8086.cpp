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

#include "asm_i8086.h"
#include "table_i8086.h"
#include "text_i8086.h"

namespace libasm {
namespace i8086 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::option;
using namespace text::i8086;

namespace {

// clang-format off
constexpr char OPT_BOOL_OPTIMIZE_SEGMENT[] PROGMEM = "optimize-segment";
constexpr char OPT_DESC_OPTIMIZE_SEGMENT[] PROGMEM = "optimize segment override";

constexpr char TEXT_DQ[] PROGMEM = "dq";
constexpr char TEXT_DT[] PROGMEM = "dt";
constexpr char TEXT_RESB[] PROGMEM = "resb";
constexpr char TEXT_RESD[] PROGMEM = "resd";
constexpr char TEXT_RESW[] PROGMEM = "resw";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_DB,   &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DS,   &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DW,   &Assembler::defineDataConstant, Assembler::DATA_WORD},
    {TEXT_RESB, &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_RESD, &Assembler::allocateSpaces,     Assembler::DATA_LONG},
    {TEXT_RESW, &Assembler::allocateSpaces,     Assembler::DATA_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

struct I8086SymbolParser final : SimpleSymbolParser, Singleton<I8086SymbolParser> {
    I8086SymbolParser() : SimpleSymbolParser(PSTR_UNDER_AT_QUESTION) {}
};

}  // namespace

const ValueParser::Plugins &AsmI8086::defaultPlugins() {
    static struct final : ValueParser::IntelPlugins {
        const SymbolParser &symbol() const { return I8086SymbolParser::singleton(); }
    } PLUGINS;
    return PLUGINS;
}

AsmI8086::AsmI8086(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_fpu),
      Config(TABLE),
      _opt_fpu(this, &Config::setFpuName, OPT_TEXT_FPU, OPT_DESC_FPU, &_opt_optimizeSegment),
      _opt_optimizeSegment(this, &AsmI8086::setOptimizeSegment, OPT_BOOL_OPTIMIZE_SEGMENT,
              OPT_DESC_OPTIMIZE_SEGMENT) {
    reset();
}

void AsmI8086::reset() {
    Assembler::reset();
    setFpuType(FPU_NONE);
    setOptimizeSegment(true);
}

Error AsmI8086::setOptimizeSegment(bool enable) {
    _optimizeSegment = enable;
    return OK;
}

Error AsmI8086::parseStringInst(StrScanner &scan, Operand &op) const {
    auto p = scan;
    StrScanner opr;
    if (parser().readSymbol(p, opr) != OK)
        return UNKNOWN_INSTRUCTION;
    Insn _insn(0);
    AsmInsn insn(_insn);
    insn.nameBuffer().text(opr);
    if (searchName(_cpuSpec, insn))
        return UNKNOWN_INSTRUCTION;
    if (!insn.stringInst())
        return UNKNOWN_INSTRUCTION;
    scan = p.skipSpaces();
    op.val.setUnsigned(insn.opCode());
    op.mode = M_ISTR;
    return OK;
}

Error AsmI8086::parsePointerSize(StrScanner &scan, Operand &op) const {
    auto p = scan;
    const auto reg = parseRegName(p, parser());
    if (reg > REG_PTR) {
        // Pointer size override
        if (parseRegName(p.skipSpaces(), parser()) == REG_PTR) {
            op.ptr = reg;
            scan = p.skipSpaces();
            return OK;
        }
        return op.setError(UNKNOWN_OPERAND);
    }
    return OK;
}

void AsmI8086::parseSegmentOverride(StrScanner &scan, Operand &op) const {
    auto p = scan;
    const auto reg = parseRegName(p, parser());
    if (isSegmentReg(reg)) {
        // Segment Override
        if (p.skipSpaces().expect(':')) {
            op.seg = reg;
            scan = p;
        }
    }
}

void AsmI8086::parseBaseRegister(StrScanner &scan, Operand &op) const {
    auto p = scan;
    const auto reg = parseRegName(p, parser());
    if (reg == REG_BX || reg == REG_BP) {
        op.reg = reg;
        scan = p.skipSpaces();
    }
}

void AsmI8086::parseIndexRegister(StrScanner &scan, Operand &op) const {
    auto p = scan;
    if (op.reg != REG_UNDEF) {
        if (!p.expect('+'))
            return;
        p.skipSpaces();
    }
    const auto reg = parseRegName(p, parser());
    if (reg == REG_SI || reg == REG_DI) {
        op.index = reg;
        scan = p.skipSpaces();
    }
}

Error AsmI8086::parseDisplacement(StrScanner &scan, Operand &op) const {
    auto p = scan;
    if (endOfLine(p) || *p == ']')
        return OK;
    if (op.reg != REG_UNDEF || op.index != REG_UNDEF) {
        if (*p != '+' && *p != '-')
            return op.setError(UNKNOWN_OPERAND);
    }
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.hasDisp = true;
    scan = p.skipSpaces();
    return OK;
}

namespace {
AddrMode pointerMode(RegName ptr, AddrMode undef, AddrMode byte, AddrMode word) {
    if (ptr == REG_UNDEF)
        return undef;
    if (ptr == REG_BYTE)
        return byte;
    if (ptr == REG_WORD)
        return word;
    if (ptr == REG_DWORD)
        return M_DMEM;
    return M_FMEM;
}
}  // namespace

Error AsmI8086::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (parseStringInst(p, op) == OK) {
        scan = p;
        return OK;
    }
    if (parsePointerSize(p, op))
        return op.getError();
    parseSegmentOverride(p, op);
    if (p.skipSpaces().expect('[')) {
        parseBaseRegister(p.skipSpaces(), op);
        parseIndexRegister(p, op);
        if (parseDisplacement(p, op))
            return op.getError();
        if (!p.skipSpaces().expect(']'))
            return op.setError(p, MISSING_CLOSING_BRACKET);
        scan = p;
        if (op.reg == REG_UNDEF && op.index == REG_UNDEF) {
            if (!op.hasDisp)
                return op.setError(UNKNOWN_OPERAND);
            op.mode = pointerMode(op.ptr, M_DIR, M_BDIR, M_WDIR);
            return OK;
        }
        op.mode = pointerMode(op.ptr, M_MEM, M_BMEM, M_WMEM);
        return OK;
    }
    if (op.ptr != REG_UNDEF || op.seg != REG_UNDEF)
        return op.setError(UNKNOWN_OPERAND);

    auto a = p;
    const auto reg = parseRegName(a, parser());
    if (isGeneralReg(reg)) {
        op.reg = reg;
        switch (reg) {
        case REG_AL:
            op.mode = M_AL;
            break;
        case REG_CL:
            op.mode = M_CL;
            break;
        case REG_AX:
            op.mode = M_AX;
            break;
        case REG_DX:
            op.mode = M_DX;
            break;
        default:
            op.mode = (generalRegSize(reg) == SZ_BYTE) ? M_BREG : M_WREG;
            break;
        }
        scan = a;
        return OK;
    }
    if (isSegmentReg(reg)) {
        op.reg = reg;
        op.mode = (reg == REG_CS) ? M_CS : M_SREG;
        scan = a;
        return OK;
    }
    if (reg == REG_ST) {
        if (a.skipSpaces().expect('(')) {
            const auto valp = a;
            op.val = parseInteger(a, op, ')');
            if (op.hasError())
                return op.getError();
            if (a.skipSpaces().expect(')')) {
                if (op.val.overflow(7))
                    op.setErrorIf(valp, OVERFLOW_RANGE);
                op.mode = (op.isOK() && op.val.isZero()) ? M_ST0 : M_STI;
                scan = a;
                return OK;
            }
            return op.setErrorIf(a, MISSING_CLOSING_PAREN);
        }
        op.mode = M_ST0;
        scan = a;
        return OK;
    }
    if (reg != REG_UNDEF)
        return op.setError(UNKNOWN_OPERAND);

    const auto valp = p.skipSpaces();
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    if (p.skipSpaces().expect(':')) {
        if (op.val.overflow(UINT16_MAX))
            return op.setError(valp, OVERFLOW_RANGE);
        op.segval = op.val;
        ErrorAt offset;
        offset.setAt(p);
        op.val = parseInteger(p, offset);
        if (op.hasError())
            return offset.getError();
        op.setErrorIf(offset);
        op.mode = M_FAR;
        scan = p;
        return OK;
    }
    op.mode = op.immediateMode();
    scan = p;
    return OK;
}

AddrMode Operand::immediateMode() const {
    if (getError())
        return M_WIMM;
    const auto v = val.getUnsigned();
    if (v == 1)
        return M_VAL1;
    if (v == 3)
        return M_VAL3;
    return val.overflowInt8() ? M_WIMM : M_BIMM;
}

void AsmI8086::emitImmediate(
        AsmInsn &insn, AddrMode mode, OprSize size, const Value &val, const ErrorAt &at) const {
    const auto imm = (mode == M_BIMM || mode == M_WIMM);
    if (size == SZ_BYTE) {
        if (imm && val.overflowUint8())
            insn.setErrorIf(at, OVERFLOW_RANGE);
        if (!imm && val.overflow(UINT8_MAX))
            insn.setErrorIf(at, OVERFLOW_RANGE);
        insn.emitOperand8(val.getUnsigned());
    } else if (size == SZ_WORD) {
        if (imm && val.overflowUint16())
            insn.setErrorIf(at, OVERFLOW_RANGE);
        if (!imm && val.overflow(UINT16_MAX))
            insn.setErrorIf(at, OVERFLOW_RANGE);
        insn.emitOperand16(val.getUnsigned());
    }
}

namespace {

constexpr Config::opcode_t JMP_rel8 = 0xEB;
constexpr Config::opcode_t JMP_rel16 = 0xE9;

constexpr bool maySmartBranch(Config::opcode_t opc) {
    return opc == JMP_rel8;
}

void shortBranch(AsmInsn &insn) {
    insn.setOpCode(JMP_rel8);
}

void longBranch(AsmInsn &insn, const Operand &op) {
    insn.setOpCode(JMP_rel16);
    insn.setError(op);
}

constexpr Config::uintptr_t segment(Config::uintptr_t addr) {
    return addr & ~0xFFFF;
}

}  // namespace

void AsmI8086::emitRelative(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    const auto base = insn.address() + 2;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    insn.setErrorIf(op, checkAddr(target, insn.address(), 16));
    const auto delta = branchDelta(base, target, insn, op);
    const auto smartBranch = maySmartBranch(insn.opCode());
    if (mode == M_REL8 && !smartBranch) {
        if (overflowDelta(delta, 8))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
    short_branch:
        insn.emitOperand8(delta);
        return;
    }
    if (mode == M_REL && !smartBranch) {
    long_branch:
        const auto base = insn.address() + 3;
        const auto target = op.getError() ? base : op.val.getUnsigned();
        insn.setErrorIf(op, checkAddr(target, insn.address(), 16));
        const auto delta = branchDelta(base, target, insn, op);
        if (overflowDelta(delta, 16))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand16(delta);
        return;
    }
    if (op.getError() || overflowDelta(delta, 8)) {
        longBranch(insn, op);
        goto long_branch;
    }
    shortBranch(insn);
    goto short_branch;
}

void AsmI8086::emitRegister(AsmInsn &insn, const Operand &op, OprPos pos) const {
    const auto num = encodeRegNum(op.reg);
    switch (pos) {
    case P_OREG:
        insn.embed(num);
        break;
    case P_OSEG:
        insn.embed(num << 3);
        break;
    case P_OMOD:
        insn.embed(0300 | num);
        break;
    case P_REG:
        insn.embedModReg(num << 3);
        break;
    case P_MOD:
        insn.embedModReg(0300 | num);
        break;
    case P_MREG:
        insn.embedModReg(0300 | (num << 3) | num);
        break;
    default:
        break;
    }
}

uint8_t Operand::encodeMod() const {
    const auto needDisp =
            (reg == REG_BP && index == REG_UNDEF) || (hasDisp && (!val.isZero() || getError()));
    if (needDisp)
        return val.overflowInt8() || getError() ? 2 : 1;
    return 0;
}

uint8_t Operand::encodeR_m() const {
    uint8_t r_m = 0;
    if (reg == REG_UNDEF) {
        r_m = (index == REG_SI) ? 4 : 5;
    } else if (index == REG_UNDEF) {
        r_m = (reg == REG_BP) ? 6 : 7;
    } else {
        if (reg == REG_BP)
            r_m |= 2;
        if (index == REG_DI)
            r_m |= 1;
    }
    return r_m;
}

Config::opcode_t AsmI8086::encodeSegmentOverride(RegName seg, RegName base) const {
    if (seg == REG_UNDEF)
        return 0;
    const Config::opcode_t segPrefix = segOverridePrefix(seg);
    if (_optimizeSegment) {
        if (base == REG_BP || base == REG_SP)
            return seg == REG_SS ? 0 : segPrefix;
        return seg == REG_DS ? 0 : segPrefix;
    }
    return segPrefix;
}

void AsmI8086::emitModReg(AsmInsn &insn, const Operand &op, OprPos pos) const {
    uint8_t mod;
    uint8_t modReg;
    switch (op.mode) {
    case M_AL:
    case M_CL:
    case M_AX:
    case M_DX:
    case M_BREG:
    case M_WREG:
        emitRegister(insn, op, pos);
        break;
    case M_BDIR:
    case M_WDIR:
    case M_DIR:
        if (insn.leaInsn() && op.seg != REG_UNDEF)
            insn.setErrorIf(op, OPCODE_HAS_NO_EFFECT);
        emitDirect(insn, op, pos);
        break;
    case M_DMEM:
    case M_FMEM:
    case M_MEM:
        if (op.reg == REG_UNDEF && op.index == REG_UNDEF) {
            emitDirect(insn, op, pos);
            break;
        }
        // Fall-trhough
    case M_BMEM:
    case M_WMEM:
        if (insn.leaInsn() && op.seg != REG_UNDEF)
            insn.setErrorIf(op, OPCODE_HAS_NO_EFFECT);
        insn.setSegment(encodeSegmentOverride(op.seg, op.reg));
        mod = op.encodeMod();
        modReg = mod << 6;
        modReg |= op.encodeR_m();
        if (pos == P_OMOD) {
            insn.embed(modReg);
        } else {
            insn.embedModReg(modReg);
        }
        if (mod == 1) {
            if (op.val.overflowInt8())
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand8(op.val.getUnsigned());
        } else if (mod == 2) {
            if (op.val.overflowUint16())
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand16(op.val.getUnsigned());
        }
        break;
    default:
        break;
    }
}

void AsmI8086::emitDirect(AsmInsn &insn, const Operand &op, OprPos pos) const {
    insn.setSegment(encodeSegmentOverride(op.seg, REG_UNDEF));
    if (pos == P_MOD)
        insn.embedModReg(0006);
    if (pos == P_OMOD)
        insn.embed(0006);
    emitImmediate(insn, M_DIR, SZ_WORD, op.val, op);
}

void AsmI8086::emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op, OprPos pos) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_CS:
        if (pos == P_NONE)  // POP CS
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case M_BREG:
    case M_WREG:
    case M_SREG:
        emitRegister(insn, op, pos);
        break;
    case M_BMOD:
    case M_WMOD:
    case M_BMEM:
    case M_WMEM:
    case M_DMEM:
    case M_FMEM:
    case M_MEM:
        emitModReg(insn, op, pos);
        break;
    case M_BDIR:
    case M_WDIR:
        emitDirect(insn, op, P_OPR);
        break;
    case M_UI16:
        if (op.val.overflow(UINT16_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_UI8:
        if (op.val.overflow(UINT8_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_BIT:
        if (insn.size() == SZ_BYTE && op.val.overflow(7))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        if (insn.size() == SZ_WORD && op.val.overflow(15))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_WIMM:
        emitImmediate(insn, mode, insn.size(), op.val, op);
        break;
    case M_IOA:
    case M_BIMM:
        emitImmediate(insn, mode, SZ_BYTE, op.val, op);
        break;
    case M_REL:
    case M_REL8:
        emitRelative(insn, op, mode);
        break;
    case M_FAR:
        emitImmediate(insn, M_OFF, SZ_WORD, op.val, op);
        emitImmediate(insn, M_SEG, SZ_WORD, op.segval, op);
        break;
    case M_SEG:
        break;  // emit with M_OFF
    case M_OFF:
        emitImmediate(insn, mode, SZ_WORD, op.val, op);
        emitImmediate(insn, M_SEG, SZ_WORD, insn.dstOp.val, op);
        break;
    case M_ISTR:
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_STI:
        insn.embed(op.val.getUnsigned() & 7);
        break;
    default:
        break;
    }
}

void AsmI8086::emitStringOperand(
        AsmInsn &insn, const Operand &op, RegName seg, RegName index) const {
    if (op.mode == M_NONE)
        return;
    if (op.reg != REG_UNDEF || op.index != index || op.hasDisp)
        insn.setErrorIf(op, ILLEGAL_OPERAND);
    if (seg == REG_ES && op.seg != REG_ES)
        insn.setErrorIf(op, ILLEGAL_SEGMENT);
    if (seg == REG_DS && op.seg != REG_UNDEF && op.seg != REG_DS)
        insn.setSegment(segOverridePrefix(op.seg));
}

void AsmI8086::emitStringInst(AsmInsn &insn, const Operand &dst, const Operand &src) const {
    insn.setErrorIf(dst);
    insn.setErrorIf(src);
    switch (insn.opCode() & ~1) {
    case 0xA4:  // MOVS ES:[DI],DS:[SI]
    case 0x20:  // ADD4S ES:[DI],DS:[SI]
    case 0x22:  // SUB4S ES:[DI],DS:[SI]
    case 0x26:  // CMP4S ES:[DI],DS:[SI]
        emitStringOperand(insn, src, REG_DS, REG_SI);
        /* Fall-through */
    case 0xAA:  // STOS ES:[DI]
    case 0xAE:  // SCAS ES:[DI]
    case 0x6C:  // INS ES:[DI]
        emitStringOperand(insn, dst, REG_ES, REG_DI);
        break;
    case 0xA6:  // CMPS DS:[SI],ES:[DI]
        emitStringOperand(insn, src, REG_ES, REG_DI);
        /* Fall-through */
    case 0xAC:  // LODS DS:[SI]
        emitStringOperand(insn, dst, REG_DS, REG_SI);
        break;
    case 0x6E:  // OUTS DS:[SI]
        emitStringOperand(insn, src, REG_DS, REG_SI);
        break;
    }
}

#if !defined(LIBASM_ASM_NOFLOAT)

Error AsmInsn::emitPackedDecimal(int64_t val64) {
    const auto sign = val64 < 0 ? 0x80 : 0;
    auto u64 = static_cast<uint64_t>(sign ? -val64 : val64);
    for (auto i = 0; i < 9; ++i) {
        uint8_t d = u64 % 10;
        u64 /= 10;
        d |= (u64 % 10) << 4;
        u64 /= 10;
        emitByte(d);
    }
    return emitByte(sign);
}

Error AsmInsn::emitTemporaryReal(const float80_t &val80) {
    emitUint64(val80.significand());
    return emitUint16(val80.tag());
}

#endif

Error AsmI8086::defineDataConstant(
        AsmInsn &insn, StrScanner &scan, I8087Type type, ErrorAt &_error) const {
    ErrorAt error;
    do {
        scan.skipSpaces();
        ErrorAt strErr;
        if (type == DATA_DD) {
            auto end = scan;
            const auto err = isString(end, strErr);
            if (err == OK) {
                generateString(scan, end, insn.insnBase(), DATA_LONG, strErr);
                if (error.setErrorIf(strErr) == NO_MEMORY)
                    break;
                continue;
            }
        }
        ErrorAt exprErr;
        auto p = scan;
        const auto val = parseExpr(p, exprErr);
        if (!endOfLine(p) && *p != ',') {
            if (strErr.getError()) {
                error.setErrorIf(strErr);
            } else {
                error.setErrorIf(scan, ILLEGAL_CONSTANT);
            }
            break;
        }
        if (exprErr.hasError()) {
            error.setErrorIf(strErr.getError() ? strErr : exprErr);
            break;
        }
        switch (type) {
        case DATA_DD:
            if (val.isInteger()) {
                if (val.overflowUint32()) {
                    exprErr.setErrorIf(scan, OVERFLOW_RANGE);
                } else {
                    exprErr.setErrorIf(scan, insn.emitUint32(val.getUnsigned()));
                }
            } else {
#if !defined(LIBASM_ASM_NOFLOAT)
                exprErr.setErrorIf(scan, insn.emitFloat32(val.getFloat()));
#else
                exprErr.setErrorIf(scan, FLOAT_NOT_SUPPORTED);
                insn.emitUint32(0);
#endif
            }
            break;
        case DATA_DQ:
#if defined(LIBASM_ASM_NOFLOAT)
            exprErr.setErrorIf(scan, FLOAT_NOT_SUPPORTED);
            insn.emitUint64(0);
#else
            if (val.isInteger()) {
                exprErr.setErrorIf(scan, insn.emitUint64(val.getInteger()));
            } else {
                exprErr.setErrorIf(scan, insn.emitFloat64(val.getFloat()));
            }
#endif
            break;
        case DATA_DT:
#if defined(LIBASM_ASM_NOFLOAT)
            exprErr.setErrorIf(scan, FLOAT_NOT_SUPPORTED);
            insn.emitUint16(0);
            insn.emitUint64(0);
#else
            if (val.isInteger()) {
                constexpr auto PDEC80_MAX = INT64_C(999'999'999'999'999'999);
                constexpr auto PDEC80_MIN = -INT64_C(999'999'999'999'999'999);
                const auto v = val.getInteger();
                if (v < PDEC80_MIN || v > PDEC80_MAX) {
                    exprErr.setErrorIf(scan, OVERFLOW_RANGE);
                } else {
                    exprErr.setErrorIf(scan, insn.emitPackedDecimal(v));
                }
            } else {
                exprErr.setErrorIf(scan, insn.emitTemporaryReal(val.getFloat()));
            }
#endif
            break;
        }
        scan = p;
        error.setErrorIf(exprErr);
        if (error.setErrorIf(exprErr) == NO_MEMORY)
            break;
    } while (scan.skipSpaces().expect(','));
    return _error.setError(error);
}

Error AsmI8086::processPseudo(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    const auto at = scan;
    if (strcasecmp_P(insn.name(), TEXT_FPU) == 0) {
        const auto error = _opt_fpu.set(scan);
        return error ? _insn.setErrorIf(at, error) : OK;
    }
    if (strcasecmp_P(insn.name(), TEXT_DD) == 0)
        return defineDataConstant(insn, scan, DATA_DD, _insn);
    if (strcasecmp_P(insn.name(), TEXT_DQ) == 0)
        return defineDataConstant(insn, scan, DATA_DQ, _insn);
    if (strcasecmp_P(insn.name(), TEXT_DT) == 0)
        return defineDataConstant(insn, scan, DATA_DT, _insn);
    return Assembler::processPseudo(scan, _insn);
}

void AsmInsn::prepairModReg() {
    if (hasPrefix())
        return;
    const auto dst = dstPos();
    const auto src = srcPos();
    if (dst == P_MOD || dst == P_REG || dst == P_MREG || src == P_MOD || src == P_REG)
        embedModReg(0);
}

void AsmInsn::embedModReg(Config::opcode_t data) {
    _modReg |= data;
    _hasModReg = true;
}

void AsmInsn::emitInsn() {
    uint_fast8_t pos = 0;
    auto pre = prefix();
    if ((pre >> 8) == FWAIT) {
        pre &= UINT8_MAX;
        emitByte(FWAIT, pos++);
    }
    if (_segment)
        emitByte(_segment, pos++);
    if (pre) {
        if (pre >= 0x100)
            emitByte(pre >> 8, pos++);
        emitByte(pre, pos++);
    }
    emitByte(opCode(), pos++);
    if (_hasModReg)
        emitByte(_modReg, pos);
}

uint_fast8_t AsmInsn::operandPos() const {
    uint_fast8_t pos = length();
    if (pos == 0) {
        if (_segment)
            pos++;
        if (hasPrefix()) {
            if (prefix() >= 0x100)
                pos++;
            pos++;
        }
        pos++;
        if (_hasModReg)
            pos++;
    }
    return pos;
}

Error AsmI8086::encodeImpl(StrScanner &scan, Insn &_insn) const {
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

    if (searchName(_cpuSpec, insn))
        return _insn.setError(insn.dstOp, insn);
    insn.prepairModReg();

    if (insn.stringInst()) {
        emitStringInst(insn, insn.dstOp, insn.srcOp);
    } else {
        emitOperand(insn, insn.dst(), insn.dstOp, insn.dstPos());
        emitOperand(insn, insn.src(), insn.srcOp, insn.srcPos());
        emitOperand(insn, insn.ext(), insn.extOp, insn.extPos());
    }
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace i8086
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
