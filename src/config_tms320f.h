/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_CONFIG_TMS320F_H__
#define __LIBASM_CONFIG_TMS320F_H__

#include "config_base.h"

namespace libasm {
namespace tms320f {

enum CpuType : uint8_t {
    TMS320C30,
    TMS320C31,
    TMS320C32,
};

// The power-down modes and the augmented parallel operands arrived with
// later silicon rather than with a new part number, so they are modelled as
// capabilities of the selected CPU rather than as CPU variants.  User's
// Guide 7.9 (7-48) lists IDLE2 and LOPOWER on 'C30 silicon 7.0 or greater,
// 'LC31, 'C31 revision 5.0 or greater, 'C32 and 'VC33; the augmented operand
// note on each parallel instruction page lists 'C31 revision 6.0 or greater,
// 'C32 revision 2.0 or greater and 'VC33.  The 'VC33 needs no CPU of its own:
// its capabilities are exactly those of a 'C32 revision 2.0 or greater.
enum Idle2Type : uint8_t { IDLE2_NONE, IDLE2_ON };
enum LoPowerType : uint8_t { LOPOWER_NONE, LOPOWER_ON };
enum EnhancedType : uint8_t { ENHANCED_NONE, ENHANCED_ON };

struct CpuSpec final {
    CpuSpec(CpuType cpu_, Idle2Type idle2_, LoPowerType lopower_, EnhancedType enhanced_)
        : cpu(cpu_), idle2(idle2_), lopower(lopower_), enhanced(enhanced_), silicon(0) {}
    CpuType cpu;
    Idle2Type idle2;
    LoPowerType lopower;
    EnhancedType enhanced;
    // Revision asked for, or 0 to follow the selected CPU.  Silicon revisions
    // are numbered from 1, so 0 is free to mean "not named".  A revision named
    // explicitly outlives a later CPU change, so "--silicon" and a "cpu"
    // directive can be given in either order.
    uint8_t silicon;
};

struct Config
    : ConfigImpl<CpuType, ADDRESS_24BIT, ADDRESS_QUAD, OPCODE_32BIT, ENDIAN_LITTLE, 1, 9> {
    Config(const InsnTable<CpuType> &table)
        : ConfigImpl(table, TMS320C30),
          _cpuSpec(TMS320C30, IDLE2_NONE, LOPOWER_NONE, ENHANCED_NONE) {}

    bool hasIdle2() const { return _cpuSpec.idle2 == IDLE2_ON; }
    bool hasLoPower() const { return _cpuSpec.lopower == LOPOWER_ON; }
    bool hasEnhanced() const { return _cpuSpec.enhanced == ENHANCED_ON; }

    // The 'C30 has no augmented parallel operands on any silicon.
    static bool cpuHasEnhanced(CpuType cpuType) { return cpuType != TMS320C30; }

    // The silicon revision is the only knob: every documented combination of
    // the capabilities below follows from the CPU and its revision, so they
    // are not settable one by one.
    Error setSilicon(int32_t rev);
    void setCpuType(CpuType cpuType) override;
    // Forget an explicitly named revision and follow the CPU again.
    void resetSilicon();

protected:
    // Revision each CPU is usually met as, when none was named.
    static uint8_t defaultSilicon(CpuType cpuType);
    void deriveCapabilities();

    Error setIdle2(bool enable);
    Error setLoPower(bool enable);
    Error setEnhanced(bool enable);

    CpuSpec _cpuSpec;

    uint8_t nameMax() const override { return 8; }
    static constexpr Config::uintptr_t MAX_ADDRESS = (UINT32_C(1) << ADDRESS_24BIT) - 1;
};

}  // namespace tms320f
}  // namespace libasm

#endif  // __LIBASM_CONFIG_TMS320F_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
