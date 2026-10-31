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

constexpr char TEXT_I8086_LIST[] =
        "i8086, i80186, i80286, i80386, i80486, V30, V33, V35";
constexpr char TEXT_CPU_8086[]   = "8086";
constexpr char TEXT_CPU_80186[]  = "80186";
constexpr char TEXT_CPU_80286[]  = "80286";
constexpr char TEXT_CPU_80386[]  = "80386";
constexpr char TEXT_CPU_80486[]  = "80486";
constexpr char TEXT_CPU_V30[]    = "V30";
constexpr char TEXT_CPU_V33[]    = "V33";
constexpr char TEXT_CPU_V35[]    = "V35";
constexpr char TEXT_FPU_8087[]   = "8087";
constexpr char TEXT_FPU_80287[]  = "80287";
constexpr char TEXT_FPU_80387[]  = "80387";
constexpr char TEXT_FPU_80487[]  = "80487";
constexpr char TEXT_FPU_80C187[] = "80C187";
constexpr char TEXT_FWAIT_AUTO[]  = "auto";
constexpr char TEXT_FWAIT_ASM86[] = "asm86";
constexpr char OPT_TEXT_FWAIT[]   = "fwait";
constexpr char OPT_DESC_FWAIT[]   = "8087 WAIT before instructions: none (default), auto or asm86";

