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

#include "asm_z80.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::z80;
using namespace libasm::test;

AsmZ80 asz80;
Assembler &assembler(asz80);

bool isZ80() {
    return strcasecmp_P("Z80", assembler.config().cpu_P()) == 0;
}

bool z180() {
    return strcasecmp_P("Z180", assembler.config().cpu_P()) == 0;
}

bool z280() {
    return strcasecmp_P("Z280", assembler.config().cpu_P()) == 0;
}

void set_up() {
    assembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu z80", true, assembler.setCpu("z80"));
    EQUALS_P("get cpu", "Z80", assembler.config().cpu_P());

    EQUALS("cpu z180", true, assembler.setCpu("z180"));
    EQUALS_P("get cpu", "Z180", assembler.config().cpu_P());

    EQUALS("cpu z280", true, assembler.setCpu("z280"));
    EQUALS_P("get cpu", "Z280", assembler.config().cpu_P());
}

void test_load_registers() {
    TEST("LD A,A", 0x7F);
    TEST("LD A,B", 0x78);
    TEST("LD A,C", 0x79);
    TEST("LD A,D", 0x7A);
    TEST("LD A,E", 0x7B);
    TEST("LD A,H", 0x7C);
    TEST("LD A,L", 0x7D);

    TEST("LD B,A", 0x47);
    TEST("LD B,B", 0x40);
    TEST("LD B,C", 0x41);
    TEST("LD B,D", 0x42);
    TEST("LD B,E", 0x43);
    TEST("LD B,H", 0x44);
    TEST("LD B,L", 0x45);

    TEST("LD C,A", 0x4F);
    TEST("LD C,B", 0x48);
    TEST("LD C,C", 0x49);
    TEST("LD C,D", 0x4A);
    TEST("LD C,E", 0x4B);
    TEST("LD C,H", 0x4C);
    TEST("LD C,L", 0x4D);

    TEST("LD D,A", 0x57);
    TEST("LD D,B", 0x50);
    TEST("LD D,C", 0x51);
    TEST("LD D,D", 0x52);
    TEST("LD D,E", 0x53);
    TEST("LD D,H", 0x54);
    TEST("LD D,L", 0x55);

    TEST("LD E,A", 0x5F);
    TEST("LD E,B", 0x58);
    TEST("LD E,C", 0x59);
    TEST("LD E,D", 0x5A);
    TEST("LD E,E", 0x5B);
    TEST("LD E,H", 0x5C);
    TEST("LD E,L", 0x5D);

    TEST("LD H,A", 0x67);
    TEST("LD H,B", 0x60);
    TEST("LD H,C", 0x61);
    TEST("LD H,D", 0x62);
    TEST("LD H,E", 0x63);
    TEST("LD H,H", 0x64);
    TEST("LD H,L", 0x65);

    TEST("LD L,A", 0x6F);
    TEST("LD L,B", 0x68);
    TEST("LD L,C", 0x69);
    TEST("LD L,D", 0x6A);
    TEST("LD L,E", 0x6B);
    TEST("LD L,H", 0x6C);
    TEST("LD L,L", 0x6D);

    if (z280()) {
        TEST("LD IXH, A",   0xDD, 0x67);
        TEST("LD IXH, B",   0xDD, 0x60);
        TEST("LD IXH, C",   0xDD, 0x61);
        TEST("LD IXH, D",   0xDD, 0x62);
        TEST("LD IXH, E",   0xDD, 0x63);
        TEST("LD IXH, IXH", 0xDD, 0x64);
        TEST("LD IXH, IXL", 0xDD, 0x65);
        TEST("LD A, IXH",   0xDD, 0x7C);
        TEST("LD B, IXH",   0xDD, 0x44);
        TEST("LD C, IXH",   0xDD, 0x4C);
        TEST("LD D, IXH",   0xDD, 0x54);
        TEST("LD E, IXH",   0xDD, 0x5C);
        TEST("LD IXH, IXH", 0xDD, 0x64);
        TEST("LD IXL, IXH", 0xDD, 0x6C);

        TEST("LD IXL, A",   0xDD, 0x6F);
        TEST("LD IXL, B",   0xDD, 0x68);
        TEST("LD IXL, C",   0xDD, 0x69);
        TEST("LD IXL, D",   0xDD, 0x6A);
        TEST("LD IXL, E",   0xDD, 0x6B);
        TEST("LD IXL, IXH", 0xDD, 0x6C);
        TEST("LD IXL, IXL", 0xDD, 0x6D);
        TEST("LD A, IXL",   0xDD, 0x7D);
        TEST("LD B, IXL",   0xDD, 0x45);
        TEST("LD C, IXL",   0xDD, 0x4D);
        TEST("LD D, IXL",   0xDD, 0x55);
        TEST("LD E, IXL",   0xDD, 0x5D);
        TEST("LD IXH, IXL", 0xDD, 0x65);
        TEST("LD IXL, IXL", 0xDD, 0x6D);

        TEST("LD IYH, A",   0xFD, 0x67);
        TEST("LD IYH, B",   0xFD, 0x60);
        TEST("LD IYH, C",   0xFD, 0x61);
        TEST("LD IYH, D",   0xFD, 0x62);
        TEST("LD IYH, E",   0xFD, 0x63);
        TEST("LD IYH, IYH", 0xFD, 0x64);
        TEST("LD IYH, IYL", 0xFD, 0x65);
        TEST("LD A, IYH",   0xFD, 0x7C);
        TEST("LD B, IYH",   0xFD, 0x44);
        TEST("LD C, IYH",   0xFD, 0x4C);
        TEST("LD D, IYH",   0xFD, 0x54);
        TEST("LD E, IYH",   0xFD, 0x5C);
        TEST("LD IYH, IYH", 0xFD, 0x64);
        TEST("LD IYL, IYH", 0xFD, 0x6C);

        TEST("LD IYL, A",   0xFD, 0x6F);
        TEST("LD IYL, B",   0xFD, 0x68);
        TEST("LD IYL, C",   0xFD, 0x69);
        TEST("LD IYL, D",   0xFD, 0x6A);
        TEST("LD IYL, E",   0xFD, 0x6B);
        TEST("LD IYL, IYH", 0xFD, 0x6C);
        TEST("LD IYL, IYL", 0xFD, 0x6D);
        TEST("LD A, IYL",   0xFD, 0x7D);
        TEST("LD B, IYL",   0xFD, 0x45);
        TEST("LD C, IYL",   0xFD, 0x4D);
        TEST("LD D, IYL",   0xFD, 0x55);
        TEST("LD E, IYL",   0xFD, 0x5D);
        TEST("LD IYH, IYL", 0xFD, 0x65);
        TEST("LD IYL, IYL", 0xFD, 0x6D);
    }

    TEST("LD I,A", 0xED, 0x47);
    TEST("LD R,A", 0xED, 0x4F);
    TEST("LD A,I", 0xED, 0x57);
    TEST("LD A,R", 0xED, 0x5F);

    TEST("LD SP,HL", 0xF9);
    TEST("LD SP,IX", 0xDD, 0xF9);
    TEST("LD SP,IY", 0xFD, 0xF9);
}

void test_load_immediate() {
    TEST("LD B,0F6H", 0x06, 0xF6);
    TEST("LD C,9FH",  0x0E, 0x9F);
    TEST("LD D,3AH",  0x16, 0x3A);
    TEST("LD E,80H",  0x1E, 0x80);
    TEST("LD H,0F6H", 0x26, 0xF6);
    TEST("LD L,0F6H", 0x2E, 0xF6);

    TEST("LD A,0FEH", 0x3E, 0xFE);
    TEST("LD A,-1",   0x3E, 0xFF);
    TEST("LD A,255",  0x3E, 0xFF);
    ERRT("LD A,256",  OVERFLOW_RANGE, "256",  0x3E, 0x00);
    TEST("LD A,-128", 0x3E, 0x80);
    ERRT("LD A,-129", OVERFLOW_RANGE, "-129", 0x3E, 0x7F);

    TEST("LD BC,0BEEFH", 0x01, 0xEF, 0xBE);
    TEST("LD DE,1234H",  0x11, 0x34, 0x12);
    TEST("LD HL,0BEEFH", 0x21, 0xEF, 0xBE);
    TEST("LD SP,6789H",  0x31, 0x89, 0x67);
    TEST("LD IX,0ABCDH", 0xDD, 0x21, 0xCD, 0xAB);
    TEST("LD IY,0ABCDH", 0xFD, 0x21, 0xCD, 0xAB);

    symtab.intern(-1,  "minus_1");
    symtab.intern(255, "max?255");

    TEST("LD B, minus_1", 0x06, 0xFF);
    TEST("LD C, max?255", 0x0E, 0xFF);
}

