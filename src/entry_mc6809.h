#ifndef __ENTRY_MC6809_H__
#define __ENTRY_MC6809_H__

#include "config_host.h"

static constexpr host::uint_t mode_shift = 3;
static constexpr host::uint_t mode_mask = 0xf;
static constexpr host::uint_t oprLen_mask = 0x7;

struct EntryMc6809 {
    const target::opcode_t opc;
    const host::uint_t flags;
    const char *name;
};

#define _flags(_oprLen, _mode) ((host::uint_t(_mode) << mode_shift) | (_oprLen))
#define P00(_opc, _name, _len, _mode) \
    { _opc,  _flags(_len - 1, _mode), TEXT_##_name },
#define P10(_opc, _name, _len, _mode) \
    { _opc,  _flags(_len - 2, _mode), TEXT_##_name },
#define P11(_opc, _name, _len, _mode) \
    { _opc,  _flags(_len - 2, _mode), TEXT_##_name },

#endif // __ENTRY_MC6809_H__