constexpr char TEXT_AAA[]    = "AAA";
constexpr char TEXT_AAD[]    = "AAD";
constexpr char TEXT_AAM[]    = "AAM";
constexpr char TEXT_AAS[]    = "AAS";
// constexpr char TEXT_ADC[]   = "ADC";
// constexpr char TEXT_ADD[]   = "ADD";
// constexpr char TEXT_AND[]   = "AND";
// constexpr char TEXT_CALL[]  = "CALL";
constexpr char TEXT_CALLF[]  = "CALLF";
constexpr char TEXT_CBW[]    = "CBW";
// constexpr char TEXT_CLC[]   = "CLC";
// constexpr char TEXT_CLD[]   = "CLD";
// constexpr char TEXT_CLI[]   = "CLI";
// constexpr char TEXT_CMC[]   = "CMC";
// constexpr char TEXT_CMP[]   = "CMP";
// constexpr char TEXT_CMPS[]  = "CMPS";
// constexpr char TEXT_CMPSB[] = "CMPSB";
// constexpr char TEXT_CMPSW[] = "CMPSW";
constexpr char TEXT_CWD[]    = "CWD";
// constexpr char TEXT_DAA[]   = "DAA";
// constexpr char TEXT_DAS[]   = "DAS";
// constexpr char TEXT_DEC[]   = "DEC";
// constexpr char TEXT_DIV[]   = "DIV";
// constexpr char TEXT_HLT[]   = "HLT";
// constexpr char TEXT_IDIV[]  = "IDIV";
constexpr char TEXT_IMUL[]   = "IMUL";
// constexpr char TEXT_IN[]    = "IN";
// constexpr char TEXT_INC[]   = "INC";
constexpr char TEXT_INT[]    = "INT";
constexpr char TEXT_INTO[]   = "INTO";
// constexpr char TEXT_IRET[]  = "IRET";
constexpr char TEXT_JA[]     = "JA";
constexpr char TEXT_JAE[]    = "JAE";
// constexpr char TEXT_JB[]    = "JB";
constexpr char TEXT_JBE[]    = "JBE";
// constexpr char TEXT_JC[]    = "JC";
constexpr char TEXT_JCXZ[]   = "JCXZ";
// constexpr char TEXT_JE[]    = "JE";
// constexpr char TEXT_JG[]    = "JG";
// constexpr char TEXT_JGE[]   = "JGE";
// constexpr char TEXT_JL[]    = "JL";
// constexpr char TEXT_JLE[]   = "JLE";
// constexpr char TEXT_JMP[]   = "JMP";
constexpr char TEXT_JMPF[]   = "JMPF";
constexpr char TEXT_JNA[]    = "JNA";
constexpr char TEXT_JNAE[]   = "JNAE";
// constexpr char TEXT_JNB[]   = "JNB";
constexpr char TEXT_JNBE[]   = "JNBE";
// constexpr char TEXT_JNC[]   = "JNC";
// constexpr char TEXT_JNE[]   = "JNE";
constexpr char TEXT_JNG[]    = "JNG";
constexpr char TEXT_JNGE[]   = "JNGE";
constexpr char TEXT_JNL[]    = "JNL";
constexpr char TEXT_JNLE[]   = "JNLE";
// constexpr char TEXT_JNO[]   = "JNO";
constexpr char TEXT_JNP[]    = "JNP";
constexpr char TEXT_JNS[]    = "JNS";
// constexpr char TEXT_JNZ[]   = "JNZ";
constexpr char TEXT_JO[]     = "JO";
// constexpr char TEXT_JP[]    = "JP";
// constexpr char TEXT_JPE[]   = "JPE";
// constexpr char TEXT_JPO[]   = "JPO";
constexpr char TEXT_JS[]     = "JS";
// constexpr char TEXT_JZ[]    = "JZ";
constexpr char TEXT_LAHF[]   = "LAHF";
// constexpr char TEXT_LDS[]   = "LDS";
// constexpr char TEXT_LEA[]   = "LEA";
constexpr char TEXT_LES[]    = "LES";
constexpr char TEXT_LOCK[]   = "LOCK";
constexpr char TEXT_LODS[]   = "LODS";
constexpr char TEXT_LODSB[]  = "LODSB";
constexpr char TEXT_LODSW[]  = "LODSW";
constexpr char TEXT_LOOP[]   = "LOOP";
constexpr char TEXT_LOOPE[]  = "LOOPE";
constexpr char TEXT_LOOPNE[] = "LOOPNE";
constexpr char TEXT_LOOPNZ[] = "LOOPNZ";
constexpr char TEXT_LOOPZ[]  = "LOOPZ";
// constexpr char TEXT_MOV[]   = "MOV";
constexpr char TEXT_MOVS[]   = "MOVS";
// constexpr char TEXT_MOVSB[] = "MOVSB";
// constexpr char TEXT_MOVSW[] = "MOVSW";
// constexpr char TEXT_MUL[]   = "MUL";
// constexpr char TEXT_NEG[]   = "NEG";
// constexpr char TEXT_NOP[]   = "NOP";
// constexpr char TEXT_NOT[]   = "NOT";
// constexpr char TEXT_OR[]    = "OR";
// constexpr char TEXT_OUT[]   = "OUT";
// constexpr char TEXT_POP[]   = "POP";
// constexpr char TEXT_POPF[]  = "POPF";
// constexpr char TEXT_PUSH[]  = "PUSH";
// constexpr char TEXT_PUSHF[] = "PUSHF";
constexpr char TEXT_RCL[]    = "RCL";
constexpr char TEXT_RCR[]    = "RCR";
// constexpr char TEXT_REP[]   = "REP";
constexpr char TEXT_REPE[]   = "REPE";
constexpr char TEXT_REPNE[]  = "REPNE";
constexpr char TEXT_REPNZ[]  = "REPNZ";
constexpr char TEXT_REPZ[]   = "REPZ";
// constexpr char TEXT_RET[]   = "RET";
constexpr char TEXT_RETF[]   = "RETF";
// constexpr char TEXT_ROL[]   = "ROL";
// constexpr char TEXT_ROR[]   = "ROR";
constexpr char TEXT_SAHF[]   = "SAHF";
constexpr char TEXT_SAL[]    = "SAL";
// constexpr char TEXT_SAR[]   = "SAR";
// constexpr char TEXT_SBB[]   = "SBB";
constexpr char TEXT_SCAS[]   = "SCAS";
constexpr char TEXT_SCASB[]  = "SCASB";
constexpr char TEXT_SCASW[]  = "SCASW";
constexpr char TEXT_SEGCS[]  = "SEGCS";
constexpr char TEXT_SEGDS[]  = "SEGDS";
constexpr char TEXT_SEGES[]  = "SEGES";
constexpr char TEXT_SEGSS[]  = "SEGSS";
// constexpr char TEXT_SHL[]   = "SHL";
// constexpr char TEXT_SHR[]   = "SHR";
// constexpr char TEXT_STC[]   = "STC";
// constexpr char TEXT_STD[]   = "STD";
// constexpr char TEXT_STI[]   = "STI";
constexpr char TEXT_STOS[]   = "STOS";
constexpr char TEXT_STOSB[]  = "STOSB";
constexpr char TEXT_STOSW[]  = "STOSW";
// constexpr char TEXT_SUB[]   = "SUB";
// constexpr char TEXT_TEST[]  = "TEST";
// constexpr char TEXT_WAIT[]  = "WAIT";
// constexpr char TEXT_XCHG[]  = "XCHG";
constexpr char TEXT_XLAT[]   = "XLAT";
constexpr char TEXT_XLATB[]  = "XLATB";
// constexpr char TEXT_XOR[]   = "XOR";

