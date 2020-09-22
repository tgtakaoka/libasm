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

class AsmCdp1802
    : public Assembler,
      public Config {
public:
    AsmCdp1802() : Assembler(_parser, TableCdp1802) {}

private:
    IntelValueParser _parser;

    Error encodeRegn(InsnCdp1802 &insn);
    Error encodeImm8(InsnCdp1802 &insn);
    Error encodePage(InsnCdp1802 &insn);
    Error encodeAddr(InsnCdp1802 &insn);
    Error encodeIoad(InsnCdp1802 &insn);

    Error encode(Insn &insn) override;
};

} // namespace cdp1802
} // namespace libasm

#endif // __ASM_CDP1802_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
