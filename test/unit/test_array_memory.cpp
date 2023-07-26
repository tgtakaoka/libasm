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

#include "test_src_helper.h"

namespace libasm {
namespace test {

void set_up() {}

void tear_down() {}

void test_byte_memory() {
    static const uint8_t block[] = {0x11, 0x22, 0x33};
    constexpr uint32_t start = 0x0100;
    constexpr uint32_t end = start + sizeof(block) - 1;
    const ArrayMemory mem(start, block, sizeof(block));

    EQ("mem.start", sizeof(block), mem.size());
    EQ("mem.end", end, mem.end());
    EQ("mem[start-1]", 0, mem.byteAt(start - 1));
    EQ("mem[start]", 0x11, mem.byteAt(start));
    EQ("mem[1]", 0x22, mem.byteAt(start + 1));
    EQ("mem[end]", 0x33, mem.byteAt(end));
    EQ("mem[end+1]", 0, mem.byteAt(end + 1));

    auto it1 = mem.iterator();
    auto it2 = mem.iterator();
    EQ("it1-0", mem.byteAt(start), it1.readByte());
    EQ("it1-1", start + 1, it1.address());
    EQ("it1-1", mem.byteAt(start + 1), it1.readByte());
    TRUE("it1-1", it1.hasNext());
    EQ("it2-0", mem.byteAt(start), it2.readByte());
    EQ("it2-1", mem.byteAt(start + 1), it2.readByte());
    TRUE("it2-1", it2.hasNext());
    EQ("it2-2", mem.byteAt(start + 2), it2.readByte());
    EQ("it2-3", end + 1, it2.address());
    EQ("it1-2", mem.byteAt(start + 2), it1.readByte());
    FALSE("it1-2", it1.hasNext());
    FALSE("it2-2", it2.hasNext());

    it1.rewind();
    EQ("it1-r", start, it1.address());
    TRUE("it1-r", it1.hasNext());
    EQ("it1-r0", mem.byteAt(start), it1.readByte());
    EQ("it1-r1", start + 1, it1.address());
}

void test_word_memory() {
    static const uint16_t block[] = {0x1122, 0x3344, 0x5566};
    constexpr uint32_t start = 0x10000000;
    constexpr uint32_t end = start + sizeof(block) - 1;
    const ArrayMemory big(start, block, sizeof(block), ENDIAN_BIG);
    const ArrayMemory little(start, block, sizeof(block), ENDIAN_LITTLE);

    EQ("big", sizeof(block), big.size());
    EQ("big", start, big.origin());
    EQ("big", end, big.end());
    auto b = big.iterator();
    EQ("b", start, b.address());
    EQ("b0", 0x11, b.readByte());
    EQ("b1", start + 1, b.address());
    EQ("b1", 0x22, b.readByte());
    EQ("b2", 0x33, b.readByte());
    EQ("b3", 0x44, b.readByte());
    EQ("b4", 0x55, b.readByte());
    EQ("b5", 0x66, b.readByte());
    EQ("b6", end + 1, b.address());
    FALSE("b6", b.hasNext());

    EQ("little", sizeof(block), little.size());
    EQ("little", start, little.origin());
    EQ("little", end, little.end());
    auto l = little.iterator();
    EQ("l", start, l.address());
    EQ("l0", 0x22, l.readByte());
    EQ("l1", start + 1, l.address());
    EQ("l1", 0x11, l.readByte());
    EQ("l2", 0x44, l.readByte());
    EQ("l3", 0x33, l.readByte());
    EQ("l4", 0x66, l.readByte());
    EQ("l5", 0x55, l.readByte());
    EQ("l6", end + 1, l.address());
    FALSE("l6", l.hasNext());
}

void run_tests() {
    RUN_TEST(test_byte_memory);
    RUN_TEST(test_word_memory);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
