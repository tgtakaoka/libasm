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

#include "config_z8000.h"
#include "reg_z8000.h"
#include "table_z8000.h"

#include <ctype.h>

namespace libasm {
namespace z8000 {

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static int8_t parseRegNum(const char *line) {
    if (isdigit(*line) && !isidchar(line[1]))
        return *line - '0';
    if (*line++ == '1'
        && *line >= '0' && *line < '6' && !isidchar(line[1]))
        return *line - '0' + 10;
    return -1;
}

RegName RegZ8000::parseRegName(const char *line) const {
    if (toupper(*line++) == 'R') {
        const char type = toupper(*line);
        if (type == 'H' || type == 'L' || type == 'R' || type == 'Q')
            line++;
        int8_t num = parseRegNum(line);
        if (num < 0) return REG_UNDEF;
        if (type == 'H')
            return num < 8 ? RegName(num + 16) : REG_ILLEGAL;
        if (type == 'L')
            return num < 8 ? RegName(num + 24) : REG_ILLEGAL;
        if (type == 'R')
            return (num % 2) == 0 ? RegName(num + 32) : REG_ILLEGAL;
        if (type == 'Q')
            return (num % 4) == 0 ? RegName(num + 48) : REG_ILLEGAL;
        return RegName(num);
    }
    return REG_UNDEF;
}

uint8_t RegZ8000::regNameLen(RegName regName) const {
    if (isCtlReg(regName)) return ctlRegLen(regName);
    const int8_t num = int8_t(regName);
    if (num < 0) return 0;
    if (isByteReg(regName)) return 3;
    const uint8_t len = isWordReg(regName) ? 1 : 2;
    return len + ((num & 0xF) < 10 ? 1 : 2);
}

uint8_t RegZ8000::encodeRegName(RegName regName) const {
    const int8_t num = int8_t(regName);
    if (isByteReg(regName)) return num - 16;
    if (isLongReg(regName)) return num - 32;
    if (isQuadReg(regName)) return num - 48;
    return num;
}

RegName RegZ8000::decodeRegNum(uint8_t regNum, OprSize size) const {
    switch (size) {
    case SZ_BYTE: return decodeByteReg(regNum);
    case SZ_WORD: return decodeWordReg(regNum);
    case SZ_LONG: return decodeLongReg(regNum);
    case SZ_QUAD: return decodeQuadReg(regNum);
    case SZ_ADDR: return TableZ8000.isSegmentModel()
            ? decodeLongReg(regNum) : decodeWordReg(regNum);
    default: return REG_UNDEF;
    }
}

RegName RegZ8000::decodeWordReg(uint8_t regNum) const {
    regNum &= 0x0f;
    return RegName(regNum);
}

RegName RegZ8000::decodeByteReg(uint8_t regNum) const {
    regNum &= 0x0f;
    return RegName(regNum + 16);
}

RegName RegZ8000::decodeLongReg(uint8_t regNum) const {
    regNum &= 0x0f;
    return regNum % 2 == 0 ? RegName(regNum + 32) : REG_ILLEGAL;
}

RegName RegZ8000::decodeQuadReg(uint8_t regNum) const {
    regNum &= 0x0f;
    return regNum % 4 == 0 ? RegName(regNum + 48) : REG_ILLEGAL;
}

bool RegZ8000::isWordReg(RegName regName) const {
    const int8_t r = static_cast<int8_t>(regName);
    return r >= 0 && r < 16;
}

bool RegZ8000::isByteReg(RegName regName) const {
    const int8_t r = static_cast<int8_t>(regName);
    return r >= 16 && r < 16 + 16;
}

bool RegZ8000::isLongReg(RegName regName) const {
    const int8_t r = static_cast<int8_t>(regName);
    return r >= 32 && r < 32 + 16;
}

bool RegZ8000::isQuadReg(RegName regName) const {
    const int8_t r = static_cast<int8_t>(regName);
    return r >= 48 && r < 48 + 16;
}

bool RegZ8000::isCtlReg(RegName regName) const {
    const int8_t r = static_cast<int8_t>(regName);
    return r >= 64 && r < 64 + 8;
}

char *RegZ8000::outChar(char *out, char c) const {
    *out++ = _uppercase ? toupper(c) : tolower(c);
    *out = 0;
    return out;
}

char *RegZ8000::outText(char *out, const char *text) const {
    while (true) {
        const char c = pgm_read_byte(text);
        if (c == 0) break;
        out = outChar(out, c);
        text++;
    }
    return out;
}

char *RegZ8000::outRegName(char *out, RegName regName) const {
    int8_t num = int8_t(regName);
    if (num < 0) return out;
    if (isCtlReg(regName)) return outCtlName(out, regName);

    out = outChar(out, 'R');
    if (isByteReg(regName)) {
        num -= 16;
        if (num < 8) {
            out = outChar(out, 'H');
        } else {
            num -= 8;
            out = outChar(out, 'L');
        }
    } else if (isLongReg(regName)) {
        num -= 32;
        out = outChar(out, 'R');
    } else if (isQuadReg(regName)) {
        num -= 48;
        out = outChar(out, 'Q');
    }
    if (num >= 10) {
        *out++ = '1';
        num -= 10;
    }
    *out++ = num + '0';
    *out = 0;
    return out;
}

struct NameEntry {
    uint8_t name;
    uint8_t codeAndLen;
    const char *text;

