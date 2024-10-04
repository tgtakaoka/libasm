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
#include "text_common.h"

namespace libasm {
namespace z8000 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr char OPT_BOOL_SHORT_DIRECT[] PROGMEM = "short-direct";
constexpr char OPT_DESC_SHORT_DIRECT[] PROGMEM = "enable optimizing direct addressing";

constexpr char TEXT_LONG[]   PROGMEM = "long";
constexpr char TEXT_STRING[] PROGMEM = "string";
constexpr char TEXT_WORD[]   PROGMEM = "word";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_BYTE,   &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_LONG,   &Assembler::defineDataConstant, Assembler::DATA_LONG|Assembler::DATA_ALIGN2},
    {TEXT_STRING, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_WORD,   &Assembler::defineDataConstant, Assembler::DATA_WORD|Assembler::DATA_ALIGN2},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

struct Z8000SymbolParser final : PrefixSymbolParser, Singleton<Z8000SymbolParser> {
    Z8000SymbolParser() : PrefixSymbolParser(nullptr, PSTR_UNDER) {}
};

}  // namespace

const ValueParser::Plugins &AsmZ8000::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return ZilogNumberParser::singleton(); }
        const SymbolParser &symbol() const override { return Z8000SymbolParser::singleton(); }
        const LetterParser &letter() const override { return ZilogLetterParser::singleton(); }
        const OperatorParser &operators() const override {
            return ZilogOperatorParser::singleton();
        }
    } PLUGINS{};
    return PLUGINS;
}

AsmZ8000::AsmZ8000(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_shortDitrect),
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

void AsmZ8000::emitData(AsmInsn &insn, OprPos pos, Config::opcode_t data) const {
    data &= 0xF;
    if (pos == OP_C4 || pos == OP_P4)
        data <<= 4;
    if (pos == OP_C8 || pos == OP_P8)
        data <<= 8;
    if (pos == OP_C0 || pos == OP_C4 || pos == OP_C8)
        insn.embed(data);
    if (pos == OP_P0 || pos == OP_P4 || pos == OP_P8)
        insn.embedPostfix(data);
}

void AsmZ8000::emitRegister(AsmInsn &insn, OprPos pos, RegName reg) const {
    emitData(insn, pos, encodeGeneralRegName(reg));
}

void AsmZ8000::emitIndirectRegister(
        AsmInsn &insn, const Operand &op, OprPos pos, RegName reg) const {
    if (segmentedModel()) {
        if (!isLongReg(reg))
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
    } else {
        if (!isWordReg(reg))
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
    }
    const auto data = encodeGeneralRegName(reg);
    if (data == 0)
        insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
    emitData(insn, pos, data);
}

void AsmZ8000::emitImmediate(AsmInsn &insn, OprPos pos, AddrMode mode, const Operand &op) const {
    if (mode == M_IM8) {
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embed(static_cast<uint8_t>(op.val.getUnsigned()));
        return;
    }
    if (mode == M_BIT) {
        if (op.val.overflow(insn.size() == SZ_BYTE ? 7 : 15))
            insn.setErrorIf(op, ILLEGAL_BIT_NUMBER);
        emitData(insn, pos, op.val.getUnsigned());
        return;
    }
    if (mode == M_CNT) {
        auto count = op.val.getUnsigned();
        if (op.val.isNegative() || count == 0 || count > 16) {
            insn.setErrorIf(op, OVERFLOW_RANGE);
            count = 1;
        }
        emitData(insn, pos, count - 1);
        return;
    }
    if (mode == M_QCNT) {
        const auto qcnt = op.val.getUnsigned();
        if (qcnt == 1 || op.getError())
            return;
        if (qcnt == 0)
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        if (qcnt == 2) {
            emitData(insn, pos, 2);
            return;
        }
        insn.setErrorIf(op, OVERFLOW_RANGE);
        return;
    }
    if (mode == M_SCNT || mode == M_NCNT) {
        if (op.val.overflow(32))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        auto count = op.val.getSigned();
        const auto size = insn.size();
        if (count < 0 || (size == SZ_BYTE && count > 8) || (size == SZ_WORD && count > 16))
            insn.setErrorIf(op, OVERFLOW_RANGE);
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
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        {
            uint16_t val16 = static_cast<uint8_t>(op.val.getUnsigned());
            val16 |= (val16 << 8);
            insn.emitOperand16(val16);
        }
        break;
    case SZ_WORD:
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case SZ_QUAD:
        insn.emitOperand32(op.val.getUnsigned());
        break;
    default:
        break;
    }
}

