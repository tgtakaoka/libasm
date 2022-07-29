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

#include "str_scanner.h"

#include "config_host.h"

namespace libasm {

StrScanner StrScanner::EMPTY("");

bool StrScanner::iequals_P(const /*PROGMEM*/ char *text_P) const {
    const auto len = strlen_P(text_P);
    return size() == len && strncasecmp_P(_str, text_P, len) == 0;
}

bool StrScanner::istarts_P(const /*PROGMEM*/ char *text_P, size_t len) const {
    if (len == 0)
        len = strlen_P(text_P);
    return strncasecmp_P(_str, text_P, len) == 0;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
