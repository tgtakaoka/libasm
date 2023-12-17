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

#ifndef __LIBASM_INTEL_HEX_H__
#define __LIBASM_INTEL_HEX_H__

#include "bin_decoder.h"
#include "bin_encoder.h"

namespace libasm {
namespace driver {

struct IntelHex : BinDecoder, BinEncoder {
    static BinDecoder &decoder();
    static BinEncoder &encoder();

private:
    uint32_t _last_addr;  // start address of the last block
    uint32_t _next_addr;  // expected address of the next block
    uint8_t _check_sum;

    // BinEncoder
    void reset(AddressWidth addrWidth, uint8_t recordSize) override;
    void begin(TextPrinter &out) override {}
    void encode(TextPrinter &out, uint32_t addr, const uint8_t *data, uint8_t size) override;
    void end(TextPrinter &out) override;
    // BinDecoder
    void reset() override;
    int decode(StrScanner &line, BinMemory &memory) override;

    void resetSum() { _check_sum = 0; }
    void addSum8(uint8_t data) { _check_sum += data; }
    void addSum16(uint16_t data) {
        addSum8(data >> 8);
        addSum8(data);
    }
    void addSum32(uint32_t data) {
        addSum16(data >> 16);
        addSum16(data);
    }
    uint8_t getSum() const;

    void formatEla(TextPrinter &out, uint32_t addr);
    void encodeLine(TextPrinter &out, uint16_t addr, const uint8_t *data, uint8_t size);
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
