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

#include "text_i8048.h"

namespace libasm {
namespace text {
namespace i8048 {
// clang-format off

constexpr char TEXT_I8048_LIST[]   PROGMEM = "i8039, i8048, i80C39, i80C48, MSM80C39, MSM80C48";
constexpr char TEXT_CPU_8039[]     PROGMEM = "8039";
constexpr char TEXT_CPU_8048[]     PROGMEM = "8048";
constexpr char TEXT_CPU_80C39[]    PROGMEM = "80C39";
constexpr char TEXT_CPU_80C48[]    PROGMEM = "80C48";
constexpr char TEXT_CPU_MSM80C39[] PROGMEM = "MSM80C39";
constexpr char TEXT_CPU_MSM80C48[] PROGMEM = "MSM80C48";

// constexpr char TEXT_ADD[]  PROGMEM = "ADD";
// constexpr char TEXT_ADDC[] PROGMEM = "ADDC";
// constexpr char TEXT_ANL[]  PROGMEM = "ANL";
constexpr char TEXT_ANLD[]  PROGMEM = "ANLD";
// constexpr char TEXT_CALL[] PROGMEM = "CALL";
// constexpr char TEXT_CLR[]  PROGMEM = "CLR";
// constexpr char TEXT_CPL[]  PROGMEM = "CPL";
// constexpr char TEXT_DA[]   PROGMEM = "DA";
// constexpr char TEXT_DEC[]  PROGMEM = "DEC";
// constexpr char TEXT_DIS[]  PROGMEM = "DIS";
// constexpr char TEXT_DJNZ[] PROGMEM = "DJNZ";
constexpr char TEXT_EN[]    PROGMEM = "EN";
constexpr char TEXT_ENT0[]  PROGMEM = "ENT0";
// constexpr char TEXT_IN[]   PROGMEM = "IN";
// constexpr char TEXT_INC[]  PROGMEM = "INC";
// constexpr char TEXT_INS[]  PROGMEM = "INS";
constexpr char TEXT_IRL[]   PROGMEM = "IRL";
// constexpr char TEXT_JB[]   PROGMEM = "JB";
// constexpr char TEXT_JC[]   PROGMEM = "JC";
constexpr char TEXT_JF0[]   PROGMEM = "JF0";
constexpr char TEXT_JF1[]   PROGMEM = "JF1";
// constexpr char TEXT_JMP[]  PROGMEM = "JMP";
constexpr char TEXT_JMPP[]  PROGMEM = "JMPP";
// constexpr char TEXT_JNC[]  PROGMEM = "JNC";
constexpr char TEXT_JNI[]   PROGMEM = "JNI";
constexpr char TEXT_JNT0[]  PROGMEM = "JNT0";
constexpr char TEXT_JNT1[]  PROGMEM = "JNT1";
// constexpr char TEXT_JNZ[]  PROGMEM = "JNZ";
constexpr char TEXT_JT0[]   PROGMEM = "JT0";
constexpr char TEXT_JT1[]   PROGMEM = "JT1";
constexpr char TEXT_JTF[]   PROGMEM = "JTF";
// constexpr char TEXT_JZ[]   PROGMEM = "JZ";
// constexpr char TEXT_MOV[]  PROGMEM = "MOV";
// constexpr char TEXT_MOVD[] PROGMEM = "MOVD";
// constexpr char TEXT_MOVP[] PROGMEM = "MOVP";
constexpr char TEXT_MOVP3[] PROGMEM = "MOVP3";
// constexpr char TEXT_MOVX[] PROGMEM = "MOVX";
// constexpr char TEXT_NOP[]  PROGMEM = "NOP";
// constexpr char TEXT_ORL[]  PROGMEM = "ORL";
constexpr char TEXT_ORLD[]  PROGMEM = "ORLD";
constexpr char TEXT_OUTL[]  PROGMEM = "OUTL";
// constexpr char TEXT_RET[]  PROGMEM = "RET";
constexpr char TEXT_RETR[]  PROGMEM = "RETR";
// constexpr char TEXT_RL[]   PROGMEM = "RL";
// constexpr char TEXT_RLC[]  PROGMEM = "RLC";
// constexpr char TEXT_RR[]   PROGMEM = "RR";
// constexpr char TEXT_RRC[]  PROGMEM = "RRC";
constexpr char TEXT_SEL[]   PROGMEM = "SEL";
// constexpr char TEXT_STOP[] PROGMEM = "STOP";
constexpr char TEXT_STRT[]  PROGMEM = "STRT";
// constexpr char TEXT_SWAP[] PROGMEM = "SWAP";
// constexpr char TEXT_XCH[]  PROGMEM = "XCH";
// constexpr char TEXT_XCHD[] PROGMEM = "XCHD";
// constexpr char TEXT_XRL[]  PROGMEM = "XRL";

// i80C48
// constexpr char TEXT_HALT[] PROGMEM = "HALT";

// MSM80C48
// constexpr char TEXT_FLT[] PROGMEM = "FLT";
constexpr char TEXT_FLTT[]  PROGMEM = "FLTT";
constexpr char TEXT_FRES[]  PROGMEM = "FRES";
constexpr char TEXT_HLTS[]  PROGMEM = "HLTS";
constexpr char TEXT_MOVP1[] PROGMEM = "MOVP1";

// constexpr char TEXT_REG_A[]   PROGMEM = "A";
constexpr char TEXT_REG_BUS[]   PROGMEM = "BUS";
// constexpr char TEXT_REG_C[]   PROGMEM = "C";
constexpr char TEXT_REG_CLK[]   PROGMEM = "CLK";
constexpr char TEXT_REG_CNT[]   PROGMEM = "CNT";
// constexpr char TEXT_REG_F0[]  PROGMEM = "F0";
// constexpr char TEXT_REG_F1[]  PROGMEM = "F1";
// constexpr char TEXT_REG_I[]   PROGMEM = "I";
constexpr char TEXT_REG_MB0[]   PROGMEM = "MB0";
constexpr char TEXT_REG_MB1[]   PROGMEM = "MB1";
// constexpr char TEXT_REG_P[]   PROGMEM = "P";
// constexpr char TEXT_REG_P1[]  PROGMEM = "P1";
// constexpr char TEXT_REG_P2[]  PROGMEM = "P2";
constexpr char TEXT_REG_P4[]    PROGMEM = "P4";
constexpr char TEXT_REG_P5[]    PROGMEM = "P5";
constexpr char TEXT_REG_P6[]    PROGMEM = "P6";
constexpr char TEXT_REG_P7[]    PROGMEM = "P7";
// constexpr char TEXT_REG_PSW[] PROGMEM = "PSW";
// constexpr char TEXT_REG_R0[]  PROGMEM = "R0";
// constexpr char TEXT_REG_R1[]  PROGMEM = "R1";
// constexpr char TEXT_REG_R2[]  PROGMEM = "R2";
// constexpr char TEXT_REG_R3[]  PROGMEM = "R3";
// constexpr char TEXT_REG_R4[]  PROGMEM = "R4";
// constexpr char TEXT_REG_R5[]  PROGMEM = "R5";
// constexpr char TEXT_REG_R6[]  PROGMEM = "R6";
// constexpr char TEXT_REG_R7[]  PROGMEM = "R7";
constexpr char TEXT_REG_RB0[]   PROGMEM = "RB0";
constexpr char TEXT_REG_RB1[]   PROGMEM = "RB1";
// constexpr char TEXT_REG_T[]   PROGMEM = "T";
constexpr char TEXT_REG_TCNT[]  PROGMEM = "TCNT";
constexpr char TEXT_REG_TCNTI[] PROGMEM = "TCNTI";

// clang-format on
}  // namespace i8048
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
