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

#include "dis_cdp1802.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::cdp1802;
using namespace libasm::test;

DisCdp1802 dis1802;
Disassembler &disassembler(dis1802);

static bool cdp1804() {
    return strcmp_P("1804", disassembler.cpu_P()) == 0;
}

static bool cdp1804a() {
    return strcmp_P("1804A", disassembler.cpu_P()) == 0;
}

static void set_up() {
    disassembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 1802", true,   disassembler.setCpu("1802"));
    EQUALS_P("cpu 1802", "1802", disassembler.cpu_P());

    EQUALS("cpu 1804", true,   disassembler.setCpu("1804"));
    EQUALS_P("cpu 1804", "1804", disassembler.cpu_P());

    EQUALS("cpu 1804A", true,   disassembler.setCpu("1804a"));
    EQUALS_P("cpu 1804A", "1804A", disassembler.cpu_P());

    EQUALS("cpu CDP1802", true,   disassembler.setCpu("CDP1802"));
    EQUALS_P("cpu CDP1802", "1802", disassembler.cpu_P());

    EQUALS("cpu CDP1804", true,   disassembler.setCpu("CDP1804"));
    EQUALS_P("cpu CDP1804", "1804", disassembler.cpu_P());

    EQUALS("cpu CDP1804A", true,   disassembler.setCpu("CDP1804A"));
    EQUALS_P("cpu CDP1804A", "1804A", disassembler.cpu_P());
}

