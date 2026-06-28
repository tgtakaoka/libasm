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

#include "dis_cp1600.h"

#include "array_memory.h"
#include "formatters.h"
#include "reg_cp1600.h"
#include "table_cp1600.h"
#include "text_common.h"
#include "text_cp1600.h"

namespace libasm {
namespace cp1600 {

using namespace reg;
using namespace text::common;
using namespace text::cp1600;

const ValueFormatter::Plugins &DisCp1600::defaultPlugins() {
    static const struct final : ValueFormatter::Plugins {
        const HexFormatter &hex() const override { return _hex; }
        char locationSymbol() const override { return '!'; }
        const /*PROGMEM*/ char *lineComment_P() const override { return PSTR_SEMI; }
        const SurroundHexFormatter _hex{PSTR_X_DASH, '\''};
    } PLUGINS{};
    return PLUGINS;
}

namespace {
constexpr char OPT_BOOL_SP_ALIAS[] PROGMEM = "sp-alias";
constexpr char OPT_DESC_SP_ALIAS[] PROGMEM = "decode R6 as SP";
constexpr char OPT_BOOL_PC_ALIAS[] PROGMEM = "pc-alias";
constexpr char OPT_DESC_PC_ALIAS[] PROGMEM = "decode R7 as PC";
}  // namespace

DisCp1600::DisCp1600(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_spAlias),
      Config(TABLE),
      _opt_spAlias(
              this, &DisCp1600::setSpAlias, OPT_BOOL_SP_ALIAS, OPT_DESC_SP_ALIAS, &_opt_pcAlias),
      _opt_pcAlias(this, &DisCp1600::setPcAlias, OPT_BOOL_PC_ALIAS, OPT_DESC_PC_ALIAS) {
    reset();
}

void DisCp1600::reset() {
    Disassembler::reset();
    setSpAlias(false);
    setPcAlias(false);
}

Error DisCp1600::setSpAlias(bool enable) {
    _spAlias = enable;
    return OK;
}

Error DisCp1600::setPcAlias(bool enable) {
    _pcAlias = enable;
    return OK;
}

StrBuffer &DisCp1600::outReg(StrBuffer &out, uint_fast8_t num) const {
    num &= 7;
    if (num == 6 && _spAlias)
        return out.text_P(TEXT_REG_SP);
    if (num == 7 && _pcAlias)
        return out.text_P(TEXT_REG_PC);
    return outRegName(out, num);
}

void DisCp1600::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto opc = insn.opCode();
    switch (mode) {
    case M_NOPP:
        insn.readUint16();
        // Fall-through
    case M_BIT0:
    case M_NONE:
        break;
    case M_DREG:
    case M_XREG:
        outReg(out, opc & 7);
        break;
    case M_SREG:
        outReg(out, (opc >> 3) & 7);
        break;
    case M_LREG:
        outReg(out, opc & 3);
        break;
    case M_SHCNT:
        if ((opc >> 2) & 1)
            out.comma().letter('2');
        break;
    case M_DADDR:
        outAbsAddr(out, insn.readUint16());
        break;
    case M_IMM16:
        if (insn.sdbd_prefix) {
            // SDBD-prefixed: read two words; CPU takes only the low 8 bits of each.
            const auto w1 = insn.readUint16();
            const auto w2 = insn.readUint16();
            if ((w1 & ~UINT8_MAX) | (w2 & ~UINT8_MAX))
                insn.setErrorIf(out, ILLEGAL_OPERAND);
            uint16_t val = (w2 & UINT8_MAX) << 8;
            val |= (w1 & UINT8_MAX);
            outHex(out, val, 16, false);
        } else {
            const auto val = insn.readUint16();
            if (val & ~UINT8_MAX)
                insn.setErrorIf(out, ILLEGAL_OPERAND);
            outHex(out, val, 8, false);
        }
        break;
    case M_INDIR:
        outReg(out, (opc >> 3) & 7);
        break;
    case M_STACK:
        outReg(out, 6);
        break;
    case M_BCOND: {
        // BEXT condition (0..17 octal) lives in bits 3:0 of the raw word1.  The
        // entry's opcode after searchOpCode no longer carries those bits, so
        // we read them from the originally-fetched low byte (bytes[1] in big
        // endian).  Emit as octal so it round-trips with the assembler's
        // RADIX_8 input radix.
        const auto v = insn.bytes()[1] & 017;
        if (v >= 010)
            out.letter('1');
        out.letter('0' + (v & 7));
        break;
    }
    case M_BDISP: {
        const auto disp = insn.readUint16();
        const auto base = insn.address() + static_cast<Config::uintptr_t>(insn.length()) / 2;
        Config::uintptr_t target;
        if (opc & 00040) {
            // backward: target = base - disp - 1
            target = base - disp - 1;
        } else {
            // forward: target = base + disp
            target = base + disp;
        }
        outRelAddr(out, target, insn.address(), -16);
        break;
    }
    case M_BREG: {
        const auto bb = insn.bytes()[2] & 3;
        outReg(out, bb + 4);
        break;
    }
    case M_JADDR: {
        // word2 = bytes[2..3] (already consumed); word3 is freshly read.
        const auto w2_hi = insn.bytes()[2];
        const auto w2_lo = insn.bytes()[3];
        const auto word2 = static_cast<uint16_t>((w2_hi << 8) | w2_lo);
        const auto word3 = insn.readUint16();
        const Config::uintptr_t addr =
                (static_cast<Config::uintptr_t>(word2 & 00374) << 8) | (word3 & 01777);
        outAbsAddr(out, addr);
        break;
    }
    default:
        break;
    }
}

// Decode the operands of the instruction at |insn| (opcode already searched).
void DisCp1600::decodeBody(DisInsn &insn, StrBuffer &out) const {
    decodeOperand(insn, out, insn.src());
    const auto dst = insn.dst();
    if (dst == M_SHCNT) {
        decodeOperand(insn, out, dst);
    } else if (dst != M_NONE) {
        decodeOperand(insn, out.comma(), dst);
    }
}

Error DisCp1600::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);

    // Continuation: the preceding SDBD decode already read the whole
    // SDBD-prefixed instruction; re-decode it here from those bytes (skipping
    // the leading SDBD word), so we touch neither the caller's reader nor this
    // insn's length.
    if (_insn.hasContinue()) {
        _insn.setContinueMark_P(nullptr);
        ArrayMemory mem(_insn.address(), _insn.bytes(), _insn.length());
        auto reader = mem.iterator();
        Insn work(_insn.address());
        DisInsn cont(work, reader, out);
        cont.readUint16();  // skip the SDBD word
        cont.setOpCode(cont.readUint16());
        if (!searchOpCode(cpuType(), cont, out)) {
            cont.sdbd_prefix = cont.is_sdbdAware();  // double-byte only when aware
            decodeBody(cont, out);
        }
        _insn.nameBuffer().reset().text(work.name());
        return _insn.setError(cont);
    }

    const auto raw1 = insn.readUint16();
    insn.setOpCode(raw1);
    if (raw1 & 0xFC00)
        return _insn.setError(insn.setError(INVALID_INSTRUCTION));
    if (isPrefix(cpuType(), raw1)) {
        insn.setPrefix(raw1);
        const auto raw2 = insn.readUint16();
        insn.setOpCode(raw2);
        if (raw2 & 0xFC00)
            return _insn.setError(insn.setError(INVALID_INSTRUCTION));
        if (insn.getError())
            return _insn.setError(insn);
    }
    if (searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    // SDBD is a prefix.  Read and decode the following instruction completely so
    // its bytes belong to this insn, emit "SDBD", and mark a continuation that
    // re-decodes that instruction (above).  A follower that the SDBD prefix has
    // no effect on flags PREFIX_HAS_NO_EFFECT and is dropped by the test
    // generator, so the follower opcode is still reached standalone.
    if (insn.isSdbd()) {
        const auto raw2 = insn.readUint16();
        if (insn.getError())  // NO_MEMORY: the caller must supply the follower
            return _insn.setError(insn);
        insn.setOpCode(raw2);
        if (raw2 & 0xFC00)
            return _insn.setError(insn.setError(INVALID_INSTRUCTION));
        if (searchOpCode(cpuType(), insn, out))
            return _insn.setError(insn);
        insn.sdbd_prefix = insn.is_sdbdAware();  // double-byte only when aware
        char buf[40];
        StrBuffer discard{buf, sizeof(buf)};
        decodeBody(insn, discard);  // consume the follower's operand bytes
        if (insn.getError() == NO_MEMORY)  // need the follower's operand words
            return _insn.setError(insn);
        insn.resetError();  // operand-validation errors belong to the continuation
        if (!insn.sdbd_prefix)  // follower the SDBD prefix has no effect on
            insn.setError(PREFIX_HAS_NO_EFFECT);
        auto save{out};
        insn.nameBuffer().reset().over(out).text_P(TEXT_SDBD).over(insn.nameBuffer());
        save.over(out);
        _insn.setContinueMark_P(TEXT_null);
        // The follower's bytes start right after the 1-word SDBD prefix, so the
        // listing shows SDBD as one word and the follower at the next address.
        _insn.setContinueOffset(sizeof(Config::opcode_t));
        return _insn.setError(insn);
    }

    decodeBody(insn, out);
    return _insn.setError(insn);
}

}  // namespace cp1600
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
