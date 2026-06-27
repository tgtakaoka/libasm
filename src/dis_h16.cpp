/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "dis_h16.h"
#include "reg_h16.h"
#include "table_h16.h"
#include "text_h16.h"

namespace libasm {
namespace h16 {

using namespace reg;
using namespace text::h16;

const ValueFormatter::Plugins &DisH16::defaultPlugins() {
    return ValueFormatter::Plugins::motorola();
}

DisH16::DisH16(const ValueFormatter::Plugins &plugins) : Disassembler(plugins), Config(TABLE) {
    reset();
}

namespace {

// Decode a 4-bit register field within a bank context (default Rn; CRn/PRn via
// a <CRn>/<PRn> prefix byte).
RegName decodeReg(uint8_t r, RegName bank = REG_R0) {
    return RegName(uint8_t(bank) | (r & 0x0F));
}

// Append the ":X" instruction-class suffix from the matched InsnClass.
void appendClassSuffix(DisInsn &insn, InsnClass ic) {
    auto &nb = insn.nameBuffer();
    switch (ic) {
    case IC_G:
        nb.letter(':').letter('G');
        break;
    case IC_Q:
        nb.letter(':').letter('Q');
        break;
    case IC_R:
        nb.letter(':').letter('R');
        break;
    case IC_RQ:
        nb.letter(':').letter('R').letter('Q');
        break;
    default:
        break;
    }
}

// Append the "/cc" condition suffix (SCB/SET/TRAP) from the pre-read cc byte.
void appendCcSuffix(DisInsn &insn) {
    if (insn.ccReg == reg::CC_UNDEF)
        return;
    auto &nb = insn.nameBuffer();
    nb.letter('/');
    outCcName(nb, insn.ccReg);
}

// Format a 16-bit register-list mask as comma-separated "Rn[-Rm]" ranges
// (e.g. 0x000F -> "R0-R3", 0x0025 -> "R0, R2, R5").
void outRegList(StrBuffer &out, uint16_t mask) {
    auto first = true;
    for (uint8_t n = 0; n < 16;) {
        if (!(mask & (1u << n))) {
            ++n;
            continue;
        }
        const uint8_t lo = n;
        while (n < 16 && (mask & (1u << n)))
            ++n;
        const uint8_t hi = n - 1;
        if (!first)
            out.comma();
        first = false;
        outRegName(out, decodeReg(lo));
        if (hi > lo) {
            out.letter('-');
            outRegName(out, decodeReg(hi));
        }
    }
}

void appendSizeSuffix(DisInsn &insn, OprSize sz) {
    auto &nb = insn.nameBuffer();
    nb.letter('.');
    switch (sz) {
    case SZ_BYTE:
        nb.letter('B');
        break;
    case SZ_WORD:
        nb.letter('W');
        break;
    case SZ_LONG:
        nb.letter('L');
        break;
    default:
        break;
    }
}

int32_t readSignedDisp(DisInsn &insn, uint8_t sdCode) {
    switch (sdCode) {
    case 0x01:
        return static_cast<int8_t>(insn.readByte());
    case 0x02:
        return static_cast<int16_t>(insn.readUint16());
    case 0x03:
        return static_cast<int32_t>(insn.readUint32());
    default:
        return 0;
    }
}

// Signed outHex width for a displacement of the given Sd code (1/2/3 = B/W/L).
int8_t dispWidth(uint8_t sd) {
    return sd == 1 ? -8 : sd == 2 ? -16 : -32;
}

// Field width in bits for an Sd/Si/Sa code (1/2/3 = B/W/L).
uint8_t fieldBits(uint8_t s) {
    return s == 1 ? 8 : s == 2 ? 16 : 32;
}

// Smallest signed field width (8/16/32 bits) that holds |value|, mirroring the
// assembler auto-size (asm_h16.cpp pickDispSize).
uint8_t naturalLen(int32_t value) {
    if (value >= -0x80 && value <= 0x7F) return 8;
    if (value >= -0x8000 && value <= 0x7FFF) return 16;
    return 32;
}

// Append the ":8"/":16"/":32" length suffix for an EA field of |bits| width.
void outLen(StrBuffer &out, uint8_t bits) {
    out.text_P(bits == 8 ? PSTR(":8") : bits == 16 ? PSTR(":16") : PSTR(":32"));
}

// Append a length suffix only when the encoded field is wider than the value's
// natural width, so a minimal disp/abs re-assembles to the same bytes.
void outAutoLen(StrBuffer &out, int32_t value, uint8_t bits) {
    if (naturalLen(value) != bits)
        outLen(out, bits);
}

// A double-indirect ds field is byte (Sd=1) or long (Sd=3) only; flag :32 only
// when a long field holds a byte-sized value.
void outDsLen(StrBuffer &out, int32_t value, uint8_t sd) {
    if (sd == 3 && naturalLen(value) == 8)
        outLen(out, 32);
}

// Flag ILLEGAL_OPERAND_MODE if any bit set in |zeroMask| is non-zero.
void requireZero(DisInsn &insn, uint8_t byte, uint8_t zeroMask) {
    if (byte & zeroMask)
        insn.setErrorIf(ILLEGAL_OPERAND_MODE);
}

}  // namespace

void DisH16::decodeEa(
        DisInsn &insn, StrBuffer &out, OprSize /*sz*/, RegName bank, EaKind kind) const {
    const auto ea = insn.readByte();

    // Bit 7 marks accumulator form; M_EADST checks the flag and substitutes R0.
    if (ea & 0x80)
        insn.accumulatorForm = true;
    const auto code = ea & 0x7F;
    const auto rn = code & 0x0F;
    const auto top3 = (code >> 4) & 0x07;

    switch (top3) {
    case 0x04:  // register direct (codes 0x40..0x4F)
        // Manual Available EA forbids Rn for a memory/peripheral operand.
        if (kind == EA_MEM || kind == EA_PERI)
            insn.setErrorIf(ILLEGAL_OPERAND_MODE);
        outRegName(out, decodeReg(rn, bank));
        return;
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03: {  // 0|Sd|Rn
        const auto sd = (code >> 4) & 0x03;
        if (sd == 0) {
            out.letter('@');
            outRegName(out, decodeReg(rn, bank));
        } else {
            const auto disp = readSignedDisp(insn, sd);
            out.letter('@').letter('(');
            outHex(out, disp, dispWidth(sd));
            outAutoLen(out, disp, fieldBits(sd));
            out.comma();
            outRegName(out, decodeReg(rn, bank));
            out.letter(')');
        }
        return;
    }
    case 0x05:  // @Rn+
    case 0x06:  // @-Rn
        // Manual forbids auto-inc/dec for peripheral and bit-field operands.
        if (kind == EA_PERI || kind == EA_BITF)
            insn.setErrorIf(ILLEGAL_OPERAND_MODE);
        if (top3 == 0x06)
            out.letter('@').letter('-');
        else
            out.letter('@');
        outRegName(out, decodeReg(rn, bank));
        if (top3 == 0x05)
            out.letter('+');
        return;
    case 0x07:
        switch (code) {
        case 0x70:    // <CRn> prefix
        case 0x74: {  // <PRn> prefix
            // Reject chained bank prefix: manual Note 4 says the last wins, but
            // the extra bytes can't round-trip from disassembled source.
            if (bank != REG_R0)
                insn.setErrorIf(ILLEGAL_OPERAND_MODE);
            decodeEa(insn, out, SZ_NONE, code == 0x70 ? REG_CR0 : REG_PR0, kind);
            return;
        }
        case 0x71:
        case 0x72:
        case 0x73:  // immediate (Si=01,10,11)
            if (kind != EA_ANY)  // illegal as a write dest (EA_DEST) or jump target (EA_MEM)
                insn.setErrorIf(ILLEGAL_OPERAND_MODE);
            [[fallthrough]];
        case 0x75:
        case 0x76:
        case 0x77: {  // absolute (Sa=01,10,11)
            // Bank prefix has no effect on imm/abs (no Rn); reject the
            // non-canonical combination so it round-trips.
            if (bank != REG_R0)
                insn.setErrorIf(ILLEGAL_OPERAND_MODE);
            const auto si = code & 0x03;
            const auto isAbs = (code & 0x04) != 0;
            const auto bits = fieldBits(si);
            out.letter(isAbs ? '@' : '#');
            int32_t value;
            switch (si) {
            case 1:  value = int8_t(insn.readByte());    break;
            case 2:  value = int16_t(insn.readUint16()); break;
            default: value = int32_t(insn.readUint32()); break;
            }
            // Imm/abs are sign-extended (manual).  A negative absolute is
            // rendered as its full 32-bit value: a leading "@-" would parse as
            // the @-Rn auto-decrement mode.  An immediate keeps :N always
            // because ASL auto-sizes a bare immediate to word, not byte.
            if (isAbs) {
                if (value < 0)
                    outHex(out, uint32_t(value), 32);
                else
                    outHex(out, uint32_t(value), int8_t(bits));
                outAutoLen(out, value, bits);
            } else {
                outHex(out, uint32_t(value), -int8_t(bits));
                outLen(out, bits);
            }
            return;
        }
        case 0x78:
        case 0x79:
        case 0x7A:
        case 0x7B: {  // @(disp,Rn*Sf)
            const auto sd = code & 0x03;
            const auto ext = insn.readByte();
            // Mode 7 has no index-size bit, so the two spare top bits must be 0.
            requireZero(insn, ext, 0xC0);
            const auto sf = (ext >> 4) & 0x03;
            const auto base = ext & 0x0F;
            const auto scale = static_cast<uint8_t>(1u << sf);
            const auto disp = sd != 0 ? readSignedDisp(insn, sd) : 0;
            out.letter('@').letter('(');
            if (sd != 0) {
                outHex(out, disp, dispWidth(sd));
                outAutoLen(out, disp, fieldBits(sd));
                out.comma();
            }
            outRegName(out, decodeReg(base, bank));
            out.letter('*');
            outHex(out, scale, 4);
            out.letter(')');
            return;
        }
        case 0x7C: {  // @([disp,] Xm[.Sz][*Sf], Rn)
            // Per manual, a <CRn>/<PRn> prefix banks the base Rn but Xm (index)
            // is always global.
            const auto ext1 = insn.readByte();
            const auto ext2 = insn.readByte();
            requireZero(insn, ext1, 0x80);  // ext1: [0|L|Sd|Rn]
            const auto sd = (ext1 >> 4) & 0x03;
            const auto base = ext1 & 0x0F;
            const auto longIdx = (ext1 & 0x40) != 0;
            const auto disp = sd != 0 ? readSignedDisp(insn, sd) : 0;
            out.letter('@').letter('(');
            if (sd != 0) {
                outHex(out, disp, dispWidth(sd));
                outAutoLen(out, disp, fieldBits(sd));
                out.comma();
            }
            outIndex(insn, out, ext2, longIdx);
            out.comma();
            outRegName(out, decodeReg(base, bank));
            out.letter(')');
            return;
        }
        case 0x7D: {  // PC-relative (with or without index)
            // Bank prefix has no effect here (PC-rel has no Rn).  Manual Note 3
            // to Table 4-7 allows it, but the extra bytes can't round-trip.
            if (bank != REG_R0)
                insn.setErrorIf(ILLEGAL_OPERAND_MODE);
            const auto ext1 = insn.readByte();
            const auto sd = (ext1 >> 4) & 0x03;
            const auto noIndex = (ext1 & 0x80) != 0;
            if (noIndex) {
                // ext1: [1|0|Sd|0000]
                requireZero(insn, ext1, 0x4F);
                const auto disp = sd != 0 ? readSignedDisp(insn, sd) : 0;
                const auto target = static_cast<uint32_t>(
                        insn.address() + insn.length() + disp);
                out.letter('@').letter('(');
                if (sd != 0) {
                    outHex(out, target, 32, false);
                    outAutoLen(out, disp, fieldBits(sd));
                    out.comma();
                }
                out.text_P(TEXT_REG_PC);
                out.letter(')');
            } else {
                // ext1: [0|L|Sd|0000]
                requireZero(insn, ext1, 0x0F);
                const auto ext2 = insn.readByte();
                const auto longIdx = (ext1 & 0x40) != 0;
                const auto disp = sd != 0 ? readSignedDisp(insn, sd) : 0;
                const auto target = static_cast<uint32_t>(
                        insn.address() + insn.length() + disp);
                out.letter('@').letter('(');
                if (sd != 0) {
                    outHex(out, target, 32, false);
                    outAutoLen(out, disp, fieldBits(sd));
                    out.comma();
                }
                outIndex(insn, out, ext2, longIdx);
                out.comma();
                out.text_P(TEXT_REG_PC);
                out.letter(')');
            }
            return;
        }
        case 0x7E: {  // @(ds2, @(ds1, Rn))
            const auto ext = insn.readByte();
            const auto s1 = (ext >> 4) & 0x03;
            const auto s2 = (ext >> 6) & 0x03;
            const auto base = ext & 0x0F;
            // Per Table 16-7 each Sd field is byte (01) or long (11); a 0 or
            // word (10) field is an illegal encoding.
            if ((s1 & 0x01) == 0 || (s2 & 0x01) == 0)
                insn.setErrorIf(ILLEGAL_OPERAND_MODE);
            const auto disp1 = readSignedDisp(insn, s1);
            const auto disp2 = readSignedDisp(insn, s2);
            out.letter('@').letter('(');
            outHex(out, disp2, s2 == 1 ? -8 : -32);
            outDsLen(out, disp2, s2);
            out.text_P(PSTR(",@("));
            outHex(out, disp1, s1 == 1 ? -8 : -32);
            outDsLen(out, disp1, s1);
            out.comma();
            outRegName(out, decodeReg(base, bank));
            out.text_P(PSTR("))"));
            return;
        }
        default:
            insn.setErrorIf(out, ILLEGAL_OPERAND);
            return;
        }
    }
}

void DisH16::outIndex(DisInsn &insn, StrBuffer &out, uint8_t ext, bool longIdx) const {
    requireZero(insn, ext, 0xC0);  // ext: [00|Sf|Xm]
    const auto sf = (ext >> 4) & 0x03;
    const auto xm = ext & 0x0F;
    outRegName(out, decodeReg(xm));  // Xm always global
    out.text_P(longIdx ? PSTR(".L") : PSTR(".W"));
    if (sf != 0) {
        out.letter('*');
        outHex(out, static_cast<uint8_t>(1u << sf), 4);
    }
}

void DisH16::decodeBranch(DisInsn &insn, StrBuffer &out, OprSize sz) const {
    int32_t delta = 0;
    int8_t width = 8;
    switch (sz) {
    case SZ_BYTE:
        delta = static_cast<int8_t>(insn.readByte());
        width = 8;
        break;
    case SZ_WORD:
        delta = static_cast<int16_t>(insn.readUint16());
        width = 16;
        break;
    case SZ_LONG:
        delta = static_cast<int32_t>(insn.readUint32());
        width = 32;
        break;
    default:
        break;
    }
    const auto base = insn.address() + insn.length();
    const auto target = static_cast<Config::uintptr_t>(base + delta);
    insn.setErrorIf(out, checkAddr(target));
    outRelAddr(out, target, insn.address(), width);
}

void DisH16::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    switch (mode) {
    case M_NONE:
        return;
    case M_EASRC:
        // CMP:G / CMPS carry the implicit-R0 second operand as M_EASRC (not
        // M_EADST), so honor accumulator form here too.
        if (insn.accumulatorForm) {
            insn.accConsumed = true;
            outRegName(out, REG_R0);
        } else {
            decodeEa(insn, out, insn.oprSize);
        }
        return;
    case M_EADST:
        if (insn.accumulatorForm) {
            insn.accConsumed = true;
            outRegName(out, REG_R0);
        } else {
            decodeEa(insn, out, insn.oprSize, REG_R0, EA_DEST);
        }
        return;
    case M_EAMEM:
        // JMP/JSR target: any EA but register-direct or immediate.
        decodeEa(insn, out, insn.oprSize, REG_R0, EA_MEM);
        return;
    case M_EAPERI:
        // MOVTP/MOVFP peripheral operand: a memory EA without auto-inc/dec.
        // Accumulator form would imply an R0 register destination, which a
        // peripheral EA forbids -- flag it.
        if (insn.accumulatorForm) {
            insn.accConsumed = true;
            insn.setErrorIf(ILLEGAL_OPERAND_MODE);
            outRegName(out, REG_R0);
        } else {
            decodeEa(insn, out, insn.oprSize, REG_R0, EA_PERI);
        }
        return;
    case M_IQ:
        out.letter('#');
        outHex(out, static_cast<int8_t>(insn.readByte()), -8);
        return;
    case M_RR: {
        const auto b = insn.readByte();
        outRegName(out, decodeReg(b >> 4));
        out.comma();
        outRegName(out, decodeReg(b & 0x0F));
        return;
    }
    case M_IRQ: {
        // ADD:RQ / SUB:RQ: zero-extended unsigned imm4 (0..15).
        const auto b = insn.readByte();
        out.letter('#');
        outHex(out, b & 0x0F, 4);
        out.comma();
        outRegName(out, decodeReg(b >> 4));
        return;
    }
    case M_SIRQ: {
        // CMP:RQ / MOV:RQ: sign-extended signed imm4 (-8..7).
        const auto b = insn.readByte();
        out.letter('#');
        int8_t v = b & 0x0F;
        if (v & 0x08)
            v -= 0x10;
        outHex(out, v, -4);
        out.comma();
        outRegName(out, decodeReg(b >> 4));
        return;
    }
    case M_RNNO: {
        // Rn in low 4 bits; upper 4 bits are zero-required.
        const auto b = insn.readByte();
        requireZero(insn, b, 0xF0);
        outRegName(out, decodeReg(b));
        return;
    }
    case M_RCC:
        // SCB: Rn was pre-read with the /cc byte; output it here.
        outRegName(out, insn.scbReg);
        return;
    case M_RLIST: {
        // STM/LDM/PGBN: 16-bit register-list mask.  An empty list (mask 0) is
        // illegal: the assembler requires at least one register, so it can't
        // round-trip.
        const auto mask = insn.readUint16();
        if (mask == 0)
            insn.setErrorIf(ILLEGAL_OPERAND_MODE);
        outRegList(out, mask);
        return;
    }
    case M_DISP:
        decodeBranch(insn, out, insn.oprSize);
        return;
    case M_CR:
        outCrName(out, insn.crReg);
        return;
    case M_TRAPV: {
        // TRAPA #imm4 in low 4 bits; upper 4 bits zero-required.
        const auto b = insn.readByte();
        requireZero(insn, b, 0xF0);
        out.letter('#');
        outHex(out, b & 0x0F, 4);
        return;
    }
    case M_FRMSZ: {
        // Sd matches insn.oprSize (same low-2-bit field of the first byte).
        const auto sd = uint8_t(insn.oprSize);
        int32_t v = 0;
        switch (sd) {
        case 0:
            v = static_cast<int8_t>(insn.readByte());
            break;
        case 1:
            v = static_cast<int16_t>(insn.readUint16());
            break;
        case 2:
            v = static_cast<int32_t>(insn.readUint32());
            break;
        }
        out.letter('#');
        outHex(out, v, sd == 0 ? -8 : (sd == 1 ? -16 : -32));
        return;
    }
    case M_SCNTR:
    case M_BNUMR:
        // Rn embedded in low 4 bits of the second-byte opcode.
        outRegName(out, decodeReg(insn.opCode() & 0x0F));
        return;
    case M_SCNTI:
    case M_BNUMI:
        // Imm5 in low 5 bits of the second-byte opcode.
        out.letter('#');
        outHex(out, insn.opCode() & 0x1F, 5);
        return;
    default:
        insn.setErrorIf(out, ILLEGAL_OPERAND);
        return;
    }
}

void DisH16::outSize(DisInsn &insn) const {
    const auto isz = insn.insnSize();
    if (isz == ISZ_DATA) {
        appendSizeSuffix(insn, insn.oprSize);
    } else if (isz == ISZ_OBIT0) {
        // SWAP: 1-bit byte/word size at opcode bit 0 (bit 1 belongs to the base
        // opcode, 0xEA/0xEB), so read bit 0 alone.
        appendSizeSuffix(insn, OprSize(insn.opCode() & 0x01));
    } else if (isz == ISZ_OWL) {
        // MOVTP/MOVFP: opcode bit 0 selects word(0) or long(1).
        appendSizeSuffix(insn, (insn.opCode() & 0x01) ? SZ_LONG : SZ_WORD);
    } else if (isz == ISZ_EXTU) {
        // EXTU/EXTS Sz mapping: 00=W, 01=L, 10=B
        OprSize sz = SZ_NONE;
        switch (insn.oprSize) {
        case SZ_BYTE:
            sz = SZ_WORD;
            break;
        case SZ_WORD:
            sz = SZ_LONG;
            break;
        case SZ_LONG:
            sz = SZ_BYTE;
            break;
        default:
            break;
        }
        appendSizeSuffix(insn, sz);
    } else if (isz == ISZ_PRSZ) {
        // Sz comes from prefix's low 2 bits (multi-byte families).
        appendSizeSuffix(insn, OprSize(insn.prefix() & 0x03));
    } else if (isz == ISZ_OBIT4) {
        // MUL/DIV size: bit 4 of opcode (0=B, 1=W).
        appendSizeSuffix(insn, (insn.opCode() & 0x10) ? SZ_WORD : SZ_BYTE);
    }
}

Error DisH16::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto first = insn.readByte();
    if (isPrefix(cpuType(), first)) {
        insn.setPrefix(first);
        insn.setOpCode(insn.readByte());
    } else {
        insn.setOpCode(first);
    }
    // Sz field is always in low 2 bits of the FIRST byte (prefix or opcode).
    insn.oprSize = OprSize(first & 0x03);

