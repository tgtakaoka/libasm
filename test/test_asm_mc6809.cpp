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

#include "asm_mc6809.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::mc6809;
using namespace libasm::test;

AsmMc6809 as6809;
Assembler &assembler(as6809);

static bool is6309() {
    return strcmp_P("6309", assembler.cpu_P()) == 0;
}

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 6809", true,   assembler.setCpu("6809"));
    EQUALS_P("cpu 6809", "6809", assembler.cpu_P());

    EQUALS("cpu 6309", true,   assembler.setCpu("6309"));
    EQUALS_P("cpu 6309", "6309", assembler.cpu_P());

    EQUALS("cpu mc6809", true,   assembler.setCpu("mc6809"));
    EQUALS_P("cpu mc6809", "6809", assembler.cpu_P());

    EQUALS("cpu hd6309", true,   assembler.setCpu("hd6309"));
    EQUALS_P("cpu hd6309", "6309", assembler.cpu_P());
}

static void test_inherent() {
    TEST("NOP",  0x12);
    TEST("SYNC", 0x13);
    TEST("DAA",  0x19);
    TEST("SEX",  0x1D);
    TEST("RTS",  0x39);
    TEST("ABX",  0x3A);
    TEST("RTI",  0x3B);
    TEST("MUL",  0x3D);
    TEST("SWI",  0x3F);
    TEST("SWI2", 0x10, 0x3F);
    TEST("SWI3", 0x11, 0x3F);

    TEST("NEGA", 0x40);
    TEST("COMA", 0x43);
    TEST("LSRA", 0x44);
    TEST("RORA", 0x46);
    TEST("ASRA", 0x47);
    TEST("ASLA", 0x48);
    TEST("ROLA", 0x49);
    TEST("DECA", 0x4A);
    TEST("INCA", 0x4C);
    TEST("TSTA", 0x4D);
    TEST("CLRA", 0x4F);

    TEST("NEGB", 0x50);
    TEST("COMB", 0x53);
    TEST("LSRB", 0x54);
    TEST("RORB", 0x56);
    TEST("ASRB", 0x57);
    TEST("ASLB", 0x58);
    TEST("ROLB", 0x59);
    TEST("DECB", 0x5A);
    TEST("INCB", 0x5C);
    TEST("TSTB", 0x5D);
    TEST("CLRB", 0x5F);

    if (is6309()) {
        // HD6309
        TEST("SEXW",  0x14);
        TEST("PSHSW", 0x10, 0x38);
        TEST("PULSW", 0x10, 0x39);
        TEST("PSHUW", 0x10, 0x3A);
        TEST("PULUW", 0x10, 0x3B);

        TEST("NEGD",  0x10, 0x40);
        TEST("COMD",  0x10, 0x43);
        TEST("LSRD",  0x10, 0x44);
        TEST("RORD",  0x10, 0x46);
        TEST("ASRD",  0x10, 0x47);
        TEST("ASLD",  0x10, 0x48);
        TEST("ROLD",  0x10, 0x49);
        TEST("DECD",  0x10, 0x4A);
        TEST("INCD",  0x10, 0x4C);
        TEST("TSTD",  0x10, 0x4D);
        TEST("CLRD",  0x10, 0x4F);

        TEST("COME",  0x11, 0x43);
        TEST("DECE",  0x11, 0x4A);
        TEST("INCE",  0x11, 0x4C);
        TEST("TSTE",  0x11, 0x4D);
        TEST("CLRE",  0x11, 0x4F);

        TEST("COMF",  0x11, 0x53);
        TEST("DECF",  0x11, 0x5A);
        TEST("INCF",  0x11, 0x5C);
        TEST("TSTF",  0x11, 0x5D);
        TEST("CLRF",  0x11, 0x5F);
    } else {
        ERUI("SEXW");
        ERUI("PSHSW");
        ERUI("PULSW");
        ERUI("PSHUW");
        ERUI("PULUW");
        ERUI("NEGD");
        ERUI("COMD");
        ERUI("LSRD");
        ERUI("RORD");
        ERUI("ASRD");
        ERUI("ASLD");
        ERUI("ROLD");
        ERUI("DECD");
        ERUI("INCD");
        ERUI("TSTD");
        ERUI("CLRD");
        ERUI("COME");
        ERUI("DECE");
        ERUI("INCE");
        ERUI("TSTE");
        ERUI("CLRE");
        ERUI("COMF");
        ERUI("DECF");
        ERUI("INCF");
        ERUI("TSTF");
        ERUI("CLRF");
    }
}

static void test_stack() {
    TEST("PSHS CC", 0x34, 0x01);
    TEST("PSHS A",  0x34, 0x02);
    TEST("PSHS B",  0x34, 0x04);
    TEST("PSHS DP", 0x34, 0x08);
    TEST("PSHS X",  0x34, 0x10);
    TEST("PSHS Y",  0x34, 0x20);
    TEST("PSHS U",  0x34, 0x40);
    TEST("PSHS PC", 0x34, 0x80);
    TEST("PSHS D",  0x34, 0x06);

    TEST("PSHU CC", 0x36, 0x01);
    TEST("PSHU A",  0x36, 0x02);
    TEST("PSHU B",  0x36, 0x04);
    TEST("PSHU DP", 0x36, 0x08);
    TEST("PSHU X",  0x36, 0x10);
    TEST("PSHU Y",  0x36, 0x20);
    TEST("PSHU S",  0x36, 0x40);
    TEST("PSHU PC", 0x36, 0x80);
    TEST("PSHU D",  0x36, 0x06);

    TEST("PULS CC", 0x35, 0x01);
    TEST("PULS A",  0x35, 0x02);
    TEST("PULS B",  0x35, 0x04);
    TEST("PULS DP", 0x35, 0x08);
    TEST("PULS X",  0x35, 0x10);
    TEST("PULS Y",  0x35, 0x20);
    TEST("PULS U",  0x35, 0x40);
    TEST("PULS PC", 0x35, 0x80);
    TEST("PULS D",  0x35, 0x06);

    TEST("PULU CC", 0x37, 0x01);
    TEST("PULU A",  0x37, 0x02);
    TEST("PULU B",  0x37, 0x04);
    TEST("PULU DP", 0x37, 0x08);
    TEST("PULU X",  0x37, 0x10);
    TEST("PULU Y",  0x37, 0x20);
    TEST("PULU S",  0x37, 0x40);
    TEST("PULU PC", 0x37, 0x80);
    TEST("PULU D",  0x37, 0x06);

    TEST("PSHU A,X",     0x36, 0x12);
    TEST("PSHU A,X,B,Y", 0x36, 0x36);

    TEST("pshs cc,a,b,dp,x,y,u,pc", 0x34, 0xFF);
    TEST("PULS PC,A,DP,B,X,U,Y,CC", 0x35, 0xFF);
    TEST("PSHU PC,S,Y,X,DP,B,A,CC", 0x36, 0xFF);
    TEST("PULU DP,CC,PC,A,B,S,Y,X", 0x37, 0xFF);
    TEST("PSHS PC,U,Y,X,DP,D,CC",   0x34, 0xFF);
    TEST("PULS CC,D,DP,X,U,Y,PC",   0x35, 0xFF);

    TEST("PSHS",      0x34, 0x00);
    TEST("PULS #$00", 0x35, 0x00);
    TEST("PSHU #$AA", 0x36, 0xAA);
    TEST("PULU #$FF", 0x37, 0xFF);

    ERRT("PSHS S",         REGISTER_NOT_ALLOWED, "S",    0x34, 0x00);
    ERRT("PULS A,S,B",     REGISTER_NOT_ALLOWED, "S,B",  0x35, 0x06);
    ERRT("PSHU CC,X,U,PC", REGISTER_NOT_ALLOWED, "U,PC", 0x36, 0x91);
    ERRT("PULU U",         REGISTER_NOT_ALLOWED, "U",    0x37, 0x00);

    ERRT("PSHS A,D",   DUPLICATE_REGISTER, "D", 0x34, 0x06);
    ERRT("PULS D,B",   DUPLICATE_REGISTER, "B", 0x35, 0x06);
    ERRT("PSHU X,D,X", DUPLICATE_REGISTER, "X", 0x36, 0x16);
    ERRT("PSHS U,D,U", DUPLICATE_REGISTER, "U", 0x34, 0x46);
    ERRT("PSHU S,D,S", DUPLICATE_REGISTER, "S", 0x36, 0x46);

    ERRT("PULS E",   REGISTER_NOT_ALLOWED, "E", 0x35, 0x00);
    ERRT("PSHU F",   REGISTER_NOT_ALLOWED, "F", 0x36, 0x00);
    ERRT("PSHS V",   REGISTER_NOT_ALLOWED, "V", 0x34, 0x00);
    ERRT("PULU W",   REGISTER_NOT_ALLOWED, "W", 0x37, 0x00);
    ERRT("PULS X,E", REGISTER_NOT_ALLOWED, "E", 0x35, 0x10);
    ERRT("PSHU Y,F", REGISTER_NOT_ALLOWED, "F", 0x36, 0x20);
    ERRT("PSHS U,V", REGISTER_NOT_ALLOWED, "V", 0x34, 0x40);
}