void test_load() {
    TEST("LD A,(HL)", 0x7E);
    TEST("ld b,(hl)", 0x46);
    TEST("LD C,(HL)", 0x4E);
    TEST("LD D,(HL)", 0x56);
    TEST("LD E,(HL)", 0x5E);
    TEST("LD H,(HL)", 0x66);
    TEST("LD L,(HL)", 0x6E);

    TEST("LD A,(BC)", 0x0A);
    TEST("LD A,(DE)", 0x1A);
    TEST("LD A,(1234H)", 0x3A, 0x34, 0x12);

    TEST("LD A,(IX+2)", 0xDD, 0x7E, 0x02);
    TEST("LD B,(IX+2)", 0xDD, 0x46, 0x02);
    TEST("LD C,(IX+2)", 0xDD, 0x4E, 0x02);
    TEST("LD D,(IX+2)", 0xDD, 0x56, 0x02);
    TEST("LD E,(IX+2)", 0xDD, 0x5E, 0x02);
    TEST("LD H,(IX+2)", 0xDD, 0x66, 0x02);
    TEST("LD L,(IX+2)", 0xDD, 0x6E, 0x02);
    TEST("LD A,(IY-2)", 0xFD, 0x7E, 0xFE);
    TEST("LD B,(IY-2)", 0xFD, 0x46, 0xFE);
    TEST("LD C,(IY-2)", 0xFD, 0x4E, 0xFE);
    TEST("LD D,(IY-2)", 0xFD, 0x56, 0xFE);
    TEST("LD E,(IY-2)", 0xFD, 0x5E, 0xFE);
    TEST("LD H,(IY-2)", 0xFD, 0x66, 0xFE);
    TEST("LD L,(IY-2)", 0xFD, 0x6E, 0xFE);
    ERRT("LD (HL),(IX+2)", OPERAND_NOT_ALLOWED, "(HL),(IX+2)");

    TEST("LD BC,(05678H)", 0xED, 0x4B, 0x78, 0x56);
    TEST("LD DE,(05678H)", 0xED, 0x5B, 0x78, 0x56);
    TEST("LD HL,(5678H)",  0x2A, 0x78, 0x56);
    TEST("LD SP,(05678H)", 0xED, 0x7B, 0x78, 0x56);
    TEST("LD IX,(5678H)",  0xDD, 0x2A, 0x78, 0x56);
    TEST("LD IY,(5678H)",  0xFD, 0x2A, 0x78, 0x56);
    if (z280()) {
        TEST("LDW BC,(05678H)", 0xED, 0x4B, 0x78, 0x56);
        TEST("LDW DE,(05678H)", 0xED, 0x5B, 0x78, 0x56);
        TEST("LDW SP,(05678H)", 0xED, 0x7B, 0x78, 0x56);
    }

    if (z280()) {
        TEST("LDW BC, (HL)", 0xED, 0x06);
        TEST("LDW DE, (HL)", 0xED, 0x16);
        TEST("LDW HL, (HL)", 0xED, 0x26);
        TEST("LDW SP, (HL)", 0xED, 0x36);
        TEST("LD  BC, (HL)", 0xED, 0x06);
        TEST("LD  DE, (HL)", 0xED, 0x16);
        TEST("LD  HL, (HL)", 0xED, 0x26);
        TEST("LD  SP, (HL)", 0xED, 0x36);
    }
    if (z280()) {
        TEST("LDA HL, (SP-1234H)", 0xED, 0x02, 0xCC, 0xED);
        TEST("LDA HL, (HL+IX)",    0xED, 0x0A);
        TEST("LDA HL, (HL+IY)",    0xED, 0x12);
        TEST("LDA HL, (IX+IY)",    0xED, 0x1A);
        ATEST(0x1000, "LDA HL, <1234H>", 0xED, 0x22, 0x30, 0x02);
        TEST("LDA HL, (IX+1234H)", 0xED, 0x2A, 0x34, 0x12);
        TEST("LDA HL, (IY-1234H)", 0xED, 0x32, 0xCC, 0xED);
        TEST("LDA HL, (HL+1234H)", 0xED, 0x3A, 0x34, 0x12);

        TEST("LDW HL, (SP-1234H)", 0xED, 0x04, 0xCC, 0xED);
        TEST("LDW HL, (HL+IX)",    0xED, 0x0C);
        TEST("LDW HL, (HL+IY)",    0xED, 0x14);
        TEST("LDW HL, (IX+IY)",    0xED, 0x1C);
        ATEST(0x1000, "LDW HL, <1234H>", 0xED, 0x24, 0x30, 0x02);
        TEST("LDW HL, (IX+1234H)", 0xED, 0x2C, 0x34, 0x12);
        TEST("LDW HL, (IY-1234H)", 0xED, 0x34, 0xCC, 0xED);
        TEST("LDW HL, (HL+1234H)", 0xED, 0x3C, 0x34, 0x12);

        TEST("LDW IX, (SP-1234H)", 0xDD, 0xED, 0x04, 0xCC, 0xED);
        TEST("LDW IX, (HL+IX)",    0xDD, 0xED, 0x0C);
        TEST("LDW IX, (HL+IY)",    0xDD, 0xED, 0x14);
        TEST("LDW IX, (IX+IY)",    0xDD, 0xED, 0x1C);
        ATEST(0x1000, "LDW IX, <1234H>", 0xDD, 0xED, 0x24, 0x2F, 0x02);
        TEST("LDW IX, (IX+1234H)", 0xDD, 0xED, 0x2C, 0x34, 0x12);
        TEST("LDW IX, (IY-1234H)", 0xDD, 0xED, 0x34, 0xCC, 0xED);
        TEST("LDW IX, (HL+1234H)", 0xDD, 0xED, 0x3C, 0x34, 0x12);

        TEST("LDW IY, (SP-1234H)", 0xFD, 0xED, 0x04, 0xCC, 0xED);
        TEST("LDW IY, (HL+IX)",    0xFD, 0xED, 0x0C);
        TEST("LDW IY, (HL+IY)",    0xFD, 0xED, 0x14);
        TEST("LDW IY, (IX+IY)",    0xFD, 0xED, 0x1C);
        ATEST(0x1000, "LDW IY, <1234H>", 0xFD, 0xED, 0x24, 0x2F, 0x02);
        TEST("LDW IY, (IX+1234H)", 0xFD, 0xED, 0x2C, 0x34, 0x12);
        TEST("LDW IY, (IY-1234H)", 0xFD, 0xED, 0x34, 0xCC, 0xED);
        TEST("LDW IY, (HL+1234H)", 0xFD, 0xED, 0x3C, 0x34, 0x12);
    }
}

void test_store() {
    TEST("LD (HL),A", 0x77);
    TEST("LD (HL),B", 0x70);
    TEST("LD (HL),C", 0x71);
    TEST("LD (HL),D", 0x72);
    TEST("LD (HL),E", 0x73);
    TEST("LD (HL),H", 0x74);
    TEST("LD (HL),L", 0x75);

    TEST("LD (BC),A", 0x02);
    TEST("LD (DE),A", 0x12);
    TEST("LD (9ABCH),A", 0x32, 0xBC, 0x9A);
    if (z280()) {
        TEST("LD (SP-1234H), A", 0xED, 0x03, 0xCC, 0xED);
        TEST("LD (HL+IX), A",    0xED, 0x0B);
        TEST("LD (HL+IY), A",    0xED, 0x13);
        TEST("LD (IX+IY), A",    0xED, 0x1B);
        ATEST(0x1000, "LD <1234H>, A", 0xED, 0x23, 0x30, 0x02);
        TEST("LD (IX+1234H), A", 0xED, 0x2B, 0x34, 0x12);
        TEST("LD (IY-1234H), A", 0xED, 0x33, 0xCC, 0xED);
        TEST("LD (HL+1234H), A", 0xED, 0x3B, 0x34, 0x12);
    }

    TEST("LD (IX+2),A", 0xDD, 0x77, 0x02);
    TEST("LD (IX+2),B", 0xDD, 0x70, 0x02);
    TEST("LD (IX+2),C", 0xDD, 0x71, 0x02);
    TEST("LD (IX+2),D", 0xDD, 0x72, 0x02);
    TEST("LD (IX+0),E", 0xDD, 0x73, 0x00);
    TEST("LD (IX+2),H", 0xDD, 0x74, 0x02);
    TEST("LD (IX+2),L", 0xDD, 0x75, 0x02);
    TEST("LD (IY-2),A", 0xFD, 0x77, 0xFE);
    TEST("LD (IY-2),B", 0xFD, 0x70, 0xFE);
    TEST("LD (IY-2),C", 0xFD, 0x71, 0xFE);
    TEST("LD (IY-2),D", 0xFD, 0x72, 0xFE);
    TEST("LD (IY-2),E", 0xFD, 0x73, 0xFE);
    TEST("LD (IY-2),H", 0xFD, 0x74, 0xFE);
    TEST("LD (IY-2),L", 0xFD, 0x75, 0xFE);
    ERRT("LD (IX+2),(HL)", OPERAND_NOT_ALLOWED, "(IX+2),(HL)");

    TEST("LD (HL),0F6H",   0x36, 0xF6);
    TEST("LD (IX+2),0F6H", 0xDD, 0x36, 0x02, 0xF6);
    TEST("LD (IY-2),0F6H", 0xFD, 0x36, 0xFE, 0xF6);
    if (z280()) {
        TEST("LD (IX+128),0F6H", 0xFD, 0x0E, 0x80, 0x00, 0xF6);
        TEST("LD (IY-129),0F6H", 0xFD, 0x16, 0x80, 0x00, 0xF6);
    } else {
        ERRT("LD (IX+128),0F6H", OVERFLOW_RANGE, "(IX+128),0F6H", 0xDD, 0x36, 0x80, 0xF6);
        ERRT("LD (IY-129),0F6H", OVERFLOW_RANGE, "(IY-129),0F6H", 0xFD, 0x36, 0x7F, 0xF6);
    }

    if (z280()) {
        TEST("LDW (HL), BC", 0xED, 0x0E);
        TEST("LDW (HL), DE", 0xED, 0x1E);
        TEST("LDW (HL), HL", 0xED, 0x2E);
        TEST("LDW (HL), SP", 0xED, 0x3E);
        TEST("LD  (HL), BC", 0xED, 0x0E);
        TEST("LD  (HL), DE", 0xED, 0x1E);
        TEST("LD  (HL), HL", 0xED, 0x2E);
        TEST("LD  (HL), SP", 0xED, 0x3E);
    }

    TEST("LD (0ABCDH),BC", 0xED, 0x43, 0xCD, 0xAB);
    TEST("LD (0ABCDH),DE", 0xED, 0x53, 0xCD, 0xAB);
    TEST("LD (0ABCDH),HL", 0x22, 0xCD, 0xAB);
    TEST("LD (0ABCDH),SP", 0xED, 0x73, 0xCD, 0xAB);
    TEST("LD (0ABCDH),IX", 0xDD, 0x22, 0xCD, 0xAB);
    TEST("LD (0ABCDH),IY", 0xFD, 0x22, 0xCD, 0xAB);
    if (z280()) {
        TEST("LDW (0ABCDH),BC", 0xED, 0x43, 0xCD, 0xAB);
        TEST("LDW (0ABCDH),DE", 0xED, 0x53, 0xCD, 0xAB);
        TEST("LDW (0ABCDH),SP", 0xED, 0x73, 0xCD, 0xAB);
    }
    if (z280()) {
        TEST("LDW (SP-1234H), HL", 0xED, 0x05, 0xCC, 0xED);
        TEST("LDW (HL+IX), HL",    0xED, 0x0D);
        TEST("LDW (HL+IY), HL",    0xED, 0x15);
        TEST("LDW (IX+IY), HL",    0xED, 0x1D);
        ATEST(0x1000, "LDW <1234H>, HL", 0xED, 0x25, 0x30, 0x02);
        TEST("LDW (IX+1234H), HL", 0xED, 0x2D, 0x34, 0x12);
        TEST("LDW (IY-1234H), HL", 0xED, 0x35, 0xCC, 0xED);
        TEST("LDW (HL+1234H), HL", 0xED, 0x3D, 0x34, 0x12);

        TEST("LDW (SP-1234H), IX", 0xDD, 0xED, 0x05, 0xCC, 0xED);
        TEST("LDW (HL+IX), IX",    0xDD, 0xED, 0x0D);
        TEST("LDW (HL+IY), IX",    0xDD, 0xED, 0x15);
        TEST("LDW (IX+IY), IX",    0xDD, 0xED, 0x1D);
        ATEST(0x1000, "LDW <1234H>, IX", 0xDD, 0xED, 0x25, 0x2F, 0x02);
        TEST("LDW (IX+1234H), IX", 0xDD, 0xED, 0x2D, 0x34, 0x12);
        TEST("LDW (IY-1234H), IX", 0xDD, 0xED, 0x35, 0xCC, 0xED);
        TEST("LDW (HL+1234H), IX", 0xDD, 0xED, 0x3D, 0x34, 0x12);

        TEST("LDW (SP-1234H), IY", 0xFD, 0xED, 0x05, 0xCC, 0xED);
        TEST("LDW (HL+IX), IY",    0xFD, 0xED, 0x0D);
        TEST("LDW (HL+IY), IY",    0xFD, 0xED, 0x15);
        TEST("LDW (IX+IY), IY",    0xFD, 0xED, 0x1D);
        ATEST(0x1000, "LDW <1234H>, IY", 0xFD, 0xED, 0x25, 0x2F, 0x02);
        TEST("LDW (IX+1234H), IY", 0xFD, 0xED, 0x2D, 0x34, 0x12);
        TEST("LDW (IY-1234H), IY", 0xFD, 0xED, 0x35, 0xCC, 0xED);
        TEST("LDW (HL+1234H), IY", 0xFD, 0xED, 0x3D, 0x34, 0x12);
    }
}

void test_exchange() {
    TEST("EX HL,DE",  0xEB);
    TEST("ex de,hl",  0xEB);
    TEST("EX AF,AF'", 0x08);
    TEST("EXX",       0xD9);

    TEST("EX (SP),HL", 0xE3);
    TEST("EX HL,(SP)", 0xE3);
    TEST("EX (SP),IX", 0xDD, 0xE3);
    TEST("EX IX,(SP)", 0xDD, 0xE3);
    TEST("EX (SP),IY", 0xFD, 0xE3);
    TEST("EX IY,(SP)", 0xFD, 0xE3);

    if (z280()) {
        TEST("EX A, A", 0xED, 0x3F);
        TEST("EX A, B", 0xED, 0x07);
        TEST("EX A, C", 0xED, 0x0F);
        TEST("EX A, D", 0xED, 0x17);
        TEST("EX A, E", 0xED, 0x1F);
        TEST("EX A, H", 0xED, 0x27);
        TEST("EX A, L", 0xED, 0x2F);
        TEST("EX A, (HL)", 0xED, 0x37);
        TEST("EX H, L", 0xED, 0xEF);
    } else {
        ERRT("EX A, A", OPERAND_NOT_ALLOWED, "A, A");
        ERRT("EX A, B", OPERAND_NOT_ALLOWED, "A, B");
        ERRT("EX A, C", OPERAND_NOT_ALLOWED, "A, C");
        ERRT("EX A, D", OPERAND_NOT_ALLOWED, "A, D");
        ERRT("EX A, E", OPERAND_NOT_ALLOWED, "A, E");
        ERRT("EX A, H", OPERAND_NOT_ALLOWED, "A, H");
        ERRT("EX A, L", OPERAND_NOT_ALLOWED, "A, L");
        ERRT("EX A, (HL)", OPERAND_NOT_ALLOWED, "A, (HL)");
        ERRT("EX H, L", OPERAND_NOT_ALLOWED, "H, L");
    }
}

