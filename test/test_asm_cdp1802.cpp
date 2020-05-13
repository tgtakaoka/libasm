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

#include "asm_cdp1802.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::cdp1802;
using namespace libasm::test;

TestAsserter asserter;
TestSymtab symtab;
AsmCdp1802 asm1802;
Assembler &assembler(asm1802);

static void set_up() {
}

static void tear_down() {
    symtab.reset();
}

static void test_cpu() {
    asserter.equals(
        "cpu 1802", true, assembler.setCpu("1802"));
    asserter.equals(
        "get cpu", "1802", assembler.getCpu());

    asserter.equals(
        "cpu CDP1802", true, assembler.setCpu("CDP1802"));
    asserter.equals(
        "get cpu", "1802", assembler.getCpu());
}

static void test_inherent() {
    TEST("IDL",  0x00);
    TEST("NOP",  0xC4);
    TEST("IRX",  0x60);
    TEST("SAV",  0x78);
    TEST("MARK", 0x79);
    TEST("REQ",  0x7A);
    TEST("SEQ",  0x7B);
    TEST("SHRC", 0x76);
    TEST("SHLC", 0x7E);
    TEST("SHR",  0xF6);
    TEST("SHL",  0xFE);
}

static void test_indexed() {
    TEST("RET",  0x70);
    TEST("DIS",  0x71);
    TEST("LDXA", 0x72);
    TEST("STXD", 0x73);
    TEST("ADC",  0x74);
    TEST("SDB",  0x75);
    TEST("SMB",  0x77);
    TEST("LDX",  0xF0);
    TEST("OR ",  0xF1);
    TEST("AND",  0xF2);
    TEST("XOR",  0xF3);
    TEST("ADD",  0xF4);
    TEST("SD ",  0xF5);
    TEST("SM ",  0xF7);
}

