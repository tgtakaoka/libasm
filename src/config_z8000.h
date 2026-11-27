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

#ifndef __LIBASM_CONFIG_Z8000_H__
#define __LIBASM_CONFIG_Z8000_H__

#include "config_base.h"

/** Disable Z8070 FPU instructions */
// #define LIBASM_Z8000_NOFPU

namespace libasm {
namespace z8000 {

enum CpuType : uint8_t {
    Z8001,
    Z8002,
};

enum FpuType : uint8_t {
    FPU_NONE,
#if !defined(LIBASM_Z8000_NOFPU)
    FPU_ON,
    FPU_Z8070,
#endif
};

struct CpuSpec final {
    CpuSpec(CpuType cpu_, FpuType fpu_) : cpu(cpu_), fpu(fpu_) {}
    CpuType cpu;
    FpuType fpu;
};

struct Config : ConfigImpl<CpuType, ADDRESS_23BIT, ADDRESS_BYTE, OPCODE_16BIT, ENDIAN_BIG, 10, 6> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, Z8001), _cpuSpec(Z8001, FPU_NONE) {}

    bool segmentedModel() const { return cpuType() == Z8001; }

    AddressWidth addressWidth() const override {
        return segmentedModel() ? ADDRESS_23BIT : ADDRESS_16BIT;
    }
    uint8_t nameMax() const override { return _cpuSpec.fpu == FPU_NONE ? 6 : 8; }

    void setCpuType(CpuType cpuType) override {
        _cpuSpec.cpu = cpuType;
        ConfigImpl::setCpuType(cpuType);
    }
    const char *fpu_P() const;
    FpuType fpuType() const { return _cpuSpec.fpu; }
    Error setFpuType(FpuType fpuType);
    Error setFpuName(StrScanner &scan) override;

protected:
    CpuSpec _cpuSpec;
};

}  // namespace z8000
}  // namespace libasm

#endif  // __LIBASM_CONFIG_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