static void test_register() {
    TEST("EXG A,B", 0x1E, 0x89);
    TEST("EXG A,A", 0x1E, 0x88);
    TEST("TFR X,Y", 0x1F, 0x12);
    TEST("TFR X,X", 0x1F, 0x11);

    TEST("TFR A,B",  0x1F, 0x89);
    TEST("TFR A,CC", 0x1F, 0x8A);
    TEST("TFR A,DP", 0x1F, 0x8B);
    TEST("TFR B,A",  0x1F, 0x98);
    TEST("TFR CC,A", 0x1F, 0xA8);
    TEST("TFR DP,A", 0x1F, 0xB8);

    TEST("TFR D,X",  0x1F, 0x01);
    TEST("TFR D,Y",  0x1F, 0x02);
    TEST("TFR D,U",  0x1F, 0x03);
    TEST("TFR D,S",  0x1F, 0x04);
    TEST("TFR D,PC", 0x1F, 0x05);
    TEST("TFR X,D",  0x1F, 0x10);
    TEST("TFR Y,D",  0x1F, 0x20);
    TEST("TFR U,D",  0x1F, 0x30);
    TEST("TFR S,D",  0x1F, 0x40);
    TEST("TFR PC,D", 0x1F, 0x50);

    ERRT("EXG A",     OPERAND_NOT_ALLOWED,  "A");
    ERRT("EXG A,",    UNKNOWN_OPERAND,      "A,");
    ERRT("EXG A,X",   ILLEGAL_SIZE,         "A,X",   0x1E, 0x81);
    ERRT("EXG A,X,Y", OPERAND_NOT_ALLOWED,  "A,X,Y");

    if (is6309()) {
        // HD6309
        TEST("ADDR A,B", 0x10, 0x30, 0x89);
        TEST("ADCR A,B", 0x10, 0x31, 0x89);
        TEST("SUBR A,B", 0x10, 0x32, 0x89);
        TEST("SBCR A,B", 0x10, 0x33, 0x89);
        TEST("ANDR A,B", 0x10, 0x34, 0x89);
        TEST("ORR  A,B", 0x10, 0x35, 0x89);
        TEST("EORR A,B", 0x10, 0x36, 0x89);
        TEST("CMPR A,B", 0x10, 0x37, 0x89);

        TEST("TFR A,E",  0x1F, 0x8E);
        TEST("TFR A,F",  0x1F, 0x8F);
        TEST("TFR E,A",  0x1F, 0xE8);
        TEST("TFR F,A",  0x1F, 0xF8);
        TEST("TFR A,Z",  0x1F, 0x8D);
        TEST("TFR Z,A",  0x1F, 0xD8);
        TEST("TFR A,0",  0x1F, 0x8D);
        TEST("TFR 0,A",  0x1F, 0xD8);
        TEST("TFR CC,0", 0x1F, 0xAD);
        TEST("TFR 0,CC", 0x1F, 0xDA);
        TEST("TFR D,W",  0x1F, 0x06);
        TEST("TFR D,V",  0x1F, 0x07);
        TEST("TFR W,D",  0x1F, 0x60);
        TEST("TFR V,D",  0x1F, 0x70);
        TEST("TFR W,PC", 0x1F, 0x65);
        TEST("TFR S,W",  0x1F, 0x46);
        TEST("TFR Z,V",  0x1F, 0xD7);
        TEST("TFR X,Z",  0x1F, 0x1D);
        TEST("TFR 0,V",  0x1F, 0xD7);
        TEST("TFR X,0",  0x1F, 0x1D);

        ERRT("ADDR A",     OPERAND_NOT_ALLOWED,  "A");
        ERRT("ADDR A,",    UNKNOWN_OPERAND,      "A,");
        ERRT("ADDR A,X",   ILLEGAL_SIZE,         "A,X", 0x10, 0x30, 0x81);
        ERRT("ADDR A,X,Y", OPERAND_NOT_ALLOWED,  "A,X,Y");

        ERRT("TFR E",     OPERAND_NOT_ALLOWED,  "E");
        ERRT("TFR E,",    UNKNOWN_OPERAND,      "E,");
        ERRT("TFR E,W",   ILLEGAL_SIZE,         "E,W", 0x1F, 0xE6);
        ERRT("TFR E,X,F", OPERAND_NOT_ALLOWED,  "E,X,F");
    } else {
        ERUI("ADDR A,B");
        ERUI("ADCR A,B");
        ERUI("SUBR A,B");
        ERUI("SBCR A,B");
        ERUI("ANDR A,B");
        ERUI("ORR  A,B");
        ERUI("EORR A,B");
        ERUI("CMPR A,B");
        ERRT("TFR A,E",  UNKNOWN_REGISTER, "A,E",  0x1F, 0x8E);
        ERRT("TFR A,F",  UNKNOWN_REGISTER, "A,F",  0x1F, 0x8F);
        ERRT("TFR E,A",  UNKNOWN_REGISTER, "E,A",  0x1F, 0xE8);
        ERRT("TFR F,A",  UNKNOWN_REGISTER, "F,A",  0x1F, 0xF8);
        ERRT("TFR A,Z",  UNKNOWN_REGISTER, "A,Z",  0x1F, 0x8D);
        ERRT("TFR Z,A",  UNKNOWN_REGISTER, "Z,A",  0x1F, 0xD8);
        ERRT("TFR A,0",  UNKNOWN_REGISTER, "A,0",  0x1F, 0x8D);
        ERRT("TFR 0,A",  UNKNOWN_REGISTER, "0,A",  0x1F, 0xD8);
        ERRT("TFR CC,0", UNKNOWN_REGISTER, "CC,0", 0x1F, 0xAD);
        ERRT("TFR 0,CC", UNKNOWN_REGISTER, "0,CC", 0x1F, 0xDA);
        ERRT("TFR D,W",  UNKNOWN_REGISTER, "D,W",  0x1F, 0x06);
        ERRT("TFR D,V",  UNKNOWN_REGISTER, "D,V",  0x1F, 0x07);
        ERRT("TFR W,D",  UNKNOWN_REGISTER, "W,D",  0x1F, 0x60);
        ERRT("TFR V,D",  UNKNOWN_REGISTER, "V,D",  0x1F, 0x70);
        ERRT("TFR W,PC", UNKNOWN_REGISTER, "W,PC", 0x1F, 0x65);
        ERRT("TFR S,W",  UNKNOWN_REGISTER, "S,W",  0x1F, 0x46);
        ERRT("TFR Z,V",  UNKNOWN_REGISTER, "Z,V",  0x1F, 0xD7);
        ERRT("TFR X,Z",  UNKNOWN_REGISTER, "X,Z",  0x1F, 0x1D);
        ERRT("TFR 0,V",  UNKNOWN_REGISTER, "0,V",  0x1F, 0xD7);
        ERRT("TFR X,0",  UNKNOWN_REGISTER, "X,0",  0x1F, 0x1D);
    }
}

static void test_relative() {
    ATEST(0x1000, "BRA $1002", 0x20, 0x00);
    ATEST(0x1000, "BRN $1000", 0x21, 0xFE);
    AERRT(0x0010, "BRA $FF92", OVERFLOW_RANGE, "$FF92", 0x20, 0x80);
    AERRT(0xFFF0, "BRA $0071", OVERFLOW_RANGE, "$0071", 0x20, 0x7F);
    ATEST(0x1000, "BHI $1004", 0x22, 0x02);
    ATEST(0x1000, "BLS $1081", 0x23, 0x7F);
    ATEST(0x1000, "BHS $0F82", 0x24, 0x80);
    ATEST(0x1000, "BLO $1002", 0x25, 0x00);
    ATEST(0x1000, "BNE $1002", 0x26, 0x00);
    ATEST(0x1000, "BEQ $1002", 0x27, 0x00);
    ATEST(0x1000, "BVC $1002", 0x28, 0x00);
    ATEST(0x1000, "BVS $1002", 0x29, 0x00);
    ATEST(0x1000, "BPL $1002", 0x2A, 0x00);
    ATEST(0x1000, "BMI $1002", 0x2B, 0x00);
    ATEST(0x1000, "BGE $1002", 0x2C, 0x00);
    ATEST(0x1000, "BLT $1002", 0x2D, 0x00);
    ATEST(0x1000, "BGT $1002", 0x2E, 0x00);
    ATEST(0x1000, "BLE $1002", 0x2F, 0x00);
    AERRT(0x1000, "BRA $1082", OPERAND_TOO_FAR, "$1082", 0x20, 0x80);
    AERRT(0x1000, "BLE $0F81", OPERAND_TOO_FAR, "$0F81", 0x2F, 0x7F);

    ATEST(0x1000, "LBRA $1003", 0x16, 0x00, 0x00);
    ATEST(0x1000, "LBRA $9002", 0x16, 0x7F, 0xFF);
    ATEST(0x9000, "LBRA $1003", 0x16, 0x80, 0x00);
    AERRT(0x9000, "LBRA $1002", OVERFLOW_RANGE, "$1002", 0x16, 0x7F, 0xFF);
    ATEST(0x1000, "LBRN $1000", 0x10, 0x21, 0xFF, 0xFC);
    ATEST(0x1000, "LBHI $1008", 0x10, 0x22, 0x00, 0x04);
    ATEST(0x1000, "LBLS $9003", 0x10, 0x23, 0x7F, 0xFF);
    AERRT(0x1000, "LBLS $9004", OVERFLOW_RANGE, "$9004", 0x10, 0x23, 0x80, 0x00);
    ATEST(0x9000, "LBHS $1004", 0x10, 0x24, 0x80, 0x00);
    ATEST(0x1000, "LBLO $1004", 0x10, 0x25, 0x00, 0x00);
    ATEST(0x1000, "LBNE $1004", 0x10, 0x26, 0x00, 0x00);
    ATEST(0x1000, "LBEQ $1004", 0x10, 0x27, 0x00, 0x00);
    ATEST(0x1000, "LBVC $1004", 0x10, 0x28, 0x00, 0x00);
    ATEST(0x1000, "LBVS $1004", 0x10, 0x29, 0x00, 0x00);
    ATEST(0x1000, "LBPL $1004", 0x10, 0x2A, 0x00, 0x00);
    ATEST(0x1000, "LBMI $1004", 0x10, 0x2B, 0x00, 0x00);
    ATEST(0x1000, "LBGE $1004", 0x10, 0x2C, 0x00, 0x00);
    ATEST(0x1000, "LBLT $1004", 0x10, 0x2D, 0x00, 0x00);
    ATEST(0x1000, "LBGT $1004", 0x10, 0x2E, 0x00, 0x00);
    ATEST(0x1000, "LBLE $1004", 0x10, 0x2F, 0x00, 0x00);
    AERRT(0x1000, "LBLE $9004", OVERFLOW_RANGE, "$9004", 0x10, 0x2F, 0x80, 0x00);
    AERRT(0x9000, "LBLE $1003", OVERFLOW_RANGE, "$1003", 0x10, 0x2F, 0x7F, 0xFF);

    ATEST(0x1000, "BSR  $1042", 0x8D, 0x40);
    ATEST(0x1000, "LBSR $1043", 0x17, 0x00, 0x40);

    symtab.intern(0x0F82, ".sub_0F82");
    symtab.intern(0x1081, ".sub$1081");
    symtab.intern(0x9002, "sub.9002");
    symtab.intern(0x9003, "sub$9003");

    ATEST(0x1000, "BSR .sub$1081", 0x8D, 0x7F);
    ATEST(0x1000, "BSR .sub_0F82", 0x8D, 0x80);
    ATEST(0x1000, "LBSR sub.9002", 0x17, 0x7F, 0xFF);
    AERRT(0x1000, "LBSR sub$9003", OVERFLOW_RANGE, "sub$9003", 0x17, 0x80, 0x00);
}

