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

IntelValueParser parser;
IntelValueFormatter formatter;

static void set_up() {}

static void tear_down() {
    symtab.reset();
}

// clang-format off
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

    E32("0x0",         0x00000000, OK);
    E32("0x7FFFFFFF",  0x7fffffff, OK);
    E32("0x80000000",  0x80000000, OK);
    E32("0xFFFFffff",  0xffffffff, OK);
    E32("0x100000000", 0,          OVERFLOW_RANGE);
}
// clang-format on

static void test_oct_constant() {
    E8("0O", 0x00, OK);
    E8("177O", 0x7f, OK);
    E8("200O", 0x80, OK);
    E8("377O", 0xff, OK);
    E8("400O", 0, OVERFLOW_RANGE);

    E16("0O", 0x0000, OK);
    E16("77777O", 0x7fff, OK);
    E16("100000O", 0x8000, OK);
    E16("177777o", 0xffff, OK);
    E16("200000O", 0, OVERFLOW_RANGE);

    E32("0O", 0x00000000, OK);
    E32("17777777777O", 0x7fffffff, OK);
    E32("20000000000o", 0x80000000, OK);
    E32("37777777777O", 0xffffffff, OK);
    E32("40000000000O", 0, OVERFLOW_RANGE);

    E32("00", 0x00000000, OK);
    E32("017777777777", 0x7fffffff, OK);
    E32("020000000000", 0x80000000, OK);
    E32("037777777777", 0xffffffff, OK);
    E32("040000000000", 0, OVERFLOW_RANGE);
}

static void test_bin_constant() {
    E8("0B", 0x00, OK);
    E8("01111111b", 0x7f, OK);
    E8("10000000B", 0x80, OK);
    E8("11111111B", 0xff, OK);
    E8("100000000B", 0, OVERFLOW_RANGE);

    E16("0B", 0x0000, OK);
    E16("0111111111111111B", 0x7fff, OK);
    E16("1000000000000000B", 0x8000, OK);
    E16("1111111111111111b", 0xffff, OK);
    E16("10000000000000000B", 0, OVERFLOW_RANGE);

    E32("0B", 0x00000000, OK);
    E32("01111111111111111111111111111111B", 0x7fffffff, OK);
    E32("10000000000000000000000000000000B", 0x80000000, OK);
    E32("11111111111111111111111111111111B", 0xffffffff, OK);
    E32("100000000000000000000000000000000b", 0, OVERFLOW_RANGE);

    E32("0b0", 0x00000000, OK);
    E32("0b01111111111111111111111111111111", 0x7fffffff, OK);
    E32("0b10000000000000000000000000000000", 0x80000000, OK);
    E32("0b11111111111111111111111111111111", 0xffffffff, OK);
    E32("0b100000000000000000000000000000000", 0, OVERFLOW_RANGE);
}

static void test_current_address() {
    parser.setCurrentOrigin(0x1000);
    E16("$", 0x1000, OK);
    E16("$+2", 0x1002, OK);
    E16("$-2", 0x0FFE, OK);
    E16("$+0F000H", 0, OVERFLOW_RANGE);
    E16("$-1001H", 0xFFFF, OK);
    E32("$+0F000H", 0x00010000, OK);
    E32("$-1001H", 0xFFFFFFFF, OK);
    E16(".", 0x1000, OK);
    E16(".+2", 0x1002, OK);
    E16(".-2", 0x0FFE, OK);
    E16(".+0xF000", 0, OVERFLOW_RANGE);
    E16(".-0x1001", 0xFFFF, OK);
    E32(".+0xF000", 0x00010000, OK);
    E32(".-0x1001", 0xFFFFFFFF, OK);

    symtab.intern(0x1000, "table");
    parser.setCurrentOrigin(0x1100);
    E16("$-table", 0x100, OK);
    E16("($-table)/2", 0x80, OK);
    E16(".-table", 0x100, OK);
    E16("(.-table)/2", 0x80, OK);
}

static void test_errors() {
    E32("0ABCGH", 0, ILLEGAL_CONSTANT);
    E32("01778O", 0, ILLEGAL_CONSTANT);
    E32("01012B", 0, ILLEGAL_CONSTANT);
    E32("56789A", 0, ILLEGAL_CONSTANT);
}

