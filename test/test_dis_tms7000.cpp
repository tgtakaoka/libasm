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

#include "dis_tms7000.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::tms7000;
using namespace libasm::test;

DisTms7000 dis7000;
Disassembler &disassembler(dis7000);

void set_up() {
    disassembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu TMS7000", true,   disassembler.setCpu("TMS7000"));
    EQUALS_P("cpu TMS7000", "TMS7000", disassembler.config().cpu_P());

    EQUALS("cpu TMS70C00", true,   disassembler.setCpu("TMS70C00"));
    EQUALS_P("cpu TMS70C00", "TMS7000", disassembler.config().cpu_P());
}

void test_implied() {
    TEST("DINT", "", 0x06);
    TEST("EINT", "", 0x05);
    TEST("IDLE", "", 0x01);
    TEST("LDSP", "", 0x0D);
    TEST("NOP",  "", 0x00);
    TEST("POP",  "ST", 0x08);
    TEST("PUSH", "ST", 0x0E);
    TEST("SETC", "", 0x07);
    TEST("STSP", "", 0x09);
    TEST("RETI", "", 0x0B);
    TEST("RETS", "", 0x0A);
    TEST("TSTA", "", 0xB0);
    TEST("TSTB", "", 0xC1);

    TEST("TRAP", "23", 0xE8);
    TEST("TRAP", "22", 0xE9);
    TEST("TRAP", "21", 0xEA);
    TEST("TRAP", "20", 0xEB);
    TEST("TRAP", "19", 0xEC);
    TEST("TRAP", "18", 0xED);
    TEST("TRAP", "17", 0xEE);
    TEST("TRAP", "16", 0xEF);
    TEST("TRAP", "15", 0xF0);
    TEST("TRAP", "14", 0xF1);
    TEST("TRAP", "13", 0xF2);
    TEST("TRAP", "12", 0xF3);
    TEST("TRAP", "11", 0xF4);
    TEST("TRAP", "10", 0xF5);
    TEST("TRAP", "9",  0xF6);
    TEST("TRAP", "8",  0xF7);
    TEST("TRAP", "7",  0xF8);
    TEST("TRAP", "6",  0xF9);
    TEST("TRAP", "5",  0xFA);
    TEST("TRAP", "4",  0xFB);
    TEST("TRAP", "3",  0xFC);
    TEST("TRAP", "2",  0xFD);
    TEST("TRAP", "1",  0xFE);
    TEST("TRAP", "0",  0xFF);
}

void test_single() {
    TEST("CLR",  "A",    0xB5);
    TEST("CLR",  "B",    0xC5);
    TEST("CLR",  "R214",     0xD5, 0xD6);
    NMEM("CLR",  "R0", "R0", 0xD5);

    TEST("DEC",  "A",    0xB2);
    TEST("DEC",  "B",    0xC2);
    TEST("DEC",  "R211", 0xD2, 0xD3);

    TEST("DECD", "A",    0xBB);
    TEST("DECD", "B",    0xCB);
    TEST("DECD", "R220", 0xDB, 0xDC);

    TEST("INC",  "A",    0xB3);
    TEST("INC",  "B",    0xC3);
    TEST("INC",  "R212", 0xD3, 0xD4);

    TEST("INV",  "A",    0xB4);
    TEST("INV",  "B",    0xC4);
    TEST("INV",  "R213", 0xD4, 0xD5);

    TEST("POP",  "A",    0xB9);
    TEST("POP",  "B",    0xC9);
    TEST("POP",  "R218", 0xD9, 0xDA);

    TEST("PUSH", "A",    0xB8);
    TEST("PUSH", "B",    0xC8);
    TEST("PUSH", "R217", 0xD8, 0xD9);

    TEST("RL",   "A",    0xBE);
    TEST("RL",   "B",    0xCE);
    TEST("RL",   "R223", 0xDE, 0xDF);

    TEST("RLC",  "A",    0xBF);
    TEST("RLC",  "B",    0xCF);
    TEST("RLC",  "R224", 0xDF, 0xE0);

    TEST("RR",   "A",    0xBC);
    TEST("RR",   "B",    0xCC);
    TEST("RR",   "R221", 0xDC, 0xDD);

    TEST("RRC",  "A",    0xBD);
    TEST("RRC",  "B",    0xCD);
    TEST("RRC",  "R222", 0xDD, 0xDE);

    TEST("SWAP", "A",    0xB7);
    TEST("SWAP", "B",    0xC7);
    TEST("SWAP", "R216", 0xD7, 0xD8);

    TEST("XCHB", "A",    0xB6);
    TEST("XCHB", "B",    0xC6);
    TEST("XCHB", "R215", 0xD6, 0xD7);
}