static void test_register() {
    ETEST(ILLEGAL_REGISTER, "LDN 0");
    TEST("LDN 1",  0x01);
    TEST("LDN 2",  0x02);
    TEST("LDN 3",  0x03);
    TEST("LDN 4",  0x04);
    TEST("LDN 5",  0x05);
    TEST("LDN 6",  0x06);
    TEST("LDN 7",  0x07);
    TEST("LDN 8",  0x08);
    TEST("LDN 9",  0x09);
    TEST("LDN 10", 0x0A);
    TEST("LDN 11", 0x0B);
    TEST("LDN 12", 0x0C);
    TEST("LDN 13", 0x0D);
    TEST("LDN 14", 0x0E);
    TEST("LDN 15", 0x0F);
    ETEST(ILLEGAL_REGISTER, "LDN 16");

    TEST("INC 0",  0x10);
    TEST("INC 1",  0x11);
    TEST("INC 2",  0x12);
    TEST("INC 3",  0x13);
    TEST("INC 4",  0x14);
    TEST("INC 5",  0x15);
    TEST("INC 6",  0x16);
    TEST("INC 7",  0x17);
    TEST("INC 8",  0x18);
    TEST("INC 9",  0x19);
    TEST("INC 10", 0x1A);
    TEST("INC 11", 0x1B);
    TEST("INC 12", 0x1C);
    TEST("INC 13", 0x1D);
    TEST("INC 14", 0x1E);
    TEST("INC 15", 0x1F);

    TEST("DEC 0",  0x20);
    TEST("DEC 1",  0x21);
    TEST("DEC 2",  0x22);
    TEST("DEC 3",  0x23);
    TEST("DEC 4",  0x24);
    TEST("DEC 5",  0x25);
    TEST("DEC 6",  0x26);
    TEST("DEC 7",  0x27);
    TEST("DEC 8",  0x28);
    TEST("DEC 9",  0x29);
    TEST("DEC 10", 0x2A);
    TEST("DEC 11", 0x2B);
    TEST("DEC 12", 0x2C);
    TEST("DEC 13", 0x2D);
    TEST("DEC 14", 0x2E);
    TEST("DEC 15", 0x2F);

    TEST("LDA 0",  0x40);
    TEST("LDA 1",  0x41);
    TEST("LDA 2",  0x42);
    TEST("LDA 3",  0x43);
    TEST("LDA 4",  0x44);
    TEST("LDA 5",  0x45);
    TEST("LDA 6",  0x46);
    TEST("LDA 7",  0x47);
    TEST("LDA 8",  0x48);
    TEST("LDA 9",  0x49);
    TEST("LDA 10", 0x4A);
    TEST("LDA 11", 0x4B);
    TEST("LDA 12", 0x4C);
    TEST("LDA 13", 0x4D);
    TEST("LDA 14", 0x4E);
    TEST("LDA 15", 0x4F);

    TEST("STR 0",  0x50);
    TEST("STR 1",  0x51);
    TEST("STR 2",  0x52);
    TEST("STR 3",  0x53);
    TEST("STR 4",  0x54);
    TEST("STR 5",  0x55);
    TEST("STR 6",  0x56);
    TEST("STR 7",  0x57);
    TEST("STR 8",  0x58);
    TEST("STR 9",  0x59);
    TEST("STR 10", 0x5A);
    TEST("STR 11", 0x5B);
    TEST("STR 12", 0x5C);
    TEST("STR 13", 0x5D);
    TEST("STR 14", 0x5E);
    TEST("STR 15", 0x5F);

    TEST("GLO 0",  0x80);
    TEST("GLO 1",  0x81);
    TEST("GLO 2",  0x82);
    TEST("GLO 3",  0x83);
    TEST("GLO 4",  0x84);
    TEST("GLO 5",  0x85);
    TEST("GLO 6",  0x86);
    TEST("GLO 7",  0x87);
    TEST("GLO 8",  0x88);
    TEST("GLO 9",  0x89);
    TEST("GLO 10", 0x8A);
    TEST("GLO 11", 0x8B);
    TEST("GLO 12", 0x8C);
    TEST("GLO 13", 0x8D);
    TEST("GLO 14", 0x8E);
    TEST("GLO 15", 0x8F);

    TEST("GHI 0",  0x90);
    TEST("GHI 1",  0x91);
    TEST("GHI 2",  0x92);
    TEST("GHI 3",  0x93);
    TEST("GHI 4",  0x94);
    TEST("GHI 5",  0x95);
    TEST("GHI 6",  0x96);
    TEST("GHI 7",  0x97);
    TEST("GHI 8",  0x98);
    TEST("GHI 9",  0x99);
    TEST("GHI 10", 0x9A);
    TEST("GHI 11", 0x9B);
    TEST("GHI 12", 0x9C);
    TEST("GHI 13", 0x9D);
    TEST("GHI 14", 0x9E);
    TEST("GHI 15", 0x9F);

    TEST("PLO 0",  0xA0);
    TEST("PLO 1",  0xA1);
    TEST("PLO 2",  0xA2);
    TEST("PLO 3",  0xA3);
    TEST("PLO 4",  0xA4);
    TEST("PLO 5",  0xA5);
    TEST("PLO 6",  0xA6);
    TEST("PLO 7",  0xA7);
    TEST("PLO 8",  0xA8);
    TEST("PLO 9",  0xA9);
    TEST("PLO 10", 0xAA);
    TEST("PLO 11", 0xAB);
    TEST("PLO 12", 0xAC);
    TEST("PLO 13", 0xAD);
    TEST("PLO 14", 0xAE);
    TEST("PLO 15", 0xAF);

    TEST("PHI 0",  0xB0);
    TEST("PHI 1",  0xB1);
    TEST("PHI 2",  0xB2);
    TEST("PHI 3",  0xB3);
    TEST("PHI 4",  0xB4);
    TEST("PHI 5",  0xB5);
    TEST("PHI 6",  0xB6);
    TEST("PHI 7",  0xB7);
    TEST("PHI 8",  0xB8);
    TEST("PHI 9",  0xB9);
    TEST("PHI 10", 0xBA);
    TEST("PHI 11", 0xBB);
    TEST("PHI 12", 0xBC);
    TEST("PHI 13", 0xBD);
    TEST("PHI 14", 0xBE);
    TEST("PHI 15", 0xBF);

    TEST("SEP 0",  0xD0);
    TEST("SEP 1",  0xD1);
    TEST("SEP 2",  0xD2);
    TEST("SEP 3",  0xD3);
    TEST("SEP 4",  0xD4);
    TEST("SEP 5",  0xD5);
    TEST("SEP 6",  0xD6);
    TEST("SEP 7",  0xD7);
    TEST("SEP 8",  0xD8);
    TEST("SEP 9",  0xD9);
    TEST("SEP 10", 0xDA);
    TEST("SEP 11", 0xDB);
    TEST("SEP 12", 0xDC);
    TEST("SEP 13", 0xDD);
    TEST("SEP 14", 0xDE);
    TEST("SEP 15", 0xDF);

    TEST("SEX 0",  0xE0);
    TEST("SEX 1",  0xE1);
    TEST("SEX 2",  0xE2);
    TEST("SEX 3",  0xE3);
    TEST("SEX 4",  0xE4);
    TEST("SEX 5",  0xE5);
    TEST("SEX 6",  0xE6);
    TEST("SEX 7",  0xE7);
    TEST("SEX 8",  0xE8);
    TEST("SEX 9",  0xE9);
    TEST("SEX 10", 0xEA);
    TEST("SEX 11", 0xEB);
    TEST("SEX 12", 0xEC);
    TEST("SEX 13", 0xED);
    TEST("SEX 14", 0xEE);
    TEST("SEX 15", 0xEF);

    symtab.intern(0, "R0");
    symtab.intern(3, "PC");
    symtab.intern(16, "XX");

    ETEST(ILLEGAL_REGISTER, "LDN R0");
    ETEST(ILLEGAL_REGISTER, "LDN XX");
    TEST("LDN PC", 0x03);
    TEST("SEP PC", 0xD3);
}

