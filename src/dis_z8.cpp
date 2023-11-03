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

#include "dis_z8.h"

#include "reg_z8.h"
#include "table_z8.h"

namespace libasm {
namespace z8 {

using namespace reg;

namespace {

const char OPT_BOOL_WORK_REGISTER[] PROGMEM = "work-register";
const char OPT_DESC_WORK_REGISTER[] PROGMEM = "prefer work register name than alias address";

}  // namespace

const ValueFormatter::Plugins &DisZ8::defaultPlugins() {
    return ValueFormatter::Plugins::zilog();
}

DisZ8::DisZ8(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_workRegister),
      Config(TABLE),
      _opt_workRegister(
              this, &DisZ8::setUseWorkRegister, OPT_BOOL_WORK_REGISTER, OPT_DESC_WORK_REGISTER) {
    reset();
}

void DisZ8::reset() {
    Disassembler::reset();
    setUseWorkRegister(true);
}

Error DisZ8::setUseWorkRegister(bool enable) {
    _useWorkRegister = enable;
    return OK;
}

struct DisZ8::Operand : ErrorReporter {
    Operand() : ErrorReporter(), mode(M_NONE), val16(0) {}
    void readByte(DisInsn &insn) {
        val16 = insn.readByte();
        setError(insn);
    }
    void readUint16(DisInsn &insn) {
        val16 = insn.readUint16();
        setError(insn);
    }
    void readUint16Le(DisInsn &insn) {
        val16 = insn.readUint16Le();
        setError(insn);
    }
    void setOperand(DisInsn &insn, AddrMode _mode, OprPos pos, Operand &byte1, Operand &byte2,
            Operand &word1, Operand &word2) {
        mode = _mode;
        switch (pos) {
        case OP_CODE:
            val16 = insn.opCode() >> 4;
            setOK();
            break;
        case OP_BYT1:
            val16 = byte1.val16;
            setError(byte1);
            break;
        case OP_BYT2:
            val16 = byte2.val16;
            setError(byte2);
            break;
        case OP_B1HI:
            val16 = byte1.val16 >> 4;
            setError(byte1);
            break;
        case OP_B1LO:
            val16 = byte1.val16 & 0xF;
            setError(byte1);
            break;
        case OP_W1BE:
            val16 = word1.val16;
            setError(word1);
            break;
        case OP_W2BE:
        case OP_W2LE:
            val16 = word2.val16;
            setError(word2);
        default:
            break;
        }
    }

