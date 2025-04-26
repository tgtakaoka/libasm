/*
 * copyright 2025 Tadashi G. Takaoka
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

#include "dis_mc68hc16.h"
#include "reg_mc68hc16.h"
#include "table_mc68hc16.h"

namespace libasm {
namespace mc68hc16 {

using namespace reg;

const ValueFormatter::Plugins &DisMc68HC16::defaultPlugins() {
    return ValueFormatter::Plugins::motorola();
}

DisMc68HC16::DisMc68HC16(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins), Config(TABLE) {
    reset();
}

void DisMc68HC16::decodeRegisterList(DisInsn &insn, StrBuffer &out) const {
    const auto push = (insn.opCode() == 0x34);  // PSHM
    static constexpr RegName MASK[] PROGMEM = {
            REG_D,    // PSHM push D first.
            REG_E,    //
            REG_X,    //
            REG_Y,    //
            REG_Z,    //
            REG_K,    //
            REG_CCR,  // PULM pull D first
    };
    auto mask = insn.readByte();
    if (mask & 0x80)
        insn.setErrorIf(out, UNKNOWN_POSTBYTE);
    if (mask == 0)
        insn.setErrorIf(out, OPCODE_HAS_NO_EFFECT);
    for (uint_fast8_t i = 0, n = 0; i < 7; ++i, mask >>= 1) {
        if (mask & 1) {
            const auto reg = RegName(pgm_read_byte(MASK + (push ? i : 6 - i)));
            if (n++)
                out.letter(',');
            outRegName(out, reg);
        }
    }
}

void DisMc68HC16::decodeIndexed(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    Config::ptrdiff_t disp = 0;
    const auto bits = (mode == M_IX16) ? -16 : (mode == M_IX20 ? -20 : 8);
    auto relax = false;
    if (bits == 8) {
        disp = insn.readByte();
        relax = true;
    } else if (bits == -16) {
        disp = static_cast<int16_t>(insn.readUint16());
        if (disp >= 0 && disp < 0x100)
            out.letter('>');
    } else {
        uint32_t val = insn.readByte();
        if (val >= 0x10) {
            insn.setErrorIf(out, OVERFLOW_RANGE);
            val &= 0x0F;
        }
        val <<= 16;
        val |= insn.readUint16();
        disp = signExtend(val, 20);
    }
    outHex(out, disp, bits, relax).letter(',');
    const auto index = (mode == M_IXX8) ? REG_X : decodeIndexNum(insn.opCode());
    outRegName(out, index);
}

void DisMc68HC16::decodeExtended(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto bits = (mode == M_EX16) ? 16 : 20;
    Config::uintptr_t addr = (bits == 16) ? insn.readUint16() : insn.readByte();
    if (mode == M_EX20) {
        if (addr >= 0x10) {
            insn.setErrorIf(out, OVERFLOW_RANGE);
            addr &= 0xF;
        }
        addr <<= 16;
        addr |= insn.readUint16();
    }
    const auto label = lookup(addr);
    if (label) {
        out.rtext(label);
    } else {
        outAbsAddr(out, addr, bits);
    }
}

void DisMc68HC16::decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto bits = (mode == M_RL16) ? 16 : 8;
    int16_t delta = (bits == 16) ? insn.readUint16() : static_cast<int8_t>(insn.readByte());
    const auto base = insn.address() + PC_OFFSET;
    const auto target = base + delta;
    insn.setErrorIf(out, checkAddr(target, true));
    outRelAddr(out, target, insn.address(), bits);
}

void DisMc68HC16::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    switch (mode) {
    case M_EX16:
    case M_EX20:
        decodeExtended(insn, out, mode);
        break;
    case M_IXX8:
        if (insn.mode2() == M_IXX8) {
            outHex(out, insn.opr, 8).letter(',');
            outRegName(out, REG_X);
            break;
        }
        // Fall-through
    case M_IX8:
    case M_IX16:
    case M_IX20:
        decodeIndexed(insn, out, mode);
        break;
    case M_IXE:
        outRegName(out, REG_E).letter(',');
        outRegName(out, decodeIndexNum(insn.opCode()));
        break;
    case M_RL8:
    case M_RL16:
        decodeRelative(insn, out, mode);
        break;
    case M_IM4H:
        insn.opr = insn.readByte();
        outDec(out, insn.opr >> 4, -4);
        break;
    case M_IM4L:
        outDec(out, insn.opr & 0xF, -4);
        break;
    case M_IM8:
        out.letter('#');
        if (insn.mode2() == M_IM8) {
            outHex(out, insn.opr, 8);
        } else {
            outHex(out, insn.readByte(), 8);
        }
        break;
    case M_IM16:
        out.letter('#');
        outHex(out, insn.readUint16(), 16);
        break;
    case M_LIST:
        decodeRegisterList(insn, out);
        break;
    default:
        break;
    }
}

Error DisMc68HC16::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readByte();
    insn.setOpCode(opc);
    if (isPrefix(cpuType(), opc)) {
        insn.setPrefix(opc);
        insn.setOpCode(insn.readByte());
        if (insn.getError())
            return _insn.setError(insn);
    }

    if (searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    if (insn.mode2() == M_IM8 || insn.mode2() == M_IXX8)
        insn.opr = insn.readByte();
    decodeOperand(insn, out, insn.mode1());
    const auto mode2 = insn.mode2();
    if (mode2 != M_NONE) {
        out.comma();
        decodeOperand(insn, out, mode2);
        const auto mode3 = insn.mode3();
        if (mode3 != M_NONE) {
            out.comma();
            decodeOperand(insn, out, mode3);
        }
    }
    return _insn.setError(insn);
}

}  // namespace mc68hc16
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
