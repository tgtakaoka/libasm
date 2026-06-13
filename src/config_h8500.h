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

enum CpuType : uint8_t {
    H8_500,
    H8_520,
};

struct Config
    : public ConfigImpl<CpuType, ADDRESS_24BIT, ADDRESS_BYTE, OPCODE_8BIT, ENDIAN_BIG, 5, 8> {
    Config(const InsnTable<CpuType> &table)
        : ConfigImpl(table, H8_500), _maxMode(false), _fpAlias(true), _spAlias(true) {}

    AddressWidth addressWidth() const override { return maxMode() ? ADDRESS_24BIT : ADDRESS_16BIT; }
    bool hasEclock() const { return cpuType() == H8_500; }
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

protected:
    bool _maxMode;
    bool _fpAlias;
    bool _spAlias;
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
