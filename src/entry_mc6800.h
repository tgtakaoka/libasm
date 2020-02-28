/* -*- mode: c++; -*- */
#ifndef __ENTRY_MC6800_H__
#define __ENTRY_MC6800_H__

#include "config_mc6800.h"

struct Entry {
    const target::opcode_t opc;
    const host::uint_t flags;
    const char *name;
    static constexpr host::uint_t code_max = 3;
    static constexpr host::uint_t name_max = 3;

    static inline AddrMode _addrMode(host::uint_t flags) {
        return AddrMode(flags & addrMode_gm);
    }

    static inline InsnAdjust _insnAdjust(host::uint_t flags) {
        return InsnAdjust((flags >> insnAdjust_gp) & insnAdjust_gm);
    }

    static inline OprSize _oprSize(host::uint_t flags) {
        return OprSize((flags >> oprSize_gp) & oprSize_gm);
    }

    static constexpr host::uint_t _flags(
        AddrMode addrMode, InsnAdjust insnAdjust, OprSize oprSize) {
        return host::uint_t(addrMode)
            | (host::uint_t(insnAdjust) << insnAdjust_gp)
            | (host::uint_t(oprSize)   << oprSize_gp);
    }
private:
    static constexpr host::uint_t addrMode_gm = 0x7;
    static constexpr host::uint_t insnAdjust_gp = 3;
    static constexpr host::uint_t insnAdjust_gm = 0x3;
    static constexpr host::uint_t oprSize_gp = 5;
    static constexpr host::uint_t oprSize_gm = 0x3;

};

#endif // __ENTRY_MC6800_H__
