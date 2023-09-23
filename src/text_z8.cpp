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
const char TEXT_Z8_LIST[]    PROGMEM = "Z8, Z86C, Z88";
const char TEXT_CPU_Z8[]     PROGMEM = "Z8";
const char TEXT_CPU_Z86[]    PROGMEM = "Z86";
const char TEXT_CPU_Z86C[]   PROGMEM = "Z86C";
const char TEXT_CPU_Z88[]    PROGMEM = "Z88";;
const char TEXT_CPU_SUPER8[] PROGMEM = "Super8";

// Z8
// const char TEXT_ADC[]  PROGMEM = "ADC";
// const char TEXT_ADD[]  PROGMEM = "ADD";
// const char TEXT_AND[]  PROGMEM = "AND";
// const char TEXT_CALL[] PROGMEM = "CALL";
// const char TEXT_CCF[]  PROGMEM = "CCF";
// const char TEXT_CLR[]  PROGMEM = "CLR";
// const char TEXT_COM[]  PROGMEM = "COM";
// const char TEXT_CP[]   PROGMEM = "CP";
// const char TEXT_DA[]   PROGMEM = "DA";
// const char TEXT_DEC[]  PROGMEM = "DEC";
// const char TEXT_DECW[] PROGMEM = "DECW";
// const char TEXT_DI[]   PROGMEM = "DI";
// const char TEXT_DJNZ[] PROGMEM = "DJNZ";
// const char TEXT_EI[]   PROGMEM = "EI";
// const char TEXT_INC[]  PROGMEM = "INC";
// const char TEXT_INCW[] PROGMEM = "INCW";
// const char TEXT_IRET[] PROGMEM = "IRET";
// const char TEXT_JP[]   PROGMEM = "JP";
// const char TEXT_JR[]   PROGMEM = "JR";
// const char TEXT_LD[]   PROGMEM = "LD";
// const char TEXT_LDC[]  PROGMEM = "LDC";
const char TEXT_LDCI[] PROGMEM = "LDCI";
// const char TEXT_LDE[]  PROGMEM = "LDE";
const char TEXT_LDEI[] PROGMEM = "LDEI";
// const char TEXT_NOP[]  PROGMEM = "NOP";
// const char TEXT_OR[]   PROGMEM = "OR";
// const char TEXT_POP[]  PROGMEM = "POP";
// const char TEXT_PUSH[] PROGMEM = "PUSH";
// const char TEXT_RCF[]  PROGMEM = "RCF";
// const char TEXT_RET[]  PROGMEM = "RET";
// const char TEXT_RL[]   PROGMEM = "RL";
// const char TEXT_RLC[]  PROGMEM = "RLC";
// const char TEXT_RR[]   PROGMEM = "RR";
// const char TEXT_RRC[]  PROGMEM = "RRC";
// const char TEXT_SBC[]  PROGMEM = "SBC";
// const char TEXT_SCF[]  PROGMEM = "SCF";
// const char TEXT_SRA[]  PROGMEM = "SRA";
const char TEXT_SRP[]  PROGMEM = "SRP";
// const char TEXT_SUB[]  PROGMEM = "SUB";
// const char TEXT_SWAP[] PROGMEM = "SWAP";
const char TEXT_TCM[]  PROGMEM = "TCM";
const char TEXT_TM[]   PROGMEM = "TM";
// const char TEXT_XOR[]  PROGMEM = "XOR";

// Z86C
// const char TEXT_HALT[] PROGMEM = "HALT";
// const char TEXT_STOP[] PROGMEM = "STOP";

// SUPER8
// const char TEXT_BAND[]   PROGMEM = "BAND";
const char TEXT_BCP[]    PROGMEM = "BCP";
const char TEXT_BITC[]   PROGMEM = "BITC";
const char TEXT_BITR[]   PROGMEM = "BITR";
const char TEXT_BITS[]   PROGMEM = "BITS";
// const char TEXT_BOR[]    PROGMEM = "BOR";
const char TEXT_BTJRF[]  PROGMEM = "BTJRF";
const char TEXT_BTJRT[]  PROGMEM = "BTJRT";
const char TEXT_BXOR[]   PROGMEM = "BXOR";
const char TEXT_CPIJE[]  PROGMEM = "CPIJE";
const char TEXT_CPIJNE[] PROGMEM = "CPIJNE";
// const char TEXT_DIV[]    PROGMEM = "DIV";
// const char TEXT_ENTER[]  PROGMEM = "ENTER";
// const char TEXT_EXIT[]   PROGMEM = "EXIT";
// const char TEXT_LDB[]    PROGMEM = "LDB";
const char TEXT_LDCD[]   PROGMEM = "LDCD";
const char TEXT_LDCPD[]  PROGMEM = "LDCPD";
const char TEXT_LDCPI[]  PROGMEM = "LDCPI";
const char TEXT_LDED[]   PROGMEM = "LDED";
const char TEXT_LDEPD[]  PROGMEM = "LDEPD";
const char TEXT_LDEPI[]  PROGMEM = "LDEPI";
// const char TEXT_LDW[]    PROGMEM = "LDW";
// const char TEXT_MULT[]   PROGMEM = "MULT";
const char TEXT_NEXT[]   PROGMEM = "NEXT";
const char TEXT_POPUD[]  PROGMEM = "POPUD";
const char TEXT_POPUI[]  PROGMEM = "POPUI";
const char TEXT_PUSHUD[] PROGMEM = "PUSHUD";
const char TEXT_PUSHUI[] PROGMEM = "PUSHUI";
const char TEXT_SB0[]    PROGMEM = "SB0";
const char TEXT_SB1[]    PROGMEM = "SB1";
const char TEXT_SRP0[]   PROGMEM = "SRP0";
const char TEXT_SRP1[]   PROGMEM = "SRP1";
const char TEXT_WFI[]    PROGMEM = "WFI";

// const char TEXT_CC_F[]   PROGMEM = "F";
// const char TEXT_CC_LT[]  PROGMEM = "LT";
// const char TEXT_CC_LE[]  PROGMEM = "LE";
// const char TEXT_CC_ULE[] PROGMEM = "ULE";
// const char TEXT_CC_OV[]  PROGMEM = "OV";
// const char TEXT_CC_MI[]  PROGMEM = "MI";
// const char TEXT_CC_Z[]   PROGMEM = "Z";
// const char TEXT_CC_C[]   PROGMEM = "C";
// const char TEXT_CC_T[]   PROGMEM = "";
// const char TEXT_CC_GE[]  PROGMEM = "GE";
// const char TEXT_CC_GT[]  PROGMEM = "GT";
// const char TEXT_CC_UGT[] PROGMEM = "UGT";
// const char TEXT_CC_NOV[] PROGMEM = "NOV";
// const char TEXT_CC_PL[]  PROGMEM = "PL";
// const char TEXT_CC_NZ[]  PROGMEM = "NZ";
// const char TEXT_CC_NC[]  PROGMEM = "NC";
// const char TEXT_CC_EQ[]  PROGMEM = "EQ";
// const char TEXT_CC_ULT[] PROGMEM = "ULT";
// const char TEXT_CC_NE[]  PROGMEM = "NE";
// const char TEXT_CC_UGE[] PROGMEM = "UGE";
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
