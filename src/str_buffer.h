/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __STR_BUFFER_H__
#define __STR_BUFFER_H__

#include "error_reporter.h"
#include "str_scanner.h"

#include <ctype.h>

namespace libasm {

struct StrBuffer : ErrorReporter {
    StrBuffer(char *buffer, size_t size) : ErrorReporter() { reset(buffer, size); }
    StrBuffer(const StrBuffer &o) : _out(o._out), _end(o._end) { setError(o.getError()); }

    StrBuffer *ptr() { return reinterpret_cast<StrBuffer *>(this); }
    size_t size() const { return _end - _out; }
    char *mark() const { return _out; }

    /** Reset output buffer to |buffer|. */
    StrBuffer &reset(char *buffer, size_t size);

    /** Hand over output buffer to |heir| and return it. */
    StrBuffer &over(StrBuffer &heir) const;

    /** Output a |letter| as is */
    StrBuffer &rletter(char c);

    /** Output a |letter| using specific case conversion. */
    virtual StrBuffer &letter(char c);

    /** Output |text| via |letter(char)|. */
    StrBuffer &text(const char *text);
    StrBuffer &text(const StrScanner &scan);
    StrBuffer &text_P(const /*PROGMEM*/ char *text_P);

    /** Output |text| via |rletter(char)| as is. */
    StrBuffer &rtext(const char *text);
    StrBuffer &rtext(const StrScanner &scan);
    StrBuffer &rtext_P(const /*PROGMEM*/ char *text_P);

    /** Output |...| arguments using |fmt| as |vsprintf| */
    StrBuffer &format_P(const /*PROGMEM*/ char *fmt, ...);

    /** output ", " */
    StrBuffer &comma();

    /** Reverse letters from |start| to |_out| */
    StrBuffer &reverse(char *start);

protected:
    StrBuffer &ref() { return reinterpret_cast<StrBuffer &>(*this); }

private:
    char *_out;
    const char *_end;
};

struct LowercaseBuffer : StrBuffer {
    LowercaseBuffer(char *buffer, size_t size) : StrBuffer(buffer, size) {}
    LowercaseBuffer(const StrBuffer &o) : StrBuffer(o) {}

    /** Output a |letter| as lowercase */
    StrBuffer &letter(char c) override {
        StrBuffer::letter(tolower(c));
        return ref();
    }
};

struct UppercaseBuffer : StrBuffer {
    UppercaseBuffer(char *buffer, size_t size) : StrBuffer(buffer, size) {}
    UppercaseBuffer(const StrBuffer &o) : StrBuffer(o) {}

    /** Output a |letter| as uppercase */
    StrBuffer &letter(char c) override {
        StrBuffer::letter(toupper(c));
        return ref();
    }
};

}  // namespace libasm

#endif  // __STR_BUFFER_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