static void test_immediate() {
    TEST("ORCC  #%10", 0x1A, 0x02);
    TEST("ANDCC #~1",  0x1C, 0xFE);
    TEST("CWAI  #$EF", 0x3C, 0xEF);

    TEST("SUBA #$90",  0x80, 0x90);
    TEST("CMPA #-1",   0x81, 0xFF);
    TEST("SBCA #+$00", 0x82, 0x00);
    TEST("ANDA #@177", 0x84, 0x7F);
    TEST("BITA #$90",  0x85, 0x90);
    TEST("LDA  #$90",  0x86, 0x90);
    ERRT("STA  #$90",  OPERAND_NOT_ALLOWED, "#$90");
    TEST("EORA #$90",  0x88, 0x90);
    TEST("ADCA #$90",  0x89, 0x90);
    TEST("ORA  #$90",  0x8A, 0x90);
    TEST("ADDA #$90",  0x8B, 0x90);

    TEST("SUBB #$90", 0xC0, 0x90);
    TEST("CMPB #$90", 0xC1, 0x90);
    TEST("SBCB #$90", 0xC2, 0x90);
    TEST("ANDB #$90", 0xC4, 0x90);
    TEST("BITB #$90", 0xC5, 0x90);
    TEST("LDB  #$90", 0xC6, 0x90);
    ERRT("STB  #$90", OPERAND_NOT_ALLOWED, "#$90");
    TEST("EORB #$90", 0xC8, 0x90);
    TEST("ADCB #$90", 0xC9, 0x90);
    TEST("ORB  #$90", 0xCA, 0x90);
    TEST("ADDB #$90", 0xCB, 0x90);

    TEST("SUBD #$90A0", 0x83, 0x90, 0xA0);
    TEST("ADDD #$90A0", 0xC3, 0x90, 0xA0);
    TEST("LDD  #$90A0", 0xCC, 0x90, 0xA0);
    ERRT("STD  #$90A0", OPERAND_NOT_ALLOWED, "#$90A0");
    TEST("CMPD #$90A0", 0x10, 0x83, 0x90, 0xA0);

    TEST("CMPX #$90A0", 0x8C, 0x90, 0xA0);
    TEST("LDX  #$90A0", 0x8E, 0x90, 0xA0);
    ERRT("STX  #$90A0", OPERAND_NOT_ALLOWED, "#$90A0");

    TEST("CMPY #$90A0", 0x10, 0x8C, 0x90, 0xA0);
    TEST("LDY  #$90A0", 0x10, 0x8E, 0x90, 0xA0);
    ERRT("STY  #$90A0", OPERAND_NOT_ALLOWED, "#$90A0");

    TEST("CMPU #$90A0", 0x11, 0x83, 0x90, 0xA0);
    TEST("LDU  #$90A0", 0xCE, 0x90, 0xA0);
    ERRT("STU  #$90A0", OPERAND_NOT_ALLOWED, "#$90A0");

    TEST("CMPS #$90A0", 0x11, 0x8C, 0x90, 0xA0);
    TEST("LDS  #$90A0", 0x10, 0xCE, 0x90, 0xA0);
    ERRT("StS  #$90A0", OPERAND_NOT_ALLOWED, "#$90A0");

    if (is6309()) {
        // HD6309
        TEST("LDMD  #$01", 0x11, 0x3D, 0x01);
        TEST("BITMD #$80", 0x11, 0x3C, 0x80);

        TEST("SBCD #$90A0", 0x10, 0x82, 0x90, 0xA0);
        TEST("ANDD #$90A0", 0x10, 0x84, 0x90, 0xA0);
        TEST("BITD #$90A0", 0x10, 0x85, 0x90, 0xA0);
        TEST("EORD #$90A0", 0x10, 0x88, 0x90, 0xA0);
        TEST("ADCD #$90A0", 0x10, 0x89, 0x90, 0xA0);
        TEST("ORD  #$90A0", 0x10, 0x8A, 0x90, 0xA0);

        TEST("SUBE #$90", 0x11, 0x80, 0x90);
        TEST("ADDE #$90", 0x11, 0x8B, 0x90);
        TEST("LDE  #$90", 0x11, 0x86, 0x90);
        ERRT("STE  #$90", OPERAND_NOT_ALLOWED, "#$90");
        TEST("CMPE #$90", 0x11, 0x81, 0x90);

        TEST("SUBF #$90", 0x11, 0xC0, 0x90);
        TEST("ADDF #$90", 0x11, 0xCB, 0x90);
        TEST("LDF  #$90", 0x11, 0xC6, 0x90);
        ERRT("STF  #$90", OPERAND_NOT_ALLOWED, "#$90");
        TEST("CMPF #$90", 0x11, 0xC1, 0x90);

        TEST("SUBW #$90A0", 0x10, 0x80, 0x90, 0xA0);
        TEST("ADDW #$90A0", 0x10, 0x8B, 0x90, 0xA0);
        TEST("LDW  #$90A0", 0x10, 0x86, 0x90, 0xA0);
        ERRT("STW  #$90A0", OPERAND_NOT_ALLOWED, "#$90A0");
        TEST("CMPW #$90A0", 0x10, 0x81, 0x90, 0xA0);

        TEST("LDQ  #$12345678", 0xCD, 0x12, 0x34, 0x56, 0x78);
        ERRT("STQ  #$12345678", OPERAND_NOT_ALLOWED, "#$12345678");

        TEST("MULD #$90A0", 0x11, 0x8F, 0x90, 0xA0);
        TEST("DIVD #$90A0", 0x11, 0x8D, 0x90, 0xA0);
        TEST("DIVQ #$90A0", 0x11, 0x8E, 0x90, 0xA0);
    } else {
        ERUI("LDMD  #$01");
        ERUI("BITMD #$80");
        ERUI("SBCD #$90A0");
        ERUI("ANDD #$90A0");
        ERUI("BITD #$90A0");
        ERUI("EORD #$90A0");
        ERUI("ADCD #$90A0");
        ERUI("ORD  #$90A0");
        ERUI("SUBE #$90");
        ERUI("ADDE #$90");
        ERUI("LDE  #$90");
        ERUI("CMPE #$90");
        ERUI("SUBF #$90");
        ERUI("ADDF #$90");
        ERUI("LDF  #$90");
        ERUI("CMPF #$90");
        ERUI("SUBW #$90A0");
        ERUI("ADDW #$90A0");
        ERUI("LDW  #$90A0");
        ERUI("CMPW #$90A0");
        ERUI("LDQ  #$12345678");
        ERUI("MULD #$90A0");
        ERUI("DIVD #$90A0");
    }

    symtab.intern(0x90, "dir90");
    symtab.intern(0x90A0, "sym90A0");
    symtab.intern(-2, "minus2");

    TEST("LDA  #dir90",   0x86, 0x90);
    TEST("CMPX #sym90A0", 0x8C, 0x90, 0xA0);
    TEST("LDY  #sym90A0", 0x10, 0x8E, 0x90, 0xA0);
    TEST("LDS  #sym90A0", 0x10, 0xCE, 0x90, 0xA0);
    TEST("LDA  # dir90",  0x86, 0x90);

    if (is6309()) {
        // HD6309
        TEST("SBCD #sym90A0", 0x10, 0x82, 0x90, 0xA0);
        TEST("LDE  #dir90",   0x11, 0x86, 0x90);

        TEST("LDQ  #sym90A0", 0xCD, 0x00, 0x00, 0x90, 0xA0);
        TEST("LDQ  #minus2",  0xCD, 0xFF, 0xFF, 0xFF, 0xFE);
        TEST("MULD #dir90",   0x11, 0x8F, 0x00, 0x90);
    }
}

static void test_direct() {
    TEST("NEG $10", 0x00, 0x10);
    TEST("COM $10", 0x03, 0x10);
    TEST("LSR $10", 0x04, 0x10);
    TEST("ROR $10", 0x06, 0x10);
    TEST("ASR $10", 0x07, 0x10);
    TEST("ASL $10", 0x08, 0x10);
    TEST("ROL $10", 0x09, 0x10);
    TEST("DEC $10", 0x0A, 0x10);
    TEST("INC $10", 0x0C, 0x10);
    TEST("TST $10", 0x0D, 0x10);
    TEST("CLR $10", 0x0F, 0x10);

    TEST("SUBA $90", 0x90, 0x90);
    TEST("CMPA $90", 0x91, 0x90);
    TEST("SBCA $90", 0x92, 0x90);
    TEST("ANDA $90", 0x94, 0x90);
    TEST("BITA $90", 0x95, 0x90);
    TEST("LDA  $90", 0x96, 0x90);
    TEST("STA  $90", 0x97, 0x90);
    TEST("EORA $90", 0x98, 0x90);
    TEST("ADCA $90", 0x99, 0x90);
    TEST("ORA  $90", 0x9A, 0x90);
    TEST("ADDA $90", 0x9B, 0x90);

    TEST("SUBB <$1290", 0xD0, 0x90);
    TEST("CMPB <$1290", 0xD1, 0x90);
    TEST("SBCB <$1290", 0xD2, 0x90);
    TEST("ANDB <$1290", 0xD4, 0x90);
    TEST("BITB <$1290", 0xD5, 0x90);
    TEST("LDB  <$1290", 0xD6, 0x90);
    TEST("STB  <$1290", 0xD7, 0x90);
    TEST("EORB <$1290", 0xD8, 0x90);
    TEST("ADCB <$1290", 0xD9, 0x90);
    TEST("ORB  <$1290", 0xDA, 0x90);
    TEST("ADDB <$1290", 0xDB, 0x90);

    TEST("SUBD $90",    0x93, 0x90);
    TEST("ADDD $90",    0xD3, 0x90);
    TEST("LDD  $90",    0xDC, 0x90);
    TEST("STD  <$1290", 0xDD, 0x90);
    TEST("CMPD <$1290", 0x10, 0x93, 0x90);

    TEST("CMPX $90",    0x9C, 0x90);
    TEST("LDX  $90",    0x9E, 0x90);
    TEST("STX  <$1290", 0x9F, 0x90);

    TEST("CMPY $90",    0x10, 0x9C, 0x90);
    TEST("LDY  $90",    0x10, 0x9E, 0x90);
    TEST("STY  <$1290", 0x10, 0x9F, 0x90);

    TEST("CMPU $90",    0x11, 0x93, 0x90);
    TEST("LDU  $90",    0xDE, 0x90);
    TEST("STU  <$1290", 0xDF, 0x90);

    TEST("CMPS $90",    0x11, 0x9C, 0x90);
    TEST("LDS  $90",    0x10, 0xDE, 0x90);
    TEST("STS  <$1290", 0x10, 0xDF, 0x90);

    TEST("JMP $10",    0x0E, 0x10);
    TEST("JSR <$1290", 0x9D, 0x90);

    if (is6309()) {
        // HD6309
        TEST("SBCD $90",    0x10, 0x92, 0x90);
        TEST("ANDD $90",    0x10, 0x94, 0x90);
        TEST("BITD $90",    0x10, 0x95, 0x90);
        TEST("EORD <$1290", 0x10, 0x98, 0x90);
        TEST("ADCD <$1290", 0x10, 0x99, 0x90);
        TEST("ORD  <$1290", 0x10, 0x9A, 0x90);

        TEST("SUBE $90", 0x11, 0x90, 0x90);
        TEST("ADDE $90", 0x11, 0x9B, 0x90);
        TEST("LDE  $90", 0x11, 0x96, 0x90);
        TEST("STE  $90", 0x11, 0x97, 0x90);
        TEST("CMPE $90", 0x11, 0x91, 0x90);

        TEST("SUBF <$1290", 0x11, 0xD0, 0x90);
        TEST("ADDF <$1290", 0x11, 0xDB, 0x90);
        TEST("LDF  <$1290", 0x11, 0xD6, 0x90);
        TEST("STF  <$1290", 0x11, 0xD7, 0x90);
        TEST("CMPF <$1290", 0x11, 0xD1, 0x90);

        TEST("SUBW $90",    0x10, 0x90, 0x90);
        TEST("ADDW $90",    0x10, 0x9B, 0x90);
        TEST("LDW  <$1290", 0x10, 0x96, 0x90);
        TEST("STW  <$1290", 0x10, 0x97, 0x90);
        TEST("CMPW <$1290", 0x10, 0x91, 0x90);

        TEST("LDQ  $90",    0x10, 0xDC, 0x90);
        TEST("STQ  <$1290", 0x10, 0xDD, 0x90);

        TEST("MULD <$1290", 0x11, 0x9F, 0x90);
        TEST("DIVD $90",    0x11, 0x9D, 0x90);
        TEST("DIVQ <$1290", 0x11, 0x9E, 0x90);

        TEST("OIM #$30,$10",    0x01, 0x30, 0x10);
        TEST("AIM #$30,$10",    0x02, 0x30, 0x10);
        TEST("EIM #$30,<$1290", 0x05, 0x30, 0x90);
        TEST("TIM #$30,<$1290", 0x0B, 0x30, 0x90);

        TEST("OIM #$30,,W",     0x61, 0x30, 0x8F);
        TEST("AIM #$30,W,X",    0x62, 0x30, 0x8E);
        TEST("AIM #$30,[W,X]",  0x62, 0x30, 0x9E);
        TEST("EIM #$30,,--X",   0x65, 0x30, 0x83);
        TEST("TIM #$30,*+$20,PCR",    0x6B, 0x30, 0x8C, 0x1C);
        TEST("TIM #$30,*+$120,PCR",   0x6B, 0x30, 0x8D, 0x01, 0x1B);
        TEST("TIM #$30,[*+$20,PCR]",  0x6B, 0x30, 0x9C, 0x1C);
        TEST("TIM #$30,[*+$120,PCR]", 0x6B, 0x30, 0x9D, 0x01, 0x1B);
    } else {
        ERUI("SBCD    $90");
        ERUI("ANDD    $90");
        ERUI("BITD    $90");
        ERUI("EORD <$1290");
        ERUI("ADCD <$1290");
        ERUI("ORD  <$1290");
        ERUI("SUBE    $90");
        ERUI("ADDE    $90");
        ERUI("LDE     $90");
        ERUI("STE     $90");
        ERUI("CMPE    $90");
        ERUI("SUBF <$1290");
        ERUI("ADDF <$1290");
        ERUI("LDF  <$1290");
        ERUI("STF  <$1290");
        ERUI("CMPF <$1290");
        ERUI("SUBW    $90");
        ERUI("ADDW    $90");
        ERUI("LDW  <$1290");
        ERUI("STW  <$1290");
        ERUI("CMPW <$1290");
        ERUI("LDQ     $90");
        ERUI("STQ  <$1290");
        ERUI("MULD <$1290");
        ERUI("DIVD    $90");
        ERUI("OIM #$30,$10");
        ERUI("AIM #$30,$10");
        ERUI("EIM #$30,<$1290");
        ERUI("TIM #$30,<$1290");
    }

    symtab.intern(0x10, "dir10");
    symtab.intern(0x90, "dir90");
    symtab.intern(0x1290, "sym1290");
    symtab.intern(0x90A0, "sym90A0");

    TEST("NEG  dir10",    0x00, 0x10);
    TEST("LDA  dir10",    0x96, 0x10);
    TEST("STB  <sym1290", 0xD7, 0x90);
    TEST("CMPX <sym1290", 0x9C, 0x90);
    TEST("STU  <dir10",   0xDF, 0x10);
    TEST("LDY  <dir10",   0x10, 0x9E, 0x10);
    TEST("LDS  <sym1290", 0x10, 0xDE, 0x90);
    TEST("JMP  <sym1290", 0x0E, 0x90);
    TEST("JSR  dir90",    0x9D, 0x90);

    if (is6309()) {
        // HD6309
        TEST("SBCD <sym90A0", 0x10, 0x92, 0xA0);
        TEST("LDE  dir90",    0x11, 0x96, 0x90);

        TEST("LDQ  <sym1290", 0x10, 0xDC, 0x90);
        TEST("MULD dir90",    0x11, 0x9F, 0x90);

        TEST("OIM #$30,dir10",    0x01, 0x30, 0x10);
        TEST("OIM #$30,<sym1290", 0x01, 0x30, 0x90);

        TEST("SETDP sym1290>>8");
        TEST("NEG dir10",    0x70, 0x00, 0x10);
        TEST("LDA dir10",    0xB6, 0x00, 0x10);
        TEST("LDA <dir10",   0x96, 0x10);
        TEST("NEG sym1290",  0x00, 0x90);
        TEST("LDA sym1290",  0x96, 0x90);
        TEST("LDA >sym1290", 0xB6, 0x12, 0x90);

        // HD6309
        TEST("LDE dir90",        0x11, 0xB6, 0x00, 0x90);
        TEST("LDQ sym1290",      0x10, 0xDC, 0x90);
        TEST("OIM #$30,sym1290", 0x01, 0x30, 0x90);

        TEST("SETDP sym90A0>>8");
        TEST("STU dir90",    0xFF, 0x00, 0x90);
        TEST("LDY dir90",    0x10, 0xBE, 0x00, 0x90);
        TEST("LDY <dir90",   0x10, 0x9E, 0x90);
        TEST("LDS sym90A0",  0x10, 0xDE, 0xA0);
        TEST("JMP sym90A0",  0x0E, 0xA0);
        TEST("JMP >sym90A0", 0x7E, 0x90, 0xA0);

        TEST("SBCD sym90A0",     0x10, 0x92, 0xA0);
        TEST("MULD dir90",       0x11, 0xBF, 0x00, 0x90);
        TEST("OIM  #$30,<dir10", 0x01, 0x30, 0x10);
    }
}

