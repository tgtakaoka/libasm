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

#include <string.h>

#include "config_host.h"
#include "error_reporter.h"

namespace libasm {

/**
 * Base class for instruction page table entry.
 */
template <typename ENTRY_T>
struct EntryPageBase {
    const ENTRY_T *table() const {
        return reinterpret_cast<const ENTRY_T *>(pgm_read_ptr(&_table));
    }
    const ENTRY_T *end() const { return reinterpret_cast<const ENTRY_T *>(pgm_read_ptr(&_end)); }
    const uint8_t *index() const {
        return reinterpret_cast<const uint8_t *>(pgm_read_ptr(&_index));
    }
    const uint8_t *iend() const { return reinterpret_cast<const uint8_t *>(pgm_read_ptr(&_iend)); }

protected:
    constexpr EntryPageBase(const ENTRY_T *table, const ENTRY_T *end,
            const uint8_t *index = nullptr, const uint8_t *iend = nullptr)
        : _table(table), _end(end), _index(index), _iend(iend) {}

private:
    const ENTRY_T *_table;
    const ENTRY_T *_end;
    const uint8_t *_index;
    const uint8_t *_iend;
};

/**
 * Base class for CPU entry.
 */
template <typename CPU_T, typename ENTRY_T>
class CpuTableBase : public EntryPageBase<ENTRY_T> {
public:
    CPU_T cpuType() const { return static_cast<CPU_T>(pgm_read_byte(&_cpuType)); }
    const /* PROGMEM */ char *name_P() const {
        return reinterpret_cast<const char *>(pgm_read_ptr(&_name_P));
    }

    static const CpuTableBase<CPU_T, ENTRY_T> *search(CPU_T cpuType,
            const CpuTableBase<CPU_T, ENTRY_T> *table, const CpuTableBase<CPU_T, ENTRY_T> *end) {
        for (const auto *t = table; t < end; t++) {
            if (cpuType == t->cpuType())
                return t;
        }
        return nullptr;
    }

    static const CpuTableBase<CPU_T, ENTRY_T> *search(const char *name,
            const CpuTableBase<CPU_T, ENTRY_T> *table, const CpuTableBase<CPU_T, ENTRY_T> *end) {
        for (const auto *t = table; t < end; t++) {
            if (strcasecmp_P(name, t->name_P()) == 0)
                return t;
        }
        return nullptr;
    }

protected:
    constexpr CpuTableBase(CPU_T cpuType, const /* PROGMEM */ char *name_P, const ENTRY_T *table,
            const ENTRY_T *end)
        : EntryPageBase<ENTRY_T>(table, end), _cpuType(cpuType), _name_P(name_P) {}

private:
    CPU_T _cpuType;
    const /* PROGMEM */ char *_name_P;
};

/**
 * Base class for instruction table.
 */
class TableBase : public ErrorReporter {
public:
    virtual /* PROGMEM */ const char *listCpu_P() const = 0;
    virtual /* PROGMEM */ const char *cpu_P() const = 0;
    virtual bool setCpu(const char *cpu) = 0;

protected:
    TableBase() {}

    /**
     * Binary search instruction entries sorted and indexed from
     * |page->index()| until |page->iend()| to find an entry which
     * has |name| and accepts |attr|. Returns the number of entries
     * matching |name| regardless of |attr|.
     */
    template <typename P, typename E, typename A>
    static const E *searchName(
            const char *name, A attr, const P *page, bool (*accept)(A, const E *), uint8_t &count) {
        const uint8_t *first = page->index();
        const uint8_t *last = page->iend();
        for (;;) {
            const auto diff = last - first;
            if (diff == 0)
                break;
            const auto *middle = first;
            middle += diff / 2;
            const auto &m = page->table()[pgm_read_byte(middle)];
            if (strcasecmp_P(name, m.name_P()) > 0) {
                first = ++middle;
            } else {
                last = middle;
            }
        }
        while (first < page->iend()) {
            const E *entry = &page->table()[pgm_read_byte(first)];
            if (strcasecmp_P(name, entry->name_P()))
                break;
            count++;
            if (accept(attr, entry))
                return entry;
            ++first;
        }
        return nullptr;
    }

    /**
     * Lookup instruction entries from |begin| until |end| to find an
     * entry which matches |opCode| converted by |convert|.
     */
    template <typename E, typename C>
    static const E *searchCode(
            const C opCode, const E *begin, const E *end, C (*convert)(C, const E *) = nullptr) {
        for (const auto *entry = begin; entry < end; entry++) {
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

}  // namespace libasm

#endif  // __TABLE_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
