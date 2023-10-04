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
constexpr char TEXT_I8086_LIST[] PROGMEM = "i8086, i80186, V30";
constexpr char TEXT_CPU_8086[]  PROGMEM = "8086";
constexpr char TEXT_CPU_80186[] PROGMEM = "80186";
constexpr char TEXT_CPU_V30[]   PROGMEM = "V30";

constexpr char TEXT_AAA[]    PROGMEM = "AAA";
constexpr char TEXT_AAD[]    PROGMEM = "AAD";
constexpr char TEXT_AAM[]    PROGMEM = "AAM";
constexpr char TEXT_AAS[]    PROGMEM = "AAS";
// constexpr char TEXT_ADC[]    PROGMEM = "ADC";
// constexpr char TEXT_ADD[]    PROGMEM = "ADD";
// constexpr char TEXT_AND[]    PROGMEM = "AND";
// constexpr char TEXT_CALL[]   PROGMEM = "CALL";
constexpr char TEXT_CALLF[]  PROGMEM = "CALLF";
constexpr char TEXT_CBW[]    PROGMEM = "CBW";
// constexpr char TEXT_CLC[]    PROGMEM = "CLC";
// constexpr char TEXT_CLD[]    PROGMEM = "CLD";
// constexpr char TEXT_CLI[]    PROGMEM = "CLI";
// constexpr char TEXT_CMC[]    PROGMEM = "CMC";
// constexpr char TEXT_CMP[]    PROGMEM = "CMP";
// constexpr char TEXT_CMPS[]   PROGMEM = "CMPS";
// constexpr char TEXT_CMPSB[]  PROGMEM = "CMPSB";
// constexpr char TEXT_CMPSW[]  PROGMEM = "CMPSW";
constexpr char TEXT_CWD[]    PROGMEM = "CWD";
// constexpr char TEXT_DAA[]    PROGMEM = "DAA";
// constexpr char TEXT_DAS[]    PROGMEM = "DAS";
// constexpr char TEXT_DEC[]    PROGMEM = "DEC";
// constexpr char TEXT_DIV[]    PROGMEM = "DIV";
// constexpr char TEXT_HLT[]    PROGMEM = "HLT";
// constexpr char TEXT_IDIV[]   PROGMEM = "IDIV";
constexpr char TEXT_IMUL[]   PROGMEM = "IMUL";
// constexpr char TEXT_IN[]     PROGMEM = "IN";
// constexpr char TEXT_INC[]    PROGMEM = "INC";
constexpr char TEXT_INT[]    PROGMEM = "INT";
constexpr char TEXT_INTO[]   PROGMEM = "INTO";
// constexpr char TEXT_IRET[]   PROGMEM = "IRET";
constexpr char TEXT_JA[]     PROGMEM = "JA";
constexpr char TEXT_JAE[]    PROGMEM = "JAE";
// constexpr char TEXT_JB[]     PROGMEM = "JB";
constexpr char TEXT_JBE[]    PROGMEM = "JBE";
// constexpr char TEXT_JC[]     PROGMEM = "JC";
constexpr char TEXT_JCXZ[]   PROGMEM = "JCXZ";
// constexpr char TEXT_JE[]     PROGMEM = "JE";
constexpr char TEXT_JG[]     PROGMEM = "JG";
// constexpr char TEXT_JGE[]    PROGMEM = "JGE";
// constexpr char TEXT_JL[]     PROGMEM = "JL";
// constexpr char TEXT_JLE[]    PROGMEM = "JLE";
// constexpr char TEXT_JMP[]    PROGMEM = "JMP";
constexpr char TEXT_JMPF[]   PROGMEM = "JMPF";
constexpr char TEXT_JNA[]    PROGMEM = "JNA";
constexpr char TEXT_JNAE[]   PROGMEM = "JNAE";
// constexpr char TEXT_JNB[]    PROGMEM = "JNB";
constexpr char TEXT_JNBE[]   PROGMEM = "JNBE";
// constexpr char TEXT_JNC[]    PROGMEM = "JNC";
// constexpr char TEXT_JNE[]    PROGMEM = "JNE";
constexpr char TEXT_JNG[]    PROGMEM = "JNG";
constexpr char TEXT_JNGE[]   PROGMEM = "JNGE";
constexpr char TEXT_JNL[]    PROGMEM = "JNL";
constexpr char TEXT_JNLE[]   PROGMEM = "JNLE";
// constexpr char TEXT_JNO[]    PROGMEM = "JNO";
constexpr char TEXT_JNP[]    PROGMEM = "JNP";
constexpr char TEXT_JNS[]    PROGMEM = "JNS";
// constexpr char TEXT_JNZ[]    PROGMEM = "JNZ";
constexpr char TEXT_JO[]     PROGMEM = "JO";
// constexpr char TEXT_JP[]     PROGMEM = "JP";
// constexpr char TEXT_JPE[]    PROGMEM = "JPE";
// constexpr char TEXT_JPO[]    PROGMEM = "JPO";
constexpr char TEXT_JS[]     PROGMEM = "JS";
// constexpr char TEXT_JZ[]     PROGMEM = "JZ";
constexpr char TEXT_LAHF[]   PROGMEM = "LAHF";
// constexpr char TEXT_LDS[]    PROGMEM = "LDS";
// constexpr char TEXT_LEA[]    PROGMEM = "LEA";
constexpr char TEXT_LES[]    PROGMEM = "LES";
constexpr char TEXT_LOCK[]   PROGMEM = "LOCK";
constexpr char TEXT_LODS[]   PROGMEM = "LODS";
constexpr char TEXT_LODSB[]  PROGMEM = "LODSB";
constexpr char TEXT_LODSW[]  PROGMEM = "LODSW";
constexpr char TEXT_LOOP[]   PROGMEM = "LOOP";
constexpr char TEXT_LOOPE[]  PROGMEM = "LOOPE";
constexpr char TEXT_LOOPNE[] PROGMEM = "LOOPNE";
constexpr char TEXT_LOOPNZ[] PROGMEM = "LOOPNZ";
constexpr char TEXT_LOOPZ[]  PROGMEM = "LOOPZ";
// constexpr char TEXT_MOV[]    PROGMEM = "MOV";
constexpr char TEXT_MOVS[]   PROGMEM = "MOVS";
// constexpr char TEXT_MOVSB[]  PROGMEM = "MOVSB";
// constexpr char TEXT_MOVSW[]  PROGMEM = "MOVSW";
// constexpr char TEXT_MUL[]    PROGMEM = "MUL";
// constexpr char TEXT_NEG[]    PROGMEM = "NEG";
// constexpr char TEXT_NOP[]    PROGMEM = "NOP";
// constexpr char TEXT_NOT[]    PROGMEM = "NOT";
// constexpr char TEXT_OR[]     PROGMEM = "OR";
// constexpr char TEXT_OUT[]    PROGMEM = "OUT";
// constexpr char TEXT_POP[]    PROGMEM = "POP";
// constexpr char TEXT_POPF[]   PROGMEM = "POPF";
// constexpr char TEXT_PUSH[]   PROGMEM = "PUSH";
// constexpr char TEXT_PUSHF[]  PROGMEM = "PUSHF";
constexpr char TEXT_RCL[]    PROGMEM = "RCL";
constexpr char TEXT_RCR[]    PROGMEM = "RCR";
// constexpr char TEXT_REP[]    PROGMEM = "REP";
constexpr char TEXT_REPE[]   PROGMEM = "REPE";
constexpr char TEXT_REPNE[]  PROGMEM = "REPNE";
constexpr char TEXT_REPNZ[]  PROGMEM = "REPNZ";
constexpr char TEXT_REPZ[]   PROGMEM = "REPZ";
// constexpr char TEXT_RET[]    PROGMEM = "RET";
constexpr char TEXT_RETF[]   PROGMEM = "RETF";
// constexpr char TEXT_ROL[]    PROGMEM = "ROL";
// constexpr char TEXT_ROR[]    PROGMEM = "ROR";
constexpr char TEXT_SAHF[]   PROGMEM = "SAHF";
constexpr char TEXT_SAL[]    PROGMEM = "SAL";
// constexpr char TEXT_SAR[]    PROGMEM = "SAR";
// constexpr char TEXT_SBB[]    PROGMEM = "SBB";
constexpr char TEXT_SCAS[]   PROGMEM = "SCAS";
constexpr char TEXT_SCASB[]  PROGMEM = "SCASB";
constexpr char TEXT_SCASW[]  PROGMEM = "SCASW";
constexpr char TEXT_SEGCS[]  PROGMEM = "SEGCS";
constexpr char TEXT_SEGDS[]  PROGMEM = "SEGDS";
constexpr char TEXT_SEGES[]  PROGMEM = "SEGES";
constexpr char TEXT_SEGSS[]  PROGMEM = "SEGSS";
// constexpr char TEXT_SHL[]    PROGMEM = "SHL";
// constexpr char TEXT_SHR[]    PROGMEM = "SHR";
// constexpr char TEXT_STC[]    PROGMEM = "STC";
// constexpr char TEXT_STD[]    PROGMEM = "STD";
constexpr char TEXT_STI[]    PROGMEM = "STI";
constexpr char TEXT_STOS[]   PROGMEM = "STOS";
constexpr char TEXT_STOSB[]  PROGMEM = "STOSB";
constexpr char TEXT_STOSW[]  PROGMEM = "STOSW";
// constexpr char TEXT_SUB[]    PROGMEM = "SUB";
// constexpr char TEXT_TEST[]   PROGMEM = "TEST";
// constexpr char TEXT_WAIT[]   PROGMEM = "WAIT";
// constexpr char TEXT_XCHG[]   PROGMEM = "XCHG";
constexpr char TEXT_XLAT[]   PROGMEM = "XLAT";
// constexpr char TEXT_XOR[]    PROGMEM = "XOR";

