/*
 * Copyright 2023 Tadashi G. Takaoka
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

const struct ZilogPlugins : ValueParser::Plugins {
    const NumberParser &number() const override { return ZilogNumberParser::singleton(); }
    const LetterParser &letter() const override { return ZilogLetterParser::singleton(); }
    const OperatorParser &operators() const override { return ZilogOperatorParser::singleton(); }
} plugins{};
struct : ValueParser::Locator {
    uint32_t location = 0;
    uint32_t currentLocation() const { return location; }
} locator;
const ValueParser parser{plugins, locator};

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
    E8("'%%'",  0x25);
    E8("'%q'",  0x27);
    E8("'%t'",  0x09);
    E8("'%l'",  0x0A);
    E8("'%p'",  0x0C);
    E8("'%r'",  0x0D);
    X8("'%x'",  UNKNOWN_ESCAPE_SEQUENCE, "'%x'");
    E8("'%00'", 0x00);
    E8("'%FF'", 0xFF);
    X8("'%0'",  UNKNOWN_ESCAPE_SEQUENCE, "'%0'");
    X8("'%a'",  UNKNOWN_ESCAPE_SEQUENCE, "'%a'");
    X8("'%f'",  UNKNOWN_ESCAPE_SEQUENCE, "'%f'");
    X8("'%1x'", UNKNOWN_ESCAPE_SEQUENCE, "'%1x'");
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
    E8("%0",   0x00);
    E8("%7f",  0x7f);
    E8("%80",  0x80);
    E8("%0FF", 0xff);
    X8("%100", OVERFLOW_RANGE, "%100");

    E16("%0",     0x0000);
    E16("%7FFF",  0x7fff);
    E16("%8000",  0x8000);
    E16("%0ffff", 0xffff);
    X16("%10000", OVERFLOW_RANGE, "%10000");
    E16("%EC",    0xEC);
    E16("%0EC",   0xEC);
    E16("%9EC",   0x9EC);
    E16("%AEC",   0xAEC);

    E32("%0",         0x00000000);
    E32("%7FFFFFFF",  0x7fffffff);
    E32("%80000000",  0x80000000);
    E32("%0FFFFffff", 0xffffffff);
    X32("%100000000", OVERFLOW_RANGE, "%100000000");

    E32("0x0",         0x00000000);
    E32("0x7FFFFFFF",  0x7fffffff);
    E32("0x80000000",  0x80000000);
    E32("0xFFFFffff",  0xffffffff);
    X32("0x100000000", OVERFLOW_RANGE, "0x100000000");
}

static void test_oct_constant() {
    E8("%(8)0",   0x00);
    E8("%(8)177", 0x7f);
    E8("%(8)200", 0x80);
    E8("%(8)377", 0xff);
    X8("%(8)400", OVERFLOW_RANGE, "%(8)400");

    E16("%(8)0",      0x0000);
    E16("%(8)77777",  0x7fff);
    E16("%(8)100000", 0x8000);
    E16("%(8)177777", 0xffff);
    X16("%(8)200000", OVERFLOW_RANGE, "%(8)200000");

    E32("%(8)0",           0x00000000);
    E32("%(8)17777777777", 0x7fffffff);
    E32("%(8)20000000000", 0x80000000);
    E32("%(8)37777777777", 0xffffffff);
    X32("%(8)40000000000", OVERFLOW_RANGE, "%(8)40000000000");

    E32("00",           0x00000000);
    E32("017777777777", 0x7fffffff);
    E32("020000000000", 0x80000000);
    E32("037777777777", 0xffffffff);
    X32("040000000000", OVERFLOW_RANGE, "040000000000");
}

static void test_bin_constant() {
    E8("%(2)0",         0x00);
    E8("%(2)01111111",  0x7f);
    E8("%(2)10000000",  0x80);
    E8("%(2)11111111",  0xff);
    X8("%(2)100000000", OVERFLOW_RANGE, "%(2)100000000");

    E16("%(2)0",                 0x0000);
    E16("%(2)0111111111111111",  0x7fff);
    E16("%(2)1000000000000000",  0x8000);
    E16("%(2)1111111111111111",  0xffff);
    X16("%(2)10000000000000000", OVERFLOW_RANGE, "%(2)10000000000000000");

    E32("%(2)0",                                 0x00000000);
    E32("%(2)01111111111111111111111111111111",  0x7fffffff);
    E32("%(2)10000000000000000000000000000000",  0x80000000);
    E32("%(2)11111111111111111111111111111111",  0xffffffff);
    X32("%(2)100000000000000000000000000000000", OVERFLOW_RANGE, "%(2)100000000000000000000000000000000");

    E32("0b0",                                 0x00000000);
    E32("0b01111111111111111111111111111111",  0x7fffffff);
    E32("0b10000000000000000000000000000000",  0x80000000);
    E32("0b11111111111111111111111111111111",  0xffffffff);
    X32("0b100000000000000000000000000000000", OVERFLOW_RANGE, "0b100000000000000000000000000000000");
}

static void test_unary_operator() {
    E8("LNOT +0",    0xFF);
    E8("LNOT (1|8)", 0xF6);
    E8("LNOT -1",    0x00);

    E16("LNOT 0x0",     0xFFFF);
    E16("LNOT (1|010)", 0xFFF6);
    E16("LNOT -0b1",    0x0000);

    E32("LNOT +0",    0xFFFFFFFF);
    E32("LNOT (1|8)", 0xFFFFFFF6);
    E32("LNOT -1",    0x00000000);

    E16("LNOT HIGH 1234H", 0xFFED); // LNOT (HIGH 1234H)
    E16("HIGH LNOT 1234H", 0xED);   // HIGH (LNOT 1234H)

    E32("HIGH LOW16 12345678H", 0x56); // HIGH (LOW16 12345678H)
    E32("LOW HIGH16 12345678H", 0x34); // LOW (HIGH16 12345678H)
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
    E8("-1 SHR 8", 255);        // (-1) SHR 8
    E8("-(1 SHR 8)", 0);
    X8("-1 SHL 8", OVERFLOW_RANGE, "-1 SHL 8");
    X8("-128 SHR 1", OVERFLOW_RANGE, "-128 SHR 1");
    X8("-128 SHR 7", OVERFLOW_RANGE, "-128 SHR 7");
    E8("-128 SHR 8", 255);      // (-128) SHR 8
    E8("%80 SHR 7", 0x01);
    E8("%80 SHR 8", 0);
    E8("%FF SHR 4", 0x0F);
    X8("%FF SHL 4", OVERFLOW_RANGE, "%FF SHL 4");
    E8("(%FF SHL 4)&%FF", 0xF0);

    E8("%(2)0001 LOR  %(2)0100", 0x05);
    E8("%(2)1011 LAND %(2)0110", 0x02);
    E8("%(2)0110 LXOR %(2)0011", 0x05);

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

    E32("%(2)0001 LOR  %(2)0100", 0x05);
    E32("%(2)1011 LAND %(2)0110", 0x02);
    E32("%(2)0110 LXOR -1",    0xfffffff9);
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
    E8("%20 SHR 2+3",   11); // (%20 SHR 2) + 3
    E8("(%20 SHR 2)+3", 11);
    E8("%40 SHR 2*3",   48); // (%40 SHR 2) * 3
    E8("(%40 SHR 2)*3", 48);

    E8("1 SHL 4 SHR 1",   8);
    E8("1 SHL (4 SHR 1)", 4);
    E8("4 SHR 1 SHL 1",   4);
    E8("4 SHR (1 SHL 1)", 1);

    E8("1 LOR 2 LAND 6 LXOR 8", 11);
    E8("1 LOR 2 LXOR 4 LAND 12", 7);
    E8("1 LAND 3 LOR 4 LXOR 8", 13);
    E8("1 LXOR 2 LOR 4 LAND 12", 7);
    E8("1 LAND 3 LXOR 4 LOR 8", 13);
    E8("1 LXOR 2 LAND 6 LOR 8", 11);

    E8("(1 LOR 2) LAND 6 LXOR 8", 10);
    E8("(1 LOR 2) LXOR 4 LAND 12", 7);
    E8("1 LAND (3 LOR 4) LXOR 8",  9);
    E8("1 LXOR (2 LOR 4) LAND 12", 5);
    E8("1 LAND 3 LXOR (4 LOR 8)", 13);
    E8("1 LXOR 2 LAND (6 LOR 8)",  3);

    E8("1 LOR 2 LAND (6 LXOR 8)",  3);
    E8("1 LOR (2 LXOR 4) LAND 12", 5);
    E8("1 LAND 3 LOR (4 LXOR 8)", 13);
    E8("(1 LXOR 2) LOR 4 LAND 12", 7);
    E8("1 LAND (3 LXOR 4) LOR 8",  9);
    E8("(1 LXOR 2) LAND 6 LOR 8", 10);

    E8(" 1 LOR 2  SHL 3", 17);
    E8("(1 LOR 2) SHL 3", 24);
    E8("3 LAND  7 SHL 1",  6);
    E8("3 LAND (7 SHL 1)", 2);
    E8("15 SHR  2 LAND 2",  2);
    E8("15 SHR (2 LAND 2)", 3);
    E8(" 1 LXOR 3  SHL 3", 25);
    E8("(1 LXOR 3) SHL 3", 16);
}

static void test_current_address() {
    locator.location = 0x1000;
    E16("$",        0x1000);
    E16("$+2",      0x1002);
    E16("$-2",      0x0FFE);
    X16("$+%F000", OVERFLOW_RANGE, "$+%F000");
    E16("$-%1001", 0xFFFF);
    E32("$+%F000", 0x00010000);
    E32("$-%1001", 0xFFFFFFFF);

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
    SCAN(',', "','+'%2C',abc", "','+'%2C'");
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
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
