/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __BIN_DECODER_H__
#define __BIN_DECODER_H__

#include "bin_memory.h"
#include "str_scanner.h"
#include "text_reader.h"

#include <stdint.h>

namespace libasm {
namespace cli {

class BinDecoder {
public:
    /**
     * Decode text format binary into |memory|.
     * @return: number of read bytes or negative if error.
     */
    static int decode(TextReader &in, BinMemory &memory);

protected:
    virtual int decode(StrScanner &line, BinMemory &memory) = 0;

    static bool parseByte(StrScanner &line, uint8_t &val);
    static bool parseUint16(StrScanner &line, uint16_t &val);
    static bool parseUint24(StrScanner &line, uint32_t &val);
    static bool parseUint32(StrScanner &line, uint32_t &val);
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
