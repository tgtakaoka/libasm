/* -*- mode: c++; -*- */
#ifndef __ENTRY_I8080_H__
#define __ENTRY_I8080_H__

#include "config_i8080.h"

struct Entry {
    const target::insn_t insnCode;
    const host::uint_t flags;
    static constexpr host::uint_t code_max = 3;
    static constexpr host::uint_t name_max = 4;
    const char name[name_max];

    static inline InsnFormat _insnFormat(host::uint_t flags) {
        return InsnFormat((flags >> insnFormat_shift) & insnFormat_mask);
    }
    static inline AddrMode _addrMode(host::uint_t flags) {
        return AddrMode(flags & addrMode_mask);
    }
    static constexpr host::uint_t _flags(AddrMode addrMode, InsnFormat iformat) {
        return (host::uint_t(iformat) << insnFormat_shift) | host::uint_t(addrMode);
    }

private:
    static constexpr host::uint_t insnFormat_shift = 4;
    static constexpr host::uint_t insnFormat_mask = 0x7;
    static constexpr host::uint_t addrMode_mask = 0x07;
};

#endif // __ENTRY_I8080_H__