static void test_extended() {
    TEST("NEG $1234", 0x70, 0x12, 0x34);
    TEST("COM $1234", 0x73, 0x12, 0x34);
    TEST("LSR $1234", 0x74, 0x12, 0x34);
    TEST("ROR $1234", 0x76, 0x12, 0x34);
    TEST("ASR $1234", 0x77, 0x12, 0x34);
    TEST("ASL $1234", 0x78, 0x12, 0x34);
    TEST("ROL $1234", 0x79, 0x12, 0x34);
    TEST("DEC $1234", 0x7A, 0x12, 0x34);
    TEST("INC $1234", 0x7C, 0x12, 0x34);
    TEST("TST $1234", 0x7D, 0x12, 0x34);
    TEST("CLR $1234", 0x7F, 0x12, 0x34);

    TEST("SUBA >$90", 0xB0, 0x00, 0x90);
    TEST("CMPA >$90", 0xB1, 0x00, 0x90);
    TEST("SBCA >$90", 0xB2, 0x00, 0x90);
    TEST("ANDA >$90", 0xB4, 0x00, 0x90);
    TEST("BITA >$90", 0xB5, 0x00, 0x90);
    TEST("LDA  >$90", 0xB6, 0x00, 0x90);
    TEST("STA  >$90", 0xB7, 0x00, 0x90);
    TEST("EORA >$90", 0xB8, 0x00, 0x90);
    TEST("ADCA >$90", 0xB9, 0x00, 0x90);
    TEST("ORA  >$90", 0xBA, 0x00, 0x90);
    TEST("ADDA >$90", 0xBB, 0x00, 0x90);

    TEST("SUBB $9ABC", 0xF0, 0x9A, 0xBC);
    TEST("CMPB $9ABC", 0xF1, 0x9A, 0xBC);
    TEST("SBCB $9ABC", 0xF2, 0x9A, 0xBC);
    TEST("ANDB $9ABC", 0xF4, 0x9A, 0xBC);
    TEST("BITB $9ABC", 0xF5, 0x9A, 0xBC);
    TEST("LDB  $9ABC", 0xF6, 0x9A, 0xBC);
    TEST("STB  $9ABC", 0xF7, 0x9A, 0xBC);
    TEST("EORB $9ABC", 0xF8, 0x9A, 0xBC);
    TEST("ADCB $9ABC", 0xF9, 0x9A, 0xBC);
    TEST("ORB  $9ABC", 0xFA, 0x9A, 0xBC);
    TEST("ADDB $9ABC", 0xFB, 0x9A, 0xBC);

    TEST("SUBD $9ABC", 0xB3, 0x9A, 0xBC);
    TEST("ADDD $9ABC", 0xF3, 0x9A, 0xBC);
    TEST("LDD  $9ABC", 0xFC, 0x9A, 0xBC);
    TEST("STD  $9ABC", 0xFD, 0x9A, 0xBC);
    TEST("CMPD >$90",  0x10, 0xB3, 0x00, 0x90);

    TEST("CMPX $9ABC", 0xBC, 0x9A, 0xBC);
    TEST("LDX  >$90",  0xBE, 0x00, 0x90);
    TEST("STX  $9ABC", 0xBF, 0x9A, 0xBC);

    TEST("CMPY $9ABC", 0x10, 0xBC, 0x9A, 0xBC);
    TEST("LDY  >$90",  0x10, 0xBE, 0x00, 0x90);
    TEST("STY  $9ABC", 0x10, 0xBF, 0x9A, 0xBC);

    TEST("CMPU $9ABC", 0x11, 0xB3, 0x9A, 0xBC);
    TEST("LDU  >$90",  0xFE, 0x00, 0x90);
    TEST("STU  $9ABC", 0xFF, 0x9A, 0xBC);

    TEST("CMPS $9ABC", 0x11, 0xBC, 0x9A, 0xBC);
    TEST("LDS  >$90",  0x10, 0xFE, 0x00, 0x90);
    TEST("STS  $9ABC", 0x10, 0xFF, 0x9A, 0xBC);

    TEST("JMP $1234", 0x7E, 0x12, 0x34);
    TEST("JSR >$90",  0xBD, 0x00, 0x90);

    if (is6309()) {
        // HD6309
        TEST("SBCD $9ABC", 0x10, 0xB2, 0x9A, 0xBC);
        TEST("ANDD $9ABC", 0x10, 0xB4, 0x9A, 0xBC);
        TEST("BITD $9ABC", 0x10, 0xB5, 0x9A, 0xBC);
        TEST("EORD $9ABC", 0x10, 0xB8, 0x9A, 0xBC);
        TEST("ORD  >$90",  0x10, 0xBA, 0x00, 0x90);
        TEST("ADCD $9ABC", 0x10, 0xB9, 0x9A, 0xBC);

        TEST("SUBE $9ABC", 0x11, 0xB0, 0x9A, 0xBC);
        TEST("ADDE $9ABC", 0x11, 0xBB, 0x9A, 0xBC);
        TEST("LDE  >$90",  0x11, 0xB6, 0x00, 0x90);
        TEST("STE  $9ABC", 0x11, 0xB7, 0x9A, 0xBC);
        TEST("CMPE $9ABC", 0x11, 0xB1, 0x9A, 0xBC);

        TEST("SUBF $9ABC", 0x11, 0xF0, 0x9A, 0xBC);
        TEST("ADDF $9ABC", 0x11, 0xFB, 0x9A, 0xBC);
        TEST("LDF  >$90",  0x11, 0xF6, 0x00, 0x90);
        TEST("STF  $9ABC", 0x11, 0xF7, 0x9A, 0xBC);
        TEST("CMPF $9ABC", 0x11, 0xF1, 0x9A, 0xBC);

        TEST("SUBW $9ABC", 0x10, 0xB0, 0x9A, 0xBC);
        TEST("ADDW $9ABC", 0x10, 0xBB, 0x9A, 0xBC);
        TEST("LDW  >$90",  0x10, 0xB6, 0x00, 0x90);
        TEST("STW  $9ABC", 0x10, 0xB7, 0x9A, 0xBC);
        TEST("CMPW $9ABC", 0x10, 0xB1, 0x9A, 0xBC);

        TEST("LDQ  >$90",  0x10, 0xFC, 0x00, 0x90);
        TEST("STQ  $1290", 0x10, 0xFD, 0x12, 0x90);

        TEST("MULD $1290", 0x11, 0xBF, 0x12, 0x90);
        TEST("DIVD >$90",  0x11, 0xBD, 0x00, 0x90);
        TEST("DIVQ $1290", 0x11, 0xBE, 0x12, 0x90);

        TEST("OIM #$30,>$10",  0x71, 0x30, 0x00, 0x10);
        TEST("AIM #$30,>$10",  0x72, 0x30, 0x00, 0x10);
        TEST("EIM #$30,$1290", 0x75, 0x30, 0x12, 0x90);
        TEST("TIM #$30,$1290", 0x7B, 0x30, 0x12, 0x90);
    } else {
        ERUI("SBCD $9ABC");
        ERUI("ANDD $9ABC");
        ERUI("BITD $9ABC");
        ERUI("EORD $9ABC");
        ERUI("ORD   >$90");
        ERUI("ADCD $9ABC");
        ERUI("SUBE $9ABC");
        ERUI("ADDE $9ABC");
        ERUI("LDE   >$90");
        ERUI("STE  $9ABC");
        ERUI("CMPE $9ABC");
        ERUI("SUBF $9ABC");
        ERUI("ADDF $9ABC");
        ERUI("LDF   >$90");
        ERUI("STF  $9ABC");
        ERUI("CMPF $9ABC");
        ERUI("SUBW $9ABC");
        ERUI("ADDW $9ABC");
        ERUI("LDW   >$90");
        ERUI("STW  $9ABC");
        ERUI("CMPW $9ABC");
        ERUI("LDQ   >$90");
        ERUI("STQ  $1290");
        ERUI("MULD $1290");
        ERUI("DIVD  >$90");
        ERUI("OIM  #$30,>$10");
        ERUI("AIM  #$30,>$10");
        ERUI("EIM #$30,$1290");
        ERUI("TIM #$30,$1290");
    }

    symtab.intern(0x0090, "dir90");
    symtab.intern(0x1290, "sym1290");

    TEST("NEG >dir90",   0x70, 0x00, 0x90);
    TEST("LDA  sym1290", 0xB6, 0x12, 0x90);
    TEST("STB >dir90",   0xF7, 0x00, 0x90);
    TEST("CMPX sym1290", 0xBC, 0x12, 0x90);
    TEST("STU >dir90",   0xFF, 0x00, 0x90);
    TEST("LDY  sym1290", 0x10, 0xBE, 0x12, 0x90);
    TEST("LDS >dir90",   0x10, 0xFE, 0x00, 0x90);
    TEST("JMP  sym1290", 0x7E, 0x12, 0x90);
    TEST("JSR >dir90",   0xBD, 0x00, 0x90);

    if (is6309()) {
        // HD6309
        TEST("SBCD sym1290", 0x10, 0xB2, 0x12, 0x90);
        TEST("LDE  >dir90",  0x11, 0xB6, 0x00, 0x90);

        TEST("LDQ  sym1290",  0x10, 0xFC, 0x12, 0x90);
        TEST("MULD >dir90",   0x11, 0xBF, 0x00, 0x90);

        TEST("OIM #$30,>dir90",  0x71, 0x30, 0x00, 0x90);
        TEST("OIM #$30,sym1290", 0x71, 0x30, 0x12, 0x90);
    }
}