static void test_immediate() {
    TEST("ADCI 0ABH", 0x7C, 0xAB);
    TEST("SDBI 0CDH", 0x7D, 0xCD);
    TEST("SMBI 0EFH", 0x7F, 0xEF);
    TEST("LDI  12H",  0xF8, 0x12);
    TEST("ORI  34H",  0xF9, 0x34);
    TEST("ANI  45H",  0xFA, 0x45);
    TEST("XRI  67H",  0xFB, 0x67);
    TEST("ADI  89H",  0xFC, 0x89);
    TEST("SDI  0ABH", 0xFD, 0xAB);
    TEST("SMI  0CDH", 0xFF, 0xCD);

    symtab.intern(-1, "neg1");
    symtab.intern(1,  "pos1");
    TEST("ADI neg1", 0xFC, 0xFF);
    TEST("SDI pos1", 0xFD, 0x01);
}

static void test_io() {
    ETEST(OVERFLOW_RANGE, "OUT 0");
    TEST("OUT 1", 0x61);
    TEST("OUT 2", 0x62);
    TEST("OUT 3", 0x63);
    TEST("OUT 4", 0x64);
    TEST("OUT 5", 0x65);
    TEST("OUT 6", 0x66);
    TEST("OUT 7", 0x67);
    ETEST(OVERFLOW_RANGE, "OUT 8");
    ETEST(OVERFLOW_RANGE, "INP 0");
    TEST("INP 1", 0x69);
    TEST("INP 2", 0x6A);
    TEST("INP 3", 0x6B);
    TEST("INP 4", 0x6C);
    TEST("INP 5", 0x6D);
    TEST("INP 6", 0x6E);
    TEST("INP 7", 0x6F);
    ETEST(OVERFLOW_RANGE, "INP 8");

    symtab.intern(1, "STDOUT");
    symtab.intern(2, "STDIN");
    TEST("OUT STDOUT", 0x61);
    TEST("INP STDIN",  0x6A);
}

