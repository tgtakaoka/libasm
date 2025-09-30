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

#include "dis_tms320f.h"
#include "reg_tms320f.h"
#include "table_tms320f.h"
#include "text_common.h"

#include <stdio.h>

namespace libasm {
namespace tms320f {

using namespace reg;
using namespace text::common;

const ValueFormatter::Plugins &DisTms320f::defaultPlugins() {
    static const struct final : ValueFormatter::Plugins {
        const HexFormatter &hex() const override { return _hex; }
        char locationSymbol() const override { return '$'; }
        const /*PROGMEM*/ char *lineComment_P() const override { return PSTR_STAR; }
        const SuffixHexFormatter _hex{'h'};
    } PLUGINS{};
    return PLUGINS;
}

DisTms320f::DisTms320f(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins), Config(TABLE) {
    reset();
}

void DisTms320f::decodeIndirect(DisInsn &insn, StrBuffer &out, uint_fast8_t mode, uint_fast8_t arn,
        uint_fast8_t disp) const {
    const auto ar = RegName(REG_AR0 + arn);
    printf("@@ indirect: mode=%02X ar=%d disp=%02X\n", mode, arn, disp);
    if (mode < 0x18) {
        const auto index = mode < 0x08 ? REG_UNDEF : (mode < 0x10 ? REG_IR0 : REG_IR1);
        out.letter('*');
        if (mode & 4)
            outRegName(out, ar);
        const auto incdec = (mode & 1) == 0 ? '+' : '-';
        out.letter(incdec);
        if ((mode & 7) >= 2)
            out.letter(incdec);
        if ((mode & 4) == 0)
            outRegName(out, ar);
        if (index != REG_UNDEF) {
            outRegName(out.letter('('), index).letter(')');
        } else {
            out.letter('(').int16(disp).letter(')');
        }
        if ((mode & 7) >= 6)
            out.letter('%');
    } else if (mode == 0x18) {
        outRegName(out.letter('*'), ar);
    } else if (mode == 0x19) {
        outRegName(out.letter('*'), ar).text_P(PSTR("++(IR0)B"));
    } else {
        insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
    }
}

void DisTms320f::decodeGeneric(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto opr = static_cast<uint16_t>(insn.opCode());
    const auto gg = (insn.opCode() >> 21) & 3;
    switch (gg) {
    case 0:
        if (opr & ~0x1F) {
            insn.setErrorIf(out, ILLEGAL_REGISTER);
        } else if ((mode == M_FGEN || mode == M_FGU8) && opr >= 8) {
            insn.setErrorIf(out, ILLEGAL_REGISTER);
        }
        outRegName(out, decodeRegName(opr & 0x1F));
        break;
    case 1:
        outAbsAddr(out.letter('@'), opr);
        break;
    case 2:
        decodeIndirect(insn, out, (opr >> 11) & 0x1F, (opr >> 8) & 7, opr & 0xFF);
        break;
    case 3:
        if (mode == M_FGEN) {
        } else {
            auto bit = (mode == M_UGEN) ? 16 : -16;
            if (mode == M_FGU8) {  // LDE
                bit = 8;
                if (opr >= 0x100) {
                    insn.setErrorIf(out, OVERFLOW_RANGE);
                    bit = 16;
                }
            }
            outHex(out, opr, bit);
        }
        break;
    }
}

void DisTms320f::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto opc = insn.opCode();
    const auto reg = decodeRegName((opc >> 16) & 0x1F);
    switch (mode) {
    case M_FREG:
        if (reg > REG_R7)
            insn.setErrorIf(out, ILLEGAL_REGISTER);
        // Fall-through
    case R_DP:
    case M_IREG:
        if (reg == REG_UNDEF) {
            insn.setErrorIf(out, ILLEGAL_REGISTER);
        } else {
            outRegName(out, reg);
        }
        break;
    case M_IGEN:
    case M_UGEN:
    case M_FGEN:
    case M_FGU8:
        decodeGeneric(insn, out, mode);
        break;
    default:
        break;
    }
}

Error DisTms320f::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    insn.setOpCode(insn.readUint32());
    if (searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    decodeOperand(insn, out, insn.mode1());
    const auto mode2 = insn.mode2();
    if (mode2 != M_NONE) {
        decodeOperand(insn, out.comma(), mode2);
        const auto mode3 = insn.mode3();
        if (mode3 != M_NONE)
            decodeOperand(insn, out.comma(), mode3);
    }
    return _insn.setError(insn);
}

}  // namespace tms320f
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
