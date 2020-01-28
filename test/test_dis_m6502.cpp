#include "dis_m6502.h"
#include "test_dis_helper.h"

TestAsserter asserter;
TestMemory memory;
TestSymtab symtab;
DisM6502 dis6502;
Disassembler<target::uintptr_t> &disassembler(dis6502);

static void set_up() {
    disassembler.acceptCpu("6502");
}

static void tear_down() {
    symtab.reset();
}

static void test_implied() {
    TEST(BRK, "", 0x00);
    TEST(RTI, "", 0x40);
    TEST(RTS, "", 0x60);
    TEST(NOP, "", 0xEA);

    TEST(PHP, "", 0x08);
    TEST(PLP, "", 0x28);
    TEST(PHA, "", 0x48);
    TEST(PLA, "", 0x68);

    TEST(DEY, "", 0x88);
    TEST(INY, "", 0xC8);
    TEST(DEX, "", 0xCA);
    TEST(INX, "", 0xE8);

    TEST(TYA, "", 0x98);
    TEST(TAY, "", 0xA8);
    TEST(TXA, "", 0x8A);
    TEST(TAX, "", 0xAA);
    TEST(TXS, "", 0x9A);
    TEST(TSX, "", 0xBA);

    TEST(CLC, "", 0x18);
    TEST(SEC, "", 0x38);
    TEST(CLI, "", 0x58);
    TEST(SEI, "", 0x78);
    TEST(CLV, "", 0xB8);
    TEST(CLD, "", 0xD8);
    TEST(SED, "", 0xF8);

    // W65C02
    disassembler.acceptCpu("65C02");
    TEST(PHY, "", 0x5A);
    TEST(PLY, "", 0x7A);
    TEST(PHX, "", 0xDA);
    TEST(PLX, "", 0xFA);
}

static void test_accumulator() {
    TEST(ASL, "A", 0x0A);
    TEST(ROL, "A", 0x2A);
    TEST(LSR, "A", 0x4A);
    TEST(ROR, "A", 0x6A);

    // W65C02
    disassembler.acceptCpu("65C02");
    TEST(INC, "A", 0x1A);
    TEST(DEC, "A", 0x3A);
}

static void test_immediate() {
    TEST(LDY, "#0",   0xA0, 0x00);
    TEST(LDX, "#9",   0xA2, 0x09);
    TEST(CPY, "#10",  0xC0, 0x0A);
    TEST(CPX, "#$FF", 0xE0, 0xFF);

    TEST(ORA, "#$90", 0x09, 0x90);
    TEST(AND, "#$90", 0x29, 0x90);
    TEST(EOR, "#$90", 0x49, 0x90);
    TEST(ADC, "#$90", 0x69, 0x90);
    TEST(LDA, "#$90", 0xA9, 0x90);
    TEST(CMP, "#$90", 0xC9, 0x90);
    TEST(SBC, "#$90", 0xE9, 0x90);

    // W65C02
    disassembler.acceptCpu("65C02");
    TEST(BIT, "#$90", 0x89, 0x90);

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");
    symtab.intern(0x0090, "zero90");
    
    disassembler.acceptCpu("6502");
    TEST(LDX, "#zero10", 0xA2, 0x10);
    TEST(CPY, "#zeroFF", 0xC0, 0xFF);
    TEST(SBC, "#zero90", 0xE9, 0x90);

    // W65C02
    disassembler.acceptCpu("65C02");
    TEST(BIT, "#zero90", 0x89, 0x90);
}

