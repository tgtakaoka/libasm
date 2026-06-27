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

/** Disable SuperH FPU (SH-2E/SH-2A floating-point) instructions and float data */
// #define LIBASM_SUPERH_NOFPU

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
#if !defined(LIBASM_SUPERH_NOFPU)
    FPU_ON,    // generic "FPU enabled" intent; resolved to the CPU's FPU below
    FPU_SH2E,  // resolution targets (used as FPU page-table keys)
    FPU_SH2A,
#endif
};

// DSP unit is an orthogonal feature (only the SH-DSP CPU enables it), searched
// like the FPU rather than baked into the CPU's instruction pages.
enum DspType : uint8_t {
    DSP_NONE,
    DSP_ON,
};

// Bundles the CPU and its optional FPU/DSP features into one value passed to the
// table search, instead of threading them as separate arguments.
struct CpuSpec final {
    CpuSpec(CpuType cpu_, FpuType fpu_, DspType dsp_) : cpu(cpu_), fpu(fpu_), dsp(dsp_) {}
    CpuType cpu;
    FpuType fpu;
    DspType dsp;
};

struct Config
    : public ConfigImpl<CpuType, ADDRESS_32BIT, ADDRESS_BYTE, OPCODE_16BIT, ENDIAN_BIG, 6, 6> {
    Config(const InsnTable<CpuType> &table)
        : ConfigImpl(table, SH1), _cpuSpec(SH1, FPU_NONE, DSP_NONE) {}

    void setCpuType(CpuType cpuType) override;
    const CpuSpec &cpuSpec() const { return _cpuSpec; }

    const /* PROGMEM */ char *fpu_P() const;
    FpuType fpuType() const { return _cpuSpec.fpu; }
    Error setFpuType(FpuType fpuType);
    Error setFpuName(StrScanner &scan) override;

protected:
    CpuSpec _cpuSpec;
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
