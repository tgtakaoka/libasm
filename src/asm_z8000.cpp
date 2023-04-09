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

#include "asm_z8000.h"

#include "reg_z8000.h"
#include "table_z8000.h"

namespace libasm {
namespace z8000 {

using namespace reg;

static const char OPT_BOOL_SHORT_DIRECT[] PROGMEM = "short-direct";
static const char OPT_DESC_SHORT_DIRECT[] PROGMEM = "enable optimizing direct addressing";

struct AsmZ8000::Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;     // M_R/M_IR/M_X/M_BX/M_CTL
    RegName base;    // M_BA/M_BX
    CcName cc;       // M_CC/M_DA/M_X
    uint32_t val32;  // M_IM/M_DA/M_X/M_BA/M_INTT/M_FLAG
    Operand() : mode(M_NONE), reg(REG_UNDEF), base(REG_UNDEF), cc(CC_UNDEF), val32(0) {}
};

AsmZ8000::AsmZ8000()
    : Assembler(&_opt_shortDitrect, _number, _comment, _symbol, _letter, _location),
      Config(TABLE),
      _opt_shortDitrect(
              this, &AsmZ8000::setShortDirect, OPT_BOOL_SHORT_DIRECT, OPT_DESC_SHORT_DIRECT) {
    reset();
}

void AsmZ8000::reset() {
    Assembler::reset();
    setShortDirect(false);
}

Error AsmZ8000::setShortDirect(bool enable) {
    _autoShortDirect = enable;
    return OK;
}

void AsmZ8000::emitData(InsnZ8000 &insn, ModeField field, Config::opcode_t data) {
    data &= 0xF;
    if (field == MF_C4 || field == MF_P4)
        data <<= 4;
    if (field == MF_C8 || field == MF_P8)
        data <<= 8;
    if (field == MF_C0 || field == MF_C4 || field == MF_C8)
        insn.embed(data);
    if (field == MF_P0 || field == MF_P4 || field == MF_P8)
        insn.embedPost(data);
}

void AsmZ8000::emitRegister(InsnZ8000 &insn, ModeField field, RegName reg) {
    emitData(insn, field, encodeGeneralRegName(reg));
}

void AsmZ8000::emitIndirectRegister(
        InsnZ8000 &insn, const Operand &op, ModeField field, RegName reg) {
    if (segmentedModel()) {
        if (!isLongReg(reg))
            setErrorIf(op, REGISTER_NOT_ALLOWED);
    } else {
        if (!isWordReg(reg))
            setErrorIf(op, REGISTER_NOT_ALLOWED);
    }
    const uint8_t data = encodeGeneralRegName(reg);
    if (data == 0)
        setErrorIf(op, REGISTER_NOT_ALLOWED);
    emitData(insn, field, data);
}

void AsmZ8000::emitImmediate(InsnZ8000 &insn, ModeField field, AddrMode mode, const Operand &op) {
    if (mode == M_IM8) {
        if (overflowUint8(op.val32))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.embed(static_cast<uint8_t>(op.val32));
        return;
    }
    if (mode == M_BIT) {
        if (op.val32 >= (insn.size() == SZ_BYTE ? 8 : 16))
            setErrorIf(op, ILLEGAL_BIT_NUMBER);
        emitData(insn, field, op.val32);
        return;
    }
    if (mode == M_CNT) {
        if (op.val32 > 16)
            setErrorIf(op, OVERFLOW_RANGE);
        const Config::opcode_t count = op.getError() ? 0 : static_cast<uint8_t>(op.val32) - 1;
        emitData(insn, field, count);
        return;
    }
    if (mode == M_QCNT) {
        if (op.val32 == 1 || op.getError())
            return;
        if (op.val32 == 0)
            setErrorIf(op, OPERAND_NOT_ALLOWED);
        if (op.val32 == 2) {
            emitData(insn, field, 2);
            return;
        }
        setErrorIf(op, OVERFLOW_RANGE);
        return;
    }
    if (mode == M_SCNT || mode == M_NCNT) {
        if (op.val32 > 32)
            setErrorIf(op, OVERFLOW_RANGE);
        auto count = static_cast<int16_t>(op.val32);
        const auto size = insn.size();
        if (count < 0 || (size == SZ_BYTE && count > 8) || (size == SZ_WORD && op.val32 > 16))
            setErrorIf(op, OVERFLOW_RANGE);
        if (mode == M_NCNT)
            count = -count;
        if (size == SZ_BYTE)
            count = static_cast<uint8_t>(count);
        insn.emitOperand16(static_cast<uint16_t>(count));
        return;
    }
    // M_IM
    switch (insn.size()) {
    case SZ_BYTE:
        if (overflowUint8(op.val32))
            setErrorIf(op, OVERFLOW_RANGE);
        {
            uint16_t val16 = static_cast<uint8_t>(op.val32);
            val16 |= (val16 << 8);
            insn.emitOperand16(val16);
        }
        return;
    case SZ_WORD:
        if (overflowUint16(op.val32))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(static_cast<uint16_t>(op.val32));
        return;
    case SZ_LONG:
        insn.emitOperand32(op.val32);
        return;
    default:
        break;
    }
}

void AsmZ8000::emitDirectAddress(InsnZ8000 &insn, const Operand &op) {
    const uint32_t addr = op.val32;
    if (segmentedModel()) {
        if (addr >= 0x800000L)
            setErrorIf(op, OVERFLOW_RANGE);
        const uint16_t seg = (addr >> 8) & 0x7F00;
        const uint16_t off = static_cast<uint16_t>(addr);
        bool autoShortDirect = _autoShortDirect && op.isOK();
        if (off < 0x100 && autoShortDirect) {
            insn.emitOperand16(seg | off);
        } else if (op.cc == CC_F) {  // short direct
            if (off >= 0x100)
                setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand16(seg | (off & 0xFF));
        } else {
            insn.emitOperand16(0x8000 | seg);
            insn.emitOperand16(off);
        }
        return;
    }
    if (addr >= 0x10000)
        setErrorIf(op, OVERFLOW_RANGE);
    insn.emitOperand16(static_cast<uint16_t>(addr));
}

void AsmZ8000::emitRelative(InsnZ8000 &insn, AddrMode mode, const Operand &op) {
    const auto base = insn.address() + (mode == M_RA ? 4 : 2);
    const auto target = op.getError() ? base : op.val32;
    if (mode == M_RA) {
        const auto delta = target - base;
        if (overflowInt16(delta))
            setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand16(static_cast<uint16_t>(delta));
        return;
    }
    auto delta = branchDelta(base, target, op) / 2;
    if (mode == M_RA12) {
        delta = -delta;
        if (overflowInt(delta, 12))
            setErrorIf(op, OPERAND_TOO_FAR);
        insn.embed(static_cast<uint16_t>(delta & 0xFFF));
        return;
    }
    if (mode == M_RA8) {
        if (overflowInt8(delta))
            setErrorIf(op, OPERAND_TOO_FAR);
        insn.embed(static_cast<uint16_t>(delta & 0xFF));
        return;
    }
    // M_RA7
    if (delta <= -0x80 || delta > 0)
        setErrorIf(op, OPERAND_TOO_FAR);
    insn.embed(static_cast<uint8_t>(-delta & 0x7F));
}

void AsmZ8000::emitIndexed(InsnZ8000 &insn, ModeField field, const Operand &op) {
    if (!isWordReg(op.reg) || op.reg == REG_R0)
        setErrorIf(op, REGISTER_NOT_ALLOWED);
    emitRegister(insn, field, op.reg);
    emitDirectAddress(insn, op);
}

void AsmZ8000::emitBaseAddress(InsnZ8000 &insn, ModeField field, const Operand &op) {
    const int32_t disp = static_cast<int32_t>(op.val32);
    if (overflowInt16(disp))
        setErrorIf(op, OVERFLOW_RANGE);
    emitIndirectRegister(insn, op, field, op.base);
    insn.emitOperand16(static_cast<uint16_t>(disp));
}

void AsmZ8000::emitBaseIndexed(InsnZ8000 &insn, ModeField field, const Operand &op) {
    if (!isWordReg(op.reg) || encodeGeneralRegName(op.reg) == 0)
        setErrorIf(op, REGISTER_NOT_ALLOWED);
    emitIndirectRegister(insn, op, field, op.base);
    emitRegister(insn, MF_P8, op.reg);
}

void AsmZ8000::emitFlags(InsnZ8000 &insn, ModeField field, const Operand &op) {
    if (op.mode == M_CC) {
        if (op.cc == CC_Z) {
            emitData(insn, field, FLAG_Z);
        } else if (op.cc == CC_C) {
            emitData(insn, field, FLAG_C);
        } else {
            setErrorIf(op, UNKNOWN_OPERAND);
        }
        return;
    }
    if (op.val32 == 0)
        setErrorIf(op, OPCODE_HAS_NO_EFFECT);
    emitData(insn, field, static_cast<uint16_t>(op.val32));
}

void AsmZ8000::emitCtlRegister(InsnZ8000 &insn, ModeField field, const Operand &op) {
    if (insn.size() == SZ_BYTE && op.reg != REG_FLAGS)
        setErrorIf(op, ILLEGAL_SIZE);
    if (insn.size() == SZ_WORD && op.reg == REG_FLAGS)
        setErrorIf(op, ILLEGAL_SIZE);
    int8_t data = encodeCtlReg(segmentedModel(),op.reg);
    if (data < 0) {
        setErrorIf(op, ILLEGAL_REGISTER);
        data = 0;
    }
    emitData(insn, field, data);
}

void AsmZ8000::emitOperand(InsnZ8000 &insn, AddrMode mode, const Operand &op, ModeField field) {
    switch (mode) {
    case M_DR:
        if (insn.size() == SZ_WORD && !isLongReg(op.reg))
            setErrorIf(op, REGISTER_NOT_ALLOWED);
        if (insn.size() == SZ_LONG && !isQuadReg(op.reg))
            setErrorIf(op, REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case M_R:
        emitRegister(insn, field, op.reg);
        return;
    case M_WR07:
        if (encodeGeneralRegName(op.reg) >= 8)
            setErrorIf(op, REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case M_WR:
        if (!isWordReg(op.reg))
            setErrorIf(op, REGISTER_NOT_ALLOWED);
        emitRegister(insn, field, op.reg);
        return;
    case M_IR:
        emitIndirectRegister(insn, op, field, op.reg);
        return;
    case M_IRIO:
        if (!isWordReg(op.reg) || encodeGeneralRegName(op.reg) == 0)
            setErrorIf(op, REGISTER_NOT_ALLOWED);
        emitRegister(insn, field, op.reg);
        return;
    case M_GENI:
        if (op.mode == M_IM) {
            emitImmediate(insn, MF_NO, M_IM, op);
            return;
        }
        /* Fall-through */
    case M_GEND:
        if (op.mode == M_R) {
            insn.embed(0x8000);
            emitRegister(insn, field, op.reg);
            return;
        }
        /* Fall-through */
    case M_GENA:
        if (op.mode == M_IR) {
            emitIndirectRegister(insn, op, field, op.reg);
            return;
        }
        insn.embed(0x4000);
        if (op.mode == M_DA) {
            emitDirectAddress(insn, op);
            return;
        }
        emitIndexed(insn, field, op);
        return;
    case M_BA:
        emitBaseAddress(insn, field, op);
        return;
    case M_BX:
        emitBaseIndexed(insn, field, op);
        return;
    case M_IO:
        if (op.val32 >= 0x10000)
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(static_cast<uint16_t>(op.val32));
        return;
    case M_IM:
    case M_IM8:
    case M_BIT:
    case M_CNT:
    case M_QCNT:
    case M_SCNT:
    case M_NCNT:
        emitImmediate(insn, field, mode, op);
        return;
    case M_CC:
        if (op.mode == M_NONE) {
            emitData(insn, field, CC_T);
            return;
        }
        emitData(insn, field, encodeCcName(op.cc));
        return;
    case M_INTR:
        if (op.val32 == 0)
            setErrorIf(op, OPCODE_HAS_NO_EFFECT);
        emitData(insn, field, static_cast<uint16_t>(3 ^ op.val32));
        return;
    case M_FLAG:
        emitFlags(insn, field, op);
        return;
    case M_RA:
    case M_RA12:
    case M_RA8:
    case M_RA7:
        emitRelative(insn, mode, op);
        return;
    case M_CTL:
        emitCtlRegister(insn, field, op);
        return;
    default:
        break;
    }
}

void AsmZ8000::checkRegisterOverlap(const Operand &dstOp, const Operand &srcOp, RegName cnt) {
    const auto dst = dstOp.reg;
    const auto src = srcOp.reg;
    const uint8_t dnum = encodeGeneralRegName(dst);
    const uint8_t ds = isByteReg(dst) && dnum >= 8 ? dnum - 8 : dnum;
    const uint8_t de = isLongReg(dst) ? ds + 1 : ds;
    const uint8_t ss = encodeGeneralRegName(src);
    const uint8_t se = isLongReg(src) ? ss + 1 : ss;
    if (ds == ss || ds == se || de == ss || de == se)
        setErrorIf(dstOp, REGISTERS_OVERLAPPED);
    if (cnt == REG_UNDEF)
        return;
    const uint8_t c = encodeGeneralRegName(cnt);
    if (ds == c || de == c)
        setErrorIf(dstOp, REGISTERS_OVERLAPPED);
    if (ss == c || se == c)
        setErrorIf(srcOp, REGISTERS_OVERLAPPED);
}

void AsmZ8000::checkRegisterOverlap(
        const InsnZ8000 &insn, const Operand &dstOp, const Operand &srcOp, const Operand &cntOp) {
    if (dstOp.mode == M_IR && (dstOp.reg == REG_R0 || dstOp.reg == REG_RR0))
        setErrorIf(dstOp, REGISTER_NOT_ALLOWED);
    if (srcOp.mode == M_IR && (srcOp.reg == REG_R0 || srcOp.reg == REG_RR0))
        setErrorIf(srcOp, REGISTER_NOT_ALLOWED);
    checkRegisterOverlap(dstOp, srcOp, cntOp.reg);
    if (insn.isTranslateInsn()) {
        // @R1 isn't allowed as dst/src.
        if (!segmentedModel()) {
            if (dstOp.reg == REG_R1)
                setErrorIf(dstOp, REGISTER_NOT_ALLOWED);
            if (srcOp.reg == REG_R1)
                setErrorIf(srcOp, REGISTER_NOT_ALLOWED);
        }
        // R1 isn't allowed as cnt.
        if (cntOp.reg == REG_R1)
            setErrorIf(cntOp, REGISTER_NOT_ALLOWED);
    }
}

int8_t AsmZ8000::parseIntrNames(StrScanner &scan) const {
    auto p = scan;
    if (endOfLine(p))
        return 0;
    int8_t num = 0;
    while (true) {
        const auto intr = parseIntrName(p);
        if (intr == INTR_UNDEF)
            return -1;
        num |= encodeIntrName(intr);
        if (endOfLine(p.skipSpaces())) {
            scan = p;
            return num;
        }
        if (!p.expect(','))
            return -1;
        p.skipSpaces();
    }
}

int8_t AsmZ8000::parseFlagNames(StrScanner &scan) const {
    auto p = scan;
    if (endOfLine(p))
        return 0;
    int8_t num = 0;
    while (true) {
        const auto flag = parseFlagName(p);
        if (flag == FLAG_UNDEF)
            return -1;
        num |= encodeFlagName(flag);
        if (endOfLine(p.skipSpaces())) {
            scan = p;
            return num;
        }
        if (!p.expect(','))
            return -1;
        p.skipSpaces();
    }
}

Error AsmZ8000::parseOperand(StrScanner &scan, Operand &op) {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (p.expect('#')) {
        op.val32 = parseExpr32(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_IM;
        scan = p;
        return OK;
    }
    if (endOfLine(p))
        return OK;

    if (p.expect('@')) {
        op.reg = parseRegName(p);
        if (op.reg == REG_UNDEF)
            return op.setError(scan, UNKNOWN_REGISTER);
        if (op.reg == REG_ILLEGAL)
            return op.setError(scan, ILLEGAL_REGISTER);
        op.mode = M_IR;
        scan = p;
        return OK;
    }

    op.reg = parseRegName(p);
    if (op.reg != REG_UNDEF) {
        if (op.reg == REG_ILLEGAL)
            return op.setError(scan, ILLEGAL_REGISTER);
        if (p.skipSpaces().expect('(')) {
            op.base = op.reg;
            if (p.skipSpaces().expect('#')) {
                op.val32 = parseExpr32(p, op, ')');
                if (op.hasError())
                    return op.getError();
                if (!p.skipSpaces().expect(')'))
                    return op.setError(p, MISSING_CLOSING_PAREN);
                op.mode = M_BA;
                scan = p;
                return OK;
            }
            op.reg = parseRegName(p);
            if (op.reg != REG_UNDEF) {
                if (!p.skipSpaces().expect(')'))
                    return op.setError(p, MISSING_CLOSING_PAREN);
                op.mode = M_BX;
                scan = p;
                return OK;
            }
            return op.setError(UNKNOWN_OPERAND);
        }
        op.mode = M_R;
        scan = p;
        return OK;
    }
    op.reg = parseCtlReg(p);
    if (op.reg != REG_UNDEF) {
        op.mode = M_CTL;
        scan = p;
        return OK;
    }
    op.cc = parseCcName(p);
    if (op.cc != CC_UNDEF) {
        // 'C' and 'Z' are parsed as M_CC, though these can be M_FLAG.
        auto a = scan;
        const int8_t num = parseFlagNames(a);
        const auto flag = FlagName(num);
        if (num > 0 && flag != FLAG_C && flag != FLAG_Z) {
            op.val32 = num;
            op.mode = M_FLAG;
            scan = a;
            return OK;
        }
        op.mode = M_CC;
        scan = p;
        return OK;
    }
    int8_t num = parseFlagNames(p);
    if (num >= 0) {
        op.val32 = num;
        op.mode = M_FLAG;
        scan = p;
        return OK;
    }
    num = parseIntrNames(p);
    if (num >= 0) {
        op.val32 = num;
        op.mode = M_INTR;
        scan = p;
        return OK;
    }
    if (p.expect('|')) {
        op.val32 = parseExpr(p, op, '|').getUnsigned();
        if (p.expect('|')) {
            op.cc = CC_F;  // short direct
        } else {
            op.setError(scan, UNKNOWN_OPERAND);
        }
    } else {
        op.val32 = parseExpr32(p, op);
    }
    if (op.hasError())
        return op.getError();
    if (p.skipSpaces().expect('(')) {
        op.reg = parseRegName(p.skipSpaces());
        if (op.reg == REG_UNDEF)
            return op.setError(UNKNOWN_OPERAND);
        if (!p.skipSpaces().expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        op.mode = M_X;
        scan = p;
        return OK;
    }
    op.mode = M_DA;
    scan = p;
    return OK;
}

Error AsmZ8000::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnZ8000 insn(_insn);
    Operand dstOp, srcOp, ex1Op, ex2Op;
    if (parseOperand(scan, dstOp) && dstOp.hasError())
        return setError(dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, srcOp) && srcOp.hasError())
            return setError(srcOp);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, ex1Op) && ex1Op.hasError())
            return setError(ex1Op);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, ex2Op) && ex2Op.hasError())
            return setError(ex2Op);
        scan.skipSpaces();
    }
    if (!endOfLine(scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(dstOp);
    setErrorIf(srcOp);
    setErrorIf(ex1Op);
    setErrorIf(ex2Op);

    insn.setAddrMode(dstOp.mode, srcOp.mode, ex1Op.mode, ex2Op.mode);
    const auto error = TABLE.searchName(cpuType(), insn);
    if (error)
        return setError(dstOp, error);

    if (insn.isThreeRegsInsn())
        checkRegisterOverlap(insn, dstOp, srcOp, ex1Op);
    if (insn.isPushPopInsn())
        checkRegisterOverlap(dstOp, srcOp);
    if (insn.isLoadMultiInsn()) {
        const auto &regOp = insn.dst() == M_R ? dstOp : srcOp;
        if (encodeGeneralRegName(regOp.reg) + ex1Op.val32 > 16)
            setErrorIf(ex1Op, OVERFLOW_RANGE);
    }

    emitOperand(insn, insn.dst(), dstOp, insn.dstField());
    emitOperand(insn, insn.src(), srcOp, insn.srcField());
    const auto ex1Field = (insn.ex1() == M_WR ? MF_P8 : MF_P0);
    emitOperand(insn, insn.ex1(), ex1Op, ex1Field);
    emitOperand(insn, insn.ex2(), ex2Op, MF_P0);
    insn.emitInsn();
    return setErrorIf(insn);
}

}  // namespace z8000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