    // searchOpCode may set ILLEGAL_OPERAND_MODE yet still match; only bail out
    // when nothing matched (UNKNOWN_INSTRUCTION).
    if (searchOpCode(cpuType(), insn, out) == UNKNOWN_INSTRUCTION)
        return _insn.setError(insn);

    // String ops: opcode = [flag|TYP|Rnc]; then [Rns|Rnd]; 4-register forms
    // (flag bit 7) also have [Rnf|cc].  Direction = opcode bit 6 (0=F, 1=B).
    if (insn.mode1() == M_STROP) {
        const auto opc = insn.opCode();
        const auto rnc = decodeReg(opc & 0x0F);
        const auto b2 = insn.readByte();
        const auto rns = decodeReg(b2 >> 4);
        const auto rnd = decodeReg(b2 & 0x0F);
        auto &nb = insn.nameBuffer();
        const bool fourReg = (opc & 0x80) != 0;
        reg::RegName rnf = reg::REG_UNDEF;
        if (fourReg) {
            const auto b3 = insn.readByte();
            rnf = decodeReg(b3 >> 4);
            nb.letter('/');
            outCcName(nb, CcName(b3 & 0x0F));
        }
        nb.letter('/').letter((opc & 0x40) ? 'B' : 'F');
        outSize(insn);
        outRegName(out, rns);
        out.comma();
        outRegName(out, rnd);
        out.comma();
        outRegName(out, rnc);
        if (fourReg) {
            out.comma();
            outRegName(out, rnf);
        }
        return _insn.setError(insn);
    }

