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

#ifndef __DIS_NS32000_H__
#define __DIS_NS32000_H__

#include "config_ns32000.h"
#include "dis_base.h"
#include "insn_ns32000.h"
#include "reg_ns32000.h"
#include "table_ns32000.h"

namespace libasm {
namespace ns32000 {

class DisNs32000 : public Disassembler, public Config {
public:
    DisNs32000()
        : Disassembler(_formatter, _regs, TableNs32000::TABLE, '*'), _formatter(), _regs() {
        reset();
    }

    const ConfigBase &config() const override { return *this; }
    void reset() override {
        _stringOptionBracket = _pcRelativeParen = _externalParen = _floatPrefix = false;
    }

    static const char OPT_BOOL_STROPT_BRACKET[] PROGMEM;
    static const char OPT_BOOL_PCREL_PAREN[] PROGMEM;
    static const char OPT_BOOL_EXTERNAL_PAREN[] PROGMEM;
    static const char OPT_BOOL_FLOAT_PREFIX[] PROGMEM;
    const Options &options() const override { return _options; }

private:
    NationalValueFormatter _formatter;
    RegNs32000 _regs;
    bool _stringOptionBracket;
    bool _pcRelativeParen;
    bool _externalParen;
    bool _floatPrefix;
    const BoolOption _opt_floatPrefix{OPT_BOOL_FLOAT_PREFIX, _floatPrefix};
    const BoolOption _opt_stroptBracket{OPT_BOOL_STROPT_BRACKET, _stringOptionBracket, _opt_floatPrefix};
    const BoolOption _opt_externalParen{OPT_BOOL_EXTERNAL_PAREN, _externalParen, _opt_stroptBracket};
    const BoolOption _opt_pcrelParel{OPT_BOOL_PCREL_PAREN, _pcRelativeParen, _opt_externalParen};
    const Options _options{_opt_pcrelParel};

    struct Displacement {
        int32_t val32;
        uint8_t bits;
    };

    StrBuffer &outDisplacement(StrBuffer &out, const Displacement &disp);

    Error readIndexByte(DisMemory &memory, InsnNs32000 &insn, AddrMode mode, OprPos pos);
    Error readDisplacement(DisMemory &memory, InsnNs32000 &insn, Displacement &disp);

    Error decodeLength(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode);
    Error decodeBitField(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode);
    Error decodeImmediate(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode);
    Error decodeDisplacement(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode);
    Error decodeRelative(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out);
    Error decodeConfig(const InsnNs32000 &insn, StrBuffer &out, OprPos pos);
    Error decodeStrOpt(const InsnNs32000 &insn, StrBuffer &out, OprPos pos);
    Error decodeRegisterList(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out);
    Error decodeGeneric(
            DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode, OprPos pos);
    Error decodeOperand(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode,
            OprPos pos, OprSize size);

    Error decode(DisMemory &memory, Insn &insn, StrBuffer &out) override;
};

}  // namespace ns32000
}  // namespace libasm

#endif  // __DIS_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
