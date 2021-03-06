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

#include <stdint.h>

#include "config_base.h"
#include "config_host.h"

namespace libasm {
namespace cli {

class BinFormatter {
public:
    BinFormatter(AddressWidth addrWidth);
    virtual ~BinFormatter();

    virtual const char *begin() = 0;
    virtual const char *prepare(uint32_t addr) = 0;
    virtual const char *encode(
            uint32_t addr, const uint8_t *data, uint8_t size) = 0;
    virtual const char *end() = 0;
    virtual uint8_t *decode(
            const char *line, uint32_t &addr, uint8_t &size) = 0;

protected:
    AddressWidth _addrWidth;
    char *_line;
    uint8_t _line_len;
    uint8_t *_data;
    uint8_t _data_len;
    uint8_t _check_sum;

    void ensureLine(uint8_t len);
    void ensureData(uint8_t len);
    void resetSum();
    void addSum(uint8_t data);
    void addSum(uint16_t data);
    void addSum(uint32_t data);
    virtual uint8_t getSum() const = 0;
    bool parseByte(const char *&line, uint8_t &val);
    bool parseUint16(const char *&line, uint16_t &val);
};

class IntelHex : public BinFormatter {
public:
    IntelHex(AddressWidth addrWidth);

    const char *begin() override;
    const char *prepare(uint32_t addr) override;
    const char *encode(
            uint32_t addr, const uint8_t *data, uint8_t size) override;
    const char *end() override;
    uint8_t *decode(const char *line, uint32_t &addr, uint8_t &size) override;

private:
    uint16_t _ela;  // Extended Linear Address

    uint8_t getSum() const override;
};

class MotoSrec : public BinFormatter {
public:
    MotoSrec(AddressWidth addrWidth);

    const char *begin() override;
    const char *prepare(uint32_t addr) override { return nullptr; }
    const char *encode(
            uint32_t addr, const uint8_t *data, uint8_t size) override;
    const char *end() override;
    uint8_t *decode(const char *line, uint32_t &addr, uint8_t &size) override;

private:
    uint8_t getSum() const override;
};

}  // namespace cli
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
