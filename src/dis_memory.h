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

#ifndef __LIBASM_DIS_MEMORY_H__
#define __LIBASM_DIS_MEMORY_H__

#include <stdint.h>

namespace libasm {

struct DisMemory {
    virtual bool hasNext() const = 0;
    uint32_t address() const { return _address; }
    uint8_t readByte() {
        const uint8_t val = nextByte();
        _address++;
        return val;
    }

protected:
    DisMemory(uint32_t address) : _address(address) {}
    void resetAddress(uint32_t address) { _address = address; }
    virtual uint8_t nextByte() = 0;

private:
    uint32_t _address;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