// GNU assembler compatibility
// constexpr char TEXT_LCALL[] = "LCALL";
// constexpr char TEXT_LJMP[]  = "LJMP";
constexpr char TEXT_LRET[] = "LRET";

// i80186
constexpr char TEXT_BOUND[] = "BOUND";
// constexpr char TEXT_ENTER[] = "ENTER";
// constexpr char TEXT_INS[]   = "INS";
// constexpr char TEXT_INSB[]  = "INSB";
// constexpr char TEXT_INSW[]  = "INSW";
constexpr char TEXT_LEAVE[] = "LEAVE";
// constexpr char TEXT_OUTS[]  = "OUTS";
constexpr char TEXT_OUTSB[] = "OUTSB";
constexpr char TEXT_OUTSW[] = "OUTSW";
// constexpr char TEXT_POPA[]  = "POPA";
// constexpr char TEXT_PUSHA[] = "PUSHA";

// i80286
constexpr char TEXT_ARPL[] = "ARPL";
constexpr char TEXT_CLTS[] = "CLTS";
// constexpr char TEXT_LAR[] = "LAR";
constexpr char TEXT_LGDT[] = "LGDT";
constexpr char TEXT_LIDT[] = "LIDT";
constexpr char TEXT_LLDT[] = "LLDT";
constexpr char TEXT_LMSW[] = "LMSW";
// constexpr char TEXT_LSL[] = "LSL";
constexpr char TEXT_LTR[]  = "LTR";
constexpr char TEXT_SGDT[] = "SGDT";
constexpr char TEXT_SIDT[] = "SIDT";
constexpr char TEXT_SLDT[] = "SLDT";
constexpr char TEXT_SMSW[] = "SMSW";
// constexpr char TEXT_STR[] = "STR";
constexpr char TEXT_VERR[] = "VERR";
constexpr char TEXT_VERW[] = "VERW";

