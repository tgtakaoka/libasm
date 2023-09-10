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
const char TEXT_CPU_LIST[]  PROGMEM = "Z8001, Z8002";
const char TEXT_CPU_Z8001[] PROGMEM = "Z8001";
const char TEXT_CPU_Z8002[] PROGMEM = "Z8002";

// const char TEXT_ADC[]    PROGMEM = "ADC";
// const char TEXT_ADCB[]   PROGMEM = "ADCB";
// const char TEXT_ADD[]    PROGMEM = "ADD";
// const char TEXT_ADDB[]   PROGMEM = "ADDB";
// const char TEXT_ADDL[]   PROGMEM = "ADDL";
// const char TEXT_AND[]    PROGMEM = "AND";
// const char TEXT_ANDB[]   PROGMEM = "ANDB";
// const char TEXT_BIT[]    PROGMEM = "BIT";
// const char TEXT_BITB[]   PROGMEM = "BITB";
// const char TEXT_CALL[]   PROGMEM = "CALL";
// const char TEXT_CALR[]   PROGMEM = "CALR";
// const char TEXT_CLR[]    PROGMEM = "CLR";
// const char TEXT_CLRB[]   PROGMEM = "CLRB";
// const char TEXT_COM[]    PROGMEM = "COM";
// const char TEXT_COMB[]   PROGMEM = "COMB";
const char TEXT_COMFLG[] PROGMEM = "COMFLG";
// const char TEXT_CP[]     PROGMEM = "CP";
const char TEXT_CPB[]    PROGMEM = "CPB";
// const char TEXT_CPD[]    PROGMEM = "CPD";
const char TEXT_CPDB[]   PROGMEM = "CPDB";
// const char TEXT_CPDR[]   PROGMEM = "CPDR";
const char TEXT_CPDRB[]  PROGMEM = "CPDRB";
// const char TEXT_CPI[]    PROGMEM = "CPI";
const char TEXT_CPIB[]   PROGMEM = "CPIB";
// const char TEXT_CPIR[]   PROGMEM = "CPIR";
const char TEXT_CPIRB[]  PROGMEM = "CPIRB";
// const char TEXT_CPL[]    PROGMEM = "CPL";
const char TEXT_CPSD[]   PROGMEM = "CPSD";
const char TEXT_CPSDB[]  PROGMEM = "CPSDB";
const char TEXT_CPSDR[]  PROGMEM = "CPSDR";
const char TEXT_CPSDRB[] PROGMEM = "CPSDRB";
const char TEXT_CPSI[]   PROGMEM = "CPSI";
const char TEXT_CPSIB[]  PROGMEM = "CPSIB";
const char TEXT_CPSIR[]  PROGMEM = "CPSIR";
const char TEXT_CPSIRB[] PROGMEM = "CPSIRB";
const char TEXT_DAB[]    PROGMEM = "DAB";
const char TEXT_DBJNZ[]  PROGMEM = "DBJNZ";
// const char TEXT_DEC[]    PROGMEM = "DEC";
// const char TEXT_DECB[]   PROGMEM = "DECB";
// const char TEXT_DI[]     PROGMEM = "DI";
// const char TEXT_DIV[]    PROGMEM = "DIV";
// const char TEXT_DIVL[]   PROGMEM = "DIVL";
// const char TEXT_DJNZ[]   PROGMEM = "DJNZ";
// const char TEXT_EI[]     PROGMEM = "EI";
// const char TEXT_EX[]     PROGMEM = "EX";
const char TEXT_EXB[]    PROGMEM = "EXB";
const char TEXT_EXTS[]   PROGMEM = "EXTS";
// const char TEXT_EXTSB[]  PROGMEM = "EXTSB";
const char TEXT_EXTSL[]  PROGMEM = "EXTSL";
// const char TEXT_HALT[]   PROGMEM = "HALT";
// const char TEXT_IN[]     PROGMEM = "IN";
const char TEXT_INB[]    PROGMEM = "INB";
// const char TEXT_INC[]    PROGMEM = "INC";
// const char TEXT_INCB[]   PROGMEM = "INCB";
// const char TEXT_IND[]    PROGMEM = "IND";
const char TEXT_INDB[]   PROGMEM = "INDB";
// const char TEXT_INDR[]   PROGMEM = "INDR";
const char TEXT_INDRB[]  PROGMEM = "INDRB";
// const char TEXT_INI[]    PROGMEM = "INI";
const char TEXT_INIB[]   PROGMEM = "INIB";
// const char TEXT_INIR[]   PROGMEM = "INIR";
const char TEXT_INIRB[]  PROGMEM = "INIRB";
// const char TEXT_IRET[]   PROGMEM = "IRET";
// const char TEXT_JP[]     PROGMEM = "JP";
// const char TEXT_JR[]     PROGMEM = "JR";
// const char TEXT_LD[]     PROGMEM = "LD";
// const char TEXT_LDA[]    PROGMEM = "LDA";
// const char TEXT_LDAR[]   PROGMEM = "LDAR";
// const char TEXT_LDB[]    PROGMEM = "LDB";
const char TEXT_LDCTL[]  PROGMEM = "LDCTL";
const char TEXT_LDCTLB[] PROGMEM = "LDCTLB";
// const char TEXT_LDD[]    PROGMEM = "LDD";
const char TEXT_LDDB[]   PROGMEM = "LDDB";
// const char TEXT_LDDR[]   PROGMEM = "LDDR";
const char TEXT_LDDRB[]  PROGMEM = "LDDRB";
// const char TEXT_LDI[]    PROGMEM = "LDI";
const char TEXT_LDIB[]   PROGMEM = "LDIB";
// const char TEXT_LDIR[]   PROGMEM = "LDIR";
const char TEXT_LDIRB[]  PROGMEM = "LDIRB";
const char TEXT_LDK[]    PROGMEM = "LDK";
const char TEXT_LDL[]    PROGMEM = "LDL";
const char TEXT_LDM[]    PROGMEM = "LDM";
const char TEXT_LDPS[]   PROGMEM = "LDPS";
const char TEXT_LDR[]    PROGMEM = "LDR";
const char TEXT_LDRB[]   PROGMEM = "LDRB";
const char TEXT_LDRL[]   PROGMEM = "LDRL";
const char TEXT_MBIT[]   PROGMEM = "MBIT";
const char TEXT_MREQ[]   PROGMEM = "MREQ";
const char TEXT_MRES[]   PROGMEM = "MRES";
const char TEXT_MSET[]   PROGMEM = "MSET";
// const char TEXT_MULT[]   PROGMEM = "MULT";
const char TEXT_MULTL[]  PROGMEM = "MULTL";
// const char TEXT_NEG[]    PROGMEM = "NEG";
// const char TEXT_NEGB[]   PROGMEM = "NEGB";
// const char TEXT_NOP[]    PROGMEM = "NOP";
// const char TEXT_OR[]     PROGMEM = "OR";
// const char TEXT_ORB[]    PROGMEM = "ORB";
// const char TEXT_OTDR[]   PROGMEM = "OTDR";
const char TEXT_OTDRB[]  PROGMEM = "OTDRB";
// const char TEXT_OTIR[]   PROGMEM = "OTIR";
const char TEXT_OTIRB[]  PROGMEM = "OTIRB";
// const char TEXT_OUT[]    PROGMEM = "OUT";
const char TEXT_OUTB[]   PROGMEM = "OUTB";
// const char TEXT_OUTD[]   PROGMEM = "OUTD";
const char TEXT_OUTDB[]  PROGMEM = "OUTDB";
// const char TEXT_OUTI[]   PROGMEM = "OUTI";
const char TEXT_OUTIB[]  PROGMEM = "OUTIB";
// const char TEXT_POP[]    PROGMEM = "POP";
const char TEXT_POPL[]   PROGMEM = "POPL";
// const char TEXT_PUSH[]   PROGMEM = "PUSH";
const char TEXT_PUSHL[]  PROGMEM = "PUSHL";
// const char TEXT_RES[]    PROGMEM = "RES";
const char TEXT_RESB[]   PROGMEM = "RESB";
const char TEXT_RESFLG[] PROGMEM = "RESFLG";
// const char TEXT_RET[]    PROGMEM = "RET";
// const char TEXT_RL[]     PROGMEM = "RL";
const char TEXT_RLB[]    PROGMEM = "RLB";
// const char TEXT_RLC[]    PROGMEM = "RLC";
const char TEXT_RLCB[]   PROGMEM = "RLCB";
const char TEXT_RLDB[]   PROGMEM = "RLDB";
// const char TEXT_RR[]     PROGMEM = "RR";
const char TEXT_RRB[]    PROGMEM = "RRB";
// const char TEXT_RRC[]    PROGMEM = "RRC";
const char TEXT_RRCB[]   PROGMEM = "RRCB";
const char TEXT_RRDB[]   PROGMEM = "RRDB";
// const char TEXT_SBC[]    PROGMEM = "SBC";
// const char TEXT_SBCB[]   PROGMEM = "SBCB";
const char TEXT_SC[]     PROGMEM = "SC";
const char TEXT_SDA[]    PROGMEM = "SDA";
const char TEXT_SDAB[]   PROGMEM = "SDAB";
const char TEXT_SDAL[]   PROGMEM = "SDAL";
const char TEXT_SDL[]    PROGMEM = "SDL";
const char TEXT_SDLB[]   PROGMEM = "SDLB";
const char TEXT_SDLL[]   PROGMEM = "SDLL";
// const char TEXT_SET[]    PROGMEM = "SET";
// const char TEXT_SETB[]   PROGMEM = "SETB";
const char TEXT_SETFLG[] PROGMEM = "SETFLG";
const char TEXT_SIN[]    PROGMEM = "SIN";
const char TEXT_SINB[]   PROGMEM = "SINB";
const char TEXT_SIND[]   PROGMEM = "SIND";
const char TEXT_SINDB[]  PROGMEM = "SINDB";
const char TEXT_SINDR[]  PROGMEM = "SINDR";
const char TEXT_SINDRB[] PROGMEM = "SINDRB";
const char TEXT_SINI[]   PROGMEM = "SINI";
const char TEXT_SINIB[]  PROGMEM = "SINIB";
const char TEXT_SINIR[]  PROGMEM = "SINIR";
const char TEXT_SINIRB[] PROGMEM = "SINIRB";
// const char TEXT_SLA[]    PROGMEM = "SLA";
const char TEXT_SLAB[]   PROGMEM = "SLAB";
const char TEXT_SLAL[]   PROGMEM = "SLAL";
// const char TEXT_SLL[]    PROGMEM = "SLL";
const char TEXT_SLLB[]   PROGMEM = "SLLB";
const char TEXT_SLLL[]   PROGMEM = "SLLL";
const char TEXT_SOTDR[]  PROGMEM = "SOTDR";
const char TEXT_SOTDRB[] PROGMEM = "SOTDRB";
const char TEXT_SOTIR[]  PROGMEM = "SOTIR";
const char TEXT_SOTIRB[] PROGMEM = "SOTIRB";
const char TEXT_SOUT[]   PROGMEM = "SOUT";
const char TEXT_SOUTB[]  PROGMEM = "SOUTB";
const char TEXT_SOUTD[]  PROGMEM = "SOUTD";
const char TEXT_SOUTDB[] PROGMEM = "SOUTDB";
const char TEXT_SOUTI[]  PROGMEM = "SOUTI";
const char TEXT_SOUTIB[] PROGMEM = "SOUTIB";
// const char TEXT_SRA[]    PROGMEM = "SRA";
const char TEXT_SRAB[]   PROGMEM = "SRAB";
const char TEXT_SRAL[]   PROGMEM = "SRAL";
// const char TEXT_SRL[]    PROGMEM = "SRL";
const char TEXT_SRLB[]   PROGMEM = "SRLB";
const char TEXT_SRLL[]   PROGMEM = "SRLL";
// const char TEXT_SUB[]    PROGMEM = "SUB";
// const char TEXT_SUBB[]   PROGMEM = "SUBB";
// const char TEXT_SUBL[]   PROGMEM = "SUBL";
const char TEXT_TCC[]    PROGMEM = "TCC";
const char TEXT_TCCB[]   PROGMEM = "TCCB";
// const char TEXT_TEST[]   PROGMEM = "TEST";
const char TEXT_TESTB[]  PROGMEM = "TESTB";
const char TEXT_TESTL[]  PROGMEM = "TESTL";
const char TEXT_TRDB[]   PROGMEM = "TRDB";
const char TEXT_TRDRB[]  PROGMEM = "TRDRB";
const char TEXT_TRIB[]   PROGMEM = "TRIB";
const char TEXT_TRIRB[]  PROGMEM = "TRIRB";
const char TEXT_TRTDB[]  PROGMEM = "TRTDB";
const char TEXT_TRTDRB[] PROGMEM = "TRTDRB";
const char TEXT_TRTIB[]  PROGMEM = "TRTIB";
const char TEXT_TRTIRB[] PROGMEM = "TRTIRB";
// const char TEXT_TSET[]   PROGMEM = "TSET";
const char TEXT_TSETB[]  PROGMEM = "TSETB";
// const char TEXT_XOR[]    PROGMEM = "XOR";
// const char TEXT_XORB[]   PROGMEM = "XORB";

