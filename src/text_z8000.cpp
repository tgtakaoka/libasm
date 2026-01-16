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

#include "text_z8000.h"

namespace libasm {
namespace text {
namespace z8000 {
// clang-format off

constexpr char TEXT_Z8000_LIST[] PROGMEM = "Z8001, Z8002";
constexpr char TEXT_CPU_Z8001[] PROGMEM = "Z8001";
constexpr char TEXT_CPU_Z8002[] PROGMEM = "Z8002";

// constexpr char TEXT_ADC[]   PROGMEM = "ADC";
// constexpr char TEXT_ADCB[]  PROGMEM = "ADCB";
// constexpr char TEXT_ADD[]   PROGMEM = "ADD";
// constexpr char TEXT_ADDB[]  PROGMEM = "ADDB";
// constexpr char TEXT_ADDL[]  PROGMEM = "ADDL";
// constexpr char TEXT_AND[]   PROGMEM = "AND";
// constexpr char TEXT_ANDB[]  PROGMEM = "ANDB";
// constexpr char TEXT_BIT[]   PROGMEM = "BIT";
// constexpr char TEXT_BITB[]  PROGMEM = "BITB";
// constexpr char TEXT_CALL[]  PROGMEM = "CALL";
// constexpr char TEXT_CALR[]  PROGMEM = "CALR";
// constexpr char TEXT_CLR[]   PROGMEM = "CLR";
// constexpr char TEXT_CLRB[]  PROGMEM = "CLRB";
// constexpr char TEXT_COM[]   PROGMEM = "COM";
// constexpr char TEXT_COMB[]  PROGMEM = "COMB";
constexpr char TEXT_COMFLG[] PROGMEM = "COMFLG";
// constexpr char TEXT_CP[]    PROGMEM = "CP";
constexpr char TEXT_CPB[]    PROGMEM = "CPB";
// constexpr char TEXT_CPD[]   PROGMEM = "CPD";
constexpr char TEXT_CPDB[]   PROGMEM = "CPDB";
// constexpr char TEXT_CPDR[]  PROGMEM = "CPDR";
constexpr char TEXT_CPDRB[]  PROGMEM = "CPDRB";
// constexpr char TEXT_CPI[]   PROGMEM = "CPI";
constexpr char TEXT_CPIB[]   PROGMEM = "CPIB";
// constexpr char TEXT_CPIR[]  PROGMEM = "CPIR";
constexpr char TEXT_CPIRB[]  PROGMEM = "CPIRB";
// constexpr char TEXT_CPL[]   PROGMEM = "CPL";
constexpr char TEXT_CPSD[]   PROGMEM = "CPSD";
constexpr char TEXT_CPSDB[]  PROGMEM = "CPSDB";
constexpr char TEXT_CPSDR[]  PROGMEM = "CPSDR";
constexpr char TEXT_CPSDRB[] PROGMEM = "CPSDRB";
constexpr char TEXT_CPSI[]   PROGMEM = "CPSI";
constexpr char TEXT_CPSIB[]  PROGMEM = "CPSIB";
constexpr char TEXT_CPSIR[]  PROGMEM = "CPSIR";
constexpr char TEXT_CPSIRB[] PROGMEM = "CPSIRB";
constexpr char TEXT_DAB[]    PROGMEM = "DAB";
constexpr char TEXT_DBJNZ[]  PROGMEM = "DBJNZ";
// constexpr char TEXT_DEC[]   PROGMEM = "DEC";
// constexpr char TEXT_DECB[]  PROGMEM = "DECB";
// constexpr char TEXT_DI[]    PROGMEM = "DI";
// constexpr char TEXT_DIV[]   PROGMEM = "DIV";
// constexpr char TEXT_DIVL[]  PROGMEM = "DIVL";
// constexpr char TEXT_DJNZ[]  PROGMEM = "DJNZ";
// constexpr char TEXT_EI[]    PROGMEM = "EI";
// constexpr char TEXT_EX[]    PROGMEM = "EX";
constexpr char TEXT_EXB[]    PROGMEM = "EXB";
// constexpr char TEXT_EXTS[]  PROGMEM = "EXTS";
// constexpr char TEXT_EXTSB[] PROGMEM = "EXTSB";
constexpr char TEXT_EXTSL[]  PROGMEM = "EXTSL";
// constexpr char TEXT_HALT[]  PROGMEM = "HALT";
// constexpr char TEXT_IN[]    PROGMEM = "IN";
constexpr char TEXT_INB[]    PROGMEM = "INB";
// constexpr char TEXT_INC[]   PROGMEM = "INC";
// constexpr char TEXT_INCB[]  PROGMEM = "INCB";
// constexpr char TEXT_IND[]   PROGMEM = "IND";
constexpr char TEXT_INDB[]   PROGMEM = "INDB";
// constexpr char TEXT_INDR[]  PROGMEM = "INDR";
constexpr char TEXT_INDRB[]  PROGMEM = "INDRB";
// constexpr char TEXT_INI[]   PROGMEM = "INI";
constexpr char TEXT_INIB[]   PROGMEM = "INIB";
// constexpr char TEXT_INIR[]  PROGMEM = "INIR";
constexpr char TEXT_INIRB[]  PROGMEM = "INIRB";
// constexpr char TEXT_IRET[]  PROGMEM = "IRET";
// constexpr char TEXT_JP[]    PROGMEM = "JP";
// constexpr char TEXT_JR[]    PROGMEM = "JR";
// constexpr char TEXT_LD[]    PROGMEM = "LD";
// constexpr char TEXT_LDA[]   PROGMEM = "LDA";
// constexpr char TEXT_LDAR[]  PROGMEM = "LDAR";
// constexpr char TEXT_LDB[]   PROGMEM = "LDB";
// constexpr char TEXT_LDCTL[] PROGMEM = "LDCTL";
constexpr char TEXT_LDCTLB[] PROGMEM = "LDCTLB";
// constexpr char TEXT_LDD[]   PROGMEM = "LDD";
constexpr char TEXT_LDDB[]   PROGMEM = "LDDB";
// constexpr char TEXT_LDDR[]  PROGMEM = "LDDR";
constexpr char TEXT_LDDRB[]  PROGMEM = "LDDRB";
// constexpr char TEXT_LDI[]   PROGMEM = "LDI";
constexpr char TEXT_LDIB[]   PROGMEM = "LDIB";
// constexpr char TEXT_LDIR[]  PROGMEM = "LDIR";
constexpr char TEXT_LDIRB[]  PROGMEM = "LDIRB";
constexpr char TEXT_LDK[]    PROGMEM = "LDK";
constexpr char TEXT_LDL[]    PROGMEM = "LDL";
constexpr char TEXT_LDM[]    PROGMEM = "LDM";
constexpr char TEXT_LDPS[]   PROGMEM = "LDPS";
constexpr char TEXT_LDR[]    PROGMEM = "LDR";
constexpr char TEXT_LDRB[]   PROGMEM = "LDRB";
constexpr char TEXT_LDRL[]   PROGMEM = "LDRL";
constexpr char TEXT_MBIT[]   PROGMEM = "MBIT";
constexpr char TEXT_MREQ[]   PROGMEM = "MREQ";
constexpr char TEXT_MRES[]   PROGMEM = "MRES";
constexpr char TEXT_MSET[]   PROGMEM = "MSET";
// constexpr char TEXT_MULT[]  PROGMEM = "MULT";
constexpr char TEXT_MULTL[]  PROGMEM = "MULTL";
// constexpr char TEXT_NEG[]   PROGMEM = "NEG";
// constexpr char TEXT_NEGB[]  PROGMEM = "NEGB";
// constexpr char TEXT_NOP[]   PROGMEM = "NOP";
// constexpr char TEXT_OR[]    PROGMEM = "OR";
// constexpr char TEXT_ORB[]   PROGMEM = "ORB";
// constexpr char TEXT_OTDR[]  PROGMEM = "OTDR";
constexpr char TEXT_OTDRB[]  PROGMEM = "OTDRB";
// constexpr char TEXT_OTIR[]  PROGMEM = "OTIR";
constexpr char TEXT_OTIRB[]  PROGMEM = "OTIRB";
// constexpr char TEXT_OUT[]   PROGMEM = "OUT";
constexpr char TEXT_OUTB[]   PROGMEM = "OUTB";
// constexpr char TEXT_OUTD[]  PROGMEM = "OUTD";
constexpr char TEXT_OUTDB[]  PROGMEM = "OUTDB";
// constexpr char TEXT_OUTI[]  PROGMEM = "OUTI";
constexpr char TEXT_OUTIB[]  PROGMEM = "OUTIB";
// constexpr char TEXT_POP[]   PROGMEM = "POP";
constexpr char TEXT_POPL[]   PROGMEM = "POPL";
// constexpr char TEXT_PUSH[]  PROGMEM = "PUSH";
constexpr char TEXT_PUSHL[]  PROGMEM = "PUSHL";
// constexpr char TEXT_RES[]   PROGMEM = "RES";
constexpr char TEXT_RESB[]   PROGMEM = "RESB";
constexpr char TEXT_RESFLG[] PROGMEM = "RESFLG";
// constexpr char TEXT_RET[]   PROGMEM = "RET";
// constexpr char TEXT_RL[]    PROGMEM = "RL";
constexpr char TEXT_RLB[]    PROGMEM = "RLB";
// constexpr char TEXT_RLC[]   PROGMEM = "RLC";
constexpr char TEXT_RLCB[]   PROGMEM = "RLCB";
constexpr char TEXT_RLDB[]   PROGMEM = "RLDB";
// constexpr char TEXT_RR[]    PROGMEM = "RR";
constexpr char TEXT_RRB[]    PROGMEM = "RRB";
// constexpr char TEXT_RRC[]   PROGMEM = "RRC";
constexpr char TEXT_RRCB[]   PROGMEM = "RRCB";
constexpr char TEXT_RRDB[]   PROGMEM = "RRDB";
// constexpr char TEXT_SBC[]   PROGMEM = "SBC";
// constexpr char TEXT_SBCB[]  PROGMEM = "SBCB";
// constexpr char TEXT_SC[]    PROGMEM = "SC";
constexpr char TEXT_SDA[]    PROGMEM = "SDA";
constexpr char TEXT_SDAB[]   PROGMEM = "SDAB";
constexpr char TEXT_SDAL[]   PROGMEM = "SDAL";
constexpr char TEXT_SDL[]    PROGMEM = "SDL";
constexpr char TEXT_SDLB[]   PROGMEM = "SDLB";
constexpr char TEXT_SDLL[]   PROGMEM = "SDLL";
// constexpr char TEXT_SET[]   PROGMEM = "SET";
// constexpr char TEXT_SETB[]  PROGMEM = "SETB";
constexpr char TEXT_SETFLG[] PROGMEM = "SETFLG";
constexpr char TEXT_SIN[]    PROGMEM = "SIN";
constexpr char TEXT_SINB[]   PROGMEM = "SINB";
constexpr char TEXT_SIND[]   PROGMEM = "SIND";
constexpr char TEXT_SINDB[]  PROGMEM = "SINDB";
constexpr char TEXT_SINDR[]  PROGMEM = "SINDR";
constexpr char TEXT_SINDRB[] PROGMEM = "SINDRB";
constexpr char TEXT_SINI[]   PROGMEM = "SINI";
constexpr char TEXT_SINIB[]  PROGMEM = "SINIB";
constexpr char TEXT_SINIR[]  PROGMEM = "SINIR";
constexpr char TEXT_SINIRB[] PROGMEM = "SINIRB";
// constexpr char TEXT_SLA[]   PROGMEM = "SLA";
constexpr char TEXT_SLAB[]   PROGMEM = "SLAB";
constexpr char TEXT_SLAL[]   PROGMEM = "SLAL";
// constexpr char TEXT_SLL[]   PROGMEM = "SLL";
constexpr char TEXT_SLLB[]   PROGMEM = "SLLB";
constexpr char TEXT_SLLL[]   PROGMEM = "SLLL";
constexpr char TEXT_SOTDR[]  PROGMEM = "SOTDR";
constexpr char TEXT_SOTDRB[] PROGMEM = "SOTDRB";
constexpr char TEXT_SOTIR[]  PROGMEM = "SOTIR";
constexpr char TEXT_SOTIRB[] PROGMEM = "SOTIRB";
constexpr char TEXT_SOUT[]   PROGMEM = "SOUT";
constexpr char TEXT_SOUTB[]  PROGMEM = "SOUTB";
constexpr char TEXT_SOUTD[]  PROGMEM = "SOUTD";
constexpr char TEXT_SOUTDB[] PROGMEM = "SOUTDB";
constexpr char TEXT_SOUTI[]  PROGMEM = "SOUTI";
constexpr char TEXT_SOUTIB[] PROGMEM = "SOUTIB";
// constexpr char TEXT_SRA[]   PROGMEM = "SRA";
constexpr char TEXT_SRAB[]   PROGMEM = "SRAB";
constexpr char TEXT_SRAL[]   PROGMEM = "SRAL";
// constexpr char TEXT_SRL[]   PROGMEM = "SRL";
constexpr char TEXT_SRLB[]   PROGMEM = "SRLB";
constexpr char TEXT_SRLL[]   PROGMEM = "SRLL";
// constexpr char TEXT_SUB[]   PROGMEM = "SUB";
// constexpr char TEXT_SUBB[]  PROGMEM = "SUBB";
// constexpr char TEXT_SUBL[]  PROGMEM = "SUBL";
constexpr char TEXT_TCC[]    PROGMEM = "TCC";
constexpr char TEXT_TCCB[]   PROGMEM = "TCCB";
// constexpr char TEXT_TEST[]  PROGMEM = "TEST";
constexpr char TEXT_TESTB[]  PROGMEM = "TESTB";
constexpr char TEXT_TESTL[]  PROGMEM = "TESTL";
constexpr char TEXT_TRDB[]   PROGMEM = "TRDB";
constexpr char TEXT_TRDRB[]  PROGMEM = "TRDRB";
constexpr char TEXT_TRIB[]   PROGMEM = "TRIB";
constexpr char TEXT_TRIRB[]  PROGMEM = "TRIRB";
constexpr char TEXT_TRTDB[]  PROGMEM = "TRTDB";
constexpr char TEXT_TRTDRB[] PROGMEM = "TRTDRB";
constexpr char TEXT_TRTIB[]  PROGMEM = "TRTIB";
constexpr char TEXT_TRTIRB[] PROGMEM = "TRTIRB";
// constexpr char TEXT_TSET[]  PROGMEM = "TSET";
constexpr char TEXT_TSETB[]  PROGMEM = "TSETB";
// constexpr char TEXT_XOR[]   PROGMEM = "XOR";
// constexpr char TEXT_XORB[]  PROGMEM = "XORB";

constexpr char TEXT_REG_FCW[]     PROGMEM = "FCW";
constexpr char TEXT_REG_FLAGS[]   PROGMEM = "FLAGS";
constexpr char TEXT_REG_NSP[]     PROGMEM = "NSP";
constexpr char TEXT_REG_NSPOFF[]  PROGMEM = "NSPOFF";
constexpr char TEXT_REG_NSPSEG[]  PROGMEM = "NSPSEG";
constexpr char TEXT_REG_PSAP[]    PROGMEM = "PSAP";
constexpr char TEXT_REG_PSAPOFF[] PROGMEM = "PSAPOFF";
constexpr char TEXT_REG_PSAPSEG[] PROGMEM = "PSAPSEG";
constexpr char TEXT_REG_REFRESH[] PROGMEM = "REFRESH";
constexpr char TEXT_REG_RH[]      PROGMEM = "RH";
constexpr char TEXT_REG_RL[]      PROGMEM = "RL";
constexpr char TEXT_REG_RQ[]      PROGMEM = "RQ";
// constexpr char TEXT_REG_RR[] PROGMEM = "RR";

// constexpr char TEXT_CC_C[]   PROGMEM = "C";
// constexpr char TEXT_CC_EQ[]  PROGMEM = "EQ";
// constexpr char TEXT_CC_F[]   PROGMEM = "F";
// constexpr char TEXT_CC_GE[]  PROGMEM = "GE";
// constexpr char TEXT_CC_GT[]  PROGMEM = "GT";
// constexpr char TEXT_CC_LE[]  PROGMEM = "LE";
// constexpr char TEXT_CC_LT[]  PROGMEM = "LT";
// constexpr char TEXT_CC_MI[]  PROGMEM = "MI";
// constexpr char TEXT_CC_NC[]  PROGMEM = "NC";
// constexpr char TEXT_CC_NE[]  PROGMEM = "NE";
// constexpr char TEXT_CC_NOV[] PROGMEM = "NOV";
// constexpr char TEXT_CC_NZ[]  PROGMEM = "NZ";
// constexpr char TEXT_CC_OV[]  PROGMEM = "OV";
// constexpr char TEXT_CC_PL[]  PROGMEM = "PL";
// constexpr char TEXT_CC_T[]   PROGMEM = "";
// constexpr char TEXT_CC_UGE[] PROGMEM = "UGE";
// constexpr char TEXT_CC_UGT[] PROGMEM = "UGT";
// constexpr char TEXT_CC_ULE[] PROGMEM = "ULE";
// constexpr char TEXT_CC_ULT[] PROGMEM = "ULT";
// constexpr char TEXT_CC_Z[]   PROGMEM = "Z";

constexpr char TEXT_FLAG_C[] PROGMEM = "C";
constexpr char TEXT_FLAG_P[] PROGMEM = "P";
constexpr char TEXT_FLAG_S[] PROGMEM = "S";
constexpr char TEXT_FLAG_V[] PROGMEM = "V";
constexpr char TEXT_FLAG_Z[] PROGMEM = "Z";

constexpr char TEXT_INTR_NVI[] PROGMEM = "NVI";
constexpr char TEXT_INTR_VI[]  PROGMEM = "VI";

// clang-format on
}  // namespace z8000
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

