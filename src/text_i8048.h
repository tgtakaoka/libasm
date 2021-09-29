/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __TEXT_I8048__
#define __TEXT_I8048__

#include "config_host.h"

namespace libasm {
namespace i8048 {

// clang-format off
static const char TEXT_CPU_LIST[] PROGMEM = "i8039, i8048, i80C39, i80C48, MSM80C39, MSM80C48";
static const char TEXT_CPU_8039[] PROGMEM = "8039";
static const char TEXT_CPU_8048[] PROGMEM = "8048";
static const char TEXT_CPU_MSM80C39[] PROGMEM = "MSM80C39";
static constexpr const char *TEXT_CPU_MSM = TEXT_CPU_MSM80C39;
static constexpr const char *TEXT_CPU_MSM80C48 = (TEXT_CPU_LIST + 40);
static constexpr const char *TEXT_CPU_80C39 = (TEXT_CPU_MSM80C39 + 3);
static constexpr const char *TEXT_CPU_80C48 = (TEXT_CPU_MSM80C48 + 3);

static const char TEXT_ADD[]  PROGMEM = "ADD";
static const char TEXT_ADDC[] PROGMEM = "ADDC";
static const char TEXT_ANL[]  PROGMEM = "ANL";
static const char TEXT_ANLD[] PROGMEM = "ANLD";
static const char TEXT_CALL[] PROGMEM = "CALL";
static const char TEXT_CLR[]  PROGMEM = "CLR";
static const char TEXT_CPL[]  PROGMEM = "CPL";
static const char TEXT_DA[]   PROGMEM = "DA";
static const char TEXT_DEC[]  PROGMEM = "DEC";
static const char TEXT_DIS[]  PROGMEM = "DIS";
static const char TEXT_DJNZ[] PROGMEM = "DJNZ";
static const char TEXT_EN[]   PROGMEM = "EN";
static const char TEXT_ENT0[] PROGMEM = "ENT0";
static const char TEXT_IN[]   PROGMEM = "IN";
static const char TEXT_INC[]  PROGMEM = "INC";
static const char TEXT_INS[]  PROGMEM = "INS";
static const char TEXT_IRL[]  PROGMEM = "IRL";
static const char TEXT_JB[]   PROGMEM = "JB";
static const char TEXT_JC[]   PROGMEM = "JC";
static const char TEXT_JF0[]  PROGMEM = "JF0";
static const char TEXT_JF1[]  PROGMEM = "JF1";
static const char TEXT_JMP[]  PROGMEM = "JMP";
static const char TEXT_JMPP[] PROGMEM = "JMPP";
static const char TEXT_JNC[]  PROGMEM = "JNC";
static const char TEXT_JNI[]  PROGMEM = "JNI";
static const char TEXT_JNT0[] PROGMEM = "JNT0";
static const char TEXT_JNT1[] PROGMEM = "JNT1";
static const char TEXT_JNZ[]  PROGMEM = "JNZ";
static const char TEXT_JT0[]  PROGMEM = "JT0";
static const char TEXT_JT1[]  PROGMEM = "JT1";
static const char TEXT_JTF[]  PROGMEM = "JTF";
static const char TEXT_JZ[]   PROGMEM = "JZ";
static const char TEXT_MOV[]  PROGMEM = "MOV";
static const char TEXT_MOVD[] PROGMEM = "MOVD";
static const char TEXT_MOVP[] PROGMEM = "MOVP";
static const char TEXT_MOVP3[] PROGMEM = "MOVP3";
static const char TEXT_MOVX[] PROGMEM = "MOVX";
static const char TEXT_NOP[]  PROGMEM = "NOP";
static const char TEXT_ORL[]  PROGMEM = "ORL";
static const char TEXT_ORLD[] PROGMEM = "ORLD";
static const char TEXT_OUTL[] PROGMEM = "OUTL";
static const char TEXT_RET[]  PROGMEM = "RET";
static const char TEXT_RETR[] PROGMEM = "RETR";
static const char TEXT_RL[]   PROGMEM = "RL";
static const char TEXT_RLC[]  PROGMEM = "RLC";
static const char TEXT_RR[]   PROGMEM = "RR";
static const char TEXT_RRC[]  PROGMEM = "RRC";
static const char TEXT_SEL[]  PROGMEM = "SEL";
static const char TEXT_STOP[] PROGMEM = "STOP";
static const char TEXT_STRT[] PROGMEM = "STRT";
static const char TEXT_SWAP[] PROGMEM = "SWAP";
static const char TEXT_XCH[]  PROGMEM = "XCH";
static const char TEXT_XCHD[] PROGMEM = "XCHD";
static const char TEXT_XRL[]  PROGMEM = "XRL";

// i80C48
static const char TEXT_HALT[] PROGMEM = "HALT";

// MSM80C48
static const char TEXT_FLT[]  PROGMEM = "FLT";
static const char TEXT_FLTT[] PROGMEM = "FLTT";
static const char TEXT_FRES[] PROGMEM = "FRES";
static const char TEXT_HLTS[] PROGMEM = "HLTS";
static const char TEXT_MOVP1[] PROGMEM = "MOVP1";

// clang-format on

}  // namespace i8048
}  // namespace libasm

#endif  // __TEXT_I8048__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
