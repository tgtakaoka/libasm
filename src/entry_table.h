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

#ifndef __ENTRY_TABLE_H__
#define __ENTRY_TABLE_H__

#include "table_base.h"

#include <string.h>

namespace libasm {

/**
 * Base class for instruction entry table.
 */
template <typename ENTRY_T>
struct EntryTableBase : IndexedTableBase<ENTRY_T, uint8_t> {
    uint8_t prefix() const { return pgm_read_byte(&_prefix); }
    bool prefixMatch(uint8_t code) const { return code == prefix(); }

    constexpr EntryTableBase(uint8_t prefix, const ENTRY_T *table, const ENTRY_T *end,
            const uint8_t *index, const uint8_t *iend)
        : IndexedTableBase<ENTRY_T, uint8_t>(table, end, index, iend), _prefix(prefix) {}

    constexpr EntryTableBase(
            const ENTRY_T *table, const ENTRY_T *end, const uint8_t *index, const uint8_t *iend)
        : IndexedTableBase<ENTRY_T, uint8_t>(table, end, index, iend), _prefix(0) {}

private:
    const uint8_t _prefix;
};

/**
 * Base class for CPU entry.
 */
template <typename CPUTYPE_T, typename PAGE_T>
struct CpuBase : TableBase<PAGE_T> {
public:
    CPUTYPE_T cpuType() const { return static_cast<CPUTYPE_T>(pgm_read_byte(&_cpuType)); }
    const /* PROGMEM */ char *name_P() const {
        return reinterpret_cast<const char *>(pgm_read_ptr(&_name_P));
    }

    template <typename CPU_T>
    static const CPU_T *search(CPUTYPE_T cpuType, const CPU_T *table, const CPU_T *end) {
        for (const auto *t = table; t < end; t++) {
            if (cpuType == t->cpuType())
                return t;
        }
        return nullptr;
    }

    template <typename CPU_T>
    static const CPU_T *search(const char *name, const CPU_T *table, const CPU_T *end) {
        for (const auto *t = table; t < end; t++) {
            if (strcasecmp_P(name, t->name_P()) == 0)
                return t;
        }
        return nullptr;
    }

    constexpr CpuBase(CPUTYPE_T cpuType, const /* PROGMEM */ char *name_P, const PAGE_T *table,
            const PAGE_T *end)
        : TableBase<PAGE_T>(table, end), _cpuType(cpuType), _name_P(name_P) {}

    bool isPrefix(uint8_t code) const {
        for (const auto *page = this->table(); page < this->end(); page++) {
            const auto prefix = page->prefix();
            if (prefix == 0)
                continue;
            if (prefix == code)
                return true;
        }
        return false;
    }

    template <typename E, typename I>
    static int nameComparator(I &insn, const E *entry) {
        return strcasecmp_P(insn.name(), entry->name_P());
    }

    /**
     * Binary search an entry from all instruction page tables which
     * are sorted and indexed from |page->index()| until
     * |page->iend()| where an entry has |insn.name()| and
     * |acceptOperands(insn, entry)| returns true.  Also updates |insn| error
     * code if any.
     */
    template <typename E, typename P, typename I>
    static void defaultReadCode(I &insn, const E *entry, const P *page) {
        insn.setFlags(entry->flags());
        insn.setOpCode(entry->opCode(), page->prefix());
    }

    template <typename E, typename I>
    const E *searchName(
            I &insn, bool (*acceptOperands)(I &, const E *),
            void (*pageSetup)(I &, const PAGE_T *) = [](I &, const PAGE_T *) {},
            void (*readCode)(I &, const E *, const PAGE_T *) = defaultReadCode) const {
        auto found = false;
        for (auto page = this->table(); page < this->end(); page++) {
            pageSetup(insn, page);
            const auto *entry =
                    page->searchIndexedEntry(insn, nameComparator, acceptOperands, readCode, page);
            if (entry == page->end()) {
                found = true;
            } else if (entry) {
                return entry;
            }
        }
        insn.setError(found ? OPERAND_NOT_ALLOWED : UNKNOWN_INSTRUCTION);
        return nullptr;
    }

    /**
     * Lookup instruction page tables from |tabel()| until |end()| to
     * find an entry which satisfis |matchCode|, then call |readName|
     * to read the table entry into |insn|. Also updates |insn| error
     * code if any.
     */
    template <typename E, typename P, typename I>
    static bool defaultMatchOpCode(I &insn, const E *entry, const P *page) {
        return insn.opCode() == entry->opCode();
    }

    template <typename E, typename P, typename I>
    static void defaultReadEntryName(I &insn, const E *entry, const P *page) {
        insn.setFlags(entry->flags());
        insn.nameBuffer().text_P(entry->name_P());
    }

    template <typename E, typename I>
    const E *searchOpCode(I &insn,
            bool (*matchCode)(I &, const E *, const PAGE_T *) = defaultMatchOpCode,
            void (*readName)(I &, const E *, const PAGE_T *) = defaultReadEntryName) const {
        for (auto page = this->table(); page < this->end(); page++) {
            if (page->prefixMatch(insn.prefix())) {
                const auto *entry = page->searchEntry(insn, matchCode, readName, page);
                if (entry)
                    return entry;
            }
        }
        insn.setError(UNKNOWN_INSTRUCTION);
        return nullptr;
    }

private:
    CPUTYPE_T _cpuType;
    const /* PROGMEM */ char *_name_P;
};

/**
 * Base class for instruction table.
 */
class EntryTable {
public:
    virtual /* PROGMEM */ const char *listCpu_P() const = 0;
    virtual /* PROGMEM */ const char *cpu_P() const = 0;
    virtual bool setCpu(const char *cpu) = 0;
};

}  // namespace libasm

#endif  // __ENTRY_TABLE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
