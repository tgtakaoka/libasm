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

constexpr char TEXT_AM29000_LIST[] = "AM29030, AM29040, AM29050, AM29200, AM29240, AM29245";
constexpr char TEXT_CPU_AM29030[]  = "AM29030";
constexpr char TEXT_CPU_AM29040[]  = "AM29040";
constexpr char TEXT_CPU_AM29050[]  = "AM29050";
constexpr char TEXT_CPU_AM29200[]  = "AM29200";
constexpr char TEXT_CPU_AM29240[]  = "AM29240";
constexpr char TEXT_CPU_AM29245[]  = "AM29245";

// Am29000
// constexpr char TEXT_ADD[]     = "ADD";
// constexpr char TEXT_ADDC[]    = "ADDC";
constexpr char TEXT_ADDCS[]   = "ADDCS";
constexpr char TEXT_ADDCU[]   = "ADDCU";
// constexpr char TEXT_ADDS[]    = "ADDS";
constexpr char TEXT_ADDU[]    = "ADDU";
// constexpr char TEXT_AND[]     = "AND";
constexpr char TEXT_ANDN[]    = "ANDN";
constexpr char TEXT_ASEQ[]    = "ASEQ";
constexpr char TEXT_ASGE[]    = "ASGE";
constexpr char TEXT_ASGEU[]   = "ASGEU";
constexpr char TEXT_ASGT[]    = "ASGT";
constexpr char TEXT_ASGTU[]   = "ASGTU";
constexpr char TEXT_ASLE[]    = "ASLE";
constexpr char TEXT_ASLEU[]   = "ASLEU";
constexpr char TEXT_ASLT[]    = "ASLT";
constexpr char TEXT_ASLTU[]   = "ASLTU";
constexpr char TEXT_ASNEQ[]   = "ASNEQ";
// constexpr char TEXT_CALL[]    = "CALL";
// constexpr char TEXT_CALLI[]   = "CALLI";
constexpr char TEXT_CLASS[]   = "CLASS";
constexpr char TEXT_CLZ[]     = "CLZ";
constexpr char TEXT_CONST[]   = "CONST";
constexpr char TEXT_CONSTH[]  = "CONSTH";
constexpr char TEXT_CONSTHZ[] = "CONSTHZ";
constexpr char TEXT_CONSTN[]  = "CONSTN";
constexpr char TEXT_CONVERT[] = "CONVERT";
constexpr char TEXT_CPBYTE[]  = "CPBYTE";
constexpr char TEXT_CPEQ[]    = "CPEQ";
constexpr char TEXT_CPGE[]    = "CPGE";
constexpr char TEXT_CPGEU[]   = "CPGEU";
constexpr char TEXT_CPGT[]    = "CPGT";
constexpr char TEXT_CPGTU[]   = "CPGTU";
constexpr char TEXT_CPLE[]    = "CPLE";
constexpr char TEXT_CPLEU[]   = "CPLEU";
constexpr char TEXT_CPLT[]    = "CPLT";
constexpr char TEXT_CPLTU[]   = "CPLTU";
constexpr char TEXT_CPNEQ[]   = "CPNEQ";
// constexpr char TEXT_DADD[]    = "DADD";
constexpr char TEXT_DDIV[]    = "DDIV";
constexpr char TEXT_DEQ[]     = "DEQ";
constexpr char TEXT_DGE[]     = "DGE";
constexpr char TEXT_DGT[]     = "DGT";
constexpr char TEXT_DMAC[]    = "DMAC";
constexpr char TEXT_DMSM[]    = "DMSM";
constexpr char TEXT_DMUL[]    = "DMUL";
constexpr char TEXT_DSUB[]    = "DSUB";
// constexpr char TEXT_DIV[]     = "DIV";
constexpr char TEXT_DIV0[]    = "DIV0";
constexpr char TEXT_DIVIDE[]  = "DIVIDE";
constexpr char TEXT_DIVIDU[]  = "DIVIDU";
// constexpr char TEXT_DIVL[]    = "DIVL";
constexpr char TEXT_DIVREM[]  = "DIVREM";
constexpr char TEXT_EMULATE[] = "EMULATE";
constexpr char TEXT_EXBYTE[]  = "EXBYTE";
constexpr char TEXT_EXHW[]    = "EXHW";
constexpr char TEXT_EXHWS[]   = "EXHWS";
constexpr char TEXT_EXTRACT[] = "EXTRACT";
// constexpr char TEXT_FADD[]    = "FADD";
// constexpr char TEXT_FDIV[]    = "FDIV";
constexpr char TEXT_FDMUL[]   = "FDMUL";
constexpr char TEXT_FEQ[]     = "FEQ";
constexpr char TEXT_FGE[]     = "FGE";
constexpr char TEXT_FGT[]     = "FGT";
constexpr char TEXT_FMAC[]    = "FMAC";
constexpr char TEXT_FMSM[]    = "FMSM";
// constexpr char TEXT_FMUL[]    = "FMUL";
// constexpr char TEXT_FSUB[]    = "FSUB";
// constexpr char TEXT_HALT[]    = "HALT";
constexpr char TEXT_INBYTE[]  = "INBYTE";
constexpr char TEXT_INHW[]    = "INHW";
// constexpr char TEXT_INV[]     = "INV";
// constexpr char TEXT_IRET[]    = "IRET";
constexpr char TEXT_IRETINV[] = "IRETINV";
// constexpr char TEXT_JMP[]     = "JMP";
constexpr char TEXT_JMPF[]    = "JMPF";
constexpr char TEXT_JMPFDEC[] = "JMPFDEC";
constexpr char TEXT_JMPFI[]   = "JMPFI";
// constexpr char TEXT_JMPI[]    = "JMPI";
constexpr char TEXT_JMPT[]    = "JMPT";
constexpr char TEXT_JMPTI[]   = "JMPTI";
constexpr char TEXT_LOAD[]    = "LOAD";
constexpr char TEXT_LOADL[]   = "LOADL";
constexpr char TEXT_LOADM[]   = "LOADM";
constexpr char TEXT_LOADSET[] = "LOADSET";
constexpr char TEXT_MFACC[]   = "MFACC";
constexpr char TEXT_MFSR[]    = "MFSR";
constexpr char TEXT_MFTLB[]   = "MFTLB";
constexpr char TEXT_MTACC[]   = "MTACC";
constexpr char TEXT_MTSR[]    = "MTSR";
constexpr char TEXT_MTSRIM[]  = "MTSRIM";
constexpr char TEXT_MTTLB[]   = "MTTLB";
// constexpr char TEXT_MUL[]     = "MUL";
constexpr char TEXT_MULL[]    = "MULL";
constexpr char TEXT_MULTIPLU[] = "MULTIPLU";
constexpr char TEXT_MULTIPLY[] = "MULTIPLY";
constexpr char TEXT_MULTM[]   = "MULTM";
constexpr char TEXT_MULTMU[]  = "MULTMU";
// constexpr char TEXT_MULU[]    = "MULU";
constexpr char TEXT_NAND[]    = "NAND";
constexpr char TEXT_NOR[]     = "NOR";
// constexpr char TEXT_OR[]      = "OR";
constexpr char TEXT_ORN[]     = "ORN";
constexpr char TEXT_SETIP[]   = "SETIP";
// constexpr char TEXT_SLL[]     = "SLL";
constexpr char TEXT_SQRT[]    = "SQRT";
// constexpr char TEXT_SRA[]     = "SRA";
// constexpr char TEXT_SRL[]     = "SRL";
constexpr char TEXT_STORE[]   = "STORE";
constexpr char TEXT_STOREL[]  = "STOREL";
constexpr char TEXT_STOREM[]  = "STOREM";
// constexpr char TEXT_SUB[]     = "SUB";
// constexpr char TEXT_SUBC[]    = "SUBC";
constexpr char TEXT_SUBCS[]   = "SUBCS";
constexpr char TEXT_SUBCU[]   = "SUBCU";
// constexpr char TEXT_SUBR[]    = "SUBR";
constexpr char TEXT_SUBRC[]   = "SUBRC";
constexpr char TEXT_SUBRCS[]  = "SUBRCS";
constexpr char TEXT_SUBRCU[]  = "SUBRCU";
constexpr char TEXT_SUBRS[]   = "SUBRS";
constexpr char TEXT_SUBRU[]   = "SUBRU";
// constexpr char TEXT_SUBS[]    = "SUBS";
constexpr char TEXT_SUBU[]    = "SUBU";
// constexpr char TEXT_XOR[]     = "XOR";
constexpr char TEXT_XNOR[]    = "XNOR";

