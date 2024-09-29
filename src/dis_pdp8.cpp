/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "dis_pdp8.h"

#include "reg_pdp8.h"
#include "table_pdp8.h"

namespace libasm {
namespace pdp8 {

using namespace reg;

const ValueFormatter::Plugins &DisPdp8::defaultPlugins() {
    static const struct fianl : ValueFormatter::Plugins {
        const /*PROGMEM*/ char *lineComment_P() const override { return PSTR("/"); }
    } PLUGINS{};
    return PLUGINS;
}

DisPdp8::DisPdp8(const ValueFormatter::Plugins &plugins) : Disassembler(plugins), Config(TABLE) {
    reset();
}

void DisPdp8::reset() {
    Disassembler::reset();
    setListRadix(RADIX_8);
}

Error DisPdp8::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readUint16();
    insn.setOpCode(opc);
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);
    const auto mode = insn.mode();
    if (mode == M_MEM) {
        if (opc & 0400)
            out.letter('i').letter(' ');
        const auto page = (opc & 0200) == 0 ? 0 : pageOf(insn.address());
        const auto addr = memoryAddress(offsetOf(opc), page);
        outAbsAddr(out, addr);
    } else if (mode == M_IOT) {
        outHex(out, (opc >> 3) & 077, 6);
        outHex(out.letter(' '), opc & 7, 3);
    } else if (mode == M_MEX) {
        if (out.len() > 0)
            out.letter(' ');
        outHex(out, opc & 070, 6);
    }
    return _insn.setError(insn);
}

}  // namespace pdp8
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
