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
constexpr char TEXT_Z8_LIST[]    PROGMEM = "Z8, Z86C, Z88";
constexpr char TEXT_CPU_Z8[]     PROGMEM = "Z8";
constexpr char TEXT_CPU_Z86[]    PROGMEM = "Z86";
constexpr char TEXT_CPU_Z86C[]   PROGMEM = "Z86C";
constexpr char TEXT_CPU_Z88[]    PROGMEM = "Z88";;
constexpr char TEXT_CPU_SUPER8[] PROGMEM = "Super8";

// Z8
// constexpr char TEXT_ADC[]  PROGMEM = "ADC";
// constexpr char TEXT_ADD[]  PROGMEM = "ADD";
// constexpr char TEXT_AND[]  PROGMEM = "AND";
// constexpr char TEXT_CALL[] PROGMEM = "CALL";
// constexpr char TEXT_CCF[]  PROGMEM = "CCF";
// constexpr char TEXT_CLR[]  PROGMEM = "CLR";
// constexpr char TEXT_COM[]  PROGMEM = "COM";
// constexpr char TEXT_CP[]   PROGMEM = "CP";
// constexpr char TEXT_DA[]   PROGMEM = "DA";
// constexpr char TEXT_DEC[]  PROGMEM = "DEC";
// constexpr char TEXT_DECW[] PROGMEM = "DECW";
// constexpr char TEXT_DI[]   PROGMEM = "DI";
// constexpr char TEXT_DJNZ[] PROGMEM = "DJNZ";
// constexpr char TEXT_EI[]   PROGMEM = "EI";
// constexpr char TEXT_INC[]  PROGMEM = "INC";
// constexpr char TEXT_INCW[] PROGMEM = "INCW";
// constexpr char TEXT_IRET[] PROGMEM = "IRET";
// constexpr char TEXT_JP[]   PROGMEM = "JP";
// constexpr char TEXT_JR[]   PROGMEM = "JR";
// constexpr char TEXT_LD[]   PROGMEM = "LD";
// constexpr char TEXT_LDC[]  PROGMEM = "LDC";
constexpr char TEXT_LDCI[] PROGMEM = "LDCI";
// constexpr char TEXT_LDE[]  PROGMEM = "LDE";
constexpr char TEXT_LDEI[] PROGMEM = "LDEI";
// constexpr char TEXT_NOP[]  PROGMEM = "NOP";
// constexpr char TEXT_OR[]   PROGMEM = "OR";
// constexpr char TEXT_POP[]  PROGMEM = "POP";
// constexpr char TEXT_PUSH[] PROGMEM = "PUSH";
// constexpr char TEXT_RCF[]  PROGMEM = "RCF";
// constexpr char TEXT_RET[]  PROGMEM = "RET";
// constexpr char TEXT_RL[]   PROGMEM = "RL";
// constexpr char TEXT_RLC[]  PROGMEM = "RLC";
// constexpr char TEXT_RR[]   PROGMEM = "RR";
// constexpr char TEXT_RRC[]  PROGMEM = "RRC";
// constexpr char TEXT_SBC[]  PROGMEM = "SBC";
// constexpr char TEXT_SCF[]  PROGMEM = "SCF";
// constexpr char TEXT_SRA[]  PROGMEM = "SRA";
constexpr char TEXT_SRP[]  PROGMEM = "SRP";
// constexpr char TEXT_SUB[]  PROGMEM = "SUB";
// constexpr char TEXT_SWAP[] PROGMEM = "SWAP";
constexpr char TEXT_TCM[]  PROGMEM = "TCM";
constexpr char TEXT_TM[]   PROGMEM = "TM";
// constexpr char TEXT_XOR[]  PROGMEM = "XOR";

// Z86C
// constexpr char TEXT_HALT[] PROGMEM = "HALT";
// constexpr char TEXT_STOP[] PROGMEM = "STOP";