const char TEXT_REG_FCW[]     PROGMEM = "FCW";
const char TEXT_REG_FLAGS[]   PROGMEM = "FLAGS";
const char TEXT_REG_NSPOFF[]  PROGMEM = "NSPOFF";
const char TEXT_REG_NSP[]     PROGMEM = "NSP";
const char TEXT_REG_NSPSEG[]  PROGMEM = "NSPSEG";
const char TEXT_REG_PSAPOFF[] PROGMEM = "PSAPOFF";
const char TEXT_REG_PSAP[]    PROGMEM = "PSAP";
const char TEXT_REG_PSAPSEG[] PROGMEM = "PSAPSEG";
const char TEXT_REG_REFRESH[] PROGMEM = "REFRESH";
const char TEXT_REG_RH[]      PROGMEM = "RH";
const char TEXT_REG_RL[]      PROGMEM = "RL";
const char TEXT_REG_RQ[]      PROGMEM = "RQ";
// const char TEXT_REG_RR[]      PROGMEM = "RR";

// const char TEXT_CC_C[]   PROGMEM = "C";
// const char TEXT_CC_EQ[]  PROGMEM = "EQ";
// const char TEXT_CC_F[]   PROGMEM = "F";
// const char TEXT_CC_GE[]  PROGMEM = "GE";
// const char TEXT_CC_GT[]  PROGMEM = "GT";
// const char TEXT_CC_LE[]  PROGMEM = "LE";
// const char TEXT_CC_LT[]  PROGMEM = "LT";
// const char TEXT_CC_MI[]  PROGMEM = "MI";
// const char TEXT_CC_NC[]  PROGMEM = "NC";
// const char TEXT_CC_NE[]  PROGMEM = "NE";
// const char TEXT_CC_NOV[] PROGMEM = "NOV";
// const char TEXT_CC_NZ[]  PROGMEM = "NZ";
// const char TEXT_CC_OV[]  PROGMEM = "OV";
// const char TEXT_CC_PL[]  PROGMEM = "PL";
// const char TEXT_CC_T[]   PROGMEM = "";
// const char TEXT_CC_UGE[] PROGMEM = "UGE";
// const char TEXT_CC_UGT[] PROGMEM = "UGT";
// const char TEXT_CC_ULE[] PROGMEM = "ULE";
// const char TEXT_CC_ULT[] PROGMEM = "ULT";
// const char TEXT_CC_Z[]   PROGMEM = "Z";

const char TEXT_FLAG_C[] PROGMEM = "C";
const char TEXT_FLAG_Z[] PROGMEM = "Z";
const char TEXT_FLAG_S[] PROGMEM = "S";
const char TEXT_FLAG_P[] PROGMEM = "P";
const char TEXT_FLAG_V[] PROGMEM = "V";

const char TEXT_INTR_VI[]  PROGMEM = "VI";
const char TEXT_INTR_NVI[] PROGMEM = "NVI";

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

