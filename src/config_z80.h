/* -*- mode: c++; -*- */
#ifndef __CONFIG_Z80_H__
#define __CONFIG_Z80_H__

#include "config_host.h"
#include "error_z80.h"

namespace target
{
    typedef ::uint8_t  byte_t;
    typedef ::uint16_t uint16_t;
    typedef ::uint32_t uint32_t;
    typedef ::int8_t   int8_t;
    typedef ::int16_t  int16_t;
    typedef ::int32_t  int32_t;
    typedef ::uint16_t uintptr_t;
    typedef ::int16_t  intptr_t;
    typedef ::int16_t  ptrdiff_t;
    typedef ::uint8_t  opcode_t;
    typedef ::uint16_t insn_t;
} // namespace target

enum AddrMode : host::uint_t {
    INHERENT,
    IMMEDIATE8,
    IMMEDIATE16,
    DIRECT,
    IOADDR,
    RELATIVE,
    INDEXED,
    INDEXED_IMMEDIATE8,
};

enum InsnFormat : host::uint_t {
    NO_FMT,
    PTR_FMT,                            // **PP_****: BC/DE/HL/SP
                                        // **PP_****: BC/DE/HL/AF
    CC4_FMT,                            // ***C_C***: NZ/Z/NC/C
    IDX_FMT,                            // ***I_****: BC/DE, 0/1
    IR_FMT,                             // ****_R***: I/R
    DST_FMT,                            // **DD_D***: B/C/D/E/H/L/(HL)/A
                                        // **VV_V***: [0123][08]H
                                        // **CC_C***: NZ/Z/NC/C/PO/PE/P/M
    SRC_FMT,                            // ****_*SSS: B/C/D/E/H/L/(HL)/A
    DST_SRC_FMT,                        // **DD_DSSS: B/C/D/E/H/L/(HL)/A
                                        // **BB_BSSS: 0-7, B/C/D?E/H/L/(HL)/A
};

enum OprFormat : host::uint_t {
    NO_OPR,
    REG8,                               // B/C/D/E/H/L/(HL)/A
    REG16,                              // BC/DE/HL/SP
    REG16X,                             // BC/DE/IX/SP, BC/DE/IY/SP
    A_REG,                              // A
    BC_REG,                             // BC
    DE_REG,                             // DE
    HL_REG,                             // HL
    SP_REG,                             // SP
    IX_REG,                             // IX/IY
    IR_REG,                             // I/R
    AF_REG,                             // AF
    AFPREG,                             // AF'
    STK16,                              // BC/DE/HL/AF
    BC_PTR,                             // (BC)/(DE)
    HL_PTR,                             // (HL)
    IX_PTR,                             // (IX)/(IY)
    SP_PTR,                             // (SP)
    C_PTR,                              // (C)
    IMM8,                               // nn
    IMM16,                              // nnnn
    ADDR8,                              // (nn)
    ADDR16,                             // (nnnn)
    CC4,                                // Z/NZ/C/NC
    CC8,                                // Z/NZ/C/NC/PO/PE/P/M
    BIT_NO,                             // 0~7
    IMM_NO,                             // 0,1,2
    VEC_NO,                             // [0123][08]H
    IX_OFF,                             // (IX/IY+nn)
    IX_BIT,                             // DD/FD CB nn xx
};

#include "registers_z80.h"

#endif // __CONFIG_Z80_H__