static void test_indexed() {
    TEST("LEAX ,Y", 0x30, 0xA4);
    TEST("LEAY ,U", 0x31, 0xC4);
    TEST("LEAU ,S", 0x33, 0xE4);
    TEST("LEAS ,X", 0x32, 0x84);

    TEST("NEG  ,X", 0x60, 0x84);
    TEST("COM  ,X", 0x63, 0x84);
    TEST("LSR  ,X", 0x64, 0x84);
    TEST("ROR  ,X", 0x66, 0x84);
    TEST("ASR  ,X", 0x67, 0x84);
    TEST("ASL  ,X", 0x68, 0x84);
    TEST("ROL  ,X", 0x69, 0x84);
    TEST("DEC  ,X", 0x6A, 0x84);
    TEST("INC  ,X", 0x6C, 0x84);
    TEST("TST  ,X", 0x6D, 0x84);
    TEST("CLR  ,X", 0x6F, 0x84);

    TEST("SUBA ,Y", 0xA0, 0xA4);
    TEST("CMPA ,Y", 0xA1, 0xA4);
    TEST("SBCA ,Y", 0xA2, 0xA4);
    TEST("ANDA ,Y", 0xA4, 0xA4);
    TEST("BITA ,Y", 0xA5, 0xA4);
    TEST("LDA  ,Y", 0xA6, 0xA4);
    TEST("STA  ,Y", 0xA7, 0xA4);
    TEST("EORA ,Y", 0xA8, 0xA4);
    TEST("ADCA ,Y", 0xA9, 0xA4);
    TEST("ORA  ,Y", 0xAA, 0xA4);
    TEST("ADDA ,Y", 0xAB, 0xA4);

    TEST("SUBB ,U", 0xE0, 0xC4);
    TEST("CMPB ,U", 0xE1, 0xC4);
    TEST("SBCB ,U", 0xE2, 0xC4);
    TEST("ANDB ,U", 0xE4, 0xC4);
    TEST("BITB ,U", 0xE5, 0xC4);
    TEST("LDB  ,U", 0xE6, 0xC4);
    TEST("STB  ,U", 0xE7, 0xC4);
    TEST("EORB ,U", 0xE8, 0xC4);
    TEST("ADCB ,U", 0xE9, 0xC4);
    TEST("ORB  ,U", 0xEA, 0xC4);
    TEST("ADDB ,U", 0xEB, 0xC4);

    TEST("SUBD ,S", 0xA3, 0xE4);
    TEST("ADDD ,S", 0xE3, 0xE4);
    TEST("LDD  ,S", 0xEC, 0xE4);
    TEST("STD  ,S", 0xED, 0xE4);
    TEST("CMPD ,S", 0x10, 0xA3, 0xE4);

    TEST("CMPX 2,Y ; 2,X A.2", 0xAC, 0x22);
    TEST("LDX  2,Y", 0xAE, 0x22);
    TEST("STX  2,Y", 0xAF, 0x22);

    TEST("CMPY -2,X", 0x10, 0xAC, 0x1E);
    TEST("LDY  -2,X", 0x10, 0xAE, 0x1E);
    TEST("STY  -2,X", 0x10, 0xAF, 0x1E);

    TEST("CMPU ,X++", 0x11, 0xA3, 0x81);
    TEST("LDU  ,X++", 0xEE, 0x81);
    TEST("STU  ,X++", 0xEF, 0x81);

    TEST("CMPS ,--Y", 0x11, 0xAC, 0xA3);
    TEST("LDS  ,--Y", 0x10, 0xEE, 0xA3);
    TEST("STS  ,--Y", 0x10, 0xEF, 0xA3);

    TEST("JMP  [,X]",   0x6E, 0x94);
    TEST("JSR  [,X++]", 0xAD, 0x91);

    if (is6309()) {
        // HD6309
        TEST("SBCD ,X",   0x10, 0xA2, 0x84);
        TEST("ANDD ,X++", 0x10, 0xA4, 0x81);
        TEST("BITD ,--X", 0x10, 0xA5, 0x83);
        TEST("EORD [,X]", 0x10, 0xA8, 0x94);
        TEST("ADCD [,X++]", 0x10, 0xA9, 0x91);
        TEST("ORD  [,--X]", 0x10, 0xAA, 0x93);

        TEST("SUBE ,X", 0x11, 0xA0, 0x84);
        TEST("ADDE ,X", 0x11, 0xAB, 0x84);
        TEST("LDE  ,X", 0x11, 0xA6, 0x84);
        TEST("STE  ,X", 0x11, 0xA7, 0x84);
        TEST("CMPE ,X", 0x11, 0xA1, 0x84);

        TEST("SUBF ,X", 0x11, 0xE0, 0x84);
        TEST("ADDF ,X", 0x11, 0xEB, 0x84);
        TEST("LDF  ,X", 0x11, 0xE6, 0x84);
        TEST("STF  ,X", 0x11, 0xE7, 0x84);
        TEST("CMPF ,X", 0x11, 0xE1, 0x84);

        TEST("SUBW ,X", 0x10, 0xA0, 0x84);
        TEST("CMPW ,X", 0x10, 0xA1, 0x84);
        TEST("LDW  ,X", 0x10, 0xA6, 0x84);
        TEST("STW  ,X", 0x10, 0xA7, 0x84);
        TEST("ADDW ,X", 0x10, 0xAB, 0x84);

        TEST("LDQ  ,X", 0x10, 0xEC, 0x84);
        TEST("STQ  ,X", 0x10, 0xED, 0x84);

        TEST("MULD ,X",   0x11, 0xAF, 0x84);
        TEST("DIVD -1,X", 0x11, 0xAD, 0x1F);
        TEST("DIVQ 1,X",  0x11, 0xAE, 0x01);

        TEST("OIM #$30,,X",     0x61, 0x30, 0x84);
        TEST("AIM #$30,,X+",    0x62, 0x30, 0x80);
        TEST("EIM #$30,[,--X]", 0x65, 0x30, 0x93);
        TEST("TIM #$30,-2,X",   0x6B, 0x30, 0x1E);
    } else {
        ERUI("SBCD ,X");
        ERUI("ANDD ,X++");
        ERUI("BITD ,--X");
        ERUI("EORD [,X]");
        ERUI("ADCD [,X++]");
        ERUI("ORD  [,--X]");
        ERUI("SUBE ,X");
        ERUI("ADDE ,X");
        ERUI("LDE  ,X");
        ERUI("STE  ,X");
        ERUI("CMPE ,X");
        ERUI("SUBF ,X");
        ERUI("ADDF ,X");
        ERUI("LDF  ,X");
        ERUI("STF  ,X");
        ERUI("CMPF ,X");
        ERUI("SUBW ,X");
        ERUI("CMPW ,X");
        ERUI("LDW  ,X");
        ERUI("STW  ,X");
        ERUI("ADDW ,X");
        ERUI("LDQ  ,X");
        ERUI("STQ  ,X");
        ERUI("MULD ,X");
        ERUI("DIVD -1,X");
        ERUI("OIM #$30,,X");
        ERUI("AIM #$30,,X+");
        ERUI("EIM #$30,[,--X]");
        ERUI("TIM #$30,-2,X");
    }
}