static void test_mem_ref() {
    // Load Immediate
    TEST(LDI, "18",   0xF8, 0x12);

    if (cdp1804() || cdp1804a()) {
        // Register Load Immediate
        TEST(RLDI, "0, X'1234'",  0x68, 0xC0, 0x12, 0x34);
        TEST(RLDI, "1, X'1234'",  0x68, 0xC1, 0x12, 0x34);
        TEST(RLDI, "2, X'1234'",  0x68, 0xC2, 0x12, 0x34);
        TEST(RLDI, "3, X'1234'",  0x68, 0xC3, 0x12, 0x34);
        TEST(RLDI, "4, X'1234'",  0x68, 0xC4, 0x12, 0x34);
        TEST(RLDI, "5, X'1234'",  0x68, 0xC5, 0x12, 0x34);
        TEST(RLDI, "6, X'1234'",  0x68, 0xC6, 0x12, 0x34);
        TEST(RLDI, "7, X'1234'",  0x68, 0xC7, 0x12, 0x34);
        TEST(RLDI, "8, X'1234'",  0x68, 0xC8, 0x12, 0x34);
        TEST(RLDI, "9, X'1234'",  0x68, 0xC9, 0x12, 0x34);
        TEST(RLDI, "10, X'1234'", 0x68, 0xCA, 0x12, 0x34);
        TEST(RLDI, "11, X'1234'", 0x68, 0xCB, 0x12, 0x34);
        TEST(RLDI, "12, X'1234'", 0x68, 0xCC, 0x12, 0x34);
        TEST(RLDI, "13, X'1234'", 0x68, 0xCD, 0x12, 0x34);
        TEST(RLDI, "14, X'1234'", 0x68, 0xCE, 0x12, 0x34);
        TEST(RLDI, "15, X'1234'", 0x68, 0xCF, 0x12, 0x34);
    }

    // Load via N
    TEST(LDN, "1",  0x01);
    TEST(LDN, "2",  0x02);
    TEST(LDN, "3",  0x03);
    TEST(LDN, "4",  0x04);
    TEST(LDN, "5",  0x05);
    TEST(LDN, "6",  0x06);
    TEST(LDN, "7",  0x07);
    TEST(LDN, "8",  0x08);
    TEST(LDN, "9",  0x09);
    TEST(LDN, "10", 0x0A);
    TEST(LDN, "11", 0x0B);
    TEST(LDN, "12", 0x0C);
    TEST(LDN, "13", 0x0D);
    TEST(LDN, "14", 0x0E);
    TEST(LDN, "15", 0x0F);

    // Load Advance
    TEST(LDA, "0",  0x40);
    TEST(LDA, "1",  0x41);
    TEST(LDA, "2",  0x42);
    TEST(LDA, "3",  0x43);
    TEST(LDA, "4",  0x44);
    TEST(LDA, "5",  0x45);
    TEST(LDA, "6",  0x46);
    TEST(LDA, "7",  0x47);
    TEST(LDA, "8",  0x48);
    TEST(LDA, "9",  0x49);
    TEST(LDA, "10", 0x4A);
    TEST(LDA, "11", 0x4B);
    TEST(LDA, "12", 0x4C);
    TEST(LDA, "13", 0x4D);
    TEST(LDA, "14", 0x4E);
    TEST(LDA, "15", 0x4F);

    // Load via X
    TEST(LDX,  "", 0xF0);

    // Load via X and Advance
    TEST(LDXA, "", 0x72);

    if (cdp1804() || cdp1804a()) {
        // Register Load via X and Advance
        TEST(RLXA, "0",  0x68, 0x60);
        TEST(RLXA, "1",  0x68, 0x61);
        TEST(RLXA, "2",  0x68, 0x62);
        TEST(RLXA, "3",  0x68, 0x63);
        TEST(RLXA, "4",  0x68, 0x64);
        TEST(RLXA, "5",  0x68, 0x65);
        TEST(RLXA, "6",  0x68, 0x66);
        TEST(RLXA, "7",  0x68, 0x67);
        TEST(RLXA, "8",  0x68, 0x68);
        TEST(RLXA, "9",  0x68, 0x69);
        TEST(RLXA, "10", 0x68, 0x6A);
        TEST(RLXA, "11", 0x68, 0x6B);
        TEST(RLXA, "12", 0x68, 0x6C);
        TEST(RLXA, "13", 0x68, 0x6D);
        TEST(RLXA, "14", 0x68, 0x6E);
        TEST(RLXA, "15", 0x68, 0x6F);
    }

    // Store via N
    TEST(STR, "0",  0x50);
    TEST(STR, "1",  0x51);
    TEST(STR, "2",  0x52);
    TEST(STR, "3",  0x53);
    TEST(STR, "4",  0x54);
    TEST(STR, "5",  0x55);
    TEST(STR, "6",  0x56);
    TEST(STR, "7",  0x57);
    TEST(STR, "8",  0x58);
    TEST(STR, "9",  0x59);
    TEST(STR, "10", 0x5A);
    TEST(STR, "11", 0x5B);
    TEST(STR, "12", 0x5C);
    TEST(STR, "13", 0x5D);
    TEST(STR, "14", 0x5E);
    TEST(STR, "15", 0x5F);

    // Store via X and Decrement
    TEST(STXD, "", 0x73);

    if (cdp1804() || cdp1804a()) {
        // Register Store via X and Decrement
        TEST(RSXD, "0",  0x68, 0xA0);
        TEST(RSXD, "1", 0x68, 0xA1);
        TEST(RSXD, "2",  0x68, 0xA2);
        TEST(RSXD, "3",  0x68, 0xA3);
        TEST(RSXD, "4",  0x68, 0xA4);
        TEST(RSXD, "5",  0x68, 0xA5);
        TEST(RSXD, "6",  0x68, 0xA6);
        TEST(RSXD, "7",  0x68, 0xA7);
        TEST(RSXD, "8",  0x68, 0xA8);
        TEST(RSXD, "9",  0x68, 0xA9);
        TEST(RSXD, "10", 0x68, 0xAA);
        TEST(RSXD, "11", 0x68, 0xAB);
        TEST(RSXD, "12", 0x68, 0xAC);
        TEST(RSXD, "13", 0x68, 0xAD);
        TEST(RSXD, "14", 0x68, 0xAE);
        TEST(RSXD, "15", 0x68, 0xAF);
    }

    symtab.intern(3, "PC");
    symtab.intern(4, "CALL");
    TEST(LDN, "PC",   0x03);
    TEST(SEP, "CALL", 0xD4);
}

