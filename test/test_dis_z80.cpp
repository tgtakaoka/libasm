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

#include "dis_z80.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::z80;
using namespace libasm::test;

DisZ80 disz80;
Disassembler &disassembler(disz80);

bool z180() {
    return strcasecmp_P("Z180", disassembler.config().cpu_P()) == 0;
}

void set_up() {
    disassembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu Z80", true, disassembler.setCpu("z80"));
    EQUALS_P("get cpu", "Z80", disassembler.config().cpu_P());

    EQUALS("cpu Z180", true, disassembler.setCpu("z180"));
    EQUALS_P("get cpu", "Z180", disassembler.config().cpu_P());
}

void test_load_registers() {
    TEST("LD", "A, A", 0x7F);
    TEST("LD", "A, B", 0x78);
    TEST("LD", "A, C", 0x79);
    TEST("LD", "A, D", 0x7A);
    TEST("LD", "A, E", 0x7B);
    TEST("LD", "A, H", 0x7C);
    TEST("LD", "A, L", 0x7D);

    TEST("LD", "B, A", 0x47);
    TEST("LD", "B, B", 0x40);
    TEST("LD", "B, C", 0x41);
    TEST("LD", "B, D", 0x42);
    TEST("LD", "B, E", 0x43);
    TEST("LD", "B, H", 0x44);
    TEST("LD", "B, L", 0x45);

    TEST("LD", "C, A", 0x4F);
    TEST("LD", "C, B", 0x48);
    TEST("LD", "C, C", 0x49);
    TEST("LD", "C, D", 0x4A);
    TEST("LD", "C, E", 0x4B);
    TEST("LD", "C, H", 0x4C);
    TEST("LD", "C, L", 0x4D);

    TEST("LD", "D, A", 0x57);
    TEST("LD", "D, B", 0x50);
    TEST("LD", "D, C", 0x51);
    TEST("LD", "D, D", 0x52);
    TEST("LD", "D, E", 0x53);
    TEST("LD", "D, H", 0x54);
    TEST("LD", "D, L", 0x55);

    TEST("LD", "E, A", 0x5F);
    TEST("LD", "E, B", 0x58);
    TEST("LD", "E, C", 0x59);
    TEST("LD", "E, D", 0x5A);
    TEST("LD", "E, E", 0x5B);
    TEST("LD", "E, H", 0x5C);
    TEST("LD", "E, L", 0x5D);

    TEST("LD", "H, A", 0x67);
    TEST("LD", "H, B", 0x60);
    TEST("LD", "H, C", 0x61);
    TEST("LD", "H, D", 0x62);
    TEST("LD", "H, E", 0x63);
    TEST("LD", "H, H", 0x64);
    TEST("LD", "H, L", 0x65);

    TEST("LD", "L, A", 0x6F);
    TEST("LD", "L, B", 0x68);
    TEST("LD", "L, C", 0x69);
    TEST("LD", "L, D", 0x6A);
    TEST("LD", "L, E", 0x6B);
    TEST("LD", "L, H", 0x6C);
    TEST("LD", "L, L", 0x6D);

    TEST("LD", "I, A", 0xED, 0x47);
    TEST("LD", "R, A", 0xED, 0x4F);
    TEST("LD", "A, I", 0xED, 0x57);
    TEST("LD", "A, R", 0xED, 0x5F);
}

void test_move_immediate() {
    TEST("LD", "B, 0F6H", 0x06, 0xF6);
    TEST("LD", "C, 9FH",  0x0E, 0x9F);
    TEST("LD", "D, 3AH",  0x16, 0x3A);
    TEST("LD", "E, 80H",  0x1E, 0x80);
    TEST("LD", "H, 0F6H", 0x26, 0xF6);
    TEST("LD", "L, 0F6H", 0x2E, 0xF6);

    TEST("LD", "A, 0FEH", 0x3E, 0xFE);
    TEST("LD", "A, 80H",  0x3E, 0x80);
    NMEM("LD", "A, 0", "0", 0x3E);

    TEST("LD", "BC, 0BEEFH", 0x01, 0xEF, 0xBE);
    TEST("LD", "DE, 1234H",  0x11, 0x34, 0x12);
    TEST("LD", "HL, 0BEEFH", 0x21, 0xEF, 0xBE);
    TEST("LD", "SP, 6789H",  0x31, 0x89, 0x67);
    TEST("LD", "IX, 0ABCDH", 0xDD, 0x21, 0xCD, 0xAB);
    TEST("LD", "IY, 0ABCDH", 0xFD, 0x21, 0xCD, 0xAB);
    NMEM("LD", "SP, 0089H", "0089H", 0x31, 0x89);
    NMEM("LD", "SP, 0",         "0", 0x31);
}

