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
#include "table_z8000.h"
#include "reg_z8000.h"

namespace libasm {
namespace z8000 {

bool AsmZ8000::endOfLine(const char *scan) const {
    return (*scan == '#') || Assembler::endOfLine(scan);
}

Error AsmZ8000::emitData(
    InsnZ8000 &insn, ModeField field, Config::opcode_t data) {
    data &= 0xF;
    if (field == MF_C4 || field == MF_P4) data <<= 4;
    if (field == MF_C8 || field == MF_P8) data <<= 8;
    if (field == MF_C0 || field == MF_C4 || field == MF_C8)
        insn.embed(data);
    if (field == MF_P0 || field == MF_P4 || field == MF_P8)
        insn.embedPost(data);
    return OK;
}

Error AsmZ8000::emitRegister(
    InsnZ8000 &insn, ModeField field, RegName reg) {
    return emitData(insn, field, RegZ8000::encodeGeneralRegName(reg));
}

Error AsmZ8000::emitIndirectRegister(
    InsnZ8000 &insn, ModeField field, RegName reg) {
    if (TableZ8000.segmentedModel()) {
        if (!RegZ8000::isLongReg(reg)) return setError(REGISTER_NOT_ALLOWED);
    } else {
        if (!RegZ8000::isWordReg(reg)) return setError(REGISTER_NOT_ALLOWED);
    }
    const uint8_t data = RegZ8000::encodeGeneralRegName(reg);
    if (data == 0) return setError(REGISTER_NOT_ALLOWED);
    return emitData(insn, field, data);
}

Error AsmZ8000::emitImmediate(
    InsnZ8000 &insn, ModeField field, AddrMode mode, const Operand &op) {
    if (mode == M_IM8) {
        const int32_t sval = static_cast<int32_t>(op.val32);
        if ((sval >= -0x80 && sval < 0x80) || op.val32 < 0x100) {
            const uint8_t val8 = static_cast<uint8_t>(op.val32);
            insn.embed(val8);
            return OK;
        }
        return setError(OVERFLOW_RANGE);
    }
    if (mode == M_BIT) {
        if (op.val32 >= (insn.oprSize() == SZ_BYTE ? 8 : 16))
            return setError(ILLEGAL_BIT_NUMBER);
        return emitData(insn, field, op.val32);
    }
    if (mode == M_CNT) {
        if (op.val32 > 16) return setError(OVERFLOW_RANGE);
        const Config::opcode_t count = op.getError() ? 0
            : static_cast<uint8_t>(op.val32) - 1;
        return emitData(insn, field, count);
    }
    if (mode == M_QCNT) {
        if (op.val32 == 1 || op.getError()) return OK;
        if (op.val32 == 2) return emitData(insn, field, 2);
        return setError(OVERFLOW_RANGE);
    }
    if (mode == M_SCNT || mode == M_NCNT) {
        int32_t sval = static_cast<int32_t>(op.val32);
        if (mode == M_NCNT) sval = -sval;
        uint16_t val16 = 0;
        switch (insn.oprSize()) {
        case SZ_BYTE:
            if (sval < -8 || sval > 8) return setError(OVERFLOW_RANGE);
            val16 = static_cast<uint8_t>(sval);
            break;
        case SZ_WORD:
            if (sval < -16 || sval > 16) return setError(OVERFLOW_RANGE);
            val16 = static_cast<uint16_t>(sval);
            break;
        case SZ_LONG:
            if (sval < -32 || sval > 32) return setError(OVERFLOW_RANGE);
            val16 = static_cast<uint16_t>(sval);
            break;
        default:
            return setError(UNKNOWN_OPERAND);
        }
        insn.emitOperand16(val16);
        return OK;
    }
    // M_IM
    const int32_t sval = static_cast<int32_t>(op.val32);
    switch (insn.oprSize()) {
    case SZ_BYTE:
        if ((sval >= -0x80 && sval < 0x80) || op.val32 < 0x100) {
            uint16_t val16 = static_cast<uint8_t>(op.val32);
            val16 |= (val16 << 8);
            insn.emitOperand16(val16);
            return OK;
        }
        break;
    case SZ_WORD:
        if ((sval >= -0x8000 && sval < 0x8000) || op.val32 < 0x10000) {
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
    return setError(OVERFLOW_RANGE);
}

Error AsmZ8000::emitDirectAddress(InsnZ8000 &insn, uint32_t addr) {
    if (TableZ8000.segmentedModel()) {
        if (addr >= 0x800000L) return setError(OVERFLOW_RANGE);
        const uint16_t seg = (addr >> 8) & 0x7F00;
        const uint16_t off = static_cast<uint16_t>(addr);
        if (off < 0x100) {
            insn.emitOperand16(seg | off);
        } else {
            insn.emitOperand16(0x8000 | seg);
            insn.emitOperand16(off);
        }
        return OK;
    }
    if (addr >= 0x10000) return setError(OVERFLOW_RANGE);
    insn.emitOperand16(static_cast<uint16_t>(addr));
    return OK;
}

Error AsmZ8000::emitRelative(
    InsnZ8000 &insn, AddrMode mode, const Operand &op) {
    const Config::uintptr_t base = insn.address() + (mode == M_RA ? 4 : 2);
    const Config::uintptr_t target = op.getError() ? base : op.val32;
    Config::ptrdiff_t delta = target - base;
    if (mode == M_RA) {
        if (delta < -0x8000 || delta >= 0x8000) return setError(OPERAND_TOO_FAR);
        insn.emitOperand16(static_cast<uint16_t>(delta));
        return OK;
    }
    if (target % 2) return setError(OPERAND_NOT_ALIGNED);
    delta /= 2;
    if (mode == M_RA12) {
        if (delta < -0x800 || delta >= 0x800) return setError(OPERAND_TOO_FAR);
        insn.embed(static_cast<uint16_t>(delta & 0xFFF));
        return OK;
    }
    if (mode == M_RA8) {
        if (delta < -0x80 || delta >= 0x80) return setError(OPERAND_TOO_FAR);
        insn.embed(static_cast<uint16_t>(delta & 0xFF));
        return OK;
    }
    // M_RA7
    if (delta <= -0x80 || delta > 0) return setError(OPERAND_TOO_FAR);
    insn.embed(static_cast<uint8_t>(-delta & 0x7F));
    return OK;
}

Error AsmZ8000::emitIndexed(
    InsnZ8000 &insn, ModeField field, const Operand &op) {
    if (!RegZ8000::isWordReg(op.reg) || op.reg == REG_R0)
        return setError(REGISTER_NOT_ALLOWED);
    if (emitRegister(insn, field, op.reg)) return getError();
    return emitDirectAddress(insn, op.val32);
}

Error AsmZ8000::emitBaseAddress(
    InsnZ8000 &insn, ModeField field, const Operand &op) {
    const int32_t disp = static_cast<int32_t>(op.val32);
    if (disp < -0x8000 || disp >= 0x8000) return setError(OVERFLOW_RANGE);
    if (emitIndirectRegister(insn, field, op.base)) return getError();
    insn.emitOperand16(static_cast<uint16_t>(disp));
    return OK;
}

Error AsmZ8000::emitBaseIndexed(
    InsnZ8000 &insn, ModeField field, const Operand &op) {
    if (!RegZ8000::isWordReg(op.reg) || RegZ8000::encodeGeneralRegName(op.reg) == 0)
        return setError(REGISTER_NOT_ALLOWED);
    if (emitIndirectRegister(insn, field, op.base)) return getError();
    return emitRegister(insn, MF_P8, op.reg);
}

Error AsmZ8000::emitFlags(
    InsnZ8000 &insn, ModeField field, const Operand &op) {
    if (op.mode == M_CC) {
        if (op.cc == CC_Z) return emitData(insn, field, FLAG_Z);
        if (op.cc == CC_C) return emitData(insn, field, FLAG_C);
        return setError(UNKNOWN_OPERAND);
    }
    if (op.val32 == 0) return setError(OPCODE_HAS_NO_EFFECT);
    return emitData(insn, field, static_cast<uint16_t>(op.val32));
}

Error AsmZ8000::emitCtlRegister(
    InsnZ8000 &insn, ModeField field, const Operand &op) {
    if (insn.oprSize() == SZ_BYTE && op.reg != REG_FLAGS)
        return setError(ILLEGAL_SIZE);
    if (insn.oprSize() == SZ_WORD && op.reg == REG_FLAGS)
        return setError(ILLEGAL_SIZE);
    return emitData(insn, field, RegZ8000::decodeCtlReg(op.reg));
}

Error AsmZ8000::emitOperand(
    InsnZ8000 &insn, AddrMode mode, const Operand &op, ModeField field) {
    switch (mode) {
    case M_DR:
        if (insn.oprSize() == SZ_WORD && !RegZ8000::isLongReg(op.reg))
            return setError(REGISTER_NOT_ALLOWED);
        if (insn.oprSize() == SZ_LONG && !RegZ8000::isQuadReg(op.reg))
            return setError(REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case M_R:
        return emitRegister(insn, field, op.reg);
    case M_WR07:
        if (RegZ8000::encodeGeneralRegName(op.reg) >= 8)
            return setError(REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case M_WR:
        if (!RegZ8000::isWordReg(op.reg)) return setError(REGISTER_NOT_ALLOWED);
        return emitRegister(insn, field, op.reg);
    case M_IR:
        return emitIndirectRegister(insn, field, op.reg);
    case M_IRIO:
        if (!RegZ8000::isWordReg(op.reg) || RegZ8000::encodeGeneralRegName(op.reg) == 0)
            return setError(REGISTER_NOT_ALLOWED);
        return emitRegister(insn, field, op.reg);
    case M_GENI:
        if (op.mode == M_IM) return emitImmediate(insn, MF_NO, M_IM, op);
        /* Fall-through */
    case M_GEND:
        if (op.mode == M_R) {
            insn.embed(0x8000);
            return emitRegister(insn, field, op.reg);
        }
        /* Fall-through */
    case M_GENA:
        if (op.mode == M_IR) return emitIndirectRegister(insn, field, op.reg);
        insn.embed(0x4000);
        if (op.mode == M_DA) return emitDirectAddress(insn, op.val32);
        return emitIndexed(insn, field, op);
    case M_BA:
        return emitBaseAddress(insn, field, op);
    case M_BX:
        return emitBaseIndexed(insn, field, op);
    case M_IO:
        if (op.val32 >= 0x10000) return setError(OVERFLOW_RANGE);
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
        if (op.mode == M_NO) return emitData(insn, field, CC_T);
        return emitData(insn, field, RegZ8000::encodeCcName(op.cc));
    case M_INTR:
        if (op.val32 == 0) return setError(OPCODE_HAS_NO_EFFECT);
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
        break;
    }
    return setError(INTERNAL_ERROR);
}

int8_t AsmZ8000::parseIntrNames(const char *scan) {
    const char *p = scan;
    if (endOfLine(p)) return 0;
    int8_t num = 0;
    while (true) {
        const IntrName intr = RegZ8000::parseIntrName(p);
        if (intr == INTR_UNDEF) return -1;
        p += RegZ8000::intrNameLen(intr);
        num |= RegZ8000::encodeIntrName(intr);
        p = skipSpaces(p);
        if (endOfLine(p)) {
            _scan = p;
            return num;
        }
        if (*p != ',') return -1;
        p = skipSpaces(p + 1);
    }
}

int8_t AsmZ8000::parseFlagNames(const char *scan) {
    const char *p = scan;
    _scan = p;
    if (endOfLine(p)) return 0;
    int8_t num = 0;
    while (true) {
        const FlagName flag = RegZ8000::parseFlagName(p);
        if (flag == FLAG_UNDEF) return -1;
        p += RegZ8000::flagNameLen(flag);
        num |= RegZ8000::encodeFlagName(flag);
        p = skipSpaces(p);
        if (endOfLine(p)) {
            _scan = p;
            return num;
        }
        if (*p != ',') return -1;
        p = skipSpaces(p + 1);
    }
}

Error AsmZ8000::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (*p == '#') {
        op.val32 = parseExpr32(p + 1);
        if (parserError()) return getError();
        op.setError(getError());
        op.mode = M_IM;
        return OK;
    }
    if (endOfLine(p)) return OK;
    if (*p == '@') {
        p++;
        op.reg = RegZ8000::parseRegName(p);
        if (op.reg == REG_UNDEF) return setError(UNKNOWN_REGISTER);
        if (op.reg == REG_ILLEGAL) return setError(ILLEGAL_REGISTER);
        p += RegZ8000::regNameLen(op.reg);
        _scan = p;
        op.mode = M_IR;
        return OK;
    }

    op.reg = RegZ8000::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        if (op.reg == REG_ILLEGAL) return setError(ILLEGAL_REGISTER);
        p += RegZ8000::regNameLen(op.reg);
        p = skipSpaces(p);
        if (*p == '(') {
            op.base = op.reg;
            p = skipSpaces(p + 1);
            if (*p == '#') {
                op.val32 = parseExpr32(p + 1);
                if (parserError()) return getError();
                op.setError(getError());
                p = skipSpaces(_scan);
                if (*p != ')') return setError(UNKNOWN_OPERAND);
                _scan = p + 1;
                op.mode = M_BA;
                return OK;
            }
            op.reg = RegZ8000::parseRegName(p);
            if (op.reg != REG_UNDEF) {
                p += RegZ8000::regNameLen(op.reg);
                p = skipSpaces(p);
                if (*p != ')') return setError(UNKNOWN_OPERAND);
                _scan = p + 1;
                op.mode = M_BX;
                return OK;
            }
            return setError(UNKNOWN_OPERAND);
        }
        _scan = p;
        op.mode = M_R;
        return OK;
    }
    op.reg = RegZ8000::parseCtlReg(p);
    if (op.reg != REG_UNDEF) {
        _scan = p + RegZ8000::regNameLen(op.reg);
        op.mode = M_CTL;
        return OK;
    }
    op.cc = RegZ8000::parseCcName(p);
    if (op.cc != CC_UNDEF) {
        const int8_t num = parseFlagNames(p);
        p += RegZ8000::ccNameLen(op.cc);
        if (num > 0 && _scan > p) {
            op.val32 = num;
            op.mode = M_FLAG;
            return OK;
        }
        _scan = p;
        op.mode = M_CC;
        return OK;
    }
    int8_t num = parseFlagNames(p);
    if (num >= 0) {
        op.val32 = num;
        op.mode = M_FLAG;
        return OK;
    }
    num = parseIntrNames(p);
    if (num >= 0) {
        op.val32 = num;
        op.mode = M_INTR;
        return OK;
    }
    op.val32 = parseExpr32(p);
    if (parserError()) return getError();
    op.setError(getError());
    p = skipSpaces(_scan);
    if (*p == '(') {
        p = skipSpaces(p + 1);
        op.reg = RegZ8000::parseRegName(p);
        if (op.reg == REG_UNDEF) return setError(UNKNOWN_OPERAND);
        p += RegZ8000::regNameLen(op.reg);
        p = skipSpaces(p);
        if (*p != ')') return setError(UNKNOWN_OPERAND);
        _scan = p + 1;
        op.mode = M_X;
        return OK;
    }
    _scan = p;
    op.mode = M_DA;
    return OK;
}

Error AsmZ8000::encode(Insn &_insn) {
    InsnZ8000 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    Operand dstOp, srcOp, ex1Op, ex2Op;
    if (parseOperand(endName, dstOp)) return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        if (parseOperand(p + 1, srcOp)) return getError();
        p = skipSpaces(_scan);
    }
    if (*p == ',') {
        if (parseOperand(p + 1, ex1Op)) return getError();
        p = skipSpaces(_scan);
    }
    if (*p == ',') {
        if (parseOperand(p + 1, ex2Op)) return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p)) return setError(GARBAGE_AT_END);
    setError(dstOp.getError());
    setErrorIf(srcOp.getError());
    setErrorIf(ex1Op.getError());
    setErrorIf(ex2Op.getError());

    insn.setAddrMode(dstOp.mode, srcOp.mode, ex1Op.mode, ex2Op.mode);
    if (TableZ8000.searchName(insn))
        return setError(TableZ8000.getError());

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

} // namespace z8000
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