static void test_zeropage() {
    TEST(BIT, "$10", 0x24, 0x10);
    TEST(ORA, "$10", 0x05, 0x10);
    TEST(AND, "$10", 0x25, 0x10);
    TEST(EOR, "$10", 0x45, 0x10);
    TEST(ADC, "$10", 0x65, 0x10);
    TEST(STA, "$10", 0x85, 0x10);
    TEST(LDA, "$10", 0xA5, 0x10);
    TEST(CMP, "$10", 0xC5, 0x10);
    TEST(SBC, "$10", 0xE5, 0x10);

    TEST(STY, "$10", 0x84, 0x10);
    TEST(LDY, "$10", 0xA4, 0x10);
    TEST(CPY, "$10", 0xC4, 0x10);
    TEST(STX, "$10", 0x86, 0x10);
    TEST(LDX, "$10", 0xA6, 0x10);
    TEST(CPX, "$10", 0xE4, 0x10);

    TEST(DEC, "$10", 0xC6, 0x10);
    TEST(INC, "$10", 0xE6, 0x10);

    TEST(ASL, "$10", 0x06, 0x10);
    TEST(ROL, "$10", 0x26, 0x10);
    TEST(LSR, "$10", 0x46, 0x10);
    TEST(ROR, "$10", 0x66, 0x10);

    // W65C02
    disassembler.acceptCpu("65C02");
    TEST(TSB, "$10", 0x04, 0x10);
    TEST(TRB, "$10", 0x14, 0x10);
    TEST(STZ, "$10", 0x64, 0x10);

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");
    symtab.intern(0x0090, "zero90");
    
    disassembler.acceptCpu("6502");
    TEST(SBC, "zero10", 0xE5, 0x10);
    TEST(DEC, "zeroFF", 0xC6, 0xFF);
    TEST(ROR, "zero90", 0x66, 0x90);

    // W65C02
    disassembler.acceptCpu("65C02");
    TEST(STZ, "zero10", 0x64, 0x10);
}

static void test_zeropage_indexed() {
    TEST(ORA, "$00,X", 0x15, 0x00);
    TEST(AND, "$09,X", 0x35, 0x09);
    TEST(EOR, "$0A,X", 0x55, 0x0A);
    TEST(ADC, "$10,X", 0x75, 0x10);
    TEST(STA, "$10,X", 0x95, 0x10);
    TEST(LDA, "$10,X", 0xB5, 0x10);
    TEST(CMP, "$10,X", 0xD5, 0x10);
    TEST(SBC, "$10,X", 0xF5, 0x10);

    TEST(STY, "$10,X", 0x94, 0x10);
    TEST(LDY, "$10,X", 0xB4, 0x10);
    TEST(STX, "$10,Y", 0x96, 0x10);
    TEST(LDX, "$10,Y", 0xB6, 0x10);

    TEST(DEC, "$10,X", 0xD6, 0x10);
    TEST(INC, "$10,X", 0xF6, 0x10);

    TEST(ASL, "$10,X", 0x16, 0x10);
    TEST(ROL, "$10,X", 0x36, 0x10);
    TEST(LSR, "$10,X", 0x56, 0x10);
    TEST(ROR, "$10,X", 0x76, 0x10);

    // W65C02
    disassembler.acceptCpu("65C02");
    TEST(BIT, "$10,X", 0x34, 0x10);
    TEST(STZ, "$10,X", 0x74, 0x10);

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");
    symtab.intern(0x0090, "zero90");
    
    disassembler.acceptCpu("6502");
    TEST(SBC, "zero10,X", 0xF5, 0x10);
    TEST(STY, "zero90,X", 0x94, 0x90);
    TEST(LDY, "zeroFF,X", 0xB4, 0xFF);
    TEST(STX, "zero90,Y", 0x96, 0x90);
    TEST(LDX, "zeroFF,Y", 0xB6, 0xFF);
    TEST(ROR, "zero90,X", 0x76, 0x90);

    // W65C02
    disassembler.acceptCpu("65C02");
    TEST(BIT, "zero10,X", 0x34, 0x10);
}

