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
    return 0;
}
