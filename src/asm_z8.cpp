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

#include "asm_z8.h"

#include "reg_z8.h"
#include "table_z8.h"
#include "text_common.h"

namespace libasm {
namespace z8 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr char OPT_BOOL_ALIAS[]  PROGMEM = "reg-alias";
constexpr char OPT_DESC_ALIAS[]  PROGMEM = "emit register alias regarding setrp value";
constexpr char OPT_INT_SETRP[]   PROGMEM = "setrp";
constexpr char OPT_DESC_SETRP[]  PROGMEM = "set register pointer";
constexpr char OPT_INT_SETRP0[]  PROGMEM = "setrp0";
constexpr char OPT_DESC_SETRP0[] PROGMEM = "set register pointer 0";
constexpr char OPT_INT_SETRP1[]  PROGMEM = "setrp1";
constexpr char OPT_DESC_SETRP1[] PROGMEM = "set register pointer 1";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_DB, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DL, &Assembler::defineDataConstant, Assembler::DATA_LONG},
    {TEXT_DS, &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DW, &Assembler::defineDataConstant, Assembler::DATA_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

struct AsmZ8::Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    CcName cc;
    uint16_t val16;
    Operand() : mode(M_NONE), reg(REG_UNDEF), cc(CC_UNDEF), val16(0) {}
};

const ValueParser::Plugins &AsmZ8::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return ZilogNumberParser::singleton(); }
        const SymbolParser &symbol() const override { return _symbol; }
        const LetterParser &letter() const override { return ZilogLetterParser::singleton(); }
        const OperatorParser &operators() const override {
            return ZilogOperatorParser::singleton();
        }
        const SimpleSymbolParser _symbol{SymbolParser::DOLLAR_DOT_QUESTION};
    } PLUGINS{};
    return PLUGINS;
}

AsmZ8::AsmZ8(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_reg_alias),
      Config(TABLE),
      _opt_reg_alias(this, &AsmZ8::setRegAlias, OPT_BOOL_ALIAS, OPT_DESC_ALIAS, _opt_setrp),
      _opt_setrp(this, &AsmZ8::setRegPointer, OPT_INT_SETRP, OPT_DESC_SETRP, _opt_setrp0),
      _opt_setrp0(this, &AsmZ8::setRegPointer0, OPT_INT_SETRP0, OPT_DESC_SETRP0, _opt_setrp1),
      _opt_setrp1(this, &AsmZ8::setRegPointer1, OPT_INT_SETRP1, OPT_DESC_SETRP1) {
    reset();
}

void AsmZ8::reset() {
    Assembler::reset();
    setRegAlias(true);
    setRegPointer(-1);
}

Error AsmZ8::setRegAlias(bool enable) {
    _reg_alias = enable;
    return OK;
}

Error AsmZ8::setRegPointer(int32_t rp) {
    if (rp >= 0 && (rp & ~0xFF))
        return ILLEGAL_OPERAND;
    const auto error0 = setRegPointer0(rp);
    const auto error1 = setRegPointer1(rp >= 0 ? rp + 8 : rp);
    return error0 ? error0 : error1;
}

Error AsmZ8::setRegPointer0(int32_t rp0) {
    if (rp0 >= 0 && (rp0 & ~0xF8))
        return ILLEGAL_OPERAND;
    _regPointer0 = rp0;
    return OK;
}

Error AsmZ8::setRegPointer1(int32_t rp1) {
    if (rp1 >= 0 && (rp1 & ~0xF8))
        return ILLEGAL_OPERAND;
    _regPointer1 = rp1;
    return OK;
}

bool AsmZ8::isWorkReg(uint8_t regAddr) const {
    const auto regPage = (regAddr & 0xF8);
    if (_regPointer0 >= 0 && regPage == _regPointer0)
        return true;
    if (_regPointer1 >= 0 && regPage == _regPointer1)
        return true;
    return false;
}

void AsmZ8::encodeOperand(AsmInsn &insn, const AddrMode mode, const Operand &op) {
    if (mode == M_NONE)
        return;
    if (op.reg != REG_UNDEF && (mode == M_R || mode == M_IR || mode == M_IRR)) {
        const auto opr = _reg_alias ? encodeWorkRegAddr(isSuper8(), op.reg) : op.val16;
        insn.emitOperand8(opr);
        return;
    }
    if ((mode != M_IM || overflowUint8(op.val16)) && op.val16 >= 0x100)
        setErrorIf(op, OVERFLOW_RANGE);
    insn.emitOperand8(op.val16);
}

void AsmZ8::encodeAbsolute(AsmInsn &insn, const Operand &dstOp, const Operand &srcOp) {
    const auto dst = insn.dst();
    const auto &op = (dst == M_DA) ? dstOp : srcOp;
    if (dst == M_cc)
        insn.embed(encodeCcName(dstOp.cc) << 4);
    insn.emitOperand16(op.val16);
}

