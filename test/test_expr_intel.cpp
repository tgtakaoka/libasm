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
    E32("0ABCGH", 0, ILLEGAL_CONSTANT);
    E32("01778O", 0, ILLEGAL_CONSTANT);
    E32("01012B", 0, ILLEGAL_CONSTANT);
    E32("56789A", 0, ILLEGAL_CONSTANT);
}

static void test_formatter_8bit() {
    F8(0,     10, true,  "0");
    F8(32,    10, true,  "32");
    F8(128,   10, true,  "128");
    F8(255,   10, true,  "255");
    F8(256,   10, true,  "0");
    F8(-32,   10, true,  "224");
    F8(-128,  10, true,  "128");
    F8(-255,  10, true,  "1");
    F8(-256,  10, true,  "0");

    F8(0,    -10, true,  "0");
    F8(32,   -10, true,  "32");
    F8(128,  -10, true,  "-128");
    F8(255,  -10, true,  "-1");
    F8(256,  -10, true,  "0");
    F8(-32,  -10, true,  "-32");
    F8(-128, -10, true,  "-128");
    F8(-255, -10, true,  "1");
    F8(-256, -10, true,  "0");

    F8(0,     10, false, "0");
    F8(32,    10, false, "32");
    F8(128,   10, false, "128");
    F8(255,   10, false, "255");
    F8(256,   10, false, "0");
    F8(-32,   10, false, "224");
    F8(-128,  10, false, "128");
    F8(-255,  10, false, "1");
    F8(-256,  10, false, "0");

    F8(0,    -10, false, "0");
    F8(32,   -10, false, "32");
    F8(128,  -10, false, "-128");
    F8(255,  -10, false, "-1");
    F8(256,  -10, false, "0");
    F8(-32,  -10, false, "-32");
    F8(-128, -10, false, "-128");
    F8(-255, -10, false, "1");
    F8(-256, -10, false, "0");

    F8(0,     2, true,  "0");
    F8(32,    2, true,  "32");
    F8(33,    2, true,  "00100001b");
    F8(128,   2, true,  "10000000b");
    F8(255,   2, true,  "11111111b");
    F8(256,   2, true,  "0");
    F8(-32,   2, true,  "11100000b");
    F8(-33,   2, true,  "11011111b");
    F8(-128,  2, true,  "10000000b");
    F8(-255,  2, true,  "1");
    F8(-256,  2, true,  "0");

    F8(0,    -2, true,  "0");
    F8(32,   -2, true,  "32");
    F8(33,   -2, true,  "00100001b");
    F8(128,  -2, true,  "-10000000b");
    F8(255,  -2, true,  "-1");
    F8(256,  -2, true,  "0");
    F8(-32,  -2, true,  "-32");
    F8(-33,  -2, true,  "-00100001b");
    F8(-128, -2, true,  "-10000000b");
    F8(-255, -2, true,  "1");
    F8(-256, -2, true,  "0");

    F8(0,     2, false, "00000000b");
    F8(32,    2, false, "00100000b");
    F8(128,   2, false, "10000000b");
    F8(255,   2, false, "11111111b");
    F8(256,   2, false, "00000000b");
    F8(-32,   2, false, "11100000b");
    F8(-128,  2, false, "10000000b");
    F8(-255,  2, false, "00000001b");
    F8(-256,  2, false, "00000000b");

    F8(0,    -2, false, "00000000b");
    F8(32,   -2, false, "00100000b");
    F8(128,  -2, false, "-10000000b");
    F8(255,  -2, false, "-00000001b");
    F8(256,  -2, false, "00000000b");
    F8(-32,  -2, false, "-00100000b");
    F8(-128, -2, false, "-10000000b");
    F8(-255, -2, false, "00000001b");
    F8(-256, -2, false, "00000000b");

    F8(0,     8, true,  "0");
    F8(32,    8, true,  "32");
    F8(33,    8, true,  "041o");
    F8(128,   8, true,  "200o");
    F8(255,   8, true,  "377o");
    F8(256,   8, true,  "0");
    F8(-32,   8, true,  "340o");
    F8(-33,   8, true,  "337o");
    F8(-128,  8, true,  "200o");
    F8(-255,  8, true,  "1");
    F8(-256,  8, true,  "0");

    F8(0,    -8, true,  "0");
    F8(32,   -8, true,  "32");
    F8(33,   -8, true,  "041o");
    F8(128,  -8, true,  "-200o");
    F8(255,  -8, true,  "-1");
    F8(256,  -8, true,  "0");
    F8(-32,  -8, true,  "-32");
    F8(-33,  -8, true,  "-041o");
    F8(-128, -8, true,  "-200o");
    F8(-255, -8, true,  "1");
    F8(-256, -8, true,  "0");

    F8(0,     8, false, "000o");
    F8(32,    8, false, "040o");
    F8(128,   8, false, "200o");
    F8(255,   8, false, "377o");
    F8(256,   8, false, "000o");
    F8(-32,   8, false, "340o");
    F8(-128,  8, false, "200o");
    F8(-255,  8, false, "001o");
    F8(-256,  8, false, "000o");

    F8(0,    -8, false, "000o");
    F8(32,   -8, false, "040o");
    F8(128,  -8, false, "-200o");
    F8(255,  -8, false, "-001o");
    F8(256,  -8, false, "000o");
    F8(-32,  -8, false, "-040o");
    F8(-128, -8, false, "-200o");
    F8(-255, -8, false, "001o");
    F8(-256, -8, false, "000o");

    F8(0,    16, true,  "0");
    F8(32,   16, true,  "32");
    F8(33,   16, true,  "21h");
    F8(128,  16, true,  "80h");
    F8(255,  16, true,  "0ffh");
    F8(256,  16, true,  "0");
    F8(-32,  16, true,  "0e0h");
    F8(-33,  16, true,  "0dfh");
    F8(-128, 16, true,  "80h");
    F8(-255, 16, true,  "1");
    F8(-256, 16, true,  "0");

    F8(0,   -16, true,  "0");
    F8(32,  -16, true,  "32");
    F8(33,  -16, true,  "21h");
    F8(128, -16, true,  "-80h");
    F8(255, -16, true,  "-1");
    F8(256, -16, true,  "0");
    F8(-32, -16, true,  "-32");
    F8(-33, -16, true,  "-21h");
    F8(-128,-16, true,  "-80h");
    F8(-255,-16, true,  "1");
    F8(-256,-16, true,  "0");

    F8(0,    16, false, "00h");
    F8(32,   16, false, "20h");
    F8(128,  16, false, "80h");
    F8(255,  16, false, "0ffh");
    F8(256,  16, false, "00h");
    F8(-32,  16, false, "0e0h");
    F8(-128, 16, false, "80h");
    F8(-255, 16, false, "01h");
    F8(-256, 16, false, "00h");

    F8(0,   -16, false, "00h");
    F8(32,  -16, false, "20h");
    F8(128, -16, false, "-80h");
    F8(255, -16, false, "-01h");
    F8(256, -16, false, "00h");
    F8(-32, -16, false, "-20h");
    F8(-128,-16, false, "-80h");
    F8(-255,-16, false, "01h");
    F8(-256,-16, false, "00h");
}

