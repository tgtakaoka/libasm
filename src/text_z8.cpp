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

#include "text_z8.h"

namespace libasm {
namespace text {
namespace z8 {
// clang-format off

constexpr char TEXT_Z8_LIST[]    = "Z8, Z86C, Z88";
constexpr char TEXT_CPU_SUPER8[] = "Super8";
constexpr char TEXT_CPU_Z8[]     = "Z8";
constexpr char TEXT_CPU_Z86[]    = "Z86";
constexpr char TEXT_CPU_Z86C[]   = "Z86C";
constexpr char TEXT_CPU_Z88[]    = "Z88";;

// Z8
// constexpr char TEXT_ADC[]  = "ADC";
// constexpr char TEXT_ADD[]  = "ADD";
// constexpr char TEXT_AND[]  = "AND";
// constexpr char TEXT_CALL[] = "CALL";
// constexpr char TEXT_CCF[]  = "CCF";
// constexpr char TEXT_CLR[]  = "CLR";
// constexpr char TEXT_COM[]  = "COM";
// constexpr char TEXT_CP[]   = "CP";
// constexpr char TEXT_DA[]   = "DA";
// constexpr char TEXT_DEC[]  = "DEC";
// constexpr char TEXT_DECW[] = "DECW";
// constexpr char TEXT_DI[]   = "DI";
// constexpr char TEXT_DJNZ[] = "DJNZ";
// constexpr char TEXT_EI[]   = "EI";
// constexpr char TEXT_INC[]  = "INC";
// constexpr char TEXT_INCW[] = "INCW";
// constexpr char TEXT_IRET[] = "IRET";
// constexpr char TEXT_JP[]   = "JP";
// constexpr char TEXT_JR[]   = "JR";
// constexpr char TEXT_LD[]   = "LD";
// constexpr char TEXT_LDC[]  = "LDC";
constexpr char TEXT_LDCI[] = "LDCI";
// constexpr char TEXT_LDE[]  = "LDE";
constexpr char TEXT_LDEI[] = "LDEI";
// constexpr char TEXT_NOP[]  = "NOP";
// constexpr char TEXT_OR[]   = "OR";
// constexpr char TEXT_POP[]  = "POP";
// constexpr char TEXT_PUSH[] = "PUSH";
// constexpr char TEXT_RCF[]  = "RCF";
// constexpr char TEXT_RET[]  = "RET";
// constexpr char TEXT_RL[]   = "RL";
// constexpr char TEXT_RLC[]  = "RLC";
// constexpr char TEXT_RR[]   = "RR";
// constexpr char TEXT_RRC[]  = "RRC";
// constexpr char TEXT_SBC[]  = "SBC";
// constexpr char TEXT_SCF[]  = "SCF";
// constexpr char TEXT_SRA[]  = "SRA";
constexpr char TEXT_SRP[]  = "SRP";
// constexpr char TEXT_SUB[]  = "SUB";
// constexpr char TEXT_SWAP[] = "SWAP";
constexpr char TEXT_TCM[]  = "TCM";
constexpr char TEXT_TM[]   = "TM";
// constexpr char TEXT_XOR[]  = "XOR";

// Z86C
// constexpr char TEXT_HALT[] = "HALT";
// constexpr char TEXT_STOP[] = "STOP";

// SUPER8
// constexpr char TEXT_BAND[]  = "BAND";
constexpr char TEXT_BCP[]    = "BCP";
constexpr char TEXT_BITC[]   = "BITC";
constexpr char TEXT_BITR[]   = "BITR";
constexpr char TEXT_BITS[]   = "BITS";
// constexpr char TEXT_BOR[]   = "BOR";
constexpr char TEXT_BTJRF[]  = "BTJRF";
constexpr char TEXT_BTJRT[]  = "BTJRT";
// constexpr char TEXT_BXOR[]   = "BXOR";
constexpr char TEXT_CPIJE[]  = "CPIJE";
constexpr char TEXT_CPIJNE[] = "CPIJNE";
// constexpr char TEXT_DIV[]   = "DIV";
// constexpr char TEXT_ENTER[] = "ENTER";
// constexpr char TEXT_EXIT[]  = "EXIT";
// constexpr char TEXT_LDB[]   = "LDB";
constexpr char TEXT_LDCD[]   = "LDCD";
constexpr char TEXT_LDCPD[]  = "LDCPD";
constexpr char TEXT_LDCPI[]  = "LDCPI";
// constexpr char TEXT_LDED[]  = "LDED";
constexpr char TEXT_LDEPD[]  = "LDEPD";
constexpr char TEXT_LDEPI[]  = "LDEPI";
// constexpr char TEXT_LDW[]   = "LDW";
// constexpr char TEXT_MULT[]  = "MULT";
constexpr char TEXT_NEXT[]   = "NEXT";
constexpr char TEXT_POPUD[]  = "POPUD";
constexpr char TEXT_POPUI[]  = "POPUI";
constexpr char TEXT_PUSHUD[] = "PUSHUD";
constexpr char TEXT_PUSHUI[] = "PUSHUI";
constexpr char TEXT_SB0[]    = "SB0";
constexpr char TEXT_SB1[]    = "SB1";
constexpr char TEXT_SRP0[]   = "SRP0";
constexpr char TEXT_SRP1[]   = "SRP1";
constexpr char TEXT_WFI[]    = "WFI";

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

// clang-format on
}  // namespace z8
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
