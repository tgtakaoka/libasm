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

#ifndef __LIBASM_TABLE_BASE_H__
#define __LIBASM_TABLE_BASE_H__

#include "config_host.h"
#include "error_reporter.h"

namespace libasm {
namespace table {

/**
 * Template for any table in PROGMEM.
 *
 * A table consists of a constant array of |ITEM| in PROGMEM. So that special consideration, such as
 * using |pgm_read_byte| etc., is necessary to read any contents via |ITEM| pointer.
 */
template <typename /*PROGMEM*/ ITEM>
struct Table {
    const ITEM *readHead() const { return reinterpret_cast<const ITEM *>(pgm_read_ptr(&_head_P)); }
    const ITEM *readTail() const { return reinterpret_cast<const ITEM *>(pgm_read_ptr(&_tail_P)); }

    constexpr Table(const /*PROGMEM*/ ITEM *head_P, const /*PROGMEM*/ ITEM *tail_P)
        : _head_P(head_P), _tail_P(tail_P) {}

    /**
     * Linear searching an item which satisfies |match| and returns the pointer to the |ITEM|.
     * Returns nullptr if such item is not found.
     */
    template <typename DATA, typename EXTRA>
    using Matcher = bool (*)(DATA &, const ITEM *, EXTRA);

    template <typename DATA, typename EXTRA>
    const ITEM *linearSearch(DATA &data, Matcher<DATA, EXTRA> matcher, EXTRA extra) const {
        const auto *end = readTail();
        for (const auto *item = readHead(); item < end; item++) {
            if (matcher(data, item, extra))
                return item;
        }
        return nullptr;
    }

    template <typename DATA>
    using Comparator = int (*)(DATA &, const ITEM *);

    template <typename DATA>
    const ITEM *binarySearch(DATA &data, Comparator<DATA> comparator) const {
        const auto *head = readHead();
        const auto *tail = readTail();
        const ITEM *found = nullptr;
        for (;;) {
            const auto diff = tail - head;
            if (diff == 0)
                break;
            const auto *middle = head;
            middle += diff / 2;
            const auto res = comparator(data, middle);
            if (res > 0) {
                head = middle + 1;
            } else if (res < 0) {
                tail = middle;
            } else {
                // search the head occurrence in table
                tail = found = middle;
            }
        }
        return found;
    }

    using NameComparator = int (*)(const char *, const ITEM *);
    const ITEM *binarySearch(const char *name, NameComparator comparator) const {
        const auto *head = readHead();
        const auto *tail = readTail();
        const ITEM *found = nullptr;
        for (;;) {
            const auto diff = tail - head;
            if (diff == 0)
                break;
            const auto *middle = head;
            middle += diff / 2;
            const auto res = comparator(name, middle);
            if (res > 0) {
                head = middle + 1;
            } else if (res < 0) {
                tail = middle;
            } else {
                // search the head occurrence in table
                tail = found = middle;
            }
        }
        return found;
    }

private:
    const /* PROGMEM */ ITEM *const _head_P;
    const /* PROGMEM */ ITEM *const _tail_P;
};

/**
 *  Template for indexed table in template.
 */
template <typename /*PROGMEM*/ ITEM, typename INDEX>
struct IndexedTable {
    const ITEM *readHead() const { return _items.readHead(); }
    const ITEM *readTail() const { return _items.readTail(); }

    constexpr IndexedTable(const /*PROGMEM*/ ITEM *head_P, const /*PROGMEM*/ ITEM *tail_P,
            const /*PROGMEM*/ INDEX *ihead_P, const /*PROGMEM*/ INDEX *itail_P)
        : _items(head_P, tail_P), _indexes(ihead_P, itail_P) {}

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

    bool notExactMatch(const ITEM *item) const { return item == _items.readTail(); }

    template <typename DATA>
    const ITEM *binarySearch(
            DATA &data, Comparator<DATA> comparator, Matcher2<DATA> matcher2) const {
        const auto *head = _indexes.readHead();
        const auto *tail = _indexes.readTail();
        for (;;) {
            const auto diff = tail - head;
            if (diff == 0)
                break;
            const auto *middle = head;
            middle += diff / 2;
            if (comparator(data, itemAt(middle)) > 0) {
                head = middle + 1;
            } else {
                // If |res| == 0, search the head occurrence in table
                tail = middle;
            }
        }
        // Search for an item which satisfies |matcher2|
        const ITEM *found = nullptr;
        while (head < _indexes.readTail()) {
            const auto *item = itemAt(head);
            // |head| may point an item which doesn't satisfies |comparator|
            if (comparator(data, item) != 0)
                break;
            if (matcher2(data, item)) {
                found = item;
                break;
            }
            // assign a marker to |found| because there is an item doesn't
            // sattisfies |matcher2|
            found = _items.readTail();
            ++head;
        }
        return found;
    }

private:
    const Table<ITEM> _items;
    const Table<INDEX> _indexes;

    const ITEM *itemAt(const INDEX *index) const {
        if (sizeof(INDEX) == 1) {
            return _items.readHead() + pgm_read_byte(index);
        } else {
            return _items.readHead() + pgm_read_word(index);
        }
    }
};

}  // namespace table
}  // namespace libasm

#endif  // __LIBASM_HEAD_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
