#include "asm_tms9995.h"
#include "test_asm_helper.h"

TestAsserter asserter;
TestSymtab symtab;
AsmTms9995 assembler;

static void set_up() {
}

static void tear_down() {
    symtab.reset();
}

static void test_inh() {
    WTEST("IDLE", 0x0340);
    WTEST("RSET", 0x0360);
    WTEST("RTWP", 0x0380);
    WTEST("CKON", 0x03A0);
    WTEST("CKOF", 0x03C0);
    WTEST("LREX", 0x03E0);
}

static void test_imm() {
    WTEST("LWPI >1234", 0x02E0, 0x1234);
    WTEST("LIMI >89AB", 0x0300, 0x89AB);

    symtab.put(0x1234, "sym1234");
    symtab.put(0x89AB, "sym89AB");

    WTEST("LWPI sym1234", 0x02E0, 0x1234);
    WTEST("LIMI sym89AB", 0x0300, 0x89AB);
}

static void test_reg() {
    WTEST("LST  R0",  0x0080);
    WTEST("LWP  R1",  0x0091);
    WTEST("STWP R14", 0x02AE);
    WTEST("STST R15", 0x02CF);
}

static void test_reg_imm() {
    WTEST("LI   R0,>0000",  0x0200, 0x0000);
    WTEST("AI   R1,1",       0x0221, 0x0001);
    WTEST("ANDI R8,>00FF",  0x0248, 0x00FF);
    WTEST("ORI  R14,>FF00", 0x026E, 0xFF00);
    WTEST("CI   R15,-1",   0x028F, 0xFFFF);

    symtab.put(0x1234, "sym1234");

    WTEST("LI R2,sym1234", 0x0202, 0x1234);
}

static void test_cnt_reg() {
    WTEST("SRA R1,R0", 0x0801);
    WTEST("SRL R4,12", 0x09C4);
    WTEST("SLA R8,4",  0x0A48);
    WTEST("SRC R9,15", 0x0BF9);
}

static void test_src() {
    WTEST("DIVS R2",         0x0182);
    WTEST("DIVS *R3",        0x0193);
    WTEST("DIVS @>1234",     0x01A0, 0x1234);
    WTEST("DIVS @>1000(R4)", 0x01A4, 0x1000);
    WTEST("DIVS *R5+",       0x01B5);
    WTEST("MPYS R0",         0x01C0);
    WTEST("MPYS @2(R8)",     0x01E8, 0x0002);
    WTEST("MPYS *R15+",      0x01FF);

    WTEST("BLWP @>9876", 0x0420, 0x9876);
    WTEST("B    R13",    0x044D);
    WTEST("X    *R10",   0x049A);
    WTEST("CLR  *R12+",  0x04FC);
    WTEST("NEG  R0",     0x0500);
    WTEST("INV  @>1234", 0x0560, 0x1234);
    WTEST("INC  @2(R7)", 0x05A7, 0x0002);
    WTEST("INCT R7",     0x05C7);
    WTEST("DEC  @-2(R7)",0x0627, 0xFFFE);
    WTEST("DECT R7",     0x0647);
    WTEST("BL   R13",    0x068D);
    WTEST("SWPB *R1",    0x06D1);
    WTEST("SETO R12",    0x070C);
    WTEST("ABS @8(R3)",  0x0763, 0x0008);

    symtab.put(-2, "neg2");
    symtab.put(0x1000, "sym1000");
    symtab.put(0x1234, "sym1234");
    symtab.put(0x9876, "sym9876");

    WTEST("DIVS @sym1234",     0x01A0, 0x1234);
    WTEST("DIVS @sym1000(R4)", 0x01A4, 0x1000);
    WTEST("BLWP @sym9876",     0x0420, 0x9876);
    WTEST("DEC  @neg2(R7)",    0x0627, 0xFFFE);
}

static void test_reg_src() {
    WTEST("COC R1,R2",         0x2081);
    WTEST("CZC @>1234(R3),R7", 0x25E3, 0x1234);
    WTEST("XOR @2(R5),R4",     0x2925, 0x0002);
    WTEST("MPY R4,R2",         0x3884);
    WTEST("DIV R14,R12",       0x3F0E);

    symtab.put(0x1234, "sym1234");
    symtab.put(0x0002, "offset2");

    WTEST("CZC @sym1234(R3),R7", 0x25E3, 0x1234);
    WTEST("XOR @offset2(R5),R4", 0x2925, 0x0002);
}

