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

#include "dis_am29000.h"

#include "reg_am29000.h"
#include "table_am29000.h"
#include "text_am29000.h"

namespace libasm {
namespace am29000 {

using namespace reg;
using namespace text::am29000;

namespace {

constexpr char OPT_BOOL_EXTERN_SYMBOL[] PROGMEM = "extern-symbol";
constexpr char OPT_DESC_EXTERN_SYMBOL[] PROGMEM = "emit absolute branch target as A_XXXXXXXX symbol";

}  // namespace

const ValueFormatter::Plugins &DisAm29000::defaultPlugins() {
    return ValueFormatter::Plugins::cstyle();
}

DisAm29000::DisAm29000(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_fpu),
      Config(TABLE),
      _opt_fpu(this, &Config::setFpu, OPT_BOOL_FPU, OPT_DESC_FPU, &_opt_intMul),
      _opt_intMul(this, &Config::setIntMul, OPT_BOOL_INTMULT, OPT_DESC_INTMULT,
              &_opt_externSymbol),
      _opt_externSymbol(this, &DisAm29000::setExternSymbol, OPT_BOOL_EXTERN_SYMBOL,
              OPT_DESC_EXTERN_SYMBOL) {
    reset();
}

void DisAm29000::reset() {
    Disassembler::reset();
    // Return the arithmetic options to what the selected CPU implements.
    setCpuType(cpuType());
    setExternSymbol(false);
}

Error DisAm29000::setExternSymbol(bool enable) {
    _externSymbol = enable;
    return OK;
}

namespace {

// A 16-bit constant is split across the instruction: I15..I8 sits in the RC
// field and I7..I0 in the RB field.
uint16_t split16(Config::opcode_t opc) {
    return static_cast<uint16_t>(((opc >> 8) & 0xFF00) | (opc & 0xFF));
}

}  // namespace

void DisAm29000::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto opc = insn.opCode();
    switch (mode) {
    case M_NONE:
        return;
    case M_RC:
        outGenReg(out, (opc >> 16) & 0xFF);
        return;
    case M_RA:
        outGenReg(out, (opc >> 8) & 0xFF);
        return;
    case M_RB:
        outGenReg(out, opc & 0xFF);
        return;
    case M_RBI:
        // The M bit tells a register from an 8-bit constant.
        if (opc & BIT_M) {
            outHex(out, opc & 0xFF, 8);
        } else {
            outGenReg(out, opc & 0xFF);
        }
        return;
    case M_IM16:
        outHex(out, split16(opc), 16);
        return;
    case M_IMH:
        // The manual makes this operand the 16-bit field itself, which is what
        // ASL takes too.  GNU as instead takes a 32-bit value and keeps its
        // high half, so that a CONST/CONSTH pair can name one symbol; write it
        // that way when the output is meant for GNU as.
        if (_externSymbol) {
            outHex(out, static_cast<uint32_t>(split16(opc)) << 16, 32);
        } else {
            outHex(out, split16(opc), 16);
        }
        return;
    case M_TGT: {
        // The target is a word address.  The A bit selects an absolute target
        // from one relative to the Program Counter.
        const auto delta = static_cast<int16_t>(split16(opc)) * 4;
        const auto base = insn.address();
        if (opc & BIT_A) {
            const auto target = static_cast<uint32_t>(split16(opc)) * 4;
            // GNU as reads the 16-bit word address of this form as signed and
            // rejects a target over 0x1FFFC, where the manual zero-extends it.
            // Naming the target leaves it to the linker, which resolves the
            // whole range.  The name has to read the same whichever case the
            // rest of the listing is in, so spell it in lower case throughout.
            if (_externSymbol) {
                out.rtext_P(PSTR("A_"));
                StrCaseBuffer lower(out, false);
                lower.hex(target, 8);
                lower.over(out);
            } else {
                outAbsAddr(out, target);
            }
        } else {
            outRelAddr(out, base + delta, base, 18);
        }
        return;
    }
    case M_VN:
        outHex(out, (opc >> 16) & 0xFF, 8);
        return;
    case M_SA: {
        const auto number = (opc >> 8) & 0xFF;
        const auto name = decodeSpReg(number, cpuType());
        // A register number this CPU does not implement reads back an
        // unpredictable value, and the manual says to avoid it for the same
        // upward-compatibility reason it gives for a reserved field, so it is
        // rejected the same way.
        if (name == REG_UNDEF) {
            insn.setErrorIf(out, UNKNOWN_INSTRUCTION);
            return;
        }
        outSpReg(out, name);
        return;
    }
    case M_CE:
        outHex(out, (opc >> 23) & 1, 1);
        return;
    case M_CNTL:
        outHex(out, (opc >> 16) & 0x7F, 7);
        return;
    case M_ID:
        outHex(out, (opc >> 16) & 3, 2);
        return;
    case M_UI:
        outHex(out, (opc >> 7) & 1, 1);
        return;
    case M_RND:
        outHex(out, (opc >> 4) & 7, 3);
        return;
    case M_FD:
    case M_TFMT:
        outHex(out, (opc >> 2) & 3, 2);
        return;
    case M_FS:
    case M_FMT:
    case M_ACN:
        outHex(out, opc & 3, 2);
        return;
    case M_ACNH:
        outHex(out, (opc >> 16) & 3, 2);
        return;
    case M_FUNC:
        outHex(out, (opc >> 18) & 0xF, 4);
        return;
    }
}

Error DisAm29000::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    insn.setOpCode(insn.readUint32());
    // An instruction is one 32-bit word, so anything shorter is a truncated
    // instruction rather than an illegal one.  Report it before the table
    // search, which starts by clearing the error the short read raised.
    if (insn.getError() == NO_MEMORY)
        return _insn.setError(insn);
    if (searchOpCode(cpuType(), hasFpu(), hasIntMul(), insn, out))
        return _insn.setError(insn);

    for (uint_fast8_t pos = 0; pos < MAX_OPERANDS; pos++) {
        const auto mode = insn.mode(pos);
        if (mode == M_NONE)
            break;
        if (pos)
            out.comma();
        decodeOperand(insn, out, mode);
    }
    return _insn.setError(insn);
}

}  // namespace am29000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
