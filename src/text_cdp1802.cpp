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

constexpr char TEXT_CDP1802_LIST[]  PROGMEM = "CDP1802, CDP1804, CDP1804A";
constexpr char TEXT_CPU_1802[]  PROGMEM = "1802";
constexpr char TEXT_CPU_1804[]  PROGMEM = "1804";
constexpr char TEXT_CPU_1804A[] PROGMEM = "1804A";

// constexpr char TEXT_null[] PROGMEM = "";

// CDP1802
// constexpr char TEXT_ADC[]  PROGMEM = "ADC";
constexpr char TEXT_ADCI[] PROGMEM = "ADCI";
// constexpr char TEXT_ADD[]  PROGMEM = "ADD";
// constexpr char TEXT_ADI[]  PROGMEM = "ADI";
// constexpr char TEXT_AND[]  PROGMEM = "AND";
// constexpr char TEXT_ANI[]  PROGMEM = "ANI";
constexpr char TEXT_B1[]   PROGMEM = "B1";
constexpr char TEXT_B2[]   PROGMEM = "B2";
constexpr char TEXT_B3[]   PROGMEM = "B3";
constexpr char TEXT_B4[]   PROGMEM = "B4";
constexpr char TEXT_BDF[]  PROGMEM = "BDF";
// constexpr char TEXT_BGE[]  PROGMEM = "BGE";
// constexpr char TEXT_BL[]   PROGMEM = "BL";
// constexpr char TEXT_BM[]   PROGMEM = "BM";
constexpr char TEXT_BN1[]  PROGMEM = "BN1";
constexpr char TEXT_BN2[]  PROGMEM = "BN2";
constexpr char TEXT_BN3[]  PROGMEM = "BN3";
constexpr char TEXT_BN4[]  PROGMEM = "BN4";
constexpr char TEXT_BNF[]  PROGMEM = "BNF";
constexpr char TEXT_BNQ[]  PROGMEM = "BNQ";
// constexpr char TEXT_BNZ[]  PROGMEM = "BNZ";
constexpr char TEXT_BPZ[]  PROGMEM = "BPZ";
constexpr char TEXT_BQ[]   PROGMEM = "BQ";
// constexpr char TEXT_BR[]   PROGMEM = "BR";
// constexpr char TEXT_BZ[]   PROGMEM = "BZ";
// constexpr char TEXT_DEC[]  PROGMEM = "DEC";
// constexpr char TEXT_DIS[]  PROGMEM = "DIS";
constexpr char TEXT_GHI[]  PROGMEM = "GHI";
constexpr char TEXT_GLO[]  PROGMEM = "GLO";
// constexpr char TEXT_IDL[]  PROGMEM = "IDL";
// constexpr char TEXT_INC[]  PROGMEM = "INC";
constexpr char TEXT_INP[]  PROGMEM = "INP";
constexpr char TEXT_IRX[]  PROGMEM = "IRX";
constexpr char TEXT_LBDF[] PROGMEM = "LBDF";
constexpr char TEXT_LBPZ[] PROGMEM = "LBPZ";
// constexpr char TEXT_LBGE[] PROGMEM = "LBGE";
constexpr char TEXT_LBNF[] PROGMEM = "LBNF";
constexpr char TEXT_LBM[]  PROGMEM = "LBM";
constexpr char TEXT_LBL[]  PROGMEM = "LBL";
constexpr char TEXT_LBNQ[] PROGMEM = "LBNQ";
constexpr char TEXT_LBNZ[] PROGMEM = "LBNZ";
constexpr char TEXT_LBQ[]  PROGMEM = "LBQ";
constexpr char TEXT_LBR[]  PROGMEM = "LBR";
constexpr char TEXT_LBZ[]  PROGMEM = "LBZ";
// constexpr char TEXT_LDA[]  PROGMEM = "LDA";
// constexpr char TEXT_LDI[]  PROGMEM = "LDI";
constexpr char TEXT_LDN[]  PROGMEM = "LDN";
// constexpr char TEXT_LDX[]  PROGMEM = "LDX";
constexpr char TEXT_LDXA[] PROGMEM = "LDXA";
constexpr char TEXT_LSDF[] PROGMEM = "LSDF";
constexpr char TEXT_LSIE[] PROGMEM = "LSIE";
constexpr char TEXT_LSKP[] PROGMEM = "LSKP";
constexpr char TEXT_LSNF[] PROGMEM = "LSNF";
constexpr char TEXT_LSNQ[] PROGMEM = "LSNQ";
constexpr char TEXT_LSNZ[] PROGMEM = "LSNZ";
constexpr char TEXT_LSQ[]  PROGMEM = "LSQ";
constexpr char TEXT_LSZ[]  PROGMEM = "LSZ";
constexpr char TEXT_MARK[] PROGMEM = "MARK";
constexpr char TEXT_NBR[]  PROGMEM = "NBR";
constexpr char TEXT_NLBR[] PROGMEM = "NLBR";
// constexpr char TEXT_NOP[]  PROGMEM = "NOP";
// constexpr char TEXT_OR[]   PROGMEM = "OR";
// constexpr char TEXT_ORI[]  PROGMEM = "ORI";
// constexpr char TEXT_OUT[]  PROGMEM = "OUT";
constexpr char TEXT_PHI[]  PROGMEM = "PHI";
constexpr char TEXT_PLO[]  PROGMEM = "PLO";
constexpr char TEXT_REQ[]  PROGMEM = "REQ";
// constexpr char TEXT_RET[]  PROGMEM = "RET";
constexpr char TEXT_RSHL[] PROGMEM = "RSHL";
constexpr char TEXT_RSHR[] PROGMEM = "RSHR";
constexpr char TEXT_SAV[]  PROGMEM = "SAV";
// constexpr char TEXT_SD[]   PROGMEM = "SD";
constexpr char TEXT_SDB[]  PROGMEM = "SDB";
constexpr char TEXT_SDBI[] PROGMEM = "SDBI";
constexpr char TEXT_SDI[]  PROGMEM = "SDI";
// constexpr char TEXT_SEP[]  PROGMEM = "SEP";
// constexpr char TEXT_SEQ[]  PROGMEM = "SEQ";
// constexpr char TEXT_SEX[]  PROGMEM = "SEX";
// constexpr char TEXT_SHL[]  PROGMEM = "SHL";
constexpr char TEXT_SHLC[] PROGMEM = "SHLC";
// constexpr char TEXT_SHR[]  PROGMEM = "SHR";
constexpr char TEXT_SHRC[] PROGMEM = "SHRC";
// constexpr char TEXT_SKP[]  PROGMEM = "SKP";
// constexpr char TEXT_SM[]   PROGMEM = "SM";
constexpr char TEXT_SMB[]  PROGMEM = "SMB";
constexpr char TEXT_SMBI[] PROGMEM = "SMBI";
// constexpr char TEXT_SMI[]  PROGMEM = "SMI";
// constexpr char TEXT_STR[]  PROGMEM = "STR";
constexpr char TEXT_STXD[] PROGMEM = "STXD";
// constexpr char TEXT_XOR[]  PROGMEM = "XOR";
// constexpr char TEXT_XRI[]  PROGMEM = "XRI";