// i80186
// constexpr char TEXT_ENTER[]  PROGMEM = "ENTER";
constexpr char TEXT_LEAVE[]  PROGMEM = "LEAVE";
constexpr char TEXT_PUSHA[]  PROGMEM = "PUSHA";
constexpr char TEXT_POPA[]   PROGMEM = "POPA";
constexpr char TEXT_BOUND[]  PROGMEM = "BOUND";
// constexpr char TEXT_INS[]    PROGMEM = "INS";
// constexpr char TEXT_INSB[]   PROGMEM = "INSB";
// constexpr char TEXT_INSW[]   PROGMEM = "INSW";
// constexpr char TEXT_OUTS[]   PROGMEM = "OUTS";
constexpr char TEXT_OUTSB[]  PROGMEM = "OUTSB";
constexpr char TEXT_OUTSW[]  PROGMEM = "OUTSW";

// V30
constexpr char TEXT_ADD4S[]  PROGMEM = "ADD4S";
constexpr char TEXT_BRKEM[]  PROGMEM = "BRKEM";
constexpr char TEXT_CLR1[]   PROGMEM = "CLR1";
constexpr char TEXT_CMP4S[]  PROGMEM = "CMP4S";
// constexpr char TEXT_EXT[]    PROGMEM = "EXT";
constexpr char TEXT_NOT1[]   PROGMEM = "NOT1";
constexpr char TEXT_REPC[]   PROGMEM = "REPC";
constexpr char TEXT_REPNC[]  PROGMEM = "REPNC";
constexpr char TEXT_ROL4[]   PROGMEM = "ROL4";
constexpr char TEXT_ROR4[]   PROGMEM = "ROR4";
constexpr char TEXT_SET1[]   PROGMEM = "SET1";
constexpr char TEXT_SUB4S[]  PROGMEM = "SUB4S";
constexpr char TEXT_TEST1[]  PROGMEM = "TEST1";