void test_register() {
    TEST("ADC", "B, A", 0x69);
    TEST("ADC", "R26, A",         0x19, 0x1A);
    NMEM("ADC", "R0, A", "R0, A", 0x19);
    TEST("ADC", "A, B",           0x39, 0x00);
    TEST("ADC", "R58, B",         0x39, 0x3A);
    NMEM("ADC", "R0, B", "R0, B", 0x39);
    TEST("ADC", "A, R75",           0x49, 0x00, 0x4B);
    TEST("ADC", "B, R75",           0x49, 0x01, 0x4B);
    TEST("ADC", "R74, R75",         0x49, 0x4A, 0x4B);
    NMEM("ADC", "B, R0",      "R0", 0x49, 0x01);
    NMEM("ADC", "R0, R0", "R0, R0", 0x49);
    TEST("ADC", "%>2A, A",    0x29, 0x2A);
    TEST("ADC", "%>5A, B",    0x59, 0x5A);
    TEST("ADC", "%>7A, R123",        0x79, 0x7A, 0x7B);
    NMEM("ADC", "%>7A, R0",    "R0", 0x79, 0x7A);
    NMEM("ADC",   "%0, R0", "0, R0", 0x79);

    TEST("ADD", "B, A",       0x68);
    TEST("ADD", "A, B",       0x38, 0x00);
    TEST("ADD", "R25, A",     0x18, 0x19);
    TEST("ADD", "R57, B",     0x38, 0x39);
    TEST("ADD", "A, R74",     0x48, 0x00, 0x4A);
    TEST("ADD", "B, R74",     0x48, 0x01, 0x4A);
    TEST("ADD", "R73, R74",   0x48, 0x49, 0x4A);
    TEST("ADD", "%>29, A",    0x28, 0x29);
    TEST("ADD", "%>59, B",    0x58, 0x59);
    TEST("ADD", "%>79, R122", 0x78, 0x79, 0x7A);

    TEST("AND", "B, A",       0x63);
    TEST("AND", "A, B",       0x33, 0x00);
    TEST("AND", "R20, A",     0x13, 0x14);
    TEST("AND", "R52, B",     0x33, 0x34);
    TEST("AND", "A, R69",     0x43, 0x00, 0x45);
    TEST("AND", "B, R69",     0x43, 0x01, 0x45);
    TEST("AND", "R68, R69",   0x43, 0x44, 0x45);
    TEST("AND", "%>24, A",    0x23, 0x24);
    TEST("AND", "%>54, B",    0x53, 0x54);
    TEST("AND", "%>74, R117", 0x73, 0x74, 0x75);

    TEST("CMP", "B, A",       0x6D);
    TEST("CMP", "A, B",       0x3D, 0x00);
    TEST("CMP", "R30, A",     0x1D, 0x1E);
    TEST("CMP", "R62, B",     0x3D, 0x3E);
    TEST("CMP", "A, R79",     0x4D, 0x00, 0x4F);
    TEST("CMP", "B, R79",     0x4D, 0x01, 0x4F);
    TEST("CMP", "R78, R79",   0x4D, 0x4E, 0x4F);
    TEST("CMP", "%>2E, A",    0x2D, 0x2E);
    TEST("CMP", "%>5E, B",    0x5D, 0x5E);
    TEST("CMP", "%>7E, R127", 0x7D, 0x7E, 0x7F);

    TEST("DAC", "B, A",       0x6E);
    TEST("DAC", "A, B",       0x3E, 0x00);
    TEST("DAC", "R31, A",     0x1E, 0x1F);
    TEST("DAC", "R63, B",     0x3E, 0x3F);
    TEST("DAC", "A, R80",     0x4E, 0x00, 0x50);
    TEST("DAC", "B, R80",     0x4E, 0x01, 0x50);
    TEST("DAC", "R79, R80",   0x4E, 0x4F, 0x50);
    TEST("DAC", "%>2F, A",    0x2E, 0x2F);
    TEST("DAC", "%>5F, B",    0x5E, 0x5F);
    TEST("DAC", "%>7F, R128", 0x7E, 0x7F, 0x80);

    TEST("DSB", "B, A",       0x6F);
    TEST("DSB", "A, B",       0x3F, 0x00);
    TEST("DSB", "R32, A",     0x1F, 0x20);
    TEST("DSB", "R64, B",     0x3F, 0x40);
    TEST("DSB", "A, R81",     0x4F, 0x00, 0x51);
    TEST("DSB", "B, R81",     0x4F, 0x01, 0x51);
    TEST("DSB", "R80, R81",   0x4F, 0x50, 0x51);
    TEST("DSB", "%>30, A",    0x2F, 0x30);
    TEST("DSB", "%>60, B",    0x5F, 0x60);
    TEST("DSB", "%>80, R129", 0x7F, 0x80, 0x81);

    TEST("MOV", "B, A",       0x62);
    TEST("MOV", "A, B",       0xC0);
    TEST("MOV", "R19, A",     0x12, 0x13);
    TEST("MOV", "R51, B",     0x32, 0x33);
    TEST("MOV", "A, R209",    0xD0, 0xD1);
    TEST("MOV", "B, R210",    0xD1, 0xD2);
    TEST("MOV", "R67, R68",   0x42, 0x43, 0x44);
    TEST("MOV", "%>23, A",    0x22, 0x23);
    TEST("MOV", "%>53, B",    0x52, 0x53);
    TEST("MOV", "%>73, R116", 0x72, 0x73, 0x74);

    TEST("MPY", "B, A",       0x6C);
    TEST("MPY", "A, B",       0x3C, 0x00);
    TEST("MPY", "R29, A",     0x1C, 0x1D);
    TEST("MPY", "R61, B",     0x3C, 0x3D);
    TEST("MPY", "A, R78",     0x4C, 0x00, 0x4E);
    TEST("MPY", "B, R78",     0x4C, 0x01, 0x4E);
    TEST("MPY", "R77, R78",   0x4C, 0x4D, 0x4E);
    TEST("MPY", "%>2D, A",    0x2C, 0x2D);
    TEST("MPY", "%>5D, B",    0x5C, 0x5D);
    TEST("MPY", "%>7D, R126", 0x7C, 0x7D, 0x7E);

    TEST("OR",  "B, A",       0x64);
    TEST("OR",  "A, B",       0x34, 0x00);
    TEST("OR",  "R21, A",     0x14, 0x15);
    TEST("OR",  "R53, B",     0x34, 0x35);
    TEST("OR",  "A, R70",     0x44, 0x00, 0x46);
    TEST("OR",  "B, R70",     0x44, 0x01, 0x46);
    TEST("OR",  "R69, R70",   0x44, 0x45, 0x46);
    TEST("OR",  "%>25, A",    0x24, 0x25);
    TEST("OR",  "%>55, B",    0x54, 0x55);
    TEST("OR",  "%>75, R118", 0x74, 0x75, 0x76);

    TEST("SBB", "B, A",       0x6B);
    TEST("SBB", "A, B",       0x3B, 0x00);
    TEST("SBB", "R28, A",     0x1B, 0x1C);
    TEST("SBB", "R60, B",     0x3B, 0x3C);
    TEST("SBB", "A, R77",     0x4B, 0x00, 0x4D);
    TEST("SBB", "B, R77",     0x4B, 0x01, 0x4D);
    TEST("SBB", "R76, R77",   0x4B, 0x4C, 0x4D);
    TEST("SBB", "%>2C, A",    0x2B, 0x2C);
    TEST("SBB", "%>5C, B",    0x5B, 0x5C);
    TEST("SBB", "%>7C, R125", 0x7B, 0x7C, 0x7D);

    TEST("SUB", "B, A",       0x6A);
    TEST("SUB", "A, B",       0x3A, 0x00);
    TEST("SUB", "R27, A",     0x1A, 0x1B);
    TEST("SUB", "R59, B",     0x3A, 0x3B);
    TEST("SUB", "A, R76",     0x4A, 0x00, 0x4C);
    TEST("SUB", "B, R76",     0x4A, 0x01, 0x4C);
    TEST("SUB", "R75, R76",   0x4A, 0x4B, 0x4C);
    TEST("SUB", "%>2B, A",    0x2A, 0x2B);
    TEST("SUB", "%>5B, B",    0x5A, 0x5B);
    TEST("SUB", "%>7B, R124", 0x7A, 0x7B, 0x7C);

    TEST("XOR", "B, A",       0x65);
    TEST("XOR", "A, B",       0x35, 0x00);
    TEST("XOR", "R22, A",     0x15, 0x16);
    TEST("XOR", "R54, B",     0x35, 0x36);
    TEST("XOR", "A, R71",     0x45, 0x00, 0x47);
    TEST("XOR", "B, R71",     0x45, 0x01, 0x47);
    TEST("XOR", "R70, R71",   0x45, 0x46, 0x47);
    TEST("XOR", "%>26, A",    0x25, 0x26);
    TEST("XOR", "%>56, B",    0x55, 0x56);
    TEST("XOR", "%>76, R119", 0x75, 0x76, 0x77);

    TEST("MOVD", "%>898A, R139",            0x88, 0x89, 0x8A, 0x8B);
    NMEM("MOVD", "%>898A, R0",        "R0", 0x88, 0x89, 0x8A);
    NMEM("MOVD", "%>8900, R0", ">8900, R0", 0x88, 0x89);
    NMEM("MOVD", "%0, R0",         "0, R0", 0x88);
    TEST("MOVD", "%>A9AA(B), R171",               0xA8, 0xA9, 0xAA, 0xAB);
    NMEM("MOVD", "%>A9AA(B), R0",           "R0", 0xA8, 0xA9, 0xAA);
    NMEM("MOVD", "%>A900(B), R0", ">A900(B), R0", 0xA8, 0xA9);
    NMEM("MOVD", "%0(B), R0",         "0(B), R0", 0xA8);
    TEST("MOVD", "R153, R154",       0x98, 0x99, 0x9A);
    NMEM("MOVD", "R153, R0",   "R0", 0x98, 0x99);
    NMEM("MOVD", "R0, R0", "R0, R0", 0x98);
}

