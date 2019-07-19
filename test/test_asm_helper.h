/* -*- mode: c++; -*- */
#ifndef __TEST_ASM_HELPER_H__
#define __TEST_ASM_HELPER_H__

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <map>

#include "error_reporter.h"
#include "symbol_table.h"

class TestSymtab : public SymbolTable {
public:
    bool hasSymbol(const char *symbol) const override {
        return _map.find(symbol) != _map.end();
    }
    target::uintptr_t lookup(const char *symbol) const override {
        auto it = _map.find(symbol);
        return it == _map.end() ? 0 : it->second;
    }
    void reset() { _map.clear(); }
    void put(target::uintptr_t addr, const char *symbol) {
        _map.insert_or_assign(symbol, addr);
    }
private:
    struct compare_text {
        bool operator()(const char *a, const char *b) const {
            return strcmp(a, b) < 0;
        }
    };
    std::map<const char *, target::uintptr_t, compare_text> _map;
} symtab;

static void assert_equals(const char *message, const Error expected, const Error actual) {
    if (expected == actual) return;
    printf("%s: expected Error '%d': actual '%d'\n", message, expected, actual);
}

static void assert_equals(const char *message,
                          const target::byte_t expected[], host::uint_t expected_len,
                          const target::byte_t actual[], host::uint_t actual_len) {
    if (expected_len == actual_len) {
        host::uint_t i;
        for (i = 0; i < expected_len; i++)
            if (expected[i] != actual[i])
                break;
        if (i == expected_len)
            return;
    }
    printf("%s: expected ", message);
    for (host::uint_t i = 0; i < expected_len; i++) {
        printf(i == 0 ?  "[" : " ");
        printf("%02" PRIX8, expected[i]);
    }
    printf("]: actual ");
    for (host::uint_t i = 0; i < actual_len; i++) {
        printf(i == 0 ? "[" : " ");
        printf("%02" PRIX8, actual[i]);
    }
    printf("\n");
}

#define ASSERT(addr, line, ...)                             \
    do {                                                    \
        const target::byte_t expected[] = { __VA_ARGS__ };  \
        Insn insn;                                          \
        char message[40];                                   \
        assembler.encode(line, insn, addr, &symtab);        \
        sprintf(message, "%s: %s", __FUNCTION__, line);     \
        assert_equals(message, OK, assembler.getError());   \
        assert_equals(message, expected, sizeof(expected),  \
                      insn.bytes(), insn.insnLen());        \
    } while (0)
#define ATEST(addr, line, ...) ASSERT(addr, line, __VA_ARGS__)
#define TEST(line, ...) ATEST(0x0000, line, __VA_ARGS__)

#define RUN_TEST(test) run_test(test, #test)

#endif