void AsmZ8000::emitDirectAddress(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    const auto align = mode == M_DA && (insn.size() == SZ_WORD || insn.size() == SZ_QUAD);
    const auto addr = op.val.getUnsigned();
    insn.setErrorIf(op, checkAddr(addr, align));
    if (segmentedModel()) {
        const uint16_t seg = (addr >> 8) & 0x7F00;
        const uint16_t disp = addr;
        const auto autoShortDirect = _autoShortDirect && op.isOK();
        if (disp < 0x100 && autoShortDirect) {
            insn.emitOperand16(seg | disp);
        } else if (op.cc == CC_F) {  // short direct
            if (disp >= 0x100)
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand16(seg | (disp & 0xFF));
        } else {
            insn.emitOperand16(0x8000 | seg);
            insn.emitOperand16(disp);
        }
        return;
    }
    insn.emitOperand16(addr);
}

void AsmZ8000::emitRelative(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    const auto base = insn.address() + (mode == M_RA ? 4 : 2);
    const auto target = op.getError() ? base : op.val.getUnsigned();
    if (segmentedModel())
        insn.setErrorIf(op, checkAddr(target, insn.address(), 16));
    if (mode == M_RA) {
        insn.setErrorIf(op, checkAddr(target, insn.size() == SZ_WORD || insn.size() == SZ_QUAD));
        const auto delta = target - base;
        if (overflowDelta(delta, 16))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand16(static_cast<uint16_t>(delta));
        return;
    }
    insn.setErrorIf(op, checkAddr(target, true));
    auto delta = branchDelta(base, target, insn, op) / 2;
    if (mode == M_RA12) {
        delta = -delta;
        if (overflowDelta(delta, 12))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.embed(static_cast<uint16_t>(delta & 0xFFF));
        return;
    }
    if (mode == M_RA8) {
        if (overflowDelta(delta, 8))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.embed(static_cast<uint16_t>(delta & 0xFF));
        return;
    }
    // M_RA7
    if (delta <= -0x80 || delta > 0)
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.embed(static_cast<uint8_t>(-delta & 0x7F));
}

void AsmZ8000::emitIndexed(AsmInsn &insn, OprPos pos, const Operand &op) const {
    if (!isWordReg(op.reg) || op.reg == REG_R0)
        insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
    emitRegister(insn, pos, op.reg);
    emitDirectAddress(insn, M_X, op);
}

void AsmZ8000::emitBaseAddress(AsmInsn &insn, OprPos pos, const Operand &op) const {
    if (op.val.overflowInt16())
        insn.setErrorIf(op.baseAt, OVERFLOW_RANGE);
    emitIndirectRegister(insn, op, pos, op.base);
    insn.emitOperand16(op.val.getUnsigned());
}

void AsmZ8000::emitBaseIndexed(AsmInsn &insn, OprPos pos, const Operand &op) const {
    if (!isWordReg(op.reg))
        insn.setErrorIf(op.baseAt, REGISTER_NOT_ALLOWED);
    emitIndirectRegister(insn, op, pos, op.base);
    emitRegister(insn, OP_P8, op.reg);
}

void AsmZ8000::emitFlags(AsmInsn &insn, OprPos pos, const Operand &op) const {
    if (op.mode == M_CC) {
        if (op.cc == CC_Z) {
            emitData(insn, pos, FLAG_Z);
        } else if (op.cc == CC_C) {
            emitData(insn, pos, FLAG_C);
        } else {
            insn.setErrorIf(op, UNKNOWN_OPERAND);
        }
        return;
    }
    if (op.val.isUndefined())
        insn.setErrorIf(op, OPCODE_HAS_NO_EFFECT);
    emitData(insn, pos, op.val.getUnsigned());
}

void AsmZ8000::emitCtlRegister(AsmInsn &insn, OprPos pos, const Operand &op) const {
    if (insn.size() == SZ_BYTE && op.reg != REG_FLAGS)
        insn.setErrorIf(op, ILLEGAL_SIZE);
    if (insn.size() == SZ_WORD && op.reg == REG_FLAGS)
        insn.setErrorIf(op, ILLEGAL_SIZE);
    auto data = encodeCtlReg(segmentedModel(), op.reg);
    if (data < 0) {
        insn.setErrorIf(op, ILLEGAL_REGISTER);
        data = 0;
    }
    emitData(insn, pos, data);
}

