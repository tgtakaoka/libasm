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

#include "array_memory.h"
#include "test_unit_helper.h"

namespace libasm {
namespace test {

void set_up() {}

void tear_down() {}

void test_byte() {
    static const uint8_t block[] = {0x11, 0x22, 0x33, 0x44, 0x55};
    constexpr uint32_t origin = 0x0100;
    constexpr uint32_t end = origin + sizeof(block) - 1;
    const ArrayMemory mem(origin, block, sizeof(block));

    EQ("mem.size", sizeof(block), mem.size());
    EQ("mem.origin", origin, mem.origin());
    EQ("mem.end", end, mem.end());
    EQ("mem[origin-1]", 0, mem.byteAt(origin - 1));
    EQ("mem[origin+0]", 0x11, mem.byteAt(origin));
    EQ("mem[origin+1]", 0x22, mem.byteAt(origin + 1));
    EQ("mem[origin+2]", 0x33, mem.byteAt(origin + 2));
    EQ("mem[origin+3]", 0x44, mem.byteAt(origin + 3));
    EQ("mem[end]", 0x55, mem.byteAt(end));
    EQ("mem[end+1]", 0, mem.byteAt(end + 1));

    auto big = mem.iterator(ENDIAN_BIG);
    EQ("big-0", 0x11, big.readByte());
    EQ("big-1", origin + 1, big.address());
    EQ("big-1", 0x22, big.readByte());
    TRUE("big-1", big.hasNext());
    EQ("big-2", 0x33, big.readByte());
    EQ("big-3", 0x4455, big.readUint16());
    FALSE("big-2", big.hasNext());

    big.rewind();
    EQ("big-r", origin, big.address());
    TRUE("big-r", big.hasNext());
    EQ("big-r0", 0x11, big.readByte());
    EQ("big-r1", origin + 1, big.address());

    auto little = mem.iterator(ENDIAN_LITTLE);
    EQ("little-0", 0x2211, little.readUint16());
    EQ("little-2", origin + 2, little.address());
    TRUE("little-2", little.hasNext());
    EQ("little-2", 0x4433, little.readUint16());
    EQ("little-4", origin + 4, little.address());
    EQ("little-5", 0x55, little.readByte());
    FALSE("little-2", little.hasNext());
}

void test_word_big() {
    static const uint16_t block[] = {0x1122, 0x3344, 0x5566};
    constexpr uint32_t origin = 0x10000000;
    constexpr uint32_t end = origin + sizeof(block) - 1;
    const ArrayMemory mem(origin, block, sizeof(block), ENDIAN_BIG, ADDRESS_BYTE);

    EQ("mem.size", sizeof(block), mem.size());
    EQ("mem.origin", origin, mem.origin());
    EQ("mem.end", end, mem.end());
    EQ("mem[origin-1]", 0, mem.byteAt(origin - 1));
    EQ("mem[origin+0]", 0x11, mem.byteAt(origin));
    EQ("mem[origin+1]", 0x22, mem.byteAt(origin + 1));
    EQ("mem[origin+2]", 0x33, mem.byteAt(origin + 2));
    EQ("mem[origin+3]", 0x44, mem.byteAt(origin + 3));
    EQ("mem[origin+4]", 0x55, mem.byteAt(origin + 4));
    EQ("mem[end]", 0x66, mem.byteAt(end));
    EQ("mem[end+1]", 0, mem.byteAt(end + 1));

    auto big = mem.iterator(ENDIAN_BIG);
    EQ("big-0", 0x11, big.readByte());
    EQ("big-1", origin + 1, big.address());
    EQ("big-1", 0x22, big.readByte());
    TRUE("big-1", big.hasNext());
    EQ("big-2", 0x33, big.readByte());
    EQ("big-3", 0x4455, big.readUint16());
    EQ("big-4", 0x66, big.readByte());
    FALSE("big-2", big.hasNext());

    big.rewind();
    EQ("big-r", origin, big.address());
    TRUE("big-r", big.hasNext());
    EQ("big-r0", 0x1122, big.readUint16());
    EQ("big-r2", origin + 2, big.address());

    auto little = mem.iterator(ENDIAN_LITTLE);
    EQ("little-0", 0x2211, little.readUint16());
    EQ("little-2", origin + 2, little.address());
    TRUE("little-2", little.hasNext());
    EQ("little-2", 0x33, little.readByte());
    EQ("little-3", 0x5544, little.readUint16());
    EQ("little-4", 0x66, little.readByte());
    FALSE("little-2", little.hasNext());
}

void test_word_little() {
    static const uint16_t block[] = {0x1122, 0x3344, 0x5566};
    constexpr uint32_t origin = 0x10000000;
    constexpr uint32_t end = origin + sizeof(block) - 1;
    const ArrayMemory mem(origin, block, sizeof(block), ENDIAN_LITTLE, ADDRESS_BYTE);

    EQ("mem.size", sizeof(block), mem.size());
    EQ("mem.origin", origin, mem.origin());
    EQ("mem.end", end, mem.end());
    EQ("mem[origin-1]", 0, mem.byteAt(origin - 1));
    EQ("mem[origin+0]", 0x22, mem.byteAt(origin));
    EQ("mem[origin+1]", 0x11, mem.byteAt(origin + 1));
    EQ("mem[origin+2]", 0x44, mem.byteAt(origin + 2));
    EQ("mem[origin+3]", 0x33, mem.byteAt(origin + 3));
    EQ("mem[origin+4]", 0x66, mem.byteAt(origin + 4));
    EQ("mem[end]", 0x55, mem.byteAt(end));
    EQ("mem[end+1]", 0, mem.byteAt(end + 1));

    auto big = mem.iterator(ENDIAN_BIG);
    EQ("big-0", 0x22, big.readByte());
    EQ("big-1", origin + 1, big.address());
    EQ("big-1", 0x11, big.readByte());
    TRUE("big-1", big.hasNext());
    EQ("big-2", 0x44, big.readByte());
    EQ("big-3", 0x3366, big.readUint16());
    EQ("big-4", 0x55, big.readByte());
    FALSE("big-2", big.hasNext());

    big.rewind();
    EQ("big-r", origin, big.address());
    TRUE("big-r", big.hasNext());
    EQ("big-r0", 0x2211, big.readUint16());
    EQ("big-r2", origin + 2, big.address());

    auto little = mem.iterator(ENDIAN_LITTLE);
    EQ("little-0", 0x1122, little.readUint16());
    EQ("little-2", origin + 2, little.address());
    TRUE("little-2", little.hasNext());
    EQ("little-2", 0x44, little.readByte());
    EQ("little-3", 0x6633, little.readUint16());
    EQ("little-4", 0x55, little.readByte());
    FALSE("little-2", little.hasNext());
}

void test_word_addr_big() {
    static const uint16_t block[] = {0x1122, 0x3344, 0x5566};
    constexpr auto origin = UINT32_C(0x10000000);
    constexpr auto end = origin + sizeof(block) / 2 - 1;
    const ArrayMemory mem(origin, block, sizeof(block), ENDIAN_BIG, ADDRESS_WORD);

    EQ("mem", sizeof(block), mem.size());
    EQ("mem", origin, mem.origin());
    EQ("mem", end, mem.end());
    auto iter = mem.iterator();

    EQ("mem[0]", 0x11, iter.readByte());
    EQ("mem[1]", 0x22, iter.readByte());
    EQ("mem[2]", 0x33, iter.readByte());
    EQ("mem[3]", 0x44, iter.readByte());
    EQ("mem[4]", 0x55, iter.readByte());
    EQ("mem[5]", 0x66, iter.readByte());

    auto big = mem.iterator(ENDIAN_BIG);
    EQ("big-0", origin, big.address() / ADDRESS_WORD);
    EQ("big-0", 0x1122, big.readUint16());
    EQ("big-2", 0x3344, big.readUint16());
    EQ("big-4", end, big.address() / ADDRESS_WORD);
    EQ("big-4", 0x5566, big.readUint16());
    FALSE("big-6", big.hasNext());

    big.rewind();
    EQ("big-r", origin, big.address() / ADDRESS_WORD);
    TRUE("big-r", big.hasNext());
    EQ("big-r0", 0x1122, big.readUint16());
    EQ("big-r2", origin + 1, big.address() / ADDRESS_WORD);

    auto little = mem.iterator(ENDIAN_LITTLE);
    EQ("little-0", origin, little.address() / ADDRESS_WORD);
    EQ("little-0", 0x2211, little.readUint16());
    EQ("little-2", 0x4433, little.readUint16());
    EQ("little-4", end, little.address() / ADDRESS_WORD);
    EQ("little-4", 0x6655, little.readUint16());
    FALSE("little-6", little.hasNext());
}

void test_word_addr_little() {
    static const uint16_t block[] = {0x1122, 0x3344, 0x5566};
    constexpr auto origin = UINT32_C(0x10000000);
    constexpr auto end = origin + sizeof(block) / 2 - 1;
    const ArrayMemory mem(origin, block, sizeof(block), ENDIAN_LITTLE, ADDRESS_WORD);

    EQ("mem", sizeof(block), mem.size());
    EQ("mem", origin, mem.origin());
    EQ("mem", end, mem.end());
    auto iter = mem.iterator();

    EQ("mem[0]", 0x22, iter.readByte());
    EQ("mem[1]", 0x11, iter.readByte());
    EQ("mem[2]", 0x44, iter.readByte());
    EQ("mem[3]", 0x33, iter.readByte());
    EQ("mem[4]", 0x66, iter.readByte());
    EQ("mem[5]", 0x55, iter.readByte());

    auto big = mem.iterator(ENDIAN_BIG);
    EQ("big-0", origin, big.address() / ADDRESS_WORD);
    EQ("big-0", 0x2211, big.readUint16());
    EQ("big-2", 0x4433, big.readUint16());
    EQ("big-4", end, big.address() / ADDRESS_WORD);
    EQ("big-4", 0x6655, big.readUint16());
    FALSE("big-6", big.hasNext());

    big.rewind();
    EQ("big-r", origin, big.address() / ADDRESS_WORD);
    TRUE("big-r", big.hasNext());
    EQ("big-r0", 0x2211, big.readUint16());
    EQ("big-r2", origin + 1, big.address() / ADDRESS_WORD);

    auto little = mem.iterator(ENDIAN_LITTLE);
    EQ("little-0", origin, little.address() / ADDRESS_WORD);
    EQ("little-0", 0x1122, little.readUint16());
    EQ("little-2", 0x3344, little.readUint16());
    EQ("little-4", end, little.address() / ADDRESS_WORD);
    EQ("little-4", 0x5566, little.readUint16());
    FALSE("little-6", little.hasNext());
}

void run_tests() {
    RUN_TEST(test_byte);
    RUN_TEST(test_word_big);
    RUN_TEST(test_word_little);
    RUN_TEST(test_word_addr_big);
    RUN_TEST(test_word_addr_little);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
