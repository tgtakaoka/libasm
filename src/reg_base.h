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

#include <ctype.h>

#include "config_host.h"

#ifndef __REG_BASE_H__
#define __REG_BASE_H__

/**
 * Base class for registers.
 */
class RegBase {
public:
    void setUppercase(bool uppercase) { _uppercase = uppercase; }
    bool isUppercase() const { return _uppercase; }

    /**
     * Entry of registers, condition codes and flags table.
     */
    struct NameEntry {
        const uint8_t _name;
        const /*PROGMEM*/ char *const _text;
#define NAME_ENTRY(name) {name, TEXT_##name}

        inline uint8_t name() const { return pgm_read_byte(&this->_name); }
        inline const /*PROGMEM*/ char *text() const {
            return reinterpret_cast<const char *>(pgm_read_ptr(&this->_text));
        }
        inline uint8_t len() const { return strlen_P(text()); }
    };

protected:
    bool _uppercase;

    RegBase() {}

    char *outChar(char *out, char c) const {
        *out++ = _uppercase ? toupper(c) : tolower(c);
        *out = 0;
        return out;
    }

    char *outText(char *out, const /*PROGMEM*/ char *text) const {
        while (true) {
            const char c = pgm_read_byte(text);
            if (c == 0)
                break;
            out = outChar(out, c);
            text++;
        }
        return out;
    }

    static bool isidchar(const char c) { return isalnum(c) || c == '_'; }

    static const NameEntry *searchName(
            uint8_t name, const NameEntry *begin, const NameEntry *end) {
        for (const NameEntry *entry = begin; entry < end; entry++) {
            if (name == entry->name())
                return entry;
        }
        return nullptr;
    }

    static uint8_t nameLen(
            uint8_t name, const NameEntry *begin, const NameEntry *end) {
        const NameEntry *entry = searchName(name, begin, end);
        return entry ? entry->len() : 0;
    }

    static const NameEntry *searchText(
            const char *str, const NameEntry *begin, const NameEntry *end) {
        for (const NameEntry *entry = begin; entry < end; entry++) {
            const /*PROGMEM*/ char *text = entry->text();
            const uint8_t len = entry->len();
            if (strncasecmp_P(str, text, len) == 0 && !isidchar(str[len]))
                return entry;
        }
        return nullptr;
    }
};

#endif  // __REG_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
