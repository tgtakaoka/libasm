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

#include "option_base.h"

#include <ctype.h>
#include <stdlib.h>

#include "asm_base.h"

namespace libasm {

namespace {

Error parseBool(StrScanner &scan, bool &value) {
    auto p = scan;
    const auto dquote = p.expect('"');
    if (p.expectTrue()) {
        if (dquote && !p.expect('"'))
            return MISSING_CLOSING_DQUOTE;
        scan = p;
        value = true;
        return OK;
    }
    if (p.expectFalse()) {
        if (dquote && !p.expect('"'))
            return MISSING_CLOSING_DQUOTE;
        scan = p;
        value = false;
        return OK;
    }
    return NOT_AN_EXPECTED;
}

Error parseInt(StrScanner &scan, int32_t &value) {
    char *end = nullptr;
    value = strtol(scan.str(), &end, 10);
    if (end == scan.str())
        return ILLEGAL_CONSTANT;
    scan += end - scan.str();
    return OK;
}

}  // namespace

Error OptionBase::parseBoolOption(StrScanner &scan, bool &value) {
    if (parseBool(scan, value) == OK)
        return OK;
    int32_t val;
    const auto error = parseIntOption(scan, val);
    if (error == OK)
        value = (val != 0);
    return error;
}

Error OptionBase::parseIntOption(StrScanner &scan, int32_t &value) {
    auto p = scan;
    const auto dquote = p.expect('"');
    const auto error = parseInt(p, value);
    if (error == OK) {
        if (dquote && !p.expect('"'))
            return ILLEGAL_CONSTANT;
        scan = p;
    }
    return error;
}

Error OptionBase::parseCharOption(StrScanner &scan, char &value) {
    auto p = scan;
    const char quote = (*p == '"' || *p == '\'') ? *p : 0;
    value = *p++;
    if (quote && !p.expect(quote))
        return quote == '"' ? MISSING_CLOSING_DQUOTE : MISSING_CLOSING_QUOTE;
    scan = p;
    return OK;
}

Error OptionBase::parseTextOption(StrScanner &scan, StrScanner &value) {
    auto p = scan;
    const auto dquote = p.expect('"');
    value = p;
    p = value.takeWhile(isalnum);
    if (value.size()) {
        if (dquote && !p.expect('"'))
            return MISSING_CLOSING_DQUOTE;
        scan = p;
        return OK;
    }
    return ILLEGAL_CONSTANT;
}

const OptionBase *Options::search(const StrScanner &name) const {
    for (auto option = _head; option != nullptr; option = option->next()) {
        if (name.iequals_P(option->name_P()))
            return option;
    }
    return nullptr;
}

Error Options::setOption(const StrScanner &name, StrScanner &scan) const {
    const auto *option = search(name);
    return option ? option->set(scan) : UNKNOWN_OPTION;
}

const char *Options::nameof(OptionBase::OptionSpec spec) {
    switch (spec) {
    case OptionBase::OptionSpec::OPT_BOOL:
        return "bool";
    case OptionBase::OptionSpec::OPT_CHAR:
        return "char";
    case OptionBase::OptionSpec::OPT_INT:
        return "int";
    case OptionBase::OptionSpec::OPT_TEXT:
        return "text";
    default:
        return "UNKNOWN";
    }
}

namespace text {
namespace option {

constexpr char OPT_BOOL_GNU_AS[] PROGMEM = "gnu-as";
constexpr char OPT_DESC_GNU_AS[] PROGMEM = "GNU assembler compatible";
constexpr char OPT_TEXT_FPU[] PROGMEM = "fpu";
constexpr char OPT_DESC_FPU[] PROGMEM = "floating point co-processor";
constexpr char OPT_BOOL_IMPLICIT_WORD[] PROGMEM = "implicit-word";
constexpr char OPT_DESC_IMPLICIT_WORD[] PROGMEM = "unknown instruction defines word";

}  // namespace option
}  // namespace text

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
