/* -*- mode: c++; -*- */
#ifndef __ENTRY_R65C02_H__
#define __ENTRY_R65C02_H__

#include "config_r65c02.h"

struct Entry {
    const target::opcode_t opc;
    const host::uint_t flags;
    const char *name;

    static constexpr host::uint_t r65c02_flag = 0x80;
    static constexpr host::uint_t addrMode_mask = 0x1f;
};

static inline McuType _mcuType(host::uint_t flags) {
    return (flags & Entry::r65c02_flag) == 0 ? R6502 : R65C02;
}
static inline AddrMode _addrMode(host::uint_t flags) {
    return AddrMode(flags & Entry::addrMode_mask);
}

static constexpr host::uint_t _flags(McuType mcuType, AddrMode addrMode) {
    return (mcuType == R6502 ? 0 : Entry::r65c02_flag) | host::uint_t(addrMode);
}

#define E(_opc, _name, _mcu, _amode)       \
    { _opc, _flags(_mcu, _amode), TEXT_##_name },

#endif // __ENTRY_R65C02_H__
