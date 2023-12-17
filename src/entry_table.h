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

#ifndef __LIBASM_ENTRY_TABLE_H__
#define __LIBASM_ENTRY_TABLE_H__

#include <string.h>

#include "str_buffer.h"
#include "str_scanner.h"
#include "table_base.h"
#include "type_traits.h"

namespace libasm {
namespace entry {

/**
 * Base for instruction entry table.
 */
template <typename ENTRY>
struct TableBase {
    constexpr TableBase(
            const ENTRY *table, const ENTRY *end, const uint8_t *index, const uint8_t *iend)
        : _entries(table, end, index, iend) {}

    template <typename DATA, typename EXTRA>
    using Matcher = bool (*)(DATA &, const ENTRY *, EXTRA);

    template <typename DATA, typename EXTRA>
    const ENTRY *linearSearch(DATA &data, Matcher<DATA, EXTRA> matcher, EXTRA extra) const {
        return _entries.linearSearch(data, matcher, extra);
    }

    template <typename DATA>
    using Comparator = int (*)(DATA &, const ENTRY *);
    template <typename DATA>
    using Matcher2 = bool (*)(DATA &, const ENTRY *);

    template <typename DATA>
    const ENTRY *binarySearch(
            DATA &data, Comparator<DATA> comparator, Matcher2<DATA> matcher2) const {
        return _entries.binarySearch(data, comparator, matcher2);
    }

    bool notExactMatch(const ENTRY *entry) const { return _entries.notExactMatch(entry); }

protected:
    const table::IndexedTable<ENTRY, uint8_t> _entries;
};

/**
 * Base for instruction entry table with prefix opecode.
 */
template <typename ENTRY>
struct PrefixTableBase : TableBase<ENTRY> {
    constexpr PrefixTableBase(uint8_t prefix, const ENTRY *table, const ENTRY *end,
            const uint8_t *index, const uint8_t *iend)
        : TableBase<ENTRY>(table, end, index, iend), _prefix(prefix) {}

    uint8_t prefix() const { return pgm_read_byte(&_prefix); }

private:
    const uint8_t _prefix;
};

/**
 * Base for CPU entry.
 */
template <typename CPUTYPE, typename ENTRY_PAGE>
struct CpuBase {
    CPUTYPE cpuType() const { return static_cast<CPUTYPE>(pgm_read_byte(&_cpuType)); }
    const /* PROGMEM */ char *name_P() const {
        return reinterpret_cast<const char *>(pgm_read_ptr(&_name_P));
    }

    template <typename CPU_T>
    static const CPU_T *search(CPUTYPE cpuType, const CPU_T *table, const CPU_T *end) {
        for (const auto *t = table; t < end; t++) {
            if (cpuType == t->cpuType())
                return t;
        }
        return nullptr;
    }

    template <typename CPU_T>
    static const CPU_T *search(const StrScanner &name, const CPU_T *table, const CPU_T *end) {
        for (const auto *t = table; t < end; t++) {
            if (name.iequals(t->name_P()))
                return t;
        }
        return nullptr;
    }

    constexpr CpuBase(CPUTYPE cpuType, const /*PROGMEM*/ char *name_P, const ENTRY_PAGE *table,
            const ENTRY_PAGE *end)
        : _pages(table, end), _cpuType(cpuType), _name_P(name_P) {}

    bool isPrefix(uint8_t code) const {
        for (const auto *page = _pages.table(); page < _pages.end(); page++) {
            const auto prefix = page->prefix();
            if (prefix == 0)
                continue;
            if (prefix == code)
                return true;
        }
        return false;
    }

    template <typename INSN, typename ENTRY>
    static int nameComparator(INSN &insn, const ENTRY *entry) {
        return strcasecmp_P(insn.name(), entry->name_P());
    }

    /** Tempalte specialization of |readCode| for non-prefixed TableBase */
    template <typename INSN, typename ENTRY, typename PAGE,
            typename enable_if<!is_base_of<PrefixTableBase<ENTRY>, PAGE>::value>::type * = nullptr>
    static void defaultReadCode(INSN &insn, const ENTRY *entry, const PAGE *page) {
        UNUSED(page);
        insn.setFlags(entry->flags());
        insn.setOpCode(entry->opCode());
    }

