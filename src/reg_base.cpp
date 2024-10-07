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
#include <ctype.h>
#include <string.h>

namespace libasm {
namespace reg {

namespace {

bool nameMatcher(int8_t &name, const NameEntry *item, int) {
    return name == item->name();
}

int textMatcher(StrScanner &symbol, const NameEntry *item) {
    const auto *text_P = item->text_P();
    const auto res = strncasecmp_P(symbol.str(), text_P, symbol.size());
    // |symbol| diffrs |item|.
    if (res)
        return res;
    // |symbol| length may be shorter than |item|.
    return symbol.size() < strlen_P(text_P) ? -1 : 0;
}

// Some architrectures have registers whose name contains a single
// quote, such as AFP'.  Some architrectures represent constant
// using prefix and surrounded by single quotes, such as C'x'.
bool isNameLetter(char c) {
    return isalnum(c) || c == '\'';
}

bool isPrefixLetter(char c) {
    return isalpha(c);
}

}  // namespace

const NameEntry *NameTable::searchName(int8_t name) const {
    return _table.linearSearch(name, nameMatcher, 0);
}

const NameEntry *NameTable::searchText(StrScanner &scan) const {
    return searchSymbol(scan, isNameLetter);
}

const NameEntry *NameTable::searchPrefix(StrScanner &scan) const {
    return searchSymbol(scan, isPrefixLetter);
}

const NameEntry *NameTable::searchSymbol(StrScanner &scan, bool (*predicator)(char)) const {
    auto symbol = scan;
    auto next = symbol.takeWhile(predicator);
    if (symbol.size() == 0)
        return nullptr;
    const auto *entry = _table.binarySearch(symbol, textMatcher);
    if (entry)
        scan = next;
    return entry;
}

int16_t parseRegNumber(StrScanner &scan) {
    auto p = scan;
    int16_t val = 0;
    int8_t leading_zero = 0;
    char c;
    while (isdigit(c = *p) && val < 1000) {
        if (val == 0 && c == '0')
            leading_zero++;
        val *= 10;
        val += c - '0';
        ++p;
    }
    if (isIdLetter(c))
        return -1;
    if ((val == 0 && leading_zero != 1) || (val != 0 && leading_zero >= 1))
        return -1;
    scan = p;
    return val;
}

}  // namespace reg
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
