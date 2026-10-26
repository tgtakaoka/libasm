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

extern const char TEXT_I8086_LIST[];
extern const char TEXT_CPU_8086[];
extern const char TEXT_CPU_80186[];
extern const char TEXT_CPU_80286[];
extern const char TEXT_CPU_80386[];
extern const char TEXT_CPU_80486[];
extern const char TEXT_CPU_V30[];
extern const char TEXT_CPU_V33[];
extern const char TEXT_CPU_V35[];
extern const char TEXT_FPU_8087[];
extern const char TEXT_FPU_80287[];
extern const char TEXT_FPU_80387[];
extern const char TEXT_FPU_80487[];
extern const char TEXT_FPU_80C187[];
using common::TEXT_none;
using common::TEXT_FPU;

extern const char TEXT_AAA[];
extern const char TEXT_AAD[];
extern const char TEXT_AAM[];
extern const char TEXT_AAS[];
using common::TEXT_ADC;
using common::TEXT_ADD;
using common::TEXT_AND;
using common::TEXT_CALL;
extern const char TEXT_CALLF[];
extern const char TEXT_CBW[];
using common::TEXT_CLC;
using common::TEXT_CLD;
using common::TEXT_CLI;
using common::TEXT_CMC;
using common::TEXT_CMP;
using common::TEXT_CMPS;
using common::TEXT_CMPSB;
using common::TEXT_CMPSW;
extern const char TEXT_CWD[];
using common::TEXT_DAA;
using common::TEXT_DAS;
using common::TEXT_DEC;
using common::TEXT_DIV;
using common::TEXT_HLT;
using common::TEXT_IDIV;
extern const char TEXT_IMUL[];
using common::TEXT_IN;
using common::TEXT_INC;
extern const char TEXT_INT[];
extern const char TEXT_INTO[];
using common::TEXT_IRET;
extern const char TEXT_JA[];
extern const char TEXT_JAE[];
using common::TEXT_JB;
extern const char TEXT_JBE[];
using common::TEXT_JC;
extern const char TEXT_JCXZ[];
using common::TEXT_JE;
using common::TEXT_JG;
using common::TEXT_JGE;
using common::TEXT_JL;
using common::TEXT_JLE;
using common::TEXT_JMP;
extern const char TEXT_JMPF[];
extern const char TEXT_JNA[];
extern const char TEXT_JNAE[];
using common::TEXT_JNB;
extern const char TEXT_JNBE[];
using common::TEXT_JNC;
using common::TEXT_JNE;
extern const char TEXT_JNG[];
extern const char TEXT_JNGE[];
extern const char TEXT_JNL[];
extern const char TEXT_JNLE[];
using common::TEXT_JNO;
extern const char TEXT_JNP[];
extern const char TEXT_JNS[];
using common::TEXT_JNZ;
extern const char TEXT_JO[];
using common::TEXT_JP;
using common::TEXT_JPE;
using common::TEXT_JPO;
extern const char TEXT_JS[];
using common::TEXT_JZ;
extern const char TEXT_LAHF[];
using common::TEXT_LDS;
using common::TEXT_LEA;
extern const char TEXT_LES[];
extern const char TEXT_LOCK[];
extern const char TEXT_LODS[];
extern const char TEXT_LODSB[];
extern const char TEXT_LODSW[];
extern const char TEXT_LOOP[];
extern const char TEXT_LOOPE[];
extern const char TEXT_LOOPNE[];
extern const char TEXT_LOOPNZ[];
extern const char TEXT_LOOPZ[];
using common::TEXT_MOV;
extern const char TEXT_MOVS[];
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
extern const char TEXT_RCL[];
extern const char TEXT_RCR[];
using common::TEXT_REP;
extern const char TEXT_REPE[];
extern const char TEXT_REPNE[];
extern const char TEXT_REPNZ[];
extern const char TEXT_REPZ[];
using common::TEXT_RET;
extern const char TEXT_RETF[];
using common::TEXT_ROL;
using common::TEXT_ROR;
extern const char TEXT_SAHF[];
extern const char TEXT_SAL[];
using common::TEXT_SAR;
using common::TEXT_SBB;
extern const char TEXT_SCAS[];
extern const char TEXT_SCASB[];
extern const char TEXT_SCASW[];
extern const char TEXT_SEGCS[];
extern const char TEXT_SEGDS[];
extern const char TEXT_SEGES[];
extern const char TEXT_SEGSS[];
using common::TEXT_SHL;
using common::TEXT_SHR;
using common::TEXT_STC;
using common::TEXT_STD;
using common::TEXT_STD;
using common::TEXT_STI;
extern const char TEXT_STOS[];
extern const char TEXT_STOSB[];
extern const char TEXT_STOSW[];
using common::TEXT_SUB;
using common::TEXT_TEST;
using common::TEXT_WAIT;
using common::TEXT_XCHG;
extern const char TEXT_XLAT[];
extern const char TEXT_XLATB[];
using common::TEXT_XOR;