static void test_formatter_16bit() {
    F16(0,         10, true,  "0");
    F16(32,        10, true,  "32");
    F16(0x8000,    10, true,  "32768");
    F16(0xffff,    10, true,  "65535");
    F16(0x10000,   10, true,  "0");
    F16(-32,       10, true,  "65504");
    F16(-0x8000,   10, true,  "32768");
    F16(-0xffff,   10, true,  "1");
    F16(-0x10000,  10, true,  "0");

    F16(0,        -10, true,  "0");
    F16(32,       -10, true,  "32");
    F16(0x8000,   -10, true,  "-32768");
    F16(0xffff,   -10, true,  "-1");
    F16(0x10000,  -10, true,  "0");
    F16(-32,      -10, true,  "-32");
    F16(-0x8000,  -10, true,  "-32768");
    F16(-0xffff,  -10, true,  "1");
    F16(-0x10000, -10, true,  "0");

    F16(0,         10, false, "0");
    F16(32,        10, false, "32");
    F16(0x8000,    10, false, "32768");
    F16(0xffff,    10, false, "65535");
    F16(0x10000,   10, false, "0");
    F16(-32,       10, false, "65504");
    F16(-0x8000,   10, false, "32768");
    F16(-0xffff,   10, false, "1");
    F16(-0x10000,  10, false, "0");

    F16(0,        -10, false, "0");
    F16(32,       -10, false, "32");
    F16(0x8000,   -10, false, "-32768");
    F16(0xffff,   -10, false, "-1");
    F16(0x10000,  -10, false, "0");
    F16(-32,      -10, false, "-32");
    F16(-0x8000,  -10, false, "-32768");
    F16(-0xffff,  -10, false, "1");
    F16(-0x10000, -10, false, "0");

    F16(0,          2, true,  "0");
    F16(32,         2, true,  "32");
    F16(33,         2, true,  "0000000000100001b");
    F16(0x8000,     2, true,  "1000000000000000b");
    F16(0xffff,     2, true,  "1111111111111111b");
    F16(0x10000,    2, true,  "0");
    F16(-32,        2, true,  "1111111111100000b");
    F16(-33,        2, true,  "1111111111011111b");
    F16(-0x8000,    2, true,  "1000000000000000b");
    F16(-0xffff,    2, true,  "1");
    F16(-0x10000,   2, true,  "0");

    F16(0,         -2, true,  "0");
    F16(32,        -2, true,  "32");
    F16(33,        -2, true,  "0000000000100001b");
    F16(0x8000,    -2, true,  "-1000000000000000b");
    F16(0xffff,    -2, true,  "-1");
    F16(0x10000,   -2, true,  "0");
    F16(-32,       -2, true,  "-32");
    F16(-33,       -2, true,  "-0000000000100001b");
    F16(-0x8000,   -2, true,  "-1000000000000000b");
    F16(-0xffff,   -2, true,  "1");
    F16(-0x10000,  -2, true,  "0");

    F16(0,          2, false, "0000000000000000b");
    F16(32,         2, false, "0000000000100000b");
    F16(0x8000,     2, false, "1000000000000000b");
    F16(0xffff,     2, false, "1111111111111111b");
    F16(0x10000,    2, false, "0000000000000000b");
    F16(-32,        2, false, "1111111111100000b");
    F16(-0x8000,    2, false, "1000000000000000b");
    F16(-0xffff,    2, false, "0000000000000001b");
    F16(-0x10000,   2, false, "0000000000000000b");

    F16(0,         -2, false, "0000000000000000b");
    F16(32,        -2, false, "0000000000100000b");
    F16(0x8000,    -2, false, "-1000000000000000b");
    F16(0xffff,    -2, false, "-0000000000000001b");
    F16(0x10000,   -2, false, "0000000000000000b");
    F16(-32,       -2, false, "-0000000000100000b");
    F16(-0x8000,   -2, false, "-1000000000000000b");
    F16(-0xffff,   -2, false, "0000000000000001b");
    F16(-0x10000,  -2, false, "0000000000000000b");

    F16(0,          8, true,  "0");
    F16(32,         8, true,  "32");
    F16(33,         8, true,  "000041o");
    F16(0x8000,     8, true,  "100000o");
    F16(0xffff,     8, true,  "177777o");
    F16(0x10000,    8, true,  "0");
    F16(-32,        8, true,  "177740o");
    F16(-33,        8, true,  "177737o");
    F16(-0x8000,    8, true,  "100000o");
    F16(-0xffff,    8, true,  "1");
    F16(-0x10000,   8, true,  "0");

    F16(0,         -8, true,  "0");
    F16(32,        -8, true,  "32");
    F16(33,        -8, true,  "000041o");
    F16(0x8000,    -8, true,  "-100000o");
    F16(0xffff,    -8, true,  "-1");
    F16(0x10000,   -8, true,  "0");
    F16(-32,       -8, true,  "-32");
    F16(-33,       -8, true,  "-000041o");
    F16(-0x8000,   -8, true,  "-100000o");
    F16(-0xffff,   -8, true,  "1");
    F16(-0x10000,  -8, true,  "0");

    F16(0,          8, false, "000000o");
    F16(32,         8, false, "000040o");
    F16(0x8000,     8, false, "100000o");
    F16(0xffff,     8, false, "177777o");
    F16(0x10000,    8, false, "000000o");
    F16(-32,        8, false, "177740o");
    F16(-0x8000,    8, false, "100000o");
    F16(-0xffff,    8, false, "000001o");
    F16(-0x10000,   8, false, "000000o");

    F16(0,         -8, false, "000000o");
    F16(32,        -8, false, "000040o");
    F16(0x8000,    -8, false, "-100000o");
    F16(0xffff,    -8, false, "-000001o");
    F16(0x10000,   -8, false, "000000o");
    F16(-32,       -8, false, "-000040o");
    F16(-0x8000,   -8, false, "-100000o");
    F16(-0xffff,   -8, false, "000001o");
    F16(-0x10000,  -8, false, "000000o");

    F16(0,         16, true,  "0");
    F16(32,        16, true,  "32");
    F16(33,        16, true,  "0021h");
    F16(0x8000,    16, true,  "8000h");
    F16(0xffff,    16, true,  "0ffffh");
    F16(0x10000,   16, true,  "0");
    F16(-32,       16, true,  "0ffe0h");
    F16(-33,       16, true,  "0ffdfh");
    F16(-0x8000,   16, true,  "8000h");
    F16(-0xffff,   16, true,  "1");
    F16(-0x10000,  16, true,  "0");

    F16(0,        -16, true,  "0");
    F16(32,       -16, true,  "32");
    F16(33,       -16, true,  "0021h");
    F16(0x8000,   -16, true,  "-8000h");
    F16(0xffff,   -16, true,  "-1");
    F16(0x10000,  -16, true,  "0");
    F16(-32,      -16, true,  "-32");
    F16(-33,      -16, true,  "-0021h");
    F16(-0x8000,  -16, true,  "-8000h");
    F16(-0xffff,  -16, true,  "1");
    F16(-0x10000, -16, true,  "0");

    F16(0,         16, false, "0000h");
    F16(32,        16, false, "0020h");
    F16(0x8000,    16, false, "8000h");
    F16(0xffff,    16, false, "0ffffh");
    F16(0x10000,   16, false, "0000h");
    F16(-32,       16, false, "0ffe0h");
    F16(-0x8000,   16, false, "8000h");
    F16(-0xffff,   16, false, "0001h");
    F16(-0x10000,  16, false, "0000h");

    F16(0,        -16, false, "0000h");
    F16(32,       -16, false, "0020h");
    F16(0x8000,   -16, false, "-8000h");
    F16(0xffff,   -16, false, "-0001h");
    F16(0x10000,  -16, false, "0000h");
    F16(-32,      -16, false, "-0020h");
    F16(-0x8000,  -16, false, "-8000h");
    F16(-0xffff,  -16, false, "0001h");
    F16(-0x10000, -16, false, "0000h");
}