static void test_reg_op() {
    // Increment reg N
    TEST(INC, "0",  0x10);
    TEST(INC, "1",  0x11);
    TEST(INC, "2",  0x12);
    TEST(INC, "3",  0x13);
    TEST(INC, "4",  0x14);
    TEST(INC, "5",  0x15);
    TEST(INC, "6",  0x16);
    TEST(INC, "7",  0x17);
    TEST(INC, "8",  0x18);
    TEST(INC, "9",  0x19);
    TEST(INC, "10", 0x1A);
    TEST(INC, "11", 0x1B);
    TEST(INC, "12", 0x1C);
    TEST(INC, "13", 0x1D);
    TEST(INC, "14", 0x1E);
    TEST(INC, "15", 0x1F);

    // Decrement reg N
    TEST(DEC, "0",  0x20);
    TEST(DEC, "1",  0x21);
    TEST(DEC, "2",  0x22);
    TEST(DEC, "3",  0x23);
    TEST(DEC, "4",  0x24);
    TEST(DEC, "5",  0x25);
    TEST(DEC, "6",  0x26);
    TEST(DEC, "7",  0x27);
    TEST(DEC, "8",  0x28);
    TEST(DEC, "9",  0x29);
    TEST(DEC, "10", 0x2A);
    TEST(DEC, "11", 0x2B);
    TEST(DEC, "12", 0x2C);
    TEST(DEC, "13", 0x2D);
    TEST(DEC, "14", 0x2E);
    TEST(DEC, "15", 0x2F);

    if (cdp1804a()) {
        // Decrement reg N and long Branch if Not equal Zero
        TEST(DBNZ, "0, X'1234'",  0x68, 0x20, 0x12, 0x34);
        TEST(DBNZ, "1, X'1234'",  0x68, 0x21, 0x12, 0x34);
        TEST(DBNZ, "2, X'1234'",  0x68, 0x22, 0x12, 0x34);
        TEST(DBNZ, "3, X'1234'",  0x68, 0x23, 0x12, 0x34);
        TEST(DBNZ, "4, X'1234'",  0x68, 0x24, 0x12, 0x34);
        TEST(DBNZ, "5, X'1234'",  0x68, 0x25, 0x12, 0x34);
        TEST(DBNZ, "6, X'1234'",  0x68, 0x26, 0x12, 0x34);
        TEST(DBNZ, "7, X'1234'",  0x68, 0x27, 0x12, 0x34);
        TEST(DBNZ, "8, X'1234'",  0x68, 0x28, 0x12, 0x34);
        TEST(DBNZ, "9, X'1234'",  0x68, 0x29, 0x12, 0x34);
        TEST(DBNZ, "10, X'1234'", 0x68, 0x2A, 0x12, 0x34);
        TEST(DBNZ, "11, X'1234'", 0x68, 0x2B, 0x12, 0x34);
        TEST(DBNZ, "12, X'1234'", 0x68, 0x2C, 0x12, 0x34);
        TEST(DBNZ, "13, X'1234'", 0x68, 0x2D, 0x12, 0x34);
        TEST(DBNZ, "14, X'1234'", 0x68, 0x2E, 0x12, 0x34);
        TEST(DBNZ, "15, X'1234'", 0x68, 0x2F, 0x12, 0x34);
    }

    // Increment reg X
    TEST(IRX,  "", 0x60);

    // Get Low reg N
    TEST(GLO, "0",  0x80);
    TEST(GLO, "1",  0x81);
    TEST(GLO, "2",  0x82);
    TEST(GLO, "3",  0x83);
    TEST(GLO, "4",  0x84);
    TEST(GLO, "5",  0x85);
    TEST(GLO, "6",  0x86);
    TEST(GLO, "7",  0x87);
    TEST(GLO, "8",  0x88);
    TEST(GLO, "9",  0x89);
    TEST(GLO, "10", 0x8A);
    TEST(GLO, "11", 0x8B);
    TEST(GLO, "12", 0x8C);
    TEST(GLO, "13", 0x8D);
    TEST(GLO, "14", 0x8E);
    TEST(GLO, "15", 0x8F);

    // Put Low reg N
    TEST(PLO, "0",  0xA0);
    TEST(PLO, "1",  0xA1);
    TEST(PLO, "2",  0xA2);
    TEST(PLO, "3",  0xA3);
    TEST(PLO, "4",  0xA4);
    TEST(PLO, "5",  0xA5);
    TEST(PLO, "6",  0xA6);
    TEST(PLO, "7",  0xA7);
    TEST(PLO, "8",  0xA8);
    TEST(PLO, "9",  0xA9);
    TEST(PLO, "10", 0xAA);
    TEST(PLO, "11", 0xAB);
    TEST(PLO, "12", 0xAC);
    TEST(PLO, "13", 0xAD);
    TEST(PLO, "14", 0xAE);
    TEST(PLO, "15", 0xAF);

    // Get High reg N
    TEST(GHI, "0",  0x90);
    TEST(GHI, "1",  0x91);
    TEST(GHI, "2",  0x92);
    TEST(GHI, "3",  0x93);
    TEST(GHI, "4",  0x94);
    TEST(GHI, "5",  0x95);
    TEST(GHI, "6",  0x96);
    TEST(GHI, "7",  0x97);
    TEST(GHI, "8",  0x98);
    TEST(GHI, "9",  0x99);
    TEST(GHI, "10", 0x9A);
    TEST(GHI, "11", 0x9B);
    TEST(GHI, "12", 0x9C);
    TEST(GHI, "13", 0x9D);
    TEST(GHI, "14", 0x9E);
    TEST(GHI, "15", 0x9F);

    // Put High reg N
    TEST(PHI, "0",  0xB0);
    TEST(PHI, "1",  0xB1);
    TEST(PHI, "2",  0xB2);
    TEST(PHI, "3",  0xB3);
    TEST(PHI, "4",  0xB4);
    TEST(PHI, "5",  0xB5);
    TEST(PHI, "6",  0xB6);
    TEST(PHI, "7",  0xB7);
    TEST(PHI, "8",  0xB8);
    TEST(PHI, "9",  0xB9);
    TEST(PHI, "10", 0xBA);
    TEST(PHI, "11", 0xBB);
    TEST(PHI, "12", 0xBC);
    TEST(PHI, "13", 0xBD);
    TEST(PHI, "14", 0xBE);
    TEST(PHI, "15", 0xBF);

    if (cdp1804() || cdp1804a()) {
        // Register N to register X copy
        TEST(RNX, "0",  0x68, 0xB0);
        TEST(RNX, "1",  0x68, 0xB1);
        TEST(RNX, "2",  0x68, 0xB2);
        TEST(RNX, "3",  0x68, 0xB3);
        TEST(RNX, "4",  0x68, 0xB4);
        TEST(RNX, "5",  0x68, 0xB5);
        TEST(RNX, "6",  0x68, 0xB6);
        TEST(RNX, "7",  0x68, 0xB7);
        TEST(RNX, "8",  0x68, 0xB8);
        TEST(RNX, "9",  0x68, 0xB9);
        TEST(RNX, "10", 0x68, 0xBA);
        TEST(RNX, "11", 0x68, 0xBB);
        TEST(RNX, "12", 0x68, 0xBC);
        TEST(RNX, "13", 0x68, 0xBD);
        TEST(RNX, "14", 0x68, 0xBE);
        TEST(RNX, "15", 0x68, 0xBF);
    }
}