void test_load() {
    TEST("LD", "A, (HL)", 0x7E);
    TEST("LD", "B, (HL)", 0x46);
    TEST("LD", "C, (HL)", 0x4E);
    TEST("LD", "D, (HL)", 0x56);
    TEST("LD", "E, (HL)", 0x5E);
    TEST("LD", "H, (HL)", 0x66);
    TEST("LD", "L, (HL)", 0x6E);

    TEST("LD", "A, (BC)", 0x0A);
    TEST("LD", "A, (DE)", 0x1A);
    TEST("LD", "A, (1234H)", 0x3A, 0x34, 0x12);

    TEST("LD", "A, (IX+2)", 0xDD, 0x7E, 0x02);
    TEST("LD", "B, (IX+2)", 0xDD, 0x46, 0x02);
    TEST("LD", "C, (IX+2)", 0xDD, 0x4E, 0x02);
    TEST("LD", "D, (IX+2)", 0xDD, 0x56, 0x02);
    TEST("LD", "E, (IX+2)", 0xDD, 0x5E, 0x02);
    TEST("LD", "H, (IX+2)", 0xDD, 0x66, 0x02);
    TEST("LD", "L, (IX+2)", 0xDD, 0x6E, 0x02);
    TEST("LD", "A, (IY-2)", 0xFD, 0x7E, 0xFE);
    TEST("LD", "B, (IY-2)", 0xFD, 0x46, 0xFE);
    TEST("LD", "C, (IY-2)", 0xFD, 0x4E, 0xFE);
    TEST("LD", "D, (IY-2)", 0xFD, 0x56, 0xFE);
    TEST("LD", "E, (IY-2)", 0xFD, 0x5E, 0xFE);
    TEST("LD", "H, (IY-2)", 0xFD, 0x66, 0xFE);
    TEST("LD", "L, (IY-2)", 0xFD, 0x6E, 0xFE);

    TEST("LD", "BC, (5678H)", 0xED, 0x4B, 0x78, 0x56);
    TEST("LD", "DE, (5678H)", 0xED, 0x5B, 0x78, 0x56);
    TEST("LD", "HL, (5678H)", 0x2A, 0x78, 0x56);
    TEST("LD", "SP, (5678H)", 0xED, 0x7B, 0x78, 0x56);
    TEST("LD", "IX, (5678H)", 0xDD, 0x2A, 0x78, 0x56);
    TEST("LD", "IY, (5678H)", 0xFD, 0x2A, 0x78, 0x56);
    NMEM("LD", "SP, (0078H)", "0078H)", 0xED, 0x7B, 0x78);
    NMEM("LD", "SP, (0000H)", "0000H)", 0xED, 0x7B);
    NMEM("",   "",                  "", 0xED);
}

void test_store() {
    TEST("LD", "(HL), A", 0x77);
    TEST("LD", "(HL), B", 0x70);
    TEST("LD", "(HL), C", 0x71);
    TEST("LD", "(HL), D", 0x72);
    TEST("LD", "(HL), E", 0x73);
    TEST("LD", "(HL), H", 0x74);
    TEST("LD", "(HL), L", 0x75);

    TEST("LD", "(BC), A", 0x02);
    TEST("LD", "(DE), A", 0x12);
    TEST("LD", "(9ABCH), A", 0x32, 0xBC, 0x9A);

    TEST("LD", "(IX+2), A", 0xDD, 0x77, 0x02);
    TEST("LD", "(IX+2), B", 0xDD, 0x70, 0x02);
    TEST("LD", "(IX+2), C", 0xDD, 0x71, 0x02);
    TEST("LD", "(IX+2), D", 0xDD, 0x72, 0x02);
    TEST("LD", "(IX+0), E", 0xDD, 0x73, 0x00);
    TEST("LD", "(IX+2), H", 0xDD, 0x74, 0x02);
    TEST("LD", "(IX+2), L", 0xDD, 0x75, 0x02);
    TEST("LD", "(IY-2), A", 0xFD, 0x77, 0xFE);
    TEST("LD", "(IY-2), B", 0xFD, 0x70, 0xFE);
    TEST("LD", "(IY-2), C", 0xFD, 0x71, 0xFE);
    TEST("LD", "(IY-2), D", 0xFD, 0x72, 0xFE);
    TEST("LD", "(IY-2), E", 0xFD, 0x73, 0xFE);
    TEST("LD", "(IY-2), H", 0xFD, 0x74, 0xFE);
    TEST("LD", "(IY-2), L", 0xFD, 0x75, 0xFE);

    TEST("LD", "(HL), 0F6H",   0x36, 0xF6);
    TEST("LD", "(IX+2), 0F6H", 0xDD, 0x36, 0x02, 0xF6);
    TEST("LD", "(IY-2), 0F6H", 0xFD, 0x36, 0xFE, 0xF6);

    TEST("LD", "(0ABCDH), BC", 0xED, 0x43, 0xCD, 0xAB);
    TEST("LD", "(0ABCDH), DE", 0xED, 0x53, 0xCD, 0xAB);
    TEST("LD", "(0ABCDH), HL", 0x22, 0xCD, 0xAB);
    TEST("LD", "(0ABCDH), SP", 0xED, 0x73, 0xCD, 0xAB);
    TEST("LD", "(0ABCDH), IX", 0xDD, 0x22, 0xCD, 0xAB);
    TEST("LD", "(0ABCDH), IY", 0xFD, 0x22, 0xCD, 0xAB);
}

