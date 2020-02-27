#include "dis_z80.h"
#include "test_dis_helper.h"

TestAsserter asserter;
TestMemory memory;
TestSymtab symtab;
DisZ80 disz80;
Disassembler<target::uintptr_t> &disassembler(disz80);

static void set_up() {
    disassembler.setCpu("8080");
}

static void tear_down() {
    symtab.reset();
}

static void test_cpu() {
    asserter.equals(
        "cpu 8080", true, disassembler.setCpu("8080"));
    asserter.equals(
        "cpu i8080", true, disassembler.setCpu("i8080"));
    asserter.equals(
        "cpu z80", true, disassembler.setCpu("z80"));
    asserter.equals(
        "cpu Z80", true, disassembler.setCpu("Z80"));
}

static void test_move_inherent() {
    TEST(LD, "B,B", 0x40);
    TEST(LD, "B,C", 0x41);
    TEST(LD, "B,D", 0x42);
    TEST(LD, "B,E", 0x43);
    TEST(LD, "B,H", 0x44);
    TEST(LD, "B,L", 0x45);
    TEST(LD, "B,(HL)", 0x46);
    TEST(LD, "B,A", 0x47);
    TEST(LD, "C,B", 0x48);
    TEST(LD, "C,C", 0x49);
    TEST(LD, "C,D", 0x4A);
    TEST(LD, "C,E", 0x4B);
    TEST(LD, "C,H", 0x4C);
    TEST(LD, "C,L", 0x4D);
    TEST(LD, "C,(HL)", 0x4E);
    TEST(LD, "C,A", 0x4F);

    TEST(LD, "D,B", 0x50);
    TEST(LD, "D,C", 0x51);
    TEST(LD, "D,D", 0x52);
    TEST(LD, "D,E", 0x53);
    TEST(LD, "D,H", 0x54);
    TEST(LD, "D,L", 0x55);
    TEST(LD, "D,(HL)", 0x56);
    TEST(LD, "D,A", 0x57);
    TEST(LD, "E,B", 0x58);
    TEST(LD, "E,C", 0x59);
    TEST(LD, "E,D", 0x5A);
    TEST(LD, "E,E", 0x5B);
    TEST(LD, "E,H", 0x5C);
    TEST(LD, "E,L", 0x5D);
    TEST(LD, "E,(HL)", 0x5E);
    TEST(LD, "E,A", 0x5F);

    TEST(LD, "H,B", 0x60);
    TEST(LD, "H,C", 0x61);
    TEST(LD, "H,D", 0x62);
    TEST(LD, "H,E", 0x63);
    TEST(LD, "H,H", 0x64);
    TEST(LD, "H,L", 0x65);
    TEST(LD, "H,(HL)", 0x66);
    TEST(LD, "H,A", 0x67);
    TEST(LD, "L,B", 0x68);
    TEST(LD, "L,C", 0x69);
    TEST(LD, "L,D", 0x6A);
    TEST(LD, "L,E", 0x6B);
    TEST(LD, "L,H", 0x6C);
    TEST(LD, "L,L", 0x6D);
    TEST(LD, "L,(HL)", 0x6E);
    TEST(LD, "L,A", 0x6F);

    TEST(LD, "(HL),B", 0x70);
    TEST(LD, "(HL),C", 0x71);
    TEST(LD, "(HL),D", 0x72);
    TEST(LD, "(HL),E", 0x73);
    TEST(LD, "(HL),H", 0x74);
    TEST(LD, "(HL),L", 0x75);
    // HALT
    TEST(LD, "(HL),A", 0x77);
    TEST(LD, "A,B", 0x78);
    TEST(LD, "A,C", 0x79);
    TEST(LD, "A,D", 0x7A);
    TEST(LD, "A,E", 0x7B);
    TEST(LD, "A,H", 0x7C);
    TEST(LD, "A,L", 0x7D);
    TEST(LD, "A,(HL)", 0x7E);
    TEST(LD, "A,A", 0x7F);

    TEST(LD, "(BC),A", 0x02);
    TEST(LD, "(DE),A", 0x12);
    TEST(LD, "A,(BC)", 0x0A);
    TEST(LD, "A,(DE)", 0x1A);

    // Z80
    disassembler.setCpu("z80");
    TEST(LD, "I,A", 0xED, 0x47);
    TEST(LD, "R,A", 0xED, 0x4F);
    TEST(LD, "A,I", 0xED, 0x57);
    TEST(LD, "A,R", 0xED, 0x5F);

    TEST(LDI,  "",  0xED, 0xA0);
    TEST(LDD,  "",  0xED, 0xA8);
    TEST(LDIR, "",  0xED, 0xB0);
    TEST(LDDR, "",  0xED, 0xB8);
    TEST(CPI,  "",  0xED, 0xA1);
    TEST(CPD,  "",  0xED, 0xA9);
    TEST(CPIR, "",  0xED, 0xB1);
    TEST(CPDR, "",  0xED, 0xB9);
    TEST(INI,  "",  0xED, 0xA2);
    TEST(IND,  "",  0xED, 0xAA);
    TEST(INIR, "",  0xED, 0xB2);
    TEST(INDR, "",  0xED, 0xBA);
    TEST(OUTI, "",  0xED, 0xA3);
    TEST(OUTD, "",  0xED, 0xAB);
    TEST(OTIR, "",  0xED, 0xB3);
    TEST(OTDR, "",  0xED, 0xBB);
}