static void test_indexed_mode() {
    TEST("LDA   ,X", 0xA6, 0x84);
    TEST("LDA   ,Y", 0xA6, 0xA4);
    TEST("LDA   ,U", 0xA6, 0xC4);
    TEST("LDA   ,S", 0xA6, 0xE4);
    TEST("LDA  0,X", 0xA6, 0x84);
    TEST("LDA  0,Y", 0xA6, 0xA4);
    TEST("LDA  0,U", 0xA6, 0xC4);
    TEST("LDA  0,S", 0xA6, 0xE4);
    TEST("LDA  [,X]", 0xA6, 0x94);
    TEST("LDA  [,Y]", 0xA6, 0xB4);
    TEST("LDA  [,U]", 0xA6, 0xD4);
    TEST("LDA  [,S]", 0xA6, 0xF4);
    TEST("LDA [0,X]", 0xA6, 0x94);
    TEST("LDA [0,Y]", 0xA6, 0xB4);
    TEST("LDA [0,U]", 0xA6, 0xD4);
    TEST("LDA [0,S]", 0xA6, 0xF4);

    TEST("LDA <<0,X", 0xA6, 0x00);
    TEST("LDA <<0,Y", 0xA6, 0x20);
    TEST("LDA <<0,U", 0xA6, 0x40);
    TEST("LDA <<0,S", 0xA6, 0x60);
    TEST("LDA  -1,X", 0xA6, 0x1F);
    TEST("LDA  -1,Y", 0xA6, 0x3F);
    TEST("LDA  -1,U", 0xA6, 0x5F);
    TEST("LDA  -1,S", 0xA6, 0x7F);
    TEST("LDA  15,X", 0xA6, 0x0F);
    TEST("LDA  15,Y", 0xA6, 0x2F);
    TEST("LDA  15,U", 0xA6, 0x4F);
    TEST("LDA  15,S", 0xA6, 0x6F);
    TEST("LDA -16,X", 0xA6, 0x10);
    TEST("LDA -16,Y", 0xA6, 0x30);
    TEST("LDA -16,U", 0xA6, 0x50);
    TEST("LDA -16,S", 0xA6, 0x70);

    TEST("LDA    <0,X", 0xA6, 0x88, 0x00);
    TEST("LDA    16,X", 0xA6, 0x88, 0x10);
    TEST("LDA   -17,X", 0xA6, 0x88, 0xEF);
    TEST("LDA   127,X", 0xA6, 0x88, 0x7F);
    TEST("LDA  -128,X", 0xA6, 0x88, 0x80);
    TEST("LDA    <0,Y", 0xA6, 0xA8, 0x00);
    TEST("LDA   127,Y", 0xA6, 0xA8, 0x7F);
    TEST("LDA  -128,Y", 0xA6, 0xA8, 0x80);
    TEST("LDA    <0,U", 0xA6, 0xC8, 0x00);
    TEST("LDA   127,U", 0xA6, 0xC8, 0x7F);
    TEST("LDA  -128,U", 0xA6, 0xC8, 0x80);
    TEST("LDA    <0,S", 0xA6, 0xE8, 0x00);
    TEST("LDA   127,S", 0xA6, 0xE8, 0x7F);
    TEST("LDA  -128,S", 0xA6, 0xE8, 0x80);
    TEST("LDA   0,PC",  0xA6, 0x8C, 0x00);
    TEST("LDA  127,PC", 0xA6, 0x8C, 0x7F);
    TEST("LDA -128,PC", 0xA6, 0x8C, 0x80);
    TEST("LDA    [<0,X]", 0xA6, 0x98, 0x00);
    TEST("LDA    [15,X]", 0xA6, 0x98, 0x0F);
    TEST("LDA   [127,X]", 0xA6, 0x98, 0x7F);
    TEST("LDA   [-16,X]", 0xA6, 0x98, 0xF0);
    TEST("LDA  [-128,X]", 0xA6, 0x98, 0x80);
    TEST("LDA    [<0,Y]", 0xA6, 0xB8, 0x00);
    TEST("LDA    [15,Y]", 0xA6, 0xB8, 0x0F);
    TEST("LDA   [127,Y]", 0xA6, 0xB8, 0x7F);
    TEST("LDA   [-16,Y]", 0xA6, 0xB8, 0xF0);
    TEST("LDA  [-128,Y]", 0xA6, 0xB8, 0x80);
    TEST("LDA    [<0,U]", 0xA6, 0xD8, 0x00);
    TEST("LDA    [15,U]", 0xA6, 0xD8, 0x0F);
    TEST("LDA   [127,U]", 0xA6, 0xD8, 0x7F);
    TEST("LDA   [-16,U]", 0xA6, 0xD8, 0xF0);
    TEST("LDA  [-128,U]", 0xA6, 0xD8, 0x80);
    TEST("LDA    [<0,S]", 0xA6, 0xF8, 0x00);
    TEST("LDA    [15,S]", 0xA6, 0xF8, 0x0F);
    TEST("LDA   [127,S]", 0xA6, 0xF8, 0x7F);
    TEST("LDA   [-16,S]", 0xA6, 0xF8, 0xF0);
    TEST("LDA  [-128,S]", 0xA6, 0xF8, 0x80);
    TEST("LDA    [0,PC]", 0xA6, 0x9C, 0x00);
    TEST("LDA  [127,PC]", 0xA6, 0x9C, 0x7F);
    TEST("LDA [-128,PC]", 0xA6, 0x9C, 0x80);

    TEST("LDA      >0,X", 0xA6, 0x89, 0x00, 0x00);
    TEST("LDA    >127,X", 0xA6, 0x89, 0x00, 0x7F);
    TEST("LDA   >-128,X", 0xA6, 0x89, 0xFF, 0x80);
    TEST("LDA     128,X", 0xA6, 0x89, 0x00, 0x80);
    TEST("LDA    -129,X", 0xA6, 0x89, 0xFF, 0x7F);
    TEST("LDA   32767,X", 0xA6, 0x89, 0x7F, 0xFF);
    TEST("LDA  -32768,X", 0xA6, 0x89, 0x80, 0x00);
    TEST("LDA      >0,Y", 0xA6, 0xA9, 0x00, 0x00);
    TEST("LDA   32767,Y", 0xA6, 0xA9, 0x7F, 0xFF);
    TEST("LDA  -32768,Y", 0xA6, 0xA9, 0x80, 0x00);
    TEST("LDA      >0,U", 0xA6, 0xC9, 0x00, 0x00);
    TEST("LDA   32767,U", 0xA6, 0xC9, 0x7F, 0xFF);
    TEST("LDA  -32768,U", 0xA6, 0xC9, 0x80, 0x00);
    TEST("LDA      >0,S", 0xA6, 0xE9, 0x00, 0x00);
    TEST("LDA   32767,S", 0xA6, 0xE9, 0x7F, 0xFF);
    TEST("LDA  -32768,S", 0xA6, 0xE9, 0x80, 0x00);
    TEST("LDA     >0,PC", 0xA6, 0x8D, 0x00, 0x00);
    TEST("LDA  32767,PC", 0xA6, 0x8D, 0x7F, 0xFF);
    TEST("LDA -32768,PC", 0xA6, 0x8D, 0x80, 0x00);
    TEST("LDA      [>0,X]", 0xA6, 0x99, 0x00, 0x00);
    TEST("LDA    [>127,X]", 0xA6, 0x99, 0x00, 0x7F);
    TEST("LDA   [>-128,X]", 0xA6, 0x99, 0xFF, 0x80);
    TEST("LDA     [128,X]", 0xA6, 0x99, 0x00, 0x80);
    TEST("LDA    [-129,X]", 0xA6, 0x99, 0xFF, 0x7F);
    TEST("LDA   [32767,X]", 0xA6, 0x99, 0x7F, 0xFF);
    TEST("LDA  [-32768,X]", 0xA6, 0x99, 0x80, 0x00);
    TEST("LDA      [>0,Y]", 0xA6, 0xB9, 0x00, 0x00);
    TEST("LDA   [32767,Y]", 0xA6, 0xB9, 0x7F, 0xFF);
    TEST("LDA  [-32768,Y]", 0xA6, 0xB9, 0x80, 0x00);
    TEST("LDA      [>0,U]", 0xA6, 0xD9, 0x00, 0x00);
    TEST("LDA   [32767,U]", 0xA6, 0xD9, 0x7F, 0xFF);
    TEST("LDA  [-32768,U]", 0xA6, 0xD9, 0x80, 0x00);
    TEST("LDA      [>0,S]", 0xA6, 0xF9, 0x00, 0x00);
    TEST("LDA   [32767,S]", 0xA6, 0xF9, 0x7F, 0xFF);
    TEST("LDA  [-32768,S]", 0xA6, 0xF9, 0x80, 0x00);
    TEST("LDA     [>0,PC]", 0xA6, 0x9D, 0x00, 0x00);
    TEST("LDA  [32767,PC]", 0xA6, 0x9D, 0x7F, 0xFF);
    TEST("LDA [-32768,PC]", 0xA6, 0x9D, 0x80, 0x00);

    TEST("LDA <<0,X",  0xA6, 0x00);
    TEST("LDA  <0,X",  0xA6, 0x88, 0x00);
    TEST("LDA  >0,X",  0xA6, 0x89, 0x00, 0x00);
    TEST("LDA [<0,X]", 0xA6, 0x98, 0x00);
    TEST("LDA [>0,X]", 0xA6, 0x99, 0x00, 0x00);

    TEST("LDA A,X",   0xA6, 0x86);
    TEST("LDA B,X",   0xA6, 0x85);
    TEST("LDA D,X",   0xA6, 0x8B);
    TEST("LDA [A,X]", 0xA6, 0x96);
    TEST("LDA [B,X]", 0xA6, 0x95);
    TEST("LDA [D,X]", 0xA6, 0x9B);

    TEST("LDA A,Y",   0xA6, 0xA6);
    TEST("LDA B,Y",   0xA6, 0xA5);
    TEST("LDA D,Y",   0xA6, 0xAB);
    TEST("LDA [A,Y]", 0xA6, 0xB6);
    TEST("LDA [B,Y]", 0xA6, 0xB5);
    TEST("LDA [D,Y]", 0xA6, 0xBB);

    TEST("LDA A,U",   0xA6, 0xC6);
    TEST("LDA B,U",   0xA6, 0xC5);
    TEST("LDA D,U",   0xA6, 0xCB);
    TEST("LDA [A,U]", 0xA6, 0xD6);
    TEST("LDA [B,U]", 0xA6, 0xD5);
    TEST("LDA [D,U]", 0xA6, 0xDB);

    TEST("LDA A,S",   0xA6, 0xE6);
    TEST("LDA B,S",   0xA6, 0xE5);
    TEST("LDA D,S",   0xA6, 0xEB);
    TEST("LDA [A,S]", 0xA6, 0xF6);
    TEST("LDA [B,S]", 0xA6, 0xF5);
    TEST("LDA [D,S]", 0xA6, 0xFB);

    TEST("LDA ,X+",    0xA6, 0x80);
    TEST("LDA ,X++",   0xA6, 0x81);
    TEST("LDA ,-X",    0xA6, 0x82);
    TEST("LDA ,--X",   0xA6, 0x83);
    TEST("LDA [,X++]", 0xA6, 0x91);
    TEST("LDA [,--X]", 0xA6, 0x93);

    TEST("LDA ,Y+",    0xA6, 0xA0);
    TEST("LDA ,Y++",   0xA6, 0xA1);
    TEST("LDA ,-Y",    0xA6, 0xA2);
    TEST("LDA ,--Y",   0xA6, 0xA3);
    TEST("LDA [,Y++]", 0xA6, 0xB1);
    TEST("LDA [,--Y]", 0xA6, 0xB3);

    TEST("LDA ,U+",    0xA6, 0xC0);
    TEST("LDA ,U++",   0xA6, 0xC1);
    TEST("LDA ,-U",    0xA6, 0xC2);
    TEST("LDA ,--U",   0xA6, 0xC3);
    TEST("LDA [,U++]", 0xA6, 0xD1);
    TEST("LDA [,--U]", 0xA6, 0xD3);

    TEST("LDA ,S+",    0xA6, 0xE0);
    TEST("LDA ,S++",   0xA6, 0xE1);
    TEST("LDA ,-S",    0xA6, 0xE2);
    TEST("LDA ,--S",   0xA6, 0xE3);
    TEST("LDA [,S++]", 0xA6, 0xF1);
    TEST("LDA [,--S]", 0xA6, 0xF3);

    ATEST(0x1000, "LDA $1003,PCR",   0xA6, 0x8C, 0x00);
    ATEST(0x1000, "LDA $1082,PCR",   0xA6, 0x8C, 0x7F);
    ATEST(0x1000, "LDA $0F83,PCR",   0xA6, 0x8C, 0x80);
    ATEST(0x1000, "LDA [$1003,PCR]", 0xA6, 0x9C, 0x00);
    ATEST(0x1000, "LDA [$1082,PCR]", 0xA6, 0x9C, 0x7F);
    ATEST(0x1000, "LDA [$0F83,PCR]", 0xa6, 0x9C, 0x80);

    ATEST(0x1000, "LDA $1004,PCR",   0xA6, 0x8C, 0x01);
    ATEST(0x1000, "LDA $9003,PCR",   0xA6, 0x8D, 0x7F, 0xFF);
    ATEST(0x1000, "LDA $9004,PCR",   0xA6, 0x8D, 0x80, 0x00);
    ATEST(0x1000, "LDA [$1004,PCR]", 0xA6, 0x9C, 0x01);
    ATEST(0x1000, "LDA [$9003,PCR]", 0xA6, 0x9D, 0x7F, 0xFF);
    ATEST(0x1000, "LDA [$9004,PCR]", 0xA6, 0x9D, 0x80, 0x00);

    TEST("LDA [$1234]", 0xA6, 0x9F, 0x12, 0x34);

    if (is6309()) {
        // HD6309
        TEST("LDA E,X",   0xA6, 0x87);
        TEST("LDA F,X",   0xA6, 0x8A);
        TEST("LDA W,X",   0xA6, 0x8E);
        TEST("LDA [E,X]", 0xA6, 0x97);
        TEST("LDA [F,X]", 0xA6, 0x9A);
        TEST("LDA [W,X]", 0xA6, 0x9E);

        TEST("LDA E,Y",   0xA6, 0xA7);
        TEST("LDA F,Y",   0xA6, 0xAA);
        TEST("LDA W,Y",   0xA6, 0xAE);
        TEST("LDA [E,Y]", 0xA6, 0xB7);
        TEST("LDA [F,Y]", 0xA6, 0xBA);
        TEST("LDA [W,Y]", 0xA6, 0xBE);

        TEST("LDA E,U",   0xA6, 0xC7);
        TEST("LDA F,U",   0xA6, 0xCA);
        TEST("LDA W,U",   0xA6, 0xCE);
        TEST("LDA [E,U]", 0xA6, 0xD7);
        TEST("LDA [F,U]", 0xA6, 0xDA);
        TEST("LDA [W,U]", 0xA6, 0xDE);

        TEST("LDA E,S",   0xA6, 0xE7);
        TEST("LDA F,S",   0xA6, 0xEA);
        TEST("LDA W,S",   0xA6, 0xEE);
        TEST("LDA [E,S]", 0xA6, 0xF7);
        TEST("LDA [F,S]", 0xA6, 0xFA);
        TEST("LDA [W,S]", 0xA6, 0xFE);

        TEST("LDA       ,W",   0xA6, 0x8F);
        TEST("LDA      0,W",   0xA6, 0x8F);
        TEST("LDA     >0,W",   0xA6, 0xAF, 0x00, 0x00);
        TEST("LDA  32767,W",   0xA6, 0xAF, 0x7F, 0xFF);
        TEST("LDA -32768,W",   0xA6, 0xAF, 0x80, 0x00);
        TEST("LDA       [,W]", 0xA6, 0x90);
        TEST("LDA      [0,W]", 0xA6, 0x90);
        TEST("LDA     [>0,W]", 0xA6, 0xB0, 0x00, 0x00);
        TEST("LDA  [32767,W]", 0xA6, 0xB0, 0x7F, 0xFF);
        TEST("LDA [-32768,W]", 0xA6, 0xB0, 0x80, 0x00);
        TEST("LDA ,W++",       0xA6, 0xCF);
        TEST("LDA ,--W",       0xA6, 0xEF);
        TEST("LDA [,W++]",     0xA6, 0xD0);
        TEST("LDA [,--W]",     0xA6, 0xF0);
    } else {
        ERRT("LDA E,X",   UNKNOWN_OPERAND, "E,X",   0xA6, 0x84);
        ERRT("LDA F,X",   UNKNOWN_OPERAND, "F,X",   0xA6, 0x84);
        ERRT("LDA W,X",   UNKNOWN_OPERAND, "W,X",   0xA6, 0x84);
        ERRT("LDA [E,X]", UNKNOWN_OPERAND, "[E,X]", 0xA6, 0x94);
        ERRT("LDA [F,X]", UNKNOWN_OPERAND, "[F,X]", 0xA6, 0x94);
        ERRT("LDA [W,X]", UNKNOWN_OPERAND, "[W,X]", 0xA6, 0x94);
        ERRT("LDA E,Y",   UNKNOWN_OPERAND, "E,Y",   0xA6, 0xA4);
        ERRT("LDA F,Y",   UNKNOWN_OPERAND, "F,Y",   0xA6, 0xA4);
        ERRT("LDA W,Y",   UNKNOWN_OPERAND, "W,Y",   0xA6, 0xA4);
        ERRT("LDA [E,Y]", UNKNOWN_OPERAND, "[E,Y]", 0xA6, 0xB4);
        ERRT("LDA [F,Y]", UNKNOWN_OPERAND, "[F,Y]", 0xA6, 0xB4);
        ERRT("LDA [W,Y]", UNKNOWN_OPERAND, "[W,Y]", 0xA6, 0xB4);
        ERRT("LDA E,U",   UNKNOWN_OPERAND, "E,U",   0xA6, 0xC4);
        ERRT("LDA F,U",   UNKNOWN_OPERAND, "F,U",   0xA6, 0xC4);
        ERRT("LDA W,U",   UNKNOWN_OPERAND, "W,U",   0xA6, 0xC4);
        ERRT("LDA [E,U]", UNKNOWN_OPERAND, "[E,U]", 0xA6, 0xD4);
        ERRT("LDA [F,U]", UNKNOWN_OPERAND, "[F,U]", 0xA6, 0xD4);
        ERRT("LDA [W,U]", UNKNOWN_OPERAND, "[W,U]", 0xA6, 0xD4);
        ERRT("LDA E,S",   UNKNOWN_OPERAND, "E,S",   0xA6, 0xE4);
        ERRT("LDA F,S",   UNKNOWN_OPERAND, "F,S",   0xA6, 0xE4);
        ERRT("LDA W,S",   UNKNOWN_OPERAND, "W,S",   0xA6, 0xE4);
        ERRT("LDA [E,S]", UNKNOWN_OPERAND, "[E,S]", 0xA6, 0xF4);
        ERRT("LDA [F,S]", UNKNOWN_OPERAND, "[F,S]", 0xA6, 0xF4);
        ERRT("LDA [W,S]", UNKNOWN_OPERAND, "[W,S]", 0xA6, 0xF4);
        ERRT("LDA       ,W",   UNKNOWN_OPERAND, ",W",         0xA6, 0x00);
        ERRT("LDA      0,W",   UNKNOWN_OPERAND, "0,W",        0xA6, 0x00);
        ERRT("LDA     >0,W",   UNKNOWN_OPERAND, ">0,W",       0xA6, 0x00, 0x00, 0x00);
        ERRT("LDA  32767,W",   UNKNOWN_OPERAND, "32767,W",    0xA6, 0x00, 0x7F, 0xFF);
        ERRT("LDA -32768,W",   UNKNOWN_OPERAND, "-32768,W",   0xA6, 0x00, 0x80, 0x00);
        ERRT("LDA       [,W]", UNKNOWN_OPERAND, "[,W]",       0xA6, 0x00);
        ERRT("LDA      [0,W]", UNKNOWN_OPERAND, "[0,W]",      0xA6, 0x00);
        ERRT("LDA     [>0,W]", UNKNOWN_OPERAND, "[>0,W]",     0xA6, 0x00, 0x00, 0x00);
        ERRT("LDA  [32767,W]", UNKNOWN_OPERAND, "[32767,W]",  0xA6, 0x00, 0x7F, 0xFF);
        ERRT("LDA [-32768,W]", UNKNOWN_OPERAND, "[-32768,W]", 0xA6, 0x00, 0x80, 0x00);
        ERRT("LDA ,W++",       UNKNOWN_OPERAND, ",W++",       0xA6, 0x00);
        ERRT("LDA ,--W",       UNKNOWN_OPERAND, ",--W",       0xA6, 0x00);
        ERRT("LDA [,W++]",     UNKNOWN_OPERAND, "[,W++]",     0xA6, 0x00);
        ERRT("LDA [,--W]",     UNKNOWN_OPERAND, "[,--W]",     0xA6, 0x00);
    }

    symtab.intern(0x0F83, "label0F83");
    symtab.intern(0x1003, "label1003");
    symtab.intern(0x1004, "label1004");
    symtab.intern(0x1082, "label1082");
    symtab.intern(0x1234, "label1234");
    symtab.intern(0x9003, "label9003");
    symtab.intern(0x9004, "label9004");

    ATEST(0x1000, "LDA label1003,PCR",   0xA6, 0x8C, 0x00);
    ATEST(0x1000, "LDA label1082,PCR",   0xA6, 0x8C, 0x7F);
    ATEST(0x1000, "LDA label0F83,PCR",   0xA6, 0x8C, 0x80);
    ATEST(0x1000, "LDA [label1003,PCR]", 0xA6, 0x9C, 0x00);
    ATEST(0x1000, "LDA [label1082,PCR]", 0xA6, 0x9C, 0x7F);
    ATEST(0x1000, "LDA [label0F83,PCR]", 0xA6, 0x9C, 0x80);

    ATEST(0x1000, "LDA label1004,PCR",   0xA6, 0x8C, 0x01);
    ATEST(0x1000, "LDA label9003,PCR",   0xA6, 0x8D, 0x7F, 0xFF);
    ATEST(0x1000, "LDA label9004,PCR",   0xA6, 0x8D, 0x80, 0x00);
    ATEST(0x1000, "LDA [label1004,PCR]", 0xA6, 0x9C, 0x01);
    ATEST(0x1000, "LDA [label9003,PCR]", 0xA6, 0x9D, 0x7F, 0xFF);
    ATEST(0x1000, "LDA [label9004,PCR]", 0xA6, 0x9D, 0x80, 0x00);

    ATEST(0x1000, "LDA [label1234]", 0xA6, 0x9F, 0x12, 0x34);
}