void test_stack() {
    TEST("PUSH BC", 0xC5);
    TEST("PUSH DE", 0xD5);
    TEST("PUSH HL", 0xE5);
    TEST("PUSH AF", 0xF5);
    TEST("PUSH IX", 0xDD, 0xE5);
    TEST("PUSH IY", 0xFD, 0xE5);

    TEST("POP BC", 0xC1);
    TEST("POP DE", 0xD1);
    TEST("POP HL", 0xE1);
    TEST("POP AF", 0xF1);
    TEST("POP IX", 0xDD, 0xE1);
    TEST("POP IY", 0xFD, 0xE1);

    TEST("RET",    0xC9);
    TEST("RET NZ", 0xC0);
    TEST("RET Z",  0xC8);
    TEST("RET NC", 0xD0);
    TEST("RET C",  0xD8);
    TEST("RET PO", 0xE0);
    TEST("RET PE", 0xE8);
    TEST("RET P",  0xF0);
    TEST("RET M",  0xF8);

    TEST("RETN", 0xED, 0x45);
    TEST("RETI", 0xED, 0x4D);
    if (z280()) {
        TEST("RETIL", 0xED, 0x55);
    } else {
        ERUI("RETIL");
    }
}

void test_jump() {
    TEST("JP 1234H",    0xC3, 0x34, 0x12);
    TEST("jp nz,1234h", 0xC2, 0x34, 0x12);
    TEST("JP Z,1234H",  0xCA, 0x34, 0x12);
    TEST("JP NC,1234H", 0xD2, 0x34, 0x12);
    TEST("JP C,1234H",  0xDA, 0x34, 0x12);
    TEST("JP PO,1234H", 0xE2, 0x34, 0x12);
    TEST("JP PE,1234H", 0xEA, 0x34, 0x12);
    TEST("JP P,1234H",  0xF2, 0x34, 0x12);
    TEST("JP M,1234H",  0xFA, 0x34, 0x12);
    if (z280()) {
        TEST("JP NV,1234H", 0xE2, 0x34, 0x12);
        TEST("JP  V,1234H", 0xEA, 0x34, 0x12);
        TEST("JP NS,1234H", 0xF2, 0x34, 0x12);
        TEST("JP  S,1234H", 0xFA, 0x34, 0x12);
    }

    TEST("JP (HL)", 0xE9);
    TEST("JP (IX)", 0xDD, 0xE9);
    TEST("JP (IY)", 0xFD, 0xE9);
    if (z280()) {
        TEST("JP NZ,(HL)", 0xDD, 0xC2);
        TEST("JP  Z,(HL)", 0xDD, 0xCA);
        TEST("JP NC,(HL)", 0xDD, 0xD2);
        TEST("JP  C,(HL)", 0xDD, 0xDA);
        TEST("JP PO,(HL)", 0xDD, 0xE2);
        TEST("JP PE,(HL)", 0xDD, 0xEA);
        TEST("JP  P,(HL)", 0xDD, 0xF2);
        TEST("JP  M,(HL)", 0xDD, 0xFA);
    }

    TEST("CALL 1234H",    0xCD, 0x34, 0x12);
    TEST("CALL NZ,1234H", 0xC4, 0x34, 0x12);
    TEST("CALL Z,1234H",  0xCC, 0x34, 0x12);
    TEST("CALL NC,1234H", 0xD4, 0x34, 0x12);
    TEST("CALL C,1234H",  0xDC, 0x34, 0x12);
    TEST("CALL PO,1234H", 0xE4, 0x34, 0x12);
    TEST("CALL PE,1234H", 0xEC, 0x34, 0x12);
    TEST("CALL P,1234H",  0xF4, 0x34, 0x12);
    TEST("CALL M,1234H",  0xFC, 0x34, 0x12);

    if (z280()) {
        TEST("CALL NV,1234H", 0xE4, 0x34, 0x12);
        TEST("CALL  V,1234H", 0xEC, 0x34, 0x12);
        TEST("CALL NS,1234H", 0xF4, 0x34, 0x12);
        TEST("CALL  S,1234H", 0xFC, 0x34, 0x12);

        TEST("CALL    (HL)", 0xDD, 0xCD);
        TEST("CALL NZ,(HL)", 0xDD, 0xC4);
        TEST("CALL  Z,(HL)", 0xDD, 0xCC);
        TEST("CALL NC,(HL)", 0xDD, 0xD4);
        TEST("CALL  C,(HL)", 0xDD, 0xDC);
        TEST("CALL PO,(HL)", 0xDD, 0xE4);
        TEST("CALL PE,(HL)", 0xDD, 0xEC);
        TEST("CALL  P,(HL)", 0xDD, 0xF4);
        TEST("CALL  M,(HL)", 0xDD, 0xFC);
    }

    ATEST(0x1000, "JR 1000H",    0x18, 0xFE);
    ATEST(0x1000, "JR NZ,1004H", 0x20, 0x02);
    ATEST(0x1000, "JR Z,1081H",  0x28, 0x7F);
    ATEST(0x1000, "JR NC,0F82H", 0x30, 0x80);
    ATEST(0x1000, "JR C,0F82H",  0x38, 0x80);
    ATEST(0x1000, "JR C,1081H",  0x38, 0x7F);
    ATEST(0x1000, "JR C,0F82H",  0x38, 0x80);
    AERRT(0x1000, "JR C,1082H",  OPERAND_TOO_FAR, "1082H", 0x38, 0x80);
    AERRT(0x1000, "JR C,0F81H",  OPERAND_TOO_FAR, "0F81H", 0x38, 0x7F);
    ATEST(0x1000, "JR $+80H",    0x18, 0x7E);
    if (z280()) {
        ATEST(0x1000, "JP    <1234H>", 0xFD, 0xC3, 0x30, 0x02);
        ATEST(0x1000, "JP NZ,<1234H>", 0xFD, 0xC2, 0x30, 0x02);
        ATEST(0x1000, "JP  Z,<1234H>", 0xFD, 0xCA, 0x30, 0x02);
        ATEST(0x1000, "JP NC,<1234H>", 0xFD, 0xD2, 0x30, 0x02);
        ATEST(0x1000, "JP  C,<1234H>", 0xFD, 0xDA, 0x30, 0x02);
        ATEST(0x1000, "JP PO,<1234H>", 0xFD, 0xE2, 0x30, 0x02);
        ATEST(0x1000, "JP PE,<1234H>", 0xFD, 0xEA, 0x30, 0x02);
        ATEST(0x1000, "JP  P,<1234H>", 0xFD, 0xF2, 0x30, 0x02);
        ATEST(0x1000, "JP  M,<1234H>", 0xFD, 0xFA, 0x30, 0x02);
        ATEST(0x1000, "JP NV,<1234H>", 0xFD, 0xE2, 0x30, 0x02);
        ATEST(0x1000, "JP  V,<1234H>", 0xFD, 0xEA, 0x30, 0x02);
        ATEST(0x1000, "JP NS,<1234H>", 0xFD, 0xF2, 0x30, 0x02);
        ATEST(0x1000, "JP  S,<1234H>", 0xFD, 0xFA, 0x30, 0x02);
    }

    if (z280()) {
        ATEST(0x1000, "CALL    <1234H>", 0xFD, 0xCD, 0x30, 0x02);
        ATEST(0x1000, "CALL NZ,<1234H>", 0xFD, 0xC4, 0x30, 0x02);
        ATEST(0x1000, "CALL  Z,<1234H>", 0xFD, 0xCC, 0x30, 0x02);
        ATEST(0x1000, "CALL NC,<1234H>", 0xFD, 0xD4, 0x30, 0x02);
        ATEST(0x1000, "CALL  C,<1234H>", 0xFD, 0xDC, 0x30, 0x02);
        ATEST(0x1000, "CALL PO,<1234H>", 0xFD, 0xE4, 0x30, 0x02);
        ATEST(0x1000, "CALL PE,<1234H>", 0xFD, 0xEC, 0x30, 0x02);
        ATEST(0x1000, "CALL  P,<1234H>", 0xFD, 0xF4, 0x30, 0x02);
        ATEST(0x1000, "CALL  M,<1234H>", 0xFD, 0xFC, 0x30, 0x02);
        ATEST(0x1000, "CALL NV,<1234H>", 0xFD, 0xE4, 0x30, 0x02);
        ATEST(0x1000, "CALL  V,<1234H>", 0xFD, 0xEC, 0x30, 0x02);
        ATEST(0x1000, "CALL NS,<1234H>", 0xFD, 0xF4, 0x30, 0x02);
        ATEST(0x1000, "CALL  S,<1234H>", 0xFD, 0xFC, 0x30, 0x02);
    }

    ATEST(0x1000, "DJNZ 1000H",  0x10, 0xFE);
    ATEST(0x1000, "DJNZ 1081H",  0x10, 0x7F);
    ATEST(0x1000, "DJNZ 0F82H",  0x10, 0x80);
    AERRT(0x1000, "DJNZ 1082H",  OPERAND_TOO_FAR, "1082H", 0x10, 0x80);
    AERRT(0x1000, "DJNZ 0F81H",  OPERAND_TOO_FAR, "0F81H", 0x10, 0x7F);
}

void test_incr_decr() {
    TEST("INC A", 0x3C);
    TEST("INC B", 0x04);
    TEST("INC C", 0x0C);
    TEST("INC D", 0x14);
    TEST("INC E", 0x1C);
    TEST("INC H", 0x24);
    TEST("INC L", 0x2C);
    if (z280()) {
        TEST("INC IXH", 0xDD, 0x24);
        TEST("INC IXL", 0xDD, 0x2C);
        TEST("INC IYH", 0xFD, 0x24);
        TEST("INC IYL", 0xFD, 0x2C);
    }

    TEST("INC (HL)", 0x34);
    TEST("INC (IX+2)", 0xDD, 0x34, 0x02);
    TEST("INC (IY-2)", 0xFD, 0x34, 0xFE);
    if (z280()) {
        TEST("INC (IX+128)", 0xFD, 0x0C, 0x80, 0x00);
        TEST("INC (IY+128)", 0xFD, 0x14, 0x80, 0x00);
    } else {
        ERRT("INC (IX+128)", OVERFLOW_RANGE, "(IX+128)", 0xDD, 0x34, 0x80);
        ERRT("INC (IY+128)", OVERFLOW_RANGE, "(IY+128)", 0xFD, 0x34, 0x80);
    }

    TEST("DEC A", 0x3D);
    TEST("DEC B", 0x05);
    TEST("DEC C", 0x0D);
    TEST("DEC D", 0x15);
    TEST("DEC E", 0x1D);
    TEST("DEC H", 0x25);
    TEST("DEC L", 0x2D);
    if (z280()) {
        TEST("DEC IXH", 0xDD, 0x25);
        TEST("DEC IXL", 0xDD, 0x2D);
        TEST("DEC IYH", 0xFD, 0x25);
        TEST("DEC IYL", 0xFD, 0x2D);
    }

    TEST("DEC (HL)", 0x35);
    TEST("DEC (IX+2)", 0xDD, 0x35, 0x02);
    TEST("DEC (IY-2)", 0xFD, 0x35, 0xFE);
    if (z280()) {
        TEST("DEC (IX-129)", 0xFD, 0x0D, 0x7F, 0xFF);
        TEST("DEC (IY-129)", 0xFD, 0x15, 0x7F, 0xFF);
    } else {
        ERRT("DEC (IX-129)", OVERFLOW_RANGE, "(IX-129)", 0xDD, 0x35, 0x7F);
        ERRT("DEC (IY-129)", OVERFLOW_RANGE, "(IY-129)", 0xFD, 0x35, 0x7F);
    }

    TEST("INC BC", 0x03);
    TEST("INC DE", 0x13);
    TEST("INC HL", 0x23);
    TEST("INC SP", 0x33);
    TEST("INC IX", 0xDD, 0x23);
    TEST("INC IY", 0xFD, 0x23);

    TEST("DEC BC", 0x0B);
    TEST("DEC DE", 0x1B);
    TEST("DEC HL", 0x2B);
    TEST("DEC SP", 0x3B);
    TEST("DEC IX", 0xDD, 0x2B);
    TEST("DEC IY", 0xFD, 0x2B);
}

