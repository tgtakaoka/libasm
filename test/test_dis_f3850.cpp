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

#include "dis_f3850.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::f3850;
using namespace libasm::test;

DisF3850 dis3850;
Disassembler &disassembler(dis3850);

void set_up() {
    disassembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 3850", true,   disassembler.setCpu("3850"));
    EQUALS_P("cpu 3850", "3850", disassembler.config().cpu_P());

    EQUALS("cpu F3850", true,   disassembler.setCpu("F3850"));
    EQUALS_P("cpu F3850", "3850", disassembler.config().cpu_P());
}

void test_accumlator() {
    TEST("SR",  "1",  0x12);
    TEST("SR",  "4",  0x14);
    TEST("SL",  "1",  0x13);
    TEST("SL",  "4",  0x15);
    TEST("COM", "",   0x18);
    TEST("LNK", "",   0x19);
    TEST("INC", "",   0x1F);
    TEST("CLR", "",   0x70);
    TEST("LIS", "1",  0x71);
    TEST("LIS", "15", 0x7F);
    TEST("LI",  "0",      0x20, 0x00);
    TEST("LI",  "H'FF'",  0x20, 0xFF);
    TEST("NI",  "1",      0x21, 0x01);
    TEST("NI",  "H'FE'",  0x21, 0xFE);
    TEST("OI",  "2",      0x22, 0x02);
    TEST("OI",  "H'FD'",  0x22, 0xFD);
    TEST("XI",  "3",      0x23, 0x03);
    TEST("XI",  "H'FC'",  0x23, 0xFC);
    TEST("AI",  "4",      0x24, 0x04);
    TEST("AI",  "H'FB'",  0x24, 0xFB);
    TEST("CI",  "5",      0x25, 0x05);
    NMEM("CI",  "0", "0", 0x25);
    disassembler.setOption("intel-hex", "on");
    TEST("CI",  "0FAH",   0x25, 0xFA);
}

void test_status() {
    TEST("LR",  "W, J", 0x1D);
    TEST("LR",  "J, W", 0x1E);
}

void test_indirect_scratchpad() {
    TEST("LR",   "A, IS", 0x0A);
    TEST("LR",   "IS, A", 0x0B);
    TEST("LISU", "0",     0x60);
    TEST("LISU", "7",     0x67);
    TEST("LISL", "0",     0x68);
    TEST("LISL", "7",     0x6F);
}

void test_scratchpad() {
    TEST("LR", "A, 0",  0x40);
    TEST("LR", "A, J",  0x49);
    TEST("LR", "A, HU", 0x4A);
    TEST("LR", "A, HL", 0x4B);
    TEST("LR", "A, S",  0x4C);
    TEST("LR", "A, I",  0x4D);
    TEST("LR", "A, D",  0x4E);
    ERRT("LR", "A, 15", OPERAND_NOT_ALLOWED, "15", 0x4F);
    TEST("LR", "0, A",  0x50);
    TEST("LR", "J, A",  0x59);
    TEST("LR", "HU, A", 0x5A);
    TEST("LR", "HL, A", 0x5B);
    TEST("LR", "S, A",  0x5C);
    TEST("LR", "I, A",  0x5D);
    TEST("LR", "D, A",  0x5E);
    ERRT("LR", "15, A", OPERAND_NOT_ALLOWED, "15, A", 0x5F);

    TEST("LR", "A, KU", 0x00);
    TEST("LR", "A, KL", 0x01);
    TEST("LR", "A, QU", 0x02);
    TEST("LR", "A, QL", 0x03);
    TEST("LR", "KU, A", 0x04);
    TEST("LR", "KL, A", 0x05);
    TEST("LR", "QU, A", 0x06);
    TEST("LR", "QL, A", 0x07);

    TEST("AS", "0",  0xC0);
    ERRT("AS", "15", OPERAND_NOT_ALLOWED, "15", 0xCF);
    TEST("AS", "J",  0xC9);
    TEST("AS", "HU", 0xCA);
    TEST("AS", "HL", 0xCB);
    TEST("AS", "S",  0xCC);
    TEST("AS", "I",  0xCD);
    TEST("AS", "D",  0xCE);

    TEST("ASD", "1",  0xD1);
    ERRT("ASD", "15", OPERAND_NOT_ALLOWED, "15", 0xDF);
    TEST("ASD", "J",  0xD9);
    TEST("ASD", "HU", 0xDA);
    TEST("ASD", "HL", 0xDB);
    TEST("ASD", "S",  0xDC);
    TEST("ASD", "I",  0xDD);
    TEST("ASD", "D",  0xDE);

    TEST("NS", "8",  0xF8);
    ERRT("NS", "15", OPERAND_NOT_ALLOWED, "15", 0xFF);
    TEST("NS", "J",  0xF9);
    TEST("NS", "HU", 0xFA);
    TEST("NS", "HL", 0xFB);
    TEST("NS", "S",  0xFC);
    TEST("NS", "I",  0xFD);
    TEST("NS", "D",  0xFE);

    TEST("XS", "2",  0xE2);
    ERRT("XS", "15", OPERAND_NOT_ALLOWED, "15", 0xEF);
    TEST("XS", "J",  0xE9);
    TEST("XS", "HU", 0xEA);
    TEST("XS", "HL", 0xEB);
    TEST("XS", "S",  0xEC);
    TEST("XS", "I",  0xED);
    TEST("XS", "D",  0xEE);

    TEST("DS", "7",  0x37);
    ERRT("DS", "15", OPERAND_NOT_ALLOWED, "15", 0x3F);
    TEST("DS", "J",  0x39);
    TEST("DS", "HU", 0x3A);
    TEST("DS", "HL", 0x3B);
    TEST("DS", "S",  0x3C);
    TEST("DS", "I",  0x3D);
    TEST("DS", "D",  0x3E);
}