void test_peripheral() {
    TEST("ANDP", "A, P132",    0x83, 0x84);
    TEST("ANDP", "B, P148",    0x93, 0x94);
    TEST("ANDP", "%>A4, P165",  0xA3, 0xA4, 0xA5);
    NMEM("ANDP", "%>A4, P0",  "P0", 0xA3, 0xA4);
    NMEM("ANDP", "%0, P0", "0, P0", 0xA3);

    TEST("MOVP", "P129, A",    0x80, 0x81);
    TEST("MOVP", "P146, B",    0x91, 0x92);
    TEST("MOVP", "A, P131",    0x82, 0x83);
    TEST("MOVP", "B, P147",    0x92, 0x93);
    TEST("MOVP", "%>A3, P164", 0xA2, 0xA3, 0xA4);

    TEST("ORP",  "A, P133",    0x84, 0x85);
    TEST("ORP",  "B, P149",    0x94, 0x95);
    TEST("ORP",  "%>A5, P166", 0xA4, 0xA5, 0xA6);

    TEST("XORP", "A, P134",    0x85, 0x86);
    TEST("XORP", "B, P150",    0x95, 0x96);
    TEST("XORP", "%>A6, P167", 0xA5, 0xA6, 0xA7);
}

void test_single_relative() {
    disassembler.setOption("relative", "on");

    ATEST(0x1000, "JMP", "$-29", 0xE0, 0xE1);
    ATEST(0x1000, "JN",  "$-28", 0xE1, 0xE2);
    ATEST(0x1000, "JZ",  "$-27", 0xE2, 0xE3);
    ATEST(0x1000, "JC",  "$-26", 0xE3, 0xE4);
    ATEST(0x1000, "JP",  "$-25", 0xE4, 0xE5);
    ATEST(0x1000, "JPZ", "$-24", 0xE5, 0xE6);
    ATEST(0x1000, "JNZ", "$-23", 0xE6, 0xE7);
    ATEST(0x1000, "JNC", "$-22",       0xE7, 0xE8);
    ANMEM(0x1000, "JNC", "$+1", "$+1", 0xE7);

    ATEST(0x1000, "DJNZ", "A, $-67",    0xBA, 0xBB);
    ATEST(0x1000, "DJNZ", "B, $-51",    0xCA, 0xCB);
    ATEST(0x1000, "DJNZ", "R219, $-33",         0xDA, 0xDB, 0xDC);
    ANMEM(0x1000, "DJNZ", "R219, $+2",   "$+2", 0xDA, 0xDB);
    ANMEM(0x1000, "DJNZ", "R0, $+1", "R0, $+1", 0xDA);
}

