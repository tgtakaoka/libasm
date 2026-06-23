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

#ifndef __LIBASM_CONFIG_H8500_H__
#define __LIBASM_CONFIG_H8500_H__

#include "config_base.h"

namespace libasm {
namespace h8500 {

// Three H8/500-family variants. The only instruction-set difference is the
// E-clock peripheral transfer (MOVFPE/MOVTPE); they differ otherwise only in
// maximum-mode physical address width. Page jumps (PJMP/PJSR/PRTS/PRTD) are a
// maximum-mode feature on every variant, not CPU-specific.
//   H8_500 - E-clock; 24-bit (16MB) maximum-mode physical address
//   H8_520 - no E-clock pin; 20-bit (1MB) physical
//   H8_530 - E-clock; 20-bit (1MB) physical
enum CpuType : uint8_t {
    H8_500,
    H8_520,
    H8_530,
};

// Page registers whose assumed value the assembler may be told via the `.set`
// directive. DP backs the 16-bit absolute mode (@aa:16) and BR the short
// absolute mode (@aa:8); EP/TP/CP back register-indirect / code addressing.
enum PageReg : uint8_t {
    PAGE_DP,
    PAGE_EP,
    PAGE_TP,
    PAGE_CP,
    PAGE_BR,
    PAGE_ENUM_END,
};

struct Config
    : public ConfigImpl<CpuType, ADDRESS_24BIT, ADDRESS_BYTE, OPCODE_8BIT, ENDIAN_BIG, 5, 8> {
    Config(const InsnTable<CpuType> &table)
        : ConfigImpl(table, H8_500),
          _maxMode(false),
          _fpAlias(true),
          _spAlias(true),
          _pageReg{-1, -1, -1, -1, -1} {}

    // Minimum mode is a flat 64K (16-bit) space; maximum mode reaches the chip's
    // physical width: 16M (24-bit) for H8/510 ("H8/500"), 1M (20-bit) for the
    // pin-limited H8/520 and H8/530.
    AddressWidth physicalWidth() const {
        return cpuType() == H8_500 ? ADDRESS_24BIT : ADDRESS_20BIT;
    }
    AddressWidth addressWidth() const override {
        return maxMode() ? physicalWidth() : ADDRESS_16BIT;
    }

    bool maxMode() const { return _maxMode; }
    Error setMaxMode(bool on) {
        _maxMode = on;
        return OK;
    }
    bool fpAlias() const { return _fpAlias; }
    Error setFpAlias(bool on) {
        _fpAlias = on;
        return OK;
    }
    bool spAlias() const { return _spAlias; }
    Error setSpAlias(bool on) {
        _spAlias = on;
        return OK;
    }

    // Assumed page-register value; negative = UNDEFINED (initial state). When
    // undefined the assembler drops an absolute operand's high bits; once
    // defined it validates them and rejects a page mismatch.
    int16_t getPageRegister(PageReg reg) const { return _pageReg[reg]; }
    Error setPageRegister(PageReg reg, int32_t value) {
        if (value >= 0 && (value & ~0xFF))
            return OVERFLOW_RANGE;
        _pageReg[reg] = static_cast<int16_t>(value);
        return OK;
    }
    void resetPageRegisters() {
        for (auto &p : _pageReg)
            p = -1;
    }

protected:
    bool _maxMode;
    bool _fpAlias;
    bool _spAlias;
    int16_t _pageReg[PAGE_ENUM_END];
};

}  // namespace h8500
}  // namespace libasm

#endif  // __LIBASM_CONFIG_H8500_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