void test_exchange() {
    TEST("EX", "DE, HL",  0xEB);
    TEST("EX", "AF, AF'", 0x08);
    TEST("EXX", "",       0xD9);

    TEST("EX", "(SP), HL", 0xE3);
    TEST("EX", "(SP), IX", 0xDD, 0xE3);
    TEST("EX", "(SP), IY", 0xFD, 0xE3);
}

void test_stack() {
    TEST("PUSH", "BC", 0xC5);
    TEST("PUSH", "DE", 0xD5);
    TEST("PUSH", "HL", 0xE5);
    TEST("PUSH", "AF", 0xF5);
    TEST("PUSH", "IX", 0xDD, 0xE5);
    TEST("PUSH", "IY", 0xFD, 0xE5);

    TEST("POP", "BC", 0xC1);
    TEST("POP", "DE", 0xD1);
    TEST("POP", "HL", 0xE1);
    TEST("POP", "AF", 0xF1);
    TEST("POP", "IX", 0xDD, 0xE1);
    TEST("POP", "IY", 0xFD, 0xE1);

    TEST("RET", "",   0xC9);
    TEST("RET", "NZ", 0xC0);
    TEST("RET", "Z",  0xC8);
    TEST("RET", "NC", 0xD0);
    TEST("RET", "C",  0xD8);
    TEST("RET", "PO", 0xE0);
    TEST("RET", "PE", 0xE8);
    TEST("RET", "P",  0xF0);
    TEST("RET", "M",  0xF8);

    TEST("RETN", "", 0xED, 0x45);
    TEST("RETI", "", 0xED, 0x4D);
}

void test_jump() {
    TEST("JP", "1234H",     0xC3, 0x34, 0x12);
    TEST("JP", "NZ, 1234H", 0xC2, 0x34, 0x12);
    TEST("JP", "Z, 1234H",  0xCA, 0x34, 0x12);
    TEST("JP", "NC, 1234H", 0xD2, 0x34, 0x12);
    TEST("JP", "C, 1234H",  0xDA, 0x34, 0x12);
    TEST("JP", "PO, 1234H", 0xE2, 0x34, 0x12);
    TEST("JP", "PE, 1234H", 0xEA, 0x34, 0x12);
    TEST("JP", "P, 1234H",  0xF2, 0x34, 0x12);
    TEST("JP", "M, 1234H",  0xFA, 0x34, 0x12);

    TEST("JP", "(HL)", 0xE9);
    TEST("JP", "(IX)", 0xDD, 0xE9);
    TEST("JP", "(IY)", 0xFD, 0xE9);

    TEST("CALL", "1234H",     0xCD, 0x34, 0x12);
    TEST("CALL", "NZ, 1234H", 0xC4, 0x34, 0x12);
    TEST("CALL", "Z, 1234H",  0xCC, 0x34, 0x12);
    TEST("CALL", "NC, 1234H", 0xD4, 0x34, 0x12);
    TEST("CALL", "C, 1234H",  0xDC, 0x34, 0x12);
    TEST("CALL", "PO, 1234H", 0xE4, 0x34, 0x12);
    TEST("CALL", "PE, 1234H", 0xEC, 0x34, 0x12);
    TEST("CALL", "P, 1234H",  0xF4, 0x34, 0x12);
    TEST("CALL", "M, 1234H",  0xFC, 0x34, 0x12);

    ATEST(0x1000, "JR", "1000H",     0x18, 0xFE);
    ATEST(0x1000, "JR", "NZ, 1004H", 0x20, 0x02);
    ATEST(0x1000, "JR", "Z, 1081H",  0x28, 0x7F);
    ATEST(0x1000, "JR", "NC, 0F82H", 0x30, 0x80);
    ATEST(0x1000, "JR", "C, 0F82H",  0x38, 0x80);
    ATEST(0x1000, "JR", "C, 1081H",  0x38, 0x7F);
    ATEST(0x1000, "JR", "C, 0F82H",  0x38, 0x80);
    AERRT(0xFF80, "JR", "C, 0001H",  OVERFLOW_RANGE, "0001H",  0x38, 0x7F);
    AERRT(0x007C, "JR", "C, 0FFFEH", OVERFLOW_RANGE, "0FFFEH", 0x38, 0x80);

    ATEST(0x1000, "DJNZ", "1000H",  0x10, 0xFE);
    ATEST(0x1000, "DJNZ", "1081H",  0x10, 0x7F);
    ATEST(0x1000, "DJNZ", "0F82H",  0x10, 0x80);
    AERRT(0xFF80, "DJNZ", "0001H",  OVERFLOW_RANGE, "0001H",  0x10, 0x7F);
    AERRT(0x007C, "DJNZ", "0FFFEH", OVERFLOW_RANGE, "0FFFEH", 0x10, 0x80);
}

