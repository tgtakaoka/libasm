/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "text_z280.h"

namespace libasm {
namespace text {
namespace z280 {
// clang-format off

constexpr char TEXT_Z280_LIST[] = "Z280, Z80";
constexpr char TEXT_CPU_Z280[]  = "Z280";
// constexpr char TEXT_CPU_Z80[] = "Z80";

// Z80
// constexpr char TEXT_ADC[]  = "ADC";
// constexpr char TEXT_ADD[]  = "ADD";
// constexpr char TEXT_AND[]  = "AND";
// constexpr char TEXT_BIT[]  = "BIT";
// constexpr char TEXT_CALL[] = "CALL";
// constexpr char TEXT_CCF[]  = "CCF";
// constexpr char TEXT_CP[]   = "CP";
// constexpr char TEXT_CPD[]  = "CPD";
// constexpr char TEXT_CPDR[] = "CPDR";
// constexpr char TEXT_CPI[]  = "CPI";
// constexpr char TEXT_CPIR[] = "CPIR";
// constexpr char TEXT_CPL[]  = "CPL";
// constexpr char TEXT_DAA[]  = "DAA";
// constexpr char TEXT_DEC[]  = "DEC";
// constexpr char TEXT_DI[]   = "DI";
// constexpr char TEXT_DJNZ[] = "DJNZ";
// constexpr char TEXT_EI[]   = "EI";
// constexpr char TEXT_EX[]   = "EX";
// constexpr char TEXT_EXX[]  = "EXX";
// constexpr char TEXT_HALT[] = "HALT";
// constexpr char TEXT_IM[]   = "IM";
// constexpr char TEXT_IN[]   = "IN";
// constexpr char TEXT_INC[]  = "INC";
// constexpr char TEXT_IND[]  = "IND";
// constexpr char TEXT_INDR[] = "INDR";
// constexpr char TEXT_INI[]  = "INI";
// constexpr char TEXT_INIR[] = "INIR";
// constexpr char TEXT_JP[]   = "JP";
// constexpr char TEXT_JR[]   = "JR";
// constexpr char TEXT_LD[]   = "LD";
// constexpr char TEXT_LDD[]  = "LDD";
// constexpr char TEXT_LDDR[] = "LDDR";
// constexpr char TEXT_LDI[]  = "LDI";
// constexpr char TEXT_LDIR[] = "LDIR";
// constexpr char TEXT_NEG[]  = "NEG";
// constexpr char TEXT_NOP[]  = "NOP";
// constexpr char TEXT_OR[]   = "OR";
// constexpr char TEXT_OTDR[] = "OTDR";
// constexpr char TEXT_OTIR[] = "OTIR";
// constexpr char TEXT_OUT[]  = "OUT";
// constexpr char TEXT_OUTD[] = "OUTD";
// constexpr char TEXT_OUTI[] = "OUTI";
// constexpr char TEXT_POP[]  = "POP";
// constexpr char TEXT_PUSH[] = "PUSH";
// constexpr char TEXT_RES[]  = "RES";
// constexpr char TEXT_RET[]  = "RET";
// constexpr char TEXT_RETI[] = "RETI";
// constexpr char TEXT_RETN[] = "RETN";
// constexpr char TEXT_RL[]   = "RL";
// constexpr char TEXT_RLA[]  = "RLA";
// constexpr char TEXT_RLC[]  = "RLC";
// constexpr char TEXT_RLCA[] = "RLCA";
// constexpr char TEXT_RLD[]  = "RLD";
// constexpr char TEXT_RR[]   = "RR";
// constexpr char TEXT_RRA[]  = "RRA";
// constexpr char TEXT_RRC[]  = "RRC";
// constexpr char TEXT_RRCA[] = "RRCA";
// constexpr char TEXT_RRD[]  = "RRD";
// constexpr char TEXT_RST[]  = "RST";
// constexpr char TEXT_SBC[]  = "SBC";
// constexpr char TEXT_SCF[]  = "SCF";
// constexpr char TEXT_SET[]  = "SET";
// constexpr char TEXT_SLA[]  = "SLA";
// constexpr char TEXT_SRA[]  = "SRA";
// constexpr char TEXT_SRL[]  = "SRL";
// constexpr char TEXT_SUB[]  = "SUB";
// constexpr char TEXT_XOR[]  = "XOR";

// Z280
// constexpr char TEXT_ADDW[]   = "ADDW";
// constexpr char TEXT_CPW[]    = "CPW";
// constexpr char TEXT_DECW[]   = "DECW";
// constexpr char TEXT_DIV[]    = "DIV";
// constexpr char TEXT_DIVU[]   = "DIVU";
// constexpr char TEXT_DIVUW[]  = "DIVUW";
// constexpr char TEXT_DIVW[]   = "DIVW";
#if defined(LIBASM_Z280_EPU)
constexpr char TEXT_EPUF[]   = "EPUF";
constexpr char TEXT_EPUI[]   = "EPUI";
constexpr char TEXT_EPUM[]   = "EPUM";
#endif
// constexpr char TEXT_EXTS[]   = "EXTS";
// constexpr char TEXT_INCW[]   = "INCW";
// constexpr char TEXT_INDRW[]  = "INDRW";
// constexpr char TEXT_INDW[]   = "INDW";
// constexpr char TEXT_INIRW[]  = "INIRW";
// constexpr char TEXT_INIW[]   = "INIW";
// constexpr char TEXT_INW[]    = "INW";
constexpr char TEXT_JAF[]    = "JAF";
constexpr char TEXT_JAR[]    = "JAR";
// constexpr char TEXT_LDA[]    = "LDA";
// constexpr char TEXT_LDCTL[]  = "LDCTL";
constexpr char TEXT_LDUD[]   = "LDUD";
constexpr char TEXT_LDUP[]   = "LDUP";
// constexpr char TEXT_LDW[]    = "LDW";
#if defined(LIBASM_Z280_EPU)
constexpr char TEXT_MEPU[]   = "MEPU";
#endif
// constexpr char TEXT_MULT[]   = "MULT";
constexpr char TEXT_MULTU[]  = "MULTU";
// constexpr char TEXT_MULTUW[] = "MULTUW";
// constexpr char TEXT_MULTW[]  = "MULTW";
// constexpr char TEXT_OTDRW[]  = "OTDRW";
// constexpr char TEXT_OTIRW[]  = "OTIRW";
// constexpr char TEXT_OUTDW[]  = "OUTDW";
// constexpr char TEXT_OUTIW[]  = "OUTIW";
// constexpr char TEXT_OUTW[]   = "OUTW";
constexpr char TEXT_PCACHE[] = "PCACHE";
constexpr char TEXT_RETIL[]  = "RETIL";
// constexpr char TEXT_SC[]     = "SC";
// constexpr char TEXT_SUBW[]   = "SUBW";
// constexpr char TEXT_TSET[]   = "TSET";
constexpr char TEXT_TSTI[]   = "TSTI";

// Z80, Z180
// constexpr char TEXT_REG_A[]  = "A";
// constexpr char TEXT_REG_AF[] = "AF";
// constexpr char TEXT_REG_B[]  = "B";
// constexpr char TEXT_REG_BC[] = "BC";
// constexpr char TEXT_REG_C[]  = "C";
// constexpr char TEXT_REG_D[]  = "D";
// constexpr char TEXT_REG_DE[] = "DE";
// constexpr char TEXT_REG_E[]  = "E";
// constexpr char TEXT_REG_H[]  = "H";
// constexpr char TEXT_REG_HL[] = "HL";
// constexpr char TEXT_REG_I[]  = "I";
// constexpr char TEXT_REG_IX[] = "IX";
// constexpr char TEXT_REG_IY[] = "IY";
// constexpr char TEXT_REG_L[]  = "L";
// constexpr char TEXT_REG_R[]  = "R";
// constexpr char TEXT_REG_SP[] = "SP";

// Z280
constexpr char TEXT_REG_DEHL[] = "DEHL";
// constexpr char TEXT_REG_IXH[]  = "IXH";
// constexpr char TEXT_REG_IXL[] = "IXL";
// constexpr char TEXT_REG_IYH[]  = "IYH";
// constexpr char TEXT_REG_IYL[] = "IYL";
// constexpr char TEXT_REG_USP[] = "USP";

// Z80, Z180
// constexpr char TEXT_CC_C[]  = "C";
// constexpr char TEXT_CC_M[]  = "M";
// constexpr char TEXT_CC_NC[] = "NC";
// constexpr char TEXT_CC_NZ[] = "NZ";
// constexpr char TEXT_CC_P[]  = "P";
// constexpr char TEXT_CC_PE[] = "PE";
// constexpr char TEXT_CC_PO[] = "PO";
// constexpr char TEXT_CC_Z[]  = "Z";

// Z280
// constexpr char TEXT_CC_NS[] = "NS";
// constexpr char TEXT_CC_NV[] = "NV";
// constexpr char TEXT_CC_S[]  = "S";
// constexpr char TEXT_CC_V[]  = "V";

// clang-format on
}  // namespace z280
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