static void test_move_immediate() {
    TEST(LD, "B,0F6H", 0x06, 0xF6);
    TEST(LD, "C,9FH",  0x0E, 0x9F);
    TEST(LD, "D,3AH",  0x16, 0x3A);
    TEST(LD, "E,80H",  0x1E, 0x80);
    TEST(LD, "H,0F6H", 0x26, 0xF6);
    TEST(LD, "L,0F6H", 0x2E, 0xF6);
    TEST(LD, "(HL),0F6H", 0x36, 0xF6);
    TEST(LD, "A,0FEH",    0x3E, 0xFE);

    TEST(LD,  "BC,0BEEFH", 0x01, 0xEF, 0xBE);
    TEST(LD,  "DE,1234H",  0x11, 0x34, 0x12);
    TEST(LD,  "HL,0BEEFH", 0x21, 0xEF, 0xBE);
    TEST(LD,  "SP,6789H",  0x31, 0x89, 0x67);
}

static void test_move_direct() {
    TEST(LD, "(9ABCH),A", 0x32, 0xBC, 0x9A);
    TEST(LD, "A,(1234H)", 0x3A, 0x34, 0x12);

    TEST(LD, "(0ABCDH),HL", 0x22, 0xCD, 0xAB);
    TEST(LD, "HL,(5678H)",  0x2A, 0x78, 0x56);

    // Z80
    disassembler.setCpu("z80");
    TEST(LD, "(0ABCDH),BC", 0xED, 0x43, 0xCD, 0xAB);
    TEST(LD, "(0ABCDH),DE", 0xED, 0x53, 0xCD, 0xAB);
    TEST(LD, "(0ABCDH),SP", 0xED, 0x73, 0xCD, 0xAB);
    TEST(LD, "BC,(5678H)",  0xED, 0x4B, 0x78, 0x56);
    TEST(LD, "DE,(5678H)",  0xED, 0x5B, 0x78, 0x56);
    TEST(LD, "SP,(5678H)",  0xED, 0x7B, 0x78, 0x56);
}

static void test_stack_op() {
    TEST(PUSH, "BC", 0xC5);
    TEST(PUSH, "DE", 0xD5);
    TEST(PUSH, "HL", 0xE5);
    TEST(PUSH, "AF", 0xF5);
    TEST(POP,  "BC", 0xC1);
    TEST(POP,  "DE", 0xD1);
    TEST(POP,  "HL", 0xE1);
    TEST(POP,  "AF", 0xF1);

    TEST(EX, "(SP),HL", 0xE3);
    TEST(JP, "(HL)",    0xE9);
    TEST(LD, "SP,HL",   0xF9);
    TEST(EX, "DE,HL",   0xEB);

    // Z80
    disassembler.setCpu("z80");
    TEST(EX, "AF,AF'",  0x08);
    TEST(EXX, "",       0xD9);
}

static void test_jump_call() {
    TEST(JP, "1234H",  0xC3, 0x34, 0x12);
    TEST(JP, "NZ,1234H",  0xC2, 0x34, 0x12);
    TEST(JP,  "Z,1234H",  0xCA, 0x34, 0x12);
    TEST(JP, "NC,1234H",  0xD2, 0x34, 0x12);
    TEST(JP,  "C,1234H",  0xDA, 0x34, 0x12);
    TEST(JP, "PO,1234H",  0xE2, 0x34, 0x12);
    TEST(JP, "PE,1234H",  0xEA, 0x34, 0x12);
    TEST(JP,  "P,1234H",  0xF2, 0x34, 0x12);
    TEST(JP,  "M,1234H",  0xFA, 0x34, 0x12);

    TEST(CALL, "1234H",  0xCD, 0x34, 0x12);
    TEST(CALL, "NZ,1234H",  0xC4, 0x34, 0x12);
    TEST(CALL,  "Z,1234H",  0xCC, 0x34, 0x12);
    TEST(CALL, "NC,1234H",  0xD4, 0x34, 0x12);
    TEST(CALL,  "C,1234H",  0xDC, 0x34, 0x12);
    TEST(CALL, "PO,1234H",  0xE4, 0x34, 0x12);
    TEST(CALL, "PE,1234H",  0xEC, 0x34, 0x12);
    TEST(CALL,  "P,1234H",  0xF4, 0x34, 0x12);
    TEST(CALL,  "M,1234H",  0xFC, 0x34, 0x12);

    TEST(RET, "",  0xC9);
    TEST(RET, "NZ",  0xC0);
    TEST(RET,  "Z",  0xC8);
    TEST(RET, "NC",  0xD0);
    TEST(RET,  "C",  0xD8);
    TEST(RET, "PO",  0xE0);
    TEST(RET, "PE",  0xE8);
    TEST(RET,  "P",  0xF0);
    TEST(RET,  "M",  0xF8);

    // Z80
    disassembler.setCpu("z80");
    TEST(RETN, "",  0xED, 0x45);
    TEST(RETI, "",  0xED, 0x4D);

    TEST(IM,   "0", 0xED, 0x46);
    TEST(IM,   "1", 0xED, 0x56);
    TEST(IM,   "2", 0xED, 0x5E);
}

static void test_incr_decr() {
    TEST(INC, "B",   0x04);
    TEST(INC, "C",   0x0C);
    TEST(INC, "D",   0x14);
    TEST(INC, "E",   0x1C);
    TEST(INC, "H",   0x24);
    TEST(INC, "L",   0x2C);
    TEST(INC, "(HL)",0x34);
    TEST(INC, "A",   0x3C);

    TEST(DEC, "B",   0x05);
    TEST(DEC, "C",   0x0D);
    TEST(DEC, "D",   0x15);
    TEST(DEC, "E",   0x1D);
    TEST(DEC, "H",   0x25);
    TEST(DEC, "L",   0x2D);
    TEST(DEC, "(HL)",0x35);
    TEST(DEC, "A",   0x3D);

    TEST(INC, "BC", 0x03);
    TEST(INC, "DE", 0x13);
    TEST(INC, "HL", 0x23);
    TEST(INC, "SP", 0x33);
    TEST(DEC, "BC", 0x0B);
    TEST(DEC, "DE", 0x1B);
    TEST(DEC, "HL", 0x2B);
    TEST(DEC, "SP", 0x3B);
}

