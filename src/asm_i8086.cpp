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

using text::i8086::TEXT_FPU;
using text::i8086::TEXT_FPU_8087;
using text::i8086::TEXT_none;

namespace {

// clang-format off
constexpr char OPT_BOOL_OPTIMIZE_SEGMENT[] PROGMEM = "optimize-segment";
constexpr char OPT_DESC_OPTIMIZE_SEGMENT[] PROGMEM = "enable optimizing segment override";

constexpr char TEXT_DQ[] PROGMEM = "dq";
constexpr char TEXT_DT[] PROGMEM = "dt";
constexpr char TEXT_RESB[] PROGMEM = "resb";
constexpr char TEXT_RESD[] PROGMEM = "resd";
constexpr char TEXT_RESW[] PROGMEM = "resw";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_DB,   &Assembler::defineDataConstant,  Assembler::DATA_BYTE},
    {TEXT_DD,   &Assembler::defineFloatConstant, Assembler::DATA_FLOAT32_LONG},
    {TEXT_DQ,   &Assembler::defineFloatConstant, Assembler::DATA_FLOAT64_QUAD},
    {TEXT_DS,   &Assembler::allocateSpaces,      Assembler::DATA_BYTE},
    {TEXT_DT,   &Assembler::defineFloatConstant, Assembler::DATA_FLOAT80_BCD},
    {TEXT_DW,   &Assembler::defineDataConstant,  Assembler::DATA_WORD},
    {TEXT_RESB, &Assembler::allocateSpaces,      Assembler::DATA_BYTE},
    {TEXT_RESD, &Assembler::allocateSpaces,      Assembler::DATA_LONG},
    {TEXT_RESW, &Assembler::allocateSpaces,      Assembler::DATA_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmI8086::defaultPlugins() {
    static struct final : ValueParser::Plugins {
        const NumberParser &number() const { return IntelNumberParser::singleton(); }
        const SymbolParser &symbol() const { return _symbol; }
        const OperatorParser &operators() const { return IntelOperatorParser::singleton(); }
        const SimpleSymbolParser _symbol{PSTR_UNDER_AT_QUESTION};
    } PLUGINS;
    return PLUGINS;
}

AsmI8086::AsmI8086(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_optimizeSegment),
      Config(TABLE),
      _opt_optimizeSegment(this, &AsmI8086::setOptimizeSegment, OPT_BOOL_OPTIMIZE_SEGMENT,
              OPT_DESC_OPTIMIZE_SEGMENT) {
    reset();
}

void AsmI8086::reset() {
    Assembler::reset();
    setFpuType(FPU_NONE);
    setOptimizeSegment(false);
}

Error AsmI8086::setFpu(StrScanner &scan) {
    auto p = scan;
    p.iexpect('i');
    if (scan.expectTrue() || p.iequals_P(TEXT_FPU_8087)) {
        setFpuType(FPU_I8087);
    } else if (scan.expectFalse() || scan.iequals_P(TEXT_none)) {
        setFpuType(FPU_NONE);
    } else {
        return UNKNOWN_OPERAND;
    }
    return OK;
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
    if (TABLE.searchName(_cpuSpec, insn))
        return UNKNOWN_INSTRUCTION;
    if (!insn.stringInst())
        return UNKNOWN_INSTRUCTION;
    scan = p.skipSpaces();
    op.val32 = insn.opCode();
    op.mode = M_ISTR;
    return OK;
}

Error AsmI8086::parsePointerSize(StrScanner &scan, Operand &op) const {
    auto p = scan;
    const auto reg = parseRegName(p);
    if (reg > REG_PTR) {
        // Pointer size override
        if (parseRegName(p.skipSpaces()) == REG_PTR) {
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
    const auto reg = parseRegName(p);
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
    const auto reg = parseRegName(p);
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
    const auto reg = parseRegName(p);
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
    op.val32 = parseExpr32(p, op);
    if (op.hasError())
        return op.getError();
    op.hasVal = true;
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
    return M_FMOD;
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
            if (!op.hasVal)
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
    const auto reg = parseRegName(a);
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
            op.val32 = parseExpr32(a, op, ')');
            if (op.hasError())
                return op.getError();
            if (a.skipSpaces().expect(')')) {
                if (op.val32 >= 8)
                    op.setErrorIf(valp, OVERFLOW_RANGE);
                op.mode = (op.isOK() && op.val32 == 0) ? M_ST0 : M_STI;
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
    op.val32 = parseExpr32(p, op);
    if (op.hasError())
        return op.getError();
    if (p.skipSpaces().expect(':')) {
        if (op.val32 >= 0x10000UL)
            return op.setError(valp, OVERFLOW_RANGE);
        op.seg16 = op.val32;
        ErrorAt offset;
        offset.setAt(p);
        op.val32 = parseExpr32(p, offset);
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
        return M_IMM;
    if (val32 == 1)
        return M_VAL1;
    if (val32 == 3)
        return M_VAL3;
    if (!ConfigBase::overflowInt8(static_cast<int32_t>(val32)))
        return M_IMM8;
    return M_IMM;
}

void AsmI8086::emitImmediate(AsmInsn &insn, const Operand &op, OprSize size, uint32_t val) const {
    if (size == SZ_BYTE) {
        if (overflowUint8(val))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(val);
    }
    if (size == SZ_WORD) {
        if (overflowUint16(val))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(val);
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
    const auto target = op.getError() ? base : op.val32;
    insn.setErrorIf(op, checkAddr(target, insn.address(), 16));
    const auto delta = branchDelta(base, target, insn, op);
    const auto smartBranch = maySmartBranch(insn.opCode());
    if (mode == M_REL8 && !smartBranch) {
        if (overflowInt8(delta))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
    short_branch:
        insn.emitOperand8(delta);
        return;
    }
    if (mode == M_REL && !smartBranch) {
    long_branch:
        const auto base = insn.address() + 3;
        const auto target = op.getError() ? base : op.val32;
        insn.setErrorIf(op, checkAddr(target, insn.address(), 16));
        const auto delta = branchDelta(base, target, insn, op);
        if (overflowInt16(delta))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand16(delta);
        return;
    }
    if (op.getError() || overflowInt8(delta)) {
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
            (reg == REG_BP && index == REG_UNDEF) || (hasVal && (val32 || getError()));
    if (needDisp) {
        const auto val = static_cast<int32_t>(val32);
        return ConfigBase::overflowInt8(val) || getError() ? 2 : 1;
    }
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
    const Config::opcode_t segPrefix = TABLE.segOverridePrefix(seg);
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
        emitDirect(insn, op, pos);
        break;
    case M_FMOD:
    case M_BMEM:
    case M_WMEM:
    case M_MEM:
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
            if (overflowInt8(static_cast<int32_t>(op.val32)))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand8(op.val32);
        } else if (mod == 2) {
            if (overflowUint16(static_cast<int32_t>(op.val32)))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand16(op.val32);
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
    emitImmediate(insn, op, SZ_WORD, op.val32);
}

void AsmI8086::emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op, OprPos pos) const {
    insn.setErrorIf(op);
    int32_t sval32 = static_cast<int32_t>(op.val32);
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
    case M_FMOD:
        if (op.reg == REG_UNDEF && op.index == REG_UNDEF) {
            emitDirect(insn, op, pos);
            break;
        }
        /* Fall-through */
    case M_BMOD:
    case M_WMOD:
    case M_BMEM:
    case M_WMEM:
        emitModReg(insn, op, pos);
        break;
    case M_BDIR:
    case M_WDIR:
        emitDirect(insn, op, P_OPR);
        break;
    case M_UI16:
        if (sval32 >= 0x10000 || sval32 < 0)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val32);
        break;
    case M_UI8:
        if (sval32 >= 0x100 || sval32 < 0)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val32);
        break;
    case M_BIT:
        if (sval32 >= 16 || sval32 < 0) {
            insn.setErrorIf(op, OVERFLOW_RANGE);
            sval32 &= 0x0F;
        }
        if (insn.size() == SZ_BYTE && sval32 >= 8) {
            insn.setErrorIf(op, OVERFLOW_RANGE);
            sval32 &= 7;
        }
        insn.emitOperand8(sval32);
        break;
    case M_IMM:
        emitImmediate(insn, op, insn.size(), op.val32);
        break;
    case M_IOA:
        if (op.val32 >= 0x100)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        /* Fall-through */
    case M_IMM8:
        emitImmediate(insn, op, SZ_BYTE, op.val32);
        break;
    case M_REL:
    case M_REL8:
        emitRelative(insn, op, mode);
        break;
    case M_FAR:
        if (op.val32 >= 0x10000)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        emitImmediate(insn, op, SZ_WORD, op.val32);
        emitImmediate(insn, op, SZ_WORD, op.seg16);
        break;
    case M_ISTR:
        insn.emitOperand8(op.val32);
        break;
    case M_STI:
        insn.embed(op.val32 & 7);
        break;
    default:
        break;
    }
}

void AsmI8086::emitStringOperand(
        AsmInsn &insn, const Operand &op, RegName seg, RegName index) const {
    if (op.mode == M_NONE)
        return;
    if (op.reg != REG_UNDEF || op.index != index || op.hasVal)
        insn.setErrorIf(op, ILLEGAL_OPERAND);
    if (seg == REG_ES && op.seg != REG_ES)
        insn.setErrorIf(op, ILLEGAL_SEGMENT);
    if (seg == REG_DS && op.seg != REG_UNDEF && op.seg != REG_DS)
        insn.setSegment(TABLE.segOverridePrefix(op.seg));
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

Error AsmI8086::processPseudo(StrScanner &scan, Insn &insn) {
    const auto at = scan;
    if (strcasecmp_P(insn.name(), TEXT_FPU) == 0) {
        const auto error = _opt_fpu.set(scan);
        return error ? insn.setErrorIf(at, error) : OK;
    }
    return Assembler::processPseudo(scan, insn);
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

    if (_insn.setErrorIf(insn.dstOp, TABLE.searchName(_cpuSpec, insn)))
        return _insn.getError();
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
