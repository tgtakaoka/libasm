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
using namespace text::option;

const ValueFormatter::Plugins &DisI8080::defaultPlugins() {
    return ValueFormatter::Plugins::intel();
}

DisI8080::DisI8080(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_zilog),
      Config(TABLE),
      _opt_zilog(this, &DisI8080::setZilogSyntax, OPT_BOOL_ZILOG_SYNTAX, OPT_DESC_ZILOG_SYNTAX) {
    reset();
}

void DisI8080::reset() {
    Disassembler::reset();
    setZilogSyntax(false);
}

Error DisI8080::setZilogSyntax(bool enable) {
    _zilogSyntax = enable;
    return OK;
}

void DisI8080::outDataReg(StrBuffer &out, uint8_t num) const {
    num &= 7;
    if (num == 6 && _zilogSyntax) {
        outRegName(out, REG_HL, true);
    } else {
        outRegName(out, decodeDataReg(num));
    }
}

void DisI8080::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto opc = insn.opCode();
    switch (mode) {
    case M_IM8:
        outHex(out, insn.readByte(), 8);
        break;
    case M_IOA:
        if (_zilogSyntax)
            out.letter('(');
        outHex(out, insn.readByte(), 8, false);
        if (_zilogSyntax)
            out.letter(')');
        break;
    case M_IM16:
        outHex(out, insn.readUint16(), 16);
        break;
    case M_ABS:
        if (_zilogSyntax)
            out.letter('(');
        outAbsAddr(out, insn.readUint16());
        if (_zilogSyntax)
            out.letter(')');
        break;
    case M_PTR:
        outRegName(out, decodePointerReg(opc >> 4, _zilogSyntax));
        break;
    case M_STK:
        outRegName(out, decodeStackReg(opc >> 4, _zilogSyntax));
        break;
    case M_IDX:
        outRegName(out, decodeIndexReg(opc >> 4, _zilogSyntax), _zilogSyntax);
        break;
    case M_SRC:
        outDataReg(out, opc);
        break;
    case M_DST:
        outDataReg(out, opc >> 3);
        break;
    case M_VEC:
        if (_zilogSyntax) {
            outHex(out, ((opc >> 3) & 7) * 8, 8);
        } else {
            outHex(out, (opc >> 3) & 7, 3);
        }
        break;
    case M_CC:
        outCcName(out, decodeCcName(opc >> 3));
        break;
    default:
        if (mode >= R_BASE && mode < I_BASE) {
            outRegName(out, RegName(mode - R_BASE));
        } else if (mode >= I_BASE) {
            outRegName(out, RegName(mode - I_BASE), true);
        }
        break;
    }
}

Error DisI8080::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readByte();
    insn.setOpCode(opc);
    if (isPrefix(cpuType(), opc)) {
        insn.setPrefix(opc);
        insn.setOpCode(insn.readByte());
        if (insn.getError())
            return _insn.setError(insn);
    }
    if (searchOpCode(cpuType(), insn, out, _zilogSyntax))
        return _insn.setError(insn);

    const auto dst = insn.dst();
    if (dst != M_NONE) {
        decodeOperand(insn, out, dst);
        const auto src = insn.src();
        if (src != M_NONE)
            decodeOperand(insn, out.comma(), src);
    }
    return _insn.setError(insn);
}

}  // namespace i8080
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