void test_data_counter() {
    TEST("LR", "Q, DC", 0x0E);
    TEST("LR", "H, DC", 0x11);
    TEST("LR", "DC, Q", 0x0F);
    TEST("LR", "DC, H", 0x10);

    TEST("ADC", "",        0x8E);
    TEST("DCI", "H'1234'",            0x2A, 0x12, 0x34);
    NMEM("DCI", "H'1200'", "H'1200'", 0x2A, 0x12);
    NMEM("DCI", "H'0000'", "H'0000'", 0x2A);
    TEST("XDC", "",        0x2C);
}

void test_memory_reference() {
    TEST("LM", "", 0x16);
    TEST("ST", "", 0x17);
    TEST("AM", "", 0x88);
    TEST("AMD", "", 0x89);
    TEST("NM", "", 0x8A);
    TEST("OM", "", 0x8B);
    TEST("XM", "", 0x8C);
    TEST("CM", "", 0x8D);
}

void test_program_counter() {
    TEST("LR", "K, P",    0x08);
    TEST("LR", "P, K",    0x09);
    TEST("LR", "P0, Q",   0x0D);
    TEST("PK", "",        0x0C);
    TEST("PI", "H'ABCD'",            0x28, 0xAB, 0xCD);
    NMEM("PI", "H'AB00'", "H'AB00'", 0x28, 0xAB);
    NMEM("PI", "H'0000'", "H'0000'", 0x28);
    TEST("POP", "",        0x1C);
}

