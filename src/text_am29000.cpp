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

#include "text_am29000.h"

namespace libasm {
namespace text {
namespace am29000 {
// clang-format off

constexpr char TEXT_AM29000_LIST[] PROGMEM = "AM29030, AM29040, AM29050, AM29200, AM29240, AM29245";
constexpr char TEXT_CPU_AM29030[]  PROGMEM = "AM29030";
constexpr char TEXT_CPU_AM29040[]  PROGMEM = "AM29040";
constexpr char TEXT_CPU_AM29050[]  PROGMEM = "AM29050";
constexpr char TEXT_CPU_AM29200[]  PROGMEM = "AM29200";
constexpr char TEXT_CPU_AM29240[]  PROGMEM = "AM29240";
constexpr char TEXT_CPU_AM29245[]  PROGMEM = "AM29245";

// Am29000
// constexpr char TEXT_ADD[]     PROGMEM = "ADD";
// constexpr char TEXT_ADDC[]    PROGMEM = "ADDC";
constexpr char TEXT_ADDCS[]   PROGMEM = "ADDCS";
constexpr char TEXT_ADDCU[]   PROGMEM = "ADDCU";
// constexpr char TEXT_ADDS[]    PROGMEM = "ADDS";
constexpr char TEXT_ADDU[]    PROGMEM = "ADDU";
// constexpr char TEXT_AND[]     PROGMEM = "AND";
constexpr char TEXT_ANDN[]    PROGMEM = "ANDN";
constexpr char TEXT_ASEQ[]    PROGMEM = "ASEQ";
constexpr char TEXT_ASGE[]    PROGMEM = "ASGE";
constexpr char TEXT_ASGEU[]   PROGMEM = "ASGEU";
constexpr char TEXT_ASGT[]    PROGMEM = "ASGT";
constexpr char TEXT_ASGTU[]   PROGMEM = "ASGTU";
constexpr char TEXT_ASLE[]    PROGMEM = "ASLE";
constexpr char TEXT_ASLEU[]   PROGMEM = "ASLEU";
constexpr char TEXT_ASLT[]    PROGMEM = "ASLT";
constexpr char TEXT_ASLTU[]   PROGMEM = "ASLTU";
constexpr char TEXT_ASNEQ[]   PROGMEM = "ASNEQ";
// constexpr char TEXT_CALL[]    PROGMEM = "CALL";
constexpr char TEXT_CALLI[]   PROGMEM = "CALLI";
constexpr char TEXT_CLASS[]   PROGMEM = "CLASS";
constexpr char TEXT_CLZ[]     PROGMEM = "CLZ";
constexpr char TEXT_CONST[]   PROGMEM = "CONST";
constexpr char TEXT_CONSTH[]  PROGMEM = "CONSTH";
constexpr char TEXT_CONSTN[]  PROGMEM = "CONSTN";
constexpr char TEXT_CONVERT[] PROGMEM = "CONVERT";
constexpr char TEXT_CPBYTE[]  PROGMEM = "CPBYTE";
constexpr char TEXT_CPEQ[]    PROGMEM = "CPEQ";
constexpr char TEXT_CPGE[]    PROGMEM = "CPGE";
constexpr char TEXT_CPGEU[]   PROGMEM = "CPGEU";
constexpr char TEXT_CPGT[]    PROGMEM = "CPGT";
constexpr char TEXT_CPGTU[]   PROGMEM = "CPGTU";
constexpr char TEXT_CPLE[]    PROGMEM = "CPLE";
constexpr char TEXT_CPLEU[]   PROGMEM = "CPLEU";
constexpr char TEXT_CPLT[]    PROGMEM = "CPLT";
constexpr char TEXT_CPLTU[]   PROGMEM = "CPLTU";
constexpr char TEXT_CPNEQ[]   PROGMEM = "CPNEQ";
// constexpr char TEXT_DADD[]    PROGMEM = "DADD";
constexpr char TEXT_DDIV[]    PROGMEM = "DDIV";
constexpr char TEXT_DEQ[]     PROGMEM = "DEQ";
constexpr char TEXT_DGE[]     PROGMEM = "DGE";
constexpr char TEXT_DGT[]     PROGMEM = "DGT";
constexpr char TEXT_DMAC[]    PROGMEM = "DMAC";
constexpr char TEXT_DMSM[]    PROGMEM = "DMSM";
constexpr char TEXT_DMUL[]    PROGMEM = "DMUL";
constexpr char TEXT_DSUB[]    PROGMEM = "DSUB";
// constexpr char TEXT_DIV[]     PROGMEM = "DIV";
constexpr char TEXT_DIV0[]    PROGMEM = "DIV0";
constexpr char TEXT_DIVIDE[]  PROGMEM = "DIVIDE";
constexpr char TEXT_DIVIDU[]  PROGMEM = "DIVIDU";
// constexpr char TEXT_DIVL[]    PROGMEM = "DIVL";
constexpr char TEXT_DIVREM[]  PROGMEM = "DIVREM";
constexpr char TEXT_EMULATE[] PROGMEM = "EMULATE";
constexpr char TEXT_EXBYTE[]  PROGMEM = "EXBYTE";
constexpr char TEXT_EXHW[]    PROGMEM = "EXHW";
constexpr char TEXT_EXHWS[]   PROGMEM = "EXHWS";
constexpr char TEXT_EXTRACT[] PROGMEM = "EXTRACT";
// constexpr char TEXT_FADD[]    PROGMEM = "FADD";
// constexpr char TEXT_FDIV[]    PROGMEM = "FDIV";
constexpr char TEXT_FDMUL[]   PROGMEM = "FDMUL";
constexpr char TEXT_FEQ[]     PROGMEM = "FEQ";
constexpr char TEXT_FGE[]     PROGMEM = "FGE";
constexpr char TEXT_FGT[]     PROGMEM = "FGT";
constexpr char TEXT_FMAC[]    PROGMEM = "FMAC";
constexpr char TEXT_FMSM[]    PROGMEM = "FMSM";
// constexpr char TEXT_FMUL[]    PROGMEM = "FMUL";
// constexpr char TEXT_FSUB[]    PROGMEM = "FSUB";
// constexpr char TEXT_HALT[]    PROGMEM = "HALT";
constexpr char TEXT_INBYTE[]  PROGMEM = "INBYTE";
constexpr char TEXT_INHW[]    PROGMEM = "INHW";
// constexpr char TEXT_INV[]     PROGMEM = "INV";
// constexpr char TEXT_IRET[]    PROGMEM = "IRET";
constexpr char TEXT_IRETINV[] PROGMEM = "IRETINV";
// constexpr char TEXT_JMP[]     PROGMEM = "JMP";
constexpr char TEXT_JMPF[]    PROGMEM = "JMPF";
constexpr char TEXT_JMPFDEC[] PROGMEM = "JMPFDEC";
constexpr char TEXT_JMPFI[]   PROGMEM = "JMPFI";
constexpr char TEXT_JMPI[]    PROGMEM = "JMPI";
constexpr char TEXT_JMPT[]    PROGMEM = "JMPT";
constexpr char TEXT_JMPTI[]   PROGMEM = "JMPTI";
constexpr char TEXT_LOAD[]    PROGMEM = "LOAD";
constexpr char TEXT_LOADL[]   PROGMEM = "LOADL";
constexpr char TEXT_LOADM[]   PROGMEM = "LOADM";
constexpr char TEXT_LOADSET[] PROGMEM = "LOADSET";
constexpr char TEXT_MFACC[]   PROGMEM = "MFACC";
constexpr char TEXT_MFSR[]    PROGMEM = "MFSR";
constexpr char TEXT_MFTLB[]   PROGMEM = "MFTLB";
constexpr char TEXT_MTACC[]   PROGMEM = "MTACC";
constexpr char TEXT_MTSR[]    PROGMEM = "MTSR";
constexpr char TEXT_MTSRIM[]  PROGMEM = "MTSRIM";
constexpr char TEXT_MTTLB[]   PROGMEM = "MTTLB";
// constexpr char TEXT_MUL[]     PROGMEM = "MUL";
constexpr char TEXT_MULL[]    PROGMEM = "MULL";
constexpr char TEXT_MULTIPLU[] PROGMEM = "MULTIPLU";
constexpr char TEXT_MULTIPLY[] PROGMEM = "MULTIPLY";
constexpr char TEXT_MULTM[]   PROGMEM = "MULTM";
constexpr char TEXT_MULTMU[]  PROGMEM = "MULTMU";
// constexpr char TEXT_MULU[]    PROGMEM = "MULU";
constexpr char TEXT_NAND[]    PROGMEM = "NAND";
constexpr char TEXT_NOR[]     PROGMEM = "NOR";
// constexpr char TEXT_OR[]      PROGMEM = "OR";
constexpr char TEXT_ORN[]     PROGMEM = "ORN";
constexpr char TEXT_SETIP[]   PROGMEM = "SETIP";
// constexpr char TEXT_SLL[]     PROGMEM = "SLL";
constexpr char TEXT_SQRT[]    PROGMEM = "SQRT";
// constexpr char TEXT_SRA[]     PROGMEM = "SRA";
// constexpr char TEXT_SRL[]     PROGMEM = "SRL";
constexpr char TEXT_STORE[]   PROGMEM = "STORE";
constexpr char TEXT_STOREL[]  PROGMEM = "STOREL";
constexpr char TEXT_STOREM[]  PROGMEM = "STOREM";
// constexpr char TEXT_SUB[]     PROGMEM = "SUB";
// constexpr char TEXT_SUBC[]    PROGMEM = "SUBC";
constexpr char TEXT_SUBCS[]   PROGMEM = "SUBCS";
constexpr char TEXT_SUBCU[]   PROGMEM = "SUBCU";
// constexpr char TEXT_SUBR[]    PROGMEM = "SUBR";
constexpr char TEXT_SUBRC[]   PROGMEM = "SUBRC";
constexpr char TEXT_SUBRCS[]  PROGMEM = "SUBRCS";
constexpr char TEXT_SUBRCU[]  PROGMEM = "SUBRCU";
constexpr char TEXT_SUBRS[]   PROGMEM = "SUBRS";
constexpr char TEXT_SUBRU[]   PROGMEM = "SUBRU";
// constexpr char TEXT_SUBS[]    PROGMEM = "SUBS";
constexpr char TEXT_SUBU[]    PROGMEM = "SUBU";
// constexpr char TEXT_XOR[]     PROGMEM = "XOR";
constexpr char TEXT_XNOR[]    PROGMEM = "XNOR";

// Options
constexpr char OPT_BOOL_FPU[]     PROGMEM = "fpu";
constexpr char OPT_DESC_FPU[]     PROGMEM = "enable floating point instructions";
constexpr char OPT_BOOL_INTMULT[] PROGMEM = "intmult";
constexpr char OPT_DESC_INTMULT[] PROGMEM = "enable integer multiply instructions";

// Special-purpose registers
constexpr char TEXT_REG_VAB[]  PROGMEM = "VAB";
constexpr char TEXT_REG_OPS[]  PROGMEM = "OPS";
constexpr char TEXT_REG_CPS[]  PROGMEM = "CPS";
constexpr char TEXT_REG_CFG[]  PROGMEM = "CFG";
constexpr char TEXT_REG_CHA[]  PROGMEM = "CHA";
constexpr char TEXT_REG_CHD[]  PROGMEM = "CHD";
constexpr char TEXT_REG_CHC[]  PROGMEM = "CHC";
constexpr char TEXT_REG_RBP[]  PROGMEM = "RBP";
constexpr char TEXT_REG_TMC[]  PROGMEM = "TMC";
constexpr char TEXT_REG_TMR[]  PROGMEM = "TMR";
constexpr char TEXT_REG_PC0[]  PROGMEM = "PC0";
constexpr char TEXT_REG_PC1[]  PROGMEM = "PC1";
constexpr char TEXT_REG_PC2[]  PROGMEM = "PC2";
constexpr char TEXT_REG_MMU[]  PROGMEM = "MMU";
constexpr char TEXT_REG_LRU[]  PROGMEM = "LRU";
constexpr char TEXT_REG_RSN[]  PROGMEM = "RSN";
constexpr char TEXT_REG_RMA0[] PROGMEM = "RMA0";
constexpr char TEXT_REG_RMC0[] PROGMEM = "RMC0";
constexpr char TEXT_REG_RMA1[] PROGMEM = "RMA1";
constexpr char TEXT_REG_RMC1[] PROGMEM = "RMC1";
constexpr char TEXT_REG_SPC0[] PROGMEM = "SPC0";
constexpr char TEXT_REG_SPC1[] PROGMEM = "SPC1";
constexpr char TEXT_REG_SPC2[] PROGMEM = "SPC2";
constexpr char TEXT_REG_IBA0[] PROGMEM = "IBA0";
constexpr char TEXT_REG_IBC0[] PROGMEM = "IBC0";
constexpr char TEXT_REG_IBA1[] PROGMEM = "IBA1";
constexpr char TEXT_REG_IBC1[] PROGMEM = "IBC1";
constexpr char TEXT_REG_EXOP[] PROGMEM = "EXOP";
constexpr char TEXT_REG_CIR[]  PROGMEM = "CIR";
constexpr char TEXT_REG_CDR[]  PROGMEM = "CDR";
constexpr char TEXT_REG_IPC[]  PROGMEM = "IPC";
constexpr char TEXT_REG_IPA[]  PROGMEM = "IPA";
constexpr char TEXT_REG_IPB[]  PROGMEM = "IPB";
constexpr char TEXT_REG_Q[]    PROGMEM = "Q";
constexpr char TEXT_REG_ALU[]  PROGMEM = "ALU";
constexpr char TEXT_REG_BP[]   PROGMEM = "BP";
constexpr char TEXT_REG_FC[]   PROGMEM = "FC";
constexpr char TEXT_REG_CR[]   PROGMEM = "CR";
constexpr char TEXT_REG_FPE[]  PROGMEM = "FPE";
constexpr char TEXT_REG_INTE[] PROGMEM = "INTE";
constexpr char TEXT_REG_FPS[]  PROGMEM = "FPS";

// clang-format on
}  // namespace am29000
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
