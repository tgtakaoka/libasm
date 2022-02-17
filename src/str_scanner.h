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

class StrScanner {
public:
    /** construct from C-string |str| to end of it, or |end| if specified */
    StrScanner(const char *str, const char *end = nullptr)
        : _str(str), _end(end ? end : str + strlen(str)) {}

    /** copy construct from |scan|, or |EMPTY| */
    StrScanner(const StrScanner &scan = EMPTY) : _str(scan._str), _end(scan._end) {}

    /** assignment */
    StrScanner &operator=(const StrScanner &scan) {
        _str = scan._str;
        _end = scan._end;
        return *this;
    }

    operator const char *() const { return _str; }
    size_t size() const { return _end - _str; }

    /** dereference; return character at the head or 0 if none */
    char operator*() const { return (*this)[0]; }
    char operator[](int delta) const {
        const char *p = _str + delta;
        return p < _end ? *p : 0;
    }

    /** pre-increment */
    StrScanner &operator++() { return *this += 1; }

    /** post-increment */
    StrScanner operator++(int) {
        StrScanner o(*this);
        *this += 1;
        return o;
    }

    /** pre-decrement */
    StrScanner &operator--() { return *this += -1; }

    /** post-decrement */
    StrScanner operator--(int) {
        StrScanner o(*this);
        *this += -1;
        return o;
    }

    /** advance */
    StrScanner &operator+=(int delta) {
        const char *p = _str + delta;
        if (p >= _end)
            p = _end;
        _str = p;
        return *this;
    }

    /** trim from start while |predicate(char)| return true */
    template <typename P>
    StrScanner &trimStart(const P &predicate) {
        while (_str < _end && predicate(*_str))
            ++_str;
        return *this;
    }

    /** trim from end while |predicate(char)| return true */
    template <typename P>
    StrScanner &trimEnd(const P &predicate) {
        while (_str < _end && predicate(_end[-1])) // NOLINT(clang-analyzer-core.CallAndMessage)
            --_end;
        return *this;
    }

    /** skip spaces */
    StrScanner &skipSpaces() { return trimStart(isspace); }

    /** trim start to |scan|, or make it empty. */
    StrScanner &trimStartAt(const char *str) {
        _str = (str < _end) ? str : _end;
        return *this;
    }

    /** trim end to |scan|, or make it empty. */
    StrScanner &trimEndAt(const char *str) {
        _end = (_str < str) ? str : _str;
        return *this;
    }

    /** return char if |predicate(char)| return true, otherwise zero */
    template <typename P>
    char expect(const P &predicate) {
        const char s = *_str;
        if (_str < _end && predicate(s)) {
            *this += 1;
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

    /** return true if this equals to |str| with ignore case. */
    bool iequals_P(const char *str) const;

    /** return true if this starts with |str| with ignore case. */
    bool istarts_P(const char *str, size_t len = 0) const;

    static StrScanner EMPTY;

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
