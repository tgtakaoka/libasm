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

#ifndef __ASM_CDP1802_H__
#define __ASM_CDP1802_H__

#include "asm_base.h"
#include "config_cdp1802.h"
#include "insn_cdp1802.h"
#include "table_cdp1802.h"

namespace libasm {
namespace cdp1802 {

class AsmCdp1802 : public Assembler, public Config {
public:
    AsmCdp1802() : Assembler(_parser, TableCdp1802::TABLE), _parser() { reset(); }

    const ConfigBase &config() const override { return *this; }
    void reset() override { _useReg = _smartBranch = false; }
    const Options &options() const override { return _options; }

private:
    IntelValueParser _parser;
    bool _useReg;
    bool _smartBranch;
    const BoolOption _opt_smartBranch{OPT_BOOL_SMART_BRANCH, OPT_DESC_SMART_BRANCH, _smartBranch};
    const BoolOption _opt_useReg{
            OPT_BOOL_USE_REGISTER, OPT_DESC_USE_REGISTER, _useReg, _opt_smartBranch};
    const Options _options{_opt_useReg};

    struct Operand : public OperandBase {
        AddrMode mode;
        uint16_t val16;
        Operand() : mode(M_NONE), val16(0) {}
    };

    Error parseOperand(StrScanner &scan, Operand &op) const;

    Error emitOperand(InsnCdp1802 &insn, AddrMode mode, const Operand &op);
    Error encodePage(InsnCdp1802 &insn, AddrMode mode, const Operand &op);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;

    static const char OPT_BOOL_USE_REGISTER[] PROGMEM;
    static const char OPT_DESC_USE_REGISTER[] PROGMEM;
    static const char OPT_BOOL_SMART_BRANCH[] PROGMEM;
    static const char OPT_DESC_SMART_BRANCH[] PROGMEM;
};

}  // namespace cdp1802
}  // namespace libasm

#endif  // __ASM_CDP1802_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
