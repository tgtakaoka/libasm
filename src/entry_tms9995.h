/* -*- mode: c++; -*- */
#ifndef __ENTRY_TMS9995_H__
#define __ENTRY_TMS9995_H__

#include "config_tms9995.h"

struct Entry {
    const target::insn_t insnCode;
    const host::uint_t flags;
    const char name[4];

    static constexpr host::uint_t name_max = sizeof(Entry::name);

};

static inline AddrMode _addrMode(host::uint_t flags) {
    return AddrMode(flags);
}
static constexpr host::uint_t _flags(AddrMode addrMode) {
    return host::uint_t(addrMode);
}

#define E(_insn, _name, _amode)                 \
    { _insn, _flags(_amode), { _name } },

#endif // __ENTRY_TMS9995_H__