void AsmZ8::encodeRelative(AsmInsn &insn, const Operand &op) {
    const auto base = insn.address() + insn.emitLength();
    const auto target = op.getError() ? base : op.val16;
    const auto delta = branchDelta(base, target, op);
    if (overflowInt8(delta))
        setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand8(delta);
}

void AsmZ8::encodeIndexed(AsmInsn &insn, const Operand &dstOp, const Operand &srcOp) {
    const auto dst = insn.dst();
    const auto &op = (dst == M_X) ? dstOp : srcOp;
    const auto reg = (dst == M_X) ? srcOp.reg : dstOp.reg;
    if (overflowUint8(static_cast<uint16_t>(op.val16)))
        setErrorIf(op, OVERFLOW_RANGE);
    const auto opr1 = encodeRegName(op.reg) | (encodeRegName(reg) << 4);
    insn.emitOperand8(opr1);
    insn.emitOperand8(op.val16);
}

void AsmZ8::encodeIndirectRegPair(AsmInsn &insn, const Operand &dstOp, const Operand &srcOp) {
    const auto dst = insn.dst();
    const auto pair = (dst == M_Irr) ? dstOp.reg : srcOp.reg;
    const auto reg = (dst == M_Irr) ? srcOp.reg : dstOp.reg;
    const auto opr = encodeRegName(pair) | (encodeRegName(reg) << 4);
    insn.emitOperand8(opr);
}

void AsmZ8::encodeInOpCode(AsmInsn &insn, const Operand &dstOp, const Operand &srcOp) {
    const auto dst = insn.dst();
    const auto src = insn.src();
    const auto reg = (dst == M_r) ? dstOp.reg : srcOp.reg;
    const auto &op = (dst == M_r) ? srcOp : dstOp;
    const auto mode = (dst == M_r) ? src : dst;
    insn.embed(encodeRegName(reg) << 4);
    encodeOperand(insn, mode, op);
}

void AsmZ8::encodeMultiOperands(
        AsmInsn &insn, const Operand &dstOp, const Operand &srcOp, const Operand &extOp) {
    const auto dst = insn.dst();
    const auto src = insn.src();
    if (src == M_Ir && insn.ext() == M_RA) {
        const auto opr1 = encodeRegName(dstOp.reg) | (encodeRegName(srcOp.reg) << 4);
        insn.emitOperand8(opr1);
        encodeRelative(insn, extOp);
        return;
    }
    if (dst == M_r && (src == M_r || src == M_Ir)) {
        const auto opr = encodeRegName(srcOp.reg) | (encodeRegName(dstOp.reg) << 4);
        insn.emitOperand8(opr);
        return;
    }
    if (dst == M_RR && src == M_IML) {
        insn.emitOperand8(dstOp.val16);
        insn.emitOperand16(srcOp.val16);
        return;
    }
    if (dst == M_r || dst == M_Ir) {
        const auto opr1 = encodeRegName(srcOp.reg) | (encodeRegName(dstOp.reg) << 4);
        insn.emitOperand8(opr1);
        return;
    }
    const auto dstFirst = insn.dstFirst();
    const auto dstVal =
            (dstOp.reg == REG_UNDEF) ? dstOp.val16 : encodeWorkRegAddr(isSuper8(), dstOp.reg);
    const auto srcVal = (srcOp.reg == REG_UNDEF || src == M_IM)
                                ? srcOp.val16
                                : encodeWorkRegAddr(isSuper8(), srcOp.reg);
    if (src == M_IM && overflowUint8(srcOp.val16))
        setErrorIf(srcOp, OVERFLOW_RANGE);
    insn.emitOperand8(dstFirst ? dstVal : srcVal);
    insn.emitOperand8(dstFirst ? srcVal : dstVal);
}

