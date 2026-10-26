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

constexpr char TEXT_Z8000_LIST[] = "Z8001, Z8002";
constexpr char TEXT_CPU_Z8001[] = "Z8001";
constexpr char TEXT_CPU_Z8002[] = "Z8002";

// constexpr char TEXT_ADC[]   = "ADC";
// constexpr char TEXT_ADCB[]  = "ADCB";
// constexpr char TEXT_ADD[]   = "ADD";
// constexpr char TEXT_ADDB[]  = "ADDB";
// constexpr char TEXT_ADDL[]  = "ADDL";
// constexpr char TEXT_AND[]   = "AND";
// constexpr char TEXT_ANDB[]  = "ANDB";
// constexpr char TEXT_BIT[]   = "BIT";
// constexpr char TEXT_BITB[]  = "BITB";
// constexpr char TEXT_CALL[]  = "CALL";
// constexpr char TEXT_CALR[]  = "CALR";
// constexpr char TEXT_CLR[]   = "CLR";
// constexpr char TEXT_CLRB[]  = "CLRB";
// constexpr char TEXT_COM[]   = "COM";
// constexpr char TEXT_COMB[]  = "COMB";
constexpr char TEXT_COMFLG[] = "COMFLG";
// constexpr char TEXT_CP[]    = "CP";
constexpr char TEXT_CPB[]    = "CPB";
// constexpr char TEXT_CPD[]   = "CPD";
constexpr char TEXT_CPDB[]   = "CPDB";
// constexpr char TEXT_CPDR[]  = "CPDR";
constexpr char TEXT_CPDRB[]  = "CPDRB";
// constexpr char TEXT_CPI[]   = "CPI";
constexpr char TEXT_CPIB[]   = "CPIB";
// constexpr char TEXT_CPIR[]  = "CPIR";
constexpr char TEXT_CPIRB[]  = "CPIRB";
// constexpr char TEXT_CPL[]   = "CPL";
constexpr char TEXT_CPSD[]   = "CPSD";
constexpr char TEXT_CPSDB[]  = "CPSDB";
constexpr char TEXT_CPSDR[]  = "CPSDR";
constexpr char TEXT_CPSDRB[] = "CPSDRB";
constexpr char TEXT_CPSI[]   = "CPSI";
constexpr char TEXT_CPSIB[]  = "CPSIB";
constexpr char TEXT_CPSIR[]  = "CPSIR";
constexpr char TEXT_CPSIRB[] = "CPSIRB";
constexpr char TEXT_DAB[]    = "DAB";
constexpr char TEXT_DBJNZ[]  = "DBJNZ";
// constexpr char TEXT_DEC[]   = "DEC";
// constexpr char TEXT_DECB[]  = "DECB";
// constexpr char TEXT_DI[]    = "DI";
// constexpr char TEXT_DIV[]   = "DIV";
// constexpr char TEXT_DIVL[]  = "DIVL";
// constexpr char TEXT_DJNZ[]  = "DJNZ";
// constexpr char TEXT_EI[]    = "EI";
// constexpr char TEXT_EX[]    = "EX";
constexpr char TEXT_EXB[]    = "EXB";
// constexpr char TEXT_EXTS[]  = "EXTS";
// constexpr char TEXT_EXTSB[] = "EXTSB";
constexpr char TEXT_EXTSL[]  = "EXTSL";
// constexpr char TEXT_HALT[]  = "HALT";
// constexpr char TEXT_IN[]    = "IN";
constexpr char TEXT_INB[]    = "INB";
// constexpr char TEXT_INC[]   = "INC";
// constexpr char TEXT_INCB[]  = "INCB";
// constexpr char TEXT_IND[]   = "IND";
constexpr char TEXT_INDB[]   = "INDB";
// constexpr char TEXT_INDR[]  = "INDR";
constexpr char TEXT_INDRB[]  = "INDRB";
// constexpr char TEXT_INI[]   = "INI";
constexpr char TEXT_INIB[]   = "INIB";
// constexpr char TEXT_INIR[]  = "INIR";
constexpr char TEXT_INIRB[]  = "INIRB";
// constexpr char TEXT_IRET[]  = "IRET";
// constexpr char TEXT_JP[]    = "JP";
// constexpr char TEXT_JR[]    = "JR";
// constexpr char TEXT_LD[]    = "LD";
// constexpr char TEXT_LDA[]   = "LDA";
// constexpr char TEXT_LDAR[]  = "LDAR";
// constexpr char TEXT_LDB[]   = "LDB";
// constexpr char TEXT_LDCTL[] = "LDCTL";
constexpr char TEXT_LDCTLB[] = "LDCTLB";
// constexpr char TEXT_LDD[]   = "LDD";
constexpr char TEXT_LDDB[]   = "LDDB";
// constexpr char TEXT_LDDR[]  = "LDDR";
constexpr char TEXT_LDDRB[]  = "LDDRB";
// constexpr char TEXT_LDI[]   = "LDI";
constexpr char TEXT_LDIB[]   = "LDIB";
// constexpr char TEXT_LDIR[]  = "LDIR";
constexpr char TEXT_LDIRB[]  = "LDIRB";
constexpr char TEXT_LDK[]    = "LDK";
constexpr char TEXT_LDL[]    = "LDL";
// constexpr char TEXT_LDM[]    = "LDM";
constexpr char TEXT_LDPS[]   = "LDPS";
constexpr char TEXT_LDR[]    = "LDR";
constexpr char TEXT_LDRB[]   = "LDRB";
constexpr char TEXT_LDRL[]   = "LDRL";
constexpr char TEXT_MBIT[]   = "MBIT";
constexpr char TEXT_MREQ[]   = "MREQ";
constexpr char TEXT_MRES[]   = "MRES";
constexpr char TEXT_MSET[]   = "MSET";
// constexpr char TEXT_MULT[]  = "MULT";
constexpr char TEXT_MULTL[]  = "MULTL";
// constexpr char TEXT_NEG[]   = "NEG";
// constexpr char TEXT_NEGB[]  = "NEGB";
// constexpr char TEXT_NOP[]   = "NOP";
// constexpr char TEXT_OR[]    = "OR";
// constexpr char TEXT_ORB[]   = "ORB";
// constexpr char TEXT_OTDR[]  = "OTDR";
constexpr char TEXT_OTDRB[]  = "OTDRB";
// constexpr char TEXT_OTIR[]  = "OTIR";
constexpr char TEXT_OTIRB[]  = "OTIRB";
// constexpr char TEXT_OUT[]   = "OUT";
constexpr char TEXT_OUTB[]   = "OUTB";
// constexpr char TEXT_OUTD[]  = "OUTD";
constexpr char TEXT_OUTDB[]  = "OUTDB";
// constexpr char TEXT_OUTI[]  = "OUTI";
constexpr char TEXT_OUTIB[]  = "OUTIB";
// constexpr char TEXT_POP[]   = "POP";
constexpr char TEXT_POPL[]   = "POPL";
// constexpr char TEXT_PUSH[]  = "PUSH";
constexpr char TEXT_PUSHL[]  = "PUSHL";
// constexpr char TEXT_RES[]   = "RES";
constexpr char TEXT_RESB[]   = "RESB";
constexpr char TEXT_RESFLG[] = "RESFLG";
// constexpr char TEXT_RET[]   = "RET";
// constexpr char TEXT_RL[]    = "RL";
constexpr char TEXT_RLB[]    = "RLB";
// constexpr char TEXT_RLC[]   = "RLC";
constexpr char TEXT_RLCB[]   = "RLCB";
constexpr char TEXT_RLDB[]   = "RLDB";
// constexpr char TEXT_RR[]    = "RR";
constexpr char TEXT_RRB[]    = "RRB";
// constexpr char TEXT_RRC[]   = "RRC";
constexpr char TEXT_RRCB[]   = "RRCB";
constexpr char TEXT_RRDB[]   = "RRDB";
// constexpr char TEXT_SBC[]   = "SBC";
// constexpr char TEXT_SBCB[]  = "SBCB";
// constexpr char TEXT_SC[]    = "SC";
constexpr char TEXT_SDA[]    = "SDA";
constexpr char TEXT_SDAB[]   = "SDAB";
constexpr char TEXT_SDAL[]   = "SDAL";
constexpr char TEXT_SDL[]    = "SDL";
constexpr char TEXT_SDLB[]   = "SDLB";
constexpr char TEXT_SDLL[]   = "SDLL";
// constexpr char TEXT_SET[]   = "SET";
// constexpr char TEXT_SETB[]  = "SETB";
constexpr char TEXT_SETFLG[] = "SETFLG";
// constexpr char TEXT_SIN[]    = "SIN";
constexpr char TEXT_SINB[]   = "SINB";
constexpr char TEXT_SIND[]   = "SIND";
constexpr char TEXT_SINDB[]  = "SINDB";
constexpr char TEXT_SINDR[]  = "SINDR";
constexpr char TEXT_SINDRB[] = "SINDRB";
constexpr char TEXT_SINI[]   = "SINI";
constexpr char TEXT_SINIB[]  = "SINIB";
constexpr char TEXT_SINIR[]  = "SINIR";
constexpr char TEXT_SINIRB[] = "SINIRB";
// constexpr char TEXT_SLA[]   = "SLA";
constexpr char TEXT_SLAB[]   = "SLAB";
constexpr char TEXT_SLAL[]   = "SLAL";
// constexpr char TEXT_SLL[]   = "SLL";
constexpr char TEXT_SLLB[]   = "SLLB";
constexpr char TEXT_SLLL[]   = "SLLL";
constexpr char TEXT_SOTDR[]  = "SOTDR";
constexpr char TEXT_SOTDRB[] = "SOTDRB";
constexpr char TEXT_SOTIR[]  = "SOTIR";
constexpr char TEXT_SOTIRB[] = "SOTIRB";
constexpr char TEXT_SOUT[]   = "SOUT";
constexpr char TEXT_SOUTB[]  = "SOUTB";
constexpr char TEXT_SOUTD[]  = "SOUTD";
constexpr char TEXT_SOUTDB[] = "SOUTDB";
constexpr char TEXT_SOUTI[]  = "SOUTI";
constexpr char TEXT_SOUTIB[] = "SOUTIB";
// constexpr char TEXT_SRA[]   = "SRA";
constexpr char TEXT_SRAB[]   = "SRAB";
constexpr char TEXT_SRAL[]   = "SRAL";
// constexpr char TEXT_SRL[]   = "SRL";
constexpr char TEXT_SRLB[]   = "SRLB";
constexpr char TEXT_SRLL[]   = "SRLL";
// constexpr char TEXT_SUB[]   = "SUB";
// constexpr char TEXT_SUBB[]  = "SUBB";
// constexpr char TEXT_SUBL[]  = "SUBL";
constexpr char TEXT_TCC[]    = "TCC";
constexpr char TEXT_TCCB[]   = "TCCB";
// constexpr char TEXT_TEST[]  = "TEST";
constexpr char TEXT_TESTB[]  = "TESTB";
constexpr char TEXT_TESTL[]  = "TESTL";
constexpr char TEXT_TRDB[]   = "TRDB";
constexpr char TEXT_TRDRB[]  = "TRDRB";
constexpr char TEXT_TRIB[]   = "TRIB";
constexpr char TEXT_TRIRB[]  = "TRIRB";
constexpr char TEXT_TRTDB[]  = "TRTDB";
constexpr char TEXT_TRTDRB[] = "TRTDRB";
constexpr char TEXT_TRTIB[]  = "TRTIB";
constexpr char TEXT_TRTIRB[] = "TRTIRB";
// constexpr char TEXT_TSET[]  = "TSET";
constexpr char TEXT_TSETB[]  = "TSETB";
// constexpr char TEXT_XOR[]   = "XOR";
// constexpr char TEXT_XORB[]  = "XORB";

