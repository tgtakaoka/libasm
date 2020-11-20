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

#ifndef __TEXT_Z8000__
#define __TEXT_Z8000__

#include "config_host.h"

namespace libasm {
namespace z8000 {

// clang-format off
static const char TEXT_CPU_LIST[] PROGMEM   = "Z8001, Z8002";
static const char TEXT_CPU_Z8001[] PROGMEM  = "Z8001";
static constexpr const char *TEXT_CPU_Z8002 = (TEXT_CPU_LIST + 7);

static const char TEXT_ADC[]    PROGMEM = "ADC";
static const char TEXT_ADCB[]   PROGMEM = "ADCB";
static const char TEXT_ADD[]    PROGMEM = "ADD";
static const char TEXT_ADDB[]   PROGMEM = "ADDB";
static const char TEXT_ADDL[]   PROGMEM = "ADDL";
static const char TEXT_AND[]    PROGMEM = "AND";
static const char TEXT_ANDB[]   PROGMEM = "ANDB";
static const char TEXT_BIT[]    PROGMEM = "BIT";
static const char TEXT_BITB[]   PROGMEM = "BITB";
static const char TEXT_CALL[]   PROGMEM = "CALL";
static const char TEXT_CALR[]   PROGMEM = "CALR";
static const char TEXT_CLR[]    PROGMEM = "CLR";
static const char TEXT_CLRB[]   PROGMEM = "CLRB";
static const char TEXT_COM[]    PROGMEM = "COM";
static const char TEXT_COMB[]   PROGMEM = "COMB";
static const char TEXT_COMFLG[] PROGMEM = "COMFLG";
static const char TEXT_CP[]     PROGMEM = "CP";
static const char TEXT_CPB[]    PROGMEM = "CPB";
static const char TEXT_CPD[]    PROGMEM = "CPD";
static const char TEXT_CPDB[]   PROGMEM = "CPDB";
static const char TEXT_CPDR[]   PROGMEM = "CPDR";
static const char TEXT_CPDRB[]  PROGMEM = "CPDRB";
static const char TEXT_CPI[]    PROGMEM = "CPI";
static const char TEXT_CPIB[]   PROGMEM = "CPIB";
static const char TEXT_CPIR[]   PROGMEM = "CPIR";
static const char TEXT_CPIRB[]  PROGMEM = "CPIRB";
static const char TEXT_CPL[]    PROGMEM = "CPL";
static const char TEXT_CPSD[]   PROGMEM = "CPSD";
static const char TEXT_CPSDB[]  PROGMEM = "CPSDB";
static const char TEXT_CPSDR[]  PROGMEM = "CPSDR";
static const char TEXT_CPSDRB[] PROGMEM = "CPSDRB";
static const char TEXT_CPSI[]   PROGMEM = "CPSI";
static const char TEXT_CPSIB[]  PROGMEM = "CPSIB";
static const char TEXT_CPSIR[]  PROGMEM = "CPSIR";
static const char TEXT_CPSIRB[] PROGMEM = "CPSIRB";
static const char TEXT_DAB[]    PROGMEM = "DAB";
static const char TEXT_DBJNZ[]  PROGMEM = "DBJNZ";
static const char TEXT_DEC[]    PROGMEM = "DEC";
static const char TEXT_DECB[]   PROGMEM = "DECB";
static const char TEXT_DI[]     PROGMEM = "DI";
static const char TEXT_DIV[]    PROGMEM = "DIV";
static const char TEXT_DIVL[]   PROGMEM = "DIVL";
static const char TEXT_DJNZ[]   PROGMEM = "DJNZ";
static const char TEXT_EI[]     PROGMEM = "EI";
static const char TEXT_EX[]     PROGMEM = "EX";
static const char TEXT_EXB[]    PROGMEM = "EXB";
static const char TEXT_EXTS[]   PROGMEM = "EXTS";
static const char TEXT_EXTSB[]  PROGMEM = "EXTSB";
static const char TEXT_EXTSL[]  PROGMEM = "EXTSL";
static const char TEXT_HALT[]   PROGMEM = "HALT";
static const char TEXT_IN[]     PROGMEM = "IN";
static const char TEXT_INB[]    PROGMEM = "INB";
static const char TEXT_INC[]    PROGMEM = "INC";
static const char TEXT_INCB[]   PROGMEM = "INCB";
static const char TEXT_IND[]    PROGMEM = "IND";
static const char TEXT_INDB[]   PROGMEM = "INDB";
static const char TEXT_INDR[]   PROGMEM = "INDR";
static const char TEXT_INDRB[]  PROGMEM = "INDRB";
static const char TEXT_INI[]    PROGMEM = "INI";
static const char TEXT_INIB[]   PROGMEM = "INIB";
static const char TEXT_INIR[]   PROGMEM = "INIR";
static const char TEXT_INIRB[]  PROGMEM = "INIRB";
static const char TEXT_IRET[]   PROGMEM = "IRET";
static const char TEXT_JP[]     PROGMEM = "JP";
static const char TEXT_JR[]     PROGMEM = "JR";
static const char TEXT_LD[]     PROGMEM = "LD";
static const char TEXT_LDA[]    PROGMEM = "LDA";
static const char TEXT_LDAR[]   PROGMEM = "LDAR";
static const char TEXT_LDB[]    PROGMEM = "LDB";
static const char TEXT_LDCTL[]  PROGMEM = "LDCTL";
static const char TEXT_LDCTLB[] PROGMEM = "LDCTLB";
static const char TEXT_LDD[]    PROGMEM = "LDD";
static const char TEXT_LDDB[]   PROGMEM = "LDDB";
static const char TEXT_LDDR[]   PROGMEM = "LDDR";
static const char TEXT_LDDRB[]  PROGMEM = "LDDRB";
static const char TEXT_LDI[]    PROGMEM = "LDI";
static const char TEXT_LDIB[]   PROGMEM = "LDIB";
static const char TEXT_LDIR[]   PROGMEM = "LDIR";
static const char TEXT_LDIRB[]  PROGMEM = "LDIRB";
static const char TEXT_LDK[]    PROGMEM = "LDK";
static const char TEXT_LDL[]    PROGMEM = "LDL";
static const char TEXT_LDM[]    PROGMEM = "LDM";
static const char TEXT_LDPS[]   PROGMEM = "LDPS";
static const char TEXT_LDR[]    PROGMEM = "LDR";
static const char TEXT_LDRB[]   PROGMEM = "LDRB";
static const char TEXT_LDRL[]   PROGMEM = "LDRL";
static const char TEXT_MBIT[]   PROGMEM = "MBIT";
static const char TEXT_MREQ[]   PROGMEM = "MREQ";
static const char TEXT_MRES[]   PROGMEM = "MRES";
static const char TEXT_MSET[]   PROGMEM = "MSET";
static const char TEXT_MULT[]   PROGMEM = "MULT";
static const char TEXT_MULTL[]  PROGMEM = "MULTL";
static const char TEXT_NEG[]    PROGMEM = "NEG";
static const char TEXT_NEGB[]   PROGMEM = "NEGB";
static const char TEXT_NOP[]    PROGMEM = "NOP";
static const char TEXT_OR[]     PROGMEM = "OR";
static const char TEXT_ORB[]    PROGMEM = "ORB";
static const char TEXT_OTDR[]   PROGMEM = "OTDR";
static const char TEXT_OTDRB[]  PROGMEM = "OTDRB";
static const char TEXT_OTIR[]   PROGMEM = "OTIR";
static const char TEXT_OTIRB[]  PROGMEM = "OTIRB";
static const char TEXT_OUT[]    PROGMEM = "OUT";
static const char TEXT_OUTB[]   PROGMEM = "OUTB";
static const char TEXT_OUTD[]   PROGMEM = "OUTD";
static const char TEXT_OUTDB[]  PROGMEM = "OUTDB";
static const char TEXT_OUTI[]   PROGMEM = "OUTI";
static const char TEXT_OUTIB[]  PROGMEM = "OUTIB";
static const char TEXT_POP[]    PROGMEM = "POP";
static const char TEXT_POPL[]   PROGMEM = "POPL";
static const char TEXT_PUSH[]   PROGMEM = "PUSH";
static const char TEXT_PUSHL[]  PROGMEM = "PUSHL";
static const char TEXT_RES[]    PROGMEM = "RES";
static const char TEXT_RESB[]   PROGMEM = "RESB";
static const char TEXT_RESFLG[] PROGMEM = "RESFLG";
static const char TEXT_RET[]    PROGMEM = "RET";
static const char TEXT_RL[]     PROGMEM = "RL";
static const char TEXT_RLB[]    PROGMEM = "RLB";
static const char TEXT_RLC[]    PROGMEM = "RLC";
static const char TEXT_RLCB[]   PROGMEM = "RLCB";
static const char TEXT_RLDB[]   PROGMEM = "RLDB";
static const char TEXT_RR[]     PROGMEM = "RR";
static const char TEXT_RRB[]    PROGMEM = "RRB";
static const char TEXT_RRC[]    PROGMEM = "RRC";
static const char TEXT_RRCB[]   PROGMEM = "RRCB";
static const char TEXT_RRDB[]   PROGMEM = "RRDB";
static const char TEXT_SBC[]    PROGMEM = "SBC";
static const char TEXT_SBCB[]   PROGMEM = "SBCB";
static const char TEXT_SC[]     PROGMEM = "SC";
static const char TEXT_SDA[]    PROGMEM = "SDA";
static const char TEXT_SDAB[]   PROGMEM = "SDAB";
static const char TEXT_SDAL[]   PROGMEM = "SDAL";
static const char TEXT_SDL[]    PROGMEM = "SDL";
static const char TEXT_SDLB[]   PROGMEM = "SDLB";
static const char TEXT_SDLL[]   PROGMEM = "SDLL";
static const char TEXT_SET[]    PROGMEM = "SET";
static const char TEXT_SETB[]   PROGMEM = "SETB";
static const char TEXT_SETFLG[] PROGMEM = "SETFLG";
static const char TEXT_SIN[]    PROGMEM = "SIN";
static const char TEXT_SINB[]   PROGMEM = "SINB";
static const char TEXT_SIND[]   PROGMEM = "SIND";
static const char TEXT_SINDB[]  PROGMEM = "SINDB";
static const char TEXT_SINDR[]  PROGMEM = "SINDR";
static const char TEXT_SINDRB[] PROGMEM = "SINDRB";
static const char TEXT_SINI[]   PROGMEM = "SINI";
static const char TEXT_SINIB[]  PROGMEM = "SINIB";
static const char TEXT_SINIR[]  PROGMEM = "SINIR";
static const char TEXT_SINIRB[] PROGMEM = "SINIRB";
static const char TEXT_SLA[]    PROGMEM = "SLA";
static const char TEXT_SLAB[]   PROGMEM = "SLAB";
static const char TEXT_SLAL[]   PROGMEM = "SLAL";
static const char TEXT_SLL[]    PROGMEM = "SLL";
static const char TEXT_SLLB[]   PROGMEM = "SLLB";
static const char TEXT_SLLL[]   PROGMEM = "SLLL";
static const char TEXT_SOTDR[]  PROGMEM = "SOTDR";
static const char TEXT_SOTDRB[] PROGMEM = "SOTDRB";
static const char TEXT_SOTIR[]  PROGMEM = "SOTIR";
static const char TEXT_SOTIRB[] PROGMEM = "SOTIRB";
static const char TEXT_SOUT[]   PROGMEM = "SOUT";
static const char TEXT_SOUTB[]  PROGMEM = "SOUTB";
static const char TEXT_SOUTD[]  PROGMEM = "SOUTD";
static const char TEXT_SOUTDB[] PROGMEM = "SOUTDB";
static const char TEXT_SOUTI[]  PROGMEM = "SOUTI";
static const char TEXT_SOUTIB[] PROGMEM = "SOUTIB";
static const char TEXT_SRA[]    PROGMEM = "SRA";
static const char TEXT_SRAB[]   PROGMEM = "SRAB";
static const char TEXT_SRAL[]   PROGMEM = "SRAL";
static const char TEXT_SRL[]    PROGMEM = "SRL";
static const char TEXT_SRLB[]   PROGMEM = "SRLB";
static const char TEXT_SRLL[]   PROGMEM = "SRLL";
static const char TEXT_SUB[]    PROGMEM = "SUB";
static const char TEXT_SUBB[]   PROGMEM = "SUBB";
static const char TEXT_SUBL[]   PROGMEM = "SUBL";
static const char TEXT_TCC[]    PROGMEM = "TCC";
static const char TEXT_TCCB[]   PROGMEM = "TCCB";
static const char TEXT_TEST[]   PROGMEM = "TEST";
static const char TEXT_TESTB[]  PROGMEM = "TESTB";
static const char TEXT_TESTL[]  PROGMEM = "TESTL";
static const char TEXT_TRDB[]   PROGMEM = "TRDB";
static const char TEXT_TRDRB[]  PROGMEM = "TRDRB";
static const char TEXT_TRIB[]   PROGMEM = "TRIB";
static const char TEXT_TRIRB[]  PROGMEM = "TRIRB";
static const char TEXT_TRTDB[]  PROGMEM = "TRTDB";
static const char TEXT_TRTDRB[] PROGMEM = "TRTDRB";
static const char TEXT_TRTIB[]  PROGMEM = "TRTIB";
static const char TEXT_TRTIRB[] PROGMEM = "TRTIRB";
static const char TEXT_TSET[]   PROGMEM = "TSET";
static const char TEXT_TSETB[]  PROGMEM = "TSETB";
static const char TEXT_XOR[]    PROGMEM = "XOR";
static const char TEXT_XORB[]   PROGMEM = "XORB";
// clang-format on

}  // namespace z8000
}  // namespace libasm

#endif  // __TEXT_Z8000__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