constexpr char TEXT_REG_AL[]   PROGMEM = "AL";
constexpr char TEXT_REG_BL[]   PROGMEM = "BL";
constexpr char TEXT_REG_CL[]   PROGMEM = "CL";
constexpr char TEXT_REG_DL[]   PROGMEM = "DL";
constexpr char TEXT_REG_AH[]   PROGMEM = "AH";
constexpr char TEXT_REG_BH[]   PROGMEM = "BH";
constexpr char TEXT_REG_CH[]   PROGMEM = "CH";
constexpr char TEXT_REG_DH[]   PROGMEM = "DH";
constexpr char TEXT_REG_AX[]   PROGMEM = "AX";
constexpr char TEXT_REG_BX[]   PROGMEM = "BX";
constexpr char TEXT_REG_CX[]   PROGMEM = "CX";
constexpr char TEXT_REG_DX[]   PROGMEM = "DX";
// constexpr char TEXT_REG_SP[]   PROGMEM = "SP";
constexpr char TEXT_REG_BP[]   PROGMEM = "BP";
constexpr char TEXT_REG_SI[]   PROGMEM = "SI";
constexpr char TEXT_REG_DI[]   PROGMEM = "DI";
constexpr char TEXT_REG_CS[]   PROGMEM = "CS";
constexpr char TEXT_REG_DS[]   PROGMEM = "DS";
constexpr char TEXT_REG_ES[]   PROGMEM = "ES";
constexpr char TEXT_REG_SS[]   PROGMEM = "SS";
constexpr char TEXT_REG_PTR[]  PROGMEM  = "PTR";
constexpr char TEXT_REG_BYTE[] PROGMEM = "BYTE";
constexpr char TEXT_REG_WORD[] PROGMEM = "WORD";
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
