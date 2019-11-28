#include "dis_tms9995.h"
#include "test_dis_helper.h"

TestAsserter asserter;
TestMemory memory;
TestSymtab symtab;
Disassembler disassembler;

static void set_up() {
}

static void tear_down() {
    symtab.reset();
}

static void test_inh() {
    TEST(IDLE, "", 0x03, 0x40);
    TEST(RSET, "", 0x03, 0x60);
    TEST(RTWP, "", 0x03, 0x80);
    TEST(CKON, "", 0x03, 0xA0);
    TEST(CKOF, "", 0x03, 0xC0);
    TEST(LREX, "", 0x03, 0xE0);
}

static void test_imm() {
    TEST(LWPI, ">1234", 0x02, 0xE0, 0x12, 0x34);
    TEST(LIMI, ">89AB", 0x03, 0x00, 0x89, 0xAB);

    symtab.put(0x1234, "sym1234");
    symtab.put(0x89AB, "sym89AB");

    TEST(LWPI, "sym1234", 0x02, 0xE0, 0x12, 0x34);
    TEST(LIMI, "sym89AB", 0x03, 0x00, 0x89, 0xAB);
}

static void test_reg() {
    TEST(LST,  "R0",  0x00, 0x80);
    TEST(LWP,  "R1",  0x00, 0x91);
    TEST(STWP, "R14", 0x02, 0xAE);
    TEST(STST, "R15", 0x02, 0xCF);
}

static void test_reg_imm() {
    TEST(LI,   "R0,>0000",  0x02, 0x00, 0x00, 0x00);
    TEST(AI,   "R1,>0001",  0x02, 0x21, 0x00, 0x01);
    TEST(ANDI, "R8,>00FF",  0x02, 0x48, 0x00, 0xFF);
    TEST(ORI,  "R14,>FF00", 0x02, 0x6E, 0xFF, 0x00);
    TEST(CI,   "R15,>FFFF", 0x02, 0x8F, 0xFF, 0xFF);

    symtab.put(0x1234, "sym1234");

    TEST(LI,   "R2,sym1234", 0x02, 0x02, 0x12, 0x34);
}

static void test_cnt_reg() {
    TEST(SRA,  "R1,R0",  0x08, 0x01);
    TEST(SRL,  "R4,12",  0x09, 0xC4);
    TEST(SLA,  "R8,4",   0x0A, 0x48);
    TEST(SRC,  "R9,15",  0x0B, 0xF9);
}

static void test_src() {
    TEST(DIVS, "R2",     0x01, 0x82);
    TEST(DIVS, "*R3",    0x01, 0x93);
    TEST(DIVS, "@>1234", 0x01, 0xA0, 0x12, 0x34);
    TEST(DIVS, "@>1000(R4)", 0x01, 0xA4, 0x10, 0x00);
    TEST(DIVS, "*R5+",   0x01, 0xB5);
    TEST(MPYS, "R0",     0x01, 0xC0);
    TEST(MPYS, "@>0002(R8)", 0x01, 0xE8, 0x00, 0x02);
    TEST(MPYS, "*R15+",  0x01, 0xFF);

    TEST(BLWP, "@>9876", 0x04, 0x20, 0x98, 0x76);
    TEST(B,    "R13",    0x04, 0x4D);
    TEST(X,    "*R10",   0x04, 0x9A);
    TEST(CLR,  "*R12+",  0x04, 0xFC);
    TEST(NEG,  "R0",     0x05, 0x00);
    TEST(INV,  "@>1234", 0x05, 0x60, 0x12, 0x34);
    TEST(INC,  "@>0002(R7)", 0x05, 0xA7, 0x00, 0x02);
    TEST(INCT, "R7",     0x05, 0xC7);
    TEST(DEC,  "@>FFFE(R7)",0x06, 0x27, 0xFF, 0xFE);
    TEST(DECT, "R7",     0x06, 0x47);
    TEST(BL,   "R13",    0x06, 0x8D);
    TEST(SWPB, "*R1",    0x06, 0xD1);
    TEST(SETO, "R12",    0x07, 0x0C);
    TEST(ABS,  "@>0008(R3)", 0x07, 0x63, 0x00, 0x08);

    symtab.put(-2, "neg2");
    symtab.put(0x1000, "sym1000");
    symtab.put(0x1234, "sym1234");
    symtab.put(0x9876, "sym9876");

    TEST(DIVS, "@sym1234",     0x01, 0xA0, 0x12, 0x34);
    TEST(DIVS, "@sym1000(R4)", 0x01, 0xA4, 0x10, 0x00);
    TEST(BLWP, "@sym9876",     0x04, 0x20, 0x98, 0x76);
    TEST(DEC,  "@neg2(R7)",    0x06, 0x27, 0xFF, 0xFE);
}

