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

#include "dis_ins8070.h"

#include "reg_ins8070.h"
#include "table_ins8070.h"

namespace libasm {
namespace ins8070 {

using namespace reg;

static const char OPT_BOOL_USE_SHARP[] PROGMEM = "use-sharp";
static const char OPT_DESC_USE_SHARP[] PROGMEM = "use # (default =) for immediate";

DisIns8070::DisIns8070()
    : Disassembler(_hexFormatter, '$', &_opt_useSharp),
      Config(TABLE),
      _opt_useSharp(
              this, &DisIns8070::setUseSharpImmediate, OPT_BOOL_USE_SHARP, OPT_DESC_USE_SHARP) {
    reset();
}

void DisIns8070::reset() {
    Disassembler::reset();
    setUseSharpImmediate(false);
}

Error DisIns8070::setUseSharpImmediate(bool enable) {
    _useSharp = enable;
    return OK;
}

Error DisIns8070::decodeImmediate(DisInsn &insn, StrBuffer &out) {
    out.letter(_useSharp ? '#' : '=');
    if (insn.oprSize() == SZ_WORD) {
        outHex(out, insn.readUint16(), 16);
    } else {
        outHex(out, insn.readByte(), 8);
    }
    return setError(insn);
}

Error DisIns8070::decodeAbsolute(DisInsn &insn, StrBuffer &out) {
    const uint8_t fetch = insn.execute() ? 1 : 0;
    const Config::uintptr_t target = insn.readUint16() + fetch;
    outAbsAddr(out, target);
    return setError(insn);
}

Error DisIns8070::decodeDirect(DisInsn &insn, StrBuffer &out) {
    const Config::uintptr_t target = 0xFF00 | insn.readByte();
    outAbsAddr(out, target);
    return setError(insn);
}

Error DisIns8070::decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) {
    const auto delta = static_cast<int8_t>(insn.readByte());
    const auto ptr = decodePointerReg(insn.opCode());
    if (mode == M_PCR) {
        const auto fetch = insn.execute() ? 1 : 0;
        const auto base = insn.address() + 1 + fetch;
        const auto target = branchTarget(base, delta);
        outRelAddr(out, target, insn.address(), 8);
        if (fetch == 0)
            outRegName(out.letter(','), REG_PC);
    } else {
        outDec(out, delta, -8).letter(',');
        outRegName(out, ptr);
    }
    return setError(insn);
}

Error DisIns8070::decodeGeneric(DisInsn &insn, StrBuffer &out) {
    const uint8_t mode = insn.opCode() & 7;
    switch (mode) {
    case 0:
        return decodeRelative(insn, out, M_PCR);
    case 1:
    case 2:
    case 3:
        return decodeRelative(insn, out, M_IDX);
    case 4:
        return decodeImmediate(insn, out);
    case 5:
        return decodeDirect(insn, out);
    default:  // 6, 7
        return decodeRelative(insn, out.letter('@'), M_IDX);
    }
}

Error DisIns8070::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) {
    switch (mode) {
    case M_AR:
        outRegName(out, REG_A);
        break;
    case M_ER:
        outRegName(out, REG_E);
        break;
    case M_SR:
        outRegName(out, REG_S);
        break;
    case M_EA:
        outRegName(out, REG_EA);
        break;
    case M_P23:
        outRegName(out, (insn.opCode() & 1) ? REG_P3 : REG_P2);
        break;
    case M_PTR:
        outRegName(out, decodePointerReg(insn.opCode()));
        break;
    case M_TR:
        outRegName(out, REG_T);
        break;
    case M_IMM:
        decodeImmediate(insn, out);
        break;
    case M_ADR:
        decodeAbsolute(insn, out);
        break;
    case M_VEC:
        outDec(out, insn.opCode() & 15, 4);
        break;
    case M_IDX:
    case M_PCR:
        decodeRelative(insn, out, mode);
        break;
    case M_GEN:
        decodeGeneric(insn, out);
        break;
    default:
        return OK;
    }
    if (isOK())
        setError(insn);
    return getError();
}

Error DisIns8070::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    DisInsn insn(_insn, memory);
    const auto opCode = insn.readByte();
    if (setError(insn))
        return getError();
    insn.setOpCode(opCode);

    if (TABLE.searchOpCode(cpuType(), insn, out))
        return setError(insn);

    const auto dst = insn.dst();
    if (dst == M_NONE)
        return setOK();
    if (decodeOperand(insn, out, dst))
        return getError();

    const auto src = insn.src();
    if (src == M_NONE)
        return setOK();
    out.comma();
    return decodeOperand(insn, out, src);
}

}  // namespace ins8070
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
