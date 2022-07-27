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

#include "moto_srec.h"

#include "bin_memory.h"
#include "test_driver_helper.h"
#include "test_printer.h"
#include "test_reader.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

int mem_size(BinMemory &memory) {
    size_t size = 0;
    for (const auto &it : memory)
        size += it.data.size();
    return size;
}

int mem_block(BinMemory &memory) {
    size_t block = 0;
    for (const auto it : memory)
        block++;
    return block;
}

// clang-format off
static const uint8_t block1[] = {
    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00,
    0x12, 0x34,
};
static const uint8_t block2[] = {
    0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
    0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
    0x45, 0x67, 0x89,
};
// clang-format on

static void writeBlock(BinMemory &memory, uint32_t base, const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++)
        memory.writeByte(base + i, data[i]);
}
#define WRITE_BLOCK(_mem, _base, _array) writeBlock(_mem, _base, _array, sizeof(_array))

void test_encoder() {
    MotoSrec motoSrec;
    BinEncoder &encoder = motoSrec.encoder();

    BinMemory memory;
    WRITE_BLOCK(memory, 0x123400, block1);

    TestPrinter out;
    encoder.reset(ADDRESS_16BIT, 32);
    encoder.encode(memory, out);
    EQ("lines", 3, out.size());
    EQ("start", "S0030000FC", out.line(1));
    EQ("line 1", "S1153400112233445566778899AABBCCDDEEFF00123478", out.line(2));
    EQ("end", "S9030000FC", out.line(3));

    out.clear();
    encoder.reset(ADDRESS_16BIT, 16);
    encoder.encode(memory, out);
    EQ("lines", 4, out.size());
    EQ("start", "S0030000FC", out.line(1));
    EQ("line 1", "S1133400112233445566778899AABBCCDDEEFF00C0", out.line(2));
    EQ("line 2", "S1053410123470", out.line(3));
    EQ("end", "S9030000FC", out.line(4));

    out.clear();
    encoder.reset(ADDRESS_24BIT, 8);
    encoder.encode(memory, out);
    EQ("lines", 5, out.size());
    EQ("start", "S0030000FC", out.line(1));
    EQ("line 1", "S20C123400112233445566778849", out.line(2));
    EQ("line 2", "S20C12340899AABBCCDDEEFF0011", out.line(3));
    EQ("line 3", "S20612341012345D", out.line(4));
    EQ("end", "S804000000FB", out.line(5));

    BinMemory interseg;
    WRITE_BLOCK(interseg, 0x1234FFF0, block1);
    out.clear();
    encoder.reset(ADDRESS_32BIT, 16);
    encoder.encode(interseg, out);
    EQ("lines", 4, out.size());
    EQ("start", "S0030000FC", out.line(1));
    EQ("line 1", "S3151234FFF0112233445566778899AABBCCDDEEFF00BD", out.line(2));
    EQ("line 2", "S3071235000012346B", out.line(3));
    EQ("end", "S70500000000FA", out.line(4));
}

void test_encoder_blocks() {
    MotoSrec motoSrec;
    BinEncoder &encoder = motoSrec.encoder();

    BinMemory memory;
    WRITE_BLOCK(memory, 0x1234, block1);
    WRITE_BLOCK(memory, 0xFF00, block2);

    TestPrinter out;
    encoder.reset(ADDRESS_16BIT, 16);
    encoder.encode(memory, out);
    EQ("lines", 6, out.size());
    EQ("start", "S0030000FC", out.line(1));
    EQ("line 1", "S1131234112233445566778899AABBCCDDEEFF00AE", out.line(2));
    EQ("line 2", "S105124412345E", out.line(3));
    EQ("line 3", "S113FF00123456789ABCDEF0FEDCBA98765432107D", out.line(4));
    EQ("line 4", "S106FF10456789B5", out.line(5));
    EQ("end", "S9030000FC", out.line(6));
}

static void readBlock(const BinMemory &memory, uint32_t base, uint8_t *data, size_t size) {
    auto reader = memory.reader(base);
    for (size_t i = 0; i < size && reader.hasNext(); i++)
        data[i] = reader.readByte();
}

#define READ_BLOCK(_mem, _base, _buffer) readBlock(_mem, _base, _buffer, sizeof(_buffer))

