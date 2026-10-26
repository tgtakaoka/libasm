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

constexpr char TEXT_CP1600_LIST[] = "CP1600";
constexpr char TEXT_CPU_CP1600[]  = "CP1600";

// CP1600
// constexpr char TEXT_ADCR[]  = "ADCR";  // common
constexpr char TEXT_ADDAT[] = "ADD@";
// constexpr char TEXT_ADD[]  = "ADD";
// constexpr char TEXT_ADDI[] = "ADDI";
// constexpr char TEXT_ADDR[] = "ADDR";
constexpr char TEXT_ANDAT[] = "AND@";
// constexpr char TEXT_AND[]  = "AND";
// constexpr char TEXT_ANDI[] = "ANDI";
// constexpr char TEXT_ANDR[] = "ANDR";
// constexpr char TEXT_BC[]   = "BC";
// constexpr char TEXT_B[]    = "B";
// constexpr char TEXT_BEQ[]   = "BEQ";  // common
constexpr char TEXT_BESC[]  = "BESC";
constexpr char TEXT_BEXT[]  = "BEXT";
// constexpr char TEXT_BGE[]  = "BGE";
// constexpr char TEXT_BGT[]  = "BGT";
// constexpr char TEXT_BLE[]  = "BLE";
constexpr char TEXT_BLGE[]  = "BLGE";
constexpr char TEXT_BLLT[]  = "BLLT";
// constexpr char TEXT_BLT[]  = "BLT";
// constexpr char TEXT_BMI[]  = "BMI";
// constexpr char TEXT_BNC[]  = "BNC";
constexpr char TEXT_BNEQ[]  = "BNEQ";
constexpr char TEXT_BNOV[]  = "BNOV";
constexpr char TEXT_BNZE[]  = "BNZE";
constexpr char TEXT_BOV[]   = "BOV";
// constexpr char TEXT_BPL[]  = "BPL";
constexpr char TEXT_BUSC[]  = "BUSC";
constexpr char TEXT_BZE[]   = "BZE";
// constexpr char TEXT_CLRC[] = "CLRC";
constexpr char TEXT_CLRR[]  = "CLRR";
constexpr char TEXT_CMPAT[] = "CMP@";
// constexpr char TEXT_CMP[]  = "CMP";
// constexpr char TEXT_CMPI[] = "CMPI";
// constexpr char TEXT_CMPR[] = "CMPR";
// constexpr char TEXT_COMR[]  = "COMR";  // common
constexpr char TEXT_DECR[]  = "DECR";
// constexpr char TEXT_DIS[]  = "DIS";
constexpr char TEXT_EIS[]   = "EIS";
constexpr char TEXT_GSWD[]  = "GSWD";
// constexpr char TEXT_HLT[]  = "HLT";
constexpr char TEXT_INCR[]  = "INCR";
constexpr char TEXT_J[]     = "J";
constexpr char TEXT_JD[]    = "JD";
// constexpr char TEXT_JE[]   = "JE";
// constexpr char TEXT_JR[]   = "JR";
// constexpr char TEXT_JSR[]  = "JSR";
constexpr char TEXT_JSRD[]  = "JSRD";
constexpr char TEXT_JSRE[]  = "JSRE";
constexpr char TEXT_MOVR[]  = "MOVR";
constexpr char TEXT_MVIAT[] = "MVI@";
// constexpr char TEXT_MVI[]  = "MVI";
constexpr char TEXT_MVII[]  = "MVII";
constexpr char TEXT_MVOAT[] = "MVO@";
constexpr char TEXT_MVO[]   = "MVO";
constexpr char TEXT_MVOI[]  = "MVOI";
// constexpr char TEXT_NEGR[]  = "NEGR";  // common
// constexpr char TEXT_NOP[]  = "NOP";
constexpr char TEXT_NOPP[]  = "NOPP";
constexpr char TEXT_PSHR[]  = "PSHR";
constexpr char TEXT_PULR[]  = "PULR";
// constexpr char TEXT_RLC[]  = "RLC";
// constexpr char TEXT_RRC[]  = "RRC";
constexpr char TEXT_RSWD[]  = "RSWD";
constexpr char TEXT_SARC[]  = "SARC";
// constexpr char TEXT_SAR[]  = "SAR";
constexpr char TEXT_SDBD[]  = "SDBD";
// constexpr char TEXT_SETC[] = "SETC";
// constexpr char TEXT_SIN[]   = "SIN";  // common
constexpr char TEXT_SLLC[]  = "SLLC";
// constexpr char TEXT_SLL[]  = "SLL";
constexpr char TEXT_SLR[]   = "SLR";
constexpr char TEXT_SUBAT[] = "SUB@";
// constexpr char TEXT_SUB[]  = "SUB";
// constexpr char TEXT_SUBI[] = "SUBI";
// constexpr char TEXT_SUBR[] = "SUBR";
// constexpr char TEXT_SWAP[] = "SWAP";
constexpr char TEXT_TCI[]   = "TCI";
constexpr char TEXT_TSTR[]  = "TSTR";
constexpr char TEXT_XORAT[] = "XOR@";
// constexpr char TEXT_XOR[]  = "XOR";
constexpr char TEXT_XORI[]  = "XORI";
constexpr char TEXT_XORR[]  = "XORR";

// constexpr char TEXT_REG_PC[]  = "PC";
// constexpr char TEXT_REG_SP[]  = "SP";

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
