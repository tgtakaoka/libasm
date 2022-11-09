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
#include "reg_mc6809.h"
#include "table_base.h"

namespace libasm {
namespace mc6809 {

struct PostSpec {
    RegName index;
    RegName base;
    int8_t size;
    bool indir;

    PostSpec() {}
    constexpr PostSpec(RegName _index, RegName _base, int8_t _size, bool _indir)
        : index(_index), base(_base), size(_size), indir(_indir) {}
};

class TableMc6809 : public TableBase {
public:
    TableMc6809();

    static TableMc6809 TABLE;

    Error searchName(InsnMc6809 &insn) const;
    Error searchOpCode(InsnMc6809 &insn) const;
    bool isPrefix(uint8_t code) const { return _cpu->isPrefix(code); }

    Error searchPostByte(const uint8_t post, PostSpec &spec) const;
    int16_t searchPostSpec(PostSpec &spec) const;

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override { return _cpu->name_P(); }
    bool setCpu(const char *cpu) override;
    CpuType cpuType() const;

    struct PostEntry : PostSpec {
        uint8_t mask;
        uint8_t byte;
        constexpr PostEntry(RegName _index, RegName _base, int8_t _size, bool _indir, uint8_t _mask,
                uint8_t _byte)
            : PostSpec(_index, _base, _size, _indir), mask(_mask), byte(_byte) {}
    };
    typedef PageBase<TableMc6809::PostEntry> PostPage;

    typedef EntryPageBase<Entry> EntryPage;
    struct Cpu : CpuBase<CpuType, EntryPage> {
        constexpr Cpu(CpuType cpuType, const /*PROGMEM*/ char *name,
                const /*PROGMEM*/ EntryPage *table, const /*PROGMEM*/ EntryPage *end,
                const /*PROGMEM*/ PostPage *postTable, const /*PROGMEM*/ PostPage *postEnd)
            : CpuBase(cpuType, name, table, end), _post_table(postTable), _post_end(postEnd) {}
        const PostPage *postTable() const;
        const PostPage *postEnd() const;

    private:
        const PostPage *_post_table;
        const PostPage *_post_end;
    };

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