static void test_logic_op() {
    TEST(OR,   "",    0xF1);
    TEST(ORI,  "X'34'", 0xF9, 0x34);
    TEST(XOR,  "",    0xF3);
    TEST(XRI,  "X'67'", 0xFB, 0x67);
    TEST(AND,  "",    0xF2);
    TEST(ANI,  "X'45'", 0xFA, 0x45);
    TEST(SHR,  "",    0xF6);
    TEST(SHRC, "",    0x76);
    TEST(SHL,  "",    0xFE);
    TEST(SHLC, "",    0x7E);
}

static void test_arith_op() {
    TEST(ADD,  "",     0xF4);
    TEST(ADI,  "X'89'",  0xFC, 0x89);
    TEST(ADC,  "",     0x74);
    TEST(ADCI, "X'AB'", 0x7C, 0xAB);
    TEST(SD,   "",     0xF5);
    TEST(SDI,  "X'AB'", 0xFD, 0xAB);
    TEST(SDB,  "",     0x75);
    TEST(SDBI, "X'CD'", 0x7D, 0xCD);
    TEST(SM,   "",     0xF7);
    TEST(SMI,  "X'CD'", 0xFF, 0xCD);
    TEST(SMB,  "",     0x77);
    TEST(SMBI, "X'EF'", 0x7F, 0xEF);

    if (cdp1804a()) {
        TEST(DADD, "",    0x68, 0xF4);
        TEST(DADI, "X'89'", 0x68, 0xFC, 0x89);
        TEST(DADC, "",    0x68, 0x74);
        TEST(DACI, "X'89'", 0x68, 0x7C, 0x89);
        TEST(DSM,  "",    0x68, 0xF7);
        TEST(DSMI, "X'89'", 0x68, 0xFF, 0x89);
        TEST(DSMB, "",    0x68, 0x77);
        TEST(DSBI, "X'89'", 0x68, 0x7F, 0x89);
    }

    symtab.intern(-1,  "neg1");
    symtab.intern(254, "pos254");
    TEST(ADI, "neg1",   0xFC, 0xFF);
    TEST(ADI, "pos254", 0xFC, 0xFE);
}

