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

#ifndef __LIBASM_TEXT_COMMON__
#define __LIBASM_TEXT_COMMON__

#include "config_host.h"

namespace libasm {
namespace text {
namespace common {
// clang-format off

extern const char TEXT_CPU_Z80[];
extern const char TEXT_CPU_Z180[];

extern const char TEXT_FPU[];
extern const char TEXT_INF[];
extern const char TEXT_INFINITY[];
extern const char TEXT_NAN[];
extern const char TEXT_PMMU[];
extern const char TEXT_none[];

extern const char TEXT_A[];
extern const char TEXT_ABA[];
extern const char TEXT_ABS[];
extern const char TEXT_ABSD[];
extern const char TEXT_ABSF[];
extern const char TEXT_ABX[];
extern const char TEXT_ABY[];
extern const char TEXT_ADC[];
extern const char TEXT_ADCA[];
extern const char TEXT_ADCB[];
extern const char TEXT_ADCD[];
extern const char TEXT_ADCR[];
extern const char TEXT_ADD[];
extern const char TEXT_ADDA[];
extern const char TEXT_ADDB[];
extern const char TEXT_ADDC[];
extern const char TEXT_ADDCB[];
extern const char TEXT_ADDD[];
extern const char TEXT_ADDE[];
extern const char TEXT_ADDF[];
extern const char TEXT_ADDI[];
extern const char TEXT_ADDL[];
extern const char TEXT_ADDR[];
extern const char TEXT_ADDS[];
extern const char TEXT_ADDW[];
extern const char TEXT_ADDX[];
extern const char TEXT_ADE[];
extern const char TEXT_ADI[];
extern const char TEXT_ADX[];
extern const char TEXT_AI[];
extern const char TEXT_AIM[];
extern const char TEXT_AIS[];
extern const char TEXT_AIX[];
extern const char TEXT_AM[];
extern const char TEXT_AND[];
extern const char TEXT_ANDA[];
extern const char TEXT_ANDB[];
extern const char TEXT_ANDC[];
extern const char TEXT_ANDCC[];
extern const char TEXT_ANDD[];
extern const char TEXT_ANDI[];
extern const char TEXT_ANDP[];
extern const char TEXT_ANDR[];
extern const char TEXT_ANDW[];
extern const char TEXT_ANI[];
extern const char TEXT_ANL[];
extern const char TEXT_ASH[];
extern const char TEXT_ASL[];
extern const char TEXT_ASLA[];
extern const char TEXT_ASLB[];
extern const char TEXT_ASLD[];
extern const char TEXT_ASLW[];
extern const char TEXT_ASLX[];
extern const char TEXT_ASR[];
extern const char TEXT_ASRA[];
extern const char TEXT_ASRB[];
extern const char TEXT_ASRD[];
extern const char TEXT_ASRW[];
extern const char TEXT_ASRX[];
extern const char TEXT_B[];
extern const char TEXT_BAND[];
extern const char TEXT_BC[];
extern const char TEXT_BCC[];
extern const char TEXT_BCLR[];
extern const char TEXT_BCS[];
extern const char TEXT_BD[];
extern const char TEXT_BEQ[];
extern const char TEXT_BF[];
extern const char TEXT_BGE[];
extern const char TEXT_BGND[];
extern const char TEXT_BGT[];
extern const char TEXT_BHI[];
extern const char TEXT_BHS[];
extern const char TEXT_BIAND[];
extern const char TEXT_BICB[];
extern const char TEXT_BIOR[];
extern const char TEXT_BIT[];
extern const char TEXT_BITA[];
extern const char TEXT_BITB[];
extern const char TEXT_BL[];
extern const char TEXT_BLD[];
extern const char TEXT_BLE[];
extern const char TEXT_BLO[];
extern const char TEXT_BLS[];
extern const char TEXT_BLT[];
extern const char TEXT_BM[];
extern const char TEXT_BMI[];
extern const char TEXT_BMOV[];
extern const char TEXT_BNC[];
extern const char TEXT_BND[];
extern const char TEXT_BNE[];
extern const char TEXT_BNOT[];
extern const char TEXT_BNV[];
extern const char TEXT_BNZ[];
extern const char TEXT_BOR[];
extern const char TEXT_BP[];
extern const char TEXT_BPL[];
extern const char TEXT_BPT[];
extern const char TEXT_BR[];
extern const char TEXT_BRA[];
extern const char TEXT_BRCLR[];
extern const char TEXT_BRN[];
extern const char TEXT_BRSET[];
extern const char TEXT_BSET[];
extern const char TEXT_BSR[];
extern const char TEXT_BST[];
extern const char TEXT_BT[];
extern const char TEXT_BTJO[];
extern const char TEXT_BTJZ[];
extern const char TEXT_BTST[];
extern const char TEXT_BV[];
extern const char TEXT_BVC[];
extern const char TEXT_BVS[];
extern const char TEXT_BXOR[];
extern const char TEXT_BZ[];
extern const char TEXT_C[];
extern const char TEXT_CALL[];
extern const char TEXT_CALLI[];
extern const char TEXT_CALLN[];
extern const char TEXT_CALLR[];
extern const char TEXT_CALLS[];
extern const char TEXT_CALR[];
extern const char TEXT_CAS[];
extern const char TEXT_CB[];
extern const char TEXT_CBA[];
extern const char TEXT_CC[];
extern const char TEXT_CCF[];
extern const char TEXT_CI[];
extern const char TEXT_CLC[];
extern const char TEXT_CLD[];
extern const char TEXT_CLI[];
extern const char TEXT_CLR[];
extern const char TEXT_CLRA[];
extern const char TEXT_CLRB[];
extern const char TEXT_CLRC[];
extern const char TEXT_CLRD[];
extern const char TEXT_CLRE[];
extern const char TEXT_CLRF[];
extern const char TEXT_CLRMAC[];
extern const char TEXT_CLRW[];
extern const char TEXT_CLRX[];
extern const char TEXT_CLV[];
extern const char TEXT_CM[];
extern const char TEXT_CMA[];
extern const char TEXT_CMC[];
extern const char TEXT_CMP[];
extern const char TEXT_CMPA[];
extern const char TEXT_CMPB[];
extern const char TEXT_CMPD[];
extern const char TEXT_CMPF[];
extern const char TEXT_CMPI[];
extern const char TEXT_CMPL[];
extern const char TEXT_CMPR[];
extern const char TEXT_CMPS[];
extern const char TEXT_CMPSB[];
extern const char TEXT_CMPSD[];
extern const char TEXT_CMPSW[];
extern const char TEXT_CMPW[];
extern const char TEXT_COM[];
extern const char TEXT_COMA[];
extern const char TEXT_COMB[];
extern const char TEXT_COMD[];
extern const char TEXT_COME[];
extern const char TEXT_COMR[];
extern const char TEXT_COMW[];
extern const char TEXT_COMX[];
extern const char TEXT_CP[];
extern const char TEXT_CPD[];
extern const char TEXT_CPDR[];
extern const char TEXT_CPE[];
extern const char TEXT_CPI[];
extern const char TEXT_CPIR[];
extern const char TEXT_CPL[];
extern const char TEXT_CPS[];
extern const char TEXT_CPU[];
extern const char TEXT_CPW[];
extern const char TEXT_CPX[];
extern const char TEXT_CPY[];
extern const char TEXT_DA[];
extern const char TEXT_DAA[];
extern const char TEXT_DAC[];
extern const char TEXT_DAD[];
extern const char TEXT_DADD[];
extern const char TEXT_DAS[];
extern const char TEXT_DBEQ[];
extern const char TEXT_DBGE[];
extern const char TEXT_DBGT[];
extern const char TEXT_DBHI[];
extern const char TEXT_DBHS[];
extern const char TEXT_DBLE[];
extern const char TEXT_DBLO[];
extern const char TEXT_DBLS[];
extern const char TEXT_DBLT[];
extern const char TEXT_DBNE[];
extern const char TEXT_DBNZ[];
extern const char TEXT_DEC[];
extern const char TEXT_DECA[];
extern const char TEXT_DECB[];
extern const char TEXT_DECD[];
extern const char TEXT_DECF[];
extern const char TEXT_DECW[];
extern const char TEXT_DECX[];
extern const char TEXT_DES[];
extern const char TEXT_DEX[];
extern const char TEXT_DEY[];
extern const char TEXT_DI[];
extern const char TEXT_DINT[];
extern const char TEXT_DIS[];
extern const char TEXT_DIV[];
extern const char TEXT_DIVB[];
extern const char TEXT_DIVD[];
extern const char TEXT_DIVF[];
extern const char TEXT_DIVL[];
extern const char TEXT_DIVS[];
extern const char TEXT_DIVU[];
extern const char TEXT_DIVUW[];
extern const char TEXT_DIVW[];
extern const char TEXT_DIVXU[];
extern const char TEXT_DJNZ[];
extern const char TEXT_DLD[];
extern const char TEXT_EDIV[];
extern const char TEXT_EDIVS[];
extern const char TEXT_EI[];
extern const char TEXT_EIM[];
extern const char TEXT_EINT[];
extern const char TEXT_EMUL[];
extern const char TEXT_EMULS[];
extern const char TEXT_ENTER[];
extern const char TEXT_EOR[];
extern const char TEXT_EORA[];
extern const char TEXT_EORB[];
extern const char TEXT_EORD[];
extern const char TEXT_EORI[];
extern const char TEXT_EORR[];
extern const char TEXT_EX[];
extern const char TEXT_EXG[];
extern const char TEXT_EXIT[];
extern const char TEXT_EXT[];
extern const char TEXT_EXTB[];
extern const char TEXT_EXTS[];
extern const char TEXT_EXTSB[];
extern const char TEXT_EXTSW[];
extern const char TEXT_EXTU[];
extern const char TEXT_EXX[];
extern const char TEXT_FABS[];
extern const char TEXT_FADD[];
extern const char TEXT_FCOS[];
extern const char TEXT_FDIV[];
extern const char TEXT_FIX[];
extern const char TEXT_FLOAT[];
extern const char TEXT_FLT[];
extern const char TEXT_FMUL[];
extern const char TEXT_FNEG[];
extern const char TEXT_FNOP[];
extern const char TEXT_FSAVE[];
extern const char TEXT_FSCALE[];
extern const char TEXT_FSIN[];
extern const char TEXT_FSINCOS[];
extern const char TEXT_FSQRT[];
extern const char TEXT_FST[];
extern const char TEXT_FSUB[];
extern const char TEXT_FTST[];
extern const char TEXT_HALT[];
extern const char TEXT_HLT[];
extern const char TEXT_IDIV[];
extern const char TEXT_IDL[];
extern const char TEXT_IDLE[];
extern const char TEXT_IDLE2[];
extern const char TEXT_ILD[];
extern const char TEXT_IM[];
extern const char TEXT_IN[];
extern const char TEXT_IN0[];
extern const char TEXT_INC[];
extern const char TEXT_INCA[];
extern const char TEXT_INCB[];
extern const char TEXT_INCF[];
extern const char TEXT_INCW[];
extern const char TEXT_INCX[];
extern const char TEXT_IND[];
extern const char TEXT_INDR[];
extern const char TEXT_INDRW[];
extern const char TEXT_INDW[];
extern const char TEXT_INI[];
extern const char TEXT_INIR[];
extern const char TEXT_INIRW[];
extern const char TEXT_INIW[];
extern const char TEXT_INS[];
extern const char TEXT_INSB[];
extern const char TEXT_INSD[];
extern const char TEXT_INSW[];
extern const char TEXT_INV[];
extern const char TEXT_INW[];
extern const char TEXT_INX[];
extern const char TEXT_INY[];
extern const char TEXT_IOT[];
extern const char TEXT_IRET[];
extern const char TEXT_JB[];
extern const char TEXT_JBC[];
extern const char TEXT_JC[];
extern const char TEXT_JE[];
extern const char TEXT_JEQ[];
extern const char TEXT_JG[];
extern const char TEXT_JGE[];
extern const char TEXT_JGT[];
extern const char TEXT_JH[];
extern const char TEXT_JHS[];
extern const char TEXT_JL[];
extern const char TEXT_JLE[];
extern const char TEXT_JLT[];
extern const char TEXT_JMP[];
extern const char TEXT_JMPI[];
extern const char TEXT_JMPL[];
extern const char TEXT_JN[];
extern const char TEXT_JNB[];
extern const char TEXT_JNC[];
extern const char TEXT_JNE[];
extern const char TEXT_JNO[];
extern const char TEXT_JNV[];
extern const char TEXT_JNZ[];
extern const char TEXT_JP[];
extern const char TEXT_JPE[];
extern const char TEXT_JPO[];
extern const char TEXT_JPZ[];
extern const char TEXT_JR[];
extern const char TEXT_JRL[];
extern const char TEXT_JSR[];
extern const char TEXT_JV[];
extern const char TEXT_JZ[];
extern const char TEXT_LAR[];
extern const char TEXT_LBCC[];
extern const char TEXT_LBCS[];
extern const char TEXT_LBEQ[];
extern const char TEXT_LBGE[];
extern const char TEXT_LBGT[];
extern const char TEXT_LBHI[];
extern const char TEXT_LBHS[];
extern const char TEXT_LBLE[];
extern const char TEXT_LBLO[];
extern const char TEXT_LBLS[];
extern const char TEXT_LBLT[];
extern const char TEXT_LBMI[];
extern const char TEXT_LBNE[];
extern const char TEXT_LBPL[];
extern const char TEXT_LBRA[];
extern const char TEXT_LBRN[];
extern const char TEXT_LBSR[];
extern const char TEXT_LBVC[];
extern const char TEXT_LBVS[];
extern const char TEXT_LCALL[];
extern const char TEXT_LD[];
extern const char TEXT_LDA[];
extern const char TEXT_LDAA[];
extern const char TEXT_LDAB[];
extern const char TEXT_LDAR[];
extern const char TEXT_LDB[];
extern const char TEXT_LDC[];
extern const char TEXT_LDCTL[];
extern const char TEXT_LDD[];
extern const char TEXT_LDDR[];
extern const char TEXT_LDE[];
extern const char TEXT_LDED[];
extern const char TEXT_LDF[];
extern const char TEXT_LDI[];
extern const char TEXT_LDIR[];
extern const char TEXT_LDM[];
extern const char TEXT_LDP[];
extern const char TEXT_LDS[];
extern const char TEXT_LDSP[];
extern const char TEXT_LDW[];
extern const char TEXT_LDX[];
extern const char TEXT_LDY[];
extern const char TEXT_LEA[];
extern const char TEXT_LEAS[];
extern const char TEXT_LEAX[];
extern const char TEXT_LEAY[];
extern const char TEXT_LI[];
extern const char TEXT_LINK[];
extern const char TEXT_LJMP[];
extern const char TEXT_LR[];
extern const char TEXT_LSL[];
extern const char TEXT_LSLA[];
extern const char TEXT_LSLB[];
extern const char TEXT_LSLD[];
extern const char TEXT_LSLW[];
extern const char TEXT_LSLX[];
extern const char TEXT_LSR[];
extern const char TEXT_LSRA[];
extern const char TEXT_LSRB[];
extern const char TEXT_LSRD[];
extern const char TEXT_LSRW[];
extern const char TEXT_LSRX[];
extern const char TEXT_LST[];
extern const char TEXT_MAC[];
extern const char TEXT_MARK[];
extern const char TEXT_MAXMODE[];
extern const char TEXT_MLT[];
extern const char TEXT_MODD[];
extern const char TEXT_MOV[];
extern const char TEXT_MOVB[];
extern const char TEXT_MOVD[];
extern const char TEXT_MOVFPE[];
extern const char TEXT_MOVP[];
extern const char TEXT_MOVSB[];
extern const char TEXT_MOVSD[];
extern const char TEXT_MOVSW[];
extern const char TEXT_MOVTPE[];
extern const char TEXT_MOVW[];
extern const char TEXT_MOVX[];
extern const char TEXT_MPY[];
extern const char TEXT_MPYS[];
extern const char TEXT_MUL[];
extern const char TEXT_MULB[];
extern const char TEXT_MULD[];
extern const char TEXT_MULF[];
extern const char TEXT_MULS[];
extern const char TEXT_MULT[];
extern const char TEXT_MULTUW[];
extern const char TEXT_MULTW[];
extern const char TEXT_MULU[];
extern const char TEXT_MULXU[];
extern const char TEXT_MVI[];
extern const char TEXT_NEG[];
extern const char TEXT_NEGA[];
extern const char TEXT_NEGB[];
extern const char TEXT_NEGD[];
extern const char TEXT_NEGF[];
extern const char TEXT_NEGR[];
extern const char TEXT_NEGW[];
extern const char TEXT_NEGX[];
extern const char TEXT_NOP[];
extern const char TEXT_NORM[];
extern const char TEXT_NOT[];
extern const char TEXT_NOTB[];
extern const char TEXT_OIM[];
extern const char TEXT_OR[];
extern const char TEXT_ORA[];
extern const char TEXT_ORAA[];
extern const char TEXT_ORAB[];
extern const char TEXT_ORB[];
extern const char TEXT_ORC[];
extern const char TEXT_ORCC[];
extern const char TEXT_ORD[];
extern const char TEXT_ORE[];
extern const char TEXT_ORI[];
extern const char TEXT_ORL[];
extern const char TEXT_ORP[];
extern const char TEXT_ORR[];
extern const char TEXT_ORW[];
extern const char TEXT_OTDM[];
extern const char TEXT_OTDMR[];
extern const char TEXT_OTDR[];
extern const char TEXT_OTDRW[];
extern const char TEXT_OTIM[];
extern const char TEXT_OTIMR[];
extern const char TEXT_OTIR[];
extern const char TEXT_OTIRW[];
extern const char TEXT_OUT[];
extern const char TEXT_OUT0[];
extern const char TEXT_OUTD[];
extern const char TEXT_OUTDW[];
extern const char TEXT_OUTI[];
extern const char TEXT_OUTIW[];
extern const char TEXT_OUTS[];
extern const char TEXT_OUTW[];
extern const char TEXT_PEA[];
extern const char TEXT_POP[];
extern const char TEXT_POPA[];
extern const char TEXT_POPF[];
extern const char TEXT_PSHA[];
extern const char TEXT_PSHB[];
extern const char TEXT_PSHD[];
extern const char TEXT_PSHM[];
extern const char TEXT_PSHX[];
extern const char TEXT_PSHY[];
extern const char TEXT_PULA[];
extern const char TEXT_PULB[];
extern const char TEXT_PULD[];
extern const char TEXT_PULX[];
extern const char TEXT_PULY[];
extern const char TEXT_PUSH[];
extern const char TEXT_PUSHA[];
extern const char TEXT_PUSHF[];
extern const char TEXT_RAL[];
extern const char TEXT_RAR[];
extern const char TEXT_RC[];
extern const char TEXT_RCF[];
extern const char TEXT_REP[];
extern const char TEXT_RER[];
extern const char TEXT_RES[];
extern const char TEXT_RESET[];
extern const char TEXT_RET[];
extern const char TEXT_RETC[];
extern const char TEXT_RETD[];
extern const char TEXT_RETE[];
extern const char TEXT_RETEM[];
extern const char TEXT_RETI[];
extern const char TEXT_RETN[];
extern const char TEXT_RETS[];
extern const char TEXT_RL[];
extern const char TEXT_RLA[];
extern const char TEXT_RLC[];
extern const char TEXT_RLCA[];
extern const char TEXT_RLD[];
extern const char TEXT_ROL[];
extern const char TEXT_ROLA[];
extern const char TEXT_ROLB[];
extern const char TEXT_ROLD[];
extern const char TEXT_ROLW[];
extern const char TEXT_ROLX[];
extern const char TEXT_ROR[];
extern const char TEXT_RORA[];
extern const char TEXT_RORB[];
extern const char TEXT_RORD[];
extern const char TEXT_RORW[];
extern const char TEXT_RORX[];
extern const char TEXT_ROTL[];
extern const char TEXT_ROTR[];
extern const char TEXT_ROTXL[];
extern const char TEXT_ROTXR[];
extern const char TEXT_RPTB[];
extern const char TEXT_RR[];
extern const char TEXT_RRA[];
extern const char TEXT_RRC[];
extern const char TEXT_RRCA[];
extern const char TEXT_RRD[];
extern const char TEXT_RRL[];
extern const char TEXT_RST[];
extern const char TEXT_RTC[];
extern const char TEXT_RTD[];
extern const char TEXT_RTE[];
extern const char TEXT_RTI[];
extern const char TEXT_RTL[];
extern const char TEXT_RTR[];
extern const char TEXT_RTS[];
extern const char TEXT_S[];
extern const char TEXT_SAR[];
extern const char TEXT_SBA[];
extern const char TEXT_SBB[];
extern const char TEXT_SBC[];
extern const char TEXT_SBCA[];
extern const char TEXT_SBCB[];
extern const char TEXT_SBCD[];
extern const char TEXT_SC[];
extern const char TEXT_SCC[];
extern const char TEXT_SCF[];
extern const char TEXT_SD[];
extern const char TEXT_SEC[];
extern const char TEXT_SEI[];
extern const char TEXT_SEP[];
extern const char TEXT_SEQ[];
extern const char TEXT_SET[];
extern const char TEXT_SETB[];
extern const char TEXT_SETC[];
extern const char TEXT_SETL[];
extern const char TEXT_SETO[];
extern const char TEXT_SETS[];
extern const char TEXT_SEV[];
extern const char TEXT_SEX[];
extern const char TEXT_SGT[];
extern const char TEXT_SHAL[];
extern const char TEXT_SHAR[];
extern const char TEXT_SHL[];
extern const char TEXT_SHLD[];
extern const char TEXT_SHLL[];
extern const char TEXT_SHLR[];
extern const char TEXT_SHR[];
extern const char TEXT_SIN[];
extern const char TEXT_SJMP[];
extern const char TEXT_SKP[];
extern const char TEXT_SL[];
extern const char TEXT_SLA[];
extern const char TEXT_SLEEP[];
extern const char TEXT_SLL[];
extern const char TEXT_SLP[];
extern const char TEXT_SM[];
extern const char TEXT_SMI[];
extern const char TEXT_SPL[];
extern const char TEXT_SR[];
extern const char TEXT_SRA[];
extern const char TEXT_SRL[];
extern const char TEXT_ST[];
extern const char TEXT_STA[];
extern const char TEXT_STAA[];
extern const char TEXT_STAB[];
extern const char TEXT_STB[];
extern const char TEXT_STC[];
extern const char TEXT_STD[];
extern const char TEXT_STE[];
extern const char TEXT_STF[];
extern const char TEXT_STI[];
extern const char TEXT_STM[];
extern const char TEXT_STOP[];
extern const char TEXT_STR[];
extern const char TEXT_STS[];
extern const char TEXT_STSP[];
extern const char TEXT_STST[];
extern const char TEXT_STX[];
extern const char TEXT_STY[];
extern const char TEXT_STZ[];
extern const char TEXT_SUB[];
extern const char TEXT_SUBA[];
extern const char TEXT_SUBB[];
extern const char TEXT_SUBC[];
extern const char TEXT_SUBCB[];
extern const char TEXT_SUBD[];
extern const char TEXT_SUBE[];
extern const char TEXT_SUBF[];
extern const char TEXT_SUBI[];
extern const char TEXT_SUBL[];
extern const char TEXT_SUBR[];
extern const char TEXT_SUBS[];
extern const char TEXT_SUBW[];
extern const char TEXT_SUBX[];
extern const char TEXT_SVC[];
extern const char TEXT_SWAP[];
extern const char TEXT_SWI[];
extern const char TEXT_SXT[];
extern const char TEXT_TAB[];
extern const char TEXT_TAP[];
extern const char TEXT_TAS[];
extern const char TEXT_TAX[];
extern const char TEXT_TBA[];
extern const char TEXT_TEST[];
extern const char TEXT_TFR[];
extern const char TEXT_TIM[];
extern const char TEXT_TPA[];
extern const char TEXT_TRAP[];
extern const char TEXT_TRAPA[];
extern const char TEXT_TRAPEQ[];
extern const char TEXT_TRAPGE[];
extern const char TEXT_TRAPGT[];
extern const char TEXT_TRAPHI[];
extern const char TEXT_TRAPHS[];
extern const char TEXT_TRAPLE[];
extern const char TEXT_TRAPLO[];
extern const char TEXT_TRAPLS[];
extern const char TEXT_TRAPLT[];
extern const char TEXT_TRAPNE[];
extern const char TEXT_TRAPV[];
extern const char TEXT_TSET[];
extern const char TEXT_TST[];
extern const char TEXT_TSTA[];
extern const char TEXT_TSTB[];
extern const char TEXT_TSTD[];
extern const char TEXT_TSTE[];
extern const char TEXT_TSTF[];
extern const char TEXT_TSTIO[];
extern const char TEXT_TSTW[];
extern const char TEXT_TSTX[];
extern const char TEXT_TSX[];
extern const char TEXT_TSY[];
extern const char TEXT_TXA[];
extern const char TEXT_TXS[];
extern const char TEXT_TXY[];
extern const char TEXT_TYS[];
extern const char TEXT_TYX[];
extern const char TEXT_UNLK[];
extern const char TEXT_WAI[];
extern const char TEXT_WAIT[];
extern const char TEXT_XCH[];
extern const char TEXT_XCHB[];
extern const char TEXT_XCHD[];
extern const char TEXT_XCHG[];
extern const char TEXT_XGDX[];
extern const char TEXT_XGDY[];
extern const char TEXT_XOR[];
extern const char TEXT_XORB[];
extern const char TEXT_XORC[];
extern const char TEXT_XORW[];
extern const char TEXT_XRI[];
extern const char TEXT_XRL[];
extern const char TEXT_null[];

extern const char TEXT_REG_A[];
extern const char TEXT_REG_A0[];
extern const char TEXT_REG_AC0[];
extern const char TEXT_REG_AC1[];
extern const char TEXT_REG_AF[];
extern const char TEXT_REG_AR[];
extern const char TEXT_REG_B[];
extern const char TEXT_REG_BC[];
extern const char TEXT_REG_C[];
extern const char TEXT_REG_CCR[];
extern const char TEXT_REG_D[];
extern const char TEXT_REG_DE[];
extern const char TEXT_REG_DP[];
extern const char TEXT_REG_DR0[];
extern const char TEXT_REG_DSR[];
extern const char TEXT_REG_E[];
extern const char TEXT_REG_F[];
extern const char TEXT_REG_F0[];
extern const char TEXT_REG_F1[];
extern const char TEXT_REG_FP[];
extern const char TEXT_REG_H[];
extern const char TEXT_REG_HL[];
extern const char TEXT_REG_I[];
extern const char TEXT_REG_IX[];
extern const char TEXT_REG_IXH[];
extern const char TEXT_REG_IXL[];
extern const char TEXT_REG_IY[];
extern const char TEXT_REG_IYH[];
extern const char TEXT_REG_IYL[];
extern const char TEXT_REG_K[];
extern const char TEXT_REG_L[];
extern const char TEXT_REG_MACH[];
extern const char TEXT_REG_MACL[];
extern const char TEXT_REG_MOD[];
extern const char TEXT_REG_NSP[];
extern const char TEXT_REG_P[];
extern const char TEXT_REG_P0[];
extern const char TEXT_REG_P1[];
extern const char TEXT_REG_P2[];
extern const char TEXT_REG_P3[];
extern const char TEXT_REG_PC[];
extern const char TEXT_REG_PSR[];
extern const char TEXT_REG_PSW[];
extern const char TEXT_REG_QL[];
extern const char TEXT_REG_R[];
extern const char TEXT_REG_R0[];
extern const char TEXT_REG_R1[];
extern const char TEXT_REG_R2[];
extern const char TEXT_REG_R3[];
extern const char TEXT_REG_R4[];
extern const char TEXT_REG_R5[];
extern const char TEXT_REG_R6[];
extern const char TEXT_REG_R7[];
extern const char TEXT_REG_RC[];
extern const char TEXT_REG_RE[];
extern const char TEXT_REG_RH[];
extern const char TEXT_REG_RL[];
extern const char TEXT_REG_RR[];
extern const char TEXT_REG_RS[];
extern const char TEXT_REG_S[];
extern const char TEXT_REG_SP[];
extern const char TEXT_REG_SPH[];
extern const char TEXT_REG_SPL[];
extern const char TEXT_REG_SR[];
extern const char TEXT_REG_ST[];
extern const char TEXT_REG_T[];
extern const char TEXT_REG_TC[];
extern const char TEXT_REG_USP[];
extern const char TEXT_REG_VBR[];
extern const char TEXT_REG_W[];
extern const char TEXT_REG_X[];
extern const char TEXT_REG_X0[];
extern const char TEXT_REG_X1[];
extern const char TEXT_REG_Y[];
extern const char TEXT_REG_Z[];

extern const char TEXT_CC_C[];
extern const char TEXT_CC_EQ[];
extern const char TEXT_CC_F[];
extern const char TEXT_CC_GE[];
extern const char TEXT_CC_GT[];
extern const char TEXT_CC_LE[];
extern const char TEXT_CC_LT[];
extern const char TEXT_CC_M[];
extern const char TEXT_CC_MI[];
extern const char TEXT_CC_NC[];
extern const char TEXT_CC_NE[];
extern const char TEXT_CC_NOV[];
extern const char TEXT_CC_NS[];
extern const char TEXT_CC_NV[];
extern const char TEXT_CC_NZ[];
extern const char TEXT_CC_OV[];
extern const char TEXT_CC_P[];
extern const char TEXT_CC_PE[];
extern const char TEXT_CC_PL[];
extern const char TEXT_CC_PO[];
extern const char TEXT_CC_S[];
extern const char TEXT_CC_T[];
extern const char TEXT_CC_UGE[];
extern const char TEXT_CC_UGT[];
extern const char TEXT_CC_ULE[];
extern const char TEXT_CC_ULT[];
extern const char TEXT_CC_V[];
extern const char TEXT_CC_Z[];

extern const char TEXT_FN_ADDR[];
extern const char TEXT_FN_H[];
extern const char TEXT_FN_HIGH[];
extern const char TEXT_FN_L[];
extern const char TEXT_FN_LOW16[];
extern const char TEXT_FN_LOW[];
extern const char TEXT_FN_PAGE[];

extern const char TEXT_ALIGN[];
extern const char TEXT_AORG[];
extern const char TEXT_BSS[];
extern const char TEXT_BYTE[];
extern const char TEXT_DATA[];
extern const char TEXT_DB[];
extern const char TEXT_DC[];
extern const char TEXT_DCB[];
extern const char TEXT_DCL[];
extern const char TEXT_DCW[];
extern const char TEXT_DD[];
extern const char TEXT_DEFB[];
extern const char TEXT_DEFL[];
extern const char TEXT_DEFM[];
extern const char TEXT_DEFS[];
extern const char TEXT_DEFW[];
extern const char TEXT_DL[];
extern const char TEXT_DS[];
extern const char TEXT_DSB[];
extern const char TEXT_DSL[];
extern const char TEXT_DSW[];
extern const char TEXT_DW[];
extern const char TEXT_EVEN[];
extern const char TEXT_FCB[];
extern const char TEXT_FCC[];
extern const char TEXT_FDB[];
extern const char TEXT_OPTION[];
extern const char TEXT_ORG[];
extern const char TEXT_RMB[];
extern const char TEXT_TEXT[];
extern const char TEXT_dADDR[];   // .addr
extern const char TEXT_dALIGN[];  // .align
extern const char TEXT_dASCII[];  // .ascii
extern const char TEXT_dBLKB[];   // .blkb
extern const char TEXT_dBLKW[];   // .blkw
extern const char TEXT_dBSS[];    // .bss
extern const char TEXT_dBYTE[];   // .byte
extern const char TEXT_dDBYTE[];  // .dbyte
extern const char TEXT_dEVEN[];   // .even
extern const char TEXT_dLONG[];   // .long
extern const char TEXT_dORG[];    // .org
extern const char TEXT_dSTRING[]; // .string
extern const char TEXT_dWORD[];   // .word
extern const char TEXT_dequal[];  // .=

extern const char PSTR_AT[];     // "@"
extern const char PSTR_COMMA[];  // ","
extern const char PSTR_DOLLAR[]; // "$"
extern const char PSTR_DOT[];    // "."
extern const char PSTR_SEMI[];   // ";"
extern const char PSTR_SHARP[];  // "#"
extern const char PSTR_STAR[];   // "*"
extern const char PSTR_UNDER[];  // "_"

extern const char PSTR_AT_QUESTION[];    // "@?"
extern const char PSTR_B_DASH[];         // "B'"
extern const char PSTR_DOT_DOLLAR[];     // ".$"
extern const char PSTR_DOT_DOT[];        // ".."
extern const char PSTR_DOT_STAR[];       // ".*"
extern const char PSTR_HAT_B[];          // "^B"
extern const char PSTR_HAT_D[];          // "^D"
extern const char PSTR_H_DASH[];         // "H'"
extern const char PSTR_O_DASH[];         // "O'"
extern const char PSTR_Q_DASH[];         // "Q'"
extern const char PSTR_UNDER_DOLLAR[];   // "_$"
extern const char PSTR_UNDER_DOT[];      // "_."
extern const char PSTR_UNDER_QUESTION[]; // "_?"
extern const char PSTR_X_DASH[];         // "X'"
extern const char PSTR_ZERO_B[];         // "0b"
extern const char PSTR_ZERO_X[];         // "0x"

extern const char PSTR_DOT_STAR_DOLLAR[];   // ".*$"
extern const char PSTR_UNDER_AT_QUESTION[]; // "_@?"
extern const char PSTR_UNDER_DOT_DOLLAR[];  // "_.$"

extern const char PSTR_PERCENT_2[];                 // "%(2)"
extern const char PSTR_PERCENT_8[];                 // "%(8)"
extern const char PSTR_UNDER_DOT_DOLLAR_QUESTION[]; // "_.$?"

// clang-format on
}  // namespace common
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_COMMON__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