// GNU assembler compatibility
using common::TEXT_LCALL;
using common::TEXT_LJMP;
extern const char TEXT_LRET[];

// i80186
extern const char TEXT_BOUND[];
using common::TEXT_ENTER;
using common::TEXT_INS;
using common::TEXT_INSB;
using common::TEXT_INSW;
extern const char TEXT_LEAVE[];
using common::TEXT_OUTS;
extern const char TEXT_OUTSB[];
extern const char TEXT_OUTSW[];
using common::TEXT_POPA;
using common::TEXT_PUSHA;

// i80286
extern const char TEXT_ARPL[];
extern const char TEXT_CLTS[];
using common::TEXT_LAR;
extern const char TEXT_LGDT[];
extern const char TEXT_LIDT[];
extern const char TEXT_LLDT[];
extern const char TEXT_LMSW[];
using common::TEXT_LSL;
extern const char TEXT_LTR[];
extern const char TEXT_SGDT[];
extern const char TEXT_SIDT[];
extern const char TEXT_SLDT[];
extern const char TEXT_SMSW[];
using common::TEXT_STR;
extern const char TEXT_VERR[];
extern const char TEXT_VERW[];

// i80386
extern const char TEXT_BSF[];
using common::TEXT_BSR;
using common::TEXT_BT;
extern const char TEXT_BTC[];
extern const char TEXT_BTR[];
extern const char TEXT_BTS[];
extern const char TEXT_CDQ[];
using common::TEXT_CMPSD;
extern const char TEXT_CWDE[];
using common::TEXT_INSD;
extern const char TEXT_IRETD[];
extern const char TEXT_JECXZ[];
extern const char TEXT_LFS[];
extern const char TEXT_LGS[];
extern const char TEXT_LODSD[];
extern const char TEXT_LSS[];
using common::TEXT_MOVSD;
extern const char TEXT_MOVSX[];
extern const char TEXT_MOVZX[];
extern const char TEXT_OUTSD[];
extern const char TEXT_POPAD[];
extern const char TEXT_POPFD[];
extern const char TEXT_PUSHAD[];
extern const char TEXT_PUSHD[];
extern const char TEXT_PUSHFD[];
extern const char TEXT_SCASD[];
extern const char TEXT_SEGFS[];
extern const char TEXT_SEGGS[];
extern const char TEXT_SETA[];
extern const char TEXT_SETAE[];
using common::TEXT_SETB;
extern const char TEXT_SETBE[];
using common::TEXT_SETC;
extern const char TEXT_SETE[];
extern const char TEXT_SETG[];
extern const char TEXT_SETGE[];
using common::TEXT_SETL;
extern const char TEXT_SETLE[];
extern const char TEXT_SETNA[];
extern const char TEXT_SETNAE[];
extern const char TEXT_SETNB[];
extern const char TEXT_SETNBE[];
extern const char TEXT_SETNC[];
extern const char TEXT_SETNE[];
extern const char TEXT_SETNG[];
extern const char TEXT_SETNGE[];
extern const char TEXT_SETNL[];
extern const char TEXT_SETNLE[];
extern const char TEXT_SETNO[];
extern const char TEXT_SETNP[];
extern const char TEXT_SETNS[];
extern const char TEXT_SETNZ[];
using common::TEXT_SETO;
extern const char TEXT_SETP[];
extern const char TEXT_SETPE[];
extern const char TEXT_SETPO[];
using common::TEXT_SETS;
extern const char TEXT_SETZ[];
using common::TEXT_SHLD;
extern const char TEXT_SHRD[];
extern const char TEXT_STOSD[];

