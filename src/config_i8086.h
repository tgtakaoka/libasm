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

namespace libasm {
namespace i8086 {

enum CpuType : uint8_t {
    I8086,
    I80186,
    I80286,
    V30,
};

enum FpuType : uint8_t {
    FPU_NONE,
#if !defined(LIBASM_I8086_NOFPU)
    FPU_ON,
    FPU_I8087,
    FPU_I80287,
#endif
};

struct CpuSpec final {
    CpuSpec(CpuType cpu_, FpuType fpu_) : cpu(cpu_), fpu(fpu_) {}
    CpuType cpu;
    FpuType fpu;
};

struct Config : ConfigImpl<CpuType, ADDRESS_20BIT, ADDRESS_BYTE, OPCODE_8BIT, ENDIAN_LITTLE, 6, 7> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, I8086), _cpuSpec(I8086, FPU_NONE) {}

    uint8_t nameMax() const override { return fpuType() == FPU_NONE ? 6 : 7; }

    void setCpuType(CpuType cpuType) override;

    const char *fpu_P() const;
    FpuType fpuType() const { return _cpuSpec.fpu; }
    Error setFpuType(FpuType fpuType);
    Error setFpuName(StrScanner &scan) override;

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