void AsmZ8000::emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op, OprPos pos) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_DBLR:
        if (insn.size() == SZ_BYTE && !isWordReg(op.reg))
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        if (insn.size() == SZ_WORD && !isLongReg(op.reg))
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        if (insn.size() == SZ_QUAD && !isQuadReg(op.reg))
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case M_R:
        emitRegister(insn, pos, op.reg);
        break;
    case M_WR07:
        if (encodeGeneralRegName(op.reg) >= 8)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case M_WR:
        if (!isWordReg(op.reg))
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        emitRegister(insn, pos, op.reg);
        break;
    case M_IR:
        emitIndirectRegister(insn, op, pos, op.reg);
        break;
    case M_IRIO:
        if (!isWordReg(op.reg) || encodeGeneralRegName(op.reg) == 0)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        emitRegister(insn, pos, op.reg);
        break;
    case M_GENI:
        if (op.mode == M_IM) {
            emitImmediate(insn, OP_NO, M_IM, op);
            break;
        }
        /* Fall-through */
    case M_GEND:
        if (op.mode == M_R) {
            insn.embed(0x8000);
            emitRegister(insn, pos, op.reg);
            break;
        }
        /* Fall-through */
    case M_GENA:
        if (op.mode == M_IR) {
            emitIndirectRegister(insn, op, pos, op.reg);
            break;
        }
        insn.embed(0x4000);
        if (op.mode == M_DA) {
            emitDirectAddress(insn, M_DA, op);
            break;
        }
        emitIndexed(insn, pos, op);
        break;
    case M_BA:
        emitBaseAddress(insn, pos, op);
        break;
    case M_BX:
        emitBaseIndexed(insn, pos, op);
        break;
    case M_IO:
        if (op.val.overflow(UINT16_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_IM:
    case M_IM8:
    case M_BIT:
    case M_CNT:
    case M_QCNT:
    case M_SCNT:
    case M_NCNT:
        emitImmediate(insn, pos, mode, op);
        break;
    case M_CC:
        if (op.mode == M_NONE) {
            emitData(insn, pos, CC_T);
            break;
        }
        emitData(insn, pos, encodeCcName(op.cc));
        break;
    case M_INTR:
        if (op.val.isUndefined())
            insn.setErrorIf(op, OPCODE_HAS_NO_EFFECT);
        emitData(insn, pos, 3 ^ op.val.getUnsigned());
        break;
    case M_FLAG:
        emitFlags(insn, pos, op);
        break;
    case M_RA:
    case M_RA12:
    case M_RA8:
    case M_RA7:
        emitRelative(insn, mode, op);
        break;
    case M_CTL:
        emitCtlRegister(insn, pos, op);
        break;
    default:
        break;
    }
}

void AsmZ8000::checkRegisterOverlap(
        AsmInsn &insn, const Operand &dstOp, const Operand &srcOp, const Operand *cntOp) const {
    const auto dst = dstOp.reg;
    const auto src = srcOp.reg;
    const uint8_t dnum = encodeGeneralRegName(dst);
    const uint8_t ds = isByteReg(dst) && dnum >= 8 ? dnum - 8 : dnum;
    const uint8_t de = isLongReg(dst) ? ds + 1 : ds;
    const uint8_t ss = encodeGeneralRegName(src);
    const uint8_t se = isLongReg(src) ? ss + 1 : ss;
    if (ds == ss || ds == se || de == ss || de == se)
        insn.setErrorIf(srcOp, REGISTERS_OVERLAPPED);
    if (cntOp == nullptr)
        return;
    const auto cnt = cntOp->reg;
    const uint8_t c = encodeGeneralRegName(cnt);
    if (ds == c || de == c)
        insn.setErrorIf(*cntOp, REGISTERS_OVERLAPPED);
    if (ss == c || se == c)
        insn.setErrorIf(*cntOp, REGISTERS_OVERLAPPED);
}

