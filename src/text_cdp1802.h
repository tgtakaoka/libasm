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

#ifndef __LIBASM_TEXT_CDP1802__
#define __LIBASM_TEXT_CDP1802__

#include "text_common.h"

namespace libasm {
namespace text {
namespace cdp1802 {
// clang-format off

extern const char TEXT_CDP1802_LIST[];
extern const char TEXT_CPU_1802[];
extern const char TEXT_CPU_1804[];
extern const char TEXT_CPU_1804A[];

// CDP1802
using common::TEXT_ADC;
extern const char TEXT_ADCI[];
using common::TEXT_ADD;
using common::TEXT_ADI;
using common::TEXT_AND;
using common::TEXT_ANI;
extern const char TEXT_B1[];
extern const char TEXT_B2[];
extern const char TEXT_B3[];
extern const char TEXT_B4[];
extern const char TEXT_BDF[];
using common::TEXT_BGE;
using common::TEXT_BL;
using common::TEXT_BM;
extern const char TEXT_BN1[];
extern const char TEXT_BN2[];
extern const char TEXT_BN3[];
extern const char TEXT_BN4[];
extern const char TEXT_BNF[];
extern const char TEXT_BNQ[];
using common::TEXT_BNZ;
extern const char TEXT_BPZ[];
extern const char TEXT_BQ[];
using common::TEXT_BR;
using common::TEXT_BZ;
using common::TEXT_DEC;
using common::TEXT_DIS;
extern const char TEXT_GHI[];
extern const char TEXT_GLO[];
using common::TEXT_IDL;
using common::TEXT_INC;
extern const char TEXT_INP[];
extern const char TEXT_IRX[];
extern const char TEXT_LBDF[];
using common::TEXT_LBGE;
extern const char TEXT_LBL[];
extern const char TEXT_LBM[];
extern const char TEXT_LBNF[];
extern const char TEXT_LBNQ[];
extern const char TEXT_LBNZ[];
extern const char TEXT_LBPZ[];
extern const char TEXT_LBQ[];
extern const char TEXT_LBR[];
extern const char TEXT_LBZ[];
using common::TEXT_LDA;
using common::TEXT_LDI;
extern const char TEXT_LDN[];
using common::TEXT_LDX;
extern const char TEXT_LDXA[];
extern const char TEXT_LSDF[];
extern const char TEXT_LSIE[];
extern const char TEXT_LSKP[];
extern const char TEXT_LSNF[];
extern const char TEXT_LSNQ[];
extern const char TEXT_LSNZ[];
extern const char TEXT_LSQ[];
extern const char TEXT_LSZ[];
using common::TEXT_MARK;
extern const char TEXT_NBR[];
extern const char TEXT_NLBR[];
using common::TEXT_NOP;
using common::TEXT_OR;
using common::TEXT_ORI;
using common::TEXT_OUT;
extern const char TEXT_PHI[];
extern const char TEXT_PLO[];
extern const char TEXT_REQ[];
using common::TEXT_RET;
extern const char TEXT_RSHL[];
extern const char TEXT_RSHR[];
extern const char TEXT_SAV[];
using common::TEXT_SD;
extern const char TEXT_SDB[];
extern const char TEXT_SDBI[];
extern const char TEXT_SDI[];
using common::TEXT_SEP;
using common::TEXT_SEQ;
using common::TEXT_SEX;
using common::TEXT_SHL;
extern const char TEXT_SHLC[];
using common::TEXT_SHR;
extern const char TEXT_SHRC[];
using common::TEXT_SKP;
using common::TEXT_SM;
extern const char TEXT_SMB[];
extern const char TEXT_SMBI[];
using common::TEXT_SMI;
using common::TEXT_STR;
extern const char TEXT_STXD[];
using common::TEXT_XOR;
using common::TEXT_XRI;

// CDP1804
extern const char TEXT_BCI[];
extern const char TEXT_BXI[];
extern const char TEXT_CID[];
extern const char TEXT_CIE[];
extern const char TEXT_DTC[];
extern const char TEXT_ETQ[];
extern const char TEXT_GEC[];
using common::TEXT_LDC;
extern const char TEXT_RLDI[];
extern const char TEXT_RLXA[];
extern const char TEXT_RNX[];
extern const char TEXT_RSXD[];
extern const char TEXT_SCAL[];
extern const char TEXT_SCM1[];
extern const char TEXT_SCM2[];
extern const char TEXT_SPM1[];
extern const char TEXT_SPM2[];
extern const char TEXT_SRET[];
using common::TEXT_STM;
extern const char TEXT_STPC[];
extern const char TEXT_XID[];
extern const char TEXT_XIE[];

// CDP1804A
extern const char TEXT_DACI[];
extern const char TEXT_DADC[];
using common::TEXT_DADD;
extern const char TEXT_DADI[];
using common::TEXT_DBNZ;
extern const char TEXT_DSAV[];
extern const char TEXT_DSBI[];
extern const char TEXT_DSM[];
extern const char TEXT_DSMB[];
extern const char TEXT_DSMI[];

// clang-format on
}  // namespace cdp1802
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_CDP1802__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
