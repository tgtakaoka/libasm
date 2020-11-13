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

#ifndef __TABLE_BASE_H__
#define __TABLE_BASE_H__

#include "config_host.h"
#include "error_reporter.h"
#include "insn_base.h"

namespace libasm {

class TableBase {
public:
    virtual const char *listCpu() const = 0;
    virtual bool setCpu(const char *cpu) = 0;
    virtual const char *getCpu() const = 0;
    Error getError() const { return _error.getError(); }

protected:
    mutable ErrorReporter _error;

    TableBase() {}

    template<typename E>
    static const E *searchName(
        const char *name, const E *begin, const E *end) {
        for (const E *entry = begin; entry < end; entry++) {
            if (strcasecmp_P(name, entry->name()) == 0)
                return entry;
        }
        return nullptr;
    }

    template<typename E, typename A>
    static const E *searchName(
        const char *name, A attr, const E *begin, const E *end,
        bool (*accept)(A, const E *), uint8_t &count) {
        for (const E *entry = begin;
             entry < end && (entry = searchName(name, entry, end));
             entry++) {
            count++;
            if (accept(attr, entry))
                return entry;
        }
        return nullptr;
    }

    template<typename E, typename C>
    static const E *searchCode(
        const C opCode, const E *begin, const E *end,
        C (*convert)(C, const E *) = nullptr) {
        for (const E *entry = begin; entry < end; entry++) {
            if (convert) {
                if (convert(opCode, entry) == entry->opCode())
                    return entry;
            } else {
                if (opCode == entry->opCode())
                    return entry;
            }
        }
        return nullptr;
    }
};

} // namespace libasm

#endif // __TABLE_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
