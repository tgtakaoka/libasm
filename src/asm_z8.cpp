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

const ValueParser::Plugins &AsmZ8::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return ZilogNumberParser::singleton(); }
        const SymbolParser &symbol() const override { return _symbol; }
        const LetterParser &letter() const override { return ZilogLetterParser::singleton(); }
        const OperatorParser &operators() const override {
            return ZilogOperatorParser::singleton();
        }
        const SimpleSymbolParser _symbol{PSTR_UNDER_DOT_DOLLER_QUESTION};
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
        op.regAt = p;
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

void AsmInsn::emitInsn() {
    emitByte(opCode(), 0);
    if (postFormat() != PF_NONE)
        emitOperand(postVal(), OP_B1LO);
}

void AsmInsn::emitOperand(uint16_t val16, OprPos pos) {
    switch (pos) {
    case OP_CODE:
        embed(val16 << 4);
        break;
    case OP_BYT1:
        emitByte(val16, 1);
        break;
    case OP_B1HI:
        emitByte(bytes()[1] | (val16 << 4), 1);
        break;
    case OP_B1LO:
        emitByte(bytes()[1] | (val16 & 0xF), 1);
        break;
    case OP_BYT2:
        emitByte(val16, 2);
        break;
    case OP_W1BE:
        emitUint16(val16, 1);
        break;
    case OP_W2BE:
        emitUint16(val16, 2);
        break;
    case OP_W2LE:
        emitUint16Le(val16, 2);
        break;
    default:
        break;
    }
}

void AsmZ8::encodeRelative(AsmInsn &insn, OprPos pos, const Operand &op) const {
    const auto base = insn.address() + (pos == OP_BYT1 ? 2 : 3);
    const auto target = op.getError() ? base : op.val16;
    const auto delta = branchDelta(base, target, insn, op);
    if (overflowInt8(delta))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand(delta, pos);
}

void AsmZ8::encodeIndexed(AsmInsn &insn, AddrMode mode, OprPos pos, const Operand &op) const {
    if (mode == M_X && overflowUint8(op.val16))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    if (mode == M_XS && overflowInt8(static_cast<int8_t>(op.val16)))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    if (mode == M_XL && op.reg == REG_RR0)
        insn.setErrorIf(op.regAt, REGISTER_NOT_ALLOWED);
    insn.emitOperand(encodeRegName(op.reg), OP_B1LO);
    insn.emitOperand(op.val16, pos);
}

void AsmZ8::encodeRegAddr(AsmInsn &insn, OprPos pos, const Operand &op) const {
    const auto addr =
            (op.reg != REG_UNDEF && _reg_alias) ? encodeWorkRegAddr(isSuper8(), op.reg) : op.val16;
    insn.emitOperand(addr, pos);
}

void AsmZ8::encodeImmediate(AsmInsn &insn, AddrMode mode, OprPos pos, const Operand &op) const {
    if (mode == M_IM && overflowUint8(op.val16))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    auto val16 = op.val16;
    if (insn.opCode() == TableZ8::SRP) {
        const auto postFormat = insn.postFormat();
        if (postFormat == PF_NONE) {  // Z8
            if ((val16 & ~0xF0) != 0)
                insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        } else {                                              // Z88
            if (postFormat == PF2_0 && (val16 & ~0xF0) != 0)  // SRP
                insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
            if (postFormat == PF2_1 && (val16 & ~0xF8) != 0)  // SRP1
                insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
            if (postFormat == PF2_2 && (val16 & ~0xF8) != 0)  // SRP0
                insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
            val16 &= ~3;
        }
    }
    if (mode == M_IMb) {
        if (overflowUint(val16, 3))
            insn.setErrorIf(op, ILLEGAL_BIT_NUMBER);
        val16 <<= 1;
    }
    insn.emitOperand(val16, pos);
}

void AsmZ8::encodeOperand(AsmInsn &insn, AddrMode mode, OprPos pos, const Operand &op) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_IM:
    case M_IMb:
    case M_IML:
        encodeImmediate(insn, mode, pos, op);
        break;
    case M_r:
    case M_Ir:
    case M_rr:
    case M_Irr:
        insn.emitOperand(op.val16 & 0xF, pos);
        break;
    case M_R:
    case M_IR:
    case M_RR:
    case M_IRR:
        encodeRegAddr(insn, pos, op);
        break;
    case M_RA:
        encodeRelative(insn, pos, op);
        break;
    case M_DA:
        insn.emitOperand(op.val16, pos);
        break;
    case M_X:
    case M_XS:
    case M_XL:
        encodeIndexed(insn, mode, pos, op);
        break;
    case M_cc:
        insn.emitOperand(encodeCcName(op.cc), pos);
        break;
    default:
        break;
    }
}

Error AsmZ8::encodeImpl(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
        return setError(insn.dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
            return setError(insn.srcOp);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, insn.extOp) && insn.extOp.hasError())
            return setError(insn.extOp);
        scan.skipSpaces();
    }

    const auto error = TABLE.searchName(cpuType(), insn);
    if (error)
        return setError(insn.dstOp, error);

    encodeOperand(insn, insn.dst(), insn.dstPos(), insn.dstOp);
    encodeOperand(insn, insn.src(), insn.srcPos(), insn.srcOp);
    encodeOperand(insn, insn.ext(), insn.extPos(), insn.extOp);
    insn.emitInsn();
    return setError(insn);
}

}  // namespace z8
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