// i80486
extern const char TEXT_BSWAP[];
extern const char TEXT_CMPXCHG[];
extern const char TEXT_INVD[];
extern const char TEXT_INVLPG[];
extern const char TEXT_WBINVD[];
extern const char TEXT_XADD[];

// V series
extern const char TEXT_ADD4S[];
extern const char TEXT_CLR1[];
extern const char TEXT_CMP4S[];
using common::TEXT_EXT;
extern const char TEXT_NOT1[];
extern const char TEXT_REPC[];
extern const char TEXT_REPNC[];
extern const char TEXT_ROL4[];
extern const char TEXT_ROR4[];
extern const char TEXT_SET1[];
extern const char TEXT_SUB4S[];
extern const char TEXT_TEST1[];

// V30
extern const char TEXT_BRKEM[];

// V33
extern const char TEXT_BRKXA[];
extern const char TEXT_RETXA[];

// V35
extern const char TEXT_BRKCS[];
extern const char TEXT_BTCLR[];
extern const char TEXT_FINT[];
extern const char TEXT_MOVSPA[];
extern const char TEXT_MOVSPB[];
extern const char TEXT_RETRBI[];
using common::TEXT_STOP;
extern const char TEXT_TSKSW[];

// i8087
extern const char TEXT_F2XM1[];
using common::TEXT_FABS;
using common::TEXT_FADD;
extern const char TEXT_FADDP[];
extern const char TEXT_FBLD[];
extern const char TEXT_FBSTP[];
extern const char TEXT_FCHS[];
extern const char TEXT_FCLEX[];
extern const char TEXT_FCOM[];
extern const char TEXT_FCOMP[];
extern const char TEXT_FCOMPP[];
extern const char TEXT_FDECSTP[];
extern const char TEXT_FDISI[];
using common::TEXT_FDIV;
extern const char TEXT_FDIVP[];
extern const char TEXT_FDIVR[];
extern const char TEXT_FDIVRP[];
extern const char TEXT_FENI[];
extern const char TEXT_FFREE[];
extern const char TEXT_FIADD[];
extern const char TEXT_FICOM[];
extern const char TEXT_FICOMP[];
extern const char TEXT_FIDIV[];
extern const char TEXT_FIDIVR[];
extern const char TEXT_FILD[];
extern const char TEXT_FIMUL[];
extern const char TEXT_FINCSTP[];
extern const char TEXT_FINIT[];
extern const char TEXT_FIST[];
extern const char TEXT_FISTP[];
extern const char TEXT_FISUB[];
extern const char TEXT_FISUBR[];
extern const char TEXT_FLD[];
extern const char TEXT_FLD1[];
extern const char TEXT_FLDCW[];
extern const char TEXT_FLDENV[];
extern const char TEXT_FLDL2E[];
extern const char TEXT_FLDL2T[];
extern const char TEXT_FLDLG2[];
extern const char TEXT_FLDLN2[];
extern const char TEXT_FLDPI[];
extern const char TEXT_FLDZ[];
using common::TEXT_FMUL;
extern const char TEXT_FMULP[];
extern const char TEXT_FNCLEX[];
extern const char TEXT_FNDISI[];
extern const char TEXT_FNENI[];
extern const char TEXT_FNINIT[];
using common::TEXT_FNOP;
extern const char TEXT_FNSAVE[];
extern const char TEXT_FNSTCW[];
extern const char TEXT_FNSTENV[];
extern const char TEXT_FNSTSW[];
extern const char TEXT_FPATAN[];
extern const char TEXT_FPREM[];
extern const char TEXT_FPTAN[];
extern const char TEXT_FRNDINT[];
extern const char TEXT_FRSTOR[];
using common::TEXT_FSAVE;
using common::TEXT_FSCALE;
using common::TEXT_FSQRT;
using common::TEXT_FST;
extern const char TEXT_FSTCW[];
extern const char TEXT_FSTENV[];
extern const char TEXT_FSTP[];
extern const char TEXT_FSTSW[];
using common::TEXT_FSUB;
extern const char TEXT_FSUBP[];
extern const char TEXT_FSUBR[];
extern const char TEXT_FSUBRP[];
using common::TEXT_FTST;
extern const char TEXT_FWAIT[];
extern const char TEXT_FXAM[];
extern const char TEXT_FXCH[];
extern const char TEXT_FXTRACT[];
extern const char TEXT_FYL2X[];
extern const char TEXT_FYL2XP1[];

