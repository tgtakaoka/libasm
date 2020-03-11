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

#ifndef __CONFIG_MC68000_H__
#define __CONFIG_MC68000_H__

#include "config_host.h"

namespace target
{
    typedef uint32_t uintptr_t;
    typedef int32_t  ptrdiff_t;
    typedef uint16_t opcode_t;
    typedef uint16_t insn_t;
} // namespace target

enum InsnFormat : host::uint_t {
    IMPLIED,  // 1 111 111 111 111 111
    ADDR_REG, // 1 111 111 111 111 AAA
    DATA_REG, // 1 111 111 111 111 DDD
    MOVE_USP, // 1 111 111 111 11d AAA d:0=AtoU,1=UtoA
    TRAP_VEC, // 1 111 111 111 11V VVV
    DATA_DST, // 1 111 111 111 MMM DDD
    DEST_OPR, // 1 111 111 111 MMM XXX
    SIGN_EXT, // 1 111 111 11S 111 DDD S:0=W,1=L
    RELATIVE, // 1 111 111 1dd ddd ddd
    DEST_SIZ, // 1 111 111 1SS MMM XXX SS:00=B,01=W,10=L
    MOVE_MLT, // 1 111 1d1 11S MMM XXX d:0=RtoM,1=MtoR
    MOVE_SR,  // 1 111 1dd 111 MMM XXX dd:00=SRtoM,10=CCRtoM,11=MtoSR
    AREG_LNG, // 1 111 AAA 111 MMM XXX
    AREG_SIZ, // 1 111 AAA S11 MMM XXX S:0=W,1=L
    DREG_DST, // 1 111 DDD 111 MMM XXX
    MOVE_QIC, // 1 111 DDD 1dd ddd ddd
    MOVE_PER, // 1 111 DDD 1dS 111 AAA d:0=DtoM,1=MtoD S:0=W,1=L
    DATA_QIC, // 1 111 ddd 1SS MMM XXX ddd:0=8 SS:00=B,01=W,10=L
    DREG_SIZ, // 1 111 DDD 1SS MMM XXX SS:00=B,01=W,10=L
    DMEM_SIZ, // 1 111 DDD dSS MMM XXX d:0=toD,1=Dto SS:00=B,01=W,10=L
    DREG_ROT, // 1 111 rrr 1SS d11 DDD d:0=Ir/1=Dr
    DMEM_DST, // 1 111 XXX 111 11R YYY R:0=D,1=M
    REGS_EXG, // 1 111 XXX 1rr 11r YYY rrr:010=DD,011=AA,101=DA
    CMPM_SIZ, // 1 111 XXX 1SS 111 YYY SS:00=B,01=W,10=L
    DMEM_OPR, // 1 111 XXX 1SS 11R YYY R:0=D,1=M SS:00=B,01=W,10=L
    MOVA_OPR, // 1 1SS AAA 111 MMM XXX SS:11=W,10=L
    MOVE_OPR, // 1 1SS XXX MMM MMM YYY SS:01=B,11=W,10=L
};

enum ExtWord : host::uint_t {
    NO_EXT,    // No extension word
    EXT_IMM,   // Immediate byte/word/long data
    EXT_BIT,   // Bit number 0-7
    EXT_DISP,  // Word displacement
    EXT_DBCC,  // Word displacement for DBcc
    EXT_BRA,   // Optional word displacement for Bcc
    EXT_REGM,  // Registers mask
};

#endif // __CONFIG_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
