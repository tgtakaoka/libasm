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

#ifndef __LIBASM_CONFIG_H8300_H__
#define __LIBASM_CONFIG_H8300_H__

#include "config_base.h"
#include "value.h"

namespace libasm {
namespace h8300 {

static constexpr uint32_t UINT24_MAX = 0xFFFFFFu;

enum CpuType : uint8_t {
    H8300,
    H8300H,
    H8S2000,
};

// MaxCode = 10: largest instruction is MOV.L @(d:24,ERn),ERd / MOV.L
// @aa:24,ERd at 10 bytes (0100 78n0 6B20|6BA0 disp24). H8S/2000 also
// reaches 10 bytes via MOV.L @(d:32,ERn). H8/300 max is 4.
// The template's ADDRESS_24BIT widens Config::uintptr_t to uint32_t so the
// @aa:24 operand fits; addressWidth() is overridden to 16 unless the
// advanced-mode option is enabled on H8/300H or H8S/2000.
struct Config
    : public ConfigImpl<CpuType, ADDRESS_24BIT, ADDRESS_BYTE, OPCODE_16BIT, ENDIAN_BIG, 10, 6> {
    Config(const InsnTable<CpuType> &table)
        : ConfigImpl(table, H8300), _advancedMode(false), _spAlias(true) {}

    AddressWidth addressWidth() const override {
        return advancedMode() ? ADDRESS_24BIT : ADDRESS_16BIT;
    }
    bool hasReg32() const { return cpuType() != H8300; }
    // EXR belongs to the H8S family; extend when H8S/2600 lands.
    bool hasExr() const { return cpuType() == H8S2000; }
    bool advancedMode() const { return cpuType() != H8300 && _advancedMode; }
    Error setAdvancedMode(bool on) {
        _advancedMode = on;
        return OK;
    }

    bool spAlias() const { return _spAlias; }
    Error setSpAlias(bool on) {
        _spAlias = on;
        return OK;
    }

    // @aa:8 short page. Returns true if |val| is outside the accepted range:
    //   normal:    [0, 0xFF], [0xFF00, 0xFFFF], or [0xFFFF00, 0xFFFFFF]
    //   advanced:  [0, 0xFF] or [0xFFFF00, 0xFFFFFF]
    // Normal mode also accepts the advanced-mode high page as an alias so
    // sources shared with advanced mode assemble identically.
    bool overflowAbs8(const Value &val) const {
        if (!val.overflow(0xFF))
            return false;
        if (!val.overflow(UINT24_MAX, 0xFFFF00))
            return false;
        if (!advancedMode() && !val.overflow(0xFFFF, 0xFF00))
            return false;
        return true;
    }

    // @aa:16 absolute. Returns true if |val| is outside the accepted range:
    //   normal:    [0, 0xFFFF] or [0xFF8000, 0xFFFFFF]
    //   advanced:  [0, 0x7FFF] or [0xFF8000, 0xFFFFFF]
    // In advanced mode bit 15 is sign-extended; normal mode also accepts the
    // sign-extended high page so shared sources assemble identically.
    bool overflowAbs16(const Value &val) const {
        const uint32_t lowMax = advancedMode() ? 0x7FFF : UINT16_MAX;
        if (!val.overflow(lowMax))
            return false;
        if (!val.overflow(UINT24_MAX, 0xFF8000))
            return false;
        return true;
    }

    // @(d:16, Rn) index displacement. Returns true if |val| is outside
    // the accepted range:
    //   normal:    [0, 0xFFFF] (unsigned; only the low 16 bits of the
    //              effective address are observable in normal mode)
    //   advanced:  [-0x8000, 0x7FFF] or [0xFF8000, 0xFFFFFF]
    // d:16 is sign-extended at runtime in advanced mode (manual section 1.7).
    // The 24-bit sign-extended high page is accepted as an alias of the signed
    // range so shared sources assemble identically -- same rule as overflowAbs16.
    bool overflowDisp16(const Value &val) const {
        if (!advancedMode())
            return val.overflowUint16();
        if (!val.overflowInt16())
            return false;
        if (!val.overflow(UINT24_MAX, 0xFF8000))
            return false;
        return true;
    }

    // Returns true if a bare absolute @|addr| (no :8/:16 qualifier) would
    // be assembled as M_ABS8 by the assembler's default-mode selection,
    // i.e. |addr| falls in the high page [0xFF00, 0xFFFF]. The disassembler
    // uses this to decide when an M_ABS16 operand needs an explicit ":16"
    // qualifier to avoid being re-encoded as the shorter ":8" form.
    bool defaultsToAbs8(uintptr_t addr) const {
        return (addr & 0xFFFF) >= 0xFF00;
    }

protected:
    bool _advancedMode;
    bool _spAlias;
};

}  // namespace h8300
}  // namespace libasm

#endif  // __LIBASM_CONFIG_H8300_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
