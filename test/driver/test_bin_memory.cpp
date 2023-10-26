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

#include "bin_memory.h"

#include "test_driver_helper.h"

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
    for (auto it : memory)
        block++;
    return block;
}

#define MEM_EQ(_mem, _msg, _start, _end, _size, _block) \
    do {                                                \
        EQ(_msg "start", _start, _mem.startAddress());  \
        EQ(_msg "end", _end, _mem.endAddress());        \
        EQ(_msg "size", _size, mem_size(_mem));         \
        EQ(_msg "block", _block, mem_block(_mem));      \
    } while (0)

#define MEM_NONE(_mem, addr)                           \
    do {                                               \
        FALSE(#addr " not exist", _mem.hasByte(addr)); \
    } while (0);

#define MEM_READ(_mem, addr, expected)                             \
    do {                                                           \
        TRUE("read " #addr " exist", _mem.hasByte(addr));          \
        EQ("read " #addr " value", expected, _mem.readByte(addr)); \
    } while (0);

#define MEM_WRITE(_mem, addr, val) \
    do {                           \
        _mem.writeByte(addr, val); \
        MEM_READ(_mem, addr, val); \
    } while (0);

void test_read_write() {
    BinMemory memory;
    MEM_EQ(memory, "empty", 0, 0, 0, 0);

    MEM_NONE(memory, 0x100);
    MEM_WRITE(memory, 0x100, 0x12);
    MEM_EQ(memory, "one byte", 0x100, 0x100, 1, 1);

    MEM_WRITE(memory, 0x102, 0x34);
    MEM_EQ(memory, "another block", 0x100, 0x102, 2, 2);

    MEM_WRITE(memory, 0x101, 0x56);
    MEM_EQ(memory, "merge block", 0x100, 0x102, 3, 1);

    MEM_WRITE(memory, 0x080, 0x77);
    MEM_EQ(memory, "block before", 0x080, 0x102, 4, 2);

    MEM_WRITE(memory, 0x07F, 0x70);
    MEM_EQ(memory, "byte before", 0x07F, 0x102, 5, 2);

    MEM_WRITE(memory, 0x180, 0x88);
    MEM_EQ(memory, "block after", 0x07F, 0x180, 6, 3);

    MEM_WRITE(memory, 0x181, 0x99);
    MEM_EQ(memory, "byte after", 0x07F, 0x181, 7, 3);

    MEM_WRITE(memory, 0x100, 0xFF);
    MEM_EQ(memory, "overwrite head", 0x07F, 0x181, 7, 3);

    MEM_WRITE(memory, 0x101, 0xFF);
    MEM_EQ(memory, "overwrite mid", 0x07F, 0x181, 7, 3);

    MEM_WRITE(memory, 0x102, 0xFF);
    MEM_EQ(memory, "overwrite tail", 0x07F, 0x181, 7, 3);
}

void test_eq() {
    BinMemory mem1;
    BinMemory mem2;

    TRUE("empty memory", mem1 == mem2);
    TRUE("empty memory", mem2 == mem1);

    MEM_WRITE(mem1, 0x100, 0x01);
    TRUE("1:0", mem1 != mem2);
    TRUE("1:0 ", mem2 != mem1);

    MEM_WRITE(mem2, 0x100, 0x01);
    TRUE("1:1", mem1 == mem2);
    TRUE("1:1", mem2 == mem1);
}

void test_swap() {
    BinMemory old;
    MEM_WRITE(old, 0x0100, 0x01);
    MEM_WRITE(old, 0x0101, 0x02);
    MEM_WRITE(old, 0x0200, 0x03);
    MEM_WRITE(old, 0x01FF, 0x04);
    MEM_EQ(old, "4 bytes", 0x100, 0x200, 4, 2);

    BinMemory memory;
    MEM_WRITE(memory, 0x0101, 0xF1);
    MEM_WRITE(memory, 0x0200, 0xF2);
    MEM_WRITE(memory, 0x0400, 0xF3);
    MEM_EQ(memory, "3 bytes", 0x101, 0x400, 3, 3);

    memory.swap(old);
    MEM_EQ(memory, "new 4 bytes", 0x100, 0x200, 4, 2);
    MEM_READ(memory, 0x100, 0x01);
    MEM_READ(memory, 0x101, 0x02);
    MEM_READ(memory, 0x1FF, 0x04);
    MEM_READ(memory, 0x200, 0x03);
    MEM_EQ(old, "old empty", 0x101, 0x400, 3, 3);
    MEM_READ(old, 0x0101, 0xF1);
    MEM_NONE(old, 0x01FF);
    MEM_READ(old, 0x0200, 0xF2);
    MEM_READ(old, 0x0400, 0xF3);
}

void test_dis_memory() {
    BinMemory memory;
    MEM_WRITE(memory, 0x0100, 0xF0);
    MEM_WRITE(memory, 0x0101, 0xF1);
    MEM_WRITE(memory, 0x0200, 0xF2);
    MEM_WRITE(memory, 0x0400, 0xF3);

    auto reader = memory.reader(0x0080);
    EQ("0x0080", 0x0080, reader.address());
    FALSE("0x0080", reader.hasNext());

    reader.setAddress(0x0100);
    EQ("0x0100", 0x0100, reader.address());
    TRUE("0x0100", reader.hasNext());
    EQ("at 0x0100", 0xF0, reader.readByte());
    EQ("0x0101", 0x0101, reader.address());
    TRUE("0x0101", reader.hasNext());
    EQ("at 0x0101", 0xF1, reader.readByte());
    EQ("0x0102", 0x0102, reader.address());
    FALSE("0x0102", reader.hasNext());
}

void run_tests() {
    RUN_TEST(test_read_write);
    RUN_TEST(test_eq);
    RUN_TEST(test_swap);
    RUN_TEST(test_dis_memory);
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
