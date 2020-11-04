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
#include "reg_z8.h"

namespace libasm {
namespace z8 {

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

Error AsmZ8::encodeOperand(
    InsnZ8 &insn, const AddrMode mode, const Operand &op) {
    if (mode == M_NO) return getError();
    if (op.reg != REG_UNDEF
        && (mode == M_R || mode == M_IR || mode == M_IRR)) {
        insn.emitByte(RegZ8::encodeWorkRegAddr(op.reg));
        return getError();
    }
    if (op.val16 >= 0x100) return setError(OVERFLOW_RANGE);
    insn.emitByte(op.val16);
    return getError();
}

Error AsmZ8::encodeAbsolute(
    InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp) {
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
    if (delta < -128 || delta >= 128) return setErrorIf(OPERAND_TOO_FAR);
    insn.emitByte(delta);
    return op.getError();
}

Error AsmZ8::encodeIndexed(
    InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp) {
    const AddrMode dst = insn.dstMode();
    const RegName index = (dst == M_X) ? dstOp.reg : srcOp.reg;
    const int16_t disp16 = static_cast<int16_t>(
        (dst == M_X) ? dstOp.val16 : srcOp.val16);
    const RegName reg = (dst == M_X) ? srcOp.reg : dstOp.reg;
    if (disp16 < -128 || disp16 >= 0x100) return setError(OVERFLOW_RANGE);
    const uint8_t opr1 =
        RegZ8::encodeRegName(index) | (RegZ8::encodeRegName(reg) << 4);
    insn.emitInsn();
    insn.emitByte(opr1);
    insn.emitByte(disp16);
    return getError();
}

Error AsmZ8::encodeIndirectRegPair(
    InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp) {
    const AddrMode dst = insn.dstMode();
    const RegName pair = (dst == M_Irr) ? dstOp.reg : srcOp.reg;
    const RegName reg = (dst == M_Irr) ? srcOp.reg : dstOp.reg;
    const uint8_t opr =
        RegZ8::encodeRegName(pair) | (RegZ8::encodeRegName(reg) << 4);
    insn.emitInsn();
    insn.emitByte(opr);
    return getError();
}

Error AsmZ8::encodeInOpCode(
    InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp) {
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
    InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp,
    const Operand &extOp) {
    insn.emitInsn();
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    if (src == M_Ir && insn.extMode() == M_RA) {
        const uint8_t opr1 = RegZ8::encodeRegName(dstOp.reg)
            | (RegZ8::encodeRegName(srcOp.reg) << 4);
        insn.emitByte(opr1);
        return encodeRelative(insn, extOp);
    }
    if (dst == M_r && (src == M_r || src == M_Ir)) {
        const uint8_t opr = RegZ8::encodeRegName(srcOp.reg)
            | (RegZ8::encodeRegName(dstOp.reg) << 4);
        insn.emitByte(opr);
        return getError();
    }
    if (dst == M_RR && src == M_IML) {
        insn.emitByte(dstOp.val16);
        insn.emitUint16(srcOp.val16);
        return getError();
    }
    if (dst == M_r || dst == M_Ir) {
        const uint8_t opr1 = RegZ8::encodeRegName(srcOp.reg)
            | (RegZ8::encodeRegName(dstOp.reg) << 4);
        insn.emitByte(opr1);
        return setOK();
    }
    const bool dstSrc = insn.dstSrc();
    const uint8_t dstVal = (dstOp.reg == REG_UNDEF) ? dstOp.val16
        : RegZ8::encodeWorkRegAddr(dstOp.reg);
    const uint8_t srcVal = (srcOp.reg == REG_UNDEF || src == M_IM) ? srcOp.val16
        : RegZ8::encodeWorkRegAddr(srcOp.reg);
    insn.emitByte(dstSrc ? dstVal : srcVal);
    insn.emitByte(dstSrc ? srcVal : dstVal);
    return getError();
}

Error AsmZ8::encodePostByte(
    InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp,
    const Operand &extOp) {
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    const PostFormat post = insn.postFormat();
    if (dst == M_IM) {          // P2: SRP, SRP0, SPR1
        uint8_t srp = dstOp.val16;
        if (post == P2_0) {     // SRP
            if (srp & 0xf)
                return setError(OPERAND_NOT_ALLOWED); // TODO: Should be warning.
        } else {                // SRP0, SRP1
            if (srp & 0x7)
                return setError(OPERAND_NOT_ALLOWED); // TODO: Should be warning.
            if (post == P2_1) srp |= 1;
            if (post == P2_2) srp |= 2;
        }
        insn.emitInsn();
        insn.emitByte(srp);
        return getError();
    }
    if (dst == M_DA || src == M_DA) { // P4: LDC, LDE
        const RegName reg = (dst == M_DA) ? srcOp.reg : dstOp.reg;
        uint8_t opr1 = RegZ8::encodeRegName(reg) << 4;
        if (post == P4_1) opr1 |= 1;
        insn.emitInsn();
        insn.emitByte(opr1);
        insn.emitUint16Le(dst == M_DA ? dstOp.val16 : srcOp.val16);
        return getError();
    }
    if (dst == M_Irr || src == M_Irr) { // P1: LDCxx, LDExx
        const RegName regL = (dst == M_Irr) ? dstOp.reg : srcOp.reg;
        const RegName regH = (dst == M_Irr) ? srcOp.reg : dstOp.reg;
        uint8_t opr1 = (RegZ8::encodeRegName(regH) << 4)
            | RegZ8::encodeRegName(regL);
        if (post == P1_1) opr1 |= 1;
        insn.emitInsn();
        insn.emitByte(opr1);
        return getError();
    }
    if (dst == M_XL || src == M_XL || dst == M_XS || src == M_XS) { // P1
        const bool dstIdx = (dst == M_XL || dst == M_XS);
        const Operand &idx = dstIdx ? dstOp : srcOp;
        const Operand &op  = dstIdx ? srcOp : dstOp;
        uint8_t opr1 = (RegZ8::encodeRegName(op.reg) << 4)
            | RegZ8::encodeRegName(idx.reg);
        if (post == P1_1) opr1 |= 1;
        insn.emitInsn();
        insn.emitByte(opr1);
        if (dst == M_XL || src == M_XL) insn.emitUint16Le(idx.val16);
        else insn.emitByte(idx.val16);
        return getError();
    }
    if (dst == M_RA) {          // P1: BTFRF, BTJRT
        if (extOp.val16 >= 8) return setError(ILLEGAL_BIT_NUMBER);
        uint8_t opr1 = (RegZ8::encodeRegName(srcOp.reg) << 4)
            | (extOp.val16 << 1);
        if (post == P1_1) opr1 |= 1;
        insn.emitInsn();
        insn.emitByte(opr1);
        return encodeRelative(insn, dstOp);
    }
    const AddrMode ext = insn.extMode();
    if (ext == M_NO) {          // P1: BITC, BITR, BITS
        if (srcOp.val16 >= 8) return setError(ILLEGAL_BIT_NUMBER);
        uint8_t opr1 = (RegZ8::encodeRegName(dstOp.reg) << 4)
            | (srcOp.val16 << 1);
        if (post == P1_1) opr1 |= 1;
        insn.emitInsn();
        insn.emitByte(opr1);
        return getError();
    }
    // P1: LDB, BAND, BOR, BXOR
    const RegName reg =     (dst == M_r) ? dstOp.reg : extOp.reg;
    const uint8_t regAddr = (dst == M_r) ? srcOp.val16 : dstOp.val16;
    const uint8_t bitPos =  (dst == M_r) ? extOp.val16 : srcOp.val16;
    if (bitPos >= 8) return setError(ILLEGAL_BIT_NUMBER);
    uint8_t opr1 = (RegZ8::encodeRegName(reg) << 4)
        | (bitPos << 1);
    if (post == P1_1) opr1 |= 1;
    insn.emitInsn();
    insn.emitByte(opr1);
    insn.emitByte(regAddr);
    return getError();
}

Error AsmZ8::setRp(
    const char *scan, InsnZ8 &insn, const /*PROGMEM*/ char *name,
    bool (AsmZ8::*set)(int16_t)) {
    if (strcasecmp_P(insn.name(), name)) return UNKNOWN_INSTRUCTION;
    const uint16_t rp = parseExpr16(scan);
    if (getError() != OK || !(this->*set)(rp))
        setError(OPERAND_NOT_ALLOWED);
    return OK;
}

static const char TEXT_SETRP[]  PROGMEM = "SETRP";
static const char TEXT_SETRP0[] PROGMEM = "SETRP0";
static const char TEXT_SETRP1[] PROGMEM = "SETRP1";

Error AsmZ8::processPseudo(const char *scan, InsnZ8 &insn) {
    insn.resetAddress(insn.address()); // make generated bytes zero.
    const char *p = skipSpaces(scan);
    if (setRp(p, insn, TEXT_SETRP, &AsmZ8::setRegPointer) == OK)
        return OK;
    if (TableZ8.isSuper8()) {
        if (setRp(p, insn, TEXT_SETRP0, &AsmZ8::setRegPointer0) == OK)
            return OK;
        if (setRp(p, insn, TEXT_SETRP1, &AsmZ8::setRegPointer1) == OK)
            return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

Error AsmZ8::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p)) {
        op.mode = M_NO;
        return OK;
    }

