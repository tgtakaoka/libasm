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

#ifndef __LIBASM_CONFIG_MC68000_H__
#define __LIBASM_CONFIG_MC68000_H__

#include "config_base.h"

/** Disable MC68881 FPU instructions */
// #define LIBASM_MC68000_NOFPU

namespace libasm {
namespace mc68000 {

enum CpuType : uint8_t {
    MC68000,
    MC68010,
};

enum FpuType : uint8_t {
    FPU_NONE,
#if !defined(LIBASM_MC68000_NOFPU)
    FPU_ON,
    FPU_MC68881,
#endif
};

struct CpuSpec final {
    CpuSpec(CpuType cpu_, FpuType fpu_, uint8_t fpuCid_) : cpu(cpu_), fpu(fpu_), fpuCid(fpuCid_) {}
    CpuType cpu;
    FpuType fpu;
    uint8_t fpuCid;  // FPU co-processor ID
};

struct Config
    : ConfigImpl<CpuType, ADDRESS_24BIT, ADDRESS_BYTE, OPCODE_16BIT, ENDIAN_BIG, 16, 5 + 2> {
    Config(const InsnTable<CpuType> &table)
        : ConfigImpl(table, MC68000), _cpuSpec(MC68000, FPU_NONE, 1) {}

    uint8_t codeMax() const override { return _cpuSpec.fpu == FPU_NONE ? 6 : 16; }

    void setCpuType(CpuType cpuType) override {
        _cpuSpec.cpu = cpuType;
        ConfigImpl::setCpuType(cpuType);
    }
    const char *fpu_P() const;
    FpuType fpuType() const { return _cpuSpec.fpu; }
    Error setFpuType(FpuType fpuType);
    Error setFpuName(StrScanner &scan) override;
    // TODO: Add option
    void setFpuId(uint8_t id) { _cpuSpec.fpuCid = id; }

protected:
    CpuSpec _cpuSpec;
};

}  // namespace mc68000
}  // namespace libasm

#endif  // __LIBASM_CONFIG_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
