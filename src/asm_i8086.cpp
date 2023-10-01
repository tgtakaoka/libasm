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

#include "operators.h"
#include "table_i8086.h"
#include "text_common.h"

namespace libasm {
namespace i8086 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

const char OPT_BOOL_OPTIMIZE_SEGMENT[] PROGMEM = "optimize-segment";
const char OPT_DESC_OPTIMIZE_SEGMENT[] PROGMEM = "enable optimizing segment override";

const char TEXT_RESB[] PROGMEM = "resb";
const char TEXT_RESD[] PROGMEM = "resd";
const char TEXT_RESW[] PROGMEM = "resw";

constexpr Pseudo PSEUDOS[] PROGMEM = {
        Pseudo{TEXT_DB, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
        Pseudo{TEXT_DD, &Assembler::defineDataConstant, Assembler::DATA_LONG},
        Pseudo{TEXT_DS, &Assembler::allocateSpaces, Assembler::DATA_BYTE},
        Pseudo{TEXT_DW, &Assembler::defineDataConstant, Assembler::DATA_WORD},
        Pseudo{TEXT_RESB, &Assembler::allocateSpaces, Assembler::DATA_BYTE},
        Pseudo{TEXT_RESD, &Assembler::allocateSpaces, Assembler::DATA_LONG},
        Pseudo{TEXT_RESW, &Assembler::allocateSpaces, Assembler::DATA_WORD},
};

}  // namespace

struct AsmI8086::Operand final : ErrorAt {
    AddrMode mode;
    RegName ptr;
    RegName seg;
    RegName reg;
    RegName index;
    bool hasVal;
    uint32_t val32;
    uint16_t seg16;
    Operand()
        : mode(M_NONE),
          ptr(REG_UNDEF),
          seg(REG_UNDEF),
          reg(REG_UNDEF),
          index(REG_UNDEF),
          hasVal(false),
          val32(0),
          seg16(0) {}
    uint8_t encodeMod() const;
    uint8_t encodeR_m() const;
    AddrMode immediateMode() const;
    void print(const char *) const;
};

const ValueParser::Plugins &AsmI8086::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return IntelNumberParser::singleton(); }
        const SymbolParser &symbol() const override { return _symbol; }
        const SimpleSymbolParser _symbol{SymbolParser::ATMARK_QUESTION};
        const OperatorParser &operators() const override {
            return IntelOperatorParser::singleton();
        }
    } PLUGINS{};
    return PLUGINS;
}

AsmI8086::AsmI8086(const ValueParser::Plugins &plugins)
    : Assembler(plugins, ARRAY_RANGE(PSEUDOS), &_opt_optimizeSegment),
      Config(TABLE),
      _opt_optimizeSegment(this, &AsmI8086::setOptimizeSegment, OPT_BOOL_OPTIMIZE_SEGMENT,
              OPT_DESC_OPTIMIZE_SEGMENT) {
    reset();
}

void AsmI8086::reset() {
    Assembler::reset();
    setOptimizeSegment(false);
}

Error AsmI8086::setOptimizeSegment(bool enable) {
    _optimizeSegment = enable;
    return OK;
}

