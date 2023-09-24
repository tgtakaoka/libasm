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

#include "operators.h"

using namespace libasm;
using namespace libasm::test;

const struct final : ValueParser::Plugins {
    const NumberParser &number() const override { return IntelNumberParser::singleton(); }
    const SymbolParser &symbol() const override { return _symbol; }
    const OperatorParser &operators() const override { return IntelOperatorParser::singleton(); }
    const SimpleSymbolParser _symbol{PSTR("_?@")};
} plugins{};
struct : ValueParser::Locator {
    uint32_t location = 0;
    uint32_t currentLocation() const { return location; }
} locator;
const ValueParser parser{plugins, locator};

const ValueFormatter formatter{ValueFormatter::Plugins::intel()};

static void set_up() {}

static void tear_down() {
    symtab.reset();
}

// clang-format off
static void test_char_constant() {
    E8("'a'",   0x61);
    E8("'a'+5", 0x66);
    E8("5+'a'", 0x66);
    X8("'a",    MISSING_CLOSING_QUOTE, "'a");
    X8("'a+5",  MISSING_CLOSING_QUOTE, "'a+5");
    X8("5+'a",  MISSING_CLOSING_QUOTE, "'a");
    X8("' ",    MISSING_CLOSING_QUOTE, "' ");
    X8("''",    MISSING_CLOSING_QUOTE, "''");
    X8("'''",   MISSING_CLOSING_QUOTE, "'''");
    E8("''''",  0x27);

    E16("'a'", 0x61);
    E32("'a'", 0x61);
}

static void test_dec_constant() {
    E8("0",   0x00);
    E8("127", 0x7f);
    E8("128", 0x80);
    E8("255", 0xff);
    X8("256", OVERFLOW_RANGE, "256");

    E16("0D",     0x0000);
    E16("32767D", 0x7fff);
    E16("32768d", 0x8000);
    E16("65535D", 0xffff);
    X16("65536D", OVERFLOW_RANGE, "65536D");

    E32("0d",          0x00000000);
    E32("2147483647D", 0x7fffffff);
    E32("2147483648",  0x80000000);
    E32("4294967295D", 0xffffffff);
    X32("4294967296",  OVERFLOW_RANGE, "4294967296");
    X32("9999999999D", OVERFLOW_RANGE, "9999999999D");
}

static void test_hex_constant() {
    E8("0H",   0x00);
    E8("7fH",  0x7f);
    E8("80H",  0x80);
    E8("0FFH", 0xff);
    X8("100H", OVERFLOW_RANGE, "100H");

    E16("0H",     0x0000);
    E16("7FFFH",  0x7fff);
    E16("8000H",  0x8000);
    E16("0ffffh", 0xffff);
    X16("10000H", OVERFLOW_RANGE, "10000H");
    X16("ECH",    UNDEFINED_SYMBOL, "ECH");
    E16("0ECH",   0xEC);
    E16("9ECH",   0x9EC);
    X16("AECH",   UNDEFINED_SYMBOL, "AECH");
    X16("ECHO",   UNDEFINED_SYMBOL, "ECHO");
    X16("0ECHO",  GARBAGE_AT_END,   "O");
    X16("9ECHO",  GARBAGE_AT_END,   "O");
    X16("ACHO",   UNDEFINED_SYMBOL, "ACHO");

    E32("0H",         0x00000000);
    E32("7FFFFFFFH",  0x7fffffff);
    E32("80000000H",  0x80000000);
    E32("0FFFFffffH", 0xffffffff);
    X32("100000000H", OVERFLOW_RANGE, "100000000H");

    E32("0x0",         0x00000000);
    E32("0x7FFFFFFF",  0x7fffffff);
    E32("0x80000000",  0x80000000);
    E32("0xFFFFffff",  0xffffffff);
    X32("0x100000000", OVERFLOW_RANGE, "0x100000000");
}

static void test_oct_constant() {
    E8("0O",   0x00);
    E8("177O", 0x7f);
    E8("200Q", 0x80);
    E8("377O", 0xff);
    X8("400Q", OVERFLOW_RANGE, "400Q");

    E16("0Q",      0x0000);
    E16("77777O",  0x7fff);
    E16("100000o", 0x8000);
    E16("177777q", 0xffff);
    X16("200000O", OVERFLOW_RANGE, "200000O");

    E32("0O",           0x00000000);
    E32("17777777777O", 0x7fffffff);
    E32("20000000000o", 0x80000000);
    E32("37777777777O", 0xffffffff);
    X32("40000000000Q", OVERFLOW_RANGE, "40000000000Q");

    E32("00",           0x00000000);
    E32("017777777777", 0x7fffffff);
    E32("020000000000", 0x80000000);
    E32("037777777777", 0xffffffff);
    X32("040000000000", OVERFLOW_RANGE, "040000000000");
}