void test_alu_8bit() {
    TEST("RLCA",  0x07);
    TEST("RLC A", 0xCB, 0x07);
    TEST("RLC B", 0xCB, 0x00);
    TEST("RLC C", 0xCB, 0x01);
    TEST("RLC D", 0xCB, 0x02);
    TEST("RLC E", 0xCB, 0x03);
    TEST("RLC H", 0xCB, 0x04);
    TEST("RLC L", 0xCB, 0x05);
    TEST("RLC (HL)", 0xCB, 0x06);
    TEST("RLC (IX+127)", 0xDD, 0xCB, 0x7F, 0x06);
    TEST("RLC (IY-128)", 0xFD, 0xCB, 0x80, 0x06);

    TEST("RRCA",  0x0F);
    TEST("RRC A", 0xCB, 0x0F);
    TEST("RRC B", 0xCB, 0x08);
    TEST("RRC C", 0xCB, 0x09);
    TEST("RRC D", 0xCB, 0x0A);
    TEST("RRC E", 0xCB, 0x0B);
    TEST("RRC H", 0xCB, 0x0C);
    TEST("RRC L", 0xCB, 0x0D);
    TEST("RRC (HL)", 0xCB, 0x0E);
    TEST("RRC (IX+127)", 0xDD, 0xCB, 0x7F, 0x0E);
    TEST("RRC (IY-128)", 0xFD, 0xCB, 0x80, 0x0E);

    TEST("RLA",  0x17);
    TEST("RL A", 0xCB, 0x17);
    TEST("RL B", 0xCB, 0x10);
    TEST("RL C", 0xCB, 0x11);
    TEST("RL D", 0xCB, 0x12);
    TEST("RL E", 0xCB, 0x13);
    TEST("RL H", 0xCB, 0x14);
    TEST("RL L", 0xCB, 0x15);
    TEST("RL (HL)", 0xCB, 0x16);
    TEST("RL  (IX+127)", 0xDD, 0xCB, 0x7F, 0x16);
    TEST("RL  (IY-128)", 0xFD, 0xCB, 0x80, 0x16);

    TEST("RRA",  0x1F);
    TEST("RR A", 0xCB, 0x1F);
    TEST("RR B", 0xCB, 0x18);
    TEST("RR C", 0xCB, 0x19);
    TEST("RR D", 0xCB, 0x1A);
    TEST("RR E", 0xCB, 0x1B);
    TEST("RR H", 0xCB, 0x1C);
    TEST("RR L", 0xCB, 0x1D);
    TEST("RR (HL)", 0xCB, 0x1E);
    TEST("RR  (IX+127)", 0xDD, 0xCB, 0x7F, 0x1E);
    TEST("RR  (IY-128)", 0xFD, 0xCB, 0x80, 0x1E);

    TEST("SLA A", 0xCB, 0x27);
    TEST("SLA B", 0xCB, 0x20);
    TEST("SLA C", 0xCB, 0x21);
    TEST("SLA D", 0xCB, 0x22);
    TEST("SLA E", 0xCB, 0x23);
    TEST("SLA H", 0xCB, 0x24);
    TEST("SLA L", 0xCB, 0x25);
    TEST("SLA (HL)", 0xCB, 0x26);
    TEST("SLA (IX+127)", 0xDD, 0xCB, 0x7F, 0x26);
    TEST("SLA (IY-128)", 0xFD, 0xCB, 0x80, 0x26);

    TEST("SRA A", 0xCB, 0x2F);
    TEST("SRA B", 0xCB, 0x28);
    TEST("SRA C", 0xCB, 0x29);
    TEST("SRA D", 0xCB, 0x2A);
    TEST("SRA E", 0xCB, 0x2B);
    TEST("SRA H", 0xCB, 0x2C);
    TEST("SRA L", 0xCB, 0x2D);
    TEST("SRA (HL)", 0xCB, 0x2E);
    TEST("SRA (IX+127)", 0xDD, 0xCB, 0x7F, 0x2E);
    TEST("SRA (IY-128)", 0xFD, 0xCB, 0x80, 0x2E);

    TEST("SRL A", 0xCB, 0x3F);
    TEST("SRL B", 0xCB, 0x38);
    TEST("SRL C", 0xCB, 0x39);
    TEST("SRL D", 0xCB, 0x3A);
    TEST("SRL E", 0xCB, 0x3B);
    TEST("SRL H", 0xCB, 0x3C);
    TEST("SRL L", 0xCB, 0x3D);
    TEST("SRL (HL)", 0xCB, 0x3E);
    TEST("SRL (IX+127)", 0xDD, 0xCB, 0x7F, 0x3E);
    TEST("SRL (IY-128)", 0xFD, 0xCB, 0x80, 0x3E);
    ERRT("SRL (IX+128)", OVERFLOW_RANGE, "(IX+128)", 0xDD, 0xCB, 0x80, 0x3E);
    ERRT("SRL (IX-129)", OVERFLOW_RANGE, "(IX-129)", 0xDD, 0xCB, 0x7F, 0x3E);

    TEST("RRD", 0xED, 0x67);
    TEST("RLD", 0xED, 0x6F);

    if (z280()) {
        TEST("TSET A", 0xCB, 0x37);
        TEST("TSET B", 0xCB, 0x30);
        TEST("TSET C", 0xCB, 0x31);
        TEST("TSET D", 0xCB, 0x32);
        TEST("TSET E", 0xCB, 0x33);
        TEST("TSET H", 0xCB, 0x34);
        TEST("TSET L", 0xCB, 0x35);
        TEST("TSET (HL)", 0xCB, 0x36);
        TEST("TSET (IX+127)", 0xDD, 0xCB, 0x7F, 0x36);
        TEST("TSET (IY-128)", 0xFD, 0xCB, 0x80, 0x36);
    }

    TEST("DAA", 0x27);
    TEST("NEG", 0xED, 0x44);
    if (z280()) {
        TEST("NEG A", 0xED, 0x44);
    } else {
        ERRT("NEG A", OPERAND_NOT_ALLOWED, "A");
    }
    if (z280()) {
        TEST("EXTS",   0xED, 0x64);
        TEST("EXTS A", 0xED, 0x64);
    } else {
        ERUI("EXTS");
    }

    TEST("ADD A,A", 0x87);
    TEST("ADD A,B", 0x80);
    TEST("ADD A,C", 0x81);
    TEST("ADD A,D", 0x82);
    TEST("ADD A,E", 0x83);
    TEST("ADD A,H", 0x84);
    TEST("ADD A,L", 0x85);
    if (z280()) {
        TEST("ADD A, IXH", 0xDD, 0x84);
        TEST("ADD A, IXL", 0xDD, 0x85);
        TEST("ADD A, IYH", 0xFD, 0x84);
        TEST("ADD A, IYL", 0xFD, 0x85);
    }
    TEST("ADD A,10B",  0xC6, 0x02);
    TEST("ADD A,(HL)", 0x86);
    TEST("ADD A,(IX+2)", 0xDD, 0x86, 0x02);
    TEST("ADD A,(IY-2)", 0xFD, 0x86, 0xFE);

    if (z280()) {
        TEST("ADD A", 0x87);
        TEST("ADD B", 0x80);
        TEST("ADD C", 0x81);
        TEST("ADD D", 0x82);
        TEST("ADD E", 0x83);
        TEST("ADD H", 0x84);
        TEST("ADD L", 0x85);
        TEST("ADD IXH", 0xDD, 0x84);
        TEST("ADD IXL", 0xDD, 0x85);
        TEST("ADD IYH", 0xFD, 0x84);
        TEST("ADD IYL", 0xFD, 0x85);
        TEST("ADD 10B",  0xC6, 0x02);
        TEST("ADD (HL)", 0x86);
        TEST("ADD (IX+2)", 0xDD, 0x86, 0x02);
        TEST("ADD (IY-2)", 0xFD, 0x86, 0xFE);
    }

    TEST("ADC A,A", 0x8F);
    TEST("ADC A,B", 0x88);
    TEST("ADC A,C", 0x89);
    TEST("ADC A,D", 0x8A);
    TEST("ADC A,E", 0x8B);
    TEST("ADC A,H", 0x8C);
    TEST("ADC A,L", 0x8D);
    if (z280()) {
        TEST("ADC A, IXH", 0xDD, 0x8C);
        TEST("ADC A, IXL", 0xDD, 0x8D);
        TEST("ADC A, IYH", 0xFD, 0x8C);
        TEST("ADC A, IYL", 0xFD, 0x8D);
    }
    TEST("ADC A,255",  0xCE, 0xFF);
    TEST("ADC A,(HL)", 0x8E);
    TEST("ADC A,(IX+2)", 0xDD, 0x8E, 0x02);
    TEST("ADC A,(IY-2)", 0xFD, 0x8E, 0xFE);

    if (z280()) {
        TEST("ADC A", 0x8F);
        TEST("ADC B", 0x88);
        TEST("ADC C", 0x89);
        TEST("ADC D", 0x8A);
        TEST("ADC E", 0x8B);
        TEST("ADC H", 0x8C);
        TEST("ADC L", 0x8D);
        TEST("ADC IXH", 0xDD, 0x8C);
        TEST("ADC IXL", 0xDD, 0x8D);
        TEST("ADC IYH", 0xFD, 0x8C);
        TEST("ADC IYL", 0xFD, 0x8D);
        TEST("ADC 255",  0xCE, 0xFF);
        TEST("ADC (HL)", 0x8E);
        TEST("ADC (IX+2)", 0xDD, 0x8E, 0x02);
        TEST("ADC (IY-2)", 0xFD, 0x8E, 0xFE);
    }

    TEST("SUB A,A", 0x97);
    TEST("SUB A,B", 0x90);
    TEST("SUB A,C", 0x91);
    TEST("SUB A,D", 0x92);
    TEST("SUB A,E", 0x93);
    TEST("SUB A,H", 0x94);
    TEST("SUB A,L", 0x95);
    if (z280()) {
        TEST("SUB A, IXH", 0xDD, 0x94);
        TEST("SUB A, IXL", 0xDD, 0x95);
        TEST("SUB A, IYH", 0xFD, 0x94);
        TEST("SUB A, IYL", 0xFD, 0x95);
    }
    TEST("SUB A,-2",   0xD6, 0xFE);
    TEST("SUB A,(HL)", 0x96);
    TEST("SUB A,(IX+2)", 0xDD, 0x96, 0x02);
    TEST("SUB A,(IY-2)", 0xFD, 0x96, 0xFE);

    TEST("SUB A", 0x97);
    TEST("SUB B", 0x90);
    TEST("SUB C", 0x91);
    TEST("SUB D", 0x92);
    TEST("SUB E", 0x93);
    TEST("SUB H", 0x94);
    TEST("SUB L", 0x95);
    if (z280()) {
        TEST("SUB IXH", 0xDD, 0x94);
        TEST("SUB IXL", 0xDD, 0x95);
        TEST("SUB IYH", 0xFD, 0x94);
        TEST("SUB IYL", 0xFD, 0x95);
    }
    TEST("SUB -2",   0xD6, 0xFE);
    TEST("SUB (HL)", 0x96);
    TEST("SUB (IX+2)", 0xDD, 0x96, 0x02);
    TEST("SUB (IY-2)", 0xFD, 0x96, 0xFE);

    TEST("SBC A,A", 0x9F);
    TEST("SBC A,B", 0x98);
    TEST("SBC A,C", 0x99);
    TEST("SBC A,D", 0x9A);
    TEST("SBC A,E", 0x9B);
    TEST("SBC A,H", 0x9C);
    TEST("SBC A,L", 0x9D);
    if (z280()) {
        TEST("SBC A, IXH", 0xDD, 0x9C);
        TEST("SBC A, IXL", 0xDD, 0x9D);
        TEST("SBC A, IYH", 0xFD, 0x9C);
        TEST("SBC A, IYL", 0xFD, 0x9D);
    }
    TEST("SBC A,177O", 0xDE, 0x7F);
    TEST("SBC A,(HL)", 0x9E);
    TEST("SBC A,(IX+2)", 0xDD, 0x9E, 0x02);
    TEST("SBC A,(IY-2)", 0xFD, 0x9E, 0xFE);

    if (z280()) {
        TEST("SBC A", 0x9F);
        TEST("SBC B", 0x98);
        TEST("SBC C", 0x99);
        TEST("SBC D", 0x9A);
        TEST("SBC E", 0x9B);
        TEST("SBC H", 0x9C);
        TEST("SBC L", 0x9D);
        TEST("SBC IXH", 0xDD, 0x9C);
        TEST("SBC IXL", 0xDD, 0x9D);
        TEST("SBC IYH", 0xFD, 0x9C);
        TEST("SBC IYL", 0xFD, 0x9D);
        TEST("SBC 177O", 0xDE, 0x7F);
        TEST("SBC (HL)", 0x9E);
        TEST("SBC (IX+2)", 0xDD, 0x9E, 0x02);
        TEST("SBC (IY-2)", 0xFD, 0x9E, 0xFE);
    }

    TEST("AND A,A", 0xA7);
    TEST("AND A,B", 0xA0);
    TEST("AND A,C", 0xA1);
    TEST("AND A,D", 0xA2);
    TEST("AND A,E", 0xA3);
    TEST("AND A,H", 0xA4);
    TEST("AND A,L", 0xA5);
    if (z280()) {
        TEST("AND A, IXH", 0xDD, 0xA4);
        TEST("AND A, IXL", 0xDD, 0xA5);
        TEST("AND A, IYH", 0xFD, 0xA4);
        TEST("AND A, IYL", 0xFD, 0xA5);
    }
    TEST("AND A,~0FH", 0xE6, 0xF0);
    TEST("AND A,(HL)", 0xA6);
    TEST("AND A,(IX+2)", 0xDD, 0xA6, 0x02);
    TEST("AND A,(IY-2)", 0xFD, 0xA6, 0xFE);

    TEST("AND A", 0xA7);
    TEST("AND B", 0xA0);
    TEST("AND C", 0xA1);
    TEST("AND D", 0xA2);
    TEST("AND E", 0xA3);
    TEST("AND H", 0xA4);
    TEST("AND L", 0xA5);
    if (z280()) {
        TEST("AND IXH", 0xDD, 0xA4);
        TEST("AND IXL", 0xDD, 0xA5);
        TEST("AND IYH", 0xFD, 0xA4);
        TEST("AND IYL", 0xFD, 0xA5);
    }
    TEST("AND ~0FH", 0xE6, 0xF0);
    TEST("AND (HL)", 0xA6);
    TEST("AND (IX+2)", 0xDD, 0xA6, 0x02);
    TEST("AND (IY-2)", 0xFD, 0xA6, 0xFE);

    TEST("XOR A,A", 0xAF);
    TEST("XOR A,B", 0xA8);
    TEST("XOR A,C", 0xA9);
    TEST("XOR A,D", 0xAA);
    TEST("XOR A,E", 0xAB);
    TEST("XOR A,H", 0xAC);
    TEST("XOR A,L", 0xAD);
    if (z280()) {
        TEST("XOR A, IXH", 0xDD, 0xAC);
        TEST("XOR A, IXL", 0xDD, 0xAD);
        TEST("XOR A, IYH", 0xFD, 0xAC);
        TEST("XOR A, IYL", 0xFD, 0xAD);
    }
    TEST("XOR A,~001B",0xEE, 0xFE);
    TEST("XOR A,(HL)", 0xAE);
    TEST("XOR A,(IX+2)", 0xDD, 0xAE, 0x02);
    TEST("XOR A,(IY-2)", 0xFD, 0xAE, 0xFE);

    TEST("XOR A", 0xAF);
    TEST("XOR B", 0xA8);
    TEST("XOR C", 0xA9);
    TEST("XOR D", 0xAA);
    TEST("XOR E", 0xAB);
    TEST("XOR H", 0xAC);
    TEST("XOR L", 0xAD);
    if (z280()) {
        TEST("XOR IXH", 0xDD, 0xAC);
        TEST("XOR IXL", 0xDD, 0xAD);
        TEST("XOR IYH", 0xFD, 0xAC);
        TEST("XOR IYL", 0xFD, 0xAD);
    }
    TEST("XOR ~001B",0xEE, 0xFE);
    TEST("XOR (HL)", 0xAE);
    TEST("XOR (IX+2)", 0xDD, 0xAE, 0x02);
    TEST("XOR (IY-2)", 0xFD, 0xAE, 0xFE);

    TEST("OR A,A", 0xB7);
    TEST("OR A,B", 0xB0);
    TEST("OR A,C", 0xB1);
    TEST("OR A,D", 0xB2);
    TEST("OR A,E", 0xB3);
    TEST("OR A,H", 0xB4);
    TEST("OR A,L", 0xB5);
    if (z280()) {
        TEST("OR A, IXH", 0xDD, 0xB4);
        TEST("OR A, IXL", 0xDD, 0xB5);
        TEST("OR A, IYH", 0xFD, 0xB4);
        TEST("OR A, IYL", 0xFD, 0xB5);
    }
    TEST("OR A,+127", 0xF6, 0x7F);
    TEST("OR A,(HL)", 0xB6);
    TEST("OR A,(IX+2)", 0xDD, 0xB6, 0x02);
    TEST("OR A,(IY-2)", 0xFD, 0xB6, 0xFE);

    TEST("OR A", 0xB7);
    TEST("OR B", 0xB0);
    TEST("OR C", 0xB1);
    TEST("OR D", 0xB2);
    TEST("OR E", 0xB3);
    TEST("OR H", 0xB4);
    TEST("OR L", 0xB5);
    if (z280()) {
        TEST("OR IXH", 0xDD, 0xB4);
        TEST("OR IXL", 0xDD, 0xB5);
        TEST("OR IYH", 0xFD, 0xB4);
        TEST("OR IYL", 0xFD, 0xB5);
    }
    TEST("OR +127", 0xF6, 0x7F);
    TEST("OR (HL)", 0xB6);
    TEST("OR (IX+2)", 0xDD, 0xB6, 0x02);
    TEST("OR (IY-2)", 0xFD, 0xB6, 0xFE);

    TEST("CP A,A", 0xBF);
    TEST("CP A,B", 0xB8);
    TEST("CP A,C", 0xB9);
    TEST("CP A,D", 0xBA);
    TEST("CP A,E", 0xBB);
    TEST("CP A,H", 0xBC);
    TEST("CP A,L", 0xBD);
    if (z280()) {
        TEST("CP A, IXH", 0xDD, 0xBC);
        TEST("CP A, IXL", 0xDD, 0xBD);
        TEST("CP A, IYH", 0xFD, 0xBC);
        TEST("CP A, IYL", 0xFD, 0xBD);
    }
    TEST("CP A,-128", 0xFE, 0x80);
    TEST("CP A,(HL)", 0xBE);
    TEST("CP A,(IX+2)", 0xDD, 0xBE, 0x02);
    TEST("CP A,(IY-2)", 0xFD, 0xBE, 0xFE);

    TEST("CP A", 0xBF);
    TEST("CP B", 0xB8);
    TEST("CP C", 0xB9);
    TEST("CP D", 0xBA);
    TEST("CP E", 0xBB);
    TEST("CP H", 0xBC);
    TEST("CP L", 0xBD);
    if (z280()) {
        TEST("CP IXH", 0xDD, 0xBC);
        TEST("CP IXL", 0xDD, 0xBD);
        TEST("CP IYH", 0xFD, 0xBC);
        TEST("CP IYL", 0xFD, 0xBD);
    }
    TEST("CP -128", 0xFE, 0x80);
    TEST("CP (HL)", 0xBE);
    TEST("CP (IX+2)", 0xDD, 0xBE, 0x02);
    TEST("CP (IY-2)", 0xFD, 0xBE, 0xFE);

    if (z180()) {
        TEST("TST A", 0xED, 0x3C);
        TEST("TST B", 0xED, 0x04);
        TEST("TST C", 0xED, 0x0C);
        TEST("TST D", 0xED, 0x14);
        TEST("TST E", 0xED, 0x1C);
        TEST("TST H", 0xED, 0x24);
        TEST("TST L", 0xED, 0x2C);
        TEST("TST 80H", 0xED, 0x64, 0x80);
    } else {
        ERUI("TST A");
    }
}

