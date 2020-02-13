/* -*- mode: c++; -*- */
#ifndef __BIN_FORMATTER_H__
#define __BIN_FORMATTER_H__

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

template<typename Addr>
class BinFormatter {
public:
    BinFormatter()
        : _line(nullptr),
          _line_len(0),
          _data(nullptr),
          _data_len(0)
    {}
    virtual ~BinFormatter() {
        if (_line) free(_line);
        if (_data) free(_data);
    }

    virtual const char *begin() = 0;
    virtual const char *encode(
        Addr addr, const uint8_t *data, size_t size) = 0;
    virtual const char *end() = 0;
    virtual uint8_t *decode(
        const char *line, Addr &addr, size_t &size) = 0;

protected:
    char *_line;
    size_t _line_len;
    uint8_t *_data;
    size_t _data_len;
    uint8_t _check_sum;

    void ensureLine(size_t len) {
        if (_line_len < len) {
            _line = static_cast<char *>(
                _line ? realloc(_line, len) : malloc(len));
            _line_len = len;
        }
    }

    void ensureData(size_t len) {
        if (_data_len < len) {
            _data = static_cast<uint8_t *>(
                _data ? realloc(_data, len) : malloc(len));
            _data_len = len;
        }
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

    static uint8_t parseHex(char c) {
        return isdigit(c) ? c - '0' : toupper(c) - 'A' + 10;
    }

    bool parseByte(const char *&line, uint8_t &val) {
        if (!isxdigit(*line)) return true;
        val = parseHex(*line++) << 4;
        if (!isxdigit(*line)) return true;
        val |= parseHex(*line++);
        addSum(val);
        return false;
    }

    bool parseUint16(const char *&line, uint16_t &val) {
        uint8_t val8 = 0;
        if (parseByte(line, val8)) return true;
        val = static_cast<uint16_t>(val8) << 8;
        if (parseByte(line, val8)) return true;
        val |= val8;
        return false;
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

    uint8_t *decode(const char *line, Addr &addr, size_t &size) override {
        if (*line++ != ':') return nullptr;
        size = 0;
        uint8_t len = 0;
        if (this->parseByte(line, len)) return nullptr;
        uint16_t val16 = 0;
        if (this->parseUint16(line, val16)) return nullptr;
        addr = val16;
        uint8_t type;
        if (this->parseByte(line, type)) return nullptr;
        if (type == 0x01) return this->_data; // terminator
        if (type != 0x00) return nullptr;

        size = len;
        this->ensureData(size);
        this->resetSum();
        this->addSum(static_cast<uint8_t>(size));
        this->addSum(addr);
        for (size_t i = 0; i < size; i++) {
            if (this->parseByte(line, this->_data[i]))
                return nullptr;
        }
        uint8_t val = 0;
        if (this->parseByte(line, val)) return nullptr;
        if (this->_check_sum) return nullptr; // checksum error

        return this->_data;
    }
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
        sprintf(p, "%02X", static_cast<uint8_t>(~this->_check_sum));
        return this->_line;
    }

    const char *end() override {
        if (sizeof(Addr) == 2) {
            return "S9030000FC";
        } else {
            return "S70500000000FA";
        }
    }

    uint8_t *decode(const char *line, Addr &addr, size_t &size) override {
        if (*line++ != 'S') return nullptr;
        const char type = *line++;
        this->ensureData(16);
        size = 0;
        if (type == '0')
            return this->_data; // start record
        if (type == '9' || type == '8' || type == '7')
            return this->_data; // end record
        if (type == '5' || type == '6')
            return this->_data; // record count
        if (type != '1' && type != '2' && type != '3')
            return nullptr;     // format error
        if (sizeof(Addr) == 2 && type != '1')
            return nullptr;     // address size overflow
        this->resetSum();
        uint8_t len = 0;
        if (this->parseByte(line, len)) return nullptr;
        uint16_t val16 = 0;
        if (type == '1') {
            if (len < 2) return nullptr;
            if (this->parseUint16(line, val16)) return nullptr;
            addr = val16;
            len -= 2;
        } else if (type == '2') {
            if (len < 3) return nullptr;
            uint8_t val8;
            if (this->parseByte(line, val8)) return nullptr;
            addr = static_cast<uint32_t>(val8) << 16;
            if (this->parseUint16(line, val16)) return nullptr;
            addr |= val16;
            len -= 3;
        } else {
            if (len < 4) return nullptr;
            if (this->parseUint16(line, val16)) return nullptr;
            addr = static_cast<uint32_t>(val16) << 16;
            if (this->parseUint16(line, val16)) return nullptr;
            addr |= val16;
            len -= 4;
        }
        if (len < 1) return nullptr;
        size = len - 1;

        this->ensureData(size);
        for (size_t i = 0; i < size; i++)
            if (this->parseByte(line, this->_data[i])) return nullptr;
        const uint8_t sum = this->_check_sum;
        uint8_t val = 0;
        if (this->parseByte(line, val)) return nullptr;
        if (static_cast<uint8_t>(~val) != sum) // checksum error
            return nullptr;

        return this->_data;
    }
};

#endif
