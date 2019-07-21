/* -*- mode: c++; -*- */
#ifndef __TEST_DIS_HELPER_H__
#define __TEST_DIS_HELPER_H__

#include <cinttypes>
#include <cstdio>
#include <cstring>
#include <map>

#include "error_reporter.h"
#include "symbol_table.h"

class TestSymtab : public SymbolTable {
public:
    const char *lookup(target::uintptr_t addr) const override {
        auto it = _map.find(addr);
        return it == _map.end() ? nullptr : it->second;
    }
    void reset() { _map.clear(); }
    void put(target:: uintptr_t addr, const char *label) { _map[addr] = label; }
private:
    std::map<target::uintptr_t, const char *> _map;
} symtab;

class TestMemory : public Memory {
public:
    TestMemory() : Memory(0) {}

    void setBytes(const target::byte_t *bytes, host::uint_t len) {
        _bytes = bytes;
        _len = len;
        _index = 0;
    }
    bool hasNext() const override { return _index < _len; }
    void setAddress(target::uintptr_t addr) { _address = addr; }
    char *dump(char *out) {
        for (host::uint_t idx = 0; idx < _len; idx++) {
            sprintf(out, "%02" PRIX8 " ", _bytes[idx]);
            out += 3;
        }
        return out;
    }

protected:
    target::byte_t nextByte() override { return _bytes[_index++]; }

private:
    const target::byte_t *_bytes;
    host::uint_t _len;
    host::uint_t _index;
} memory;

static void assert_equals(
    const char *message, const Error expected, const Error actual) {
    if (expected == actual) return;
    printf("%s: expected Error '%d': actual '%d'\n", message, expected, actual);
}

static void assert_equals(
    const char *message, const char *expected, const char *actual) {
    if (strcmp(expected, actual) == 0) return;
    printf("%s: expected '%s': actual '%s'\n", message, expected, actual);
}

static void assert_equals(
    const char *message,
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

#define ASSERT(addr, mnemonic, expected_operands, expected_comments, ...) \
    do {                                                                \
        const uint8_t mnemonic[] = { __VA_ARGS__ };                     \
        memory.setAddress(addr);                                        \
        memory.setBytes(mnemonic, sizeof(mnemonic));                    \
        Insn insn;                                                      \
        char operands[40], comments[40], message[40];                   \
        disassembler.decode(memory, insn, operands, comments, &symtab); \
        sprintf(message, "%s: %s: ", __FUNCTION__, #mnemonic);          \
        memory.dump(message + strlen(message));                         \
        assert_equals(message, OK, disassembler.getError());            \
        assert_equals(message, #mnemonic, insn.name());                 \
        assert_equals(message, expected_operands, operands);            \
        assert_equals(message, expected_comments, comments);            \
        assert_equals(message,                                          \
                      mnemonic, sizeof(mnemonic),                       \
                      insn.bytes(), insn.insnLen());                    \
    } while (0)
#define ATEST(addr, mnemonic, opr, com, ...)        \
    ASSERT(addr, mnemonic, opr, com, __VA_ARGS__)
#define TEST(mnemonic, opr, com, ...)               \
    ASSERT(0x0000, mnemonic, opr, com, __VA_ARGS__)

#define RUN_TEST(test) run_test(test, #test)

#endif
