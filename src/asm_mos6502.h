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

#ifndef __ASM_MOS6502_H__
#define __ASM_MOS6502_H__

#include "asm_base.h"
#include "config_mos6502.h"
#include "insn_mos6502.h"
#include "reg_mos6502.h"
#include "table_mos6502.h"

namespace libasm {
namespace mos6502 {

class AsmMos6502 : public Assembler, public Config {
public:
    AsmMos6502()
        : Assembler(_parser, TableMos6502::TABLE, _pseudos),
          _parser(_number, _comment, _symbol, _letter, _location),
          _pseudos() {
        reset();
    }

    const ConfigBase &config() const override { return *this; }
    AddressWidth addressWidth() const override { return TableMos6502::TABLE.addressWidth(); }
    void reset() override { TableMos6502::TABLE.reset(); }
    const Options &options() const override { return _options; }

private:
    ValueParser _parser;
    const MotorolaNumberParser _number;
    const SemicolonCommentParser _comment;
    const DefaultSymbolParser _symbol;
    const MotorolaLetterParser _letter;
    const AsteriskLocationParser _location;
    struct PseudoMos6502 : PseudoBase {
        Error processPseudo(StrScanner &scan, Insn &insn, Assembler &assembler) override;

    private:
        Error parseTableOnOff(
                StrScanner &scan, Assembler &assembler, bool (TableMos6502::*set)(bool val));
    } _pseudos;

    const struct OptLongI : public BoolOptionBase {
        OptLongI();
        Error set(bool value) const override;
    } _opt_longi{};
    const struct OptLongA : public BoolOptionBase {
        OptLongA(const OptionBase &next);
        Error set(bool value) const override;
    } _opt_longa{_opt_longi};
    const Options _options{_opt_longa};

    struct Operand : public OperandBase {
        AddrMode mode;
        uint32_t val32;
        Operand() : mode(M_NONE), val32(0) {}
        void embed(AddrMode indirectFlags) {
            mode = AddrMode(uint8_t(InsnMos6502::indirectFlags(indirectFlags)) |
                            uint8_t(InsnMos6502::baseMode(mode)));
        }
    };

    Error selectMode(char size, Operand &op, AddrMode zp, AddrMode abs, AddrMode labs) const;
    Error parseOpenIndirect(StrScanner &scan, Operand &op, char &indirect) const;
    Error parseCloseIndirect(StrScanner &scan, Operand &op, char &indirect) const;
    Error parseOperand(StrScanner &scan, Operand &op, char &indirect) const;

    void emitImmediate(InsnMos6502 &insn, const Operand &op, bool imm16);
    void encodeRelative(InsnMos6502 &insn, AddrMode mode, const Operand &op);
    void encodeOperand(InsnMos6502 &insn, AddrMode mode, const Operand &op);

    Error encodeImpl(StrScanner &scan, Insn &insn) override;
};

}  // namespace mos6502
}  // namespace libasm

#endif  // __ASM_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