static void test_alu_register() {
    TEST(ADD, "A,B", 0x80);
    TEST(ADD, "A,C", 0x81);
    TEST(ADD, "A,D", 0x82);
    TEST(ADD, "A,E", 0x83);
    TEST(ADD, "A,H", 0x84);
    TEST(ADD, "A,L", 0x85);
    TEST(ADD, "A,(HL)", 0x86);
    TEST(ADD, "A,A", 0x87);

    TEST(ADC, "A,B", 0x88);
    TEST(ADC, "A,C", 0x89);
    TEST(ADC, "A,D", 0x8A);
    TEST(ADC, "A,E", 0x8B);
    TEST(ADC, "A,H", 0x8C);
    TEST(ADC, "A,L", 0x8D);
    TEST(ADC, "A,(HL)", 0x8E);
    TEST(ADC, "A,A", 0x8F);

    TEST(SUB, "B", 0x90);
    TEST(SUB, "C", 0x91);
    TEST(SUB, "D", 0x92);
    TEST(SUB, "E", 0x93);
    TEST(SUB, "H", 0x94);
    TEST(SUB, "L", 0x95);
    TEST(SUB, "(HL)", 0x96);
    TEST(SUB, "A", 0x97);

    TEST(SBC, "A,B", 0x98);
    TEST(SBC, "A,C", 0x99);
    TEST(SBC, "A,D", 0x9A);
    TEST(SBC, "A,E", 0x9B);
    TEST(SBC, "A,H", 0x9C);
    TEST(SBC, "A,L", 0x9D);
    TEST(SBC, "A,(HL)", 0x9E);
    TEST(SBC, "A,A", 0x9F);

    TEST(AND, "B", 0xA0);
    TEST(AND, "C", 0xA1);
    TEST(AND, "D", 0xA2);
    TEST(AND, "E", 0xA3);
    TEST(AND, "H", 0xA4);
    TEST(AND, "L", 0xA5);
    TEST(AND, "(HL)", 0xA6);
    TEST(AND, "A", 0xA7);

    TEST(XOR, "B", 0xA8);
    TEST(XOR, "C", 0xA9);
    TEST(XOR, "D", 0xAA);
    TEST(XOR, "E", 0xAB);
    TEST(XOR, "H", 0xAC);
    TEST(XOR, "L", 0xAD);
    TEST(XOR, "(HL)", 0xAE);
    TEST(XOR, "A", 0xAF);

    TEST(OR,  "B", 0xB0);
    TEST(OR,  "C", 0xB1);
    TEST(OR,  "D", 0xB2);
    TEST(OR,  "E", 0xB3);
    TEST(OR,  "H", 0xB4);
    TEST(OR,  "L", 0xB5);
    TEST(OR,  "(HL)", 0xB6);
    TEST(OR,  "A", 0xB7);

    TEST(CP,  "B", 0xB8);
    TEST(CP,  "C", 0xB9);
    TEST(CP,  "D", 0xBA);
    TEST(CP,  "E", 0xBB);
    TEST(CP,  "H", 0xBC);
    TEST(CP,  "L", 0xBD);
    TEST(CP,  "(HL)", 0xBE);
    TEST(CP,  "A", 0xBF);

    TEST(ADD, "HL,BC", 0x09);
    TEST(ADD, "HL,DE", 0x19);
    TEST(ADD, "HL,HL", 0x29);
    TEST(ADD, "HL,SP", 0x39);

    // Z80
    disassembler.setCpu("z80");
    TEST(ADC, "HL,BC", 0xED, 0x4A);
    TEST(ADC, "HL,DE", 0xED, 0x5A);
    TEST(ADC, "HL,HL", 0xED, 0x6A);
    TEST(ADC, "HL,SP", 0xED, 0x7A);
    TEST(SBC, "HL,BC", 0xED, 0x42);
    TEST(SBC, "HL,DE", 0xED, 0x52);
    TEST(SBC, "HL,HL", 0xED, 0x62);
    TEST(SBC, "HL,SP", 0xED, 0x72);
}

static void test_alu_immediate() {
    TEST(ADD, "A,1",    0xC6, 0x01);
    TEST(ADC, "A,0FFH", 0xCE, 0xFF);
    TEST(SUB, "2",      0xD6, 0x02);
    TEST(SBC, "A,0FFH", 0xDE, 0xFF);
    TEST(AND, "9",      0xE6, 0x09);
    TEST(XOR, "0FFH",   0xEE, 0xFF);
    TEST(OR,  "10",     0xF6, 0x0A);
    TEST(CP,  "0",      0xFE, 0x00);
}

static void test_io() {
    TEST(OUT, "(0F1H),A", 0xD3, 0xF1);
    TEST(IN,  "A,(0F0H)", 0xDB, 0xF0);

    // Z80
    disassembler.setCpu("z80");
    TEST(IN,  "B,(C)", 0xED, 0x40);
    TEST(IN,  "C,(C)", 0xED, 0x48);
    TEST(IN,  "D,(C)", 0xED, 0x50);
    TEST(IN,  "E,(C)", 0xED, 0x58);
    TEST(IN,  "H,(C)", 0xED, 0x60);
    TEST(IN,  "L,(C)", 0xED, 0x68);
    TEST(IN,  "A,(C)", 0xED, 0x78);

    TEST(OUT, "(C),B", 0xED, 0x41);
    TEST(OUT, "(C),C", 0xED, 0x49);
    TEST(OUT, "(C),D", 0xED, 0x51);
    TEST(OUT, "(C),E", 0xED, 0x59);
    TEST(OUT, "(C),H", 0xED, 0x61);
    TEST(OUT, "(C),L", 0xED, 0x69);
    TEST(OUT, "(C),A", 0xED, 0x79);
}