// i80386
constexpr char TEXT_BSF[]    = "BSF";
// constexpr char TEXT_BSR[]   = "BSR";
// constexpr char TEXT_BT[]    = "BT";
constexpr char TEXT_BTC[]    = "BTC";
constexpr char TEXT_BTR[]    = "BTR";
constexpr char TEXT_BTS[]    = "BTS";
constexpr char TEXT_CDQ[]    = "CDQ";
// constexpr char TEXT_CMPSD[] = "CMPSD";
constexpr char TEXT_CWDE[]   = "CWDE";
// constexpr char TEXT_INSD[]  = "INSD";
constexpr char TEXT_IRETD[]  = "IRETD";
constexpr char TEXT_JECXZ[]  = "JECXZ";
constexpr char TEXT_LFS[]    = "LFS";
constexpr char TEXT_LGS[]    = "LGS";
constexpr char TEXT_LODSD[]  = "LODSD";
constexpr char TEXT_LSS[]    = "LSS";
// constexpr char TEXT_MOVSD[] = "MOVSD";
constexpr char TEXT_MOVSX[]  = "MOVSX";
constexpr char TEXT_MOVZX[]  = "MOVZX";
constexpr char TEXT_OUTSD[]  = "OUTSD";
constexpr char TEXT_POPAD[]  = "POPAD";
constexpr char TEXT_POPFD[]  = "POPFD";
constexpr char TEXT_PUSHAD[] = "PUSHAD";
constexpr char TEXT_PUSHD[]  = "PUSHD";
constexpr char TEXT_PUSHFD[] = "PUSHFD";
constexpr char TEXT_SCASD[]  = "SCASD";
constexpr char TEXT_SEGFS[]  = "SEGFS";
constexpr char TEXT_SEGGS[]  = "SEGGS";
constexpr char TEXT_SETA[]   = "SETA";
constexpr char TEXT_SETAE[]  = "SETAE";
// constexpr char TEXT_SETB[]  = "SETB";
constexpr char TEXT_SETBE[]  = "SETBE";
// constexpr char TEXT_SETC[]   = "SETC";
constexpr char TEXT_SETE[]   = "SETE";
constexpr char TEXT_SETG[]   = "SETG";
constexpr char TEXT_SETGE[]  = "SETGE";
// constexpr char TEXT_SETL[]  = "SETL";
constexpr char TEXT_SETLE[]  = "SETLE";
constexpr char TEXT_SETNA[]  = "SETNA";
constexpr char TEXT_SETNAE[] = "SETNAE";
constexpr char TEXT_SETNB[]  = "SETNB";
constexpr char TEXT_SETNBE[] = "SETNBE";
constexpr char TEXT_SETNC[]  = "SETNC";
constexpr char TEXT_SETNE[]  = "SETNE";
constexpr char TEXT_SETNG[]  = "SETNG";
constexpr char TEXT_SETNGE[] = "SETNGE";
constexpr char TEXT_SETNL[]  = "SETNL";
constexpr char TEXT_SETNLE[] = "SETNLE";
constexpr char TEXT_SETNO[]  = "SETNO";
constexpr char TEXT_SETNP[]  = "SETNP";
constexpr char TEXT_SETNS[]  = "SETNS";
constexpr char TEXT_SETNZ[]  = "SETNZ";
// constexpr char TEXT_SETO[]  = "SETO";
constexpr char TEXT_SETP[]   = "SETP";
constexpr char TEXT_SETPE[]  = "SETPE";
constexpr char TEXT_SETPO[]  = "SETPO";
// constexpr char TEXT_SETS[]  = "SETS";
constexpr char TEXT_SETZ[]   = "SETZ";
// constexpr char TEXT_SHLD[]  = "SHLD";
constexpr char TEXT_SHRD[]   = "SHRD";
constexpr char TEXT_STOSD[]  = "STOSD";

// i80486
constexpr char TEXT_BSWAP[]    = "BSWAP";
constexpr char TEXT_CMPXCHG[]  = "CMPXCHG";
constexpr char TEXT_INVD[]     = "INVD";
constexpr char TEXT_INVLPG[]   = "INVLPG";
constexpr char TEXT_WBINVD[]   = "WBINVD";
constexpr char TEXT_XADD[]     = "XADD";

// V series
constexpr char TEXT_ADD4S[] = "ADD4S";
constexpr char TEXT_CLR1[]  = "CLR1";
constexpr char TEXT_CMP4S[] = "CMP4S";
// constexpr char TEXT_EXT[] = "EXT";
constexpr char TEXT_NOT1[]  = "NOT1";
constexpr char TEXT_REPC[]  = "REPC";
constexpr char TEXT_REPNC[] = "REPNC";
constexpr char TEXT_ROL4[]  = "ROL4";
constexpr char TEXT_ROR4[]  = "ROR4";
constexpr char TEXT_SET1[]  = "SET1";
constexpr char TEXT_SUB4S[] = "SUB4S";
constexpr char TEXT_TEST1[] = "TEST1";

// V30
constexpr char TEXT_BRKEM[] = "BRKEM";

// V33
constexpr char TEXT_BRKXA[] = "BRKXA";
constexpr char TEXT_RETXA[] = "RETXA";

// V35
constexpr char TEXT_BRKCS[]  = "BRKCS";
constexpr char TEXT_BTCLR[]  = "BTCLR";
constexpr char TEXT_FINT[]   = "FINT";
constexpr char TEXT_MOVSPA[] = "MOVSPA";
constexpr char TEXT_MOVSPB[] = "MOVSPB";
constexpr char TEXT_RETRBI[] = "RETRBI";
// constexpr char TEXT_STOP[] = "STOP";
constexpr char TEXT_TSKSW[]  = "TSKSW";

