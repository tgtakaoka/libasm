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

constexpr char TEXT_I960_LIST[] = "80960";
constexpr char TEXT_CPU_80960[] = "80960";
constexpr char OPT_BOOL_MODE_BITS[] = "mode-bits";
constexpr char OPT_DESC_MODE_BITS[] = "value of the mode bit of an unused operand field";
constexpr char OPT_TEXT_IP_OPERAND[] = "ip-operand";
constexpr char OPT_DESC_IP_OPERAND[] = "IP-relative operand: offset (default) or target";
constexpr char OPT_BOOL_CANONICAL_MEM[] = "canonical-mem";
constexpr char OPT_BOOL_FPU[] = "fpu";
constexpr char OPT_DESC_FPU[] = "enable 80960KB floating point and decimal instructions";
constexpr char TEXT_REG_PFP[] = "PFP";
constexpr char TEXT_REG_RIP[] = "RIP";
constexpr char TEXT_REG_IP[]  = "IP";
// constexpr char TEXT_REG_FP[]  = "FP";
// constexpr char TEXT_REG_SP[]  = "SP";

// i960
// constexpr char TEXT_ADDC[]     = "ADDC";
// constexpr char TEXT_ADDI[]     = "ADDI";
constexpr char TEXT_ADDO[]     = "ADDO";
constexpr char TEXT_ALTERBIT[] = "ALTERBIT";
// constexpr char TEXT_AND[]      = "AND";
constexpr char TEXT_ANDNOT[]   = "ANDNOT";
constexpr char TEXT_ATADD[]    = "ATADD";
constexpr char TEXT_ATMOD[]    = "ATMOD";
// constexpr char TEXT_B[]        = "B";
constexpr char TEXT_BAL[]      = "BAL";
constexpr char TEXT_BALX[]     = "BALX";
constexpr char TEXT_BBC[]      = "BBC";
constexpr char TEXT_BBS[]      = "BBS";
constexpr char TEXT_BE[]       = "BE";
constexpr char TEXT_BG[]       = "BG";
// constexpr char TEXT_BGE[]      = "BGE";
// constexpr char TEXT_BL[]       = "BL";
// constexpr char TEXT_BLE[]      = "BLE";
// constexpr char TEXT_BNE[]      = "BNE";
constexpr char TEXT_BNO[]      = "BNO";
constexpr char TEXT_BO[]       = "BO";
constexpr char TEXT_BX[]       = "BX";
// constexpr char TEXT_CALL[]     = "CALL";
// constexpr char TEXT_CALLS[]    = "CALLS";
constexpr char TEXT_CALLX[]    = "CALLX";
constexpr char TEXT_CHKBIT[]   = "CHKBIT";
constexpr char TEXT_CLRBIT[]   = "CLRBIT";
constexpr char TEXT_CMPDECI[]  = "CMPDECI";
constexpr char TEXT_CMPDECO[]  = "CMPDECO";
// constexpr char TEXT_CMPI[]     = "CMPI";
constexpr char TEXT_CMPIBE[]   = "CMPIBE";
constexpr char TEXT_CMPIBG[]   = "CMPIBG";
constexpr char TEXT_CMPIBGE[]  = "CMPIBGE";
constexpr char TEXT_CMPIBL[]   = "CMPIBL";
constexpr char TEXT_CMPIBLE[]  = "CMPIBLE";
constexpr char TEXT_CMPIBNE[]  = "CMPIBNE";
constexpr char TEXT_CMPIBNO[]  = "CMPIBNO";
constexpr char TEXT_CMPIBO[]   = "CMPIBO";
constexpr char TEXT_CMPINCI[]  = "CMPINCI";
constexpr char TEXT_CMPINCO[]  = "CMPINCO";
constexpr char TEXT_CMPO[]     = "CMPO";
constexpr char TEXT_CMPOBE[]   = "CMPOBE";
constexpr char TEXT_CMPOBG[]   = "CMPOBG";
constexpr char TEXT_CMPOBGE[]  = "CMPOBGE";
constexpr char TEXT_CMPOBL[]   = "CMPOBL";
constexpr char TEXT_CMPOBLE[]  = "CMPOBLE";
constexpr char TEXT_CMPOBNE[]  = "CMPOBNE";
constexpr char TEXT_CONCMPI[]  = "CONCMPI";
constexpr char TEXT_CONCMPO[]  = "CONCMPO";
constexpr char TEXT_DIVI[]     = "DIVI";
constexpr char TEXT_DIVO[]     = "DIVO";
// constexpr char TEXT_EDIV[]     = "EDIV";
// constexpr char TEXT_EMUL[]     = "EMUL";
constexpr char TEXT_EXTRACT[]  = "EXTRACT";
constexpr char TEXT_FAULTE[]   = "FAULTE";
constexpr char TEXT_FAULTG[]   = "FAULTG";
constexpr char TEXT_FAULTGE[]  = "FAULTGE";
constexpr char TEXT_FAULTL[]   = "FAULTL";
constexpr char TEXT_FAULTLE[]  = "FAULTLE";
constexpr char TEXT_FAULTNE[]  = "FAULTNE";
constexpr char TEXT_FAULTNO[]  = "FAULTNO";
constexpr char TEXT_FAULTO[]   = "FAULTO";
constexpr char TEXT_FLUSHREG[] = "FLUSHREG";
constexpr char TEXT_FMARK[]    = "FMARK";
// constexpr char TEXT_LD[]       = "LD";
// constexpr char TEXT_LDA[]      = "LDA";
constexpr char TEXT_LDIB[]     = "LDIB";
constexpr char TEXT_LDIS[]     = "LDIS";
constexpr char TEXT_LDL[]      = "LDL";
constexpr char TEXT_LDOB[]     = "LDOB";
constexpr char TEXT_LDOS[]     = "LDOS";
constexpr char TEXT_LDQ[]      = "LDQ";
constexpr char TEXT_LDT[]      = "LDT";
// constexpr char TEXT_MARK[]     = "MARK";
constexpr char TEXT_MODAC[]    = "MODAC";
constexpr char TEXT_MODI[]     = "MODI";
constexpr char TEXT_MODIFY[]   = "MODIFY";
constexpr char TEXT_MODPC[]    = "MODPC";
constexpr char TEXT_MODTC[]    = "MODTC";
// constexpr char TEXT_MOV[]      = "MOV";
constexpr char TEXT_MOVL[]     = "MOVL";
constexpr char TEXT_MOVQ[]     = "MOVQ";
constexpr char TEXT_MOVT[]     = "MOVT";
constexpr char TEXT_MULI[]     = "MULI";
constexpr char TEXT_MULO[]     = "MULO";
constexpr char TEXT_NAND[]     = "NAND";
constexpr char TEXT_NOR[]      = "NOR";
// constexpr char TEXT_NOT[]      = "NOT";
constexpr char TEXT_NOTAND[]   = "NOTAND";
constexpr char TEXT_NOTBIT[]   = "NOTBIT";
constexpr char TEXT_NOTOR[]    = "NOTOR";
// constexpr char TEXT_OR[]       = "OR";
constexpr char TEXT_ORNOT[]    = "ORNOT";
constexpr char TEXT_REMI[]     = "REMI";
constexpr char TEXT_REMO[]     = "REMO";
// constexpr char TEXT_RET[]      = "RET";
constexpr char TEXT_ROTATE[]   = "ROTATE";
constexpr char TEXT_SCANBIT[]  = "SCANBIT";
constexpr char TEXT_SCANBYTE[] = "SCANBYTE";
constexpr char TEXT_SETBIT[]   = "SETBIT";
constexpr char TEXT_SHLI[]     = "SHLI";
constexpr char TEXT_SHLO[]     = "SHLO";
constexpr char TEXT_SHRDI[]    = "SHRDI";
constexpr char TEXT_SHRI[]     = "SHRI";
constexpr char TEXT_SHRO[]     = "SHRO";
constexpr char TEXT_SPANBIT[]  = "SPANBIT";
// constexpr char TEXT_ST[]       = "ST";
constexpr char TEXT_STIB[]     = "STIB";
constexpr char TEXT_STIS[]     = "STIS";
constexpr char TEXT_STL[]      = "STL";
constexpr char TEXT_STOB[]     = "STOB";
constexpr char TEXT_STOS[]     = "STOS";
constexpr char TEXT_STQ[]      = "STQ";
constexpr char TEXT_STT[]      = "STT";
// constexpr char TEXT_SUBC[]     = "SUBC";
// constexpr char TEXT_SUBI[]     = "SUBI";
constexpr char TEXT_SUBO[]     = "SUBO";
constexpr char TEXT_SYNCF[]    = "SYNCF";
constexpr char TEXT_SYNLD[]    = "SYNLD";
constexpr char TEXT_SYNMOV[]   = "SYNMOV";
constexpr char TEXT_SYNMOVL[]  = "SYNMOVL";
constexpr char TEXT_SYNMOVQ[]  = "SYNMOVQ";
constexpr char TEXT_TESTE[]    = "TESTE";
constexpr char TEXT_TESTG[]    = "TESTG";
constexpr char TEXT_TESTGE[]   = "TESTGE";
constexpr char TEXT_TESTL[]    = "TESTL";
constexpr char TEXT_TESTLE[]   = "TESTLE";
constexpr char TEXT_TESTNE[]   = "TESTNE";
constexpr char TEXT_TESTNO[]   = "TESTNO";
constexpr char TEXT_TESTO[]    = "TESTO";
constexpr char TEXT_XNOR[]     = "XNOR";
// constexpr char TEXT_XOR[]      = "XOR";