void AsmZ8::encodePostByte(
        AsmInsn &insn, const Operand &dstOp, const Operand &srcOp, const Operand &extOp) {
    const auto dst = insn.dst();
    const auto src = insn.src();
    const auto post = insn.postFormat();
    if (dst == M_IM) {  // P2: SRP, SRP0, SPR1
        if (dstOp.val16 >= 0x100)
            setErrorIf(dstOp, OVERFLOW_RANGE);
        uint8_t srp = dstOp.val16;
        if (post == PF2_0) {  // SRP
            if (srp & 0xF) {
                setErrorIf(dstOp, OPERAND_NOT_ALLOWED);  // TODO: Should be warning.
                srp &= ~0xF;
            }
        } else {  // SRP0, SRP1
            if (srp & 7) {
                setErrorIf(dstOp, OPERAND_NOT_ALLOWED);  // TODO: Should be warning.
                srp &= ~7;
            }
            if (post == PF2_1)
                srp |= 1;
            if (post == PF2_2)
                srp |= 2;
        }
        insn.emitOperand8(srp);
        return;
    }
    if (dst == M_DA || src == M_DA) {  // P4: LDC, LDE
        const auto reg = (dst == M_DA) ? srcOp.reg : dstOp.reg;
        uint8_t opr1 = encodeRegName(reg) << 4;
        if (post == PF4_1)
            opr1 |= 1;
        insn.emitOperand8(opr1);
        insn.emitOperand16Le(dst == M_DA ? dstOp.val16 : srcOp.val16);
        return;
    }
    if (dst == M_Irr || src == M_Irr) {  // P1: LDCxx, LDExx
        const auto regL = (dst == M_Irr) ? dstOp.reg : srcOp.reg;
        const auto regH = (dst == M_Irr) ? srcOp.reg : dstOp.reg;
        uint8_t opr1 = (encodeRegName(regH) << 4) | encodeRegName(regL);
        if (post == PF1_1)
            opr1 |= 1;
        insn.emitOperand8(opr1);
        return;
    }
    if (dst == M_XL || src == M_XL || dst == M_XS || src == M_XS) {  // P1
        const auto dstIdx = (dst == M_XL || dst == M_XS);
        const auto &idx = dstIdx ? dstOp : srcOp;
        const auto &op = dstIdx ? srcOp : dstOp;
        uint8_t opr1 = (encodeRegName(op.reg) << 4) | encodeRegName(idx.reg);
        if (post == PF1_1)
            opr1 |= 1;
        insn.emitOperand8(opr1);
        if (dst == M_XL || src == M_XL)
            insn.emitOperand16Le(idx.val16);
        else
            insn.emitOperand8(idx.val16);
        return;
    }
    if (dst == M_RA) {  // P1: BTFRF, BTJRT
        auto bitNo = extOp.val16;
        if (bitNo >= 8) {
            setErrorIf(extOp, ILLEGAL_BIT_NUMBER);
            bitNo &= 7;
        }
        uint8_t opr1 = (encodeRegName(srcOp.reg) << 4) | (bitNo << 1);
        if (post == PF1_1)
            opr1 |= 1;
        insn.emitOperand8(opr1);
        encodeRelative(insn, dstOp);
        return;
    }
    const auto ext = insn.ext();
    if (ext == M_NONE) {  // P1: BITC, BITR, BITS
        auto bitNo = srcOp.val16;
        if (bitNo >= 8) {
            setErrorIf(srcOp, ILLEGAL_BIT_NUMBER);
            bitNo &= 7;
        }
        uint8_t opr1 = (encodeRegName(dstOp.reg) << 4) | (bitNo << 1);
        if (post == PF1_1)
            opr1 |= 1;
        insn.emitOperand8(opr1);
        return;
    }
    // P1: LDB, BAND, BOR, BXOR
    const auto reg = (dst == M_r) ? dstOp.reg : extOp.reg;
    const auto regAddr = (dst == M_r) ? srcOp.val16 : dstOp.val16;
    const auto &bitOp = (dst == M_r) ? extOp : srcOp;
    auto bitNo = bitOp.val16;
    if (bitNo >= 8) {
        setErrorIf(bitOp, ILLEGAL_BIT_NUMBER);
        bitNo &= 7;
    }
    uint8_t opr1 = (encodeRegName(reg) << 4) | (bitNo << 1);
    if (post == PF1_1)
        opr1 |= 1;
    insn.emitOperand8(opr1);
    insn.emitOperand8(regAddr);
}

