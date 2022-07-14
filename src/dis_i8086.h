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

#ifndef __DIS_I8086_H__
#define __DIS_I8086_H__

#include "config_i8086.h"
#include "dis_base.h"
#include "insn_i8086.h"
#include "reg_i8086.h"
#include "table_i8086.h"

namespace libasm {
namespace i8086 {

class DisI8086 : public Disassembler, public Config {
public:
    DisI8086() : Disassembler(_formatter, _regs, TableI8086, '$'), _formatter(), _regs() {
        reset();
    }

    const ConfigBase &config() const override { return *this; }
    void reset() override {
        _segOverrideInsn = true;
        _repeatHasStringInst = false;
    }

    static const char OPT_BOOL_SEGMENT_INSN[] PROGMEM;
    static const char OPT_BOOL_STRING_INSN[] PROGMEM;

private:
    IntelValueFormatter _formatter;
    RegI8086 _regs;
    bool _segOverrideInsn;
    bool _repeatHasStringInst;
    const BoolOption _opt_segmentInsn{OPT_BOOL_SEGMENT_INSN, _segOverrideInsn, _options};
    const BoolOption _opt_stringInsn{OPT_BOOL_STRING_INSN, _repeatHasStringInst, _options};

    StrBuffer &outRegister(StrBuffer &out, RegName name, const char prefix = 0);
    Error outMemReg(DisMemory &memory, InsnI8086 &insn, StrBuffer &out, RegName seg, uint8_t mode,
            uint8_t r_m);

    RegName decodeRegister(const InsnI8086 &insn, AddrMode mode, OprPos pos);
    Error decodeRelative(DisMemory &memory, InsnI8086 &insn, StrBuffer &out, AddrMode mode);
    Error decodeImmediate(DisMemory &memory, InsnI8086 &insn, StrBuffer &out, AddrMode mode);
    Error decodeRepeatStr(DisMemory &memory, InsnI8086 &insn, StrBuffer &out);
    Error decodeMemReg(
            DisMemory &memory, InsnI8086 &insn, StrBuffer &out, AddrMode mode, OprPos pos);
    Error decodeOperand(
            DisMemory &memory, InsnI8086 &insn, StrBuffer &out, AddrMode mode, OprPos pos);
    Error readCodes(DisMemory &memory, InsnI8086 &insn);
    Error decodeStringInst(DisMemory &memory, InsnI8086 &insn, StrBuffer &out);
    Error decode(DisMemory &memory, Insn &insn, StrBuffer &out) override;
};

}  // namespace i8086
}  // namespace libasm

#endif  // __DIS_I8086_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
