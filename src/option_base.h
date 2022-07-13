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

class Options;

struct OptionBase {
    enum OptionSpec : uint8_t {
        OPT_BOOL = 0,
        OPT_CHAR = 1,
        OPT_INT = 2,
        OPT_TEXT = 3,
    };

    const /* PROGMEM */ char *name_P() const { return _name_P; }
    OptionSpec spec() const { return _spec; }
    virtual Error set(StrScanner &scan) const = 0;

protected:
    OptionBase(const /* PROGMEM */ char *name_P, Options &options, OptionSpec spec);

    Error parseBoolOption(StrScanner &scan, bool &value) const;
    Error parseIntOption(StrScanner &scan, int32_t &value) const;

private:
    const /* PROGMEM */ char *_name_P;
    OptionSpec _spec;
};

class Options {
public:
    Options() : _num_options(0) {}

    Error setOption(const char *name, const char *text) const;

private:
    friend class OptionBase;
    void registerOption(const OptionBase *option);

    static constexpr auto MAX_OPTIONS = 10;
    uint8_t _num_options;
    const OptionBase *_options[MAX_OPTIONS];
};

struct BoolOptionBase : public OptionBase {
    BoolOptionBase(const /* PROGMEM */ char *name_P, Options &options)
        : OptionBase(name_P, options, OPT_BOOL) {}
    Error set(StrScanner &scan) const override {
        bool value = false;
        const auto error = parseBoolOption(scan, value);
        return error ? error : set(value);
    }
    virtual Error set(bool value) const = 0;
};

struct IntOptionBase : public OptionBase {
    IntOptionBase(const /* PROGMEM */ char *name_P, Options &options)
        : OptionBase(name_P, options, OPT_INT) {}
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
    BoolOption(const /* PROGMEM */ char *name_P, bool &value, Options &options)
        : OptionBase(name_P, options, OPT_BOOL), _value(value) {}

    Error set(StrScanner &scan) const override { return parseBoolOption(scan, _value); }

private:
    bool &_value;
};

struct CharOption : public OptionBase {
    CharOption(const /* PROGMEM */ char *name_P, char &value, Options &options)
        : OptionBase(name_P, options, OPT_CHAR), _value(value) {}

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

}  // namespace libasm

#endif  // __OPT_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