static void test_inherent() {
    TEST(DI,   "", 0xF3);
    TEST(EI,   "", 0xFB);

    TEST(NOP,  "", 0x00);
    TEST(HALT, "", 0x76);

    TEST(RLCA, "", 0x07);
    TEST(RRCA, "", 0x0F);
    TEST(RLA,  "", 0x17);
    TEST(RRA,  "", 0x1F);

    TEST(DAA, "", 0x27);
    TEST(CPL, "", 0x2F);
    TEST(SCF, "", 0x37);
    TEST(CCF, "", 0x3F);

    // Z80
    disassembler.setCpu("z80");
    TEST(NEG, "", 0xED, 0x44);
}

static void test_restart() {
    TEST(RST, "00H", 0xC7);
    TEST(RST, "08H", 0xCF);
    TEST(RST, "10H", 0xD7);
    TEST(RST, "18H", 0xDF);
    TEST(RST, "20H", 0xE7);
    TEST(RST, "28H", 0xEF);
    TEST(RST, "30H", 0xF7);
    TEST(RST, "38H", 0xFF);
}

static void test_relative() {
    // Z80
    disassembler.setCpu("z80");
    ATEST(0x1000, DJNZ, "1000H",    0x10, 0xFE);
    ATEST(0x1000, JR,   "1000H",    0x18, 0xFE);
    ATEST(0x1000, JR,   "NZ,1004H", 0x20, 0x02);
    ATEST(0x1000, JR,   "Z,1081H",  0x28, 0x7F);
    ATEST(0x1000, JR,   "NC,0F82H", 0x30, 0x80);
    ATEST(0x1000, JR,   "C,0F82H",  0x38, 0x80);
}

static void test_shift() {
    // Z80
    disassembler.setCpu("z80");
    TEST(RLC, "B", 0xCB, 0x00);
    TEST(RLC, "C", 0xCB, 0x01);
    TEST(RLC, "D", 0xCB, 0x02);
    TEST(RLC, "E", 0xCB, 0x03);
    TEST(RLC, "H", 0xCB, 0x04);
    TEST(RLC, "L", 0xCB, 0x05);
    TEST(RLC, "(HL)", 0xCB, 0x06);
    TEST(RLC, "A", 0xCB, 0x07);

    TEST(RRC, "B", 0xCB, 0x08);
    TEST(RRC, "C", 0xCB, 0x09);
    TEST(RRC, "D", 0xCB, 0x0A);
    TEST(RRC, "E", 0xCB, 0x0B);
    TEST(RRC, "H", 0xCB, 0x0C);
    TEST(RRC, "L", 0xCB, 0x0D);
    TEST(RRC, "(HL)", 0xCB, 0x0E);
    TEST(RRC, "A", 0xCB, 0x0F);

    TEST(RL,  "B", 0xCB, 0x10);
    TEST(RL,  "C", 0xCB, 0x11);
    TEST(RL,  "D", 0xCB, 0x12);
    TEST(RL,  "E", 0xCB, 0x13);
    TEST(RL,  "H", 0xCB, 0x14);
    TEST(RL,  "L", 0xCB, 0x15);
    TEST(RL,  "(HL)", 0xCB, 0x16);
    TEST(RL,  "A", 0xCB, 0x17);

    TEST(RR,  "B", 0xCB, 0x18);
    TEST(RR,  "C", 0xCB, 0x19);
    TEST(RR,  "D", 0xCB, 0x1A);
    TEST(RR,  "E", 0xCB, 0x1B);
    TEST(RR,  "H", 0xCB, 0x1C);
    TEST(RR,  "L", 0xCB, 0x1D);
    TEST(RR,  "(HL)", 0xCB, 0x1E);
    TEST(RR,  "A", 0xCB, 0x1F);

    TEST(SLA, "B", 0xCB, 0x20);
    TEST(SLA, "C", 0xCB, 0x21);
    TEST(SLA, "D", 0xCB, 0x22);
    TEST(SLA, "E", 0xCB, 0x23);
    TEST(SLA, "H", 0xCB, 0x24);
    TEST(SLA, "L", 0xCB, 0x25);
    TEST(SLA, "(HL)", 0xCB, 0x26);
    TEST(SLA, "A", 0xCB, 0x27);

    TEST(SRA, "B", 0xCB, 0x28);
    TEST(SRA, "C", 0xCB, 0x29);
    TEST(SRA, "D", 0xCB, 0x2A);
    TEST(SRA, "E", 0xCB, 0x2B);
    TEST(SRA, "H", 0xCB, 0x2C);
    TEST(SRA, "L", 0xCB, 0x2D);
    TEST(SRA, "(HL)", 0xCB, 0x2E);
    TEST(SRA, "A", 0xCB, 0x2F);

    TEST(SRL, "B", 0xCB, 0x38);
    TEST(SRL, "C", 0xCB, 0x39);
    TEST(SRL, "D", 0xCB, 0x3A);
    TEST(SRL, "E", 0xCB, 0x3B);
    TEST(SRL, "H", 0xCB, 0x3C);
    TEST(SRL, "L", 0xCB, 0x3D);
    TEST(SRL, "(HL)", 0xCB, 0x3E);
    TEST(SRL, "A", 0xCB, 0x3F);

    TEST(RRD, "", 0xED, 0x67);
    TEST(RLD, "", 0xED, 0x6F);
}

