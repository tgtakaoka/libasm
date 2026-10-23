/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "dis_c166.h"
#include "reg_c166.h"
#include "table_c166.h"

namespace libasm {
namespace c166 {

using namespace reg;

namespace {

constexpr char OPT_BOOL_CC_PREFIX[] PROGMEM = "cc-prefix";
constexpr char OPT_DESC_CC_PREFIX[] PROGMEM = "use cc_ prefix for condition code";
constexpr char OPT_BOOL_INDEX_SHARP[] PROGMEM = "index-sharp";
constexpr char OPT_DESC_INDEX_SHARP[] PROGMEM = "use # for [Rw+#data16]";
constexpr char OPT_BOOL_CANONICAL_MEM[] PROGMEM = "canonical-mem";
constexpr char OPT_BOOL_SFR_NAME[] PROGMEM = "sfr-name";
constexpr char OPT_DESC_SFR_NAME[] PROGMEM = "use name for core SFR";

// Flag ILLEGAL_OPERAND_MODE if any bit set in |zeroMask| is non-zero.
void requireZero(DisInsn &insn, StrBuffer &out, uint8_t byte, uint8_t zeroMask) {
    if (byte & zeroMask)
        insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
}

// Whether |addr| is one a reg operand can address.
bool regAddress(uint16_t addr) {
    return addr >= 0xFE00 && addr <= 0xFE00 + 2 * 0xEF && (addr & 1) == 0;
}

StrBuffer &outIndirect(StrBuffer &out, uint8_t num, char pre, char post) {
    out.letter('[');
    if (pre)
        out.letter(pre);
    outRegName(out, decodeWordReg(num));
    if (post)
        out.letter(post);
    return out.letter(']');
}

}  // namespace

const ValueFormatter::Plugins &DisC166::defaultPlugins() {
    return ValueFormatter::Plugins::intel();
}

DisC166::DisC166(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_ccPrefix),
      Config(TABLE),
      _opt_ccPrefix(this, &DisC166::setCcPrefix, OPT_BOOL_CC_PREFIX, OPT_DESC_CC_PREFIX,
              &_opt_indexSharp),
      _opt_indexSharp(this, &DisC166::setIndexSharp, OPT_BOOL_INDEX_SHARP, OPT_DESC_INDEX_SHARP,
              &_opt_canonicalMem),
      _opt_canonicalMem(
              this, &DisC166::setCanonicalMem, OPT_BOOL_CANONICAL_MEM, nullptr, &_opt_sfrName),
      _opt_sfrName(this, &DisC166::setSfrName, OPT_BOOL_SFR_NAME, OPT_DESC_SFR_NAME) {
    reset();
}

void DisC166::reset() {
    Disassembler::reset();
    setCcPrefix(true);
    setIndexSharp(true);
    setCanonicalMem(false);
    setSfrName(false);
}

Error DisC166::setCcPrefix(bool enable) {
    _ccPrefix = enable;
    return OK;
}

Error DisC166::setIndexSharp(bool enable) {
    _indexSharp = enable;
    return OK;
}

Error DisC166::setCanonicalMem(bool enable) {
    _canonicalMem = enable;
    return OK;
}

Error DisC166::setSfrName(bool enable) {
    _sfrName = enable;
    return OK;
}

// GNU as knows the core SFRs by name only.
Error DisC166::setGnuAs(bool enable) {
    _sfrName = enable;
    return Disassembler::setGnuAs(enable);
}

// A memory operand, by name where it is a core SFR; GNU as reads a name
// there as a symbol in some instructions, so not for it.
void DisC166::decodeMem(StrBuffer &out, uint16_t addr) const {
    const auto name_P = (_sfrName && !_gnuAs) ? sfrName_P(addr, cpuType()) : nullptr;
    if (name_P) {
        out.text_P(name_P);
    } else {
        outAbsAddr(out, addr, 16);
    }
}

// An SFR by name, or by |addr| where GNU as has no name for it.
void DisC166::decodeSfr(DisInsn &insn, StrBuffer &out, uint16_t addr) const {
    const auto name_P = _sfrName ? sfrName_P(addr, cpuType()) : nullptr;
    if (name_P) {
        out.text_P(name_P);
        return;
    }
    if (_gnuAs)
        insn.setErrorIf(out, UNKNOWN_REGISTER);
    outAbsAddr(out, addr, 16);
}

void DisC166::decodeShortReg(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto num = insn.post();
    if (num >= 0xF0) {
        outRegName(out, mode == M_REGW ? decodeWordReg(num) : decodeByteReg(num));
    } else {
        decodeSfr(insn, out, 0xFE00 + 2 * num);
    }
}

void DisC166::decodeBitOff(DisInsn &insn, StrBuffer &out, uint8_t bitoff) const {
    if (bitoff >= 0xF0) {
        outRegName(out, decodeWordReg(bitoff));
    } else if (bitoff < 0x80) {
        // GNU as has no way to name a bit in the bit-addressable RAM.
        if (_gnuAs)
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
        outAbsAddr(out, 0xFD00 + 2 * bitoff, 16);
    } else {
        decodeSfr(insn, out, 0xFF00 + 2 * (bitoff - 0x80));
    }
}

void DisC166::decodeRelative(DisInsn &insn, StrBuffer &out, uint8_t rel) const {
    // The instruction pointer wraps within the code segment.
    const auto delta = static_cast<int8_t>(rel) * 2;
    const auto ip = insn.address() + insn.length() + delta;
    const auto target = (insn.address() & ~UINT32_C(0xFFFF)) | (ip & 0xFFFF);
    insn.setErrorIf(out, checkAddr(target));
    outRelAddr(out, target, insn.address(), 9);
}

void DisC166::decodeCodeAddress(DisInsn &insn, StrBuffer &out) const {
    if (insn.dst() == M_SEG) {
        outAbsAddr(out, insn.word2(), 16);
        return;
    }
    // A target in the code segment of the instruction.
    const auto target = (insn.address() & ~UINT32_C(0xFFFF)) | insn.word2();
    // GNU as takes the offset in the segment, which a target against the
    // location counter gives as well.
    if (_gnuAs) {
        if (_relativeTarget) {
            outRelAddr(out, target, insn.address(), 16);
        } else {
            outAbsAddr(out, insn.word2(), 16);
        }
        return;
    }
    outAbsAddr(out, target);
}

void DisC166::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto post = insn.post();
    switch (mode) {
    case M_RWH:
        outRegName(out, decodeWordReg(post >> 4));
        break;
    case M_RWL:
        outRegName(out, decodeWordReg(post));
        break;
    case M_RBH:
        outRegName(out, decodeByteReg(post >> 4));
        break;
    case M_RBL:
        outRegName(out, decodeByteReg(post));
        break;
    case M_IRH:
        outIndirect(out, post >> 4, 0, 0);
        break;
    case M_IRL:
        outIndirect(out, post, 0, 0);
        break;
    case M_PIH:
        outIndirect(out, post >> 4, 0, '+');
        break;
    case M_PIL:
        outIndirect(out, post, 0, '+');
        break;
    case M_PDL:
        outIndirect(out, post, '-', 0);
        break;
    case M_IR2:
        outIndirect(out, post & 3, 0, 0);
        break;
    case M_PI2:
        outIndirect(out, post & 3, 0, '+');
        break;
    case M_D3:
        // GNU as knows #data3 as 1 to 7 only.
        if (_gnuAs && (post & 7) == 0)
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
        outHex(out.letter('#'), post & 7, 3);
        break;
    case M_D4:
        outHex(out.letter('#'), post >> 4, 4);
        break;
    case M_TRAP:
        outHex(out.letter('#'), post >> 1, 7);
        break;
    case M_REGW:
    case M_REGB:
        decodeShortReg(insn, out, mode);
        break;
    case M_CC:
        outCcName(out, decodeCcNum(post >> 4), _ccPrefix);
        break;
    case M_CCO:
        outCcName(out, decodeCcNum(insn.opCode() >> 4), _ccPrefix);
        break;
    case M_BITO:
        decodeBitOff(insn, out, post);
        outDec(out.letter('.'), insn.opCode() >> 4, 4);
        break;
    case M_BOFF:
        decodeBitOff(insn, out, post);
        break;
    case M_REL:
        decodeRelative(insn, out, post);
        break;
    case M_SEG:
        requireZero(insn, out, post, ~((1 << (addressWidth() - 16)) - 1));
        outHex(out, post, 8);
        break;
    case M_IXL:
        outRegName(out.letter('['), decodeWordReg(post)).letter('+');
        if (_indexSharp)
            out.letter('#');
        outHex(out, insn.word2(), 16).letter(']');
        break;
    case M_IM16:
        outHex(out.letter('#'), insn.word2(), 16);
        break;
    case M_IM8:
        requireZero(insn, out, insn.byte4(), 0xFF);
        outHex(out.letter('#'), insn.byte3(), 8);
        break;
    case M_MEM:
        decodeMem(out, insn.word2());
        break;
    case M_CADR:
        decodeCodeAddress(insn, out);
        break;
    case M_BITQ:
        decodeBitOff(insn, out, post);
        outDec(out.letter('.'), insn.byte4() >> 4, 4);
        break;
    case M_BITZ:
        decodeBitOff(insn, out, insn.byte3());
        outDec(out.letter('.'), insn.byte4() & 0xF, 4);
        break;
    case M_MDL:
        outHex(out.letter('#'), insn.byte3(), 8).comma();
        outHex(out.letter('#'), insn.byte4(), 8);
        break;
    case M_MDH:
        outHex(out.letter('#'), insn.byte4(), 8).comma();
        outHex(out.letter('#'), insn.byte3(), 8);
        break;
    case M_RELW:
        requireZero(insn, out, insn.byte4(), 0x0F);
        decodeRelative(insn, out, insn.byte3());
        break;
    default:
        break;
    }
}

Error DisC166::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    insn.setOpCode(insn.readByte());
    insn.readPost();
    if (insn.getError())
        return _insn.setError(insn);
    if (searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);
    if (insn.hasWord2()) {
        insn.readWord2();
        if (insn.getError())
            return _insn.setError(insn);
        if (insn.cf() == CF_PD && (insn.byte3() != insn.opCode() || insn.byte4() != insn.opCode())) {
            insn.nameBuffer().reset();
            insn.setErrorIf(UNKNOWN_INSTRUCTION);
            return _insn.setError(insn);
        }
    }

    const auto dst = insn.dst();
    const auto src = insn.src();
    // With an SFR in both operands, "mem, reg" reads back as "reg, mem".
    if (_canonicalMem && dst == M_MEM && (src == M_REGW || src == M_REGB) && insn.post() < 0xF0 &&
            regAddress(insn.word2())) {
        insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
        return _insn.setError(insn);
    }
    if (dst != M_NONE) {
        decodeOperand(insn, out, dst);
        if (src != M_NONE)
            decodeOperand(insn, out.comma(), src);
    }
    return _insn.setError(insn);
}

}  // namespace c166
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
