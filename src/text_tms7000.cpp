/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "text_tms7000.h"

namespace libasm {
namespace text {
namespace tms7000 {

// clang-format off
const char TEXT_CPU_TMS7000[]  PROGMEM = "TMS7000";
const char TEXT_CPU_TMS70C00[] PROGMEM = "TMS70C00";

// TMS7000
// const char TEXT_ADC[]  PROGMEM = "ADC";
// const char TEXT_ADD[]  PROGMEM = "ADD";
// const char TEXT_AND[]  PROGMEM = "AND";
const char TEXT_ANDP[]  PROGMEM = "ANDP";
const char TEXT_BTJO[]  PROGMEM = "BTJO";
const char TEXT_BTJOP[] PROGMEM = "BTJOP";
const char TEXT_BTJZ[]  PROGMEM = "BTJZ";
const char TEXT_BTJZP[] PROGMEM = "BTJZP";
// const char TEXT_BR[]    PROGMEM = "BR";
// const char TEXT_CALL[]  PROGMEM = "CALL";
// const char TEXT_CLR[]   PROGMEM = "CLR";
// const char TEXT_CLRC[]  PROGMEM = "CLRC";
// const char TEXT_CMP[]   PROGMEM = "CMP";
const char TEXT_DAC[]   PROGMEM = "DAC";
// const char TEXT_DEC[]  PROGMEM = "DEC";
// const char TEXT_DECD[]  PROGMEM = "DECD";
// const char TEXT_DINT[]  PROGMEM = "DINT";
// const char TEXT_DJNZ[]  PROGMEM = "DJNZ";
// const char TEXT_DSB[]   PROGMEM = "DSB";
// const char TEXT_EINT[]  PROGMEM = "EINT";
// const char TEXT_IDLE[]  PROGMEM = "IDLE";
// const char TEXT_INC[]   PROGMEM = "INC";
// const char TEXT_INV[]   PROGMEM = "INV";
const char TEXT_JC[]    PROGMEM = "JC";;
// const char TEXT_JEQ[]   PROGMEM = "JEQ";;
const char TEXT_JHS[]   PROGMEM = "JHS";;
const char TEXT_JL[]    PROGMEM = "JL";;
// const char TEXT_JMP[]   PROGMEM = "JMP";
const char TEXT_JNC[]   PROGMEM = "JNC";;
const char TEXT_JNE[]   PROGMEM = "JNE";;
const char TEXT_JN[]    PROGMEM = "JN";;
const char TEXT_JNZ[]   PROGMEM = "JNZ";;
const char TEXT_JP[]    PROGMEM = "JP";;
const char TEXT_JPZ[]   PROGMEM = "JPZ";;
const char TEXT_JZ[]    PROGMEM = "JZ";;
// const char TEXT_LDA[]   PROGMEM = "LDA";
const char TEXT_LDSP[]  PROGMEM = "LDSP";
// const char TEXT_MOVD[]  PROGMEM = "MOVD";
// const char TEXT_MOV[]   PROGMEM = "MOV";
// const char TEXT_MOVP[]  PROGMEM = "MOVP";
// const char TEXT_MPY[]   PROGMEM = "MPY";
// const char TEXT_NOP[]   PROGMEM = "NOP";
// const char TEXT_OR[]    PROGMEM = "OR";
const char TEXT_ORP[]   PROGMEM = "ORP";
// const char TEXT_POP[]   PROGMEM = "POP";
// const char TEXT_PUSH[]  PROGMEM = "PUSH";
// const char TEXT_RETI[]  PROGMEM = "RETI";
const char TEXT_RETS[]  PROGMEM = "RETS";
// const char TEXT_RL[]    PROGMEM = "RL";
// const char TEXT_RLC[]   PROGMEM = "RLC";
// const char TEXT_RR[]    PROGMEM = "RR";
// const char TEXT_RRC[]   PROGMEM = "RRC";
// const char TEXT_SBB[]   PROGMEM = "SBB";
// const char TEXT_SETC[]  PROGMEM = "SETC";
// const char TEXT_STA[]   PROGMEM = "STA";
const char TEXT_STSP[] PROGMEM = "STSP";
// const char TEXT_SUB[]   PROGMEM = "SUB";
// const char TEXT_SWAP[]  PROGMEM = "SWAP";
// const char TEXT_TSTA[]  PROGMEM = "TSTA";
// const char TEXT_TSTB[]  PROGMEM = "TSTB";
// const char TEXT_TRAP[]  PROGMEM = "TRAP";
const char TEXT_XCHB[] PROGMEM = "XCHB";
// const char TEXT_XOR[]  PROGMEM = "XOR";
const char TEXT_XORP[] PROGMEM = "XORP";

const char TEXT_REG_ST[] PROGMEM = "ST";

// clang-format on

}  // namespace tms7000
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