static void test_bitop() {
    // Z80
    disassembler.setCpu("z80");
    TEST(BIT, "0,B", 0xCB, 0x40);
    TEST(BIT, "1,C", 0xCB, 0x49);
    TEST(BIT, "2,D", 0xCB, 0x52);
    TEST(BIT, "3,E", 0xCB, 0x5B);
    TEST(BIT, "4,H", 0xCB, 0x64);
    TEST(BIT, "5,L", 0xCB, 0x6D);
    TEST(BIT, "6,(HL)", 0xCB, 0x76);
    TEST(BIT, "7,A", 0xCB, 0x7F);

    TEST(RES, "0,B", 0xCB, 0x80);
    TEST(RES, "1,C", 0xCB, 0x89);
    TEST(RES, "2,D", 0xCB, 0x92);
    TEST(RES, "3,E", 0xCB, 0x9B);
    TEST(RES, "4,H", 0xCB, 0xA4);
    TEST(RES, "5,L", 0xCB, 0xAD);
    TEST(RES, "6,(HL)", 0xCB, 0xB6);
    TEST(RES, "7,A", 0xCB, 0xBF);

    TEST(SET, "0,B", 0xCB, 0xC0);
    TEST(SET, "1,C", 0xCB, 0xC9);
    TEST(SET, "2,D", 0xCB, 0xD2);
    TEST(SET, "3,E", 0xCB, 0xDB);
    TEST(SET, "4,H", 0xCB, 0xE4);
    TEST(SET, "5,L", 0xCB, 0xED);
    TEST(SET, "6,(HL)", 0xCB, 0xF6);
    TEST(SET, "7,A", 0xCB, 0xFF);
}

static void test_index_registers() {
    // Z80
    disassembler.setCpu("z80");
    TEST(ADD, "IX,BC", 0xDD, 0x09);
    TEST(ADD, "IX,DE", 0xDD, 0x19);
    TEST(ADD, "IX,IX", 0xDD, 0x29);
    TEST(ADD, "IX,SP", 0xDD, 0x39);
    TEST(LD,  "(0ABCDH),IX", 0xDD, 0x22, 0xCD, 0xAB);
    TEST(LD,  "IX,(5678H)",  0xDD, 0x2A, 0x78, 0x56);
    TEST(INC, "IX", 0xDD, 0x23);
    TEST(DEC, "IX", 0xDD, 0x2B);
    TEST(POP, "IX", 0xDD, 0xE1);
    TEST(PUSH,"IX", 0xDD, 0xE5);
    TEST(EX,  "(SP),IX", 0xDD, 0xE3);
    TEST(JP,  "(IX)",    0xDD, 0xE9);
    TEST(LD,  "SP,IX",   0xDD, 0xF9);

    TEST(ADD, "IY,BC", 0xFD, 0x09);
    TEST(ADD, "IY,DE", 0xFD, 0x19);
    TEST(ADD, "IY,IY", 0xFD, 0x29);
    TEST(ADD, "IY,SP", 0xFD, 0x39);
    TEST(LD,  "(0ABCDH),IY", 0xFD, 0x22, 0xCD, 0xAB);
    TEST(LD,  "IY,(5678H)",  0xFD, 0x2A, 0x78, 0x56);
    TEST(INC, "IY", 0xFD, 0x23);
    TEST(DEC, "IY", 0xFD, 0x2B);
    TEST(POP, "IY", 0xFD, 0xE1);
    TEST(PUSH,"IY", 0xFD, 0xE5);
    TEST(EX,  "(SP),IY", 0xFD, 0xE3);
    TEST(JP,  "(IY)",    0xFD, 0xE9);
    TEST(LD,  "SP,IY",   0xFD, 0xF9);
}

