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

#include "dis_i8051.h"

#include "reg_i8051.h"
#include "table_i8051.h"

namespace libasm {
namespace i8051 {

using namespace reg;

const ValueFormatter::Plugins &DisI8051::defaultPlugins() {
    return ValueFormatter::Plugins::intel();
}

DisI8051::DisI8051(const ValueFormatter::Plugins &plugins) : Disassembler(plugins), Config(TABLE) {
    reset();
}

Error DisI8051::decodeRelative(DisInsn &insn, StrBuffer &out) {
    const auto delta = static_cast<int8_t>(insn.readByte());
    const auto base = insn.address() + insn.length();
    const auto target = branchTarget(base, delta);
    outRelAddr(out, target, insn.address(), 8);
    return setError(insn);
}

Error DisI8051::decodeBitAddr(DisInsn &insn, StrBuffer &out) {
    uint8_t val8 = insn.readByte();
    const uint8_t addr8 = (val8 & 0x80) ? (val8 & ~7) : ((val8 >> 3) + 0x20);
    val8 &= 7;
    outAbsAddr(out, addr8, 8).letter('.');
    outHex(out, val8, 3);
    return setError(insn);
}

Error DisI8051::decodeRReg(DisInsn &insn, StrBuffer &out, const AddrMode mode) {
    const auto reg = decodeRRegNum(insn.opCode() & (mode == M_IDIRR ? 1 : 7));
    if (mode == M_IDIRR)
        out.letter('@');
    outRegName(out, reg);
    return setOK();
}

Error DisI8051::decodeAddress(DisInsn &insn, StrBuffer &out, const AddrMode mode) {
    if (mode == M_ADR8) {
        outAbsAddr(out, insn.readByte(), 8);
    } else if (mode == M_ADR11) {
        const uint8_t val8 = insn.readByte();
        Config::uintptr_t addr = (insn.address() + insn.length()) & 0xF800;
        addr |= (insn.opCode() & 0xE0) << 3;
        addr |= val8;
        outAbsAddr(out, addr);
    } else {
        outAbsAddr(out, insn.readUint16());
    }
    return setError(insn);
}

Error DisI8051::decodeImmediate(DisInsn &insn, StrBuffer &out, const AddrMode mode) {
    out.letter('#');
    if (mode == M_IMM8) {
        outHex(out, insn.readByte(), 8);
    } else {
        outHex(out, insn.readUint16(), 16);
    }
    return setError(insn);
}

Error DisI8051::decodeOperand(DisInsn &insn, StrBuffer &out, const AddrMode mode) {
    switch (mode) {
    case M_NONE:
        break;
    case M_REL:
        return decodeRelative(insn, out);
    case M_AREG:
        outRegName(out, REG_A);
        break;
    case M_RREG:
    case M_IDIRR:
        return decodeRReg(insn, out, mode);
    case M_CREG:
        outRegName(out, REG_C);
        break;
    case M_IDIRD:
        out.letter('@');
        /* Fall-through */
    case M_DREG:
        outRegName(out, REG_DPTR);
        break;
    case M_ABREG:
        outRegName(out, REG_AB);
        break;
    case M_ADR8:
    case M_ADR11:
    case M_ADR16:
        return decodeAddress(insn, out, mode);
    case M_NOTAD:
        out.letter('/');
        /* Fall-through */
    case M_BITAD:
        return decodeBitAddr(insn, out);
    case M_IMM8:
    case M_IMM16:
        return decodeImmediate(insn, out, mode);
    case M_INDXD:
        outRegName(out.letter('@'), REG_A).letter('+');
        outRegName(out, REG_DPTR);
        break;
    case M_INDXP:
        outRegName(out.letter('@'), REG_A).letter('+');
        outRegName(out, REG_PC);
        break;
    }
    return setOK();
}

Error DisI8051::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    DisInsn insn(_insn, memory);
    const auto opCode = insn.readByte();
    insn.setOpCode(opCode);
    if (setError(insn))
        return getError();

    if (TABLE.searchOpCode(cpuType(), insn, out))
        return setError(insn);

    const auto dst = insn.dst();
    const auto src = insn.src();
    if (dst == M_ADR8 && src == M_ADR8) {  // MOV dst,src
        const auto src8 = insn.readByte();
        const auto dst8 = insn.readByte();
        outAbsAddr(out, dst8, 8).comma();
        outAbsAddr(out, src8, 8);
    } else {
        if (dst != M_NONE) {
            if (decodeOperand(insn, out, dst))
                return getError();
        }
        if (src != M_NONE) {
            out.comma();
            if (decodeOperand(insn, out, src))
                return getError();
        }
    }
    const auto ext = insn.ext();
    if (ext != M_NONE) {
        out.comma();
        if (decodeOperand(insn, out, ext))
            return getError();
    }
    return setError(insn);
}

}  // namespace i8051
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
