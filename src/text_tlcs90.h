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

#ifndef __TEXT_TLCS90__
#define __TEXT_TLCS90__

#include "config_host.h"

namespace libasm {
namespace tlcs90 {

static const char TEXT_CPU_TLCS90[] PROGMEM = "TLCS90";

// clang-format off
static const char TEXT_ADC[]  PROGMEM = "ADC";
static const char TEXT_ADD[]  PROGMEM = "ADD";
static const char TEXT_AND[]  PROGMEM = "AND";
static const char TEXT_BIT[]  PROGMEM = "BIT";
static const char TEXT_CALL[] PROGMEM = "CALL";
static const char TEXT_CALR[] PROGMEM = "CALR";
static const char TEXT_CCF[]  PROGMEM = "CCF";
static const char TEXT_CP[]   PROGMEM = "CP";
static const char TEXT_CPD[]  PROGMEM = "CPD";
static const char TEXT_CPDR[] PROGMEM = "CPDR";
static const char TEXT_CPI[]  PROGMEM = "CPI";
static const char TEXT_CPIR[] PROGMEM = "CPIR";
static const char TEXT_CPL[]  PROGMEM = "CPL";
static const char TEXT_DAA[]  PROGMEM = "DAA";
static const char TEXT_DEC[]  PROGMEM = "DEC";
static const char TEXT_DECW[] PROGMEM = "DECW";
static const char TEXT_DECX[] PROGMEM = "DECX";
static const char TEXT_DI[]   PROGMEM = "DI";
static const char TEXT_DIV[]  PROGMEM = "DIV";
static const char TEXT_DJNZ[] PROGMEM = "DJNZ";
static const char TEXT_EI[]   PROGMEM = "EI";
static const char TEXT_EX[]   PROGMEM = "EX";
static const char TEXT_EXX[]  PROGMEM = "EXX";
static const char TEXT_HALT[] PROGMEM = "HALT";
static const char TEXT_INC[]  PROGMEM = "INC";
static const char TEXT_INCW[] PROGMEM = "INCW";
static const char TEXT_INCX[] PROGMEM = "INCX";
static const char TEXT_JP[]   PROGMEM = "JP";
static const char TEXT_JR[]   PROGMEM = "JR";
static const char TEXT_JRL[]  PROGMEM = "JRL";
static const char TEXT_LD[]   PROGMEM = "LD";
static const char TEXT_LDA[]  PROGMEM = "LDA";
static const char TEXT_LDAR[] PROGMEM = "LDAR";
static const char TEXT_LDD[]  PROGMEM = "LDD";
static const char TEXT_LDDR[] PROGMEM = "LDDR";
static const char TEXT_LDI[]  PROGMEM = "LDI";
static const char TEXT_LDIR[] PROGMEM = "LDIR";
static const char TEXT_LDW[]  PROGMEM = "LDW";
static const char TEXT_MUL[]  PROGMEM = "MUL";
static const char TEXT_NEG[]  PROGMEM = "NEG";
static const char TEXT_NOP[]  PROGMEM = "NOP";
static const char TEXT_OR[]   PROGMEM = "OR";
static const char TEXT_POP[]  PROGMEM = "POP";
static const char TEXT_PUSH[] PROGMEM = "PUSH";
static const char TEXT_RCF[]  PROGMEM = "RCF";
static const char TEXT_RES[]  PROGMEM = "RES";
static const char TEXT_RET[]  PROGMEM = "RET";
static const char TEXT_RETI[] PROGMEM = "RETI";
static const char TEXT_RL[]   PROGMEM = "RL";
static const char TEXT_RLA[]  PROGMEM = "RLA";
static const char TEXT_RLC[]  PROGMEM = "RLC";
static const char TEXT_RLCA[] PROGMEM = "RLCA";
static const char TEXT_RLD[]  PROGMEM = "RLD";
static const char TEXT_RR[]   PROGMEM = "RR";
static const char TEXT_RRA[]  PROGMEM = "RRA";
static const char TEXT_RRC[]  PROGMEM = "RRC";
static const char TEXT_RRCA[] PROGMEM = "RRCA";
static const char TEXT_RRD[]  PROGMEM = "RRD";
static const char TEXT_SBC[]  PROGMEM = "SBC";
static const char TEXT_SCF[]  PROGMEM = "SCF";
static const char TEXT_SET[]  PROGMEM = "SET";
static const char TEXT_SLA[]  PROGMEM = "SLA";
static const char TEXT_SLAA[] PROGMEM = "SLAA";
static const char TEXT_SLL[]  PROGMEM = "SLL";
static const char TEXT_SLLA[] PROGMEM = "SLLA";
static const char TEXT_SRA[]  PROGMEM = "SRA";
static const char TEXT_SRAA[] PROGMEM = "SRAA";
static const char TEXT_SRL[]  PROGMEM = "SRL";
static const char TEXT_SRLA[] PROGMEM = "SRLA";
static const char TEXT_SUB[]  PROGMEM = "SUB";
static const char TEXT_SWI[]  PROGMEM = "SWI";
static const char TEXT_TSET[] PROGMEM = "TSET";
static const char TEXT_XOR[]  PROGMEM = "XOR";
// clang-format on

}  // namespace tlcs90
}  // namespace libasm

#endif  // __TEXT_TLCS90__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