static void test_reg_src() {
    TEST(COC, "R1,R2",         0x20, 0x81);
    TEST(CZC, "@>1234(R3),R7", 0x25, 0xE3, 0x12, 0x34);
    TEST(XOR, "@>0002(R5),R4", 0x29, 0x25, 0x00, 0x02);
    TEST(MPY, "R4,R2",         0x38, 0x84);
    TEST(DIV, "R14,R12",       0x3F, 0x0E);

    symtab.put(0x1234, "sym1234");
    symtab.put(0x0002, "offset2");

    TEST(CZC, "@sym1234(R3),R7", 0x25, 0xE3, 0x12, 0x34);
    TEST(XOR, "@offset2(R5),R4", 0x29, 0x25, 0x00, 0x02);
}

static void test_cnt_src() {
    TEST(LDCR, "*R13+,16",      0x30, 0x3D);
    TEST(STCR, "@>0002(R4),15", 0x37, 0xE4, 0x00, 0x02);

    symtab.put(7, "size7");
    symtab.put(2, "offset2");

    TEST(STCR, "@offset2(R4),size7", 0x35, 0xE4, 0x00, 0x02);
    TEST(STCR, "@offset2(R4),16",    0x34, 0x24, 0x00, 0x02);
    TEST(STCR, "@>1000(R4),size7",   0x35, 0xE4, 0x10, 0x00);
}

static void test_xop_src() {
    TEST(XOP,  "@>9876,0",  0x2C, 0x20, 0x98, 0x76);
    TEST(XOP,  "@>9876,15", 0x2F, 0xE0, 0x98, 0x76);

    symtab.put(10, "xop10");
    symtab.put(0x9876, "sym9876");

    TEST(XOP,  "@sym9876,xop10",   0x2E, 0xA0, 0x98, 0x76);
    TEST(XOP,  "@sym9876(R1),8",   0x2E, 0x21, 0x98, 0x76);
    TEST(XOP,  "@>1234(R1),xop10", 0x2E, 0xA1, 0x12, 0x34);
}

static void test_dst_src() {
    TEST(SZC,  "@>1234(R10),@>5678(R11)", 0x4A, 0xEA, 0x12, 0x34, 0x56, 0x78);
    TEST(SZCB, "@>1234,@>5678",           0x58, 0x20, 0x12, 0x34, 0x56, 0x78);
    TEST(S,    "*R10,*R11",               0x66, 0xDA);
    TEST(SB,   "*R10+,*R11+",             0x7E, 0xFA);
    TEST(C,    "*R10+,*R10+",             0x8E, 0xBA);
    TEST(CB,   "*R10+,*R11+",             0x9E, 0xFA);
    TEST(A,    "@>2000,R11",              0xA2, 0xE0, 0x20, 0x00);
    TEST(AB,   "R10,@>4000(R11)",         0xBA, 0xCA, 0x40, 0x00);
    TEST(MOV,  "@>0000(R10),@>0001(R11)", 0xCA, 0xEA, 0x00, 0x00, 0x00, 0x01);
    TEST(MOVB, "R10,R11",                 0xD2, 0xCA);
    TEST(SOC,  "@>1234,@>5678(R11)",      0xEA, 0xE0, 0x12, 0x34, 0x56, 0x78);
    TEST(SOCB, "@>1234(R10),@>5678",      0xF8, 0x2A, 0x12, 0x34, 0x56, 0x78);

    symtab.put(0x0000, "zero");
    symtab.put(0x1234, "sym1234");
    symtab.put(0x4000, "sym4000");
    symtab.put(0x5678, "sym5678");

    TEST(SZC,  "@sym1234(R10),@sym5678(R11)", 0x4A, 0xEA, 0x12, 0x34, 0x56, 0x78);
    TEST(SZCB, "@sym1234,@sym5678",           0x58, 0x20, 0x12, 0x34, 0x56, 0x78);
    TEST(AB,   "R10,@sym4000(R11)",           0xBA, 0xCA, 0x40, 0x00);
    TEST(MOV,  "@zero(R10),@>0001(R11)",      0xCA, 0xEA, 0x00, 0x00, 0x00, 0x01);
    TEST(SOC,  "@sym1234,@sym5678(R11)",      0xEA, 0xE0, 0x12, 0x34, 0x56, 0x78);
    TEST(SOCB, "@sym1234(R10),@sym5678",      0xF8, 0x2A, 0x12, 0x34, 0x56, 0x78);
}