// i80287
extern const char TEXT_FNSETPM[];
extern const char TEXT_FSETPM[];

// i80387/i80C187
using common::TEXT_FCOS;
extern const char TEXT_FPREM1[];
using common::TEXT_FSIN;
using common::TEXT_FSINCOS;
extern const char TEXT_FUCOM[];
extern const char TEXT_FUCOMP[];
extern const char TEXT_FUCOMPP[];

// i8086
extern const char TEXT_REG_AH[];
extern const char TEXT_REG_AL[];
extern const char TEXT_REG_AX[];
extern const char TEXT_REG_BH[];
extern const char TEXT_REG_BL[];
extern const char TEXT_REG_BP[];
extern const char TEXT_REG_BX[];
extern const char TEXT_REG_CH[];
extern const char TEXT_REG_CL[];
extern const char TEXT_REG_CS[];
extern const char TEXT_REG_CX[];
extern const char TEXT_REG_DH[];
extern const char TEXT_REG_DI[];
extern const char TEXT_REG_DL[];
extern const char TEXT_REG_DS[];
extern const char TEXT_REG_DX[];
extern const char TEXT_REG_ES[];
extern const char TEXT_REG_SI[];
using common::TEXT_REG_SP;
extern const char TEXT_REG_SS[];
using common::TEXT_REG_ST;

extern const char TEXT_PRE_BYTE[];
extern const char TEXT_PRE_DWORD[];
extern const char TEXT_PRE_PTR[];
extern const char TEXT_PRE_QWORD[];
extern const char TEXT_PRE_TBYTE[];
extern const char TEXT_PRE_WORD[];

// i80386
extern const char TEXT_REG_CR[];
extern const char TEXT_REG_DR[];
extern const char TEXT_REG_EAX[];
extern const char TEXT_REG_EBP[];
extern const char TEXT_REG_EBX[];
extern const char TEXT_REG_ECX[];
extern const char TEXT_REG_EDI[];
extern const char TEXT_REG_EDX[];
extern const char TEXT_REG_ESI[];
extern const char TEXT_REG_ESP[];
extern const char TEXT_REG_FS[];
extern const char TEXT_REG_GS[];
extern const char TEXT_REG_TR[];

// 80386 prefix
extern const char TEXT_PRE_ADDR16[];
extern const char TEXT_PRE_ADDR32[];
extern const char TEXT_PRE_DATA16[];
extern const char TEXT_PRE_DATA32[];
extern const char TEXT_PRE_FAR[];

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
