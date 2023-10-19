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

#include "dis_i8080.h"

#include "reg_i8080.h"
#include "table_i8080.h"

namespace libasm {
namespace i8080 {

using namespace reg;

const ValueFormatter::Plugins &DisI8080::defaultPlugins() {
    return ValueFormatter::Plugins::intel();
}

DisI8080::DisI8080(const ValueFormatter::Plugins &plugins) : Disassembler(plugins), Config(TABLE) {
    reset();
}

void DisI8080::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    switch (mode) {
    case M_IM8:
        outHex(out, insn.readByte(), 8);
        break;
    case M_IOA:
        outHex(out, insn.readByte(), 8, false);
        break;
    case M_IM16:
        outHex(out, insn.readUint16(), 16);
        break;
    case M_ABS:
        outAbsAddr(out, insn.readUint16());
        break;
    case M_PTR:
        outRegName(out, decodePointerReg(insn.opCode() >> 4));
        break;
    case M_STK:
        outRegName(out, decodeStackReg(insn.opCode() >> 4));
        break;
    case M_IDX:
        outRegName(out, decodeIndexReg(insn.opCode() >> 4));
        break;
    case M_REG:
        outRegName(out, decodeDataReg(insn.opCode()));
        break;
    case M_DST:
        outRegName(out, decodeDataReg(insn.opCode() >> 3));
        break;
    case M_VEC:
        outHex(out, (insn.opCode() >> 3) & 7, 3);
        break;
    default:
        break;
    }
}

Error DisI8080::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readByte();
    insn.setOpCode(opc);
    if (TABLE.isPrefix(cpuType(), opc)) {
        insn.setOpCode(insn.readByte(), opc);
        if (insn.getError())
            return setError(insn);
    }
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return setError(insn);

    const auto dst = insn.dst();
    if (dst != M_NONE) {
        decodeOperand(insn, out, dst);
        const auto src = insn.src();
        if (src != M_NONE)
            decodeOperand(insn, out.comma(), src);
    }
    return setError(insn);
}

}  // namespace i8080
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
