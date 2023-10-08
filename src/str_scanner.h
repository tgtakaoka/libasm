/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __STR_SCANNER_H__
#define __STR_SCANNER_H__

#include <ctype.h>
#include <stddef.h>
#include <string.h>

namespace libasm {

struct StrScanner {
    /** construct from C-string |str| to end of it. */
    StrScanner(const char *str) : _str(str), _end(str + strlen(str)) {}

    /** construct from string from |str| to |end|. */
    constexpr StrScanner(const char *str, const char *end) : _str(str), _end(end) {}

    /** copy construct from |scan|, or |EMPTY|. */
    constexpr StrScanner(const StrScanner &scan = EMPTY) : _str(scan._str), _end(scan._end) {}

    /** assignment. */
    StrScanner &operator=(const StrScanner &scan) {
        _str = scan._str;
        _end = scan._end;
        return *this;
    }

    /** return head of underlying string. */
    constexpr const char *str() const { return _str; }

    /** return remaining size. */
    constexpr size_t size() const { return _end - _str; }

    /** dereference; returns character at the head, or 0 otherwise. */
    constexpr char operator*() const { return (*this)[0]; }

    /** indexing; character at offset |delta|, 0 if |delta| beyonds end. */
    constexpr char operator[](int delta) const { return (_str + delta) < _end ? _str[delta] : 0; }

    /** pre-increment. */
    StrScanner &operator++() { return *this += 1; }

    /** post-increment. */
    StrScanner operator++(int) {
        auto o = *this;
        *this += 1;
        return o;
    }

    /** pre-decrement. */
    StrScanner &operator--() { return *this += -1; }

    /** post-decrement. */
    StrScanner operator--(int) {
        auto o = *this;
        *this += -1;
        return o;
    }

    /** advance. */
    StrScanner &operator+=(int delta) {
        const char *p = _str + delta;
        if (p >= _end)
            p = _end;
        _str = p;
        return *this;
    }

    /** trim from start while |predicate(char)| return true. */
    template <typename P>
    StrScanner &trimStart(const P &predicate) {
        while (_str < _end && predicate(*_str))
            ++_str;
        return *this;
    }

    /** trim from end while |predicate(char)| return true. */
    template <typename P>
    StrScanner &trimEnd(const P &predicate) {
        while (_str < _end && predicate(_end[-1]))  // NOLINT(clang-analyzer-core.CallAndMessage)
            --_end;
        return *this;
    }

    /**
     * take letters from start while |predicate(char)| return true, then return a StrScanner
     * instance which points the continuation.
     */
    template <typename P>
    StrScanner takeWhile(const P &predicate) {
        auto *p = _str;
        while (p < _end && predicate(*p))
            p++;
        const auto end = _end;
        _end = p;
        return StrScanner{p, end};
    }

    /** skip spaces. */
    StrScanner &skipSpaces() { return trimStart(isspace); }

    /** return char if |predicate(char)| return true, otherwise zero. */
    template <typename P>
    char expect(const P &predicate) {
        const char s = *_str;
        if (_str < _end && predicate(s)) {
            ++_str;
            return s;
        }
        return 0;
    }

    /** return true if |c| exists then advance. */
    char expect(char c) {
        return expect([c](char s) { return s == c; });
    }

    /** return true if case-ignored |c| exists then advance. */
    char iexpect(char c) {
        return expect([c](char s) { return toupper(s) == toupper(c); });
    }

    /**
     * return true if this starts with |text_P| of |len| with ignore case, then dvance. Otherwise
     * return false.
     */
    bool iexpectText_P(const /*PROGMEM*/ char *text_P);
    bool iexpectText_P(const /*PROGMEM*/ char *text_P, size_t len);

    /**
     * return true if this starts with |text| with ignore case and must end with non-alphanumeric
     * (or '_').
     */
    bool iexpectWord_P(const /*PROGMEM*/ char *text_P);

    /** return true if this equals to |text| with ignore case. */
    bool iequals(const StrScanner &text) const;

    /** return true if this equals to |text_P| with ignore case. */
    bool iequals_P(const /*PROGMEM*/ char *text_P) const;

    static const StrScanner EMPTY;

private:
    /** |str| must not be greater than |_end|. */
    const char *_str;
    const char *_end;
};

}  // namespace libasm

#endif  // __STR_SCANNER_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