// CDP1804
constexpr char TEXT_BCI[]  PROGMEM = "BCI";
constexpr char TEXT_BXI[]  PROGMEM = "BXI";
constexpr char TEXT_CID[]  PROGMEM = "CID";
constexpr char TEXT_CIE[]  PROGMEM = "CIE";
constexpr char TEXT_DTC[]  PROGMEM = "DTC";
constexpr char TEXT_ETQ[]  PROGMEM = "ETQ";
constexpr char TEXT_GEC[]  PROGMEM = "GEC";
// constexpr char TEXT_LDC[]  PROGMEM = "LDC";
constexpr char TEXT_RLDI[] PROGMEM = "RLDI";
constexpr char TEXT_RLXA[] PROGMEM = "RLXA";
constexpr char TEXT_RNX[]  PROGMEM = "RNX";
constexpr char TEXT_RSXD[] PROGMEM = "RSXD";
constexpr char TEXT_SCAL[] PROGMEM = "SCAL";
constexpr char TEXT_SCM1[] PROGMEM = "SCM1";
constexpr char TEXT_SCM2[] PROGMEM = "SCM2";
constexpr char TEXT_SPM1[] PROGMEM = "SPM1";
constexpr char TEXT_SPM2[] PROGMEM = "SPM2";
constexpr char TEXT_SRET[] PROGMEM = "SRET";
constexpr char TEXT_STM[]  PROGMEM = "STM";
constexpr char TEXT_STPC[] PROGMEM = "STPC";
constexpr char TEXT_XID[]  PROGMEM = "XID";
constexpr char TEXT_XIE[]  PROGMEM = "XIE";

// CDP1804A
constexpr char TEXT_DACI[] PROGMEM = "DACI";
constexpr char TEXT_DADC[] PROGMEM = "DADC";
constexpr char TEXT_DADD[] PROGMEM = "DADD";
constexpr char TEXT_DADI[] PROGMEM = "DADI";
constexpr char TEXT_DBNZ[] PROGMEM = "DBNZ";
constexpr char TEXT_DSAV[] PROGMEM = "DSAV";
constexpr char TEXT_DSBI[] PROGMEM = "DSBI";
constexpr char TEXT_DSM[]  PROGMEM = "DSM";
constexpr char TEXT_DSMB[] PROGMEM = "DSMB";
constexpr char TEXT_DSMI[] PROGMEM = "DSMI";

// clang-format on
}  // namespace cdp1802
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
