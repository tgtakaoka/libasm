#ifndef __ENTRY_I8080_H__
#define __ENTRY_I8080_H__

#include "config_host.h"

struct EntryI8080 {
    const target::insn_t insnCode;
    const host::uint_t flags;
    const char name[4];
};

static constexpr host::uint_t iformat_shift = 4;
static constexpr host::uint_t iformat_mask = 0x7;
static constexpr host::uint_t amode_mask = 0x07;
static constexpr host::uint_t name_max = sizeof(EntryI8080::name);

#define _flags(_amode, _iformat) \
    ((host::uint_t(_iformat) << iformat_shift) | (_amode))
#define _amode(_flags) ((_flags) & amode_mask)
#define _iformat(_flags) (((_flags) >> iformat_shift) & iformat_mask)
#define E(_opc, _name, _amode, _iformat)            \
     { _opc, _flags(_amode, _iformat), { _name } },

#endif // __ENTRY_I8080_H__
