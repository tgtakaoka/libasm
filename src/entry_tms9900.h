/* -*- mode: c++; -*- */
#ifndef __ENTRY_TMS9900_H__
#define __ENTRY_TMS9900_H__

#include "config_tms9900.h"

struct Entry {
    const target::insn_t insnCode;
    const host::uint_t flags;
    const char name[4];

    static constexpr host::uint_t name_max = sizeof(Entry::name);
    static constexpr host::uint_t tms9995_bm = 0x80;
    static constexpr host::uint_t addrMode_gm = 0x0f;
};

static inline McuType _mcuType(host::uint_t flags) {
    return (flags & Entry::tms9995_bm) == 0 ? TMS9900 : TMS9995;
}

static inline AddrMode _addrMode(host::uint_t flags) {
    return AddrMode(flags & Entry::addrMode_gm);
}
static constexpr host::uint_t _flags(McuType mcuType, AddrMode addrMode) {
    return (mcuType == TMS9995 ? Entry::tms9995_bm : 0)
        | host::uint_t(addrMode);
}

#define E(_insn, _name, _amode, _mcu)           \
    { _insn, _flags(_mcu, _amode), { _name } },

#endif // __ENTRY_TMS9900_H__
