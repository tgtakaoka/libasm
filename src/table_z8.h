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

#ifndef __TABLE_Z8_H__
#define __TABLE_Z8_H__

#include "config_z8.h"
#include "insn_z8.h"
#include "table_base.h"

namespace libasm {
namespace z8 {

class TableZ8 : public TableBase {
public:
    Error searchName(InsnZ8 &insn) const;
    Error searchOpCode(InsnZ8 &insn) const;

    const char *listCpu() override { return "Z8"; }
    bool setCpu(const char *cpu) override;
    const char *getCpu() override { return listCpu(); }
};

extern TableZ8 TableZ8;

} // namespace z8
} // namespace libasm

#endif // __TABLE_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
