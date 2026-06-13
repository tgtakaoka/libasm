/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "dis_h8500.h"
#include "reg_h8500.h"
#include "table_h8500.h"
#include "text_h8500.h"

namespace libasm {
namespace h8500 {

using namespace reg;
using namespace text::h8500;

namespace {

// Returns the number of EA extension bytes for the given EA byte.
uint8_t eaExtCount(uint8_t ea) {
    if ((ea >> 4) == 0xE) return 1;  // @(d:8,Rn)
    if ((ea >> 4) == 0xF) return 2;  // @(d:16,Rn)
    if (ea == 0x04 || ea == 0x05 || ea == 0x0D) return 1;
    if (ea == 0x0C || ea == 0x15 || ea == 0x1D) return 2;
    return 0;
}

}  // namespace

const ValueFormatter::Plugins &DisH8500::defaultPlugins() {
    return ValueFormatter::Plugins::hitachi();
}

DisH8500::DisH8500(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_fpAlias),
      Config(TABLE),
      _opt_fpAlias(this, &Config::setFpAlias,
              OPT_BOOL_FP_ALIAS, OPT_DESC_FP_ALIAS,
              &_opt_spAlias),
      _opt_spAlias(this, &Config::setSpAlias,
              OPT_BOOL_SP_ALIAS, OPT_DESC_SP_ALIAS) {
    reset();
}

void DisH8500::outReg(StrBuffer &out, RegName reg) const {
    if (reg == REG_R6 && !fpAlias()) {
        out.text_P(TEXT_REG_R6);
        return;
    }
    outRegName(out, reg);
}

void DisH8500::outAddrReg(StrBuffer &out, RegName reg) const {
    if (reg == REG_R7 && spAlias()) {
        out.text_P(TEXT_REG_SP);
        return;
    }
    outReg(out, reg);
}

void DisH8500::outCr(StrBuffer &out, CrName cr) const {
    outCrName(out, cr);
}

void DisH8500::outImm8(StrBuffer &out, uint8_t val) const {
    out.letter('#');
    outHex(out, val, 8);
}

void DisH8500::outImm16(DisInsn &insn, StrBuffer &out) const {
    out.letter('#');
    outHex(out, insn.readUint16Be(), 16);
}

void DisH8500::outAbs8(StrBuffer &out, uint8_t addr) const {
    out.letter('@');
    outAbsAddr(out, addr, 8);
    if (!_gnuAs)
        out.text_P(PSTR(":8"));
}

void DisH8500::outAbs16(DisInsn &insn, StrBuffer &out) const {
    out.letter('@');
    outAbsAddr(out, insn.readUint16Be());
    if (!_gnuAs)
        out.text_P(PSTR(":16"));
}

void DisH8500::outPcRel8(DisInsn &insn, StrBuffer &out) const {
    const auto delta = static_cast<int8_t>(insn.readByte());
    const auto base = insn.address() + insn.length();
    const auto target = static_cast<Config::uintptr_t>(base + delta);
    insn.setErrorIf(out, checkAddr(target));
    outRelAddr(out, target, insn.address(), 8);
}

void DisH8500::outPcRel16(DisInsn &insn, StrBuffer &out) const {
    const auto delta = static_cast<int16_t>(insn.readUint16Be());
    const auto base = insn.address() + insn.length();
    const auto target = static_cast<Config::uintptr_t>(base + delta);
    insn.setErrorIf(out, checkAddr(target));
    outRelAddr(out, target, insn.address(), 16);
}

void DisH8500::outBit(StrBuffer &out, uint8_t bit) const {
    out.letter('#');
    outHex(out, bit, 4);
}

// Decode and output the EA operand. EA extensions are pre-buffered in insn.eaExt1/eaExt2.
void DisH8500::outEaOperand(DisInsn &insn, StrBuffer &out, uint8_t ea) const {
    const auto hi = ea >> 4;
    const auto rrr = ea & 0x07;
    const auto reg = decodeReg(rrr);

    switch (hi) {
    case 0xA:  // Rn direct
        outReg(out, reg);
        break;
    case 0xB:  // @-Rn pre-decrement
        out.text_P(PSTR("@-"));
        outAddrReg(out, reg);
        break;
    case 0xC:  // @Rn+ post-increment
        out.letter('@');
        outAddrReg(out, reg);
        out.letter('+');
        break;
    case 0xD:  // @Rn indirect
        out.letter('@');
        outAddrReg(out, reg);
        break;
    case 0xE:  // @(d:8,Rn)
        out.letter('@').letter('(');
        outHex(out, static_cast<int8_t>(insn.eaExt1), -8);
        out.comma();
        outAddrReg(out, reg);
        out.letter(')');
        break;
    case 0xF:  // @(d:16,Rn)
        out.letter('@').letter('(');
        outHex(out,
                static_cast<int16_t>((static_cast<uint16_t>(insn.eaExt1) << 8) | insn.eaExt2),
                -16);
        out.comma();
        outAddrReg(out, reg);
        out.letter(')');
        break;
    default:
        // Low-range EA bytes
        switch (ea) {
        case 0x04:  // #xx:8 immediate
            outImm8(out, insn.eaExt1);
            break;
        case 0x0C:  // #xx:16 immediate
            out.letter('#');
            outHex(out,
                    static_cast<uint16_t>(
                            (static_cast<uint16_t>(insn.eaExt1) << 8) | insn.eaExt2),
                    16);
            break;
        case 0x05:  // @aa:8 short absolute (byte)
        case 0x0D:  // @aa:8 short absolute (word)
            outAbs8(out, insn.eaExt1);
            break;
        case 0x15:  // @aa:16 absolute (byte)
        case 0x1D: {  // @aa:16 absolute (word)
            const auto addr = static_cast<uint16_t>(
                    (static_cast<uint16_t>(insn.eaExt1) << 8) | insn.eaExt2);
            out.letter('@');
            outAbsAddr(out, addr);
            if (!_gnuAs)
                out.text_P(PSTR(":16"));
            break;
        }
        default:
            insn.setError(UNKNOWN_INSTRUCTION);
            break;
        }
        break;
    }
}

// Output register list byte as {R7, R6, ...} (high-numbered first).
static void outRegList(DisInsn &insn, StrBuffer &out) {
    const auto list = insn.readByte();
    out.letter('{');
    bool first = true;
    for (int i = 7; i >= 0; i--) {
        if (list & (1 << i)) {
            if (!first)
                out.comma();
            first = false;
            outRegName(out, decodeReg(i));
        }
    }
    out.letter('}');
}

// Decode a single operand into |out|. |opByte| supplies the register field
// for modes that take it from the FMT-specific opcode byte (M_REG, M_CR,
// M_BIT in GEN; M_IND/M_DISP8/M_DISP16 in SEC; etc.). Modes that consume
// following bytes (M_IMM*, M_ABS*, M_PCREL*, M_TRAPV, M_SCB, M_DISP8F)
// read them off insn directly. Per-instruction byte validation lives here
// so the per-FMT callers stay a simple src-then-dst dispatch.
void DisH8500::decodeOperand(
        DisInsn &insn, StrBuffer &out, AddrMode mode, uint8_t opByte) const {
    switch (mode) {
    case M_NONE:
        break;
    case M_REG:
        outReg(out, decodeReg(opByte & 0x07));
        break;
    case M_CR: {
        const auto cr = decodeCr(opByte & 0x07);
        if (cr == CR_UNDEF) {
            insn.setError(UNKNOWN_INSTRUCTION);
            break;
        }
        outCr(out, cr);
        break;
    }
    case M_BIT:
        outBit(out, opByte & 0x0F);
        break;
    case M_FP:
        outReg(out, REG_FP);
        break;
    case M_PCREL8:
        outPcRel8(insn, out);
        break;
    case M_PCREL16:
        outPcRel16(insn, out);
        break;
    case M_IMM8:
        outImm8(out, insn.readByte());
        break;
    case M_IMM16:
        outImm16(insn, out);
        break;
    case M_ABS8:
        outAbs8(out, insn.readByte());
        break;
    case M_ABS16:
        outAbs16(insn, out);
        break;
    case M_ABS24: {
        const auto page = insn.readByte();
        const auto addr = insn.readUint16Be();
        out.letter('@');
        outHex(out, (static_cast<uint32_t>(page) << 16) | addr, 24);
        break;
    }
    case M_REGLIST:
        outRegList(insn, out);
        break;
    case M_DISP8F: {
        const auto disp = static_cast<int8_t>(insn.readByte());
        out.letter('@').letter('(');
        outHex(out, disp, -8);
        out.comma();
        outReg(out, REG_FP);
        out.letter(')');
        break;
    }
    case M_TRAPV: {
        const auto b1 = insn.readByte();
        if ((b1 & 0xFC) != 0x10) {
            insn.setError(UNKNOWN_INSTRUCTION);
            break;
        }
        outImm8(out, b1 & 0x03);
        break;
    }
    case M_SCB: {
        // SCB's count-register byte; the dst (M_PCREL8) is dispatched next.
        const auto b1 = insn.readByte();
        if ((b1 & 0xF8) != 0xB8) {
            insn.setError(UNKNOWN_INSTRUCTION);
            break;
        }
        outReg(out, decodeReg(b1 & 0x07));
        break;
    }
    case M_IND:
        out.letter('@');
        outAddrReg(out, decodeReg(opByte & 0x07));
        break;
    case M_DISP8: {
        const auto disp = static_cast<int8_t>(insn.readByte());
        out.letter('@').letter('(');
        outHex(out, disp, -8);
        out.comma();
        outAddrReg(out, decodeReg(opByte & 0x07));
        out.letter(')');
        break;
    }
    case M_DISP16: {
        const auto disp = static_cast<int16_t>(insn.readUint16Be());
        out.letter('@').letter('(');
        outHex(out, disp, -16);
        out.comma();
        outAddrReg(out, decodeReg(opByte & 0x07));
        out.letter(')');
        break;
    }
    default:
        break;
    }
}

// Decode GEN (Format A) or EXT (Format C) instruction.
// insn.insnFmt, insn.eaByte, insn.opByte, insn.eaExt1/eaExt2 must be set.
Error DisH8500::decodeGenFmt(DisInsn &insn, StrBuffer &out) const {
    searchOpCode(cpuType(), insn, out);
    if (insn.getError())
        return insn.getError();

    const auto flags = insn.flags();
    const auto esrc = flags.src();
    const auto edst = flags.dst();

    // FMT_GEN dispatch: one of src/dst is M_EA. ANDC/ORC/XORC declare src=
    // M_IMM8 in the table but their EA byte (0x04) already encodes the imm8
    // in eaExt1, so outEaOperand handles them identically to M_EA.
    const bool srcIsEa = (esrc == M_EA || esrc == M_IMM8);
    if (srcIsEa) {
        outEaOperand(insn, out, insn.eaByte);
        if (edst != M_NONE) {
            out.comma();
            decodeOperand(insn, out, edst, insn.opByte);
        }
    } else {
        decodeOperand(insn, out, esrc, insn.opByte);
        out.comma();
        outEaOperand(insn, out, insn.eaByte);
    }
    return insn.getError();
}

// Decode secondary (0x11-prefix) instruction. Always single-operand
// (or no-operand for PRTS).
Error DisH8500::decodeSecFmt(DisInsn &insn, StrBuffer &out) const {
    searchOpCode(cpuType(), insn, out);
    if (insn.getError())
        return insn.getError();
    decodeOperand(insn, out, insn.flags().src(), insn.opByte);
    return insn.getError();
}

// Decode special format (Format B) instruction.
Error DisH8500::decodeSpcFmt(DisInsn &insn, StrBuffer &out) const {
    searchOpCode(cpuType(), insn, out);
    if (insn.getError())
        return insn.getError();
    const auto flags = insn.flags();
    decodeOperand(insn, out, flags.src(), insn.opByte);
    if (insn.getError())
        return insn.getError();
    if (flags.dst() != M_NONE) {
        out.comma();
        decodeOperand(insn, out, flags.dst(), insn.opByte);
    }
    return insn.getError();
}

Error DisH8500::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    insn.maxMode = maxMode();
    const auto b0 = insn.readByte();
    insn.setOpCode(b0);

