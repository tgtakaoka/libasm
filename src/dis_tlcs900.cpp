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

#include "dis_tlcs900.h"
#include "reg_tlcs900.h"
#include "table_tlcs900.h"
#include "text_tlcs900.h"

namespace libasm {
namespace tlcs900 {

using namespace reg;
using namespace text::tlcs900;

const ValueFormatter::Plugins &DisTlcs900::defaultPlugins() {
    return ValueFormatter::Plugins::intel();
}

DisTlcs900::DisTlcs900(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_maxMode),
      Config(TABLE),
      _opt_maxMode(this, &Config::setMaxMode, OPT_BOOL_MAXIMUM_MODE, OPT_DESC_MAXIMUM_MODE) {
    reset();
}

// Decode the sub-byte for complex/pre-dec/post-inc addressing (0xF0-0xFF range).
// Returns the index register and the operand size (0=byte, 1=word, 2=lword).
// Split a complex sub-byte "r32' zz" into the base register code (low 2 bits
// cleared) and the increment-size (zz) field.
static uint8_t decodeSubReg(uint8_t sub, uint8_t &size) {
    size = sub & 3;
    return sub & 0xFC;
}

// Decode a complex (C3/D3/E3/F3) sub-addressing. The sub-byte low 2 bits
// select the form:
//   00 -> (r32) register-indirect              sub = r32' code
//   01 -> (r32+d16)                            sub = r32' code, then 16-bit disp
//   11 -> sub==0x03: (r32+r8) register-indexed (reads base code + index byte)
static Error decodeComplexAddr(DisInsn &insn, Operand &op) {
    const uint8_t sub = insn.sub;
    const uint8_t low2 = sub & 3;
    if (sub == 0x03) {
        // Register-indexed: (r32 + r8). Next byte is the base register code,
        // then an 8-bit index register selected from the current-bank SFR map.
        op.base = insn.readByte();
        const uint8_t idx_byte = insn.readByte();
        // E0=A, E1=W, E4=C, E5=B, E8=E, E9=D, EC=L, ED=H
        static const RegName IDX_MAP[] PROGMEM = {
                REG_A,  // E0
                REG_W,  // E1
                REG_UNDEF,
                REG_UNDEF,
                REG_C,  // E4
                REG_B,  // E5
                REG_UNDEF,
                REG_UNDEF,
                REG_E,  // E8
                REG_D,  // E9
                REG_UNDEF,
                REG_UNDEF,
                REG_L,  // EC
                REG_H,  // ED
        };
        const uint8_t offset = idx_byte - 0xE0;
        if (offset < sizeof(IDX_MAP) / sizeof(IDX_MAP[0])) {
            op.idx = RegName(pgm_read_byte(&IDX_MAP[offset]));
        }
        if (op.idx == REG_UNDEF)
            op.setError(UNKNOWN_INSTRUCTION);
        op.mode = M_IDXR;
    } else if (low2 == 0) {
        // (r32) register-indirect
        op.base = sub;
        op.mode = M_IND;
    } else if (low2 == 1) {
        // (r32 + d16)
        op.base = sub & 0xFC;
        op.val.setUnsigned(insn.readUint16());
        op.mode = M_IDX16;
    } else {
        op.setError(UNKNOWN_INSTRUCTION);
    }
    if (!op.hasError() && !reg::isMemBaseCode(op.base))
        op.setError(UNKNOWN_INSTRUCTION);
    return op.getError();
}

// Decode the prefix byte into an addressing Operand for register contexts.
static void decodeRegPrefix(Config::opcode_t prefix, PrefixMode pm, Operand &op) {
    const uint8_t reg_idx = prefix & 7;
    switch (pm) {
    case PM_REG8:
        op.mode = M_REG8;
        op.reg = decodeReg8(reg_idx);
        break;
    case PM_REG16:
        op.mode = M_REG16;
        op.reg = decodeReg16(reg_idx);
        break;
    case PM_REG32:
        op.mode = M_REG32;
        op.reg = decodeReg32(reg_idx);
        break;
    default:
        break;
    }
}

// Decode the prefix byte (and sub-bytes) for memory contexts.
Error DisTlcs900::decodePrefixAddr(
        DisInsn &insn, PrefixMode pm, Operand &op, Config::opcode_t prefix) const {
    if (pm == PM_REG8 || pm == PM_REG16 || pm == PM_REG32) {
        decodeRegPrefix(prefix, pm, op);
        return OK;
    }
    if (pm == PM_ABREG8 || pm == PM_ABREG16 || pm == PM_ABREG32) {
        op.val.setUnsigned(insn.readByte());
        return OK;
    }
    if (pm == PM_BLOCK || pm == PM_RETCC)
        return OK;

    // Memory context: decode addressing mode from prefix byte.
    // 0x80-0xBF single-byte forms address the 8 current-bank registers
    // (XWA..XSP) by their 3-bit index in the low nibble:
    //   nibble 0-7: register-indirect (XWA)..(XSP)
    //   nibble 8-F: 8-bit displacement indexed (Xrr+d8)

    const uint8_t nibble = prefix & 0x0F;

    if (prefix >= 0x80 && prefix <= 0xBF) {
        op.base = reg::currentBankBaseCode(nibble & 7);
        if (nibble <= 7) {
            op.mode = M_IND;
        } else {
            op.val.setSigned(static_cast<int8_t>(insn.readByte()));
            op.mode = M_IDX8;
        }
    } else {
        // 0xC0-0xC5, 0xD0-0xD5, 0xE0-0xE5, 0xF0-0xF5
        switch (nibble) {
        case 0:  // abs8
            op.val.setUnsigned(insn.readByte());
            op.mode = M_ABS8;
            break;
        case 1:  // abs16
            op.val.setUnsigned(insn.readUint16());
            op.mode = M_ABS16;
            break;
        case 2:  // abs24
            op.val.setUnsigned(insn.readUint24());
            op.mode = M_ABS24;
            break;
        case 3:  // complex sub-byte
            insn.sub = insn.readByte();
            decodeComplexAddr(insn, op);
            break;
        case 4: {  // complex pre-decrement (-r32)
            const uint8_t sub = insn.readByte();
            uint8_t size;
            op.base = decodeSubReg(sub, size);
            op.size = size;
            op.mode = M_PRDC;
            if (!reg::isMemBaseCode(op.base))
                op.setError(UNKNOWN_INSTRUCTION);
            break;
        }
        case 5: {  // complex post-increment (r32+)
            const uint8_t sub = insn.readByte();
            uint8_t size;
            op.base = decodeSubReg(sub, size);
            op.size = size;
            op.mode = M_PINC;
            if (!reg::isMemBaseCode(op.base))
                op.setError(UNKNOWN_INSTRUCTION);
            break;
        }
        default:
            op.setError(UNKNOWN_INSTRUCTION);
            break;
        }
    }
    return op.getError();
}

// Output memory address operand (parenthesised).
void DisTlcs900::outMemAddr(StrBuffer &out, const Operand &op) const {
    switch (op.mode) {
    case M_IND:
        out.letter('(');
        outAbsReg(out, op.base, 2);
        out.letter(')');
        break;
    case M_PRDC:
        out.letter('(').letter('-');
        outAbsReg(out, op.base, 2);
        out.letter(')');
        break;
    case M_PINC:
        out.letter('(');
        outAbsReg(out, op.base, 2);
        out.letter('+').letter(')');
        break;
    case M_IDX8: {
        const auto disp = static_cast<int16_t>(op.val.getSigned());
        out.letter('(');
        outAbsReg(out, op.base, 2);
        if (disp >= 0)
            out.letter('+');
        outHex(out, disp, -8);
        out.letter(')');
        break;
    }
    case M_IDX16: {
        const auto disp = static_cast<int16_t>(op.val.getUnsigned());
        out.letter('(');
        outAbsReg(out, op.base, 2);
        if (disp >= 0)
            out.letter('+');
        outHex(out, disp, -16);
        out.letter(')');
        break;
    }
    case M_IDXR:
        out.letter('(');
        outAbsReg(out, op.base, 2);
        out.letter('+');
        outRegName(out, op.idx).letter(')');
        break;
    case M_ABS8:
        outHex(out.letter('('), op.val.getUnsigned(), 8).letter(')');
        break;
    case M_ABS16:
        outHex(out.letter('('), op.val.getUnsigned(), 16).letter(')');
        break;
    case M_ABS24:
        outHex(out.letter('('), op.val.getUnsigned(), 24).letter(')');
        break;
    default:
        break;
    }
}

void DisTlcs900::outReg32(DisInsn &insn, StrBuffer &out, RegName reg) const {
    if (!maxMode() && reg::isReg32General(reg))
        insn.setError(UNKNOWN_INSTRUCTION);
    outRegName(out, reg);
}

// Addressing modes that carry a memory base register in Operand::base.
static bool isMemBaseMode(AddrMode mode) {
    return mode == M_IND || mode == M_IDX8 || mode == M_IDX16 || mode == M_IDXR ||
           mode == M_PRDC || mode == M_PINC;
}

// In minimum mode the general 32-bit registers XWA-XHL and any non-index base
// register don't exist. Detect a prefix operand carrying one so the caller can
// reject the instruction before reading its opcode.
static bool minModeIllegalPrefix(const Operand &op) {
    if (op.mode == M_REG32 && reg::isReg32General(op.reg))
        return true;
    return isMemBaseMode(op.mode) && !reg::isIndexBaseCode(op.base);
}

// Whether the prefix-encoded register size (from |pm|) is valid for a
// size-restricted source mode (M_SRCB/M_SRCW/M_SRCWL).
static bool srcSizeAllowed(AddrMode mode, PrefixMode pm) {
    uint8_t sz;
    if (pm == PM_REG8 || pm == PM_ABREG8)
        sz = 8;
    else if (pm == PM_REG16 || pm == PM_ABREG16)
        sz = 16;
    else if (pm == PM_REG32 || pm == PM_ABREG32)
        sz = 32;
    else
        return false;
    switch (mode) {
    case M_SRCB:
        return sz == 8;
    case M_SRCW:
        return sz == 16;
    case M_SRCWL:
        return sz == 16 || sz == 32;
    case M_SRCBW:
        return sz == 8 || sz == 16;
    default:
        return true;
    }
}

void DisTlcs900::decodeOperand(
        DisInsn &insn, StrBuffer &out, AddrMode mode, const Operand &prefixOp) const {
    const auto opc = insn.opCode();
    const auto pm = insn.pre();
    switch (mode) {
    case M_NONE:
        break;
    case M_SRCB:   // byte-only source register (DAA)
    case M_SRCW:   // word-only source register (MIRR, MINC/MDEC)
    case M_SRCWL:  // word/long source register (EXTZ/EXTS/PAA)
    case M_SRCBW:  // byte/word source register (NEG, CPL)
        if (!srcSizeAllowed(mode, pm)) {
            insn.setError(UNKNOWN_INSTRUCTION);
            break;
        }
        // fall through: render like M_SRC once the size is validated.
        [[fallthrough]];
    case M_SRC:
        // M_SRC: output the prefix-encoded source operand
        if (pm == PM_REG8 || pm == PM_REG16 || pm == PM_REG32) {
            outRegName(out, prefixOp.reg);
        } else if (pm == PM_ABREG8 || pm == PM_ABREG16 || pm == PM_ABREG32) {
            const uint8_t size = uint8_t(pm) - uint8_t(PM_ABREG8);
            if (!outAbsReg(out, static_cast<uint8_t>(prefixOp.val.getUnsigned()), size))
                insn.setError(UNKNOWN_INSTRUCTION);
        } else {
            outMemAddr(out, prefixOp);
        }
        break;
    case M_ABSDST:
        // M_ABSDST: abs-bank register in dst (prefix-encoded). PM is always PM_ABREG{8,16,32}.
        {
            const uint8_t size = uint8_t(pm) - uint8_t(PM_ABREG8);
            if (!outAbsReg(out, static_cast<uint8_t>(prefixOp.val.getUnsigned()), size))
                insn.setError(UNKNOWN_INSTRUCTION);
        }
        break;
    case M_DST:
        if (pm == PM_MEMD) {
            outMemAddr(out, prefixOp);
        } else {
            switch (pm) {
            case PM_REG8:
            case PM_MEM8:
            case PM_ABREG8:
                outRegName(out, decodeReg8(opc & 7));
                break;
            case PM_REG16:
            case PM_MEM16:
            case PM_ABREG16:
                outRegName(out, decodeReg16(opc & 7));
                break;
            case PM_REG32:
            case PM_MEM32:
            case PM_ABREG32:
                outReg32(insn, out, decodeReg32(opc & 7));
                break;
            default:
                break;
            }
        }
        break;
    case M_MULDST: {
        // MUL/DIV dest in opcode low bits, double the source size: a 16-bit
        // pair for a byte-op (R=2*idx+1, R must be odd) or a 32-bit register
        // for a word-op (R=idx). Long-op (PM_REG32 etc.) is not a valid form.
        const uint8_t r = opc & 7;
        if (pm == PM_REG8 || pm == PM_ABREG8 || pm == PM_MEM8) {
            if ((r & 1) == 0) {
                insn.setError(UNKNOWN_INSTRUCTION);
                break;
            }
            outRegName(out, decodeReg16(r >> 1));
        } else if (pm == PM_REG16 || pm == PM_ABREG16 || pm == PM_MEM16) {
            outReg32(insn, out, decodeReg32(r));
        } else {
            insn.setError(UNKNOWN_INSTRUCTION);
        }
        break;
    }
    case M_MULDSTP: {
        // MUL/DIV rr,#: dest carried in the prefix low bits (same code scheme
        // as M_MULDST).
        const uint8_t r = insn.prefix() & 7;
        if (pm == PM_REG8) {
            if ((r & 1) == 0) {
                insn.setError(UNKNOWN_INSTRUCTION);
                break;
            }
            outRegName(out, decodeReg16(r >> 1));
        } else if (pm == PM_REG16) {
            outReg32(insn, out, decodeReg32(r));
        } else {
            insn.setError(UNKNOWN_INSTRUCTION);
        }
        break;
    }
    case M_REG8:
        outRegName(out, decodeReg8(opc & 7));
        break;
    case M_REG16:
        outRegName(out, decodeReg16(opc & 7));
        break;
    case M_REG32:
        outReg32(insn, out, decodeReg32(opc & 7));
        break;
    case M_IMM8:
        outHex(out, insn.readByte(), 8);
        break;
    case M_IMM16:
        outHex(out, insn.readUint16(), 16);
        break;
    case M_IMM32:
        outHex(out, insn.readUint32(), 32);
        break;
    case M_IMMX:
        // Context-size immediate
        switch (pm) {
        case PM_REG8:
        case PM_MEM8:
        case PM_ABREG8:
            outHex(out, insn.readByte(), 8);
            break;
        case PM_REG16:
        case PM_MEM16:
        case PM_ABREG16:
            outHex(out, insn.readUint16(), 16);
            break;
        case PM_REG32:
        case PM_MEM32:
        case PM_ABREG32:
            outHex(out, insn.readUint32(), 32);
            break;
        default:
            outHex(out, insn.readByte(), 8);
            break;
        }
        break;
    case M_BIT:
        if (pm == PM_MEMD)
            outHex(out, opc & 7, 4);  // bit is in opcode low bits
        else
            outHex(out, insn.readByte() & 7, 4);
        break;
    case M_CC:
        outCcName(out, decodeCcName(opc & 0xF));
        break;
    case M_REL8: {
        const auto delta = static_cast<int8_t>(insn.readByte());
        const auto target = static_cast<uint32_t>(insn.address() + insn.length() + delta);
        insn.setErrorIf(out, checkAddr(target));
        outRelAddr(out, target, insn.address(), 8);
        break;
    }
    case M_REL16: {
        const auto delta = static_cast<int16_t>(insn.readUint16());
        const auto target = static_cast<uint32_t>(insn.address() + insn.length() + delta);
        insn.setErrorIf(out, checkAddr(target));
        outRelAddr(out, target, insn.address(), 16);
        break;
    }
    case M_LDARREL: {
        // rel16 was read into prefixOp at decodeImpl time; base = address + 4.
        const auto delta = static_cast<int16_t>(prefixOp.val.getSigned());
        const auto target = static_cast<uint32_t>(insn.address() + 4 + delta);
        insn.setErrorIf(out, checkAddr(target));
        outRelAddr(out, target, insn.address(), 16);
        break;
    }
    case M_ABS8:
        outHex(out.letter('('), insn.readByte(), 8).letter(')');
        break;
    case M_ABS16:
        outHex(out.letter('('), insn.readUint16(), 16).letter(')');
        break;
    case M_ABS24:
        outHex(out.letter('('), insn.readUint24(), 24).letter(')');
        break;
    case M_LDF: {
        // RFP width: 3-bit (banks 0-7) in MIN mode, 2-bit (0-3) in MAX mode.
        const uint8_t mask = maxMode() ? 3 : 7;
        outHex(out, insn.readByte() & mask, 4);
        break;
    }
    case M_INTLVL:
        outHex(out, insn.readByte() & 7, 4);
        break;
    case M_SWI:
        outHex(out, opc & 7, 4);
        break;
    case M_RCOUNT:
        outHex(out, insn.readByte(), 8);
        break;
    case M_STEP1:
        out.letter('1');
        break;
    case M_STEP2:
        out.letter('2');
        break;
    case M_STEP4:
        out.letter('4');
        break;
    case M_BUF: {
        const auto step = 1u << (opc & 3);
        const auto word = insn.readUint16();
        const auto bufSize = word + step;
        // Buffer size must be 2^n (manual: n = (opc&3)+1..15), i.e. a power
        // of two with 2*step <= bufSize <= 0x8000.  Other encodings do not
        // round-trip through the assembler.
        if ((bufSize & (bufSize - 1)) || bufSize < step * 2 || bufSize > 0x8000)
            insn.setErrorIf(out, ILLEGAL_OPERAND);
        outHex(out, static_cast<uint16_t>(bufSize), 16);
        break;
    }
    case M_LDXDST: {
        const uint8_t sub = insn.readByte();
        if (sub != 0x00) {
            insn.setError(UNKNOWN_INSTRUCTION);
            break;
        }
        outHex(out.letter('('), insn.readUint16(), 16).letter(')');
        break;
    }
    case R_A:
        outRegName(out, REG_A);
        break;
    case R_WA:
        outRegName(out, REG_WA);
        break;
    case R_SR:
        outRegName(out, REG_SR);
        break;
    case R_F:
        out.text_P(TEXT_REG_F);
        break;
    case R_FP:
        out.text_P(TEXT_REG_F).letter('\'');
        break;
    case M_CREG: {
        const uint8_t sub = insn.readByte();
        const RegName creg = decodeCtrlReg(sub, pm == PM_REG32, cpuType() == TLCS900L);
        const Size expected = (pm == PM_REG8 || pm == PM_ABREG8)   ? SZ_BYTE
                              : (pm == PM_REG16 || pm == PM_ABREG16) ? SZ_WORD
                                                                     : SZ_QUAD;
        if (creg == REG_UNDEF || ctrlRegSize(creg) != expected) {
            insn.setError(UNKNOWN_INSTRUCTION);
            break;
        }
        outCtrlRegName(out, creg);
        break;
    }
    case M_DISUF:
        if (insn.readByte() != 0x07)
            insn.setError(UNKNOWN_INSTRUCTION);
        break;
    case M_R32SRC:
        // MULA xrr32 (alias): the PM_REG16 prefix (D8+r) encodes XWA..XHL, so
        // map the 16-bit prefix register back to its 32-bit form. Valid ONLY
        // with PM_REG16; any other register prefix is not a MULA encoding.
        if (pm != PM_REG16) {
            insn.setError(UNKNOWN_INSTRUCTION);
            break;
        }
        outReg32(insn, out, RegName(uint8_t(REG_XWA) + (uint8_t(prefixOp.reg) - uint8_t(REG_WA))));
        break;
    default:
        break;
    }
}

// Read the instruction's prefix byte(s), addressing sub-bytes, and final
// opcode. Sets insn.prefixMode and insn.opCode; for prefix modes that carry
// an operand in the prefix slot (M_LDARREL for PM_LDAR, memory addressing
// for PM_MEMD etc.) the operand is decoded into |prefixOp|.
Error DisTlcs900::readPrefix(DisInsn &insn, Operand &prefixOp) const {
    const auto firstByte = insn.readByte();

    // 0xF3 is ambiguous: LDAR's 2-byte prefix (F3 13) or PM_MEMD's complex
    // addressing (F3 + complex sub-byte). Read the second byte once into
    // insn.sub and route based on its value.
    if (firstByte == 0xF3) {
        insn.sub = insn.readByte();
        if (insn.sub == 0x13) {
            // pageMatcher derives PM_LDAR from prefix > UINT8_MAX.
            insn.setPrefix(0xF313);
            prefixOp.val.setSigned(static_cast<int16_t>(insn.readUint16()));
            insn.setOpCode(insn.readByte());
        } else {
            // pageMatcher derives PM_MEMD from 0xF3 byte.
            insn.setPrefix(firstByte);
            if (decodeComplexAddr(insn, prefixOp))
                return insn.setError(prefixOp);
            if (!maxMode() && minModeIllegalPrefix(prefixOp))
                return insn.setError(UNKNOWN_INSTRUCTION);
            insn.setOpCode(insn.readByte());
        }
        return insn.getError();
    }

    PrefixMode pm;
    if (isPrefix(cpuType(), firstByte, pm)) {
        // Record the prefix byte so searchOpCode's pageMatcher can derive
        // the PrefixMode and accept the right page.
        insn.setPrefix(firstByte);
        if (decodePrefixAddr(insn, pm, prefixOp, firstByte))
            return insn.setError(prefixOp);
        if (!maxMode() && minModeIllegalPrefix(prefixOp))
            return insn.setError(UNKNOWN_INSTRUCTION);
        insn.setOpCode(insn.readByte());
        return insn.getError();
    }

    insn.setOpCode(firstByte);
    return insn.getError();
}

Error DisTlcs900::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    Operand prefixOp;

    if (readPrefix(insn, prefixOp))
        return _insn.setError(insn);

    if (searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    const auto dst = insn.dst();
    if (dst == M_NONE)
        return _insn.setError(insn);

    const auto *start = out.mark();
    decodeOperand(insn, out, dst, prefixOp);

    const auto src = insn.src();
    if (src != M_NONE) {
        if (out.mark() != start)
            out.comma();
        decodeOperand(insn, out, src, prefixOp);
    }

    return _insn.setError(insn);
}

}  // namespace tlcs900
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