static void test_absolute() {
    TEST(BIT, "$1234", 0x2C, 0x34, 0x12);
    TEST(ORA, "$1234", 0x0D, 0x34, 0x12);
    TEST(AND, "$1234", 0x2D, 0x34, 0x12);
    TEST(EOR, "$1234", 0x4D, 0x34, 0x12);
    TEST(ADC, "$1234", 0x6D, 0x34, 0x12);
    TEST(STA, "$1234", 0x8D, 0x34, 0x12);
    TEST(LDA, "$1234", 0xAD, 0x34, 0x12);
    TEST(CMP, "$1234", 0xCD, 0x34, 0x12);
    TEST(SBC, "$1234", 0xED, 0x34, 0x12);

    TEST(STY, "$1234", 0x8C, 0x34, 0x12);
    TEST(LDY, "$1234", 0xAC, 0x34, 0x12);
    TEST(CPY, "$1234", 0xCC, 0x34, 0x12);
    TEST(STX, "$1234", 0x8E, 0x34, 0x12);
    TEST(LDX, "$1234", 0xAE, 0x34, 0x12);
    TEST(CPX, "$1234", 0xEC, 0x34, 0x12);

    TEST(DEC, "$1234", 0xCE, 0x34, 0x12);
    TEST(INC, "$1234", 0xEE, 0x34, 0x12);

    TEST(ASL, "$1234", 0x0E, 0x34, 0x12);
    TEST(ROL, "$1234", 0x2E, 0x34, 0x12);
    TEST(LSR, "$1234", 0x4E, 0x34, 0x12);
    TEST(ROR, "$1234", 0x6E, 0x34, 0x12);

    TEST(JMP, "$1234", 0x4C, 0x34, 0x12);
    TEST(JSR, "$1234", 0x20, 0x34, 0x12);

    // W65C02
    disassembler.acceptCpu("65C02");
    TEST(TSB, "$1234", 0x0C, 0x34, 0x12);
    TEST(TRB, "$1234", 0x1C, 0x34, 0x12);
    TEST(STZ, "$1234", 0x9C, 0x34, 0x12);

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");
    symtab.intern(0x0100, "abs0100");
    
    disassembler.acceptCpu("6502");
    TEST(SBC, ">abs0010", 0xED, 0x10, 0x00);
    TEST(LDX, "abs1234",  0xAE, 0x34, 0x12);
    TEST(JSR, "abs0100",  0x20, 0x00, 0x01);

    // W65C02
    disassembler.acceptCpu("65C02");
    TEST(TSB, "abs1234",  0x0C, 0x34, 0x12);
}

static void test_absolute_indexed() {
    TEST(ORA, "$0000,X", 0x1D, 0x00, 0x00);
    TEST(AND, "$0009,X", 0x3D, 0x09, 0x00);
    TEST(EOR, "$0034,X", 0x5D, 0x34, 0x00);
    TEST(ADC, "$1234,X", 0x7D, 0x34, 0x12);
    TEST(STA, "$1234,X", 0x9D, 0x34, 0x12);
    TEST(LDA, "$1234,X", 0xBD, 0x34, 0x12);
    TEST(CMP, "$1234,X", 0xDD, 0x34, 0x12);
    TEST(SBC, "$1234,X", 0xFD, 0x34, 0x12);

    TEST(ORA, "$1234,Y", 0x19, 0x34, 0x12);
    TEST(AND, "$1234,Y", 0x39, 0x34, 0x12);
    TEST(EOR, "$0000,Y", 0x59, 0x00, 0x00);
    TEST(ADC, "$0009,Y", 0x79, 0x09, 0x00);
    TEST(STA, "$0034,Y", 0x99, 0x34, 0x00);
    TEST(LDA, "$1234,Y", 0xB9, 0x34, 0x12);
    TEST(CMP, "$1234,Y", 0xD9, 0x34, 0x12);
    TEST(SBC, "$1234,Y", 0xF9, 0x34, 0x12);

    TEST(LDY, "$1234,X", 0xBC, 0x34, 0x12);
    TEST(LDX, "$1234,Y", 0xBE, 0x34, 0x12);

    TEST(DEC, "$1234,X", 0xDE, 0x34, 0x12);
    TEST(INC, "$1234,X", 0xFE, 0x34, 0x12);

    TEST(ASL, "$1234,X", 0x1E, 0x34, 0x12);
    TEST(ROL, "$1234,X", 0x3E, 0x34, 0x12);
    TEST(LSR, "$1234,X", 0x5E, 0x34, 0x12);
    TEST(ROR, "$1234,X", 0x7E, 0x34, 0x12);

    // W65C02
    disassembler.acceptCpu("65C02");
    TEST(BIT, "$1234,X", 0x3C, 0x34, 0x12);
    TEST(STZ, "$1234,X", 0x9E, 0x34, 0x12);

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");
    symtab.intern(0x0100, "abs0100");
    
    disassembler.acceptCpu("6502");
    TEST(SBC, ">abs0010,X", 0xFD, 0x10, 0x00);
    TEST(STA, "abs1234,Y",  0x99, 0x34, 0x12);
    TEST(LDY, "abs0100,X",  0xBC, 0x00, 0x01);
    TEST(LDX, "abs1234,Y",  0xBE, 0x34, 0x12);
    TEST(LSR, "abs0100,X",  0x5E, 0x00, 0x01);

    // W65C02
    disassembler.acceptCpu("65C02");
    TEST(STZ, "abs1234,X",  0x9E, 0x34, 0x12);
}

