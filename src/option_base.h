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
    virtual Error set(const StrScanner &scan) const = 0;
    const OptionBase *next() const { return _next; }

    static Error parseBoolOption(const StrScanner &scan, bool &var);
    static Error parseIntOption(const StrScanner &scan, int32_t &var);
    static StrScanner readSymbol(const StrScanner &scan);

protected:
    OptionBase(const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P, OptionSpec spec,
            const OptionBase &next)
        : _name_P(name_P), _desc_P(desc_P), _spec(spec), _next(&next) {}

    OptionBase(const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P, OptionSpec spec)
        : _name_P(name_P), _desc_P(desc_P), _spec(spec), _next(nullptr) {}

private:
    const /*PROGMEM*/ char *_name_P;
    const /*PROGMEM*/ char *_desc_P;
    const OptionSpec _spec;
    const OptionBase *_next;
};

template <typename APP>
struct BoolOption : public OptionBase {
    using Setter = Error (APP::*)(bool);

    BoolOption(APP *app, Setter setter, const /*PROGMEM*/ char *name_P,
            const /*PROGMEM*/ char *desc_P, const OptionBase &next)
        : OptionBase(name_P, desc_P, OPT_BOOL, next), _app(app), _setter(setter) {}
    BoolOption(
            APP *app, Setter setter, const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P)
        : OptionBase(name_P, desc_P, OPT_BOOL), _app(app), _setter(setter) {}

    Error set(const StrScanner &scan) const override {
        bool value = false;
        const auto error = parseBoolOption(scan, value);
        return error ? error : (_app->*_setter)(value);
    }

private:
    APP *const _app;
    const Setter _setter;
};

template <typename APP>
struct IntOption : public OptionBase {
    using Setter = Error (APP::*)(int32_t);

    IntOption(APP *app, Setter setter, const /*PROGMEM*/ char *name_P,
            const /*PROGMEM*/ char *desc_P, const OptionBase &next)
        : OptionBase(name_P, desc_P, OPT_INT, next), _app(app), _setter(setter) {}
    IntOption(
            APP *app, Setter setter, const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P)
        : OptionBase(name_P, desc_P, OPT_INT), _app(app), _setter(setter) {}

    Error set(const StrScanner &scan) const override {
        int32_t value = 0;
        const auto error = parseIntOption(scan, value);
        return error ? error : (_app->*_setter)(value);
    }

private:
    APP *const _app;
    const Setter _setter;
};

template <typename APP>
struct CharOption : public OptionBase {
    using Setter = Error (APP::*)(char);

    CharOption(APP *app, Setter setter, const /*PROGMEM*/ char *name_P,
            const /*PROGMEM*/ char *desc_P, const OptionBase &next)
        : OptionBase(name_P, desc_P, OPT_CHAR, next), _app(app), _setter(setter) {}
    CharOption(
            APP *app, Setter setter, const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P)
        : OptionBase(name_P, desc_P, OPT_CHAR), _app(app), _setter(setter) {}

    Error set(const StrScanner &scan) const override {
        return scan.size() <= 1 ? (_app->*_setter)(*scan) : ILLEGAL_CONSTANT;
    }

private:
    APP *const _app;
    const Setter _setter;
};

template <typename APP>
struct TextOption : public OptionBase {
    using Setter = Error (APP::*)(const StrScanner &scan);

    TextOption(APP *app, Setter setter, const /*PROGMEM*/ char *name_P,
            const /*PROGMEM*/ char *desc_P, const OptionBase &next)
        : OptionBase(name_P, desc_P, OPT_TEXT, next), _app(app), _setter(setter) {}
    TextOption(
            APP *app, Setter setter, const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P)
        : OptionBase(name_P, desc_P, OPT_TEXT), _app(app), _setter(setter) {}

    Error set(const StrScanner &scan) const override { return (_app->*_setter)(scan); }

private:
    APP *const _app;
    const Setter _setter;
};

struct Options {
    Options(const OptionBase *head) : _head(head) {}

    const OptionBase *head() const { return _head; }
    Error setOption(const StrScanner &name, const StrScanner &text) const;

    static const char *nameof(OptionBase::OptionSpec spec);

private:
    const OptionBase *_head;
};

}  // namespace libasm

#endif  // __OPT_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