static void test_branch() {
    ATEST(0x1000, BR,  "X'1031'", 0x30, 0x31);
    ATEST(0x1000, BZ,  "X'1033'", 0x32, 0x33);
    ATEST(0x1000, BNZ, "X'103B'", 0x3A, 0x3B);
    ATEST(0x1000, BDF, "X'1034'", 0x33, 0x34);
    ATEST(0x1000, BNF, "X'103C'", 0x3B, 0x3C);
    ATEST(0x1000, BQ,  "X'1032'", 0x31, 0x32);
    ATEST(0x1000, BNQ, "X'103A'", 0x39, 0x3A);
    ATEST(0x1000, B1,  "X'1035'", 0x34, 0x35);
    ATEST(0x1000, BN1, "X'103D'", 0x3C, 0x3D);
    ATEST(0x1000, B2,  "X'1036'", 0x35, 0x36);
    ATEST(0x1000, BN2, "X'103E'", 0x3D, 0x3E);
    ATEST(0x1000, B3,  "X'1037'", 0x36, 0x37);
    ATEST(0x1000, BN3, "X'103F'", 0x3E, 0x3F);
    ATEST(0x1000, B4,  "X'1038'", 0x37, 0x38);
    ATEST(0x1000, BN4, "X'1040'", 0x3F, 0x40);
    if (cdp1804() || cdp1804a()) {
        ATEST(0x1000, BCI, "X'1041'", 0x68, 0x3E, 0x41);
        ATEST(0x1000, BXI, "X'1042'", 0x68, 0x3F, 0x42);
    }

    ATEST(0x10FD, BR,  "X'1031'", 0x30, 0x31);
    ATEST(0x10FE, BR,  "X'1131'", 0x30, 0x31);
    ATEST(0x10FF, BR,  "X'1131'", 0x30, 0x31);

    ATEST(0x1000, LBR,  "X'1234'",  0xC0, 0x12, 0x34);
    ATEST(0x1000, LBZ,  "X'9ABC'",  0xC2, 0x9A, 0xBC);
    ATEST(0x1000, LBNZ, "X'5678'",  0xCA, 0x56, 0x78);
    ATEST(0x1000, LBDF, "X'DEF0'", 0xC3, 0xDE, 0xF0);
    ATEST(0x1000, LBNF, "X'9ABC'",  0xCB, 0x9A, 0xBC);
    ATEST(0x1000, LBQ,  "X'5678'",  0xC1, 0x56, 0x78);
    ATEST(0x1000, LBNQ, "X'1234'",  0xC9, 0x12, 0x34);

    TEST(SKP,  "", 0x38);
    TEST(LSKP, "", 0xC8);
    TEST(LSZ,  "", 0xCE);
    TEST(LSNZ, "", 0xC6);
    TEST(LSDF, "", 0xCF);
    TEST(LSNF, "", 0xC7);
    TEST(LSQ,  "", 0xCD);
    TEST(LSNQ, "", 0xC5);
    TEST(LSIE, "", 0xCC);

    symtab.intern(0x1031, "sym1031");
    symtab.intern(0x1131, "sym1131");

    ATEST(0x10FD, BR, "sym1031", 0x30, 0x31);
    ATEST(0x10FE, BR, "sym1131", 0x30, 0x31);
    ATEST(0x10FF, BR, "sym1131", 0x30, 0x31);

    TEST(LBNF, "sym1131", 0xCB, 0x11, 0x31);
}

