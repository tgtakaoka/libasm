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

#include "text_mc68hc16.h"

namespace libasm {
namespace text {
namespace mc68hc16 {
// clang-format off

constexpr char TEXT_CPU_MC68HC16[] = "MC68HC16";
constexpr char TEXT_CPU_68HC16[]   = "68HC16";

// MC68HC16
// constexpr char TEXT_ABA[]   = "ABA";
// constexpr char TEXT_ABX[]   = "ABX";
// constexpr char TEXT_ABY[]   = "ABY";
constexpr char TEXT_ABZ[]    = "ABZ";
constexpr char TEXT_ACE[]    = "ACE";
constexpr char TEXT_ACED[]   = "ACED";
// constexpr char TEXT_ADCA[]  = "ADCA";
// constexpr char TEXT_ADCB[]  = "ADCB";
// constexpr char TEXT_ADCD[]  = "ADCD";
constexpr char TEXT_ADCE[]   = "ADCE";
// constexpr char TEXT_ADDA[]  = "ADDA";
// constexpr char TEXT_ADDB[]  = "ADDB";
// constexpr char TEXT_ADDD[]  = "ADDD";
// constexpr char TEXT_ADDE[]  = "ADDE";
// constexpr char TEXT_ADE[]   = "ADE";
// constexpr char TEXT_ADX[]   = "ADX";
constexpr char TEXT_ADY[]    = "ADY";
constexpr char TEXT_ADZ[]    = "ADZ";
constexpr char TEXT_AEX[]    = "AEX";
constexpr char TEXT_AEY[]    = "AEY";
constexpr char TEXT_AEZ[]    = "AEZ";
// constexpr char TEXT_AIS[]   = "AIS";
// constexpr char TEXT_AIX[]   = "AIX";
constexpr char TEXT_AIY[]    = "AIY";
constexpr char TEXT_AIZ[]    = "AIZ";
// constexpr char TEXT_ANDA[]  = "ANDA";
// constexpr char TEXT_ANDB[]  = "ANDB";
// constexpr char TEXT_ANDD[]  = "ANDD";
constexpr char TEXT_ANDE[]   = "ANDE";
// constexpr char TEXT_ANDP[]  = "ANDP";
// constexpr char TEXT_ASL[]   = "ASL";
// constexpr char TEXT_ASLA[]  = "ASLA";
// constexpr char TEXT_ASLB[]  = "ASLB";
// constexpr char TEXT_ASLD[]  = "ASLD";
constexpr char TEXT_ASLE[]   = "ASLE";
constexpr char TEXT_ASLM[]   = "ASLM";
// constexpr char TEXT_ASLW[]   = "ASLW";
// constexpr char TEXT_ASR[]   = "ASR";
// constexpr char TEXT_ASRA[]  = "ASRA";
// constexpr char TEXT_ASRB[]  = "ASRB";
// constexpr char TEXT_ASRD[]  = "ASRD";
constexpr char TEXT_ASRE[]   = "ASRE";
constexpr char TEXT_ASRM[]   = "ASRM";
// constexpr char TEXT_ASRW[]   = "ASRW";
// constexpr char TEXT_BCC[]   = "BCC";
// constexpr char TEXT_BCLR[]  = "BCLR";
constexpr char TEXT_BCLRW[]  = "BCLRW";
// constexpr char TEXT_BCS[]   = "BCS";
// constexpr char TEXT_BEQ[]   = "BEQ";
// constexpr char TEXT_BGE[]   = "BGE";
// constexpr char TEXT_BGND[]  = "BGND";
// constexpr char TEXT_BGT[]   = "BGT";
// constexpr char TEXT_BHI[]   = "BHI";
// constexpr char TEXT_BHS[]   = "BHS";
// constexpr char TEXT_BITA[]  = "BITA";
// constexpr char TEXT_BITB[]  = "BITB";
// constexpr char TEXT_BLE[]   = "BLE";
// constexpr char TEXT_BLO[]   = "BLO";
// constexpr char TEXT_BLS[]   = "BLS";
// constexpr char TEXT_BLT[]   = "BLT";
// constexpr char TEXT_BMI[]   = "BMI";
// constexpr char TEXT_BNE[]   = "BNE";
// constexpr char TEXT_BPL[]   = "BPL";
// constexpr char TEXT_BRA[]   = "BRA";
// constexpr char TEXT_BRCLR[] = "BRCLR";
// constexpr char TEXT_BRN[]   = "BRN";
// constexpr char TEXT_BRSET[] = "BRSET";
// constexpr char TEXT_BSET[]  = "BSET";
constexpr char TEXT_BSETW[]  = "BSETW";
// constexpr char TEXT_BSR[]   = "BSR";
// constexpr char TEXT_BVC[]   = "BVC";
// constexpr char TEXT_BVS[]   = "BVS";
// constexpr char TEXT_CBA[]   = "CBA";
// constexpr char TEXT_CLC[]   = "CLC";
// constexpr char TEXT_CLI[]   = "CLI";
// constexpr char TEXT_CLR[]   = "CLR";
// constexpr char TEXT_CLRA[]  = "CLRA";
// constexpr char TEXT_CLRB[]  = "CLRB";
// constexpr char TEXT_CLRD[]  = "CLRD";
// constexpr char TEXT_CLRE[]  = "CLRE";
constexpr char TEXT_CLRM[]   = "CLRM";
// constexpr char TEXT_CLRW[]  = "CLRW";
// constexpr char TEXT_CLV[]   = "CLV";
// constexpr char TEXT_CMPA[]  = "CMPA";
// constexpr char TEXT_CMPB[]  = "CMPB";
// constexpr char TEXT_COM[]   = "COM";
// constexpr char TEXT_COMA[]  = "COMA";
// constexpr char TEXT_COMB[]  = "COMB";
// constexpr char TEXT_COMD[]  = "COMD";
// constexpr char TEXT_COME[]  = "COME";
// constexpr char TEXT_COMW[]  = "COMW";
// constexpr char TEXT_CPD[]   = "CPD";
// constexpr char TEXT_CPE[]   = "CPE";
// constexpr char TEXT_CPS[]   = "CPS";
// constexpr char TEXT_CPX[]   = "CPX";
// constexpr char TEXT_CPY[]   = "CPY";
constexpr char TEXT_CPZ[]    = "CPZ";
// constexpr char TEXT_DAA[]   = "DAA";
// constexpr char TEXT_DEC[]   = "DEC";
// constexpr char TEXT_DECA[]  = "DECA";
// constexpr char TEXT_DECB[]  = "DECB";
// constexpr char TEXT_DECW[]  = "DECW";
// constexpr char TEXT_DES[]   = "DES";
// constexpr char TEXT_DEX[]   = "DEX";
// constexpr char TEXT_DEY[]   = "DEY";
// constexpr char TEXT_EDIV[]  = "EDIV";
// constexpr char TEXT_EDIVS[] = "EDIVS";
// constexpr char TEXT_EMUL[]  = "EMUL";
// constexpr char TEXT_EMULS[] = "EMULS";
// constexpr char TEXT_EORA[]  = "EORA";
// constexpr char TEXT_EORB[]  = "EORB";
// constexpr char TEXT_EORD[]  = "EORD";
constexpr char TEXT_EORE[]   = "EORE";
// constexpr char TEXT_FDIV[]  = "FDIV";
constexpr char TEXT_FMULS[]  = "FMULS";
// constexpr char TEXT_IDIV[]  = "IDIV";
// constexpr char TEXT_INC[]   = "INC";
// constexpr char TEXT_INCA[]  = "INCA";
// constexpr char TEXT_INCB[]  = "INCB";
// constexpr char TEXT_INCW[]  = "INCW";
// constexpr char TEXT_INS[]   = "INS";
// constexpr char TEXT_INX[]   = "INX";
// constexpr char TEXT_INY[]   = "INY";
// constexpr char TEXT_JMP[]   = "JMP";
// constexpr char TEXT_JSR[]   = "JSR";
// constexpr char TEXT_LBCC[]  = "LBCC";
// constexpr char TEXT_LBCS[]  = "LBCS";
// constexpr char TEXT_LBEQ[]  = "LBEQ";
constexpr char TEXT_LBEV[]   = "LBEV";
// constexpr char TEXT_LBGE[]  = "LBGE";
// constexpr char TEXT_LBGT[]  = "LBGT";
// constexpr char TEXT_LBHI[]  = "LBHI";
// constexpr char TEXT_LBHS[]  = "LBHS";
// constexpr char TEXT_LBLE[]  = "LBLE";
// constexpr char TEXT_LBLO[]  = "LBLO";
// constexpr char TEXT_LBLS[]  = "LBLS";
// constexpr char TEXT_LBLT[]  = "LBLT";
// constexpr char TEXT_LBMI[]  = "LBMI";
constexpr char TEXT_LBMV[]   = "LBMV";
// constexpr char TEXT_LBNE[]  = "LBNE";
// constexpr char TEXT_LBPL[]  = "LBPL";
// constexpr char TEXT_LBRA[]  = "LBRA";
// constexpr char TEXT_LBRN[]  = "LBRN";
// constexpr char TEXT_LBSR[]  = "LBSR";
// constexpr char TEXT_LBVC[]  = "LBVC";
// constexpr char TEXT_LBVS[]  = "LBVS";
// constexpr char TEXT_LDAA[]  = "LDAA";
// constexpr char TEXT_LDAB[]  = "LDAB";
// constexpr char TEXT_LDD[]   = "LDD";
// constexpr char TEXT_LDE[]   = "LDE";
// constexpr char TEXT_LDED[]  = "LDED";
constexpr char TEXT_LDHI[]   = "LDHI";
// constexpr char TEXT_LDS[]   = "LDS";
// constexpr char TEXT_LDX[]   = "LDX";
// constexpr char TEXT_LDY[]   = "LDY";
constexpr char TEXT_LDZ[]    = "LDZ";
constexpr char TEXT_LPSTOP[] = "LPSTOP";
// constexpr char TEXT_LSL[]   = "LSL";
// constexpr char TEXT_LSLA[]  = "LSLA";
// constexpr char TEXT_LSLB[]  = "LSLB";
// constexpr char TEXT_LSLD[]  = "LSLD";
constexpr char TEXT_LSLE[]   = "LSLE";
// constexpr char TEXT_LSLW[]   = "LSLW";
// constexpr char TEXT_LSR[]   = "LSR";
// constexpr char TEXT_LSRA[]  = "LSRA";
// constexpr char TEXT_LSRB[]  = "LSRB";
// constexpr char TEXT_LSRD[]  = "LSRD";
constexpr char TEXT_LSRE[]   = "LSRE";
// constexpr char TEXT_LSRW[]  = "LSRW";
// constexpr char TEXT_MAC[]   = "MAC";
// constexpr char TEXT_MOVB[]  = "MOVB";
// constexpr char TEXT_MOVW[]  = "MOVW";
// constexpr char TEXT_MUL[]   = "MUL";
// constexpr char TEXT_NEG[]   = "NEG";
// constexpr char TEXT_NEGA[]  = "NEGA";
// constexpr char TEXT_NEGB[]  = "NEGB";
// constexpr char TEXT_NEGD[]  = "NEGD";
constexpr char TEXT_NEGE[]   = "NEGE";
// constexpr char TEXT_NEGW[]  = "NEGW";
// constexpr char TEXT_NOP[]   = "NOP";
// constexpr char TEXT_ORAA[]  = "ORAA";
// constexpr char TEXT_ORAB[]  = "ORAB";
// constexpr char TEXT_ORD[]   = "ORD";
// constexpr char TEXT_ORE[]   = "ORE";
// constexpr char TEXT_ORP[]   = "ORP";
// constexpr char TEXT_PSHA[]  = "PSHA";
// constexpr char TEXT_PSHB[]  = "PSHB";
// constexpr char TEXT_PSHD[]  = "PSHD";
// constexpr char TEXT_PSHM[]  = "PSHM";
constexpr char TEXT_PSHMAC[] = "PSHMAC";
// constexpr char TEXT_PSHX[]  = "PSHX";
// constexpr char TEXT_PSHY[]  = "PSHY";
// constexpr char TEXT_PULA[]  = "PULA";
// constexpr char TEXT_PULB[]  = "PULB";
// constexpr char TEXT_PULD[]  = "PULD";
constexpr char TEXT_PULM[]   = "PULM";
constexpr char TEXT_PULMAC[] = "PULMAC";
// constexpr char TEXT_PULX[]  = "PULX";
// constexpr char TEXT_PULY[]  = "PULY";
constexpr char TEXT_RMAC[]   = "RMAC";
// constexpr char TEXT_ROL[]   = "ROL";
// constexpr char TEXT_ROLA[]  = "ROLA";
// constexpr char TEXT_ROLB[]  = "ROLB";
// constexpr char TEXT_ROLD[]  = "ROLD";
constexpr char TEXT_ROLE[]   = "ROLE";
// constexpr char TEXT_ROLW[]  = "ROLW";
// constexpr char TEXT_ROR[]   = "ROR";
// constexpr char TEXT_RORA[]  = "RORA";
// constexpr char TEXT_RORB[]  = "RORB";
// constexpr char TEXT_RORD[]  = "RORD";
constexpr char TEXT_RORE[]   = "RORE";
// constexpr char TEXT_RORW[]  = "RORW";
// constexpr char TEXT_RTI[]   = "RTI";
// constexpr char TEXT_RTS[]   = "RTS";
// constexpr char TEXT_SBA[]   = "SBA";
// constexpr char TEXT_SBCA[]  = "SBCA";
// constexpr char TEXT_SBCB[]  = "SBCB";
// constexpr char TEXT_SBCD[]  = "SBCD";
constexpr char TEXT_SBCE[]   = "SBCE";
constexpr char TEXT_SDE[]    = "SDE";
// constexpr char TEXT_SEC[]   = "SEC";
// constexpr char TEXT_SEI[]   = "SEI";
// constexpr char TEXT_SEV[]   = "SEV";
// constexpr char TEXT_STAA[]  = "STAA";
// constexpr char TEXT_STAB[]  = "STAB";
// constexpr char TEXT_STD[]   = "STD";
// constexpr char TEXT_STE[]   = "STE";
constexpr char TEXT_STED[]   = "STED";
// constexpr char TEXT_STS[]   = "STS";
// constexpr char TEXT_STX[]   = "STX";
// constexpr char TEXT_STY[]   = "STY";
// constexpr char TEXT_STZ[]   = "STZ";
// constexpr char TEXT_SUBA[]  = "SUBA";
// constexpr char TEXT_SUBB[]  = "SUBB";
// constexpr char TEXT_SUBD[]  = "SUBD";
// constexpr char TEXT_SUBE[]  = "SUBE";
// constexpr char TEXT_SWI[]   = "SWI";
// constexpr char TEXT_SXT[]   = "SXT";
// constexpr char TEXT_TAB[]   = "TAB";
// constexpr char TEXT_TAP[]   = "TAP";
// constexpr char TEXT_TBA[]   = "TBA";
constexpr char TEXT_TBEK[]   = "TBEK";
constexpr char TEXT_TBSK[]   = "TBSK";
constexpr char TEXT_TBXK[]   = "TBXK";
constexpr char TEXT_TBYK[]   = "TBYK";
constexpr char TEXT_TBZK[]   = "TBZK";
constexpr char TEXT_TDE[]    = "TDE";
constexpr char TEXT_TDMSK[]  = "TDMSK";
constexpr char TEXT_TDP[]    = "TDP";
constexpr char TEXT_TED[]    = "TED";
constexpr char TEXT_TEDM[]   = "TEDM";
constexpr char TEXT_TEKB[]   = "TEKB";
constexpr char TEXT_TEM[]    = "TEM";
constexpr char TEXT_TMER[]   = "TMER";
constexpr char TEXT_TMET[]   = "TMET";
constexpr char TEXT_TMXED[]  = "TMXED";
// constexpr char TEXT_TPA[]   = "TPA";
constexpr char TEXT_TPD[]    = "TPD";
constexpr char TEXT_TSKB[]   = "TSKB";
// constexpr char TEXT_TST[]   = "TST";
// constexpr char TEXT_TSTA[]  = "TSTA";
// constexpr char TEXT_TSTB[]  = "TSTB";
// constexpr char TEXT_TSTD[]  = "TSTD";
// constexpr char TEXT_TSTE[]  = "TSTE";
// constexpr char TEXT_TSTW[]  = "TSTW";
// constexpr char TEXT_TSX[]   = "TSX";
// constexpr char TEXT_TSY[]   = "TSY";
constexpr char TEXT_TSZ[]    = "TSZ";
constexpr char TEXT_TXKB[]   = "TXKB";
// constexpr char TEXT_TXS[]   = "TXS";
// constexpr char TEXT_TXY[]   = "TXY";
constexpr char TEXT_TXZ[]    = "TXZ";
constexpr char TEXT_TYKB[]   = "TYKB";
// constexpr char TEXT_TYS[]   = "TYS";
// constexpr char TEXT_TYX[]   = "TYX";
constexpr char TEXT_TYZ[]    = "TYZ";
constexpr char TEXT_TZKB[]   = "TZKB";
constexpr char TEXT_TZS[]    = "TZS";
constexpr char TEXT_TZX[]    = "TZX";
constexpr char TEXT_TZY[]    = "TZY";
// constexpr char TEXT_WAI[]   = "WAI";
constexpr char TEXT_XGAB[]   = "XGAB";
constexpr char TEXT_XGDE[]   = "XGDE";
// constexpr char TEXT_XGDX[]  = "XGDX";
// constexpr char TEXT_XGDY[]  = "XGDY";
constexpr char TEXT_XGDZ[]   = "XGDZ";
constexpr char TEXT_XGEX[]   = "XGEX";
constexpr char TEXT_XGEY[]   = "XGEY";
constexpr char TEXT_XGEZ[]   = "XGEZ";

// constexpr char TEXT_REG_CCR[] = "CCR";
// constexpr char TEXT_REG_D[]   = "D";
// constexpr char TEXT_REG_E[]   = "E";
// constexpr char TEXT_REG_K[]   = "K";
// constexpr char TEXT_REG_X[]   = "REG_X";
// constexpr char TEXT_REG_Y[]   = "REG_Y";
// constexpr char TEXT_REG_Z[]   = "Z";

// clang-format on
}  // namespace mc68hc16
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