    static const NameEntry *searchName(
            uint8_t name, const NameEntry *begin, const NameEntry *end) {
        for (const NameEntry *entry = begin; entry < end; entry++) {
            const uint8_t n = pgm_read_byte(&entry->name);
            if (name == n) return entry;
        }
        return nullptr;
    }
    static const NameEntry *searchCode(
            uint8_t code, const NameEntry *begin, const NameEntry *end) {
        for (const NameEntry *entry = begin; entry < end; entry++) {
            const uint8_t c = (pgm_read_byte(&entry->codeAndLen) >> 4);
            if (code == c) return entry;
        }
        return nullptr;
    }
    static const NameEntry *searchText(
            const char *str, const NameEntry *begin, const NameEntry *end) {
        for (const NameEntry *entry = begin; entry < end; entry++) {
            const char *text = reinterpret_cast<const char *>(
                    pgm_read_ptr(&entry->text));
            const uint8_t len = pgm_read_byte(&entry->codeAndLen) & 0xF;
            if (strncasecmp_P(str, text, len) == 0 && !isidchar(str[len]))
                return entry;
        }
        return nullptr;
    }
    static uint8_t nameLen(
            uint8_t name, const NameEntry *begin, const NameEntry *end) {
        const NameEntry *entry = searchName(name, begin, end);
        return entry ? (pgm_read_byte(&entry->codeAndLen) & 0xF) : 0;
    }
};

static const char TEXT_REG_FLAGS[]   PROGMEM = "FLAGS";
static const char TEXT_REG_FCW[]     PROGMEM = "FCW";
static const char TEXT_REG_REFRESH[] PROGMEM = "REFRESH";
static const char TEXT_REG_PSAPSEG[] PROGMEM = "PSAPSEG";
static const char TEXT_REG_PSAPOFF[] PROGMEM = "PSAPOFF";
static const char TEXT_REG_NSPSEG[] PROGMEM  = "NSPSEG";
static const char TEXT_REG_NSPOFF[] PROGMEM  = "NSPOFF";
static const NameEntry CTL_TABLE[] PROGMEM = {
    { REG_FLAGS,   0x15, TEXT_REG_FLAGS   },
    { REG_FCW,     0x23, TEXT_REG_FCW     },
    { REG_REFRESH, 0x37, TEXT_REG_REFRESH },
    { REG_PSAPSEG, 0x47, TEXT_REG_PSAPSEG },
    { REG_PSAPOFF, 0x57, TEXT_REG_PSAPOFF },
    { REG_NSPSEG,  0x66, TEXT_REG_NSPSEG  },
    { REG_NSPOFF,  0x76, TEXT_REG_NSPOFF  },
};

RegName RegZ8000::parseCtlReg(const char *line) const {
    const NameEntry *entry =
        NameEntry::searchText(line, ARRAY_RANGE(CTL_TABLE));
    return entry ? RegName(pgm_read_byte(&entry->name)) : REG_UNDEF;
}

RegName RegZ8000::decodeCtlReg(uint8_t ctlNum) const {
    const NameEntry *entry =
        NameEntry::searchCode(ctlNum & 7, ARRAY_RANGE(CTL_TABLE));
    return entry ? RegName(pgm_read_byte(&entry->name)) : REG_ILLEGAL;
}

uint8_t RegZ8000::ctlRegLen(RegName regName) const {
    return NameEntry::nameLen(regName, ARRAY_RANGE(CTL_TABLE));
}

char *RegZ8000::outCtlName(char *out, RegName regName) const {
    const NameEntry *entry =
        NameEntry::searchName(regName, ARRAY_RANGE(CTL_TABLE));
    if (entry) {
        const char *text = reinterpret_cast<const char *>(
                pgm_read_ptr(&entry->text));
        out = outText(out, text);
    }
    return out;
}

int8_t RegZ8000::encodeCtlReg(RegName ctlReg) const {
    const NameEntry *entry =
        NameEntry::searchName(ctlReg, ARRAY_RANGE(CTL_TABLE));
    return entry ? (pgm_read_byte(&entry->codeAndLen) >> 4) : -1;
}

static const char TEXT_IT_VI[] PROGMEM  = "VI";
static const char TEXT_IT_NVI[] PROGMEM = "NVI";
static const NameEntry IT_TABLE[] PROGMEM = {
    { IT_NVI, 0x13, TEXT_IT_NVI },
    { IT_VI,  0x22, TEXT_IT_VI  },
};

char *RegZ8000::outIntrNames(char *out, uint8_t intrNames) const {
    char c = 0;
    if ((intrNames & int8_t(IT_VI)) == 0) {
        out = outText(out, TEXT_IT_VI);
        c = ',';
    }
    if ((intrNames & int8_t(IT_NVI)) == 0) {
        if (c) *out++ = c;
        out = outText(out, TEXT_IT_NVI);
    }
    *out = 0;
    return out;
}

IntrName RegZ8000::parseIntrName(const char *p) const {
    const NameEntry *entry =
        NameEntry::searchText(p, ARRAY_RANGE(IT_TABLE));
    return entry ? IntrName(pgm_read_byte(&entry->name)) : IT_UNDEF;
}

uint8_t RegZ8000::intrNameLen(IntrName intrName) const {
    const NameEntry *entry =
        NameEntry::searchName(intrName, ARRAY_RANGE(IT_TABLE));
    return entry ? (pgm_read_byte(&entry->codeAndLen) & 0xF) : 0;
}

uint8_t RegZ8000::encodeIntrName(IntrName intrName) const {
    const NameEntry *entry =
        NameEntry::searchName(intrName, ARRAY_RANGE(IT_TABLE));
    return entry ? (pgm_read_byte(&entry->codeAndLen) >> 4) : 0;
}

static const char *TEXT_CC_F   PROGMEM = "F";
static const char *TEXT_CC_LT  PROGMEM = "LT";
static const char *TEXT_CC_LE  PROGMEM = "LE";
static const char *TEXT_CC_ULE PROGMEM = "ULE";
static const char *TEXT_CC_OV  PROGMEM = "OV";
static const char *TEXT_CC_MI  PROGMEM = "MI";
static const char *TEXT_CC_Z   PROGMEM = "Z";
static const char *TEXT_CC_C   PROGMEM = "C";
static const char *TEXT_CC_T   PROGMEM = "";
static const char *TEXT_CC_GE  PROGMEM = "GE";
static const char *TEXT_CC_GT  PROGMEM = "GT";
static const char *TEXT_CC_UGT PROGMEM = "UGT";
static const char *TEXT_CC_NOV PROGMEM = "NOV";
static const char *TEXT_CC_PL  PROGMEM = "PL";
static const char *TEXT_CC_NZ  PROGMEM = "NZ";
static const char *TEXT_CC_NC  PROGMEM = "NC";
static const char *TEXT_CC_EQ  PROGMEM = "EQ";
static const char *TEXT_CC_ULT PROGMEM = "ULT";
static const char *TEXT_CC_NE  PROGMEM = "NE";
static const char *TEXT_CC_UGE PROGMEM = "UGE";
static const NameEntry CC_TABLE[] PROGMEM = {
    { CC_F,   0x01, TEXT_CC_F   },
    { CC_LT,  0x12, TEXT_CC_LT  },
    { CC_LE,  0x22, TEXT_CC_LE  },
    { CC_ULE, 0x33, TEXT_CC_ULE },
    { CC_OV,  0x42, TEXT_CC_OV  },
    { CC_MI,  0x52, TEXT_CC_MI  },
    { CC_Z,   0x61, TEXT_CC_Z   },
    { CC_C,   0x71, TEXT_CC_C   },
    { CC_GE,  0x92, TEXT_CC_GE  },
    { CC_GT,  0xA2, TEXT_CC_GT  },
    { CC_UGT, 0xB3, TEXT_CC_UGT },
    { CC_NOV, 0xC3, TEXT_CC_NOV },
    { CC_PL,  0xD2, TEXT_CC_PL  },
    { CC_NZ,  0xE2, TEXT_CC_NZ  },
    { CC_NC,  0xF2, TEXT_CC_NC  },
    { CC_EQ,  0x62, TEXT_CC_EQ  },
    { CC_ULT, 0x73, TEXT_CC_ULT },
    { CC_NE,  0xE2, TEXT_CC_NE  },
    { CC_UGE, 0xF3, TEXT_CC_UGE },
    { CC_T,   0x80, TEXT_CC_T   },
};

CcName RegZ8000::parseCcName(const char *line) const {
    const NameEntry *entry =
        NameEntry::searchText(line, ARRAY_RANGE(CC_TABLE));
    const CcName cc = entry ? CcName(pgm_read_byte(&entry->name)) : CC_UNDEF;
    return cc == CC_T ? CC_UNDEF : cc;
}

uint8_t RegZ8000::ccNameLen(const CcName ccName) const {
    const NameEntry *entry =
        NameEntry::searchName(ccName, ARRAY_RANGE(CC_TABLE));
    return entry ? (pgm_read_byte(&entry->codeAndLen) & 0xF) : 0;
}

int8_t RegZ8000::encodeCcName(CcName ccName) const {
    const NameEntry *entry =
        NameEntry::searchName(ccName, ARRAY_RANGE(CC_TABLE));
    return entry ? (pgm_read_byte(&entry->codeAndLen) >> 4) : -1;
}

CcName RegZ8000::decodeCcNum(uint8_t ccNum) const {
    const NameEntry *entry =
        NameEntry::searchCode(ccNum & 0xF, ARRAY_RANGE(CC_TABLE));
    return entry ? CcName(pgm_read_byte(&entry->name)) : CC_UNDEF;
}

char *RegZ8000::outCcName(char *out, CcName ccName) const {
    const NameEntry *entry =
        NameEntry::searchName(ccName, ARRAY_RANGE(CC_TABLE));
    const char *text =
        reinterpret_cast<const char *>(pgm_read_ptr(&entry->text));
    return outText(out, text);
}

static const char *TEXT_FL_C PROGMEM = "C";
static const char *TEXT_FL_Z PROGMEM = "Z";
static const char *TEXT_FL_S PROGMEM = "S";
static const char *TEXT_FL_P PROGMEM = "P";
static const char *TEXT_FL_V PROGMEM = "V";
static const NameEntry FLAG_TABLE[] PROGMEM = {
    { FL_C, 0x81, TEXT_FL_C },
    { FL_Z, 0x41, TEXT_FL_Z },
    { FL_S, 0x21, TEXT_FL_S },
    { FL_P, 0x11, TEXT_FL_P },
    { FL_V, 0x11, TEXT_FL_V },
};

char *RegZ8000::outFlagNames(char *out, uint8_t flags) const {
    char c = 0;
    for (uint8_t bit = 0x8; bit; bit >>= 1) {
        if (flags & bit) {
            if (c) *out++ = c;
            c = ',';
            const NameEntry *entry =
                NameEntry::searchCode(bit, ARRAY_RANGE(FLAG_TABLE));
            if (entry) {
                const char *text = reinterpret_cast<const char *>(
                        pgm_read_ptr(&entry->text));
                out = outText(out, text);
            }
        }
    }
    return out;
}

FlagName RegZ8000::parseFlagName(const char *p) const {
    const NameEntry *entry =
        NameEntry::searchText(p, ARRAY_RANGE(FLAG_TABLE));
    return entry ? FlagName(pgm_read_byte(&entry->name)) : FL_UNDEF;
}

uint8_t RegZ8000::flagNameLen(FlagName flag) const {
    return flag == FL_UNDEF ? 0 : 1;
}

uint8_t RegZ8000::encodeFlagName(FlagName flag) const {
    const NameEntry *entry =
        NameEntry::searchName(flag, ARRAY_RANGE(FLAG_TABLE));
    return entry ? (pgm_read_byte(&entry->codeAndLen) >> 4) : 0;
}

} // namespace z8000
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
