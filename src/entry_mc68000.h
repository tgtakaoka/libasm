/* -*- mode: c++; -*- */
#ifndef __ENTRY_MC68000_H__
#define __ENTRY_MC68000_H__

#include "config_mc68000.h"

struct Entry {
    const target::insn_t insnCode;
    const host::uint_t flags;
    const char *name;

    static constexpr host::uint_t insnFmt_gp = 0;
    static constexpr host::uint_t insnFmt_gm = 0x1f;
    static constexpr host::uint_t extWord_gp = 5;
    static constexpr host::uint_t extWord_gm = 0x7;
};

static inline InsnFormat _insnFormat(host::uint_t flags) {
    return InsnFormat((flags >> Entry::insnFmt_gp) & Entry::insnFmt_gm);
}
static inline ExtWord _extWord(host::uint_t flags) {
    return ExtWord((flags >> Entry::extWord_gp) & Entry::extWord_gm);
}
static constexpr host::uint_t _flags(InsnFormat iformat, ExtWord extWord) {
    return (host::uint_t(iformat) << Entry::insnFmt_gp)
        | (host::uint_t(extWord) << Entry::extWord_gp);
}

#define E(_opc, _name, _iformat, _extWord)                  \
    { _opc,  _flags(_iformat, _extWord), TEXT_##_name },

#endif // __ENTRY_MC68000_H__
