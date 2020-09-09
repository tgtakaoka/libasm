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

enum IndexedSubMode : uint8_t {
    PNTR_IDX = 0,  // ,X [,X}
    DISP_IDX = 1,  // n5,X n8,X   n8,PCR   n16,X   n16,PCR
                   //     [n8,X] [n8,PCR] [n16,X] [n16,PCR]
    ACCM_IDX = 2,  // R,X [R,X]
    AUTO_IDX = 3,  // ,X+ ,X++ ,-X, ,--X [,X++] [,--X]
    ABS_IDIR = 4,  // [n16]
};

struct PostSpec {
    IndexedSubMode mode;
    RegName index;
    RegName base;
    int8_t size;
    bool indir;
};

class TableMc6809 : public TableBase {
public:
    TableMc6809();

    Error searchName(InsnMc6809 &insn) const;
    Error searchOpCode(InsnMc6809 &insn) const;
    Error searchPostByte(const uint8_t post, PostSpec &spec) const;
    Error searchPostSpec(PostSpec &spec, uint8_t &post) const;

    const char *listCpu() const override;
    bool setCpu(const char *cpu) override;
    const char *getCpu() const override;
    CpuType cpuType() const { return _cpuType; }

    static bool isPrefixCode(Config::opcode_t opCode);

    struct EntryPage;
    struct PostEntry;

private:
    CpuType _cpuType;
    const EntryPage *_table;
    const EntryPage *_end;

    void setCpu(CpuType cpuType);

    static Error searchName(
        InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end);
    static Error searchOpCode(
        InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end);
    static Error searchPostByte(
        const uint8_t post, PostSpec &spec,
        const PostEntry *table, const PostEntry *end);
    static Error searchPostSpec(
        PostSpec &spec, uint8_t &post,
        const PostEntry *table, const PostEntry *end);
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
