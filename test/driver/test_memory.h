/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __TEST_MEMORY_H__
#define __TEST_MEMORY_H__

#include "bin_memory.h"
#include "config_base.h"

namespace libasm {
namespace driver {
namespace test {

struct TestMemory : BinMemory {
    TestMemory() : BinMemory() {}

    // write byte iterator
    struct ByteWriter {
        uint32_t address() const { return _addr; }
        const BinMemory &memory() const { return _memory; }
        ByteWriter &add(uint8_t val) { return emitByte(val); }

    protected:
        ByteWriter &emitByte(uint8_t val) {
            _memory.writeByte(_addr++, val);
            return *this;
        }

    private:
        friend TestMemory;
        ByteWriter(BinMemory &memory, uint32_t addr) : _memory(memory), _addr(addr) {}

        BinMemory &_memory;
        uint32_t _addr;
    };

    // write uint16_t iterator
    struct Uint16Writer : ByteWriter {
        uint32_t address() const { return ByteWriter::address(); }
        const BinMemory &memory() const { return ByteWriter::memory(); }
        Uint16Writer &add(uint16_t val) { return emitUint16(val); }

    private:
        friend TestMemory;
        Uint16Writer(BinMemory &memory, uint32_t addr, Endian endian)
            : ByteWriter(memory, addr), _endian(endian) {}
        Uint16Writer &emitUint16(uint16_t val) {
            if (_endian == ENDIAN_BIG) {
                emitByte(val >> 8).emitByte(val);
            } else {
                emitByte(val).emitByte(val >> 8);
            }
            return *this;
        }

        const Endian _endian;
    };

    auto writer(uint32_t addr) { return ByteWriter(*this, addr); }
    auto writer(uint32_t addr, Endian endian) { return Uint16Writer(*this, addr, endian); }
};

}  // namespace test
}  // namespace driver
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