Error AsmZ8::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p)) {
        op.mode = M_NONE;
        return OK;
    }

    op.cc = parseCcName(p);
    if (op.cc != CC_UNDEF) {
        op.mode = M_cc;
        scan = p;
        return OK;
    }

    if (p.expect('#')) {
        op.val16 = parseExpr16(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_IM;
        scan = p;
        return OK;
    }

    const auto indir = p.expect('@');
    if (indir && isspace(*p))
        return op.setError(UNKNOWN_OPERAND);

    op.reg = parseRegName(p);
    if (op.reg != REG_UNDEF) {
        if (op.reg == REG_ILLEGAL)
            return op.setError(ILLEGAL_REGISTER);
        const auto pair = isPairReg(op.reg);
        if (indir) {
            op.mode = pair ? M_Irr : M_Ir;
        } else {
            op.mode = pair ? M_rr : M_r;
        }
        op.val16 = encodeWorkRegAddr(isSuper8(), op.reg);
        scan = p;
        return OK;
    }

    const auto forceRegAddr = p.expect('>');
    if (forceRegAddr && isspace(*p))
        return op.setError(UNKNOWN_OPERAND);
    const int32_t val32 = parseExpr32(p, op);
    if (op.hasError())
        return getError();
    if (p.skipSpaces().expect('(')) {
        if (indir || forceRegAddr)
            op.setError(UNKNOWN_OPERAND);
        op.reg = parseRegName(p);
        if (op.reg == REG_UNDEF) {
            const auto val16 = parseExpr16(p, op, ')');
            if (op.hasError())
                return getError();
            if (!isWorkReg(val16))
                return op.setError(UNKNOWN_OPERAND);
            op.reg = decodeRegNum(val16 & 0xF);
        }
        if (!p.skipSpaces().expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        scan = p;
        if (val32 >= -128 && val32 < 128) {
            op.mode = M_XS;
        } else {
            op.mode = M_XL;
        }
        op.val16 = val32;
        return OK;
    }
    if (val32 < 0)
        op.setErrorIf(OVERFLOW_RANGE);
    scan = p;
    op.val16 = val32;
    if (indir) {
        if (op.val16 >= 0x100)
            op.setErrorIf(OVERFLOW_RANGE);
        if (!forceRegAddr && isWorkReg(op.val16)) {
            op.mode = (op.val16 & 1) == 0 ? M_IWW : M_IW;
            op.reg = decodeRegNum(op.val16 & 0xF);
            return OK;
        }
        op.mode = (op.val16 & 1) == 0 ? M_IRR : M_IR;
        return OK;
    }
    if (forceRegAddr) {
        if (op.val16 >= 0x100)
            op.setErrorIf(OVERFLOW_RANGE);
        op.mode = (op.val16 & 1) == 0 ? M_RR : M_R;
        return OK;
    }
    if (op.val16 >= 0x100) {
        op.mode = M_DA;
        return OK;
    }
    if (isWorkReg(op.val16)) {
        op.mode = (op.val16 & 1) == 0 ? M_WW : M_W;
        op.reg = decodeRegNum(op.val16 & 0xF);
        return OK;
    }
    op.mode = (op.val16 & 1) == 0 ? M_RR : M_R;
    return OK;
}

Error AsmZ8::setRp(StrScanner &scan, IntOption<AsmZ8>::Setter setter) {
    auto p = scan.skipSpaces();
    const int32_t rp = parseExpr32(p, *this);
    if (isOK()) {
        const auto error = (this->*setter)(rp);
        if (error)
            return setError(scan, error);
        scan = p;
        return OK;
    }
    setError(scan, OPERAND_NOT_ALLOWED);
    return OK;
}

Error AsmZ8::processPseudo(StrScanner &scan, Insn &insn) {
    if (strcasecmp_P(insn.name(), OPT_INT_SETRP) == 0)
        return setRp(scan, &AsmZ8::setRegPointer);
    if (isSuper8()) {
        if (strcasecmp_P(insn.name(), OPT_INT_SETRP0) == 0)
            return setRp(scan, &AsmZ8::setRegPointer0);
        if (strcasecmp_P(insn.name(), OPT_INT_SETRP1) == 0)
            return setRp(scan, &AsmZ8::setRegPointer1);
    }
    return Assembler::processPseudo(scan, insn);
}

Error AsmZ8::encodeImpl(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    Operand dstOp, srcOp, extOp;
    if (parseOperand(scan, dstOp) && dstOp.hasError())
        return setError(dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, srcOp) && srcOp.hasError())
            return setError(srcOp);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
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
    const auto dst = insn.dst();
    const auto src = insn.src();

    if (dst == M_NONE) {
        ;
    } else if (insn.postFormat() != PF_NONE) {
        encodePostByte(insn, dstOp, srcOp, extOp);
    } else {
        if (insn.opCode() == TableZ8::SRP && (dstOp.val16 & 0xF) != 0) {
            setErrorIf(dstOp, OPERAND_NOT_ALLOWED);
            dstOp.val16 &= ~0xF;
        }
        if (dst == M_DA || src == M_DA) {
            encodeAbsolute(insn, dstOp, srcOp);
        } else if (dst == M_RA || src == M_RA) {
            if (dst == M_cc)
                insn.embed(encodeCcName(dstOp.cc) << 4);
            if (dst == M_r)
                insn.embed(encodeRegName(dstOp.reg) << 4);
            encodeRelative(insn, dst == M_RA ? dstOp : srcOp);
        } else if (dst == M_X || src == M_X) {
            encodeIndexed(insn, dstOp, srcOp);
        } else if (dst == M_Irr || src == M_Irr) {
            encodeIndirectRegPair(insn, dstOp, srcOp);
        } else if (AsmInsn::operandInOpCode(insn.opCode())) {
            encodeInOpCode(insn, dstOp, srcOp);
        } else if (src == M_NONE) {
            encodeOperand(insn, dst, dstOp);
        } else {
            encodeMultiOperands(insn, dstOp, srcOp, extOp);
        }
    }
    insn.emitInsn();
    return setErrorIf(insn);
}

}  // namespace z8
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
