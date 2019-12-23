#include "test_expr_helper.h"

AsmIntelOperand parser;
TestSymtab symtab;
TestAsserter asserter;

static void set_up() {
}

static void tear_down() {
    symtab.reset();
}

static void test_dec_constant() {
    E8("0",    0x00, OK);
    E8("-1",   0xff, OK);
    E8("-128", 0x80, OK);
    E8(" 255", 0xff, OK);
    E8("-129", 0, OVERFLOW_RANGE);
    E8(" 256", 0, OVERFLOW_RANGE);

    E16("0",      0x0000, OK);
    E16("-1",     0xffff, OK);
    E16("-32768", 0x8000, OK);
    E16(" 65535", 0xffff, OK);
    E16("-32769", 0, OVERFLOW_RANGE);
    E16(" 65536", 0, OVERFLOW_RANGE);

    E32("0",           0x00000000, OK);
    E32("-1",          0xffffffff, OK);
    E32("-2147483648", 0x80000000, OK);
    E32(" 4294967295", 0xffffffff, OK);
    E32("-2147483649", 0, OVERFLOW_RANGE);
    E32(" 4294967296", 0, OVERFLOW_RANGE);
}

static void test_hex_constant() {
    E8("0H",   0x00, OK);
    E8("-1H",  0xff, OK);
    E8("-80H", 0x80, OK);
    E8("0FFH", 0xff, OK);
    E8("-81H", 0, OVERFLOW_RANGE);
    E8("100H", 0, OVERFLOW_RANGE);

    E16("0H",     0x0000, OK);
    E16("-1H",    0xffff, OK);
    E16("-8000H", 0x8000, OK);
    E16("0ffffh", 0xffff, OK);
    E16("-8001H", 0, OVERFLOW_RANGE);
    E16("10000H", 0, OVERFLOW_RANGE);

    E32("0H",         0x00000000, OK);
    E32("-1H",        0xffffffff, OK);
    E32("-80000000H", 0x80000000, OK);
    E32("0FFFFffffH", 0xffffffff, OK);
    E32("-80000001H", 0, OVERFLOW_RANGE);
    E32("100000000H", 0, OVERFLOW_RANGE);
}

static void test_oct_constant() {
    E8("0O",    0x00, OK);
    E8("-1O",   0xff, OK);
    E8("-200O", 0x80, OK);
    E8(" 377O", 0xff, OK);
    E8("-201O", 0, OVERFLOW_RANGE);
    E8(" 400O", 0, OVERFLOW_RANGE);

    E16("0O",       0x0000, OK);
    E16("-1O",      0xffff, OK);
    E16("-100000O", 0x8000, OK);
    E16(" 177777o", 0xffff, OK);
    E16("-100001O", 0, OVERFLOW_RANGE);
    E16(" 200000O", 0, OVERFLOW_RANGE);

    E32("0b",            0x00000000, OK);
    E32("-1b",           0xffffffff, OK);
    E32("-20000000000o", 0x80000000, OK);
    E32(" 37777777777O", 0xffffffff, OK);
    E32("-20000000001o", 0, OVERFLOW_RANGE);
    E32(" 40000000000O", 0, OVERFLOW_RANGE);
}

static void test_bin_constant() {
    E8(" 0B",        0x00, OK);
    E8(" 01111111b", 0x7f, OK);
    E8(" 10000000B", 0x80, OK);
    E8(" 11111111B", 0xff, OK);
    E8("100000000B", 0,    OVERFLOW_RANGE);
    E8("-1B",        0xff, OK);
    E8("-10000000B", 0x80, OK);
    E8("-10000001B", 0,    OVERFLOW_RANGE);

    E16(" 0B",                0x0000, OK);
    E16(" 0111111111111111B", 0x7fff, OK);
    E16(" 1000000000000000B", 0x8000, OK);
    E16(" 1111111111111111b", 0xffff, OK);
    E16("10000000000000000B", 0,      OVERFLOW_RANGE);
    E16("-1B",                0xffff, OK);
    E16("-1000000000000000B", 0x8000, OK);
    E16("-1000000000000001B", 0,      OVERFLOW_RANGE);

    E32(" 0B",                                0x00000000, OK);
    E32(" 01111111111111111111111111111111B", 0x7fffffff, OK);
    E32(" 10000000000000000000000000000000B", 0x80000000, OK);
    E32(" 11111111111111111111111111111111B", 0xffffffff, OK);
    E32("100000000000000000000000000000000b", 0,          OVERFLOW_RANGE);
    E32("-1b",                                0xffffffff, OK);
    E32("-10000000000000000000000000000000B", 0x80000000, OK);
    E32("-10000000000000000000000000000001B", 0,          OVERFLOW_RANGE);
}

static void test_current_address() {
    symtab.setCurrentAddress(0x1000);
    E16("$",         0x1000, OK);
    E16("$+2",       0x1002, OK);
    E16("$-2",       0x0FFE, OK);
    E16("$+0F000H",  0,       OVERFLOW_RANGE);
    E32("$+0F000H", 0x10000, OK);
    E16("$-1001H",   0xFFFF, OK);

    symtab.put(0x1000, "table");
    symtab.setCurrentAddress(0x1100);
    E16("$-table",     0x100, OK);
    E16("($-table)/2",  0x80, OK);
}

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_dec_constant);
    RUN_TEST(test_hex_constant);
    RUN_TEST(test_oct_constant);
    RUN_TEST(test_bin_constant);
    RUN_TEST(test_current_address);
    return 0;
}
