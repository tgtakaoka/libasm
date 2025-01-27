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

#include "dis_tms9900.h"
#include "reg_tms9900.h"
#include "table_tms9900.h"

namespace libasm {
namespace tms9900 {

using namespace reg;

const ValueFormatter::Plugins &DisTms9900::defaultPlugins() {
    return ValueFormatter::Plugins::texas();
}

DisTms9900::DisTms9900(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins), Config(TABLE) {
    reset();
}

Error DisTms9900::checkPostWord(DisInsn &insn) const {
    const auto post = insn.postfix();
    const auto src = (post >> 4 & 3);
    switch (insn.dst()) {
    case M_DST0:
        if ((post & 0xF000) != 0x0000)
            insn.setErrorIf(UNKNOWN_POSTBYTE);
        break;
    case M_DST4:
        if ((post & 0xF000) != 0x4000)
            insn.setErrorIf(UNKNOWN_POSTBYTE);
        break;
    case M_CNT4:
        if ((post & 0xFC00) != 0x4000)
            insn.setErrorIf(UNKNOWN_POSTBYTE);
        break;
    case M_BIT0:
        // no auto increment mode.
        if ((post & 0xFC00) != 0x0000 || src == 3)
            insn.setErrorIf(UNKNOWN_POSTBYTE);
        break;
    default:
        break;
    }
    return insn.getError();
}

void DisTms9900::decodeModeReg(DisInsn &insn, StrBuffer &out, uint8_t modeReg) const {
    const auto mode = (modeReg >> 4) & 3;
    const auto reg = modeReg & 0xF;
    if (mode != 2) {
        if (mode == 1 || mode == 3)
            out.letter('*');
        outRegName(out, reg);
        if (mode == 3)
            out.letter('+');
        return;
    }
    out.letter('@');
    const auto addr = insn.readUint16();
    if (reg == 0)
        insn.setErrorIf(out, checkAddr(addr, !insn.byteOp()));
    outHex(out, addr, 16);
    if (reg != 0)
        outRegName(out.letter('('), reg).letter(')');
}

void DisTms9900::decodeRelative(DisInsn &insn, StrBuffer &out) const {
    const auto delta = static_cast<int8_t>(insn.opCode() & 0xff) * 2;
    const auto base = insn.address() + 2;
    const auto target = base + delta;
    insn.setErrorIf(out, checkAddr(target));
    outRelAddr(out, target, insn.address(), 9);
}

void DisTms9900::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto opc = insn.opCode();
    const auto post = insn.postfix();
    uint8_t val8;
    switch (mode) {
    case M_IMM:
        outHex(out, insn.readUint16(), 16);
        break;
    case M_REG:
        outRegName(out, opc);
        break;
    case M_DREG:
        outRegName(out, opc >> 6);
        break;
    case M_SRC:
    case M_SRC2:
        val8 = (mode == M_SRC) ? opc : post;
        decodeModeReg(insn, out, val8);
        break;
    case M_DST:
    case M_DST0:
    case M_DST4:
        val8 = ((mode == M_DST) ? opc : post) >> 6;
        decodeModeReg(insn, out, val8);
        break;
    case M_CNT4:
    case M_XOP:
    case M_CNT:
        val8 = (((mode == M_CNT4) ? post : opc) >> 6) & 0xF;
        if (mode == M_CNT4 && val8 == 0) {
            outRegName(out, REG_R0);
            break;
        }
        if (mode == M_CNT && val8 == 0)
            val8 = 16;
        outDec(out, val8, 5);
        break;
    case M_BIT0:
        val8 = (post >> 6) & 0xF;
        outDec(out, val8, 4);
        break;
    case M_REL:
        decodeRelative(insn, out);
        break;
    case M_SCNT:
        val8 = (opc >> 4) & 0xF;
        if (val8 == 0)
            outRegName(out, REG_R0);
        else
            outDec(out, val8, 4);
        break;
    case M_CRU:
        val8 = opc & 0xFF;
        outDec(out, val8, -8);
        break;
    case M_RTWP:
        val8 = opc & 7;
        if (val8)
            outHex(out, val8, 7);
        break;
    default:
        break;
    }
}

Error DisTms9900::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    insn.setOpCode(insn.readUint16());
    if (searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    const auto src = insn.src();
    if (src == M_SRC2) {
        insn.setPostfix(insn.readUint16());
        if (checkPostWord(insn))
            return _insn.setError(insn);
    }
    decodeOperand(insn, out, src);
    const auto dst = insn.dst();
    if (dst != M_NONE)
        decodeOperand(insn, out.comma(), dst);
    return _insn.setError(insn);
}

}  // namespace tms9900
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
