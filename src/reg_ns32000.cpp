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

#include "reg_ns32000.h"

#include <ctype.h>

#include "config_ns32000.h"
#include "table_ns32000.h"

namespace libasm {
namespace ns32000 {

// clang-format off
static const char TEXT_REG_R0[] PROGMEM = "R0";
static const char TEXT_REG_R1[] PROGMEM = "R1";
static const char TEXT_REG_R2[] PROGMEM = "R2";
static const char TEXT_REG_R3[] PROGMEM = "R3";
static const char TEXT_REG_R4[] PROGMEM = "R4";
static const char TEXT_REG_R5[] PROGMEM = "R5";
static const char TEXT_REG_R6[] PROGMEM = "R6";
static const char TEXT_REG_R7[] PROGMEM = "R7";

static const char TEXT_REG_FP[]  PROGMEM = "FP";
static const char TEXT_REG_SP[]  PROGMEM = "SP";
static const char TEXT_REG_SB[]  PROGMEM = "SB";
static const char TEXT_REG_PC[]  PROGMEM = "PC";
static const char TEXT_REG_TOS[] PROGMEM = "TOS";
static const char TEXT_REG_EXT[] PROGMEM = "EXT";

// NS32081/FPU Registers
static const char TEXT_REG_F0[] PROGMEM = "F0";
static const char TEXT_REG_F1[] PROGMEM = "F1";
static const char TEXT_REG_F2[] PROGMEM = "F2";
static const char TEXT_REG_F3[] PROGMEM = "F3";
static const char TEXT_REG_F4[] PROGMEM = "F4";
static const char TEXT_REG_F5[] PROGMEM = "F5";
static const char TEXT_REG_F6[] PROGMEM = "F6";
static const char TEXT_REG_F7[] PROGMEM = "F7";
// clang-format on

static const RegBase::NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_R0),
        NAME_ENTRY(REG_R1),
        NAME_ENTRY(REG_R2),
        NAME_ENTRY(REG_R3),
        NAME_ENTRY(REG_R4),
        NAME_ENTRY(REG_R5),
        NAME_ENTRY(REG_R6),
        NAME_ENTRY(REG_R7),
        NAME_ENTRY(REG_FP),
        NAME_ENTRY(REG_SP),
        NAME_ENTRY(REG_SB),
        NAME_ENTRY(REG_PC),
        NAME_ENTRY(REG_TOS),

        NAME_ENTRY(REG_F0),
        NAME_ENTRY(REG_F1),
        NAME_ENTRY(REG_F2),
        NAME_ENTRY(REG_F3),
        NAME_ENTRY(REG_F4),
        NAME_ENTRY(REG_F5),
        NAME_ENTRY(REG_F6),
        NAME_ENTRY(REG_F7),

        NAME_ENTRY(REG_EXT),
};