void test_alu_16bit() {
    if (z280()) {
        TEST("ADD HL, A", 0xED, 0x6D);
    } else {
        ERRT("ADD HL, A", OPERAND_NOT_ALLOWED, "HL, A");
    }

    if (z280()) {
        TEST("NEG HL", 0xED, 0x4C);
    } else {
        ERRT("NEG HL", OPERAND_NOT_ALLOWED, "HL");
    }
    if (z280()) {
        TEST("EXTS HL", 0xED, 0x6C);
    } else {
        ERUI("EXTS");
    }

    TEST("ADD HL,BC", 0x09);
    TEST("ADD HL,DE", 0x19);
    TEST("ADD HL,HL", 0x29);
    TEST("ADD HL,SP", 0x39);
    TEST("ADD IX,BC", 0xDD, 0x09);
    TEST("ADD IX,DE", 0xDD, 0x19);
    TEST("ADD IX,IX", 0xDD, 0x29);
    TEST("ADD IX,SP", 0xDD, 0x39);
    TEST("ADD IY,BC", 0xFD, 0x09);
    TEST("ADD IY,DE", 0xFD, 0x19);
    TEST("ADD IY,IY", 0xFD, 0x29);
    TEST("ADD IY,SP", 0xFD, 0x39);
    if (z280()) {
        TEST("ADDW HL, BC", 0xED, 0xC6);
        TEST("ADDW HL, DE", 0xED, 0xD6);
        TEST("ADDW HL, HL", 0xED, 0xE6);
        TEST("ADDW HL, SP", 0xED, 0xF6);
        TEST("ADDW BC", 0xED, 0xC6);
        TEST("ADDW DE", 0xED, 0xD6);
        TEST("ADDW HL", 0xED, 0xE6);
        TEST("ADDW SP", 0xED, 0xF6);
    }

    if (z280()) {
        TEST("SUBW HL, BC", 0xED, 0xCE);
        TEST("SUBW HL, DE", 0xED, 0xDE);
        TEST("SUBW HL, HL", 0xED, 0xEE);
        TEST("SUBW HL, SP", 0xED, 0xFE);
        TEST("SUBW BC", 0xED, 0xCE);
        TEST("SUBW DE", 0xED, 0xDE);
        TEST("SUBW HL", 0xED, 0xEE);
        TEST("SUBW SP", 0xED, 0xFE);
    }

    TEST("ADC HL,BC", 0xED, 0x4A);
    TEST("ADC HL,DE", 0xED, 0x5A);
    TEST("ADC HL,HL", 0xED, 0x6A);
    TEST("ADC HL,SP", 0xED, 0x7A);

    TEST("SBC HL,BC", 0xED, 0x42);
    TEST("SBC HL,DE", 0xED, 0x52);
    TEST("SBC HL,HL", 0xED, 0x62);
    TEST("SBC HL,SP", 0xED, 0x72);

    if (z280()) {
        TEST("CPW HL, BC", 0xED, 0xC7);
        TEST("CPW HL, DE", 0xED, 0xD7);
        TEST("CPW HL, HL", 0xED, 0xE7);
        TEST("CPW HL, SP", 0xED, 0xF7);
        TEST("CPW BC", 0xED, 0xC7);
        TEST("CPW DE", 0xED, 0xD7);
        TEST("CPW HL", 0xED, 0xE7);
        TEST("CPW SP", 0xED, 0xF7);
    }

    if (z180()) {
        TEST("MLT BC", 0xED, 0x4C);
        TEST("MLT DE", 0xED, 0x5C);
        TEST("MLT HL", 0xED, 0x6C);
        TEST("MLT SP", 0xED, 0x7C);
    } else {
        ERUI("MLT BC");
    }

    if (z280()) {
        TEST("MULT A, B",    0xED, 0xC0);
        TEST("MULT A, C",    0xED, 0xC8);
        TEST("MULT A, D",    0xED, 0xD0);
        TEST("MULT A, E",    0xED, 0xD8);
        TEST("MULT A, H",    0xED, 0xE0);
        TEST("MULT A, L",    0xED, 0xE8);
        TEST("MULT A, (HL)", 0xED, 0xF0);
        TEST("MULT A, A",    0xED, 0xF8);
        TEST("MULT A, IXH",  0xDD, 0xED, 0xE0);
        TEST("MULT A, IXL",  0xDD, 0xED, 0xE8);
        TEST("MULT A, IYH",  0xFD, 0xED, 0xE0);
        TEST("MULT A, IYL",  0xFD, 0xED, 0xE8);
        TEST("MULT B",    0xED, 0xC0);
        TEST("MULT C",    0xED, 0xC8);
        TEST("MULT D",    0xED, 0xD0);
        TEST("MULT E",    0xED, 0xD8);
        TEST("MULT H",    0xED, 0xE0);
        TEST("MULT L",    0xED, 0xE8);
        TEST("MULT (HL)", 0xED, 0xF0);
        TEST("MULT A",    0xED, 0xF8);
        TEST("MULT IXH",  0xDD, 0xED, 0xE0);
        TEST("MULT IXL",  0xDD, 0xED, 0xE8);
        TEST("MULT IYH",  0xFD, 0xED, 0xE0);
        TEST("MULT IYL",  0xFD, 0xED, 0xE8);

        TEST("MULTU A, B",    0xED, 0xC1);
        TEST("MULTU A, C",    0xED, 0xC9);
        TEST("MULTU A, D",    0xED, 0xD1);
        TEST("MULTU A, E",    0xED, 0xD9);
        TEST("MULTU A, H",    0xED, 0xE1);
        TEST("MULTU A, L",    0xED, 0xE9);
        TEST("MULTU A, (HL)", 0xED, 0xF1);
        TEST("MULTU A, A",    0xED, 0xF9);
        TEST("MULTU A, IXH",  0xDD, 0xED, 0xE1);
        TEST("MULTU A, IXL",  0xDD, 0xED, 0xE9);
        TEST("MULTU A, IYH",  0xFD, 0xED, 0xE1);
        TEST("MULTU A, IYL",  0xFD, 0xED, 0xE9);
        TEST("MULTU B",    0xED, 0xC1);
        TEST("MULTU C",    0xED, 0xC9);
        TEST("MULTU D",    0xED, 0xD1);
        TEST("MULTU E",    0xED, 0xD9);
        TEST("MULTU H",    0xED, 0xE1);
        TEST("MULTU L",    0xED, 0xE9);
        TEST("MULTU (HL)", 0xED, 0xF1);
        TEST("MULTU A",    0xED, 0xF9);
        TEST("MULTU IXH",  0xDD, 0xED, 0xE1);
        TEST("MULTU IXL",  0xDD, 0xED, 0xE9);
        TEST("MULTU IYH",  0xFD, 0xED, 0xE1);
        TEST("MULTU IYL",  0xFD, 0xED, 0xE9);

        TEST("DIV HL, B",    0xED, 0xC4);
        TEST("DIV HL, C",    0xED, 0xCC);
        TEST("DIV HL, D",    0xED, 0xD4);
        TEST("DIV HL, E",    0xED, 0xDC);
        TEST("DIV HL, H",    0xED, 0xE4);
        TEST("DIV HL, L",    0xED, 0xEC);
        TEST("DIV HL, (HL)", 0xED, 0xF4);
        TEST("DIV HL, A",    0xED, 0xFC);
        TEST("DIV HL, IXH",  0xDD, 0xED, 0xE4);
        TEST("DIV HL, IXL",  0xDD, 0xED, 0xEC);
        TEST("DIV HL, IYH",  0xFD, 0xED, 0xE4);
        TEST("DIV HL, IYL",  0xFD, 0xED, 0xEC);
        TEST("DIV B",    0xED, 0xC4);
        TEST("DIV C",    0xED, 0xCC);
        TEST("DIV D",    0xED, 0xD4);
        TEST("DIV E",    0xED, 0xDC);
        TEST("DIV H",    0xED, 0xE4);
        TEST("DIV L",    0xED, 0xEC);
        TEST("DIV (HL)", 0xED, 0xF4);
        TEST("DIV A",    0xED, 0xFC);
        TEST("DIV IXH",  0xDD, 0xED, 0xE4);
        TEST("DIV IXL",  0xDD, 0xED, 0xEC);
        TEST("DIV IYH",  0xFD, 0xED, 0xE4);
        TEST("DIV IYL",  0xFD, 0xED, 0xEC);

        TEST("DIVU HL, B",    0xED, 0xC5);
        TEST("DIVU HL, C",    0xED, 0xCD);
        TEST("DIVU HL, D",    0xED, 0xD5);
        TEST("DIVU HL, E",    0xED, 0xDD);
        TEST("DIVU HL, H",    0xED, 0xE5);
        TEST("DIVU HL, L",    0xED, 0xED);
        TEST("DIVU HL, (HL)", 0xED, 0xF5);
        TEST("DIVU HL, A",    0xED, 0xFD);
        TEST("DIVU HL, IXH",  0xDD, 0xED, 0xE5);
        TEST("DIVU HL, IXL",  0xDD, 0xED, 0xED);
        TEST("DIVU HL, IYH",  0xFD, 0xED, 0xE5);
        TEST("DIVU HL, IYL",  0xFD, 0xED, 0xED);
        TEST("DIVU B",    0xED, 0xC5);
        TEST("DIVU C",    0xED, 0xCD);
        TEST("DIVU D",    0xED, 0xD5);
        TEST("DIVU E",    0xED, 0xDD);
        TEST("DIVU H",    0xED, 0xE5);
        TEST("DIVU L",    0xED, 0xED);
        TEST("DIVU (HL)", 0xED, 0xF5);
        TEST("DIVU A",    0xED, 0xFD);
        TEST("DIVU IXH",  0xDD, 0xED, 0xE5);
        TEST("DIVU IXL",  0xDD, 0xED, 0xED);
        TEST("DIVU IYH",  0xFD, 0xED, 0xE5);
        TEST("DIVU IYL",  0xFD, 0xED, 0xED);

        TEST("MULTW HL, BC", 0xED, 0xC2);
        TEST("MULTW HL, DE", 0xED, 0xD2);
        TEST("MULTW HL, HL", 0xED, 0xE2);
        TEST("MULTW HL, SP", 0xED, 0xF2);
        TEST("MULTW BC", 0xED, 0xC2);
        TEST("MULTW DE", 0xED, 0xD2);
        TEST("MULTW HL", 0xED, 0xE2);
        TEST("MULTW SP", 0xED, 0xF2);

        TEST("MULTUW HL, BC", 0xED, 0xC3);
        TEST("MULTUW HL, DE", 0xED, 0xD3);
        TEST("MULTUW HL, HL", 0xED, 0xE3);
        TEST("MULTUW HL, SP", 0xED, 0xF3);
        TEST("MULTUW BC", 0xED, 0xC3);
        TEST("MULTUW DE", 0xED, 0xD3);
        TEST("MULTUW HL", 0xED, 0xE3);
        TEST("MULTUW SP", 0xED, 0xF3);

        TEST("DIVW DEHL, BC", 0xED, 0xCA);
        TEST("DIVW DEHL, DE", 0xED, 0xDA);
        TEST("DIVW DEHL, HL", 0xED, 0xEA);
        TEST("DIVW DEHL, SP", 0xED, 0xFA);
        TEST("DIVW BC", 0xED, 0xCA);
        TEST("DIVW DE", 0xED, 0xDA);
        TEST("DIVW HL", 0xED, 0xEA);
        TEST("DIVW SP", 0xED, 0xFA);

        TEST("DIVUW DEHL, BC", 0xED, 0xCB);
        TEST("DIVUW DEHL, DE", 0xED, 0xDB);
        TEST("DIVUW DEHL, HL", 0xED, 0xEB);
        TEST("DIVUW DEHL, SP", 0xED, 0xFB);
        TEST("DIVUW BC", 0xED, 0xCB);
        TEST("DIVUW DE", 0xED, 0xDB);
        TEST("DIVUW HL", 0xED, 0xEB);
        TEST("DIVUW SP", 0xED, 0xFB);
    }
}

