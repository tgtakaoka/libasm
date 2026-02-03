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

template <typename APP>
struct __Pseudo {
    using Handler = Error (APP::*)(StrScanner &scan, Insn &insn, uint16_t extra);

    constexpr __Pseudo(const /*PROGMEM*/ char *name_P, Handler handler, uint16_t extra = 0)
        : _name_P(name_P), _handler_P(handler), _extra_P(extra) {}

    const /*PROGMEM*/ char *name_P() const {
        return reinterpret_cast<const char *>(pgm_read_ptr(&_name_P));
    }

    Error invoke(APP *app, StrScanner &scan, Insn &insn) const {
        const auto h = handler();
        return (app->*h)(scan, insn, extra());
    }

private:
    const /*PROGMEM*/ char *const _name_P;
    const Handler _handler_P;
    const uint16_t _extra_P;

    Handler handler() const {
        Handler h;
        memcpy_P(&h, &_handler_P, sizeof(Handler));
        return h;
    }

    uint16_t extra() const { return pgm_read_word(&_extra_P); }
};

template <typename PSEUDO>
struct __Pseudos {
    constexpr __Pseudos(const /*PROGMEM*/ PSEUDO *table_P, const /*PROGMEM*/ PSEUDO *end_P)
        : _table(table_P, end_P) {}

    const PSEUDO *search(const Insn &insn) const { return _table.binarySearch(insn, comparator); }

private:
    const table::Table<PSEUDO> _table;

    static int comparator(const Insn &insn, const /*PROGMEM*/ PSEUDO *item) {
        return strcasecmp_P(insn.name(), item->name_P());
    }
};

using Pseudo = __Pseudo<Assembler>;
using Pseudos = __Pseudos<Pseudo>;

}  // namespace pseudo
}  // namespace libasm

#endif  // __LIBASM_PSEUDOS_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
