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

#include "bin_formatter.h"

#include <ctype.h>
#include <stdlib.h>

namespace libasm {
namespace cli {

static uint8_t addressSize(AddressWidth addrWidth) {
    const uint8_t bits = uint8_t(addrWidth);
    if (bits <= 16)
        return 2;
    if (bits <= 24)
        return 3;
    return 4;
}

BinFormatter::BinFormatter(AddressWidth addrWidth)
    : _addrSize(addressSize(addrWidth)),
      _line(nullptr),
      _line_len(0),
      _data(nullptr),
      _data_len(0) {}

BinFormatter::~BinFormatter() {
    if (_line)
        free(_line);
    if (_data)
        free(_data);
}

void BinFormatter::format(const char *text) const {
    if (_out) {
        fprintf(_out, "%s\n", text);
        fflush(_out);
    }
}

void BinFormatter::ensureLine(uint8_t len) {
    len += 1;
    if (_line_len < len) {
        _line = static_cast<char *>(realloc(_line, len));
        _line_len = len;
    }
}

void BinFormatter::ensureData(uint8_t len) {
    if (_data_len < len) {
        _data = static_cast<uint8_t *>(realloc(_data, len));
        _data_len = len;
    }
}

void BinFormatter::resetSum() {
    _check_sum = 0;
}

void BinFormatter::addSum(uint8_t data) {
    _check_sum += data;
}

void BinFormatter::addSum(uint16_t data) {
    addSum(static_cast<uint8_t>(data >> 8));
    addSum(static_cast<uint8_t>(data));
}

void BinFormatter::addSum(uint32_t data) {
    addSum(static_cast<uint16_t>(data >> 16));
    addSum(static_cast<uint16_t>(data));
}

static uint8_t parseHex(char c) {
    return isdigit(c) ? c - '0' : toupper(c) - 'A' + 10;
}

bool BinFormatter::parseByte(const char *&line, uint8_t &val) {
    if (!isxdigit(*line))
        return true;
    val = parseHex(*line++) << 4;
    if (!isxdigit(*line))
        return true;
    val |= parseHex(*line++);
    addSum(val);
    return false;
}

bool BinFormatter::parseUint16(const char *&line, uint16_t &val) {
    uint8_t val8 = 0;
    if (parseByte(line, val8))
        return true;
    val = static_cast<uint16_t>(val8) << 8;
    if (parseByte(line, val8))
        return true;
    val |= val8;
    return false;
}
}  // namespace cli
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
