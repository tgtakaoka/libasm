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

#include "dis_z8000.h"

#include "reg_z8000.h"
#include "table_z8000.h"

namespace libasm {
namespace z8000 {

using namespace reg;

namespace {

const char OPT_BOOL_IOADDR_PREFIX[] PROGMEM = "ioaddr-prefix";
const char OPT_DESC_IOADDR_PREFIX[] PROGMEM = "I/O address prefix # (default none)";
const char OPT_BOOL_SHORT_DIRECT[] PROGMEM = "short-direct";
const char OPT_DESC_SHORT_DIRECT[] PROGMEM = "short direct addressing as ||";

}  // namespace

const ValueFormatter::Plugins &DisZ8000::defaultPlugins() {
    return ValueFormatter::Plugins::zilog();
}

DisZ8000::DisZ8000(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_shortDirect),
      Config(TABLE),
      _opt_shortDirect(this, &DisZ8000::setShortDirect, OPT_BOOL_SHORT_DIRECT,
              OPT_DESC_SHORT_DIRECT, &_opt_ioaddrPrefix),
      _opt_ioaddrPrefix(
              this, &DisZ8000::setIoAddressPrefix, OPT_BOOL_IOADDR_PREFIX, OPT_DESC_IOADDR_PREFIX) {
    reset();
}

void DisZ8000::reset() {
    Disassembler::reset();
    setShortDirect(true);
    setIoAddressPrefix(false);
}

Error DisZ8000::setShortDirect(bool enable) {
    _shortDirect = enable;
    return OK;
}

Error DisZ8000::setIoAddressPrefix(bool enable) {
    _ioAddressPrefix = enable;
    return OK;
}

void DisZ8000::decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode, OprSize size) const {
    out.letter('#');
    if (mode == M_IM8) {
        outHex(out, insn.opCode() & 0xFF, 8);
    } else if (mode == M_SCNT || mode == M_NCNT) {
        auto data = insn.readUint16();
        if (size == SZ_BYTE && (data & 0xFF00) != 0)
            insn.setErrorIf(out, ILLEGAL_OPERAND);
        const auto count =
                (size == SZ_BYTE) ? static_cast<int8_t>(data) : static_cast<int16_t>(data);
        if (count < 0) {
            TABLE.searchOpCodeAlias(cpuType(), insn, out);
            data = -count;
        }
        if (size == SZ_BYTE && data > 8)
            insn.setErrorIf(out, ILLEGAL_OPERAND);
        if (size == SZ_WORD && data > 16)
            insn.setErrorIf(out, ILLEGAL_OPERAND);
        if (size == SZ_LONG && data > 32)
            insn.setErrorIf(out, ILLEGAL_OPERAND);
        outDec(out, data, 6);
    } else if (size == SZ_BYTE) {
        outHex(out, insn.readUint16(), 8);
    } else if (size == SZ_WORD) {
        outHex(out, insn.readUint16(), 16);
    } else if (size == SZ_LONG) {
        outHex(out, insn.readUint32(), 32);
    }
}

void DisZ8000::decodeFlags(DisInsn &insn, StrBuffer &out, uint8_t flags) const {
    flags &= 0xF;
    if (flags == 0)
        insn.setErrorIf(out, OPCODE_HAS_NO_EFFECT);
    outFlagNames(out, flags);
}

void DisZ8000::decodeGeneralRegister(
        DisInsn &insn, StrBuffer &out, AddrMode mode, uint8_t num) const {
    auto size = insn.size();
    if (mode == M_DR)
        size = OprSize(size + 1);
    if (mode == M_IR || mode == M_BA || mode == M_BX)
        size = SZ_ADDR;
    if (mode == M_WR || mode == M_WR07 || mode == M_IRIO)
        size = SZ_WORD;

    if (mode == M_IR || mode == M_IRIO)
        out.letter('@');

    if (mode == M_WR07 && num >= 8)
        insn.setErrorIf(out, REGISTER_NOT_ALLOWED);
    if ((mode == M_IR || mode == M_IRIO) && num == 0)
        insn.setErrorIf(out, REGISTER_NOT_ALLOWED);
    if (insn.isTranslateInsn()) {
        if (mode == M_IR && num < 2)
            insn.setErrorIf(out, REGISTER_NOT_ALLOWED);
        if (mode == M_WR && num == 1)
            insn.setErrorIf(out, REGISTER_NOT_ALLOWED);
    }
    if (insn.isCheckRegisterOverlap())
        checkRegisterOverlap(insn, out);

    RegName reg;
    if (!decodeRegNum(segmentedModel(), num, size, reg))
        insn.setErrorIf(out, ILLEGAL_REGISTER);
    outRegName(out, reg);
}

void DisZ8000::decodeControlRegister(
        DisInsn &insn, StrBuffer &out, uint8_t ctlNum, OprSize size) const {
    const auto reg = decodeCtlReg(segmentedModel(), ctlNum);
    if (reg == REG_UNDEF)
        insn.setErrorIf(out, UNKNOWN_REGISTER);
    if (size == SZ_BYTE && reg != REG_FLAGS)
        insn.setErrorIf(out, ILLEGAL_SIZE);
    if (size == SZ_WORD && reg == REG_FLAGS)
        insn.setErrorIf(out, ILLEGAL_SIZE);
    outRegName(out, reg);
}

void DisZ8000::decodeBaseAddressing(
        DisInsn &insn, StrBuffer &out, AddrMode mode, uint8_t num) const {
    num &= 0xf;
    if (num == 0)
        insn.setErrorIf(out, REGISTER_NOT_ALLOWED);
    decodeGeneralRegister(insn, out, mode, num);
    out.letter('(');
    if (mode == M_BA) {
        outHex(out.letter('#'), insn.readUint16(), -16);
    } else {  // M_BX
        decodeGeneralRegister(insn, out, M_WR, insn.post() >> 8);
    }
    out.letter(')');
}

void DisZ8000::decodeGenericAddressing(
        DisInsn &insn, StrBuffer &out, AddrMode mode, uint8_t num) const {
    num &= 0xF;
    const auto addressing = insn.opCode() >> 14;
    if (addressing == 0) {
        if (num == 0) {
            if (mode == M_GENI) {
                constexpr Config::opcode_t LDB_R_GENI = 0x2000;
                if ((insn.opCode() & ~0x000F) == LDB_R_GENI)
                    insn.setErrorIf(out, SUBOPTIMAL_INSTRUCTION);
                return decodeImmediate(insn, out, M_IM, insn.size());  // M_IM
            }
        }
        return decodeGeneralRegister(insn, out, M_IR, num);  // M_IR
    }
    if (addressing == 2) {
        if (mode == M_GENA)
            insn.setErrorIf(out, REGISTER_NOT_ALLOWED);
        return decodeGeneralRegister(insn,out, mode, num); // M_R
    }
    if (addressing == 1) {  // M_DA/M_X
        decodeDirectAddress(insn, out);
        if (num) {
            out.letter('(');
            decodeGeneralRegister(insn, out, M_WR, num);
            out.letter(')');
        }
    }
}

void DisZ8000::decodeDirectAddress(DisInsn &insn, StrBuffer &out) const {
    const auto addr = insn.readUint16();
    if (segmentedModel()) {
        const uint32_t seg = static_cast<uint32_t>(addr & 0x7F00) << 8;
        uint16_t off = static_cast<uint8_t>(addr);
        auto shortDirect = _shortDirect;
        if (addr & 0x8000) {
            if (addr & 0x00FF)
                insn.setErrorIf(out, ILLEGAL_OPERAND);
            off = insn.readUint16();
            shortDirect = false;
        }
        const uint32_t linear = seg | off;
        if (shortDirect)
            out.letter('|');
        outAbsAddr(out, linear);
        if (shortDirect)
            out.letter('|');
    } else {
        outAbsAddr(out, addr);
    }
}

void DisZ8000::decodeRelativeAddressing(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    int16_t delta = 0;
    if (mode == M_RA) {
        delta = static_cast<int16_t>(insn.readUint16());
    } else if (mode == M_RA12) {
        // Sign extends 12-bit number as 0x800 is a sign bit.
        const auto ra12 = signExtend(insn.opCode(), 12);
        delta = -ra12 * 2;
    } else if (mode == M_RA8) {
        // Sign extends 8-bit number as 0x80 is a sign bit
        const auto ra8 = signExtend(insn.opCode(), 8);
        delta = ra8 * 2;
    } else if (mode == M_RA7) {
        // Unsigned 7-bit as always negative offset.
        const auto ra7 = insn.opCode() & 0x7F;
        delta = -static_cast<int16_t>(ra7) * 2;
    }
    const auto base = insn.address() + insn.length();
    Error error;
    const auto target = branchTarget(base, delta, error);
    if (error)
        insn.setErrorIf(out, error);
    outRelAddr(out, target, insn.address(), mode == M_RA ? 16 : 13);
}

namespace {

uint8_t modePos(const DisInsn &insn, OprPos pos) {
    switch (pos) {
    case OP_C0:
        return (insn.opCode() & 0xF);
    case OP_C4:
        return (insn.opCode() >> 4) & 0xF;
    case OP_C8:
        return (insn.opCode() >> 8) & 0xF;
    case OP_P0:
        return (insn.post() & 0xF);
    case OP_P4:
        return (insn.post() >> 4) & 0xF;
    case OP_P8:
        return (insn.post() >> 8) & 0xF;
    default:
        return 0;
    }
}

}  // namespace

void DisZ8000::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos) const {
    auto num = modePos(insn, pos);
    switch (mode) {
    case M_NONE:
        break;
    case M_BA:
    case M_BX:
        decodeBaseAddressing(insn, out, mode, num);
        break;
    case M_CC:
        outCcName(out, decodeCcNum(num));
        break;
    case M_CTL:
        decodeControlRegister(insn, out, num, insn.size());
        break;
    case M_FLAG:
        decodeFlags(insn, out, num);
        break;
    case M_IO:
        if (_ioAddressPrefix)
            out.letter('#');
        outAbsAddr(out, insn.readUint16(), 16);
        break;
    case M_WR:
    case M_WR07:
    case M_R:
    case M_DR:
    case M_IR:
    case M_IRIO:
        decodeGeneralRegister(insn, out, mode, num);
        break;
    case M_INTR:
        num &= 3;
        if (num == 3)
            insn.setError(out, OPCODE_HAS_NO_EFFECT);
        outIntrNames(out, num);
        break;
    case M_RA:
    case M_RA12:
    case M_RA8:
    case M_RA7:
        decodeRelativeAddressing(insn, out, mode);
        break;
    case M_DA:
        decodeDirectAddress(insn, out);
        break;
    case M_GENA:
    case M_GEND:
    case M_GENI:
        decodeGenericAddressing(insn, out, mode, num);
        break;
    case M_BIT:
        out.letter('#');
        if (insn.size() == SZ_BYTE && num >= 8)
            insn.setErrorIf(out, ILLEGAL_BIT_NUMBER);
        outHex(out, num, 8);
        break;
    case M_CNT:
        out.letter('#');
        if (insn.ex1() == M_CNT) {  // LDM
            const auto reg = modePos(insn, OP_P8);
            const auto cnt = modePos(insn, OP_P0);
            if (reg + cnt >= 16)
                insn.setErrorIf(out, OVERFLOW_RANGE);
        }
        outHex(out, num + 1, 5);
        break;
    case M_QCNT:
        outHex(out.letter('#'), (num & 2) ? 2 : 1, 2);
        break;
    case M_IM8:
    case M_IM:
    case M_SCNT:
    case M_NCNT:
        decodeImmediate(insn, out, mode, insn.size());
        break;
    case M_X:
        decodeGenericAddressing(insn, out, mode, num);
        break;
    default:
        break;
    }
}

