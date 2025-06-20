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

#ifndef __LIBASM_REG_BASE_H__
#define __LIBASM_REG_BASE_H__

#include <ctype.h>
#include "config_host.h"
#include "table_base.h"

namespace libasm {

struct StrBuffer;
struct StrScanner;

namespace reg {

static inline bool isIdLetter(char c) {
    return isalnum(c) || c == '_';
}

/**
 * Entry of register, condition codes, and flags table.
 */
struct NameEntry {
    constexpr NameEntry(const /*PROGMEM*/ char *text_P, int8_t name)
        : _text_P(text_P), _name_P(name) {}

    const /*PROGMEM*/ char *text_P() const {
        return reinterpret_cast<const char *>(pgm_read_ptr(&_text_P));
    }
    int8_t name() const { return pgm_read_byte(&_name_P); }

    StrBuffer &outText(StrBuffer &out) const;

private:
    const /*PROGMEM*/ char *const _text_P;
    const int8_t _name_P;
};

/**
 * Name table
 */
struct NameTable {
    /**
     * NameEntry array |table|~|end| must be sorted by NameEntry::text_P.
     */
    constexpr NameTable(const /*PROGMEM*/ NameEntry *table_P, const /*PROGMEM*/ NameEntry *end_P)
        : _table(table_P, end_P) {}

    /**
     * Return pointer to an entry which has |name| as NameEntry::name,
     * otherwise nullptr.
     */
    const NameEntry *searchName(int8_t name) const;

    /**
     * Return pointer to an entrey which has |symbol| as
     * NameEntry::text_P. Otherwise return |nullptr|.
     */
    const NameEntry *searchText(const StrScanner &symbol) const;

private:
    const table::Table<NameEntry> _table;
};

/**
 * Parse register number.  returns -1 if number is invalid number.
 */
int16_t parseRegNumber(StrScanner &scan, bool allowHex = false);

}  // namespace reg
}  // namespace libasm

#endif  // __LIBASM_REG_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
