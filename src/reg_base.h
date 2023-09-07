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

#include "str_buffer.h"
#include "str_scanner.h"
#include "table_base.h"

namespace libasm {
namespace reg {

static inline bool isIdLetter(char c) {
    return isalnum(c) || c == '_';
}

/**
 * Entry of register, condition codes, and flags table.
 */
struct NameEntry {
    constexpr NameEntry(const /*PROGMEM*/ char *text_P, int8_t name)
        : _text_P(text_P), _name(name) {}

    constexpr const /*PROGMEM*/ char *text_P() const { return _text_P; }
    constexpr int8_t name() const { return _name; }

    StrBuffer &outText(StrBuffer &out) const { return out.text_P(_text_P); }

private:
    const /*PROGMEM*/ char *const _text_P;
    const int8_t _name;
};

/**
 * Name table
 */
struct NameTable {
    /**
     * NameEntry array |table|~|end| must be sorted by NameEntry::text_P.
     */
    constexpr NameTable(const /*PROGMEM*/ NameEntry *table, const /*PROGMEM*/ NameEntry *end)
        : _table(table, end) {}

    /**
     * Return pointer to an entry which has |name| as NameEntry::name,
     * otherwise nullptr.
     */
    const NameEntry *searchName(int8_t name) const;

    /**
     * Return pointer to an entrey which has |scan| as
     * NameEntry::text_P, then advance |scan| including alphanumerics
     * and single quote letters. Otherwise return |nullptr|.
     */
    const NameEntry *searchText(StrScanner &scan) const;

    /**
     * Similar to searchText(StrScanner&) but checks only alphabet
     * letters in |scan|.
     */
    const NameEntry *searchPrefix(StrScanner &scan) const;

private:
    const table::Table<NameEntry> _table;

    const NameEntry *searchSymbol(StrScanner &scan, bool (*)(char)) const;
};

/**
 * Parse register number.  returns -1 if number is invalid number.
 */
int16_t parseRegNumber(StrScanner &scan);

}  // namespace reg
}  // namespace libasm

#endif  // __REG_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
