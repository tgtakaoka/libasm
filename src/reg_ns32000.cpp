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

#include "config_ns32000.h"
#include "reg_ns32000.h"
#include "table_ns32000.h"

#include <ctype.h>

namespace libasm {
namespace ns32000 {

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

static const char TEXT_REG_R0[] PROGMEM = "R0";
static const char TEXT_REG_R1[] PROGMEM = "R1";
static const char TEXT_REG_R2[] PROGMEM = "R2";
static const char TEXT_REG_R3[] PROGMEM = "R3";
static const char TEXT_REG_R4[] PROGMEM = "R4";
static const char TEXT_REG_R5[] PROGMEM = "R5";
static const char TEXT_REG_R6[] PROGMEM = "R6";
static const char TEXT_REG_R7[] PROGMEM = "R7";
static const char TEXT_REG_F0[] PROGMEM = "F0";
static const char TEXT_REG_F1[] PROGMEM = "F1";
static const char TEXT_REG_F2[] PROGMEM = "F2";
static const char TEXT_REG_F3[] PROGMEM = "F3";
static const char TEXT_REG_F4[] PROGMEM = "F4";
static const char TEXT_REG_F5[] PROGMEM = "F5";
static const char TEXT_REG_F6[] PROGMEM = "F6";
static const char TEXT_REG_F7[] PROGMEM = "F7";
static const char TEXT_REG_FP[] PROGMEM = "FP";
static const char TEXT_REG_SP[] PROGMEM = "SP";
static const char TEXT_REG_SB[] PROGMEM = "SB";
static const char TEXT_REG_PC[] PROGMEM = "PC";
static const char TEXT_REG_TOS[] PROGMEM = "TOS";
static const char TEXT_REG_EXT[] PROGMEM = "EXT";
static const NameEntry REG_TABLE[] PROGMEM = {
    { REG_R0,   2, TEXT_REG_R0 },
    { REG_R1,   2, TEXT_REG_R1 },
    { REG_R2,   2, TEXT_REG_R2 },
    { REG_R3,   2, TEXT_REG_R3 },
    { REG_R4,   2, TEXT_REG_R4 },
    { REG_R5,   2, TEXT_REG_R5 },
    { REG_R6,   2, TEXT_REG_R6 },
    { REG_R7,   2, TEXT_REG_R7 },
    { REG_FP,   2, TEXT_REG_FP },
    { REG_SP,   2, TEXT_REG_SP },
    { REG_SB,   2, TEXT_REG_SB },
    { REG_PC,   2, TEXT_REG_PC },
    { REG_TOS,  3, TEXT_REG_TOS },
    { REG_F0,   2, TEXT_REG_F0 },
    { REG_F1,   2, TEXT_REG_F1 },
    { REG_F2,   2, TEXT_REG_F2 },
    { REG_F3,   2, TEXT_REG_F3 },
    { REG_F4,   2, TEXT_REG_F4 },
    { REG_F5,   2, TEXT_REG_F5 },
    { REG_F6,   2, TEXT_REG_F6 },
    { REG_F7,   2, TEXT_REG_F7 },
    { REG_EXT,  3, TEXT_REG_EXT },
};

RegName RegNs32000::parseRegName(const char *line) const {
    const NameEntry *entry =
        NameEntry::searchText(line, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(pgm_read_byte(&entry->name)) : REG_UNDEF;
}

RegName RegNs32000::decodeRegName(uint8_t num, bool floating) const {
    num &= 7;
    return RegName(num + (floating ? 'A' : '0'));
}

int8_t RegNs32000::encodeRegName(RegName name) const {
    if (isGeneric(name)) return char(name) - '0';
    if (isFloat(name)) return char(name) - 'A';
    return -1;
}

uint8_t RegNs32000::regNameLen(RegName name) const {
    return NameEntry::nameLen(name, ARRAY_RANGE(REG_TABLE));
}

char *RegNs32000::outRegName(char *out, RegName name) const {
    const NameEntry *entry =
        NameEntry::searchName(name, ARRAY_RANGE(REG_TABLE));
    if (entry) {
        const char *text = reinterpret_cast<const char *>(
                pgm_read_ptr(&entry->text));
        out = outText(out, text);
    }
    return out;
}

bool RegNs32000::isGeneric(RegName name) const {
    return isdigit(char(name));
}

bool RegNs32000::isFloat(RegName name) const {
    return isupper(char(name));
}

static const char TEXT_PREG_UPSR[]    PROGMEM = "UPSR";
static const char TEXT_PREG_US[]      PROGMEM = "US";
static const char TEXT_PREG_PSR[]     PROGMEM = "PSR";
static const char TEXT_PREG_INTBASE[] PROGMEM = "INTBASE";
static const char TEXT_PREG_MOD[]     PROGMEM = "MOD";
static const NameEntry PREG_TABLE[] PROGMEM = {
    { PREG_UPSR,    4, TEXT_PREG_UPSR },
    { PREG_UPSR,    2, TEXT_PREG_US },
    { PREG_FP,      2, TEXT_REG_FP },
    { PREG_SP,      2, TEXT_REG_SP },
    { PREG_SB,      2, TEXT_REG_SB },
    { PREG_PSR,     3, TEXT_PREG_PSR },
    { PREG_INTBASE, 7, TEXT_PREG_INTBASE },
    { PREG_MOD,     3, TEXT_PREG_MOD },
};

PregName RegNs32000::parsePregName(const char *line) const {
    const NameEntry *entry =
        NameEntry::searchText(line, ARRAY_RANGE(PREG_TABLE));
    return entry ? PregName(pgm_read_byte(&entry->name)) : PREG_UNDEF;
}

PregName RegNs32000::decodePregName(uint8_t num) const {
    num &= 0xF;
    const NameEntry *entry =
        NameEntry::searchName(num, ARRAY_RANGE(PREG_TABLE));
    return entry ? PregName(pgm_read_byte(&entry->name)) : PREG_UNDEF;
}

int8_t RegNs32000::encodePregName(PregName name) const {
    return name == PREG_UNDEF ? -1 : uint8_t(name);
}

uint8_t RegNs32000::pregNameLen(PregName name) const {
    return NameEntry::nameLen(name, ARRAY_RANGE(PREG_TABLE));
}

char *RegNs32000::outPregName(char *out, PregName name) const {
    const NameEntry *entry =
        NameEntry::searchName(name, ARRAY_RANGE(PREG_TABLE));
    if (entry) {
        const char *text = reinterpret_cast<const char *>(
                pgm_read_ptr(&entry->text));
        out = outText(out, text);
    }
    return out;
}

static const char TEXT_MREG_BPR0[] PROGMEM = "BPR0";
static const char TEXT_MREG_BPR1[] PROGMEM = "BPR1";
static const char TEXT_MREG_MSR[]  PROGMEM = "MSR";
static const char TEXT_MREG_BCNT[] PROGMEM = "BCNT";
static const char TEXT_MREG_PTB0[] PROGMEM = "PTB0";
static const char TEXT_MREG_PTB1[] PROGMEM = "PTB1";
static const char TEXT_MREG_EIA[]  PROGMEM = "EIA";
static const NameEntry MREG_TABLE[] PROGMEM = {
    { MREG_BPR0, 4, TEXT_MREG_BPR0 },
    { MREG_BPR1, 4, TEXT_MREG_BPR1 },
    { MREG_MSR,  3, TEXT_MREG_MSR  },
    { MREG_BCNT, 4, TEXT_MREG_BCNT },
    { MREG_PTB0, 4, TEXT_MREG_PTB0 },
    { MREG_PTB1, 4, TEXT_MREG_PTB1 },
    { MREG_EIA,  3, TEXT_MREG_EIA  },
};

MregName RegNs32000::parseMregName(const char *line) const {
    const NameEntry *entry =
        NameEntry::searchText(line, ARRAY_RANGE(MREG_TABLE));
    return entry ? MregName(pgm_read_byte(&entry->name)) : MREG_UNDEF;
}

MregName RegNs32000::decodeMregName(uint8_t num) const {
    num &= 0xF;
    const NameEntry *entry =
        NameEntry::searchName(num, ARRAY_RANGE(MREG_TABLE));
    return entry ? MregName(pgm_read_byte(&entry->name)) : MREG_UNDEF;
}

int8_t RegNs32000::encodeMregName(MregName name) const {
    return name == MREG_UNDEF ? -1 : uint8_t(name);
}

uint8_t RegNs32000::mregNameLen(MregName name) const {
    return NameEntry::nameLen(name, ARRAY_RANGE(MREG_TABLE));
}

char *RegNs32000::outMregName(char *out, MregName name) const {
    const NameEntry *entry =
        NameEntry::searchName(name, ARRAY_RANGE(MREG_TABLE));
    if (entry) {
        const char *text = reinterpret_cast<const char *>(
                pgm_read_ptr(&entry->text));
        out = outText(out, text);
    }
    return out;
}

static const char TEXT_CONFIG_I[] PROGMEM = "I";
static const char TEXT_CONFIG_F[] PROGMEM = "F";
static const char TEXT_CONFIG_M[] PROGMEM = "M";
static const char TEXT_CONFIG_C[] PROGMEM = "C";
static const NameEntry CONFIG_TABLE[] PROGMEM = {
    { CONFIG_I, 1, TEXT_CONFIG_I },
    { CONFIG_M, 1, TEXT_CONFIG_M },
    { CONFIG_F, 1, TEXT_CONFIG_F },
    { CONFIG_C, 1, TEXT_CONFIG_C },
};

ConfigName RegNs32000::parseConfigName(const char *line) const {
    const NameEntry *entry =
        NameEntry::searchText(line, ARRAY_RANGE(CONFIG_TABLE));
    return entry ? ConfigName(pgm_read_byte(&entry->name)) : CONFIG_UNDEF;
}

ConfigName RegNs32000::decodeConfigName(uint8_t num) const {
    num &= 0xF;
    const NameEntry *entry =
        NameEntry::searchName(num, ARRAY_RANGE(CONFIG_TABLE));
    return entry ? ConfigName(pgm_read_byte(&entry->name)) : CONFIG_UNDEF;
}

uint8_t RegNs32000::configNameLen(ConfigName name) const {
    return name == CONFIG_UNDEF ? 0 : 1;
}

char *RegNs32000::outConfigName(char *out, ConfigName name) const {
    const NameEntry *entry =
        NameEntry::searchName(name, ARRAY_RANGE(CONFIG_TABLE));
    if (entry) {
        const char *text = reinterpret_cast<const char *>(
                pgm_read_ptr(&entry->text));
        out = outText(out, text);
    }
    return out;
}

static const char TEXT_STROPT_B[] PROGMEM = "B";
static const char TEXT_STROPT_W[] PROGMEM = "W";
static const char TEXT_STROPT_U[] PROGMEM = "U";
static const NameEntry STROPT_TABLE[] PROGMEM = {
    { STROPT_B, 1, TEXT_STROPT_B },
    { STROPT_W, 1, TEXT_STROPT_W },
    { STROPT_U, 1, TEXT_STROPT_U },
};

StrOptName RegNs32000::parseStrOptName(const char *line) const {
    const NameEntry *entry =
        NameEntry::searchText(line, ARRAY_RANGE(STROPT_TABLE));
    return entry ? StrOptName(pgm_read_byte(&entry->name)) : STROPT_UNDEF;
}

StrOptName RegNs32000::decodeStrOptName(uint8_t num) const {
    num &= 0xE;
    const NameEntry *entry =
        NameEntry::searchName(num, ARRAY_RANGE(STROPT_TABLE));
    return entry ? StrOptName(pgm_read_byte(&entry->name)) : STROPT_UNDEF;
}

uint8_t RegNs32000::strOptNameLen(StrOptName name) const {
    return name == STROPT_UNDEF ? 0 : 1;
}

char *RegNs32000::outStrOptName(char *out, StrOptName name) const {
    const NameEntry *entry =
        NameEntry::searchName(name, ARRAY_RANGE(STROPT_TABLE));
    if (entry) {
        const char *text = reinterpret_cast<const char *>(
                pgm_read_ptr(&entry->text));
        out = outText(out, text);
    }
    return out;
}

OprSize RegNs32000::parseIndexSize(const char *line) const {
    OprSize size = SZ_NONE;
    switch (toupper(*line)) {
    case 'B': size = SZ_BYTE; break;
    case 'W': size = SZ_WORD; break;
    case 'D': size = SZ_LONG; break;
    case 'Q': size = SZ_QUAD; break;
    default: return size;
    }
    return isidchar(line[1]) ? SZ_NONE : size;
}

uint8_t RegNs32000::indexSizeLen(OprSize size) const {
    return size == SZ_NONE ? 0 : 1;
}

char RegNs32000::indexSizeChar(OprSize size) const {
    char c;
    switch (size) {
    case SZ_BYTE: c = 'B'; break;
    case SZ_WORD: c = 'W'; break;
    case SZ_LONG: c = 'D'; break;
    case SZ_QUAD: c = 'Q'; break;
    default: c = 0; break;
    }
    return _uppercase ? c : tolower(c);
}

char *RegNs32000::outChar(char *out, char c) const {
    *out++ = _uppercase ? toupper(c) : tolower(c);
    *out = 0;
    return out;
}

char *RegNs32000::outText(char *out, const char *text) const {
    while (true) {
        const char c = pgm_read_byte(text);
        if (c == 0) break;
        out = outChar(out, c);
        text++;
    }
    return out;
}

} // namespace ns32000
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
