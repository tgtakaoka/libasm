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

#include <ctype.h>

namespace libasm {
namespace z8 {

const char AsmZ8::OPT_INT_SETRP[] PROGMEM = "setrp";
const char AsmZ8::OPT_INT_SETRP0[] PROGMEM = "setrp0";
const char AsmZ8::OPT_INT_SETRP1[] PROGMEM = "setrp1";

bool AsmZ8::setRegPointer(int16_t rp) {
    if (rp < 0) {
        setRegPointer0(rp);
        setRegPointer1(rp);
        return true;
    }
    return setRegPointer0(rp) && setRegPointer1(rp + 8);
}

bool AsmZ8::setRegPointer0(int16_t rp0) {
    if (rp0 >= 0 && (rp0 & ~0xF8))
        return false;
    _regPointer0 = rp0;
    return true;
}

bool AsmZ8::setRegPointer1(int16_t rp1) {
    if (rp1 >= 0 && (rp1 & ~0xF8))
        return false;
    _regPointer1 = rp1;
    return true;
}

bool AsmZ8::isWorkReg(uint8_t regAddr) const {
    const uint8_t regPage = (regAddr & 0xF8);
    if (_regPointer0 >= 0 && regPage == _regPointer0)
        return true;
    if (_regPointer1 >= 0 && regPage == _regPointer1)
        return true;
    return false;
}

Error AsmZ8::encodeOperand(InsnZ8 &insn, const AddrMode mode, const Operand &op) {
    if (mode == M_NO)
        return getError();
    if (op.reg != REG_UNDEF && (mode == M_R || mode == M_IR || mode == M_IRR)) {
        insn.emitByte(RegZ8::encodeWorkRegAddr(op.reg));
        return getError();
    }
    if ((mode == M_IM && !overflowUint8(op.val16)) || op.val16 < 0x100) {
        insn.emitByte(op.val16);
        return getError();
    }
    return setError(op, OVERFLOW_RANGE);
}

Error AsmZ8::encodeAbsolute(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp) {
    const AddrMode dst = insn.dstMode();
    const Operand &op = (dst == M_DA) ? dstOp : srcOp;
    if (dst == M_cc)
        insn.embed(RegZ8::encodeCcName(dstOp.cc) << 4);
    insn.emitInsn();
    insn.emitUint16(op.val16);
    return op.getError();
}

Error AsmZ8::encodeRelative(InsnZ8 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + insn.length() + 1;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    const Config::ptrdiff_t delta = target - base;
    if (overflowRel8(delta))
        return setError(op, OPERAND_TOO_FAR);
    insn.emitByte(delta);
    return op.getError();
}

Error AsmZ8::encodeIndexed(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp) {
    const AddrMode dst = insn.dstMode();
    const Operand &op = (dst == M_X) ? dstOp : srcOp;
    const RegName reg = (dst == M_X) ? srcOp.reg : dstOp.reg;
    if (overflowUint8(static_cast<uint16_t>(op.val16)))
        return setError(op, OVERFLOW_RANGE);
    const uint8_t opr1 = RegZ8::encodeRegName(op.reg) | (RegZ8::encodeRegName(reg) << 4);
    insn.emitInsn();
    insn.emitByte(opr1);
    insn.emitByte(op.val16);
    return getError();
}

Error AsmZ8::encodeIndirectRegPair(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp) {
    const AddrMode dst = insn.dstMode();
    const RegName pair = (dst == M_Irr) ? dstOp.reg : srcOp.reg;
    const RegName reg = (dst == M_Irr) ? srcOp.reg : dstOp.reg;
    const uint8_t opr = RegZ8::encodeRegName(pair) | (RegZ8::encodeRegName(reg) << 4);
    insn.emitInsn();
    insn.emitByte(opr);
    return getError();
}

Error AsmZ8::encodeInOpCode(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp) {
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    const RegName reg = (dst == M_r) ? dstOp.reg : srcOp.reg;
    const Operand &op = (dst == M_r) ? srcOp : dstOp;
    const AddrMode mode = (dst == M_r) ? src : dst;
    insn.embed(RegZ8::encodeRegName(reg) << 4);
    insn.emitInsn();
    return encodeOperand(insn, mode, op);
}

Error AsmZ8::encodeMultiOperands(
        InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp, const Operand &extOp) {
    insn.emitInsn();
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    if (src == M_Ir && insn.extMode() == M_RA) {
        const uint8_t opr1 =
                RegZ8::encodeRegName(dstOp.reg) | (RegZ8::encodeRegName(srcOp.reg) << 4);
        insn.emitByte(opr1);
        return encodeRelative(insn, extOp);
    }
    if (dst == M_r && (src == M_r || src == M_Ir)) {
        const uint8_t opr =
                RegZ8::encodeRegName(srcOp.reg) | (RegZ8::encodeRegName(dstOp.reg) << 4);
        insn.emitByte(opr);
        return getError();
    }
    if (dst == M_RR && src == M_IML) {
        insn.emitByte(dstOp.val16);
        insn.emitUint16(srcOp.val16);
        return getError();
    }
    if (dst == M_r || dst == M_Ir) {
        const uint8_t opr1 =
                RegZ8::encodeRegName(srcOp.reg) | (RegZ8::encodeRegName(dstOp.reg) << 4);
        insn.emitByte(opr1);
        return setOK();
    }
    const bool dstSrc = insn.dstSrc();
    const uint8_t dstVal =
            (dstOp.reg == REG_UNDEF) ? dstOp.val16 : RegZ8::encodeWorkRegAddr(dstOp.reg);
    const uint8_t srcVal = (srcOp.reg == REG_UNDEF || src == M_IM)
                                   ? srcOp.val16
                                   : RegZ8::encodeWorkRegAddr(srcOp.reg);
    if (src == M_IM && overflowUint8(srcOp.val16))
        return setError(srcOp, OVERFLOW_RANGE);
    insn.emitByte(dstSrc ? dstVal : srcVal);
    insn.emitByte(dstSrc ? srcVal : dstVal);
    return getError();
}

Error AsmZ8::encodePostByte(
        InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp, const Operand &extOp) {
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    const PostFormat post = insn.postFormat();
    if (dst == M_IM) {  // P2: SRP, SRP0, SPR1
        if (dstOp.val16 >= 0x100)
            return setError(dstOp, OVERFLOW_RANGE);
        uint8_t srp = dstOp.val16;
        if (post == P2_0) {  // SRP
            if (srp & 0xf)
                return setError(srcOp, OPERAND_NOT_ALLOWED);  // TODO: Should be warning.
        } else {                                              // SRP0, SRP1
            if (srp & 0x7)
                return setError(srcOp, OPERAND_NOT_ALLOWED);  // TODO: Should be warning.
            if (post == P2_1)
                srp |= 1;
            if (post == P2_2)
                srp |= 2;
        }
        insn.emitInsn();
        insn.emitByte(srp);
        return getError();
    }
    if (dst == M_DA || src == M_DA) {  // P4: LDC, LDE
        const RegName reg = (dst == M_DA) ? srcOp.reg : dstOp.reg;
        uint8_t opr1 = RegZ8::encodeRegName(reg) << 4;
        if (post == P4_1)
            opr1 |= 1;
        insn.emitInsn();
        insn.emitByte(opr1);
        insn.emitUint16Le(dst == M_DA ? dstOp.val16 : srcOp.val16);
        return getError();
    }
    if (dst == M_Irr || src == M_Irr) {  // P1: LDCxx, LDExx
        const RegName regL = (dst == M_Irr) ? dstOp.reg : srcOp.reg;
        const RegName regH = (dst == M_Irr) ? srcOp.reg : dstOp.reg;
        uint8_t opr1 = (RegZ8::encodeRegName(regH) << 4) | RegZ8::encodeRegName(regL);
        if (post == P1_1)
            opr1 |= 1;
        insn.emitInsn();
        insn.emitByte(opr1);
        return getError();
    }
    if (dst == M_XL || src == M_XL || dst == M_XS || src == M_XS) {  // P1
        const bool dstIdx = (dst == M_XL || dst == M_XS);
        const Operand &idx = dstIdx ? dstOp : srcOp;
        const Operand &op = dstIdx ? srcOp : dstOp;
        uint8_t opr1 = (RegZ8::encodeRegName(op.reg) << 4) | RegZ8::encodeRegName(idx.reg);
        if (post == P1_1)
            opr1 |= 1;
        insn.emitInsn();
        insn.emitByte(opr1);
        if (dst == M_XL || src == M_XL)
            insn.emitUint16Le(idx.val16);
        else
            insn.emitByte(idx.val16);
        return getError();
    }
    if (dst == M_RA) {  // P1: BTFRF, BTJRT
        if (extOp.val16 >= 8)
            return setError(extOp, ILLEGAL_BIT_NUMBER);
        uint8_t opr1 = (RegZ8::encodeRegName(srcOp.reg) << 4) | (extOp.val16 << 1);
        if (post == P1_1)
            opr1 |= 1;
        insn.emitInsn();
        insn.emitByte(opr1);
        return encodeRelative(insn, dstOp);
    }
    const AddrMode ext = insn.extMode();
    if (ext == M_NO) {  // P1: BITC, BITR, BITS
        if (srcOp.val16 >= 8)
            return setError(srcOp, ILLEGAL_BIT_NUMBER);
        uint8_t opr1 = (RegZ8::encodeRegName(dstOp.reg) << 4) | (srcOp.val16 << 1);
        if (post == P1_1)
            opr1 |= 1;
        insn.emitInsn();
        insn.emitByte(opr1);
        return getError();
    }
    // P1: LDB, BAND, BOR, BXOR
    const RegName reg = (dst == M_r) ? dstOp.reg : extOp.reg;
    const uint8_t regAddr = (dst == M_r) ? srcOp.val16 : dstOp.val16;
    const Operand &bitOp = (dst == M_r) ? extOp : srcOp;
    if (bitOp.val16 >= 8)
        return setError(bitOp, ILLEGAL_BIT_NUMBER);
    uint8_t opr1 = (RegZ8::encodeRegName(reg) << 4) | (bitOp.val16 << 1);
    if (post == P1_1)
        opr1 |= 1;
    insn.emitInsn();
    insn.emitByte(opr1);
    insn.emitByte(regAddr);
    return getError();
}

Error AsmZ8::setRp(StrScanner &scan, bool (AsmZ8::*set)(int16_t)) {
    StrScanner p(scan.skipSpaces());
    const uint16_t rp = parseExpr16(p, *this);
    if (isOK() && (this->*set)(rp)) {
        scan = p;
        return OK;
    }
    setError(scan, OPERAND_NOT_ALLOWED);
    return OK;
}

Error AsmZ8::processPseudo(StrScanner &scan, const char *name) {
    if (strcasecmp_P(name, OPT_INT_SETRP) == 0)
        return setRp(scan, &AsmZ8::setRegPointer);
    if (TableZ8::TABLE.isSuper8()) {
        if (strcasecmp_P(name, OPT_INT_SETRP0) == 0)
            return setRp(scan, &AsmZ8::setRegPointer0);
        if (strcasecmp_P(name, OPT_INT_SETRP1) == 0)
            return setRp(scan, &AsmZ8::setRegPointer1);
    }
    return UNKNOWN_INSTRUCTION;
}

Error AsmZ8::parseOperand(StrScanner &scan, Operand &op) {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p)) {
        op.mode = M_NO;
        return OK;
    }

