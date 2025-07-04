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

#include "text_common.h"

namespace libasm {
namespace text {
namespace common {

// clang-format off
constexpr char TEXT_CPU_Z80[]  PROGMEM = "Z80";
constexpr char TEXT_CPU_Z180[] PROGMEM = "Z180";

constexpr char TEXT_FPU[]  PROGMEM = "FPU";
constexpr char TEXT_none[] PROGMEM = "none";
constexpr char TEXT_NAN[]  PROGMEM = "nan";
constexpr char TEXT_INF[]  PROGMEM = "inf";
constexpr char TEXT_INFINITY[] PROGMEM = "infinity";
constexpr char TEXT_PMMU[] PROGMEM = "PMMU";

constexpr char TEXT_ABA[]   PROGMEM = "ABA";
constexpr char TEXT_ABSD[]  PROGMEM = "ABSD";
constexpr char TEXT_ABSF[]  PROGMEM = "ABSF";
constexpr char TEXT_ABS[]   PROGMEM = "ABS";
constexpr char TEXT_ABX[]   PROGMEM = "ABX";
constexpr char TEXT_ABY[]   PROGMEM = "ABY";
constexpr char TEXT_ADCA[]  PROGMEM = "ADCA";
constexpr char TEXT_ADCB[]  PROGMEM = "ADCB";
constexpr char TEXT_ADCD[]  PROGMEM = "ADCD";
constexpr char TEXT_ADC[]   PROGMEM = "ADC";
constexpr char TEXT_ADDA[]  PROGMEM = "ADDA";
constexpr char TEXT_ADDB[]  PROGMEM = "ADDB";
constexpr char TEXT_ADDCB[] PROGMEM = "ADDCB";
constexpr char TEXT_ADDC[]  PROGMEM = "ADDC";
constexpr char TEXT_ADDD[]  PROGMEM = "ADDD";
constexpr char TEXT_ADDE[]  PROGMEM = "ADDE";
constexpr char TEXT_ADDF[]  PROGMEM = "ADDF";
constexpr char TEXT_ADDI[]  PROGMEM = "ADDI";
constexpr char TEXT_ADDL[]  PROGMEM = "ADDL";
constexpr char TEXT_ADD[]   PROGMEM = "ADD";
constexpr char TEXT_ADDR[]  PROGMEM = "ADDR";
constexpr char TEXT_ADDW[]  PROGMEM = "ADDW";
constexpr char TEXT_ADE[]   PROGMEM = "ADE";
constexpr char TEXT_ADI[]   PROGMEM = "ADI";
constexpr char TEXT_ADX[]   PROGMEM = "ADX";
constexpr char TEXT_AIM[]   PROGMEM = "AIM";
constexpr char TEXT_AI[]    PROGMEM = "AI";
constexpr char TEXT_AIS[]   PROGMEM = "AIS";
constexpr char TEXT_AIX[]   PROGMEM = "AIX";
constexpr char TEXT_AM[]    PROGMEM = "AM";
constexpr char TEXT_ANDA[]  PROGMEM = "ANDA";
constexpr char TEXT_ANDB[]  PROGMEM = "ANDB";
constexpr char TEXT_ANDCC[] PROGMEM = "ANDCC";
constexpr char TEXT_ANDD[]  PROGMEM = "ANDD";
constexpr char TEXT_ANDI[]  PROGMEM = "ANDI";
constexpr char TEXT_ANDP[]  PROGMEM = "ANDP";
constexpr char TEXT_AND[]   PROGMEM = "AND";
constexpr char TEXT_ANDR[]  PROGMEM = "ANDR";
constexpr char TEXT_ANDW[]  PROGMEM = "ANDW";
constexpr char TEXT_ANI[]   PROGMEM = "ANI";
constexpr char TEXT_ANL[]   PROGMEM = "ANL";
constexpr char TEXT_A[]     PROGMEM = "A";
constexpr char TEXT_ASLA[]  PROGMEM = "ASLA";
constexpr char TEXT_ASLB[]  PROGMEM = "ASLB";
constexpr char TEXT_ASLD[]  PROGMEM = "ASLD";
constexpr char TEXT_ASL[]   PROGMEM = "ASL";
constexpr char TEXT_ASRA[]  PROGMEM = "ASRA";
constexpr char TEXT_ASRB[]  PROGMEM = "ASRB";
constexpr char TEXT_ASRD[]  PROGMEM = "ASRD";
constexpr char TEXT_ASR[]   PROGMEM = "ASR";
constexpr char TEXT_BAND[]  PROGMEM = "BAND";
constexpr char TEXT_BCC[]   PROGMEM = "BCC";
constexpr char TEXT_BCLR[]  PROGMEM = "BCLR";
constexpr char TEXT_BC[]    PROGMEM = "BC";
constexpr char TEXT_BCS[]   PROGMEM = "BCS";
constexpr char TEXT_BD[]    PROGMEM = "BD";
constexpr char TEXT_BEQ[]   PROGMEM = "BEQ";
constexpr char TEXT_BGE[]   PROGMEM = "BGE";
constexpr char TEXT_BGND[]  PROGMEM = "BGND";
constexpr char TEXT_BGT[]   PROGMEM = "BGT";
constexpr char TEXT_BHI[]   PROGMEM = "BHI";
constexpr char TEXT_BHS[]   PROGMEM = "BHS";
constexpr char TEXT_BICB[]  PROGMEM = "BICB";
constexpr char TEXT_BITA[]  PROGMEM = "BITA";
constexpr char TEXT_BITB[]  PROGMEM = "BITB";
constexpr char TEXT_BIT[]   PROGMEM = "BIT";
constexpr char TEXT_BLE[]   PROGMEM = "BLE";
constexpr char TEXT_BLO[]   PROGMEM = "BLO";
constexpr char TEXT_BL[]    PROGMEM = "BL";
constexpr char TEXT_BLS[]   PROGMEM = "BLS";
constexpr char TEXT_BLT[]   PROGMEM = "BLT";
constexpr char TEXT_BMI[]   PROGMEM = "BMI";
constexpr char TEXT_BM[]    PROGMEM = "BM";
constexpr char TEXT_BNC[]   PROGMEM = "BNC";
constexpr char TEXT_BNE[]   PROGMEM = "BNE";
constexpr char TEXT_BNZ[]   PROGMEM = "BNZ";
constexpr char TEXT_BOR[]   PROGMEM = "BOR";
constexpr char TEXT_BPL[]   PROGMEM = "BPL";
constexpr char TEXT_BP[]    PROGMEM = "BP";
constexpr char TEXT_B[]     PROGMEM = "B";
constexpr char TEXT_BPT[]   PROGMEM = "BPT";
constexpr char TEXT_BRA[]   PROGMEM = "BRA";
constexpr char TEXT_BRCLR[] PROGMEM = "BRCLR";
constexpr char TEXT_BRN[]   PROGMEM = "BRN";
constexpr char TEXT_BR[]    PROGMEM = "BR";
constexpr char TEXT_BRSET[] PROGMEM = "BRSET";
constexpr char TEXT_BSET[]  PROGMEM = "BSET";
constexpr char TEXT_BSR[]   PROGMEM = "BSR";
constexpr char TEXT_BTJO[]  PROGMEM = "BTJO";
constexpr char TEXT_BTJZ[]  PROGMEM = "BTJZ";
constexpr char TEXT_BT[]    PROGMEM = "BT";
constexpr char TEXT_BTST[]  PROGMEM = "BTST";
constexpr char TEXT_BVC[]   PROGMEM = "BVC";
constexpr char TEXT_BVS[]   PROGMEM = "BVS";
constexpr char TEXT_BZ[]    PROGMEM = "BZ";
constexpr char TEXT_CALLN[] PROGMEM = "CALLN";
constexpr char TEXT_CALL[]  PROGMEM = "CALL";
constexpr char TEXT_CALR[]  PROGMEM = "CALR";
constexpr char TEXT_CAS[]   PROGMEM = "CAS";
constexpr char TEXT_CBA[]   PROGMEM = "CBA";
constexpr char TEXT_CB[]    PROGMEM = "CB";
constexpr char TEXT_CCF[]   PROGMEM = "CCF";
constexpr char TEXT_CC[]    PROGMEM = "CC";
constexpr char TEXT_CI[]    PROGMEM = "CI";
constexpr char TEXT_CLC[]   PROGMEM = "CLC";
constexpr char TEXT_CLD[]   PROGMEM = "CLD";
constexpr char TEXT_CLI[]   PROGMEM = "CLI";
constexpr char TEXT_CLRA[]  PROGMEM = "CLRA";
constexpr char TEXT_CLRB[]  PROGMEM = "CLRB";
constexpr char TEXT_CLRC[]  PROGMEM = "CLRC";
constexpr char TEXT_CLRD[]  PROGMEM = "CLRD";
constexpr char TEXT_CLRE[]  PROGMEM = "CLRE";
constexpr char TEXT_CLRF[]  PROGMEM = "CLRF";
constexpr char TEXT_CLR[]   PROGMEM = "CLR";
constexpr char TEXT_CLRW[]  PROGMEM = "CLRW";
constexpr char TEXT_CLV[]   PROGMEM = "CLV";
constexpr char TEXT_CMA[]   PROGMEM = "CMA";
constexpr char TEXT_CMC[]   PROGMEM = "CMC";
constexpr char TEXT_CMPA[]  PROGMEM = "CMPA";
constexpr char TEXT_CMPB[]  PROGMEM = "CMPB";
constexpr char TEXT_CMPD[]  PROGMEM = "CMPD";
constexpr char TEXT_CMPF[]  PROGMEM = "CMPF";
constexpr char TEXT_CMPL[]  PROGMEM = "CMPL";
constexpr char TEXT_CMP[]   PROGMEM = "CMP";
constexpr char TEXT_CM[]    PROGMEM = "CM";
constexpr char TEXT_CMPR[]  PROGMEM = "CMPR";
constexpr char TEXT_CMPSB[] PROGMEM = "CMPSB";
constexpr char TEXT_CMPS[]  PROGMEM = "CMPS";
constexpr char TEXT_CMPSW[] PROGMEM = "CMPSW";
constexpr char TEXT_CMPW[]  PROGMEM = "CMPW";
constexpr char TEXT_COMA[]  PROGMEM = "COMA";
constexpr char TEXT_COMB[]  PROGMEM = "COMB";
constexpr char TEXT_COMD[]  PROGMEM = "COMD";
constexpr char TEXT_COME[]  PROGMEM = "COME";
constexpr char TEXT_COM[]   PROGMEM = "COM";
constexpr char TEXT_COMW[]  PROGMEM = "COMW";
constexpr char TEXT_CPD[]   PROGMEM = "CPD";
constexpr char TEXT_CPDR[]  PROGMEM = "CPDR";
constexpr char TEXT_CPE[]   PROGMEM = "CPE";
constexpr char TEXT_CPI[]   PROGMEM = "CPI";
constexpr char TEXT_CPIR[]  PROGMEM = "CPIR";
constexpr char TEXT_CPL[]   PROGMEM = "CPL";
constexpr char TEXT_CP[]    PROGMEM = "CP";
constexpr char TEXT_C[]     PROGMEM = "C";
constexpr char TEXT_CPS[]   PROGMEM = "CPS";
constexpr char TEXT_CPU[]   PROGMEM = "CPU";
constexpr char TEXT_CPW[]   PROGMEM = "CPW";
constexpr char TEXT_CPX[]   PROGMEM = "CPX";
constexpr char TEXT_CPY[]   PROGMEM = "CPY";
constexpr char TEXT_DAA[]   PROGMEM = "DAA";
constexpr char TEXT_DAC[]   PROGMEM = "DAC";
constexpr char TEXT_DAD[]   PROGMEM = "DAD";
constexpr char TEXT_DA[]    PROGMEM = "DA";
constexpr char TEXT_DAS[]   PROGMEM = "DAS";
constexpr char TEXT_DBEQ[]  PROGMEM = "DBEQ";
constexpr char TEXT_DBNE[]  PROGMEM = "DBNE";
constexpr char TEXT_DBNZ[]  PROGMEM = "DBNZ";
constexpr char TEXT_DECA[]  PROGMEM = "DECA";
constexpr char TEXT_DECB[]  PROGMEM = "DECB";
constexpr char TEXT_DECD[]  PROGMEM = "DECD";
constexpr char TEXT_DEC[]   PROGMEM = "DEC";
constexpr char TEXT_DECW[]  PROGMEM = "DECW";
constexpr char TEXT_DECX[]  PROGMEM = "DECX";
constexpr char TEXT_DES[]   PROGMEM = "DES";
constexpr char TEXT_DEX[]   PROGMEM = "DEX";
constexpr char TEXT_DEY[]   PROGMEM = "DEY";
constexpr char TEXT_DINT[]  PROGMEM = "DINT";
constexpr char TEXT_DI[]    PROGMEM = "DI";
constexpr char TEXT_DIS[]   PROGMEM = "DIS";
constexpr char TEXT_DIVB[]  PROGMEM = "DIVB";
constexpr char TEXT_DIVD[]  PROGMEM = "DIVD";
constexpr char TEXT_DIVF[]  PROGMEM = "DIVF";
constexpr char TEXT_DIVL[]  PROGMEM = "DIVL";
constexpr char TEXT_DIV[]   PROGMEM = "DIV";
constexpr char TEXT_DIVS[]  PROGMEM = "DIVS";
constexpr char TEXT_DIVU[]  PROGMEM = "DIVU";
constexpr char TEXT_DIVUW[] PROGMEM = "DIVUW";
constexpr char TEXT_DIVW[]  PROGMEM = "DIVW";
constexpr char TEXT_DJNZ[]  PROGMEM = "DJNZ";
constexpr char TEXT_DLD[]   PROGMEM = "DLD";
constexpr char TEXT_EDIV[]  PROGMEM = "EDIV";
constexpr char TEXT_EDIVS[] PROGMEM = "EDIVS";
constexpr char TEXT_EIM[]   PROGMEM = "EIM";
constexpr char TEXT_EINT[]  PROGMEM = "EINT";
constexpr char TEXT_EI[]    PROGMEM = "EI";
constexpr char TEXT_EMUL[]  PROGMEM = "EMUL";
constexpr char TEXT_EMULS[] PROGMEM = "EMULS";
constexpr char TEXT_ENTER[] PROGMEM = "ENTER";
constexpr char TEXT_EORA[]  PROGMEM = "EORA";
constexpr char TEXT_EORB[]  PROGMEM = "EORB";
constexpr char TEXT_EORD[]  PROGMEM = "EORD";
constexpr char TEXT_EORI[]  PROGMEM = "EORI";
constexpr char TEXT_EOR[]   PROGMEM = "EOR";
constexpr char TEXT_EORR[]  PROGMEM = "EORR";
constexpr char TEXT_EXG[]   PROGMEM = "EXG";
constexpr char TEXT_EXIT[]  PROGMEM = "EXIT";
constexpr char TEXT_EX[]    PROGMEM = "EX";
constexpr char TEXT_EXTB[]  PROGMEM = "EXTB";
constexpr char TEXT_EXT[]   PROGMEM = "EXT";
constexpr char TEXT_EXTSB[] PROGMEM = "EXTSB";
constexpr char TEXT_EXTS[]  PROGMEM = "EXTS";
constexpr char TEXT_EXTSW[] PROGMEM = "EXTSW";
constexpr char TEXT_EXX[]   PROGMEM = "EXX";
constexpr char TEXT_FABS[]  PROGMEM = "FABS";
constexpr char TEXT_FADD[]  PROGMEM = "FADD";
constexpr char TEXT_FCOS[]  PROGMEM = "FCOS";
constexpr char TEXT_FDIV[]  PROGMEM = "FDIV";
constexpr char TEXT_FLT[]   PROGMEM = "FLT";
constexpr char TEXT_FMUL[]  PROGMEM = "FMUL";
constexpr char TEXT_FNOP[]  PROGMEM = "FNOP";
constexpr char TEXT_FSAVE[] PROGMEM = "FSAVE";
constexpr char TEXT_FSCALE[] PROGMEM = "FSCALE";
constexpr char TEXT_FSINCOS[] PROGMEM = "FSINCOS";
constexpr char TEXT_FSIN[]  PROGMEM = "FSIN";
constexpr char TEXT_FSQRT[] PROGMEM = "FSQRT";
constexpr char TEXT_FST[]   PROGMEM = "FST";
constexpr char TEXT_FSUB[]  PROGMEM = "FSUB";
constexpr char TEXT_FTST[]  PROGMEM = "FTST";
constexpr char TEXT_HALT[]  PROGMEM = "HALT";
constexpr char TEXT_HLT[]   PROGMEM = "HLT";
constexpr char TEXT_IDIV[]  PROGMEM = "IDIV";
constexpr char TEXT_IDLE[]  PROGMEM = "IDLE";
constexpr char TEXT_IDL[]   PROGMEM = "IDL";
constexpr char TEXT_ILD[]   PROGMEM = "ILD";
constexpr char TEXT_IM[]    PROGMEM = "IM";
constexpr char TEXT_IN0[]   PROGMEM = "IN0";
constexpr char TEXT_INCA[]  PROGMEM = "INCA";
constexpr char TEXT_INCB[]  PROGMEM = "INCB";
constexpr char TEXT_INC[]   PROGMEM = "INC";
constexpr char TEXT_INCW[]  PROGMEM = "INCW";
constexpr char TEXT_INCX[]  PROGMEM = "INCX";
constexpr char TEXT_IND[]   PROGMEM = "IND";
constexpr char TEXT_INDR[]  PROGMEM = "INDR";
constexpr char TEXT_INDRW[] PROGMEM = "INDRW";
constexpr char TEXT_INDW[]  PROGMEM = "INDW";
constexpr char TEXT_INI[]   PROGMEM = "INI";
constexpr char TEXT_INIR[]  PROGMEM = "INIR";
constexpr char TEXT_INIRW[] PROGMEM = "INIRW";
constexpr char TEXT_INIW[]  PROGMEM = "INIW";
constexpr char TEXT_IN[]    PROGMEM = "IN";
constexpr char TEXT_INSB[]  PROGMEM = "INSB";
constexpr char TEXT_INS[]   PROGMEM = "INS";
constexpr char TEXT_INSW[]  PROGMEM = "INSW";
constexpr char TEXT_INV[]   PROGMEM = "INV";
constexpr char TEXT_INW[]   PROGMEM = "INW";
constexpr char TEXT_INX[]   PROGMEM = "INX";
constexpr char TEXT_INY[]   PROGMEM = "INY";
constexpr char TEXT_IOT[]   PROGMEM = "IOT";
constexpr char TEXT_IRET[]  PROGMEM = "IRET";
constexpr char TEXT_JBC[]   PROGMEM = "JBC";
constexpr char TEXT_JB[]    PROGMEM = "JB";
constexpr char TEXT_JC[]    PROGMEM = "JC";
constexpr char TEXT_JE[]    PROGMEM = "JE";
constexpr char TEXT_JEQ[]   PROGMEM = "JEQ";
constexpr char TEXT_JGE[]   PROGMEM = "JGE";
constexpr char TEXT_JG[]    PROGMEM = "JG";
constexpr char TEXT_JGT[]   PROGMEM = "JGT";
constexpr char TEXT_JH[]    PROGMEM = "JH";
constexpr char TEXT_JHS[]   PROGMEM = "JHS";;
constexpr char TEXT_JLE[]   PROGMEM = "JLE";
constexpr char TEXT_JL[]    PROGMEM = "JL";
constexpr char TEXT_JLT[]   PROGMEM = "JLT";
constexpr char TEXT_JMPL[]  PROGMEM = "JMPL";
constexpr char TEXT_JMP[]   PROGMEM = "JMP";
constexpr char TEXT_JNB[]   PROGMEM = "JNB";
constexpr char TEXT_JNC[]   PROGMEM = "JNC";
constexpr char TEXT_JNE[]   PROGMEM = "JNE";
constexpr char TEXT_JNO[]   PROGMEM = "JNO";
constexpr char TEXT_JN[]    PROGMEM = "JN";;
constexpr char TEXT_JNV[]   PROGMEM = "JNV";
constexpr char TEXT_JNZ[]   PROGMEM = "JNZ";
constexpr char TEXT_JPE[]   PROGMEM = "JPE";
constexpr char TEXT_JPO[]   PROGMEM = "JPO";
constexpr char TEXT_JP[]    PROGMEM = "JP";
constexpr char TEXT_JPZ[]   PROGMEM = "JPZ";;
constexpr char TEXT_JR[]    PROGMEM = "JR";
constexpr char TEXT_JSR[]   PROGMEM = "JSR";
constexpr char TEXT_JV[]    PROGMEM = "JV";
constexpr char TEXT_JZ[]    PROGMEM = "JZ";
constexpr char TEXT_LAR[]   PROGMEM = "LAR";
constexpr char TEXT_LBCC[]  PROGMEM = "LBCC";
constexpr char TEXT_LBCS[]  PROGMEM = "LBCS";
constexpr char TEXT_LBEQ[]  PROGMEM = "LBEQ";
constexpr char TEXT_LBGE[]  PROGMEM = "LBGE";
constexpr char TEXT_LBGT[]  PROGMEM = "LBGT";
constexpr char TEXT_LBHI[]  PROGMEM = "LBHI";
constexpr char TEXT_LBHS[]  PROGMEM = "LBHS";
constexpr char TEXT_LBLE[]  PROGMEM = "LBLE";
constexpr char TEXT_LBLO[]  PROGMEM = "LBLO";
constexpr char TEXT_LBLS[]  PROGMEM = "LBLS";
constexpr char TEXT_LBLT[]  PROGMEM = "LBLT";
constexpr char TEXT_LBMI[]  PROGMEM = "LBMI";
constexpr char TEXT_LBNE[]  PROGMEM = "LBNE";
constexpr char TEXT_LBPL[]  PROGMEM = "LBPL";
constexpr char TEXT_LBRA[]  PROGMEM = "LBRA";
constexpr char TEXT_LBRN[]  PROGMEM = "LBRN";
constexpr char TEXT_LBSR[]  PROGMEM = "LBSR";
constexpr char TEXT_LBVC[]  PROGMEM = "LBVC";
constexpr char TEXT_LBVS[]  PROGMEM = "LBVS";
constexpr char TEXT_LCALL[] PROGMEM = "LCALL";
constexpr char TEXT_LDAA[]  PROGMEM = "LDAA";
constexpr char TEXT_LDAB[]  PROGMEM = "LDAB";
constexpr char TEXT_LDA[]   PROGMEM = "LDA";
constexpr char TEXT_LDAR[]  PROGMEM = "LDAR";
constexpr char TEXT_LDB[]   PROGMEM = "LDB";
constexpr char TEXT_LDC[]   PROGMEM = "LDC";
constexpr char TEXT_LDCTL[] PROGMEM = "LDCTL";
constexpr char TEXT_LDD[]   PROGMEM = "LDD";
constexpr char TEXT_LDDR[]  PROGMEM = "LDDR";
constexpr char TEXT_LDED[]  PROGMEM = "LDED";
constexpr char TEXT_LDE[]   PROGMEM = "LDE";
constexpr char TEXT_LDF[]   PROGMEM = "LDF";
constexpr char TEXT_LDI[]   PROGMEM = "LDI";
constexpr char TEXT_LDIR[]  PROGMEM = "LDIR";
constexpr char TEXT_LD[]    PROGMEM = "LD";
constexpr char TEXT_LDSP[]  PROGMEM = "LDSP";
constexpr char TEXT_LDS[]   PROGMEM = "LDS";
constexpr char TEXT_LDW[]   PROGMEM = "LDW";
constexpr char TEXT_LDX[]   PROGMEM = "LDX";
constexpr char TEXT_LDY[]   PROGMEM = "LDY";
constexpr char TEXT_LEA[]   PROGMEM = "LEA";
constexpr char TEXT_LEAS[]  PROGMEM = "LEAS";
constexpr char TEXT_LEAX[]  PROGMEM = "LEAX";
constexpr char TEXT_LEAY[]  PROGMEM = "LEAY";
constexpr char TEXT_LI[]    PROGMEM = "LI";
constexpr char TEXT_LJMP[]  PROGMEM = "LJMP";
constexpr char TEXT_LR[]    PROGMEM = "LR";
constexpr char TEXT_LSLA[]  PROGMEM = "LSLA";
constexpr char TEXT_LSLB[]  PROGMEM = "LSLB";
constexpr char TEXT_LSLD[]  PROGMEM = "LSLD";
constexpr char TEXT_LSL[]   PROGMEM = "LSL";
constexpr char TEXT_LSRA[]  PROGMEM = "LSRA";
constexpr char TEXT_LSRB[]  PROGMEM = "LSRB";
constexpr char TEXT_LSRD[]  PROGMEM = "LSRD";
constexpr char TEXT_LSR[]   PROGMEM = "LSR";
constexpr char TEXT_LSRW[]  PROGMEM = "LSRW";
constexpr char TEXT_LST[]   PROGMEM = "LST";
constexpr char TEXT_MAC[]   PROGMEM = "MAC";
constexpr char TEXT_MARK[]  PROGMEM = "MARK";
constexpr char TEXT_MLT[]   PROGMEM = "MLT";
constexpr char TEXT_MODD[]  PROGMEM = "MODD";
constexpr char TEXT_MOVB[]  PROGMEM = "MOVB";
constexpr char TEXT_MOVD[]  PROGMEM = "MOVD";
constexpr char TEXT_MOVP[]  PROGMEM = "MOVP";
constexpr char TEXT_MOV[]   PROGMEM = "MOV";
constexpr char TEXT_MOVSB[] PROGMEM = "MOVSB";
constexpr char TEXT_MOVSW[] PROGMEM = "MOVSW";
constexpr char TEXT_MOVW[]  PROGMEM = "MOVW";
constexpr char TEXT_MOVX[]  PROGMEM = "MOVX";
constexpr char TEXT_MPY[]   PROGMEM = "MPY";
constexpr char TEXT_MPYS[]  PROGMEM = "MPYS";
constexpr char TEXT_MULB[]  PROGMEM = "MULB";
constexpr char TEXT_MULD[]  PROGMEM = "MULD";
constexpr char TEXT_MULF[]  PROGMEM = "MULF";
constexpr char TEXT_MUL[]   PROGMEM = "MUL";
constexpr char TEXT_MULT[]  PROGMEM = "MULT";
constexpr char TEXT_MULTUW[] PROGMEM = "MULTUW";
constexpr char TEXT_MULTW[] PROGMEM = "MULTW";
constexpr char TEXT_MULU[]  PROGMEM = "MULU";
constexpr char TEXT_MVI[]   PROGMEM = "MVI";
constexpr char TEXT_NEGA[]  PROGMEM = "NEGA";
constexpr char TEXT_NEGB[]  PROGMEM = "NEGB";
constexpr char TEXT_NEGD[]  PROGMEM = "NEGD";
constexpr char TEXT_NEGF[]  PROGMEM = "NEGF";
constexpr char TEXT_NEG[]   PROGMEM = "NEG";
constexpr char TEXT_NEGW[]  PROGMEM = "NEGW";
constexpr char TEXT_NEGX[]  PROGMEM = "NEGX";
constexpr char TEXT_NOP[]   PROGMEM = "NOP";
constexpr char TEXT_NOTB[]  PROGMEM = "NOTB";
constexpr char TEXT_NOT[]   PROGMEM = "NOT";
constexpr char TEXT_null[]  PROGMEM = "";
constexpr char TEXT_OIM[]   PROGMEM = "OIM";
constexpr char TEXT_ORAA[]  PROGMEM = "ORAA";
constexpr char TEXT_ORAB[]  PROGMEM = "ORAB";
constexpr char TEXT_ORA[]   PROGMEM = "ORA";
constexpr char TEXT_ORB[]   PROGMEM = "ORB";
constexpr char TEXT_ORCC[]  PROGMEM = "ORCC";
constexpr char TEXT_ORD[]   PROGMEM = "ORD";
constexpr char TEXT_ORE[]   PROGMEM = "ORE";
constexpr char TEXT_ORI[]   PROGMEM = "ORI";
constexpr char TEXT_ORL[]   PROGMEM = "ORL";
constexpr char TEXT_ORP[]   PROGMEM = "ORP";
constexpr char TEXT_OR[]    PROGMEM = "OR";
constexpr char TEXT_ORR[]   PROGMEM = "ORR";
constexpr char TEXT_ORW[]   PROGMEM = "ORW";
constexpr char TEXT_OTDM[]  PROGMEM = "OTDM";
constexpr char TEXT_OTDMR[] PROGMEM = "OTDMR";
constexpr char TEXT_OTDR[]  PROGMEM = "OTDR";
constexpr char TEXT_OTDRW[] PROGMEM = "OTDRW";
constexpr char TEXT_OTIM[]  PROGMEM = "OTIM";
constexpr char TEXT_OTIMR[] PROGMEM = "OTIMR";
constexpr char TEXT_OTIR[]  PROGMEM = "OTIR";
constexpr char TEXT_OTIRW[] PROGMEM = "OTIRW";
constexpr char TEXT_OUT0[]  PROGMEM = "OUT0";
constexpr char TEXT_OUTD[]  PROGMEM = "OUTD";
constexpr char TEXT_OUTDW[] PROGMEM = "OUTDW";
constexpr char TEXT_OUTI[]  PROGMEM = "OUTI";
constexpr char TEXT_OUTIW[] PROGMEM = "OUTIW";
constexpr char TEXT_OUT[]   PROGMEM = "OUT";
constexpr char TEXT_OUTS[]  PROGMEM = "OUTS";
constexpr char TEXT_OUTW[]  PROGMEM = "OUTW";
constexpr char TEXT_PEA[]   PROGMEM = "PEA";
constexpr char TEXT_POPA[]  PROGMEM = "POPA";
constexpr char TEXT_POPF[]  PROGMEM = "POPF";
constexpr char TEXT_POP[]   PROGMEM = "POP";
constexpr char TEXT_PSHA[]  PROGMEM = "PSHA";
constexpr char TEXT_PSHB[]  PROGMEM = "PSHB";
constexpr char TEXT_PSHD[]  PROGMEM = "PSHD";
constexpr char TEXT_PSHM[]  PROGMEM = "PSHM";
constexpr char TEXT_PSHX[]  PROGMEM = "PSHX";
constexpr char TEXT_PSHY[]  PROGMEM = "PSHY";
constexpr char TEXT_PULA[]  PROGMEM = "PULA";
constexpr char TEXT_PULB[]  PROGMEM = "PULB";
constexpr char TEXT_PULD[]  PROGMEM = "PULD";
constexpr char TEXT_PULX[]  PROGMEM = "PULX";
constexpr char TEXT_PULY[]  PROGMEM = "PULY";
constexpr char TEXT_PUSHA[] PROGMEM = "PUSHA";
constexpr char TEXT_PUSHF[] PROGMEM = "PUSHF";
constexpr char TEXT_PUSH[]  PROGMEM = "PUSH";
constexpr char TEXT_RAL[]   PROGMEM = "RAL";
constexpr char TEXT_RAR[]   PROGMEM = "RAR";
constexpr char TEXT_RCF[]   PROGMEM = "RCF";
constexpr char TEXT_RC[]    PROGMEM = "RC";
constexpr char TEXT_REP[]   PROGMEM = "REP";
constexpr char TEXT_RESET[] PROGMEM = "RESET";
constexpr char TEXT_RES[]   PROGMEM = "RES";
constexpr char TEXT_RETC[]  PROGMEM = "RETC";
constexpr char TEXT_RETE[]  PROGMEM = "RETE";
constexpr char TEXT_RETEM[] PROGMEM = "RETEM";
constexpr char TEXT_RETI[]  PROGMEM = "RETI";
constexpr char TEXT_RETN[]  PROGMEM = "RETN";
constexpr char TEXT_RET[]   PROGMEM = "RET";
constexpr char TEXT_RLA[]   PROGMEM = "RLA";
constexpr char TEXT_RLCA[]  PROGMEM = "RLCA";
constexpr char TEXT_RLC[]   PROGMEM = "RLC";
constexpr char TEXT_RLD[]   PROGMEM = "RLD";
constexpr char TEXT_RL[]    PROGMEM = "RL";
constexpr char TEXT_ROLA[]  PROGMEM = "ROLA";
constexpr char TEXT_ROLB[]  PROGMEM = "ROLB";
constexpr char TEXT_ROLD[]  PROGMEM = "ROLD";
constexpr char TEXT_ROL[]   PROGMEM = "ROL";
constexpr char TEXT_ROLW[]  PROGMEM = "ROLW";
constexpr char TEXT_RORA[]  PROGMEM = "RORA";
constexpr char TEXT_RORB[]  PROGMEM = "RORB";
constexpr char TEXT_RORD[]  PROGMEM = "RORD";
constexpr char TEXT_ROR[]   PROGMEM = "ROR";
constexpr char TEXT_RORW[]  PROGMEM = "RORW";
constexpr char TEXT_RRA[]   PROGMEM = "RRA";
constexpr char TEXT_RRCA[]  PROGMEM = "RRCA";
constexpr char TEXT_RRC[]   PROGMEM = "RRC";
constexpr char TEXT_RRD[]   PROGMEM = "RRD";
constexpr char TEXT_RRL[]   PROGMEM = "RRL";
constexpr char TEXT_RR[]    PROGMEM = "RR";
constexpr char TEXT_RST[]   PROGMEM = "RST";
constexpr char TEXT_RTC[]   PROGMEM = "RTC";
constexpr char TEXT_RTI[]   PROGMEM = "RTI";
constexpr char TEXT_RTL[]   PROGMEM = "RTL";
constexpr char TEXT_RTR[]   PROGMEM = "RTR";
constexpr char TEXT_RTS[]   PROGMEM = "RTS";
constexpr char TEXT_SAR[]   PROGMEM = "SAR";
constexpr char TEXT_SBA[]   PROGMEM = "SBA";
constexpr char TEXT_SBB[]   PROGMEM = "SBB";
constexpr char TEXT_SBCA[]  PROGMEM = "SBCA";
constexpr char TEXT_SBCB[]  PROGMEM = "SBCB";
constexpr char TEXT_SBCD[]  PROGMEM = "SBCD";
constexpr char TEXT_SBC[]   PROGMEM = "SBC";
constexpr char TEXT_SCC[]   PROGMEM = "SCC";
constexpr char TEXT_SCF[]   PROGMEM = "SCF";
constexpr char TEXT_SC[]    PROGMEM = "SC";
constexpr char TEXT_SD[]    PROGMEM = "SD";
constexpr char TEXT_SEC[]   PROGMEM = "SEC";
constexpr char TEXT_SEI[]   PROGMEM = "SEI";
constexpr char TEXT_SEP[]   PROGMEM = "SEP";
constexpr char TEXT_SEQ[]   PROGMEM = "SEQ";
constexpr char TEXT_SETB[]  PROGMEM = "SETB";
constexpr char TEXT_SETC[]  PROGMEM = "SETC";
constexpr char TEXT_SET[]   PROGMEM = "SET";
constexpr char TEXT_SEV[]   PROGMEM = "SEV";
constexpr char TEXT_SEX[]   PROGMEM = "SEX";
constexpr char TEXT_SGT[]   PROGMEM = "SGT";
constexpr char TEXT_SHL[]   PROGMEM = "SHL";
constexpr char TEXT_SHR[]   PROGMEM = "SHR";
constexpr char TEXT_SJMP[]  PROGMEM = "SJMP";
constexpr char TEXT_SKP[]   PROGMEM = "SKP";
constexpr char TEXT_SLA[]   PROGMEM = "SLA";
constexpr char TEXT_SLL[]   PROGMEM = "SLL";
constexpr char TEXT_SLP[]   PROGMEM = "SLP";
constexpr char TEXT_SL[]    PROGMEM = "SL";
constexpr char TEXT_SMI[]   PROGMEM = "SMI";
constexpr char TEXT_SM[]    PROGMEM = "SM";
constexpr char TEXT_SPL[]   PROGMEM = "SPL";
constexpr char TEXT_S[]     PROGMEM = "S";
constexpr char TEXT_SRA[]   PROGMEM = "SRA";
constexpr char TEXT_SRL[]   PROGMEM = "SRL";
constexpr char TEXT_SR[]    PROGMEM = "SR";
constexpr char TEXT_STAA[]  PROGMEM = "STAA";
constexpr char TEXT_STAB[]  PROGMEM = "STAB";
constexpr char TEXT_STA[]   PROGMEM = "STA";
constexpr char TEXT_STB[]   PROGMEM = "STB";
constexpr char TEXT_STC[]   PROGMEM = "STC";
constexpr char TEXT_STD[]   PROGMEM = "STD";
constexpr char TEXT_STE[]   PROGMEM = "STE";
constexpr char TEXT_STF[]   PROGMEM = "STF";
constexpr char TEXT_STOP[]  PROGMEM = "STOP";
constexpr char TEXT_ST[]    PROGMEM = "ST";
constexpr char TEXT_STR[]   PROGMEM = "STR";
constexpr char TEXT_STSP[]  PROGMEM = "STSP";
constexpr char TEXT_STS[]   PROGMEM = "STS";
constexpr char TEXT_STST[]  PROGMEM = "STST";
constexpr char TEXT_STX[]   PROGMEM = "STX";
constexpr char TEXT_STY[]   PROGMEM = "STY";
constexpr char TEXT_STZ[]   PROGMEM = "STZ";
constexpr char TEXT_SUBA[]  PROGMEM = "SUBA";
constexpr char TEXT_SUBB[]  PROGMEM = "SUBB";
constexpr char TEXT_SUBCB[] PROGMEM = "SUBCB";
constexpr char TEXT_SUBC[]  PROGMEM = "SUBC";
constexpr char TEXT_SUBD[]  PROGMEM = "SUBD";
constexpr char TEXT_SUBE[]  PROGMEM = "SUBE";
constexpr char TEXT_SUBF[]  PROGMEM = "SUBF";
constexpr char TEXT_SUBI[]  PROGMEM = "SUBI";
constexpr char TEXT_SUBL[]  PROGMEM = "SUBL";
constexpr char TEXT_SUB[]   PROGMEM = "SUB";
constexpr char TEXT_SUBR[]  PROGMEM = "SUBR";
constexpr char TEXT_SUBW[]  PROGMEM = "SUBW";
constexpr char TEXT_SVC[]   PROGMEM = "SVC";
constexpr char TEXT_SWAP[]  PROGMEM = "SWAP";
constexpr char TEXT_SWI[]   PROGMEM = "SWI";
constexpr char TEXT_SXT[]   PROGMEM = "SXT";
constexpr char TEXT_TAB[]   PROGMEM = "TAB";
constexpr char TEXT_TAP[]   PROGMEM = "TAP";
constexpr char TEXT_TAX[]   PROGMEM = "TAX";
constexpr char TEXT_TBA[]   PROGMEM = "TBA";
constexpr char TEXT_TEST[]  PROGMEM = "TEST";
constexpr char TEXT_TFR[]   PROGMEM = "TFR";
constexpr char TEXT_TIM[]   PROGMEM = "TIM";
constexpr char TEXT_TPA[]   PROGMEM = "TPA";
constexpr char TEXT_TRAP[]  PROGMEM = "TRAP";
constexpr char TEXT_TSET[]  PROGMEM = "TSET";
constexpr char TEXT_TSTA[]  PROGMEM = "TSTA";
constexpr char TEXT_TSTB[]  PROGMEM = "TSTB";
constexpr char TEXT_TSTD[]  PROGMEM = "TSTD";
constexpr char TEXT_TSTE[]  PROGMEM = "TSTE";
constexpr char TEXT_TSTF[]  PROGMEM = "TSTF";
constexpr char TEXT_TSTIO[] PROGMEM = "TSTIO";
constexpr char TEXT_TST[]   PROGMEM = "TST";
constexpr char TEXT_TSTW[]  PROGMEM = "TSTW";
constexpr char TEXT_TSX[]   PROGMEM = "TSX";
constexpr char TEXT_TSY[]   PROGMEM = "TSY";
constexpr char TEXT_TXA[]   PROGMEM = "TXA";
constexpr char TEXT_TXS[]   PROGMEM = "TXS";
constexpr char TEXT_TXY[]   PROGMEM = "TXY";
constexpr char TEXT_TYS[]   PROGMEM = "TYS";
constexpr char TEXT_TYX[]   PROGMEM = "TYX";
constexpr char TEXT_WAI[]   PROGMEM = "WAI";
constexpr char TEXT_WAIT[]  PROGMEM = "WAIT";
constexpr char TEXT_XCHB[]  PROGMEM = "XCHB";
constexpr char TEXT_XCHD[]  PROGMEM = "XCHD";
constexpr char TEXT_XCHG[]  PROGMEM = "XCHG";
constexpr char TEXT_XCH[]   PROGMEM = "XCH";
constexpr char TEXT_XGDX[]  PROGMEM = "XGDX";
constexpr char TEXT_XGDY[]  PROGMEM = "XGDY";
constexpr char TEXT_XORB[]  PROGMEM = "XORB";
constexpr char TEXT_XOR[]   PROGMEM = "XOR";
constexpr char TEXT_XORW[]  PROGMEM = "XORW";
constexpr char TEXT_XRI[]   PROGMEM = "XRI";
constexpr char TEXT_XRL[]   PROGMEM = "XRL";

constexpr char TEXT_REG_AC0[] PROGMEM = "AC0";
constexpr char TEXT_REG_AC1[] PROGMEM = "AC1";
constexpr char TEXT_REG_AF[]  PROGMEM = "AF";
constexpr char TEXT_REG_A[]   PROGMEM = "A";
constexpr char TEXT_REG_BC[]  PROGMEM = "BC";
constexpr char TEXT_REG_B[]   PROGMEM = "B";
constexpr char TEXT_REG_CCR[] PROGMEM = "CCR";
constexpr char TEXT_REG_C[]   PROGMEM = "C";
constexpr char TEXT_REG_DE[]  PROGMEM = "DE";
constexpr char TEXT_REG_D[]   PROGMEM = "D";
constexpr char TEXT_REG_E[]   PROGMEM = "E";
constexpr char TEXT_REG_F0[]  PROGMEM = "F0";
constexpr char TEXT_REG_F1[]  PROGMEM = "F1";
constexpr char TEXT_REG_HL[]  PROGMEM = "HL";
constexpr char TEXT_REG_H[]   PROGMEM = "H";
constexpr char TEXT_REG_I[]   PROGMEM = "I";
constexpr char TEXT_REG_IXL[] PROGMEM = "IXL";
constexpr char TEXT_REG_IX[]  PROGMEM = "IX";
constexpr char TEXT_REG_IYL[] PROGMEM = "IYL";
constexpr char TEXT_REG_IY[]  PROGMEM = "IY";
constexpr char TEXT_REG_K[]   PROGMEM = "K";
constexpr char TEXT_REG_L[]   PROGMEM = "L";
constexpr char TEXT_REG_P0[]  PROGMEM = "P0";
constexpr char TEXT_REG_P1[]  PROGMEM = "P1";
constexpr char TEXT_REG_P2[]  PROGMEM = "P2";
constexpr char TEXT_REG_P3[]  PROGMEM = "P3";
constexpr char TEXT_REG_PC[]  PROGMEM = "PC";
constexpr char TEXT_REG_P[]   PROGMEM = "P";
constexpr char TEXT_REG_PSR[] PROGMEM = "PSR";
constexpr char TEXT_REG_PSW[] PROGMEM = "PSW";
constexpr char TEXT_REG_R0[]  PROGMEM = "R0";
constexpr char TEXT_REG_R1[]  PROGMEM = "R1";
constexpr char TEXT_REG_R2[]  PROGMEM = "R2";
constexpr char TEXT_REG_R3[]  PROGMEM = "R3";
constexpr char TEXT_REG_R4[]  PROGMEM = "R4";
constexpr char TEXT_REG_R5[]  PROGMEM = "R5";
constexpr char TEXT_REG_R6[]  PROGMEM = "R6";
constexpr char TEXT_REG_R7[]  PROGMEM = "R7";
constexpr char TEXT_REG_R[]   PROGMEM = "R";
constexpr char TEXT_REG_RR[]  PROGMEM = "RR";
constexpr char TEXT_REG_SP[]  PROGMEM = "SP";
constexpr char TEXT_REG_S[]   PROGMEM = "S";
constexpr char TEXT_REG_SR[]  PROGMEM = "SR";
constexpr char TEXT_REG_ST[]  PROGMEM = "ST";
constexpr char TEXT_REG_TC[]  PROGMEM = "TC";
constexpr char TEXT_REG_T[]   PROGMEM = "T";
constexpr char TEXT_REG_USP[] PROGMEM = "USP";
constexpr char TEXT_REG_W[]   PROGMEM = "W";
constexpr char TEXT_REG_X[]   PROGMEM = "X";
constexpr char TEXT_REG_Y[]   PROGMEM = "Y";
constexpr char TEXT_REG_Z[]   PROGMEM = "Z";

constexpr char TEXT_CC_C[]   PROGMEM = "C";
constexpr char TEXT_CC_EQ[]  PROGMEM = "EQ";
constexpr char TEXT_CC_F[]   PROGMEM = "F";
constexpr char TEXT_CC_GE[]  PROGMEM = "GE";
constexpr char TEXT_CC_GT[]  PROGMEM = "GT";
constexpr char TEXT_CC_LE[]  PROGMEM = "LE";
constexpr char TEXT_CC_LT[]  PROGMEM = "LT";
constexpr char TEXT_CC_MI[]  PROGMEM = "MI";
constexpr char TEXT_CC_M[]   PROGMEM = "M";
constexpr char TEXT_CC_NC[]  PROGMEM = "NC";
constexpr char TEXT_CC_NE[]  PROGMEM = "NE";
constexpr char TEXT_CC_NOV[] PROGMEM = "NOV";
constexpr char TEXT_CC_NS[]  PROGMEM = "NS";
constexpr char TEXT_CC_NV[]  PROGMEM = "NV";
constexpr char TEXT_CC_NZ[]  PROGMEM = "NZ";
constexpr char TEXT_CC_OV[]  PROGMEM = "OV";
constexpr char TEXT_CC_PE[]  PROGMEM = "PE";
constexpr char TEXT_CC_PL[]  PROGMEM = "PL";
constexpr char TEXT_CC_PO[]  PROGMEM = "PO";
constexpr char TEXT_CC_P[]   PROGMEM = "P";
constexpr char TEXT_CC_S[]   PROGMEM = "S";
constexpr char TEXT_CC_T[]   PROGMEM = "";
constexpr char TEXT_CC_UGE[] PROGMEM = "UGE";
constexpr char TEXT_CC_UGT[] PROGMEM = "UGT";
constexpr char TEXT_CC_ULE[] PROGMEM = "ULE";
constexpr char TEXT_CC_ULT[] PROGMEM = "ULT";
constexpr char TEXT_CC_V[]   PROGMEM = "V";
constexpr char TEXT_CC_Z[]   PROGMEM = "Z";

constexpr char TEXT_FN_ADDR[] PROGMEM = "ADDR";
constexpr char TEXT_FN_H[]    PROGMEM = "H";
constexpr char TEXT_FN_L[]    PROGMEM = "L";

constexpr char TEXT_dADDR[]  PROGMEM = ".addr";
constexpr char TEXT_dALIGN[] PROGMEM = ".align";
constexpr char TEXT_dASCII[] PROGMEM = ".ascii";
constexpr char TEXT_dBLKB[]  PROGMEM = ".blkb";
constexpr char TEXT_dBLKW[]  PROGMEM = ".blkw";
constexpr char TEXT_dBSS[]   PROGMEM = ".bss";
constexpr char TEXT_dBYTE[]  PROGMEM = ".byte";
constexpr char TEXT_dDBYTE[] PROGMEM = ".dbyte";
constexpr char TEXT_dequal[] PROGMEM = ".=";
constexpr char TEXT_dEVEN[]  PROGMEM = ".even";
constexpr char TEXT_dLONG[]  PROGMEM = ".long";
constexpr char TEXT_dORG[]   PROGMEM = ".org";
constexpr char TEXT_dSTRING[] PROGMEM = ".string";
constexpr char TEXT_dWORD[]  PROGMEM = ".word";
constexpr char TEXT_ALIGN[] PROGMEM = "align";
constexpr char TEXT_AORG[]  PROGMEM = "aorg";
constexpr char TEXT_BSS[]   PROGMEM = "bss";
constexpr char TEXT_BYTE[]  PROGMEM = "byte";
constexpr char TEXT_DATA[]  PROGMEM = "data";
constexpr char TEXT_DB[]    PROGMEM = "db";
constexpr char TEXT_DCB[]   PROGMEM = "dcb";
constexpr char TEXT_DCL[]   PROGMEM = "dcl";
constexpr char TEXT_DC[]    PROGMEM = "dc";
constexpr char TEXT_DCW[]   PROGMEM = "dcw";
constexpr char TEXT_DD[]    PROGMEM = "dd";
constexpr char TEXT_DEFB[]  PROGMEM = "defb";
constexpr char TEXT_DEFL[]  PROGMEM = "defl";
constexpr char TEXT_DEFM[]  PROGMEM = "defm";
constexpr char TEXT_DEFS[]  PROGMEM = "defs";
constexpr char TEXT_DEFW[]  PROGMEM = "defw";
constexpr char TEXT_DL[]    PROGMEM = "dl";
constexpr char TEXT_DSB[]   PROGMEM = "dsb";
constexpr char TEXT_DSL[]   PROGMEM = "dsl";
constexpr char TEXT_DS[]    PROGMEM = "ds";
constexpr char TEXT_DSW[]   PROGMEM = "dsw";
constexpr char TEXT_DW[]    PROGMEM = "dw";
constexpr char TEXT_EVEN[]  PROGMEM = "even";
constexpr char TEXT_FCB[]   PROGMEM = "fcb";
constexpr char TEXT_FCC[]   PROGMEM = "fcc";
constexpr char TEXT_FDB[]   PROGMEM = "fdb";
constexpr char TEXT_OPTION[] PROGMEM = "option";
constexpr char TEXT_ORG[]   PROGMEM = "org";
constexpr char TEXT_RMB[]   PROGMEM = "rmb";
constexpr char TEXT_TEXT[]  PROGMEM = "text";

constexpr char PSTR_AT[]      PROGMEM = "@";
constexpr char PSTR_DOLLAR[]  PROGMEM = "$";
constexpr char PSTR_COMMA[]   PROGMEM = ",";
constexpr char PSTR_DOT[]     PROGMEM = ".";
constexpr char PSTR_SEMI[]    PROGMEM = ";";
constexpr char PSTR_SHARP[]   PROGMEM = "#";
constexpr char PSTR_STAR[]    PROGMEM = "*";
constexpr char PSTR_UNDER[]   PROGMEM = "_";

constexpr char PSTR_AT_QUESTION[]    PROGMEM = "@?";
constexpr char PSTR_B_DASH[]         PROGMEM = "b'";
constexpr char PSTR_DOT_DOLLAR[]     PROGMEM = ".$";
constexpr char PSTR_DOT_DOT[]        PROGMEM = "..";
constexpr char PSTR_DOT_STAR[]       PROGMEM = ".*";
constexpr char PSTR_HAT_B[]          PROGMEM = "^B";
constexpr char PSTR_HAT_D[]          PROGMEM = "^D";
constexpr char PSTR_H_DASH[]         PROGMEM = "h'";
constexpr char PSTR_O_DASH[]         PROGMEM = "o'";
constexpr char PSTR_UNDER_DOLLAR[]   PROGMEM = "_$";
constexpr char PSTR_UNDER_DOT[]      PROGMEM = "_.";
constexpr char PSTR_UNDER_QUESTION[] PROGMEM = "_?";
constexpr char PSTR_X_DASH[]         PROGMEM = "x'";
constexpr char PSTR_ZERO_B[]         PROGMEM = "0b";
constexpr char PSTR_ZERO_X[]         PROGMEM = "0x";

constexpr char PSTR_DOT_STAR_DOLLAR[]   PROGMEM = ".*$";
constexpr char PSTR_UNDER_AT_QUESTION[] PROGMEM = "_@?";
constexpr char PSTR_UNDER_DOT_DOLLAR[]  PROGMEM = "_.$";

constexpr char PSTR_PERCENT_2[]                 PROGMEM = "%(2)";
constexpr char PSTR_PERCENT_8[]                 PROGMEM = "%(8)";
constexpr char PSTR_UNDER_DOT_DOLLAR_QUESTION[] PROGMEM = "_.$?";

// clang-format on

}  // namespace common
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