static void test_bin_constant() {
    E8("0B",         0x00);
    E8("01111111b",  0x7f);
    E8("10000000B",  0x80);
    E8("11111111B",  0xff);
    X8("100000000B", OVERFLOW_RANGE, "100000000B");

    E16("0B",                 0x0000);
    E16("0111111111111111B",  0x7fff);
    E16("1000000000000000B",  0x8000);
    E16("1111111111111111b",  0xffff);
    X16("10000000000000000B", OVERFLOW_RANGE, "10000000000000000B");

    E32("0B",                                 0x00000000);
    E32("01111111111111111111111111111111B",  0x7fffffff);
    E32("10000000000000000000000000000000B",  0x80000000);
    E32("11111111111111111111111111111111B",  0xffffffff);
    X32("100000000000000000000000000000000b", OVERFLOW_RANGE, "100000000000000000000000000000000b");

    E32("0b0",                                 0x00000000);
    E32("0b01111111111111111111111111111111",  0x7fffffff);
    E32("0b10000000000000000000000000000000",  0x80000000);
    E32("0b11111111111111111111111111111111",  0xffffffff);
    X32("0b100000000000000000000000000000000", OVERFLOW_RANGE, "0b100000000000000000000000000000000");
}

static void test_unary_operator() {
    E8("NOT +0",    0xFF);
    E8("NOT (1|8)", 0xF6);
    E8("NOT -1",    0x00);

    E16("NOT 0x0",     0xFFFF);
    E16("NOT (1|010)", 0xFFF6);
    E16("NOT -0b1",    0x0000);

    E32("NOT +0",    0xFFFFFFFF);
    E32("NOT (1|8)", 0xFFFFFFF6);
    E32("NOT -1",    0x00000000);

    E16("HIGH 1234H", 0x12);
    E16("LOW  1234H", 0x34);

    E16("HIGH 1234H+1", 0x13); // (HIGH 1234H) + 1
    E16("LOW  1234H+1", 0x35); // (LOW 1234H) + 1

    E16("NOT HIGH 1234H", 0xFFED); // NOT (HIGH 1234H)
    E16("HIGH (NOT 1234H)", 0xED); // HIGH (NOT 1234H)

    E32("HIGH LSW 12345678H", 0x56); // HIGH (LSW 12345678H)
    E32("LOW MSW 12345678H", 0x34);  // LOW (MSW 12345678H)
}

static void test_binary_operator() {
    E8("( 20) MOD ( 3)",  2);
    E8("( 20) MOD (-3)",  2);
    E8("(-20) MOD ( 3)", -2);
    E8("(-20) MOD (-3)", -2);

    E8("1 SHL 0", 1);
    E8("1 SHL 7", 128);
    X8("1 SHL 8", OVERFLOW_RANGE, "1 SHL 8");
    E8("0 SHL 8", 0);
    E8("-1 SHR 8", 0);        // logical shift
    X8("-1 SHL 8", OVERFLOW_RANGE, "-1 SHL 8");
    E8("-128 SHR 1", -64);
    E8("-128 SHR 7", 255);
    E8("-128 SHR 8", 0);      // logical shift
    E8("0x80 SHR 7", 0x01);
    E8("0x80 SHR 8", 0);
    E8("0xFF SHR 4", 0x0F);
    X8("0xFF SHL 4", OVERFLOW_RANGE, "0xFF SHL 4");
    E8("(0xFF SHL 4)&0xFF", 0xF0);

    E8("0b0001 OR 0b0100", 0x05);
    E8("0b1011 AND 0b0110", 0x02);
    E8("0b0110 XOR 0b0011", 0x05);

    E32("( 20) MOD ( 3)",  2);
    E32("( 20) MOD (-3)",  2);
    E32("(-20) MOD ( 3)", -2);
    E32("(-20) MOD (-3)", -2);

    E32("0x00000001 SHL 31", 0); // 16 bit value
    E32("0x00000001 SHL 32", 0);
    E32("0x80000000 SHR 31", 0); // 16 bit value
    E32("0x80000000 SHR 32", 0);
    E32("1 SHR (-1)", 0);
    E32("1 SHL (-1)", 0);
    E32("-1 SHL 31", 0);        // 16 bit value
    E32("-1 SHL 32", 0);
    E32("-1 SHR 31", 0);        // 16 bit value
    E32("-1 SHR 32", 0);        // 16 bit value

    E32("0b0001 OR 0b0100", 0x05);
    E32("0b1011 AND 0b0110", 0x02);
    E32("0b0110 XOR -1",    0xfffffff9);
}

