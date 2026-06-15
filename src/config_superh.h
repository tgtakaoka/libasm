/*
 * Copyright 2026 Tadashi G. Takaoka
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

#ifndef __LIBASM_CONFIG_SUPERH_H__
#define __LIBASM_CONFIG_SUPERH_H__

#include "config_base.h"

namespace libasm {
namespace superh {

enum CpuType : uint8_t {
    SH1,
    SH2,
    SH_DSP,
    SH2E,
    SH2A,
};

enum FpuType : uint8_t {
    FPU_NONE,
    FPU_SH2E,
    FPU_SH2A,
};

struct Config
    : public ConfigImpl<CpuType, ADDRESS_32BIT, ADDRESS_BYTE, OPCODE_16BIT, ENDIAN_BIG, 6, 6> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, SH1), _fpu(FPU_NONE) {}

    void setCpuType(CpuType cpuType) override;

    const /* PROGMEM */ char *fpu_P() const;
    FpuType fpuType() const { return _fpu; }
    Error setFpuType(FpuType fpuType);
    Error setFpuName(StrScanner &scan) override;

protected:
    FpuType _fpu;
};

}  // namespace superh
}  // namespace libasm

#endif  // __LIBASM_CONFIG_SUPERH_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