    // BFMOV: six registers [Rnx|Rnb][Rny|Rno][Rns|Rnd].
    if (insn.mode1() == M_BFMOV) {
        const auto b1 = insn.readByte();
        const auto b2 = insn.readByte();
        const auto b3 = insn.readByte();
        outRegName(out, decodeReg(b1 >> 4));
        out.comma();
        outRegName(out, decodeReg(b1 & 0x0F));
        out.comma();
        outRegName(out, decodeReg(b2 >> 4));
        out.comma();
        outRegName(out, decodeReg(b2 & 0x0F));
        out.comma();
        outRegName(out, decodeReg(b3 >> 4));
        out.comma();
        outRegName(out, decodeReg(b3 & 0x0F));
        return _insn.setError(insn);
    }
    // Bit-field ops BFEXT/BFINS/BFSCH: [Rnb|Rnf] then EAs, EAd.
    if (insn.mode1() == M_BITF) {
        const auto b1 = insn.readByte();
        outRegName(out, decodeReg(b1 >> 4));
        out.comma();
        outRegName(out, decodeReg(b1 & 0x0F));
        // Both bit-field operands forbid immediate and auto-inc/dec
        // (manual Available EA); register-direct and memory modes are fine.
        out.comma();
        decodeEa(insn, out, SZ_NONE, REG_R0, EA_BITF);  // EAs
        out.comma();
        decodeEa(insn, out, SZ_NONE, REG_R0, EA_BITF);  // EAd
        // A bit-field EA never uses accumulator form, so bit 7 is reserved-zero;
        // this branch returns before the shared check below, so verify it here.
        if (insn.accumulatorForm)
            insn.setErrorIf(ILLEGAL_OPERAND_MODE);
        return _insn.setError(insn);
    }

