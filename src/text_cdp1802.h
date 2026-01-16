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

extern const char TEXT_CDP1802_LIST[]  PROGMEM;
extern const char TEXT_CPU_1802[]  PROGMEM;
extern const char TEXT_CPU_1804[]  PROGMEM;
extern const char TEXT_CPU_1804A[] PROGMEM;

// CDP1802
using common::TEXT_ADC;
extern const char TEXT_ADCI[] PROGMEM;
using common::TEXT_ADD;
using common::TEXT_ADI;
using common::TEXT_AND;
using common::TEXT_ANI;
extern const char TEXT_B1[]   PROGMEM;
extern const char TEXT_B2[]   PROGMEM;
extern const char TEXT_B3[]   PROGMEM;
extern const char TEXT_B4[]   PROGMEM;
extern const char TEXT_BDF[]  PROGMEM;
using common::TEXT_BGE;
using common::TEXT_BL;
using common::TEXT_BM;
extern const char TEXT_BN1[]  PROGMEM;
extern const char TEXT_BN2[]  PROGMEM;
extern const char TEXT_BN3[]  PROGMEM;
extern const char TEXT_BN4[]  PROGMEM;
extern const char TEXT_BNF[]  PROGMEM;
extern const char TEXT_BNQ[]  PROGMEM;
using common::TEXT_BNZ;
extern const char TEXT_BPZ[]  PROGMEM;
extern const char TEXT_BQ[]   PROGMEM;
using common::TEXT_BR;
using common::TEXT_BZ;
using common::TEXT_DEC;
using common::TEXT_DIS;
extern const char TEXT_GHI[]  PROGMEM;
extern const char TEXT_GLO[]  PROGMEM;
using common::TEXT_IDL;
using common::TEXT_INC;
extern const char TEXT_INP[]  PROGMEM;
extern const char TEXT_IRX[]  PROGMEM;
extern const char TEXT_LBDF[] PROGMEM;
using common::TEXT_LBGE;
extern const char TEXT_LBL[]  PROGMEM;
extern const char TEXT_LBM[]  PROGMEM;
extern const char TEXT_LBNF[] PROGMEM;
extern const char TEXT_LBNQ[] PROGMEM;
extern const char TEXT_LBNZ[] PROGMEM;
extern const char TEXT_LBPZ[] PROGMEM;
extern const char TEXT_LBQ[]  PROGMEM;
extern const char TEXT_LBR[]  PROGMEM;
extern const char TEXT_LBZ[]  PROGMEM;
using common::TEXT_LDA;
using common::TEXT_LDI;
extern const char TEXT_LDN[]  PROGMEM;
using common::TEXT_LDX;
extern const char TEXT_LDXA[] PROGMEM;
extern const char TEXT_LSDF[] PROGMEM;
extern const char TEXT_LSIE[] PROGMEM;
extern const char TEXT_LSKP[] PROGMEM;
extern const char TEXT_LSNF[] PROGMEM;
extern const char TEXT_LSNQ[] PROGMEM;
extern const char TEXT_LSNZ[] PROGMEM;
extern const char TEXT_LSQ[]  PROGMEM;
extern const char TEXT_LSZ[]  PROGMEM;
using common::TEXT_MARK;
extern const char TEXT_NBR[]  PROGMEM;
extern const char TEXT_NLBR[] PROGMEM;
using common::TEXT_NOP;
using common::TEXT_OR;
using common::TEXT_ORI;
using common::TEXT_OUT;
extern const char TEXT_PHI[]  PROGMEM;
extern const char TEXT_PLO[]  PROGMEM;
extern const char TEXT_REQ[]  PROGMEM;
using common::TEXT_RET;
extern const char TEXT_RSHL[] PROGMEM;
extern const char TEXT_RSHR[] PROGMEM;
extern const char TEXT_SAV[]  PROGMEM;
using common::TEXT_SD;
extern const char TEXT_SDB[]  PROGMEM;
extern const char TEXT_SDBI[] PROGMEM;
extern const char TEXT_SDI[]  PROGMEM;
using common::TEXT_SEP;
using common::TEXT_SEQ;
using common::TEXT_SEX;
using common::TEXT_SHL;
extern const char TEXT_SHLC[] PROGMEM;
using common::TEXT_SHR;
extern const char TEXT_SHRC[] PROGMEM;
using common::TEXT_SKP;
using common::TEXT_SM;
extern const char TEXT_SMB[]  PROGMEM;
extern const char TEXT_SMBI[] PROGMEM;
using common::TEXT_SMI;
using common::TEXT_STR;
extern const char TEXT_STXD[] PROGMEM;
using common::TEXT_XOR;
using common::TEXT_XRI;

// CDP1804
extern const char TEXT_BCI[]  PROGMEM;
extern const char TEXT_BXI[]  PROGMEM;
extern const char TEXT_CID[]  PROGMEM;
extern const char TEXT_CIE[]  PROGMEM;
extern const char TEXT_DTC[]  PROGMEM;
extern const char TEXT_ETQ[]  PROGMEM;
extern const char TEXT_GEC[]  PROGMEM;
using common::TEXT_LDC;
extern const char TEXT_RLDI[] PROGMEM;
extern const char TEXT_RLXA[] PROGMEM;
extern const char TEXT_RNX[]  PROGMEM;
extern const char TEXT_RSXD[] PROGMEM;
extern const char TEXT_SCAL[] PROGMEM;
extern const char TEXT_SCM1[] PROGMEM;
extern const char TEXT_SCM2[] PROGMEM;
extern const char TEXT_SPM1[] PROGMEM;
extern const char TEXT_SPM2[] PROGMEM;
extern const char TEXT_SRET[] PROGMEM;
extern const char TEXT_STM[]  PROGMEM;
extern const char TEXT_STPC[] PROGMEM;
extern const char TEXT_XID[]  PROGMEM;
extern const char TEXT_XIE[]  PROGMEM;

// CDP1804A
extern const char TEXT_DACI[] PROGMEM;
extern const char TEXT_DADC[] PROGMEM;
extern const char TEXT_DADD[] PROGMEM;
extern const char TEXT_DADI[] PROGMEM;
using common::TEXT_DBNZ;
extern const char TEXT_DSAV[] PROGMEM;
extern const char TEXT_DSBI[] PROGMEM;
extern const char TEXT_DSM[]  PROGMEM;
extern const char TEXT_DSMB[] PROGMEM;
extern const char TEXT_DSMI[] PROGMEM;

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