static void test_precedence() {
    E16("1+2-3+4",    4);
    E16("1+2*3+4",   11);
    E16("1+2-7/3",    1);
    E16("1-8 MOD 3*3",   -5);
    E16("(1+2)*3+4", 13);
    E16("1+(2-7)/3",  0);
    E16("(1-8) MOD 3*3", -3);

    E8("1 SHL 2+3", 7);  // (1 SHL 2) + 3
    E8("(1 SHL 2)+3" , 7);
    E8("1 SHL 2*3", 12); // (1 SHL 2) * 3
    E8("(1 SHL 2)*3", 12);
    E8("0x20 SHR 2+3",   11); // (0x20 SHR 2) + 3
    E8("(0x20 SHR 2)+3", 11);
    E8("0x40 SHR 2*3",   48); // (0x40 SHR 2) * 3
    E8("(0x40 SHR 2)*3", 48);

    E8("1 SHL 4 SHR 1",   8);
    E8("1 SHL (4 SHR 1)", 4);
    E8("4 SHR 1 SHL 1",   4);
    E8("4 SHR (1 SHL 1)", 1);

    E8("1 OR 2 AND 6 XOR 8", 11);
    E8("1 OR 2 XOR 4 AND 12", 7);
    E8("1 AND 3 OR 4 XOR 8", 13);
    E8("1 XOR 2 OR 4 AND 12", 7);
    E8("1 AND 3 XOR 4 OR 8", 13);
    E8("1 XOR 2 AND 6 OR 8", 11);

    E8("(1 OR 2) AND 6 XOR 8", 10);
    E8("(1 OR 2) XOR 4 AND 12", 7);
    E8("1 AND (3 OR 4) XOR 8",  9);
    E8("1 XOR (2 OR 4) AND 12", 5);
    E8("1 AND 3 XOR (4 OR 8)", 13);
    E8("1 XOR 2 AND (6 OR 8)",  3);

    E8("1 OR 2 AND (6 XOR 8)",  3);
    E8("1 OR (2 XOR 4) AND 12", 5);
    E8("1 AND 3 OR (4 XOR 8)", 13);
    E8("(1 XOR 2) OR 4 AND 12", 7);
    E8("1 AND (3 XOR 4) OR 8",  9);
    E8("(1 XOR 2) AND 6 OR 8", 10);

    E8("1 OR 2 SHL 3",   17);
    E8("(1 OR 2) SHL 3", 24);
    E8("3 AND 7 SHL 1",    2);
    E8("(3 AND 7) SHL 1",  6);
    E8("1 XOR 3 SHL 3",   25);
    E8("(1 XOR 3) SHL 3", 16);
}

static void test_current_address() {
    locator.location = 0x1000;
    E16("$",        0x1000);
    E16("$+2",      0x1002);
    E16("$-2",      0x0FFE);
    X16("$+0F000H", OVERFLOW_RANGE, "$+0F000H");
    E16("$-1001H",  0xFFFF);
    E32("$+0F000H", 0x00010000);
    E32("$-1001H",  0xFFFFFFFF);

    symtab.intern(0x1000, "table");
    locator.location = 0x1100;
    E16("$-table",     0x100);
    E16("($-table)/2", 0x080);
}

static void test_scan() {
    SCAN('|', "|a+b|c+d",     "");
    SCAN('|', "a+b|c+d",      "a+b");
    SCAN('|', "a+(b|c)+d|e",  "a+(b|c)+d");
    SCAN('|', "a+'|'+d|e",    "a+'|'+d");
    SCAN(',', "|a+b|c+d",     "");
    SCAN(',', "','+'''',abc", "','+''''");
    SCAN('x', "0x1230xG",     "0x1230");
    SCAN('b', "0b1010b0",     "0b1010");
    SCAN('H', "1AB0HHX",      "1AB0H");
    SCAN('O', "1230OOX",      "1230O");
    SCAN('O', "1239OOX",      "1239");
    SCAN('B', "1010BBX",      "1010B");
    SCAN('B', "1012BBX",      "1012");
}

static void test_errors() {
    X32("0ABCGH", ILLEGAL_CONSTANT, "0ABCGH");
    X32("01778O", GARBAGE_AT_END, "8O");
    X32("01012B", GARBAGE_AT_END, "B");
    X32("56789A", GARBAGE_AT_END, "A");
}

