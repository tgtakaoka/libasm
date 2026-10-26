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

template <typename APP, typename INSN>
struct __Pseudo {
    using Handler = Error (APP::*)(StrScanner &scan, INSN &insn, uint16_t extra);

    constexpr __Pseudo(const char *name_P, Handler handler, uint16_t extra = 0)
        : _name_P(name_P), _handler_P(handler), _extra_P(extra) {}

    const char *name_P() const {
        return _name_P;
    }

    Error invoke(APP *app, StrScanner &scan, INSN &insn) const {
        return (app->*_handler_P)(scan, insn, readExtra());
    }

    uint16_t readExtra() const { return _extra_P; }

private:
    const char *const _name_P;
    const Handler _handler_P;
    const uint16_t _extra_P;
};

template <typename PSEUDO>
struct __Pseudos {
    constexpr __Pseudos(const PSEUDO *table_P, const PSEUDO *end_P)
        : _table(table_P, end_P) {}

    const PSEUDO *search(const char *name) const { return _table.binarySearch(name, comparator); }

private:
    const table::Table<PSEUDO> _table;

    static int comparator(const char *name, const PSEUDO *item) {
        return strcasecmp(name, item->name_P());
    }
};

using Pseudo = __Pseudo<Assembler, Insn>;
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
