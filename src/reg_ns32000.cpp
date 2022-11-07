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

namespace libasm {
namespace ns32000 {

// clang-format off
static constexpr char TEXT_REG_R0[] PROGMEM = "R0";
static constexpr char TEXT_REG_R1[] PROGMEM = "R1";
static constexpr char TEXT_REG_R2[] PROGMEM = "R2";
static constexpr char TEXT_REG_R3[] PROGMEM = "R3";
static constexpr char TEXT_REG_R4[] PROGMEM = "R4";
static constexpr char TEXT_REG_R5[] PROGMEM = "R5";
static constexpr char TEXT_REG_R6[] PROGMEM = "R6";
static constexpr char TEXT_REG_R7[] PROGMEM = "R7";

static constexpr char TEXT_REG_FP[]  PROGMEM = "FP";
static constexpr char TEXT_REG_SP[]  PROGMEM = "SP";
static constexpr char TEXT_REG_SB[]  PROGMEM = "SB";
static constexpr char TEXT_REG_PC[]  PROGMEM = "PC";
static constexpr char TEXT_REG_TOS[] PROGMEM = "TOS";
static constexpr char TEXT_REG_EXT[] PROGMEM = "EXT";

// NS32081/FPU Registers
static constexpr char TEXT_REG_F0[] PROGMEM = "F0";
static constexpr char TEXT_REG_F1[] PROGMEM = "F1";
static constexpr char TEXT_REG_F2[] PROGMEM = "F2";
static constexpr char TEXT_REG_F3[] PROGMEM = "F3";
static constexpr char TEXT_REG_F4[] PROGMEM = "F4";
static constexpr char TEXT_REG_F5[] PROGMEM = "F5";
static constexpr char TEXT_REG_F6[] PROGMEM = "F6";
static constexpr char TEXT_REG_F7[] PROGMEM = "F7";
// clang-format on

static constexpr RegBase::NameEntry REG_TABLE[] PROGMEM = {
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

RegName RegNs32000::parseRegName(StrScanner &scan) {
    const NameEntry *entry = searchText(scan, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &RegNs32000::outRegName(StrBuffer &out, RegName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
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
static constexpr char TEXT_PREG_UPSR[]    PROGMEM = "UPSR";
static constexpr char TEXT_PREG_US[]      PROGMEM = "US";
static constexpr char TEXT_PREG_PSR[]     PROGMEM = "PSR";
static constexpr char TEXT_PREG_INTBASE[] PROGMEM = "INTBASE";
static constexpr char TEXT_PREG_MOD[]     PROGMEM = "MOD";
// clang-format on
static constexpr RegBase::NameEntry PREG_TABLE[] PROGMEM = {
        NAME_ENTRY(PREG_UPSR),
        {PREG_UPSR, TEXT_PREG_US},
        {PREG_FP, TEXT_REG_FP},
        {PREG_SP, TEXT_REG_SP},
        {PREG_SB, TEXT_REG_SB},
        NAME_ENTRY(PREG_PSR),
        NAME_ENTRY(PREG_INTBASE),
        NAME_ENTRY(PREG_MOD),
};

PregName RegNs32000::parsePregName(StrScanner &scan) {
    const NameEntry *entry = searchText(scan, ARRAY_RANGE(PREG_TABLE));
    return entry ? PregName(entry->name()) : PREG_UNDEF;
}

StrBuffer &RegNs32000::outPregName(StrBuffer &out, PregName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(PREG_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
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
static constexpr char TEXT_MREG_BPR0[] PROGMEM = "BPR0";
static constexpr char TEXT_MREG_BPR1[] PROGMEM = "BPR1";
static constexpr char TEXT_MREG_MSR[]  PROGMEM = "MSR";
static constexpr char TEXT_MREG_BCNT[] PROGMEM = "BCNT";
static constexpr char TEXT_MREG_PTB0[] PROGMEM = "PTB0";
static constexpr char TEXT_MREG_PTB1[] PROGMEM = "PTB1";
static constexpr char TEXT_MREG_EIA[]  PROGMEM = "EIA";
// clang-format on
static constexpr RegBase::NameEntry MREG_TABLE[] PROGMEM = {
        NAME_ENTRY(MREG_BPR0),
        NAME_ENTRY(MREG_BPR1),
        NAME_ENTRY(MREG_MSR),
        NAME_ENTRY(MREG_BCNT),
        NAME_ENTRY(MREG_PTB0),
        NAME_ENTRY(MREG_PTB1),
        NAME_ENTRY(MREG_EIA),
};

MregName RegNs32000::parseMregName(StrScanner &scan) {
    const NameEntry *entry = searchText(scan, ARRAY_RANGE(MREG_TABLE));
    return entry ? MregName(entry->name()) : MREG_UNDEF;
}

StrBuffer &RegNs32000::outMregName(StrBuffer &out, MregName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(MREG_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
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

static constexpr char TEXT_CONFIG_I[] PROGMEM = "I";
static constexpr char TEXT_CONFIG_F[] PROGMEM = "F";
static constexpr char TEXT_CONFIG_M[] PROGMEM = "M";
static constexpr char TEXT_CONFIG_C[] PROGMEM = "C";
static constexpr RegBase::NameEntry CONFIG_TABLE[] PROGMEM = {
        NAME_ENTRY(CONFIG_I),
        NAME_ENTRY(CONFIG_M),
        NAME_ENTRY(CONFIG_F),
        NAME_ENTRY(CONFIG_C),
};

ConfigName RegNs32000::parseConfigName(StrScanner &scan) {
    const NameEntry *entry = searchText(scan, ARRAY_RANGE(CONFIG_TABLE));
    return entry ? ConfigName(entry->name()) : CONFIG_UNDEF;
}

StrBuffer &RegNs32000::outConfigNames(StrBuffer &out, uint8_t configs) const {
    out.letter('[');
    char sep = 0;
    for (uint8_t mask = 0x01; mask < 0x10; mask <<= 1) {
        if (configs & mask) {
            if (sep)
                out.letter(sep);
            const NameEntry *entry = searchName(mask, ARRAY_RANGE(CONFIG_TABLE));
            out.text_P(entry->text_P(), isUppercase());
            sep = ',';
        }
    }
    return out.letter(']');
}

static constexpr char TEXT_STROPT_B[] PROGMEM = "B";
static constexpr char TEXT_STROPT_W[] PROGMEM = "W";
static constexpr char TEXT_STROPT_U[] PROGMEM = "U";
static constexpr RegBase::NameEntry STROPT_TABLE[] PROGMEM = {
        NAME_ENTRY(STROPT_B),
        NAME_ENTRY(STROPT_W),
        NAME_ENTRY(STROPT_U),
};

StrOptName RegNs32000::parseStrOptName(StrScanner &scan) {
    const NameEntry *entry = searchText(scan, ARRAY_RANGE(STROPT_TABLE));
    return entry ? StrOptName(entry->name()) : STROPT_UNDEF;
}

StrBuffer &RegNs32000::outStrOptNames(StrBuffer &out, uint8_t strOpts) const {
    char sep = 0;
    if (strOpts & uint8_t(STROPT_B)) {
        out.text_P(TEXT_STROPT_B, isUppercase());
        sep = ',';
    }
    const NameEntry *entry = searchName(strOpts & 0xC, ARRAY_RANGE(STROPT_TABLE));
    if (entry) {
        if (sep)
            out.letter(sep);
        out.text_P(entry->text_P(), isUppercase());
    }
    return out;
}

OprSize RegNs32000::parseIndexSize(StrScanner &scan) {
    StrScanner p(scan);
    OprSize size = SZ_NONE;
    if (p.iexpect('B')) {
        size = SZ_BYTE;
    } else if (p.iexpect('W')) {
        size = SZ_WORD;
    } else if (p.iexpect('D')) {
        size = SZ_DOUBLE;
    } else if (p.iexpect('Q')) {
        size = SZ_QUAD;
    }
    if (size == SZ_NONE || isidchar(*p))
        return SZ_NONE;
    scan = p;
    return size;
}

char RegNs32000::indexSizeChar(OprSize size) const {
    char base = isUppercase() ? 0 : 'a' - 'A';
    switch (size) {
    case SZ_BYTE:
        return base + 'B';
    case SZ_WORD:
        return base + 'W';
    case SZ_DOUBLE:
        return base + 'D';
    case SZ_QUAD:
        return base + 'Q';
    default:
        return 0;
    }
}

}  // namespace ns32000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