RegName RegNs32000::parseRegName(const char *line) {
    const NameEntry *entry = searchText(line, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

uint8_t RegNs32000::regNameLen(RegName name) {
    switch (name) {
    case REG_UNDEF:
        return 0;
    case REG_TOS:
    case REG_EXT:
        return 3;
    default:
        return 2;
    }
}

char *RegNs32000::outRegName(char *out, RegName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out = outText(out, entry->text());
    return out;
}

RegName RegNs32000::decodeRegName(uint8_t num, bool floating) {
    return RegName((num & 7) + (floating ? 8 : 0));
}

uint8_t RegNs32000::encodeRegName(RegName name) {
    if (isFloat(name))
        return uint8_t(name) - 8;
    return uint8_t(name);
}

bool RegNs32000::isGeneric(RegName name) {
    const int8_t num = int8_t(name);
    return num >= 0 && num < 8;
}

bool RegNs32000::isFloat(RegName name) {
    const int8_t num = int8_t(name) - 8;
    return num >= 0 && num < 8;
}

bool RegNs32000::isRegPair(RegName name) {
    const int8_t num = int8_t(name);
    return num >= 0 && num < 16 && num % 2 == 0;
}

// clang-format off
static const char TEXT_PREG_UPSR[]    PROGMEM = "UPSR";
static const char TEXT_PREG_US[]      PROGMEM = "US";
static const char TEXT_PREG_PSR[]     PROGMEM = "PSR";
static const char TEXT_PREG_INTBASE[] PROGMEM = "INTBASE";
static const char TEXT_PREG_MOD[]     PROGMEM = "MOD";
// clang-format on
static const RegBase::NameEntry PREG_TABLE[] PROGMEM = {
        NAME_ENTRY(PREG_UPSR),
        {PREG_UPSR, TEXT_PREG_US},
        {PREG_FP, TEXT_REG_FP},
        {PREG_SP, TEXT_REG_SP},
        {PREG_SB, TEXT_REG_SB},
        NAME_ENTRY(PREG_PSR),
        NAME_ENTRY(PREG_INTBASE),
        NAME_ENTRY(PREG_MOD),
};

PregName RegNs32000::parsePregName(const char *line) {
    const NameEntry *entry = searchText(line, ARRAY_RANGE(PREG_TABLE));
    return entry ? PregName(entry->name()) : PREG_UNDEF;
}

uint8_t RegNs32000::pregNameLen(PregName name) {
    return nameLen(uint8_t(name), ARRAY_RANGE(PREG_TABLE));
}

char *RegNs32000::outPregName(char *out, PregName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(PREG_TABLE));
    if (entry)
        out = outText(out, entry->text());
    return out;
}

PregName RegNs32000::decodePregName(uint8_t num) {
    num &= 0xF;
    if (num >= 1 && num < 8)
        return PREG_UNDEF;
    if (num == 11 || num == 12)
        return PREG_UNDEF;
    return PregName(num);
}

uint8_t RegNs32000::encodePregName(PregName name) {
    return uint8_t(name);
}

// clang-format off
// NS32082/MMU registers.
static const char TEXT_MREG_BPR0[] PROGMEM = "BPR0";
static const char TEXT_MREG_BPR1[] PROGMEM = "BPR1";
static const char TEXT_MREG_MSR[]  PROGMEM = "MSR";
static const char TEXT_MREG_BCNT[] PROGMEM = "BCNT";
static const char TEXT_MREG_PTB0[] PROGMEM = "PTB0";
static const char TEXT_MREG_PTB1[] PROGMEM = "PTB1";
static const char TEXT_MREG_EIA[]  PROGMEM = "EIA";
// clang-format on
static const RegBase::NameEntry MREG_TABLE[] PROGMEM = {
        NAME_ENTRY(MREG_BPR0),
        NAME_ENTRY(MREG_BPR1),
        NAME_ENTRY(MREG_MSR),
        NAME_ENTRY(MREG_BCNT),
        NAME_ENTRY(MREG_PTB0),
        NAME_ENTRY(MREG_PTB1),
        NAME_ENTRY(MREG_EIA),
};

MregName RegNs32000::parseMregName(const char *line) {
    const NameEntry *entry = searchText(line, ARRAY_RANGE(MREG_TABLE));
    return entry ? MregName(entry->name()) : MREG_UNDEF;
}

uint8_t RegNs32000::mregNameLen(MregName name) {
    return nameLen(uint8_t(name), ARRAY_RANGE(MREG_TABLE));
}

char *RegNs32000::outMregName(char *out, MregName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(MREG_TABLE));
    if (entry)
        out = outText(out, entry->text());
    return out;
}

MregName RegNs32000::decodeMregName(uint8_t num) {
    num &= 0xF;
    if (num == 14 || (num >= 2 && num < 10))
        return MREG_UNDEF;
    return MregName(num);
}

uint8_t RegNs32000::encodeMregName(MregName name) {
    return uint8_t(name);
}