static void test_indexed() {
    // Z80
    disassembler.setCpu("z80");
    TEST(INC, "(IX+2)",0xDD, 0x34, 0x02);
    TEST(DEC, "(IX+2)",0xDD, 0x35, 0x02);

    TEST(LD, "B,(IX+2)", 0xDD, 0x46, 0x02);
    TEST(LD, "C,(IX+2)", 0xDD, 0x4E, 0x02);
    TEST(LD, "D,(IX+2)", 0xDD, 0x56, 0x02);
    TEST(LD, "E,(IX+2)", 0xDD, 0x5E, 0x02);
    TEST(LD, "H,(IX+2)", 0xDD, 0x66, 0x02);
    TEST(LD, "L,(IX+2)", 0xDD, 0x6E, 0x02);
    TEST(LD, "A,(IX+2)", 0xDD, 0x7E, 0x02);

    TEST(LD, "(IX+2),B", 0xDD, 0x70, 0x02);
    TEST(LD, "(IX+2),C", 0xDD, 0x71, 0x02);
    TEST(LD, "(IX+2),D", 0xDD, 0x72, 0x02);
    TEST(LD, "(IX+0),E", 0xDD, 0x73, 0x00);
    TEST(LD, "(IX+2),H", 0xDD, 0x74, 0x02);
    TEST(LD, "(IX+2),L", 0xDD, 0x75, 0x02);
    TEST(LD, "(IX+2),A", 0xDD, 0x77, 0x02);

    TEST(LD, "(IX+2),0F6H", 0xDD, 0x36, 0x02, 0xF6);

    TEST(ADD, "A,(IX+2)", 0xDD, 0x86, 0x02);
    TEST(ADC, "A,(IX+2)", 0xDD, 0x8E, 0x02);
    TEST(SUB, "(IX+2)",   0xDD, 0x96, 0x02);
    TEST(SBC, "A,(IX+2)", 0xDD, 0x9E, 0x02);
    TEST(AND, "(IX+2)",   0xDD, 0xA6, 0x02);
    TEST(XOR, "(IX+2)",   0xDD, 0xAE, 0x02);
    TEST(OR,  "(IX+2)",   0xDD, 0xB6, 0x02);
    TEST(CP,  "(IX+2)",   0xDD, 0xBE, 0x02);

    TEST(INC, "(IY-2)",   0xFD, 0x34, 0xFE);
    TEST(DEC, "(IY-2)",   0xFD, 0x35, 0xFE);

    TEST(LD, "B,(IY-2)", 0xFD, 0x46, 0xFE);
    TEST(LD, "C,(IY-2)", 0xFD, 0x4E, 0xFE);
    TEST(LD, "D,(IY-2)", 0xFD, 0x56, 0xFE);
    TEST(LD, "E,(IY-2)", 0xFD, 0x5E, 0xFE);
    TEST(LD, "H,(IY-2)", 0xFD, 0x66, 0xFE);
    TEST(LD, "L,(IY-2)", 0xFD, 0x6E, 0xFE);
    TEST(LD, "A,(IY-2)", 0xFD, 0x7E, 0xFE);

    TEST(LD, "(IY-2),B", 0xFD, 0x70, 0xFE);
    TEST(LD, "(IY-2),C", 0xFD, 0x71, 0xFE);
    TEST(LD, "(IY-2),D", 0xFD, 0x72, 0xFE);
    TEST(LD, "(IY-2),E", 0xFD, 0x73, 0xFE);
    TEST(LD, "(IY-2),H", 0xFD, 0x74, 0xFE);
    TEST(LD, "(IY-2),L", 0xFD, 0x75, 0xFE);
    TEST(LD, "(IY-2),A", 0xFD, 0x77, 0xFE);

    TEST(LD, "(IY-2),0F6H", 0xFD, 0x36, 0xFE, 0xF6);

    TEST(ADD, "A,(IY-2)", 0xFD, 0x86, 0xFE);
    TEST(ADC, "A,(IY-2)", 0xFD, 0x8E, 0xFE);
    TEST(SUB, "(IY-2)",   0xFD, 0x96, 0xFE);
    TEST(SBC, "A,(IY-2)", 0xFD, 0x9E, 0xFE);
    TEST(AND, "(IY-2)",   0xFD, 0xA6, 0xFE);
    TEST(XOR, "(IY-2)",   0xFD, 0xAE, 0xFE);
    TEST(OR,  "(IY-2)",   0xFD, 0xB6, 0xFE);
    TEST(CP,  "(IY-2)",   0xFD, 0xBE, 0xFE);
}

static void test_shift_indexed() {
    // Z80
    disassembler.setCpu("z80");
    TEST(RLC, "(IX+127)", 0xDD, 0xCB, 0x7F, 0x06);
    TEST(RRC, "(IX+127)", 0xDD, 0xCB, 0x7F, 0x0E);
    TEST(RL,  "(IX+127)", 0xDD, 0xCB, 0x7F, 0x16);
    TEST(RR,  "(IX+127)", 0xDD, 0xCB, 0x7F, 0x1E);
    TEST(SLA, "(IX+127)", 0xDD, 0xCB, 0x7F, 0x26);
    TEST(SRA, "(IX+127)", 0xDD, 0xCB, 0x7F, 0x2E);
    TEST(SRL, "(IX+127)", 0xDD, 0xCB, 0x7F, 0x3E);

    TEST(RLC, "(IY-128)", 0xFD, 0xCB, 0x80, 0x06);
    TEST(RRC, "(IY-128)", 0xFD, 0xCB, 0x80, 0x0E);
    TEST(RL,  "(IY-128)", 0xFD, 0xCB, 0x80, 0x16);
    TEST(RR,  "(IY-128)", 0xFD, 0xCB, 0x80, 0x1E);
    TEST(SLA, "(IY-128)", 0xFD, 0xCB, 0x80, 0x26);
    TEST(SRA, "(IY-128)", 0xFD, 0xCB, 0x80, 0x2E);
    TEST(SRL, "(IY-128)", 0xFD, 0xCB, 0x80, 0x3E);
}

static void test_bitop_indexed() {
    // Z80
    disassembler.setCpu("z80");
    TEST(BIT, "0,(IX-128)", 0xDD, 0xCB, 0x80, 0x46);
    TEST(RES, "1,(IX-128)", 0xDD, 0xCB, 0x80, 0x8E);
    TEST(SET, "2,(IX-128)", 0xDD, 0xCB, 0x80, 0xD6);

    TEST(BIT, "5,(IY+127)", 0xFD, 0xCB, 0x7F, 0x6E);
    TEST(RES, "6,(IY+127)", 0xFD, 0xCB, 0x7F, 0xB6);
    TEST(SET, "7,(IY+127)", 0xFD, 0xCB, 0x7F, 0xFE);
}

static void test_illegal_i8080() {
    ETEST(UNKNOWN_INSTRUCTION, _, "", 0x08);
    ETEST(UNKNOWN_INSTRUCTION, _, "", 0x10);
    ETEST(UNKNOWN_INSTRUCTION, _, "", 0x18);
    ETEST(UNKNOWN_INSTRUCTION, _, "", 0x20);
    ETEST(UNKNOWN_INSTRUCTION, _, "", 0x28);
    ETEST(UNKNOWN_INSTRUCTION, _, "", 0x38);
    ETEST(UNKNOWN_INSTRUCTION, _, "", 0xD9);
    ETEST(UNKNOWN_INSTRUCTION, _, "", 0xDD);
    ETEST(UNKNOWN_INSTRUCTION, _, "", 0xED);
    ETEST(UNKNOWN_INSTRUCTION, _, "", 0xFD);
}

