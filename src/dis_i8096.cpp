/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "dis_i8096.h"

#include "reg_i8096.h"
#include "table_i8096.h"

namespace libasm {
namespace i8096 {

using namespace reg;

static const char OPT_BOOL_ABSOLUTE[] PROGMEM = "absolute";
static const char OPT_DESC_ABSOLUTE[] PROGMEM = "zero register indexing as absolute addressing";

DisI8096::DisI8096() : Disassembler(_formatter, TableI8096::TABLE, '$'), _formatter() {
    reset();
}

void DisI8096::reset() {
    Disassembler::reset();
    _useAbsolute = false;
}

DisI8096::OptUseAbsolute::OptUseAbsolute(bool &var)
    : BoolOption(OPT_BOOL_ABSOLUTE, OPT_DESC_ABSOLUTE, var) {}

StrBuffer &DisI8096::outRegister(StrBuffer &out, uint8_t regno, bool indir) const {
    if (regno == 0 && indir && _useAbsolute)  // omit [0]
        return out;
    if (indir)
        out.letter('[');
    outDec(out, regno, 8);
    if (indir)
        out.letter(']');
    return out;
}

StrBuffer &DisI8096::outRelative(StrBuffer &out, const InsnI8096 &insn, const Operand &op) {
    if (op.mode == M_REL8) {
        // Jx: 2 bytes, DJNZ/JBx: 3 bytes
        const auto base = insn.address() + ((insn.opCode() & 0xF0) == 0xD0 ? 2 : 3);
        const auto target = branchTarget(base, op.int8());
        return outRelAddr(out, target, insn.address(), 8);
    } else if (op.mode == M_REL11) {
        const auto base = insn.address() + 2;
        // Sign exetends 11-bit number.
        const auto offset = signExtend(op.val16, 11);
        const auto target = branchTarget(base, offset);
        return outRelAddr(out, target, insn.address(), 11);
    } else {  // M_REL16:
        const auto base = insn.address() + 3;
        const auto target = branchTarget(base, op.int16());
        return outRelAddr(out, target, insn.address(), 16);
    }
}
StrBuffer &DisI8096::outOperand(StrBuffer &out, const InsnI8096 &insn, const Operand &op) {
    switch (op.mode) {
    case M_COUNT:
        if (op.regno < 16)
            return outDec(out.letter('#'), op.regno, 4);
        // Fall-through
    case M_BREG:
    case M_WREG:
    case M_LREG:
        return outRegister(out, op.regno);
    case M_INDIR:
        return outRegister(out, op.regno, true);
    case M_IDX8:
        return outRegister(outDec(out, op.int8(), -8), op.regno, true);
    case M_IDX16:
        return outRegister(outAbsAddr(out, op.val16, 16), op.regno, true);
    case M_REL8:
    case M_REL11:
    case M_REL16:
        return outRelative(out, insn, op);
    case M_BITNO:
        return outHex(out, op.val16, 3);
    case M_IMM8:
        return outHex(out.letter('#'), op.val16, 8);
    case M_IMM16:
        return outHex(out.letter('#'), op.val16, 16);
    default:
        return out;
    }
}

Error DisI8096::Operand::read(DisMemory &memory, InsnI8096 &insn, AddrMode opMode) {
    switch (mode = opMode) {
    case M_BREG:
    case M_COUNT:
        regno = insn.readByte(memory);
        break;
    case M_WREG:
    case M_INDIR:
        regno = insn.readByte(memory);
        if (!isWreg(regno))
            return setError(REGISTER_NOT_ALLOWED);
        break;
    case M_LREG:
        regno = insn.readByte(memory);
        if (!isLreg(regno))
            return setError(REGISTER_NOT_ALLOWED);
        break;
    case M_BAOP:
        switch (insn.aa()) {
        case AA_REG:
            regno = insn.readByte(memory);
            mode = M_BREG;
            break;
        case AA_IMM:
            val16 = insn.readByte(memory);
            mode = M_IMM8;
            break;
        case AA_INDIR:
            regno = insn.readByte(memory);
            if (!isWreg(regno))
                return setError(REGISTER_NOT_ALLOWED);
            mode = M_INDIR;
            break;
        case AA_IDX:
            regno = insn.readByte(memory);
            if (isWreg(regno)) {  // 8bit displacement
                val16 = static_cast<int16_t>(insn.readByte(memory));
                mode = M_IDX8;
            } else {
                regno &= ~1;
                val16 = insn.readUint16(memory);
                mode = M_IDX16;
            }
            break;
        }
        break;
    case M_WAOP:
        switch (insn.aa()) {
        case AA_REG:
            regno = insn.readByte(memory);
            if (!isWreg(regno))
                return setError(REGISTER_NOT_ALLOWED);
            mode = M_WREG;
            break;
        case AA_IMM:
            val16 = insn.readUint16(memory);
            mode = M_IMM16;
            break;
        case AA_INDIR:
            regno = insn.readByte(memory);
            if (!isWreg(regno))
                return setError(REGISTER_NOT_ALLOWED);
            mode = M_INDIR;
            break;
        case AA_IDX:
            regno = insn.readByte(memory);
            if (isWreg(regno)) {  // 8bit displacement
                val16 = static_cast<int16_t>(insn.readByte(memory));
                mode = M_IDX8;
            } else {
                regno &= ~1;
                val16 = insn.readUint16(memory);
                mode = M_IDX16;
            }
            break;
        }
        break;
    case M_REL8:
        val16 = insn.readByte(memory);
        break;
    case M_REL11:
        val16 = (static_cast<int16_t>(insn.opCode() & 7) << 8) | insn.readByte(memory);
        break;
    case M_REL16:
        val16 = insn.readUint16(memory);
        break;
    case M_BITNO:
        val16 = insn.opCode() & 7;
        break;
    default:
        break;
    }
    return setError(insn.getError());
}

Error DisI8096::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnI8096 insn(_insn);
    const auto opc = insn.readByte(memory);
    if (TableI8096::TABLE.isPrefix(opc)) {
        insn.setOpCode(insn.readByte(memory), opc);
    } else {
        insn.setOpCode(opc);
    }

    if (TableI8096::TABLE.searchOpCode(insn, out))
        return setError(insn);
    Operand dst, src1, src2;
    const bool jbx_djnz = insn.src2() == M_REL8 || insn.src1() == M_REL8;
    if (jbx_djnz) {
        dst.read(memory, insn, insn.dst());
        src1.read(memory, insn, insn.src1());
        src2.read(memory, insn, insn.src2());
    } else {
        src2.read(memory, insn, insn.src2());
        src1.read(memory, insn, insn.src1());
        dst.read(memory, insn, insn.dst());
    }
    if (src2.getError())
        return setError(src2);
    if (src1.getError())
        return setError(src1);
    if (dst.getError())
        return setError(dst);
    outOperand(out, insn, dst);
    if (insn.src1() != M_NONE)
        out.comma();
    outOperand(out, insn, src1);
    if (insn.src2() != M_NONE)
        out.comma();
    outOperand(out, insn, src2);
    return getError();
}

}  // namespace i8096
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
