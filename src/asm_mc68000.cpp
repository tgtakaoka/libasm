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

#include "asm_mc68000.h"

#include <ctype.h>

#include "table_mc68000.h"

namespace libasm {
namespace mc68000 {

static int8_t modePos(OprPos pos) {
    switch (pos) {
    case OP_10:
        return 3;
    case OP_23:
        return 6;
    default:
        return -1;
    }
}

static int8_t regPos(OprPos pos) {
    switch (pos) {
    case OP_10:
    case OP__0:
        return 0;
    case OP_23:
    case OP__3:
        return 9;
    default:
        return -1;
    }
}

void emitOprSize(InsnMc68000 &insn, OprSize size) {
    Config::opcode_t sz = 0;
    switch (insn.oprSize()) {
    case SZ_DATA:
        if (size == SZ_NONE)
            size = SZ_WORD;
        sz = static_cast<uint8_t>(size) << 6;
        break;
    case SZ_ADR6:
        if (size == SZ_NONE)
            size = SZ_WORD;
        sz = (size == SZ_LONG) ? (1 << 6) : 0;
        break;
    case SZ_ADR8:
        if (size == SZ_NONE)
            size = SZ_WORD;
        sz = (size == SZ_LONG) ? (1 << 8) : 0;
        break;
    default:
        return;
    }
    insn.embed(sz);
}

Error AsmMc68000::checkAlignment(OprSize size, Config::uintptr_t addr) {
    if (size == SZ_WORD && (addr % 2))
        return setError(OPERAND_NOT_ALIGNED);
    if (size == SZ_LONG && (addr % 4))
        return setError(OPERAND_NOT_ALIGNED);
    return OK;
}

Error AsmMc68000::emitBriefExtension(InsnMc68000 &insn, RegName index,
        OprSize size, Config::ptrdiff_t disp) {
    if (disp < -0x80 || disp > 0x80)
        return setError(OVERFLOW_RANGE);
    uint16_t ext = static_cast<uint8_t>(disp);
    ext |= RegMc68000::encodeGeneralRegNo(index) << 12;
    if (RegMc68000::isAddrReg(index))
        ext |= (1 << 15);
    if (size == SZ_LONG)
        ext |= (1 << 11);
    insn.emitOperand16(ext);
    return OK;
}

Error AsmMc68000::emitDisplacement(InsnMc68000 &insn, Config::ptrdiff_t disp) {
    if (disp < -0x8000 || disp > 0x8000)
        return setError(OVERFLOW_RANGE);
    insn.emitOperand16(static_cast<uint16_t>(disp));
    return OK;
}

Error AsmMc68000::emitAbsoluteAddr(InsnMc68000 &insn, OprSize size,
        AddrMode mode, Config::uintptr_t addr) {
    if (checkAlignment(size, addr))
        return getError();
    if (mode == M_AWORD) {
        if (addr >= 0x8000 && addr < 0xFF800000L)
            return setError(OVERFLOW_RANGE);
        insn.emitOperand16(static_cast<uint16_t>(addr));
    }
    if (mode == M_ALONG)
        insn.emitOperand32(addr);
    return OK;
}

Error AsmMc68000::emitRelativeAddr(
        InsnMc68000 &insn, AddrMode mode, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = op.getError() ? base : op.val32;
    if (target % 2)
        return setError(OPERAND_NOT_ALIGNED);
    const Config::ptrdiff_t disp = target - base;
    if (mode == M_REL8) {
        if (disp >= -0x80 && disp < 0x80) {
            insn.embed(static_cast<uint8_t>(disp));
            return OK;
        }
    }
    if (disp < -0x8000 || disp >= 0x8000)
        return setError(OPERAND_TOO_FAR);
    insn.emitOperand16(static_cast<uint16_t>(disp));
    return OK;
}

Error AsmMc68000::emitImmediateData(
        InsnMc68000 &insn, OprSize size, uint32_t data) {
    if (size == SZ_LONG) {
        insn.emitOperand32(data);
        return OK;
    }
    if (size == SZ_WORD) {
        const int32_t val = static_cast<int32_t>(data);
        if ((val >= -0x8000 && val < 0x8000) || data < 0x10000L) {
            insn.emitOperand16(static_cast<uint16_t>(data));
            return OK;
        }
    }
    if (size == SZ_BYTE) {
        const int32_t val = static_cast<int32_t>(data);
        if ((val >= -0x80 && val < 0x80) || data < 0x100) {
            insn.emitOperand16(static_cast<uint8_t>(data));
            return OK;
        }
    }
    return setError(OVERFLOW_RANGE);
}

Config::uintptr_t AsmMc68000::Operand::offset(const InsnMc68000 &insn) const {
    if (getError())
        return 0;
    uint8_t len = insn.length();
    if (len == 0)
        len = sizeof(Config::opcode_t);
    return val32 - (insn.address() + len);
}

Error AsmMc68000::emitEffectiveAddr(InsnMc68000 &insn, OprSize size,
        const Operand &op, AddrMode mode, OprPos pos) {
    if (mode == M_NONE)
        return op.mode == M_NONE ? OK : UNKNOWN_OPERAND;

    const int8_t mode_gp = modePos(pos);
    if (mode_gp >= 0) {
        const Config::opcode_t m = EaMc68000::encodeMode(op.mode);
        insn.embed(m << mode_gp);
    }

    const int8_t reg_gp = regPos(pos);
    if (reg_gp >= 0) {
        const Config::opcode_t r = EaMc68000::encodeRegNo(op.mode, op.reg);
        insn.embed(r << reg_gp);
    }

    switch (op.mode) {
    case M_AREG:
        if (size == SZ_BYTE)
            return setError(OPERAND_NOT_ALLOWED);
        break;
    case M_INDX:
        return emitBriefExtension(insn, op.indexReg, op.indexSize,
                static_cast<Config::ptrdiff_t>(op.val32));
    case M_PCIDX:
        return emitBriefExtension(
                insn, op.indexReg, op.indexSize, op.offset(insn));
    case M_DISP:
        return emitDisplacement(insn, static_cast<Config::ptrdiff_t>(op.val32));
    case M_PCDSP:
        if (checkAlignment(size, op.val32))
            return getError();
        return emitDisplacement(insn, op.offset(insn));
    case M_AWORD:
    case M_ALONG:
        return emitAbsoluteAddr(insn, size, op.mode, op.val32);
    case M_IMDAT:
        if (mode == M_IMBIT)
            return OK;
        if (mode == M_IM3) {
            if (op.val32 > 8)
                return setError(OVERFLOW_RANGE);
            const Config::opcode_t count = op.getError() ? 8 : op.val32;
            const Config::opcode_t data = (count == 8 ? 0 : count) << 9;
            insn.embed(data);
            return OK;
        }
        if (mode == M_IM8) {
            const int32_t data = static_cast<int32_t>(op.val32);
            if (data < -0x80 || data >= 0x80)
                return setError(OVERFLOW_RANGE);
            insn.embed(static_cast<uint8_t>(data));
            return OK;
        }
        if (mode == M_IMVEC) {
            if (op.val32 >= 16)
                return setError(OVERFLOW_RANGE);
            insn.embed(static_cast<uint8_t>(op.val32));
            return OK;
        }
        if (mode == M_IMDSP) {
            const int32_t disp = static_cast<int32_t>(op.val32);
            if (disp < -0x8000 || disp >= 0x8000)
                return setError(OVERFLOW_RANGE);
            insn.emitOperand16(static_cast<uint16_t>(op.val32));
            return OK;
        }
        return emitImmediateData(insn, size, op.val32);
    case M_LABEL:
        if (size == SZ_LONG || (size == SZ_BYTE && mode == M_REL16))
            return setError(ILLEGAL_SIZE);
        if (size == SZ_WORD && mode == M_REL8)
            mode = M_REL16;
        return emitRelativeAddr(insn, mode, op);
    default:
        break;
    }
    return OK;
}

static uint16_t reverseBits(uint16_t bits) {
    uint16_t reverse = 0;
    for (uint8_t i = 0;; i++) {
        if (bits & 1)
            reverse |= 1;
        if (i == 15)
            break;
        bits >>= 1;
        reverse <<= 1;
    }
    return reverse;
}

void AsmMc68000::Operand::fixupMultiRegister() {
    if (mode == M_DREG || mode == M_AREG) {
        val32 = (1 << RegMc68000::encodeGeneralRegPos(reg));
        mode = M_MULT;
    }
}

Error AsmMc68000::parseMoveMultiRegList(const char *scan, Operand &opr) {
    const char *p = skipSpaces(scan);
    Error error = OK;
    for (;;) {
        RegName start = RegMc68000::parseRegName(p);
        if (!RegMc68000::isGeneralReg(start))
            return opr.setError(REGISTER_NOT_ALLOWED);
        p = skipSpaces(p + RegMc68000::regNameLen(start));
        uint8_t s = RegMc68000::encodeGeneralRegPos(start);
        uint8_t e = s;
        if (*p == '-') {
            p = skipSpaces(p + 1);
            RegName last = RegMc68000::parseRegName(p);
            if (!RegMc68000::isGeneralReg(start))
                return opr.setError(REGISTER_NOT_ALLOWED);
            e = RegMc68000::encodeGeneralRegPos(last);
            p = skipSpaces(p + RegMc68000::regNameLen(last));
        }
        if (s > e)
            return opr.setError(UNKNOWN_OPERAND);
        for (uint8_t i = s; i <= e; i++) {
            const uint32_t bm = (1 << i);
            if (opr.val32 & bm)
                error = DUPLICATE_REGISTER;
            opr.val32 |= bm;
        }
        if (*p != '/')
            break;
        p++;
    }
    opr.mode = M_MULT;
    _scan = p;
    return opr.setError(error);
}

Error AsmMc68000::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p))
        return OK;
    if (*p == '#') {
        op.val32 = parseExpr32(p + 1);
        if (parserError())
            return getError();
        op.setError(getError());
        op.mode = M_IMDAT;
        return OK;
    }
    const char pdec = *p;
    if (pdec == '-' && p[1] == '(')
        p++;
    if (*p == '(') {
        p = skipSpaces(p + 1);
        op.reg = RegMc68000::parseRegName(p);
        if (RegMc68000::isAddrReg(op.reg)) {
            p += RegMc68000::regNameLen(op.reg);
            p = skipSpaces(p);
            if (*p++ != ')')
                return op.setError(MISSING_CLOSING_PAREN);
            if (*p == '+') {
                p++;
                op.mode = M_PINC;
            } else {
                op.mode = (pdec == '-') ? M_PDEC : M_AIND;
            }
            _scan = p;
            return OK;
        }
        op.val32 = parseExpr32(p);
        if (parserError())
            return getError();
        op.setErrorIf(getError());
        p = skipSpaces(_scan);
        if (*p == ')') {
            p++;
            const OprSize size = RegMc68000::parseSize(p);
            p += RegMc68000::sizeNameLen(size);
            if ((op.val32 & 0xFF8000) == 0xFF8000)
                op.val32 |= 0xFFFF0000;
            if (size == SZ_NONE) {
                op.mode = (op.val32 >= 0xFFFF8000 || op.val32 < 0x8000)
                                  ? M_AWORD
                                  : M_ALONG;
            } else if (size == SZ_WORD) {
                op.mode = M_AWORD;
            } else if (size == SZ_LONG) {
                op.mode = M_ALONG;
            } else {
                return setError(UNKNOWN_OPERAND);
            }
            _scan = p;
            return OK;
        }
        if (*p == ',') {
            p = skipSpaces(p + 1);
            op.reg = RegMc68000::parseRegName(p);
            if (!RegMc68000::isAddrReg(op.reg) && op.reg != REG_PC)
                return setError(REGISTER_NOT_ALLOWED);
            p += RegMc68000::regNameLen(op.reg);
            p = skipSpaces(p);
            if (*p == ')') {
                op.mode = (op.reg == REG_PC) ? M_PCDSP : M_DISP;
                _scan = p + 1;
                return OK;
            }
            if (*p != ',')
                return setError(MISSING_COMMA);
            p = skipSpaces(p + 1);
            op.indexReg = RegMc68000::parseRegName(p);
            if (!RegMc68000::isGeneralReg(op.indexReg))
                return setError(UNKNOWN_OPERAND);
            p += RegMc68000::regNameLen(op.indexReg);
            op.indexSize = RegMc68000::parseSize(p);
            p = skipSpaces(p + RegMc68000::sizeNameLen(op.indexSize));
            if (op.indexSize == SZ_ERROR)
                return setError(UNKNOWN_OPERAND);
            if (op.indexSize == SZ_NONE)
                op.indexSize = SZ_WORD;
            if (*p++ != ')')
                return setError(MISSING_CLOSING_PAREN);
            op.mode = (op.reg == REG_PC) ? M_PCIDX : M_INDX;
            _scan = p;
            return OK;
        }
        return op.setError(UNKNOWN_OPERAND);
    }

    op.reg = RegMc68000::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        const char *a = skipSpaces(p + RegMc68000::regNameLen(op.reg));
        if ((*a == '/' || *a == '-') && RegMc68000::isGeneralReg(op.reg))
            return parseMoveMultiRegList(p, op);
        if (RegMc68000::isAddrReg(op.reg)) {
            op.mode = M_AREG;
        } else if (RegMc68000::isDataReg(op.reg)) {
            op.mode = M_DREG;
        } else if (op.reg == REG_USP) {
            op.mode = M_USP;
        } else if (op.reg == REG_CCR) {
            op.mode = M_CCR;
        } else if (op.reg == REG_SR) {
            op.mode = M_SR;
        } else
            return setError(REGISTER_NOT_ALLOWED);
        _scan = a;
        return OK;
    }
    op.val32 = parseExpr32(p);
    if (parserError())
        return getError();
    op.setErrorIf(getError());
    op.mode = M_LABEL;
    return OK;
}

