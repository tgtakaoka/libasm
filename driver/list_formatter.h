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

#ifndef __CLI_LISTING_H__
#define __CLI_LISTING_H__

#include "config_base.h"
#include "str_buffer.h"
#include "value_formatter.h"

#include <cstdint>

namespace libasm {
namespace driver {

struct ListFormatter {
    virtual void setUpperHex(bool enable);

    bool hasNextContent() const;
    const char *getContent();
    virtual bool hasNextLine() const = 0;
    virtual const char *getLine() = 0;

protected:
    ListFormatter();

    const ValueFormatter _formatter;
    StrBuffer _out;
    bool _upperHex;
    char _out_buffer[256];

    void resetOut() { _out.reset(_out_buffer, sizeof(_out_buffer)); }
    int outLength() const { return _out.mark() - _out_buffer; }

    virtual uint32_t startAddress() const = 0;
    virtual int generatedSize() const = 0;
    virtual uint8_t getByte(int offset) const = 0;
    virtual const ConfigBase &config() const = 0;

    void formatDec(uint32_t val, int8_t width = 0);
    void formatHex(uint32_t val, uint8_t bits = 0, bool zeroSuppress = false);
    void formatAddress(uint32_t addr, bool fixedWidth = true);
    int bytesInLine() const { return 6; }
    int bytesColumnWidth() const { return 19; }
    int formatBytes(int base);
    void formatTab(int tabPosition, int delta = 4);
};

}  // namespace driver
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
