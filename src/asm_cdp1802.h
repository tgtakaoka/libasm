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
    AsmCdp1802()
        : Assembler(_parser, TableCdp1802::TABLE, _pseudos), _parser(_commentParser), _pseudos() {
        reset();
    }

    const ConfigBase &config() const override { return *this; }
    void reset() override { _useReg = _smartBranch = false; }
    const Options &options() const override { return _options; }

private:
    IntelValueParser _parser;
    const struct : CommentParser {
        bool commentLine(const StrScanner &scan) const override {
            return (*scan == '.' && scan[1] == '.') || endOfLine(scan);
        }
        bool endOfLine(const StrScanner &scan) const override { return *scan == 0 || *scan == ';'; }
    } _commentParser;
    PseudoBase _pseudos;

    bool _useReg;
    bool _smartBranch;
    const struct OptSmartBranch : public BoolOption {
        OptSmartBranch(bool &var);
    } _opt_smartBranch{_smartBranch};
    const struct OptUseRegister : public BoolOption {
        OptUseRegister(bool &var, const OptionBase &next);
    } _opt_useReg{_useReg, _opt_smartBranch};
    const Options _options{_opt_useReg};

    struct Operand : public OperandBase {
        AddrMode mode;
        uint16_t val16;
        Operand() : mode(M_NONE), val16(0) {}
    };

    Error parseOperand(StrScanner &scan, Operand &op) const;

    void emitOperand(InsnCdp1802 &insn, AddrMode mode, const Operand &op);
    void encodePage(InsnCdp1802 &insn, AddrMode mode, const Operand &op);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;
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
