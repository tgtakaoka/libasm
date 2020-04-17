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

#ifndef __TABLE_TMS9900_H__
#define __TABLE_TMS9900_H__

#include "config_tms9900.h"
#include "insn_tms9900.h"
#include "table_base.h"

namespace libasm {
namespace tms9900 {

class TableTms9900 : private TableBase {
public:
    Error searchName(InsnTms9900 &insn) const;
    Error searchInsnCode(InsnTms9900 &insn) const;

    const char *listCpu() override;
    bool setCpu(const char *cpu) override;
    bool is9995() const { return _cpuType == TMS9995; }

private:
    CpuType _cpuType;
};

extern TableTms9900 TableTms9900;

} // namespace tms9900
} // namespace libasm

#endif // __TABLE_TMS9900_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