// i8087
constexpr char TEXT_F2XM1[]    = "F2XM1";
// constexpr char TEXT_FABS[]   = "FABS";
// constexpr char TEXT_FADD[]   = "FADD";
constexpr char TEXT_FADDP[]    = "FADDP";
constexpr char TEXT_FBLD[]     = "FBLD";
constexpr char TEXT_FBSTP[]    = "FBSTP";
constexpr char TEXT_FCHS[]     = "FCHS";
constexpr char TEXT_FCLEX[]    = "FCLEX";
constexpr char TEXT_FCOM[]     = "FCOM";
constexpr char TEXT_FCOMP[]    = "FCOMP";
constexpr char TEXT_FCOMPP[]   = "FCOMPP";
constexpr char TEXT_FDECSTP[]  = "FDECSTP";
constexpr char TEXT_FDISI[]    = "FDISI";
// constexpr char TEXT_FDIV[]   = "FDIV";
constexpr char TEXT_FDIVP[]    = "FDIVP";
constexpr char TEXT_FDIVR[]    = "FDIVR";
constexpr char TEXT_FDIVRP[]   = "FDIVRP";
constexpr char TEXT_FENI[]     = "FENI";
constexpr char TEXT_FFREE[]    = "FFREE";
constexpr char TEXT_FIADD[]    = "FIADD";
constexpr char TEXT_FICOM[]    = "FICOM";
constexpr char TEXT_FICOMP[]   = "FICOMP";
constexpr char TEXT_FIDIV[]    = "FIDIV";
constexpr char TEXT_FIDIVR[]   = "FIDIVR";
constexpr char TEXT_FILD[]     = "FILD";
constexpr char TEXT_FIMUL[]    = "FIMUL";
constexpr char TEXT_FINCSTP[]  = "FINCSTP";
constexpr char TEXT_FINIT[]    = "FINIT";
constexpr char TEXT_FIST[]     = "FIST";
constexpr char TEXT_FISTP[]    = "FISTP";
constexpr char TEXT_FISUB[]    = "FISUB";
constexpr char TEXT_FISUBR[]   = "FISUBR";
constexpr char TEXT_FLD[]      = "FLD";
constexpr char TEXT_FLD1[]     = "FLD1";
constexpr char TEXT_FLDCW[]    = "FLDCW";
constexpr char TEXT_FLDENV[]   = "FLDENV";
constexpr char TEXT_FLDL2E[]   = "FLDL2E";
constexpr char TEXT_FLDL2T[]   = "FLDL2T";
constexpr char TEXT_FLDLG2[]   = "FLDLG2";
constexpr char TEXT_FLDLN2[]   = "FLDLN2";
constexpr char TEXT_FLDPI[]    = "FLDPI";
constexpr char TEXT_FLDZ[]     = "FLDZ";
// constexpr char TEXT_FMUL[]   = "FMUL";
constexpr char TEXT_FMULP[]    = "FMULP";
constexpr char TEXT_FN2XM1[]   = "FN2XM1";
constexpr char TEXT_FNABS[]    = "FNABS";
constexpr char TEXT_FNADD[]    = "FNADD";
constexpr char TEXT_FNADDP[]   = "FNADDP";
constexpr char TEXT_FNBLD[]    = "FNBLD";
constexpr char TEXT_FNBSTP[]   = "FNBSTP";
constexpr char TEXT_FNCHS[]    = "FNCHS";
constexpr char TEXT_FNCLEX[]   = "FNCLEX";
constexpr char TEXT_FNCOM[]    = "FNCOM";
constexpr char TEXT_FNCOMP[]   = "FNCOMP";
constexpr char TEXT_FNCOMPP[]  = "FNCOMPP";
constexpr char TEXT_FNDECSTP[] = "FNDECSTP";
constexpr char TEXT_FNDISI[]   = "FNDISI";
constexpr char TEXT_FNDIV[]    = "FNDIV";
constexpr char TEXT_FNDIVP[]   = "FNDIVP";
constexpr char TEXT_FNDIVR[]   = "FNDIVR";
constexpr char TEXT_FNDIVRP[]  = "FNDIVRP";
constexpr char TEXT_FNENI[]    = "FNENI";
constexpr char TEXT_FNFREE[]   = "FNFREE";
constexpr char TEXT_FNIADD[]   = "FNIADD";
constexpr char TEXT_FNICOM[]   = "FNICOM";
constexpr char TEXT_FNICOMP[]  = "FNICOMP";
constexpr char TEXT_FNIDIV[]   = "FNIDIV";
constexpr char TEXT_FNIDIVR[]  = "FNIDIVR";
constexpr char TEXT_FNILD[]    = "FNILD";
constexpr char TEXT_FNIMUL[]   = "FNIMUL";
constexpr char TEXT_FNINCSTP[] = "FNINCSTP";
constexpr char TEXT_FNINIT[]   = "FNINIT";
constexpr char TEXT_FNIST[]    = "FNIST";
constexpr char TEXT_FNISTP[]   = "FNISTP";
constexpr char TEXT_FNISUB[]   = "FNISUB";
constexpr char TEXT_FNISUBR[]  = "FNISUBR";
constexpr char TEXT_FNLD[]     = "FNLD";
constexpr char TEXT_FNLD1[]    = "FNLD1";
constexpr char TEXT_FNLDCW[]   = "FNLDCW";
constexpr char TEXT_FNLDENV[]  = "FNLDENV";
constexpr char TEXT_FNLDL2E[]  = "FNLDL2E";
constexpr char TEXT_FNLDL2T[]  = "FNLDL2T";
constexpr char TEXT_FNLDLG2[]  = "FNLDLG2";
constexpr char TEXT_FNLDLN2[]  = "FNLDLN2";
constexpr char TEXT_FNLDPI[]   = "FNLDPI";
constexpr char TEXT_FNLDZ[]    = "FNLDZ";
constexpr char TEXT_FNMUL[]    = "FNMUL";
constexpr char TEXT_FNMULP[]   = "FNMULP";
constexpr char TEXT_FNNOP[]    = "FNNOP";
// constexpr char TEXT_FNOP[]   = "FNOP";
constexpr char TEXT_FNPATAN[]  = "FNPATAN";
constexpr char TEXT_FNPREM[]   = "FNPREM";
constexpr char TEXT_FNPTAN[]   = "FNPTAN";
constexpr char TEXT_FNRNDINT[] = "FNRNDINT";
constexpr char TEXT_FNRSTOR[]  = "FNRSTOR";
constexpr char TEXT_FNSAVE[]   = "FNSAVE";
constexpr char TEXT_FNSCALE[]  = "FNSCALE";
constexpr char TEXT_FNSQRT[]   = "FNSQRT";
constexpr char TEXT_FNST[]     = "FNST";
constexpr char TEXT_FNSTCW[]   = "FNSTCW";
constexpr char TEXT_FNSTENV[]  = "FNSTENV";
constexpr char TEXT_FNSTP[]    = "FNSTP";
constexpr char TEXT_FNSTSW[]   = "FNSTSW";
constexpr char TEXT_FNSUB[]    = "FNSUB";
constexpr char TEXT_FNSUBP[]   = "FNSUBP";
constexpr char TEXT_FNSUBR[]   = "FNSUBR";
constexpr char TEXT_FNSUBRP[]  = "FNSUBRP";
constexpr char TEXT_FNTST[]    = "FNTST";
constexpr char TEXT_FNXAM[]    = "FNXAM";
constexpr char TEXT_FNXCH[]    = "FNXCH";
constexpr char TEXT_FNXTRACT[] = "FNXTRACT";
constexpr char TEXT_FNYL2X[]   = "FNYL2X";
constexpr char TEXT_FNYL2XP1[] = "FNYL2XP1";
constexpr char TEXT_FPATAN[]   = "FPATAN";
constexpr char TEXT_FPREM[]    = "FPREM";
constexpr char TEXT_FPTAN[]    = "FPTAN";
constexpr char TEXT_FRNDINT[]  = "FRNDINT";
constexpr char TEXT_FRSTOR[]   = "FRSTOR";
// constexpr char TEXT_FSAVE[]  = "FSAVE";
// constexpr char TEXT_FSCALE[] = "FSCALE";
// constexpr char TEXT_FSQRT[]  = "FSQRT";
// constexpr char TEXT_FST[]    = "FST";
constexpr char TEXT_FSTCW[]    = "FSTCW";
constexpr char TEXT_FSTENV[]   = "FSTENV";
constexpr char TEXT_FSTP[]     = "FSTP";
constexpr char TEXT_FSTSW[]    = "FSTSW";
// constexpr char TEXT_FSUB[]   = "FSUB";
constexpr char TEXT_FSUBP[]    = "FSUBP";
constexpr char TEXT_FSUBR[]    = "FSUBR";
constexpr char TEXT_FSUBRP[]   = "FSUBRP";
// constexpr char TEXT_FTST[]   = "FTST";
constexpr char TEXT_FWAIT[]    = "FWAIT";
constexpr char TEXT_FXAM[]     = "FXAM";
constexpr char TEXT_FXCH[]     = "FXCH";
constexpr char TEXT_FXTRACT[]  = "FXTRACT";
constexpr char TEXT_FYL2X[]    = "FYL2X";
constexpr char TEXT_FYL2XP1[]  = "FYL2XP1";