// Options
constexpr char OPT_BOOL_FPU[]     = "fpu";
constexpr char OPT_DESC_FPU[]     = "enable floating point instructions";
constexpr char OPT_BOOL_INTMULT[] = "intmult";
constexpr char OPT_DESC_INTMULT[] = "enable integer multiply instructions";

// Special-purpose registers
constexpr char TEXT_REG_VAB[]  = "VAB";
constexpr char TEXT_REG_OPS[]  = "OPS";
constexpr char TEXT_REG_CPS[]  = "CPS";
constexpr char TEXT_REG_CFG[]  = "CFG";
constexpr char TEXT_REG_CHA[]  = "CHA";
constexpr char TEXT_REG_CHD[]  = "CHD";
constexpr char TEXT_REG_CHC[]  = "CHC";
constexpr char TEXT_REG_RBP[]  = "RBP";
constexpr char TEXT_REG_TMC[]  = "TMC";
constexpr char TEXT_REG_TMR[]  = "TMR";
constexpr char TEXT_REG_PC0[]  = "PC0";
constexpr char TEXT_REG_PC1[]  = "PC1";
constexpr char TEXT_REG_PC2[]  = "PC2";
constexpr char TEXT_REG_MMU[]  = "MMU";
constexpr char TEXT_REG_LRU[]  = "LRU";
constexpr char TEXT_REG_RSN[]  = "RSN";
constexpr char TEXT_REG_RMA0[] = "RMA0";
constexpr char TEXT_REG_RMC0[] = "RMC0";
constexpr char TEXT_REG_RMA1[] = "RMA1";
constexpr char TEXT_REG_RMC1[] = "RMC1";
constexpr char TEXT_REG_SPC0[] = "SPC0";
constexpr char TEXT_REG_SPC1[] = "SPC1";
constexpr char TEXT_REG_SPC2[] = "SPC2";
constexpr char TEXT_REG_IBA0[] = "IBA0";
constexpr char TEXT_REG_IBC0[] = "IBC0";
constexpr char TEXT_REG_IBA1[] = "IBA1";
constexpr char TEXT_REG_IBC1[] = "IBC1";
constexpr char TEXT_REG_EXOP[] = "EXOP";
constexpr char TEXT_REG_CIR[]  = "CIR";
constexpr char TEXT_REG_CDR[]  = "CDR";
constexpr char TEXT_REG_IPC[]  = "IPC";
constexpr char TEXT_REG_IPA[]  = "IPA";
constexpr char TEXT_REG_IPB[]  = "IPB";
constexpr char TEXT_REG_Q[]    = "Q";
constexpr char TEXT_REG_ALU[]  = "ALU";
constexpr char TEXT_REG_BP[]   = "BP";
constexpr char TEXT_REG_FC[]   = "FC";
constexpr char TEXT_REG_CR[]   = "CR";
constexpr char TEXT_REG_FPE[]  = "FPE";
constexpr char TEXT_REG_INTE[] = "INTE";
constexpr char TEXT_REG_FPS[]  = "FPS";

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
