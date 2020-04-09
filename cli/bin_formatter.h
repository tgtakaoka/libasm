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

#ifndef __BIN_FORMATTER_H__
#define __BIN_FORMATTER_H__

#include "config_host.h"

#include <stdint.h>

namespace libasm {
namespace cli {

class BinFormatter {
public:
    BinFormatter(host::uint_t addrWidth);
    virtual ~BinFormatter();

    virtual const char *begin() = 0;
    virtual const char *encode(
        uint32_t addr, const uint8_t *data, host::uint_t size) = 0;
    virtual const char *end() = 0;
    virtual uint8_t *decode(
        const char *line, uint32_t &addr, host::uint_t &size) = 0;

protected:
    const host::uint_t _addrWidth;
    char *_line;
    host::uint_t _line_len;
    uint8_t *_data;
    host::uint_t _data_len;
    uint8_t _check_sum;

    void ensureLine(host::uint_t len);
    void ensureData(host::uint_t len);
    void resetSum();
    void addSum(uint8_t data);
    void addSum(uint16_t data);
    void addSum(uint32_t data);
    bool parseByte(const char *&line, uint8_t &val);
    bool parseUint16(const char *&line, uint16_t &val);
};

class IntelHex : public BinFormatter {
public:
    IntelHex(host::uint_t addrWidth);

    const char *begin() override;
    const char *encode(
        uint32_t addr, const uint8_t *data, host::uint_t size) override;
    const char *end() override;
    uint8_t *decode(
        const char *line, uint32_t &addr, host::uint_t &size) override;
};

class MotoSrec : public BinFormatter {
public:
    MotoSrec(host::uint_t addrWidth);

    const char *begin() override;
    const char *encode(
        uint32_t addr, const uint8_t *data, host::uint_t size) override;
    const char *end() override;
    uint8_t *decode(
        const char *line, uint32_t &addr, host::uint_t &size) override;
};

} // namespace cli
} // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