static void test_control() {
    TEST(IDL,  "", 0x00);
    TEST(NOP,  "", 0xC4);

    TEST(SEP, "0",  0xD0);
    TEST(SEP, "1",  0xD1);
    TEST(SEP, "2",  0xD2);
    TEST(SEP, "3",  0xD3);
    TEST(SEP, "4",  0xD4);
    TEST(SEP, "5",  0xD5);
    TEST(SEP, "6",  0xD6);
    TEST(SEP, "7",  0xD7);
    TEST(SEP, "8",  0xD8);
    TEST(SEP, "9",  0xD9);
    TEST(SEP, "10", 0xDA);
    TEST(SEP, "11", 0xDB);
    TEST(SEP, "12", 0xDC);
    TEST(SEP, "13", 0xDD);
    TEST(SEP, "14", 0xDE);
    TEST(SEP, "15", 0xDF);

    TEST(SEX, "0",  0xE0);
    TEST(SEX, "1",  0xE1);
    TEST(SEX, "2",  0xE2);
    TEST(SEX, "3",  0xE3);
    TEST(SEX, "4",  0xE4);
    TEST(SEX, "5",  0xE5);
    TEST(SEX, "6",  0xE6);
    TEST(SEX, "7",  0xE7);
    TEST(SEX, "8",  0xE8);
    TEST(SEX, "9",  0xE9);
    TEST(SEX, "10", 0xEA);
    TEST(SEX, "11", 0xEB);
    TEST(SEX, "12", 0xEC);
    TEST(SEX, "13", 0xED);
    TEST(SEX, "14", 0xEE);
    TEST(SEX, "15", 0xEF);

    TEST(SEQ,  "", 0x7B);
    TEST(REQ,  "", 0x7A);
    TEST(MARK, "", 0x79);
}

static void test_timer() {
    TEST(LDC,  "", 0x68, 0x06);
    TEST(GEC,  "", 0x68, 0x08);
    TEST(STPC, "", 0x68, 0x00);
    TEST(DTC,  "", 0x68, 0x01);
    TEST(STM,  "", 0x68, 0x07);
    TEST(SCM1, "", 0x68, 0x05);
    TEST(SCM2, "", 0x68, 0x03);
    TEST(SPM1, "", 0x68, 0x04);
    TEST(SPM2, "", 0x68, 0x02);
    TEST(ETQ,  "", 0x68, 0x09);
}

static void test_intr() {
    TEST(RET, "", 0x70);
    TEST(DIS, "", 0x71);
    TEST(SAV, "", 0x78);

    if (cdp1804() || cdp1804a()) {
        TEST(XIE, "", 0x68, 0x0A);
        TEST(XID, "", 0x68, 0x0B);
        TEST(CIE, "", 0x68, 0x0C);
        TEST(CID, "", 0x68, 0x0D);
    }
    if (cdp1804a()) {
        TEST(DSAV, "", 0x68, 0x76);
    }
}

static void test_io() {
    TEST(OUT, "1", 0x61);
    TEST(OUT, "2", 0x62);
    TEST(OUT, "3", 0x63);
    TEST(OUT, "4", 0x64);
    TEST(OUT, "5", 0x65);
    TEST(OUT, "6", 0x66);
    TEST(OUT, "7", 0x67);

    TEST(INP, "1", 0x69);
    TEST(INP, "2", 0x6A);
    TEST(INP, "3", 0x6B);
    TEST(INP, "4", 0x6C);
    TEST(INP, "5", 0x6D);
    TEST(INP, "6", 0x6E);
    TEST(INP, "7", 0x6F);

    symtab.intern(1, "STDIN");
    TEST(INP, "STDIN", 0x69);
}

static void test_call() {
    TEST(SCAL, "0, X'1234'",  0x68, 0x80, 0x12, 0x34);
    TEST(SCAL, "1, X'1234'",  0x68, 0x81, 0x12, 0x34);
    TEST(SCAL, "2, X'1234'",  0x68, 0x82, 0x12, 0x34);
    TEST(SCAL, "3, X'1234'",  0x68, 0x83, 0x12, 0x34);
    TEST(SCAL, "4, X'1234'",  0x68, 0x84, 0x12, 0x34);
    TEST(SCAL, "5, X'1234'",  0x68, 0x85, 0x12, 0x34);
    TEST(SCAL, "6, X'1234'",  0x68, 0x86, 0x12, 0x34);
    TEST(SCAL, "7, X'1234'",  0x68, 0x87, 0x12, 0x34);
    TEST(SCAL, "8, X'1234'",  0x68, 0x88, 0x12, 0x34);
    TEST(SCAL, "9, X'1234'",  0x68, 0x89, 0x12, 0x34);
    TEST(SCAL, "10, X'1234'", 0x68, 0x8A, 0x12, 0x34);
    TEST(SCAL, "11, X'1234'", 0x68, 0x8B, 0x12, 0x34);
    TEST(SCAL, "12, X'1234'", 0x68, 0x8C, 0x12, 0x34);
    TEST(SCAL, "13, X'1234'", 0x68, 0x8D, 0x12, 0x34);
    TEST(SCAL, "14, X'1234'", 0x68, 0x8E, 0x12, 0x34);
    disassembler.setOption("intel-hex", "off");
    TEST(SCAL, "15, X'1234'", 0x68, 0x8F, 0x12, 0x34);

    TEST(SRET, "0",  0x68, 0x90);
    TEST(SRET, "1",  0x68, 0x91);
    TEST(SRET, "2",  0x68, 0x92);
    TEST(SRET, "3",  0x68, 0x93);
    TEST(SRET, "4",  0x68, 0x94);
    TEST(SRET, "5",  0x68, 0x95);
    TEST(SRET, "6",  0x68, 0x96);
    TEST(SRET, "7",  0x68, 0x97);
    TEST(SRET, "8",  0x68, 0x98);
    TEST(SRET, "9",  0x68, 0x99);
    TEST(SRET, "10", 0x68, 0x9A);
    TEST(SRET, "11", 0x68, 0x9B);
    TEST(SRET, "12", 0x68, 0x9C);
    TEST(SRET, "13", 0x68, 0x9D);
    TEST(SRET, "14", 0x68, 0x9E);
    TEST(SRET, "15", 0x68, 0x9F);
}

