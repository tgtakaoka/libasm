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
extern const char TEXT_FPU[]   PROGMEM;
extern const char TEXT_none[]  PROGMEM;
extern const char TEXT_NAN[]   PROGMEM;
extern const char TEXT_INF[]   PROGMEM;
extern const char TEXT_INFINITY[] PROGMEM;

extern const char TEXT_ABSD[]  PROGMEM;
extern const char TEXT_ABSF[]  PROGMEM;
extern const char TEXT_ABS[]   PROGMEM;
extern const char TEXT_ABX[]   PROGMEM;
extern const char TEXT_ADCA[]  PROGMEM;
extern const char TEXT_ADCB[]  PROGMEM;
extern const char TEXT_ADC[]   PROGMEM;
extern const char TEXT_ADDA[]  PROGMEM;
extern const char TEXT_ADDB[]  PROGMEM;
extern const char TEXT_ADDCB[] PROGMEM;
extern const char TEXT_ADDC[]  PROGMEM;
extern const char TEXT_ADDD[]  PROGMEM;
extern const char TEXT_ADDF[]  PROGMEM;
extern const char TEXT_ADDI[]  PROGMEM;
extern const char TEXT_ADDL[]  PROGMEM;
extern const char TEXT_ADD[]   PROGMEM;
extern const char TEXT_ADDR[]  PROGMEM;
extern const char TEXT_ADDW[]  PROGMEM;
extern const char TEXT_ADI[]   PROGMEM;
extern const char TEXT_AIM[]   PROGMEM;
extern const char TEXT_AI[]    PROGMEM;
extern const char TEXT_AM[]    PROGMEM;
extern const char TEXT_ANDA[]  PROGMEM;
extern const char TEXT_ANDB[]  PROGMEM;
extern const char TEXT_ANDD[]  PROGMEM;
extern const char TEXT_ANDI[]  PROGMEM;
extern const char TEXT_AND[]   PROGMEM;
extern const char TEXT_ANDR[]  PROGMEM;
extern const char TEXT_ANI[]   PROGMEM;
extern const char TEXT_ANL[]   PROGMEM;
extern const char TEXT_A[]     PROGMEM;
extern const char TEXT_ASLA[]  PROGMEM;
extern const char TEXT_ASLB[]  PROGMEM;
extern const char TEXT_ASLD[]  PROGMEM;
extern const char TEXT_ASL[]   PROGMEM;
extern const char TEXT_ASRA[]  PROGMEM;
extern const char TEXT_ASRB[]  PROGMEM;
extern const char TEXT_ASR[]   PROGMEM;
extern const char TEXT_BAND[]  PROGMEM;
extern const char TEXT_BC[]    PROGMEM;
extern const char TEXT_BCC[]   PROGMEM;
extern const char TEXT_BCLR[]  PROGMEM;
extern const char TEXT_BCS[]   PROGMEM;
extern const char TEXT_BEQ[]   PROGMEM;
extern const char TEXT_BGE[]   PROGMEM;
extern const char TEXT_BGT[]   PROGMEM;
extern const char TEXT_BHI[]   PROGMEM;
extern const char TEXT_BHS[]   PROGMEM;
extern const char TEXT_BICB[]  PROGMEM;
extern const char TEXT_BITA[]  PROGMEM;
extern const char TEXT_BITB[]  PROGMEM;
extern const char TEXT_BIT[]   PROGMEM;
extern const char TEXT_BLE[]   PROGMEM;
extern const char TEXT_BLO[]   PROGMEM;
extern const char TEXT_BL[]    PROGMEM;
extern const char TEXT_BLS[]   PROGMEM;
extern const char TEXT_BLT[]   PROGMEM;
extern const char TEXT_BMI[]   PROGMEM;
extern const char TEXT_BM[]    PROGMEM;
extern const char TEXT_BNC[]   PROGMEM;
extern const char TEXT_BNE[]   PROGMEM;
extern const char TEXT_BNZ[]   PROGMEM;
extern const char TEXT_BOR[]   PROGMEM;
extern const char TEXT_BPL[]   PROGMEM;
extern const char TEXT_BP[]    PROGMEM;
extern const char TEXT_B[]     PROGMEM;
extern const char TEXT_BPT[]   PROGMEM;
extern const char TEXT_BRA[]   PROGMEM;
extern const char TEXT_BRCLR[] PROGMEM;
extern const char TEXT_BRN[]   PROGMEM;
extern const char TEXT_BR[]    PROGMEM;
extern const char TEXT_BRSET[] PROGMEM;
extern const char TEXT_BSET[]  PROGMEM;
extern const char TEXT_BSR[]   PROGMEM;
extern const char TEXT_BT[]    PROGMEM;
extern const char TEXT_BTST[]  PROGMEM;
extern const char TEXT_BVC[]   PROGMEM;
extern const char TEXT_BVS[]   PROGMEM;
extern const char TEXT_BZ[]    PROGMEM;
extern const char TEXT_CALLN[] PROGMEM;
extern const char TEXT_CALL[]  PROGMEM;
extern const char TEXT_CALR[]  PROGMEM;
extern const char TEXT_CB[]    PROGMEM;
extern const char TEXT_CCF[]   PROGMEM;
extern const char TEXT_CI[]    PROGMEM;
extern const char TEXT_CLC[]   PROGMEM;
extern const char TEXT_CLD[]   PROGMEM;
extern const char TEXT_CLI[]   PROGMEM;
extern const char TEXT_CLRA[]  PROGMEM;
extern const char TEXT_CLRB[]  PROGMEM;
extern const char TEXT_CLRC[]  PROGMEM;
extern const char TEXT_CLRD[]  PROGMEM;
extern const char TEXT_CLRF[]  PROGMEM;
extern const char TEXT_CLR[]   PROGMEM;
extern const char TEXT_CLV[]   PROGMEM;
extern const char TEXT_CMA[]   PROGMEM;
extern const char TEXT_CMC[]   PROGMEM;
extern const char TEXT_CMPA[]  PROGMEM;
extern const char TEXT_CMPB[]  PROGMEM;
extern const char TEXT_CMPD[]  PROGMEM;
extern const char TEXT_CMPF[]  PROGMEM;
extern const char TEXT_CMPL[]  PROGMEM;
extern const char TEXT_CMP[]   PROGMEM;
extern const char TEXT_CM[]    PROGMEM;
extern const char TEXT_CMPR[]  PROGMEM;
extern const char TEXT_CMPSB[] PROGMEM;
extern const char TEXT_CMPS[]  PROGMEM;
extern const char TEXT_CMPSW[] PROGMEM;
extern const char TEXT_CMPW[]  PROGMEM;
extern const char TEXT_COMA[]  PROGMEM;
extern const char TEXT_COMB[]  PROGMEM;
extern const char TEXT_COMD[]  PROGMEM;
extern const char TEXT_COM[]   PROGMEM;
extern const char TEXT_COMW[]  PROGMEM;
extern const char TEXT_CPD[]   PROGMEM;
extern const char TEXT_CPDR[]  PROGMEM;
extern const char TEXT_CPI[]   PROGMEM;
extern const char TEXT_CPIR[]  PROGMEM;
extern const char TEXT_CPL[]   PROGMEM;
extern const char TEXT_CP[]    PROGMEM;
extern const char TEXT_C[]     PROGMEM;
extern const char TEXT_CPU[]   PROGMEM;
extern const char TEXT_CPX[]   PROGMEM;
extern const char TEXT_CPY[]   PROGMEM;
extern const char TEXT_DAA[]   PROGMEM;
extern const char TEXT_DAD[]   PROGMEM;
extern const char TEXT_DA[]    PROGMEM;
extern const char TEXT_DAS[]   PROGMEM;
extern const char TEXT_DBNZ[]  PROGMEM;
extern const char TEXT_DECA[]  PROGMEM;
extern const char TEXT_DECB[]  PROGMEM;
extern const char TEXT_DECD[]  PROGMEM;
extern const char TEXT_DEC[]   PROGMEM;
extern const char TEXT_DECW[]  PROGMEM;
extern const char TEXT_DECX[]  PROGMEM;
extern const char TEXT_DEX[]   PROGMEM;
extern const char TEXT_DEY[]   PROGMEM;
extern const char TEXT_DINT[]  PROGMEM;
extern const char TEXT_DI[]    PROGMEM;
extern const char TEXT_DIS[]   PROGMEM;
extern const char TEXT_DIVB[]  PROGMEM;
extern const char TEXT_DIVD[]  PROGMEM;
extern const char TEXT_DIVF[]  PROGMEM;
extern const char TEXT_DIVL[]  PROGMEM;
extern const char TEXT_DIV[]   PROGMEM;
extern const char TEXT_DIVS[]  PROGMEM;
extern const char TEXT_DIVU[]  PROGMEM;
extern const char TEXT_DJNZ[]  PROGMEM;
extern const char TEXT_DLD[]   PROGMEM;
extern const char TEXT_EIM[]   PROGMEM;
extern const char TEXT_EINT[]  PROGMEM;
extern const char TEXT_EI[]    PROGMEM;
extern const char TEXT_ENTER[] PROGMEM;
extern const char TEXT_EORA[]  PROGMEM;
extern const char TEXT_EORB[]  PROGMEM;
extern const char TEXT_EORI[]  PROGMEM;
extern const char TEXT_EOR[]   PROGMEM;
extern const char TEXT_EORR[]  PROGMEM;
extern const char TEXT_EXG[]   PROGMEM;
extern const char TEXT_EXIT[]  PROGMEM;
extern const char TEXT_EX[]    PROGMEM;
extern const char TEXT_EXTB[]  PROGMEM;
extern const char TEXT_EXT[]   PROGMEM;
extern const char TEXT_EXTSB[] PROGMEM;
extern const char TEXT_EXX[]   PROGMEM;
extern const char TEXT_FABS[]  PROGMEM;
extern const char TEXT_FADD[]  PROGMEM;
extern const char TEXT_FDIV[]  PROGMEM;
extern const char TEXT_FLT[]   PROGMEM;
extern const char TEXT_FMUL[]  PROGMEM;
extern const char TEXT_FNOP[]  PROGMEM;
extern const char TEXT_FSAVE[] PROGMEM;
extern const char TEXT_FSCALE[] PROGMEM;
extern const char TEXT_FSQRT[] PROGMEM;
extern const char TEXT_FST[]   PROGMEM;
extern const char TEXT_FSUB[]  PROGMEM;
extern const char TEXT_FTST[]  PROGMEM;
extern const char TEXT_HALT[]  PROGMEM;
extern const char TEXT_HLT[]   PROGMEM;
extern const char TEXT_IDIV[]  PROGMEM;
extern const char TEXT_IDLE[]  PROGMEM;
extern const char TEXT_IDL[]   PROGMEM;
extern const char TEXT_ILD[]   PROGMEM;
extern const char TEXT_INCA[]  PROGMEM;
extern const char TEXT_INCB[]  PROGMEM;
extern const char TEXT_INC[]   PROGMEM;
extern const char TEXT_INCW[]  PROGMEM;
extern const char TEXT_INCX[]  PROGMEM;
extern const char TEXT_IND[]   PROGMEM;
extern const char TEXT_INDR[]  PROGMEM;
extern const char TEXT_INI[]   PROGMEM;
extern const char TEXT_INIR[]  PROGMEM;
extern const char TEXT_IN[]    PROGMEM;
extern const char TEXT_INSB[]  PROGMEM;
extern const char TEXT_INS[]   PROGMEM;
extern const char TEXT_INSW[]  PROGMEM;
extern const char TEXT_INV[]   PROGMEM;
extern const char TEXT_INX[]   PROGMEM;
extern const char TEXT_INY[]   PROGMEM;
extern const char TEXT_IOT[]   PROGMEM;
extern const char TEXT_IRET[]  PROGMEM;
extern const char TEXT_JBC[]   PROGMEM;
extern const char TEXT_JB[]    PROGMEM;
extern const char TEXT_JC[]    PROGMEM;
extern const char TEXT_JE[]    PROGMEM;
extern const char TEXT_JEQ[]   PROGMEM;
extern const char TEXT_JGE[]   PROGMEM;
extern const char TEXT_JGT[]   PROGMEM;
extern const char TEXT_JH[]    PROGMEM;
extern const char TEXT_JLE[]   PROGMEM;
extern const char TEXT_JL[]    PROGMEM;
extern const char TEXT_JLT[]   PROGMEM;
extern const char TEXT_JMP[]   PROGMEM;
extern const char TEXT_JNB[]   PROGMEM;
extern const char TEXT_JNC[]   PROGMEM;
extern const char TEXT_JNE[]   PROGMEM;
extern const char TEXT_JNO[]   PROGMEM;
extern const char TEXT_JNZ[]   PROGMEM;
extern const char TEXT_JPE[]   PROGMEM;
extern const char TEXT_JPO[]   PROGMEM;
extern const char TEXT_JP[]    PROGMEM;
extern const char TEXT_JR[]    PROGMEM;
extern const char TEXT_JSR[]   PROGMEM;
extern const char TEXT_JZ[]    PROGMEM;
extern const char TEXT_LBGE[]  PROGMEM;
extern const char TEXT_LCALL[] PROGMEM;
extern const char TEXT_LDA[]   PROGMEM;
extern const char TEXT_LDAR[]  PROGMEM;
extern const char TEXT_LDB[]   PROGMEM;
extern const char TEXT_LDC[]   PROGMEM;
extern const char TEXT_LDD[]   PROGMEM;
extern const char TEXT_LDDR[]  PROGMEM;
extern const char TEXT_LDE[]   PROGMEM;
extern const char TEXT_LDF[]   PROGMEM;
extern const char TEXT_LDI[]   PROGMEM;
extern const char TEXT_LDIR[]  PROGMEM;
extern const char TEXT_LD[]    PROGMEM;
extern const char TEXT_LDS[]   PROGMEM;
extern const char TEXT_LDW[]   PROGMEM;
extern const char TEXT_LDX[]   PROGMEM;
extern const char TEXT_LDY[]   PROGMEM;
extern const char TEXT_LEA[]   PROGMEM;
extern const char TEXT_LI[]    PROGMEM;
extern const char TEXT_LJMP[]  PROGMEM;
extern const char TEXT_LR[]    PROGMEM;
extern const char TEXT_LSLA[]  PROGMEM;
extern const char TEXT_LSLB[]  PROGMEM;
extern const char TEXT_LSLD[]  PROGMEM;
extern const char TEXT_LSL[]   PROGMEM;
extern const char TEXT_LSRA[]  PROGMEM;
extern const char TEXT_LSRB[]  PROGMEM;
extern const char TEXT_LSRD[]  PROGMEM;
extern const char TEXT_LSR[]   PROGMEM;
extern const char TEXT_LST[]   PROGMEM;
extern const char TEXT_MARK[]  PROGMEM;
extern const char TEXT_MODD[]  PROGMEM;
extern const char TEXT_MOVB[]  PROGMEM;
extern const char TEXT_MOVD[]  PROGMEM;
extern const char TEXT_MOVP[]  PROGMEM;
extern const char TEXT_MOV[]   PROGMEM;
extern const char TEXT_MOVSB[] PROGMEM;
extern const char TEXT_MOVSW[] PROGMEM;
extern const char TEXT_MOVX[]  PROGMEM;
extern const char TEXT_MPY[]   PROGMEM;
extern const char TEXT_MPYS[]  PROGMEM;
extern const char TEXT_MULB[]  PROGMEM;
extern const char TEXT_MULD[]  PROGMEM;
extern const char TEXT_MULF[]  PROGMEM;
extern const char TEXT_MUL[]   PROGMEM;
extern const char TEXT_MULT[]  PROGMEM;
extern const char TEXT_MULU[]  PROGMEM;
extern const char TEXT_MVI[]   PROGMEM;
extern const char TEXT_NEGA[]  PROGMEM;
extern const char TEXT_NEGB[]  PROGMEM;
extern const char TEXT_NEGD[]  PROGMEM;
extern const char TEXT_NEGF[]  PROGMEM;
extern const char TEXT_NEG[]   PROGMEM;
extern const char TEXT_NEGX[]  PROGMEM;
extern const char TEXT_NOP[]   PROGMEM;
extern const char TEXT_NOTB[]  PROGMEM;
extern const char TEXT_NOT[]   PROGMEM;
extern const char TEXT_null[]  PROGMEM;
extern const char TEXT_OIM[]   PROGMEM;
extern const char TEXT_ORA[]   PROGMEM;
extern const char TEXT_ORB[]   PROGMEM;
extern const char TEXT_ORD[]   PROGMEM;
extern const char TEXT_ORI[]   PROGMEM;
extern const char TEXT_ORL[]   PROGMEM;
extern const char TEXT_OR[]    PROGMEM;
extern const char TEXT_ORR[]   PROGMEM;
extern const char TEXT_OTDR[]  PROGMEM;
extern const char TEXT_OTIR[]  PROGMEM;
extern const char TEXT_OUTD[]  PROGMEM;
extern const char TEXT_OUTI[]  PROGMEM;
extern const char TEXT_OUT[]   PROGMEM;
extern const char TEXT_OUTS[]  PROGMEM;
extern const char TEXT_PEA[]   PROGMEM;
extern const char TEXT_POPF[]  PROGMEM;
extern const char TEXT_POP[]   PROGMEM;
extern const char TEXT_PSHA[]  PROGMEM;
extern const char TEXT_PSHX[]  PROGMEM;
extern const char TEXT_PULA[]  PROGMEM;
extern const char TEXT_PULX[]  PROGMEM;
extern const char TEXT_PUSHF[] PROGMEM;
extern const char TEXT_PUSH[]  PROGMEM;
extern const char TEXT_RAL[]   PROGMEM;
extern const char TEXT_RAR[]   PROGMEM;
extern const char TEXT_RC[]    PROGMEM;
extern const char TEXT_RCF[]   PROGMEM;
extern const char TEXT_REP[]   PROGMEM;
extern const char TEXT_RESET[] PROGMEM;
extern const char TEXT_RES[]   PROGMEM;
extern const char TEXT_RETEM[] PROGMEM;
extern const char TEXT_RETI[]  PROGMEM;
extern const char TEXT_RET[]   PROGMEM;
extern const char TEXT_RLA[]   PROGMEM;
extern const char TEXT_RLCA[]  PROGMEM;
extern const char TEXT_RLC[]   PROGMEM;
extern const char TEXT_RLD[]   PROGMEM;
extern const char TEXT_RL[]    PROGMEM;
extern const char TEXT_ROLA[]  PROGMEM;
extern const char TEXT_ROLB[]  PROGMEM;
extern const char TEXT_ROL[]   PROGMEM;
extern const char TEXT_RORA[]  PROGMEM;
extern const char TEXT_RORB[]  PROGMEM;
extern const char TEXT_ROR[]   PROGMEM;
extern const char TEXT_RRA[]   PROGMEM;
extern const char TEXT_RRCA[]  PROGMEM;
extern const char TEXT_RRC[]   PROGMEM;
extern const char TEXT_RRD[]   PROGMEM;
extern const char TEXT_RRL[]   PROGMEM;
extern const char TEXT_RR[]    PROGMEM;
extern const char TEXT_RST[]   PROGMEM;
extern const char TEXT_RTI[]   PROGMEM;
extern const char TEXT_RTL[]   PROGMEM;
extern const char TEXT_RTR[]   PROGMEM;
extern const char TEXT_RTS[]   PROGMEM;
extern const char TEXT_SAR[]   PROGMEM;
extern const char TEXT_SBB[]   PROGMEM;
extern const char TEXT_SBCA[]  PROGMEM;
extern const char TEXT_SBCB[]  PROGMEM;
extern const char TEXT_SBCD[]  PROGMEM;
extern const char TEXT_SBC[]   PROGMEM;
extern const char TEXT_SC[]    PROGMEM;
extern const char TEXT_SCC[]   PROGMEM;
extern const char TEXT_SCF[]   PROGMEM;
extern const char TEXT_SD[]    PROGMEM;
extern const char TEXT_SEC[]   PROGMEM;
extern const char TEXT_SEI[]   PROGMEM;
extern const char TEXT_SEP[]   PROGMEM;
extern const char TEXT_SEQ[]   PROGMEM;
extern const char TEXT_SETB[]  PROGMEM;
extern const char TEXT_SETC[]  PROGMEM;
extern const char TEXT_SET[]   PROGMEM;
extern const char TEXT_SEV[]   PROGMEM;
extern const char TEXT_SEX[]   PROGMEM;
extern const char TEXT_SGT[]   PROGMEM;
extern const char TEXT_SHL[]   PROGMEM;
extern const char TEXT_SHR[]   PROGMEM;
extern const char TEXT_SJMP[]  PROGMEM;
extern const char TEXT_SKP[]   PROGMEM;
extern const char TEXT_SLA[]   PROGMEM;
extern const char TEXT_SLL[]   PROGMEM;
extern const char TEXT_SL[]    PROGMEM;
extern const char TEXT_SMI[]   PROGMEM;
extern const char TEXT_SM[]    PROGMEM;
extern const char TEXT_SPL[]   PROGMEM;
extern const char TEXT_S[]     PROGMEM;
extern const char TEXT_SRA[]   PROGMEM;
extern const char TEXT_SRL[]   PROGMEM;
extern const char TEXT_SR[]    PROGMEM;
extern const char TEXT_STA[]   PROGMEM;
extern const char TEXT_STB[]   PROGMEM;
extern const char TEXT_STC[]   PROGMEM;
extern const char TEXT_STD[]   PROGMEM;
extern const char TEXT_STF[]   PROGMEM;
extern const char TEXT_STOP[]  PROGMEM;
extern const char TEXT_ST[]    PROGMEM;
extern const char TEXT_STR[]   PROGMEM;
extern const char TEXT_STS[]   PROGMEM;
extern const char TEXT_STST[]  PROGMEM;
extern const char TEXT_STX[]   PROGMEM;
extern const char TEXT_STY[]   PROGMEM;
extern const char TEXT_SUBA[]  PROGMEM;
extern const char TEXT_SUBB[]  PROGMEM;
extern const char TEXT_SUBCB[] PROGMEM;
extern const char TEXT_SUBC[]  PROGMEM;
extern const char TEXT_SUBD[]  PROGMEM;
extern const char TEXT_SUBF[]  PROGMEM;
extern const char TEXT_SUBI[]  PROGMEM;
extern const char TEXT_SUBL[]  PROGMEM;
extern const char TEXT_SUB[]   PROGMEM;
extern const char TEXT_SUBR[]  PROGMEM;
extern const char TEXT_SUBW[]  PROGMEM;
extern const char TEXT_SVC[]   PROGMEM;
extern const char TEXT_SWAP[]  PROGMEM;
extern const char TEXT_SWI[]   PROGMEM;
extern const char TEXT_TAP[]   PROGMEM;
extern const char TEXT_TAX[]   PROGMEM;
extern const char TEXT_TEST[]  PROGMEM;
extern const char TEXT_TIM[]   PROGMEM;
extern const char TEXT_TPA[]   PROGMEM;
extern const char TEXT_TRAP[]  PROGMEM;
extern const char TEXT_TSET[]  PROGMEM;
extern const char TEXT_TSTA[]  PROGMEM;
extern const char TEXT_TSTB[]  PROGMEM;
extern const char TEXT_TSTD[]  PROGMEM;
extern const char TEXT_TSTF[]  PROGMEM;
extern const char TEXT_TST[]   PROGMEM;
extern const char TEXT_TSX[]   PROGMEM;
extern const char TEXT_TXA[]   PROGMEM;
extern const char TEXT_TXS[]   PROGMEM;
extern const char TEXT_WAI[]   PROGMEM;
extern const char TEXT_WAIT[]  PROGMEM;
extern const char TEXT_XCHD[]  PROGMEM;
extern const char TEXT_XCHG[]  PROGMEM;
extern const char TEXT_XCH[]   PROGMEM;
extern const char TEXT_XORB[]  PROGMEM;
extern const char TEXT_XOR[]   PROGMEM;
extern const char TEXT_XRI[]   PROGMEM;
extern const char TEXT_XRL[]   PROGMEM;

