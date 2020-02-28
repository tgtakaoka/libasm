/* -*- mode: c++; -*- */
#ifndef __ENTRY_TMS9900_H__
#define __ENTRY_TMS9900_H__

#include "config_tms9900.h"

struct Entry {
    const target::insn_t insnCode;
    const host::uint_t flags;
    static constexpr host::uint_t code_max = 6;
    static constexpr host::uint_t name_max = 4;
    const char name[name_max];

    static inline CpuType _cpuType(host::uint_t flags) {
        return (flags & tms9995_bm) == 0 ? TMS9900 : TMS9995;
    }

    static inline AddrMode _addrMode(host::uint_t flags) {
        return AddrMode(flags & addrMode_gm);
    }
    static constexpr host::uint_t _flags(CpuType cpuType, AddrMode addrMode) {
        return (cpuType == TMS9995 ? tms9995_bm : 0)
            | host::uint_t(addrMode);
    }

private:
    static constexpr host::uint_t tms9995_bm = 0x80;
    static constexpr host::uint_t addrMode_gm = 0x0f;
};

#endif // __ENTRY_TMS9900_H__
