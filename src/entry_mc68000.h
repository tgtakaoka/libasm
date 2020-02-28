/* -*- mode: c++; -*- */
#ifndef __ENTRY_MC68000_H__
#define __ENTRY_MC68000_H__

#include "config_mc68000.h"

struct Entry {
    const target::insn_t insnCode;
    const host::uint_t flags;
    const char *name;
    static constexpr host::uint_t code_max = 10;
    static constexpr host::uint_t name_max = 7;

    static inline InsnFormat _insnFormat(host::uint_t flags) {
        return InsnFormat((flags >> insnFmt_gp) & insnFmt_gm);
    }
    static constexpr host::uint_t _flags(InsnFormat iformat) {
        return (host::uint_t(iformat) << insnFmt_gp);
    }

private:
    static constexpr host::uint_t insnFmt_gp = 0;
    static constexpr host::uint_t insnFmt_gm = 0x1f;
};

#endif // __ENTRY_MC68000_H__