static void test_formatter_8bit() {
    DEC(0, 8, "0");
    DEC(32, 8, "32");
    DEC(128, 8, "128");
    DEC(255, 8, "255");
    DEC(256, 8, "0");
    DEC(-32, 8, "224");
    DEC(-128, 8, "128");
    DEC(-255, 8, "1");
    DEC(-256, 8, "0");

    DEC(0, -8, "0");
    DEC(32, -8, "32");
    DEC(128, -8, "-128");
    DEC(255, -8, "-1");
    DEC(256, -8, "0");
    DEC(-32, -8, "-32");
    DEC(-128, -8, "-128");
    DEC(-255, -8, "1");
    DEC(-256, -8, "0");

    DEC(0, 8, "0");
    DEC(32, 8, "32");
    DEC(128, 8, "128");
    DEC(255, 8, "255");
    DEC(256, 8, "0");
    DEC(-32, 8, "224");
    DEC(-128, 8, "128");
    DEC(-255, 8, "1");
    DEC(-256, 8, "0");

    DEC(0, -8, "0");
    DEC(32, -8, "32");
    DEC(128, -8, "-128");
    DEC(255, -8, "-1");
    DEC(256, -8, "0");
    DEC(-32, -8, "-32");
    DEC(-128, -8, "-128");
    DEC(-255, -8, "1");
    DEC(-256, -8, "0");

    HEX(0, 8, true, "0");
    HEX(32, 8, true, "32");
    HEX(33, 8, true, "21h");
    HEX(128, 8, true, "80h");
    HEX(255, 8, true, "0ffh");
    HEX(256, 8, true, "0");
    HEX(-32, 8, true, "0e0h");
    HEX(-33, 8, true, "0dfh");
    HEX(-128, 8, true, "80h");
    HEX(-255, 8, true, "1");
    HEX(-256, 8, true, "0");

    HEX(0, -8, true, "0");
    HEX(32, -8, true, "32");
    HEX(33, -8, true, "21h");
    HEX(128, -8, true, "-80h");
    HEX(255, -8, true, "-1");
    HEX(256, -8, true, "0");
    HEX(-32, -8, true, "-32");
    HEX(-33, -8, true, "-21h");
    HEX(-128, -8, true, "-80h");
    HEX(-255, -8, true, "1");
    HEX(-256, -8, true, "0");

    HEX(0, 8, false, "00h");
    HEX(32, 8, false, "20h");
    HEX(128, 8, false, "80h");
    HEX(255, 8, false, "0ffh");
    HEX(256, 8, false, "00h");
    HEX(-32, 8, false, "0e0h");
    HEX(-128, 8, false, "80h");
    HEX(-255, 8, false, "01h");
    HEX(-256, 8, false, "00h");

    HEX(0, -8, false, "00h");
    HEX(32, -8, false, "20h");
    HEX(128, -8, false, "-80h");
    HEX(255, -8, false, "-01h");
    HEX(256, -8, false, "00h");
    HEX(-32, -8, false, "-20h");
    HEX(-128, -8, false, "-80h");
    HEX(-255, -8, false, "01h");
    HEX(-256, -8, false, "00h");
}