static void test_absolute_indirect() {
    TEST(JMP, "($0009)", 0x6C, 0x09, 0x00);
    TEST(JMP, "($1234)", 0x6C, 0x34, 0x12);

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");

    TEST(JMP, "(>abs0010)", 0x6C, 0x10, 0x00);
    TEST(JMP, "(abs1234)",  0x6C, 0x34, 0x12);
}

static void test_indexed_absolute_indirect() {
    // W65C02
    disassembler.acceptCpu("65C02");
    TEST(JMP, "($0009,X)", 0x7C, 0x09, 0x00);
    TEST(JMP, "($1234,X)", 0x7C, 0x34, 0x12);

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");

    // W65C02
    TEST(JMP, "(>abs0010,X)", 0x7C, 0x10, 0x00);
    TEST(JMP, "(abs1234,X)",  0x7C, 0x34, 0x12);
}

static void test_zeropage_indirect() {
    // W65C02
    disassembler.acceptCpu("65C02");
    TEST(ORA, "($00)", 0x12, 0x00);
    TEST(AND, "($09)", 0x32, 0x09);
    TEST(EOR, "($10)", 0x52, 0x10);
    TEST(ADC, "($10)", 0x72, 0x10);
    TEST(STA, "($10)", 0x92, 0x10);
    TEST(LDA, "($10)", 0xB2, 0x10);
    TEST(CMP, "($10)", 0xD2, 0x10);
    TEST(SBC, "($10)", 0xF2, 0x10);

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");

    TEST(ORA, "(zero10)", 0x12, 0x10);
    TEST(AND, "(zeroFF)", 0x32, 0xFF);
}

static void test_indexed_indirect() {
    TEST(ORA, "($00,X)", 0x01, 0x00);
    TEST(AND, "($09,X)", 0x21, 0x09);
    TEST(EOR, "($10,X)", 0x41, 0x10);
    TEST(ADC, "($10,X)", 0x61, 0x10);
    TEST(STA, "($10,X)", 0x81, 0x10);
    TEST(LDA, "($10,X)", 0xA1, 0x10);
    TEST(CMP, "($10,X)", 0xC1, 0x10);
    TEST(SBC, "($10,X)", 0xE1, 0x10);

    symtab.intern(0x0010, "zero10");

    TEST(LDA, "(zero10,X)", 0xA1, 0x10);
}

static void test_indirect_indexed() {
    TEST(ORA, "($00),Y", 0x11, 0x00);
    TEST(AND, "($09),Y", 0x31, 0x09);
    TEST(EOR, "($10),Y", 0x51, 0x10);
    TEST(ADC, "($10),Y", 0x71, 0x10);
    TEST(STA, "($10),Y", 0x91, 0x10);
    TEST(LDA, "($10),Y", 0xB1, 0x10);
    TEST(CMP, "($10),Y", 0xD1, 0x10);
    TEST(SBC, "($10),Y", 0xF1, 0x10);

    symtab.intern(0x0010, "zero10");

    TEST(LDA, "(zero10),Y", 0xB1, 0x10);
}

