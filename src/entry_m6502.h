/* -*- mode: c++; -*- */
#ifndef __ENTRY_M6502_H__
#define __ENTRY_M6502_H__

#include "config_m6502.h"

struct Entry {
    const target::insn_t insnCode;
    const host::uint_t flags;
    const char *name;

    static constexpr host::uint_t w65c02_bm = 0x80;
    static constexpr host::uint_t addrMode_gm = 0x1f;
};

static inline McuType _mcuType(host::uint_t flags) {
    return (flags & Entry::w65c02_bm) == 0 ? M6502 : W65C02;
}
static inline AddrMode _addrMode(host::uint_t flags) {
    return AddrMode(flags & Entry::addrMode_gm);
}

static constexpr host::uint_t _flags(McuType mcuType, AddrMode addrMode) {
    return (mcuType == M6502 ? 0 : Entry::w65c02_bm) | host::uint_t(addrMode);
}

#define E(_opc, _name, _mcu, _amode)       \
    { _opc, _flags(_mcu, _amode), TEXT_##_name },

#endif // __ENTRY_M6502_H__
