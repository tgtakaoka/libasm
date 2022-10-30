/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __ASM_MC6805_H__
#define __ASM_MC6805_H__

#include "asm_base.h"
#include "config_mc6805.h"
#include "insn_mc6805.h"
#include "reg_mc6805.h"
#include "table_mc6805.h"

namespace libasm {
namespace mc6805 {

class AsmMc6805 : public Assembler, public Config {
public:
    AsmMc6805() : Assembler(_parser, TableMc6805::TABLE), _parser() { reset(); }

    const ConfigBase &config() const override { return *this; }
    AddressWidth addressWidth() const override;
    void reset() override { _pc_bits = 0; }

    const Options &options() const override { return _options; }

private:
    MotorolaValueParser _parser;
    uint8_t _pc_bits;
    const struct OptPcBits : public IntOptionBase {
        OptPcBits(uint8_t &value)
            : IntOptionBase(OPT_INT_PCBITS, OPT_DESC_PCBITS), _pc_bits(value) {}
        Error check(int32_t value) const override {
            return value >= 0 && value <= 16 ? OK : OVERFLOW_RANGE;
        }
        void set(int32_t value) const override { _pc_bits = value; }
        uint8_t &_pc_bits;
    } _opt_pc_bits{_pc_bits};
    const Options _options{_opt_pc_bits};

    struct Operand : public OperandBase {
        AddrMode mode;
        int8_t size;
        uint16_t val16;
        Operand() : mode(M_NONE), size(0), val16(0) {}
    };

    Error parseOperand(StrScanner &scan, Operand &op) const;
    Error emitRelative(InsnMc6805 &insn, const Operand &op);
    Error emitBitNumber(InsnMc6805 &insn, const Operand &op);
    Error emitOperand(InsnMc6805 &insn, AddrMode mode, const Operand &op);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;

    static const char OPT_INT_PCBITS[] PROGMEM;
    static const char OPT_DESC_PCBITS[] PROGMEM;
};

}  // namespace mc6805
}  // namespace libasm

#endif  // __ASM_MC6805_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