    op.cc = RegZ8::parseCcName(p);
    if (op.cc != CC_UNDEF) {
        op.mode = M_cc;
        scan = p;
        return OK;
    }

    if (p.expect('#')) {
        op.val16 = parseExpr16(p, op);
        if (parserError())
            return getError();
        op.mode = M_IM;
        scan = p;
        return OK;
    }

    const bool indir = p.expect('@');
    if (indir && isspace(*p))
        return setError(p, UNKNOWN_OPERAND);

    op.reg = RegZ8::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        const bool pair = RegZ8::isPairReg(op.reg);
        if (indir) {
            op.mode = pair ? M_Irr : M_Ir;
        } else {
            op.mode = pair ? M_rr : M_r;
        }
        op.val16 = RegZ8::encodeWorkRegAddr(op.reg);
        scan = p;
        return OK;
    }

    const bool forceRegAddr = p.expect('>');
    if (forceRegAddr && isspace(*p))
        return setError(p, UNKNOWN_OPERAND);
    const int32_t val32 = parseExpr32(p, op);
    if (parserError())
        return getError();
    if (p.skipSpaces().expect('(')) {
        if (indir || forceRegAddr)
            setError(p, UNKNOWN_OPERAND);
        op.reg = RegZ8::parseRegName(p);
        if (op.reg == REG_UNDEF) {
            const uint16_t val16 = parseExpr16(p, op);
            if (parserError())
                return getError();
            if (!isWorkReg(val16))
                return setError(op, UNKNOWN_OPERAND);
            op.reg = RegZ8::decodeRegNum(val16 & 0xF);
        }
        if (!p.skipSpaces().expect(')'))
            return setError(p, MISSING_CLOSING_PAREN);
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
        return setError(op, OVERFLOW_RANGE);
    scan = p;
    op.val16 = val32;
    if (indir) {
        if (op.val16 >= 0x100)
            return setError(op, OVERFLOW_RANGE);
        if (!forceRegAddr && isWorkReg(op.val16)) {
            op.mode = (op.val16 & 1) == 0 ? M_IWW : M_IW;
            op.reg = RegZ8::decodeRegNum(op.val16 & 0xF);
            return OK;
        }
        op.mode = (op.val16 & 1) == 0 ? M_IRR : M_IR;
        return OK;
    }
    if (forceRegAddr) {
        if (op.val16 >= 0x100)
            return setError(op, OVERFLOW_RANGE);
        op.mode = (op.val16 & 1) == 0 ? M_RR : M_R;
        return OK;
    }
    if (op.val16 >= 0x100) {
        op.mode = M_DA;
        return OK;
    }
    if (isWorkReg(op.val16)) {
        op.mode = (op.val16 & 1) == 0 ? M_WW : M_W;
        op.reg = RegZ8::decodeRegNum(op.val16 & 0xF);
        return OK;
    }
    op.mode = (op.val16 & 1) == 0 ? M_RR : M_R;
    return OK;
}

