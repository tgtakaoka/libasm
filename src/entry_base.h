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

#ifndef __ENTRY_BASE_H__
#define __ENTRY_BASE_H__

#include "config_base.h"

namespace libasm {

/**
 * Base class for instruction table entry.
 */
template <typename Conf>
class EntryBase {
public:
    constexpr EntryBase(const /* PROGMEM */ char *name_P, typename Conf::opcode_t opCode)
        : _name_P(name_P), _opCode(opCode) {}

    const /* PROGMEM */ char *name_P() const {
        return reinterpret_cast<const char *>(pgm_read_ptr(&_name_P));
    }

    typename Conf::opcode_t opCode() const {
        if (sizeof(typename Conf::opcode_t) == 1) {
            return pgm_read_byte(&_opCode);
        } else {
            return pgm_read_word(&_opCode);
        }
    }

private:
    const /* PROGMEM */ char *_name_P;
    typename Conf::opcode_t _opCode;
};

}  // namespace libasm

#endif  // __ENTRY_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
