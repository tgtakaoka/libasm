#include "dis_mc68000.h"
#include "test_dis_helper.h"

TestAsserter asserter;
TestMemory memory;
TestSymtab symtab;
DisMc68000 dis68000;
Disassembler<target::uintptr_t> &disassembler(dis68000);

static void set_up() {
}

static void tear_down() {
    symtab.reset();
}

static void test_dest_size() {
    WTEST(ORI_B,  "#$23,D1",     000001, 0x0023);
    WTEST(ORI_W,  "#$6789,D5",   000105, 0x6789);
    WTEST(ORI_W,  "#$5152,(A0)", 000120, 0x5152);
    WTEST(ORI_L,  "#$BCDEF012,(A2)+", 00232, 0xBCDE, 0XF012);
    WTEST(ANDI_B, "#4,D3",       001003, 0x0004);
    WTEST(ANDI_W, "#$6789,D5",   001105, 0x6789);
    WTEST(ANDI_W, "#$789A,(A6)", 001126, 0x789A);
    WTEST(ANDI_L, "#$A8A9AAAB,-(A7)", 001247, 0xA8A9, 0xAAAB);
    WTEST(SUBI_B, "#6,D5",       002005, 0x0006);
    WTEST(SUBI_W, "#$5667,D5",   002105, 0x5667);
    WTEST(SUBI_W, "#$789A,(A6)", 002126, 0x789A);
    WTEST(SUBI_L, "#$04850486,D4",    002204, 0x0485, 0x0486);
    WTEST(ADDI_B, "#8,D7",       003007, 0x0008);
    WTEST(ADDI_W, "#$3210,(A4)", 003124, 0x3210);
    WTEST(ADDI_L, "#$06870688,D6",    003206, 0x0687, 0x0688);
    WTEST(EORI_B, "#2,D1",       005001, 0x0002);
    WTEST(EORI_W, "#$5152,(A0)", 005120, 0x5152);
    WTEST(EORI_L, "#$9A9B9C9D,(A1)+", 005231, 0x9A9B, 0x9C9D);
#if 0
    WTEST(CMPI);
    WTEST(NEGX);
    WTEST(CLR);
    WTEST(NEG);
    WTEST(NOT);
    WTEST(TST);
#endif

    WTEST(ORI,  "#$34,CCR",  000074, 0x0034);
    WTEST(ORI,  "#$1234,SR", 000174, 0x1234);
    WTEST(ANDI, "#$34,CCR",  001074, 0x0034);
    WTEST(ANDI, "#$1234,SR", 001174, 0x1234);
    WTEST(EORI, "#$34,CCR",  005074, 0x0034);
    WTEST(EORI, "#$1234,SR", 005174, 0x1234);
}

static void test_implied() {
    WTEST(ILLEGAL,"", 045374);
    WTEST(RESET, "",  047160);
    WTEST(NOP,   "",  047161);
    WTEST(RTS,   "",  047165);
    WTEST(TRAPV, "",  047166);
    WTEST(RTR,   "",  047167);
    WTEST(RTE,   "",  047163);
    WTEST(STOP,  "#$1234", 047162, 0x1234);
}

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_dest_size);
    RUN_TEST(test_implied);
    return 0;
}