static void assert_illegal(uint8_t prefix, uint8_t opc, uint8_t opr1 = 0, uint8_t opr2 = 0) {
    char operands[40];
    Insn insn;
    const uint8_t codes[] = { prefix, opc, opr1, opr2 };
    memory.setMemory(&codes[0], sizeof(codes));
    disassembler.decode(memory, insn, operands, nullptr);
    char message[40];
    sprintf(message, "%s opecode %02X %02X %02X %02X",
            __FUNCTION__, prefix, opc, opr1, opr2);
    asserter.equals(message, UNKNOWN_INSTRUCTION, disassembler.getError());
}

static void test_illegal_z80() {
    disassembler.setCpu("z80");

    for (uint8_t opc = 0x30; opc < 0x38; opc++)
        assert_illegal(0xCB, opc);

    for (uint8_t opc = 0x00; opc < 0x40; opc++)
        assert_illegal(0xED, opc);
    for (uint8_t opc = 0x7C; opc < 0xA0; opc++)
        assert_illegal(0xED, opc);
    for (uint8_t opc = 0xBC; opc; opc++)
        assert_illegal(0xED, opc);
    const uint8_t ed_illegals[] = {
        0x4C, 0x4E,
        0x54, 0x55, 0x5C, 0x5D,
        0x63, 0x64, 0x65, 0x66, 0x6B, 0x6C, 0x6D, 0x6E,
        0x70, 0x71, 0x74, 0x75, 0x76, 0x77,
        0xA4, 0xA5, 0xA6, 0xA7, 0xAC, 0xAD, 0xAE, 0xAF,
        0xB4, 0xB5, 0xB6, 0xB7,
    };
    for (int idx = 0; idx < sizeof(ed_illegals); idx++)
        assert_illegal(0xED, ed_illegals[idx]);

    const uint8_t ddfd_legals[] = {
        0x09, 0x19, 0x21, 0x22, 0x23, 0x29, 0x2A, 0x2B, 0x34, 0x35, 0x36, 0x39,
        0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E,
        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x77, 0x7E,
        0x86, 0x8E, 0x96, 0x9E, 0xA6, 0xAE, 0xB6, 0xBE,
        0xCB, 0xE1, 0xE3, 0xE5, 0xE9, 0xF9
    };
    uint8_t prefix = 0xdd;
    while (true) {
        int idx = 0;
        for (uint16_t opc = 0x00; opc < 0x100; opc++) {
            if (idx == sizeof(ddfd_legals) || opc < ddfd_legals[idx])
                assert_illegal(prefix, opc++);
            else idx++;
        }
        if (prefix == 0xfd) break;
        prefix = 0xfd;
    }

    const uint8_t ddfdcb_legals[] = {
        0x06, 0x0E, 0x16, 0x1E, 0x26, 0x2E, 0x3E,
        0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E, 0x76, 0x7E,
        0x86, 0x8E, 0x96, 0x9E, 0xA6, 0xAE, 0xB6, 0xBE,
        0xC6, 0xCE, 0xD6, 0xDE, 0xE6, 0xEE, 0xF6, 0xFE,
    };
    prefix = 0xdd;
    while (true) {
        int idx = 0;
        for (uint16_t opc = 0x00; opc < 0x100; opc++) {
            if (idx == sizeof(ddfdcb_legals) || opc < ddfdcb_legals[idx])
                assert_illegal(prefix, 0xCB, 0x00, opc++);
            else idx++;
        }
        if (prefix == 0xfd) break;
        prefix = 0xfd;
    }
}

// 00 000 000 NOP
// 00 001 000 EX AF,AF'
// 00 010 000 DJNZ dd
// 00 011 000 JR dd
// 00 1cc 000 JR NZ,dd
// 00 1cc 000 JR Z,dd
// 00 1cc 000 JR NC,dd
// 00 1cc 000 JR C,dd
// 00 pp0 001 LD   pp,nnnn
// 00 pp1 001 ADD  HL,pp
// 00 0p0 010 LD  (BC/DE),A
// 00 0p1 010 LD  A,(BC/DE)
// 00 100 010 LD  (nnnn),HL
// 00 101 010 LD  HL,(nnnn)
// 00 110 010 LD  (nnnn),A
// 00 111 010 LD  A,(nnnn)
// 00 pp0 011 INC pp
// 00 pp1 011 DEC pp
// 00 ddd 100 INC r
// 00 ddd 101 DEC r
// 00 ddd 110 LD  r,nn
// 00 000 111 RLCA
// 00 001 111 RRCA
// 00 010 111 RLA
// 00 011 111 RRA
// 00 100 111 DAA
// 00 101 111 CPL
// 00 110 111 SCF
// 00 111 111 CCF

// 01 ddd sss LD  r,r
// 01 110 110 HALT LD (HL),(HL)

// 10 000 sss ADD A,r
// 10 001 sss ADC A,r
// 10 010 sss SUB A,r
// 10 011 sss SBC A,r
// 10 100 sss AND A,r
// 10 101 sss XOR A,r
// 10 110 sss OR  A,r
// 10 111 sss CP  A,r

