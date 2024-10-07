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
// #define LIBASM_NS32000_NOMMU
#if defined(LIBASM_ASM_NOFLOAT) && defined(LIBASM_DIS_NOFLOAT)
#define LIBASM_NS32000_NOFPU
#endif

namespace libasm {
namespace ns32000 {

enum CpuType : uint8_t {
    NS32032,
};

enum FpuType : uint8_t {
    FPU_NONE,
#if !defined(LIBASM_NS32000_NOFPU)
    FPU_NS32081,
#endif
};
enum MmuType : uint8_t {
    MMU_NONE,
#if !defined(LIBASM_NS32000_NOMMU)
    MMU_NS32082,
#endif
};

struct CpuSpec final {
    CpuSpec(CpuType cpu_, FpuType fpu_, MmuType mmu_) : cpu(cpu_), fpu(fpu_), mmu(mmu_) {}
    CpuType cpu;
    FpuType fpu;
    MmuType mmu;
};

struct Config
    : ConfigImpl<CpuType, ADDRESS_24BIT, ADDRESS_BYTE, OPCODE_8BIT, ENDIAN_LITTLE, 25, 7> {
    Config(const InsnTable<CpuType> &table)
        : ConfigImpl(table, NS32032),
          _cpuSpec(NS32032,
#if defined(LIBASM_NS32000_NOFPU)
                   FPU_NONE,
#else
                   FPU_NS32081,
#endif
#if defined(LIBASM_NS32000_NOMMU)
                   MMU_NONE
#else
                   MMU_NS32082
#endif
          ) {
    }

    void setCpuType(CpuType cpuType) override {
        _cpuSpec.cpu = cpuType;
        ConfigImpl::setCpuType(cpuType);
    }
    void setFpuType(FpuType fpuType) { _cpuSpec.fpu = fpuType; }
    void setMmuType(MmuType mmuType) { _cpuSpec.mmu = mmuType; }
    FpuType fpuType() const { return _cpuSpec.fpu; }
    MmuType mmuType() const { return _cpuSpec.mmu; }

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