namespace {

OprSize registerSize(const DisInsn &insn, AddrMode mode) {
    if (mode == M_IR)
        return SZ_ADDR;
    if (mode == M_GEND && (insn.opCode() >> 14) == 2)  // M_R
        return insn.size();
    if (mode == M_R)
        return insn.size();
    return SZ_WORD;
}

}  // namespace

Error DisZ8000::checkRegisterOverlap(DisInsn &insn, StrBuffer &out) const {
    const auto pass = insn.isCheckRegisterOverlap();
    insn.setCheckRegisterOverlap(0);
    const auto dmode = insn.dst();
    const auto smode = insn.src();
    const auto dnum = modePos(insn, insn.dstPos());
    const auto snum = modePos(insn, insn.srcPos());
    const auto dsize = registerSize(insn, dmode);
    const auto ssize = registerSize(insn, smode);
    RegName dst, src;
    decodeRegNum(segmentedModel(), dnum, dsize, dst);
    decodeRegNum(segmentedModel(), snum, ssize, src);
    if (checkOverlap(dst, src))
        insn.setErrorIf(out, REGISTERS_OVERLAPPED);

    if (pass == 2 && insn.isThreeRegsInsn()) {
        const auto cnum = modePos(insn, OP_P8);
        RegName cnt;
        decodeRegNum(segmentedModel(), cnum, SZ_WORD, cnt);
        if (dst != REG_ILLEGAL && src != REG_ILLEGAL && checkOverlap(dst, src, cnt))
            insn.setErrorIf(out, REGISTERS_OVERLAPPED);
    }
    return insn.getError();
}

