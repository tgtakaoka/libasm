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

#ifndef __LIBASM_CONFIG_NS32000_H__
#define __LIBASM_CONFIG_NS32000_H__

#include "config_base.h"

/** Disable NS32081 FPU instructions */
// #define LIBASM_NS32000_NOFPU
/** Disable NS32082 PMMU instructions */
// #define LIBASM_NS32000_NOPMMU

namespace libasm {
namespace ns32000 {

enum CpuType : uint8_t {
    NS32032,
};

enum FpuType : uint8_t {
    FPU_NONE,
#if !defined(LIBASM_NS32000_NOFPU)
    FPU_ON,
    FPU_NS32081,
#endif
};
enum PmmuType : uint8_t {
    PMMU_NONE,
#if !defined(LIBASM_NS32000_NOPMMU)
    PMMU_ON,
    PMMU_NS32082,
#endif
};

struct CpuSpec final {
    CpuSpec(CpuType cpu_, FpuType fpu_, PmmuType pmmu_) : cpu(cpu_), fpu(fpu_), pmmu(pmmu_) {}
    CpuType cpu;
    FpuType fpu;
    PmmuType pmmu;
};

struct Config
    : ConfigImpl<CpuType, ADDRESS_24BIT, ADDRESS_BYTE, OPCODE_8BIT, ENDIAN_LITTLE, 25, 7> {
    Config(const InsnTable<CpuType> &table)
        : ConfigImpl(table, NS32032), _cpuSpec(NS32032, FPU_NONE, PMMU_NONE) {}

    void setCpuType(CpuType cpuType) override {
        _cpuSpec.cpu = cpuType;
        ConfigImpl::setCpuType(cpuType);
    }
    const char *fpu_P() const;
    FpuType fpuType() const { return _cpuSpec.fpu; }
    Error setFpuType(FpuType fpuType);
    Error setFpuName(StrScanner &scan) override;

    const char *pmmu_P() const;
    PmmuType pmmuType() const { return _cpuSpec.pmmu; }
    Error setPmmuType(PmmuType pmmuType);
    Error setPmmuName(StrScanner &scan);

protected:
    CpuSpec _cpuSpec;
};

}  // namespace ns32000
}  // namespace libasm

#endif  // __LIBASM_CONFIG_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
