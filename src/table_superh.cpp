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

#include "table_superh.h"

#include "entry_table.h"
#include "text_superh.h"

using namespace libasm::text::superh;

namespace libasm {
namespace superh {

// clang-format off

#define E2(opc, name, src, dst) \
    {opc, Entry::Flags::create(src, dst), name}
#define E1(opc, name, src) E2(opc, name, src, M_NONE)
#define E0(opc, name)      E1(opc, name, M_NONE)

// Common SH-1/SH-2 instructions.
// Each Entry opcode is the 16-bit template with all variable bits zero;
// matchOpCode() masks those bits out (per opcodeMask) before comparing.
constexpr Entry TABLE_COMMON[] PROGMEM = {
    E2(0x0002, TEXT_STC,     M_SR,   M_RN),     // STC SR,Rn
    E2(0x0012, TEXT_STC,     M_GBR,  M_RN),     // STC GBR,Rn
    E2(0x0022, TEXT_STC,     M_VBR,  M_RN),     // STC VBR,Rn
    E2(0x0004, TEXT_MOV_B,   M_RM,   M_IDXN),   // MOV.B Rm,@(R0,Rn)
    E2(0x0005, TEXT_MOV_W,   M_RM,   M_IDXN),   // MOV.W Rm,@(R0,Rn)
    E2(0x0006, TEXT_MOV_L,   M_RM,   M_IDXN),   // MOV.L Rm,@(R0,Rn)
    E0(0x0009, TEXT_NOP),                       // NOP
    E2(0x000A, TEXT_STS,     M_MACH, M_RN),     // STS MACH,Rn
    E0(0x000B, TEXT_RTS),                       // RTS
    E2(0x000C, TEXT_MOV_B,   M_IDXM, M_RN),     // MOV.B @(R0,Rm),Rn
    E2(0x000D, TEXT_MOV_W,   M_IDXM, M_RN),     // MOV.W @(R0,Rm),Rn
    E2(0x000E, TEXT_MOV_L,   M_IDXM, M_RN),     // MOV.L @(R0,Rm),Rn
    E0(0x0008, TEXT_CLRT),                      // CLRT
    E0(0x0018, TEXT_SETT),                      // SETT
    E0(0x0019, TEXT_DIV0U),                     // DIV0U
    E2(0x001A, TEXT_STS,     M_MACL, M_RN),     // STS MACL,Rn
    E0(0x001B, TEXT_SLEEP),                     // SLEEP
    E0(0x0028, TEXT_CLRMAC),                    // CLRMAC
    E1(0x0029, TEXT_MOVT,    M_RN),             // MOVT Rn
    E2(0x002A, TEXT_STS,     M_PR,   M_RN),     // STS PR,Rn
    E0(0x002B, TEXT_RTE),                       // RTE
    E2(0x1000, TEXT_MOV_L,   M_RM,   M_D4N),    // MOV.L Rm,@(disp,Rn)
    E2(0x2000, TEXT_MOV_B,   M_RM,   M_IRN),    // MOV.B Rm,@Rn
    E2(0x2001, TEXT_MOV_W,   M_RM,   M_IRN),    // MOV.W Rm,@Rn
    E2(0x2002, TEXT_MOV_L,   M_RM,   M_IRN),    // MOV.L Rm,@Rn
    E2(0x2004, TEXT_MOV_B,   M_RM,   M_DECN),   // MOV.B Rm,@-Rn
    E2(0x2005, TEXT_MOV_W,   M_RM,   M_DECN),   // MOV.W Rm,@-Rn
    E2(0x2006, TEXT_MOV_L,   M_RM,   M_DECN),   // MOV.L Rm,@-Rn
    E2(0x2007, TEXT_DIV0S,   M_RM,   M_RN),     // DIV0S Rm,Rn
    E2(0x2008, TEXT_TST,     M_RM,   M_RN),     // TST Rm,Rn
    E2(0x2009, TEXT_AND,     M_RM,   M_RN),     // AND Rm,Rn
    E2(0x200A, TEXT_XOR,     M_RM,   M_RN),     // XOR Rm,Rn
    E2(0x200B, TEXT_OR,      M_RM,   M_RN),     // OR Rm,Rn
    E2(0x200C, TEXT_CMP_STR, M_RM,   M_RN),     // CMP/STR Rm,Rn
    E2(0x200D, TEXT_XTRCT,   M_RM,   M_RN),     // XTRCT Rm,Rn
    E2(0x200E, TEXT_MULU_W,  M_RM,   M_RN),     // MULU.W Rm,Rn
    E2(0x200F, TEXT_MULS_W,  M_RM,   M_RN),     // MULS.W Rm,Rn
    E2(0x3000, TEXT_CMP_EQ,  M_RM,   M_RN),     // CMP/EQ Rm,Rn
    E2(0x3002, TEXT_CMP_HS,  M_RM,   M_RN),     // CMP/HS Rm,Rn
    E2(0x3003, TEXT_CMP_GE,  M_RM,   M_RN),     // CMP/GE Rm,Rn
    E2(0x3004, TEXT_DIV1,    M_RM,   M_RN),     // DIV1 Rm,Rn
    E2(0x3006, TEXT_CMP_HI,  M_RM,   M_RN),     // CMP/HI Rm,Rn
    E2(0x3007, TEXT_CMP_GT,  M_RM,   M_RN),     // CMP/GT Rm,Rn
    E2(0x3008, TEXT_SUB,     M_RM,   M_RN),     // SUB Rm,Rn
    E2(0x300A, TEXT_SUBC,    M_RM,   M_RN),     // SUBC Rm,Rn
    E2(0x300B, TEXT_SUBV,    M_RM,   M_RN),     // SUBV Rm,Rn
    E2(0x300C, TEXT_ADD,     M_RM,   M_RN),     // ADD Rm,Rn
    E2(0x300E, TEXT_ADDC,    M_RM,   M_RN),     // ADDC Rm,Rn
    E2(0x300F, TEXT_ADDV,    M_RM,   M_RN),     // ADDV Rm,Rn
    E1(0x4000, TEXT_SHLL,    M_RN),             // SHLL Rn
    E1(0x4001, TEXT_SHLR,    M_RN),             // SHLR Rn
    E2(0x4002, TEXT_STS_L,   M_MACH, M_DECN),   // STS.L MACH,@-Rn
    E2(0x4003, TEXT_STC_L,   M_SR,   M_DECN),   // STC.L SR,@-Rn
    E1(0x4004, TEXT_ROTL,    M_RN),             // ROTL Rn
    E1(0x4005, TEXT_ROTR,    M_RN),             // ROTR Rn
    E2(0x4006, TEXT_LDS_L,   M_INCN, M_MACH),   // LDS.L @Rm+,MACH
    E2(0x4007, TEXT_LDC_L,   M_INCN, M_SR),     // LDC.L @Rm+,SR
    E1(0x4008, TEXT_SHLL2,   M_RN),             // SHLL2 Rn
    E1(0x4009, TEXT_SHLR2,   M_RN),             // SHLR2 Rn
    E2(0x400A, TEXT_LDS,     M_RN,   M_MACH),   // LDS Rm,MACH
    E1(0x400B, TEXT_JSR,     M_IRN),            // JSR @Rm
    E2(0x400E, TEXT_LDC,     M_RN,   M_SR),     // LDC Rm,SR
    E2(0x400F, TEXT_MAC_W,   M_INCM, M_INCN),   // MAC.W @Rm+,@Rn+
    E1(0x4011, TEXT_CMP_PZ,  M_RN),             // CMP/PZ Rn
    E2(0x4012, TEXT_STS_L,   M_MACL, M_DECN),   // STS.L MACL,@-Rn
    E2(0x4013, TEXT_STC_L,   M_GBR,  M_DECN),   // STC.L GBR,@-Rn
    E1(0x4015, TEXT_CMP_PL,  M_RN),             // CMP/PL Rn
    E2(0x4016, TEXT_LDS_L,   M_INCN, M_MACL),   // LDS.L @Rm+,MACL
    E2(0x4017, TEXT_LDC_L,   M_INCN, M_GBR),    // LDC.L @Rm+,GBR
    E1(0x4018, TEXT_SHLL8,   M_RN),             // SHLL8 Rn
    E1(0x4019, TEXT_SHLR8,   M_RN),             // SHLR8 Rn
    E2(0x401A, TEXT_LDS,     M_RN,   M_MACL),   // LDS Rm,MACL
    E1(0x401B, TEXT_TAS_B,   M_IRN),            // TAS.B @Rn
    E2(0x401E, TEXT_LDC,     M_RN,   M_GBR),    // LDC Rm,GBR
    E1(0x4020, TEXT_SHAL,    M_RN),             // SHAL Rn
    E1(0x4021, TEXT_SHAR,    M_RN),             // SHAR Rn
    E2(0x4022, TEXT_STS_L,   M_PR,   M_DECN),   // STS.L PR,@-Rn
    E2(0x4023, TEXT_STC_L,   M_VBR,  M_DECN),   // STC.L VBR,@-Rn
    E1(0x4024, TEXT_ROTCL,   M_RN),             // ROTCL Rn
    E1(0x4025, TEXT_ROTCR,   M_RN),             // ROTCR Rn
    E2(0x4026, TEXT_LDS_L,   M_INCN, M_PR),     // LDS.L @Rm+,PR
    E2(0x4027, TEXT_LDC_L,   M_INCN, M_VBR),    // LDC.L @Rm+,VBR
    E1(0x4028, TEXT_SHLL16,  M_RN),             // SHLL16 Rn
    E1(0x4029, TEXT_SHLR16,  M_RN),             // SHLR16 Rn
    E2(0x402A, TEXT_LDS,     M_RN,   M_PR),     // LDS Rm,PR
    E1(0x402B, TEXT_JMP,     M_IRN),            // JMP @Rm
    E2(0x402E, TEXT_LDC,     M_RN,   M_VBR),    // LDC Rm,VBR
    E2(0x5000, TEXT_MOV_L,   M_D4M,  M_RN),     // MOV.L @(disp,Rm),Rn
    E2(0x6000, TEXT_MOV_B,   M_IRM,  M_RN),     // MOV.B @Rm,Rn
    E2(0x6001, TEXT_MOV_W,   M_IRM,  M_RN),     // MOV.W @Rm,Rn
    E2(0x6002, TEXT_MOV_L,   M_IRM,  M_RN),     // MOV.L @Rm,Rn
    E2(0x6003, TEXT_MOV,     M_RM,   M_RN),     // MOV Rm,Rn
    E2(0x6004, TEXT_MOV_B,   M_INCM, M_RN),     // MOV.B @Rm+,Rn
    E2(0x6005, TEXT_MOV_W,   M_INCM, M_RN),     // MOV.W @Rm+,Rn
    E2(0x6006, TEXT_MOV_L,   M_INCM, M_RN),     // MOV.L @Rm+,Rn
    E2(0x6007, TEXT_NOT,     M_RM,   M_RN),     // NOT Rm,Rn
    E2(0x6008, TEXT_SWAP_B,  M_RM,   M_RN),     // SWAP.B Rm,Rn
    E2(0x6009, TEXT_SWAP_W,  M_RM,   M_RN),     // SWAP.W Rm,Rn
    E2(0x600A, TEXT_NEGC,    M_RM,   M_RN),     // NEGC Rm,Rn
    E2(0x600B, TEXT_NEG,     M_RM,   M_RN),     // NEG Rm,Rn
    E2(0x600C, TEXT_EXTU_B,  M_RM,   M_RN),     // EXTU.B Rm,Rn
    E2(0x600D, TEXT_EXTU_W,  M_RM,   M_RN),     // EXTU.W Rm,Rn
    E2(0x600E, TEXT_EXTS_B,  M_RM,   M_RN),     // EXTS.B Rm,Rn
    E2(0x600F, TEXT_EXTS_W,  M_RM,   M_RN),     // EXTS.W Rm,Rn
    E2(0x7000, TEXT_ADD,     M_IMM8, M_RN),     // ADD #imm,Rn
    E2(0x8000, TEXT_MOV_B,   M_R0,   M_D4M),    // MOV.B R0,@(disp,Rn)
    E2(0x8100, TEXT_MOV_W,   M_R0,   M_D4M),    // MOV.W R0,@(disp,Rn)
    E2(0x8400, TEXT_MOV_B,   M_D4M,  M_R0),     // MOV.B @(disp,Rm),R0
    E2(0x8500, TEXT_MOV_W,   M_D4M,  M_R0),     // MOV.W @(disp,Rm),R0
    E2(0x8800, TEXT_CMP_EQ,  M_IMM8, M_R0),     // CMP/EQ #imm,R0
    E1(0x8900, TEXT_BT,      M_REL8),           // BT label
    E1(0x8B00, TEXT_BF,      M_REL8),           // BF label
    E2(0x9000, TEXT_MOV_W,   M_PCW,  M_RN),     // MOV.W @(disp,PC),Rn
    E1(0xA000, TEXT_BRA,     M_REL12),          // BRA label
    E1(0xB000, TEXT_BSR,     M_REL12),          // BSR label
    E2(0xC000, TEXT_MOV_B,   M_R0,   M_D8B),    // MOV.B R0,@(disp,GBR)
    E2(0xC100, TEXT_MOV_W,   M_R0,   M_D8W),    // MOV.W R0,@(disp,GBR)
    E2(0xC200, TEXT_MOV_L,   M_R0,   M_D8L),    // MOV.L R0,@(disp,GBR)
    E1(0xC300, TEXT_TRAPA,   M_TNUM),           // TRAPA #imm
    E2(0xC400, TEXT_MOV_B,   M_D8B,  M_R0),     // MOV.B @(disp,GBR),R0
    E2(0xC500, TEXT_MOV_W,   M_D8W,  M_R0),     // MOV.W @(disp,GBR),R0
    E2(0xC600, TEXT_MOV_L,   M_D8L,  M_R0),     // MOV.L @(disp,GBR),R0
    E2(0xC700, TEXT_MOVA,    M_PCL,  M_R0),     // MOVA @(disp,PC),R0
    E2(0xC800, TEXT_TST,     M_IMM8, M_R0),     // TST #imm,R0
    E2(0xC900, TEXT_AND,     M_IMM8, M_R0),     // AND #imm,R0
    E2(0xCA00, TEXT_XOR,     M_IMM8, M_R0),     // XOR #imm,R0
    E2(0xCB00, TEXT_OR,      M_IMM8, M_R0),     // OR #imm,R0
    E2(0xCC00, TEXT_TST_B,   M_IMM8, M_IGBR),   // TST.B #imm,@(R0,GBR)
    E2(0xCD00, TEXT_AND_B,   M_IMM8, M_IGBR),   // AND.B #imm,@(R0,GBR)
    E2(0xCE00, TEXT_XOR_B,   M_IMM8, M_IGBR),   // XOR.B #imm,@(R0,GBR)
    E2(0xCF00, TEXT_OR_B,    M_IMM8, M_IGBR),   // OR.B  #imm,@(R0,GBR)
    E2(0xD000, TEXT_MOV_L,   M_PCL,  M_RN),     // MOV.L @(disp,PC),Rn
    E2(0xE000, TEXT_MOV,     M_IMM8, M_RN),     // MOV #imm,Rn
};

// SH-2-only additions.
constexpr Entry TABLE_SH2[] PROGMEM = {
    E1(0x0003, TEXT_BSRF,    M_RN),            // BSRF Rm
    E2(0x0007, TEXT_MUL_L,   M_RM,   M_RN),    // MUL.L Rm,Rn
    E2(0x000F, TEXT_MAC_L,   M_INCM, M_INCN),  // MAC.L @Rm+,@Rn+
    E1(0x0023, TEXT_BRAF,    M_RN),            // BRAF Rm
    E2(0x3005, TEXT_DMULU_L, M_RM,   M_RN),    // DMULU.L Rm,Rn
    E2(0x300D, TEXT_DMULS_L, M_RM,   M_RN),    // DMULS.L Rm,Rn
    E1(0x4010, TEXT_DT,      M_RN),            // DT Rn
    E1(0x8D00, TEXT_BT_S,    M_REL8),          // BT/S label
    E1(0x8F00, TEXT_BF_S,    M_REL8),          // BF/S label
};

// Placeholder index arrays. The disassembler reaches the tables via
// linearSearch in searchOpCode and never consults the index; the real
// alphabetically-sorted INDEX[] arrays land alongside the assembler.
constexpr uint8_t INDEX_COMMON[] PROGMEM = {0};
constexpr uint8_t INDEX_SH2[] PROGMEM = {0};

// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage SH1_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
};

constexpr EntryPage SH2_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
        {ARRAY_RANGE(TABLE_SH2), ARRAY_RANGE(INDEX_SH2)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {SH1, TEXT_CPU_SH1, ARRAY_RANGE(SH1_PAGES)},
        {SH2, TEXT_CPU_SH2, ARRAY_RANGE(SH2_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

// matchOpCode: mask out variable bits (register/immediate fields) before comparing.
static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    opc &= ~entry->readFlags().opcodeMask();
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /* PROGMEM */ char *TableSuperH::listCpu_P() const {
    return TEXT_SUPERH_LIST;
}

const /* PROGMEM */ char *TableSuperH::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableSuperH::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    const auto *t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t == nullptr)
        return UNSUPPORTED_CPU;
    cpuType = t->readCpuType();
    return OK;
}

const TableSuperH TABLE;

}  // namespace superh
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
