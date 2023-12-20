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

namespace {

const char OPT_BOOL_ABSOLUTE[] PROGMEM = "use-absolute";
const char OPT_DESC_ABSOLUTE[] PROGMEM = "zero register indexing as absolute addressing";

}  // namespace

const ValueFormatter::Plugins &DisI8096::defaultPlugins() {
    return ValueFormatter::Plugins::intel();
}

DisI8096::DisI8096(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_absolute),
      Config(TABLE),
      _opt_absolute(this, &DisI8096::setUseAbsolute, OPT_BOOL_ABSOLUTE, OPT_DESC_ABSOLUTE) {
    reset();
}

void DisI8096::reset() {
    Disassembler::reset();
    setUseAbsolute(false);
}

Error DisI8096::setUseAbsolute(bool enable) {
    _useAbsolute = enable;
    return OK;
}

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

StrBuffer &DisI8096::outRelative(StrBuffer &out, DisInsn &insn, const Operand &op) const {
    if (op.mode == M_REL8) {
        // Jx: 2 bytes, DJNZ/JBx: 3 bytes
        const auto base = insn.address() + ((insn.opCode() & 0xF0) == 0xD0 ? 2 : 3);
        const auto target = base + op.int8();
        insn.setErrorIf(out, checkAddr(target));
        return outRelAddr(out, target, insn.address(), 8);
    } else if (op.mode == M_REL11) {
        const auto base = insn.address() + 2;
        // Sign exetends 11-bit number.
        const auto offset = signExtend(op.val16, 11);
        const auto target = base + offset;
        insn.setErrorIf(out, checkAddr(target));
        return outRelAddr(out, target, insn.address(), 11);
    } else {  // M_REL16:
        const auto base = insn.address() + 3;
        const auto target = base + op.int16();
        insn.setErrorIf(out, checkAddr(target));
        return outRelAddr(out, target, insn.address(), 16);
    }
}

StrBuffer &DisI8096::outOperand(StrBuffer &out, DisInsn &insn, const Operand &op) const {
    switch (op.mode) {
    case M_COUNT:
        if (op.regno < 16) {
            out.letter('#');
            if (op.getError())
                insn.setErrorIf(out, op);
            return outDec(out, op.regno, 4);
        }
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
    case M_IMM16:
        out.letter('#');
        if (op.getError())
            insn.setErrorIf(out, op);
        return outHex(out, op.val16, op.mode == M_IMM8 ? 8 : 16);
    default:
        return out;
    }
}

Error DisI8096::Operand::read(DisInsn &insn, AddrMode opMode) {
    switch (mode = opMode) {
    case M_BREG:
    case M_COUNT:
        regno = insn.readByte();
        break;
    case M_WREG:
    case M_INDIR:
        regno = insn.readByte();
        if (!isWreg(regno))
            setErrorIf(OPERAND_NOT_ALIGNED);
        break;
    case M_LREG:
        regno = insn.readByte();
        if (!isLreg(regno))
            setErrorIf(OPERAND_NOT_ALIGNED);
        break;
    case M_BAOP:
        switch (insn.aa()) {
        case AA_REG:
            regno = insn.readByte();
            mode = M_BREG;
            break;
        case AA_IMM:
            val16 = insn.readByte();
            mode = M_IMM8;
            break;
        case AA_INDIR:
            regno = insn.readByte();
            if (!isWreg(regno))
                setErrorIf(OPERAND_NOT_ALIGNED);
            mode = M_INDIR;
            break;
        case AA_IDX:
            regno = insn.readByte();
            if (isWreg(regno)) {  // 8bit displacement
                val16 = static_cast<int16_t>(insn.readByte());
                mode = M_IDX8;
            } else {  // 16bit displacement
                regno &= ~1;
                val16 = insn.readUint16();
                mode = M_IDX16;
            }
            break;
        }
        break;
    case M_WAOP:
        switch (insn.aa()) {
        case AA_REG:
            regno = insn.readByte();
            if (!isWreg(regno))
                setErrorIf(OPERAND_NOT_ALIGNED);
            mode = M_WREG;
            break;
        case AA_IMM:
            val16 = insn.readUint16();
            mode = M_IMM16;
            break;
        case AA_INDIR:
            regno = insn.readByte();
            if (!isWreg(regno))
                setErrorIf(OPERAND_NOT_ALIGNED);
            mode = M_INDIR;
            break;
        case AA_IDX:
            regno = insn.readByte();
            if (isWreg(regno)) {  // 8bit displacement
                val16 = static_cast<int16_t>(insn.readByte());
                mode = M_IDX8;
            } else {
                regno &= ~1;
                val16 = insn.readUint16();
                mode = M_IDX16;
            }
            if (regno == 0 && !isWreg(val16))
                setErrorIf(OPERAND_NOT_ALIGNED);
            break;
        }
        break;
    case M_REL8:
        val16 = insn.readByte();
        break;
    case M_REL11:
        val16 = (static_cast<int16_t>(insn.opCode() & 7) << 8) | insn.readByte();
        break;
    case M_REL16:
        val16 = insn.readUint16();
        break;
    case M_BITNO:
        val16 = insn.opCode() & 7;
        break;
    default:
        break;
    }
    return getError();
}

Error DisI8096::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readByte();
    insn.setOpCode(opc);
    if (TABLE.isPrefix(cpuType(), opc)) {
        insn.setPrefix(opc);
        insn.setOpCode(insn.readByte());
        if (insn.getError())
            return _insn.setError(insn);
    }
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return _insn.setErrorIf(insn);

    const auto jbx_djnz = insn.src2() == M_REL8 || insn.src1() == M_REL8;
    Operand src2, src1, dst;
    if (jbx_djnz) {
        if (dst.read(insn, insn.dst()))
            insn.setErrorIf(out, dst);
        outOperand(out, insn, dst);
        if (insn.src1() != M_NONE) {
            out.comma();
            if (src1.read(insn, insn.src1()))
                insn.setErrorIf(out, src1);
            outOperand(out, insn, src1);
        }
        if (insn.src2() != M_NONE) {
            out.comma();
            if (src2.read(insn, insn.src2()))
                insn.setErrorIf(out, src2);
            outOperand(out, insn, src2);
        }
    } else {
        src2.read(insn, insn.src2());
        src1.read(insn, insn.src1());
        if (insn.dst() != M_NONE) {
            if (dst.read(insn, insn.dst()))
                insn.setErrorIf(out, dst);
            outOperand(out, insn, dst);
        }
        if (insn.src1() != M_NONE) {
            out.comma();
            if (src1.getError())
                insn.setErrorIf(out, src1);
            outOperand(out, insn, src1);
        }
        if (insn.src2() != M_NONE) {
            out.comma();
            if (src2.getError())
                insn.setErrorIf(out, src2);
            outOperand(out, insn, src2);
        }
    }
    return _insn.setError(insn);
}

}  // namespace i8096
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