static void test_formatter_8bit() {
    DEC(0,    8, "0");
    DEC(32,   8, "32");
    DEC(128,  8, "128");
    DEC(255,  8, "255");
    DEC(256,  8, "0");
    DEC(-32,  8, "224");
    DEC(-128, 8, "128");
    DEC(-255, 8, "1");
    DEC(-256, 8, "0");

    DEC(0,    -8, "0");
    DEC(32,   -8, "32");
    DEC(128,  -8, "-128");
    DEC(255,  -8, "-1");
    DEC(256,  -8, "0");
    DEC(-32,  -8, "-32");
    DEC(-128, -8, "-128");
    DEC(-255, -8, "1");
    DEC(-256, -8, "0");

    UDEC(0,    8, "0");
    UDEC(32,   8, "32");
    UDEC(128,  8, "128");
    UDEC(255,  8, "255");
    UDEC(256,  8, "0");
    UDEC(-32,  8, "224");
    UDEC(-128, 8, "128");
    UDEC(-255, 8, "1");
    UDEC(-256, 8, "0");

    UDEC(0,    -8, "0");
    UDEC(32,   -8, "32");
    UDEC(128,  -8, "-128");
    UDEC(255,  -8, "-1");
    UDEC(256,  -8, "0");
    UDEC(-32,  -8, "-32");
    UDEC(-128, -8, "-128");
    UDEC(-255, -8, "1");
    UDEC(-256, -8, "0");

    BIN(0,    8, "00000000b");
    BIN(32,   8, "00100000b");
    BIN(128,  8, "10000000b");
    BIN(255,  8, "11111111b");
    BIN(256,  8, "00000000b");
    BIN(-32,  8, "11100000b");
    BIN(-128, 8, "10000000b");
    BIN(-255, 8, "00000001b");
    BIN(-256, 8, "00000000b");

    BIN(0,    -8, "00000000b");
    BIN(32,   -8, "00100000b");
    BIN(128,  -8, "-10000000b");
    BIN(255,  -8, "-00000001b");
    BIN(256,  -8, "00000000b");
    BIN(-32,  -8, "-00100000b");
    BIN(-128, -8, "-10000000b");
    BIN(-255, -8, "00000001b");
    BIN(-256, -8, "00000000b");

    UBIN(0,    8, "00000000B");
    UBIN(32,   8, "00100000B");
    UBIN(128,  8, "10000000B");
    UBIN(255,  8, "11111111B");
    UBIN(256,  8, "00000000B");
    UBIN(-32,  8, "11100000B");
    UBIN(-128, 8, "10000000B");
    UBIN(-255, 8, "00000001B");
    UBIN(-256, 8, "00000000B");

    UBIN(0,    -8, "00000000B");
    UBIN(32,   -8, "00100000B");
    UBIN(128,  -8, "-10000000B");
    UBIN(255,  -8, "-00000001B");
    UBIN(256,  -8, "00000000B");
    UBIN(-32,  -8, "-00100000B");
    UBIN(-128, -8, "-10000000B");
    UBIN(-255, -8, "00000001B");
    UBIN(-256, -8, "00000000B");

    OCT(0,    8, "000q");
    OCT(32,   8, "040q");
    OCT(128,  8, "200q");
    OCT(255,  8, "377q");
    OCT(256,  8, "000q");
    OCT(-32,  8, "340q");
    OCT(-128, 8, "200q");
    OCT(-255, 8, "001q");
    OCT(-256, 8, "000q");

    OCT(0,    -8, "000q");
    OCT(32,   -8, "040q");
    OCT(128,  -8, "-200q");
    OCT(255,  -8, "-001q");
    OCT(256,  -8, "000q");
    OCT(-32,  -8, "-040q");
    OCT(-128, -8, "-200q");
    OCT(-255, -8, "001q");
    OCT(-256, -8, "000q");

    UOCT(0,    8, "000Q");
    UOCT(32,   8, "040Q");
    UOCT(128,  8, "200Q");
    UOCT(255,  8, "377Q");
    UOCT(256,  8, "000Q");
    UOCT(-32,  8, "340Q");
    UOCT(-128, 8, "200Q");
    UOCT(-255, 8, "001Q");
    UOCT(-256, 8, "000Q");

    UOCT(0,    -8, "000Q");
    UOCT(32,   -8, "040Q");
    UOCT(128,  -8, "-200Q");
    UOCT(255,  -8, "-001Q");
    UOCT(256,  -8, "000Q");
    UOCT(-32,  -8, "-040Q");
    UOCT(-128, -8, "-200Q");
    UOCT(-255, -8, "001Q");
    UOCT(-256, -8, "000Q");

    HEX(0,    8, "00h");
    HEX(32,   8, "20h");
    HEX(128,  8, "80h");
    HEX(255,  8, "0ffh");
    HEX(256,  8, "00h");
    HEX(-32,  8, "0e0h");
    HEX(-128, 8, "80h");
    HEX(-255, 8, "01h");
    HEX(-256, 8, "00h");

    HEX(0,    -8, "00h");
    HEX(32,   -8, "20h");
    HEX(128,  -8, "-80h");
    HEX(255,  -8, "-01h");
    HEX(256,  -8, "00h");
    HEX(-32,  -8, "-20h");
    HEX(-128, -8, "-80h");
    HEX(-255, -8, "01h");
    HEX(-256, -8, "00h");

    UHEX(0,    8, "00H");
    UHEX(32,   8, "20H");
    UHEX(128,  8, "80H");
    UHEX(255,  8, "0FFH");
    UHEX(256,  8, "00H");
    UHEX(-32,  8, "0E0H");
    UHEX(-128, 8, "80H");
    UHEX(-255, 8, "01H");
    UHEX(-256, 8, "00H");

    UHEX(0,    -8, "00H");
    UHEX(32,   -8, "20H");
    UHEX(128,  -8, "-80H");
    UHEX(241,  -8, "-0FH");
    UHEX(256,  -8, "00H");
    UHEX(-32,  -8, "-20H");
    UHEX(-128, -8, "-80H");
    UHEX(-255, -8, "01H");
    UHEX(-256, -8, "00H");

    // Relax

    RHEX(0,    8, "0");
    RHEX(32,   8, "32");
    RHEX(33,   8, "21h");
    RHEX(128,  8, "80h");
    RHEX(255,  8, "0ffh");
    RHEX(256,  8, "0");
    RHEX(-32,  8, "0e0h");
    RHEX(-33,  8, "0dfh");
    RHEX(-128, 8, "80h");
    RHEX(-255, 8, "1");
    RHEX(-256, 8, "0");

    RHEX(0,    -8, "0");
    RHEX(32,   -8, "32");
    RHEX(33,   -8, "21h");
    RHEX(128,  -8, "-80h");
    RHEX(255,  -8, "-1");
    RHEX(256,  -8, "0");
    RHEX(-32,  -8, "-32");
    RHEX(-33,  -8, "-21h");
    RHEX(-128, -8, "-80h");
    RHEX(-255, -8, "1");
    RHEX(-256, -8, "0");

    URHEX(0,    8, "0");
    URHEX(32,   8, "32");
    URHEX(33,   8, "21H");
    URHEX(128,  8, "80H");
    URHEX(255,  8, "0FFH");
    URHEX(256,  8, "0");
    URHEX(-32,  8, "0E0H");
    URHEX(-33,  8, "0DFH");
    URHEX(-128, 8, "80H");
    URHEX(-255, 8, "1");
    URHEX(-256, 8, "0");

    URHEX(0,    -8, "0");
    URHEX(32,   -8, "32");
    URHEX(33,   -8, "21H");
    URHEX(128,  -8, "-80H");
    URHEX(255,  -8, "-1");
    URHEX(256,  -8, "0");
    URHEX(-32,  -8, "-32");
    URHEX(-47,  -8, "-2FH");
    URHEX(-128, -8, "-80H");
    URHEX(-255, -8, "1");
    URHEX(-256, -8, "0");
}