static void test_formatter_16bit() {
    DEC(0, 16, "0");
    DEC(32, 16, "32");
    DEC(0x8000, 16, "32768");
    DEC(0xffff, 16, "65535");
    DEC(0x10000, 16, "0");
    DEC(-32, 16, "65504");
    DEC(-0x8000, 16, "32768");
    DEC(-0xffff, 16, "1");
    DEC(-0x10000, 16, "0");

    DEC(0, -16, "0");
    DEC(32, -16, "32");
    DEC(0x8000, -16, "-32768");
    DEC(0xffff, -16, "-1");
    DEC(0x10000, -16, "0");
    DEC(-32, -16, "-32");
    DEC(-0x8000, -16, "-32768");
    DEC(-0xffff, -16, "1");
    DEC(-0x10000, -16, "0");

    DEC(0, 16, "0");
    DEC(32, 16, "32");
    DEC(0x8000, 16, "32768");
    DEC(0xffff, 16, "65535");
    DEC(0x10000, 16, "0");
    DEC(-32, 16, "65504");
    DEC(-0x8000, 16, "32768");
    DEC(-0xffff, 16, "1");
    DEC(-0x10000, 16, "0");

    DEC(0, -16, "0");
    DEC(32, -16, "32");
    DEC(0x8000, -16, "-32768");
    DEC(0xffff, -16, "-1");
    DEC(0x10000, -16, "0");
    DEC(-32, -16, "-32");
    DEC(-0x8000, -16, "-32768");
    DEC(-0xffff, -16, "1");
    DEC(-0x10000, -16, "0");

    HEX(0, 16, true, "0");
    HEX(32, 16, true, "32");
    HEX(33, 16, true, "0021h");
    HEX(0x8000, 16, true, "8000h");
    HEX(0xffff, 16, true, "0ffffh");
    HEX(0x10000, 16, true, "0");
    HEX(-32, 16, true, "0ffe0h");
    HEX(-33, 16, true, "0ffdfh");
    HEX(-0x8000, 16, true, "8000h");
    HEX(-0xffff, 16, true, "1");
    HEX(-0x10000, 16, true, "0");

    HEX(0, -16, true, "0");
    HEX(32, -16, true, "32");
    HEX(33, -16, true, "0021h");
    HEX(0x8000, -16, true, "-8000h");
    HEX(0xffff, -16, true, "-1");
    HEX(0x10000, -16, true, "0");
    HEX(-32, -16, true, "-32");
    HEX(-33, -16, true, "-0021h");
    HEX(-0x8000, -16, true, "-8000h");
    HEX(-0xffff, -16, true, "1");
    HEX(-0x10000, -16, true, "0");

    HEX(0, 16, false, "0000h");
    HEX(32, 16, false, "0020h");
    HEX(0x8000, 16, false, "8000h");
    HEX(0xffff, 16, false, "0ffffh");
    HEX(0x10000, 16, false, "0000h");
    HEX(-32, 16, false, "0ffe0h");
    HEX(-0x8000, 16, false, "8000h");
    HEX(-0xffff, 16, false, "0001h");
    HEX(-0x10000, 16, false, "0000h");

    HEX(0, -16, false, "0000h");
    HEX(32, -16, false, "0020h");
    HEX(0x8000, -16, false, "-8000h");
    HEX(0xffff, -16, false, "-0001h");
    HEX(0x10000, -16, false, "0000h");
    HEX(-32, -16, false, "-0020h");
    HEX(-0x8000, -16, false, "-8000h");
    HEX(-0xffff, -16, false, "0001h");
    HEX(-0x10000, -16, false, "0000h");
}

static void test_formatter_24bit() {
    DEC(0, 24, "0");
    DEC(32, 24, "32");
    DEC(0x800000, 24, "8388608");
    DEC(0xffffff, 24, "16777215");
    DEC(0x1000000, 24, "0");
    DEC(-32, 24, "16777184");
    DEC(-0x800000, 24, "8388608");
    DEC(-0xffffff, 24, "1");
    DEC(-0x1000000, 24, "0");

    DEC(0, -24, "0");
    DEC(32, -24, "32");
    DEC(0x800000, -24, "-8388608");
    DEC(0xffffff, -24, "-1");
    DEC(0x1000000, -24, "0");
    DEC(-32, -24, "-32");
    DEC(-0x800000, -24, "-8388608");
    DEC(-0xffffff, -24, "1");
    DEC(-0x1000000, -24, "0");

    DEC(0, 24, "0");
    DEC(32, 24, "32");
    DEC(0x800000, 24, "8388608");
    DEC(0xffffff, 24, "16777215");
    DEC(0x1000000, 24, "0");
    DEC(-32, 24, "16777184");
    DEC(-0x800000, 24, "8388608");
    DEC(-0xffffff, 24, "1");
    DEC(-0x1000000, 24, "0");

    DEC(0, -24, "0");
    DEC(32, -24, "32");
    DEC(0x800000, -24, "-8388608");
    DEC(0xffffff, -24, "-1");
    DEC(0x1000000, -24, "0");
    DEC(-32, -24, "-32");
    DEC(-0x800000, -24, "-8388608");
    DEC(-0xffffff, -24, "1");
    DEC(-0x1000000, -24, "0");

    HEX(0, 24, true, "0");
    HEX(32, 24, true, "32");
    HEX(33, 24, true, "000021h");
    HEX(0x800000, 24, true, "800000h");
    HEX(0xffffff, 24, true, "0ffffffh");
    HEX(0x1000000, 24, true, "0");
    HEX(-32, 24, true, "0ffffe0h");
    HEX(-33, 24, true, "0ffffdfh");
    HEX(-0x800000, 24, true, "800000h");
    HEX(-0xffffff, 24, true, "1");
    HEX(-0x1000000, 24, true, "0");

    HEX(0, -24, true, "0");
    HEX(32, -24, true, "32");
    HEX(33, -24, true, "000021h");
    HEX(0x800000, -24, true, "-800000h");
    HEX(0xffffff, -24, true, "-1");
    HEX(0x1000000, -24, true, "0");
    HEX(-32, -24, true, "-32");
    HEX(-33, -24, true, "-000021h");
    HEX(-0x800000, -24, true, "-800000h");
    HEX(-0xffffff, -24, true, "1");
    HEX(-0x1000000, -24, true, "0");

    HEX(0, 24, false, "000000h");
    HEX(32, 24, false, "000020h");
    HEX(0x800000, 24, false, "800000h");
    HEX(0xffffff, 24, false, "0ffffffh");
    HEX(0x1000000, 24, false, "000000h");
    HEX(-32, 24, false, "0ffffe0h");
    HEX(-0x800000, 24, false, "800000h");
    HEX(-0xffffff, 24, false, "000001h");
    HEX(-0x1000000, 24, false, "000000h");

    HEX(0, -24, false, "000000h");
    HEX(32, -24, false, "000020h");
    HEX(0x800000, -24, false, "-800000h");
    HEX(0xffffff, -24, false, "-000001h");
    HEX(0x1000000, -24, false, "000000h");
    HEX(-32, -24, false, "-000020h");
    HEX(-0x800000, -24, false, "-800000h");
    HEX(-0xffffff, -24, false, "000001h");
    HEX(-0x1000000, -24, false, "000000h");
}

