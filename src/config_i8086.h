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

#ifndef __LIBASM_CONFIG_I8086_H__
#define __LIBASM_CONFIG_I8086_H__

#include "config_base.h"

/** Disable i8087 FPU instructions */
// #define LIBASM_I8086_NOFPU
#if defined(LIBASM_ASM_NOFLOAT) && defined(LIBASM_DIS_NOFLOAT)
#define LIBASM_I8086_NOFPU
#endif

namespace libasm {
namespace i8086 {

enum CpuType : uint8_t {
    I8086,
    I80186,
    V30,
};

enum FpuType : uint8_t {
    FPU_NONE,
#if !defined(LIBASM_I8086_NOFPU)
    FPU_I8087,
#endif
};

struct CpuSpec final {
    CpuSpec(CpuType cpu_, FpuType fpu_) : cpu(cpu_), fpu(fpu_) {}
    CpuType cpu;
    FpuType fpu;
};

struct Config : ConfigImpl<CpuType, ADDRESS_20BIT, ADDRESS_BYTE, OPCODE_8BIT, ENDIAN_LITTLE, 8, 6> {
    Config(const InsnTable<CpuType> &table)
        : ConfigImpl(table, I8086),
          _cpuSpec(I8086,
#if defined(LIBASM_I8086_NOFPU)
                  FPU_NONE
#else
                  FPU_I8087
#endif
          ) {
    }

    void setCpuType(CpuType cpuType) override {
        _cpuSpec.cpu = cpuType;
        ConfigImpl::setCpuType(cpuType);
    }
    void setFpuType(FpuType fpuType) { _cpuSpec.fpu = fpuType; }
    FpuType fpuType() const { return _cpuSpec.fpu; }

protected:
    CpuSpec _cpuSpec;
};

}  // namespace i8086
}  // namespace libasm

#endif  // __LIBASM_CONFIG_I8086_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
