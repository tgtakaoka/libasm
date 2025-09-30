/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_TEXT_TMS320F__
#define __LIBASM_TEXT_TMS320F__

#include "text_common.h"

namespace libasm {
namespace text {
namespace tms320f {

// clang-format off
extern const char TEXT_TMS320F_LIST[] PROGMEM;
extern const char TEXT_CPU_320C3X[] PROGMEM;

// TMS320F
ABSF
ABSI
ADD
ADDC
ADDC3
ADDF
ADDF3
ADDI3
AND
AND3
ANDN
ANDN3
ASH
ASH3
Bcc
BccD
BED
BR
CALL
CALLcc
CALLccD
CMPF
CMPF3
CMPI
CMPI3
FIX
FLOAT
IACK
IDLE
IDLE2
LDE
LDF
LDFcc
LDFI
LDI
LDIcc
LDII
LDM
LDP
LOPOWER
LSH
LSH3
MAXSPEED
MPYF
MPYF3
MPYI
MPYI3
NEGB
NEGF
NEGI
NOP
NORM
NOT
OR
OR3
POP
POPF
PUSH
PUSHF
RETIcc
RETScc
RND
ROL
ROLC
ROR
RORC
RPTB
RPTS
SIGI
STF
STFI
STI
STII
SUB
SUB3
SUBB
SUBB3
SUBC
SUBF
SUBF3
SUBRB
SUBRF
SUBRI
SWI
TRAPcc
TSTB
TSTB3
XOR
XOR3

REG_AR
REG_BK
REG_DP
REG_IE
REG_IF
REG_IOF
REG_IR
REG_R
REG_RC
REG_RE
REG_RS
REG_SP
REG_ST

CC_C
CC_EQ
CC_GE
CC_GT
CC_HI
CC_HS
CC_LE
CC_LO
CC_LS
CC_LT
CC_LUF
CC_LV
CC_N
CC_NC
CC_NE
CC_NLUF
CC_NLV
CC_NN
CC_NUF
CC_NV
CC_NZ
CC_P
CC_U
CC_UF
CC_V
CC_Z
CC_ZUF

// clang-format on
}  // namespace tms320f
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_TMS320F__
// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
