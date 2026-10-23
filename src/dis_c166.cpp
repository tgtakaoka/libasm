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
#include "array_memory.h"
#include "reg_c166.h"
#include "table_c166.h"
#include "text_common.h"

namespace libasm {
namespace c166 {

using namespace reg;

namespace {

constexpr char OPT_BOOL_CC_PREFIX[] PROGMEM = "cc-prefix";
constexpr char OPT_DESC_CC_PREFIX[] PROGMEM = "use cc_ prefix for condition code";
constexpr char OPT_BOOL_INDEX_SHARP[] PROGMEM = "index-sharp";
constexpr char OPT_DESC_INDEX_SHARP[] PROGMEM = "use # for [Rw+#data16]";
constexpr char OPT_BOOL_CANONICAL_MEM[] PROGMEM = "canonical-mem";
constexpr char OPT_BOOL_EXT_PREFIX[] PROGMEM = "ext-prefix";
constexpr char OPT_BOOL_SFR_NAME[] PROGMEM = "sfr-name";
constexpr char OPT_DESC_SFR_NAME[] PROGMEM = "use name for core SFR";

// Flag ILLEGAL_OPERAND_MODE if any bit set in |zeroMask| is non-zero.
void requireZero(DisInsn &insn, StrBuffer &out, uint8_t byte, uint8_t zeroMask) {
    if (byte & zeroMask)
        insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
}

// Only the ESFR side of a sequence matters here; mem prints as it is encoded.
void beginSequence(ExtState &state, uint8_t post, AddrMode mode) {
    state.count = ((post >> 4) & 3) + 1;
    state.esfr = mode == M_RNGR;
}

// EXTR, EXTPR and EXTSR move reg and bitoff to the ESFR area.
bool beginsEsfrSequence(const DisInsn &insn) {
    return insn.dst() == M_RNGR || insn.src() == M_RNGR;
}

bool remapsBitOff(uint8_t bitoff) {
    return bitoff >= 0x80 && bitoff < 0xF0;
}

// Whether the ESFR area moves the operand of |mode|, as far as the bytes read
// so far tell; a BITZ bitoff is in the 3rd byte.
bool remaps(const DisInsn &insn, AddrMode mode, bool byte3) {
    switch (mode) {
    case M_REGW:
    case M_REGB:
        return insn.post() < 0xF0;
    case M_BOFF:
    case M_BITO:
    case M_BITQ:
        return remapsBitOff(insn.post());
    case M_BITZ:
        return byte3 && remapsBitOff(insn.byte3());
    default:
        return false;
    }
}

bool remaps(const DisInsn &insn, bool byte3) {
    return remaps(insn, insn.dst(), byte3) || remaps(insn, insn.src(), byte3);
}

// Whether |addr| is one a reg operand can address.
bool regAddress(const DisInsn &insn, uint16_t addr) {
    const uint16_t base = insn.esfr() ? 0xF000 : 0xFE00;
    return addr >= base && addr <= base + 2 * 0xEF && (addr & 1) == 0;
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
              this, &DisC166::setCanonicalMem, OPT_BOOL_CANONICAL_MEM, nullptr, &_opt_extPrefix),
      _opt_extPrefix(
              this, &DisC166::setExtPrefix, OPT_BOOL_EXT_PREFIX, nullptr, &_opt_sfrName),
      _opt_sfrName(this, &DisC166::setSfrName, OPT_BOOL_SFR_NAME, OPT_DESC_SFR_NAME) {
    reset();
}

void DisC166::reset() {
    Disassembler::reset();
    setCcPrefix(true);
    setIndexSharp(true);
    setCanonicalMem(false);
    setExtPrefix(false);
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

Error DisC166::setExtPrefix(bool enable) {
    _extPrefix = enable;
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
    // GNU as cannot address the ESFR space at all.
    if (_gnuAs)
        insn.setErrorIf(out, insn.esfr() ? ILLEGAL_OPERAND_MODE : UNKNOWN_REGISTER);
    outAbsAddr(out, addr, 16);
}

void DisC166::decodeShortReg(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto num = insn.post();
    if (num >= 0xF0) {
        outRegName(out, mode == M_REGW ? decodeWordReg(num) : decodeByteReg(num));
    } else {
        const uint16_t base = insn.esfr() ? 0xF000 : 0xFE00;
        decodeSfr(insn, out, base + 2 * num);
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
        const uint16_t base = insn.esfr() ? 0xF100 : 0xFF00;
        decodeSfr(insn, out, base + 2 * (bitoff - 0x80));
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
    case M_RNG:
    case M_RNGR:
        outDec(out.letter('#'), ((post >> 4) & 3) + 1, 3);
        beginSequence(insn.insnBase().state<ExtState>(), post, mode);
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
    case M_PAG:
        requireZero(insn, out, insn.byte4(), 0xFC);
        outHex(out.letter('#'), insn.word2() & 0x3FF, 10);
        break;
    case M_SEG8:
        requireZero(insn, out, insn.byte4(), 0xFF);
        outHex(out.letter('#'), insn.byte3(), 8);
        break;
    default:
        break;
    }
}

Error DisC166::decodeInsn(DisInsn &insn, StrBuffer &out, bool follower) const {
    insn.setOpCode(insn.readByte());
    insn.readPost();
    if (insn.getError())
        return insn.getError();
    if (searchOpCode(cpuType(), insn, out))
        return insn.getError();
    // As a prefix, a sequence covers the one instruction read with it; its
    // length is in the post byte, so turn down any other before reading
    // further.
    if (_extPrefix && !follower && beginsEsfrSequence(insn) && (insn.post() & 0x30) != 0)
        return insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
    // Turn down a follower its prefix leaves alone before reading further.
    const auto bitz = insn.dst() == M_BITZ || insn.src() == M_BITZ;
    if (follower && !bitz && !remaps(insn, false))
        return insn.setErrorIf(PREFIX_HAS_NO_EFFECT);
    if (insn.hasWord2()) {
        insn.readByte3();
        if (insn.getError())
            return insn.getError();
        if (follower && !remaps(insn, true))
            return insn.setErrorIf(PREFIX_HAS_NO_EFFECT);
        insn.readByte4();
        if (insn.getError())
            return insn.getError();
        if (insn.cf() == CF_PD &&
                (insn.byte3() != insn.opCode() || insn.byte4() != insn.opCode())) {
            insn.nameBuffer().reset();
            insn.setErrorIf(UNKNOWN_INSTRUCTION);
            return insn.getError();
        }
    }

    const auto dst = insn.dst();
    const auto src = insn.src();
    // With an SFR in both operands, "mem, reg" reads back as "reg, mem".
    if (_canonicalMem && dst == M_MEM && (src == M_REGW || src == M_REGB) && insn.post() < 0xF0 &&
            regAddress(insn, insn.word2())) {
        insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
        return insn.getError();
    }
    if (dst != M_NONE) {
        decodeOperand(insn, out, dst);
        if (src != M_NONE)
            decodeOperand(insn, out.comma(), src);
    }
    return insn.getError();
}

// The instruction an EXTR, EXTPR or EXTSR prefix has already read, on its own
// line after the prefix.
Error DisC166::decodeFollower(Insn &_insn, StrBuffer &out) const {
    _insn.setContinueMark_P(nullptr);
    const auto offset = _insn.continueOffset();
    const ArrayMemory memory(
            _insn.address() + offset, _insn.bytes() + offset, _insn.length() - offset);
    auto reader = memory.iterator();
    Insn work(memory.origin());
    DisInsn follower(work, reader, out);
    auto &state = _insn.state<ExtState>();
    follower.setEsfr(state.esfr);
    state = ExtState{};
    decodeInsn(follower, out);
    _insn.nameBuffer().reset().text(work.name());
    return _insn.setError(follower);
}

Error DisC166::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    if (_insn.hasContinue())
        return decodeFollower(_insn, out);
    auto &state = _insn.state<ExtState>();
    // Each prefix decodes its own sequence.
    if (_extPrefix)
        state = ExtState{};
    DisInsn insn(_insn, memory, out);
    insn.setEsfr(state.count != 0 && state.esfr);
    if (state.count)
        state.count--;
    if (decodeInsn(insn, out) || !_extPrefix || !beginsEsfrSequence(insn))
        return _insn.setError(insn);

    const auto prefixLength = _insn.length();
    char buffer[40];
    StrBuffer discard{buffer, sizeof(buffer)};
    Insn work(_insn.address() + prefixLength);
    DisInsn follower(work, memory, discard);
    follower.setEsfr(state.esfr);
    const auto error = decodeInsn(follower, discard, true);
    for (uint8_t i = 0; i < work.length(); i++)
        _insn.emitByte(work.bytes()[i]);
    if (error == NO_MEMORY || error == UNKNOWN_INSTRUCTION || error == PREFIX_HAS_NO_EFFECT) {
        insn.setErrorIf(out, error);
        return _insn.setError(insn);
    }
    _insn.setContinueOffset(prefixLength);
    _insn.setContinueMark_P(text::common::TEXT_null);
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
