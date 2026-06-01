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
    : Disassembler(plugins), Config(TABLE) {
    reset();
}

// Decode the sub-byte for complex/pre-dec/post-inc addressing (0xF0-0xFF range).
// Returns the index register and the operand size (0=byte, 1=word, 2=lword).
static RegName decodeSubReg(uint8_t sub, uint8_t &size) {
    size = sub & 3;
    return decodeIndReg((sub >> 2) & 3);
}

// Decode a complex (C3/D3/E3/F3) sub-addressing.
// sub_byte == 0x03: register-indexed (reads base_reg + idx_reg bytes)
// sub_byte 0xF0+: d16-indexed (reads reg from sub-byte, then 16-bit displacement)
static Error decodeComplexAddr(DisInsn &insn, Operand &op) {
    const uint8_t sub = insn.readByte();
    if (sub == 0x03) {
        // Register-indexed: (XIx + r8)
        // Next byte selects base register (F0=XIX, F4=XIY, F8=XIZ, FC=XSP)
        const uint8_t base_byte = insn.readByte();
        // Next byte selects 8-bit index register in SFR map
        const uint8_t idx_byte = insn.readByte();
        // Decode base register: bits[3:2] of (base_byte >> 2) & 3
        op.reg = decodeIndReg((base_byte >> 2) & 3);
        // Decode index register from current bank SFR map byte:
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
    } else if ((sub & 0xF0) == 0xF0 && (sub & 0x03) == 0x01) {
        // d16-indexed: (XIx + d16)
        uint8_t size;
        op.reg = decodeSubReg(sub, size);
        op.val.setUnsigned(insn.readUint16());
        op.mode = M_IDX16;
    } else {
        op.setError(UNKNOWN_INSTRUCTION);
    }
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

    // Memory context: decode addressing mode from prefix byte.
    // 0x80-0xBF: bits[1:0] = register index (XIX=0..XSP=3)
    //   nibble 0-3: compact pre-decrement (-xrr)
    //   nibble 4-7: indirect (xrr)
    //   nibble 8-B: compact post-increment (xrr+)
    //   nibble C-F: 8-bit displacement indexed (xrr+d8)

    const uint8_t nibble = prefix & 0x0F;

    if (prefix >= 0x80 && prefix <= 0xBF) {
        op.reg = decodeIndReg(prefix & 3);
        if (nibble <= 3) {
            op.mode = M_PRDC;
        } else if (nibble <= 7) {
            op.mode = M_IND;
        } else if (nibble <= 0xB) {
            op.mode = M_PINC;
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
            decodeComplexAddr(insn, op);
            break;
        case 4: {  // complex pre-decrement
            const uint8_t sub = insn.readByte();
            uint8_t size;
            op.reg = decodeSubReg(sub, size);
            op.size = size;
            op.mode = M_PRDC;
            break;
        }
        case 5: {  // complex post-increment
            const uint8_t sub = insn.readByte();
            uint8_t size;
            op.reg = decodeSubReg(sub, size);
            op.size = size;
            op.mode = M_PINC;
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
        outRegName(out.letter('('), op.reg).letter(')');
        break;
    case M_PRDC:
        out.letter('(').letter('-');
        outRegName(out, op.reg).letter(')');
        break;
    case M_PINC:
        out.letter('(');
        outRegName(out, op.reg).letter('+').letter(')');
        break;
    case M_IDX8: {
        const auto disp = static_cast<int16_t>(op.val.getSigned());
        out.letter('(');
        outRegName(out, op.reg);
        if (disp >= 0)
            out.letter('+');
        outHex(out, disp, -8);
        out.letter(')');
        break;
    }
    case M_IDX16: {
        const auto disp = static_cast<int32_t>(op.val.getUnsigned());
        out.letter('(');
        outRegName(out, op.reg);
        if (disp >= 0)
            out.letter('+');
        outHex(out, disp, -16);
        out.letter(')');
        break;
    }
    case M_IDXR:
        out.letter('(');
        outRegName(out, op.reg).letter('+');
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

void DisTlcs900::decodeOperand(
        DisInsn &insn, StrBuffer &out, AddrMode mode, const Operand &prefixOp) const {
    const auto opc = insn.opCode();
    const auto pm = insn.pre();
    switch (mode) {
    case M_NONE:
        break;
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
                outRegName(out, decodeReg32(opc & 7));
                break;
            default:
                break;
            }
        }
        break;
    case M_REG8:
        outRegName(out, decodeReg8(opc & 7));
        break;
    case M_REG16:
        outRegName(out, decodeReg16(opc & 7));
        break;
    case M_REG32:
        outRegName(out, decodeReg32(opc & 7));
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
        outHex(out, static_cast<uint16_t>(word + step), 16);
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
        const RegName creg = decodeCtrlReg(sub, pm == PM_REG32, cpuType() == TLCS900);
        const Size expected = pm == PM_REG8 ? SZ_BYTE : pm == PM_REG16 ? SZ_WORD : SZ_QUAD;
        if (creg == REG_UNDEF || ctrlRegSize(creg) != expected) {
            insn.setError(UNKNOWN_INSTRUCTION);
            break;
        }
        outCtrlRegName(out, creg);
        break;
    }
    default:
        break;
    }
}

Error DisTlcs900::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto firstByte = insn.readByte();
    PrefixMode pm = PM_NONE;
    Operand prefixOp;

    if (isPrefix(cpuType(), firstByte, pm)) {
        // Record the prefix byte so searchOpCode's pageMatcher can derive
        // the PrefixMode and accept the right page.
        insn.setPrefix(firstByte);
        if (decodePrefixAddr(insn, pm, prefixOp, firstByte))
            return _insn.setError(insn);
        insn.setOpCode(insn.readByte());
        // Preserve NO_MEMORY etc. that readByte may have set; CpuBase::searchOpCode
        // would call setOK() and replace it with UNKNOWN_INSTRUCTION.
        if (insn.getError())
            return _insn.setError(insn);
    } else {
        insn.setOpCode(firstByte);
    }

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