Error AsmZ8::encode(StrScanner &scan, Insn &_insn) {
    InsnZ8 insn(_insn);
    insn.setName(_parser.readSymbol(scan));

    if (processPseudo(scan, insn.name()) == OK)
        return getError();

    Operand dstOp, srcOp, extOp;
    if (parseOperand(scan, dstOp))
        return getError();
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, srcOp))
            return getError();
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, extOp))
            return getError();
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(dstOp);
    setErrorIf(srcOp);
    setErrorIf(extOp);

    insn.setAddrMode(dstOp.mode, srcOp.mode, extOp.mode);
    if (TableZ8::TABLE.searchName(insn))
        return setError(TableZ8::TABLE.getError());
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();

    if (dst == M_NO) {
        insn.emitInsn();
        return setOK();
    }
    if (insn.postFormat() != P0)
        return encodePostByte(insn, dstOp, srcOp, extOp);
    // TODO: This should be warning
    if (insn.opCode() == TableZ8::SRP && (dstOp.val16 & 0xF) != 0)
        return setError(dstOp, OPERAND_NOT_ALLOWED);
    if (dst == M_DA || src == M_DA)
        return encodeAbsolute(insn, dstOp, srcOp);
    if (dst == M_RA || src == M_RA) {
        if (dst == M_cc)
            insn.embed(RegZ8::encodeCcName(dstOp.cc) << 4);
        if (dst == M_r)
            insn.embed(RegZ8::encodeRegName(dstOp.reg) << 4);
        insn.emitInsn();
        return encodeRelative(insn, dst == M_RA ? dstOp : srcOp);
    }
    if (dst == M_X || src == M_X)
        return encodeIndexed(insn, dstOp, srcOp);
    if (dst == M_Irr || src == M_Irr)
        return encodeIndirectRegPair(insn, dstOp, srcOp);
    if (InsnZ8::operandInOpCode(insn.opCode()))
        return encodeInOpCode(insn, dstOp, srcOp);
    if (src == M_NO) {
        insn.emitInsn();
        return encodeOperand(insn, dst, dstOp);
    }
    return encodeMultiOperands(insn, dstOp, srcOp, extOp);
}

}  // namespace z8
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
