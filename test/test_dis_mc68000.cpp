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
    TEST(ORI_B,  "#$23,D1",     000001, 0x0023);
    TEST(ORI_W,  "#$6789,D5",   000105, 0x6789);
    TEST(ORI_W,  "#$5152,(A0)", 000120, 0x5152);
    TEST(ORI_L,  "#$BCDEF012,(A2)+", 00232, 0xBCDE, 0XF012);
    TEST(ANDI_B, "#4,D3",       001003, 0x0004);
    TEST(ANDI_W, "#$6789,D5",   001105, 0x6789);
    TEST(ANDI_W, "#$789A,(A6)", 001126, 0x789A);
    TEST(ANDI_L, "#$A8A9AAAB,-(A7)", 001247, 0xA8A9, 0xAAAB);
    TEST(SUBI_B, "#6,D5",       002005, 0x0006);
    TEST(SUBI_W, "#$5667,D5",   002105, 0x5667);
    TEST(SUBI_W, "#$789A,(A6)", 002126, 0x789A);
    TEST(SUBI_L, "#$04850486,D4",    002204, 0x0485, 0x0486);
    TEST(ADDI_B, "#8,D7",       003007, 0x0008);
    TEST(ADDI_W, "#$3210,(A4)", 003124, 0x3210);
    TEST(ADDI_L, "#$06870688,D6",    003206, 0x0687, 0x0688);
    TEST(EORI_B, "#2,D1",       005001, 0x0002);
    TEST(EORI_W, "#$5152,(A0)", 005120, 0x5152);
    TEST(EORI_L, "#$9A9B9C9D,(A1)+", 005231, 0x9A9B, 0x9C9D);
#if 0
    TEST(CMPI);
    TEST(NEGX);
    TEST(CLR);
    TEST(NEG);
    TEST(NOT);
    TEST(TST);
#endif

    TEST(ORI,  "#$34,CCR",  000074, 0x0034);
    TEST(ORI,  "#$1234,SR", 000174, 0x1234);
    TEST(ANDI, "#$34,CCR",  001074, 0x0034);
    TEST(ANDI, "#$1234,SR", 001174, 0x1234);
    TEST(EORI, "#$34,CCR",  005074, 0x0034);
    TEST(EORI, "#$1234,SR", 005174, 0x1234);
}

static void test_implied() {
    TEST(ILLEGAL,"", 045374);
    TEST(RESET, "",  047160);
    TEST(NOP,   "",  047161);
    TEST(RTS,   "",  047165);
    TEST(TRAPV, "",  047166);
    TEST(RTR,   "",  047167);
    TEST(RTE,   "",  047163);
    TEST(STOP,  "#$1234", 047162, 0x1234);
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
