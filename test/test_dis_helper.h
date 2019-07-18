/* -*- mode: c++; -*- */
#ifndef __TEST_DIS_HELPER_H__
#define __TEST_DIS_HELPER_H__

#include <inttypes.h>
#include <string.h>
#include <map>

#include "insn.h"

class TestSymtab : public Symtab {
public:
    const char *lookup(uint16_t addr) const override {
        auto it = _map.find(addr);
        return it == _map.end() ? nullptr : it->second;
    }
    void reset() { _map.clear(); }
    void put(uint16_t addr, const char *label) { _map[addr] = label; }
private:
    std::map<uint16_t, const char *> _map;
} symtab;

class TestMemory : public Memory {
public:
    TestMemory() : Memory(0) {}

    void setData(const uint8_t *data, uint8_t len) {
        _data = data;
        _len = len;
        _index = 0;
    }
    bool hasNext() const override { return _index < _len; }
    void setAddress(uint16_t addr) { _address = addr; }
    char *dump(char *out) {
        for (uint8_t idx = 0; idx < _len; idx++) {
            sprintf(out, "%02" PRIX8 " ", _data[idx]);
            out += 3;
        }
        return out;
    }

protected:
    uint8_t nextUint8() override { return _data[_index++]; }

private:
    const uint8_t *_data;
    uint8_t _len;
    uint8_t _index;
} memory;

static void assert_equals(const char *message, const Error expected, const Error actual) {
    if (expected == actual) return;
    printf("%s: expected Error '%d': actual '%d'\n", message, expected, actual);
}

static void assert_equals(const char *message, const char *expected, const char *actual) {
    if (strcmp(expected, actual) == 0) return;
    printf("%s: expected '%s': actual '%s'\n", message, expected, actual);
}

Insn insn;
char operands[40];
char comments[40];
char message[40];
#define ASSERT(addr, mnemonic, expected_operands, expected_comments, ...) \
    do {                                                                \
        const uint8_t mnemonic[] = { __VA_ARGS__ };                     \
        memory.setAddress(addr);                                        \
        memory.setData(mnemonic, sizeof(mnemonic));                     \
        insn.decode(memory, &symtab, operands, comments);               \
        sprintf(message, "%s: %s: ", __FUNCTION__, #mnemonic);          \
        memory.dump(message + strlen(message));                         \
        assert_equals(message, OK, insn.getError());                    \
        assert_equals(message, #mnemonic, insn.name());                 \
        assert_equals(message, expected_operands, operands);            \
        assert_equals(message, expected_comments, comments);            \
    } while (0)
#define ATEST(addr, mnemonic, opr, com, ...)        \
    ASSERT(addr, mnemonic, opr, com, __VA_ARGS__)
#define TEST(mnemonic, opr, com, ...)               \
    ASSERT(0x0000, mnemonic, opr, com, __VA_ARGS__)

#define RUN_TEST(test) run_test(test, #test)

#endif
