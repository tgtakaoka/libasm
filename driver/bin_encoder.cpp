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

#include "bin_encoder.h"

namespace libasm {
namespace driver {

uint_fast8_t BinEncoder::addressSize(uint32_t addr) {
    if ((addr & UINT32_C(0xFFFF0000)) == 0)
        return 2;
    if ((addr & UINT32_C(0xFF000000)) == 0)
        return 3;
    return 4;
}

uint_fast8_t BinEncoder::maxAddressSize(const BinMemory &memory) {
    uint_fast8_t max_size = 0;
    for (auto *block = memory.begin(); block != nullptr; block = block->next()) {
        const auto last_size = addressSize(block->base() + block->size() - 1);
        if (last_size > max_size)
            max_size = last_size;
    };
    return max_size;
}

int BinEncoder::encode(const BinMemory &memory, TextPrinter &out) {
    _address_size = maxAddressSize(memory);
    this->begin(out);
    for (auto *block = memory.begin(); block != nullptr; block = block->next()) {
        for (size_t i = 0; i < block->size(); i += _record_size) {
            auto size = _record_size;
            if (size >= block->size() - i)
                size = block->size() - i;
            this->encode(out, block->base() + i, block->data() + i, size);
        }
    };
    this->end(out);
    return 0;
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
