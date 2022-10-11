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

namespace libasm {
namespace z8000 {

const char AsmZ8000::OPT_BOOL_SHORT_DIRECT[] PROGMEM = "short-direct";
const char AsmZ8000::OPT_DESC_SHORT_DIRECT[] PROGMEM = "enable optimizing direct addressing";

Error AsmZ8000::emitData(InsnZ8000 &insn, ModeField field, Config::opcode_t data) {
    data &= 0xF;
    if (field == MF_C4 || field == MF_P4)
        data <<= 4;
    if (field == MF_C8 || field == MF_P8)
        data <<= 8;
    if (field == MF_C0 || field == MF_C4 || field == MF_C8)
        insn.embed(data);
    if (field == MF_P0 || field == MF_P4 || field == MF_P8)
        insn.embedPost(data);
    return OK;
}

Error AsmZ8000::emitRegister(InsnZ8000 &insn, ModeField field, RegName reg) {
    return emitData(insn, field, RegZ8000::encodeGeneralRegName(reg));
}

Error AsmZ8000::emitIndirectRegister(
        InsnZ8000 &insn, const Operand &op, ModeField field, RegName reg) {
    if (TableZ8000::TABLE.segmentedModel()) {
        if (!RegZ8000::isLongReg(reg))
            return setError(op, REGISTER_NOT_ALLOWED);
    } else {
        if (!RegZ8000::isWordReg(reg))
            return setError(op, REGISTER_NOT_ALLOWED);
    }
    const uint8_t data = RegZ8000::encodeGeneralRegName(reg);
    if (data == 0)
        return setError(op, REGISTER_NOT_ALLOWED);
    return emitData(insn, field, data);
}

Error AsmZ8000::emitImmediate(InsnZ8000 &insn, ModeField field, AddrMode mode, const Operand &op) {
    if (mode == M_IM8) {
        if (overflowUint8(op.val32))
            return setError(op, OVERFLOW_RANGE);
        const uint8_t val8 = static_cast<uint8_t>(op.val32);
        insn.embed(val8);
        return OK;
    }
    if (mode == M_BIT) {
        if (op.val32 >= (insn.oprSize() == SZ_BYTE ? 8 : 16))
            return setError(op, ILLEGAL_BIT_NUMBER);
        return emitData(insn, field, op.val32);
    }
    if (mode == M_CNT) {
        if (op.val32 > 16)
            return setError(op, OVERFLOW_RANGE);
        const Config::opcode_t count = op.getError() ? 0 : static_cast<uint8_t>(op.val32) - 1;
        return emitData(insn, field, count);
    }
    if (mode == M_QCNT) {
        if (op.val32 == 1 || op.getError())
            return OK;
        if (op.val32 == 0)
            return setError(op, OPERAND_NOT_ALLOWED);
        if (op.val32 == 2)
            return emitData(insn, field, 2);
        return setError(op, OVERFLOW_RANGE);
    }
    if (mode == M_SCNT || mode == M_NCNT) {
        if (op.val32 > 32)
            return setError(op, OVERFLOW_RANGE);
        int16_t count = static_cast<int16_t>(op.val32);
        const OprSize sz = insn.oprSize();
        if (count < 0 || (sz == SZ_BYTE && count > 8) || (sz == SZ_WORD && op.val32 > 16))
            return setError(op, OVERFLOW_RANGE);
        if (mode == M_NCNT)
            count = -count;
        if (sz == SZ_BYTE)
            count = static_cast<uint8_t>(count);
        insn.emitOperand16(static_cast<uint16_t>(count));
        return OK;
    }
    // M_IM
    switch (insn.oprSize()) {
    case SZ_BYTE:
        if (!overflowUint8(op.val32)) {
            uint16_t val16 = static_cast<uint8_t>(op.val32);
            val16 |= (val16 << 8);
            insn.emitOperand16(val16);
            return OK;
        }
        break;
    case SZ_WORD:
        if (!overflowUint16(op.val32)) {
            const uint16_t val16 = static_cast<uint16_t>(op.val32);
            insn.emitOperand16(val16);
            return OK;
        }
        break;
    case SZ_LONG:
        insn.emitOperand32(op.val32);
        return OK;
    default:
        break;
    }
    return setError(op, OVERFLOW_RANGE);
}

Error AsmZ8000::emitDirectAddress(InsnZ8000 &insn, const Operand &op) {
    const uint32_t addr = op.val32;
    if (TableZ8000::TABLE.segmentedModel()) {
        if (addr >= 0x800000L)
            return setError(op, OVERFLOW_RANGE);
        const uint16_t seg = (addr >> 8) & 0x7F00;
        const uint16_t off = static_cast<uint16_t>(addr);
        bool autoShortDirect = _autoShortDirect && op.isOK();
        if (off < 0x100 && autoShortDirect) {
            insn.emitOperand16(seg | off);
        } else if (op.cc == CC_F) {  // short direct
            if (off >= 0x100)
                return setError(op, OVERFLOW_RANGE);
            insn.emitOperand16(seg | off);
        } else {
            insn.emitOperand16(0x8000 | seg);
            insn.emitOperand16(off);
        }
        return OK;
    }
    if (addr >= 0x10000)
        return setError(op, OVERFLOW_RANGE);
    insn.emitOperand16(static_cast<uint16_t>(addr));
    return OK;
}

Error AsmZ8000::emitRelative(InsnZ8000 &insn, AddrMode mode, const Operand &op) {
    const Config::uintptr_t base = insn.address() + (mode == M_RA ? 4 : 2);
    const Config::uintptr_t target = op.getError() ? base : op.val32;
    Config::ptrdiff_t delta = target - base;
    if (mode == M_RA) {
        if (overflowRel16(delta))
            return setError(op, OPERAND_TOO_FAR);
        insn.emitOperand16(static_cast<uint16_t>(delta));
        return OK;
    }
    if (target % 2)
        return setError(op, OPERAND_NOT_ALIGNED);
    delta /= 2;
    if (mode == M_RA12) {
        delta = -delta;
        if (delta < -0x800 || delta >= 0x800)
            return setError(op, OPERAND_TOO_FAR);
        insn.embed(static_cast<uint16_t>(delta & 0xFFF));
        return OK;
    }
    if (mode == M_RA8) {
        if (overflowRel8(delta))
            return setError(op, OPERAND_TOO_FAR);
        insn.embed(static_cast<uint16_t>(delta & 0xFF));
        return OK;
    }
    // M_RA7
    if (delta <= -0x80 || delta > 0)
        return setError(op, OPERAND_TOO_FAR);
    insn.embed(static_cast<uint8_t>(-delta & 0x7F));
    return OK;
}

Error AsmZ8000::emitIndexed(InsnZ8000 &insn, ModeField field, const Operand &op) {
    if (!RegZ8000::isWordReg(op.reg) || op.reg == REG_R0)
        return setError(op, REGISTER_NOT_ALLOWED);
    if (emitRegister(insn, field, op.reg))
        return getError();
    return emitDirectAddress(insn, op);
}

Error AsmZ8000::emitBaseAddress(InsnZ8000 &insn, ModeField field, const Operand &op) {
    const int32_t disp = static_cast<int32_t>(op.val32);
    if (overflowRel16(disp))
        return setError(op, OVERFLOW_RANGE);
    if (emitIndirectRegister(insn, op, field, op.base))
        return getError();
    insn.emitOperand16(static_cast<uint16_t>(disp));
    return OK;
}

Error AsmZ8000::emitBaseIndexed(InsnZ8000 &insn, ModeField field, const Operand &op) {
    if (!RegZ8000::isWordReg(op.reg) || RegZ8000::encodeGeneralRegName(op.reg) == 0)
        return setError(op, REGISTER_NOT_ALLOWED);
    if (emitIndirectRegister(insn, op, field, op.base))
        return getError();
    return emitRegister(insn, MF_P8, op.reg);
}

Error AsmZ8000::emitFlags(InsnZ8000 &insn, ModeField field, const Operand &op) {
    if (op.mode == M_CC) {
        if (op.cc == CC_Z)
            return emitData(insn, field, FLAG_Z);
        if (op.cc == CC_C)
            return emitData(insn, field, FLAG_C);
        return setError(op, UNKNOWN_OPERAND);
    }
    if (op.val32 == 0)
        return setError(op, OPCODE_HAS_NO_EFFECT);
    return emitData(insn, field, static_cast<uint16_t>(op.val32));
}

Error AsmZ8000::emitCtlRegister(InsnZ8000 &insn, ModeField field, const Operand &op) {
    if (insn.oprSize() == SZ_BYTE && op.reg != REG_FLAGS)
        return setError(op, ILLEGAL_SIZE);
    if (insn.oprSize() == SZ_WORD && op.reg == REG_FLAGS)
        return setError(op, ILLEGAL_SIZE);
    const int8_t data = RegZ8000::encodeCtlReg(op.reg);
    if (data < 0)
        return setError(op, ILLEGAL_REGISTER);
    return emitData(insn, field, data);
}

Error AsmZ8000::emitOperand(InsnZ8000 &insn, AddrMode mode, const Operand &op, ModeField field) {
    switch (mode) {
    case M_DR:
        if (insn.oprSize() == SZ_WORD && !RegZ8000::isLongReg(op.reg))
            return setError(op, REGISTER_NOT_ALLOWED);
        if (insn.oprSize() == SZ_LONG && !RegZ8000::isQuadReg(op.reg))
            return setError(op, REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case M_R:
        return emitRegister(insn, field, op.reg);
    case M_WR07:
        if (RegZ8000::encodeGeneralRegName(op.reg) >= 8)
            return setError(op, REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case M_WR:
        if (!RegZ8000::isWordReg(op.reg))
            return setError(op, REGISTER_NOT_ALLOWED);
        return emitRegister(insn, field, op.reg);
    case M_IR:
        return emitIndirectRegister(insn, op, field, op.reg);
    case M_IRIO:
        if (!RegZ8000::isWordReg(op.reg) || RegZ8000::encodeGeneralRegName(op.reg) == 0)
            return setError(op, REGISTER_NOT_ALLOWED);
        return emitRegister(insn, field, op.reg);
    case M_GENI:
        if (op.mode == M_IM)
            return emitImmediate(insn, MF_NO, M_IM, op);
        /* Fall-through */
    case M_GEND:
        if (op.mode == M_R) {
            insn.embed(0x8000);
            return emitRegister(insn, field, op.reg);
        }
        /* Fall-through */
    case M_GENA:
        if (op.mode == M_IR)
            return emitIndirectRegister(insn, op, field, op.reg);
        insn.embed(0x4000);
        if (op.mode == M_DA)
            return emitDirectAddress(insn, op);
        return emitIndexed(insn, field, op);
    case M_BA:
        return emitBaseAddress(insn, field, op);
    case M_BX:
        return emitBaseIndexed(insn, field, op);
    case M_IO:
        if (op.val32 >= 0x10000)
            return setError(op, OVERFLOW_RANGE);
        insn.emitOperand16(static_cast<uint16_t>(op.val32));
        return OK;
    case M_IM:
    case M_IM8:
    case M_BIT:
    case M_CNT:
    case M_QCNT:
    case M_SCNT:
    case M_NCNT:
        return emitImmediate(insn, field, mode, op);
    case M_CC:
        if (op.mode == M_NO)
            return emitData(insn, field, CC_T);
        return emitData(insn, field, RegZ8000::encodeCcName(op.cc));
    case M_INTR:
        if (op.val32 == 0)
            return setError(op, OPCODE_HAS_NO_EFFECT);
        return emitData(insn, field, static_cast<uint16_t>(3 ^ op.val32));
    case M_FLAG:
        return emitFlags(insn, field, op);
    case M_RA:
    case M_RA12:
    case M_RA8:
    case M_RA7:
        return emitRelative(insn, mode, op);
    case M_CTL:
        return emitCtlRegister(insn, field, op);
    default:
        return OK;
    }
}

Error AsmZ8000::checkRegisterOverlap(const Operand &dstOp, const Operand &srcOp, RegName cnt) {
    const RegName dst = dstOp.reg;
    const RegName src = srcOp.reg;
    const uint8_t dnum = RegZ8000::encodeGeneralRegName(dst);
    const uint8_t ds = RegZ8000::isByteReg(dst) && dnum >= 8 ? dnum - 8 : dnum;
    const uint8_t de = RegZ8000::isLongReg(dst) ? ds + 1 : ds;
    const uint8_t ss = RegZ8000::encodeGeneralRegName(src);
    const uint8_t se = RegZ8000::isLongReg(src) ? ss + 1 : ss;
    if (ds == ss || ds == se || de == ss || de == se)
        return setError(dstOp, REGISTERS_OVERLAPPED);
    if (cnt == REG_UNDEF)
        return OK;
    const uint8_t c = RegZ8000::encodeGeneralRegName(cnt);
    if (ds == c || de == c)
        return setError(dstOp, REGISTERS_OVERLAPPED);
    if (ss == c || se == c)
        return setError(srcOp, REGISTERS_OVERLAPPED);
    return OK;
}

Error AsmZ8000::checkRegisterOverlap(
        const InsnZ8000 &insn, const Operand &dstOp, const Operand &srcOp, const Operand &cntOp) {
    if (dstOp.mode == M_IR && (dstOp.reg == REG_R0 || dstOp.reg == REG_RR0))
        return setError(dstOp, REGISTER_NOT_ALLOWED);
    if (srcOp.mode == M_IR && (srcOp.reg == REG_R0 || srcOp.reg == REG_RR0))
        return setError(srcOp, REGISTER_NOT_ALLOWED);
    if (checkRegisterOverlap(dstOp, srcOp, cntOp.reg))
        return getError();
    if (insn.isTranslateInsn()) {
        // @R1 isn't allowed as dst/src.
        if (!TableZ8000::TABLE.segmentedModel()) {
            if (dstOp.reg == REG_R1)
                return setError(dstOp, REGISTER_NOT_ALLOWED);
            if (srcOp.reg == REG_R1)
                return setError(srcOp, REGISTER_NOT_ALLOWED);
        }
        // R1 isn't allowed as cnt.
        if (cntOp.reg == REG_R1)
            return setError(cntOp, REGISTER_NOT_ALLOWED);
    }
    return OK;
}

int8_t AsmZ8000::parseIntrNames(StrScanner &scan) const {
    StrScanner p(scan);
    if (endOfLine(*p))
        return 0;
    int8_t num = 0;
    while (true) {
        const IntrName intr = RegZ8000::parseIntrName(p);
        if (intr == INTR_UNDEF)
            return -1;
        num |= RegZ8000::encodeIntrName(intr);
        if (endOfLine(*p.skipSpaces())) {
            scan = p;
            return num;
        }
        if (!p.expect(','))
            return -1;
        p.skipSpaces();
    }
}

int8_t AsmZ8000::parseFlagNames(StrScanner &scan) const {
    StrScanner p(scan);
    if (endOfLine(*p))
        return 0;
    int8_t num = 0;
    while (true) {
        const FlagName flag = RegZ8000::parseFlagName(p);
        if (flag == FLAG_UNDEF)
            return -1;
        num |= RegZ8000::encodeFlagName(flag);
        if (endOfLine(*p.skipSpaces())) {
            scan = p;
            return num;
        }
        if (!p.expect(','))
            return -1;
        p.skipSpaces();
    }
}

Error AsmZ8000::parseOperand(StrScanner &scan, Operand &op) {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (p.expect('#')) {
        op.val32 = parseExpr32(p, op);
        if (parserError())
            return op.getError();
        op.mode = M_IM;
        scan = p;
        return OK;
    }
    if (endOfLine(*p))
        return OK;

    StrScanner a(p);
    if (p.expect('@')) {
        op.reg = RegZ8000::parseRegName(p);
        if (op.reg == REG_UNDEF)
            return op.setError(a, UNKNOWN_REGISTER);
        if (op.reg == REG_ILLEGAL)
            return op.setError(a, ILLEGAL_REGISTER);
        op.mode = M_IR;
        scan = p;
        return OK;
    }

    op.reg = RegZ8000::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        if (op.reg == REG_ILLEGAL)
            return op.setError(a, ILLEGAL_REGISTER);
        if (p.skipSpaces().expect('(')) {
            op.base = op.reg;
            if (p.skipSpaces().expect('#')) {
                op.val32 = parseExpr32(p, op);
                if (parserError())
                    return op.getError();
                if (!p.skipSpaces().expect(')'))
                    return op.setError(p, MISSING_CLOSING_PAREN);
                op.mode = M_BA;
                scan = p;
                return OK;
            }
            op.reg = RegZ8000::parseRegName(p);
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
    op.reg = RegZ8000::parseCtlReg(p);
    if (op.reg != REG_UNDEF) {
        op.mode = M_CTL;
        scan = p;
        return OK;
    }
    op.cc = RegZ8000::parseCcName(p);
    if (op.cc != CC_UNDEF) {
        // 'C' and 'Z' are parsed as M_CC, though these can be M_FLAG.
        const int8_t num = parseFlagNames(a);
        const FlagName flag = FlagName(num);
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
    if (*p == '|') {
        StrScanner expr = _parser.scanExpr(++a, '|');
        const auto size = expr.size();
        if (size == 0) {
            op.val32 = parseExpr32(p, op);
        } else {
            op.cc = CC_F;  // short direct
            op.val32 = parseExpr32(expr, op);
            p += size + 2;
        }
    } else {
        op.val32 = parseExpr32(p, op);
    }
    if (parserError())
        return op.getError();
    if (p.skipSpaces().expect('(')) {
        op.reg = RegZ8000::parseRegName(p.skipSpaces());
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
    insn.nameBuffer().text(_parser.readSymbol(scan));

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
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(dstOp);
    setErrorIf(srcOp);
    setErrorIf(ex1Op);
    setErrorIf(ex2Op);

    insn.setAddrMode(dstOp.mode, srcOp.mode, ex1Op.mode, ex2Op.mode);
    const auto error = TableZ8000::TABLE.searchName(insn);
    if (error)
        return setError(dstOp, error);
    if (insn.isThreeRegsInsn() && checkRegisterOverlap(insn, dstOp, srcOp, ex1Op))
        return getError();
    if (insn.isPushPopInsn() && checkRegisterOverlap(dstOp, srcOp))
        return getError();
    if (insn.isLoadMultiInsn()) {
        const Operand &regOp = insn.dstMode() == M_R ? dstOp : srcOp;
        if (RegZ8000::encodeGeneralRegName(regOp.reg) + ex1Op.val32 > 16)
            return setError(ex1Op, OVERFLOW_RANGE);
    }

    const AddrMode dst = insn.dstMode();
    if (dst != M_NO) {
        if (emitOperand(insn, dst, dstOp, insn.dstField()))
            return getError();
    }
    const AddrMode src = insn.srcMode();
    if (src != M_NO) {
        if (emitOperand(insn, src, srcOp, insn.srcField()))
            return getError();
    }
    const AddrMode ex1 = insn.ex1Mode();
    if (ex1 != M_NO) {
        const ModeField field = (ex1 == M_WR ? MF_P8 : MF_P0);
        if (emitOperand(insn, ex1, ex1Op, field))
            return getError();
    }
    const AddrMode ex2 = insn.ex2Mode();
    if (ex2 != M_NO) {
        if (emitOperand(insn, ex2, ex2Op, MF_P0))
            return getError();
    }
    insn.emitInsn();
    return getError();
}

}  // namespace z8000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
