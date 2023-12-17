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

#ifndef __LIBASM_BIN_ENCODER_H__
#define __LIBASM_BIN_ENCODER_H__

#include "bin_memory.h"
#include "config_base.h"
#include "text_printer.h"

#include <cstdint>

namespace libasm {
namespace driver {

struct BinEncoder {
    virtual void reset(AddressWidth addrWidth, uint8_t recordSize);
    int encode(const BinMemory &memory, TextPrinter &out);

protected:
    AddressWidth _addr_width;
    uint8_t _record_size;

    virtual void begin(TextPrinter &out) = 0;
    virtual void encode(TextPrinter &out, uint32_t addr, const uint8_t *data, uint8_t size) = 0;
    virtual void end(TextPrinter &out) = 0;

    static uint8_t addressSize(AddressWidth addrWidth);
};

}  // namespace driver
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