// 80960KB floating-point and decimal extensions
// constexpr char TEXT_ADDR[]    = "ADDR";
constexpr char TEXT_ADDRL[]   = "ADDRL";
constexpr char TEXT_ATANR[]   = "ATANR";
constexpr char TEXT_ATANRL[]  = "ATANRL";
constexpr char TEXT_CLASSR[]  = "CLASSR";
constexpr char TEXT_CLASSRL[] = "CLASSRL";
constexpr char TEXT_CMPOR[]   = "CMPOR";
constexpr char TEXT_CMPORL[]  = "CMPORL";
// constexpr char TEXT_CMPR[]    = "CMPR";
constexpr char TEXT_CMPRL[]   = "CMPRL";
constexpr char TEXT_COSR[]    = "COSR";
constexpr char TEXT_COSRL[]   = "COSRL";
constexpr char TEXT_CPYRSRE[] = "CPYRSRE";
constexpr char TEXT_CPYSRE[]  = "CPYSRE";
constexpr char TEXT_CVTILR[]  = "CVTILR";
constexpr char TEXT_CVTIR[]   = "CVTIR";
constexpr char TEXT_CVTRI[]   = "CVTRI";
constexpr char TEXT_CVTRIL[]  = "CVTRIL";
constexpr char TEXT_CVTZRI[]  = "CVTZRI";
constexpr char TEXT_CVTZRIL[] = "CVTZRIL";
constexpr char TEXT_DADDC[]   = "DADDC";
constexpr char TEXT_DIVR[]    = "DIVR";
constexpr char TEXT_DIVRL[]   = "DIVRL";
constexpr char TEXT_DMOVT[]   = "DMOVT";
constexpr char TEXT_DSUBC[]   = "DSUBC";
constexpr char TEXT_EXPR[]    = "EXPR";
constexpr char TEXT_EXPRL[]   = "EXPRL";
constexpr char TEXT_LOGBNR[]  = "LOGBNR";
constexpr char TEXT_LOGBNRL[] = "LOGBNRL";
constexpr char TEXT_LOGEPR[]  = "LOGEPR";
constexpr char TEXT_LOGEPRL[] = "LOGEPRL";
constexpr char TEXT_LOGR[]    = "LOGR";
constexpr char TEXT_LOGRL[]   = "LOGRL";
constexpr char TEXT_MOVR[]    = "MOVR";
constexpr char TEXT_MOVRE[]   = "MOVRE";
constexpr char TEXT_MOVRL[]   = "MOVRL";
constexpr char TEXT_MULR[]    = "MULR";
constexpr char TEXT_MULRL[]   = "MULRL";
constexpr char TEXT_REMR[]    = "REMR";
constexpr char TEXT_REMRL[]   = "REMRL";
constexpr char TEXT_ROUNDR[]  = "ROUNDR";
constexpr char TEXT_ROUNDRL[] = "ROUNDRL";
constexpr char TEXT_SCALER[]  = "SCALER";
constexpr char TEXT_SCALERL[] = "SCALERL";
constexpr char TEXT_SINR[]    = "SINR";
constexpr char TEXT_SINRL[]   = "SINRL";
constexpr char TEXT_SQRTR[]   = "SQRTR";
constexpr char TEXT_SQRTRL[]  = "SQRTRL";
// constexpr char TEXT_SUBR[]    = "SUBR";
constexpr char TEXT_SUBRL[]   = "SUBRL";
constexpr char TEXT_TANR[]    = "TANR";
constexpr char TEXT_TANRL[]   = "TANRL";

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