static void test_transfer() {
    if (is6309()) {
        // HD6309
        TEST("TFM X+,Y+", 0x11, 0x38, 0x12);
        TEST("TFM X-,Y-", 0x11, 0x39, 0x12);
        TEST("TFM X+,Y",  0x11, 0x3A, 0x12);
        TEST("TFM X,Y+",  0x11, 0x3B, 0x12);

        TEST("TFM D+,X+", 0x11, 0x38, 0x01);
        TEST("TFM D-,X-", 0x11, 0x39, 0x01);
        TEST("TFM D+,X",  0x11, 0x3A, 0x01);
        TEST("TFM D,X+",  0x11, 0x3B, 0x01);

        TEST("TFM D+,X+", 0x11, 0x38, 0x01);
        TEST("TFM Y+,U+", 0x11, 0x38, 0x23);
        TEST("TFM U+,S+", 0x11, 0x38, 0x34);
        TEST("TFM S+,D+", 0x11, 0x38, 0x40);

        ERRT("TFM X+,W",  REGISTER_NOT_ALLOWED, "W",     0x11, 0x3A, 0x16);
        ERRT("TFM W+,X",  REGISTER_NOT_ALLOWED, "W+,X",  0x11, 0x3A, 0x61);
        ERRT("TFM W-,X-", REGISTER_NOT_ALLOWED, "W-,X-", 0x11, 0x39, 0x61);
    } else {
        ERUI("TFM D,Y");
        ERUI("TFM X+,Y+");
    }
}

static void test_bit_position() {
    if (is6309()) {
        // HD6309
        TEST("BAND  A.1,$34.2",                                     0x11, 0x30, 0x51, 0x34);
        ERRT("BAND  A.9,$34.2",   ILLEGAL_BIT_NUMBER, ".9,$34.2",   0x11, 0x30, 0x51, 0x34);
        ERRT("BAND  A.1,$34,10",  ILLEGAL_BIT_NUMBER, ",10",        0x11, 0x30, 0x51, 0x34);
        ERRT("BAND  A.1,$34.2*8", ILLEGAL_BIT_NUMBER, ".2*8",       0x11, 0x30, 0x41, 0x34);
        ERRT("BAND  A,9,$34,2",   ILLEGAL_BIT_NUMBER, ",9,$34,2",   0x11, 0x30, 0x51, 0x34);
        ERRT("BAND  A,2+9,$34,2", ILLEGAL_BIT_NUMBER, ",2+9,$34,2", 0x11, 0x30, 0x53, 0x34);
        ERRT("BAND  A,1,$34,10",  ILLEGAL_BIT_NUMBER, ",10",        0x11, 0x30, 0x51, 0x34);
        TEST("BIAND A.1,$34.2",   0x11, 0x31, 0x51, 0x34);
        TEST("BOR   A.1,$34.2",   0x11, 0x32, 0x51, 0x34);
        TEST("BIAND A.1,$34,2",   0x11, 0x31, 0x51, 0x34);
        ERRT("BIEOR A.1,$1234.2", OPERAND_NOT_ALLOWED, "$1234.2");
        // ',' can be accepted as bit number separator.
        TEST("BIOR  A,1,$34,2",   0x11, 0x33, 0x51, 0x34);
        TEST("BEOR  A,1,$34,2",   0x11, 0x34, 0x51, 0x34);
        TEST("SETDP $12");
        TEST("STBT  A.1,<$5634.2", 0x11, 0x37, 0x51, 0x34);
        TEST("LDBT  A,1,$1234,2",  0x11, 0x36, 0x51, 0x34);
        TEST("STBT  B,0,<$5634.2", 0x11, 0x37, 0x90, 0x34);

        TEST("SETDP 0");
        TEST("LDBT CC.1,$34.7", 0x11, 0x36, 0x39, 0x34);
        TEST("LDBT CC.2,$34.7", 0x11, 0x36, 0x3A, 0x34);
        // ',' can be accepted as bit number separator.
        TEST("LDBT CC,0,$34.7", 0x11, 0x36, 0x38, 0x34);
        TEST("LDBT CC,3,$34.7", 0x11, 0x36, 0x3B, 0x34);
        TEST("LDBT CC,4,$34,7", 0x11, 0x36, 0x3C, 0x34);
        ERRT("LDBT CC,4,$1234,7", OPERAND_NOT_ALLOWED, "$1234,7");
        TEST("SETDP $12");
        TEST("LDBT CC.5,$1234.7",  0x11, 0x36, 0x3D, 0x34);
        TEST("LDBT CC,6,$1234,7",  0x11, 0x36, 0x3E, 0x34);
        TEST("LDBT CC.7,<$5634.7", 0x11, 0x36, 0x3F, 0x34);
        TEST("LDBT A,7,$1234,0",   0x11, 0x36, 0x47, 0x34);
        TEST("LDBT B,2,<$5634,4",  0x11, 0x36, 0xA2, 0x34);

        symtab.intern(0x0034, "dir34");
        symtab.intern(0x9030, "sym9030");
        symtab.intern(4, "bit");

        TEST("SETDP 0");
        TEST("LDBT  CC.0, dir34.7",   0x11, 0x36, 0x38, 0x34);
        TEST("LDBT  CC,0, dir34,7",   0x11, 0x36, 0x38, 0x34);
        TEST("LDBT  B.2, <sym9030.4", 0x11, 0x36, 0xA2, 0x30);
        TEST("LDBT  B,2, <sym9030,4", 0x11, 0x36, 0xA2, 0x30);
        TEST("SETDP $90");
        TEST("LDBT  B.6-bit, sym9030.2*bit/2", 0x11, 0x36, 0xA2, 0x30);
        TEST("LDBT  B,6-bit, sym9030,2*bit/2", 0x11, 0x36, 0xA2, 0x30);

        TEST("LDBT  B.6-bit, 2+sym9030.bit",   0x11, 0x36, 0xA2, 0x32);
        TEST("LDBT  B.6-bit, sym9030+2.bit",   0x11, 0x36, 0xA2, 0x32);
        TEST("LDBT  B.bit-1, sym9030-2.bit/2", 0x11, 0x36, 0x93, 0x2E);
        TEST("LDBT  B.bit-1,-2+sym9030.bit/2", 0x11, 0x36, 0x93, 0x2E);
    } else {
        ERUI("BAND  A.1,$34.2");
        ERUI("BIAND A.1,$34.2");
        ERUI("BOR   A.1,$34.2");
        ERUI("BIOR  A,bit,$34,bit2");
        ERUI("BEOR  A,1,undef,2");
        ERUI("LDBT  A,1,$34,2");
        ERUI("STBT  A.1,$34.2");
    }
}

