/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __ARRAY_MEMORY_H__
#define __ARRAY_MEMORY_H__

#include <stdint.h>

#include "config_base.h"
#include "dis_memory.h"

namespace libasm {

class ArrayMemory : public DisMemory {
public:
    ArrayMemory(uint32_t origin, const uint8_t *bytes, size_t size, Endian endian = ENDIAN_BIG);
    ArrayMemory(uint32_t origin, const uint16_t *words, size_t size, Endian endian);

    uint32_t origin() const;
    size_t size() const;
    void rewind();
    bool hasNext() const override;

protected:
    uint8_t nextByte() override;

private:
    const uint32_t _origin;
    const uint8_t *_bytes;
    const uint16_t *_words;
    const size_t _size;
    const Endian _endian;
    size_t _index;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