void test_dual_relative() {
    disassembler.setOption("relative", "on");

    ATEST(0x1000, "BTJO", "B, A, $+105",       0x66, 0x67);
    ATEST(0x1000, "BTJO", "R23, A, $+27",      0x16, 0x17, 0x18);
    ATEST(0x1000, "BTJO", "R55, B, $+59",             0x36, 0x37, 0x38);
    ANMEM(0x1000, "BTJO", "R55, B, $+2",       "$+2", 0x36, 0x37);
    ANMEM(0x1000, "BTJO", "R0, B, $+1", "R0, B, $+1", 0x36);
    ATEST(0x1000, "BTJO", "R71, R72, $+77",    0x46, 0x47, 0x48, 0x49);
    ATEST(0x1000, "BTJO", "%>27, A, $+43",     0x26, 0x27, 0x28);
    ATEST(0x1000, "BTJO", "%>57, B, $+91"   ,  0x56, 0x57, 0x58);
    ATEST(0x1000, "BTJO", "%>77, R120, $+125", 0x76, 0x77, 0x78, 0x79);

    ATEST(0x1000, "BTJZ", "B, A, $+106",       0x67, 0x68);
    ATEST(0x1000, "BTJZ", "R24, A, $+28",      0x17, 0x18, 0x19);
    ATEST(0x1000, "BTJZ", "R56, B, $+60",      0x37, 0x38, 0x39);
    ATEST(0x1000, "BTJZ", "R72, R73, $+78",    0x47, 0x48, 0x49, 0x4A);
    ATEST(0x1000, "BTJZ", "%>28, A, $+44",     0x27, 0x28, 0x29);
    ATEST(0x1000, "BTJZ", "%>58, B, $+92"   ,  0x57, 0x58, 0x59);
    ATEST(0x1000, "BTJZ", "%>78, R121, $+126", 0x77, 0x78, 0x79, 0x7A);

    ATEST(0x1000, "BTJOP", "A, P135, $-117",   0x86, 0x87, 0x88);
    ATEST(0x1000, "BTJOP", "B, P151, $-101",   0x96, 0x97, 0x98);
    ATEST(0x1000, "BTJOP", "%>A7, P168, $-83", 0xA6, 0xA7, 0xA8, 0xA9);

    ATEST(0x1000, "BTJZP", "A, P136, $-116",   0x87, 0x88, 0x89);
    ATEST(0x1000, "BTJZP", "B, P152, $-100",   0x97, 0x98, 0x99);
    ATEST(0x1000, "BTJZP", "%>A8, P169, $-82", 0xA7, 0xA8, 0xA9, 0xAA);
}