// SUPER8
// constexpr char TEXT_BAND[]   PROGMEM = "BAND";
constexpr char TEXT_BCP[]    PROGMEM = "BCP";
constexpr char TEXT_BITC[]   PROGMEM = "BITC";
constexpr char TEXT_BITR[]   PROGMEM = "BITR";
constexpr char TEXT_BITS[]   PROGMEM = "BITS";
// constexpr char TEXT_BOR[]    PROGMEM = "BOR";
constexpr char TEXT_BTJRF[]  PROGMEM = "BTJRF";
constexpr char TEXT_BTJRT[]  PROGMEM = "BTJRT";
constexpr char TEXT_BXOR[]   PROGMEM = "BXOR";
constexpr char TEXT_CPIJE[]  PROGMEM = "CPIJE";
constexpr char TEXT_CPIJNE[] PROGMEM = "CPIJNE";
// constexpr char TEXT_DIV[]    PROGMEM = "DIV";
// constexpr char TEXT_ENTER[]  PROGMEM = "ENTER";
// constexpr char TEXT_EXIT[]   PROGMEM = "EXIT";
// constexpr char TEXT_LDB[]    PROGMEM = "LDB";
constexpr char TEXT_LDCD[]   PROGMEM = "LDCD";
constexpr char TEXT_LDCPD[]  PROGMEM = "LDCPD";
constexpr char TEXT_LDCPI[]  PROGMEM = "LDCPI";
constexpr char TEXT_LDED[]   PROGMEM = "LDED";
constexpr char TEXT_LDEPD[]  PROGMEM = "LDEPD";
constexpr char TEXT_LDEPI[]  PROGMEM = "LDEPI";
// constexpr char TEXT_LDW[]    PROGMEM = "LDW";
// constexpr char TEXT_MULT[]   PROGMEM = "MULT";
constexpr char TEXT_NEXT[]   PROGMEM = "NEXT";
constexpr char TEXT_POPUD[]  PROGMEM = "POPUD";
constexpr char TEXT_POPUI[]  PROGMEM = "POPUI";
constexpr char TEXT_PUSHUD[] PROGMEM = "PUSHUD";
constexpr char TEXT_PUSHUI[] PROGMEM = "PUSHUI";
constexpr char TEXT_SB0[]    PROGMEM = "SB0";
constexpr char TEXT_SB1[]    PROGMEM = "SB1";
constexpr char TEXT_SRP0[]   PROGMEM = "SRP0";
constexpr char TEXT_SRP1[]   PROGMEM = "SRP1";
constexpr char TEXT_WFI[]    PROGMEM = "WFI";

// constexpr char TEXT_CC_F[]   PROGMEM = "F";
// constexpr char TEXT_CC_LT[]  PROGMEM = "LT";
// constexpr char TEXT_CC_LE[]  PROGMEM = "LE";
// constexpr char TEXT_CC_ULE[] PROGMEM = "ULE";
// constexpr char TEXT_CC_OV[]  PROGMEM = "OV";
// constexpr char TEXT_CC_MI[]  PROGMEM = "MI";
// constexpr char TEXT_CC_Z[]   PROGMEM = "Z";
// constexpr char TEXT_CC_C[]   PROGMEM = "C";
// constexpr char TEXT_CC_T[]   PROGMEM = "";
// constexpr char TEXT_CC_GE[]  PROGMEM = "GE";
// constexpr char TEXT_CC_GT[]  PROGMEM = "GT";
// constexpr char TEXT_CC_UGT[] PROGMEM = "UGT";
// constexpr char TEXT_CC_NOV[] PROGMEM = "NOV";
// constexpr char TEXT_CC_PL[]  PROGMEM = "PL";
// constexpr char TEXT_CC_NZ[]  PROGMEM = "NZ";
// constexpr char TEXT_CC_NC[]  PROGMEM = "NC";
// constexpr char TEXT_CC_EQ[]  PROGMEM = "EQ";
// constexpr char TEXT_CC_ULT[] PROGMEM = "ULT";
// constexpr char TEXT_CC_NE[]  PROGMEM = "NE";
// constexpr char TEXT_CC_UGE[] PROGMEM = "UGE";
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
