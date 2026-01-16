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

#ifndef __LIBASM_TEXT_I8086__
#define __LIBASM_TEXT_I8086__

#include "text_common.h"

namespace libasm {
namespace text {
namespace i8086 {
// clang-format off

extern const char TEXT_I8086_LIST[] PROGMEM;
extern const char TEXT_CPU_8086[]  PROGMEM;
extern const char TEXT_CPU_80186[] PROGMEM;
extern const char TEXT_CPU_80286[] PROGMEM;
extern const char TEXT_CPU_V30[]   PROGMEM;
extern const char TEXT_FPU_8087[]  PROGMEM;
extern const char TEXT_FPU_80287[] PROGMEM;
extern const char TEXT_FPU_80C187[] PROGMEM;
using common::TEXT_none;
using common::TEXT_FPU;

extern const char TEXT_AAA[]    PROGMEM;
extern const char TEXT_AAD[]    PROGMEM;
extern const char TEXT_AAM[]    PROGMEM;
extern const char TEXT_AAS[]    PROGMEM;
using common::TEXT_ADC;
using common::TEXT_ADD;
using common::TEXT_AND;
using common::TEXT_CALL;
extern const char TEXT_CALLF[]  PROGMEM;
extern const char TEXT_CBW[]    PROGMEM;
using common::TEXT_CLC;
using common::TEXT_CLD;
using common::TEXT_CLI;
using common::TEXT_CMC;
using common::TEXT_CMP;
using common::TEXT_CMPS;
using common::TEXT_CMPSB;
using common::TEXT_CMPSW;
extern const char TEXT_CWD[]    PROGMEM;
using common::TEXT_DAA;
using common::TEXT_DAS;
using common::TEXT_DEC;
using common::TEXT_DIV;
using common::TEXT_HLT;
using common::TEXT_IDIV;
extern const char TEXT_IMUL[]   PROGMEM;
using common::TEXT_IN;
using common::TEXT_INC;
extern const char TEXT_INT[]    PROGMEM;
extern const char TEXT_INTO[]   PROGMEM;
using common::TEXT_IRET;
extern const char TEXT_JA[]     PROGMEM;
extern const char TEXT_JAE[]    PROGMEM;
using common::TEXT_JB;
extern const char TEXT_JBE[]    PROGMEM;
using common::TEXT_JC;
extern const char TEXT_JCXZ[]   PROGMEM;
using common::TEXT_JE;
using common::TEXT_JG;
using common::TEXT_JGE;
using common::TEXT_JL;
using common::TEXT_JLE;
using common::TEXT_JMP;
extern const char TEXT_JMPF[]   PROGMEM;
extern const char TEXT_JNA[]    PROGMEM;
extern const char TEXT_JNAE[]   PROGMEM;
using common::TEXT_JNB;
extern const char TEXT_JNBE[]   PROGMEM;
using common::TEXT_JNC;
using common::TEXT_JNE;
extern const char TEXT_JNG[]    PROGMEM;
extern const char TEXT_JNGE[]   PROGMEM;
extern const char TEXT_JNL[]    PROGMEM;
extern const char TEXT_JNLE[]   PROGMEM;
using common::TEXT_JNO;
extern const char TEXT_JNP[]    PROGMEM;
extern const char TEXT_JNS[]    PROGMEM;
using common::TEXT_JNZ;
extern const char TEXT_JO[]     PROGMEM;
using common::TEXT_JP;
using common::TEXT_JPE;
using common::TEXT_JPO;
extern const char TEXT_JS[]     PROGMEM;
using common::TEXT_JZ;
extern const char TEXT_LAHF[]   PROGMEM;
using common::TEXT_LDS;
using common::TEXT_LEA;
extern const char TEXT_LES[]    PROGMEM;
extern const char TEXT_LOCK[]   PROGMEM;
extern const char TEXT_LODS[]   PROGMEM;
extern const char TEXT_LODSB[]  PROGMEM;
extern const char TEXT_LODSW[]  PROGMEM;
extern const char TEXT_LOOP[]   PROGMEM;
extern const char TEXT_LOOPE[]  PROGMEM;
extern const char TEXT_LOOPNE[] PROGMEM;
extern const char TEXT_LOOPNZ[] PROGMEM;
extern const char TEXT_LOOPZ[]  PROGMEM;
using common::TEXT_MOV;
extern const char TEXT_MOVS[]   PROGMEM;
using common::TEXT_MOVSB;
using common::TEXT_MOVSW;
using common::TEXT_MUL;
using common::TEXT_NEG;
using common::TEXT_NOP;
using common::TEXT_NOT;
using common::TEXT_OR;
using common::TEXT_OUT;
using common::TEXT_POP;
using common::TEXT_POPF;
using common::TEXT_PUSH;
using common::TEXT_PUSHF;
extern const char TEXT_RCL[]    PROGMEM;
extern const char TEXT_RCR[]    PROGMEM;
using common::TEXT_REP;
extern const char TEXT_REPE[]   PROGMEM;
extern const char TEXT_REPNE[]  PROGMEM;
extern const char TEXT_REPNZ[]  PROGMEM;
extern const char TEXT_REPZ[]   PROGMEM;
using common::TEXT_RET;
extern const char TEXT_RETF[]   PROGMEM;
using common::TEXT_ROL;
using common::TEXT_ROR;
extern const char TEXT_SAHF[]   PROGMEM;
extern const char TEXT_SAL[]    PROGMEM;
using common::TEXT_SAR;
using common::TEXT_SBB;
extern const char TEXT_SCAS[]   PROGMEM;
extern const char TEXT_SCASB[]  PROGMEM;
extern const char TEXT_SCASW[]  PROGMEM;
extern const char TEXT_SEGCS[]  PROGMEM;
extern const char TEXT_SEGDS[]  PROGMEM;
extern const char TEXT_SEGES[]  PROGMEM;
extern const char TEXT_SEGSS[]  PROGMEM;
using common::TEXT_SHL;
using common::TEXT_SHR;
using common::TEXT_STC;
using common::TEXT_STD;
extern const char TEXT_STI[]    PROGMEM;
extern const char TEXT_STOS[]   PROGMEM;
extern const char TEXT_STOSB[]  PROGMEM;
extern const char TEXT_STOSW[]  PROGMEM;
using common::TEXT_SUB;
using common::TEXT_TEST;
using common::TEXT_WAIT;
using common::TEXT_XCHG;
extern const char TEXT_XLAT[]   PROGMEM;
using common::TEXT_XOR;

// GNU assembler compatibility
using common::TEXT_LCALL;
using common::TEXT_LJMP;
extern const char TEXT_LRET[] PROGMEM;

// i80186
extern const char TEXT_BOUND[] PROGMEM;
using common::TEXT_ENTER;
using common::TEXT_INS;
using common::TEXT_INSB;
using common::TEXT_INSW;
extern const char TEXT_LEAVE[] PROGMEM;
using common::TEXT_OUTS;
extern const char TEXT_OUTSB[] PROGMEM;
extern const char TEXT_OUTSW[] PROGMEM;
using common::TEXT_POPA;
using common::TEXT_PUSHA;

// i80286
extern const char TEXT_ARPL[] PROGMEM;
extern const char TEXT_CLTS[] PROGMEM;
using common::TEXT_LAR;
extern const char TEXT_LGDT[] PROGMEM;
extern const char TEXT_LIDT[] PROGMEM;
extern const char TEXT_LLDT[] PROGMEM;
extern const char TEXT_LMSW[] PROGMEM;
using common::TEXT_LSL;
extern const char TEXT_LTR[]  PROGMEM;
extern const char TEXT_SGDT[] PROGMEM;
extern const char TEXT_SIDT[] PROGMEM;
extern const char TEXT_SLDT[] PROGMEM;
extern const char TEXT_SMSW[] PROGMEM;
using common::TEXT_STR;
extern const char TEXT_VERR[] PROGMEM;
extern const char TEXT_VERW[] PROGMEM;

// V30
extern const char TEXT_ADD4S[] PROGMEM;
extern const char TEXT_BRKEM[] PROGMEM;
extern const char TEXT_CLR1[]  PROGMEM;
extern const char TEXT_CMP4S[] PROGMEM;
using common::TEXT_EXT;
extern const char TEXT_NOT1[]  PROGMEM;
extern const char TEXT_REPC[]  PROGMEM;
extern const char TEXT_REPNC[] PROGMEM;
extern const char TEXT_ROL4[]  PROGMEM;
extern const char TEXT_ROR4[]  PROGMEM;
extern const char TEXT_SET1[]  PROGMEM;
extern const char TEXT_SUB4S[] PROGMEM;
extern const char TEXT_TEST1[] PROGMEM;

// i8087
extern const char TEXT_F2XM1[]   PROGMEM;
using common::TEXT_FABS;
using common::TEXT_FADD;
extern const char TEXT_FADDP[]   PROGMEM;
extern const char TEXT_FBLD[]    PROGMEM;
extern const char TEXT_FBSTP[]   PROGMEM;
extern const char TEXT_FCHS[]    PROGMEM;
extern const char TEXT_FCLEX[]   PROGMEM;
extern const char TEXT_FCOM[]    PROGMEM;
extern const char TEXT_FCOMP[]   PROGMEM;
extern const char TEXT_FCOMPP[]  PROGMEM;
extern const char TEXT_FDECSTP[] PROGMEM;
extern const char TEXT_FDISI[]   PROGMEM;
using common::TEXT_FDIV;
extern const char TEXT_FDIVP[]   PROGMEM;
extern const char TEXT_FDIVR[]   PROGMEM;
extern const char TEXT_FDIVRP[]  PROGMEM;
extern const char TEXT_FENI[]    PROGMEM;
extern const char TEXT_FFREE[]   PROGMEM;
extern const char TEXT_FIADD[]   PROGMEM;
extern const char TEXT_FICOM[]   PROGMEM;
extern const char TEXT_FICOMP[]  PROGMEM;
extern const char TEXT_FIDIV[]   PROGMEM;
extern const char TEXT_FIDIVR[]  PROGMEM;
extern const char TEXT_FILD[]    PROGMEM;
extern const char TEXT_FIMUL[]   PROGMEM;
extern const char TEXT_FINCSTP[] PROGMEM;
extern const char TEXT_FINIT[]   PROGMEM;
extern const char TEXT_FIST[]    PROGMEM;
extern const char TEXT_FISTP[]   PROGMEM;
extern const char TEXT_FISUB[]   PROGMEM;
extern const char TEXT_FISUBR[]  PROGMEM;
extern const char TEXT_FLD[]     PROGMEM;
extern const char TEXT_FLD1[]    PROGMEM;
extern const char TEXT_FLDCW[]   PROGMEM;
extern const char TEXT_FLDENV[]  PROGMEM;
extern const char TEXT_FLDL2E[]  PROGMEM;
extern const char TEXT_FLDL2T[]  PROGMEM;
extern const char TEXT_FLDLG2[]  PROGMEM;
extern const char TEXT_FLDLN2[]  PROGMEM;
extern const char TEXT_FLDPI[]   PROGMEM;
extern const char TEXT_FLDZ[]    PROGMEM;
using common::TEXT_FMUL;
extern const char TEXT_FMULP[]   PROGMEM;
extern const char TEXT_FNCLEX[]  PROGMEM;
extern const char TEXT_FNDISI[]  PROGMEM;
extern const char TEXT_FNENI[]   PROGMEM;
extern const char TEXT_FNINIT[]  PROGMEM;
using common::TEXT_FNOP;
extern const char TEXT_FNSAVE[]  PROGMEM;
extern const char TEXT_FNSTCW[]  PROGMEM;
extern const char TEXT_FNSTENV[] PROGMEM;
extern const char TEXT_FNSTSW[]  PROGMEM;
extern const char TEXT_FPATAN[]  PROGMEM;
extern const char TEXT_FPREM[]   PROGMEM;
extern const char TEXT_FPTAN[]   PROGMEM;
extern const char TEXT_FRNDINT[] PROGMEM;
extern const char TEXT_FRSTOR[]  PROGMEM;
using common::TEXT_FSAVE;
using common::TEXT_FSCALE;
using common::TEXT_FSQRT;
using common::TEXT_FST;
extern const char TEXT_FSTCW[]   PROGMEM;
extern const char TEXT_FSTENV[]  PROGMEM;
extern const char TEXT_FSTP[]    PROGMEM;
extern const char TEXT_FSTSW[]   PROGMEM;
using common::TEXT_FSUB;
extern const char TEXT_FSUBP[]   PROGMEM;
extern const char TEXT_FSUBR[]   PROGMEM;
extern const char TEXT_FSUBRP[]  PROGMEM;
using common::TEXT_FTST;
extern const char TEXT_FWAIT[]   PROGMEM;
extern const char TEXT_FXAM[]    PROGMEM;
extern const char TEXT_FXCH[]    PROGMEM;
extern const char TEXT_FXTRACT[] PROGMEM;
extern const char TEXT_FYL2X[]   PROGMEM;
extern const char TEXT_FYL2XP1[] PROGMEM;

// i80287
extern const char TEXT_FNSETPM[] PROGMEM;

// i80C187
using common::TEXT_FCOS;
extern const char TEXT_FPREM1[]  PROGMEM;
using common::TEXT_FSIN;
using common::TEXT_FSINCOS;
extern const char TEXT_FUCOM[]   PROGMEM;
extern const char TEXT_FUCOMP[]  PROGMEM;
extern const char TEXT_FUCOMPP[] PROGMEM;

extern const char TEXT_REG_AH[] PROGMEM;
extern const char TEXT_REG_AL[] PROGMEM;
extern const char TEXT_REG_AX[] PROGMEM;
extern const char TEXT_REG_BH[] PROGMEM;
extern const char TEXT_REG_BL[] PROGMEM;
extern const char TEXT_REG_BP[] PROGMEM;
extern const char TEXT_REG_BX[] PROGMEM;
extern const char TEXT_REG_CH[] PROGMEM;
extern const char TEXT_REG_CL[] PROGMEM;
extern const char TEXT_REG_CS[] PROGMEM;
extern const char TEXT_REG_CX[] PROGMEM;
extern const char TEXT_REG_DH[] PROGMEM;
extern const char TEXT_REG_DI[] PROGMEM;
extern const char TEXT_REG_DL[] PROGMEM;
extern const char TEXT_REG_DS[] PROGMEM;
extern const char TEXT_REG_DX[] PROGMEM;
extern const char TEXT_REG_ES[] PROGMEM;
extern const char TEXT_REG_SI[] PROGMEM;
using common::TEXT_REG_SP;
extern const char TEXT_REG_SS[] PROGMEM;
using common::TEXT_REG_ST;

extern const char TEXT_PRE_BYTE[]  PROGMEM;
extern const char TEXT_PRE_DWORD[] PROGMEM;
extern const char TEXT_PRE_PTR[]   PROGMEM;
extern const char TEXT_PRE_QWORD[] PROGMEM;
extern const char TEXT_PRE_TBYTE[] PROGMEM;
extern const char TEXT_PRE_WORD[]  PROGMEM;

// clang-format on
}  // namespace i8086
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_I8086__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
