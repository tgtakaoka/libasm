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

#ifndef __REG_BASE_H__
#define __REG_BASE_H__

#include <ctype.h>

#include "config_host.h"
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {
namespace reg {

/**
 * Entry of registers, condition codes and flags table.
 */
struct NameEntry {
    const uint8_t _name;
    const /*PROGMEM*/ char *const _text_P;
#define NAME_ENTRY(name) \
    { name, TEXT_##name }

    inline uint8_t name() const {
        return pgm_read_byte(&this->_name);
    }
    inline const /*PROGMEM*/ char *text_P() const {
        return reinterpret_cast<const char *>(pgm_read_ptr(&this->_text_P));
    }
    inline uint8_t len() const {
        return strlen_P(text_P());
    }
};

static inline bool isidchar(char c) {
    return isalnum(c) || c == '_';
}

const NameEntry *searchName(uint8_t name, const NameEntry *begin, const NameEntry *end);
uint8_t nameLen(uint8_t name, const NameEntry *begin, const NameEntry *end);
const NameEntry *searchText(StrScanner &scan, const NameEntry *begin, const NameEntry *end);

/**
 * Parse register number from 0 to less than 20.
 *
 * returns -1 if number is invalid or greater than |max|.
 */
int8_t parseRegNumber(StrScanner &scan, int8_t max);

/** Output register number from 0 to less than 20. */
StrBuffer &outRegNumber(StrBuffer &out, int8_t num);

}  // namespace reg
}  // namespace libasm

#endif  // __REG_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