StrBuffer &DisZ8000::outComma(
        StrBuffer &out, const DisInsn &insn, AddrMode mode, OprPos pos) const {
    if (mode == M_CC && decodeCcNum(modePos(insn, pos)) == CC_T)
        return out;
    return out.comma();
}

Error DisZ8000::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    insn.setOpCode(insn.readUint16());
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    const auto dst = insn.dst();
    decodeOperand(insn, out, dst, insn.dstPos());
    const auto src = insn.src();
    if (src != M_NONE) {
        outComma(out, insn, dst, insn.dstPos());
        if (insn.isPushPopInsn() || insn.isThreeRegsInsn())
            insn.setCheckRegisterOverlap(1);
        decodeOperand(insn, out, src, insn.srcPos());
        const auto ex1 = insn.ex1();
        if (ex1 != M_NONE) {
            const auto ex1Pos = (ex1 == M_CNT ? OP_P0 : OP_P8);
            outComma(out, insn, ex1, ex1Pos);
            if (insn.isThreeRegsInsn())
                insn.setCheckRegisterOverlap(2);
            decodeOperand(insn, out, ex1, ex1Pos);
            const auto ex2 = insn.ex2();
            if (ex2 != M_NONE) {
                outComma(out, insn, ex2, OP_P0);
                decodeOperand(insn, out, ex2, OP_P0);
            }
        }
    }
    return _insn.setError(insn);
}

}  // namespace z8000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
