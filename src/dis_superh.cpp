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

#include "dis_superh.h"

#include "option_base.h"
#include "reg_superh.h"
#include "table_superh.h"

namespace libasm {
namespace superh {

using namespace reg;
using namespace text::option;

const ValueFormatter::Plugins &DisSuperH::defaultPlugins() {
    return ValueFormatter::Plugins::hitachi();
}

DisSuperH::DisSuperH(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_fpu),
      Config(TABLE),
      _opt_fpu(this, &Config::setFpuName, OPT_TEXT_FPU, OPT_DESC_FPU) {
    reset();
}

void DisSuperH::reset() {
    Disassembler::reset();
#if defined(LIBASM_SUPERH_NOFPU)
    setFpuType(FPU_NONE);
#else
    setFpuType(FPU_ON);  // disassembler decodes FPU instructions by default
#endif
}

// SH-2A's FPU is optional; SH-2E's is mandatory and implied by the CPU
// name.  Only SH-2A needs an explicit `option "fpu", "true"` pseudo to
// survive a disassemble-then-reassemble round-trip.
bool DisSuperH::currentOption(const char *&name, const char *&value) const {
    if (cpuType() == SH2A && fpuType() != FPU_NONE) {
        name = "fpu";
        value = "true";
        return true;
    }
    return false;
}

namespace {

// 16-bit instruction word -> 4-bit fields.
inline uint8_t fieldN(uint16_t w) {
    return (w >> 8) & 0xF;
}
inline uint8_t fieldM(uint16_t w) {
    return (w >> 4) & 0xF;
}

void outRn(StrBuffer &out, uint16_t w) {
    outRegName(out, RegName(fieldN(w)));
}

void outRm(StrBuffer &out, uint16_t w) {
    outRegName(out, RegName(fieldM(w)));
}

// Disassembly suffix character for the access size ('B'/'W'/'L'); 0 for none.
char insnSizeSuffix(InsnSize sz) {
    switch (sz) {
    case ISZ_BYTE:
        return 'B';
    case ISZ_WORD:
        return 'W';
    case ISZ_LONG:
        return 'L';
    default:
        return 0;
    }
}

// Displacement scale for size-bearing displacement modes (.B=1, .W=2, .L=4);
// ISZ_NONE -> 4 so MOVA's suffix-less PC-relative operand scales by 4.
unsigned dispScale(InsnSize sz) {
    switch (sz) {
    case ISZ_BYTE:
        return 1;
    case ISZ_WORD:
        return 2;
    default:
        return 4;
    }
}

}  // namespace

void DisSuperH::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto opc = insn.opCode();
    switch (mode) {
    case M_NONE:
        break;
    case M_RN:
        outRn(out, opc);
        break;
    case M_RM:
        outRm(out, opc);
        break;
    case M_R0:
        outRegName(out, REG_R0);
        break;
    case M_IRN:
        out.letter('@');
        outRn(out, opc);
        break;
    case M_IRM:
        out.letter('@');
        outRm(out, opc);
        break;
    case M_INCN:
        out.letter('@');
        outRn(out, opc);
        out.letter('+');
        break;
    case M_INCM:
        out.letter('@');
        outRm(out, opc);
        out.letter('+');
        break;
    case M_DECN:
        out.text_P(PSTR("@-"));
        outRn(out, opc);
        break;
    case M_IDXN:
        out.text_P(PSTR("@(R0,"));
        outRn(out, opc);
        out.letter(')');
        break;
    case M_IDXM:
        out.text_P(PSTR("@(R0,"));
        outRm(out, opc);
        out.letter(')');
        break;
    case M_D4N: {
        // Always MOV.L Rm,@(disp,Rn); disp scaled by 4.
        const auto d = opc & 0xF;
        out.text_P(PSTR("@("));
        outHex(out, d * 4, 8);
        out.letter(',');
        outRn(out, opc);
        out.letter(')');
        break;
    }
    case M_D4M: {
        // @(disp,Rm): disp scaled by the access size (.B=1, .W=2, .L=4). We
        // print the scaled byte offset to match assembler input.
        const auto d = opc & 0xF;
        const auto scale = dispScale(insn.effectiveSize());
        out.text_P(PSTR("@("));
        outHex(out, d * scale, 8);
        out.letter(',');
        outRm(out, opc);
        out.letter(')');
        break;
    }
    case M_D8: {
        const auto d = opc & 0xFF;
        const auto scale = dispScale(insn.effectiveSize());
        out.text_P(PSTR("@("));
        outHex(out, d * scale, 16);
        out.letter(',');
        outRegName(out, REG_GBR);
        out.letter(')');
        break;
    }
    case M_PCREL: {
        // .W: target = (PC+4) + d*2; .L (and MOVA): target = ((PC+4)&~3) + d*4.
        const auto d = static_cast<uint8_t>(opc & 0xFF);
        const auto pc4 = insn.address() + 4;
        const auto word = dispScale(insn.effectiveSize()) == 2;
        const auto target = word ? pc4 + d * 2u : (pc4 & ~Config::uintptr_t(3)) + d * 4u;
        // Emit the bare target (manual table 3-1 "symbol" form), never the
        // "@(disp,PC)" notation: a label if one is defined at the target,
        // otherwise the absolute address ("$+N" under --relative).
        if (const auto *label = lookup(target, config().addressWidth())) {
            out.rtext(label);
        } else {
            outRelAddr(out, target, insn.address(), word ? 9 : 10);
        }
        break;
    }
    case M_IMM8: {
        const auto imm = static_cast<int8_t>(opc & 0xFF);
        out.letter('#');
        outHex(out, imm, -8);
        break;
    }
    case M_TVEC: {
        const auto imm = opc & 0xFF;
        out.letter('#');
        outHex(out, imm, 8);
        break;
    }
    case M_REL8: {
        const auto d = static_cast<int8_t>(opc & 0xFF);
        const auto target =
                static_cast<Config::uintptr_t>(insn.address() + 4 + static_cast<int32_t>(d) * 2);
        outRelAddr(out, target, insn.address(), 9);
        break;
    }
    case M_REL8P: {
        // LDRS/LDRE emit the bare target (label if defined, else address), like
        // the PC-relative loads -- never the "@(disp,PC)" notation.
        const auto d = static_cast<int8_t>(opc & 0xFF);
        const auto target =
                static_cast<Config::uintptr_t>(insn.address() + 4 + static_cast<int32_t>(d) * 2);
        if (const auto *label = lookup(target, config().addressWidth())) {
            out.rtext(label);
        } else {
            outRelAddr(out, target, insn.address(), 9);
        }
        break;
    }
    case M_REL12: {
        auto d = static_cast<int16_t>(opc & 0xFFF);
        if (d & 0x800)
            d |= 0xF000;  // sign-extend 12->16
        const auto target =
                static_cast<Config::uintptr_t>(insn.address() + 4 + static_cast<int32_t>(d) * 2);
        outRelAddr(out, target, insn.address(), 13);
        break;
    }
    case M_SR:
        outRegName(out, REG_SR);
        break;
    case M_GBR:
        outRegName(out, REG_GBR);
        break;
    case M_VBR:
        outRegName(out, REG_VBR);
        break;
    case M_MACH:
        outRegName(out, REG_MACH);
        break;
    case M_MACL:
        outRegName(out, REG_MACL);
        break;
    case M_PR:
        outRegName(out, REG_PR);
        break;
    case M_DSR:
        outRegName(out, REG_DSR);
        break;
    case M_A0:
        outRegName(out, REG_A0);
        break;
    case M_X0:
        outRegName(out, REG_X0);
        break;
    case M_X1:
        outRegName(out, REG_X1);
        break;
    case M_Y0:
        outRegName(out, REG_Y0);
        break;
    case M_Y1:
        outRegName(out, REG_Y1);
        break;
    case M_MOD:
        outRegName(out, REG_MOD);
        break;
    case M_RS:
        outRegName(out, REG_RS);
        break;
    case M_RE:
        outRegName(out, REG_RE);
        break;
    case M_FRN:
        outRegName(out, decodeFn(opc));
        break;
    case M_FRM:
        outRegName(out, decodeFm(opc));
        break;
    case M_FPUL:
        outRegName(out, REG_FPUL);
        break;
    case M_FPSCR:
        outRegName(out, REG_FPSCR);
        break;
    case M_DRN:
        outRegName(out, decodeDn(opc));
        break;
    case M_DRM:
        outRegName(out, decodeDm(opc));
        break;
    case M_IMM3: {
        const auto v = (opc >> 1) & 0x7;  // bits[3:1]
        out.letter('#');
        outHex(out, v, 3);
        break;
    }
    case M_IMM20: {
        // 20-bit signed: hi 4 bits in opc[7:4], lo 16 in opc2
        const auto hi = (opc >> 4) & 0xF;
        const auto lo = insn.opCode2() & 0xFFFFu;
        auto val = static_cast<int32_t>((hi << 16) | lo);
        if (val & 0x80000)
            val |= 0xFFF00000u;  // sign-extend 20 -> 32
        out.letter('#');
        outHex(out, val, -20);
        break;
    }
    case M_IMM20S: {
        // 20-bit signed shifted left 8: hi 4 in opc[7:4], next 16 in opc2;
        // print the "real" 28-bit value (hi:lo << 8).
        const auto hi = (opc >> 4) & 0xF;
        const auto lo = insn.opCode2() & 0xFFFFu;
        auto val = static_cast<int32_t>((hi << 24) | (lo << 8));
        if (val & 0x08000000)
            val |= 0xF0000000u;  // sign-extend 28 -> 32
        out.letter('#');
        outHex(out, val, -28);
        break;
    }
    case M_D12N:
    case M_D12M: {
        // 12-bit displacement is in opc2[11:0]; scaled 1/2/4 by the access size.
        const auto scale = dispScale(insn.effectiveSize());
        const auto d = insn.opCode2() & 0xFFFu;
        out.text_P(PSTR("@("));
        outHex(out, d * scale, 16);
        out.letter(',');
        outRegName(out, (mode == M_D12N) ? decodeRn(opc) : decodeRm(opc));
        out.letter(')');
        break;
    }
    case M_BANK:
        outRegName(out, REG_R0);
        break;
    case M_PUSH:
        out.text_P(PSTR("@-"));
        outRegName(out, REG_R15);
        break;
    case M_PULL:
        out.letter('@');
        outRegName(out, REG_R15);
        out.letter('+');
        break;
    case M_IGBR:
        out.text_P(PSTR("@(R0,"));
        outRegName(out, REG_GBR);
        out.letter(')');
        break;
    }
}

Error DisSuperH::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    // SH instructions are 16-bit; PC must be even.
    if (insn.address() & 1)
        return _insn.setError(OPERAND_NOT_ALIGNED);
    const auto opc = insn.readUint16();
    insn.setOpCode(opc);

    if (searchOpCode(cpuSpec(), insn, out) != OK) {
        insn.nameBuffer().reset();
        out.reset();
        return _insn.setError(insn.getError());
    }

    // SH-2A 32-bit instructions have a second 16-bit word carrying the
    // displacement, immediate or bit field. The matched entry's longForm
    // flag tells us to fetch it.
    if (insn.longForm())
        insn.setOpCode2(insn.readUint16());

    // Append the access-size suffix (.B/.W/.L). For ISZ_DATA the size comes
    // from the opcode bits; fixed entries carry it directly; ISZ_NONE (and the
    // float forms whose suffix is part of the name) get none.
    const auto suffix = insnSizeSuffix(insn.effectiveSize());
    if (suffix) {
        insn.nameBuffer().letter('.');
        insn.appendName(out, suffix);
    }

    const auto src = insn.src();
    const auto dst = insn.dst();
    if (src != M_NONE) {
        decodeOperand(insn, out, src);
        if (dst != M_NONE) {
            out.comma();
            decodeOperand(insn, out, dst);
        }
    }
    return _insn.setError(insn.getError());
}

}  // namespace superh
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
