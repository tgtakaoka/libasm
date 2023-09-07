/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "reg_base.h"

namespace libasm {
namespace reg {

/**
 * Parse register number from 0 to less than 20.  returns -1 if number is invalid or greater than
 * |max|.
 */
int8_t parseRegNumber(StrScanner &scan, int8_t max) {
    const auto c1 = scan.expect(isdigit);
    if (c1) {
        int8_t num = c1 - '0';
        const auto c2 = scan.expect(isdigit);
        if (c2)
            num = c2 - '0' + 10;
        if (num < max && !isidchar(*scan))
            return num;
    }
    return -1;
}

const NameEntry *searchName(uint8_t name, const NameEntry *begin, const NameEntry *end) {
    for (const auto *entry = begin; entry < end; entry++) {
        if (name == entry->name())
            return entry;
    }
    return nullptr;
}

uint8_t nameLen(uint8_t name, const NameEntry *begin, const NameEntry *end) {
    const auto *entry = searchName(name, begin, end);
    return entry ? entry->len() : 0;
}

const NameEntry *searchText(StrScanner &scan, const NameEntry *begin, const NameEntry *end) {
    for (const auto *entry = begin; entry < end; entry++) {
        const /*PROGMEM*/ char *text_P = entry->text_P();
        const uint8_t len = entry->len();
        if (scan.istarts_P(text_P, len) && !isidchar(scan[len])) {
            scan += len;
            return entry;
        }
    }
    return nullptr;
}

}  // namespace reg
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
