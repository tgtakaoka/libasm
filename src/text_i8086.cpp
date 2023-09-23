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

#include "text_i8086.h"

namespace libasm {
namespace text {
namespace i8086 {

// clang-format off
const char TEXT_I8086_LIST[] PROGMEM = "i8086, i80186, V30";
const char TEXT_CPU_8086[]  PROGMEM = "8086";
const char TEXT_CPU_80186[] PROGMEM = "80186";
const char TEXT_CPU_V30[]   PROGMEM = "V30";

const char TEXT_AAA[]    PROGMEM = "AAA";
const char TEXT_AAD[]    PROGMEM = "AAD";
const char TEXT_AAM[]    PROGMEM = "AAM";
const char TEXT_AAS[]    PROGMEM = "AAS";
// const char TEXT_ADC[]    PROGMEM = "ADC";
// const char TEXT_ADD[]    PROGMEM = "ADD";
// const char TEXT_AND[]    PROGMEM = "AND";
// const char TEXT_CALL[]   PROGMEM = "CALL";
const char TEXT_CALLF[]  PROGMEM = "CALLF";
const char TEXT_CBW[]    PROGMEM = "CBW";
// const char TEXT_CLC[]    PROGMEM = "CLC";
// const char TEXT_CLD[]    PROGMEM = "CLD";
// const char TEXT_CLI[]    PROGMEM = "CLI";
// const char TEXT_CMC[]    PROGMEM = "CMC";
// const char TEXT_CMP[]    PROGMEM = "CMP";
// const char TEXT_CMPS[]   PROGMEM = "CMPS";
// const char TEXT_CMPSB[]  PROGMEM = "CMPSB";
// const char TEXT_CMPSW[]  PROGMEM = "CMPSW";
const char TEXT_CWD[]    PROGMEM = "CWD";
// const char TEXT_DAA[]    PROGMEM = "DAA";
// const char TEXT_DAS[]    PROGMEM = "DAS";
// const char TEXT_DEC[]    PROGMEM = "DEC";
// const char TEXT_DIV[]    PROGMEM = "DIV";
// const char TEXT_HLT[]    PROGMEM = "HLT";
// const char TEXT_IDIV[]   PROGMEM = "IDIV";
const char TEXT_IMUL[]   PROGMEM = "IMUL";
// const char TEXT_IN[]     PROGMEM = "IN";
// const char TEXT_INC[]    PROGMEM = "INC";
const char TEXT_INT[]    PROGMEM = "INT";
const char TEXT_INTO[]   PROGMEM = "INTO";
// const char TEXT_IRET[]   PROGMEM = "IRET";
const char TEXT_JA[]     PROGMEM = "JA";
const char TEXT_JAE[]    PROGMEM = "JAE";
// const char TEXT_JB[]     PROGMEM = "JB";
const char TEXT_JBE[]    PROGMEM = "JBE";
// const char TEXT_JC[]     PROGMEM = "JC";
const char TEXT_JCXZ[]   PROGMEM = "JCXZ";
// const char TEXT_JE[]     PROGMEM = "JE";
const char TEXT_JG[]     PROGMEM = "JG";
// const char TEXT_JGE[]    PROGMEM = "JGE";
// const char TEXT_JL[]     PROGMEM = "JL";
// const char TEXT_JLE[]    PROGMEM = "JLE";
// const char TEXT_JMP[]    PROGMEM = "JMP";
const char TEXT_JMPF[]   PROGMEM = "JMPF";
const char TEXT_JNA[]    PROGMEM = "JNA";
const char TEXT_JNAE[]   PROGMEM = "JNAE";
// const char TEXT_JNB[]    PROGMEM = "JNB";
const char TEXT_JNBE[]   PROGMEM = "JNBE";
// const char TEXT_JNC[]    PROGMEM = "JNC";
// const char TEXT_JNE[]    PROGMEM = "JNE";
const char TEXT_JNG[]    PROGMEM = "JNG";
const char TEXT_JNGE[]   PROGMEM = "JNGE";
const char TEXT_JNL[]    PROGMEM = "JNL";
const char TEXT_JNLE[]   PROGMEM = "JNLE";
// const char TEXT_JNO[]    PROGMEM = "JNO";
const char TEXT_JNP[]    PROGMEM = "JNP";
const char TEXT_JNS[]    PROGMEM = "JNS";
// const char TEXT_JNZ[]    PROGMEM = "JNZ";
const char TEXT_JO[]     PROGMEM = "JO";
// const char TEXT_JP[]     PROGMEM = "JP";
// const char TEXT_JPE[]    PROGMEM = "JPE";
// const char TEXT_JPO[]    PROGMEM = "JPO";
const char TEXT_JS[]     PROGMEM = "JS";
// const char TEXT_JZ[]     PROGMEM = "JZ";
const char TEXT_LAHF[]   PROGMEM = "LAHF";
// const char TEXT_LDS[]    PROGMEM = "LDS";
// const char TEXT_LEA[]    PROGMEM = "LEA";
const char TEXT_LES[]    PROGMEM = "LES";
const char TEXT_LOCK[]   PROGMEM = "LOCK";
const char TEXT_LODS[]   PROGMEM = "LODS";
const char TEXT_LODSB[]  PROGMEM = "LODSB";
const char TEXT_LODSW[]  PROGMEM = "LODSW";
const char TEXT_LOOP[]   PROGMEM = "LOOP";
const char TEXT_LOOPE[]  PROGMEM = "LOOPE";
const char TEXT_LOOPNE[] PROGMEM = "LOOPNE";
const char TEXT_LOOPNZ[] PROGMEM = "LOOPNZ";
const char TEXT_LOOPZ[]  PROGMEM = "LOOPZ";
// const char TEXT_MOV[]    PROGMEM = "MOV";
const char TEXT_MOVS[]   PROGMEM = "MOVS";
// const char TEXT_MOVSB[]  PROGMEM = "MOVSB";
// const char TEXT_MOVSW[]  PROGMEM = "MOVSW";
// const char TEXT_MUL[]    PROGMEM = "MUL";
// const char TEXT_NEG[]    PROGMEM = "NEG";
// const char TEXT_NOP[]    PROGMEM = "NOP";
// const char TEXT_NOT[]    PROGMEM = "NOT";
// const char TEXT_OR[]     PROGMEM = "OR";
// const char TEXT_OUT[]    PROGMEM = "OUT";
// const char TEXT_POP[]    PROGMEM = "POP";
// const char TEXT_POPF[]   PROGMEM = "POPF";
// const char TEXT_PUSH[]   PROGMEM = "PUSH";
// const char TEXT_PUSHF[]  PROGMEM = "PUSHF";
const char TEXT_RCL[]    PROGMEM = "RCL";
const char TEXT_RCR[]    PROGMEM = "RCR";
// const char TEXT_REP[]    PROGMEM = "REP";
const char TEXT_REPE[]   PROGMEM = "REPE";
const char TEXT_REPNE[]  PROGMEM = "REPNE";
const char TEXT_REPNZ[]  PROGMEM = "REPNZ";
const char TEXT_REPZ[]   PROGMEM = "REPZ";
// const char TEXT_RET[]    PROGMEM = "RET";
const char TEXT_RETF[]   PROGMEM = "RETF";
// const char TEXT_ROL[]    PROGMEM = "ROL";
// const char TEXT_ROR[]    PROGMEM = "ROR";
const char TEXT_SAHF[]   PROGMEM = "SAHF";
const char TEXT_SAL[]    PROGMEM = "SAL";
// const char TEXT_SAR[]    PROGMEM = "SAR";
// const char TEXT_SBB[]    PROGMEM = "SBB";
const char TEXT_SCAS[]   PROGMEM = "SCAS";
const char TEXT_SCASB[]  PROGMEM = "SCASB";
const char TEXT_SCASW[]  PROGMEM = "SCASW";
const char TEXT_SEGCS[]  PROGMEM = "SEGCS";
const char TEXT_SEGDS[]  PROGMEM = "SEGDS";
const char TEXT_SEGES[]  PROGMEM = "SEGES";
const char TEXT_SEGSS[]  PROGMEM = "SEGSS";
// const char TEXT_SHL[]    PROGMEM = "SHL";
// const char TEXT_SHR[]    PROGMEM = "SHR";
// const char TEXT_STC[]    PROGMEM = "STC";
// const char TEXT_STD[]    PROGMEM = "STD";
const char TEXT_STI[]    PROGMEM = "STI";
const char TEXT_STOS[]   PROGMEM = "STOS";
const char TEXT_STOSB[]  PROGMEM = "STOSB";
const char TEXT_STOSW[]  PROGMEM = "STOSW";
// const char TEXT_SUB[]    PROGMEM = "SUB";
// const char TEXT_TEST[]   PROGMEM = "TEST";
// const char TEXT_WAIT[]   PROGMEM = "WAIT";
// const char TEXT_XCHG[]   PROGMEM = "XCHG";
const char TEXT_XLAT[]   PROGMEM = "XLAT";
// const char TEXT_XOR[]    PROGMEM = "XOR";

// i80186
// const char TEXT_ENTER[]  PROGMEM = "ENTER";
const char TEXT_LEAVE[]  PROGMEM = "LEAVE";
const char TEXT_PUSHA[]  PROGMEM = "PUSHA";
const char TEXT_POPA[]   PROGMEM = "POPA";
const char TEXT_BOUND[]  PROGMEM = "BOUND";
// const char TEXT_INS[]    PROGMEM = "INS";
// const char TEXT_INSB[]   PROGMEM = "INSB";
// const char TEXT_INSW[]   PROGMEM = "INSW";
// const char TEXT_OUTS[]   PROGMEM = "OUTS";
const char TEXT_OUTSB[]  PROGMEM = "OUTSB";
const char TEXT_OUTSW[]  PROGMEM = "OUTSW";

// V30
const char TEXT_ADD4S[]  PROGMEM = "ADD4S";
const char TEXT_BRKEM[]  PROGMEM = "BRKEM";
const char TEXT_CLR1[]   PROGMEM = "CLR1";
const char TEXT_CMP4S[]  PROGMEM = "CMP4S";
// const char TEXT_EXT[]    PROGMEM = "EXT";
const char TEXT_NOT1[]   PROGMEM = "NOT1";
const char TEXT_REPC[]   PROGMEM = "REPC";
const char TEXT_REPNC[]  PROGMEM = "REPNC";
const char TEXT_ROL4[]   PROGMEM = "ROL4";
const char TEXT_ROR4[]   PROGMEM = "ROR4";
const char TEXT_SET1[]   PROGMEM = "SET1";
const char TEXT_SUB4S[]  PROGMEM = "SUB4S";
const char TEXT_TEST1[]  PROGMEM = "TEST1";

const char TEXT_REG_AL[]   PROGMEM = "AL";
const char TEXT_REG_BL[]   PROGMEM = "BL";
const char TEXT_REG_CL[]   PROGMEM = "CL";
const char TEXT_REG_DL[]   PROGMEM = "DL";
const char TEXT_REG_AH[]   PROGMEM = "AH";
const char TEXT_REG_BH[]   PROGMEM = "BH";
const char TEXT_REG_CH[]   PROGMEM = "CH";
const char TEXT_REG_DH[]   PROGMEM = "DH";
const char TEXT_REG_AX[]   PROGMEM = "AX";
const char TEXT_REG_BX[]   PROGMEM = "BX";
const char TEXT_REG_CX[]   PROGMEM = "CX";
const char TEXT_REG_DX[]   PROGMEM = "DX";
// const char TEXT_REG_SP[]   PROGMEM = "SP";
const char TEXT_REG_BP[]   PROGMEM = "BP";
const char TEXT_REG_SI[]   PROGMEM = "SI";
const char TEXT_REG_DI[]   PROGMEM = "DI";
const char TEXT_REG_CS[]   PROGMEM = "CS";
const char TEXT_REG_DS[]   PROGMEM = "DS";
const char TEXT_REG_ES[]   PROGMEM = "ES";
const char TEXT_REG_SS[]   PROGMEM = "SS";
const char TEXT_REG_PTR[]  PROGMEM  = "PTR";
const char TEXT_REG_BYTE[] PROGMEM = "BYTE";
const char TEXT_REG_WORD[] PROGMEM = "WORD";
// clang-format on

}  // namespace i8086
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
