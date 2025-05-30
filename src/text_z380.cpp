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

#include "text_z380.h"

namespace libasm {
namespace text {
namespace z380 {

// clang-format off
constexpr char TEXT_Z380_LIST[] PROGMEM = "Z380, Z180, Z80";
// constexpr char TEXT_CPU_Z80[]  PROGMEM = "Z80";
// constexpr char TEXT_CPU_Z180[] PROGMEM = "Z180";
constexpr char TEXT_CPU_Z380[] PROGMEM = "Z380";

// constexpr char TEXT_ADC[]  PROGMEM = "ADC";
// constexpr char TEXT_ADD[]  PROGMEM = "ADD";
// constexpr char TEXT_AND[]  PROGMEM = "AND";
// constexpr char TEXT_BIT[]  PROGMEM = "BIT";
// constexpr char TEXT_CALL[] PROGMEM = "CALL";
// constexpr char TEXT_CCF[]  PROGMEM = "CCF";
// constexpr char TEXT_CP[]   PROGMEM = "CP";
// constexpr char TEXT_CPD[]  PROGMEM = "CPD";
// constexpr char TEXT_CPDR[] PROGMEM = "CPDR";
// constexpr char TEXT_CPI[]  PROGMEM = "CPI";
// constexpr char TEXT_CPIR[] PROGMEM = "CPIR";
// constexpr char TEXT_CPL[]  PROGMEM = "CPL";
// constexpr char TEXT_DAA[]  PROGMEM = "DAA";
// constexpr char TEXT_DEC[]  PROGMEM = "DEC";
// constexpr char TEXT_DI[]   PROGMEM = "DI";
// constexpr char TEXT_DJNZ[] PROGMEM = "DJNZ";
// constexpr char TEXT_EI[]   PROGMEM = "EI";
// constexpr char TEXT_EX[]   PROGMEM = "EX";
// constexpr char TEXT_EXX[]  PROGMEM = "EXX";
// constexpr char TEXT_HALT[] PROGMEM = "HALT";
// constexpr char TEXT_IM[]   PROGMEM = "IM";
// constexpr char TEXT_IN[]   PROGMEM = "IN";
// constexpr char TEXT_INC[]  PROGMEM = "INC";
// constexpr char TEXT_IND[]  PROGMEM = "IND";
// constexpr char TEXT_INDR[] PROGMEM = "INDR";
// constexpr char TEXT_INI[]  PROGMEM = "INI";
// constexpr char TEXT_INIR[] PROGMEM = "INIR";
// constexpr char TEXT_JP[]   PROGMEM = "JP";
// constexpr char TEXT_JR[]   PROGMEM = "JR";
// constexpr char TEXT_LD[]   PROGMEM = "LD";
// constexpr char TEXT_LDD[]  PROGMEM = "LDD";
// constexpr char TEXT_LDDR[] PROGMEM = "LDDR";
// constexpr char TEXT_LDI[]  PROGMEM = "LDI";
// constexpr char TEXT_LDIR[] PROGMEM = "LDIR";
// constexpr char TEXT_NEG[]  PROGMEM = "NEG";
// constexpr char TEXT_NOP[]  PROGMEM = "NOP";
// constexpr char TEXT_OR[]   PROGMEM = "OR";
// constexpr char TEXT_OTDR[] PROGMEM = "OTDR";
// constexpr char TEXT_OTIR[] PROGMEM = "OTIR";
// constexpr char TEXT_OUT[]  PROGMEM = "OUT";
// constexpr char TEXT_OUTD[] PROGMEM = "OUTD";
// constexpr char TEXT_OUTI[] PROGMEM = "OUTI";
// constexpr char TEXT_POP[]  PROGMEM = "POP";
// constexpr char TEXT_PUSH[] PROGMEM = "PUSH";
// constexpr char TEXT_RES[]  PROGMEM = "RES";
// constexpr char TEXT_RET[]  PROGMEM = "RET";
// constexpr char TEXT_RETI[] PROGMEM = "RETI";
// constexpr char TEXT_RETN[] PROGMEM = "RETN";
// constexpr char TEXT_RL[]   PROGMEM = "RL";
// constexpr char TEXT_RLA[]  PROGMEM = "RLA";
// constexpr char TEXT_RLC[]  PROGMEM = "RLC";
// constexpr char TEXT_RLCA[] PROGMEM = "RLCA";
// constexpr char TEXT_RLD[]  PROGMEM = "RLD";
// constexpr char TEXT_RR[]   PROGMEM = "RR";
// constexpr char TEXT_RRA[]  PROGMEM = "RRA";
// constexpr char TEXT_RRC[]  PROGMEM = "RRC";
// constexpr char TEXT_RRCA[] PROGMEM = "RRCA";
// constexpr char TEXT_RRD[]  PROGMEM = "RRD";
// constexpr char TEXT_RST[]  PROGMEM = "RST";
// constexpr char TEXT_SBC[]  PROGMEM = "SBC";
// constexpr char TEXT_SCF[]  PROGMEM = "SCF";
// constexpr char TEXT_SET[]  PROGMEM = "SET";
// constexpr char TEXT_SLA[]  PROGMEM = "SLA";
// constexpr char TEXT_SRA[]  PROGMEM = "SRA";
// constexpr char TEXT_SRL[]  PROGMEM = "SRL";
// constexpr char TEXT_SUB[]  PROGMEM = "SUB";
// constexpr char TEXT_XOR[]  PROGMEM = "XOR";

// Z180
// constexpr char TEXT_MLT[]   PROGMEM = "MLT";
// constexpr char TEXT_TST[]   PROGMEM = "TST";
// constexpr char TEXT_TSTIO[] PROGMEM = "TSTIO";
// constexpr char TEXT_IN0[]   PROGMEM = "IN0";
// constexpr char TEXT_OTDM[]  PROGMEM = "OTDM";
// constexpr char TEXT_OTDMR[] PROGMEM = "OTDMR";
// constexpr char TEXT_OTIM[]  PROGMEM = "OTIM";
// constexpr char TEXT_OTIMR[] PROGMEM = "OTIMR";
// constexpr char TEXT_OUT0[]  PROGMEM = "OUT0";
// constexpr char TEXT_SLP[]   PROGMEM = "SLP";

// Z380
constexpr char TEXT_ADCW[]  PROGMEM = "ADCW";
// constexpr char TEXT_ADDW[]  PROGMEM = "ADDW";
// constexpr char TEXT_ANDW[]  PROGMEM = "ANDW";
constexpr char TEXT_BTEST[] PROGMEM = "BTEST";
// constexpr char TEXT_CALR[]  PROGMEM = "CALR";
constexpr char TEXT_CPLW[]  PROGMEM = "CPLW";
// constexpr char TEXT_CPW[]   PROGMEM = "CPW";
// constexpr char TEXT_DECW[]   PROGMEM = "DECW";
// constexpr char TEXT_DIVUW[]  PROGMEM = "DIVUW";
// constexpr char TEXT_DIVW[]   PROGMEM = "DIVW";
constexpr char TEXT_DDIR[]  PROGMEM = "DDIR";
constexpr char TEXT_EXALL[] PROGMEM = "EXALL";
// constexpr char TEXT_EXTS[]  PROGMEM = "EXTS";
// constexpr char TEXT_EXTSW[] PROGMEM = "EXTSW";
constexpr char TEXT_EXXX[]  PROGMEM = "EXXX";
constexpr char TEXT_EXXY[]  PROGMEM = "EXXY";
constexpr char TEXT_INA[]   PROGMEM = "INA";
constexpr char TEXT_INAW[]  PROGMEM = "INAW";
// constexpr char TEXT_INCW[]   PROGMEM = "INCW";
// constexpr char TEXT_INDRW[] PROGMEM = "INDRW";
// constexpr char TEXT_INDW[]  PROGMEM = "INDW";
// constexpr char TEXT_INIRW[] PROGMEM = "INIRW";
// constexpr char TEXT_INIW[]  PROGMEM = "INIW";
// constexpr char TEXT_INW[]    PROGMEM = "INW";
// constexpr char TEXT_LDCTL[]  PROGMEM = "LDCTL";
constexpr char TEXT_LDDW[]  PROGMEM = "LDDW";
constexpr char TEXT_LDIRW[] PROGMEM = "LDIRW";
constexpr char TEXT_LDIW[]  PROGMEM = "LDIW";
constexpr char TEXT_LDDRW[] PROGMEM = "LDDRW";
// constexpr char TEXT_LDW[]    PROGMEM = "LDW";
// constexpr char TEXT_MULTUW[] PROGMEM = "MULTUW";
// constexpr char TEXT_MULTW[]  PROGMEM = "MULTW";
constexpr char TEXT_MTEST[]  PROGMEM = "MTEST";
// constexpr char TEXT_NEGW[]  PROGMEM = "NEGW";
// constexpr char TEXT_ORW[]   PROGMEM = "ORW";
// constexpr char TEXT_OTDRW[] PROGMEM = "OTDRW";
// constexpr char TEXT_OTIRW[] PROGMEM = "OTIRW";
constexpr char TEXT_OUTA[]  PROGMEM = "OUTA";
constexpr char TEXT_OUTAW[] PROGMEM = "OUTAW";
// constexpr char TEXT_OUTDW[] PROGMEM = "OUTDW";
// constexpr char TEXT_OUTIW[] PROGMEM = "OUTIW";
// constexpr char TEXT_OUTW[]   PROGMEM = "OUTW";
constexpr char TEXT_RESC[]  PROGMEM = "RESC";
constexpr char TEXT_RETB[]  PROGMEM = "RETB";
constexpr char TEXT_RLCW[]  PROGMEM = "RLCW";
constexpr char TEXT_RLW[]   PROGMEM = "RLW";
constexpr char TEXT_RRCW[]  PROGMEM = "RRCW";
constexpr char TEXT_RRW[]   PROGMEM = "RRW";
constexpr char TEXT_SBCW[]  PROGMEM = "SBCW";
// constexpr char TEXT_SETC[]  PROGMEM = "SETC";
constexpr char TEXT_SLAW[]  PROGMEM = "SLAW";
constexpr char TEXT_SRAW[]  PROGMEM = "SRAW";
constexpr char TEXT_SRLW[]  PROGMEM = "SRLW";
// constexpr char TEXT_SUBW[]  PROGMEM = "SUBW";
// constexpr char TEXT_SWAP[]  PROGMEM = "SWAP";
// constexpr char TEXT_XORW[]  PROGMEM = "XORW";

// Z80, Z180
// constexpr char TEXT_REG_BC[]  PROGMEM = "BC";
// constexpr char TEXT_REG_DE[]  PROGMEM = "DE";
// constexpr char TEXT_REG_HL[]  PROGMEM = "HL";
// constexpr char TEXT_REG_IX[]  PROGMEM = "IX";
// constexpr char TEXT_REG_IY[]  PROGMEM = "IY";
// constexpr char TEXT_REG_SP[]  PROGMEM = "SP";
// constexpr char TEXT_REG_AF[]  PROGMEM = "AF";
// constexpr char TEXT_REG_A[]   PROGMEM = "A";
// constexpr char TEXT_REG_B[]   PROGMEM = "B";
// constexpr char TEXT_REG_C[]   PROGMEM = "C";
// constexpr char TEXT_REG_D[]   PROGMEM = "D";
// constexpr char TEXT_REG_E[]   PROGMEM = "E";
// constexpr char TEXT_REG_H[]   PROGMEM = "H";
// constexpr char TEXT_REG_L[]   PROGMEM = "L";
// constexpr char TEXT_REG_R[]   PROGMEM = "R";
// constexpr char TEXT_REG_I[]   PROGMEM = "I";

// Z380
// constexpr char TEXT_REG_IXL[] PROGMEM = "IXL";
// constexpr char TEXT_REG_IYL[] PROGMEM = "IYL";
constexpr char TEXT_REG_DSR[] PROGMEM = "DSR";
constexpr char TEXT_REG_IXU[] PROGMEM = "IXU";
constexpr char TEXT_REG_IYU[] PROGMEM = "IYU";
// constexpr char TEXT_REG_SR[]  PROGMEM = "SR";
constexpr char TEXT_REG_XSR[] PROGMEM = "XSR";
constexpr char TEXT_REG_YSR[] PROGMEM = "YSR";

// Z80, Z180
// constexpr char TEXT_CC_NZ[] PROGMEM = "NZ";
// constexpr char TEXT_CC_Z[]  PROGMEM = "Z";
// constexpr char TEXT_CC_NC[] PROGMEM = "NC";
// constexpr char TEXT_CC_C[]  PROGMEM = "C";
// constexpr char TEXT_CC_PO[] PROGMEM = "PO";
// constexpr char TEXT_CC_PE[] PROGMEM = "PE";
// constexpr char TEXT_CC_P[]  PROGMEM = "P";
// constexpr char TEXT_CC_M[]  PROGMEM = "M";

// Z380
// constexpr char TEXT_CC_NS[] PROGMEM = "NS";
// constexpr char TEXT_CC_NV[] PROGMEM = "NV";
// constexpr char TEXT_CC_S[]  PROGMEM = "S";
// constexpr char TEXT_CC_V[]  PROGMEM = "V";
constexpr char PSTR_IB[]  PROGMEM = "IB";
constexpr char PSTR_IW[]  PROGMEM = "IW";
constexpr char PSTR_LCK[] PROGMEM = "LCK";
constexpr char PSTR_LW[]  PROGMEM = "LW";
constexpr char PSTR_W[]   PROGMEM = "W";
constexpr char PSTR_XM[]  PROGMEM = "XM";

// clang-format on

}  // namespace z380
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