static void test_formatter_32bit() {
    DEC(0, 32, "0");
    DEC(32, 32, "32");
    DEC(0x80000000, 32, "2147483648");
    DEC(0xffffffff, 32, "4294967295");
    DEC(-32, 32, "4294967264");
    DEC(-0x80000000, 32, "2147483648");
    DEC(-0xffffffff, 32, "1");

    DEC(0, -32, "0");
    DEC(32, -32, "32");
    DEC(0x80000000, -32, "-2147483648");
    DEC(0xffffffff, -32, "-1");
    DEC(-32, -32, "-32");
    DEC(-0x80000000, -32, "-2147483648");
    DEC(-0xffffffff, -32, "1");

    DEC(0, 32, "0");
    DEC(32, 32, "32");
    DEC(0x80000000, 32, "2147483648");
    DEC(0xffffffff, 32, "4294967295");
    DEC(-32, 32, "4294967264");
    DEC(-0x80000000, 32, "2147483648");
    DEC(-0xffffffff, 32, "1");

    DEC(0, -32, "0");
    DEC(32, -32, "32");
    DEC(0x80000000, -32, "-2147483648");
    DEC(0xffffffff, -32, "-1");
    DEC(-32, -32, "-32");
    DEC(-0x80000000, -32, "-2147483648");
    DEC(-0xffffffff, -32, "1");

    HEX(0, 32, true, "0");
    HEX(32, 32, true, "32");
    HEX(33, 32, true, "00000021h");
    HEX(0x80000000, 32, true, "80000000h");
    HEX(0xffffffff, 32, true, "0ffffffffh");
    HEX(-32, 32, true, "0ffffffe0h");
    HEX(-33, 32, true, "0ffffffdfh");
    HEX(-0x80000000, 32, true, "80000000h");
    HEX(-0xffffffff, 32, true, "1");

    HEX(0, -32, true, "0");
    HEX(32, -32, true, "32");
    HEX(33, -32, true, "00000021h");
    HEX(0x80000000, -32, true, "-80000000h");
    HEX(0xffffffff, -32, true, "-1");
    HEX(-32, -32, true, "-32");
    HEX(-33, -32, true, "-00000021h");
    HEX(-0x80000000, -32, true, "-80000000h");
    HEX(-0xffffffff, -32, true, "1");

    HEX(0, 32, false, "00000000h");
    HEX(32, 32, false, "00000020h");
    HEX(0x80000000, 32, false, "80000000h");
    HEX(0xffffffff, 32, false, "0ffffffffh");
    HEX(-32, 32, false, "0ffffffe0h");
    HEX(-0x80000000, 32, false, "80000000h");
    HEX(-0xffffffff, 32, false, "00000001h");

    HEX(0, -32, false, "00000000h");
    HEX(32, -32, false, "00000020h");
    HEX(0x80000000, -32, false, "-80000000h");
    HEX(0xffffffff, -32, false, "-00000001h");
    HEX(-32, -32, false, "-00000020h");
    HEX(-0x80000000, -32, false, "-80000000h");
    HEX(-0xffffffff, -32, false, "00000001h");
}

void run_tests() {
    RUN_TEST(test_hex_constant);
    RUN_TEST(test_oct_constant);
    RUN_TEST(test_bin_constant);
    RUN_TEST(test_current_address);
    RUN_TEST(test_errors);
    RUN_TEST(test_formatter_8bit);
    RUN_TEST(test_formatter_16bit);
    RUN_TEST(test_formatter_24bit);
    RUN_TEST(test_formatter_32bit);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