    AddrMode mode;
    uint16_t val16;
};

namespace {

bool needByte1(OprPos pos) {
    return pos == OP_BYT1 || pos == OP_B1HI || pos == OP_B1LO;
}
bool needByte2(OprPos pos) {
    return pos == OP_BYT2;
}
bool needWord1(OprPos pos) {
    return pos == OP_W1BE;
}
bool needWord2(OprPos pos) {
    return pos == OP_W2BE;
}
bool needWord2Le(OprPos pos) {
    return pos == OP_W2LE;
}
}  // namespace

void DisZ8::readOperands(DisInsn &insn, Operand &dstOp, Operand &srcOp, Operand &extOp) const {
    const auto dstPos = insn.dstPos();
    const auto srcPos = insn.srcPos();
    const auto extPos = insn.extPos();
    Operand byte1, byte2, word1, word2;
    if (insn.hasPost()) {
        byte1.val16 = insn.post();
    } else if (needByte1(dstPos) || needByte1(srcPos) || needByte1(extPos)) {
        byte1.readByte(insn);
    }
    if (needWord1(dstPos) || needWord1(srcPos))
        word1.readUint16(insn);
    if (needByte2(dstPos) || needByte2(srcPos) || needByte2(extPos))
        byte2.readByte(insn);
    if (needWord2(dstPos) || needWord2(srcPos))
        word2.readUint16(insn);
    if (needWord2Le(dstPos) || needWord2Le(srcPos))
        word2.readUint16Le(insn);
    dstOp.setOperand(insn, insn.dst(), dstPos, byte1, byte2, word1, word2);
    srcOp.setOperand(insn, insn.src(), srcPos, byte1, byte2, word1, word2);
    extOp.setOperand(insn, insn.ext(), extPos, byte1, byte2, word1, word2);
    insn.setOK();  // clear error
}

namespace {

StrBuffer &outWorkReg(StrBuffer &out, uint8_t num, bool indir = false) {
    const auto reg = decodeRegNum(num);
    if (indir)
        out.letter('@');
    return outRegName(out, reg);
}

StrBuffer &outPairReg(StrBuffer &out, uint8_t num, bool indir = false) {
    const auto reg = decodePairRegNum(num);
    if (indir)
        out.letter('@');
    return outRegName(out, reg);
}

}  // namespace

StrBuffer &DisZ8::outRegAddr(StrBuffer &out, uint8_t addr, bool indir) const {
    if (_useWorkRegister && isWorkRegAlias(isSuper8(), addr))
        return outWorkReg(out, addr & 0xF, indir);
    if (indir)
        out.letter('@');
    const auto label = lookup(addr);
    if (label)
        return out.letter('>').text(label);
    if (addr < 16 && !indir)
        out.letter('>');
    return outAbsAddr(out, addr, 8);
}

StrBuffer &DisZ8::outPairAddr(StrBuffer &out, uint8_t addr, bool indir) const {
    if (_useWorkRegister && isWorkRegAlias(isSuper8(), addr))
        return outPairReg(out, addr & 0xF, indir);
    if (indir)
        out.letter('@');
    const auto label = lookup(addr);
    if (label)
        return out.letter('>').text(label);
    if (addr < 16 && !indir)
        out.letter('>');
    return outAbsAddr(out, addr, 8);
}

void DisZ8::decodeWorkReg(DisInsn &insn, StrBuffer &out, Operand &op) const {
    if (op.mode == M_Irr && insn.postFormat() != PF_NONE)
        op.val16 &= ~1;
    if ((op.mode == M_rr || op.mode == M_Irr) && (op.val16 & 1) != 0)
        op.setErrorIf(REGISTER_NOT_ALIGNED);
    const auto reg = (op.mode == M_r || op.mode == M_Ir) ? decodeRegNum(op.val16)
                                                         : decodePairRegNum(op.val16);
    if (op.mode == M_Ir || op.mode == M_Irr)
        out.letter('@');
    if (op.getError())
        insn.setErrorIf(out, op);
    outRegName(out, reg);
}

void DisZ8::decodeRegAddr(DisInsn &insn, StrBuffer &out, Operand &op) const {
    if (_useWorkRegister && isWorkRegAlias(isSuper8(), op.val16)) {
        op.val16 &= 0xF;
        op.mode = AddrMode(op.mode - (M_R - M_r));
        decodeWorkReg(insn, out, op);
        return;
    }
    if ((op.mode == M_RR || op.mode == M_IRR) && (op.val16 & 1) != 0)
        op.setErrorIf(REGISTER_NOT_ALIGNED);
    if (op.mode == M_IR || op.mode == M_IRR)
        out.letter('@');
    if (op.getError())
        insn.setError(out, op);
    const auto label = lookup(op.val16);
    if (label) {
        out.rtext(label);
        return;
    }
    if (op.val16 < 16 && (op.mode == M_R || op.mode == M_RR))
        out.letter('>');
    outAbsAddr(out, op.val16, 8);
}

void DisZ8::decodeImmediate(DisInsn &insn, StrBuffer &out, Operand &op) const {
    if (insn.opCode() == TableZ8::SRP) {
        const auto postFormat = insn.postFormat();
        if (postFormat == PF_NONE && (op.val16 & ~0xF0) != 0)  // Z8
            op.setErrorIf(OPERAND_NOT_ALLOWED);
        if (postFormat == PF2_0 && (op.val16 & ~0xF0) != 0x00)  // Z88 SRP
            op.setErrorIf(OPERAND_NOT_ALLOWED);
        if (postFormat == PF2_1 && (op.val16 & ~0xF8) != 0x01)  // Z88 SRP1
            op.setErrorIf(OPERAND_NOT_ALLOWED);
        if (postFormat == PF2_2 && (op.val16 & ~0xF8) != 0x02)  // Z88 SRP0
            op.setErrorIf(OPERAND_NOT_ALLOWED);
        if (postFormat == PF2_0 || postFormat == PF2_1 || postFormat == PF2_2)
            op.val16 &= ~3;
    }
    out.letter('#');
    if (op.getError())
        insn.setErrorIf(out, op);
    if (op.mode == M_IMb)
        op.val16 >>= 1;
    const auto bits = op.mode == M_IM ? 8 : (op.mode == M_IMb ? 3 : 16);
    outHex(out, op.val16, bits);
}

void DisZ8::decodeRelative(DisInsn &insn, StrBuffer &out, Operand &op) const {
    if (op.getError())
        insn.setErrorIf(out, op);
    const auto delta = static_cast<int8_t>(op.val16);
    const auto base = insn.address() + insn.length();
    Error error;
    const auto target = branchTarget(base, delta, error);
    if (error)
        insn.setErrorIf(out, error);
    outRelAddr(out, target, insn.address(), 8);
}

void DisZ8::decodeIndexed(DisInsn &insn, StrBuffer &out, Operand &op) const {
    if (op.getError())
        insn.setError(out, op);
    if (op.mode == M_XL) {
        outAbsAddr(out, op.val16);
    } else if (op.mode == M_XS) {
        const auto disp = static_cast<int8_t>(op.val16);
        if (disp > 0)
            out.letter('+');
        outDec(out, disp, -8);
    } else {  // M_X
        outHex(out, op.val16, 8);
    }
    const auto reg = (insn.hasPost() ? insn.post() & ~1 : insn.bytes()[1]) & 0xF; // OP_B1LO
    const auto idx = (op.mode == M_X) ? decodeRegNum(reg) : decodePairRegNum(reg);
    out.letter('(');
    outRegName(out, idx).letter(')');
}

void DisZ8::decodeOperand(DisInsn &insn, StrBuffer &out, Operand &op) const {
    switch (op.mode) {
    case M_IM:
    case M_IMb:
    case M_IML:
        decodeImmediate(insn, out, op);
        break;
    case M_r:
    case M_Ir:
    case M_rr:
    case M_Irr:
        decodeWorkReg(insn, out, op);
        break;
    case M_R:
    case M_IR:
    case M_RR:
    case M_IRR:
        decodeRegAddr(insn, out, op);
        break;
    case M_RA:
        decodeRelative(insn, out, op);
        break;
    case M_DA:
        if (op.getError())
            insn.setErrorIf(out, op);
        outAbsAddr(out, op.val16);
        break;
    case M_X:
    case M_XS:
    case M_XL:
        decodeIndexed(insn, out, op);
        break;
    case M_cc:
        outCcName(out, decodeCcNum(op.val16));
        break;
    default:
        break;
    }
}

Error DisZ8::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    DisInsn insn(_insn, memory, out);
    insn.setOpCode(insn.readByte());
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return setError(insn);

    Operand dstOp, srcOp, extOp;
    readOperands(insn, dstOp, srcOp, extOp);
    decodeOperand(insn, out, dstOp);
    if (insn.src() != M_NONE) {
        decodeOperand(insn, out.comma(), srcOp);
        if (insn.ext() != M_NONE)
            decodeOperand(insn, out.comma(), extOp);
    }
    return setError(insn);
}

}  // namespace z8
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
