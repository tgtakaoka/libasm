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

namespace libasm {

Error OptionBase::parseBoolOption(StrScanner &scan, bool &value) {
    if (scan.iequals_P(PSTR("on")) || scan.iequals_P(PSTR("true")) || scan.iequals_P(PSTR("yes")) ||
            scan.iequals_P(PSTR("enable"))) {
        value = true;
        return OK;
    }
    if (scan.iequals_P(PSTR("off")) || scan.iequals_P(PSTR("false")) ||
            scan.iequals_P(PSTR("no")) || scan.iequals_P(PSTR("disable"))) {
        value = false;
        return OK;
    }
    return ILLEGAL_CONSTANT;
}

Error OptionBase::parseIntOption(StrScanner &scan, int32_t &value) {
    char *end = nullptr;
    value = strtol(scan.str(), &end, 10);
    if (end != scan.str() && *end == 0)
        return OK;
    return ILLEGAL_CONSTANT;
}

Error Options::setOption(const char *name, const char *text) const {
    for (auto option = _head; option != nullptr; option = option->next()) {
        if (strcmp_P(name, option->name_P()) == 0) {
            StrScanner scan(text);
            return option->set(scan);
        }
    }
    return UNKNOWN_OPTION;
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

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
