/*
 * Copyright 2020 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __STR_MEMORY_H__
#define __STR_MEMORY_H__

#include "dis_memory.h"

#include <ctype.h>

namespace libasm {

class StrMemory : public DisMemory<target::uintptr_t> {
public:
    StrMemory(target::uintptr_t addr, const char *line)
        : DisMemory(addr), _next(line) {
        skipSpaces();
    }
    
    bool hasNext() const override {
        const char *saved_next = _next;
        if (readNumber(nullptr)) {
            _next = saved_next;
            return true;
        }
        return false;
    }

protected:
    uint8_t nextByte() override {
        uint8_t val8 = 0;
        readNumber(&val8);
        skipSpaces();
        return val8;
    }

private:
    mutable const char *_next;

    static bool isValidDigit(const char c, const uint8_t base) {
        if (base == 16) return isxdigit(c);
        return c >= '0' && c < '0' + base;
    }

    static uint8_t toNumber(const char c, const uint8_t base) {
        if (base == 16 && !isdigit(c))
            return toupper(c) - 'A' + 10;
        return c - '0';
    }

    static bool scanNumberEnd(
        const char *p, const uint8_t base, char suffix = 0) {
        if (!isValidDigit(*p, base))
            return false;
        while (isValidDigit(*p, base))
            p++;
        if (suffix && toupper(*p++) != suffix)
            return false;
        return isspace(*p) || *p == 0;
    }

    bool parseNumber(
        const char *p, uint8_t *val, const uint8_t base,
        const char suffix = 0) const {
        if (!isValidDigit(*p, base))
            return false;
        uint8_t v = 0;
        while (isValidDigit(*p, base)) {
            v *= base;
            v += toNumber(*p, base);
            p++;
        }
        if (suffix && toupper(*p++) != suffix)
            return false;
        if (isspace(*p) || *p == 0) {
            _next = p;
            if (val) *val = v;
            return true;
        }
        return false;
    }

    void skipSpaces() {
        while (isspace(*_next))
            _next++;
    }

    bool readNumber(uint8_t *val) const {
        const char *p = _next;

        // Intel style
        if (scanNumberEnd(p, 16, 'H'))
            return parseNumber(p, val, 16, 'H');
        if (scanNumberEnd(p, 8, 'O'))
            return parseNumber(p, val, 8, 'O');
        if (scanNumberEnd(p, 2, 'B'))
            return parseNumber(p, val, 2, 'B');
        if (scanNumberEnd(p, 10))
            return parseNumber(p, val, 10);

        // Motorola style
        if (*p == '$' && scanNumberEnd(p + 1, 16))
            return parseNumber(p + 1, val, 16);
        if (*p == '@' && scanNumberEnd(p + 1, 8))
            return parseNumber(p + 1, val, 8);
        if (*p == '%' && scanNumberEnd(p + 1, 2))
            return parseNumber(p + 1, val, 2);

        // C-style
        if (*p == '0') {
            const char c = toupper(p[1]);
            if (c == 'X' && scanNumberEnd(p + 2, 16))
                return parseNumber(p + 2, val, 16);
            if (isValidDigit(c, 8) && scanNumberEnd(p + 1, 8))
                return parseNumber(p + 1, val, 8);
            if (c == 'B' && scanNumberEnd(p + 2, 2))
                return parseNumber(p + 2, val, 2);
        }

        return false;
    }
};

} // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