static void test_rel() {
    ATEST(0x1000, JMP, ">1002", 0x10, 0x00);
    ATEST(0x1000, JLT, ">1000", 0x11, 0xFF);
    ATEST(0x1000, JLE, ">1100", 0x12, 0x7F);
    ATEST(0x1000, JEQ, ">0F02", 0x13, 0x80);
    ATEST(0x1000, JHE, ">1004", 0x14, 0x01);
    ATEST(0x1000, JGT, ">1006", 0x15, 0x02);
    ATEST(0x1000, JNE, ">1008", 0x16, 0x03);
    ATEST(0x1000, JNC, ">100A", 0x17, 0x04);
    ATEST(0x1000, JOC, ">0FFE", 0x18, 0xFE);
    ATEST(0x1000, JNO, ">0FFC", 0x19, 0xFD);
    ATEST(0x1000, JL,  ">0FFA", 0x1A, 0xFC);
    ATEST(0x1000, JH,  ">0FF8", 0x1B, 0xFB);
    ATEST(0x1000, JOP, ">0FF6", 0x1C, 0xFA);

    symtab.put(0x0F02, "sym0F02");
    symtab.put(0x1000, "sym1000");
    symtab.put(0x1002, "sym1002");
    symtab.put(0x1100, "sym1100");

    ATEST(0x1000, JMP, "sym1002", 0x10, 0x00);
    ATEST(0x1000, JLT, "sym1000", 0x11, 0xFF);
    ATEST(0x1000, JLE, "sym1100", 0x12, 0x7F);
    ATEST(0x1000, JEQ, "sym0F02", 0x13, 0x80);
}

static void test_cru_off() {
    TEST(SBO, ">00", 0x1D, 0x00);
    TEST(SBZ, ">7F", 0x1E, 0x7F);
    TEST(TB,  ">80", 0x1F, 0x80);

    symtab.put(0x00, "zero");
    symtab.put(127, "off127");
    symtab.put(-128, "off_128");

    TEST(SBO, "zero",    0x1D, 0x00);
    TEST(SBZ, "off127",  0x1E, 0x7F);
    TEST(TB,  "off_128", 0x1F, 0x80);
}

// Macro Instruction Detect
static void test_mid() {
    static const struct mid_range {
        uint16_t start;
        uint16_t end;
    } mids[] = {
        { 0x0000, 0x007f }, { 0x00A0, 0x017f },
        { 0x0210, 0x021f }, { 0x0230, 0x023f }, { 0x0250, 0x025f }, { 0x0270, 0x027f },
        { 0x0290, 0x029f }, { 0x02b0, 0x02bf }, { 0x02d0, 0x02df }, { 0x02e1, 0x02ff },
        { 0x0301, 0x033f }, { 0x0341, 0x035f }, { 0x0361, 0x037f }, { 0x0381, 0x039f },
        { 0x03a1, 0x03bf }, { 0x03c1, 0x03df }, { 0x03e1, 0x03ff },
        { 0x0780, 0x07ff }, { 0x0c00, 0x0fff }
    };
    uint8_t bytes[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    Insn insn;
    char operands[40], message[40];
    const mid_range *m = &mids[0];
    for (uint16_t h = 0; h < 0x100; h++) {
        for (uint16_t l = 0; l < 0x100; l++) {
            const uint16_t i = (h << 8) | l;
            memory.setAddress(0x1000);
            bytes[0] = h; bytes[1] = l;
            memory.setBytes(bytes, sizeof(bytes));
            disassembler.decode(memory, insn, operands, nullptr);
            if (m && i > m->end) {
                if (++m >= &mids[sizeof(mids)/sizeof(mids[0])])
                    m = nullptr;
            }
            if (m && i >= m->start && i <= m->end) {
                sprintf(message, "%s: >%04X is MID", __FUNCTION__, i);
                asserter.equals(message, OK, disassembler.getError());
                asserter.equals(message, "MID", insn.name());
                asserter.equals(message, i, insn.insnCode());
                asserter.equals(message, 2, insn.insnLen());
            } else {
                sprintf(message, "%s: >%04X is not MID", __FUNCTION__, i);
                asserter.equals(message, OK, disassembler.getError());
                asserter.not_equals(message, "MID", insn.name());
            }
        }
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
    RUN_TEST(test_inh);
    RUN_TEST(test_imm);
    RUN_TEST(test_reg);
    RUN_TEST(test_reg_imm);
    RUN_TEST(test_cnt_reg);
    RUN_TEST(test_src);
    RUN_TEST(test_reg_src);
    RUN_TEST(test_cnt_src);
    RUN_TEST(test_xop_src);
    RUN_TEST(test_dst_src);
    RUN_TEST(test_rel);
    RUN_TEST(test_cru_off);
    RUN_TEST(test_mid);
    return 0;
}
