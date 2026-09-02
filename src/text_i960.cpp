/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "text_i960.h"

namespace libasm {
namespace text {
namespace i960 {
// clang-format off

constexpr char TEXT_I960_LIST[] PROGMEM = "80960";
constexpr char TEXT_CPU_80960[] PROGMEM = "80960";
constexpr char OPT_BOOL_MODE_BITS[] PROGMEM = "mode-bits";
constexpr char OPT_DESC_MODE_BITS[] PROGMEM = "value of the mode bit of an unused operand field";
constexpr char OPT_TEXT_IP_OPERAND[] PROGMEM = "ip-operand";
constexpr char OPT_DESC_IP_OPERAND[] PROGMEM = "IP-relative operand: offset (default) or target";
constexpr char OPT_BOOL_CANONICAL_MEM[] PROGMEM = "canonical-mem";
constexpr char OPT_DESC_CANONICAL_MEM[] PROGMEM = "reject memory operand which has a shorter encoding";
constexpr char OPT_BOOL_FPU[] PROGMEM = "fpu";
constexpr char OPT_DESC_FPU[] PROGMEM = "enable 80960KB floating point and decimal instructions";
constexpr char TEXT_REG_PFP[] PROGMEM = "PFP";
constexpr char TEXT_REG_RIP[] PROGMEM = "RIP";
constexpr char TEXT_REG_IP[]  PROGMEM = "IP";
// constexpr char TEXT_REG_FP[]  PROGMEM = "FP";
// constexpr char TEXT_REG_SP[]  PROGMEM = "SP";

// i960
// constexpr char TEXT_ADDC[]     PROGMEM = "ADDC";
// constexpr char TEXT_ADDI[]     PROGMEM = "ADDI";
constexpr char TEXT_ADDO[]     PROGMEM = "ADDO";
constexpr char TEXT_ALTERBIT[] PROGMEM = "ALTERBIT";
// constexpr char TEXT_AND[]      PROGMEM = "AND";
constexpr char TEXT_ANDNOT[]   PROGMEM = "ANDNOT";
constexpr char TEXT_ATADD[]    PROGMEM = "ATADD";
constexpr char TEXT_ATMOD[]    PROGMEM = "ATMOD";
// constexpr char TEXT_B[]        PROGMEM = "B";
constexpr char TEXT_BAL[]      PROGMEM = "BAL";
constexpr char TEXT_BALX[]     PROGMEM = "BALX";
constexpr char TEXT_BBC[]      PROGMEM = "BBC";
constexpr char TEXT_BBS[]      PROGMEM = "BBS";
constexpr char TEXT_BE[]       PROGMEM = "BE";
constexpr char TEXT_BG[]       PROGMEM = "BG";
// constexpr char TEXT_BGE[]      PROGMEM = "BGE";
// constexpr char TEXT_BL[]       PROGMEM = "BL";
// constexpr char TEXT_BLE[]      PROGMEM = "BLE";
// constexpr char TEXT_BNE[]      PROGMEM = "BNE";
constexpr char TEXT_BNO[]      PROGMEM = "BNO";
constexpr char TEXT_BO[]       PROGMEM = "BO";
constexpr char TEXT_BX[]       PROGMEM = "BX";
// constexpr char TEXT_CALL[]     PROGMEM = "CALL";
constexpr char TEXT_CALLS[]    PROGMEM = "CALLS";
constexpr char TEXT_CALLX[]    PROGMEM = "CALLX";
constexpr char TEXT_CHKBIT[]   PROGMEM = "CHKBIT";
constexpr char TEXT_CLRBIT[]   PROGMEM = "CLRBIT";
constexpr char TEXT_CMPDECI[]  PROGMEM = "CMPDECI";
constexpr char TEXT_CMPDECO[]  PROGMEM = "CMPDECO";
// constexpr char TEXT_CMPI[]     PROGMEM = "CMPI";
constexpr char TEXT_CMPIBE[]   PROGMEM = "CMPIBE";
constexpr char TEXT_CMPIBG[]   PROGMEM = "CMPIBG";
constexpr char TEXT_CMPIBGE[]  PROGMEM = "CMPIBGE";
constexpr char TEXT_CMPIBL[]   PROGMEM = "CMPIBL";
constexpr char TEXT_CMPIBLE[]  PROGMEM = "CMPIBLE";
constexpr char TEXT_CMPIBNE[]  PROGMEM = "CMPIBNE";
constexpr char TEXT_CMPIBNO[]  PROGMEM = "CMPIBNO";
constexpr char TEXT_CMPIBO[]   PROGMEM = "CMPIBO";
constexpr char TEXT_CMPINCI[]  PROGMEM = "CMPINCI";
constexpr char TEXT_CMPINCO[]  PROGMEM = "CMPINCO";
constexpr char TEXT_CMPO[]     PROGMEM = "CMPO";
constexpr char TEXT_CMPOBE[]   PROGMEM = "CMPOBE";
constexpr char TEXT_CMPOBG[]   PROGMEM = "CMPOBG";
constexpr char TEXT_CMPOBGE[]  PROGMEM = "CMPOBGE";
constexpr char TEXT_CMPOBL[]   PROGMEM = "CMPOBL";
constexpr char TEXT_CMPOBLE[]  PROGMEM = "CMPOBLE";
constexpr char TEXT_CMPOBNE[]  PROGMEM = "CMPOBNE";
constexpr char TEXT_CONCMPI[]  PROGMEM = "CONCMPI";
constexpr char TEXT_CONCMPO[]  PROGMEM = "CONCMPO";
constexpr char TEXT_DIVI[]     PROGMEM = "DIVI";
constexpr char TEXT_DIVO[]     PROGMEM = "DIVO";
// constexpr char TEXT_EDIV[]     PROGMEM = "EDIV";
// constexpr char TEXT_EMUL[]     PROGMEM = "EMUL";
constexpr char TEXT_EXTRACT[]  PROGMEM = "EXTRACT";
constexpr char TEXT_FAULTE[]   PROGMEM = "FAULTE";
constexpr char TEXT_FAULTG[]   PROGMEM = "FAULTG";
constexpr char TEXT_FAULTGE[]  PROGMEM = "FAULTGE";
constexpr char TEXT_FAULTL[]   PROGMEM = "FAULTL";
constexpr char TEXT_FAULTLE[]  PROGMEM = "FAULTLE";
constexpr char TEXT_FAULTNE[]  PROGMEM = "FAULTNE";
constexpr char TEXT_FAULTNO[]  PROGMEM = "FAULTNO";
constexpr char TEXT_FAULTO[]   PROGMEM = "FAULTO";
constexpr char TEXT_FLUSHREG[] PROGMEM = "FLUSHREG";
constexpr char TEXT_FMARK[]    PROGMEM = "FMARK";
// constexpr char TEXT_LD[]       PROGMEM = "LD";
// constexpr char TEXT_LDA[]      PROGMEM = "LDA";
constexpr char TEXT_LDIB[]     PROGMEM = "LDIB";
constexpr char TEXT_LDIS[]     PROGMEM = "LDIS";
constexpr char TEXT_LDL[]      PROGMEM = "LDL";
constexpr char TEXT_LDOB[]     PROGMEM = "LDOB";
constexpr char TEXT_LDOS[]     PROGMEM = "LDOS";
constexpr char TEXT_LDQ[]      PROGMEM = "LDQ";
constexpr char TEXT_LDT[]      PROGMEM = "LDT";
// constexpr char TEXT_MARK[]     PROGMEM = "MARK";
constexpr char TEXT_MODAC[]    PROGMEM = "MODAC";
constexpr char TEXT_MODI[]     PROGMEM = "MODI";
constexpr char TEXT_MODIFY[]   PROGMEM = "MODIFY";
constexpr char TEXT_MODPC[]    PROGMEM = "MODPC";
constexpr char TEXT_MODTC[]    PROGMEM = "MODTC";
// constexpr char TEXT_MOV[]      PROGMEM = "MOV";
constexpr char TEXT_MOVL[]     PROGMEM = "MOVL";
constexpr char TEXT_MOVQ[]     PROGMEM = "MOVQ";
constexpr char TEXT_MOVT[]     PROGMEM = "MOVT";
constexpr char TEXT_MULI[]     PROGMEM = "MULI";
constexpr char TEXT_MULO[]     PROGMEM = "MULO";
constexpr char TEXT_NAND[]     PROGMEM = "NAND";
constexpr char TEXT_NOR[]      PROGMEM = "NOR";
// constexpr char TEXT_NOT[]      PROGMEM = "NOT";
constexpr char TEXT_NOTAND[]   PROGMEM = "NOTAND";
constexpr char TEXT_NOTBIT[]   PROGMEM = "NOTBIT";
constexpr char TEXT_NOTOR[]    PROGMEM = "NOTOR";
// constexpr char TEXT_OR[]       PROGMEM = "OR";
constexpr char TEXT_ORNOT[]    PROGMEM = "ORNOT";
constexpr char TEXT_REMI[]     PROGMEM = "REMI";
constexpr char TEXT_REMO[]     PROGMEM = "REMO";
// constexpr char TEXT_RET[]      PROGMEM = "RET";
constexpr char TEXT_ROTATE[]   PROGMEM = "ROTATE";
constexpr char TEXT_SCANBIT[]  PROGMEM = "SCANBIT";
constexpr char TEXT_SCANBYTE[] PROGMEM = "SCANBYTE";
constexpr char TEXT_SETBIT[]   PROGMEM = "SETBIT";
constexpr char TEXT_SHLI[]     PROGMEM = "SHLI";
constexpr char TEXT_SHLO[]     PROGMEM = "SHLO";
constexpr char TEXT_SHRDI[]    PROGMEM = "SHRDI";
constexpr char TEXT_SHRI[]     PROGMEM = "SHRI";
constexpr char TEXT_SHRO[]     PROGMEM = "SHRO";
constexpr char TEXT_SPANBIT[]  PROGMEM = "SPANBIT";
// constexpr char TEXT_ST[]       PROGMEM = "ST";
constexpr char TEXT_STIB[]     PROGMEM = "STIB";
constexpr char TEXT_STIS[]     PROGMEM = "STIS";
constexpr char TEXT_STL[]      PROGMEM = "STL";
constexpr char TEXT_STOB[]     PROGMEM = "STOB";
constexpr char TEXT_STOS[]     PROGMEM = "STOS";
constexpr char TEXT_STQ[]      PROGMEM = "STQ";
constexpr char TEXT_STT[]      PROGMEM = "STT";
// constexpr char TEXT_SUBC[]     PROGMEM = "SUBC";
// constexpr char TEXT_SUBI[]     PROGMEM = "SUBI";
constexpr char TEXT_SUBO[]     PROGMEM = "SUBO";
constexpr char TEXT_SYNCF[]    PROGMEM = "SYNCF";
constexpr char TEXT_SYNLD[]    PROGMEM = "SYNLD";
constexpr char TEXT_SYNMOV[]   PROGMEM = "SYNMOV";
constexpr char TEXT_SYNMOVL[]  PROGMEM = "SYNMOVL";
constexpr char TEXT_SYNMOVQ[]  PROGMEM = "SYNMOVQ";
constexpr char TEXT_TESTE[]    PROGMEM = "TESTE";
constexpr char TEXT_TESTG[]    PROGMEM = "TESTG";
constexpr char TEXT_TESTGE[]   PROGMEM = "TESTGE";
constexpr char TEXT_TESTL[]    PROGMEM = "TESTL";
constexpr char TEXT_TESTLE[]   PROGMEM = "TESTLE";
constexpr char TEXT_TESTNE[]   PROGMEM = "TESTNE";
constexpr char TEXT_TESTNO[]   PROGMEM = "TESTNO";
constexpr char TEXT_TESTO[]    PROGMEM = "TESTO";
constexpr char TEXT_XNOR[]     PROGMEM = "XNOR";
// constexpr char TEXT_XOR[]      PROGMEM = "XOR";


// 80960KB floating-point and decimal extensions
// constexpr char TEXT_ADDR[]    PROGMEM = "ADDR";
constexpr char TEXT_ADDRL[]   PROGMEM = "ADDRL";
constexpr char TEXT_ATANR[]   PROGMEM = "ATANR";
constexpr char TEXT_ATANRL[]  PROGMEM = "ATANRL";
constexpr char TEXT_CLASSR[]  PROGMEM = "CLASSR";
constexpr char TEXT_CLASSRL[] PROGMEM = "CLASSRL";
constexpr char TEXT_CMPOR[]   PROGMEM = "CMPOR";
constexpr char TEXT_CMPORL[]  PROGMEM = "CMPORL";
// constexpr char TEXT_CMPR[]    PROGMEM = "CMPR";
constexpr char TEXT_CMPRL[]   PROGMEM = "CMPRL";
constexpr char TEXT_COSR[]    PROGMEM = "COSR";
constexpr char TEXT_COSRL[]   PROGMEM = "COSRL";
constexpr char TEXT_CPYRSRE[] PROGMEM = "CPYRSRE";
constexpr char TEXT_CPYSRE[]  PROGMEM = "CPYSRE";
constexpr char TEXT_CVTILR[]  PROGMEM = "CVTILR";
constexpr char TEXT_CVTIR[]   PROGMEM = "CVTIR";
constexpr char TEXT_CVTRI[]   PROGMEM = "CVTRI";
constexpr char TEXT_CVTRIL[]  PROGMEM = "CVTRIL";
constexpr char TEXT_CVTZRI[]  PROGMEM = "CVTZRI";
constexpr char TEXT_CVTZRIL[] PROGMEM = "CVTZRIL";
constexpr char TEXT_DADDC[]   PROGMEM = "DADDC";
constexpr char TEXT_DIVR[]    PROGMEM = "DIVR";
constexpr char TEXT_DIVRL[]   PROGMEM = "DIVRL";
constexpr char TEXT_DMOVT[]   PROGMEM = "DMOVT";
constexpr char TEXT_DSUBC[]   PROGMEM = "DSUBC";
constexpr char TEXT_EXPR[]    PROGMEM = "EXPR";
constexpr char TEXT_EXPRL[]   PROGMEM = "EXPRL";
constexpr char TEXT_LOGBNR[]  PROGMEM = "LOGBNR";
constexpr char TEXT_LOGBNRL[] PROGMEM = "LOGBNRL";
constexpr char TEXT_LOGEPR[]  PROGMEM = "LOGEPR";
constexpr char TEXT_LOGEPRL[] PROGMEM = "LOGEPRL";
constexpr char TEXT_LOGR[]    PROGMEM = "LOGR";
constexpr char TEXT_LOGRL[]   PROGMEM = "LOGRL";
constexpr char TEXT_MOVR[]    PROGMEM = "MOVR";
constexpr char TEXT_MOVRE[]   PROGMEM = "MOVRE";
constexpr char TEXT_MOVRL[]   PROGMEM = "MOVRL";
constexpr char TEXT_MULR[]    PROGMEM = "MULR";
constexpr char TEXT_MULRL[]   PROGMEM = "MULRL";
constexpr char TEXT_REMR[]    PROGMEM = "REMR";
constexpr char TEXT_REMRL[]   PROGMEM = "REMRL";
constexpr char TEXT_ROUNDR[]  PROGMEM = "ROUNDR";
constexpr char TEXT_ROUNDRL[] PROGMEM = "ROUNDRL";
constexpr char TEXT_SCALER[]  PROGMEM = "SCALER";
constexpr char TEXT_SCALERL[] PROGMEM = "SCALERL";
constexpr char TEXT_SINR[]    PROGMEM = "SINR";
constexpr char TEXT_SINRL[]   PROGMEM = "SINRL";
constexpr char TEXT_SQRTR[]   PROGMEM = "SQRTR";
constexpr char TEXT_SQRTRL[]  PROGMEM = "SQRTRL";
// constexpr char TEXT_SUBR[]    PROGMEM = "SUBR";
constexpr char TEXT_SUBRL[]   PROGMEM = "SUBRL";
constexpr char TEXT_TANR[]    PROGMEM = "TANR";
constexpr char TEXT_TANRL[]   PROGMEM = "TANRL";

// clang-format on
}  // namespace i960
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