extern const char TEXT_CPU_8080[]   PROGMEM;
extern const char TEXT_CPU_8085[]   PROGMEM;
extern const char TEXT_CPU_V30EMU[] PROGMEM;

extern const char TEXT_REG_AFP[] PROGMEM;
extern const char TEXT_REG_AF[]  PROGMEM;
extern const char TEXT_REG_A[]   PROGMEM;
extern const char TEXT_REG_BC[]  PROGMEM;
extern const char TEXT_REG_B[]   PROGMEM;
extern const char TEXT_REG_C[]   PROGMEM;
extern const char TEXT_REG_DE[]  PROGMEM;
extern const char TEXT_REG_D[]   PROGMEM;
extern const char TEXT_REG_E[]   PROGMEM;
extern const char TEXT_REG_F0[]  PROGMEM;
extern const char TEXT_REG_F1[]  PROGMEM;
extern const char TEXT_REG_HL[]  PROGMEM;
extern const char TEXT_REG_H[]   PROGMEM;
extern const char TEXT_REG_I[]   PROGMEM;
extern const char TEXT_REG_IX[]  PROGMEM;
extern const char TEXT_REG_IY[]  PROGMEM;
extern const char TEXT_REG_L[]   PROGMEM;
extern const char TEXT_REG_P0[]  PROGMEM;
extern const char TEXT_REG_P1[]  PROGMEM;
extern const char TEXT_REG_P2[]  PROGMEM;
extern const char TEXT_REG_P3[]  PROGMEM;
extern const char TEXT_REG_PC[]  PROGMEM;
extern const char TEXT_REG_P[]   PROGMEM;
extern const char TEXT_REG_PSW[] PROGMEM;
extern const char TEXT_REG_R0[]  PROGMEM;
extern const char TEXT_REG_R1[]  PROGMEM;
extern const char TEXT_REG_R2[]  PROGMEM;
extern const char TEXT_REG_R3[]  PROGMEM;
extern const char TEXT_REG_R4[]  PROGMEM;
extern const char TEXT_REG_R5[]  PROGMEM;
extern const char TEXT_REG_R6[]  PROGMEM;
extern const char TEXT_REG_R7[]  PROGMEM;
extern const char TEXT_REG_RR[]  PROGMEM;
extern const char TEXT_REG_SP[]  PROGMEM;
extern const char TEXT_REG_S[]   PROGMEM;
extern const char TEXT_REG_ST[]  PROGMEM;
extern const char TEXT_REG_T[]   PROGMEM;
extern const char TEXT_REG_W[]   PROGMEM;
extern const char TEXT_REG_X[]   PROGMEM;
extern const char TEXT_REG_Y[]   PROGMEM;

