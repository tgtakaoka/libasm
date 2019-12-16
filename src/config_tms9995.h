/* -*- mode: c++; -*- */
#ifndef __CONFIG_TMS9995_H__
#define __CONFIG_TMS9995_H__

#include "config_host.h"

namespace target
{
    typedef uint16_t uintptr_t;
    typedef int16_t  ptrdiff_t;
    typedef uint16_t insn_t;
} // namespace target

enum AddrMode : host::uint_t {
    INH,                        // ---- ---- ---- ----
    IMM,                        // ---- ---- ---- ---- + nnnn
    REG,                        // ---- ---- ---- wwww
    REG_IMM,                    // ---- ---- ---- wwww + nnnn
    CNT_REG,                    // ---- ---- cccc wwww
    SRC,                        // ---- ---- --SS ssss
    CNT_SRC,                    // ---- --cc ccSS ssss
    XOP_SRC,                    // ---- --vv vvSS ssss
    REG_SRC,                    // ---- --dd ddSS ssss
    DST_SRC,                    // ---- DDdd ddSS ssss
    REL,                        // ---- ---- nnnn nnnn
    CRU_OFF,                    // ---- ---- nnnn nnnn
};

//#include "registers_tms9995.h"

#endif // __CONFIG_TMS99951_H__