static void test_formatter_16bit() {
    DEC(0,        16, "0");
    DEC(32,       16, "32");
    DEC(0x8000,   16, "32768");
    DEC(0xffff,   16, "65535");
    DEC(0x10000,  16, "0");
    DEC(-32,      16, "65504");
    DEC(-0x8000,  16, "32768");
    DEC(-0xffff,  16, "1");
    DEC(-0x10000, 16, "0");

    DEC(0,        -16, "0");
    DEC(32,       -16, "32");
    DEC(0x8000,   -16, "-32768");
    DEC(0xffff,   -16, "-1");
    DEC(0x10000,  -16, "0");
    DEC(-32,      -16, "-32");
    DEC(-0x8000,  -16, "-32768");
    DEC(-0xffff,  -16, "1");
    DEC(-0x10000, -16, "0");

    BIN(0,         16, "0000000000000000b");
    BIN(32,        16, "0000000000100000b");
    BIN(0x8000,    16, "1000000000000000b");
    BIN(0xffff,    16, "1111111111111111b");
    BIN(0x10000,   16, "0000000000000000b");
    BIN(-32,       16, "1111111111100000b");
    BIN(-0x8000,   16, "1000000000000000b");
    BIN(-0xffff,   16, "0000000000000001b");
    BIN(-0x10000,  16, "0000000000000000b");

    BIN(0,        -16, "0000000000000000b");
    BIN(32,       -16, "0000000000100000b");
    BIN(0x8000,   -16, "-1000000000000000b");
    BIN(0xffff,   -16, "-0000000000000001b");
    BIN(0x10000,  -16, "0000000000000000b");
    BIN(-32,      -16, "-0000000000100000b");
    BIN(-0x8000,  -16, "-1000000000000000b");
    BIN(-0xffff,  -16, "0000000000000001b");
    BIN(-0x10000, -16, "0000000000000000b");

    OCT(0,         16, "000000q");
    OCT(32,        16, "000040q");
    OCT(0x8000,    16, "100000q");
    OCT(0xffff,    16, "177777q");
    OCT(0x10000,   16, "000000q");
    OCT(-32,       16, "177740q");
    OCT(-0x8000,   16, "100000q");
    OCT(-0xffff,   16, "000001q");
    OCT(-0x10000,  16, "000000q");

    OCT(0,        -16, "000000q");
    OCT(32,       -16, "000040q");
    OCT(0x8000,   -16, "-100000q");
    OCT(0xffff,   -16, "-000001q");
    OCT(0x10000,  -16, "000000q");
    OCT(-32,      -16, "-000040q");
    OCT(-0x8000,  -16, "-100000q");
    OCT(-0xffff,  -16, "000001q");
    OCT(-0x10000, -16, "000000q");

    HEX(0,        16, "0000h");
    HEX(32,       16, "0020h");
    HEX(0x8000,   16, "8000h");
    HEX(0xffff,   16, "0ffffh");
    HEX(0x10000,  16, "0000h");
    HEX(-32,      16, "0ffe0h");
    HEX(-0x8000,  16, "8000h");
    HEX(-0xffff,  16, "0001h");
    HEX(-0x10000, 16, "0000h");

    HEX(0,        -16, "0000h");
    HEX(32,       -16, "0020h");
    HEX(0x8000,   -16, "-8000h");
    HEX(0xffff,   -16, "-0001h");
    HEX(0x10000,  -16, "0000h");
    HEX(-32,      -16, "-0020h");
    HEX(-0x8000,  -16, "-8000h");
    HEX(-0xffff,  -16, "0001h");
    HEX(-0x10000, -16, "0000h");

    // Relax

    RHEX(0,        16, "0");
    RHEX(32,       16, "32");
    RHEX(33,       16, "0021h");
    RHEX(0x8000,   16, "8000h");
    RHEX(0xffff,   16, "0ffffh");
    RHEX(0x10000,  16, "0");
    RHEX(-32,      16, "0ffe0h");
    RHEX(-33,      16, "0ffdfh");
    RHEX(-0x8000,  16, "8000h");
    RHEX(-0xffff,  16, "1");
    RHEX(-0x10000, 16, "0");

    RHEX(0,        -16, "0");
    RHEX(32,       -16, "32");
    RHEX(33,       -16, "0021h");
    RHEX(0x8000,   -16, "-8000h");
    RHEX(0xffff,   -16, "-1");
    RHEX(0x10000,  -16, "0");
    RHEX(-32,      -16, "-32");
    RHEX(-33,      -16, "-0021h");
    RHEX(-0x8000,  -16, "-8000h");
    RHEX(-0xffff,  -16, "1");
    RHEX(-0x10000, -16, "0");
}

