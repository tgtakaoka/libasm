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

constexpr char TEXT_I8048_LIST[]   = "i8039, i8048, i80C39, i80C48, MSM80C39, MSM80C48";
constexpr char TEXT_CPU_8039[]     = "8039";
constexpr char TEXT_CPU_8048[]     = "8048";
constexpr char TEXT_CPU_80C39[]    = "80C39";
constexpr char TEXT_CPU_80C48[]    = "80C48";
constexpr char TEXT_CPU_MSM80C39[] = "MSM80C39";
constexpr char TEXT_CPU_MSM80C48[] = "MSM80C48";

// constexpr char TEXT_ADD[]  = "ADD";
// constexpr char TEXT_ADDC[] = "ADDC";
// constexpr char TEXT_ANL[]  = "ANL";
constexpr char TEXT_ANLD[]  = "ANLD";
// constexpr char TEXT_CALL[] = "CALL";
// constexpr char TEXT_CLR[]  = "CLR";
// constexpr char TEXT_CPL[]  = "CPL";
// constexpr char TEXT_DA[]   = "DA";
// constexpr char TEXT_DEC[]  = "DEC";
// constexpr char TEXT_DIS[]  = "DIS";
// constexpr char TEXT_DJNZ[] = "DJNZ";
constexpr char TEXT_EN[]    = "EN";
constexpr char TEXT_ENT0[]  = "ENT0";
// constexpr char TEXT_IN[]   = "IN";
// constexpr char TEXT_INC[]  = "INC";
// constexpr char TEXT_INS[]  = "INS";
constexpr char TEXT_IRL[]   = "IRL";
// constexpr char TEXT_JB[]   = "JB";
// constexpr char TEXT_JC[]   = "JC";
constexpr char TEXT_JF0[]   = "JF0";
constexpr char TEXT_JF1[]   = "JF1";
// constexpr char TEXT_JMP[]  = "JMP";
constexpr char TEXT_JMPP[]  = "JMPP";
// constexpr char TEXT_JNC[]  = "JNC";
constexpr char TEXT_JNI[]   = "JNI";
constexpr char TEXT_JNT0[]  = "JNT0";
constexpr char TEXT_JNT1[]  = "JNT1";
// constexpr char TEXT_JNZ[]  = "JNZ";
constexpr char TEXT_JT0[]   = "JT0";
constexpr char TEXT_JT1[]   = "JT1";
constexpr char TEXT_JTF[]   = "JTF";
// constexpr char TEXT_JZ[]   = "JZ";
// constexpr char TEXT_MOV[]  = "MOV";
// constexpr char TEXT_MOVD[] = "MOVD";
// constexpr char TEXT_MOVP[] = "MOVP";
constexpr char TEXT_MOVP3[] = "MOVP3";
// constexpr char TEXT_MOVX[] = "MOVX";
// constexpr char TEXT_NOP[]  = "NOP";
// constexpr char TEXT_ORL[]  = "ORL";
constexpr char TEXT_ORLD[]  = "ORLD";
constexpr char TEXT_OUTL[]  = "OUTL";
// constexpr char TEXT_RET[]  = "RET";
constexpr char TEXT_RETR[]  = "RETR";
// constexpr char TEXT_RL[]   = "RL";
// constexpr char TEXT_RLC[]  = "RLC";
// constexpr char TEXT_RR[]   = "RR";
// constexpr char TEXT_RRC[]  = "RRC";
constexpr char TEXT_SEL[]   = "SEL";
// constexpr char TEXT_STOP[] = "STOP";
constexpr char TEXT_STRT[]  = "STRT";
// constexpr char TEXT_SWAP[] = "SWAP";
// constexpr char TEXT_XCH[]  = "XCH";
// constexpr char TEXT_XCHD[] = "XCHD";
// constexpr char TEXT_XRL[]  = "XRL";

// i80C48
// constexpr char TEXT_HALT[] = "HALT";

// MSM80C48
// constexpr char TEXT_FLT[] = "FLT";
constexpr char TEXT_FLTT[]  = "FLTT";
constexpr char TEXT_FRES[]  = "FRES";
constexpr char TEXT_HLTS[]  = "HLTS";
constexpr char TEXT_MOVP1[] = "MOVP1";

// constexpr char TEXT_REG_A[]   = "A";
constexpr char TEXT_REG_BUS[]   = "BUS";
// constexpr char TEXT_REG_C[]   = "C";
constexpr char TEXT_REG_CLK[]   = "CLK";
constexpr char TEXT_REG_CNT[]   = "CNT";
// constexpr char TEXT_REG_F0[]  = "F0";
// constexpr char TEXT_REG_F1[]  = "F1";
// constexpr char TEXT_REG_I[]   = "I";
constexpr char TEXT_REG_MB0[]   = "MB0";
constexpr char TEXT_REG_MB1[]   = "MB1";
// constexpr char TEXT_REG_P[]   = "P";
// constexpr char TEXT_REG_P1[]  = "P1";
// constexpr char TEXT_REG_P2[]  = "P2";
constexpr char TEXT_REG_P4[]    = "P4";
constexpr char TEXT_REG_P5[]    = "P5";
constexpr char TEXT_REG_P6[]    = "P6";
constexpr char TEXT_REG_P7[]    = "P7";
// constexpr char TEXT_REG_PSW[] = "PSW";
// constexpr char TEXT_REG_R0[]  = "R0";
// constexpr char TEXT_REG_R1[]  = "R1";
// constexpr char TEXT_REG_R2[]  = "R2";
// constexpr char TEXT_REG_R3[]  = "R3";
// constexpr char TEXT_REG_R4[]  = "R4";
// constexpr char TEXT_REG_R5[]  = "R5";
// constexpr char TEXT_REG_R6[]  = "R6";
// constexpr char TEXT_REG_R7[]  = "R7";
constexpr char TEXT_REG_RB0[]   = "RB0";
constexpr char TEXT_REG_RB1[]   = "RB1";
// constexpr char TEXT_REG_T[]   = "T";
constexpr char TEXT_REG_TCNT[]  = "TCNT";
constexpr char TEXT_REG_TCNTI[] = "TCNTI";

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
