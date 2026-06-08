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
};

// MaxCode = 10: largest H8/300H instruction is MOV.L @(d:24,ERn),ERd / MOV.L
// @aa:24,ERd at 10 bytes (0100 78n0 6B20|6BA0 disp24). H8/300 max is 4.
// The template's ADDRESS_24BIT widens Config::uintptr_t to uint32_t so the
// @aa:24 operand fits; addressWidth() is overridden to 16 since H8/300 and
// H8/300H normal mode both run in a 16-bit address space.
struct Config
    : public ConfigImpl<CpuType, ADDRESS_24BIT, ADDRESS_BYTE, OPCODE_16BIT, ENDIAN_BIG, 10, 6> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, H8300), _spAlias(true) {}

    AddressWidth addressWidth() const override { return ADDRESS_16BIT; }
    bool hasReg32() const { return cpuType() != H8300; }

    bool spAlias() const { return _spAlias; }
    Error setSpAlias(bool on) {
        _spAlias = on;
        return OK;
    }

    // @aa:8 short page. Returns true if |val| is outside the accepted range
    // [0, 0xFF], [0xFF00, 0xFFFF], or [0xFFFF00, 0xFFFFFF]. The high page
    // [0xFFFF00, 0xFFFFFF] is accepted as an alias so that sources shared
    // with H8/300H advanced mode (e.g. @0xFFFFXX:8) assemble identically.
    bool overflowAbs8(const Value &val) const {
        if (!val.overflow(0xFF))
            return false;
        if (!val.overflow(0xFFFF, 0xFF00))
            return false;
        if (!val.overflow(UINT24_MAX, 0xFFFF00))
            return false;
        return true;
    }

    // @aa:16 absolute. Returns true if |val| is outside the accepted range
    // [0, 0xFFFF] or [0xFF8000, 0xFFFFFF]. The advanced-mode sign-extended
    // high page is accepted as an alias so shared sources assemble identically.
    bool overflowAbs16(const Value &val) const {
        if (!val.overflow(UINT16_MAX))
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
