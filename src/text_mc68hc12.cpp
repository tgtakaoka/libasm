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

#include "text_mc68hc12.h"

namespace libasm {
namespace text {
namespace mc68hc12 {
// clang-format off

constexpr char TEXT_MC68HC12_LIST[] = "MC68HC12, MC68HCS12, MC68HCS12X";
constexpr char TEXT_CPU_68HC12[]     = "68HC12";
constexpr char TEXT_CPU_68HC12X[]    = "68HC12X";
constexpr char TEXT_CPU_68HCS12[]    = "68HCS12";
constexpr char TEXT_CPU_68HCS12X[]   = "68HCS12X";

constexpr char OPT_BOOL_MMC[] = "mmc";
constexpr char OPT_DESC_MMC[] = "enable logical page:address with memory map controller";

// MC68HC12
// constexpr char TEXT_ABA[]   = "ABA";
// constexpr char TEXT_ABX[]   = "ABX";
// constexpr char TEXT_ABY[]   = "ABY";
// constexpr char TEXT_ADCA[]  = "ADCA";
// constexpr char TEXT_ADCB[]  = "ADCB";
// constexpr char TEXT_ADDA[]  = "ADDA";
// constexpr char TEXT_ADDB[]  = "ADDB";
// constexpr char TEXT_ADDD[]  = "ADDD";
// constexpr char TEXT_ANDA[]  = "ANDA";
// constexpr char TEXT_ANDB[]  = "ANDB";
// constexpr char TEXT_ANDCC[] = "ANDCC";
// constexpr char TEXT_ASL[]   = "ASL";
// constexpr char TEXT_ASLA[]  = "ASLA";
// constexpr char TEXT_ASLB[]  = "ASLB";
// constexpr char TEXT_ASLD[]  = "ASLD";
// constexpr char TEXT_ASR[]   = "ASR";
// constexpr char TEXT_ASRA[]  = "ASRA";
// constexpr char TEXT_ASRB[]  = "ASRB";
// constexpr char TEXT_BCC[]   = "BCC";
// constexpr char TEXT_BCLR[]  = "BCLR";
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
// constexpr char TEXT_BSR[]   = "BSR";
// constexpr char TEXT_BVC[]   = "BVC";
// constexpr char TEXT_BVS[]   = "BVS";
// constexpr char TEXT_CALL[]  = "CALL";
// constexpr char TEXT_CBA[]   = "CBA";
// constexpr char TEXT_CLC[]   = "CLC";
// constexpr char TEXT_CLI[]   = "CLI";
// constexpr char TEXT_CLR[]   = "CLR";
// constexpr char TEXT_CLRA[]  = "CLRA";
// constexpr char TEXT_CLRB[]  = "CLRB";
// constexpr char TEXT_CLV[]   = "CLV";
// constexpr char TEXT_CMPA[]  = "CMPA";
// constexpr char TEXT_CMPB[]  = "CMPB";
// constexpr char TEXT_COM[]   = "COM";
// constexpr char TEXT_COMA[]  = "COMA";
// constexpr char TEXT_COMB[]  = "COMB";
// constexpr char TEXT_CPD[]   = "CPD";
// constexpr char TEXT_CPS[]   = "CPS";
// constexpr char TEXT_CPX[]   = "CPX";
// constexpr char TEXT_CPY[]   = "CPY";
// constexpr char TEXT_DAA[]   = "DAA";
// constexpr char TEXT_DBEQ[]  = "DBEQ";
// constexpr char TEXT_DBNE[]  = "DBNE";
// constexpr char TEXT_DEC[]   = "DEC";
// constexpr char TEXT_DECA[]  = "DECA";
// constexpr char TEXT_DECB[]  = "DECB";
// constexpr char TEXT_DES[]   = "DES";
// constexpr char TEXT_DEY[]   = "DEY";
// constexpr char TEXT_DEY[]   = "DEY";
// constexpr char TEXT_EDIV[]  = "EDIV";
// constexpr char TEXT_EDIVS[] = "EDIVS";
constexpr char TEXT_EMACS[] = "EMACS";
constexpr char TEXT_EMAXD[] = "EMAXD";
constexpr char TEXT_EMAXM[] = "EMAXM";
constexpr char TEXT_EMIND[] = "EMIND";
constexpr char TEXT_EMINM[] = "EMINM";
// constexpr char TEXT_EMUL[]  = "EMUL";
// constexpr char TEXT_EMULS[] = "EMULS";
// constexpr char TEXT_EORA[]  = "EORA";
// constexpr char TEXT_EORB[]  = "EORB";
constexpr char TEXT_ETBL[]  = "ETBL";
// constexpr char TEXT_EXG[]   = "EXG";
// constexpr char TEXT_FDIV[]  = "FDIV";
constexpr char TEXT_IBEQ[]  = "IBEQ";
constexpr char TEXT_IBNE[]  = "IBNE";
// constexpr char TEXT_IDIV[]  = "IDIV";
constexpr char TEXT_IDIVS[] = "IDIVS";
// constexpr char TEXT_INC[]   = "INC";
// constexpr char TEXT_INCA[]  = "INCA";
// constexpr char TEXT_INCB[]  = "INCB";
// constexpr char TEXT_INS[]   = "INS";
// constexpr char TEXT_INX[]   = "INX";
// constexpr char TEXT_INY[]   = "INY";
// constexpr char TEXT_JMP[]   = "JMP";
// constexpr char TEXT_JSR[]   = "JSR";
// constexpr char TEXT_LBCC[]  = "LBCC";
// constexpr char TEXT_LBCS[]  = "LBCS";
// constexpr char TEXT_LBEQ[]  = "LBEQ";
// constexpr char TEXT_LBGE[]  = "LBGE";
// constexpr char TEXT_LBGT[]  = "LBGT";
// constexpr char TEXT_LBHI[]  = "LBHI";
// constexpr char TEXT_LBHS[]  = "LBHS";
// constexpr char TEXT_LBLE[]  = "LBLE";
// constexpr char TEXT_LBLO[]  = "LBLO";
// constexpr char TEXT_LBLS[]  = "LBLS";
// constexpr char TEXT_LBLT[]  = "LBLT";
// constexpr char TEXT_LBMI[]  = "LBMI";
// constexpr char TEXT_LBNE[]  = "LBNE";
// constexpr char TEXT_LBPL[]  = "LBPL";
// constexpr char TEXT_LBRA[]  = "LBRA";
// constexpr char TEXT_LBRN[]  = "LBRN";
// constexpr char TEXT_LBVC[]  = "LBVC";
// constexpr char TEXT_LBVS[]  = "LBVS";
// constexpr char TEXT_LDAA[]  = "LDAA";
// constexpr char TEXT_LDAB[]  = "LDAB";
// constexpr char TEXT_LDD[]   = "LDD";
// constexpr char TEXT_LDS[]   = "LDS";
// constexpr char TEXT_LDX[]   = "LDX";
// constexpr char TEXT_LDY[]   = "LDY";
// constexpr char TEXT_LEAS[]  = "LEAS";
// constexpr char TEXT_LEAX[]  = "LEAX";
// constexpr char TEXT_LEAY[]  = "LEAY";
// constexpr char TEXT_LSL[]   = "LSL";
// constexpr char TEXT_LSLA[]  = "LSLA";
// constexpr char TEXT_LSLB[]  = "LSLB";
// constexpr char TEXT_LSLD[]  = "LSLD";
// constexpr char TEXT_LSR[]   = "LSR";
// constexpr char TEXT_LSRA[]  = "LSRA";
// constexpr char TEXT_LSRB[]  = "LSRB";
// constexpr char TEXT_LSRD[]  = "LSRD";
constexpr char TEXT_MAXA[]  = "MAXA";
constexpr char TEXT_MAXM[]  = "MAXM";
constexpr char TEXT_MEM[]   = "MEM";
constexpr char TEXT_MINA[]  = "MINA";
constexpr char TEXT_MINM[]  = "MINM";
// constexpr char TEXT_MOVB[]  = "MOVB";
// constexpr char TEXT_MOVW[]  = "MOVW";
// constexpr char TEXT_MUL[]   = "MUL";
// constexpr char TEXT_NEG[]   = "NEG";
// constexpr char TEXT_NEGA[]  = "NEGA";
// constexpr char TEXT_NEGB[]  = "NEGB";
// constexpr char TEXT_NOP[]   = "NOP";
// constexpr char TEXT_ORAA[]  = "ORAA";
// constexpr char TEXT_ORAB[]  = "ORAB";
// constexpr char TEXT_ORCC[]  = "ORCC";
// constexpr char TEXT_PSHA[]  = "PSHA";
// constexpr char TEXT_PSHB[]  = "PSHB";
constexpr char TEXT_PSHC[]  = "PSHC";
// constexpr char TEXT_PSHD[]  = "PSHD";
// constexpr char TEXT_PSHX[]  = "PSHX";
// constexpr char TEXT_PSHY[]  = "PSHY";
// constexpr char TEXT_PULA[]  = "PULA";
// constexpr char TEXT_PULB[]  = "PULB";
constexpr char TEXT_PULC[]  = "PULC";
// constexpr char TEXT_PULD[]  = "PULD";
// constexpr char TEXT_PULX[]  = "PULX";
// constexpr char TEXT_PULY[]  = "PULY";
constexpr char TEXT_REV[]   = "REV";
constexpr char TEXT_REVW[]  = "REVW";
// constexpr char TEXT_ROL[]   = "ROL";
// constexpr char TEXT_ROLA[]  = "ROLA";
// constexpr char TEXT_ROLB[]  = "ROLB";
// constexpr char TEXT_ROR[]   = "ROR";
// constexpr char TEXT_RORA[]  = "RORA";
// constexpr char TEXT_RORB[]  = "RORB";
// constexpr char TEXT_RTC[]   = "RTC";
// constexpr char TEXT_RTI[]   = "RTI";
// constexpr char TEXT_RTS[]   = "RTS";
// constexpr char TEXT_SBA[]   = "SBA";
// constexpr char TEXT_SBCA[]  = "SBCA";
// constexpr char TEXT_SBCB[]  = "SBCB";
// constexpr char TEXT_SEC[]   = "SEC";
// constexpr char TEXT_SEI[]   = "SEI";
// constexpr char TEXT_SEV[]   = "SEV";
// constexpr char TEXT_SEX[]   = "SEX";
// constexpr char TEXT_STAA[]  = "STAA";
// constexpr char TEXT_STAB[]  = "STAB";
// constexpr char TEXT_STD[]   = "STD";
// constexpr char TEXT_STOP[]  = "STOP";
// constexpr char TEXT_STS[]   = "STS";
// constexpr char TEXT_STX[]   = "STX";
// constexpr char TEXT_STY[]   = "STY";
// constexpr char TEXT_SUBA[]  = "SUBA";
// constexpr char TEXT_SUBB[]  = "SUBB";
// constexpr char TEXT_SUBD[]  = "SUBD";
// constexpr char TEXT_SWI[]   = "SWI";
// constexpr char TEXT_TAB[]   = "TAB";
// constexpr char TEXT_TAP[]   = "TAP";
// constexpr char TEXT_TBA[]   = "TBA";
constexpr char TEXT_TBEQ[]  = "TBEQ";
constexpr char TEXT_TBL[]   = "TBL";
constexpr char TEXT_TBNE[]  = "TBNE";
// constexpr char TEXT_TFR[]   = "TFR";
// constexpr char TEXT_TPA[]   = "TPA";
// constexpr char TEXT_TRAP[]  = "TRAP";
// constexpr char TEXT_TST[]   = "TST";
// constexpr char TEXT_TSTA[]  = "TSTA";
// constexpr char TEXT_TSTB[]  = "TSTB";
// constexpr char TEXT_WAI[]   = "WAI";
constexpr char TEXT_WAV[]   = "WAV";
// constexpr char TEXT_XGDX[]  = "XGDX";
// constexpr char TEXT_XGDY[]  = "XGDY";

// MC68HCS12X
// constexpr char TEXT_ADDX[]  = "ADDX";
constexpr char TEXT_ADDY[]  = "ADDY";
constexpr char TEXT_ADED[]  = "ADED";
constexpr char TEXT_ADEX[]  = "ADEX";
constexpr char TEXT_ADEY[]  = "ADEY";
constexpr char TEXT_ANDX[]  = "ANDX";
constexpr char TEXT_ANDY[]  = "ANDY";
// constexpr char TEXT_ASLW[]  = "ASLW";
// constexpr char TEXT_ASLX[]  = "ASLX";
constexpr char TEXT_ASLY[]  = "ASLY";
// constexpr char TEXT_ASRW[]  = "ASRW";
// constexpr char TEXT_ASRX[]  = "ASRX";
constexpr char TEXT_ASRY[]  = "ASRY";
constexpr char TEXT_BITX[]  = "BITX";
constexpr char TEXT_BITY[]  = "BITY";
constexpr char TEXT_BTAS[]  = "BTAS";
// constexpr char TEXT_CLRW[]  = "CLRW";
// constexpr char TEXT_CLRX[]  = "CLRX";
constexpr char TEXT_CLRY[]  = "CLRY";
// constexpr char TEXT_COMW[]  = "COMW";
// constexpr char TEXT_COMX[]  = "COMX";
constexpr char TEXT_COMY[]  = "COMY";
constexpr char TEXT_CPED[]  = "CPED";
constexpr char TEXT_CPES[]  = "CPES";
constexpr char TEXT_CPEX[]  = "CPEX";
constexpr char TEXT_CPEY[]  = "CPEY";
// constexpr char TEXT_DECW[]  = "DECW";
// constexpr char TEXT_DECX[]  = "DECX";
constexpr char TEXT_DECY[]  = "DECY";
constexpr char TEXT_EORX[]  = "EORX";
constexpr char TEXT_EORY[]  = "EORY";
constexpr char TEXT_GLDAA[] = "GLDAA";
constexpr char TEXT_GLDAB[] = "GLDAB";
constexpr char TEXT_GLDD[]  = "GLDD";
constexpr char TEXT_GLDS[]  = "GLDS";
constexpr char TEXT_GLDX[]  = "GLDX";
constexpr char TEXT_GLDY[]  = "GLDY";
constexpr char TEXT_GSTAA[] = "GSTAA";
constexpr char TEXT_GSTAB[] = "GSTAB";
constexpr char TEXT_GSTD[]  = "GSTD";
constexpr char TEXT_GSTS[]  = "GSTS";
constexpr char TEXT_GSTX[]  = "GSTX";
constexpr char TEXT_GSTY[]  = "GSTY";
// constexpr char TEXT_INCW[]  = "INCW";
// constexpr char TEXT_INCX[]  = "INCX";
constexpr char TEXT_INCY[]  = "INCY";
// constexpr char TEXT_LSLW[]  = "LSLW";
// constexpr char TEXT_LSLX[]  = "LSLX";
constexpr char TEXT_LSLY[]  = "LSLY";
// constexpr char TEXT_LSRW[]  = "LSRW";
// constexpr char TEXT_LSRX[]  = "LSRX";
constexpr char TEXT_LSRY[]  = "LSRY";
// constexpr char TEXT_NEGW[]  = "NEGW";
// constexpr char TEXT_NEGX[]  = "NEGX";
constexpr char TEXT_NEGY[]  = "NEGY";
constexpr char TEXT_ORX[]   = "ORX";
constexpr char TEXT_ORY[]   = "ORY";
constexpr char TEXT_PSHCW[] = "PSHCW";
constexpr char TEXT_PULCW[] = "PULCW";
// constexpr char TEXT_ROLW[]  = "ROLW";
// constexpr char TEXT_ROLX[]  = "ROLX";
constexpr char TEXT_ROLY[]  = "ROLY";
// constexpr char TEXT_RORW[]  = "RORW";
// constexpr char TEXT_RORX[]  = "RORX";
constexpr char TEXT_RORY[]  = "RORY";
constexpr char TEXT_SBED[]  = "SBED";
constexpr char TEXT_SBEX[]  = "SBEX";
constexpr char TEXT_SBEY[]  = "SBEY";
// constexpr char TEXT_SUBX[]  = "SUBX";
constexpr char TEXT_SUBY[]  = "SUBY";
constexpr char TEXT_SYS[]   = "SYS";
// constexpr char TEXT_TSTW[]  = "TSTW";
// constexpr char TEXT_TSTX[]  = "TSTX";
constexpr char TEXT_TSTY[]  = "TSTY";

// constexpr char TEXT_REG_A[]   = "REG_A";
// constexpr char TEXT_REG_B[]   = "REG_B";
// constexpr char TEXT_REG_CCR[] = "REG_CCR";
constexpr char TEXT_REG_CCRH[] = "CCRH";
constexpr char TEXT_REG_CCRL[] = "CCRL";
constexpr char TEXT_REG_CCRW[] = "CCRW";
// constexpr char TEXT_REG_D[]   = "REG_D";
// constexpr char TEXT_REG_PC[]  = "REG_PC";
// constexpr char TEXT_REG_PCR[] = "REG_PCR";
// constexpr char TEXT_REG_SP[]  = "REG_SP";
// constexpr char TEXT_REG_SPH[] = "SPH";
// constexpr char TEXT_REG_SPL[] = "SPL";
// constexpr char TEXT_REG_X[]   = "REG_X";
constexpr char TEXT_REG_XH[]   = "XH";
constexpr char TEXT_REG_XL[]   = "XL";
// constexpr char TEXT_REG_Y[]   = "REG_Y";
constexpr char TEXT_REG_YH[]   = "YH";
constexpr char TEXT_REG_YL[]   = "YL";

// clang-format on
}  // namespace mc68hc12
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