    Error err;
    if (isEaByte(b0)) {
        insn.eaByte = b0;
        const auto cnt = eaExtCount(b0);
        if (cnt >= 1)
            insn.eaExt1 = insn.readByte();
        if (cnt >= 2)
            insn.eaExt2 = insn.readByte();
        const auto bN = insn.readByte();
        if (bN == 0x00) {
            insn.opByte = insn.readByte();
            insn.insnFmt = FMT_EXT;
        } else {
            insn.opByte = bN;
            insn.insnFmt = FMT_GEN;
        }
        insn.setOpCode(insn.opByte);
        err = decodeGenFmt(insn, out);
    } else if (isPrefix(cpuType(), b0)) {
        insn.setPrefix(b0);
        insn.opByte = insn.readByte();
        insn.insnFmt = FMT_SEC;
        insn.setOpCode(insn.opByte);
        err = decodeSecFmt(insn, out);
    } else {
        insn.opByte = b0;
        insn.insnFmt = FMT_SPC;
        err = decodeSpcFmt(insn, out);
    }
    // If decode bailed midway, the partial mnemonic / operand text would
    // otherwise leak into the caller's buffer; reset both so the caller
    // sees a clean "unknown instruction" line.
    if (err == UNKNOWN_INSTRUCTION) {
        insn.nameBuffer().reset();
        out.reset();
    }
    return _insn.setError(err);
}

}  // namespace h8500
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
