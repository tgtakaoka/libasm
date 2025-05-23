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

#include "intel_hex.h"

#include "bin_memory.h"
#include "stored_printer.h"
#include "test_driver_helper.h"
#include "test_reader.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

// clang-format off
const uint8_t block1[] = {
    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00,
    0x12, 0x34,
};
const uint8_t block2[] = {
    0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
    0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
    0x45, 0x67, 0x89,
};
// clang-format on

void writeBlock(BinMemory &memory, uint32_t base, const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++)
        memory.writeByte(base + i, data[i]);
}
#define WRITE_BLOCK(_mem, _base, _array) writeBlock(_mem, _base, _array, sizeof(_array))

void test_encoder() {
    IntelHex intelHex;
    BinEncoder &encoder = intelHex.encoder();

    BinMemory memory;
    WRITE_BLOCK(memory, 0x123400, block1);

    StoredPrinter out;
    encoder.reset(32);
    encoder.encode(memory, out);
    EQ("lines", 3, out.size());
    EQ("line 1", ":020000040012E8", out.line(1));
    EQ("line 2", ":12340000112233445566778899AABBCCDDEEFF0012347C", out.line(2));
    EQ("end", ":00000001FF", out.line(3));

    out.clear();
    encoder.reset(16);
    encoder.encode(memory, out);
    EQ("lines", 4, out.size());
    EQ("line 1", ":020000040012E8", out.line(1));
    EQ("line 2", ":10340000112233445566778899AABBCCDDEEFF00C4", out.line(2));
    EQ("line 3", ":02341000123474", out.line(3));
    EQ("end", ":00000001FF", out.line(4));

    out.clear();
    encoder.reset(8);
    encoder.encode(memory, out);
    EQ("lines", 5, out.size());
    EQ("line 1", ":020000040012E8", out.line(1));
    EQ("line 2", ":08340000112233445566778860", out.line(2));
    EQ("line 3", ":0834080099AABBCCDDEEFF0028", out.line(3));
    EQ("line 4", ":02341000123474", out.line(4));
    EQ("end", ":00000001FF", out.line(5));

    BinMemory interseg;
    WRITE_BLOCK(interseg, 0x1234FFF0, block1);
    out.clear();
    encoder.reset(16);
    encoder.encode(interseg, out);
    EQ("lines", 5, out.size());
    EQ("extended", ":020000041234B4", out.line(1));
    EQ("line 1", ":10FFF000112233445566778899AABBCCDDEEFF0009", out.line(2));
    EQ("extended", ":020000041235B3", out.line(3));
    EQ("line 2", ":020000001234B8", out.line(4));
    EQ("line 3", ":00000001FF", out.line(5));
}

void test_encoder_blocks() {
    IntelHex intelHex;
    BinEncoder &encoder = intelHex.encoder();

    BinMemory memory;
    WRITE_BLOCK(memory, 0x1234, block1);
    WRITE_BLOCK(memory, 0xFFF0, block2);

    StoredPrinter out;
    encoder.reset(16);
    encoder.encode(memory, out);
    EQ("lines", 6, out.size());
    EQ("line 1", ":10123400112233445566778899AABBCCDDEEFF00B2", out.line(1));
    EQ("line 2", ":02124400123462", out.line(2));
    EQ("line 3", ":10FFF000123456789ABCDEF0FEDCBA987654321091", out.line(3));
    EQ("line 4", ":020000040001F9", out.line(4));
    EQ("line 5", ":03000000456789C8", out.line(5));
    EQ("end", ":00000001FF", out.line(6));
}

#define BLOCK_EQ(_msg, _expected, _actual, _base)                                     \
    do {                                                                              \
        asserter.equals(__FILE__, __LINE__, _msg, _expected.size(), _actual->size()); \
        uint8_t actual[_expected.size()];                                             \
        auto reader = _actual->reader();                                              \
        for (size_t i = 0; i < _expected.size() && reader.hasNext(); i++)             \
            actual[i] = reader.readByte();                                            \
        asserter.equals(__FILE__, __LINE__, _msg, _expected, actual, sizeof(actual)); \
    } while (0)

