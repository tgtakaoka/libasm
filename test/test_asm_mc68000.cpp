#include "asm_mc68000.h"
#include "test_asm_helper.h"

TestAsserter asserter;
TestSymtab symtab;
AsmMc68000 as68000;
Assembler<target::uintptr_t> &assembler(as68000);

static void set_up() {
}

static void tear_down() {
    symtab.reset();
}

static void test_cpu() {
    asserter.equals(
        "cpu 68000", true, assembler.acceptCpu("68000"));
    asserter.equals(
        "cpu 68008", true, assembler.acceptCpu("68008"));
}

static void test_inherent() {
    TEST("RESET", 047160);
    TEST("NOP",   047161);
    TEST("STOP #$1234", 047162, 0x1234);
    TEST("RTE",   047163);
    TEST("RTS",   047165);
    TEST("TRAPV", 047166);
    TEST("RTR",   047167);
}

static void test_undefined_symbol() {
    ETEST(UNDEFINED_SYMBOL, "ORI  #UNDEF,CCR", 000074, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "ANDI #UNDEF,SR",  001174, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "MOVE (UNDEF),D1",       031070, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "MOVE #UNDEF,D1",        031074, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "MOVE D1,(UNDEF)",       030701, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "MOVE D2,(UNDEF,A3)",    033502, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "MOVE (UNDEF,A1,D2),D3", 033061, 0x2000);
    ETEST(UNDEFINED_SYMBOL, "MOVEP D1,(UNDEF,A1)",   001611, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "MOVEP (UNDEF,A1),D1",   001411, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "MOVEQ #UNDEF,D1",       071000);
    ETEST(UNDEFINED_SYMBOL, "ADDQ  #UNDEF,D1",       050101);
    ETEST(UNDEFINED_SYMBOL, "ADDQ  #1,(UNDEF,A1)",   051151, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "LSR   #UNDEF,D1",       0160111);
    ETEST(UNDEFINED_SYMBOL, "ROR   (UNDEF,A1)",      0163351, 0x0000);

    EATEST(UNDEFINED_SYMBOL, 0x1000, "BRA UNDEF",      060000|0xFE);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "DBRA  D0,UNDEF", 050710, 0xFFFE);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "MOVEA (UNDEF,PC),A1",      021172, 0xFFFE);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "MOVEA (UNDEF,PC,D1.L),A1", 021173, 0x18FE);
}

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_cpu);
    RUN_TEST(test_inherent);
    RUN_TEST(test_undefined_symbol);
    return 0;
}