static void test_formatter_24bit() {
    DEC(0,           24, "0");
    DEC(32,          24, "32");
    DEC(0x800000,    24, "8388608");
    DEC(0xffffff,    24, "16777215");
    DEC(0x1000000,   24, "0");
    DEC(-32,         24, "16777184");
    DEC(-0x800000,   24, "8388608");
    DEC(-0xffffff,   24, "1");
    DEC(-0x1000000,  24, "0");

    DEC(0,          -24, "0");
    DEC(32,         -24, "32");
    DEC(0x800000,   -24, "-8388608");
    DEC(0xffffff,   -24, "-1");
    DEC(0x1000000,  -24, "0");
    DEC(-32,        -24, "-32");
    DEC(-0x800000,  -24, "-8388608");
    DEC(-0xffffff,  -24, "1");
    DEC(-0x1000000, -24, "0");

    BIN(0,           24, "000000000000000000000000b");
    BIN(32,          24, "000000000000000000100000b");
    BIN(0x800000,    24, "100000000000000000000000b");
    BIN(0xffffff,    24, "111111111111111111111111b");
    BIN(0x1000000,   24, "000000000000000000000000b");
    BIN(-32,         24, "111111111111111111100000b");
    BIN(-0x800000,   24, "100000000000000000000000b");
    BIN(-0xffffff,   24, "000000000000000000000001b");
    BIN(-0x1000000,  24, "000000000000000000000000b");

    BIN(0,          -24, "000000000000000000000000b");
    BIN(32,         -24, "000000000000000000100000b");
    BIN(0x800000,   -24, "-100000000000000000000000b");
    BIN(0xffffff,   -24, "-000000000000000000000001b");
    BIN(0x1000000,  -24, "000000000000000000000000b");
    BIN(-32,        -24, "-000000000000000000100000b");
    BIN(-0x800000,  -24, "-100000000000000000000000b");
    BIN(-0xffffff,  -24, "000000000000000000000001b");
    BIN(-0x1000000, -24, "000000000000000000000000b");

    OCT(0,           24, "00000000q");
    OCT(32,          24, "00000040q");
    OCT(0x800000,    24, "40000000q");
    OCT(0xffffff,    24, "77777777q");
    OCT(0x1000000,   24, "00000000q");
    OCT(-32,         24, "77777740q");
    OCT(-0x800000,   24, "40000000q");
    OCT(-0xffffff,   24, "00000001q");
    OCT(-0x1000000,  24, "00000000q");

    OCT(0,          -24, "00000000q");
    OCT(32,         -24, "00000040q");
    OCT(0x800000,   -24, "-40000000q");
    OCT(0xffffff,   -24, "-00000001q");
    OCT(0x1000000,  -24, "00000000q");
    OCT(-32,        -24, "-00000040q");
    OCT(-0x800000,  -24, "-40000000q");
    OCT(-0xffffff,  -24, "00000001q");
    OCT(-0x1000000, -24, "00000000q");

    HEX(0,           24, "000000h");
    HEX(32,          24, "000020h");
    HEX(0x800000,    24, "800000h");
    HEX(0xffffff,    24, "0ffffffh");
    HEX(0x1000000,   24, "000000h");
    HEX(-32,         24, "0ffffe0h");
    HEX(-0x800000,   24, "800000h");
    HEX(-0xffffff,   24, "000001h");
    HEX(-0x1000000,  24, "000000h");

    HEX(0,          -24, "000000h");
    HEX(32,         -24, "000020h");
    HEX(0x800000,   -24, "-800000h");
    HEX(0xffffff,   -24, "-000001h");
    HEX(0x1000000,  -24, "000000h");
    HEX(-32,        -24, "-000020h");
    HEX(-0x800000,  -24, "-800000h");
    HEX(-0xffffff,  -24, "000001h");
    HEX(-0x1000000, -24, "000000h");

    // Relax

    RHEX(0,           24, "0");
    RHEX(32,          24, "32");
    RHEX(33,          24, "000021h");
    RHEX(0x800000,    24, "800000h");
    RHEX(0xffffff,    24, "0ffffffh");
    RHEX(0x1000000,   24, "0");
    RHEX(-32,         24, "0ffffe0h");
    RHEX(-33,         24, "0ffffdfh");
    RHEX(-0x800000,   24, "800000h");
    RHEX(-0xffffff,   24, "1");
    RHEX(-0x1000000,  24, "0");

    RHEX(0,          -24, "0");
    RHEX(32,         -24, "32");
    RHEX(33,         -24, "000021h");
    RHEX(0x800000,   -24, "-800000h");
    RHEX(0xffffff,   -24, "-1");
    RHEX(0x1000000,  -24, "0");
    RHEX(-32,        -24, "-32");
    RHEX(-33,        -24, "-000021h");
    RHEX(-0x800000,  -24, "-800000h");
    RHEX(-0xffffff,  -24, "1");
    RHEX(-0x1000000, -24, "0");
}