// i80287
constexpr char TEXT_FNSETPM[] = "FNSETPM";
constexpr char TEXT_FSETPM[]  = "FSETPM";

// i80387/i80C187
// constexpr char TEXT_FCOS[]    = "FCOS";
constexpr char TEXT_FPREM1[]    = "FPREM1";
// constexpr char TEXT_FSIN[]    = "FSIN";
// constexpr char TEXT_FSINCOS[] = "FSINCOS";
constexpr char TEXT_FUCOM[]     = "FUCOM";
constexpr char TEXT_FUCOMP[]    = "FUCOMP";
constexpr char TEXT_FUCOMPP[]   = "FUCOMPP";


// i8086
constexpr char TEXT_REG_AH[]    = "AH";
constexpr char TEXT_REG_AL[]    = "AL";
constexpr char TEXT_REG_AX[]    = "AX";
constexpr char TEXT_REG_BH[]    = "BH";
constexpr char TEXT_REG_BL[]    = "BL";
constexpr char TEXT_REG_BP[]    = "BP";
constexpr char TEXT_REG_BX[]    = "BX";
constexpr char TEXT_REG_CH[]    = "CH";
constexpr char TEXT_REG_CL[]    = "CL";
constexpr char TEXT_REG_CS[]    = "CS";
constexpr char TEXT_REG_CX[]    = "CX";
constexpr char TEXT_REG_DH[]    = "DH";
constexpr char TEXT_REG_DI[]    = "DI";
constexpr char TEXT_REG_DL[]    = "DL";
constexpr char TEXT_REG_DS[]    = "DS";
constexpr char TEXT_REG_DX[]    = "DX";
constexpr char TEXT_REG_ES[]    = "ES";
constexpr char TEXT_REG_SI[]    = "SI";
// constexpr char TEXT_REG_SP[]  = "SP";
constexpr char TEXT_REG_SS[]    = "SS";