#define MEM_EQ(_msg, _expected, _actual, _base)                                       \
    do {                                                                              \
        uint8_t actual[_expected.size()];                                             \
        READ_BLOCK(_actual, _base, actual);                                           \
        asserter.equals(__FILE__, __LINE__, _msg, _expected, actual, sizeof(actual)); \
    } while (0)

void test_decoder() {
    const uint32_t start_expected16 = 0x3400;
    const uint32_t end_expected16 = start_expected16 + sizeof(block1) - 1;
    const ArrayMemory expected16(start_expected16, block1, sizeof(block1));

    TestReader hex("test");
    hex.clear("16bit-32block1")
            .add("S0030000FC")
            .add("S1153400112233445566778899AABBCCDDEEFF00123478")
            .add("S9030000FC");
    BinMemory mem16_32;
    EQ("16bit-32block1", 0, BinDecoder::decode(hex, mem16_32));
    EQ("16bit-32block1", start_expected16, mem16_32.startAddress());
    EQ("16bit-32block1", end_expected16, mem16_32.endAddress());
    MEM_EQ("16bit-32block1", expected16, mem16_32, start_expected16);

    hex.clear("16bit-8block1")
            .add("S0030000FC")
            .add("S1133400112233445566778899AABBCCDDEEFF00C0")
            .add("S1053410123470")
            .add("S9030000FC");
    BinMemory mem16_8;
    EQ("16bit-8block1", 0, BinDecoder::decode(hex, mem16_8));
    EQ("16bit-8block1", start_expected16, mem16_8.startAddress());
    EQ("16bit-8block1", end_expected16, mem16_8.endAddress());
    MEM_EQ("16bit-8block1", expected16, mem16_8, start_expected16);

    const uint32_t start_expected24 = 0x123400;
    const uint32_t end_expected24 = start_expected24 + sizeof(block1) - 1;
    hex.clear("24bit-16block1")
        .add("S0030000FC")
        .add("S20C123400112233445566778849")
        .add("S20C12340899AABBCCDDEEFF0011")
        .add("S20612341012345D")
        .add("S804000000FB");
    BinMemory mem24_16;
    EQ("24bit-16block1", 0, BinDecoder::decode(hex, mem24_16));
    EQ("24bit-16block1", start_expected24, mem24_16.startAddress());
    EQ("24bit-16block1", end_expected24, mem24_16.endAddress());
    MEM_EQ("24bit-32block1", expected16, mem24_16, start_expected24);

    const uint32_t start_expected32 = 0x1234FFF0;
    const uint32_t end_expected32 = start_expected32 + sizeof(block1) - 1;
    hex.clear("32bit-16block1")
        .add("S0030000FC")
        .add("S3151234FFF0112233445566778899AABBCCDDEEFF00BD")
        .add("S3071235000012346B")
        .add("S70500000000FA");
    BinMemory mem32_16;
    EQ("32bit-16block1", 0, BinDecoder::decode(hex, mem32_16));
    EQ("32bit-16block1", start_expected32, mem32_16.startAddress());
    EQ("32bit-16block1", end_expected32, mem32_16.endAddress());
    MEM_EQ("32bit-32block1", expected16, mem32_16, start_expected32);
}

void test_decoder_blocks() {
    const uint32_t start_block1 = 0x1234;
    const uint32_t start_block2 = 0xFF00;
    const uint32_t end_block2 = start_block2 + sizeof(block2) - 1;
    const ArrayMemory expected1(start_block1, block1, sizeof(block1));
    const ArrayMemory expected2(start_block2, block2, sizeof(block2));
    TestReader hex("blocks");
    hex
        .add("S0030000FC")
        .add("S113FF00123456789ABCDEF0FEDCBA98765432107D")
        .add("S105124412345E")
        .add("S106FF10456789B5")
        .add("S1131234112233445566778899AABBCCDDEEFF00AE")
        .add("S9030000FC");
    BinMemory mem;
    EQ("blocks", 0, BinDecoder::decode(hex, mem));
    EQ("blocks", start_block1, mem.startAddress());
    EQ("blocks", end_block2, mem.endAddress());
    MEM_EQ("block1", expected1, mem, start_block1);
    MEM_EQ("block2", expected2, mem, start_block2);
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
