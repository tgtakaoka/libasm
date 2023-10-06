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

#include "dis_im6100.h"

#include "reg_im6100.h"
#include "table_im6100.h"

namespace libasm {
namespace im6100 {

using namespace reg;

namespace {
const char OPT_BOOL_IGNORE_LITERAL[] PROGMEM = "ignore-literal";
const char OPT_DESC_IGNORE_LITERAL[] PROGMEM = "Ignore literal constant";
}  // namespace

const ValueFormatter::Plugins &DisIm6100::defaultPlugins() {
    static const struct fianl : ValueFormatter::Plugins {
        const /*PROGMEM*/ char *lineComment_P() const override { return PSTR("/"); }
    } PLUGINS{};
    return PLUGINS;
}

DisIm6100::DisIm6100(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_ignoreliteral),
      Config(TABLE),
      _opt_ignoreliteral(this, &DisIm6100::setIgnoreliteral, OPT_BOOL_IGNORE_LITERAL,
              OPT_DESC_IGNORE_LITERAL) {
    reset();
}

void DisIm6100::reset() {
    Disassembler::reset();
    setIgnoreliteral(false);
    setListRadix(RADIX_8);
}

Error DisIm6100::setIgnoreliteral(bool enable) {
    _ignoreliteral = enable;
    return OK;
}

Error DisIm6100::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    DisInsn insn(_insn, memory);
    const auto opc = insn.readUint16();
    insn.setOpCode(opc);
    if (TABLE.searchOpCode(cpuType(), insn, out)) {
        if (!_ignoreliteral) {
            insn.setOK();
            out.reset();
            outHex(insn.nameBuffer().reset().over(out), insn.opCode(), 12).over(insn.nameBuffer());
        }
        return setErrorIf(insn);
    }
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
    return setErrorIf(insn);
}

}  // namespace im6100
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