static const char TEXT_CONFIG_I[] PROGMEM = "I";
static const char TEXT_CONFIG_F[] PROGMEM = "F";
static const char TEXT_CONFIG_M[] PROGMEM = "M";
static const char TEXT_CONFIG_C[] PROGMEM = "C";
static const RegBase::NameEntry CONFIG_TABLE[] PROGMEM = {
        NAME_ENTRY(CONFIG_I),
        NAME_ENTRY(CONFIG_M),
        NAME_ENTRY(CONFIG_F),
        NAME_ENTRY(CONFIG_C),
};

ConfigName RegNs32000::parseConfigName(const char *line) {
    const NameEntry *entry = searchText(line, ARRAY_RANGE(CONFIG_TABLE));
    return entry ? ConfigName(entry->name()) : CONFIG_UNDEF;
}

uint8_t RegNs32000::configNameLen(ConfigName name) {
    return name == CONFIG_UNDEF ? 0 : 1;
}

char *RegNs32000::outConfigNames(char *out, uint8_t configs) const {
    *out++ = '[';
    char sep = 0;
    for (uint8_t mask = 0x01; mask < 0x10; mask <<= 1) {
        if (configs & mask) {
            if (sep)
                *out++ = sep;
            const NameEntry *entry = searchName(mask, ARRAY_RANGE(CONFIG_TABLE));
            out = outText(out, entry->text());
            sep = ',';
        }
    }
    *out++ = ']';
    *out = 0;
    return out;
}

static const char TEXT_STROPT_B[] PROGMEM = "B";
static const char TEXT_STROPT_W[] PROGMEM = "W";
static const char TEXT_STROPT_U[] PROGMEM = "U";
static const RegBase::NameEntry STROPT_TABLE[] PROGMEM = {
        NAME_ENTRY(STROPT_B),
        NAME_ENTRY(STROPT_W),
        NAME_ENTRY(STROPT_U),
};

StrOptName RegNs32000::parseStrOptName(const char *line) {
    const NameEntry *entry = searchText(line, ARRAY_RANGE(STROPT_TABLE));
    return entry ? StrOptName(entry->name()) : STROPT_UNDEF;
}

uint8_t RegNs32000::strOptNameLen(StrOptName name) {
    return name == STROPT_UNDEF ? 0 : 1;
}

char *RegNs32000::outStrOptNames(char *out, uint8_t strOpts) const {
    char sep = 0;
    if (strOpts & uint8_t(STROPT_B)) {
        out = outText(out, TEXT_STROPT_B);
        sep = ',';
    }
    const NameEntry *entry = searchName(strOpts & 0xC, ARRAY_RANGE(STROPT_TABLE));
    if (entry) {
        if (sep)
            *out++ = sep;
        out = outText(out, entry->text());
    }
    *out = 0;
    return out;
}

OprSize RegNs32000::parseIndexSize(const char *line) {
    OprSize size = SZ_NONE;
    switch (toupper(*line)) {
    case 'B':
        size = SZ_BYTE;
        break;
    case 'W':
        size = SZ_WORD;
        break;
    case 'D':
        size = SZ_DOUBLE;
        break;
    case 'Q':
        size = SZ_QUAD;
        break;
    default:
        return size;
    }
    return isidchar(line[1]) ? SZ_NONE : size;
}

uint8_t RegNs32000::indexSizeLen(OprSize size) {
    return size == SZ_NONE ? 0 : 1;
}

char RegNs32000::indexSizeChar(OprSize size) const {
    char c;
    switch (size) {
    case SZ_BYTE:
        c = 'B';
        break;
    case SZ_WORD:
        c = 'W';
        break;
    case SZ_DOUBLE:
        c = 'D';
        break;
    case SZ_QUAD:
        c = 'Q';
        break;
    default:
        c = 0;
        break;
    }
    return _uppercase ? c : tolower(c);
}

}  // namespace ns32000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