void test_io() {
    TEST("IN A,(0F0H)", 0xDB, 0xF0);
    ERRT("IN A,(-1)",   OVERFLOW_RANGE, "(-1)",  0xDB, 0xFF);
    ERRT("IN A,(256)",  OVERFLOW_RANGE, "(256)", 0xDB, 0x00);

    TEST("OUT (0F1H),A", 0xD3, 0xF1);
    ERRT("OUT (-1),A",   OVERFLOW_RANGE, "(-1),A",  0xD3, 0xFF);
    ERRT("OUT (256),A",  OVERFLOW_RANGE, "(256),A", 0xD3, 0x00);

    TEST("IN A,(C)", 0xED, 0x78);
    TEST("IN B,(C)", 0xED, 0x40);
    TEST("IN C,(C)", 0xED, 0x48);
    TEST("IN D,(C)", 0xED, 0x50);
    TEST("IN E,(C)", 0xED, 0x58);
    TEST("IN H,(C)", 0xED, 0x60);
    TEST("IN L,(C)", 0xED, 0x68);
    if (z280()) {
        TEST("IN IXH, (C)", 0xDD, 0xED, 0x60);
        TEST("IN IXL, (C)", 0xDD, 0xED, 0x68);
        TEST("IN IYH, (C)", 0xFD, 0xED, 0x60);
        TEST("IN IYL, (C)", 0xFD, 0xED, 0x68);
    }
    ERRT("IN (HL),(C)", OPERAND_NOT_ALLOWED, "(HL),(C)");

    TEST("OUT (C),A", 0xED, 0x79);
    TEST("OUT (C),B", 0xED, 0x41);
    TEST("OUT (C),C", 0xED, 0x49);
    TEST("OUT (C),D", 0xED, 0x51);
    TEST("OUT (C),E", 0xED, 0x59);
    TEST("OUT (C),H", 0xED, 0x61);
    TEST("OUT (C),L", 0xED, 0x69);
    ERRT("OUT (C),(HL)", OPERAND_NOT_ALLOWED, "(C),(HL)");
    if (z280()) {
        TEST("OUT (C), IXH", 0xDD, 0xED, 0x61);
        TEST("OUT (C), IXL", 0xDD, 0xED, 0x69);
        TEST("OUT (C), IYH", 0xFD, 0xED, 0x61);
        TEST("OUT (C), IYL", 0xFD, 0xED, 0x69);
    }

    if (z180()) {
        TEST("TSTIO 23H", 0xED, 0x74, 0x23);

        TEST("IN0 A, (0F0H)", 0xED, 0x38, 0xF0);
        TEST("IN0 B, (0F0H)", 0xED, 0x00, 0xF0);
        TEST("IN0 C, (0F0H)", 0xED, 0x08, 0xF0);
        TEST("IN0 D, (0F0H)", 0xED, 0x10, 0xF0);
        TEST("IN0 E, (0F0H)", 0xED, 0x18, 0xF0);
        TEST("IN0 H, (0F0H)", 0xED, 0x20, 0xF0);
        TEST("IN0 L, (0F0H)", 0xED, 0x28, 0xF0);

        TEST("OUT0 (0F1H), A", 0xED, 0x39, 0xF1);
        TEST("OUT0 (0F1H), B", 0xED, 0x01, 0xF1);
        TEST("OUT0 (0F1H), C", 0xED, 0x09, 0xF1);
        TEST("OUT0 (0F1H), D", 0xED, 0x11, 0xF1);
        TEST("OUT0 (0F1H), E", 0xED, 0x19, 0xF1);
        TEST("OUT0 (0F1H), H", 0xED, 0x21, 0xF1);
        TEST("OUT0 (0F1H), L", 0xED, 0x29, 0xF1);
    } else {
        ERUI("IN0  B, (0F0H)");
        ERUI("OUT0 (0F1H), C");
    }

    if (z280()) {
        TEST("TSTI (C)",     0xED, 0x70);
        TEST("IN   HL, (C)", 0xED, 0xB7);
        TEST("INW  HL, (C)", 0xED, 0xB7);
        TEST("OUT  (C), HL", 0xED, 0xBF);
        TEST("OUTW (C), HL", 0xED, 0xBF);
    } else {
        ERUI("TSTI (C)");
        ERRT("IN   HL, (C)", OPERAND_NOT_ALLOWED, "HL, (C)");
        ERUI("INW  HL, (C)");
        ERRT("OUT  (C), HL", OPERAND_NOT_ALLOWED, "(C), HL");
        ERUI("OUTW (C), HL");
    }
}

