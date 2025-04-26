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
constexpr char TEXT_CPU_MC68HC16[] PROGMEM = "MC68HC16";
constexpr char TEXT_CPU_68HC16[]   PROGMEM = "68HC16";

// MC68HC16
// constexpr char TEXT_ABA[]   PROGMEM = "ABA";
// constexpr char TEXT_ABX[]   PROGMEM = "ABX";
// constexpr char TEXT_ABY[]   PROGMEM = "ABY";
constexpr char TEXT_ABZ[]   PROGMEM = "ABZ";
constexpr char TEXT_ACE[]   PROGMEM = "ACE";
constexpr char TEXT_ACED[]  PROGMEM = "ACED";
// constexpr char TEXT_ADCA[]  PROGMEM = "ADCA";
// constexpr char TEXT_ADCB[]  PROGMEM = "ADCB";
// constexpr char TEXT_ADCD[]  PROGMEM = "ADCD";
constexpr char TEXT_ADCE[]  PROGMEM = "ADCE";
// constexpr char TEXT_ADDA[]  PROGMEM = "ADDA";
// constexpr char TEXT_ADDB[]  PROGMEM = "ADDB";
// constexpr char TEXT_ADDD[]  PROGMEM = "ADDD";
// constexpr char TEXT_ADDE[]  PROGMEM = "ADDE";
// constexpr char TEXT_ADE[]   PROGMEM = "ADE";
// constexpr char TEXT_ADX[]   PROGMEM = "ADX";
constexpr char TEXT_ADY[]   PROGMEM = "ADY";
constexpr char TEXT_ADZ[]   PROGMEM = "ADZ";
constexpr char TEXT_AEX[]   PROGMEM = "AEX";
constexpr char TEXT_AEY[]   PROGMEM = "AEY";
constexpr char TEXT_AEZ[]   PROGMEM = "AEZ";
// constexpr char TEXT_AIS[]   PROGMEM = "AIS";
// constexpr char TEXT_AIX[]   PROGMEM = "AIX";
constexpr char TEXT_AIY[]   PROGMEM = "AIY";
constexpr char TEXT_AIZ[]   PROGMEM = "AIZ";
// constexpr char TEXT_ANDA[]  PROGMEM = "ANDA";
// constexpr char TEXT_ANDB[]  PROGMEM = "ANDB";
// constexpr char TEXT_ANDD[]  PROGMEM = "ANDD";
constexpr char TEXT_ANDE[]  PROGMEM = "ANDE";
// constexpr char TEXT_ANDP[]  PROGMEM = "ANDP";
// constexpr char TEXT_ASL[]   PROGMEM = "ASL";
// constexpr char TEXT_ASLA[]  PROGMEM = "ASLA";
// constexpr char TEXT_ASLB[]  PROGMEM = "ASLB";
// constexpr char TEXT_ASLD[]  PROGMEM = "ASLD";
constexpr char TEXT_ASLE[]  PROGMEM = "ASLE";
constexpr char TEXT_ASLM[]  PROGMEM = "ASLM";
constexpr char TEXT_ASLW[]  PROGMEM = "ASLW";
// constexpr char TEXT_ASR[]   PROGMEM = "ASR";
// constexpr char TEXT_ASRA[]  PROGMEM = "ASRA";
// constexpr char TEXT_ASRB[]  PROGMEM = "ASRB";
// constexpr char TEXT_ASRD[]  PROGMEM = "ASRD";
constexpr char TEXT_ASRE[]  PROGMEM = "ASRE";
constexpr char TEXT_ASRM[]  PROGMEM = "ASRM";
constexpr char TEXT_ASRW[]  PROGMEM = "ASRW";
// constexpr char TEXT_BCC[]   PROGMEM = "BCC";
// constexpr char TEXT_BCLR[]  PROGMEM = "BCLR";
constexpr char TEXT_BCLRW[] PROGMEM = "BCLRW";
// constexpr char TEXT_BCS[]   PROGMEM = "BCS";
// constexpr char TEXT_BEQ[]   PROGMEM = "BEQ";
// constexpr char TEXT_BGE[]   PROGMEM = "BGE";
// constexpr char TEXT_BGND[]  PROGMEM = "BGND";
// constexpr char TEXT_BGT[]   PROGMEM = "BGT";
// constexpr char TEXT_BHI[]   PROGMEM = "BHI";
// constexpr char TEXT_BHS[]   PROGMEM = "BHS";
// constexpr char TEXT_BITA[]  PROGMEM = "BITA";
// constexpr char TEXT_BITB[]  PROGMEM = "BITB";
// constexpr char TEXT_BLE[]   PROGMEM = "BLE";
// constexpr char TEXT_BLO[]   PROGMEM = "BLO";
// constexpr char TEXT_BLS[]   PROGMEM = "BLS";
// constexpr char TEXT_BLT[]   PROGMEM = "BLT";
// constexpr char TEXT_BMI[]   PROGMEM = "BMI";
// constexpr char TEXT_BNE[]   PROGMEM = "BNE";
// constexpr char TEXT_BPL[]   PROGMEM = "BPL";
// constexpr char TEXT_BRA[]   PROGMEM = "BRA";
// constexpr char TEXT_BRCLR[] PROGMEM = "BRCLR";
// constexpr char TEXT_BRN[]   PROGMEM = "BRN";
// constexpr char TEXT_BRSET[] PROGMEM = "BRSET";
// constexpr char TEXT_BSET[]  PROGMEM = "BSET";
constexpr char TEXT_BSETW[] PROGMEM = "BSETW";
// constexpr char TEXT_BSR[]   PROGMEM = "BSR";
// constexpr char TEXT_BVC[]   PROGMEM = "BVC";
// constexpr char TEXT_BVS[]   PROGMEM = "BVS";
// constexpr char TEXT_CBA[]   PROGMEM = "CBA";
// constexpr char TEXT_CLC[]   PROGMEM = "CLC";
// constexpr char TEXT_CLI[]   PROGMEM = "CLI";
// constexpr char TEXT_CLR[]   PROGMEM = "CLR";
// constexpr char TEXT_CLRA[]  PROGMEM = "CLRA";
// constexpr char TEXT_CLRB[]  PROGMEM = "CLRB";
// constexpr char TEXT_CLRD[]  PROGMEM = "CLRD";
// constexpr char TEXT_CLRE[]  PROGMEM = "CLRE";
constexpr char TEXT_CLRM[]  PROGMEM = "CLRM";
// constexpr char TEXT_CLRW[]  PROGMEM = "CLRW";
// constexpr char TEXT_CLV[]   PROGMEM = "CLV";
// constexpr char TEXT_CMPA[]  PROGMEM = "CMPA";
// constexpr char TEXT_CMPB[]  PROGMEM = "CMPB";
// constexpr char TEXT_COM[]   PROGMEM = "COM";
// constexpr char TEXT_COMA[]  PROGMEM = "COMA";
// constexpr char TEXT_COMB[]  PROGMEM = "COMB";
// constexpr char TEXT_COMD[]  PROGMEM = "COMD";
// constexpr char TEXT_COME[]  PROGMEM = "COME";
// constexpr char TEXT_COMW[]  PROGMEM = "COMW";
// constexpr char TEXT_CPD[]   PROGMEM = "CPD";
// constexpr char TEXT_CPE[]   PROGMEM = "CPE";
// constexpr char TEXT_CPS[]   PROGMEM = "CPS";
// constexpr char TEXT_CPX[]   PROGMEM = "CPX";
// constexpr char TEXT_CPY[]   PROGMEM = "CPY";
constexpr char TEXT_CPZ[]   PROGMEM = "CPZ";
// constexpr char TEXT_DAA[]   PROGMEM = "DAA";
// constexpr char TEXT_DEC[]   PROGMEM = "DEC";
// constexpr char TEXT_DECA[]  PROGMEM = "DECA";
// constexpr char TEXT_DECB[]  PROGMEM = "DECB";
// constexpr char TEXT_DECW[]  PROGMEM = "DECW";
// constexpr char TEXT_DES[]   PROGMEM = "DES";
// constexpr char TEXT_DEX[]   PROGMEM = "DEX";
// constexpr char TEXT_DEY[]   PROGMEM = "DEY";
// constexpr char TEXT_EDIV[]  PROGMEM = "EDIV";
// constexpr char TEXT_EDIVS[] PROGMEM = "EDIVS";
// constexpr char TEXT_EMUL[]  PROGMEM = "EMUL";
// constexpr char TEXT_EMULS[] PROGMEM = "EMULS";
// constexpr char TEXT_EORA[]  PROGMEM = "EORA";
// constexpr char TEXT_EORB[]  PROGMEM = "EORB";
// constexpr char TEXT_EORD[]  PROGMEM = "EORD";
constexpr char TEXT_EORE[]  PROGMEM = "EORE";
// constexpr char TEXT_FDIV[]  PROGMEM = "FDIV";
constexpr char TEXT_FMULS[] PROGMEM = "FMULS";
// constexpr char TEXT_IDIV[]  PROGMEM = "IDIV";
// constexpr char TEXT_INC[]   PROGMEM = "INC";
// constexpr char TEXT_INCA[]  PROGMEM = "INCA";
// constexpr char TEXT_INCB[]  PROGMEM = "INCB";
// constexpr char TEXT_INCW[]  PROGMEM = "INCW";
// constexpr char TEXT_INS[]   PROGMEM = "INS";
// constexpr char TEXT_INX[]   PROGMEM = "INX";
// constexpr char TEXT_INY[]   PROGMEM = "INY";
// constexpr char TEXT_JMP[]   PROGMEM = "JMP";
// constexpr char TEXT_JSR[]   PROGMEM = "JSR";
// constexpr char TEXT_LBCC[]  PROGMEM = "LBCC";
// constexpr char TEXT_LBCS[]  PROGMEM = "LBCS";
// constexpr char TEXT_LBEQ[]  PROGMEM = "LBEQ";
constexpr char TEXT_LBEV[]  PROGMEM = "LBEV";
// constexpr char TEXT_LBGE[]  PROGMEM = "LBGE";
// constexpr char TEXT_LBGT[]  PROGMEM = "LBGT";
// constexpr char TEXT_LBHI[]  PROGMEM = "LBHI";
// constexpr char TEXT_LBHS[]  PROGMEM = "LBHS";
// constexpr char TEXT_LBLO[]  PROGMEM = "LBLO";
// constexpr char TEXT_LBLE[]  PROGMEM = "LBLE";
// constexpr char TEXT_LBLS[]  PROGMEM = "LBLS";
// constexpr char TEXT_LBLT[]  PROGMEM = "LBLT";
// constexpr char TEXT_LBMI[]  PROGMEM = "LBMI";
constexpr char TEXT_LBMV[]  PROGMEM = "LBMV";
// constexpr char TEXT_LBNE[]  PROGMEM = "LBNE";
// constexpr char TEXT_LBPL[]  PROGMEM = "LBPL";
// constexpr char TEXT_LBRA[]  PROGMEM = "LBRA";
// constexpr char TEXT_LBRN[]  PROGMEM = "LBRN";
// constexpr char TEXT_LBSR[]  PROGMEM = "LBSR";
// constexpr char TEXT_LBVC[]  PROGMEM = "LBVC";
// constexpr char TEXT_LBVS[]  PROGMEM = "LBVS";
// constexpr char TEXT_LDAA[]  PROGMEM = "LDAA";
// constexpr char TEXT_LDAB[]  PROGMEM = "LDAB";
// constexpr char TEXT_LDD[]   PROGMEM = "LDD";
// constexpr char TEXT_LDE[]   PROGMEM = "LDE";
// constexpr char TEXT_LDED[]  PROGMEM = "LDED";
constexpr char TEXT_LDHI[]  PROGMEM = "LDHI";
// constexpr char TEXT_LDS[]   PROGMEM = "LDS";
// constexpr char TEXT_LDX[]   PROGMEM = "LDX";
// constexpr char TEXT_LDY[]   PROGMEM = "LDY";
constexpr char TEXT_LDZ[]   PROGMEM = "LDZ";
constexpr char TEXT_LPSTOP[] PROGMEM = "LPSTOP";
// constexpr char TEXT_LSL[]   PROGMEM = "LSL";
// constexpr char TEXT_LSLA[]  PROGMEM = "LSLA";
// constexpr char TEXT_LSLB[]  PROGMEM = "LSLB";
// constexpr char TEXT_LSLD[]  PROGMEM = "LSLD";
constexpr char TEXT_LSLE[]   PROGMEM = "LSLE";
constexpr char TEXT_LSLW[]   PROGMEM = "LSLW";
// constexpr char TEXT_LSR[]   PROGMEM = "LSR";
// constexpr char TEXT_LSRA[]  PROGMEM = "LSRA";
// constexpr char TEXT_LSRB[]  PROGMEM = "LSRB";
// constexpr char TEXT_LSRD[]  PROGMEM = "LSRD";
constexpr char TEXT_LSRE[]  PROGMEM = "LSRE";
// constexpr char TEXT_LSRW[]   PROGMEM = "LSRW";
// constexpr char TEXT_MAC[]   PROGMEM = "MAC";
// constexpr char TEXT_MOVB[]  PROGMEM = "MOVB";
// constexpr char TEXT_MOVW[]  PROGMEM = "MOVW";
// constexpr char TEXT_MUL[]   PROGMEM = "MUL";
// constexpr char TEXT_NEG[]   PROGMEM = "NEG";
// constexpr char TEXT_NEGA[]  PROGMEM = "NEGA";
// constexpr char TEXT_NEGB[]  PROGMEM = "NEGB";
// constexpr char TEXT_NEGD[]  PROGMEM = "NEGD";
constexpr char TEXT_NEGE[]  PROGMEM = "NEGE";
// constexpr char TEXT_NEGW[]  PROGMEM = "NEGW";
// constexpr char TEXT_NOP[]   PROGMEM = "NOP";
// constexpr char TEXT_ORAA[]  PROGMEM = "ORAA";
// constexpr char TEXT_ORAB[]  PROGMEM = "ORAB";
// constexpr char TEXT_ORD[]   PROGMEM = "ORD";
// constexpr char TEXT_ORE[]   PROGMEM = "ORE";
// constexpr char TEXT_ORP[]   PROGMEM = "ORP";
// constexpr char TEXT_PSHA[]  PROGMEM = "PSHA";
// constexpr char TEXT_PSHB[]  PROGMEM = "PSHB";
// constexpr char TEXT_PSHD[]  PROGMEM = "PSHD";
// constexpr char TEXT_PSHM[]  PROGMEM = "PSHM";
constexpr char TEXT_PSHMAC[] PROGMEM = "PSHMAC";
// constexpr char TEXT_PSHX[]  PROGMEM = "PSHX";
// constexpr char TEXT_PSHY[]  PROGMEM = "PSHY";
// constexpr char TEXT_PULA[]  PROGMEM = "PULA";
// constexpr char TEXT_PULB[]  PROGMEM = "PULB";
// constexpr char TEXT_PULD[]  PROGMEM = "PULD";
constexpr char TEXT_PULM[]  PROGMEM = "PULM";
constexpr char TEXT_PULMAC[] PROGMEM = "PULMAC";
// constexpr char TEXT_PULX[]  PROGMEM = "PULX";
// constexpr char TEXT_PULY[]  PROGMEM = "PULY";
constexpr char TEXT_RMAC[]  PROGMEM = "RMAC";
// constexpr char TEXT_ROL[]   PROGMEM = "ROL";
// constexpr char TEXT_ROLA[]  PROGMEM = "ROLA";
// constexpr char TEXT_ROLB[]  PROGMEM = "ROLB";
// constexpr char TEXT_ROLD[]  PROGMEM = "ROLD";
constexpr char TEXT_ROLE[]  PROGMEM = "ROLE";
// constexpr char TEXT_ROLW[]  PROGMEM = "ROLW";
// constexpr char TEXT_ROR[]   PROGMEM = "ROR";
// constexpr char TEXT_RORA[]  PROGMEM = "RORA";
// constexpr char TEXT_RORB[]  PROGMEM = "RORB";
// constexpr char TEXT_RORD[]  PROGMEM = "RORD";
constexpr char TEXT_RORE[]  PROGMEM = "RORE";
// constexpr char TEXT_RORW[]  PROGMEM = "RORW";
// constexpr char TEXT_RTI[]   PROGMEM = "RTI";
// constexpr char TEXT_RTS[]   PROGMEM = "RTS";
// constexpr char TEXT_SBA[]   PROGMEM = "SBA";
// constexpr char TEXT_SBCA[]  PROGMEM = "SBCA";
// constexpr char TEXT_SBCB[]  PROGMEM = "SBCB";
// constexpr char TEXT_SBCD[]  PROGMEM = "SBCD";
constexpr char TEXT_SBCE[]  PROGMEM = "SBCE";
constexpr char TEXT_SDE[]   PROGMEM = "SDE";
// constexpr char TEXT_SEC[]   PROGMEM = "SEC";
// constexpr char TEXT_SEI[]   PROGMEM = "SEI";
// constexpr char TEXT_SEV[]   PROGMEM = "SEV";
// constexpr char TEXT_STAA[]  PROGMEM = "STAA";
// constexpr char TEXT_STAB[]  PROGMEM = "STAB";
// constexpr char TEXT_STD[]   PROGMEM = "STD";
// constexpr char TEXT_STE[]   PROGMEM = "STE";
constexpr char TEXT_STED[]  PROGMEM = "STED";
// constexpr char TEXT_STS[]   PROGMEM = "STS";
// constexpr char TEXT_STX[]   PROGMEM = "STX";
// constexpr char TEXT_STY[]   PROGMEM = "STY";
// constexpr char TEXT_STZ[]   PROGMEM = "STZ";
// constexpr char TEXT_SUBA[]  PROGMEM = "SUBA";
// constexpr char TEXT_SUBB[]  PROGMEM = "SUBB";
// constexpr char TEXT_SUBD[]  PROGMEM = "SUBD";
// constexpr char TEXT_SUBE[]  PROGMEM = "SUBE";
// constexpr char TEXT_SWI[]   PROGMEM = "SWI";
// constexpr char TEXT_SXT[]   PROGMEM = "SXT";
// constexpr char TEXT_TAB[]   PROGMEM = "TAB";
// constexpr char TEXT_TAP[]   PROGMEM = "TAP";
// constexpr char TEXT_TBA[]   PROGMEM = "TBA";
constexpr char TEXT_TBEK[]  PROGMEM = "TBEK";
constexpr char TEXT_TBSK[]  PROGMEM = "TBSK";
constexpr char TEXT_TBXK[]  PROGMEM = "TBXK";
constexpr char TEXT_TBYK[]  PROGMEM = "TBYK";
constexpr char TEXT_TBZK[]  PROGMEM = "TBZK";
constexpr char TEXT_TDE[]   PROGMEM = "TDE";
constexpr char TEXT_TDMSK[] PROGMEM = "TDMSK";
constexpr char TEXT_TDP[]   PROGMEM = "TDP";
constexpr char TEXT_TED[]   PROGMEM = "TED";
constexpr char TEXT_TEDM[]  PROGMEM = "TEDM";
constexpr char TEXT_TEKB[]  PROGMEM = "TEKB";
constexpr char TEXT_TEM[]   PROGMEM = "TEM";
constexpr char TEXT_TMER[]  PROGMEM = "TMER";
constexpr char TEXT_TMET[]  PROGMEM = "TMET";
constexpr char TEXT_TMXED[] PROGMEM = "TMXED";
// constexpr char TEXT_TPA[]   PROGMEM = "TPA";
constexpr char TEXT_TPD[]   PROGMEM = "TPD";
constexpr char TEXT_TSKB[]  PROGMEM = "TSKB";
// constexpr char TEXT_TST[]   PROGMEM = "TST";
// constexpr char TEXT_TSTA[]  PROGMEM = "TSTA";
// constexpr char TEXT_TSTB[]  PROGMEM = "TSTB";
// constexpr char TEXT_TSTD[]  PROGMEM = "TSTD";
// constexpr char TEXT_TSTE[]  PROGMEM = "TSTE";
// constexpr char TEXT_TSTW[]  PROGMEM = "TSTW";
// constexpr char TEXT_TSX[]   PROGMEM = "TSX";
// constexpr char TEXT_TSY[]   PROGMEM = "TSY";
constexpr char TEXT_TSZ[]   PROGMEM = "TSZ";
constexpr char TEXT_TXKB[]  PROGMEM = "TXKB";
// constexpr char TEXT_TXS[]   PROGMEM = "TXS";
// constexpr char TEXT_TXY[]   PROGMEM = "TXY";
constexpr char TEXT_TXZ[]   PROGMEM = "TXZ";
constexpr char TEXT_TYKB[]  PROGMEM = "TYKB";
// constexpr char TEXT_TYS[]   PROGMEM = "TYS";
// constexpr char TEXT_TYX[]   PROGMEM = "TYX";
constexpr char TEXT_TYZ[]   PROGMEM = "TYZ";
constexpr char TEXT_TZKB[]  PROGMEM = "TZKB";
constexpr char TEXT_TZS[]   PROGMEM = "TZS";
constexpr char TEXT_TZX[]   PROGMEM = "TZX";
constexpr char TEXT_TZY[]   PROGMEM = "TZY";
// constexpr char TEXT_WAI[]   PROGMEM = "WAI";
constexpr char TEXT_XGAB[]  PROGMEM = "XGAB";
constexpr char TEXT_XGDE[]  PROGMEM = "XGDE";
// constexpr char TEXT_XGDX[]  PROGMEM = "XGDX";
// constexpr char TEXT_XGDY[]  PROGMEM = "XGDY";
constexpr char TEXT_XGDZ[]  PROGMEM = "XGDZ";
constexpr char TEXT_XGEX[]  PROGMEM = "XGEX";
constexpr char TEXT_XGEY[]  PROGMEM = "XGEY";
constexpr char TEXT_XGEZ[]  PROGMEM = "XGEZ";

// constexpr char TEXT_REG_CCR[] PROGMEM = "CCR";
// constexpr char TEXT_REG_D[]   PROGMEM = "D";
// constexpr char TEXT_REG_E[]   PROGMEM = "E";
// constexpr char TEXT_REG_K[]   PROGMEM = "K";
// constexpr char TEXT_REG_X[]   PROGMEM = "REG_X";
// constexpr char TEXT_REG_Y[]   PROGMEM = "REG_Y";
// constexpr char TEXT_REG_Z[]   PROGMEM = "Z";

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