void test_incr_decr() {
    TEST("INC", "A", 0x3C);
    TEST("INC", "B", 0x04);
    TEST("INC", "C", 0x0C);
    TEST("INC", "D", 0x14);
    TEST("INC", "E", 0x1C);
    TEST("INC", "H", 0x24);
    TEST("INC", "L", 0x2C);
    TEST("INC", "(HL)", 0x34);
    TEST("INC", "(IX+2)", 0xDD, 0x34, 0x02);
    TEST("INC", "(IY-2)", 0xFD, 0x34, 0xFE);

    TEST("DEC", "A", 0x3D);
    TEST("DEC", "B", 0x05);
    TEST("DEC", "C", 0x0D);
    TEST("DEC", "D", 0x15);
    TEST("DEC", "E", 0x1D);
    TEST("DEC", "H", 0x25);
    TEST("DEC", "L", 0x2D);
    TEST("DEC", "(HL)", 0x35);
    TEST("DEC", "(IX+2)", 0xDD, 0x35, 0x02);
    TEST("DEC", "(IY-2)", 0xFD, 0x35, 0xFE);

    TEST("INC", "BC", 0x03);
    TEST("INC", "DE", 0x13);
    TEST("INC", "HL", 0x23);
    TEST("INC", "SP", 0x33);
    TEST("INC", "IX", 0xDD, 0x23);
    TEST("INC", "IY", 0xFD, 0x23);

    TEST("DEC", "BC", 0x0B);
    TEST("DEC", "DE", 0x1B);
    TEST("DEC", "HL", 0x2B);
    TEST("DEC", "SP", 0x3B);
    TEST("DEC", "IX", 0xDD, 0x2B);
    TEST("DEC", "IY", 0xFD, 0x2B);
}