static void test_formatter_24bit() {
    F24(0,           10, true,  "0");
    F24(32,          10, true,  "32");
    F24(0x800000,    10, true,  "8388608");
    F24(0xffffff,    10, true,  "16777215");
    F24(0x1000000,   10, true,  "0");
    F24(-32,         10, true,  "16777184");
    F24(-0x800000,   10, true,  "8388608");
    F24(-0xffffff,   10, true,  "1");
    F24(-0x1000000,  10, true,  "0");

    F24(0,          -10, true,  "0");
    F24(32,         -10, true,  "32");
    F24(0x800000,   -10, true,  "-8388608");
    F24(0xffffff,   -10, true,  "-1");
    F24(0x1000000,  -10, true,  "0");
    F24(-32,        -10, true,  "-32");
    F24(-0x800000,  -10, true,  "-8388608");
    F24(-0xffffff,  -10, true,  "1");
    F24(-0x1000000, -10, true,  "0");

    F24(0,           10, false, "0");
    F24(32,          10, false, "32");
    F24(0x800000,    10, false, "8388608");
    F24(0xffffff,    10, false, "16777215");
    F24(0x1000000,   10, false, "0");
    F24(-32,         10, false, "16777184");
    F24(-0x800000,   10, false, "8388608");
    F24(-0xffffff,   10, false, "1");
    F24(-0x1000000,  10, false, "0");

    F24(0,          -10, false, "0");
    F24(32,         -10, false, "32");
    F24(0x800000,   -10, false, "-8388608");
    F24(0xffffff,   -10, false, "-1");
    F24(0x1000000,  -10, false, "0");
    F24(-32,        -10, false, "-32");
    F24(-0x800000,  -10, false, "-8388608");
    F24(-0xffffff,  -10, false, "1");
    F24(-0x1000000, -10, false, "0");

    F24(0,            2, true,  "0");
    F24(32,           2, true,  "32");
    F24(33,           2, true,  "000000000000000000100001b");
    F24(0x800000,     2, true,  "100000000000000000000000b");
    F24(0xffffff,     2, true,  "111111111111111111111111b");
    F24(0x1000000,    2, true,  "0");
    F24(-32,          2, true,  "111111111111111111100000b");
    F24(-33,          2, true,  "111111111111111111011111b");
    F24(-0x800000,    2, true,  "100000000000000000000000b");
    F24(-0xffffff,    2, true,  "1");
    F24(-0x1000000,   2, true,  "0");

    F24(0,           -2, true,  "0");
    F24(32,          -2, true,  "32");
    F24(33,          -2, true,  "000000000000000000100001b");
    F24(0x800000,    -2, true,  "-100000000000000000000000b");
    F24(0xffffff,    -2, true,  "-1");
    F24(0x1000000,   -2, true,  "0");
    F24(-32,         -2, true,  "-32");
    F24(-33,         -2, true,  "-000000000000000000100001b");
    F24(-0x800000,   -2, true,  "-100000000000000000000000b");
    F24(-0xffffff,   -2, true,  "1");
    F24(-0x1000000,  -2, true,  "0");

    F24(0,            2, false, "000000000000000000000000b");
    F24(32,           2, false, "000000000000000000100000b");
    F24(0x800000,     2, false, "100000000000000000000000b");
    F24(0xffffff,     2, false, "111111111111111111111111b");
    F24(0x1000000,    2, false, "000000000000000000000000b");
    F24(-32,          2, false, "111111111111111111100000b");
    F24(-0x800000,    2, false, "100000000000000000000000b");
    F24(-0xffffff,    2, false, "000000000000000000000001b");
    F24(-0x1000000,   2, false, "000000000000000000000000b");

    F24(0,           -2, false, "000000000000000000000000b");
    F24(32,          -2, false, "000000000000000000100000b");
    F24(0x800000,    -2, false, "-100000000000000000000000b");
    F24(0xffffff,    -2, false, "-000000000000000000000001b");
    F24(0x1000000,   -2, false, "000000000000000000000000b");
    F24(-32,         -2, false, "-000000000000000000100000b");
    F24(-0x800000,   -2, false, "-100000000000000000000000b");
    F24(-0xffffff,   -2, false, "000000000000000000000001b");
    F24(-0x1000000,  -2, false, "000000000000000000000000b");

    F24(0,            8, true,  "0");
    F24(32,           8, true,  "32");
    F24(33,           8, true,  "00000041o");
    F24(0x800000,     8, true,  "40000000o");
    F24(0xffffff,     8, true,  "77777777o");
    F24(0x1000000,    8, true,  "0");
    F24(-32,          8, true,  "77777740o");
    F24(-33,          8, true,  "77777737o");
    F24(-0x800000,    8, true,  "40000000o");
    F24(-0xffffff,    8, true,  "1");
    F24(-0x1000000,   8, true,  "0");

    F24(0,           -8, true,  "0");
    F24(32,          -8, true,  "32");
    F24(33,          -8, true,  "00000041o");
    F24(0x800000,    -8, true,  "-40000000o");
    F24(0xffffff,    -8, true,  "-1");
    F24(0x1000000,   -8, true,  "0");
    F24(-32,         -8, true,  "-32");
    F24(-33,         -8, true,  "-00000041o");
    F24(-0x800000,   -8, true,  "-40000000o");
    F24(-0xffffff,   -8, true,  "1");
    F24(-0x1000000,  -8, true,  "0");

    F24(0,            8, false, "00000000o");
    F24(32,           8, false, "00000040o");
    F24(0x800000,     8, false, "40000000o");
    F24(0xffffff,     8, false, "77777777o");
    F24(0x1000000,    8, false, "00000000o");
    F24(-32,          8, false, "77777740o");
    F24(-0x800000,    8, false, "40000000o");
    F24(-0xffffff,    8, false, "00000001o");
    F24(-0x1000000,   8, false, "00000000o");

    F24(0,           -8, false, "00000000o");
    F24(32,          -8, false, "00000040o");
    F24(0x800000,    -8, false, "-40000000o");
    F24(0xffffff,    -8, false, "-00000001o");
    F24(0x1000000,   -8, false, "00000000o");
    F24(-32,         -8, false, "-00000040o");
    F24(-0x800000,   -8, false, "-40000000o");
    F24(-0xffffff,   -8, false, "00000001o");
    F24(-0x1000000,  -8, false, "00000000o");

    F24(0,           16, true,  "0");
    F24(32,          16, true,  "32");
    F24(33,          16, true,  "000021h");
    F24(0x800000,    16, true,  "800000h");
    F24(0xffffff,    16, true,  "0ffffffh");
    F24(0x1000000,   16, true,  "0");
    F24(-32,         16, true,  "0ffffe0h");
    F24(-33,         16, true,  "0ffffdfh");
    F24(-0x800000,   16, true,  "800000h");
    F24(-0xffffff,   16, true,  "1");
    F24(-0x1000000,  16, true,  "0");

    F24(0,          -16, true,  "0");
    F24(32,         -16, true,  "32");
    F24(33,         -16, true,  "000021h");
    F24(0x800000,   -16, true,  "-800000h");
    F24(0xffffff,   -16, true,  "-1");
    F24(0x1000000,  -16, true,  "0");
    F24(-32,        -16, true,  "-32");
    F24(-33,        -16, true,  "-000021h");
    F24(-0x800000,  -16, true,  "-800000h");
    F24(-0xffffff,  -16, true,  "1");
    F24(-0x1000000, -16, true,  "0");

    F24(0,           16, false, "000000h");
    F24(32,          16, false, "000020h");
    F24(0x800000,    16, false, "800000h");
    F24(0xffffff,    16, false, "0ffffffh");
    F24(0x1000000,   16, false, "000000h");
    F24(-32,         16, false, "0ffffe0h");
    F24(-0x800000,   16, false, "800000h");
    F24(-0xffffff,   16, false, "000001h");
    F24(-0x1000000,  16, false, "000000h");

    F24(0,          -16, false, "000000h");
    F24(32,         -16, false, "000020h");
    F24(0x800000,   -16, false, "-800000h");
    F24(0xffffff,   -16, false, "-000001h");
    F24(0x1000000,  -16, false, "000000h");
    F24(-32,        -16, false, "-000020h");
    F24(-0x800000,  -16, false, "-800000h");
    F24(-0xffffff,  -16, false, "000001h");
    F24(-0x1000000, -16, false, "000000h");
}