Error AsmI8086::parseStringInst(StrScanner &scan, Operand &op) const {
    Insn _insn(0);
    AsmInsn insn(_insn);
    auto p = scan;
    StrScanner opr;
    if (parser().readSymbol(p, opr) != OK)
        return UNKNOWN_INSTRUCTION;
    insn.nameBuffer().text(opr);
    insn.setAddrMode(M_NONE, M_NONE, M_NONE);
    if (TABLE.searchName(cpuType(), insn))
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
    if (reg == REG_BYTE || reg == REG_WORD) {
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
            return getError();
        if (!p.skipSpaces().expect(']'))
            return op.setError(p, MISSING_CLOSING_BRACKET);
        scan = p;
        if (op.reg == REG_UNDEF && op.index == REG_UNDEF) {
            if (!op.hasVal)
                return op.setError(UNKNOWN_OPERAND);
            op.mode = (op.ptr == REG_UNDEF) ? M_DIR : (op.ptr == REG_BYTE ? M_BDIR : M_WDIR);
            return OK;
        }
        op.mode = (op.ptr == REG_UNDEF) ? M_MEM : (op.ptr == REG_BYTE ? M_BMEM : M_WMEM);
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
    if (reg != REG_UNDEF)
        return op.setError(UNKNOWN_OPERAND);

    const auto valp = p.skipSpaces();
    ;
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

AddrMode AsmI8086::Operand::immediateMode() const {
    if (getError())
        return M_IMM;
    if (val32 == 1)
        return M_VAL1;
    if (val32 == 3)
        return M_VAL3;
    if (!overflowInt8(static_cast<int32_t>(val32)))
        return M_IMM8;
    return M_IMM;
}

void AsmI8086::emitImmediate(AsmInsn &insn, const Operand &op, OprSize size, uint32_t val) {
    if (size == SZ_BYTE) {
        if (overflowUint8(val))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(val);
    }
    if (size == SZ_WORD) {
        if (overflowUint16(val))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(val);
    }
}

void AsmI8086::emitRelative(AsmInsn &insn, const Operand &op, AddrMode mode) {
    const auto base = insn.address() + (mode == M_REL8 ? 2 : 3);
    const auto target = op.getError() ? base : op.val32;
    const auto delta = branchDelta(base, target, op);
    if (mode == M_REL8) {
        const auto overflow = overflowInt8(delta);
        if (insn.opCode() == 0xEB && (overflow || op.getError())) {
            insn.setOpCode(0xE9, 0);
            emitRelative(insn, op, M_REL);
            return;
        }
        if (overflow)
            setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
        return;
    }
    // M_REL
    if (overflowInt16(delta))
        setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand16(delta);
}

void AsmI8086::emitRegister(AsmInsn &insn, const Operand &op, OprPos pos) {
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

uint8_t AsmI8086::Operand::encodeMod() const {
    const auto needDisp =
            (reg == REG_BP && index == REG_UNDEF) || (hasVal && (val32 || getError()));
    if (needDisp) {
        const auto val = static_cast<int32_t>(val32);
        return overflowInt8(val) || getError() ? 2 : 1;
    }
    return 0;
}

uint8_t AsmI8086::Operand::encodeR_m() const {
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

Config::opcode_t AsmI8086::encodeSegmentOverride(RegName seg, RegName base) {
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

void AsmI8086::emitModReg(AsmInsn &insn, const Operand &op, OprPos pos) {
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
                setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand8(op.val32);
        } else if (mod == 2) {
            if (overflowUint16(static_cast<int32_t>(op.val32)))
                setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand16(op.val32);
        }
        break;
    default:
        break;
    }
}

void AsmI8086::emitDirect(AsmInsn &insn, const Operand &op, OprPos pos) {
    insn.setSegment(encodeSegmentOverride(op.seg, REG_UNDEF));
    if (pos == P_MOD)
        insn.embedModReg(0006);
    if (pos == P_OMOD)
        insn.embed(0006);
    emitImmediate(insn, op, SZ_WORD, op.val32);
}

void AsmI8086::emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op, OprPos pos) {
    int32_t sval32 = static_cast<int32_t>(op.val32);
    switch (mode) {
    case M_CS:
        if (pos == P_NONE)  // POP CS
            setErrorIf(op, REGISTER_NOT_ALLOWED);
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
        emitModReg(insn, op, pos);
        break;
    case M_BDIR:
    case M_WDIR:
        emitDirect(insn, op, P_OPR);
        break;
    case M_UI16:
        if (sval32 >= 0x10000 || sval32 < 0)
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val32);
        break;
    case M_UI8:
        if (sval32 >= 0x100 || sval32 < 0)
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val32);
        break;
    case M_BIT:
        if (sval32 >= 16 || sval32 < 0) {
            setErrorIf(op, OVERFLOW_RANGE);
            sval32 &= 0x0F;
        }
        if (insn.size() == SZ_BYTE && sval32 >= 8) {
            setErrorIf(op, OVERFLOW_RANGE);
            sval32 &= 7;
        }
        insn.emitOperand8(sval32);
        break;
    case M_IMM:
        emitImmediate(insn, op, insn.size(), op.val32);
        break;
    case M_IOA:
        if (op.val32 >= 0x100)
            setErrorIf(op, OVERFLOW_RANGE);
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
            setErrorIf(op, OVERFLOW_RANGE);
        emitImmediate(insn, op, SZ_WORD, op.val32);
        emitImmediate(insn, op, SZ_WORD, op.seg16);
        break;
    case M_ISTR:
        insn.emitOperand8(op.val32);
        break;
    default:
        break;
    }
}

void AsmI8086::emitStringOperand(AsmInsn &insn, const Operand &op, RegName seg, RegName index) {
    if (op.mode == M_NONE)
        return;
    if (op.reg != REG_UNDEF || op.index != index || op.hasVal)
        setErrorIf(op, ILLEGAL_OPERAND);
    if (seg == REG_ES && op.seg != REG_ES)
        setErrorIf(op, ILLEGAL_SEGMENT);
    if (seg == REG_DS && op.seg != REG_UNDEF && op.seg != REG_DS)
        insn.setSegment(TABLE.segOverridePrefix(op.seg));
}

void AsmI8086::emitStringInst(AsmInsn &insn, const Operand &dst, const Operand &src) {
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

Error AsmI8086::encodeImpl(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    Operand dstOp, srcOp, extOp;
    if (parseOperand(scan, dstOp) && dstOp.hasError())
        return setError(dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, srcOp) && srcOp.hasError())
            return setError(srcOp);
        scan.skipSpaces();
    }
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, extOp) && extOp.hasError())
            return setError(extOp);
        scan.skipSpaces();
    }
    if (!endOfLine(scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(dstOp);
    setErrorIf(srcOp);
    setErrorIf(extOp);

    insn.setAddrMode(dstOp.mode, srcOp.mode, extOp.mode);
    const auto error = TABLE.searchName(cpuType(), insn);
    if (error)
        return setError(dstOp, error);
    insn.prepairModReg();

    if (insn.stringInst()) {
        emitStringInst(insn, dstOp, srcOp);
    } else {
        emitOperand(insn, insn.dst(), dstOp, insn.dstPos());
        emitOperand(insn, insn.src(), srcOp, insn.srcPos());
        emitOperand(insn, insn.ext(), extOp, insn.extPos());
    }
    insn.emitInsn();
    return setErrorIf(insn);
}

}  // namespace i8086
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