static void test_cnt_src() {
    WTEST("LDCR *R13+,16",  0x303D);
    WTEST("STCR @2(R4),15", 0x37E4, 0x0002);

    symtab.put(7, "size7");
    symtab.put(2, "offset2");

    WTEST("STCR @offset2(R4),size7", 0x35E4, 0x0002);
    WTEST("STCR @offset2(R4),16",    0x3424, 0x0002);
    WTEST("STCR @>1000(R4),size7",   0x35E4, 0x1000);
}

static void test_xop_src() {
    WTEST("XOP @>9876,0",  0x2C20, 0x9876);
    WTEST("XOP @>9876,15", 0x2FE0, 0x9876);

    symtab.put(10, "xop10");
    symtab.put(0x9876, "sym9876");

    WTEST("XOP @sym9876,xop10",   0x2EA0, 0x9876);
    WTEST("XOP @sym9876(R1),8",   0x2E21, 0x9876);
    WTEST("XOP @>1234(R1),xop10", 0x2EA1, 0x1234);
}

static void test_dst_src() {
    WTEST("SZC  @>1234(R10),@>5678(R11)", 0x4AEA, 0x1234, 0x5678);
    WTEST("SZCB @>1234,@>5678",   0x5820, 0x1234, 0x5678);
    WTEST("S    *R10,*R11",       0x66DA);
    WTEST("SB   *R10+,*R11+",     0x7EFA);
    WTEST("C    *R10+,*R10+",     0x8EBA);
    WTEST("CB   *R10+,*R11+",     0x9EFA);
    WTEST("A    @>2000,R11",      0xA2E0, 0x2000);
    WTEST("AB   R10,@>4000(R11)", 0xBACA, 0x4000);
    WTEST("MOV  @0(R10),@1(R11)", 0xCAEA, 0x0000, 0x0001);
    WTEST("MOVB R10,R11",         0xD2CA);
    WTEST("SOC  @>1234,@>5678(R11)", 0xEAE0, 0x1234, 0x5678);
    WTEST("SOCB @>1234(R10),@>5678", 0xF82A, 0x1234, 0x5678);

    symtab.put(0x0000, "zero");
    symtab.put(0x1234, "sym1234");
    symtab.put(0x4000, "sym4000");
    symtab.put(0x5678, "sym5678");

    WTEST("SZC  @sym1234(R10),@sym5678(R11)", 0x4AEA, 0x1234, 0x5678);
    WTEST("SZCB @sym1234,@sym5678",      0x5820, 0x1234, 0x5678);
    WTEST("AB   R10,@sym4000(R11)",      0xBACA, 0x4000);
    WTEST("MOV  @zero(R10),@>0001(R11)", 0xCAEA, 0x0000, 0x0001);
    WTEST("SOC  @sym1234,@sym5678(R11)", 0xEAE0, 0x1234, 0x5678);
    WTEST("SOCB @sym1234(R10),@sym5678", 0xF82A, 0x1234, 0x5678);
}

static void test_rel() {
    AWTEST(0x1000, "JMP >1002", 0x1000);
    AWTEST(0x1000, "JLT >1000", 0x11FF);
    AWTEST(0x1000, "JLE >1100", 0x127F);
    AWTEST(0x1000, "JEQ >0F02", 0x1380);
    AWTEST(0x1000, "JHE >1004", 0x1401);
    AWTEST(0x1000, "JGT >1000", 0x15FF);
    AWTEST(0x1000, "JNE >1008", 0x1603);
    AWTEST(0x1000, "JNC >100A", 0x1704);
    AWTEST(0x1000, "JOC >0FFE", 0x18FE);
    AWTEST(0x1000, "JNO >0FFC", 0x19FD);
    AWTEST(0x1000, "JL  >0FFA", 0x1AFC);
    AWTEST(0x1000, "JH  >0FF8", 0x1BFB);
    AWTEST(0x1000, "JOP >0FF6", 0x1CFA);

    symtab.put(0x0F02, "sym0F02");
    symtab.put(0x1000, "sym1000");
    symtab.put(0x1002, "sym1002");
    symtab.put(0x1100, "sym1100");

    AWTEST(0x1000, "JMP sym1002", 0x1000);
    AWTEST(0x1000, "JLT sym1000", 0x11FF);
    AWTEST(0x1000, "JLE sym1100", 0x127F);
    AWTEST(0x1000, "JEQ sym0F02", 0x1380);
}

static void test_cru_off() {
    WTEST("SBO 0",    0x1D00);
    WTEST("SBZ >7F",  0x1E7F);
    WTEST("TB  -128", 0x1F80);

    symtab.put(0x00, "zero");
    symtab.put(127, "off127");
    symtab.put(-128, "off_128");

    WTEST("SBO zero",   0x1D00);
    WTEST("SBZ off127", 0x1E7F);
    WTEST("TB off_128", 0x1F80);
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
    return 0;
}
