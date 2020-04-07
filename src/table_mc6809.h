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

#ifndef __TABLE_MC6809_H__
#define __TABLE_MC6809_H__

#include "config_mc6809.h"
#include "insn_mc6809.h"

namespace libasm {
namespace mc6809 {

#define PSEUDO_SETDP   0x00     // reuse NEG opecode
#define PSEUDO_ASSUME  0x03     // reuse COM opecode

struct EntryPage {
    const Config::opcode_t prefix;
    const Entry *const table;
    const Entry *const end;
};

class TableMc6809 {
public:
    Error searchName(InsnMc6809 &insn) const;
    Error searchNameAndAddrMode(InsnMc6809 &insn) const;
    Error searchInsnCode(InsnMc6809 &insn) const;

    bool setCpu(const char *cpu);
    static const char *listCpu();
    bool is6309() const { return _cpuType == HD6309; }

    static bool isPrefixCode(Config::opcode_t opCode);

private:
    CpuType _cpuType;

    static const Entry *searchEntry(
        const char *name, const Entry *table, const Entry *end);
    static const Entry *searchEntry(
        const Config::opcode_t opCode, const Entry *table, const Entry *end);

    static Error searchName(
        InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end);
    static Error searchNameAndAddrMode(
        InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end);
    static Error searchInsnCode(
        InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end);
};

extern TableMc6809 TableMc6809;

} // namespace mc6809
} // namespace libasm

#endif // __TABLE_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
