/*
 * Copyright 2020 Tadashi G. Takaoka
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

#include "test_expr_helper.h"

using namespace libasm;
using namespace libasm::test;

AsmIntelOperand parser;
TestSymtab symtab;
TestAsserter asserter;

static void set_up() {
}

static void tear_down() {
    symtab.reset();
}

static void test_hex_constant() {
    E8("0H",   0x00, OK);
    E8("7fH",  0x7f, OK);
    E8("80H",  0x80, OK);
    E8("0FFH", 0xff, OK);
    E8("100H", 0,    OVERFLOW_RANGE);

    E16("0H",     0x0000, OK);
    E16("7FFFH",  0x7fff, OK);
    E16("8000H",  0x8000, OK);
    E16("0ffffh", 0xffff, OK);
    E16("10000H", 0,      OVERFLOW_RANGE);

    E32("0H",         0x00000000, OK);
    E32("7FFFFFFFH",  0x7fffffff, OK);
    E32("80000000H",  0x80000000, OK);
    E32("0FFFFffffH", 0xffffffff, OK);
    E32("100000000H", 0,          OVERFLOW_RANGE);
}

static void test_oct_constant() {
    E8("0O",   0x00, OK);
    E8("177O", 0x7f, OK);
    E8("200O", 0x80, OK);
    E8("377O", 0xff, OK);
    E8("400O", 0,    OVERFLOW_RANGE);

    E16("0O",      0x0000, OK);
    E16("77777O",  0x7fff, OK);
    E16("100000O", 0x8000, OK);
    E16("177777o", 0xffff, OK);
    E16("200000O", 0,      OVERFLOW_RANGE);

    E32("0O",           0x00000000, OK);
    E32("17777777777O", 0x7fffffff, OK);
    E32("20000000000o", 0x80000000, OK);
    E32("37777777777O", 0xffffffff, OK);
    E32("40000000000O", 0,          OVERFLOW_RANGE);
}

static void test_bin_constant() {
    E8("0B",         0x00, OK);
    E8("01111111b",  0x7f, OK);
    E8("10000000B",  0x80, OK);
    E8("11111111B",  0xff, OK);
    E8("100000000B", 0,    OVERFLOW_RANGE);

    E16("0B",                 0x0000, OK);
    E16("0111111111111111B",  0x7fff, OK);
    E16("1000000000000000B",  0x8000, OK);
    E16("1111111111111111b",  0xffff, OK);
    E16("10000000000000000B", 0,      OVERFLOW_RANGE);


    E32("0B",                                 0x00000000, OK);
    E32("01111111111111111111111111111111B",  0x7fffffff, OK);
    E32("10000000000000000000000000000000B",  0x80000000, OK);
    E32("11111111111111111111111111111111B",  0xffffffff, OK);
    E32("100000000000000000000000000000000b", 0,          OVERFLOW_RANGE);
}

static void test_current_address() {
    symtab.setCurrentOrigin(0x1000);
    E16("$",        0x1000, OK);
    E16("$+2",      0x1002, OK);
    E16("$-2",      0x0FFE, OK);
    E16("$+0F000H", 0,       OVERFLOW_RANGE);
    E16("$-1001H",  0xFFFF, OK);
    E32("$+0F000H", 0x00010000, OK);
    E32("$-1001H",  0xFFFFFFFF, OK);

    symtab.intern(0x1000, "table");
    symtab.setCurrentOrigin(0x1100);
    E16("$-table",     0x100, OK);
    E16("($-table)/2", 0x80,  OK);
}

static void test_errors() {
    E32("0ABCX",   0, ILLEGAL_CONSTANT);
    E32("0177A",   0, ILLEGAL_CONSTANT);
    E32("0101C",   0, ILLEGAL_CONSTANT);
}

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_hex_constant);
    RUN_TEST(test_oct_constant);
    RUN_TEST(test_bin_constant);
    RUN_TEST(test_current_address);
    RUN_TEST(test_errors);
    return 0;
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
