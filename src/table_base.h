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
namespace table {

/**
 * Base class for any table in PROGMEM.
 *
 * A table consists of a constant array of |ITEM| in PROGMEM. So that special consideration, such as
 * using |pgm_read_byte| etc., is necessary to read any contents via |ITEM| pointer.
 */
template <typename /*PROGMEM*/ ITEM>
struct Table {
    const ITEM *table() const { return reinterpret_cast<const ITEM *>(pgm_read_ptr(&_table)); }
    const ITEM *end() const { return reinterpret_cast<const ITEM *>(pgm_read_ptr(&_end)); }

    constexpr Table(const /*PROGMEM*/ ITEM *table, const /*PROGMEM*/ ITEM *end)
        : _table(table), _end(end) {}

    /**
     * Linear searching an item which satisfies |match| and returns the pointer to the |ITEM|.
     * Returns nullptr if such item is not found.
     */
    template <typename DATA, typename EXTRA>
    using Matcher = bool (*)(DATA &, const ITEM *, EXTRA);

    template <typename DATA, typename EXTRA>
    const ITEM *linearSearch(DATA &data, Matcher<DATA, EXTRA> matcher, EXTRA extra) const {
        for (const auto *item = table(); item < end(); item++) {
            if (matcher(data, item, extra))
                return item;
        }
        return nullptr;
    }

    template <typename DATA>
    using Comparator = int (*)(DATA &, const ITEM *);

    template <typename DATA>
    const ITEM *binarySearch(DATA &data, Comparator<DATA> comparator) const {
        const auto *first = table();
        const auto *last = end();
        for (;;) {
            const auto diff = last - first;
            if (diff == 0)
                return nullptr;
            const auto *middle = first;
            middle += diff / 2;
            const auto res = comparator(data, middle);
            if (res == 0) {
                return middle;
            } else if (res > 0) {
                first = middle + 1;
            } else {
                last = middle;
            }
        }
    }

private:
    const ITEM *_table;
    const ITEM *_end;
};

/**
 *  class for indexed table in template.
 */
template <typename /*PROGMEM*/ ITEM, typename INDEX>
struct IndexedTable {
    constexpr IndexedTable(const /*PROGMEM*/ ITEM *table, const /*PROGMEM*/ ITEM *end,
            const /*PROGMEM*/ INDEX *itable, const /*PROGMEM*/ INDEX *iend)
        : _items(table, end), _indexes(itable, iend) {}

    template <typename DATA, typename EXTRA>
    using Matcher = bool (*)(DATA &, const ITEM *, EXTRA);

    template <typename DATA, typename EXTRA>
    const ITEM *linearSearch(DATA &data, Matcher<DATA, EXTRA> matcher, EXTRA extra) const {
        return _items.linearSearch(data, matcher, extra);
    }

    template <typename DATA>
    using Comparator = int (*)(DATA &, const ITEM *);
    template <typename DATA>
    using Matcher2 = bool (*)(DATA &, const ITEM *);

    bool notExactMatch(const ITEM *item) const { return item == _items.end(); }

    template <typename DATA>
    const ITEM *binarySearch(
            DATA &data, Comparator<DATA> comparator, Matcher2<DATA> matcher2) const {
        const auto *first = _indexes.table();
        const auto *last = _indexes.end();
        for (;;) {
            const auto diff = last - first;
            if (diff == 0)
                break;
            const auto *middle = first;
            middle += diff / 2;
            if (comparator(data, itemAt(middle)) > 0) {
                first = middle + 1;
            } else {
                last = middle;
            }
        }
        // Search for the same key items.
        const ITEM *found = nullptr;
        while (first < _indexes.end()) {
            const auto *item = itemAt(first);
            if (comparator(data, item))
                return found;
            found = _items.end();
            if (matcher2(data, item))
                return item;
            ++first;
        }
        return found;
    }

private:
    const Table<ITEM> _items;
    const Table<INDEX> _indexes;

    const ITEM *itemAt(const INDEX *index) const {
        if (sizeof(INDEX) == 1) {
            return _items.table() + pgm_read_byte(index);
        } else {
            return _items.table() + pgm_read_word(index);
        }
    }
};

}  // namespace table
}  // namespace libasm

#endif  // __TABLE_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