static void test_comment() {
    TEST("NOP      ; comment", 0x12);
    TEST("PSHS A   ; comment", 0x34, 0x02);
    TEST("PSHS CC , A , B , DP , X , Y , U , PC ; comment", 0x34, 0xFF);
    TEST("TFR  D , X   ; comment", 0x1F, 0x01);
    TEST("SUBB # $90   ; comment", 0xC0, 0x90);
    TEST("NEG  $10     ; comment", 0x00, 0x10);
    ATEST(0x1000, "BRA $1002           ; comment", 0x20, 0x00);
    ATEST(0x1000, "LDA [ $1004 , PCR ] ; comment", 0xA6, 0x9C, 0x01);

    TEST("LDA ,S      ; comment", 0xA6, 0xE4);
    TEST("LDA <<0 , S ; comment", 0xA6, 0x60);
    TEST("LDA <0 , X  ; comment", 0xA6, 0x88, 0x00);
    TEST("LDA >0 , X  ; comment", 0xA6, 0x89, 0x00, 0x00);
    TEST("LDA D , X   ; comment", 0xA6, 0x8B);
    TEST("LDA - 1 , S ; comment", 0xA6, 0x7F);
    TEST("LDA 15 , S  ; comment", 0xA6, 0x6F);
    TEST("LDA [ ,S ]          ; comment", 0xA6, 0xF4);
    TEST("LDA [ 0 , S ]       ; comment", 0xA6, 0xF4);
    TEST("LDA [ <0 , X ]      ; comment", 0xA6, 0x98, 0x00);
    TEST("LDA [ >0 , X ]      ; comment", 0xA6, 0x99, 0x00, 0x00);
    TEST("LDA [ -32768 , PC ] ; comment", 0xA6, 0x9D, 0x80, 0x00);
    TEST("LDA [ D , X ]       ; comment", 0xA6, 0x9B);
    ATEST(0x1000, "LDA $0F83 , PCR     ; comment", 0xA6, 0x8C, 0x80);
    ATEST(0x1000, "LDA [ $0F83 , PCR ] ; comment", 0xa6, 0x9C, 0x80);

    if (is6309()) {
        // HD6309
        TEST("TFM S+ , D+  ; comment", 0x11, 0x38, 0x40);
        TEST("TFM X- , Y-  ; comment", 0x11, 0x39, 0x12);
        TEST("TFM X+ , Y   ; comment", 0x11, 0x3A, 0x12);
        TEST("TFM X , Y+   ; comment", 0x11, 0x3B, 0x12);
        TEST("BOR A.1   , $34.2   ; comment", 0x11, 0x32, 0x51, 0x34);
        TEST("BOR A , 1 , $34 , 2 ; comment", 0x11, 0x32, 0x51, 0x34);
    }
}

static void test_error() {
    ERRT("LDA , S",       UNKNOWN_OPERAND, ", S");
    ERRT("LDA , X+",      UNKNOWN_OPERAND, ", X+");
    ERRT("LDA , X++",     UNKNOWN_OPERAND, ", X++");
    ERRT("LDA , -X",      UNKNOWN_OPERAND, ", -X");
    ERRT("LDA , --X",     UNKNOWN_OPERAND, ", --X");
    ERRT("LDA [ , X++ ]", UNKNOWN_OPERAND, "[ , X++ ]");
    ERRT("LDA [ , --X ]", UNKNOWN_OPERAND, "[ , --X ]");
    ERRT("LDA ,X +",  GARBAGE_AT_END, "+");
    ERRT("LDA ,X ++", GARBAGE_AT_END, "++");
    ERRT("LDA ,X+ +", GARBAGE_AT_END, "+");
    ERRT("LDA ,- X",  UNKNOWN_OPERAND, ",- X");
    ERRT("LDA ,-- X", UNKNOWN_OPERAND, ",-- X");
    ERRT("LDA ,- -X", UNKNOWN_OPERAND, ",- -X");
    ERRT("LDA ,X]",   GARBAGE_AT_END, "]");
    ERRT("LDA [,X ; comment", MISSING_CLOSING_BRACKET, "[,X ; comment");

    if (is6309()) {
        // HD6309
        ERRT("LDA ,W+",   UNKNOWN_OPERAND, ",W+", 0xA6, 0x00);
        ERRT("LDA ,-W",   UNKNOWN_OPERAND, ",-W", 0xA6, 0x00);
        ERRT("LDA , W++", UNKNOWN_OPERAND, ", W++");
        ERRT("LDA , --W", UNKNOWN_OPERAND, ", --W");
        ERRT("LDA [ , W++ ]", UNKNOWN_OPERAND, "[ , W++ ]");
        ERRT("LDA [ , --W ]", UNKNOWN_OPERAND, "[ , --W ]");

        ERRT("TFM D+,W+",    REGISTER_NOT_ALLOWED, "W+", 0x11, 0x38, 0x06);
        ERRT("TFM D + , X+", UNKNOWN_OPERAND, "D + , X+");
        ERRT("TFM D+ , X +", UNKNOWN_OPERAND, "X +");
        ERRT("TFM X - , Y-", UNKNOWN_OPERAND, "X - , Y-");
        ERRT("TFM X- , Y -", UNKNOWN_OPERAND, "Y -");
        ERRT("BOR D.1, $34.2",      ILLEGAL_REGISTER, "D.1, $34.2");
        ERRT("BOR $3456, $34.2",    UNKNOWN_OPERAND, "$3456, $34.2");
        ERRT("BOR A,X, $34.2",      UNKNOWN_OPERAND, "A,X, $34.2");
        ERRT("BOR A.X, $34.2",      UNKNOWN_OPERAND, "A.X, $34.2");
        ERRT("BOR [A.1], $34.2",    UNKNOWN_OPERAND, "[A.1], $34.2");
        ERRT("BOR A.1, $34.Y",      UNKNOWN_OPERAND, "$34.Y");
        ERRT("BOR A.1, $34,Y",      UNKNOWN_OPERAND, "$34,Y");
        ERRT("BOR CC.1, [$34]",     UNKNOWN_OPERAND, "[$34]");
        ERRT("BOR CC.1, $3456",     UNKNOWN_OPERAND, "$3456");
        ERRT("BOR A . 1 , $34 , 2", UNKNOWN_OPERAND, "A . 1 , $34 , 2");
        ERRT("BOR A.1 , $34 ,",     UNKNOWN_OPERAND, "$34 ,");
        ERRT("BOR A.1 , $34. ; comment",   UNKNOWN_OPERAND, "$34. ; comment");
        ERRT("BOR A , 1 , $34 . 2; $34.2", UNKNOWN_OPERAND, "$34 . 2; $34.2");
        TEST("BOR A , 1 , $34 , 2", 0x11, 0x32, 0x51, 0x34);
    }
}

static void test_undefined_symbol() {
    ERUS("LDA  #UNDEF", "UNDEF", 0x86, 0x00);
    ERUS("SUBD #UNDEF", "UNDEF", 0x83, 0x00, 0x00);
    ERUS("NEG   UNDEF", "UNDEF", 0x00, 0x00);
    ERUS("SUBA <UNDEF", "UNDEF", 0x90, 0x00);
    ERUS("SUBB >UNDEF", "UNDEF", 0xF0, 0x00, 0x00);

    ERUS("LDA    UNDEF,Y",  "UNDEF,Y",  0xA6, 0xA9, 0x00, 0x00);
    ERUS("LDA  <<UNDEF,Y",  "UNDEF,Y",  0xA6, 0x20);
    ERUS("LDA   <UNDEF,Y",  "UNDEF,Y",  0xA6, 0xA8, 0x00);
    ERUS("LDA   >UNDEF,Y",  "UNDEF,Y",  0xA6, 0xA9, 0x00, 0x00);
    ERUS("LDA   [UNDEF,Y]", "UNDEF,Y]", 0xA6, 0xB9, 0x00, 0x00);
    ERUS("LDA  [<UNDEF,Y]", "UNDEF,Y]", 0xA6, 0xB8, 0x00);
    ERUS("LDA  [>UNDEF,Y]", "UNDEF,Y]", 0xA6, 0xB9, 0x00, 0x00);
    ERUS("LDA     [UNDEF]", "UNDEF]",   0xA6, 0x9F, 0x00, 0x00);
    ERUS("LDA    [>UNDEF]", "UNDEF]",   0xA6, 0x9F, 0x00, 0x00);

    ERUS("BRA   UNDEF", "UNDEF", 0x20, 0x00);
    ERUS("LBRA  UNDEF", "UNDEF", 0x16, 0x00, 0x00);
    ERUS("LBCC  UNDEF", "UNDEF", 0x10, 0x24, 0x00, 0x00);
    ERUS("LDA   UNDEF,PCR",  "UNDEF,PCR",  0xA6, 0x8D, 0x00, 0x00);
    ERUS("LDA  <UNDEF,PCR",  "UNDEF,PCR",  0xA6, 0x8C, 0x00);
    ERUS("LDA  >UNDEF,PCR",  "UNDEF,PCR",  0xA6, 0x8D, 0x00, 0x00);
    ERUS("LDA  [UNDEF,PCR]", "UNDEF,PCR]", 0xA6, 0x9D, 0x00, 0x00);
    ERUS("LDA [<UNDEF,PCR]", "UNDEF,PCR]", 0xA6, 0x9C, 0x00);
    ERUS("LDA [>UNDEF,PCR]", "UNDEF,PCR]", 0xA6, 0x9D, 0x00, 0x00);

    if (is6309()) {
        // HD6309
        ERUS("BITMD #UNDEF",   "UNDEF",    0x11, 0x3C, 0x00);
        ERUS("LDQ   #UNDEF",   "UNDEF",    0xCD, 0x00, 0x00, 0x00, 0x00);
        ERUS("MULD  #UNDEF",   "UNDEF",    0x11, 0x8F, 0x00, 0x00);
        ERUS("LDA   UNDEF,W",  "UNDEF,W",  0xA6, 0xAF, 0x00, 0x00);
        ERUS("LDA  >UNDEF,W",  "UNDEF,W",  0xA6, 0xAF, 0x00, 0x00);
        ERUS("LDA [UNDEF,W]",  "UNDEF,W]", 0xA6, 0xB0, 0x00, 0x00);
        ERUS("LDA [>UNDEF,W]", "UNDEF,W]", 0xA6, 0xB0, 0x00, 0x00);
        ERUS("OIM #UNDEF,$10",       "UNDEF,$10",       0x01, 0x00, 0x10);
        ERUS("OIM #UNDEF,$1000",     "UNDEF,$1000",     0x71, 0x00, 0x10, 0x00);
        ERUS("OIM #UNDEF,UNDEF,X",   "UNDEF,UNDEF,X",   0x61, 0x00, 0x89, 0x00, 0x00);
        ERUS("OIM #UNDEF,[$1000,X]", "UNDEF,[$1000,X]", 0x61, 0x00, 0x99, 0x10, 0x00);
        ERUS("OIM #$30,UNDEF",       "UNDEF",    0x01, 0x30, 0x00);
        ERUS("OIM #$30,>UNDEF",      "UNDEF",    0x71, 0x30, 0x00, 0x00);
        ERUS("OIM #$30,UNDEF,X",     "UNDEF,X",  0x61, 0x30, 0x89, 0x00, 0x00);
        ERUS("OIM #$30,[UNDEF,X]",   "UNDEF,X]", 0x61, 0x30, 0x99, 0x00, 0x00);
        ERUS("BOR A.UNDEF,$34.2", "UNDEF,$34.2", 0x11, 0x32, 0x50, 0x34);
        ERUS("BOR A.1,UNDEF.2",   "UNDEF.2",     0x11, 0x32, 0x51, 0x00);
        ERUS("BOR A.1,$34.UNDEF", "UNDEF",       0x11, 0x32, 0x41, 0x34);
    }
}

static void test_data_constant() {
    TEST("FCB -128, 255", 0x80, 0xFF);
    TEST("FCB 'A', '\"'", 0x41, 0x22);
    TEST("FCB '9'-'0'",   0x09);
    TEST("FCB '''",       0x27);
    TEST("FCB ''",        0x27);
    ERRT("FCB '",         ILLEGAL_CONSTANT, "'");
    TEST("FCB 'A,'','B",  0x41, 0x27, 0x42);
    TEST("FDB -128, 255", 0xFF, 0x80, 0x00, 0xFF);
    TEST("FDB 'A, '\"",   0x00, 0x41, 0x00, 0x22);
    TEST("FCC 'ABC'",     0x41, 0x42, 0x43);
    TEST("FCC /ABC/",     0x41, 0x42, 0x43);
    ERRT("FCC 'ABC",      MISSING_CLOSING_DELIMITER, "", 0x41, 0x42, 0x43);
    ERRT("FCC /ABC",      MISSING_CLOSING_DELIMITER, "", 0x41, 0x42, 0x43);
}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_inherent);
    RUN_TEST(test_immediate);
    RUN_TEST(test_direct);
    RUN_TEST(test_extended);
    RUN_TEST(test_indexed);
    RUN_TEST(test_indexed_mode);
    RUN_TEST(test_relative);
    RUN_TEST(test_stack);
    RUN_TEST(test_register);
    RUN_TEST(test_transfer);
    RUN_TEST(test_bit_position);
    RUN_TEST(test_comment);
    RUN_TEST(test_error);
    RUN_TEST(test_undefined_symbol);
    RUN_TEST(test_data_constant);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
