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

#define ASSERT(_mem, _msg, _start, _end, _size, _block) \
    do {                                                \
        EQ(_msg " start", _start, _mem.startAddress()); \
        EQ(_msg " end", _end, _mem.endAddress());       \
        EQ(_msg " size", _size, _mem.size());           \
        EQ(_msg " block", _block, _mem.blocks());       \
    } while (0)

#define READ(_mem, addr, expected)                                 \
    do {                                                           \
        TRUE("read " #addr " exist", _mem.hasByte(addr));          \
        EQ("read " #addr " value", expected, _mem.readByte(addr)); \
    } while (0);

#define WRITE(_mem, addr, val)     \
    do {                           \
        _mem.writeByte(addr, val); \
        READ(_mem, addr, val);     \
    } while (0);

void test_read_write() {
    BinMemory memory;
    ASSERT(memory, "empty", 0, 0, 0, 0);

    FALSE("empty", memory.hasByte(0x100));
    WRITE(memory, 0x100, 0x12);
    ASSERT(memory, "one byte", 0x100, 0x100, 1, 1);

    WRITE(memory, 0x102, 0x34);
    ASSERT(memory, "another block", 0x100, 0x102, 2, 2);

    WRITE(memory, 0x101, 0x56);
    ASSERT(memory, "merge block", 0x100, 0x102, 3, 1);

    WRITE(memory, 0x080, 0x77);
    ASSERT(memory, "block before", 0x080, 0x102, 4, 2);

    WRITE(memory, 0x07F, 0x70);
    ASSERT(memory, "byte before", 0x07F, 0x102, 5, 2);

    WRITE(memory, 0x180, 0x88);
    ASSERT(memory, "block after", 0x07F, 0x180, 6, 3);

    WRITE(memory, 0x181, 0x99);
    ASSERT(memory, "byte after", 0x07F, 0x181, 7, 3);

    WRITE(memory, 0x100, 0xFF);
    ASSERT(memory, "overwrite head", 0x07F, 0x181, 7, 3);

    WRITE(memory, 0x101, 0xFF);
    ASSERT(memory, "overwrite mid", 0x07F, 0x181, 7, 3);

    WRITE(memory, 0x102, 0xFF);
    ASSERT(memory, "overwrite tail", 0x07F, 0x181, 7, 3);

    memory.clear();
    ASSERT(memory, "clear", 0x000, 0x000, 0, 0);
}

void test_equal() {
    BinMemory mem1;
    BinMemory mem2;

    TRUE("empty mem1", mem1 == mem2);
    TRUE("empty mem2", mem2 == mem1);

    WRITE(mem1, 0x100, 0x01);
    TRUE("1:0", mem1 != mem2);
    TRUE("1:0", mem2 != mem1);

    WRITE(mem2, 0x100, 0x01);
    TRUE("1:1", mem1 == mem2);
    TRUE("1:1", mem2 == mem1);

    WRITE(mem1, 0x200, 0x02);
    TRUE("2:1", mem1 != mem2);
    TRUE("2:1", mem2 != mem1);

    WRITE(mem2, 0x300, 0x03);
    TRUE("2:2", mem1 != mem2);
    TRUE("2:2", mem2 != mem1);

    WRITE(mem1, 0x300, 0x03);
    WRITE(mem2, 0x200, 0x02);
    TRUE("3:3", mem1 == mem2);
    TRUE("3:3", mem2 == mem1);

    WRITE(mem1, 0x201, 0x21);
    WRITE(mem2, 0x201, 0x22);
    TRUE("4:4", mem1 != mem2);
    TRUE("4:4", mem2 != mem1);

    WRITE(mem2, 0x201, 0x21);
    TRUE("4:4", mem1 == mem2);
    TRUE("4:4", mem2 == mem1);
}

void test_swap() {
    BinMemory old;
    WRITE(old, 0x0100, 0x01);
    WRITE(old, 0x0101, 0x02);
    WRITE(old, 0x0200, 0x03);
    WRITE(old, 0x01FF, 0x04);
    ASSERT(old, "4 bytes", 0x100, 0x200, 4, 2);

    BinMemory memory;
    WRITE(memory, 0x0101, 0xF1);
    WRITE(memory, 0x0200, 0xF2);
    WRITE(memory, 0x0400, 0xF3);
    ASSERT(memory, "3 bytes", 0x101, 0x400, 3, 3);

    memory.swap(old);
    ASSERT(old, "old swapped", 0x101, 0x400, 3, 3);
    READ(old, 0x0101, 0xF1);
    FALSE("old empty", old.hasByte(0x01FF));
    READ(old, 0x0200, 0xF2);
    READ(old, 0x0400, 0xF3);
    ASSERT(memory, "memory swapped", 0x100, 0x200, 4, 2);
    READ(memory, 0x100, 0x01);
    READ(memory, 0x101, 0x02);
    READ(memory, 0x1FF, 0x04);
    READ(memory, 0x200, 0x03);
}

void test_reader() {
    BinMemory memory;
    WRITE(memory, 0x0100, 0xF0);
    WRITE(memory, 0x0101, 0xF1);
    WRITE(memory, 0x0200, 0xF2);
    WRITE(memory, 0x0400, 0xF3);

    auto reader = memory.begin()->reader();
    EQ("0x0100", 0x0100, reader.address());
    TRUE("0x0100", reader.hasNext());
    EQ("at 0x0100", 0xF0, reader.readByte());
    EQ("0x0101", 0x0101, reader.address());
    TRUE("0x0101", reader.hasNext());
    EQ("at 0x0101", 0xF1, reader.readByte());
    EQ("0x0102", 0x0102, reader.address());
    FALSE("0x0102", reader.hasNext());
}

void test_range() {
    BinMemory memory;
    // block 1
    WRITE(memory, 0x0100, 0xF0);
    // block2
    WRITE(memory, 0x0200, 0xF1);
    WRITE(memory, 0x0201, 0xF2);
    WRITE(memory, 0x0202, 0xF3);
    // block 3
    WRITE(memory, 0x0300, 0xF4);
    WRITE(memory, 0x0301, 0xF5);
    WRITE(memory, 0x0302, 0xF6);
    WRITE(memory, 0x0303, 0xF7);
    // block 4
    WRITE(memory, 0x0400, 0xF8);
    WRITE(memory, 0x0401, 0xF9);
    WRITE(memory, 0x0402, 0xFA);
    // block 5
    WRITE(memory, 0x0500, 0xFB);
    ASSERT(memory, "5 blocks", 0x0100, 0x0500, 12, 5);

    memory.setRange(0x0100, 0x0500);
    ASSERT(memory, "intact", 0x0100, 0x0500, 12, 5);

    memory.setRange(0x0180, 0x0480);
    ASSERT(memory, "3 blocks", 0x0200, 0x0402, 10, 3);

    memory.setRange(0x0201, 0x0401);
    ASSERT(memory, "3 blocks", 0x0201, 0x0401, 8, 3);
    auto reader = memory.begin()->reader();
    EQ("block 2", 0x201, reader.address());
    EQ("block 2", 2, reader.size());
    EQ("block 2-1", 0xF2, reader.readByte());
    EQ("block 2-2", 0xF3, reader.readByte());
    FALSE("block 2", reader.hasNext());
    reader = memory.begin()->next()->next()->reader();
    EQ("block 4", 0x400, reader.address());
    EQ("block 4", 2, reader.size());
    EQ("block 4-0", 0xF8, reader.readByte());
    EQ("block 4-1", 0xF9, reader.readByte());
    FALSE("block 4", reader.hasNext());

    memory.setRange(0x0280, 0x0380);
    ASSERT(memory, "1 block", 0x0300, 0x0303, 4, 1);

    memory.setRange(0x0301, 0x0302);
    ASSERT(memory, "1 block", 0x0301, 0x0302, 2, 1);
    reader = memory.begin()->reader();
    EQ("block 3", 0x301, reader.address());
    EQ("block 3", 2, reader.size());
    EQ("block 3-1", 0xF5, reader.readByte());
    EQ("block 3-2", 0xF6, reader.readByte());
    FALSE("block 3", reader.hasNext());
}

void run_tests() {
    RUN_TEST(test_read_write);
    RUN_TEST(test_equal);
    RUN_TEST(test_swap);
    RUN_TEST(test_reader);
    RUN_TEST(test_range);
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
