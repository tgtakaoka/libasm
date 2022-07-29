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

#include "test_memory.h"
#include "test_printer.h"
#include "test_reader.h"

#include "test_driver_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_printer() {
    TestPrinter out;

    EQ("empty", 0, out.size());

    out.println("line 1");
    EQ("line 1", 1, out.size());
    EQ("line 1", "line 1", out.line(1));

    out.format("line %d", 2);
    EQ("line 2", 1, out.size());
    out.format("; newline\nline 3");
    EQ("line 2", 2, out.size());
    EQ("line 2", "line 2; newline", out.line(2));

    out.println("; append\nline 4");
    EQ("line 3 and 4", 4, out.size());
    EQ("line 3 and 4", "line 3; append", out.line(3));
    EQ("line 4", "line 4", out.line(4));
}

void test_reader() {
    TestReader empty("empty");

    EQ("empty", "empty", empty.name().c_str());
    EQ("empty", 0, empty.lineno());
    TRUE("empty", empty.readLine() == nullptr);

    TestReader in("test");
    EQ("in", "test", in.name().c_str());
    in.add("line 1").add("line 2").add("line 3");
    EQ("line 0", 0, in.lineno());
    EQ("line 1", "line 1", in.readLine()->str());
    EQ("line 1", 1, in.lineno());
    EQ("line 2", "line 2", in.readLine()->str());
    EQ("line 2", 2, in.lineno());
    EQ("line 3", "line 3", in.readLine()->str());
    EQ("line 3", 3, in.lineno());
    TRUE("eof 1", in.readLine() == nullptr);
    EQ("eof 1", 3, in.lineno());
    TRUE("eof 2", in.readLine() == nullptr);
    EQ("eof 2", 3, in.lineno());

    in.rewind();
    EQ("rewind", "test", in.name().c_str());
    EQ("rewind 0", 0, in.lineno());
    EQ("rewind 1", "line 1", in.readLine()->str());
    EQ("rewind 1", 1, in.lineno());
    EQ("rewind 2", "line 2", in.readLine()->str());
    EQ("rewind 2", 2, in.lineno());
    EQ("rewind 3", "line 3", in.readLine()->str());
    EQ("rewind 3", 3, in.lineno());
    TRUE("eof 1", in.readLine() == nullptr);
    EQ("eof 1", 3, in.lineno());
    TRUE("eof 2", in.readLine() == nullptr);
    EQ("eof 2", 3, in.lineno());

    in.clear("clear");
    EQ("clear", "clear", in.name().c_str());
    EQ("clear", 0, in.lineno());
    TRUE("clear", in.readLine() == nullptr);
}

int mem_size(const BinMemory &memory) {
    size_t size = 0;
    for (const auto &it : memory)
        size += it.data.size();
    return size;
}

int mem_block(const BinMemory &memory) {
    size_t block = 0;
    for (auto it : memory)
        block++;
    return block;
}

void test_memory() {
    TestMemory memory;

    const uint32_t start1 = 0x100;
    auto writer1 = memory.writer(start1);
    writer1.add(0x11).add(0x22).add(0x33);
    EQ("3 bytes", 3, mem_size(writer1.memory()));
    EQ("3 bytes", 1, mem_block(writer1.memory()));
    EQ("3 bytes", start1, writer1.memory().startAddress());
    EQ("3 bytes", writer1.address() - 1, writer1.memory().endAddress());
    auto reader1 = writer1.memory().reader(start1);
    EQ("3-0", 0x11, reader1.readByte());
    EQ("3-1", 0x22, reader1.readByte());
    EQ("3-2", 0x33, reader1.readByte());

    const uint32_t start2 = 0x1234;
    auto writer2 = memory.writer(start2);
    writer2.add(0xff).add(0xee);
    EQ("2 bytes", 5, mem_size(writer1.memory()));
    EQ("2 bytes", 2, mem_block(writer1.memory()));
    EQ("2 bytes", start1, writer2.memory().startAddress());
    EQ("2 bytes", writer2.address() - 1, writer2.memory().endAddress());
    auto reader2 = writer2.memory().reader(start2);
    EQ("2-0", 0xff, reader2.readByte());
    EQ("2-1", 0xee, reader2.readByte());

    const uint32_t start3 = 0x2000;
    auto writer3 = memory.writer(start3, ENDIAN_BIG);
    writer3.add(0x1234).add(0x5678);
    EQ("2 big", 9, mem_size(writer1.memory()));
    EQ("2 big", 3, mem_block(writer1.memory()));
    EQ("2 big", start1, writer2.memory().startAddress());
    EQ("2 big", writer3.address() - 1, writer3.memory().endAddress());
    auto reader3 = writer3.memory().reader(start3);
    EQ("2-0", 0x12, reader3.readByte());
    EQ("2-1", 0x34, reader3.readByte());
    EQ("2-2", 0x56, reader3.readByte());
    EQ("2-3", 0x78, reader3.readByte());

    const uint32_t start4 = writer3.address();
    auto writer4 = memory.writer(start4, ENDIAN_LITTLE);
    writer4.add(0x1234).add(0x5678);
    EQ("2 little", 13, mem_size(writer1.memory()));
    EQ("2 little", 3, mem_block(writer1.memory()));
    EQ("2 little", start1, writer2.memory().startAddress());
    EQ("2 little", writer4.address() - 1, writer4.memory().endAddress());
    auto reader4 = writer4.memory().reader(start4);
    EQ("2-0", 0x34, reader4.readByte());
    EQ("2-1", 0x12, reader4.readByte());
    EQ("2-2", 0x78, reader4.readByte());
    EQ("2-3", 0x56, reader4.readByte());
}

void run_tests() {
    RUN_TEST(test_printer);
    RUN_TEST(test_reader);
    RUN_TEST(test_memory);
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
