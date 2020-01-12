/* -*- mode: c++; -*- */
#ifndef __BIN_FORMATTER_H__
#define __BIN_FORMATTER_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

template<typename Addr>
class BinFormatter {
public:
    BinFormatter()
        : _line(nullptr),
          _line_len(0)
    {}
    virtual ~BinFormatter() {
        free(_line);
    }

    virtual const char *begin() = 0;
    virtual const char *encode(
        Addr addr, const uint8_t *data, size_t size) = 0;
    virtual const char *end() = 0;

protected:
    char *_line;
    unsigned _line_len;
    uint8_t _check_sum;

    void ensureLine(size_t len) {
        _line = static_cast<char *>(
            _line ? realloc(_line, len) : malloc(len));
    }

    void resetSum() { _check_sum = 0; }
    void addSum(uint8_t data) { _check_sum += data; }
    void addSum(uint16_t data) {
        addSum(static_cast<uint8_t>(data >> 8));
        addSum(static_cast<uint8_t>(data));
    }
    void addSum(uint32_t data) {
        addSum(static_cast<uint16_t>(data >> 16));
        addSum(static_cast<uint16_t>(data));
    }
};

template<typename Addr>
class IntelHex : public BinFormatter<Addr> {
public:
    IntelHex() : BinFormatter<Addr>() {}
    const char *begin() override { return nullptr; }
    const char *encode(
        Addr addr, const uint8_t *data, size_t size) override {
        this->ensureLine((sizeof(Addr) + size + 3) * 2);
        char *p = this->_line;
        p += sprintf(p, ":%02X%04X00",
                     static_cast<uint8_t>(size),
                     static_cast<uint16_t>(addr));
        this->resetSum();
        this->addSum(static_cast<uint8_t>(size));
        this->addSum(addr);
        for (size_t i = 0; i < size; i++) {
            p += sprintf(p, "%02X", data[i]);
            this->addSum(data[i]);
        }
        sprintf(p, "%02X", static_cast<uint8_t>(
                    -this->_check_sum & 0xff));
        return this->_line;
    }
    const char *end() override { return ":00000001FF"; }
};

template<typename Addr>
class SRecord : public BinFormatter<Addr> {
public:
    SRecord() : BinFormatter<Addr>() {}
    const char *begin() override {
        return "S0030000FC";
    }

    const char *encode(
        Addr addr, const uint8_t *data, size_t size) override {
        this->ensureLine((sizeof(Addr) + size + 3) * 2);
        const uint8_t len = sizeof(addr) + size + 1;
        this->resetSum();
        this->addSum(len);
        char *p = this->_line;
        if (sizeof(Addr) == 2) {
            p += sprintf(p, "S1%02X%04X", len, static_cast<uint16_t>(addr));
        } else { 
            p += sprintf(p, "S3%02X%08X", len, static_cast<uint32_t>(addr));
        }
        this->addSum(addr);
        for (size_t i = 0; i < size; i++) {
            p += sprintf(p, "%02X", data[i]);
            this->addSum(data[i]);
        }
        sprintf(p, "%02X", static_cast<uint8_t>(
                    ~this->_check_sum & 0xff));
        return this->_line;
    }
    const char *end() override {
        if (sizeof(Addr) == 2) {
            return "S9030000FC";
        } else {
            return "S70500000000FA";
        }
    }
};

#endif
