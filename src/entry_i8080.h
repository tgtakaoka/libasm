/* -*- mode: c++; -*- */
#ifndef __ENTRY_I8080_H__
#define __ENTRY_I8080_H__

#include "config_i8080.h"

struct Entry {
    const target::insn_t insnCode;
    const host::uint_t flags;
    const char name[4];

    static constexpr host::uint_t insnFormat_shift = 4;
    static constexpr host::uint_t insnFormat_mask = 0x7;
    static constexpr host::uint_t addrMode_mask = 0x07;
    static constexpr host::uint_t name_max = sizeof(Entry::name);

};

static inline InsnFormat _insnFormat(host::uint_t flags) {
    return InsnFormat((flags >> Entry::insnFormat_shift) & Entry::insnFormat_mask);
}
static inline AddrMode _addrMode(host::uint_t flags) {
    return AddrMode(flags & Entry::addrMode_mask);
}
static constexpr host::uint_t _flags(AddrMode addrMode, InsnFormat iformat) {
  return (host::uint_t(iformat) << Entry::insnFormat_shift) | host::uint_t(addrMode);
}

#define E(_opc, _name, _amode, _iformat)            \
     { _opc, _flags(_amode, _iformat), { _name } },

#endif // __ENTRY_I8080_H__
