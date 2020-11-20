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

template <typename Conf>
class EntryBase {
public:
    constexpr EntryBase(const char *name, typename Conf::opcode_t opCode)
        : _name(name), _opCode(opCode) {}

    const char *name() const {
        return reinterpret_cast<const char *>(pgm_read_ptr(&_name));
    }

    typename Conf::opcode_t opCode() const {
        if (sizeof(typename Conf::opcode_t) == 1) {
            return pgm_read_byte(&_opCode);
        } else {
            return pgm_read_word(&_opCode);
        }
    }

private:
    const char *_name;
    typename Conf::opcode_t _opCode;
};

template <typename ENTRY_T>
class EntryPageBase {
public:
    const ENTRY_T *table() const {
        return reinterpret_cast<const ENTRY_T *>(pgm_read_ptr(&_table));
    }
    const ENTRY_T *end() const {
        return reinterpret_cast<const ENTRY_T *>(pgm_read_ptr(&_end));
    }

protected:
    constexpr EntryPageBase(const ENTRY_T *table, const ENTRY_T *end)
        : _table(table), _end(end) {}

private:
    const ENTRY_T *_table;
    const ENTRY_T *_end;
};

}  // namespace libasm

#endif  // __ENTRY_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