void test_alu_8bit() {
    TEST("RLCA", "", 0x07);
    TEST("RLC", "A", 0xCB, 0x07);
    TEST("RLC", "B", 0xCB, 0x00);
    TEST("RLC", "C", 0xCB, 0x01);
    TEST("RLC", "D", 0xCB, 0x02);
    TEST("RLC", "E", 0xCB, 0x03);
    TEST("RLC", "H", 0xCB, 0x04);
    TEST("RLC", "L", 0xCB, 0x05);
    TEST("RLC", "(HL)", 0xCB, 0x06);
    TEST("RLC", "(IX+127)", 0xDD, 0xCB, 0x7F, 0x06);
    TEST("RLC", "(IY-128)", 0xFD, 0xCB, 0x80, 0x06);

    TEST("RRCA", "", 0x0F);
    TEST("RRC", "A", 0xCB, 0x0F);
    TEST("RRC", "B", 0xCB, 0x08);
    TEST("RRC", "C", 0xCB, 0x09);
    TEST("RRC", "D", 0xCB, 0x0A);
    TEST("RRC", "E", 0xCB, 0x0B);
    TEST("RRC", "H", 0xCB, 0x0C);
    TEST("RRC", "L", 0xCB, 0x0D);
    TEST("RRC", "(HL)", 0xCB, 0x0E);
    TEST("RRC", "(IX+127)", 0xDD, 0xCB, 0x7F, 0x0E);
    TEST("RRC", "(IY-128)", 0xFD, 0xCB, 0x80, 0x0E);

    TEST("RLA", "", 0x17);
    TEST("RL", "A", 0xCB, 0x17);
    TEST("RL", "B", 0xCB, 0x10);
    TEST("RL", "C", 0xCB, 0x11);
    TEST("RL", "D", 0xCB, 0x12);
    TEST("RL", "E", 0xCB, 0x13);
    TEST("RL", "H", 0xCB, 0x14);
    TEST("RL", "L", 0xCB, 0x15);
    TEST("RL", "(HL)", 0xCB, 0x16);
    TEST("RL", "(IX+127)", 0xDD, 0xCB, 0x7F, 0x16);
    TEST("RL", "(IY-128)", 0xFD, 0xCB, 0x80, 0x16);

    TEST("RRA", "", 0x1F);
    TEST("RR", "A", 0xCB, 0x1F);
    TEST("RR", "B", 0xCB, 0x18);
    TEST("RR", "C", 0xCB, 0x19);
    TEST("RR", "D", 0xCB, 0x1A);
    TEST("RR", "E", 0xCB, 0x1B);
    TEST("RR", "H", 0xCB, 0x1C);
    TEST("RR", "L", 0xCB, 0x1D);
    TEST("RR", "(HL)", 0xCB, 0x1E);
    TEST("RR", "(IX+127)", 0xDD, 0xCB, 0x7F, 0x1E);
    TEST("RR", "(IY-128)", 0xFD, 0xCB, 0x80, 0x1E);

    TEST("SLA", "A", 0xCB, 0x27);
    TEST("SLA", "B", 0xCB, 0x20);
    TEST("SLA", "C", 0xCB, 0x21);
    TEST("SLA", "D", 0xCB, 0x22);
    TEST("SLA", "E", 0xCB, 0x23);
    TEST("SLA", "H", 0xCB, 0x24);
    TEST("SLA", "L", 0xCB, 0x25);
    TEST("SLA", "(HL)", 0xCB, 0x26);
    TEST("SLA", "(IX+127)", 0xDD, 0xCB, 0x7F, 0x26);
    TEST("SLA", "(IY-128)", 0xFD, 0xCB, 0x80, 0x26);

    TEST("SRA", "A", 0xCB, 0x2F);
    TEST("SRA", "B", 0xCB, 0x28);
    TEST("SRA", "C", 0xCB, 0x29);
    TEST("SRA", "D", 0xCB, 0x2A);
    TEST("SRA", "E", 0xCB, 0x2B);
    TEST("SRA", "H", 0xCB, 0x2C);
    TEST("SRA", "L", 0xCB, 0x2D);
    TEST("SRA", "(HL)", 0xCB, 0x2E);
    TEST("SRA", "(IX+127)", 0xDD, 0xCB, 0x7F, 0x2E);
    TEST("SRA", "(IY-128)", 0xFD, 0xCB, 0x80, 0x2E);

    TEST("SRL", "A", 0xCB, 0x3F);
    TEST("SRL", "B", 0xCB, 0x38);
    TEST("SRL", "C", 0xCB, 0x39);
    TEST("SRL", "D", 0xCB, 0x3A);
    TEST("SRL", "E", 0xCB, 0x3B);
    TEST("SRL", "H", 0xCB, 0x3C);
    TEST("SRL", "L", 0xCB, 0x3D);
    TEST("SRL", "(HL)", 0xCB, 0x3E);
    TEST("SRL", "(IX+127)", 0xDD, 0xCB, 0x7F, 0x3E);
    TEST("SRL", "(IY-128)", 0xFD, 0xCB, 0x80, 0x3E);

    TEST("RRD", "", 0xED, 0x67);
    TEST("RLD", "", 0xED, 0x6F);

    TEST("DAA", "", 0x27);
    TEST("NEG", "", 0xED, 0x44);

    TEST("ADD", "A, A", 0x87);
    TEST("ADD", "A, B", 0x80);
    TEST("ADD", "A, C", 0x81);
    TEST("ADD", "A, D", 0x82);
    TEST("ADD", "A, E", 0x83);
    TEST("ADD", "A, H", 0x84);
    TEST("ADD", "A, L", 0x85);
    TEST("ADD", "A, 2", 0xC6, 0x02);
    TEST("ADD", "A, (HL)", 0x86);
    TEST("ADD", "A, (IX+2)", 0xDD, 0x86, 0x02);
    TEST("ADD", "A, (IY-2)", 0xFD, 0x86, 0xFE);

    TEST("ADC", "A, A", 0x8F);
    TEST("ADC", "A, B", 0x88);
    TEST("ADC", "A, C", 0x89);
    TEST("ADC", "A, D", 0x8A);
    TEST("ADC", "A, E", 0x8B);
    TEST("ADC", "A, H", 0x8C);
    TEST("ADC", "A, L", 0x8D);
    TEST("ADC", "A, 0FFH", 0xCE, 0xFF);
    TEST("ADC", "A, (HL)", 0x8E);
    TEST("ADC", "A, (IX+2)", 0xDD, 0x8E, 0x02);
    TEST("ADC", "A, (IY-2)", 0xFD, 0x8E, 0xFE);

    TEST("SUB", "A", 0x97);
    TEST("SUB", "B", 0x90);
    TEST("SUB", "C", 0x91);
    TEST("SUB", "D", 0x92);
    TEST("SUB", "E", 0x93);
    TEST("SUB", "H", 0x94);
    TEST("SUB", "L", 0x95);
    TEST("SUB", "0FEH", 0xD6, 0xFE);
    TEST("SUB", "(HL)", 0x96);
    TEST("SUB", "(IX+2)", 0xDD, 0x96, 0x02);
    TEST("SUB", "(IY-2)", 0xFD, 0x96, 0xFE);

    TEST("SBC", "A, A", 0x9F);
    TEST("SBC", "A, B", 0x98);
    TEST("SBC", "A, C", 0x99);
    TEST("SBC", "A, D", 0x9A);
    TEST("SBC", "A, E", 0x9B);
    TEST("SBC", "A, H", 0x9C);
    TEST("SBC", "A, L", 0x9D);
    TEST("SBC", "A, 7FH", 0xDE, 0x7F);
    TEST("SBC", "A, (HL)", 0x9E);
    TEST("SBC", "A, (IX+2)", 0xDD, 0x9E, 0x02);
    TEST("SBC", "A, (IY-2)", 0xFD, 0x9E, 0xFE);

    TEST("AND", "A", 0xA7);
    TEST("AND", "B", 0xA0);
    TEST("AND", "C", 0xA1);
    TEST("AND", "D", 0xA2);
    TEST("AND", "E", 0xA3);
    TEST("AND", "H", 0xA4);
    TEST("AND", "L", 0xA5);
    TEST("AND", "0F0H", 0xE6, 0xF0);
    TEST("AND", "(HL)", 0xA6);
    TEST("AND", "(IX+2)", 0xDD, 0xA6, 0x02);
    TEST("AND", "(IY-2)", 0xFD, 0xA6, 0xFE);

    TEST("XOR", "A", 0xAF);
    TEST("XOR", "B", 0xA8);
    TEST("XOR", "C", 0xA9);
    TEST("XOR", "D", 0xAA);
    TEST("XOR", "E", 0xAB);
    TEST("XOR", "H", 0xAC);
    TEST("XOR", "L", 0xAD);
    TEST("XOR", "0FEH", 0xEE, 0xFE);
    TEST("XOR", "(HL)", 0xAE);
    TEST("XOR", "(IX+2)", 0xDD, 0xAE, 0x02);
    TEST("XOR", "(IY-2)", 0xFD, 0xAE, 0xFE);

    TEST("OR", "A", 0xB7);
    TEST("OR", "B", 0xB0);
    TEST("OR", "C", 0xB1);
    TEST("OR", "D", 0xB2);
    TEST("OR", "E", 0xB3);
    TEST("OR", "H", 0xB4);
    TEST("OR", "L", 0xB5);
    TEST("OR", "7FH",  0xF6, 0x7F);
    TEST("OR", "(HL)", 0xB6);
    TEST("OR", "(IX+2)", 0xDD, 0xB6, 0x02);
    TEST("OR", "(IY-2)", 0xFD, 0xB6, 0xFE);

    TEST("CP", "A", 0xBF);
    TEST("CP", "B", 0xB8);
    TEST("CP", "C", 0xB9);
    TEST("CP", "D", 0xBA);
    TEST("CP", "E", 0xBB);
    TEST("CP", "H", 0xBC);
    TEST("CP", "L", 0xBD);
    TEST("CP", "80H",  0xFE, 0x80);
    TEST("CP", "(HL)", 0xBE);
    TEST("CP", "(IX+2)", 0xDD, 0xBE, 0x02);
    TEST("CP", "(IY-2)", 0xFD, 0xBE, 0xFE);
}

