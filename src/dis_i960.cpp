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

#include "dis_i960.h"

#include "reg_i960.h"
#include "table_i960.h"
#include "text_i960.h"

namespace libasm {
namespace i960 {

using namespace reg;
using namespace text::i960;

const ValueFormatter::Plugins &DisI960::defaultPlugins() {
    return ValueFormatter::Plugins::cstyle();
}

DisI960::DisI960(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_modeBits),
      Config(TABLE),
      _opt_modeBits(this, &Config::setModeBits, OPT_BOOL_MODE_BITS, OPT_DESC_MODE_BITS,
              &_opt_ipOperand),
      _opt_ipOperand(this, &Config::setIpOperand, OPT_TEXT_IP_OPERAND, OPT_DESC_IP_OPERAND,
              &_opt_canonicalMem),
      _opt_canonicalMem(this, &DisI960::setCanonicalMem, OPT_BOOL_CANONICAL_MEM,
              OPT_DESC_CANONICAL_MEM, &_opt_fpu),
      _opt_fpu(this, &Config::setFpu, OPT_BOOL_FPU, OPT_DESC_FPU) {
    reset();
}

void DisI960::reset() {
    Disassembler::reset();
    // Otherwise the settings leak from one disassembly into the next.
    setModeBits(MODE_BITS_SET);
    setIpMode(IP_OFFSET);
    setCanonicalMem(false);
    // A disassembler is given whatever the target ran, so it decodes the
    // 80960KB extensions unless told otherwise.  The assembler defaults the
    // other way, where the user says which processor is being written for.
    setFpu(true);
}

Error DisI960::setCanonicalMem(bool enable) {
    _canonicalMem = enable;
    return OK;
}

namespace {

// A MEMB addressing mode which names an index register.
bool memHasIndex(MemMode mode) {
    return mode == MEM_ABASE_IX || mode == MEM_DISP_IX || mode == MEM_DISP_ABASE_IX;
}

// A MEMB addressing mode which names an address base register.
bool memHasAbase(MemMode mode) {
    return mode == MEM_ABASE || mode == MEM_ABASE_IX || mode == MEM_DISP_ABASE ||
           mode == MEM_DISP_ABASE_IX;
}

// A MEMB addressing mode carrying a second word.  Mode 5 is IP-relative and
// has one even though bit 13, which says so for the others, is clear.
bool memHasDisplacement(MemMode mode) {
    return mode == MEM_IP || mode >= MEM_DISP;
}

}  // namespace

Error DisI960::decodeMemory(DisInsn &insn, StrBuffer &out) const {
    const auto opc = insn.opCode();
    const auto abase = (opc >> 14) & 0x1F;
    if ((opc & MEMB_BIT) == 0) {
        // MEMA: a twelve-bit unsigned offset, and an address base register
        // when bit 13 is set.  Without one the field has to read zero, or two
        // encodings would disassemble alike and only one of them would
        // assemble back.
        if ((opc & MEMA_ABASE) == 0 && abase)
            return insn.setError(out, ILLEGAL_OPERAND_MODE);
        outHex(out, opc & 0xFFF, 12);
        if (opc & MEMA_ABASE)
            outRegName(out.letter('('), abase).letter(')');
        return OK;
    }

    const auto mode = insn.memMode();
    if (mode == MEM_RESERVED)
        return insn.setError(out, ILLEGAL_OPERAND_MODE);
    const auto scale = (opc >> 7) & 7;
    const auto index = opc & 0x1F;
    // Bits 6 and 5 are reserved.  A mode which names no index register leaves
    // the index and the scale unused, and both have to read zero for the same
    // reason the MEMA base does.
    if (opc & 0x60)
        return insn.setError(out, ILLEGAL_OPERAND_MODE);
    if (memHasIndex(mode)) {
        if (scale > MAX_SCALE)
            return insn.setError(out, ILLEGAL_OPERAND_MODE);
    } else if (scale || index) {
        return insn.setError(out, ILLEGAL_OPERAND_MODE);
    }
    if (!memHasAbase(mode) && abase)
        return insn.setError(out, ILLEGAL_OPERAND_MODE);

    if (mode == MEM_IP) {
        // The operand is the displacement, not the address it computes: Table
        // 5-1 gives the syntax as "exp (IP)" and 5-8 says "the displacement
        // plus a constant of 8 is added to the IP of the instruction".  GNU as
        // reads it that way too.  ASL instead takes the operand as the target
        // and subtracts IP + 8 from it, which is why the same source assembles
        // differently there; see ~/asl_bugs/asl_bug_i960.md.  Setting the
        // ip-operand option to target writes it the way ASL reads it.
        if (ipOffset()) {
            outHex(out, insn.displacement, -32);
        } else {
            const auto base = insn.address();
            outRelAddr(out, base + 8 + insn.displacement, base, 32);
        }
    } else if (memHasDisplacement(mode)) {
        // A displacement which fits the twelve-bit MEMA offset also has a
        // MEMA encoding, and that shorter one is what this operand assembles
        // back to, so the pair cannot both round trip.  The longer form is
        // ordinary code all the same -- GNU as and the Intel assembler reserve
        // the 32-bit displacement for any operand that names a symbol, however
        // low the symbol turns out to be -- so it is decoded by default and
        // refused only for a caller which needs the encoding to survive
        // reassembly, such as the test generator.
        if (_canonicalMem && !memHasIndex(mode) && insn.displacement <= 0xFFF)
            return insn.setError(out, ILLEGAL_OPERAND_MODE);
        outHex(out, insn.displacement, 32);
    }
    if (memHasAbase(mode))
        outRegName(out.letter('('), abase).letter(')');
    if (mode == MEM_IP)
        out.letter('(').text_P(TEXT_REG_IP).letter(')');
    if (memHasIndex(mode)) {
        outRegName(out.letter('['), index);
        // The multiplier is always written.  GNU as reads a missing one as the
        // operand size of the instruction rather than as one, so leaving it
        // out would turn ldl (g5)[g4] into a scale of eight; its own
        // disassembler omits it and does not survive its own round trip.
        outDec(out.letter('*'), 1 << scale, 8);
        out.letter(']');
    }
    return OK;
}

// A floating-point source: a global or local register when the mode bit is
// clear, otherwise one of the four floating-point registers or one of the two
// real literals.  Table B-1 leaves every other value of the field reserved.
Error DisI960::decodeFloat(DisInsn &insn, StrBuffer &out, uint8_t field, bool literal) const {
    if (!literal) {
        outRegName(out, field);
        return OK;
    }
    if (field < FLOAT_REGS) {
        outFloatRegName(out, field);
    } else if (field == FLOAT_ZERO) {
        out.rtext_P(PSTR("0.0"));
    } else if (field == FLOAT_ONE) {
        out.rtext_P(PSTR("1.0"));
    } else {
        return insn.setError(out, ILLEGAL_REGISTER);
    }
    return OK;
}

Error DisI960::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto opc = insn.opCode();
    switch (mode) {
    case M_NONE:
        return OK;
    case M_SRC1:
    case M_SRC1L:
    case M_SRC1Q:
        // A set mode bit makes the field a five-bit literal.
        if (opc & BIT_M1) {
            outDec(out, opc & 0x1F, 5);
        } else {
            outRegName(out, opc & 0x1F);
        }
        return OK;
    case M_REG1:
        outRegName(out, opc & 0x1F);
        return OK;
    case M_SRC2:
    case M_SRC2L:
        if (opc & BIT_M2) {
            outDec(out, (opc >> 14) & 0x1F, 5);
        } else {
            outRegName(out, (opc >> 14) & 0x1F);
        }
        return OK;
    case M_REG2:
    case M_CSRC2:
        outRegName(out, (opc >> 14) & 0x1F);
        return OK;
    case M_DST:
    case M_DSTL:
    case M_DSTQ:
    case M_MDST:
    case M_MDSTL:
    case M_MDSTQ:
    case M_CREG1:
        outRegName(out, (opc >> 19) & 0x1F);
        return OK;
    case M_CSRC1:
        // COBR keeps src1 where REG keeps src/dst, and its mode bit is m1.
        if (opc & BIT_COBR_M1) {
            outDec(out, (opc >> 19) & 0x1F, 5);
        } else {
            outRegName(out, (opc >> 19) & 0x1F);
        }
        return OK;
    case M_CDISP: {
        const auto base = insn.address();
        auto delta = static_cast<int32_t>(opc & 0x1FFC);
        if (delta & 0x1000)
            delta -= 0x2000;
        outRelAddr(out, base + delta, base, 13);
        return OK;
    }
    case M_TDISP: {
        const auto base = insn.address();
        auto delta = static_cast<int32_t>(opc & 0x00FFFFFC);
        if (delta & 0x00800000)
            delta -= 0x01000000;
        outRelAddr(out, base + delta, base, 24);
        return OK;
    }
    case M_FREG:
    case M_REG:
    case M_VALUE:
        // Reported by the assembler's parser only; no table entry names them.
        return OK;
    case M_MEM:
        return decodeMemory(insn, out);
    case M_FSRC1:
    case M_FSRC1L:
    case M_FSRC1Q:
        return decodeFloat(insn, out, static_cast<uint8_t>(opc & 0x1F), (opc & BIT_M1) != 0);
    case M_FSRC2:
    case M_FSRC2L:
    case M_FSRC2Q:
        return decodeFloat(insn, out, static_cast<uint8_t>((opc >> 14) & 0x1F),
                (opc & BIT_M2) != 0);
    case M_FDST:
    case M_FDSTL:
    case M_FDSTQ:
        // A destination takes no literal, so a set mode bit is a register.
        if (opc & BIT_M3) {
            const auto regno = static_cast<uint8_t>((opc >> 19) & 0x1F);
            if (regno >= FLOAT_REGS)
                return insn.setError(out, ILLEGAL_REGISTER);
            outFloatRegName(out, regno);
        } else {
            outRegName(out, (opc >> 19) & 0x1F);
        }
        return OK;
    }
    return OK;
}

Error DisI960::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    insn.setOpCode(insn.readUint32());
    // An instruction is at least one 32-bit word, so anything shorter is a
    // truncated instruction rather than an illegal one.  Report it before the
    // table search, which starts by clearing the error the short read raised.
    if (insn.getError() == NO_MEMORY)
        return _insn.setError(insn);
    if (searchOpCode(cpuType(), modeBits(), hasFpu(), insn, out))
        return _insn.setError(insn);

    // The MEMB displacement is a second word.  Read it before any operand is
    // written so that the instruction knows its own length.
    insn.displacement = 0;
    if (insn.hasMem() && memHasDisplacement(insn.memMode())) {
        insn.displacement = insn.readUint32();
        if (insn.getError() == NO_MEMORY)
            return _insn.setError(insn);
    }

    for (uint_fast8_t pos = 0; pos < MAX_OPERANDS; pos++) {
        const auto mode = insn.mode(pos);
        if (mode == M_NONE)
            break;
        if (pos)
            out.comma();
        if (decodeOperand(insn, out, mode))
            return _insn.setError(insn);
    }
    return _insn.setError(insn);
}

}  // namespace i960
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