void AsmZ8000::checkRegisterOverlap(
        AsmInsn &insn, const Operand &dstOp, const Operand &srcOp, const Operand &cntOp) const {
    if (dstOp.mode == M_IR && (dstOp.reg == REG_R0 || dstOp.reg == REG_RR0))
        insn.setErrorIf(dstOp, REGISTER_NOT_ALLOWED);
    if (srcOp.mode == M_IR && (srcOp.reg == REG_R0 || srcOp.reg == REG_RR0))
        insn.setErrorIf(srcOp, REGISTER_NOT_ALLOWED);
    if (insn.isTranslateInsn()) {
        // Original content of RH1 are lost, so that R1 must not be used as dst/src.
        if (!segmentedModel()) {
            if (dstOp.reg == REG_R1)
                insn.setErrorIf(dstOp, REGISTER_NOT_ALLOWED);
            if (srcOp.reg == REG_R1)
                insn.setErrorIf(srcOp, REGISTER_NOT_ALLOWED);
        }
        // R1 should not be used as cnt as well.
        if (cntOp.reg == REG_R1)
            insn.setErrorIf(cntOp, REGISTER_NOT_ALLOWED);
    }
    checkRegisterOverlap(insn, dstOp, srcOp, &cntOp);
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

void AsmZ8000::parseAddress(StrScanner &scan, Operand &op) const {
    auto p = scan;
    const auto delim = p.expect('|');
    if (p.skipSpaces().iexpectText_P(PSTR("<<"))) {
        const auto segAt = p.skipSpaces();
        ErrorAt segError;
        auto seg = parseInteger(p, segError, '>');
        if (segError.hasError()) {
            op.setError(segError);
        } else if (p.skipSpaces().iexpectText_P(PSTR(">>"))) {
            const auto dispAt = p;
            constexpr auto UINT7_MAX = UINT8_C(0x7F);
            if (seg.overflow(UINT7_MAX)) {
                op.setErrorIf(segAt, OVERFLOW_RANGE);
                seg.setUnsigned(seg.getUnsigned() & UINT7_MAX);
            }
            op.val = parseInteger(p, op, delim);
            if (op.val.overflow(UINT16_MAX)) {
                op.setErrorIf(dispAt, OVERFLOW_RANGE);
                op.val.setUnsigned(op.val.getUnsigned() & UINT16_MAX);
            }
            op.val.setUnsigned(op.val.getUnsigned() | (seg.getUnsigned() << 16));
            if (segError.getError() == UNDEFINED_SYMBOL)
                op.setError(segError);
        } else {
            op.setError(scan, UNKNOWN_OPERAND);
        }
    } else {
        op.val = parseInteger(p, op, delim);
    }
    if (delim) {
        if (p.skipSpaces().expect(delim)) {
            op.cc = CC_F;  // short direct
        } else {
            op.setErrorIf(scan, UNKNOWN_OPERAND);
        }
    }
    scan = p;
}

Error AsmZ8000::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (p.expect('#')) {
        op.val = parseInteger(p, op);
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
            op.baseAt = p;
            if (p.skipSpaces().expect('#')) {
                op.val = parseInteger(p, op, ')');
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
            op.val.setUnsigned(num);
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
        op.val.setUnsigned(num);
        op.mode = M_FLAG;
        scan = p;
        return OK;
    }
    num = parseIntrNames(p);
    if (num >= 0) {
        op.val.setUnsigned(num);
        op.mode = M_INTR;
        scan = p;
        return OK;
    }
    parseAddress(p, op);
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

Error AsmZ8000::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
        return _insn.setError(insn.dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
            return _insn.setError(insn.srcOp);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, insn.ex1Op) && insn.ex1Op.hasError())
            return _insn.setError(insn.ex1Op);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, insn.ex2Op) && insn.ex2Op.hasError())
            return _insn.setError(insn.ex2Op);
        scan.skipSpaces();
    }

    if (_insn.setErrorIf(insn.dstOp, TABLE.searchName(cpuType(), insn)))
        return _insn.getError();

    if (insn.isThreeRegsInsn())
        checkRegisterOverlap(insn, insn.dstOp, insn.srcOp, insn.ex1Op);
    if (insn.isPushPopInsn())
        checkRegisterOverlap(insn, insn.dstOp, insn.srcOp);
    if (insn.isLoadMultiInsn()) {
        auto &len = insn.ex1Op.val;
        auto n = len.getUnsigned();
        if (len.isNegative() || n == 0) {
            if (!insn.ex1Op.hasError())
                insn.ex1Op.setErrorIf(ILLEGAL_CONSTANT);
            n = 1;
        }
        const auto &regOp = insn.dst() == M_R ? insn.dstOp : insn.srcOp;
        if (encodeGeneralRegName(regOp.reg) + n > 16)
            insn.setErrorIf(insn.ex1Op, OVERFLOW_RANGE);
    }

    emitOperand(insn, insn.dst(), insn.dstOp, insn.dstPos());
    emitOperand(insn, insn.src(), insn.srcOp, insn.srcPos());
    const auto ex1Pos = (insn.ex1() == M_WR ? OP_P8 : OP_P0);
    emitOperand(insn, insn.ex1(), insn.ex1Op, ex1Pos);
    emitOperand(insn, insn.ex2(), insn.ex2Op, OP_P0);
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace z8000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
