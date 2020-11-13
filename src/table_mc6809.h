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
#include "error_reporter.h"
#include "insn_mc6809.h"
#include "table_base.h"
#include "reg_mc6809.h"

namespace libasm {
namespace mc6809 {

struct PostSpec {
    RegName index;
    RegName base;
    int8_t size;
    bool indir;

    PostSpec() {}
    constexpr PostSpec(
        RegName _index,
        RegName _base,
        int8_t _size,
        bool _indir)
        : index(_index),
          base(_base),
          size(_size),
          indir(_indir) {}
};

class TableMc6809 : public TableBase {
public:
    TableMc6809();

    Error searchName(InsnMc6809 &insn) const;
    Error searchOpCode(InsnMc6809 &insn) const;
    Error searchPostByte(const uint8_t post, PostSpec &spec) const;
    int16_t searchPostSpec(PostSpec &spec) const;

    const char *listCpu() const override;
    bool setCpu(const char *cpu) override;
    const char *getCpu() const override;
    CpuType cpuType() const { return _cpuType; }

    static bool isPrefix(Config::opcode_t opCode);

    struct EntryPage;
    struct PostEntry;

private:
    CpuType _cpuType;
    const EntryPage *_table;
    const EntryPage *_end;

    bool setCpu(CpuType cpuType);

    Error searchName(
      InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end) const;
    Error searchOpCode(
        InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end) const;
    Error searchPostByte(
        const uint8_t post, PostSpec &spec,
        const PostEntry *table, const PostEntry *end) const;
    int16_t searchPostSpec(
        PostSpec &spec, const PostEntry *table, const PostEntry *end) const;
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
