/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "text_cp1600.h"

namespace libasm {
namespace text {
namespace cp1600 {
// clang-format off

constexpr char TEXT_CP1600_LIST[] PROGMEM = "CP1600";
constexpr char TEXT_CPU_CP1600[]  PROGMEM = "CP1600";

// CP1600
constexpr char TEXT_ADCR[]  PROGMEM = "ADCR";
constexpr char TEXT_ADDAT[] PROGMEM = "ADD@";
// constexpr char TEXT_ADD[]  PROGMEM = "ADD";
// constexpr char TEXT_ADDI[] PROGMEM = "ADDI";
// constexpr char TEXT_ADDR[] PROGMEM = "ADDR";
constexpr char TEXT_ANDAT[] PROGMEM = "AND@";
// constexpr char TEXT_AND[]  PROGMEM = "AND";
// constexpr char TEXT_ANDI[] PROGMEM = "ANDI";
// constexpr char TEXT_ANDR[] PROGMEM = "ANDR";
// constexpr char TEXT_BC[]   PROGMEM = "BC";
// constexpr char TEXT_B[]    PROGMEM = "B";
// constexpr char TEXT_BEQ[]   PROGMEM = "BEQ";  // common
constexpr char TEXT_BESC[]  PROGMEM = "BESC";
constexpr char TEXT_BEXT[]  PROGMEM = "BEXT";
// constexpr char TEXT_BGE[]  PROGMEM = "BGE";
// constexpr char TEXT_BGT[]  PROGMEM = "BGT";
// constexpr char TEXT_BLE[]  PROGMEM = "BLE";
constexpr char TEXT_BLGE[]  PROGMEM = "BLGE";
constexpr char TEXT_BLLT[]  PROGMEM = "BLLT";
// constexpr char TEXT_BLT[]  PROGMEM = "BLT";
// constexpr char TEXT_BMI[]  PROGMEM = "BMI";
// constexpr char TEXT_BNC[]  PROGMEM = "BNC";
constexpr char TEXT_BNEQ[]  PROGMEM = "BNEQ";
constexpr char TEXT_BNOV[]  PROGMEM = "BNOV";
constexpr char TEXT_BNZE[]  PROGMEM = "BNZE";
constexpr char TEXT_BOV[]   PROGMEM = "BOV";
// constexpr char TEXT_BPL[]  PROGMEM = "BPL";
constexpr char TEXT_BUSC[]  PROGMEM = "BUSC";
constexpr char TEXT_BZE[]   PROGMEM = "BZE";
// constexpr char TEXT_CLRC[] PROGMEM = "CLRC";
constexpr char TEXT_CLRR[]  PROGMEM = "CLRR";
constexpr char TEXT_CMPAT[] PROGMEM = "CMP@";
// constexpr char TEXT_CMP[]  PROGMEM = "CMP";
// constexpr char TEXT_CMPI[] PROGMEM = "CMPI";
// constexpr char TEXT_CMPR[] PROGMEM = "CMPR";
constexpr char TEXT_COMR[]  PROGMEM = "COMR";
constexpr char TEXT_DECR[]  PROGMEM = "DECR";
// constexpr char TEXT_DIS[]  PROGMEM = "DIS";
constexpr char TEXT_EIS[]   PROGMEM = "EIS";
constexpr char TEXT_GSWD[]  PROGMEM = "GSWD";
// constexpr char TEXT_HLT[]  PROGMEM = "HLT";
constexpr char TEXT_INCR[]  PROGMEM = "INCR";
constexpr char TEXT_J[]     PROGMEM = "J";
constexpr char TEXT_JD[]    PROGMEM = "JD";
// constexpr char TEXT_JE[]   PROGMEM = "JE";
// constexpr char TEXT_JR[]   PROGMEM = "JR";
// constexpr char TEXT_JSR[]  PROGMEM = "JSR";
constexpr char TEXT_JSRD[]  PROGMEM = "JSRD";
constexpr char TEXT_JSRE[]  PROGMEM = "JSRE";
constexpr char TEXT_MOVR[]  PROGMEM = "MOVR";
constexpr char TEXT_MVIAT[] PROGMEM = "MVI@";
// constexpr char TEXT_MVI[]  PROGMEM = "MVI";
constexpr char TEXT_MVII[]  PROGMEM = "MVII";
constexpr char TEXT_MVOAT[] PROGMEM = "MVO@";
constexpr char TEXT_MVO[]   PROGMEM = "MVO";
constexpr char TEXT_MVOI[]  PROGMEM = "MVOI";
constexpr char TEXT_NEGR[]  PROGMEM = "NEGR";
// constexpr char TEXT_NOP[]  PROGMEM = "NOP";
constexpr char TEXT_NOPP[]  PROGMEM = "NOPP";
constexpr char TEXT_PSHR[]  PROGMEM = "PSHR";
constexpr char TEXT_PULR[]  PROGMEM = "PULR";
// constexpr char TEXT_RLC[]  PROGMEM = "RLC";
// constexpr char TEXT_RRC[]  PROGMEM = "RRC";
constexpr char TEXT_RSWD[]  PROGMEM = "RSWD";
constexpr char TEXT_SARC[]  PROGMEM = "SARC";
// constexpr char TEXT_SAR[]  PROGMEM = "SAR";
constexpr char TEXT_SDBD[]  PROGMEM = "SDBD";
// constexpr char TEXT_SETC[] PROGMEM = "SETC";
constexpr char TEXT_SIN[]   PROGMEM = "SIN";
constexpr char TEXT_SLLC[]  PROGMEM = "SLLC";
// constexpr char TEXT_SLL[]  PROGMEM = "SLL";
constexpr char TEXT_SLR[]   PROGMEM = "SLR";
constexpr char TEXT_SUBAT[] PROGMEM = "SUB@";
// constexpr char TEXT_SUB[]  PROGMEM = "SUB";
// constexpr char TEXT_SUBI[] PROGMEM = "SUBI";
// constexpr char TEXT_SUBR[] PROGMEM = "SUBR";
// constexpr char TEXT_SWAP[] PROGMEM = "SWAP";
constexpr char TEXT_TCI[]   PROGMEM = "TCI";
constexpr char TEXT_TSTR[]  PROGMEM = "TSTR";
constexpr char TEXT_XORAT[] PROGMEM = "XOR@";
// constexpr char TEXT_XOR[]  PROGMEM = "XOR";
constexpr char TEXT_XORI[]  PROGMEM = "XORI";
constexpr char TEXT_XORR[]  PROGMEM = "XORR";

// constexpr char TEXT_REG_PC[]  PROGMEM = "PC";
// constexpr char TEXT_REG_SP[]  PROGMEM = "SP";

// clang-format on
}  // namespace cp1600
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