static void test_reg() {
    dis1802.setOption("use-register", "on");

    TEST(LDN, "R1",  0x01);
    TEST(LDA, "R0",  0x40);
    TEST(STR, "R15", 0x5F);
    TEST(INC, "R14", 0x1E);
    TEST(DEC, "R13", 0x2D);
    TEST(GLO, "R12", 0x8C);
    TEST(PLO, "R11", 0xAB);
    TEST(GHI, "R10", 0x9A);
    TEST(PHI, "R9",  0xB9);
    TEST(SEP, "R8",  0xD8);
    TEST(SEX, "R7",  0xE7);

    if (cdp1804()) {
        TEST(RLDI, "R0, X'1234'", 0x68, 0xC0, 0x12, 0x34);
        TEST(RLXA, "R15",       0x68, 0x6F);
        TEST(RSXD, "R14",       0x68, 0xAE);
        TEST(RNX,  "R12",       0x68, 0xBC);
        TEST(SCAL, "R11, X'1234'", 0x68, 0x8B, 0x12, 0x34);
        TEST(SRET, "R10",       0x68, 0x9A);
    }
    if (cdp1804a()) {
        TEST(DBNZ, "R13, X'1234'", 0x68, 0x2D, 0x12, 0x34);
    }
}

static void test_illegal_cdp1802() {
    ERRI(0x68);
}

static void test_illegal_cdp1804() {
    static const Config::opcode_t illegals[] = {
        0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
        0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
        0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
        0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
        0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
        0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
        0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
        0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
        0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
        0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
        0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
    };

    for (size_t idx = 0; idx < sizeof(illegals); idx++)
        ERRI(0x68, illegals[idx]);
}

static void test_illegal_cdp1804a() {
    static const Config::opcode_t illegals[] = {
        0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
        0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
        0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x70, 0x71, 0x72, 0x73, 0x75,
        0x78, 0x79, 0x7A, 0x7B, 0x7D, 0x7E,
        0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
        0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
        0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
        0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
        0xF0, 0xF1, 0xF2, 0xF3, 0xF5, 0xF6,
        0xF8, 0xF9, 0xFA, 0xFB, 0xFD, 0xFE,
    };

    for (size_t idx = 0; idx < sizeof(illegals); idx++)
        ERRI(0x68, illegals[idx]);
}

// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_mem_ref);
    RUN_TEST(test_reg_op);
    RUN_TEST(test_logic_op);
    RUN_TEST(test_arith_op);
    RUN_TEST(test_branch);
    RUN_TEST(test_control);
    RUN_TEST(test_intr);
    RUN_TEST(test_io);
    RUN_TEST(test_reg);
    if (cdp1804()) {
        RUN_TEST(test_timer);
        RUN_TEST(test_call);
        RUN_TEST(test_illegal_cdp1804);
    } else if (cdp1804a()) {
        RUN_TEST(test_timer);
        RUN_TEST(test_call);
        RUN_TEST(test_illegal_cdp1804a);
    } else {
        RUN_TEST(test_illegal_cdp1802);
    }
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