extern const char TEXT_CC_Z[]   PROGMEM;
extern const char TEXT_CC_NZ[]  PROGMEM;
extern const char TEXT_CC_NC[]  PROGMEM;
extern const char TEXT_CC_LT[]  PROGMEM;
extern const char TEXT_CC_GT[]  PROGMEM;
extern const char TEXT_CC_EQ[]  PROGMEM;
extern const char TEXT_CC_C[]   PROGMEM;
extern const char TEXT_CC_ULT[] PROGMEM;
extern const char TEXT_CC_ULE[] PROGMEM;
extern const char TEXT_CC_UGT[] PROGMEM;
extern const char TEXT_CC_UGE[] PROGMEM;
extern const char TEXT_CC_T[]   PROGMEM;
extern const char TEXT_CC_PL[]  PROGMEM;
extern const char TEXT_CC_OV[]  PROGMEM;
extern const char TEXT_CC_NOV[] PROGMEM;
extern const char TEXT_CC_NE[]  PROGMEM;
extern const char TEXT_CC_MI[]  PROGMEM;
extern const char TEXT_CC_LE[]  PROGMEM;
extern const char TEXT_CC_GE[]  PROGMEM;
extern const char TEXT_CC_F[]   PROGMEM;
extern const char TEXT_CC_P[]   PROGMEM;
extern const char TEXT_CC_PO[]  PROGMEM;
extern const char TEXT_CC_PE[]  PROGMEM;
extern const char TEXT_CC_M[]   PROGMEM;

