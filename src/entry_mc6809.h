/* -*- mode: c++; -*- */
#ifndef __ENTRY_MC6809_H__
#define __ENTRY_MC6809_H__

#include "config_mc6809.h"

struct Entry {
    const target::opcode_t opc;
    const host::uint_t flags;
    const char *name;
    static constexpr host::uint_t code_max = 5;
    static constexpr host::uint_t name_max = 6;

    static inline CpuType _cpuType(host::uint_t flags) {
        return (flags & hd6309_bm) == 0 ? MC6809 : HD6309;
    }

    static inline OprSize _oprSize(host::uint_t flags) {
        return OprSize((flags >> oprSize_gp) & oprSize_gm);
    }

    static inline AddrMode _addrMode(host::uint_t flags) {
        return AddrMode((flags >> addrMode_gp) & addrMode_gm);
    }

    static constexpr host::uint_t _flags(
        CpuType cpuType, OprSize oprSize, AddrMode addrMode) {
        return (cpuType == MC6809 ? 0 : hd6309_bm)
            | (host::uint_t(oprSize) << oprSize_gp)
            | (host::uint_t(addrMode) << addrMode_gp);
    }

private:
    static constexpr host::uint_t hd6309_bm = 0x80;
    static constexpr host::uint_t oprSize_gp = 4;
    static constexpr host::uint_t oprSize_gm = 0x03;
    static constexpr host::uint_t addrMode_gp = 0;
    static constexpr host::uint_t addrMode_gm = 0x0f;
};

#endif // __ENTRY_MC6809_H__
