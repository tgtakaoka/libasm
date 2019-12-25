#include "test_expr_helper.h"

AsmMotoOperand parser;
TestSymtab symtab;
TestAsserter asserter;

static void set_up() {
}

static void tear_down() {
    symtab.reset();
}

static void test_char_constant() {
    E8("'a'",    0x61, OK);
    E8("'\\''",  0x27, OK);
    E8("'\\\"'", 0x22, OK);
    E8("'\\?'",  0x3F, OK);
    E8("'\\\\'", 0x5C, OK);
    E8("'\\b'",  0x08, OK);
    E8("'\\t'",  0x09, OK);
    E8("'\\n'",  0x0A, OK);
    E8("'\\r'",  0x0D, OK);
    E8("'\\X0'", 0x00, OK);
    E8("'\\xfF'", 0xFF, OK);
    E8("'\\0'",  0x00, OK);
    E8("'\\377'", 0xFF, OK);

    E8("'\\x100'", 0,  OVERFLOW_RANGE);
    E8("'\\400'", 0,   OVERFLOW_RANGE);
    E8("''",     0,    MISSING_CLOSING_QUOTE);
    E8("'a",     0,    MISSING_CLOSING_QUOTE);
    E8("'\\z'",  0,    UNKNOWN_ESCAPE_SEQUENCE);

    E16("'a'", 0x61, OK);
    E32("'a'", 0x61, OK);
}

static void test_dec_constant() {
    E8("0",   0x00, OK);
    E8("127", 0x7f, OK);
    E8("128", 0x80, OK);
    E8("255", 0xff, OK);
    E8("256", 0,    OVERFLOW_RANGE);

    E16("0",     0x0000, OK);
    E16("32767", 0x7fff, OK);
    E16("32768", 0x8000, OK);
    E16("65535", 0xffff, OK);
    E16("65536", 0,      OVERFLOW_RANGE);

    E32("0",          0x00000000, OK);
    E32("2147483647", 0x7fffffff, OK);
    E32("2147483648", 0x80000000, OK);
    E32("4294967295", 0xffffffff, OK);
    E32("4294967296", 0,          OVERFLOW_RANGE);
}

static void test_hex_constant() {
    E8("$0",  0x00, OK);
    E8("$7f", 0x7f, OK);
    E8("$80", 0x80, OK);
    E8("$FF", 0xff, OK);
    E8("$100", 0,    OVERFLOW_RANGE);

    E16("$0",    0x0000, OK);
    E16("$7fff", 0x7fff, OK);
    E16("$8000", 0x8000, OK);
    E16("$ffff", 0xffff, OK);
    E16("$10000", 0,      OVERFLOW_RANGE);

    E32("$0",        0x00000000, OK);
    E32("$7FFFFFFF", 0x7fffffff, OK);
    E32("$80000000", 0x80000000, OK);
    E32("$FFFFffff", 0xffffffff, OK);
    E32("$100000000", 0,          OVERFLOW_RANGE);
}

static void test_oct_constant() {
    E8("@0",   0x00, OK);
    E8("@177", 0x7f, OK);
    E8("@200", 0x80, OK);
    E8("@377", 0xff, OK);
    E8("@400", 0,    OVERFLOW_RANGE);

    E16("@0",      0x0000, OK);
    E16("@077777", 0x7fff, OK);
    E16("@100000", 0x8000, OK);
    E16("@177777", 0xffff, OK);
    E16("@200000", 0,      OVERFLOW_RANGE);

    E32("@0",           0x00000000, OK);
    E32("@17777777777", 0x7fffffff, OK);
    E32("@20000000000", 0x80000000, OK);
    E32("@37777777777", 0xffffffff, OK);
    E32("@40000000000", 0,          OVERFLOW_RANGE);
}

static void test_bin_constant() {
    E8("%0",        0x00, OK);
    E8("%01111111", 0x7f, OK);
    E8("%10000000", 0x80, OK);
    E8("%11111111", 0xff, OK);
    E8("100000000", 0,    OVERFLOW_RANGE);

    E16("%0",                0x0000, OK);
    E16("%0111111111111111", 0x7fff, OK);
    E16("%1000000000000000", 0x8000, OK);
    E16("%1111111111111111", 0xffff, OK);
    E16("%10000000000000000", 0,      OVERFLOW_RANGE);

    E32("%0",                                0x00000000, OK);
    E32("%01111111111111111111111111111111", 0x7fffffff, OK);
    E32("%10000000000000000000000000000000", 0x80000000, OK);
    E32("%11111111111111111111111111111111", 0xffffffff, OK);
    E32("%100000000000000000000000000000000", 0,          OVERFLOW_RANGE);
}