static void test_relative() {
    ATEST(0x1000, BPL, "$1002", 0x10, 0x00);
    ATEST(0x1000, BMI, "$1000", 0x30, 0xFE);
    ATEST(0x1000, BVC, "$1004", 0x50, 0x02);
    ATEST(0x1000, BVS, "$1081", 0x70, 0x7F);
    ATEST(0x1000, BCC, "$0F82", 0x90, 0x80);
    ATEST(0x1000, BCS, "$1002", 0xB0, 0x00);
    ATEST(0x1000, BNE, "$1002", 0xD0, 0x00);
    ATEST(0x1000, BEQ, "$1002", 0xF0, 0x00);

    // W65C02
    disassembler.acceptCpu("65C02");
    ATEST(0x1000, BRA, "$1002", 0x80, 0x00);

    symtab.intern(0x0F82, "label0F82");
    symtab.intern(0x1000, "label1000");
    symtab.intern(0x1081, "label1081");

    disassembler.acceptCpu("6502");
    ATEST(0x1000, BMI, "label1000", 0x30, 0xFE);
    ATEST(0x1000, BVS, "label1081", 0x70, 0x7F);
    ATEST(0x1000, BCC, "label0F82", 0x90, 0x80);

    // W65C02
    disassembler.acceptCpu("65C02");
    ATEST(0x1000, BRA, "label1000", 0x80, 0xFE);
}

#ifdef W65C02_ENABLE_BITOPS
static void test_bit_manipulation() {
    // W65C02
    disassembler.acceptCpu("65C02");
    TEST(RMB0, "$10", 0x07, 0x10);
    TEST(RMB1, "$10", 0x17, 0x10);
    TEST(RMB2, "$10", 0x27, 0x10);
    TEST(RMB3, "$10", 0x37, 0x10);
    TEST(RMB4, "$10", 0x47, 0x10);
    TEST(RMB5, "$10", 0x57, 0x10);
    TEST(RMB6, "$10", 0x67, 0x10);
    TEST(RMB7, "$10", 0x77, 0x10);
    TEST(SMB0, "$10", 0x87, 0x10);
    TEST(SMB1, "$10", 0x97, 0x10);
    TEST(SMB2, "$10", 0xA7, 0x10);
    TEST(SMB3, "$10", 0xB7, 0x10);
    TEST(SMB4, "$10", 0xC7, 0x10);
    TEST(SMB5, "$10", 0xD7, 0x10);
    TEST(SMB6, "$10", 0xE7, 0x10);
    TEST(SMB7, "$10", 0xF7, 0x10);

    symtab.intern(0x0010, "zero10");

    TEST(RMB7, "zero10", 0x77, 0x10);
    TEST(SMB0, "zero10", 0x87, 0x10);
}

static void test_zeropage_relative() {
    // W65C02
    disassembler.acceptCpu("65C02");
    ATEST(0x1000, BBR0, "$10,$1003", 0x0F, 0x10, 0x00);
    ATEST(0x1000, BBR1, "$10,$1000", 0x1F, 0x10, 0xFD);
    ATEST(0x1000, BBR2, "$10,$1006", 0x2F, 0x10, 0x03);
    ATEST(0x1000, BBR3, "$10,$1082", 0x3F, 0x10, 0x7F);
    ATEST(0x1000, BBR4, "$10,$0F83", 0x4F, 0x10, 0x80);
    ATEST(0x1000, BBR5, "$10,$1003", 0x5F, 0x10, 0x00);
    ATEST(0x1000, BBR6, "$10,$1003", 0x6F, 0x10, 0x00);
    ATEST(0x1000, BBR7, "$10,$1003", 0x7F, 0x10, 0x00);
    ATEST(0x1000, BBS0, "$10,$1003", 0x8F, 0x10, 0x00);
    ATEST(0x1000, BBS1, "$10,$1000", 0x9F, 0x10, 0xFD);
    ATEST(0x1000, BBS2, "$10,$1006", 0xAF, 0x10, 0x03);
    ATEST(0x1000, BBS3, "$10,$1082", 0xBF, 0x10, 0x7F);
    ATEST(0x1000, BBS4, "$10,$0F83", 0xCF, 0x10, 0x80);
    ATEST(0x1000, BBS5, "$10,$1003", 0xDF, 0x10, 0x00);
    ATEST(0x1000, BBS6, "$10,$1003", 0xEF, 0x10, 0x00);
    ATEST(0x1000, BBS7, "$10,$1003", 0xFF, 0x10, 0x00);

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x0F83, "label0F83");
    symtab.intern(0x1000, "label1000");
    symtab.intern(0x1003, "label1003");
    symtab.intern(0x1082, "label1082");

    ATEST(0x1000, BBR3, "zero10,label1082", 0x3F, 0x10, 0x7F);
    ATEST(0x1000, BBR4, "zero10,label0F83", 0x4F, 0x10, 0x80);
    ATEST(0x1000, BBS4, "zero10,label1003", 0xCF, 0x10, 0x00);
    ATEST(0x1000, BBS5, "zero10,label1000", 0xDF, 0x10, 0xFD);
}
#endif

