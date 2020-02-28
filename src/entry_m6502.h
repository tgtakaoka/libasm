/* -*- mode: c++; -*- */
#ifndef __ENTRY_M6502_H__
#define __ENTRY_M6502_H__

#include "config_m6502.h"

struct Entry {
    const target::insn_t insnCode;
    const host::uint_t flags;
    const char *name;
    static constexpr host::uint_t code_max = 3;
    static constexpr host::uint_t name_max = 4;

    static inline CpuType _cpuType(host::uint_t flags) {
        return (flags & w65c02_bm) == 0 ? M6502 : W65C02;
    }
    static inline AddrMode _addrMode(host::uint_t flags) {
        return AddrMode(flags & addrMode_gm);
    }
    static constexpr host::uint_t _flags(CpuType cpuType, AddrMode addrMode) {
        return (cpuType == M6502 ? 0 : w65c02_bm) | host::uint_t(addrMode);
    }

private:
    static constexpr host::uint_t w65c02_bm = 0x80;
    static constexpr host::uint_t addrMode_gm = 0x1f;
};

#endif // __ENTRY_M6502_H__