static void test_branch() {
    ATEST(0x1000, "BR  1031H", 0x30, 0x31);
    ATEST(0x1000, "BZ  1033H", 0x32, 0x33);
    ATEST(0x1000, "BDF 1034H", 0x33, 0x34);
    ATEST(0x1000, "B1  1035H", 0x34, 0x35);
    ATEST(0x1000, "B2  1036H", 0x35, 0x36);
    ATEST(0x1000, "B3  1037H", 0x36, 0x37);
    ATEST(0x1000, "B4  1038H", 0x37, 0x38);
    ATEST(0x1000, "BNQ 103AH", 0x39, 0x3A);
    ATEST(0x1000, "BNZ 103BH", 0x3A, 0x3B);
    ATEST(0x1000, "BNF 103CH", 0x3B, 0x3C);
    ATEST(0x1000, "BN1 103DH", 0x3C, 0x3D);
    ATEST(0x1000, "BN2 103EH", 0x3D, 0x3E);
    ATEST(0x1000, "BN3 103FH", 0x3E, 0x3F);
    ATEST(0x1000, "BN4 1040H", 0x3F, 0x40);

    ATEST(0x10FD, "BR  1031H", 0x30, 0x31);
    ATEST(0x10FE, "BR  1131H", 0x30, 0x31);
    ATEST(0x10FF, "BR  1131H", 0x30, 0x31);

    TEST("LBR  1234H",  0xC0, 0x12, 0x34);
    TEST("LBQ  5678H",  0xC1, 0x56, 0x78);
    TEST("LBZ  9ABCH",  0xC2, 0x9A, 0xBC);
    TEST("LBDF 0DEF0H", 0xC3, 0xDE, 0xF0);
    TEST("LBNQ 1234H",  0xC9, 0x12, 0x34);
    TEST("LBNZ 5678H",  0xCA, 0x56, 0x78);
    TEST("LBNF 9ABCH",  0xCB, 0x9A, 0xBC);

    symtab.intern(0x1031, "sym1031");
    symtab.intern(0x1131, "sym1131");
    symtab.intern(0x1234, "sym1234");

    ATEST(0x10FD, "BR sym1031", 0x30, 0x31);
    ATEST(0x10FE, "BR sym1131", 0x30, 0x31);
    ATEST(0x10FF, "BR sym1131", 0x30, 0x31);
    EATEST(OPERAND_TOO_FAR, 0x10FE, "BR sym1031");
    EATEST(OPERAND_TOO_FAR, 0x10FF, "BR sym1031");
    EATEST(OPERAND_TOO_FAR, 0x1100, "BR sym1031");

    TEST("LBQ sym1234",  0xC1, 0x12, 0x34);

    TEST("SKP",  0x31);
    TEST("LSNQ", 0xC5);
    TEST("LSNZ", 0xC6);
    TEST("LSNF", 0xC7);
    TEST("LSKP", 0xC8);
    TEST("LSIE", 0xCC);
    TEST("LSQ",  0xCD);
    TEST("LSZ",  0xCE);
    TEST("LSDF", 0xCF);
}

static void test_comment() {
    TEST("NOP; comment",     0xC4);
    TEST("RET    ; comment", 0x70);
    TEST("LDN  1 ; comment", 0x01);
    TEST("DEC  15; comment", 0x2F);
    TEST("LDI 12H; comment", 0xF8, 0x12);
    TEST("INP 4  ; comment", 0x6C);
    TEST("BNQ 3AH; comment", 0x39, 0x3A);
    TEST("LBZ 3AH; comment", 0xC2, 0x00, 0x3A);
    TEST("LSKP   ; comment", 0xC8);
}

static void test_undefined_symbol() {
    ETEST(UNDEFINED_SYMBOL, "LDN UNDEF", 0x07); // Default work register
    ETEST(UNDEFINED_SYMBOL, "OUT UNDEF", 0x61); // Default IO
    ETEST(UNDEFINED_SYMBOL, "INP UNDEF", 0x69); // Default IO

    ETEST(UNDEFINED_SYMBOL, "ADCI UNDEF", 0x7C, 0x00);

    EATEST(UNDEFINED_SYMBOL, 0x10FD, "BR  UNDEF", 0x30, 0xFF);
    EATEST(UNDEFINED_SYMBOL, 0x10FE, "BR  UNDEF", 0x30, 0x00);
    EATEST(UNDEFINED_SYMBOL, 0x10FF, "BR  UNDEF", 0x30, 0x01);
    EATEST(UNDEFINED_SYMBOL, 0x1100, "BR  UNDEF", 0x30, 0x02);

    EATEST(UNDEFINED_SYMBOL, 0x1234, "LBR UNDEF", 0xC0, 0x00, 0x00);
}

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_cpu);
    RUN_TEST(test_inherent);
    RUN_TEST(test_indexed);
    RUN_TEST(test_register);
    RUN_TEST(test_immediate);
    RUN_TEST(test_io);
    RUN_TEST(test_branch);
    RUN_TEST(test_comment);
    RUN_TEST(test_undefined_symbol);
    return 0;
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
