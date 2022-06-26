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

#ifndef __TEXT_I8086__
#define __TEXT_I8086__

#include "config_host.h"

namespace libasm {
namespace i8086 {

// clang-format off
static const char TEXT_CPU_LIST[] PROGMEM = "i8086, i80186";
static const char TEXT_CPU_I8086[] PROGMEM = "i8086";
static constexpr const char *TEXT_CPU_I80186 = TEXT_CPU_LIST + 7;
static constexpr const char *TEXT_CPU_8086 = TEXT_CPU_I8086 + 1;
static constexpr const char *TEXT_CPU_80186 = TEXT_CPU_I80186 + 1;

static const char TEXT_AAA[]    PROGMEM = "AAA";
static const char TEXT_AAD[]    PROGMEM = "AAD";
static const char TEXT_AAM[]    PROGMEM = "AAM";
static const char TEXT_AAS[]    PROGMEM = "AAS";
static const char TEXT_ADC[]    PROGMEM = "ADC";
static const char TEXT_ADD[]    PROGMEM = "ADD";
static const char TEXT_AND[]    PROGMEM = "AND";
static const char TEXT_CALL[]   PROGMEM = "CALL";
static const char TEXT_CALLF[]  PROGMEM = "CALLF";
static const char TEXT_CBW[]    PROGMEM = "CBW";
static const char TEXT_CLC[]    PROGMEM = "CLC";
static const char TEXT_CLD[]    PROGMEM = "CLD";
static const char TEXT_CLI[]    PROGMEM = "CLI";
static const char TEXT_CMC[]    PROGMEM = "CMC";
static const char TEXT_CMP[]    PROGMEM = "CMP";
static const char TEXT_CMPS[]   PROGMEM = "CMPS";
static const char TEXT_CMPSB[]  PROGMEM = "CMPSB";
static const char TEXT_CMPSW[]  PROGMEM = "CMPSW";
static const char TEXT_CWD[]    PROGMEM = "CWD";
static const char TEXT_DAA[]    PROGMEM = "DAA";
static const char TEXT_DAS[]    PROGMEM = "DAS";
static const char TEXT_DEC[]    PROGMEM = "DEC";
static const char TEXT_DIV[]    PROGMEM = "DIV";
static const char TEXT_HLT[]    PROGMEM = "HLT";
static const char TEXT_IDIV[]   PROGMEM = "IDIV";
static const char TEXT_IMUL[]   PROGMEM = "IMUL";
static const char TEXT_IN[]     PROGMEM = "IN";
static const char TEXT_INC[]    PROGMEM = "INC";
static const char TEXT_INT[]    PROGMEM = "INT";
static const char TEXT_INTO[]   PROGMEM = "INTO";
static const char TEXT_IRET[]   PROGMEM = "IRET";
static const char TEXT_JA[]     PROGMEM = "JA";
static const char TEXT_JAE[]    PROGMEM = "JAE";
static const char TEXT_JB[]     PROGMEM = "JB";
static const char TEXT_JBE[]    PROGMEM = "JBE";
static const char TEXT_JC[]     PROGMEM = "JC";
static const char TEXT_JCXZ[]   PROGMEM = "JCXZ";
static const char TEXT_JE[]     PROGMEM = "JE";
static const char TEXT_JG[]     PROGMEM = "JG";
static const char TEXT_JGE[]    PROGMEM = "JGE";
static const char TEXT_JL[]     PROGMEM = "JL";
static const char TEXT_JLE[]    PROGMEM = "JLE";
static const char TEXT_JMP[]    PROGMEM = "JMP";
static const char TEXT_JMPF[]   PROGMEM = "JMPF";
static const char TEXT_JNA[]    PROGMEM = "JNA";
static const char TEXT_JNAE[]   PROGMEM = "JNAE";
static const char TEXT_JNB[]    PROGMEM = "JNB";
static const char TEXT_JNBE[]   PROGMEM = "JNBE";
static const char TEXT_JNC[]    PROGMEM = "JNC";
static const char TEXT_JNE[]    PROGMEM = "JNE";
static const char TEXT_JNG[]    PROGMEM = "JNG";
static const char TEXT_JNGE[]   PROGMEM = "JNGE";
static const char TEXT_JNL[]    PROGMEM = "JNL";
static const char TEXT_JNLE[]   PROGMEM = "JNLE";
static const char TEXT_JNO[]    PROGMEM = "JNO";
static const char TEXT_JNP[]    PROGMEM = "JNP";
static const char TEXT_JNS[]    PROGMEM = "JNS";
static const char TEXT_JNZ[]    PROGMEM = "JNZ";
static const char TEXT_JO[]     PROGMEM = "JO";
static const char TEXT_JP[]     PROGMEM = "JP";
static const char TEXT_JPE[]    PROGMEM = "JPE";
static const char TEXT_JPO[]    PROGMEM = "JPO";
static const char TEXT_JS[]     PROGMEM = "JS";
static const char TEXT_JZ[]     PROGMEM = "JZ";
static const char TEXT_LAHF[]   PROGMEM = "LAHF";
static const char TEXT_LDS[]    PROGMEM = "LDS";
static const char TEXT_LEA[]    PROGMEM = "LEA";
static const char TEXT_LES[]    PROGMEM = "LES";
static const char TEXT_LOCK[]   PROGMEM = "LOCK";
static const char TEXT_LODS[]   PROGMEM = "LODS";
static const char TEXT_LODSB[]  PROGMEM = "LODSB";
static const char TEXT_LODSW[]  PROGMEM = "LODSW";
static const char TEXT_LOOP[]   PROGMEM = "LOOP";
static const char TEXT_LOOPE[]  PROGMEM = "LOOPE";
static const char TEXT_LOOPNE[] PROGMEM = "LOOPNE";
static const char TEXT_LOOPNZ[] PROGMEM = "LOOPNZ";
static const char TEXT_LOOPZ[]  PROGMEM = "LOOPZ";
static const char TEXT_MOV[]    PROGMEM = "MOV";
static const char TEXT_MOVS[]   PROGMEM = "MOVS";
static const char TEXT_MOVSB[]  PROGMEM = "MOVSB";
static const char TEXT_MOVSW[]  PROGMEM = "MOVSW";
static const char TEXT_MUL[]    PROGMEM = "MUL";
static const char TEXT_NEG[]    PROGMEM = "NEG";
static const char TEXT_NOP[]    PROGMEM = "NOP";
static const char TEXT_NOT[]    PROGMEM = "NOT";
static const char TEXT_OR[]     PROGMEM = "OR";
static const char TEXT_OUT[]    PROGMEM = "OUT";
static const char TEXT_POP[]    PROGMEM = "POP";
static const char TEXT_POPF[]   PROGMEM = "POPF";
static const char TEXT_PUSH[]   PROGMEM = "PUSH";
static const char TEXT_PUSHF[]  PROGMEM = "PUSHF";
static const char TEXT_RCL[]    PROGMEM = "RCL";
static const char TEXT_RCR[]    PROGMEM = "RCR";
static const char TEXT_REP[]    PROGMEM = "REP";
static const char TEXT_REPE[]   PROGMEM = "REPE";
static const char TEXT_REPNE[]  PROGMEM = "REPNE";
static const char TEXT_REPNZ[]  PROGMEM = "REPNZ";
static const char TEXT_REPZ[]   PROGMEM = "REPZ";
static const char TEXT_RET[]    PROGMEM = "RET";
static const char TEXT_RETF[]   PROGMEM = "RETF";
static const char TEXT_ROL[]    PROGMEM = "ROL";
static const char TEXT_ROR[]    PROGMEM = "ROR";
static const char TEXT_SAHF[]   PROGMEM = "SAHF";
static const char TEXT_SAL[]    PROGMEM = "SAL";
static const char TEXT_SAR[]    PROGMEM = "SAR";
static const char TEXT_SBB[]    PROGMEM = "SBB";
static const char TEXT_SCAS[]   PROGMEM = "SCAS";
static const char TEXT_SCASB[]  PROGMEM = "SCASB";
static const char TEXT_SCASW[]  PROGMEM = "SCASW";
static const char TEXT_SEGCS[]  PROGMEM = "SEGCS";
static const char TEXT_SEGDS[]  PROGMEM = "SEGDS";
static const char TEXT_SEGES[]  PROGMEM = "SEGES";
static const char TEXT_SEGSS[]  PROGMEM = "SEGSS";
static const char TEXT_SHL[]    PROGMEM = "SHL";
static const char TEXT_SHR[]    PROGMEM = "SHR";
static const char TEXT_STC[]    PROGMEM = "STC";
static const char TEXT_STD[]    PROGMEM = "STD";
static const char TEXT_STI[]    PROGMEM = "STI";
static const char TEXT_STOS[]   PROGMEM = "STOS";
static const char TEXT_STOSB[]  PROGMEM = "STOSB";
static const char TEXT_STOSW[]  PROGMEM = "STOSW";
static const char TEXT_SUB[]    PROGMEM = "SUB";
static const char TEXT_TEST[]   PROGMEM = "TEST";
static const char TEXT_WAIT[]   PROGMEM = "WAIT";
static const char TEXT_XCHG[]   PROGMEM = "XCHG";
static const char TEXT_XLAT[]   PROGMEM = "XLAT";
static const char TEXT_XOR[]    PROGMEM = "XOR";

// i80186
static const char TEXT_ENTER[]  PROGMEM = "ENTER";
static const char TEXT_LEAVE[]  PROGMEM = "LEAVE";
static const char TEXT_PUSHA[]  PROGMEM = "PUSHA";
static const char TEXT_POPA[]   PROGMEM = "POPA";
static const char TEXT_BOUND[]  PROGMEM = "BOUND";
static const char TEXT_INS[]    PROGMEM = "INS";
static const char TEXT_INSB[]   PROGMEM = "INSB";
static const char TEXT_INSW[]   PROGMEM = "INSW";
static const char TEXT_OUTS[]   PROGMEM = "OUTS";
static const char TEXT_OUTSB[]  PROGMEM = "OUTSB";
static const char TEXT_OUTSW[]  PROGMEM = "OUTSW";

// clang-format on

}  // namespace i8086
}  // namespace libasm

#endif  // __TEXT_I8086__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
