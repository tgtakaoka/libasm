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
#include "config_host.h"
#include "str_buffer.h"
#include "value_formatter.h"

#include <cstdint>

namespace libasm {
namespace driver {

class ListFormatter {
public:
    virtual void setUppercase(bool uppercase);

    bool hasNextLine() const;
    virtual const char *getLine();

protected:
    ValueFormatter _formatter{/*cstyle*/ false};
    bool _uppercase = false;
    int _nextLine;
    bool _errorLine;
    char _outBuffer[256];
    StrBuffer _out{_outBuffer, sizeof(_outBuffer)};

    virtual void reset();

    virtual uint32_t startAddress() const = 0;
    virtual int generatedSize() const = 0;
    virtual uint8_t getByte(int offset) const = 0;
    virtual bool hasInstruction() const = 0;
    virtual const StrScanner getInstruction() const = 0;
    virtual bool hasOperand() const = 0;
    virtual const StrScanner  getOperand() const = 0;

    // assemble listing only
    virtual uint32_t lineNumber() const { return 0; }
    virtual int16_t columnNumber() const { return -1; }
    virtual uint16_t includeNest() const { return 0; }
    virtual bool hasValue() const { return false; }
    virtual uint32_t value() const { return 0; }
    virtual bool hasLabel() const { return false; }
    virtual const StrScanner getLabel() const { return StrScanner::EMPTY; }
    virtual bool hasComment() const { return false; }
    virtual const StrScanner getComment() const { return StrScanner::EMPTY; }

    // configuration
    virtual const ConfigBase &config() const = 0;
    virtual int labelWidth() const = 0;
    virtual int nameWidth() const = 0;
    virtual int codeBytes() const = 0;
    virtual int operandWidth() const = 0;

    void formatDec(uint32_t val, int8_t width = 0);
    void formatHex(uint32_t val, uint8_t bits = 0, bool zeroSuppress = false);
    void formatAddress(uint32_t addr, bool fixedWidth = true, bool zeroSuppress = false);
    virtual int formatBytes(int base);
    void formatTab(size_t pos, int delta = 4);
    void formatContent(int pos);
    virtual void formatLine();
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
