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
        // For MOV.L Rm,@(disp,Rn): disp scaled *4; for MOV.B/W via @(disp,Rn)
        // also scaled by size. The mnemonic encodes the scale; for raw disp
        // output we show the scaled byte offset to match assembler input.
        const auto d = opc & 0xF;
        const auto name = insn.name();
        // disp scale is 1 (.B), 2 (.W), or 4 (.L); name suffix tells us
        auto scale = 1u;
        if (pgm_read_byte(&name[4]) == 'W' || pgm_read_byte(&name[4]) == 'w')
            scale = 2;
        else if (pgm_read_byte(&name[4]) == 'L' || pgm_read_byte(&name[4]) == 'l')
            scale = 4;
        out.text_P(PSTR("@("));
        outHex(out, d * scale, 8);
        out.letter(',');
        outRm(out, opc);
        out.letter(')');
        break;
    }
    case M_D8B:
    case M_D8W:
    case M_D8L: {
        const auto d = opc & 0xFF;
        const auto scale = (mode == M_D8B) ? 1u : (mode == M_D8W) ? 2u : 4u;
        out.text_P(PSTR("@("));
        outHex(out, d * scale, 16);
        out.letter(',');
        outRegName(out, REG_GBR);
        out.letter(')');
        break;
    }
    case M_PCW:
    case M_PCL: {
        // M_PCW:  target = (PC+4) + d*2
        // M_PCL:  target = ((PC+4) & ~3) + d*4
        // outRelAddr emits "*+N" when --relative is set (Hitachi/ASL idiom),
        // absolute form otherwise (GAS-compatible). The assembler accepts
        // both because '*' is the location symbol.
        const auto d = static_cast<uint8_t>(opc & 0xFF);
        const auto pc4 = insn.address() + 4;
        const auto target = (mode == M_PCW) ? pc4 + d * 2u : (pc4 & ~Config::uintptr_t(3)) + d * 4u;
        out.text_P(PSTR("@("));
        outRelAddr(out, target, insn.address(), (mode == M_PCW) ? 9 : 10);
        out.letter(',');
        outRegName(out, REG_PC);
        out.letter(')');
        break;
    }
    case M_IMM8: {
        const auto imm = static_cast<int8_t>(opc & 0xFF);
        out.letter('#');
        outHex(out, imm, -8);
        break;
    }
    case M_TNUM: {
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
        // LDRS/LDRE wrap the relative target in "@(*+N,PC)" form per the
        // Hitachi manual, distinguishing it from BT/BF's bare label form.
        const auto d = static_cast<int8_t>(opc & 0xFF);
        const auto target =
                static_cast<Config::uintptr_t>(insn.address() + 4 + static_cast<int32_t>(d) * 2);
        out.text_P(PSTR("@("));
        outRelAddr(out, target, insn.address(), 9);
        out.letter(',');
        outRegName(out, REG_PC);
        out.letter(')');
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
    const auto opc = insn.readUint16Be();
    insn.setOpCode(opc);

    if (searchOpCode(cpuType(), fpuType(), insn, out) != OK) {
        insn.nameBuffer().reset();
        out.reset();
        return _insn.setError(insn.getError());
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
