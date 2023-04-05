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

#ifndef __TABLE_BASE_H__
#define __TABLE_BASE_H__

#include "config_host.h"
#include "error_reporter.h"

namespace libasm {

/**
 * Base class for any table.
 */
template <typename ENTRY>
struct TableBase {
    const ENTRY *table() const { return reinterpret_cast<const ENTRY *>(pgm_read_ptr(&_table)); }
    const ENTRY *end() const { return reinterpret_cast<const ENTRY *>(pgm_read_ptr(&_end)); }

    constexpr TableBase(const ENTRY *table, const ENTRY *end) : _table(table), _end(end) {}

    template <typename DATA, typename EXTRA>
    const ENTRY *searchEntry(DATA &data, bool (*matcher)(DATA &, const ENTRY *, EXTRA),
            void (*fetcher)(DATA &, const ENTRY *, EXTRA), EXTRA extra) const {
        for (const auto *entry = table(); entry < end(); entry++) {
            if (matcher(data, entry, extra)) {
                fetcher(data, entry, extra);
                return entry;
            }
        }
        return nullptr;
    }

private:
    const ENTRY *_table;
    const ENTRY *_end;
};

/**
 * Base class for indexed table.
 */
template <typename ENTRY, typename INDEX>
struct IndexedTableBase : TableBase<ENTRY> {
    constexpr IndexedTableBase(
            const ENTRY *table, const ENTRY *end, const INDEX *itable, const INDEX *iend)
        : TableBase<ENTRY>(table, end), _indexes(itable, iend) {}

    template <typename DATA, typename EXTRA>
    const ENTRY *searchIndexedEntry(DATA &data, int (*comparator)(DATA &, const ENTRY *),
            bool (*matcher)(DATA &, const ENTRY *), void (*fetcher)(DATA &, const ENTRY *, EXTRA),
            EXTRA extra) const {
        const auto *first = _indexes.table();
        const auto *last = _indexes.end();
        for (;;) {
            const auto diff = last - first;
            if (diff == 0)
                break;
            const auto *middle = first;
            middle += diff / 2;
            if (comparator(data, entryAt(middle)) > 0) {
                first = ++middle;
            } else {
                last = middle;
            }
        }
        // Search for the same key entries.
        const ENTRY *found = nullptr;
        while (first < _indexes.end()) {
            const auto *entry = entryAt(first);
            if (comparator(data, entry))
                return found;
            found = TableBase<ENTRY>::end();
            if (matcher(data, entry)) {
                fetcher(data, entry, extra);
                return entry;
            }
            ++first;
        }
        return found;
    }

private:
    const TableBase<INDEX> _indexes;

    const ENTRY *entryAt(const INDEX *index) const {
        if (sizeof(INDEX) == 1) {
            return TableBase<ENTRY>::table() + pgm_read_byte(index);
        } else {
            return TableBase<ENTRY>::table() + pgm_read_word(index);
        }
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