void test_decoder() {
    const uint32_t start_expected16 = 0x3400;
    const uint32_t end_expected16 = start_expected16 + sizeof(block1) - 1;
    const ArrayMemory expected16(start_expected16, block1, sizeof(block1));

    TestReader hex("test");
    hex.clear("16bit-32block1")
            .add(":12340000112233445566778899AABBCCDDEEFF0012347C")
        .add(":00000001FF");
    BinMemory mem16_32;
    EQ("16bit-32block1", 18, BinDecoder::decode(hex, mem16_32));
    EQ("16bit-32block1", start_expected16, mem16_32.startAddress());
    EQ("16bit-32block1", end_expected16, mem16_32.endAddress());
    BLOCK_EQ("16bit-32block1", expected16, mem16_32.begin(), start_expected16);

    hex.clear("16bit-8block1")
            .add(":10340000112233445566778899AABBCCDDEEFF00C4")
            .add(":02341000123474")
            .add(":00000001FF");
    BinMemory mem16_8;
    EQ("16bit-8block1", 18, BinDecoder::decode(hex, mem16_8));
    EQ("16bit-8block1", start_expected16, mem16_8.startAddress());
    EQ("16bit-8block1", end_expected16, mem16_8.endAddress());
    BLOCK_EQ("16bit-8block1", expected16, mem16_8.begin(), start_expected16);

    const uint32_t start_expected24 = 0x123400;
    const uint32_t end_expected24 = start_expected24 + sizeof(block1) - 1;
    hex.clear("24bit-16block1")
            .add(":020000040012E8")
            .add(":08340000112233445566778860")
            .add(":0834080099AABBCCDDEEFF0028")
            .add(":02341000123474")
            .add(":00000001FF");
    BinMemory mem24_16;
    EQ("24bit-16block1", 18, BinDecoder::decode(hex, mem24_16));
    EQ("24bit-16block1", start_expected24, mem24_16.startAddress());
    EQ("24bit-16block1", end_expected24, mem24_16.endAddress());
    BLOCK_EQ("24bit-32block1", expected16, mem24_16.begin(), start_expected24);

    const uint32_t start_expected32 = 0x1234FFF0;
    const uint32_t end_expected32 = start_expected32 + sizeof(block1) - 1;
    hex.clear("32bit-16block1")
            .add(":020000041234B4")
            .add(":10FFF000112233445566778899AABBCCDDEEFF0009")
            .add(":020000041235B3")
            .add(":020000001234B8")
            .add(":00000001FF");
    BinMemory mem32_16;
    EQ("32bit-16block1", 18, BinDecoder::decode(hex, mem32_16));
    EQ("32bit-16block1", start_expected32, mem32_16.startAddress());
    EQ("32bit-16block1", end_expected32, mem32_16.endAddress());
    BLOCK_EQ("32bit-32block1", expected16, mem32_16.begin(), start_expected32);
}

void test_decoder_blocks() {
    const uint32_t start_block1 = 0x1234;
    const uint32_t start_block2 = 0xFF00;
    const uint32_t end_block2 = start_block2 + sizeof(block2) - 1;
    const ArrayMemory expected1(start_block1, block1, sizeof(block1));
    const ArrayMemory expected2(start_block2, block2, sizeof(block2));
    TestReader hex("blocks");
    hex.add(":10FF0000123456789ABCDEF0FEDCBA987654321081")
            .add(":02124400123462")
            .add(":10123400112233445566778899AABBCCDDEEFF00B2")
            .add(":03FF1000456789B9")
            .add(":00000001FF");
    BinMemory mem;
    EQ("blocks", 37, BinDecoder::decode(hex, mem));
    EQ("blocks", start_block1, mem.startAddress());
    EQ("blocks", end_block2, mem.endAddress());
    BLOCK_EQ("block1", expected1, mem.begin(), start_block1);
    BLOCK_EQ("block2", expected2, mem.begin()->next(), start_block2);
}

void run_tests() {
    RUN_TEST(test_encoder);
    RUN_TEST(test_encoder_blocks);
    RUN_TEST(test_decoder);
    RUN_TEST(test_decoder_blocks);
}

}  // namespace test
}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
