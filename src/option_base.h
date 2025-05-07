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

#ifndef __LIBASM_OPTION_BASE_H__
#define __LIBASM_OPTION_BASE_H__

#include <stdint.h>

#include "error_reporter.h"
#include "str_scanner.h"
#include "type_traits.h"

namespace libasm {

struct Assembler;

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

    static Error parseBoolOption(StrScanner &scan, bool &value);

protected:
    constexpr OptionBase(const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P,
            OptionSpec spec, const OptionBase *next)
        : _name_P(name_P), _desc_P(desc_P), _spec(spec), _next(next) {}

    static Error parseIntOption(StrScanner &scan, int32_t &value);
    static Error parseCharOption(StrScanner &scan, char &value);
    static Error parseTextOption(StrScanner &scan, StrScanner &value);

private:
    const /*PROGMEM*/ char *const _name_P;
    const /*PROGMEM*/ char *const _desc_P;
    const OptionSpec _spec;
    const OptionBase *const _next;
};

template <typename APP>
struct BoolOption : public OptionBase {
    using Setter = Error (APP::*)(bool);

    constexpr BoolOption(APP *app, Setter setter, const /*PROGMEM*/ char *name_P,
            const /*PROGMEM*/ char *desc_P = nullptr, const OptionBase *next = nullptr)
        : OptionBase(name_P, desc_P, OPT_BOOL, next), _app(app), _setter(setter) {}

    template <typename A, typename enable_if<is_base_of<Assembler, A>::value>::type * = nullptr>
    static Error parseBool(StrScanner &scan, bool &value, const A *app) {
        return app->parseBoolOption(scan, value);
    }
    template <typename A, typename enable_if<!is_base_of<Assembler, A>::value>::type * = nullptr>
    static Error parseBool(StrScanner &scan, bool &value, const A *) {
        return parseBoolOption(scan, value);
    }

    Error set(StrScanner &scan) const override {
        bool value = false;
        const auto error = parseBool(scan, value, _app);
        return error ? error : (_app->*_setter)(value);
    }

private:
    APP *const _app;
    const Setter _setter;
};

template <typename APP>
struct IntOption : public OptionBase {
    using Setter = Error (APP::*)(int32_t);

    constexpr IntOption(APP *app, Setter setter, const /*PROGMEM*/ char *name_P,
            const /*PROGMEM*/ char *desc_P = nullptr, const OptionBase *next = nullptr)
        : OptionBase(name_P, desc_P, OPT_INT, next), _app(app), _setter(setter) {}

    template <typename A, typename enable_if<is_base_of<Assembler, A>::value>::type * = nullptr>
    static Error parseInt(StrScanner &scan, int32_t &value, const A *app) {
        return app->parseIntOption(scan, value);
    }
    template <typename A, typename enable_if<!is_base_of<Assembler, A>::value>::type * = nullptr>
    static Error parseInt(StrScanner &scan, int32_t &value, const A *) {
        return parseIntOption(scan, value);
    }

    Error set(StrScanner &scan) const override {
        int32_t value = 0;
        const auto error = parseInt(scan, value, _app);
        return error ? error : (_app->*_setter)(value);
    }

private:
    APP *const _app;
    const Setter _setter;
};

template <typename APP>
struct CharOption : public OptionBase {
    using Setter = Error (APP::*)(char);

    constexpr CharOption(APP *app, Setter setter, const /*PROGMEM*/ char *name_P,
            const /*PROGMEM*/ char *desc_P = nullptr, const OptionBase *next = nullptr)
        : OptionBase(name_P, desc_P, OPT_CHAR, next), _app(app), _setter(setter) {}

    Error set(StrScanner &scan) const override {
        char value = 0;
        const auto error = parseCharOption(scan, value);
        return error ? error : (_app->*_setter)(value);
    }

private:
    APP *const _app;
    const Setter _setter;
};

template <typename APP>
struct TextOption : public OptionBase {
    using Setter = Error (APP::*)(StrScanner &scan);

    constexpr TextOption(APP *app, Setter setter, const /*PROGMEM*/ char *name_P,
            const /*PROGMEM*/ char *desc_P = nullptr, const OptionBase *next = nullptr)
        : OptionBase(name_P, desc_P, OPT_TEXT, next), _app(app), _setter(setter) {}

    Error set(StrScanner &scan) const override {
        StrScanner value;
        const auto error = parseTextOption(scan, value);
        return error ? error : (_app->*_setter)(value);
    }

private:
    APP *const _app;
    const Setter _setter;
};

struct Options {
    constexpr Options(const OptionBase *head) : _head(head) {}

    const OptionBase *head() const { return _head; }
    const OptionBase *search(const StrScanner &name) const;
    Error setOption(const StrScanner &name, StrScanner &scan) const;

    static const char *nameof(OptionBase::OptionSpec spec);

private:
    const OptionBase *const _head;
};

namespace text {
namespace option {

extern const char OPT_TEXT_FPU[] PROGMEM;
extern const char OPT_DESC_FPU[] PROGMEM;
extern const char OPT_TEXT_PMMU[] PROGMEM;
extern const char OPT_DESC_PMMU[] PROGMEM;
extern const char OPT_BOOL_LONGA[] PROGMEM;
extern const char OPT_DESC_LONGA[] PROGMEM;
extern const char OPT_BOOL_LONGI[] PROGMEM;
extern const char OPT_DESC_LONGI[] PROGMEM;
extern const char OPT_BOOL_IMPLICIT_WORD[] PROGMEM;
extern const char OPT_DESC_IMPLICIT_WORD[] PROGMEM;
extern const char OPT_BOOL_ZILOG_SYNTAX[] PROGMEM;
extern const char OPT_DESC_ZILOG_SYNTAX[] PROGMEM;
extern const char OPT_BOOL_EXTMODE[] PROGMEM;
extern const char OPT_DESC_EXTMODE[] PROGMEM;
extern const char OPT_BOOL_LWORDMODE[] PROGMEM;
extern const char OPT_DESC_LWORDMODE[] PROGMEM;

}  // namespace option
}  // namespace text

}  // namespace libasm

#endif  // __LIBASM_OPT_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
