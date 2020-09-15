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
        return readNumber(_next, nullptr) != _next;
    }

    static const char *readNumber(const char *p, uint32_t *val) {
        // Intel style
        if (scanNumberEnd(p, 'H'))
            return parseNumber(p, val, 'H');

        // Motorola style
        if (*p == '$' && scanNumberEnd(p + 1))
            return parseNumber(p + 1, val);

        // C-style
        if (*p == '0') {
            const char c = toupper(p[1]);
            if (c == 'X' && scanNumberEnd(p + 2))
                return parseNumber(p + 2, val);
        }
        return p;
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

    static char *parseNumber(const char *scan, uint32_t *val, char suffix = 0) {
        const char *p = skipSpaces(scan);
        uint32_t v = 0;
        while (isxdigit(*p)) {
            v <<= 4;;
            v += isdigit(*p) ? *p - '0' : toupper(*p) - 'A' + 10;
            p++;
        }
        if (suffix && toupper(*p++) != suffix)
            return scan;
        if (isspace(*p) || *p == 0) {
            if (val) *val = v;
            return p;
        }
        return scan;
    }

    static char *skipSpaces(char *p) {
        while (isspace(*p))
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
