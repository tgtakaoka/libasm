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

#ifndef __TABLE_W65C816_H__
#define __TABLE_W65C816_H__

#include "config_w65c816.h"
#include "insn_w65c816.h"
#include "table_base.h"

namespace libasm {
namespace w65c816 {

static constexpr Config::opcode_t PSEUDO_LONGA  = 0xCB;
static constexpr Config::opcode_t PSEUDO_LONGI  = 0xDB;
static constexpr Config::opcode_t PSEUDO_ASSUME = 0x0B;

class TableW65C816 : private TableBase {
public:
    Error searchName(InsnW65C816 &insn) const;
    Error searchNameAndAddrMode(InsnW65C816 &insn) const;
    Error searchOpCode(InsnW65C816 &insn, bool acceptIndirectLong) const;

    const char *listCpu() override;
    bool setCpu(const char *cpu) override;

private:
    Error searchName(
        InsnW65C816 &insn, const Entry *table, const Entry *end) const;
    Error searchNameAndAddrMode(
        InsnW65C816 &insn, const Entry *table, const Entry *end) const;
    Error searchOpCode(
        InsnW65C816 &insn, bool acceptIndirectLong,
        const Entry *table, const Entry *end) const;
};

extern TableW65C816 TableW65C816;

} // namespace w65c816
} // namespace libasm

#endif // __TABLE_W65C816_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