static void test_unary_operator() {
    E8("-1",   0xff, OK);
    E8("-128", 0x80, OK);
    E8("-129", 0,    OVERFLOW_RANGE);

    E16("-1",     0xffff, OK);
    E16("-32768", 0x8000, OK);
    E16("-32769", 0,      OVERFLOW_RANGE);

    E32("-1",          0xffffffff, OK);
    E32("-2147483648", 0x80000000, OK);
    E32("-2147483649", 0,          OVERFLOW_RANGE);

    E8("+128", 0x80, OK);
    E8("+129", 0x81, OK);
    E8("+256", 0,    OVERFLOW_RANGE);

    E16("+32768", 0x8000, OK);
    E16("+32769", 0x8001, OK);
    E16("+65536", 0,      OVERFLOW_RANGE);

    E32("+2147483648", 0x80000000, OK);
    E32("+2147483649", 0x80000001, OK);
    E32("+4294967296", 0,          OVERFLOW_RANGE);

    E8("~+0",    0xFF, OK);
    E8("~(1|8)", 0xF6, OK);
    E8("~-1",    0x00, OK);

    E16("~$0",      0xFFFF, OK);
    E16("~(1|@10)", 0xFFF6, OK);
    E16("~-%1",     0x0000, OK);

    E32("~+0",    0xFFFFFFFF, OK);
    E32("~(1|8)", 0xFFFFFFF6, OK);
    E32("~-1",    0x00000000, OK);
}

static void test_current_address() {
    symtab.setCurrentAddress(0x1000);
    E16("*",       0x1000, OK);
    E16("*+2",     0x1002, OK);
    E16("*-2",     0x0FFE, OK);
    E16("*+$F000", 0,      OVERFLOW_RANGE);
    E16("*-$1001", 0xFFFF, OK);
    E32("*+$F000", 0x00010000, OK);
    E32("*-$1001", 0xFFFFFFFF, OK);

    symtab.put(0x1000, "table");
    symtab.setCurrentAddress(0x1100);
    E16("*-table",     0x100, OK);
    E16("(*-table)/2", 0x80,  OK);
}

static void test_errors() {
    E32("undef",   0, UNDEFINED_SYMBOL);
    E32("$xxx",    0, ILLEGAL_CONSTANT);
    E32("@ooo",    0, ILLEGAL_CONSTANT);
    E32("%bbb",    0, ILLEGAL_CONSTANT);
    E32("2*(1+3",  0, MISSING_CLOSING_PAREN);
    E32("'a",      0, MISSING_CLOSING_QUOTE);
    E32("'\\'",    0, MISSING_CLOSING_QUOTE);
    E32("'\\x20",  0, MISSING_CLOSING_QUOTE);
    E32("'\\a'",   0, UNKNOWN_ESCAPE_SEQUENCE);
    E32("--1",     0, UNKNOWN_EXPR_OPERATOR);
    E32("-+1",     0, UNKNOWN_EXPR_OPERATOR);
    E32("+-1",     0, UNKNOWN_EXPR_OPERATOR);
    E32("++1",     0, UNKNOWN_EXPR_OPERATOR);
    E32("1<2",     0, UNKNOWN_EXPR_OPERATOR);
    E32("1>2",     0, UNKNOWN_EXPR_OPERATOR);
    E32("100/0",   0, DIVIDE_BY_ZERO);
    E32("100%0",   0, DIVIDE_BY_ZERO);
}

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_char_constant);
    RUN_TEST(test_dec_constant);
    RUN_TEST(test_hex_constant);
    RUN_TEST(test_oct_constant);
    RUN_TEST(test_bin_constant);
    RUN_TEST(test_unary_operator);
    RUN_TEST(test_current_address);
    RUN_TEST(test_errors);
    return 0;
}
