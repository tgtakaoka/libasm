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
#include "entry_table.h"
#include "insn_mc6809.h"
#include "reg_mc6809.h"

namespace libasm {
namespace mc6809 {

using namespace reg;

struct PostSpec {
    RegName index;
    RegName base;
    int8_t size;
    bool indir;

    PostSpec() {}
    constexpr PostSpec(RegName _index, RegName _base, int8_t _size, bool _indir)
        : index(_index), base(_base), size(_size), indir(_indir) {}
};

struct TableMc6809 : entry::Table {
    TableMc6809();

    static TableMc6809 TABLE;

    Error searchName(InsnMc6809 &insn) const;
    Error hasName(InsnMc6809 &insn) const;
    Error searchOpCode(InsnMc6809 &insn, StrBuffer &out) const;
    bool isPrefix(uint8_t code) const;

    Error searchPostByte(const uint8_t post, PostSpec &spec) const;
    int16_t searchPostSpec(PostSpec &spec) const;

    const /* PROGMEM */ char *cpu_P() const override;
    bool setCpu(const char *cpu) override;
    CpuType cpuType() const;

    typedef entry::TableBase<Entry> EntryPage;
    struct Cpu;

private:
    const Cpu *_cpu;

    bool setCpu(CpuType cpuType);
};

}  // namespace mc6809
}  // namespace libasm

#endif  // __TABLE_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
