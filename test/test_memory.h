/* -*- mode: c++; -*- */
#ifndef __TEST_MEMORY_H__
#define __TEST_MEMORY_H__

#include "dis_memory.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

class TestMemory : public DisMemory<target::uintptr_t> {
public:
    TestMemory()
        : DisMemory(0),
          _bytes(nullptr),
          _words(nullptr)
    {}

    template<typename T>
    void setMemory(const T *data, host::uint_t size);
    bool hasNext() const override { return _index < _size; }
    void setAddress(target::uintptr_t addr) { _address = addr; }
    char *dump(char *out) {
        if (_words) {
            for (host::uint_t idx = 0; idx < _size; idx += 2) {
                sprintf(out, "%04" PRIX16 " ", _words[idx / 2]);
                out += 8;
            }
            return out;
        }
        for (host::uint_t idx = 0; idx < _size; idx++) {
            sprintf(out, "%02" PRIX8 " ", _bytes[idx]);
            out += 3;
        }
        return out;
    }

protected:
    uint8_t nextByte() override {
        if (_words) {
            const uint16_t word = _words[_index / 2];
            // Big-endian word. Most significant byte is in lowest address.
            return (_index++ % 2) == 0 ? (word >> 8) : word;
        }
        return _bytes[_index++];
    }

private:
    const uint8_t *_bytes;
    const uint16_t *_words;
    host::uint_t _size;
    host::uint_t _index;
};

template<>
void TestMemory::setMemory<uint8_t>(const uint8_t *data, host::uint_t size) {
    _bytes = data;
    _words = nullptr;
    _size = size;
    _index = 0;
}

template<>
void TestMemory::setMemory<uint16_t>(const uint16_t *data, host::uint_t size) {
    _bytes = nullptr;
    _words = data;
    _size = size;
    _index = 0;
}

#endif
