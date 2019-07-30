/* -*- mode: c++; -*- */
#ifndef __ENTRY_HD6309_H__
#define __ENTRY_HD6309_H__

#include "config_hd6309.h"

struct Entry {
    const target::opcode_t opc;
    const host::uint_t flags;
    const char *name;

    static constexpr host::uint_t hd6309_flag = 0x80;
    static constexpr host::uint_t addrMode_mask = 0x1f;
};

static inline McuMode _mcuMode(host::uint_t flags) {
    return (flags & Entry::hd6309_flag) == 0 ? MC6809 : HD6309;
}
static inline AddrMode _addrMode(host::uint_t flags) {
    return AddrMode(flags & Entry::addrMode_mask);
}

#define _flags(_mcuMode, _addrMode)                 \
    ((_mcuMode == MC6809 ? 0 : Entry::hd6309_flag)  \
     | host::uint_t(_addrMode))
#define P00(_opc, _name,  _mcu, _mode)              \
    { _opc,  _flags(_mcu, _mode), TEXT_##_name },
#define P10(_opc, _name, _mcu, _mode)               \
    { _opc,  _flags(_mcu, _mode), TEXT_##_name },
#define P11(_opc, _name,  _mcu, _mode)              \
    { _opc,  _flags(_mcu, _mode), TEXT_##_name },

#endif // __ENTRY_HD6309_H__
