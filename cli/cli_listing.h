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

#include <stdint.h>

#include <string>

#include "config_base.h"
#include "config_host.h"

namespace libasm {
namespace cli {

class ListingLine {
public:
    virtual uint32_t startAddress() const = 0;
    virtual int generatedSize() const = 0;
    virtual uint8_t getByte(int offset) const = 0;
    virtual bool hasInstruction() const = 0;
    virtual std::string getInstruction() const = 0;
    virtual bool hasOperand() const = 0;
    virtual std::string getOperand() const = 0;

    // assemble listing only
    virtual uint16_t lineNumber() const = 0;
    virtual uint16_t includeNest() const = 0;
    virtual bool hasValue() const = 0;
    virtual uint32_t value() const = 0;
    virtual bool hasLabel() const = 0;
    virtual std::string getLabel() const = 0;
    virtual bool hasComment() const = 0;
    virtual std::string getComment() const = 0;

    // configuration
    virtual AddressWidth addressWidth() const = 0;
    virtual OpCodeWidth opCodeWidth() const = 0;
    virtual int maxBytes() const = 0;
    virtual int labelWidth() const = 0;
    virtual int instructionWidth() const = 0;
    virtual int operandWidth() const = 0;
};

class CliListing {
public:
    void reset(ListingLine &line);
    void setUppercase(bool uppercase) { _uppercase = uppercase; }
    void enableLineNumber(bool enable) { _lineNumber = enable; }
    bool hasNext() const;
    const char *getContent();
    const char *getLine();

private:
    const ListingLine *_line;
    bool _uppercase = false;
    bool _lineNumber = false;
    int _next;
    std::string _out;

    void formatHex(uint8_t val);
    void formatUint8(uint8_t val, bool fixedWidth = true, bool zeroSuppress = false);
    void formatUint12(uint16_t val, bool fixedWidth = true, bool zeroSuppress = false);
    void formatUint16(uint16_t val, bool fixedWidth = true, bool zeroSuppress = false);
    void formatUint20(uint32_t val, bool fixedWidth = true, bool zeroSuppress = false);
    void formatUint24(uint32_t val, bool fixedWidth = true, bool zeroSuppress = false);
    void formatUint32(uint32_t val, bool fixedWidth = true, bool zeroSuppress = false);
    void formatAddress(uint32_t addr, bool fixedWidth = true, bool zeroSuppress = false);
    int formatBytes(int base);
    void formatTab(size_t pos, int delta = 4);
    void formatContent(int pos);
    void formatLine();
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
