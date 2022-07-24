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

#ifndef __OPTION_BASE_H__
#define __OPTION_BASE_H__

#include "config_host.h"
#include "error_reporter.h"
#include "str_scanner.h"

#include <stdint.h>
#include <stdlib.h>

namespace libasm {

struct OptionBase {
    enum OptionSpec : uint8_t {
        OPT_BOOL = 0,
        OPT_CHAR = 1,
        OPT_INT = 2,
        OPT_TEXT = 3,
    };

    const /*PROGMEM*/ char *name_P() const { return _name_P; }
    OptionSpec spec() const { return _spec; }
    const /*PROGMEM*/ char *description_P() const { return _desc_P; }
    virtual Error set(StrScanner &scan) const = 0;
    const OptionBase *next() const { return _next; }

protected:
    OptionBase(const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P, OptionSpec spec,
            const OptionBase &next)
        : _name_P(name_P), _desc_P(desc_P), _spec(spec), _next(&next) {}

    OptionBase(const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P, OptionSpec spec)
        : _name_P(name_P), _desc_P(desc_P), _spec(spec), _next(nullptr) {}

    Error parseBoolOption(StrScanner &scan, bool &value) const;
    Error parseIntOption(StrScanner &scan, int32_t &value) const;

private:
    const /*PROGMEM*/ char *_name_P;
    const /*PROGMEM*/ char *_desc_P;
    OptionSpec _spec;
    const OptionBase *_next;
};

struct BoolOptionBase : public OptionBase {
    BoolOptionBase(
            const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P, const OptionBase &next)
        : OptionBase(name_P, desc_P, OPT_BOOL, next) {}
    BoolOptionBase(const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P)
        : OptionBase(name_P, desc_P, OPT_BOOL) {}

    Error set(StrScanner &scan) const override {
        bool value = false;
        const auto error = parseBoolOption(scan, value);
        return error ? error : set(value);
    }
    virtual Error set(bool value) const = 0;
};

struct IntOptionBase : public OptionBase {
    IntOptionBase(
            const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P, const OptionBase &next)
        : OptionBase(name_P, desc_P, OPT_INT, next) {}
    IntOptionBase(const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P)
        : OptionBase(name_P, desc_P, OPT_INT) {}

    Error set(StrScanner &scan) const override {
        int32_t value = 0;
        auto error = parseIntOption(scan, value);
        if (error == OK && (error = check(value)) == OK)
            set(value);
        return error;
    }
    virtual Error check(int32_t value) const {
        (void)value;
        return OK;
    }
    virtual void set(int32_t value) const = 0;
};

struct BoolOption : public OptionBase {
    BoolOption(const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P, bool &value,
            const OptionBase &next)
        : OptionBase(name_P, desc_P, OPT_BOOL, next), _value(value) {}
    BoolOption(const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P, bool &value)
        : OptionBase(name_P, desc_P, OPT_BOOL), _value(value) {}

    Error set(StrScanner &scan) const override { return parseBoolOption(scan, _value); }

private:
    bool &_value;
};

struct CharOption : public OptionBase {
    CharOption(const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P, char &value,
            const OptionBase &next)
        : OptionBase(name_P, desc_P, OPT_CHAR, next), _value(value) {}
    CharOption(const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P, char &value)
        : OptionBase(name_P, desc_P, OPT_CHAR), _value(value) {}

    Error set(StrScanner &scan) const override {
        if (scan.size() == 1) {
            _value = *scan;
            return OK;
        }
        return ILLEGAL_CONSTANT;
    }

private:
    char &_value;
};

class Options {
public:
    Options(const OptionBase &head) : _head(&head) {}

    const OptionBase *head() const { return _head; }
    Error setOption(const char *name, const char *text) const;

    static const Options EMPTY;
    static const char *nameof(OptionBase::OptionSpec spec);

private:
    const OptionBase *_head;

    Options() : _head(nullptr) {}
};

}  // namespace libasm

#endif  // __OPT_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
