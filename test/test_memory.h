/* -*- mode: c++; -*- */
#ifndef __TEST_MEMORY_H__
#define __TEST_MEMORY_H__

#include "memory.h"

#include <cinttypes>
#include <cstdint>
#include <cstdio>

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
};

#endif