void test_block() {
    TEST("LDI",  0xED, 0xA0);
    TEST("LDD",  0xED, 0xA8);
    TEST("LDIR", 0xED, 0xB0);
    TEST("LDDR", 0xED, 0xB8);
    TEST("CPI",  0xED, 0xA1);
    TEST("CPD",  0xED, 0xA9);
    TEST("CPIR", 0xED, 0xB1);
    TEST("CPDR", 0xED, 0xB9);
    TEST("INI",  0xED, 0xA2);
    TEST("IND",  0xED, 0xAA);
    TEST("INIR", 0xED, 0xB2);
    TEST("INDR", 0xED, 0xBA);
    TEST("OUTI", 0xED, 0xA3);
    TEST("OUTD", 0xED, 0xAB);
    TEST("OTIR", 0xED, 0xB3);
    TEST("OTDR", 0xED, 0xBB);

    if (z180()) {
        TEST("OTIM",  0xED, 0x83);
        TEST("OTIMR", 0xED, 0x93);
        TEST("OTDM",  0xED, 0x8B);
        TEST("OTDMR", 0xED, 0x9B);
    } else {
        ERUI("OTIM");
        ERUI("OTIMR");
        ERUI("OTDM");
        ERUI("OTDMR");
    }

    if (z280()) {
        TEST("INIW",  0xED, 0x82);
        TEST("INDW",  0xED, 0x8A);
        TEST("INIRW", 0xED, 0x92);
        TEST("INDRW", 0xED, 0x9A);
        TEST("OUTIW", 0xED, 0x83);
        TEST("OUTDW", 0xED, 0x8B);
        TEST("OTIRW", 0xED, 0x93);
        TEST("OTDRW", 0xED, 0x9B);
    } else {
        ERUI("INIW");
        ERUI("INDW");
        ERUI("INIRW");
        ERUI("INDRW");
        ERUI("OUTIW");
        ERUI("OUTDW");
        ERUI("OTIRW");
        ERUI("OTDRW");
    }
}

void test_inherent() {
    TEST("DI",  0xF3);
    TEST("EI",  0xFB);
    if (z280()) {
        TEST("DI    23H", 0xED, 0x77, 0x23);
        TEST("EI    45H", 0xED, 0x7F, 0x45);
    } else {
        ERRT("DI 23H", OPERAND_NOT_ALLOWED, "23H");
        ERRT("EI 45H", OPERAND_NOT_ALLOWED, "45H");
    }
    TEST("CPL", 0x2F);
    TEST("SCF", 0x37);
    TEST("CCF", 0x3F);

    TEST("NOP",  0x00);
    TEST("HALT", 0x76);
    if (z180()) {
        TEST("SLP", 0xED, 0x76);
    } else {
        ERUI("SLP");
    }

    TEST("IM 0", 0xED, 0x46);
    TEST("IM 1", 0xED, 0x56);
    TEST("IM 2", 0xED, 0x5E);
    if (z280()) {
        TEST("IM 3", 0xED, 0x4E);
    } else {
        ERRT("IM 3",  ILLEGAL_OPERAND, "3",  0xED, 0x46);
    }
    ERRT("IM -1", ILLEGAL_OPERAND, "-1", 0xED, 0x46);
}

void test_privileged() {
    if (z280()) {
        TEST("LDCTL HL, (C)", 0xED, 0x66);
        TEST("LDCTL IX, (C)", 0xDD, 0xED, 0x66);
        TEST("LDCTL IY, (C)", 0xFD, 0xED, 0x66);
        TEST("LDCTL HL, USP", 0xED, 0x87);
        TEST("LDCTL IX, USP", 0xDD, 0xED, 0x87);
        TEST("LDCTL IY, USP", 0xFD, 0xED, 0x87);
        TEST("LDCTL (C), HL", 0xED, 0x6E);
        TEST("LDCTL (C), IX", 0xDD, 0xED, 0x6E);
        TEST("LDCTL (C), IY", 0xFD, 0xED, 0x6E);
        TEST("LDCTL USP, HL", 0xED, 0x8F);
        TEST("LDCTL USP, IX", 0xDD, 0xED, 0x8F);
        TEST("LDCTL USP, IY", 0xFD, 0xED, 0x8F);
        TEST("PCACHE ", 0xED, 0x65);
        TEST("LDUD A, (HL)", 0xED, 0x86);
        TEST("LDUP A, (HL)", 0xED, 0x96);
        TEST("EPUM (HL)",    0xED, 0xA6);
        TEST("EPUF ",        0xED, 0x97);
        TEST("EPUM (1234H)", 0xED, 0xA7, 0x34, 0x12);
        TEST("EPUI ",        0xED, 0x9F);
        TEST("LDUD (HL), A", 0xED, 0x8E);
        TEST("LDUP (HL), A", 0xED, 0x9E);
        TEST("MEPU (HL)",    0xED, 0xAE);
        TEST("MEPU (1234H)", 0xED, 0xAF, 0x34, 0x12);
    } else {
        ERUI("LDCTL HL, (C)");
        ERUI("LDCTL (C), HL");
        ERUI("PCACHE ");
        ERUI("LDUD");
        ERUI("LDUP");
        ERUI("EPUF");
        ERUI("EPUI");
        ERUI("EPUM");
        ERUI("MEPU");
    }
}

void test_restart() {
    TEST("RST 00H", 0xC7);
    TEST("RST 08H", 0xCF);
    TEST("RST 10H", 0xD7);
    TEST("RST 18H", 0xDF);
    TEST("RST 20H", 0xE7);
    TEST("RST 28H", 0xEF);
    TEST("RST 30H", 0xF7);
    TEST("RST 38H", 0xFF);
    ERRT("RST 39H", ILLEGAL_OPERAND, "39H", 0xC7);
    ERRT("RST 40H", ILLEGAL_OPERAND, "40H", 0xC7);
    ERRT("RST -1",  ILLEGAL_OPERAND, "-1",  0xC7);

    if (z280()) {
        TEST("SC 67H", 0xED, 0x71, 0x67);
    } else {
        ERUI("SC 67H");
    }
}

void test_bitop() {
    TEST("BIT 7,A", 0xCB, 0x7F);
    TEST("BIT 0,B", 0xCB, 0x40);
    TEST("BIT 1,C", 0xCB, 0x49);
    TEST("BIT 2,D", 0xCB, 0x52);
    TEST("BIT 3,E", 0xCB, 0x5B);
    TEST("BIT 4,H", 0xCB, 0x64);
    TEST("BIT 5,L", 0xCB, 0x6D);
    TEST("BIT 6,(HL)", 0xCB, 0x76);
    TEST("BIT 0,(IX-128)", 0xDD, 0xCB, 0x80, 0x46);
    TEST("BIT 5,(IY+127)", 0xFD, 0xCB, 0x7F, 0x6E);

    ERRT("BIT 8,A",  ILLEGAL_BIT_NUMBER, "8,A",  0xCB, 0x47);
    ERRT("BIT -1,A", ILLEGAL_BIT_NUMBER, "-1,A", 0xCB, 0x7F);
    ERRT("BIT 2,(IX-129)", OVERFLOW_RANGE, "(IX-129)", 0xDD, 0xCB, 0x7F, 0x56);
    ERRT("BIT 2,(IX+128)", OVERFLOW_RANGE, "(IX+128)", 0xDD, 0xCB, 0x80, 0x56);

    TEST("RES 7,A", 0xCB, 0xBF);
    TEST("RES 0,B", 0xCB, 0x80);
    TEST("RES 1,C", 0xCB, 0x89);
    TEST("RES 2,D", 0xCB, 0x92);
    TEST("RES 3,E", 0xCB, 0x9B);
    TEST("RES 4,H", 0xCB, 0xA4);
    TEST("RES 5,L", 0xCB, 0xAD);
    TEST("RES 6,(HL)", 0xCB, 0xB6);
    TEST("RES 1,(IX-128)", 0xDD, 0xCB, 0x80, 0x8E);
    TEST("RES 6,(IY+127)", 0xFD, 0xCB, 0x7F, 0xB6);

    ERRT("RES 8,A",  ILLEGAL_BIT_NUMBER, "8,A",  0xCB, 0x87);
    ERRT("RES -1,A", ILLEGAL_BIT_NUMBER, "-1,A", 0xCB, 0xBF);
    ERRT("RES 2,(IY-129)", OVERFLOW_RANGE, "(IY-129)", 0xFD, 0xCB, 0x7F, 0x96);
    ERRT("RES 2,(IY+128)", OVERFLOW_RANGE, "(IY+128)", 0xFD, 0xCB, 0x80, 0x96);

    TEST("SET 7,A", 0xCB, 0xFF);
    TEST("SET 0,B", 0xCB, 0xC0);
    TEST("SET 1,C", 0xCB, 0xC9);
    TEST("SET 2,D", 0xCB, 0xD2);
    TEST("SET 3,E", 0xCB, 0xDB);
    TEST("SET 4,H", 0xCB, 0xE4);
    TEST("SET 5,L", 0xCB, 0xED);
    TEST("SET 6,(HL)", 0xCB, 0xF6);
    TEST("SET 2,(IX-128)", 0xDD, 0xCB, 0x80, 0xD6);
    TEST("SET 7,(IY+127)", 0xFD, 0xCB, 0x7F, 0xFE);

    ERRT("SET 8,(IX-128)", ILLEGAL_BIT_NUMBER, "8,(IX-128)", 0xDD, 0xCB, 0x80, 0xC6);
    ERRT("SET -1,(IX+1)",  ILLEGAL_BIT_NUMBER, "-1,(IX+1)",  0xDD, 0xCB, 0x01, 0xFE);
    ERRT("SET 2,(IX-129)", OVERFLOW_RANGE, "(IX-129)",       0xDD, 0xCB, 0x7F, 0xD6);
    ERRT("SET 2,(IX+128)", OVERFLOW_RANGE, "(IX+128)",       0xDD, 0xCB, 0x80, 0xD6);
}

