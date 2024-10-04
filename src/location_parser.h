/*
 * Copyright 2024 Tadashi G. Takaoka
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

#ifndef __LIBASM_LOCATION_PARSER_H__
#define __LIBASM_LOCATION_PARSER_H__

#include <ctype.h>
#include <string.h>
#include "config_host.h"
#include "str_scanner.h"
#include "text_common.h"

namespace libasm {

/**
 * Location counter parser.
 */
struct LocationParser {
    virtual bool locationSymbol(StrScanner &scan) const = 0;
};

struct SimpleLocationParser : LocationParser {
    SimpleLocationParser(const /*PROGMEM*/ char *letters) : _letters_P(letters) {}
    bool locationSymbol(StrScanner &scan) const override {
        auto p = scan;
        const auto c = *p++;
        if (c && strchr_P(_letters_P, c) && !isalnum(*p)) {
            scan = p;
            return true;
        }
        return false;
    }

private:
    const /*PROGMEM*/ char *_letters_P;
};

struct DollarLocationParser final : SimpleLocationParser, Singleton<DollarLocationParser> {
    DollarLocationParser() : SimpleLocationParser(text::common::PSTR_DOLLAR) {}
};

struct DotStarLocationParser final : SimpleLocationParser, Singleton<DotStarLocationParser> {
    DotStarLocationParser() : SimpleLocationParser(text::common::PSTR_DOT_STAR) {}
};

struct DotDollarLocationParser final : SimpleLocationParser, Singleton<DotDollarLocationParser> {
    DotDollarLocationParser() : SimpleLocationParser(text::common::PSTR_DOT_DOLLAR) {}
};

struct StarLocationParser final : SimpleLocationParser, Singleton<StarLocationParser> {
    StarLocationParser() : SimpleLocationParser(text::common::PSTR_STAR) {}
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
