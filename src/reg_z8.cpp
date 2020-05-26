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

#include "config_z8.h"
#include "reg_z8.h"
#include "table_z8.h"
#include "reg_z8.h"

#include <ctype.h>

namespace libasm {
namespace z8 {

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static host::int_t parseRegNum(const char *line) {
    if (isdigit(*line) && !isidchar(line[1]))
        return *line - '0';
    if (*line++ == '1' && (*line >= '0' && *line < '6') && !isidchar(line[1]))
        return *line - '0' + 10;
    return -1;
}

RegName RegZ8::parseRegName(const char *line) const {
    if (toupper(*line++) == 'R') {
        if (toupper(*line) == 'R') {
            const host::int_t regNum = parseRegNum(line + 1);
            if (regNum >= 0 && regNum % 2 == 0)
                return RegName(regNum + 16);
        } else {
            const host::int_t regNum = parseRegNum(line);
            if (regNum >= 0)
                return RegName(regNum);
        }
    }
    return REG_UNDEF;
}

host::uint_t RegZ8::regNameLen(RegName regName) {
    const host::int_t num = host::int_t(regName);
    if (num >= 16 + 10) return 4; // RRnn
    if (num >= 10)      return 3; // RRn, Rnn
    if (num >= 0)       return 2; // Rn
    return 0;
}

uint8_t RegZ8::encodeRegName(RegName regName) {
    const host::int_t num = host::int_t(regName);
    if (num >= 16) return num - 16; // RRn
    if (num >= 0)  return num;      // Rn
    return 0;
}

RegName RegZ8::decodeRegNum(uint8_t regNum, bool pair) {
    regNum &= 0x0f;
    if (pair)
        return regNum % 2 == 0 ? RegName(regNum + 16) : REG_UNDEF;
    return RegName(regNum);
}

char *RegZ8::outRegName(char *out, RegName regName) const {
    host::int_t num = host::int_t(regName);
    if (num >= 0) {
        const char r = _uppercase ? 'R' : 'r';
        *out++ = r;
        if (num >= 16) {
            *out++ = r;
            num -= 16;
        }
        if (num >= 10) {
            *out++ = '1';
            num -= 10;
        }
        *out++ = num + '0';
    }
    *out = 0;
    return out;
}

#define CC_ENTRY(name, code, len, ...) \
    static_cast<uint8_t>(name), ((code) | ((len) << 4)), __VA_ARGS__
static const uint8_t CC_TABLE[] PROGMEM = {
    CC_ENTRY(CC_F,    0, 1, 'F'),
    CC_ENTRY(CC_LT,   1, 2, 'L', 'T'),
    CC_ENTRY(CC_LE,   2, 2, 'L', 'E'),
    CC_ENTRY(CC_ULE,  3, 3, 'U', 'L', 'E'),
    CC_ENTRY(CC_OV,   4, 2, 'O', 'V'),
    CC_ENTRY(CC_MI,   5, 2, 'M', 'I'),
    CC_ENTRY(CC_Z,    6, 1, 'Z'),
    CC_ENTRY(CC_C,    7, 1, 'C'),
    CC_ENTRY(CC_T,    8, 0)
    CC_ENTRY(CC_GE,   9, 2, 'G', 'E'),
    CC_ENTRY(CC_GT,  10, 2, 'G', 'T'),
    CC_ENTRY(CC_UGT, 11, 3, 'U', 'G', 'T'),
    CC_ENTRY(CC_NOV, 12, 3, 'N', 'O', 'V'),
    CC_ENTRY(CC_PL,  13, 2, 'P', 'L'),
    CC_ENTRY(CC_NZ,  14, 2, 'N', 'Z'),
    CC_ENTRY(CC_NC,  15, 2, 'N', 'C'),
    CC_ENTRY(CC_EQ,   6, 2, 'E', 'Q'),
    CC_ENTRY(CC_ULT,  7, 3, 'U', 'L', 'T'),
    CC_ENTRY(CC_NE,  14, 2, 'N', 'E'),
    CC_ENTRY(CC_UGE, 15, 3, 'U', 'G', 'E'),
};
static CcName CC_NAME(host::uint_t idx) {
    return CcName(pgm_read_byte(&CC_TABLE[idx]));
}
static uint8_t CC_CODE(host::uint_t idx) {
    return pgm_read_byte(&CC_TABLE[idx + 1]) & 0xf;
}
static host::uint_t CC_LEN(host::uint_t idx)  {
    return pgm_read_byte(&CC_TABLE[idx + 1]) >> 4;
}
static const char *CC_TEXT(host::uint_t idx) {
    return reinterpret_cast<const char *>(&CC_TABLE[idx + 2]);
}
static host::uint_t CC_NEXT(host::uint_t idx) {
    return idx + 2 + CC_LEN(idx);
}

CcName RegZ8::parseCcName(const char *line) const {
    for (host::uint_t idx = 0; idx < sizeof(CC_TABLE); idx = CC_NEXT(idx)) {
        const host::uint_t len = CC_LEN(idx);
        if (len && pgm_strncasecmp(line, CC_TEXT(idx), len) == 0)
            return CC_NAME(idx);
    }
    return CC_UNDEF;
}

host::uint_t RegZ8::ccNameLen(const CcName ccName) {
    for (host::uint_t idx = 0; idx < sizeof(CC_TABLE); idx = CC_NEXT(idx)) {
        if (ccName == CC_NAME(idx))
            return CC_LEN(idx);
    }
    return 0;
}

host::int_t RegZ8::encodeCcName(CcName ccName) {
    for (host::uint_t idx = 0; idx < sizeof(CC_TABLE); idx = CC_NEXT(idx)) {
        if (ccName == CC_NAME(idx))
            return CC_CODE(idx);
    }
    return -1;
}

CcName RegZ8::decodeCcNum(uint8_t ccNum) {
    for (host::uint_t idx = 0; idx < sizeof(CC_TABLE); idx = CC_NEXT(idx)) {
        if (ccNum == CC_CODE(idx))
            return CC_NAME(idx);
    }
    return CC_UNDEF;
}

char *RegZ8::outCcName(char *out, CcName ccName) const {
    for (host::uint_t idx = 0; idx < sizeof(CC_TABLE); idx = CC_NEXT(idx)) {
        if (ccName != CC_NAME(idx)) continue;
        const char *text = CC_TEXT(idx);
        const host::uint_t len = CC_LEN(idx);
        for (host::uint_t i = 0; i < len; i++) {
            const char c = pgm_read_byte(&text[i]);
            *out++ = _uppercase ? c : tolower(c);
        }
        break;
    }
    *out = 0;
    return out;
}

} // namespace z8
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
