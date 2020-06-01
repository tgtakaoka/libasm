/*
 * Copyright 2020 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ENTRY_Z80_H__
#define __ENTRY_Z80_H__

#include "config_z80.h"

namespace libasm {
namespace z80 {

enum CpuType {
    Z80,
    I8080,
    I8085,
};

enum OprSize {
    SZ_NONE = 0,  // unknown
    SZ_BYTE = 1,
    SZ_WORD = 2,
};

enum AddrMode {
    INHR      = 0,  // Inherent
    IMM8      = 1,  // Immediate 8-bit
    IMM16     = 2,  // Immediate 16-bit
    DIRECT    = 3,  // Direct address 16-bit
    IOADR     = 4,  // I/O address 8-bit
    REL8      = 5,  // PC-Relative
    INDX      = 6,  // Indexed
    INDX_IMM8 = 7,  // Indexed and Immediate 8-bit
};

enum InsnFormat {
    NO_FMT      = 0,
    PTR_FMT     = 1,  // **PP_****: BC/DE/HL/SP
                      // **PP_****: BC/DE/HL/AF
    CC4_FMT     = 2,  // ***C_C***: NZ/Z/NC/C
    IDX_FMT     = 3,  // ***I_****: BC/DE, 0/1
    IR_FMT      = 4,  // ****_R***: I/R
    DST_FMT     = 5,  // **DD_D***: B/C/D/E/H/L/(HL)/A
                      // **VV_V***: [0123][08]H
                      // **CC_C***: NZ/Z/NC/C/PO/PE/P/M
    SRC_FMT     = 6,  // ****_*SSS: B/C/D/E/H/L/(HL)/A
    DST_SRC_FMT = 7,  // **DD_DSSS: B/C/D/E/H/L/(HL)/A
                      // **BB_BSSS: 0-7, B/C/D?E/H/L/(HL)/A
};

enum OprFormat {
    NO_OPR  = 0,
    REG_8   = 1,   // B/C/D/E/H/L/(HL)/A
    REG_16  = 2,   // BC/DE/HL/SP
    REG_16X = 3,   // BC/DE/IX/SP, BC/DE/IY/SP
    A_REG   = 4,   // A
    C_REG   = 5,   // C-register or Carry-condition
    BC_REG  = 6,   // BC
    DE_REG  = 7,   // DE
    HL_REG  = 8,   // HL
    SP_REG  = 9,   // SP
    IX_REG  = 10,  // IX/IY
    IR_REG  = 11,  // I/R
    AF_REG  = 12,  // AF
    AFPREG  = 13,  // AF'
    STK_16  = 14,  // BC/DE/HL/AF
    BC_PTR  = 15,  // (BC)/(DE)
    HL_PTR  = 16,  // (HL)
    IX_PTR  = 17,  // (IX)/(IY)
    SP_PTR  = 18,  // (SP)
    C_PTR   = 19,  // (C)
    IMM_8   = 20,  // nn
    IMM_16  = 21,  // nnnn
    ADDR_8  = 22,  // (nn)
    ADDR_16 = 23,  // (nnnn)
    COND_4  = 24,  // Z/NZ/C/NC
    COND_8  = 25,  // Z/NZ/C/NC/PO/PE/P/M
    BIT_NO  = 26,  // 0~7
    IMM_NO  = 27,  // 0,1,2
    VEC_NO  = 28,  // [0123][08]H
    IX_OFF  = 29,  // (IX/IY+nn)
    IX_BIT  = 30,  // DD/FD CB nn xx
    IM_REG  = 31,  // 8080/IM
};

struct Entry {
    const Config::opcode_t opCode;
    const uint16_t flags;
    const char *name;

    static inline InsnFormat _insnFormat(uint16_t flags) {
        return InsnFormat((flags >> insnFormat_gp) & insnFormat_gm);
    }
    static inline OprFormat _dstFormat(uint16_t flags) {
        return OprFormat((flags >> dstFormat_gp) & oprFormat_gm);
    }
    static inline OprFormat _srcFormat(uint16_t flags) {
        return OprFormat((flags >> srcFormat_gp) & oprFormat_gm);
    }
    static inline AddrMode _addrMode(uint16_t flags) {
        return AddrMode((flags >> addrMode_gp) & addrMode_gm);
    }
    static constexpr uint16_t _flags(
        InsnFormat iformat, AddrMode addrMode,
        OprFormat dstFormat, OprFormat srcFormat) {
        return (static_cast<uint16_t>(iformat) << insnFormat_gp)
            | (static_cast<uint16_t>(dstFormat) << dstFormat_gp)
            | (static_cast<uint16_t>(addrMode) << addrMode_gp)
            | (static_cast<uint16_t>(srcFormat) << srcFormat_gp);
    }

private:
    static constexpr uint8_t insnFormat_gm = 0x7;
    static constexpr uint8_t addrMode_gm = 0x7;
    static constexpr uint8_t oprFormat_gm = 0x1f;
    static constexpr int insnFormat_gp = 0;
    static constexpr int dstFormat_gp = 3;
    static constexpr int addrMode_gp = 8;
    static constexpr int srcFormat_gp = 11;
};

} // namespace z80
} // namespace libasm

#endif // __ENTRY_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