void test_extended() {
    TEST("BR",   "@>8D8E",    0x8C, 0x8D, 0x8E);
    NMEM("BR",   "@>8D00", ">8D00",  0x8C, 0x8D);
    NMEM("BR",   "@>0000", ">0000",  0x8C);
    TEST("BR",   "*R157",     0x9C, 0x9D);
    NMEM("BR",   "*R0", "R0", 0x9C);
    TEST("BR",   "@>ADAE(B)", 0xAC, 0xAD, 0xAE);

    TEST("CALL", "@>8F90",    0x8E, 0x8F, 0x90);
    TEST("CALL", "*R159",     0x9E, 0x9F);
    TEST("CALL", "@>AFB0(B)", 0xAE, 0xAF, 0xB0);

    TEST("CMPA", "@>8E8F",    0x8D, 0x8E, 0x8F);
    TEST("CMPA", "*R158",     0x9D, 0x9E);
    TEST("CMPA", "@>AEAF(B)", 0xAD, 0xAE, 0xAF);

    TEST("LDA",  "@>8B8C",    0x8A, 0x8B, 0x8C);
    TEST("LDA",  "*R155",     0x9A, 0x9B);
    TEST("LDA",  "@>ABAC(B)", 0xAA, 0xAB, 0xAC);

    TEST("STA",  "@>8C8D",    0x8B, 0x8C, 0x8D);
    TEST("STA",  "*R156",     0x9B, 0x9C);
    TEST("STA",  "@>ACAD(B)", 0xAB, 0xAC, 0xAD);
}

void test_illegal() {
    static constexpr Config::opcode_t illegals[] = {
        0x02, 0x03, 0x04, 0x0C, 0x0F,
        0x10, 0x11,
        0x20, 0x21,
        0x30, 0x31,
        0x40, 0x41,
        0x50, 0x51,
        0x60, 0x61,
        0x70, 0x71,
        0x81, 0x89, 0x8F,
        0x90, 0x99, 0x9F,
        0xA0, 0xA9, 0xAF,
        0xB1,
    };
    for (const auto opc : illegals)
        UNKN(opc);
}
// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);

    RUN_TEST(test_implied);
    RUN_TEST(test_single);
    RUN_TEST(test_register);
    RUN_TEST(test_peripheral);
    RUN_TEST(test_single_relative);
    RUN_TEST(test_dual_relative);
    RUN_TEST(test_extended);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