    op.cc = RegZ8::parseCcName(p);
    if (op.cc != CC_UNDEF) {
        _scan = p + RegZ8::ccNameLen(op.cc);
        op.mode = M_cc;
        return OK;
    }

    if (*p == '#') {
        op.val16 = parseExpr16(p + 1);
        if (parserError()) return getError();
        op.setError(getError());
        op.mode = M_IM;
        return OK;
    }

    const bool indir = (*p == '@');
    if (indir) {
        p++;
        if (isspace(*p)) return setError(UNKNOWN_OPERAND);
    }

    op.reg = RegZ8::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        _scan = p + RegZ8::regNameLen(op.reg);
        const bool pair = RegZ8::isPairReg(op.reg);
        if (indir) {
            op.mode = pair ? M_Irr : M_Ir;
        } else {
            op.mode = pair ? M_rr : M_r;
        }
        op.val16 = RegZ8::encodeWorkRegAddr(op.reg);
        return OK;
    }

    const bool forceRegAddr = (*p == '>');
    if (forceRegAddr) {
        p++;
        if (isspace(*p)) return setError(UNKNOWN_OPERAND);
    }
    const int32_t val32 = parseExpr32(p);
    if (parserError()) return getError();
    op.setError(getError());
    p = skipSpaces(_scan);
    if (*p == '(') {
        if (indir || forceRegAddr) setError(UNKNOWN_OPERAND);
        p++;
        op.reg = RegZ8::parseRegName(p);
        if (op.reg != REG_UNDEF) {
            p += RegZ8::regNameLen(op.reg);
        } else {
            const uint16_t val16 = parseExpr16(p);
            if (parserError()) return getError();
            if (!isWorkReg(val16)) return setError(UNKNOWN_OPERAND);
            p = _scan;
            op.reg = RegZ8::decodeRegNum(val16 & 0xF);
        }
        p = skipSpaces(p);
        if (*p != ')') return setError(MISSING_CLOSING_PAREN);
        _scan = p + 1;
        if (val32 >= -128 && val32 < 128) {
            op.mode = M_XS;
        } else {
            op.mode = M_XL;
        }
        op.val16 = val32;
        return OK;
    }
    if (val32 < 0) return setError(OVERFLOW_RANGE);
    op.val16 = val32;
    if (indir) {
        if (op.val16 >= 0x100) return setError(OVERFLOW_RANGE);
        if (!forceRegAddr && isWorkReg(op.val16)) {
            op.mode = (op.val16 & 1) == 0 ? M_IWW : M_IW;
            op.reg = RegZ8::decodeRegNum(op.val16 & 0xF);
            return OK;
        }
        op.mode = (op.val16 & 1) == 0 ? M_IRR : M_IR;
        return OK;
    }
    if (forceRegAddr) {
        if (op.val16 >= 0x100) return setError(OVERFLOW_RANGE);
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

Error AsmZ8::encode(Insn &_insn) {
    InsnZ8 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    if (processPseudo(endName, insn) == OK)
        return getError();

    Operand dstOp, srcOp, extOp;
    if (parseOperand(endName, dstOp)) return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        if (parseOperand(p + 1, srcOp)) return getError();
        p = skipSpaces(_scan);
    }
    if (*p == ',') {
        if (parseOperand(p + 1, extOp)) return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p)) return setError(GARBAGE_AT_END);
    setError(dstOp.getError());
    setErrorIf(srcOp.getError());
    setErrorIf(extOp.getError());

    insn.setAddrMode(dstOp.mode, srcOp.mode, extOp.mode);
    if (TableZ8.searchName(insn))
        return setError(TableZ8.getError());
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
        return setError(OPERAND_NOT_ALLOWED);
    if (dst == M_DA || src == M_DA)
        return encodeAbsolute(insn, dstOp, srcOp);
    if (dst == M_RA || src == M_RA) {
        if (dst == M_cc) insn.embed(RegZ8::encodeCcName(dstOp.cc) << 4);
        if (dst == M_r) insn.embed(RegZ8::encodeRegName(dstOp.reg) << 4);
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

} // namespace z8
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