extern const char TEXT_FN_ADDR[] PROGMEM;
extern const char TEXT_FN_H[]    PROGMEM;
extern const char TEXT_FN_L[]    PROGMEM;

extern const char TEXT_dADDR[]  PROGMEM; // .addr
extern const char TEXT_dALIGN[] PROGMEM; // .align
extern const char TEXT_dASCII[] PROGMEM; // .ascii
extern const char TEXT_dBLKB[]  PROGMEM; // .blkb
extern const char TEXT_dBLKW[]  PROGMEM; // .blkw
extern const char TEXT_dBYTE[]  PROGMEM; // .byte
extern const char TEXT_dDBYTE[] PROGMEM; // .dbyte
extern const char TEXT_dequal[] PROGMEM; // .=
extern const char TEXT_dEVEN[]  PROGMEM; // .even
extern const char TEXT_dLONG[]  PROGMEM; // .long
extern const char TEXT_dORG[]   PROGMEM; // .org
extern const char TEXT_dSTRING[] PROGMEM; // .string
extern const char TEXT_dWORD[]  PROGMEM; // .word
extern const char TEXT_ALIGN[] PROGMEM;
extern const char TEXT_AORG[]  PROGMEM;
extern const char TEXT_BSS[]   PROGMEM;
extern const char TEXT_BYTE[]  PROGMEM;
extern const char TEXT_DATA[]  PROGMEM;
extern const char TEXT_DB[]    PROGMEM;
extern const char TEXT_DCB[]   PROGMEM;
extern const char TEXT_DCL[]   PROGMEM;
extern const char TEXT_DC[]    PROGMEM;
extern const char TEXT_DCW[]   PROGMEM;
extern const char TEXT_DD[]    PROGMEM;
extern const char TEXT_DEFB[]  PROGMEM;
extern const char TEXT_DEFL[]  PROGMEM;
extern const char TEXT_DEFM[]  PROGMEM;
extern const char TEXT_DEFS[]  PROGMEM;
extern const char TEXT_DEFW[]  PROGMEM;
extern const char TEXT_DL[]    PROGMEM;
extern const char TEXT_DSB[]   PROGMEM;
extern const char TEXT_DSL[]   PROGMEM;
extern const char TEXT_DS[]    PROGMEM;
extern const char TEXT_DSW[]   PROGMEM;
extern const char TEXT_DW[]    PROGMEM;
extern const char TEXT_EVEN[]  PROGMEM;
extern const char TEXT_FCB[]   PROGMEM;
extern const char TEXT_FCC[]   PROGMEM;
extern const char TEXT_FDB[]   PROGMEM;
extern const char TEXT_OPTION[] PROGMEM;
extern const char TEXT_ORG[]   PROGMEM;
extern const char TEXT_RMB[]   PROGMEM;
extern const char TEXT_TEXT[]  PROGMEM;

