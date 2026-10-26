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

constexpr char TEXT_CDP1802_LIST[] = "CDP1802, CDP1804, CDP1804A";
constexpr char TEXT_CPU_1802[]  = "1802";
constexpr char TEXT_CPU_1804[]  = "1804";
constexpr char TEXT_CPU_1804A[] = "1804A";

// constexpr char TEXT_null[] = "";

// CDP1802
// constexpr char TEXT_ADC[]  = "ADC";
constexpr char TEXT_ADCI[] = "ADCI";
// constexpr char TEXT_ADD[]  = "ADD";
// constexpr char TEXT_ADI[]  = "ADI";
// constexpr char TEXT_AND[]  = "AND";
// constexpr char TEXT_ANI[]  = "ANI";
constexpr char TEXT_B1[]   = "B1";
constexpr char TEXT_B2[]   = "B2";
constexpr char TEXT_B3[]   = "B3";
constexpr char TEXT_B4[]   = "B4";
constexpr char TEXT_BDF[]  = "BDF";
// constexpr char TEXT_BGE[]  = "BGE";
// constexpr char TEXT_BL[]   = "BL";
// constexpr char TEXT_BM[]   = "BM";
constexpr char TEXT_BN1[]  = "BN1";
constexpr char TEXT_BN2[]  = "BN2";
constexpr char TEXT_BN3[]  = "BN3";
constexpr char TEXT_BN4[]  = "BN4";
constexpr char TEXT_BNF[]  = "BNF";
constexpr char TEXT_BNQ[]  = "BNQ";
// constexpr char TEXT_BNZ[]  = "BNZ";
constexpr char TEXT_BPZ[]  = "BPZ";
constexpr char TEXT_BQ[]   = "BQ";
// constexpr char TEXT_BR[]   = "BR";
// constexpr char TEXT_BZ[]   = "BZ";
// constexpr char TEXT_DEC[]  = "DEC";
// constexpr char TEXT_DIS[]  = "DIS";
constexpr char TEXT_GHI[]  = "GHI";
constexpr char TEXT_GLO[]  = "GLO";
// constexpr char TEXT_IDL[]  = "IDL";
// constexpr char TEXT_INC[]  = "INC";
constexpr char TEXT_INP[]  = "INP";
constexpr char TEXT_IRX[]  = "IRX";
constexpr char TEXT_LBDF[] = "LBDF";
// constexpr char TEXT_LBGE[] = "LBGE";
constexpr char TEXT_LBL[]  = "LBL";
constexpr char TEXT_LBM[]  = "LBM";
constexpr char TEXT_LBNF[] = "LBNF";
constexpr char TEXT_LBNQ[] = "LBNQ";
constexpr char TEXT_LBNZ[] = "LBNZ";
constexpr char TEXT_LBPZ[] = "LBPZ";
constexpr char TEXT_LBQ[]  = "LBQ";
constexpr char TEXT_LBR[]  = "LBR";
constexpr char TEXT_LBZ[]  = "LBZ";
// constexpr char TEXT_LDA[]  = "LDA";
// constexpr char TEXT_LDI[]  = "LDI";
constexpr char TEXT_LDN[]  = "LDN";
// constexpr char TEXT_LDX[]  = "LDX";
constexpr char TEXT_LDXA[] = "LDXA";
constexpr char TEXT_LSDF[] = "LSDF";
constexpr char TEXT_LSIE[] = "LSIE";
constexpr char TEXT_LSKP[] = "LSKP";
constexpr char TEXT_LSNF[] = "LSNF";
constexpr char TEXT_LSNQ[] = "LSNQ";
constexpr char TEXT_LSNZ[] = "LSNZ";
constexpr char TEXT_LSQ[]  = "LSQ";
constexpr char TEXT_LSZ[]  = "LSZ";
// constexpr char TEXT_MARK[] = "MARK";
constexpr char TEXT_NBR[]  = "NBR";
constexpr char TEXT_NLBR[] = "NLBR";
// constexpr char TEXT_NOP[]  = "NOP";
// constexpr char TEXT_OR[]   = "OR";
// constexpr char TEXT_ORI[]  = "ORI";
// constexpr char TEXT_OUT[]  = "OUT";
constexpr char TEXT_PHI[]  = "PHI";
constexpr char TEXT_PLO[]  = "PLO";
constexpr char TEXT_REQ[]  = "REQ";
// constexpr char TEXT_RET[]  = "RET";
constexpr char TEXT_RSHL[] = "RSHL";
constexpr char TEXT_RSHR[] = "RSHR";
constexpr char TEXT_SAV[]  = "SAV";
// constexpr char TEXT_SD[]   = "SD";
constexpr char TEXT_SDB[]  = "SDB";
constexpr char TEXT_SDBI[] = "SDBI";
constexpr char TEXT_SDI[]  = "SDI";
// constexpr char TEXT_SEP[]  = "SEP";
// constexpr char TEXT_SEQ[]  = "SEQ";
// constexpr char TEXT_SEX[]  = "SEX";
// constexpr char TEXT_SHL[]  = "SHL";
constexpr char TEXT_SHLC[] = "SHLC";
// constexpr char TEXT_SHR[]  = "SHR";
constexpr char TEXT_SHRC[] = "SHRC";
// constexpr char TEXT_SKP[]  = "SKP";
// constexpr char TEXT_SM[]   = "SM";
constexpr char TEXT_SMB[]  = "SMB";
constexpr char TEXT_SMBI[] = "SMBI";
// constexpr char TEXT_SMI[]  = "SMI";
// constexpr char TEXT_STR[]  = "STR";
constexpr char TEXT_STXD[] = "STXD";
// constexpr char TEXT_XOR[]  = "XOR";
// constexpr char TEXT_XRI[]  = "XRI";

// CDP1804
constexpr char TEXT_BCI[]  = "BCI";
constexpr char TEXT_BXI[]  = "BXI";
constexpr char TEXT_CID[]  = "CID";
constexpr char TEXT_CIE[]  = "CIE";
constexpr char TEXT_DTC[]  = "DTC";
constexpr char TEXT_ETQ[]  = "ETQ";
constexpr char TEXT_GEC[]  = "GEC";
// constexpr char TEXT_LDC[] = "LDC";
constexpr char TEXT_RLDI[] = "RLDI";
constexpr char TEXT_RLXA[] = "RLXA";
constexpr char TEXT_RNX[]  = "RNX";
constexpr char TEXT_RSXD[] = "RSXD";
constexpr char TEXT_SCAL[] = "SCAL";
constexpr char TEXT_SCM1[] = "SCM1";
constexpr char TEXT_SCM2[] = "SCM2";
constexpr char TEXT_SPM1[] = "SPM1";
constexpr char TEXT_SPM2[] = "SPM2";
constexpr char TEXT_SRET[] = "SRET";
// constexpr char TEXT_STM[]  = "STM";
constexpr char TEXT_STPC[] = "STPC";
constexpr char TEXT_XID[]  = "XID";
constexpr char TEXT_XIE[]  = "XIE";

// CDP1804A
constexpr char TEXT_DACI[] = "DACI";
constexpr char TEXT_DADC[] = "DADC";
// constexpr char TEXT_DADD[] = "DADD";
constexpr char TEXT_DADI[] = "DADI";
// constexpr char TEXT_DBNZ[] = "DBNZ";
constexpr char TEXT_DSAV[] = "DSAV";
constexpr char TEXT_DSBI[] = "DSBI";
constexpr char TEXT_DSM[]  = "DSM";
constexpr char TEXT_DSMB[] = "DSMB";
constexpr char TEXT_DSMI[] = "DSMI";

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