void test_comment() {
    COMM("RST 18H        ; comment", "; comment", 0xDF);
    COMM("LD B , B       ; comment", "; comment", 0x40);
    COMM("NOP            ; comment", "; comment", 0x00);
    COMM("LD BC , 0BEEFH ; comment", "; comment", 0x01, 0xEF, 0xBE);
    COMM("LD HL , ( 5678H ) ; comment", "; comment", 0x2A, 0x78, 0x56);
    COMM("CALL C , 1234H ; comment", "; comment", 0xDC, 0x34, 0x12);
    COMM("RET         ; comment", "; comment", 0xC9);
    COMM("RET NZ      ; comment", "; comment", 0xC0);

    COMM("IM 2           ; comment", "; comment", 0xED, 0x5E);
    COMM("BIT 0 , B      ; comment", "; comment", 0xCB, 0x40);
    COMM("BIT 6 , ( HL ) ; comment", "; comment", 0xCB, 0x76);
    COMM("INC ( IX + 2 ) ; comment", "; comment", 0xDD, 0x34, 0x02);
    COMM("LD ( IY - 2 ) , B  ; comment", "; comment", 0xFD, 0x70, 0xFE);
    COMM("ADD A , ( IY - 2 ) ; comment", "; comment", 0xFD, 0x86, 0xFE);
    COMM("EX AF , AF'        ; comment", "; comment", 0x08);
    COMM("LD ( 0ABCDH ) , IX ; comment", "; comment", 0xDD, 0x22, 0xCD, 0xAB);
    COMM("LD IX , ( 5678H )  ; comment", "; comment", 0xDD, 0x2A, 0x78, 0x56);
    COMM("EX ( SP ) , IX     ; comment", "; comment", 0xDD, 0xE3);
    COMM("JP ( IX )          ; comment", "; comment", 0xDD, 0xE9);

    ACOMM(0x1000, "JR 1000H     ; comment", "; comment", 0x18, 0xFE);
    ACOMM(0x1000, "JR NZ , 1004H; comment", "; comment", 0x20, 0x02);
}

void test_undefined_symbol() {
    ERUS("LD  B,UNDEF",   "UNDEF",     0x06, 0x00);
    ERUS("LD BC,UNDEF",   "UNDEF",     0x01, 0x00, 0x00);
    ERUS("LD (UNDEF),A",  "UNDEF),A",  0x32, 0x00, 0x00);
    ERUS("LD A,(UNDEF)",  "UNDEF)",    0x3A, 0x00, 0x00);
    ERUS("LD (UNDEF),HL", "UNDEF),HL", 0x22, 0x00, 0x00);
    ERUS("LD HL,(UNDEF)", "UNDEF)",    0x2A, 0x00, 0x00);
    ERUS("JP    UNDEF",   "UNDEF",     0xC3, 0x00, 0x00);
    ERUS("JP NZ,UNDEF",   "UNDEF",     0xC2, 0x00, 0x00);
    ERUS("CALL    UNDEF", "UNDEF",     0xCD, 0x00, 0x00);
    ERUS("CALL NZ,UNDEF", "UNDEF",     0xC4, 0x00, 0x00);
    ERUS("ADD  A,UNDEF",  "UNDEF",     0xC6, 0x00);
    ERUS("OUT (UNDEF),A", "UNDEF),A",  0xD3, 0x00);
    ERUS("IN  A,(UNDEF)", "UNDEF)",    0xDB, 0x00);
    ERUS("RST UNDEF",     "UNDEF",     0xC7);

    ERUS("LD (UNDEF),BC",        "UNDEF),BC",      0xED, 0x43, 0x00, 0x00);
    ERUS("LD BC,(UNDEF)",        "UNDEF)",         0xED, 0x4B, 0x00, 0x00);
    ERUS("IM UNDEF",             "UNDEF",          0xED, 0x46);
    ERUS("BIT UNDEF,B",          "UNDEF,B",        0xCB, 0x40);
    ERUS("LD (UNDEF),IY",        "UNDEF),IY",      0xFD, 0x22, 0x00, 0x00);
    ERUS("LD IY,(UNDEF)",        "UNDEF)",         0xFD, 0x2A, 0x00, 0x00);
    ERUS("INC (IX+UNDEF)",       "UNDEF)",         0xDD, 0x34, 0x00);
    ERUS("LD (IY-UNDEF),B",      "UNDEF),B",       0xFD, 0x70, 0x00);
    ERUS("LD (IY-2),UNDEF",      "UNDEF",          0xFD, 0x36, 0xFE, 0x00);
    ERUS("RLC (IX+UNDEF)",       "UNDEF)",         0xDD, 0xCB, 0x00, 0x06);
    ERUS("BIT UNDEF,(IX-128)",   "UNDEF,(IX-128)", 0xDD, 0xCB, 0x80, 0x46);
    ERUS("RES 7,(IX-UNDEF)",     "UNDEF)",         0xDD, 0xCB, 0x00, 0xBE);
    ERUS("SET UNDEF,(IX-UNDEF)", "UNDEF,(IX-UNDEF)", 0xDD, 0xCB, 0x00, 0xC6);

    AERUS(0x1000, "JR UNDEF",    "UNDEF", 0x18, 0x00);
    AERUS(0x1000, "JR NZ,UNDEF", "UNDEF", 0x20, 0x00);
    AERUS(0x1000, "DJNZ UNDEF",  "UNDEF", 0x10, 0x00);

    COMM("DB -128, 255 ; comment", "; comment", 0x80, 0xFF);
    COMM("DB 'TEXT'    ; comment", "; comment", 0x54, 0x45, 0x58, 0x54);
    COMM("DW -128, 255 ; comment", "; comment", 0x80, 0xFF, 0xFF, 0x00);
    COMM("DL 12345678H ; comment", "; comment", 0x78, 0x56, 0x34, 0x12);
    COMM("DEFB -128, 255 ; comment", "; comment", 0x80, 0xFF);
    COMM("DEFB 'TEXT'    ; comment", "; comment", 0x54, 0x45, 0x58, 0x54);
    COMM("DEFM 'TEXT'    ; comment", "; comment", 0x54, 0x45, 0x58, 0x54);
    COMM("DEFW -128, 255 ; comment", "; comment", 0x80, 0xFF, 0xFF, 0x00);
}

void test_data_constant() {
    TEST("DB -128, 255", 0x80, 0xFF);
    TEST(R"(DB 'A', '"')", 0x41, 0x22);
    TEST("DB '9'-'0'",   0x09);
    TEST("DB ''''",      0x27);
    ERRT("DB '''",       MISSING_CLOSING_QUOTE, "'''");
    TEST("DB 'A''B',0",  0x41, 0x27, 0x42, 0x00);
    ERRT("DB 'A''B,0",   MISSING_CLOSING_QUOTE, "'A''B,0");
    TEST("DW -128, 255", 0x80, 0xFF, 0xFF, 0x00);
    TEST("DW 'A''B'",    0x41, 0x27, 0x42, 0x00);
    ERRT("DW 'A''B",     MISSING_CLOSING_QUOTE, "'A''B");
    TEST("DL 12345678H", 0x78, 0x56, 0x34, 0x12);
    TEST(R"(DL 'A''B"C')", 0x41, 0x27, 0x42, 0x22, 0x43, 0x00, 0x00, 0x00);
    ERRT(R"(DL 'A''B"C)",  MISSING_CLOSING_QUOTE, R"('A''B"C)");
    TEST("DEFB -128, 255", 0x80, 0xFF);
    TEST(R"(DEFB 'A', '"')", 0x41, 0x22);
    TEST("DEFB '9'-'0'",   0x09);
    TEST("DEFB ''''",      0x27);
    ERRT("DEFB '''",       MISSING_CLOSING_QUOTE, "'''");
    TEST("DEFB 'A''B',0",  0x41, 0x27, 0x42, 0x00);
    ERRT("DEFB 'A''B,0",   MISSING_CLOSING_QUOTE, "'A''B,0");
    TEST("DEFW -128, 255", 0x80, 0xFF, 0xFF, 0x00);
    TEST("DEFW 'A''B'",    0x41, 0x27, 0x42, 0x00);
    ERRT("DEFW 'A''B",     MISSING_CLOSING_QUOTE, "'A''B");
    TEST(R"(DEFM 'A''B"C')", 0x41, 0x27, 0x42, 0x22, 0x43);
    ERUS("DB 1, UNDEF, 2", "UNDEF, 2", 0x01, 0x00, 0x02);
    ERUS("DW 1, UNDEF, 2", "UNDEF, 2", 0x01, 0x00, 0x00, 0x00, 0x02, 0x00);
    ERUS("DL 1, UNDEF, 2", "UNDEF, 2",
         0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00);
    ERUS("DEFB 1, UNDEF, 2", "UNDEF, 2", 0x01, 0x00, 0x02);
    ERUS("DEFM 1, UNDEF, 2", "UNDEF, 2", 0x01, 0x00, 0x02);
    ERUS("DEFW 1, UNDEF, 2", "UNDEF, 2", 0x01, 0x00, 0x00, 0x00, 0x02, 0x00);

    ERRT("DB '"
         "1234567890" "1234567890" "1234567890" "1234567890" "1234567890" "1234567890"
         "1234567890'",
         NO_MEMORY,
         "567890'",
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34);

    ERRT("DW "
         "1234H, 5678H, 9ABCH, 0DEF0H, 1234H, 5678H, 9ABCH, 0DEF0H, "
         "1234H, 5678H, 9ABCH, 0DEF0H, 1234H, 5678H, 9ABCH, 0DEF0H, "
         "1234H, 5678H, 9ABCH, 0DEF0H, 1234H, 5678H, 9ABCH, 0DEF0H, "
         "1234H, 5678H, 9ABCH, 0DEF0H, 1234H, 5678H, 9ABCH, 0DEH, "
         "0AAAAH, 0BBBBH",
         NO_MEMORY,
         "0AAAAH, 0BBBBH",
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE,
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE,
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE,
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xDE, 0x00);
}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_load_registers);
    RUN_TEST(test_load_immediate);
    RUN_TEST(test_load);
    RUN_TEST(test_store);
    RUN_TEST(test_exchange);
    RUN_TEST(test_stack);
    RUN_TEST(test_jump);
    RUN_TEST(test_incr_decr);
    RUN_TEST(test_alu_8bit);
    RUN_TEST(test_alu_16bit);
    RUN_TEST(test_io);
    RUN_TEST(test_block);
    RUN_TEST(test_inherent);
    RUN_TEST(test_privileged);
    RUN_TEST(test_restart);
    RUN_TEST(test_bitop);
    RUN_TEST(test_comment);
    RUN_TEST(test_undefined_symbol);
    RUN_TEST(test_data_constant);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