static void test_illegal_m6502() {
    disassembler.acceptCpu("6502");

    Insn insn;
    char operands[40];

    const uint8_t illegals[] = {
        0x80,
        0x02, 0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x72, 0x82, 0x92, 0xB2, 0xC2, 0xD2, 0xE2, 0xF2,
        0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73, 0x83, 0x93, 0xA3, 0xB3, 0xC3, 0xD3, 0xE3, 0xF3,
        0x04, 0x14, 0x34, 0x44, 0x54, 0x64, 0x74, 0xD4, 0xF4,
        0x07, 0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77, 0x87, 0x97, 0xA7, 0xB7, 0xC7, 0xD7, 0xE7, 0xF7,
        0x89,
        0x1A, 0x3A, 0x5A, 0x7A, 0xDA, 0xFA,
        0x0B, 0x1B, 0x2B, 0x3B, 0x4B, 0x5B, 0x6B, 0xBB, 0x8B, 0x9B, 0xAB, 0xBB, 0xCB, 0xDB, 0xEB, 0xFB,
        0x0C, 0x1C, 0x3C, 0x5C, 0x7C, 0x9C, 0xDC, 0xFC,
        0x9E,
        0x0F, 0x1F, 0x2F, 0x3F, 0x4F, 0x5F, 0x6F, 0xBF, 0x8F, 0x9F, 0xAF, 0xBF, 0xCF, 0xDF, 0xEF, 0xFF,
    };
    for (uint8_t idx = 0; idx < sizeof(illegals); idx++) {
        memory.setBytes(&illegals[idx], 1);
        disassembler.decode(memory, insn, operands, nullptr);
        char message[40];
        sprintf(message, "%s opecode 0x%02" PRIX8, __FUNCTION__, illegals[idx]);
        asserter.equals(message, UNKNOWN_INSTRUCTION, disassembler.getError());
    }
}

static void test_illegal_w65c02() {
    disassembler.acceptCpu("65C02");

    Insn insn;
    char operands[40];

    const uint8_t illegals[] = {
        0x02, 0x22, 0x42, 0x62, 0x82, 0xC2, 0xE2,
        0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73, 0x83, 0x93, 0xA3, 0xB3, 0xC3, 0xD3, 0xE3, 0xF3,
        0x44, 0x54, 0xD4, 0xF4,
        0x0B, 0x1B, 0x2B, 0x3B, 0x4B, 0x5B, 0x6B, 0xBB, 0x8B, 0x9B, 0xAB, 0xBB, 0xCB, 0xDB, 0xEB, 0xFB,
        0x5C, 0xDC, 0xFC,
    };
    for (uint8_t idx = 0; idx < sizeof(illegals); idx++) {
        memory.setBytes(&illegals[idx], 1);
        disassembler.decode(memory, insn, operands, nullptr);
        char message[40];
        sprintf(message, "%s opecode 0x%02" PRIX8, __FUNCTION__, illegals[idx]);
        asserter.equals(message, UNKNOWN_INSTRUCTION, disassembler.getError());
    }
}

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_implied);
    RUN_TEST(test_accumulator);
    RUN_TEST(test_immediate);
    RUN_TEST(test_zeropage);
    RUN_TEST(test_zeropage_indexed);
    RUN_TEST(test_absolute);
    RUN_TEST(test_absolute_indexed);
    RUN_TEST(test_absolute_indirect);
    RUN_TEST(test_zeropage_indirect);
    RUN_TEST(test_indexed_absolute_indirect);
    RUN_TEST(test_indexed_indirect);
    RUN_TEST(test_indirect_indexed);
    RUN_TEST(test_relative);
#ifdef W65C02_ENABLE_BITOPS
    RUN_TEST(test_bit_manipulation);
    RUN_TEST(test_zeropage_relative);
#endif
    RUN_TEST(test_illegal_m6502);
    RUN_TEST(test_illegal_w65c02);
    return 0;
}
