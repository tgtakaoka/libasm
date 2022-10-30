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

#ifndef __INSN_TLCS90_H__
#define __INSN_TLCS90_H__

#include "config_tlcs90.h"
#include "entry_tlcs90.h"
#include "insn_base.h"

namespace libasm {
namespace tlcs90 {

class InsnTlcs90 : public InsnImpl<Config, Entry> {
public:
    InsnTlcs90(Insn &insn) : InsnImpl(insn) {}

    AddrMode dst() const { return flags().dst(); }
    AddrMode src() const { return flags().src(); }
    AddrMode pre() const { return _preMode; }
    void setAddrMode(AddrMode dst, AddrMode src) { setFlags(Entry::Flags::create(dst, src)); }
    void setPreMode(AddrMode pre) { _preMode = pre; }

    void setEmitInsn() { _emitInsn = true; }
    void emitInsn(Config::opcode_t opc) {
        if (_emitInsn) {
            emitByte(opc);
            _emitInsn = false;
        }
    }

private:
    AddrMode _preMode;
    bool _emitInsn;
};

}  // namespace tlcs90
}  // namespace libasm

#endif  // __INSN_TLCS90_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