Error AsmMc68000::encode(Insn &_insn) {
    InsnMc68000 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    const OprSize isize = RegMc68000::parseSize(endName);
    if (isize == SZ_ERROR)
        return setError(ILLEGAL_SIZE);
    insn.setInsnSize(isize);
    endName += RegMc68000::sizeNameLen(isize);

    Operand srcOp, dstOp;
    if (parseOperand(endName, srcOp))
        return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        if (parseOperand(p + 1, dstOp))
            return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setErrorIf(srcOp.getError());
    setErrorIf(dstOp.getError());

    insn.setAddrMode(srcOp.mode, dstOp.mode);
    if (TableMc68000.searchName(insn))
        return setError(TableMc68000.getError());

    const AddrMode src = insn.srcMode();
    const AddrMode dst = insn.dstMode();
    if (src == M_MULT)
        srcOp.fixupMultiRegister();
    if (dst == M_MULT)
        dstOp.fixupMultiRegister();
    if (src == M_MULT && dstOp.mode == M_PDEC)
        srcOp.val32 = reverseBits(srcOp.val32);
    if (src == M_MULT)
        insn.emitOperand16(static_cast<uint16_t>(srcOp.val32));
    if (dst == M_MULT)
        insn.emitOperand16(static_cast<uint16_t>(dstOp.val32));
    if (src == M_IMBIT) {
        if (srcOp.mode != M_IMDAT)
            return setError(OPERAND_NOT_ALLOWED);
        if (insn.oprSize() == SZ_BYTE && srcOp.val32 >= 8)
            return setError(ILLEGAL_BIT_NUMBER);
        if (insn.oprSize() == SZ_LONG && srcOp.val32 >= 32)
            return setError(ILLEGAL_BIT_NUMBER);
        insn.emitOperand16(static_cast<uint16_t>(srcOp.val32));
    }
    emitOprSize(insn, isize);
    insn.setInsnSize(isize);
    const OprSize osize = (isize == SZ_NONE) ? insn.oprSize() : isize;
    if (emitEffectiveAddr(insn, osize, srcOp, src, insn.srcPos()))
        return getError();
    if (emitEffectiveAddr(insn, osize, dstOp, dst, insn.dstPos()))
        return getError();
    insn.emitInsn();
    return getError();
}

}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
