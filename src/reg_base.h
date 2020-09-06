/*
 * Copyright 2020 Tadashi G. Takaoka
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

#include "config_host.h"

#include <ctype.h>

#ifndef __REG_BASE_H__
#define __REG_BASE_H__

class RegBase {
public:
    void setUppercase(bool uppercase) { _uppercase = uppercase; }

protected:
    bool _uppercase;

    char *outChar(char *out, char c) const {
        *out++ = _uppercase ? toupper(c) : tolower(c);
        *out = 0;
        return out;
    }

    char *outText(char *out, const char *text) const {
        while (true) {
            const char c = pgm_read_byte(text);
            if (c == 0) break;
            out = outChar(out, c);
            text++;
        }
        return out;
    }
};

#endif // __REG_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
