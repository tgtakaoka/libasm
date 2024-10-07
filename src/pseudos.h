/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __LIBASM_PSEUDOS_H__
#define __LIBASM_PSEUDOS_H__

#include "config_host.h"
#include "insn_base.h"
#include "str_scanner.h"
#include "table_base.h"

namespace libasm {

struct Assembler;

namespace pseudo {

struct Pseudo {
    using Handler = Error (Assembler::*)(StrScanner &scan, Insn &insn, uint8_t);

    constexpr Pseudo(const /*PROGMEM*/ char *name_P, Handler handler, uint8_t extra = 0)
        : _name_P(name_P), _handler_P(handler), _extra_P(extra) {}

    const /*PROGMEM*/ char *name_P() const {
        return reinterpret_cast<const char *>(pgm_read_ptr(&_name_P));
    }

    Error invoke(Assembler *assembler, StrScanner &scan, Insn &insn) const {
        const auto h = handler();
        return (assembler->*h)(scan, insn, extra());
    }

private:
    const /*PROGMEM*/ char *const _name_P;
    const Handler _handler_P;
    const uint8_t _extra_P;

    Handler handler() const {
        Handler h;
        memcpy_P(&h, &_handler_P, sizeof(Handler));
        return h;
    }

    uint8_t extra() const { return pgm_read_byte(&_extra_P); }
};

struct Pseudos {
    constexpr Pseudos(const /*PROGMEM*/ Pseudo *table_P, const /*PROGMEM*/ Pseudo *end_P)
        : _table(table_P, end_P) {}

    const Pseudo *search(const Insn &insn) const { return _table.binarySearch(insn, comparator); }

private:
    const table::Table<Pseudo> _table;

    static int comparator(const Insn &insn, const /*PROGMEM*/ Pseudo *item) {
        return strcasecmp_P(insn.name(), item->name_P());
    }
};

}  // namespace pseudo
}  // namespace libasm

#endif  // __LIBASM_PSEUDOS_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
