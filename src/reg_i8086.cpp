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

#include "config_i8086.h"
#include "reg_i8086.h"
#include "table_i8086.h"

#include <ctype.h>

namespace libasm {
namespace i8086 {

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

struct NameEntry {
    uint8_t name;
    uint8_t len;
    const char *text;

    static const NameEntry *searchName(
            uint8_t name, const NameEntry *begin, const NameEntry *end) {
        for (const NameEntry *entry = begin; entry < end; entry++) {
            const uint8_t n = pgm_read_byte(&entry->name);
            if (name == n) return entry;
        }
        return nullptr;
    }
    static const NameEntry *searchText(
            const char *str, const NameEntry *begin, const NameEntry *end) {
        for (const NameEntry *entry = begin; entry < end; entry++) {
            const char *text = reinterpret_cast<const char *>(
                    pgm_read_ptr(&entry->text));
            const uint8_t len = pgm_read_byte(&entry->len);
            if (strncasecmp_P(str, text, len) == 0 && !isidchar(str[len]))
                return entry;
        }
        return nullptr;
    }
    static uint8_t nameLen(
            uint8_t name, const NameEntry *begin, const NameEntry *end) {
        const NameEntry *entry = searchName(name, begin, end);
        return entry ? pgm_read_byte(&entry->len) : 0;
    }
};

static const char TEXT_AL[]   PROGMEM = "AL";
static const char TEXT_BL[]   PROGMEM = "BL";
static const char TEXT_CL[]   PROGMEM = "CL";
static const char TEXT_DL[]   PROGMEM = "DL";
static const char TEXT_AH[]   PROGMEM = "AH";
static const char TEXT_BH[]   PROGMEM = "BH";
static const char TEXT_CH[]   PROGMEM = "CH";
static const char TEXT_DH[]   PROGMEM = "DH";
static const char TEXT_AX[]   PROGMEM = "AX";
static const char TEXT_BX[]   PROGMEM = "BX";
static const char TEXT_CX[]   PROGMEM = "CX";
static const char TEXT_DX[]   PROGMEM = "DX";
static const char TEXT_SP[]   PROGMEM = "SP";
static const char TEXT_BP[]   PROGMEM = "BP";
static const char TEXT_SI[]   PROGMEM = "SI";
static const char TEXT_DI[]   PROGMEM = "DI";
static const char TEXT_CS[]   PROGMEM = "CS";
static const char TEXT_DS[]   PROGMEM = "DS";
static const char TEXT_ES[]   PROGMEM = "ES";
static const char TEXT_SS[]   PROGMEM = "SS";
static const char TEXT_PTR[]  PROGMEM = "PTR";
static const char TEXT_BYTE[] PROGMEM = "BYTE";
static const char TEXT_WORD[] PROGMEM = "WORD";
static const NameEntry REG_TABLE[] PROGMEM = {
    { REG_AL,   2, TEXT_AL },
    { REG_AH,   2, TEXT_AH },
    { REG_AX,   2, TEXT_AX },
    { REG_BL,   2, TEXT_BL },
    { REG_BH,   2, TEXT_BH },
    { REG_BX,   2, TEXT_BX },
    { REG_CL,   2, TEXT_CL },
    { REG_CH,   2, TEXT_CH },
    { REG_CX,   2, TEXT_CX },
    { REG_DL,   2, TEXT_DL },
    { REG_DH,   2, TEXT_DH },
    { REG_DX,   2, TEXT_DX },
    { REG_BP,   2, TEXT_BP },
    { REG_SP,   2, TEXT_SP },
    { REG_SI,   2, TEXT_SI },
    { REG_DI,   2, TEXT_DI },
    { REG_CS,   2, TEXT_CS },
    { REG_DS,   2, TEXT_DS },
    { REG_ES,   2, TEXT_ES },
    { REG_SS,   2, TEXT_SS },
    { REG_PTR,  3, TEXT_PTR  },
    { REG_BYTE, 4, TEXT_BYTE },
    { REG_WORD, 4, TEXT_WORD },
};

RegName RegI8086::decodeByteReg(uint8_t num) const {
    num &= 7;
    return RegName(num + 8);
}

RegName RegI8086::decodeWordReg(uint8_t num) const {
    num &= 7;
    return RegName(num + 16);
}

RegName RegI8086::decodeSegReg(uint8_t num) const {
    num &= 3;
    return RegName(num + 24);
}

char *RegI8086::outRegName(char *out, RegName name) const {
    const NameEntry *entry =
        NameEntry::searchName(name, ARRAY_RANGE(REG_TABLE));
    if (entry) {
        const char *text = reinterpret_cast<const char *>(
                pgm_read_ptr(&entry->text));
        out = outText(out, text);
    }
    return out;
}

} // namespace i8086
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
