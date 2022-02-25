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

#ifndef __TABLE_MC68000_H__
#define __TABLE_MC68000_H__

#include "config_mc68000.h"
#include "insn_mc68000.h"
#include "table_base.h"

namespace libasm {
namespace mc68000 {

class TableMc68000 : public TableBase {
public:
    TableMc68000();

    Error searchName(InsnMc68000 &insn) const;
    Error searchOpCode(InsnMc68000 &insn) const;

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override;
    bool setCpu(const char *cpu) override;

    void setAlias(bool enable) { _aliasEnabled = enable; }

private:
    bool _aliasEnabled;
};

extern TableMc68000 TableMc68000;

}  // namespace mc68000
}  // namespace libasm

#endif  // __TABLE_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
