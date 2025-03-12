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
    constexpr TableBase(const /* PROGMEM */ ENTRY *head_P, const /* PROGMEM */ ENTRY *tail_P,
            const /* PROGMEM */ uint8_t *index_P, const /* PROGMEM */ uint8_t *itail_P)
        : _entries(head_P, tail_P, index_P, itail_P) {}

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
    constexpr PrefixTableBase(uint16_t prefix, const /* PROGMEM */ ENTRY *head_P,
            const /* PROGMEM */ ENTRY *tail_P, const /* PROGMEM */ uint8_t *index_P,
            const /* PROGMEM */ uint8_t *itail_P)
        : TableBase<ENTRY>(head_P, tail_P, index_P, itail_P), _prefix_P(prefix) {}

    uint16_t readPrefix() const { return pgm_read_word(&_prefix_P); }

private:
    const /* PROGMEM */ uint16_t _prefix_P;
};

/**
 * Base for CPU entry.
 */
template <typename CPUTYPE, typename ENTRY_PAGE>
struct CpuBase {
    CPUTYPE readCpuType() const { return static_cast<CPUTYPE>(pgm_read_byte(&_cpuType_P)); }
    const /* PROGMEM */ char *name_P() const {
        return reinterpret_cast<const char *>(pgm_read_ptr(&_name_P));
    }

    template <typename CPU_T>
    static const CPU_T *search(CPUTYPE cpuType, const CPU_T *table, const CPU_T *end) {
        for (const auto *t = table; t < end; t++) {
            if (cpuType == t->readCpuType())
                return t;
        }
        return nullptr;
    }

    template <typename CPU_T>
    static const CPU_T *search(const StrScanner &name, const CPU_T *table, const CPU_T *end) {
        for (const auto *t = table; t < end; t++) {
            if (name.iequals_P(t->name_P()))
                return t;
        }
        return nullptr;
    }

    constexpr CpuBase(CPUTYPE cpuType, const /*PROGMEM*/ char *name_P,
            const /* PROGMEM */ ENTRY_PAGE *head_P, const /* PROGMEM */ ENTRY_PAGE *tail_P)
        : _pages(head_P, tail_P), _cpuType_P(cpuType), _name_P(name_P) {}

    bool isPrefix(uint16_t code) const {
        const auto *tail = _pages.readTail();
        for (const auto *page = _pages.readHead(); page < tail; page++) {
            const auto prefix = page->readPrefix();
            if (prefix == 0)
                continue;
            if (prefix == code)
                return true;
        }
        return false;
    }

    template <typename INSN, typename ENTRY>
    static int nameComparator(INSN &insn, const ENTRY *entry_P) {
        return strcasecmp_P(insn.name(), entry_P->name_P());
    }

    template <typename INSN, typename ENTRY>
    static bool acceptAll(INSN &, const ENTRY *) {
        return true;
    }

    /** Tempalte specialization of |readCode| for non-prefixed TableBase */
    template <typename INSN, typename ENTRY,
            typename enable_if<!is_base_of<PrefixTableBase<ENTRY>, ENTRY_PAGE>::value>::type * =
                    nullptr>
    static void defaultReadCode(INSN &insn, const ENTRY *entry_P, const ENTRY_PAGE *) {
        insn.setOpCode(entry_P->readOpCode());
        insn.setFlags(entry_P->readFlags());
    }

    /** Template specialization of |readCode| for PrefixTableBase */
    template <typename INSN, typename ENTRY,
            typename enable_if<is_base_of<PrefixTableBase<ENTRY>, ENTRY_PAGE>::value>::type * =
                    nullptr>
    static void defaultReadCode(INSN &insn, const ENTRY *entry_P, const ENTRY_PAGE *page_P) {
        insn.setPrefix(page_P->readPrefix());
        insn.setOpCode(entry_P->readOpCode());
        insn.setFlags(entry_P->readFlags());
    }

    template <typename INSN>
    static void defaultPageSetup(INSN &, const ENTRY_PAGE *) {}

    /**
     * Binary search an entry from all instruction |_pages| table where an entry has |insn.name()|
     * and |acceptOperands(insn, entry)| returns true.  Also updates |insn| error code if any.
     */
    template <typename INSN, typename ENTRY>
    const ENTRY *searchName(INSN &insn, bool (*acceptOperands)(INSN &, const ENTRY *),
            void (*pageSetup)(INSN &, const ENTRY_PAGE *) = defaultPageSetup,
            void (*readCode)(INSN &, const ENTRY *, const ENTRY_PAGE *) = defaultReadCode) const {
        insn.setOK();
        auto found = false;
        const auto *tail = _pages.readTail();
        for (auto page = _pages.readHead(); page < tail; page++) {
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
            INSN &insn, const ENTRY *entry_P, StrBuffer &out, const ENTRY_PAGE * = nullptr) {
        insn.setFlags(entry_P->readFlags());
        auto save{out};
        insn.nameBuffer().reset().over(out).text_P(entry_P->name_P()).over(insn.nameBuffer());
        save.over(out);
    }

    template <typename INSN, typename ENTRY>
    static bool defaultMatchOpCode(INSN &insn, const ENTRY *entry_P, const ENTRY_PAGE *page_P) {
        UNUSED(page_P);
        return insn.opCode() == entry_P->readOpCode();
    }

    /** Template specialization of |pageMatcher| for non-prefixed TableBase */
    template <typename INSN, typename PAGE,
            typename enable_if<!is_base_of_template<PrefixTableBase, PAGE>::value>::type * =
                    nullptr>
    static bool defaultPageMatcher(INSN &, const PAGE *) {
        return true;
    }

    /** Template specialization of |pageMatcher| for PrefixTableBase */
    template <typename INSN, typename PAGE,
            typename enable_if<is_base_of_template<PrefixTableBase, PAGE>::value>::type * = nullptr>
    static bool defaultPageMatcher(INSN &insn, const PAGE *page_P) {
        return insn.prefix() == page_P->readPrefix();
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
        insn.setOK();
        const auto *tail = _pages.readTail();
        for (const ENTRY_PAGE *page = _pages.readHead(); page < tail; page++) {
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
    CPUTYPE _cpuType_P;
    const /* PROGMEM */ char *_name_P;
};

}  // namespace entry
}  // namespace libasm

#endif  // __LIBASM_ENTRY_TABLE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