void test_alu_16bit() {
    TEST("ADD", "HL, BC", 0x09);
    TEST("ADD", "HL, DE", 0x19);
    TEST("ADD", "HL, HL", 0x29);
    TEST("ADD", "HL, SP", 0x39);
    TEST("ADD", "IX, BC", 0xDD, 0x09);
    TEST("ADD", "IX, DE", 0xDD, 0x19);
    TEST("ADD", "IX, IX", 0xDD, 0x29);
    TEST("ADD", "IX, SP", 0xDD, 0x39);
    TEST("ADD", "IY, BC", 0xFD, 0x09);
    TEST("ADD", "IY, DE", 0xFD, 0x19);
    TEST("ADD", "IY, IY", 0xFD, 0x29);
    TEST("ADD", "IY, SP", 0xFD, 0x39);

    TEST("ADC", "HL, BC", 0xED, 0x4A);
    TEST("ADC", "HL, DE", 0xED, 0x5A);
    TEST("ADC", "HL, HL", 0xED, 0x6A);
    TEST("ADC", "HL, SP", 0xED, 0x7A);

    TEST("SBC", "HL, BC", 0xED, 0x42);
    TEST("SBC", "HL, DE", 0xED, 0x52);
    TEST("SBC", "HL, HL", 0xED, 0x62);
    TEST("SBC", "HL, SP", 0xED, 0x72);

    if (z180()) {
        TEST("TST", "A", 0xED, 0x3C);
        TEST("TST", "B", 0xED, 0x04);
        TEST("TST", "C", 0xED, 0x0C);
        TEST("TST", "D", 0xED, 0x14);
        TEST("TST", "E", 0xED, 0x1C);
        TEST("TST", "H", 0xED, 0x24);
        TEST("TST", "L", 0xED, 0x2C);
        TEST("TST", "80H", 0xED, 0x64, 0x80);

        TEST("MLT", "BC", 0xED, 0x4C);
        TEST("MLT", "DE", 0xED, 0x5C);
        TEST("MLT", "HL", 0xED, 0x6C);
        TEST("MLT", "SP", 0xED, 0x7C);
    }
}

