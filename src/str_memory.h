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

class StrMemory : public DisMemory {
public:
    StrMemory(uint32_t addr, const char *line)
        : DisMemory(addr),
          _next(line)
    {}
    
    bool hasNext() const override {
        const char *scan = _next;
        return readNumber(scan, nullptr) != scan;
    }

    static const char *readNumber(const char *scan, uint32_t *val) {
        // Intel style
        if (scanNumberEnd(scan, 'H'))
            return parseNumber(scan, val, 'H');

        // Motorola style
        if (*scan == '$' && scanNumberEnd(scan + 1))
            return parseNumber(scan + 1, val);

        // C-style
        if (*scan == '0') {
            if (toupper(scan[1]) == 'X' && scanNumberEnd(scan + 2))
                return parseNumber(scan + 2, val);
        }
        return scan;
    }

protected:
    uint8_t nextByte() override {
        uint32_t val32 = 0;
        _next = readNumber(_next, &val32);
        return val32;
    }

private:
    mutable const char *_next;

    static bool scanNumberEnd(const char *scan, const char suffix = 0) {
        return parseNumber(scan, nullptr, suffix) != scan;
    }

    static const char *parseNumber(
        const char *scan, uint32_t *val, char suffix = 0) {
        const char *p = skipSpaces(scan);
        uint32_t v = 0;
        while (isxdigit(*p)) {
            v <<= 4;;
            v += isdigit(*p) ? *p - '0' : toupper(*p) - 'A' + 10;
            p++;
        }
        if (suffix && toupper(*p++) != suffix)
            return scan;
        if (isSpace(*p) || *p == 0) {
            if (val) *val = v;
            return p;
        }
        return scan;
    }

    static bool isSpace(char c) {
        return c == ' ' || c == '\t';
    }

    static const char *skipSpaces(const char *p) {
        while (isSpace(*p))
            p++;
        return p;
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
