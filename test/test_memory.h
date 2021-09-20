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

#ifndef __TEST_MEMORY_H__
#define __TEST_MEMORY_H__

#include <stdint.h>

#include "config_base.h"
#include "config_host.h"
#include "dis_memory.h"

namespace libasm {
namespace test {

class TestMemory : public DisMemory {
public:
    TestMemory();
    virtual ~TestMemory();

    void setMemory(uint32_t addr, const uint8_t *data, uint8_t size);
    void setMemory(uint32_t addr, const uint16_t *data, uint8_t size, Endian endian);
    bool hasNext() const override;
    const uint8_t *bytes() const;
    uint8_t length() const;
    uint32_t origin() const;
    char *dump(char *out) const;

protected:
    uint8_t nextByte() override;

private:
    size_t _size;
    uint8_t *_bytes;
    uint32_t _origin;
    uint8_t _length;
    uint8_t _index;
    const uint16_t *_words;

    void ensureBytes(size_t size);
};

}  // namespace test
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