static void test_formatter_32bit() {
    F32(0,            10, true, "0");
    F32(32,           10, true, "32");
    F32(0x80000000,   10, true, "2147483648");
    F32(0xffffffff,   10, true, "4294967295");
    F32(-32,          10, true, "4294967264");
    F32(-0x80000000,  10, true, "2147483648");
    F32(-0xffffffff,  10, true, "1");

    F32(0,           -10, true, "0");
    F32(32,          -10, true, "32");
    F32(0x80000000,  -10, true, "-2147483648");
    F32(0xffffffff,  -10, true, "-1");
    F32(-32,         -10, true, "-32");
    F32(-0x80000000, -10, true, "-2147483648");
    F32(-0xffffffff, -10, true, "1");

    F32(0,            10, false, "0");
    F32(32,           10, false, "32");
    F32(0x80000000,   10, false, "2147483648");
    F32(0xffffffff,   10, false, "4294967295");
    F32(-32,          10, false, "4294967264");
    F32(-0x80000000,  10, false, "2147483648");
    F32(-0xffffffff,  10, false, "1");

    F32(0,           -10, false, "0");
    F32(32,          -10, false, "32");
    F32(0x80000000,  -10, false, "-2147483648");
    F32(0xffffffff,  -10, false, "-1");
    F32(-32,         -10, false, "-32");
    F32(-0x80000000, -10, false, "-2147483648");
    F32(-0xffffffff, -10, false, "1");

    F32(0,            2, true,  "0");
    F32(32,           2, true,  "32");
    F32(33,           2, true,  "00000000000000000000000000100001b");
    F32(0x80000000,   2, true,  "10000000000000000000000000000000b");
    F32(0xffffffff,   2, true,  "11111111111111111111111111111111b");
    F32(-32,          2, true,  "11111111111111111111111111100000b");
    F32(-33,          2, true,  "11111111111111111111111111011111b");
    F32(-0x80000000,  2, true,  "10000000000000000000000000000000b");
    F32(-0xffffffff,  2, true,  "1");

    F32(0,           -2, true,  "0");
    F32(32,          -2, true,  "32");
    F32(33,          -2, true,  "00000000000000000000000000100001b");
    F32(0x80000000,  -2, true,  "-10000000000000000000000000000000b");
    F32(0xffffffff,  -2, true,  "-1");
    F32(-32,         -2, true,  "-32");
    F32(-33,         -2, true,  "-00000000000000000000000000100001b");
    F32(-0x80000000, -2, true,  "-10000000000000000000000000000000b");
    F32(-0xffffffff, -2, true,  "1");

    F32(0,            2, false, "00000000000000000000000000000000b");
    F32(32,           2, false, "00000000000000000000000000100000b");
    F32(0x80000000,   2, false, "10000000000000000000000000000000b");
    F32(0xffffffff,   2, false, "11111111111111111111111111111111b");
    F32(-32,          2, false, "11111111111111111111111111100000b");
    F32(-0x80000000,  2, false, "10000000000000000000000000000000b");
    F32(-0xffffffff,  2, false, "00000000000000000000000000000001b");

    F32(0,           -2, false, "00000000000000000000000000000000b");
    F32(32,          -2, false, "00000000000000000000000000100000b");
    F32(0x80000000,  -2, false, "-10000000000000000000000000000000b");
    F32(0xffffffff,  -2, false, "-00000000000000000000000000000001b");
    F32(-32,         -2, false, "-00000000000000000000000000100000b");
    F32(-0x80000000, -2, false, "-10000000000000000000000000000000b");
    F32(-0xffffffff, -2, false, "00000000000000000000000000000001b");

    F32(0,            8, true,  "0");
    F32(32,           8, true,  "32");
    F32(33,           8, true,  "00000000041o");
    F32(0x80000000,   8, true,  "20000000000o");
    F32(0xffffffff,   8, true,  "37777777777o");
    F32(-32,          8, true,  "37777777740o");
    F32(-33,          8, true,  "37777777737o");
    F32(-0x80000000,  8, true,  "20000000000o");
    F32(-0xffffffff,  8, true,  "1");

    F32(0,           -8, true,  "0");
    F32(32,          -8, true,  "32");
    F32(33,          -8, true,  "00000000041o");
    F32(0x80000000,  -8, true,  "-20000000000o");
    F32(0xffffffff,  -8, true,  "-1");
    F32(-32,         -8, true,  "-32");
    F32(-33,         -8, true,  "-00000000041o");
    F32(-0x80000000, -8, true,  "-20000000000o");
    F32(-0xffffffff, -8, true,  "1");

    F32(0,            8, false, "00000000000o");
    F32(32,           8, false, "00000000040o");
    F32(0x80000000,   8, false, "20000000000o");
    F32(0xffffffff,   8, false, "37777777777o");
    F32(-32,          8, false, "37777777740o");
    F32(-0x80000000,  8, false, "20000000000o");
    F32(-0xffffffff,  8, false, "00000000001o");

    F32(0,           -8, false, "00000000000o");
    F32(32,          -8, false, "00000000040o");
    F32(0x80000000,  -8, false, "-20000000000o");
    F32(0xffffffff,  -8, false, "-00000000001o");
    F32(-32,         -8, false, "-00000000040o");
    F32(-0x80000000, -8, false, "-20000000000o");
    F32(-0xffffffff, -8, false, "00000000001o");

    F32(0,           16, true,  "0");
    F32(32,          16, true,  "32");
    F32(33,          16, true,  "00000021h");
    F32(0x80000000,  16, true,  "80000000h");
    F32(0xffffffff,  16, true,  "0ffffffffh");
    F32(-32,         16, true,  "0ffffffe0h");
    F32(-33,         16, true,  "0ffffffdfh");
    F32(-0x80000000, 16, true,  "80000000h");
    F32(-0xffffffff, 16, true,  "1");

    F32(0,           -16, true,  "0");
    F32(32,          -16, true,  "32");
    F32(33,          -16, true,  "00000021h");
    F32(0x80000000,  -16, true,  "-80000000h");
    F32(0xffffffff,  -16, true,  "-1");
    F32(-32,         -16, true,  "-32");
    F32(-33,         -16, true,  "-00000021h");
    F32(-0x80000000, -16, true,  "-80000000h");
    F32(-0xffffffff, -16, true,  "1");

    F32(0,           16, false,  "00000000h");
    F32(32,          16, false,  "00000020h");
    F32(0x80000000,  16, false,  "80000000h");
    F32(0xffffffff,  16, false,  "0ffffffffh");
    F32(-32,         16, false,  "0ffffffe0h");
    F32(-0x80000000, 16, false,  "80000000h");
    F32(-0xffffffff, 16, false,  "00000001h");

    F32(0,           -16, false, "00000000h");
    F32(32,          -16, false, "00000020h");
    F32(0x80000000,  -16, false, "-80000000h");
    F32(0xffffffff,  -16, false, "-00000001h");
    F32(-32,         -16, false, "-00000020h");
    F32(-0x80000000, -16, false, "-80000000h");
    F32(-0xffffffff, -16, false, "00000001h");
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
    RUN_TEST(test_formatter_8bit);
    RUN_TEST(test_formatter_16bit);
    RUN_TEST(test_formatter_24bit);
    RUN_TEST(test_formatter_32bit);
    return 0;
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
