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
    _sdbdPrefix = false;
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

Error DisCp1600::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    // One-shot: consume the carry-over flag set by the previous decode.
    insn.sdbd_prefix = _sdbdPrefix;
    _sdbdPrefix = false;

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

    if (insn.sdbd_prefix && !insn.is_sdbdAware())
        insn.setErrorIf(PREFIX_HAS_NO_EFFECT);

    // Standalone SDBD: emit a one-line "sdbd" insn and carry the flag forward
    // so the next decode reads its M_IMM16 as a two-word immediate.
    if (insn.isSdbd() && !insn.sdbd_prefix) {
        _sdbdPrefix = true;
        return _insn.setError(insn);
    }

    decodeOperand(insn, out, insn.src());
    const auto dst = insn.dst();
    if (dst == M_SHCNT) {
        decodeOperand(insn, out, insn.dst());
    } else if (dst != M_NONE) {
        decodeOperand(insn, out.comma(), insn.dst());
    }

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