extern const char PSTR_AT[]      PROGMEM; // "*"
extern const char PSTR_DOLLAR[]  PROGMEM; // "$"
extern const char PSTR_DOT[]     PROGMEM; // "."
extern const char PSTR_SEMI[]    PROGMEM; // ";"
extern const char PSTR_SHARP[]   PROGMEM; // "#"
extern const char PSTR_STAR[]    PROGMEM; // "*"
extern const char PSTR_UNDER[]   PROGMEM; // "_"

extern const char PSTR_AT_QUESTION[]    PROGMEM; // "@?"
extern const char PSTR_B_DASH[]         PROGMEM; // "B'"
extern const char PSTR_DOT_DOLLAR[]     PROGMEM; // ".$"
extern const char PSTR_DOT_DOT[]        PROGMEM; // ".."
extern const char PSTR_DOT_STAR[]       PROGMEM; // ".*"
extern const char PSTR_HAT_B[]          PROGMEM; // "^B"
extern const char PSTR_HAT_D[]          PROGMEM; // "^D"
extern const char PSTR_H_DASH[]         PROGMEM; // "H'"
extern const char PSTR_O_DASH[]         PROGMEM; // "O'"
extern const char PSTR_UNDER_DOLLAR[]   PROGMEM; // "_$"
extern const char PSTR_UNDER_DOT[]      PROGMEM; // "_."
extern const char PSTR_UNDER_QUESTION[] PROGMEM; // "_?"
extern const char PSTR_X_DASH[]         PROGMEM; // "X'"
extern const char PSTR_ZERO_B[]         PROGMEM; // "0b"
extern const char PSTR_ZERO_X[]         PROGMEM; // "0x"

extern const char PSTR_DOT_STAR_DOLLAR[]   PROGMEM; // ".*$"
extern const char PSTR_UNDER_AT_QUESTION[] PROGMEM; // "_@?"
extern const char PSTR_UNDER_DOT_DOLLAR[]  PROGMEM; // "_.$"

extern const char PSTR_PERCENT_2[]                 PROGMEM; // "%(2)"
extern const char PSTR_PERCENT_8[]                 PROGMEM; // "%(8)"
extern const char PSTR_UNDER_DOT_DOLLAR_QUESTION[] PROGMEM; // "_.$?"

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