void test_io() {
    TEST("IN",  "A, (0F0H)", 0xDB, 0xF0);
    TEST("OUT", "(0F1H), A", 0xD3, 0xF1);

    TEST("IN", "A, (C)", 0xED, 0x78);
    TEST("IN", "B, (C)", 0xED, 0x40);
    TEST("IN", "C, (C)", 0xED, 0x48);
    TEST("IN", "D, (C)", 0xED, 0x50);
    TEST("IN", "E, (C)", 0xED, 0x58);
    TEST("IN", "H, (C)", 0xED, 0x60);
    TEST("IN", "L, (C)", 0xED, 0x68);

    TEST("OUT", "(C), A", 0xED, 0x79);
    TEST("OUT", "(C), B", 0xED, 0x41);
    TEST("OUT", "(C), C", 0xED, 0x49);
    TEST("OUT", "(C), D", 0xED, 0x51);
    TEST("OUT", "(C), E", 0xED, 0x59);
    TEST("OUT", "(C), H", 0xED, 0x61);
    TEST("OUT", "(C), L", 0xED, 0x69);
    if (z180()) {
        TEST("TSTIO", "23H", 0xED, 0x74, 0x23);

        TEST("IN0",  "A, (0F0H)", 0xED, 0x38, 0xF0);
        TEST("IN0",  "B, (0F0H)", 0xED, 0x00, 0xF0);
        TEST("IN0",  "C, (0F0H)", 0xED, 0x08, 0xF0);
        TEST("IN0",  "D, (0F0H)", 0xED, 0x10, 0xF0);
        TEST("IN0",  "E, (0F0H)", 0xED, 0x18, 0xF0);
        TEST("IN0",  "H, (0F0H)", 0xED, 0x20, 0xF0);
        TEST("IN0",  "L, (0F0H)", 0xED, 0x28, 0xF0);

        TEST("OUT0", "(0F1H), A", 0xED, 0x39, 0xF1);
        TEST("OUT0", "(0F1H), B", 0xED, 0x01, 0xF1);
        TEST("OUT0", "(0F1H), C", 0xED, 0x09, 0xF1);
        TEST("OUT0", "(0F1H), D", 0xED, 0x11, 0xF1);
        TEST("OUT0", "(0F1H), E", 0xED, 0x19, 0xF1);
        TEST("OUT0", "(0F1H), H", 0xED, 0x21, 0xF1);
        TEST("OUT0", "(0F1H), L", 0xED, 0x29, 0xF1);
    }
}

void test_block() {
    TEST("LDI",  "", 0xED, 0xA0);
    TEST("LDD",  "", 0xED, 0xA8);
    TEST("LDIR", "", 0xED, 0xB0);
    TEST("LDDR", "", 0xED, 0xB8);
    TEST("CPI",  "", 0xED, 0xA1);
    TEST("CPD",  "", 0xED, 0xA9);
    TEST("CPIR", "", 0xED, 0xB1);
    TEST("CPDR", "", 0xED, 0xB9);
    TEST("INI",  "", 0xED, 0xA2);
    TEST("IND",  "", 0xED, 0xAA);
    TEST("INIR", "", 0xED, 0xB2);
    TEST("INDR", "", 0xED, 0xBA);
    TEST("OUTI", "", 0xED, 0xA3);
    TEST("OUTD", "", 0xED, 0xAB);
    TEST("OTIR", "", 0xED, 0xB3);
    TEST("OTDR", "", 0xED, 0xBB);

    if (z180()) {
        TEST("OTIM",  "", 0xED, 0x83);
        TEST("OTIMR", "", 0xED, 0x93);
        TEST("OTDM",  "", 0xED, 0x8B);
        TEST("OTDMR", "", 0xED, 0x9B);
    }
}

void test_inherent() {
    TEST("DI",  "", 0xF3);
    TEST("EI",  "", 0xFB);

    TEST("NOP",  "", 0x00);
    TEST("HALT", "", 0x76);
    if (z180()) {
        TEST("SLP", "", 0xED, 0x76);
    }

    TEST("IM", "0", 0xED, 0x46);
    TEST("IM", "1", 0xED, 0x56);
    TEST("IM", "2", 0xED, 0x5E);
    UNKN(           0xED, 0x4E);
}

void test_restart() {
    TEST("RST", "0",   0xC7);
    TEST("RST", "8",   0xCF);
    TEST("RST", "16",  0xD7);
    TEST("RST", "24",  0xDF);
    TEST("RST", "32",  0xE7);
    TEST("RST", "28H", 0xEF);
    TEST("RST", "30H", 0xF7);
    TEST("RST", "38H", 0xFF);
}

void test_bitop() {
    TEST("BIT", "7, A", 0xCB, 0x7F);
    TEST("BIT", "0, B", 0xCB, 0x40);
    TEST("BIT", "1, C", 0xCB, 0x49);
    TEST("BIT", "2, D", 0xCB, 0x52);
    TEST("BIT", "3, E", 0xCB, 0x5B);
    TEST("BIT", "4, H", 0xCB, 0x64);
    TEST("BIT", "5, L", 0xCB, 0x6D);
    TEST("BIT", "6, (HL)", 0xCB, 0x76);
    TEST("BIT", "0, (IX-128)", 0xDD, 0xCB, 0x80, 0x46);
    TEST("BIT", "5, (IY+127)", 0xFD, 0xCB, 0x7F, 0x6E);

    TEST("RES", "7, A", 0xCB, 0xBF);
    TEST("RES", "0, B", 0xCB, 0x80);
    TEST("RES", "1, C", 0xCB, 0x89);
    TEST("RES", "2, D", 0xCB, 0x92);
    TEST("RES", "3, E", 0xCB, 0x9B);
    TEST("RES", "4, H", 0xCB, 0xA4);
    TEST("RES", "5, L", 0xCB, 0xAD);
    TEST("RES", "6, (HL)", 0xCB, 0xB6);
    TEST("RES", "1, (IX-128)", 0xDD, 0xCB, 0x80, 0x8E);
    TEST("RES", "6, (IY+127)", 0xFD, 0xCB, 0x7F, 0xB6);

    TEST("SET", "7, A", 0xCB, 0xFF);
    TEST("SET", "0, B", 0xCB, 0xC0);
    TEST("SET", "1, C", 0xCB, 0xC9);
    TEST("SET", "2, D", 0xCB, 0xD2);
    TEST("SET", "3, E", 0xCB, 0xDB);
    TEST("SET", "4, H", 0xCB, 0xE4);
    TEST("SET", "5, L", 0xCB, 0xED);
    TEST("SET", "6, (HL)", 0xCB, 0xF6);
    TEST("SET", "2, (IX-128)", 0xDD, 0xCB, 0x80, 0xD6);
    TEST("SET", "7, (IY+127)", 0xFD, 0xCB, 0x7F, 0xFE);
}

