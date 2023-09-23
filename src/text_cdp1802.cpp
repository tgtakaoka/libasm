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

#include "text_cdp1802.h"

namespace libasm {
namespace text {
namespace cdp1802 {
// clang-format off

const char TEXT_CDP1802_LIST[]  PROGMEM = "CDP1802, CDP1804, CDP1804A";
const char TEXT_CPU_1802[]  PROGMEM = "1802";
const char TEXT_CPU_1804[]  PROGMEM = "1804";
const char TEXT_CPU_1804A[] PROGMEM = "1804A";

// const char TEXT_null[] PROGMEM = "";

// CDP1802
// const char TEXT_ADC[]  PROGMEM = "ADC";
const char TEXT_ADCI[] PROGMEM = "ADCI";
// const char TEXT_ADD[]  PROGMEM = "ADD";
// const char TEXT_ADI[]  PROGMEM = "ADI";
// const char TEXT_AND[]  PROGMEM = "AND";
// const char TEXT_ANI[]  PROGMEM = "ANI";
const char TEXT_B1[]   PROGMEM = "B1";
const char TEXT_B2[]   PROGMEM = "B2";
const char TEXT_B3[]   PROGMEM = "B3";
const char TEXT_B4[]   PROGMEM = "B4";
const char TEXT_BDF[]  PROGMEM = "BDF";
// const char TEXT_BGE[]  PROGMEM = "BGE";
// const char TEXT_BL[]   PROGMEM = "BL";
// const char TEXT_BM[]   PROGMEM = "BM";
const char TEXT_BN1[]  PROGMEM = "BN1";
const char TEXT_BN2[]  PROGMEM = "BN2";
const char TEXT_BN3[]  PROGMEM = "BN3";
const char TEXT_BN4[]  PROGMEM = "BN4";
const char TEXT_BNF[]  PROGMEM = "BNF";
const char TEXT_BNQ[]  PROGMEM = "BNQ";
// const char TEXT_BNZ[]  PROGMEM = "BNZ";
const char TEXT_BPZ[]  PROGMEM = "BPZ";
const char TEXT_BQ[]   PROGMEM = "BQ";
// const char TEXT_BR[]   PROGMEM = "BR";
// const char TEXT_BZ[]   PROGMEM = "BZ";
// const char TEXT_DEC[]  PROGMEM = "DEC";
// const char TEXT_DIS[]  PROGMEM = "DIS";
const char TEXT_GHI[]  PROGMEM = "GHI";
const char TEXT_GLO[]  PROGMEM = "GLO";
// const char TEXT_IDL[]  PROGMEM = "IDL";
// const char TEXT_INC[]  PROGMEM = "INC";
const char TEXT_INP[]  PROGMEM = "INP";
const char TEXT_IRX[]  PROGMEM = "IRX";
const char TEXT_LBDF[] PROGMEM = "LBDF";
const char TEXT_LBPZ[] PROGMEM = "LBPZ";
// const char TEXT_LBGE[] PROGMEM = "LBGE";
const char TEXT_LBNF[] PROGMEM = "LBNF";
const char TEXT_LBM[]  PROGMEM = "LBM";
const char TEXT_LBL[]  PROGMEM = "LBL";
const char TEXT_LBNQ[] PROGMEM = "LBNQ";
const char TEXT_LBNZ[] PROGMEM = "LBNZ";
const char TEXT_LBQ[]  PROGMEM = "LBQ";
const char TEXT_LBR[]  PROGMEM = "LBR";
const char TEXT_LBZ[]  PROGMEM = "LBZ";
// const char TEXT_LDA[]  PROGMEM = "LDA";
// const char TEXT_LDI[]  PROGMEM = "LDI";
const char TEXT_LDN[]  PROGMEM = "LDN";
// const char TEXT_LDX[]  PROGMEM = "LDX";
const char TEXT_LDXA[] PROGMEM = "LDXA";
const char TEXT_LSDF[] PROGMEM = "LSDF";
const char TEXT_LSIE[] PROGMEM = "LSIE";
const char TEXT_LSKP[] PROGMEM = "LSKP";
const char TEXT_LSNF[] PROGMEM = "LSNF";
const char TEXT_LSNQ[] PROGMEM = "LSNQ";
const char TEXT_LSNZ[] PROGMEM = "LSNZ";
const char TEXT_LSQ[]  PROGMEM = "LSQ";
const char TEXT_LSZ[]  PROGMEM = "LSZ";
const char TEXT_MARK[] PROGMEM = "MARK";
const char TEXT_NBR[]  PROGMEM = "NBR";
const char TEXT_NLBR[] PROGMEM = "NLBR";
const char TEXT_NOP[]  PROGMEM = "NOP";
const char TEXT_OR[]   PROGMEM = "OR";
const char TEXT_ORI[]  PROGMEM = "ORI";
const char TEXT_OUT[]  PROGMEM = "OUT";
const char TEXT_PHI[]  PROGMEM = "PHI";
const char TEXT_PLO[]  PROGMEM = "PLO";
const char TEXT_REQ[]  PROGMEM = "REQ";
const char TEXT_RET[]  PROGMEM = "RET";
const char TEXT_RSHL[] PROGMEM = "RSHL";
const char TEXT_RSHR[] PROGMEM = "RSHR";
const char TEXT_SAV[]  PROGMEM = "SAV";
const char TEXT_SD[]   PROGMEM = "SD";
const char TEXT_SDB[]  PROGMEM = "SDB";
const char TEXT_SDBI[] PROGMEM = "SDBI";
const char TEXT_SDI[]  PROGMEM = "SDI";
const char TEXT_SEP[]  PROGMEM = "SEP";
const char TEXT_SEQ[]  PROGMEM = "SEQ";
const char TEXT_SEX[]  PROGMEM = "SEX";
const char TEXT_SHL[]  PROGMEM = "SHL";
const char TEXT_SHLC[] PROGMEM = "SHLC";
const char TEXT_SHR[]  PROGMEM = "SHR";
const char TEXT_SHRC[] PROGMEM = "SHRC";
const char TEXT_SKP[]  PROGMEM = "SKP";
const char TEXT_SM[]   PROGMEM = "SM";
const char TEXT_SMB[]  PROGMEM = "SMB";
const char TEXT_SMBI[] PROGMEM = "SMBI";
const char TEXT_SMI[]  PROGMEM = "SMI";
const char TEXT_STR[]  PROGMEM = "STR";
const char TEXT_STXD[] PROGMEM = "STXD";
const char TEXT_XOR[]  PROGMEM = "XOR";
const char TEXT_XRI[]  PROGMEM = "XRI";

// CDP1804
const char TEXT_BCI[]  PROGMEM = "BCI";
const char TEXT_BXI[]  PROGMEM = "BXI";
const char TEXT_CID[]  PROGMEM = "CID";
const char TEXT_CIE[]  PROGMEM = "CIE";
const char TEXT_DTC[]  PROGMEM = "DTC";
const char TEXT_ETQ[]  PROGMEM = "ETQ";
const char TEXT_GEC[]  PROGMEM = "GEC";
const char TEXT_LDC[]  PROGMEM = "LDC";
const char TEXT_RLDI[] PROGMEM = "RLDI";
const char TEXT_RLXA[] PROGMEM = "RLXA";
const char TEXT_RNX[]  PROGMEM = "RNX";
const char TEXT_RSXD[] PROGMEM = "RSXD";
const char TEXT_SCAL[] PROGMEM = "SCAL";
const char TEXT_SCM1[] PROGMEM = "SCM1";
const char TEXT_SCM2[] PROGMEM = "SCM2";
const char TEXT_SPM1[] PROGMEM = "SPM1";
const char TEXT_SPM2[] PROGMEM = "SPM2";
const char TEXT_SRET[] PROGMEM = "SRET";
const char TEXT_STM[]  PROGMEM = "STM";
const char TEXT_STPC[] PROGMEM = "STPC";
const char TEXT_XID[]  PROGMEM = "XID";
const char TEXT_XIE[]  PROGMEM = "XIE";

// CDP1804A
const char TEXT_DACI[] PROGMEM = "DACI";
const char TEXT_DADC[] PROGMEM = "DADC";
const char TEXT_DADD[] PROGMEM = "DADD";
const char TEXT_DADI[] PROGMEM = "DADI";
const char TEXT_DBNZ[] PROGMEM = "DBNZ";
const char TEXT_DSAV[] PROGMEM = "DSAV";
const char TEXT_DSBI[] PROGMEM = "DSBI";
const char TEXT_DSM[]  PROGMEM = "DSM";
const char TEXT_DSMB[] PROGMEM = "DSMB";
const char TEXT_DSMI[] PROGMEM = "DSMI";

// clang-format om
} // namespace cdp1802
} // namespace text
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