constexpr char TEXT_PRE_BYTE[]  = "BYTE";
constexpr char TEXT_PRE_DWORD[] = "DWORD";
constexpr char TEXT_PRE_PTR[]   = "PTR";
constexpr char TEXT_PRE_QWORD[] = "QWORD";
constexpr char TEXT_PRE_TBYTE[] = "TBYTE";
constexpr char TEXT_PRE_WORD[]  = "WORD";

// i80386
constexpr char TEXT_REG_CR[]  = "CR";
constexpr char TEXT_REG_DR[]  = "DR";
constexpr char TEXT_REG_EAX[] = "EAX";
constexpr char TEXT_REG_EBP[] = "EBP";
constexpr char TEXT_REG_EBX[] = "EBX";
constexpr char TEXT_REG_ECX[] = "ECX";
constexpr char TEXT_REG_EDI[] = "EDI";
constexpr char TEXT_REG_EDX[] = "EDX";
constexpr char TEXT_REG_ESI[] = "ESI";
constexpr char TEXT_REG_ESP[] = "ESP";
constexpr char TEXT_REG_FS[]  = "FS";
constexpr char TEXT_REG_GS[]  = "GS";
constexpr char TEXT_REG_TR[]  = "TR";

// 80386 prefix
constexpr char TEXT_PRE_ADDR16[] = "ADDR16";
constexpr char TEXT_PRE_ADDR32[] = "ADDR32";
constexpr char TEXT_PRE_DATA16[] = "DATA16";
constexpr char TEXT_PRE_DATA32[] = "DATA32";
constexpr char TEXT_PRE_FAR[]    = "FAR";

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
