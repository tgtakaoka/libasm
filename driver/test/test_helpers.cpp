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

void run_tests() {
    RUN_TEST(test_printer);
    RUN_TEST(test_reader);
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