// 11 ccc 000 RET ccc
// 11 ss0 001 POP ss
// 11 001 001 RET
// 11 011 001 EXX
// 11 101 001 JP  (HL)
// 11 111 001 LD  SP,HL
// 11 ccc 010 JP  ccc,nnnn
// 11 000 011 JP  nnnn
// 11 001 011 PREFIX bit
// 11 010 011 OUT (nn),A
// 11 011 011 IN  A,(nn)
// 11 100 011 EX  (SP),HL
// 11 101 011 EX  DE,HL
// 11 110 011 DI
// 11 111 011 EI
// 11 ccc 100 CALL ccc
// 11 ss0 101 PUSH ss
// 11 001 101 CALL nnnn
// 11 011 101 PREFIX IX
// 11 101 101 PREFIX extended
// 11 111 101 PREFIX IY
// 11 000 110 ADD A,nn
// 11 001 110 ADC A,nn
// 11 010 110 SUB A,nn
// 11 011 110 SBC A,nn
// 11 100 110 AND A,nn
// 11 101 110 XOR A,nn
// 11 110 110 OR  A,nn
// 11 111 110 CP  A,nn
// 11 aaa 111 RST aaa

// CB 00 000 rrr RLC rr
// CB 00 001 rrr RRC rr
// CB 00 010 rrr RL  rr
// CB 00 011 rrr RR  rr
// CB 00 100 rrr SLA rr
// CB 00 101 rrr SRA rr
// CB 00 111 rrr SRL rr
// CB 01 bbb rrr BIT rr,bbb
// CB 10 bbb rrr RES rr,bbb
// CB 11 bbb rrr SET rr,bbb

// ED 01 rrr 000 IN  r,(C)
// ED 01 rrr 001 OUT (C),r
// ED 01 pp0 010 SBC HL,pp
// ED 01 pp1 010 ADC HL,pp
// ED 01 pp0 011 LD  (nnnn),pp
// ED 01 pp1 011 LD  pp,(nnnn)
// ED 01 xxx 100 NEG*
// ED 01 xx0 101 RETN*
// ED 01 xx1 101 RETI*
// ED 01 x00 110 IM 0
// ED 01 x10 110 IM 1
// ED 01 x11 110 IM 2
// ED 01 00R 111 LD I,A I/R
// ED 01 01R 111 LD A,I I/R
// ED 01 100 111 RRD
// ED 01 101 111 RLD
// ED 10 1ri 000 LDi/LDir
// ED 10 1ri 001 CPi/CPir
// ED 10 1ri 010 INi/INir
// ED 10 1ri 011 OUTi/OTir

// DD 00 pp1 001 ADD IX,pp
// DD 00 100 001 LD  IX,nnnn
// DD 00 100 010 LD  (nnnn),IX
// DD 00 101 010 LD  IX,(nnnn)
// DD 00 HL0 011 INC IX
// DD 00 HL1 011 DEC IX
// DD 00 MMM 100 INC (IX+dd)
// DD 00 MMM 101 DEC (IX+dd)
// DD 00 MMM 110 LD  (IX+dd),nn
// DD 01 ddd MMM LD  r,(IX+dd)
// DD 01 MMM rrr LD  (IX+dd),r
// DD 10 000 MMM ADD A,(IX+dd)
// DD 10 001 MMM ADC A,(IX+dd)
// DD 10 010 MMM SUB A,(IX+dd)
// DD 10 011 MMM SBC A,(IX+dd)
// DD 10 100 MMM AND A,(IX+dd)
// DD 10 101 MMM XOR A,(IX+dd)
// DD 10 110 MMM OR  A,(IX+dd)
// DD 10 111 MMM CP  A,(IX+dd)
// DD 11 001 011 PREFIX bit/IX
// DD 11 HL0 001 POP IX
// DD 11 101 001 JP  (IX)
// DD 11 111 001 LD  SP,IX
// DD 11 100 011 EX  (SP),IX
// DD 11 HL0 101 PUSH IX

// FD 00 pp1 001 ADD IY,pp
// FD 00 100 001 LD  IY,nnnn
// FD 00 100 010 LD  (nnnn),IY
// FD 00 101 010 LD  IY,(nnnn)
// FD 00 HL0 011 INC IY
// FD 00 HL1 011 DEC IY
// FD 00 MMM 100 INC (IY+dd)
// FD 00 MMM 101 DEC (IY+dd)
// FD 00 MMM 110 LD  (IY+dd),nn
// FD 01 ddd MMM LD  r,(IY+dd)
// FD 01 MMM rrr LD  (IY+dd),r
// FD 11 000 MMM ADD A,(IY+dd)
// FD 11 001 MMM ADC A,(IY+dd)
// FD 11 010 MMM SUB A,(IY+dd)
// FD 11 011 MMM SBC A,(IY+dd)
// FD 11 100 MMM AND A,(IY+dd)
// FD 11 101 MMM XOR A,(IY+dd)
// FD 11 110 MMM OR  A,(IY+dd)
// FD 11 111 MMM CP  A,(IY+dd)
// FD 11 001 011 PREFIX bit/IY
// FD 11 HL0 001 POP IY
// FD 11 101 001 JP  IY
// FD 11 111 001 LD  SP,IY
// FD 11 100 011 EX  (SP),IY
// FD 11 HL0 101 PUSH IY

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_cpu);
    RUN_TEST(test_move_inherent);
    RUN_TEST(test_move_immediate);
    RUN_TEST(test_move_direct);
    RUN_TEST(test_stack_op);
    RUN_TEST(test_jump_call);
    RUN_TEST(test_incr_decr);
    RUN_TEST(test_alu_register);
    RUN_TEST(test_alu_immediate);
    RUN_TEST(test_io);
    RUN_TEST(test_inherent);
    RUN_TEST(test_restart);
    RUN_TEST(test_relative);
    RUN_TEST(test_shift);
    RUN_TEST(test_bitop);
    RUN_TEST(test_index_registers);
    RUN_TEST(test_indexed);
    RUN_TEST(test_shift_indexed);
    RUN_TEST(test_bitop_indexed);
    RUN_TEST(test_illegal_i8080);
    RUN_TEST(test_illegal_z80);
    return 0;
}