void test_branch() {
    ATEST(0x1000, "BR",  "H'1000'",            0x90, 0xFF);
    ANMEM(0x1000, "BR",  "H'1001'", "H'1001'", 0x90);
    ATEST(0x1000, "JMP", "H'1234'",            0x29, 0x12, 0x34);
    ANMEM(0x1000, "JMP", "H'1200'", "H'1200'", 0x29, 0x12);
    ANMEM(0x1000, "JMP", "H'0000'", "H'0000'", 0x29);

    ATEST(0x1000, "BT",  "0, H'1002'", 0x80, 0x01);
    ATEST(0x1000, "BP",  "H'1080'",    0x81, 0x7F);
    ATEST(0x1000, "BC",  "H'0F81'",    0x82, 0x80);
    ATEST(0x1000, "BT",  "3, H'1002'", 0x83, 0x01);
    ATEST(0x1000, "BZ",  "H'1002'",    0x84, 0x01);
    ATEST(0x1000, "BT",  "5, H'1002'", 0x85, 0x01);
    ATEST(0x1000, "BT",  "6, H'1002'", 0x86, 0x01);
    ATEST(0x1000, "BT",  "7, H'1002'", 0x87, 0x01);
    ATEST(0x1000, "BR7", "H'1002'",    0x8F, 0x01);

    ATEST(0x1000, "BR",  "H'1002'",     0x90, 0x01);
    ATEST(0x1000, "BM",  "H'1080'",     0x91, 0x7F);
    ATEST(0x1000, "BNC", "H'0F81'",     0x92, 0x80);
    ATEST(0x1000, "BF",  "3, H'1002'",  0x93, 0x01);
    ATEST(0x1000, "BNZ", "H'1002'",     0x94, 0x01);
    ATEST(0x1000, "BF",  "5, H'1002'",  0x95, 0x01);
    ATEST(0x1000, "BF",  "6, H'1002'",  0x96, 0x01);
    ATEST(0x1000, "BF",  "7, H'1002'",  0x97, 0x01);
    ATEST(0x1000, "BNO", "H'1002'",     0x98, 0x01);
    ATEST(0x1000, "BF",  "9, H'1080'",  0x99, 0x7F);
    ATEST(0x1000, "BF",  "10, H'0F81'", 0x9A, 0x80);
    ATEST(0x1000, "BF",  "11, H'1002'", 0x9B, 0x01);
    ATEST(0x1000, "BF",  "12, H'1002'", 0x9C, 0x01);
    disassembler.setOption("relative", "on");
    ATEST(0x1000, "BF",  "13, $+2",     0x9D, 0x01);
    disassembler.setOption("origin-char", ".");
    ATEST(0x1000, "BF",  "14, .+2",     0x9E, 0x01);
    disassembler.setOption("origin-char", "");
    ATEST(0x1000, "BF",  "15, $+2",     0x9F, 0x01);
}

void test_io() {
    TEST("INS",  "0",  0xA0);
    TEST("INS",  "1",  0xA1);
    ERRT("INS",  "2",  OPERAND_NOT_ALLOWED, "2", 0xA2);
    ERRT("INS",  "3",  OPERAND_NOT_ALLOWED, "3", 0xA3);
    TEST("INS",  "4",  0xA4);
    TEST("INS",  "15", 0xAF);
    ERRT("IN",   "0",  OPERAND_NOT_ALLOWED, "0", 0x26, 0x00);
    ERRT("IN",   "1",  OPERAND_NOT_ALLOWED, "1", 0x26, 0x01);
    ERRT("IN",   "2",  OPERAND_NOT_ALLOWED, "2", 0x26, 0x02);
    ERRT("IN",   "3",  OPERAND_NOT_ALLOWED, "3", 0x26, 0x03);
    TEST("IN",   "4",      0x26, 0x04);
    TEST("IN",   "H'FF'",  0x26, 0xFF);
    NMEM("IN",   "0", "0", 0x26);
    TEST("OUTS", "0",  0xB0);
    TEST("OUTS", "1",  0xB1);
    ERRT("OUTS", "2",  OPERAND_NOT_ALLOWED, "2", 0xB2);
    ERRT("OUTS", "3",  OPERAND_NOT_ALLOWED, "3", 0xB3);
    TEST("OUTS", "4",  0xB4);
    TEST("OUTS", "15", 0xBF);
    ERRT("OUT",  "0",  OPERAND_NOT_ALLOWED, "0", 0x27, 0x00);
    ERRT("OUT",  "1",  OPERAND_NOT_ALLOWED, "1", 0x27, 0x01);
    ERRT("OUT",  "2",  OPERAND_NOT_ALLOWED, "2", 0x27, 0x02);
    ERRT("OUT",  "3",  OPERAND_NOT_ALLOWED, "3", 0x27, 0x03);
    TEST("OUT",  "4",  0x27, 0x04);
    TEST("OUT",  "H'AB'",  0x27, 0xAB);
    TEST("OUT",  "H'AB'",  0x27, 0xAB);
    NMEM("OUT",  "0", "0", 0x27);
}

void test_control() {
    TEST("DI",  "", 0x1A);
    TEST("EI",  "", 0x1B);
    TEST("NOP", "", 0x2B);
}

void test_illegal() {
    UNKN(0x2D);
    UNKN(0x2E);
    UNKN(0x2F);
}
// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);

    RUN_TEST(test_accumlator);
    RUN_TEST(test_status);
    RUN_TEST(test_indirect_scratchpad);
    RUN_TEST(test_scratchpad);
    RUN_TEST(test_data_counter);
    RUN_TEST(test_memory_reference);
    RUN_TEST(test_program_counter);
    RUN_TEST(test_branch);
    RUN_TEST(test_io);
    RUN_TEST(test_control);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
