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

#include "intel_hex.h"
#include "moto_srec.h"

#include <ctype.h>

namespace libasm {
namespace cli {

int BinDecoder::decode(TextReader &in, BinMemory &memory) {
    StrScanner *line;
    BinDecoder *decoder = nullptr;
    int size = 0;
    while ((line = in.readLine()) != nullptr) {
        if (decoder == nullptr) {
            switch (**line) {
            case 'S':
                decoder = &MotoSrec::decoder();
                break;
            case ':':
                decoder = &IntelHex::decoder();
                break;
            default:
                return 1;
            }
        }
        const auto len = decoder->decode(*line, memory);
        if (len < 0)
            return len;
        size += len;
    }
    return 0;
}

static uint8_t parseHex(char c) {
    return isdigit(c) ? c - '0' : toupper(c) - 'A' + 10;
}

bool BinDecoder::parseByte(StrScanner &line, uint8_t &val) {
    const auto c1 = line.expect(isxdigit);
    if (c1 == 0)
        return false;
    val = parseHex(c1) << 4;
    const auto c2 = line.expect(isxdigit);
    if (c2 == 0)
        return false;
    val |= parseHex(c2);
    return true;
}

bool BinDecoder::parseUint16(StrScanner &line, uint16_t &val) {
    uint8_t val8 = 0;
    if (!parseByte(line, val8))
        return false;
    val = static_cast<uint16_t>(val8) << 8;
    if (!parseByte(line, val8))
        return false;
    val |= val8;
    return true;
}

bool BinDecoder::parseUint24(StrScanner &line, uint32_t &val) {
    uint8_t val8 = 0;
    if (!parseByte(line, val8))
        return false;
    val = static_cast<uint32_t>(val8) << 16;
    uint16_t val16 = 0;
    if (!parseUint16(line, val16))
        return false;
    val |= val16;
    return true;
}

bool BinDecoder::parseUint32(StrScanner &line, uint32_t &val) {
    uint16_t val16 = 0;
    if (!parseUint16(line, val16))
        return false;
    val = static_cast<uint32_t>(val16) << 16;
    if (!parseUint16(line, val16))
        return false;
    val |= val16;
    return true;
}

}  // namespace cli
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
