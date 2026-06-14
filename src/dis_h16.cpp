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

// Decode a 4-bit register field with the given bank context: REG_R0..R15 by
// default; REG_CR0..CR15 if the EA was preceded by a <CRn> prefix byte
// (0x70); REG_PR0..PR15 if preceded by <PRn> (0x74).
RegName decodeReg(uint8_t r, RegName bank = REG_R0) {
    return RegName(uint8_t(bank) | (r & 0x0F));
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

// Verify that bits set in |zeroMask| of |byte| are all zero.  Reports
// ILLEGAL_OPERAND_MODE if any required-zero bit is non-zero.
void requireZero(DisInsn &insn, uint8_t byte, uint8_t zeroMask) {
    if (byte & zeroMask)
        insn.setErrorIf(ILLEGAL_OPERAND_MODE);
}

}  // namespace

void DisH16::decodeEa(
        DisInsn &insn, StrBuffer &out, OprSize /*sz*/, RegName bank, bool destOnly) const {
    const auto ea = insn.readByte();

    // Bit 7 of the source EA byte is the accumulator-form marker.  Set the
    // flag; M_EAD will check it and substitute R0 instead of reading EAd.
    if (ea & 0x80)
        insn.accumulatorForm = true;
    const auto code = ea & 0x7F;
    const auto rn = code & 0x0F;
    const auto top3 = (code >> 4) & 0x07;

    switch (top3) {
    case 0x04:  // 1 0 0 r r r r : register direct (codes 0x40..0x4F)
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
            outHex(out, disp, sd == 1 ? -8 : (sd == 2 ? -16 : -32));
            out.comma();
            outRegName(out, decodeReg(rn, bank));
            out.letter(')');
        }
        return;
    }
    case 0x05:  // 1 0 1 r r r r : @Rn+
        out.letter('@');
        outRegName(out, decodeReg(rn, bank));
        out.letter('+');
        return;
    case 0x06:  // 1 1 0 r r r r : @-Rn
        out.text_P(PSTR("@-"));
        outRegName(out, decodeReg(rn, bank));
        return;
    case 0x07:
        switch (code) {
        case 0x70:    // <CRn> prefix
        case 0x74: {  // <PRn> prefix
            // Reject back-to-back bank prefix.  Manual Note 4 says the last
            // prefix wins, but the chain adds bytes the disassembled source
            // can't re-introduce, so we flag chained prefix ILLEGAL.
            if (bank != REG_R0)
                insn.setErrorIf(ILLEGAL_OPERAND_MODE);
            decodeEa(insn, out, SZ_NONE, code == 0x70 ? REG_CR0 : REG_PR0, destOnly);
            return;
        }
        case 0x71:
        case 0x72:
        case 0x73:  // immediate (Si=01,10,11)
            if (destOnly)
                insn.setErrorIf(ILLEGAL_OPERAND_MODE);
            [[fallthrough]];
        case 0x75:
        case 0x76:
        case 0x77: {  // absolute (Sa=01,10,11)
            // Bank prefix has no effect on imm/abs (no Rn).  Reject the
            // non-canonical prefix-imm and prefix-abs combinations so they
            // don't appear in autogen and break round-trip.
            if (bank != REG_R0)
                insn.setErrorIf(ILLEGAL_OPERAND_MODE);
            const auto si = code & 0x03;
            const auto isAbs = (code & 0x04) != 0;
            out.letter(isAbs ? '@' : '#');
            // Absolute is sign-extended to 32 bits; emit the natural width
            // for positive values and the full 32-bit form for negatives so
            // ASL accepts both without "range overflow".
            switch (si) {
            case 1: {
                const auto b = insn.readByte();
                if (isAbs && (b & 0x80))
                    outHex(out, uint32_t(int32_t(int8_t(b))), 32);
                else
                    outHex(out, b, 8);
                out.text_P(PSTR(":8"));
                break;
            }
            case 2: {
                const auto w = insn.readUint16();
                if (isAbs && (w & 0x8000))
                    outHex(out, uint32_t(int32_t(int16_t(w))), 32);
                else
                    outHex(out, w, 16);
                out.text_P(PSTR(":16"));
                break;
            }
            case 3:
                outHex(out, insn.readUint32(), 32);
                out.text_P(PSTR(":32"));
                break;
            }
            return;
        }
        case 0x78:
        case 0x79:
        case 0x7A:
        case 0x7B: {  // @(disp,Rn*Sf)
            const auto sd = code & 0x03;
            const auto ext = insn.readByte();
            requireZero(insn, ext, 0x80);  // ext byte: [*|Sf|Rn] - bit 7 = 0
            const auto sf = (ext >> 4) & 0x03;
            const auto base = ext & 0x0F;
            const auto scale = static_cast<uint8_t>(1u << sf);
            const auto disp = sd != 0 ? readSignedDisp(insn, sd) : 0;
            out.letter('@').letter('(');
            if (sd != 0) {
                outHex(out, disp, sd == 1 ? -8 : (sd == 2 ? -16 : -32));
                out.comma();
            }
            outRegName(out, decodeReg(base, bank));
            out.letter('*');
            outHex(out, scale, 4);
            out.letter(')');
            return;
        }
        case 0x7C: {  // @([disp,] Xm[.Sz][*Sf], Rn)
            // Per manual: with <CRn>/<PRn> prefix the base Rn is bank-relative
            // but Xm (index) is always global.
            const auto ext1 = insn.readByte();
            const auto ext2 = insn.readByte();
            requireZero(insn, ext1, 0x80);  // ext1: [0|L|Sd|Rn] - bit 7 = 0
            requireZero(insn, ext2, 0x80);  // ext2: [*|Sf|Xm] - bit 7 = 0
            const auto sd = (ext1 >> 4) & 0x03;
            const auto base = ext1 & 0x0F;
            const auto sf = (ext2 >> 4) & 0x03;
            const auto xm = ext2 & 0x0F;
            const auto scale = static_cast<uint8_t>(1u << sf);
            const auto longIdx = (ext1 & 0x40) != 0;
            const auto disp = sd != 0 ? readSignedDisp(insn, sd) : 0;
            out.letter('@').letter('(');
            if (sd != 0) {
                outHex(out, disp, sd == 1 ? -8 : (sd == 2 ? -16 : -32));
                out.comma();
            }
            outRegName(out, decodeReg(xm));  // Xm always global
            out.text_P(longIdx ? PSTR(".L") : PSTR(".W"));
            if (sf != 0) {
                out.letter('*');
                outHex(out, scale, 4);
            }
            out.comma();
            outRegName(out, decodeReg(base, bank));
            out.letter(')');
            return;
        }
        case 0x7D: {  // PC-relative (with or without index)
            // Bank prefix has no effect here (PC-rel has no Rn).  The
            // manual (Note 3 to Table 4-7) allows the prefix, but it adds
            // bytes that the disassembled source can't re-introduce on
            // round-trip, so we flag it ILLEGAL_OPERAND_MODE.
            if (bank != REG_R0)
                insn.setErrorIf(ILLEGAL_OPERAND_MODE);
            const auto ext1 = insn.readByte();
            const auto sd = (ext1 >> 4) & 0x03;
            const auto noIndex = (ext1 & 0x80) != 0;
            if (noIndex) {
                // ext1: [1|0|Sd|****] - bit 6 must be 0; low 4 bits must be 0
                requireZero(insn, ext1, 0x4F);
                const auto disp = sd != 0 ? readSignedDisp(insn, sd) : 0;
                const auto target = static_cast<uint32_t>(
                        insn.address() + insn.length() + disp);
                out.letter('@').letter('(');
                if (sd != 0) {
                    outHex(out, target, 32, false);
                    out.comma();
                }
                out.text_P(TEXT_REG_PC);
                out.letter(')');
            } else {
                // ext1: [0|L|Sd|****] - low 4 bits must be 0
                requireZero(insn, ext1, 0x0F);
                const auto ext2 = insn.readByte();
                requireZero(insn, ext2, 0x80);  // ext2: [*|Sf|Xm]
                const auto sf = (ext2 >> 4) & 0x03;
                const auto xm = ext2 & 0x0F;
                const auto scale = static_cast<uint8_t>(1u << sf);
                const auto longIdx = (ext1 & 0x40) != 0;
                const auto disp = sd != 0 ? readSignedDisp(insn, sd) : 0;
                const auto target = static_cast<uint32_t>(
                        insn.address() + insn.length() + disp);
                out.letter('@').letter('(');
                if (sd != 0) {
                    outHex(out, target, 32, false);
                    out.comma();
                }
                outRegName(out, decodeReg(xm));  // Xm always global
                out.text_P(longIdx ? PSTR(".L") : PSTR(".W"));
                if (sf != 0) {
                    out.letter('*');
                    outHex(out, scale, 4);
                }
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
            const auto disp1 = readSignedDisp(insn, s1);
            const auto disp2 = readSignedDisp(insn, s2);
            out.letter('@').letter('(');
            outHex(out, disp2, s2 == 1 ? -8 : -32);
            out.text_P(PSTR(",@("));
            outHex(out, disp1, s1 == 1 ? -8 : -32);
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
    case M_EA:
        decodeEa(insn, out, insn.oprSize);
        return;
    case M_EAD:
        // Accumulator-form check is owned by the destination operand decode
        // path so callers don't need a two-EA special case.
        if (insn.accumulatorForm) {
            outRegName(out, REG_R0);
        } else {
            decodeEa(insn, out, insn.oprSize, REG_R0, /*destOnly=*/true);
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
        const auto b = insn.readByte();
        out.letter('#');
        outHex(out, b & 0x0F, 4);
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
    case M_DISP:
        decodeBranch(insn, out, insn.oprSize);
        return;
    case M_CR:
        outCrName(out, insn.crReg);
        return;
    case M_ITRPN: {
        // TRAPA #imm4 in low 4 bits; upper 4 bits zero-required.
        const auto b = insn.readByte();
        requireZero(insn, b, 0xF0);
        out.letter('#');
        outHex(out, b & 0x0F, 4);
        return;
    }
    case M_FRMSZ: {
        const auto sd = insn.opCode() & 0x03;
        int32_t v = 0;
        switch (sd) {
        case 0: v = static_cast<int8_t>(insn.readByte()); break;
        case 1: v = static_cast<int16_t>(insn.readUint16()); break;
        case 2: v = static_cast<int32_t>(insn.readUint32()); break;
        }
        out.letter('#');
        outHex(out, v, sd == 0 ? -8 : (sd == 1 ? -16 : -32));
        return;
    }
    case M_RFRM: {
        const auto rn = insn.readByte();
        requireZero(insn, rn, 0xF0);  // Rn byte upper nibble zero-required
        outRegName(out, decodeReg(rn));
        out.comma();
        const auto sd = insn.opCode() & 0x03;
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
    } else if (isz == ISZ_WORD) {
        appendSizeSuffix(insn, SZ_WORD);
    } else if (isz == ISZ_FIXB) {
        appendSizeSuffix(insn, SZ_BYTE);
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

namespace {

// Returns true if |b| is the first byte of a multi-byte opcode family that
// needs to be captured as a prefix.  When true, the caller should set the
// prefix to |b| and read the next byte as the opcode.
bool isPrefixByte(uint8_t b) {
    // SFT dynamic 0x60-0x62, SFT static 0x64-0x66
    // BIT dynamic 0x68-0x6A, BIT static 0x6C-0x6E
    // MUL 0xEE, DIV 0xEF
    // Bcc:G 0xA4-0xA6
    if ((b & 0xFC) == 0x60 || (b & 0xFC) == 0x64)
        return true;
    if ((b & 0xFC) == 0x68 || (b & 0xFC) == 0x6C)
        return true;
    if ((b & 0xFC) == 0xA4)
        return true;
    if (b == 0xEE || b == 0xEF)
        return true;
    return false;
}

}  // namespace

Error DisH16::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto first = insn.readByte();
    if (isPrefixByte(first)) {
        insn.setPrefix(first);
        insn.setOpCode(insn.readByte());
    } else {
        insn.setOpCode(first);
    }
    // Sz field is always in low 2 bits of the FIRST byte (prefix or opcode).
    insn.oprSize = OprSize(first & 0x03);

    // searchOpCode may set ILLEGAL_OPERAND_MODE via the zero-required check
    // even when an entry is matched.  Only bail out if no entry matched (the
    // table reports UNKNOWN_INSTRUCTION in that case).
    if (searchOpCode(cpuType(), insn, out) == UNKNOWN_INSTRUCTION)
        return _insn.setError(insn);

    // ANDC/ORC/XORC/LDC/STC byte order is [opcode][CR-code][EA]; pre-read the
    // CR byte so M_CR can output it regardless of operand position.
    if (insn.src() == M_CR || insn.dst() == M_CR) {
        insn.crReg = crFromCode(insn.readByte());
        if (insn.crReg == CR_UNDEF)
            insn.setErrorIf(ILLEGAL_OPERAND);
    }

    outSize(insn);

    const auto src = insn.src();
    const auto dst = insn.dst();
    if (src != M_NONE) {
        decodeOperand(insn, out, src);
        if (dst != M_NONE) {
            out.comma();
            decodeOperand(insn, out, dst);
        }
    } else if (dst != M_NONE) {
        decodeOperand(insn, out, dst);
    }

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
