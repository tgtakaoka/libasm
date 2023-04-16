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
#include "text_tms9900.h"

namespace libasm {
namespace tms9900 {

using namespace reg;
using text::tms9900::TEXT_MID;

DisTms9900::DisTms9900() : Disassembler(_hexFormatter, '$'), Config(TABLE) {
    reset();
}

Error DisTms9900::checkPostWord(DisInsn &insn, StrBuffer &out) {
    const Config::opcode_t post = insn.post();
    const uint8_t src = (post >> 4 & 3);
    switch (insn.dst()) {
    case M_DST2:
        if ((post & 0xF000) == 0x4000)
            return OK;
        break;
    case M_CNT2:
        if ((post & 0xFC00) == 0x4000)
            return OK;
        break;
    case M_BIT2:
        // no auto increment mode.
        if ((post & 0xFC00) == 0x0000 && src != 3)
            return OK;
        break;
    default:
        return OK;
    }
    return decodeMacroInstructionDetect(insn, out);
}

Error DisTms9900::decodeMacroInstructionDetect(DisInsn &insn, StrBuffer &out) {
    auto save = out;
    insn.clearNameBuffer().over(out).text_P(TEXT_MID).over(insn.nameBuffer());
    save.over(out);
    return setError(UNKNOWN_INSTRUCTION);
}

Error DisTms9900::decodeModeReg(DisInsn &insn, StrBuffer &out, uint8_t mode, uint8_t reg) {
    switch (mode &= 3) {
    case 1:
    case 3:
        out.letter('*');
        /* Fall-through */
    case 0:
        outRegName(out, reg);
        if (mode == 3)
            out.letter('+');
        break;
    default:
        outHex(out.letter('@'), insn.readUint16(), 16);
        if (reg & 0xF)
            outRegName(out.letter('('), reg).letter(')');
        break;
    }
    return setError(insn);
}

Error DisTms9900::decodeRelative(DisInsn &insn, StrBuffer &out) {
    int16_t delta = static_cast<int8_t>(insn.opCode() & 0xff);
    delta *= 2;
    const auto base = insn.address() + 2;
    const auto target = branchTarget(base, delta);
    outRelAddr(out, target, insn.address(), 9);
    return OK;
}

Error DisTms9900::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) {
    const Config::opcode_t opc = insn.opCode();
    const Config::opcode_t post = insn.post();
    uint8_t val8;
    switch (mode) {
    case M_IMM:
        outHex(out, insn.readUint16(), 16);
        return setError(insn);
    case M_REG:
        outRegName(out, opc);
        return OK;
    case M_DREG:
        outRegName(out, opc >> 6);
        return OK;
    case M_SRC2:
        if (checkPostWord(insn, out)) {
            return getError();
        }
        /* Fall-through */
    case M_SRC:
        val8 = (mode == M_SRC) ? opc : post;
        return decodeModeReg(insn, out, val8 >> 4, val8);
    case M_DST2:
    case M_DST:
        val8 = ((mode == M_DST) ? opc : post) >> 6;
        return decodeModeReg(insn, out, val8 >> 4, val8);
    case M_CNT2:
    case M_XOP:
    case M_CNT:
        val8 = (((mode == M_CNT2) ? post : opc) >> 6) & 0xF;
        if (mode == M_CNT2 && val8 == 0) {
            outRegName(out, REG_R0);
            return OK;
        }
        if (mode == M_CNT && val8 == 0)
            val8 = 16;
        outDec(out, val8, 5);
        return OK;
    case M_BIT2:
        val8 = (post >> 6) & 0xF;
        outDec(out, val8, 4);
        return OK;
    case M_REL:
        return decodeRelative(insn, out);
    case M_SCNT:
        val8 = (opc >> 4) & 0xF;
        if (val8 == 0)
            outRegName(out, REG_R0);
        else
            outDec(out, val8, 4);
        return OK;
    case M_CRU:
        val8 = opc & 0xFF;
        outDec(out, val8, -8);
        return OK;
    case M_RTWP:
        val8 = opc & 7;
        if (val8 == 0 || val8 == 1 || val8 == 2 || val8 == 4) {
            if (val8)
                outHex(out, val8, 7);
            return OK;
        }
        return decodeMacroInstructionDetect(insn, out);
    default:
        return OK;
    }
}

Error DisTms9900::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    DisInsn insn(_insn, memory);
    const Config::opcode_t opCode = insn.readUint16();

    insn.setOpCode(opCode);
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return decodeMacroInstructionDetect(insn, out);
    insn.readPost();
    if (setError(insn))
        return getError();

    const auto src = insn.src();
    if (src == M_NONE)
        return OK;
    if (decodeOperand(insn, out, src))
        return getError();
    const auto dst = insn.dst();
    if (dst == M_NONE)
        return OK;
    out.comma();
    return decodeOperand(insn, out, dst);
}

}  // namespace tms9900
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