constexpr char TEXT_REG_FCW[]     = "FCW";
constexpr char TEXT_REG_FLAGS[]   = "FLAGS";
// constexpr char TEXT_REG_NSP[]     = "NSP";
constexpr char TEXT_REG_NSPOFF[]  = "NSPOFF";
constexpr char TEXT_REG_NSPSEG[]  = "NSPSEG";
constexpr char TEXT_REG_PSAP[]    = "PSAP";
constexpr char TEXT_REG_PSAPOFF[] = "PSAPOFF";
constexpr char TEXT_REG_PSAPSEG[] = "PSAPSEG";
constexpr char TEXT_REG_REFRESH[] = "REFRESH";
// constexpr char TEXT_REG_RH[]      = "RH";
// constexpr char TEXT_REG_RL[]      = "RL";
constexpr char TEXT_REG_RQ[]      = "RQ";
// constexpr char TEXT_REG_RR[] = "RR";

// constexpr char TEXT_CC_C[]   = "C";
// constexpr char TEXT_CC_EQ[]  = "EQ";
// constexpr char TEXT_CC_F[]   = "F";
// constexpr char TEXT_CC_GE[]  = "GE";
// constexpr char TEXT_CC_GT[]  = "GT";
// constexpr char TEXT_CC_LE[]  = "LE";
// constexpr char TEXT_CC_LT[]  = "LT";
// constexpr char TEXT_CC_MI[]  = "MI";
// constexpr char TEXT_CC_NC[]  = "NC";
// constexpr char TEXT_CC_NE[]  = "NE";
// constexpr char TEXT_CC_NOV[] = "NOV";
// constexpr char TEXT_CC_NZ[]  = "NZ";
// constexpr char TEXT_CC_OV[]  = "OV";
// constexpr char TEXT_CC_PL[]  = "PL";
// constexpr char TEXT_CC_T[]   = "";
// constexpr char TEXT_CC_UGE[] = "UGE";
// constexpr char TEXT_CC_UGT[] = "UGT";
// constexpr char TEXT_CC_ULE[] = "ULE";
// constexpr char TEXT_CC_ULT[] = "ULT";
// constexpr char TEXT_CC_Z[]   = "Z";

constexpr char TEXT_FLAG_C[] = "C";
constexpr char TEXT_FLAG_P[] = "P";
constexpr char TEXT_FLAG_S[] = "S";
constexpr char TEXT_FLAG_V[] = "V";
constexpr char TEXT_FLAG_Z[] = "Z";

constexpr char TEXT_INTR_NVI[] = "NVI";
constexpr char TEXT_INTR_VI[]  = "VI";

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
