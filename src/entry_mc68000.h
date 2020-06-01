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

#ifndef __ENTRY_MC68000_H__
#define __ENTRY_MC68000_H__

#include "config_mc68000.h"

namespace libasm {
namespace mc68000 {

enum InsnFormat {
    IMPLIED  = 0,  // 1 111 111 111 111 111
    ADDR_REG = 1,  // 1 111 111 111 111 AAA
    DATA_REG = 2,  // 1 111 111 111 111 DDD
    MOVE_USP = 3,  // 1 111 111 111 11d AAA d:0=AtoU,1=UtoA
    TRAP_VEC = 4,  // 1 111 111 111 11V VVV
    DATA_DST = 5,  // 1 111 111 111 MMM DDD
    DEST_OPR = 6,  // 1 111 111 111 MMM XXX
    SIGN_EXT = 7,  // 1 111 111 11S 111 DDD S:0=W,1=L
    RELATIVE = 8,  // 1 111 111 1dd ddd ddd
    DEST_SIZ = 9,  // 1 111 111 1SS MMM XXX SS:00=B,01=W,10=L
    MOVE_MLT = 10, // 1 111 1d1 11S MMM XXX d:0=RtoM,1=MtoR
    MOVE_SR  = 11, // 1 111 1dd 111 MMM XXX dd:00=SRtoM,10=CCRtoM,11=MtoSR
    AREG_LNG = 12, // 1 111 AAA 111 MMM XXX
    AREG_SIZ = 13, // 1 111 AAA S11 MMM XXX S:0=W,1=L
    DREG_DST = 14, // 1 111 DDD 111 MMM XXX
    MOVE_QIC = 15, // 1 111 DDD 1dd ddd ddd
    MOVE_PER = 16, // 1 111 DDD 1dS 111 AAA d:0=DtoM,1=MtoD S:0=W,1=L
    DATA_QIC = 17, // 1 111 ddd 1SS MMM XXX ddd:0=8 SS:00=B,01=W,10=L
    DREG_SIZ = 18, // 1 111 DDD 1SS MMM XXX SS:00=B,01=W,10=L
    DMEM_SIZ = 19, // 1 111 DDD dSS MMM XXX d:0=toD,1=Dto SS:00=B,01=W,10=L
    DREG_ROT = 20, // 1 111 rrr 1SS d11 DDD d:0=Ir/1=Dr
    DMEM_DST = 21, // 1 111 XXX 111 11R YYY R:0=D,1=M
    REGS_EXG = 22, // 1 111 XXX 1rr 11r YYY rrr:010=DD,011=AA,101=DA
    CMPM_SIZ = 23, // 1 111 XXX 1SS 111 YYY SS:00=B,01=W,10=L
    DMEM_OPR = 24, // 1 111 XXX 1SS 11R YYY R:0=D,1=M SS:00=B,01=W,10=L
    MOVA_OPR = 25, // 1 1SS AAA 111 MMM XXX SS:11=W,10=L
    MOVE_OPR = 26, // 1 1SS XXX MMM MMM YYY SS:01=B,11=W,10=L
};

enum ExtWord {
    NO_EXT   = 0,  // No extension word
    EXT_IMM  = 1,  // Immediate byte/word/long data
    EXT_BIT  = 2,  // Bit number 0-7
    EXT_DISP = 3,  // Word displacement
    EXT_DBCC = 4,  // Word displacement for DBcc
    EXT_BRA  = 5,  // Optional word displacement for Bcc
    EXT_REGM = 6,  // Registers mask
};

struct Entry {
    const Config::opcode_t opCode;
    const uint8_t flags;
    const char *name;

    static inline InsnFormat _insnFormat(uint8_t flags) {
        return InsnFormat((flags >> insnFmt_gp) & insnFmt_gm);
    }
    static constexpr uint8_t _flags(InsnFormat iformat) {
        return (static_cast<uint8_t>(iformat) << insnFmt_gp);
    }

private:
    static constexpr int insnFmt_gp = 0;
    static constexpr uint8_t insnFmt_gm = 0x1f;
};

} // namespace mc68000
} // namespace libasm

#endif // __ENTRY_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
