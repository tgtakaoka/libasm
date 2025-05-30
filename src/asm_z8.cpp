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
constexpr char OPT_BOOL_OPTIMIZE_INDEX[] PROGMEM = "optimize-index";
constexpr char OPT_DESC_OPTIMIZE_INDEX[] PROGMEM = "optimize zero index";
constexpr char OPT_INT_SETRP[]   PROGMEM = "setrp";
constexpr char OPT_INT_SETRP0[]  PROGMEM = "setrp0";
constexpr char OPT_INT_SETRP1[]  PROGMEM = "setrp1";

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
    static const struct final : ValueParser::ZilogPlugins {
        const SymbolParser &symbol() const override { return Z8SymbolParser::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmZ8::AsmZ8(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_reg_alias),
      Config(TABLE),
      _opt_reg_alias(
              this, &AsmZ8::setRegAlias, OPT_BOOL_ALIAS, OPT_DESC_ALIAS, &_opt_optimize_index),
      _opt_optimize_index(
              this, &AsmZ8::setOptimizeIndex, OPT_BOOL_OPTIMIZE_INDEX, OPT_DESC_OPTIMIZE_INDEX),
      _opt_setrp(this, &AsmZ8::setRegPointer, OPT_INT_SETRP),
      _opt_setrp0(this, &AsmZ8::setRegPointer0, OPT_INT_SETRP0),
      _opt_setrp1(this, &AsmZ8::setRegPointer1, OPT_INT_SETRP1) {
    reset();
}

void AsmZ8::reset() {
    Assembler::reset();
    setRegAlias(true);
    setRegPointer(-1);
    setOptimizeIndex(false);
}

Error AsmZ8::setRegAlias(bool enable) {
    _regAlias = enable;
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

Error AsmZ8::setOptimizeIndex(bool enable) {
    _optimizeIndex = enable;
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

    op.cc = parseCcName(p, parser());
    if (op.cc != CC_UNDEF) {
        op.mode = M_cc;
        scan = p;
        return OK;
    }

    if (p.expect('#')) {
        op.val = parseInteger(p, op);
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
        op.val.setUnsigned(encodeWorkRegAddr(isSuper8(), op.reg));
        scan = p;
        return OK;
    }

    const auto forceRegAddr = p.expect('>');
    if (forceRegAddr && isspace(*p))
        return op.setError(UNKNOWN_OPERAND);
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    const auto val16 = op.val.getUnsigned();
    if (p.skipSpaces().expect('(')) {
        if (indir || forceRegAddr)
            op.setError(UNKNOWN_OPERAND);
        op.regAt = p;
        op.reg = parseRegName(p);
        if (op.reg == REG_UNDEF) {
            const auto regno = parseInteger(p, op, ')');
            if (op.hasError())
                return op.getError();
            if (regno.overflow(UINT8_MAX) || !isWorkReg(regno.getUnsigned()))
                return op.setError(UNKNOWN_OPERAND);
            op.reg = decodeRegNum(regno.getUnsigned() & 0xF);
        }
        if (!p.skipSpaces().expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        scan = p;
        op.mode = op.val.overflowInt8() ? M_XL : M_XS;
        return OK;
    }
    scan = p;
    if (indir) {
        if (op.val.overflow(UINT8_MAX))
            op.setErrorIf(OVERFLOW_RANGE);
        if (!forceRegAddr && isWorkReg(val16)) {
            op.mode = (val16 & 1) == 0 ? M_IWW : M_IW;
            op.reg = decodeRegNum(val16 & 0xF);
            return OK;
        }
        op.mode = (val16 & 1) == 0 ? M_IRR : M_IR;
        return OK;
    }
    if (forceRegAddr) {
        if (op.val.overflow(UINT8_MAX))
            op.setErrorIf(OVERFLOW_RANGE);
        op.mode = (val16 & 1) == 0 ? M_RR : M_R;
        return OK;
    }
    if (op.val.overflow(UINT8_MAX)) {
        op.mode = M_DA;
        return OK;
    }
    if (isWorkReg(val16)) {
        op.mode = (val16 & 1) == 0 ? M_WW : M_W;
        op.reg = decodeRegNum(val16 & 0xF);
        return OK;
    }
    op.mode = (val16 & 1) == 0 ? M_RR : M_R;
    return OK;
}

Error AsmZ8::processPseudo(StrScanner &scan, Insn &insn) {
    const auto at = scan;
    if (strcasecmp_P(insn.name(), OPT_INT_SETRP) == 0) {
        const auto error = _opt_setrp.set(scan);
        return error ? insn.setErrorIf(at, error) : OK;
    }
    if (isSuper8()) {
        if (strcasecmp_P(insn.name(), OPT_INT_SETRP0) == 0) {
            const auto error = _opt_setrp0.set(scan);
            return error ? insn.setErrorIf(at, error) : OK;
        }
        if (strcasecmp_P(insn.name(), OPT_INT_SETRP1) == 0) {
            const auto error = _opt_setrp1.set(scan);
            return error ? insn.setErrorIf(at, error) : OK;
        }
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
    const auto target = op.getError() ? base : op.val.getUnsigned();
    const auto delta = branchDelta(base, target, insn, op);
    if (overflowDelta(delta, 8))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand(delta, pos);
}

void AsmZ8::encodeIndexed(AsmInsn &insn, AddrMode mode, OprPos pos, const Operand &op) const {
    if (mode == M_X && op.val.overflowUint8())
        insn.setErrorIf(op, OVERFLOW_RANGE);
    if (mode == M_XS && op.val.overflowInt8())
        insn.setErrorIf(op, OVERFLOW_RANGE);
    if (mode == M_XL && op.reg == REG_RR0)
        insn.setErrorIf(op.regAt, REGISTER_NOT_ALLOWED);
    if (_optimizeIndex && op.val.isZero()) {
        const auto opc = insn.opCode();
        auto pos = OP_B1LO;
        if (cpuType() == SUPER8) {
            if (opc == 0x87)           // LD r, x(r)
                insn.setOpCode(0xC7);  // LD r, @r
            if (opc == 0x97) {         // LD x(r), r
                insn.setOpCode(0xD7);  // LD @r, r
                insn.setSrcPos(OP_B1LO);
                pos = OP_B1HI;
            }
            if (opc == 0xE7)           // LDx r, xs(rr)
                insn.setOpCode(0xC3);  // LDx r, @rr
            if (opc == 0xF7)           // LDx xs(rr), r
                insn.setOpCode(0xD3);  // LDx @rr, r
        } else {
            if (opc == 0xC7)           // LD r, x(r)
                insn.setOpCode(0xE3);  // LD r, @r
            if (opc == 0xD7) {         // LD x(r), r
                insn.setOpCode(0xF3);  // LD @r, r
                insn.setSrcPos(OP_B1LO);
                pos = OP_B1HI;
            }
        }
        insn.emitOperand(encodeRegName(op.reg), pos);
    } else {
        insn.emitOperand(op.val.getUnsigned(), pos);
        insn.emitOperand(encodeRegName(op.reg), OP_B1LO);  // Index register
    }
}

void AsmZ8::encodeRegAddr(AsmInsn &insn, OprPos pos, const Operand &op) const {
    const auto addr = (op.reg != REG_UNDEF && _regAlias) ? encodeWorkRegAddr(isSuper8(), op.reg)
                                                         : op.val.getUnsigned();
    insn.emitOperand(addr, pos);
}

void AsmZ8::encodeImmediate(AsmInsn &insn, AddrMode mode, OprPos pos, const Operand &op) const {
    if ((mode == M_IM || mode == M_IA) && op.val.overflowUint8())
        insn.setErrorIf(op, OVERFLOW_RANGE);
    auto val16 = op.val.getUnsigned();
    if (mode == M_IA && val16 % 2)
        insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
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
        if (op.val.overflow(7))
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
    case M_IA:
        encodeImmediate(insn, mode, pos, op);
        break;
    case M_r:
    case M_Ir:
    case M_rr:
    case M_Irr:
        insn.emitOperand(op.val.getUnsigned() & 0xF, pos);
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
        insn.emitOperand(op.val.getUnsigned(), pos);
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

Error AsmZ8::encodeImpl(StrScanner &scan, Insn &_insn) const {
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

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.dstOp, insn);

    encodeOperand(insn, insn.dst(), insn.dstPos(), insn.dstOp);
    encodeOperand(insn, insn.src(), insn.srcPos(), insn.srcOp);
    encodeOperand(insn, insn.ext(), insn.extPos(), insn.extOp);
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace z8
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