void test_illegal() {
    for (Config::opcode_t opc = 0x30; opc < 0x38; opc++)
        UNKN(0xCB, opc);
    for (Config::opcode_t opc = 0x00; opc < 0x40; opc++) {
        if (z180()) {
            const auto lo = opc & 0xF;
            if (lo == 0 || lo == 1 || lo == 4 || lo == 8 || lo == 9 || lo == 0xC) {
                if (opc == 0x30 || opc == 0x31) {
                    UNKN(0xED, opc); // IN0 (HL),(io)/OUT0 (io),(HL)
                } else {
                    continue;        // IN0, OUT0, TST
                }
            }
        }
        UNKN(0xED, opc);
    }
    for (Config::opcode_t opc = z180() ? 0x7d : 0x7C; opc < 0x80; opc++) {
        UNKN(0xED, opc);
    }
    for (Config::opcode_t opc = 0x80; opc < 0xA0; opc++) {
        if (z180()) {
            const auto lo = opc & 0xF;
            if (lo == 3 || lo == 0xB)
                continue;       // OTxMx
        }
        UNKN(0xED, opc);
    }
    for (Config::opcode_t opc = 0xBC; opc; opc++)
        UNKN(0xED, opc);
    static constexpr Config::opcode_t ed_illegals[] = {
        0x4C, 0x4E,
        0x54, 0x55, 0x5C, 0x5D,
        0x63, 0x64, 0x65, 0x66, 0x6B, 0x6C, 0x6D, 0x6E,
        0x70, 0x71, 0x74, 0x75, 0x76, 0x77,
        0xA4, 0xA5, 0xA6, 0xA7, 0xAC, 0xAD, 0xAE, 0xAF,
        0xB4, 0xB5, 0xB6, 0xB7,
    };
    for (const auto opc : ed_illegals) {
        if (z180()) {
            if (opc == 0x4C || opc == 0x5C || opc == 0x6C || opc == 0x7C)
                continue;       // MLT
            if (opc == 0x64 || opc == 0x74 || opc == 0x76)
                continue;       // TST, TSTIO, SLP
        }
        UNKN(0xED, opc);
    }

    static constexpr Config::opcode_t ddfd_legals[] = {
        0x09, 0x19, 0x21, 0x22, 0x23, 0x29, 0x2A, 0x2B, 0x34, 0x35, 0x36, 0x39,
        0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E,
        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x7E,
        0x86, 0x8E, 0x96, 0x9E, 0xA6, 0xAE, 0xB6, 0xBE,
        0xCB, 0xE1, 0xE3, 0xE5, 0xE9, 0xF9, 0
    };
    Config::opcode_t opc = 0;
    for (const auto legal : ddfd_legals) {
        while (opc != legal) {
            UNKN(0xDD, opc);
            UNKN(0xFD, opc);
            opc++;
        }
        opc++;
    }

    static constexpr Config::opcode_t ddfdcb_legals[] = {
        0x06, 0x0E, 0x16, 0x1E, 0x26, 0x2E, 0x3E,
        0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E, 0x76, 0x7E,
        0x86, 0x8E, 0x96, 0x9E, 0xA6, 0xAE, 0xB6, 0xBE,
        0xC6, 0xCE, 0xD6, 0xDE, 0xE6, 0xEE, 0xF6, 0xFE, 0
    };
    opc = 0;
    for (const auto legal : ddfdcb_legals) {
        while (opc != legal) {
            for (Config::opcode_t offset = 0;; offset++) {
                UNKN(0xDD, 0xCB, offset, opc);
                UNKN(0xFD, 0xCB, offset, opc);
                if (offset == 0xFF)
                    break;
            }
            opc++;
        }
        opc++;
    }
}
// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_load_registers);
    RUN_TEST(test_move_immediate);
    RUN_TEST(test_load);
    RUN_TEST(test_store);
    RUN_TEST(test_exchange);
    RUN_TEST(test_stack);
    RUN_TEST(test_jump);
    RUN_TEST(test_incr_decr);
    RUN_TEST(test_alu_8bit);
    RUN_TEST(test_alu_16bit);
    RUN_TEST(test_io);
    RUN_TEST(test_inherent);
    RUN_TEST(test_restart);
    RUN_TEST(test_bitop);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