static void test_formatter_32bit() {
    DEC(0,            32, "0");
    DEC(32,           32, "32");
    DEC(0x80000000,   32, "2147483648");
    DEC(0xffffffff,   32, "4294967295");
    DEC(-32,          32, "4294967264");
    DEC(-0x80000000,  32, "2147483648");
    DEC(-0xffffffff,  32, "1");

    DEC(0,           -32, "0");
    DEC(32,          -32, "32");
    DEC(0x80000000,  -32, "-2147483648");
    DEC(0xffffffff,  -32, "-1");
    DEC(-32,         -32, "-32");
    DEC(-0x80000000, -32, "-2147483648");
    DEC(-0xffffffff, -32, "1");

    BIN(0,           32, "00000000000000000000000000000000b");
    BIN(32,          32, "00000000000000000000000000100000b");
    BIN(0x80000000,  32, "10000000000000000000000000000000b");
    BIN(0xffffffff,  32, "11111111111111111111111111111111b");
    BIN(-32,         32, "11111111111111111111111111100000b");
    BIN(-0x80000000, 32, "10000000000000000000000000000000b");
    BIN(-0xffffffff, 32, "00000000000000000000000000000001b");

    BIN(0,           -32, "00000000000000000000000000000000b");
    BIN(32,          -32, "00000000000000000000000000100000b");
    BIN(0x80000000,  -32, "-10000000000000000000000000000000b");
    BIN(0xffffffff,  -32, "-00000000000000000000000000000001b");
    BIN(-32,         -32, "-00000000000000000000000000100000b");
    BIN(-0x80000000, -32, "-10000000000000000000000000000000b");
    BIN(-0xffffffff, -32, "00000000000000000000000000000001b");

    OCT(0,           32, "00000000000q");
    OCT(32,          32, "00000000040q");
    OCT(0x80000000,  32, "20000000000q");
    OCT(0xffffffff,  32, "37777777777q");
    OCT(-32,         32, "37777777740q");
    OCT(-0x80000000, 32, "20000000000q");
    OCT(-0xffffffff, 32, "00000000001q");

    OCT(0,           -32, "00000000000q");
    OCT(32,          -32, "00000000040q");
    OCT(0x80000000,  -32, "-20000000000q");
    OCT(0xffffffff,  -32, "-00000000001q");
    OCT(-32,         -32, "-00000000040q");
    OCT(-0x80000000, -32, "-20000000000q");
    OCT(-0xffffffff, -32, "00000000001q");

    HEX(0,           32, "00000000h");
    HEX(32,          32, "00000020h");
    HEX(33,          32, "00000021h");
    HEX(0x80000000,  32, "80000000h");
    HEX(0xffffffff,  32, "0ffffffffh");
    HEX(-32,         32, "0ffffffe0h");
    HEX(-33,         32, "0ffffffdfh");
    HEX(-0x80000000, 32, "80000000h");
    HEX(-0xffffffff, 32, "00000001h");

    HEX(0,           -32, "00000000h");
    HEX(32,          -32, "00000020h");
    HEX(33,          -32, "00000021h");
    HEX(0x80000000,  -32, "-80000000h");
    HEX(0xffffffff,  -32, "-00000001h");
    HEX(-32,         -32, "-00000020h");
    HEX(-33,         -32, "-00000021h");
    HEX(-0x80000000, -32, "-80000000h");
    HEX(-0xffffffff, -32, "00000001h");

    // Relax

    RHEX(0,           32, "0");
    RHEX(32,          32, "32");
    RHEX(33,          32, "00000021h");
    RHEX(0x80000000,  32, "80000000h");
    RHEX(0xffffffff,  32, "0ffffffffh");
    RHEX(-32,         32, "0ffffffe0h");
    RHEX(-33,         32, "0ffffffdfh");
    RHEX(-0x80000000, 32, "80000000h");
    RHEX(-0xffffffff, 32, "1");

    RHEX(0,           -32, "0");
    RHEX(32,          -32, "32");
    RHEX(33,          -32, "00000021h");
    RHEX(0x80000000,  -32, "-80000000h");
    RHEX(0xffffffff,  -32, "-1");
    RHEX(-32,         -32, "-32");
    RHEX(-33,         -32, "-00000021h");
    RHEX(-0x80000000, -32, "-80000000h");
    RHEX(-0xffffffff, -32, "1");

    // Uppercase

    UHEX(0,           32, "00000000H");
    UHEX(32,          32, "00000020H");
    UHEX(0x80000000,  32, "80000000H");
    UHEX(0xffffffff,  32, "0FFFFFFFFH");
    UHEX(-32,         32, "0FFFFFFE0H");
    UHEX(-0x80000000, 32, "80000000H");
    UHEX(-0xffffffff, 32, "00000001H");

    UHEX(0,           -32, "00000000H");
    UHEX(32,          -32, "00000020H");
    UHEX(0x80000000,  -32, "-80000000H");
    UHEX(0xffffffff,  -32, "-00000001H");
    UHEX(-32,         -32, "-00000020H");
    UHEX(-0x80000000, -32, "-80000000H");
    UHEX(-0xffffffff, -32, "00000001H");

    // Uppercase and Relax

    URHEX(0,           32, "0");
    URHEX(32,          32, "32");
    URHEX(33,          32, "00000021H");
    URHEX(0x80000000,  32, "80000000H");
    URHEX(0xffffffff,  32, "0FFFFFFFFH");
    URHEX(-32,         32, "0FFFFFFE0H");
    URHEX(-33,         32, "0FFFFFFDFH");
    URHEX(-0x80000000, 32, "80000000H");
    URHEX(-0xffffffff, 32, "1");

    URHEX(0,           -32, "0");
    URHEX(32,          -32, "32");
    URHEX(33,          -32, "00000021H");
    URHEX(0x80000000,  -32, "-80000000H");
    URHEX(0xffffffff,  -32, "-1");
    URHEX(-32,         -32, "-32");
    URHEX(-33,         -32, "-00000021H");
    URHEX(-0x80000000, -32, "-80000000H");
    URHEX(-0xffffffff, -32, "1");

}
// clang-format on

void run_tests() {
    RUN_TEST(test_char_constant);
    RUN_TEST(test_dec_constant);
    RUN_TEST(test_hex_constant);
    RUN_TEST(test_oct_constant);
    RUN_TEST(test_bin_constant);
    RUN_TEST(test_unary_operator);
    RUN_TEST(test_binary_operator);
    RUN_TEST(test_precedence);
    RUN_TEST(test_current_address);
    RUN_TEST(test_scan);
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