    // ANDC/ORC/XORC/LDC/STC: order is [opcode][CR-code][EA]; pre-read the CR
    // byte so M_CR can output it regardless of operand position.
    if (insn.mode1() == M_CR || insn.mode2() == M_CR) {
        insn.crReg = crFromCode(insn.readByte());
        if (insn.crReg == CR_UNDEF)
            insn.setErrorIf(ILLEGAL_OPERAND);
    }

    // /cc condition byte follows the opcode: TRAP/SET (IC_CC) use [*|cc], SCB
    // packs [Rn|cc].  Pre-read it so "/cc" precedes the size suffix.
    if (insn.insnClass() == IC_CC) {
        const auto b = insn.readByte();
        requireZero(insn, b, 0xF0);  // [*|cc]: the high nibble must be 0
        insn.ccReg = CcName(b & 0x0F);
    } else if (insn.mode1() == M_RCC) {
        const auto b = insn.readByte();
        insn.ccReg = CcName(b & 0x0F);
        insn.scbReg = decodeReg(b >> 4);
    }

    appendClassSuffix(insn, insn.insnClass());
    appendCcSuffix(insn);
    outSize(insn);

    const auto src = insn.mode1();
    const auto dst = insn.mode2();
    if (src == M_RLIST && dst == M_EADST) {
        // STM: bytes are EAd then reglist, but mnemonic order is reglist, EAd.
        // The EAd is a memory location (no register-direct / immediate).
        char tmp[40];
        StrBuffer ea{tmp, sizeof(tmp)};
        decodeEa(insn, ea, insn.oprSize, REG_R0, EA_MEM);  // EAd, then reglist
        decodeOperand(insn, out, src);
        out.comma().text(tmp);
    } else if (src == M_EASRC && dst == M_RLIST) {
        // LDM: EAs (a memory location) then reglist.
        decodeEa(insn, out, insn.oprSize, REG_R0, EA_MEM);
        out.comma();
        decodeOperand(insn, out, dst);
    } else if (src != M_NONE) {
        decodeOperand(insn, out, src);
        if (dst != M_NONE) {
            out.comma();
            decodeOperand(insn, out, dst);
        }
    } else if (dst != M_NONE) {
        decodeOperand(insn, out, dst);
    }

    // Bit 7 is valid only on the source EA of a two-EA instruction (consumed by
    // the destination); set-but-unconsumed elsewhere is an illegal encoding.
    if (insn.accumulatorForm && !insn.accConsumed)
        insn.setErrorIf(ILLEGAL_OPERAND_MODE);

    return _insn.setError(insn);
}

}  // namespace h16
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
