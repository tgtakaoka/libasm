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

uint8_t BinEncoder::addressSize(AddressWidth addrWidth) {
    const uint8_t bits = uint8_t(addrWidth);
    if (bits <= 16)
        return 2;
    if (bits <= 24)
        return 3;
    return 4;
}

void BinEncoder::reset(AddressWidth addrWidth, uint8_t recordSize) {
    _addr_width = addrWidth;
    _record_size = recordSize;
}

int BinEncoder::encode(const BinMemory &memory, TextPrinter &out) {
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
