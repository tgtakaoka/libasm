/* -*- mode: c++; -*- */
#ifndef __STR_MEMORY_H__
#define __STR_MEMORY_H__

#include "dis_memory.h"

#include <ctype.h>

class StrMemory : public DisMemory {
public:
    StrMemory(target::uintptr_t addr, const char *line)
        : DisMemory(addr), _line(skipSpace(line)) {
    }
    
    bool hasNext() const override {
        return readNext(_line, nullptr) != nullptr;
    }

protected:
    uint8_t nextByte() override {
        uint8_t _val = 0;
        _line = readNext(_line, &_val);
        return _val;
    }

private:
    const char *_line;

    static bool isValid(char c, int radix) {
        if (radix == 10) return isdigit(c);
        if (radix == 16) return isxdigit(c);
        if (radix == 2) return c == '0' || c == '1';
        if (radix == 8) return c >= '0' && c < '8';
        return false;
    }

    static int toNumber(char c, int radix) {
        if (radix == 16 && !isdigit(c))
            return tolower(c) - 'a' + 10;
        return c - '0';
    }

    static const char *skipSpace(const char *p) {
        while (isspace(*p)) p++;
        return p;
    }

    static const char *readNum(
        const char *p, int radix, uint8_t *val, char suffix = 0) {
        uint8_t v = 0;
        if (!isValid(*p, radix)) return nullptr;
        while (isValid(*p, radix)) {
            v *= radix;
            v += toNumber(*p++, radix);
        }
        if (suffix) {
            if (tolower(*p) == suffix && isspace(p[1])) {
                p++;
            } else {
                p = nullptr;
            }
        } else {
            if (!isspace(*p))
                p = nullptr;
        }
        if (val)
            *val = v;
        return p;
    }

    static const char *readNext(const char *line, uint8_t *v) {
        if (!line) return nullptr;
        const char *p = skipSpace(line);
        const char *n = nullptr;
        if (*p == '$' && (n = readNum(p + 1, 16, v)))        // $<hex>
            return n;
        if (*p == '%' && (n = readNum(p + 1, 2, v)))         // %<bin>
            return n;
        if (*p == '@' && (n = readNum(p + 1, 8, v)))         // @<oct>
            return n;
        if (!isdigit(*p))
            return nullptr;     // error
        if (*p == '0') {
            const char t = tolower(p[1]);
            if (t == 'x' && (n = readNum(p + 2, 16, v)))     // 0x<hex>
                return n;
            if (t == 'b' && (n = readNum(p + 2, 2, v)))      // 0b<bin>
                return n;
            if (isValid(t, 8) && (n = readNum(p + 1, 8, v))) // 0<oct>
                return n;
        }
        if ((n = readNum(p, 16, v, 'h')))                    // <hex>h
            return n;
        if ((n = readNum(p, 2, v, 'b')))                     // <bin>b
            return n;
        if ((n = readNum(p, 8, v, 'o')))                     // <oct>o
            return n;
        if ((n = readNum(p, 10, v)))                         // <dec>
            return n;
        return nullptr;
    }
};

#endif