    /** Template specialization of |readCode| for PrefixTableBase */
    template <typename INSN, typename ENTRY, typename PAGE,
            typename enable_if<is_base_of<PrefixTableBase<ENTRY>, PAGE>::value>::type * = nullptr>
    static void defaultReadCode(INSN &insn, const ENTRY *entry, const PAGE *page) {
        UNUSED(page);
        insn.setFlags(entry->flags());
        insn.setOpCode(entry->opCode(), page->prefix());
    }

    template <typename INSN>
    static void defaultPageSetup(INSN &insn, const ENTRY_PAGE *page) {
        UNUSED(insn);
        UNUSED(page);
    }

    /**
     * Binary search an entry from all instruction |_pages| table where an entry has |insn.name()|
     * and |acceptOperands(insn, entry)| returns true.  Also updates |insn| error code if any.
     */
    template <typename INSN, typename ENTRY>
    const ENTRY *searchName(INSN &insn, bool (*acceptOperands)(INSN &, const ENTRY *),
            void (*pageSetup)(INSN &, const ENTRY_PAGE *) = defaultPageSetup,
            void (*readCode)(INSN &, const ENTRY *, const ENTRY_PAGE *) = defaultReadCode) const {
        auto found = false;
        for (auto page = _pages.table(); page < _pages.end(); page++) {
            pageSetup(insn, page);
            const auto *entry = page->binarySearch(insn, nameComparator, acceptOperands);
            if (page->notExactMatch(entry)) {
                found = true;
            } else if (entry) {
                readCode(insn, entry, page);
                return entry;
            }
        }
        insn.setErrorIf(found ? OPERAND_NOT_ALLOWED : UNKNOWN_INSTRUCTION);
        return nullptr;
    }

    template <typename INSN, typename ENTRY>
    static void defaultReadName(
            INSN &insn, const ENTRY *entry, StrBuffer &out, const ENTRY_PAGE *page) {
        UNUSED(page);
        insn.setFlags(entry->flags());
        auto save{out};
        insn.nameBuffer().reset().over(out).text_P(entry->name_P()).over(insn.nameBuffer());
        save.over(out);
    }

    template <typename INSN, typename ENTRY>
    static bool defaultMatchOpCode(INSN &insn, const ENTRY *entry, const ENTRY_PAGE *page) {
        return insn.opCode() == entry->opCode();
    }

    /** Template specialization of |pageMatcher| for non-prefixed TableBase */
    template <typename INSN, typename PAGE,
            typename enable_if<!is_base_of_template<PrefixTableBase, PAGE>::value>::type * =
                    nullptr>
    static bool defaultPageMatcher(INSN &insn, const PAGE *page) {
        UNUSED(insn);
        UNUSED(page);
        return true;
    }

    /** Template specialization of |pageMatcher| for PrefixTableBase */
    template <typename INSN, typename PAGE,
            typename enable_if<is_base_of_template<PrefixTableBase, PAGE>::value>::type * = nullptr>
    static bool defaultPageMatcher(INSN &insn, const PAGE *page) {
        return insn.prefix() == page->prefix();
    }

    /**
     * Lookup instruction |_pages| table to find an entry which satisfis |matchCode|, then call
     * |readName| to read the table entry into |insn|. Also updates |insn| error code if any.
     */
    template <typename INSN, typename ENTRY>
    const ENTRY *searchOpCode(INSN &insn, StrBuffer &out,
            bool (*matchCode)(INSN &, const ENTRY *, const ENTRY_PAGE *) = defaultMatchOpCode,
            bool (*pageMatcher)(INSN &, const ENTRY_PAGE *) = defaultPageMatcher,
            void (*readName)(INSN &, const ENTRY *, StrBuffer &,
                    const ENTRY_PAGE *) = defaultReadName) const {
        for (const ENTRY_PAGE *page = _pages.table(); page < _pages.end(); page++) {
            if (pageMatcher(insn, page)) {
                const auto *entry = page->linearSearch(insn, matchCode, page);
                if (entry) {
                    readName(insn, entry, out, page);
                    return entry;
                }
            }
        }
        insn.setErrorIf(UNKNOWN_INSTRUCTION);
        return nullptr;
    }

protected:
    const table::Table<ENTRY_PAGE> _pages;
    CPUTYPE _cpuType;
    const /* PROGMEM */ char *_name_P;
};

}  // namespace entry
}  // namespace libasm

#endif  // __ENTRY_TABLE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
