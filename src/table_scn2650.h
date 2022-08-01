/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __TABLE_SCN2650_H__
#define __TABLE_SCN2650_H__

#include "config_scn2650.h"
#include "insn_scn2650.h"
#include "table_base.h"

namespace libasm {
namespace scn2650 {

class TableScn2650 : public TableBase {
public:
    static TableScn2650 TABLE;

    Error searchName(InsnScn2650 &insn);
    Error searchOpCode(InsnScn2650 &insn);

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override;
    bool setCpu(const char *cpu) override;
};

}  // namespace scn2650
}  // namespace libasm

#endif  // __TABLE_SCN2650_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4: