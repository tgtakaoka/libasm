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

#ifndef __LIBASM_CONFIG_AM29000_H__
#define __LIBASM_CONFIG_AM29000_H__

#include "config_base.h"

namespace libasm {
namespace am29000 {

// The family splits along three axes: whether the memory management unit and
// its cache registers exist, whether integer multiply runs in hardware, and
// whether floating point runs in hardware.  Only the first is fixed to the
// chip; the multiplier and the floating-point unit follow it by default but
// may be turned on by hand, see FpuType and IntMulType below.
enum CpuType : uint8_t {
    AM29030,  // MMU
    AM29040,  // MMU, hardware multiply
    AM29050,  // MMU, hardware multiply, FPU, plus seven exclusive opcodes
    AM29200,  // no MMU, no hardware multiply, no FPU
    AM29240,  // MMU, hardware multiply
    AM29245,  // MMU
};

// Arithmetic a family member may not execute directly still occupies a defined
// operation code; a chip without the hardware traps it to a software routine.
// Selecting a CPU turns each on for the chips which have it.  A chip which
// lacks the hardware may still have it turned on, to assemble for a system
// whose trap handlers provide the arithmetic; a chip which has it cannot have
// it turned off.
enum FpuType : uint8_t { FPU_NONE, FPU_ON };
enum IntMulType : uint8_t { MUL_NONE, MUL_ON };

struct Config
    : ConfigImpl<CpuType, ADDRESS_32BIT, ADDRESS_BYTE, OPCODE_32BIT, ENDIAN_BIG, 4, 8> {
    Config(const InsnTable<CpuType> &table)
        : ConfigImpl(table, AM29200), _fpuType(FPU_NONE), _intMulType(MUL_NONE) {}

    FpuType fpuType() const { return _fpuType; }
    IntMulType intMulType() const { return _intMulType; }
    bool hasFpu() const { return _fpuType == FPU_ON; }
    bool hasIntMul() const { return _intMulType == MUL_ON; }

    // Only the Am29050 has the floating-point unit.
    static bool cpuHasFpu(CpuType cpuType) { return cpuType == AM29050; }

    // Only the Am29050 has a coprocessor interface; on the others the
    // coprocessor-enable bit of a load or store is reserved and must be zero,
    // and their manuals spell the operand as a literal 0.
    static bool cpuHasCopro(CpuType cpuType) { return cpuType == AM29050; }
    bool hasCopro() const { return cpuHasCopro(cpuType()); }

    // The Am29240, Am29040 and Am29050 have the 32-by-32 multiplier.
    static bool cpuHasIntMul(CpuType cpuType) {
        return cpuType == AM29240 || cpuType == AM29040 || cpuType == AM29050;
    }

    Error setFpu(bool enable) {
        if (!enable && cpuHasFpu(cpuType()))
            return OPERAND_NOT_ALLOWED;
        _fpuType = enable ? FPU_ON : FPU_NONE;
        return OK;
    }

    Error setIntMul(bool enable) {
        if (!enable && cpuHasIntMul(cpuType()))
            return OPERAND_NOT_ALLOWED;
        _intMulType = enable ? MUL_ON : MUL_NONE;
        return OK;
    }

    void setCpuType(CpuType cpuType) override {
        ConfigImpl::setCpuType(cpuType);
        _fpuType = cpuHasFpu(cpuType) ? FPU_ON : FPU_NONE;
        _intMulType = cpuHasIntMul(cpuType) ? MUL_ON : MUL_NONE;
    }

private:
    FpuType _fpuType;
    IntMulType _intMulType;
};

}  // namespace am29000
}  // namespace libasm

#endif  // __LIBASM_CONFIG_AM29000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
