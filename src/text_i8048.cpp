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
const char TEXT_CPU_LIST[]     PROGMEM = "i8039, i8048, i80C39, i80C48, MSM80C39, MSM80C48";
const char TEXT_CPU_8039[]     PROGMEM = "8039";
const char TEXT_CPU_8048[]     PROGMEM = "8048";
const char TEXT_CPU_80C39[]    PROGMEM = "80C39";
const char TEXT_CPU_80C48[]    PROGMEM = "80C48";
const char TEXT_CPU_MSM80C39[] PROGMEM = "MSM80C39";
const char TEXT_CPU_MSM80C48[] PROGMEM = "MSM80C48";

// const char TEXT_ADD[]  PROGMEM = "ADD";
// const char TEXT_ADDC[] PROGMEM = "ADDC";
// const char TEXT_ANL[]  PROGMEM = "ANL";
const char TEXT_ANLD[] PROGMEM = "ANLD";
// const char TEXT_CALL[] PROGMEM = "CALL";
// const char TEXT_CLR[]  PROGMEM = "CLR";
// const char TEXT_CPL[]  PROGMEM = "CPL";
// const char TEXT_DA[]   PROGMEM = "DA";
// const char TEXT_DEC[]  PROGMEM = "DEC";
// const char TEXT_DIS[]  PROGMEM = "DIS";
// const char TEXT_DJNZ[] PROGMEM = "DJNZ";
const char TEXT_EN[]   PROGMEM = "EN";
const char TEXT_ENT0[] PROGMEM = "ENT0";
// const char TEXT_IN[]   PROGMEM = "IN";
// const char TEXT_INC[]  PROGMEM = "INC";
// const char TEXT_INS[]  PROGMEM = "INS";
const char TEXT_IRL[]  PROGMEM = "IRL";
// const char TEXT_JB[]   PROGMEM = "JB";
// const char TEXT_JC[]   PROGMEM = "JC";
const char TEXT_JF0[]  PROGMEM = "JF0";
const char TEXT_JF1[]  PROGMEM = "JF1";
// const char TEXT_JMP[]  PROGMEM = "JMP";
const char TEXT_JMPP[] PROGMEM = "JMPP";
// const char TEXT_JNC[]  PROGMEM = "JNC";
const char TEXT_JNI[]  PROGMEM = "JNI";
const char TEXT_JNT0[] PROGMEM = "JNT0";
const char TEXT_JNT1[] PROGMEM = "JNT1";
// const char TEXT_JNZ[]  PROGMEM = "JNZ";
const char TEXT_JT0[]  PROGMEM = "JT0";
const char TEXT_JT1[]  PROGMEM = "JT1";
const char TEXT_JTF[]  PROGMEM = "JTF";
// const char TEXT_JZ[]   PROGMEM = "JZ";
// const char TEXT_MOV[]  PROGMEM = "MOV";
// const char TEXT_MOVD[] PROGMEM = "MOVD";
const char TEXT_MOVP[] PROGMEM = "MOVP";
const char TEXT_MOVP3[] PROGMEM = "MOVP3";
// const char TEXT_MOVX[] PROGMEM = "MOVX";
// const char TEXT_NOP[]  PROGMEM = "NOP";
// const char TEXT_ORL[]  PROGMEM = "ORL";
const char TEXT_ORLD[] PROGMEM = "ORLD";
const char TEXT_OUTL[] PROGMEM = "OUTL";
// const char TEXT_RET[]  PROGMEM = "RET";
const char TEXT_RETR[] PROGMEM = "RETR";
// const char TEXT_RL[]   PROGMEM = "RL";
// const char TEXT_RLC[]  PROGMEM = "RLC";
// const char TEXT_RR[]   PROGMEM = "RR";
// const char TEXT_RRC[]  PROGMEM = "RRC";
const char TEXT_SEL[]  PROGMEM = "SEL";
// const char TEXT_STOP[] PROGMEM = "STOP";
const char TEXT_STRT[] PROGMEM = "STRT";
// const char TEXT_SWAP[] PROGMEM = "SWAP";
// const char TEXT_XCH[]  PROGMEM = "XCH";
// const char TEXT_XCHD[] PROGMEM = "XCHD";
// const char TEXT_XRL[]  PROGMEM = "XRL";

// i80C48
// const char TEXT_HALT[] PROGMEM = "HALT";

// MSM80C48
// const char TEXT_FLT[]  PROGMEM = "FLT";
const char TEXT_FLTT[] PROGMEM = "FLTT";
const char TEXT_FRES[] PROGMEM = "FRES";
const char TEXT_HLTS[] PROGMEM = "HLTS";
const char TEXT_MOVP1[] PROGMEM = "MOVP1";

// clang-format off
// const char TEXT_REG_A[]    PROGMEM = "A";
const char TEXT_REG_BUS[]  PROGMEM = "BUS";
// const char TEXT_REG_C[]    PROGMEM = "C";
const char TEXT_REG_CLK[]  PROGMEM = "CLK";
const char TEXT_REG_CNT[]  PROGMEM = "CNT";
// const char TEXT_REG_F0[]   PROGMEM = "F0";
// const char TEXT_REG_F1[]   PROGMEM = "F1";
// const char TEXT_REG_I[]    PROGMEM = "I";
const char TEXT_REG_MB0[]  PROGMEM = "MB0";
const char TEXT_REG_MB1[]  PROGMEM = "MB1";
// const char TEXT_REG_P[]    PROGMEM = "P";
// const char TEXT_REG_P1[]   PROGMEM = "P1";
// const char TEXT_REG_P2[]   PROGMEM = "P2";
const char TEXT_REG_P4[]   PROGMEM = "P4";
const char TEXT_REG_P5[]   PROGMEM = "P5";
const char TEXT_REG_P6[]   PROGMEM = "P6";
const char TEXT_REG_P7[]   PROGMEM = "P7";
// const char TEXT_REG_PSW[]  PROGMEM = "PSW";
// const char TEXT_REG_R0[]   PROGMEM = "R0";
// const char TEXT_REG_R1[]   PROGMEM = "R1";
// const char TEXT_REG_R2[]   PROGMEM = "R2";
// const char TEXT_REG_R3[]   PROGMEM = "R3";
// const char TEXT_REG_R4[]   PROGMEM = "R4";
// const char TEXT_REG_R5[]   PROGMEM = "R5";
// const char TEXT_REG_R6[]   PROGMEM = "R6";
// const char TEXT_REG_R7[]   PROGMEM = "R7";
const char TEXT_REG_RB0[]  PROGMEM = "RB0";
const char TEXT_REG_RB1[]  PROGMEM = "RB1";
// const char TEXT_REG_T[]    PROGMEM = "T";
const char TEXT_REG_TCNT[] PROGMEM = "TCNT";
const char TEXT_REG_TCNTI[] PROGMEM = "TCNTI";
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
