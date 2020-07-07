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

static const char TEXT_REG_FLAGS[]   PROGMEM = "FLAGS";
static const char TEXT_REG_FCW[]     PROGMEM = "FCW";
static const char TEXT_REG_REFRESH[] PROGMEM = "REFRESH";
static const char TEXT_REG_PSAPSEG[] PROGMEM = "PSAPSEG";
static const char TEXT_REG_PSAPOFF[] PROGMEM = "PSAPOFF";
static const char TEXT_REG_NSPSEG[] PROGMEM  = "NSPSEG";
static const char TEXT_REG_NSPOFF[] PROGMEM  = "NSPOFF";

static const char TEXT_IT_VI[] PROGMEM  = "VI";
static const char TEXT_IT_NVI[] PROGMEM = "NVI";

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
        const char type = *line;
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
    }
    return REG_UNDEF;
}

uint8_t RegZ8000::regNameLen(RegName regName) const {
    if (isByteReg(regName))  return 3;
    const int8_t num = int8_t(regName);
    if (num < 0) return 0;
    return (num & 0xF) < 10 ? 3 : 4;
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

RegName RegZ8000::decodeCtlReg(uint8_t ctlNum) const {
    ctlNum &= 7;
    return ctlNum == 0 ? REG_ILLEGAL : RegName(ctlNum + 64);
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
    if (num >= 0) {
        if (isCtlReg(regName)) {
            if (regName == REG_FLAGS)   out = outText(out, TEXT_REG_FLAGS);
            if (regName == REG_FCW)     out = outText(out, TEXT_REG_FCW);
            if (regName == REG_REFRESH) out = outText(out, TEXT_REG_REFRESH);
            if (regName == REG_PSAPSEG) out = outText(out, TEXT_REG_PSAPSEG);
            if (regName == REG_PSAPOFF) out = outText(out, TEXT_REG_PSAPOFF);
            if (regName == REG_NSPSEG)  out = outText(out, TEXT_REG_NSPSEG);
            if (regName == REG_NSPOFF)  out = outText(out, TEXT_REG_NSPOFF);
        } else {
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
        }
    }
    *out = 0;
    return out;
}

#define CC_ENTRY(name, code, len, ...)                                  \
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
static CcName CC_NAME(uint8_t idx) {
    return CcName(pgm_read_byte(&CC_TABLE[idx]));
}
static uint8_t CC_CODE(uint8_t idx) {
    return pgm_read_byte(&CC_TABLE[idx + 1]) & 0xf;
}
static uint8_t CC_LEN(uint8_t idx)  {
    return pgm_read_byte(&CC_TABLE[idx + 1]) >> 4;
}
static const char *CC_TEXT(uint8_t idx) {
    return reinterpret_cast<const char *>(&CC_TABLE[idx + 2]);
}
static uint8_t CC_NEXT(uint8_t idx) {
    return idx + 2 + CC_LEN(idx);
}

CcName RegZ8000::parseCcName(const char *line) const {
    for (uint8_t idx = 0; idx < sizeof(CC_TABLE); idx = CC_NEXT(idx)) {
        const uint8_t len = CC_LEN(idx);
        const char *text = CC_TEXT(idx);
        if (len && strncasecmp_P(line, text, len) == 0
            && !isidchar(line[len]))
            return CC_NAME(idx);
    }
    return CC_UNDEF;
}

uint8_t RegZ8000::ccNameLen(const CcName ccName) const {
    for (uint8_t idx = 0; idx < sizeof(CC_TABLE); idx = CC_NEXT(idx)) {
        if (ccName == CC_NAME(idx))
            return CC_LEN(idx);
    }
    return 0;
}

int8_t RegZ8000::encodeCcName(CcName ccName) const {
    for (uint8_t idx = 0; idx < sizeof(CC_TABLE); idx = CC_NEXT(idx)) {
        if (ccName == CC_NAME(idx))
            return CC_CODE(idx);
    }
    return -1;
}

CcName RegZ8000::decodeCcNum(uint8_t ccNum) const {
    ccNum &= 0xF;
    for (uint8_t idx = 0; idx < sizeof(CC_TABLE); idx = CC_NEXT(idx)) {
        if (ccNum == CC_CODE(idx))
            return CC_NAME(idx);
    }
    return CC_UNDEF;
}

char *RegZ8000::outCcName(char *out, CcName ccName) const {
    for (uint8_t idx = 0; idx < sizeof(CC_TABLE); idx = CC_NEXT(idx)) {
        if (ccName != CC_NAME(idx)) continue;
        const char *text = CC_TEXT(idx);
        const uint8_t len = CC_LEN(idx);
        for (uint8_t i = 0; i < len; i++)
            out = outChar(out, pgm_read_byte(&text[i]));
        break;
    }
    *out = 0;
    return out;
}

char *RegZ8000::outFlagNames(char *out, uint8_t flagNames) const {
    char c = 0;
    if (flagNames & int8_t(FL_C)) {
        out = outChar(out, 'C');
        c = ',';
    }
    if (flagNames & int8_t(FL_Z)) {
        if (c) *out++ = c;
        out = outChar(out, 'Z');
        c = ',';
    }
    if (flagNames & int8_t(FL_S)) {
        if (c) *out++ = c;
        out = outChar(out, 'S');
        c = ',';
    }
    if (flagNames & int8_t(FL_P)) {
        if (c) *out++ = c;
        out = outChar(out, 'P');
    }
    return out;
}

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
    return out;
}

} // namespace z8000
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
