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

// SH-DSP additions: DSP register file (DSR/A0/X0/X1/Y0/Y1), repeat control
// (MOD/RS/RE), and the LDRS/LDRE/SETRC repeat instructions.
constexpr Entry TABLE_DSP[] PROGMEM = {
    E2(0x0052, TEXT_STC,     M_MOD,  M_RN),    // STC MOD,Rn
    E2(0x0062, TEXT_STC,     M_RS,   M_RN),    // STC RS,Rn
    E2(0x006A, TEXT_STS,     M_DSR,  M_RN),    // STS DSR,Rn
    E2(0x0072, TEXT_STC,     M_RE,   M_RN),    // STC RE,Rn
    E2(0x007A, TEXT_STS,     M_A0,   M_RN),    // STS A0,Rn
    E2(0x008A, TEXT_STS,     M_X0,   M_RN),    // STS X0,Rn
    E2(0x009A, TEXT_STS,     M_X1,   M_RN),    // STS X1,Rn
    E2(0x00AA, TEXT_STS,     M_Y0,   M_RN),    // STS Y0,Rn
    E2(0x00BA, TEXT_STS,     M_Y1,   M_RN),    // STS Y1,Rn
    E1(0x4014, TEXT_SETRC,   M_RN),            // SETRC Rm  (n-field used for Rm)
    E2(0x4053, TEXT_STC_L,   M_MOD,  M_DECN),  // STC.L MOD,@-Rn
    E2(0x4057, TEXT_LDC_L,   M_INCN, M_MOD),   // LDC.L @Rm+,MOD
    E2(0x405E, TEXT_LDC,     M_RN,   M_MOD),   // LDC Rm,MOD
    E2(0x4062, TEXT_STS_L,   M_DSR,  M_DECN),  // STS.L DSR,@-Rn
    E2(0x4063, TEXT_STC_L,   M_RS,   M_DECN),  // STC.L RS,@-Rn
    E2(0x4066, TEXT_LDS_L,   M_INCN, M_DSR),   // LDS.L @Rm+,DSR
    E2(0x4067, TEXT_LDC_L,   M_INCN, M_RS),    // LDC.L @Rm+,RS
    E2(0x406A, TEXT_LDS,     M_RN,   M_DSR),   // LDS Rm,DSR
    E2(0x406E, TEXT_LDC,     M_RN,   M_RS),    // LDC Rm,RS
    E2(0x4072, TEXT_STS_L,   M_A0,   M_DECN),  // STS.L A0,@-Rn
    E2(0x4073, TEXT_STC_L,   M_RE,   M_DECN),  // STC.L RE,@-Rn
    E2(0x4076, TEXT_LDS_L,   M_INCN, M_A0),    // LDS.L @Rm+,A0
    E2(0x4077, TEXT_LDC_L,   M_INCN, M_RE),    // LDC.L @Rm+,RE
    E2(0x407A, TEXT_LDS,     M_RN,   M_A0),    // LDS Rm,A0
    E2(0x407E, TEXT_LDC,     M_RN,   M_RE),    // LDC Rm,RE
    E2(0x4082, TEXT_STS_L,   M_X0,   M_DECN),  // STS.L X0,@-Rn
    E2(0x4086, TEXT_LDS_L,   M_INCN, M_X0),    // LDS.L @Rm+,X0
    E2(0x408A, TEXT_LDS,     M_RN,   M_X0),    // LDS Rm,X0
    E2(0x4092, TEXT_STS_L,   M_X1,   M_DECN),  // STS.L X1,@-Rn
    E2(0x4096, TEXT_LDS_L,   M_INCN, M_X1),    // LDS.L @Rm+,X1
    E2(0x409A, TEXT_LDS,     M_RN,   M_X1),    // LDS Rm,X1
    E2(0x40A2, TEXT_STS_L,   M_Y0,   M_DECN),  // STS.L Y0,@-Rn
    E2(0x40A6, TEXT_LDS_L,   M_INCN, M_Y0),    // LDS.L @Rm+,Y0
    E2(0x40AA, TEXT_LDS,     M_RN,   M_Y0),    // LDS Rm,Y0
    E2(0x40B2, TEXT_STS_L,   M_Y1,   M_DECN),  // STS.L Y1,@-Rn
    E2(0x40B6, TEXT_LDS_L,   M_INCN, M_Y1),    // LDS.L @Rm+,Y1
    E2(0x40BA, TEXT_LDS,     M_RN,   M_Y1),    // LDS Rm,Y1
    E1(0x8200, TEXT_SETRC,   M_TNUM),          // SETRC #imm
    E1(0x8C00, TEXT_LDRS,    M_REL8P),         // LDRS @(disp,PC)
    E1(0x8E00, TEXT_LDRE,    M_REL8P),         // LDRE @(disp,PC)
};

// SH-2E FPU instructions (shared with SH-2A when FPU is enabled).
// All encodings have the 0xF000 high nibble.
constexpr Entry TABLE_FPU[] PROGMEM = {
    E2(0x005A, TEXT_STS,     M_FPUL,  M_RN),    // STS FPUL,Rn
    E2(0x006A, TEXT_STS,     M_FPSCR, M_RN),    // STS FPSCR,Rn
    E2(0x4052, TEXT_STS_L,   M_FPUL,  M_DECN),  // STS.L FPUL,@-Rn
    E2(0x4056, TEXT_LDS_L,   M_INCN,  M_FPUL),  // LDS.L @Rm+,FPUL
    E2(0x405A, TEXT_LDS,     M_RN,    M_FPUL),  // LDS Rm,FPUL
    E2(0x4062, TEXT_STS_L,   M_FPSCR, M_DECN),  // STS.L FPSCR,@-Rn
    E2(0x4066, TEXT_LDS_L,   M_INCN,  M_FPSCR), // LDS.L @Rm+,FPSCR
    E2(0x406A, TEXT_LDS,     M_RN,    M_FPSCR), // LDS Rm,FPSCR
    E2(0xF000, TEXT_FADD,    M_FRM,   M_FRN),   // FADD FRm,FRn
    E2(0xF001, TEXT_FSUB,    M_FRM,   M_FRN),   // FSUB FRm,FRn
    E2(0xF002, TEXT_FMUL,    M_FRM,   M_FRN),   // FMUL FRm,FRn
    E2(0xF003, TEXT_FDIV,    M_FRM,   M_FRN),   // FDIV FRm,FRn
    E2(0xF004, TEXT_FCMP_EQ, M_FRM,   M_FRN),   // FCMP/EQ FRm,FRn
    E2(0xF005, TEXT_FCMP_GT, M_FRM,   M_FRN),   // FCMP/GT FRm,FRn
    E2(0xF006, TEXT_FMOV_S,  M_IDXM,  M_FRN),   // FMOV.S @(R0,Rm),FRn
    E2(0xF007, TEXT_FMOV_S,  M_FRM,   M_IDXN),  // FMOV.S FRm,@(R0,Rn)
    E2(0xF008, TEXT_FMOV_S,  M_IRM,   M_FRN),   // FMOV.S @Rm,FRn
    E2(0xF009, TEXT_FMOV_S,  M_INCM,  M_FRN),   // FMOV.S @Rm+,FRn
    E2(0xF00A, TEXT_FMOV_S,  M_FRM,   M_IRN),   // FMOV.S FRm,@Rn
    E2(0xF00B, TEXT_FMOV_S,  M_FRM,   M_DECN),  // FMOV.S FRm,@-Rn
    E2(0xF00C, TEXT_FMOV,    M_FRM,   M_FRN),   // FMOV FRm,FRn
    E2(0xF00D, TEXT_FSTS,    M_FPUL,  M_FRN),   // FSTS FPUL,FRn
    E2(0xF00E, TEXT_FMAC,    M_FRM,   M_FRN),   // FMAC FR0,FRm,FRn
    E2(0xF01D, TEXT_FLDS,    M_FRN,   M_FPUL),  // FLDS FRm,FPUL  (FRm in n-slot)
    E2(0xF02D, TEXT_FLOAT,   M_FPUL,  M_FRN),   // FLOAT FPUL,FRn
    E2(0xF03D, TEXT_FTRC,    M_FRN,   M_FPUL),  // FTRC FRm,FPUL  (FRm in n-slot)
    E1(0xF04D, TEXT_FNEG,    M_FRN),            // FNEG FRn
    E1(0xF05D, TEXT_FABS,    M_FRN),            // FABS FRn
    E1(0xF08D, TEXT_FLDI0,   M_FRN),            // FLDI0 FRn
    E1(0xF09D, TEXT_FLDI1,   M_FRN),            // FLDI1 FRn
};

// INDEX arrays sorted alphabetically by the mnemonic STRING (not by the
// TEXT_X identifier), so the binary-search comparator (strcasecmp_P on
// the mnemonic) sees the index ordering match its sort. Regenerate via
// the awk pipeline in this file's commit history if TABLE_* changes.
constexpr uint8_t INDEX_COMMON[] PROGMEM = {
    104,  // TEXT_ADD
     46,  // TEXT_ADD
     47,  // TEXT_ADDC
     48,  // TEXT_ADDV
    124,  // TEXT_AND
     30,  // TEXT_AND
    128,  // TEXT_AND_B
    111,  // TEXT_BF
    113,  // TEXT_BRA
    114,  // TEXT_BSR
    110,  // TEXT_BT
     17,  // TEXT_CLRMAC
     12,  // TEXT_CLRT
    109,  // TEXT_CMP_EQ
     37,  // TEXT_CMP_EQ
     39,  // TEXT_CMP_GE
     42,  // TEXT_CMP_GT
     41,  // TEXT_CMP_HI
     38,  // TEXT_CMP_HS
     66,  // TEXT_CMP_PL
     63,  // TEXT_CMP_PZ
     33,  // TEXT_CMP_STR
     28,  // TEXT_DIV0S
     14,  // TEXT_DIV0U
     40,  // TEXT_DIV1
    102,  // TEXT_EXTS_B
    103,  // TEXT_EXTS_W
    100,  // TEXT_EXTU_B
    101,  // TEXT_EXTU_W
     85,  // TEXT_JMP
     60,  // TEXT_JSR
     61,  // TEXT_LDC
     73,  // TEXT_LDC
     86,  // TEXT_LDC
     56,  // TEXT_LDC_L
     68,  // TEXT_LDC_L
     81,  // TEXT_LDC_L
     59,  // TEXT_LDS
     71,  // TEXT_LDS
     84,  // TEXT_LDS
     55,  // TEXT_LDS_L
     67,  // TEXT_LDS_L
     80,  // TEXT_LDS_L
     62,  // TEXT_MAC_W
    132,  // TEXT_MOV
     91,  // TEXT_MOV
    105,  // TEXT_MOV_B
    107,  // TEXT_MOV_B
    115,  // TEXT_MOV_B
    119,  // TEXT_MOV_B
     22,  // TEXT_MOV_B
     25,  // TEXT_MOV_B
      3,  // TEXT_MOV_B
     88,  // TEXT_MOV_B
      9,  // TEXT_MOV_B
     92,  // TEXT_MOV_B
     11,  // TEXT_MOV_L
    117,  // TEXT_MOV_L
    121,  // TEXT_MOV_L
    131,  // TEXT_MOV_L
     21,  // TEXT_MOV_L
     24,  // TEXT_MOV_L
     27,  // TEXT_MOV_L
      5,  // TEXT_MOV_L
     87,  // TEXT_MOV_L
     90,  // TEXT_MOV_L
     94,  // TEXT_MOV_L
     10,  // TEXT_MOV_W
    106,  // TEXT_MOV_W
    108,  // TEXT_MOV_W
    112,  // TEXT_MOV_W
    116,  // TEXT_MOV_W
    120,  // TEXT_MOV_W
     23,  // TEXT_MOV_W
     26,  // TEXT_MOV_W
      4,  // TEXT_MOV_W
     89,  // TEXT_MOV_W
     93,  // TEXT_MOV_W
    122,  // TEXT_MOVA
     18,  // TEXT_MOVT
     36,  // TEXT_MULS_W
     35,  // TEXT_MULU_W
     99,  // TEXT_NEG
     98,  // TEXT_NEGC
      6,  // TEXT_NOP
     95,  // TEXT_NOT
    126,  // TEXT_OR
     32,  // TEXT_OR
    130,  // TEXT_OR_B
     78,  // TEXT_ROTCL
     79,  // TEXT_ROTCR
     53,  // TEXT_ROTL
     54,  // TEXT_ROTR
     20,  // TEXT_RTE
      8,  // TEXT_RTS
     13,  // TEXT_SETT
     74,  // TEXT_SHAL
     75,  // TEXT_SHAR
     49,  // TEXT_SHLL
     82,  // TEXT_SHLL16
     57,  // TEXT_SHLL2
     69,  // TEXT_SHLL8
     50,  // TEXT_SHLR
     83,  // TEXT_SHLR16
     58,  // TEXT_SHLR2
     70,  // TEXT_SHLR8
     16,  // TEXT_SLEEP
      0,  // TEXT_STC
      1,  // TEXT_STC
      2,  // TEXT_STC
     52,  // TEXT_STC_L
     65,  // TEXT_STC_L
     77,  // TEXT_STC_L
     15,  // TEXT_STS
     19,  // TEXT_STS
      7,  // TEXT_STS
     51,  // TEXT_STS_L
     64,  // TEXT_STS_L
     76,  // TEXT_STS_L
     43,  // TEXT_SUB
     44,  // TEXT_SUBC
     45,  // TEXT_SUBV
     96,  // TEXT_SWAP_B
     97,  // TEXT_SWAP_W
     72,  // TEXT_TAS_B
    118,  // TEXT_TRAPA
    123,  // TEXT_TST
     29,  // TEXT_TST
    127,  // TEXT_TST_B
    125,  // TEXT_XOR
     31,  // TEXT_XOR
    129,  // TEXT_XOR_B
     34,  // TEXT_XTRCT
};

constexpr uint8_t INDEX_SH2[] PROGMEM = {
      8,  // TEXT_BF_S
      3,  // TEXT_BRAF
      0,  // TEXT_BSRF
      7,  // TEXT_BT_S
      5,  // TEXT_DMULS_L
      4,  // TEXT_DMULU_L
      6,  // TEXT_DT
      2,  // TEXT_MAC_L
      1,  // TEXT_MUL_L
};

constexpr uint8_t INDEX_FPU[] PROGMEM = {0};

constexpr uint8_t INDEX_DSP[] PROGMEM = {
     12,  // TEXT_LDC
     18,  // TEXT_LDC
     24,  // TEXT_LDC
     11,  // TEXT_LDC_L
     16,  // TEXT_LDC_L
     22,  // TEXT_LDC_L
     39,  // TEXT_LDRE
     38,  // TEXT_LDRS
     17,  // TEXT_LDS
     23,  // TEXT_LDS
     27,  // TEXT_LDS
     30,  // TEXT_LDS
     33,  // TEXT_LDS
     36,  // TEXT_LDS
     15,  // TEXT_LDS_L
     21,  // TEXT_LDS_L
     26,  // TEXT_LDS_L
     29,  // TEXT_LDS_L
     32,  // TEXT_LDS_L
     35,  // TEXT_LDS_L
      9,  // TEXT_SETRC
     37,  // TEXT_SETRC
      0,  // TEXT_STC
      1,  // TEXT_STC
      3,  // TEXT_STC
     10,  // TEXT_STC_L
     14,  // TEXT_STC_L
     20,  // TEXT_STC_L
      2,  // TEXT_STS
      4,  // TEXT_STS
      5,  // TEXT_STS
      6,  // TEXT_STS
      7,  // TEXT_STS
      8,  // TEXT_STS
     13,  // TEXT_STS_L
     19,  // TEXT_STS_L
     25,  // TEXT_STS_L
     28,  // TEXT_STS_L
     31,  // TEXT_STS_L
     34,  // TEXT_STS_L
};

// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage SH1_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
};

constexpr EntryPage SH2_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
        {ARRAY_RANGE(TABLE_SH2), ARRAY_RANGE(INDEX_SH2)},
};

constexpr EntryPage SH_DSP_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
        {ARRAY_RANGE(TABLE_SH2), ARRAY_RANGE(INDEX_SH2)},
        {ARRAY_RANGE(TABLE_DSP), ARRAY_RANGE(INDEX_DSP)},
};

// SH-2E inherits SH-2 instructions. The FPU page comes from the FPU
// dispatch table below (mandatory FPU_SH2E).
constexpr EntryPage SH2E_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
        {ARRAY_RANGE(TABLE_SH2), ARRAY_RANGE(INDEX_SH2)},
};

constexpr EntryPage FPU_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_FPU), ARRAY_RANGE(INDEX_FPU)},
};

#define EMPTY_RANGE(a) ARRAY_BEGIN(a), ARRAY_BEGIN(a)

using Cpu = entry::CpuBase<CpuType, EntryPage>;
using Fpu = entry::CpuBase<FpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {SH1, TEXT_CPU_SH1, ARRAY_RANGE(SH1_PAGES)},
        {SH2, TEXT_CPU_SH2, ARRAY_RANGE(SH2_PAGES)},
        {SH_DSP, TEXT_CPU_SH_DSP, ARRAY_RANGE(SH_DSP_PAGES)},
        {SH2E, TEXT_CPU_SH2E, ARRAY_RANGE(SH2E_PAGES)},
};

constexpr Fpu FPU_TABLE[] PROGMEM = {
        {FPU_NONE, TEXT_FPU_NONE, EMPTY_RANGE(FPU_PAGES)},
        {FPU_SH2E, TEXT_FPU_SH2E, ARRAY_RANGE(FPU_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

static const Fpu *fpu(FpuType fpuType) {
    return Fpu::search(fpuType, ARRAY_RANGE(FPU_TABLE));
}

// matchOpCode: mask out variable bits (register/immediate fields) before comparing.
static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    opc &= ~entry->readFlags().opcodeMask();
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, FpuType fpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    if (insn.getError() == UNKNOWN_INSTRUCTION) {
        insn.nameBuffer().reset();
        out.reset();
        insn.setError(insn.errorAt(), OK);
        fpu(fpuType)->searchOpCode(insn, out, matchOpCode);
    }
    return insn.getError();
}

// acceptMode: the parser produces "broad" modes; the table holds narrower
// variants that share encoding shape. Lead with the equality fast path per
// the libasm convention, then list per-table-mode relaxations.
static bool acceptMode(const Operand &op, AddrMode table) {
    if (op.mode == table)
        return true;
    switch (table) {
    case M_RM:
        return op.mode == M_RN;
    case M_R0:
        return op.mode == M_RN && op.reg == REG_R0;
    case M_IRM:
        return op.mode == M_IRN;
    case M_INCM:
        return op.mode == M_INCN;
    case M_IDXM:
        return op.mode == M_IDXN;
    case M_D4M:
        return op.mode == M_D4N;
    case M_D8W:
    case M_D8L:
        return op.mode == M_D8B;  // parser collapses GBR-relative to M_D8B
    case M_PCW:
        return op.mode == M_PCL;  // parser collapses PC-relative to M_PCL
    case M_REL8P:
        return op.mode == M_PCL;  // LDRS/LDRE accept @(target,PC)
    case M_TNUM:
        return op.mode == M_IMM8;
    case M_REL12:
        return op.mode == M_REL8;
    default:
        return false;
    }
}

static bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto flags = entry->readFlags();
    return acceptMode(insn.srcOp, flags.src()) && acceptMode(insn.dstOp, flags.dst());
}

Error searchName(CpuType cpuType, FpuType fpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    if (insn.getError() == UNKNOWN_INSTRUCTION) {
        insn.setError(insn.errorAt(), OK);
        fpu(fpuType)->searchName(insn, acceptModes);
    }
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

void Config::setCpuType(CpuType cpuType) {
    ConfigImpl::setCpuType(cpuType);
    // SH-2E mandates the FPU; other CPUs default to FPU_NONE.
    _fpu = (cpuType == SH2E) ? FPU_SH2E : FPU_NONE;
}

const /* PROGMEM */ char *Config::fpu_P() const {
    return fpu(_fpu)->name_P();
}

Error Config::setFpuType(FpuType fpuType) {
    if (fpuType == FPU_NONE || fpuType == FPU_SH2E) {
        _fpu = fpuType;
        return OK;
    }
    return UNKNOWN_OPERAND;
}

Error Config::setFpuName(StrScanner &scan) {
    if (scan.expectFalse())
        return setFpuType(FPU_NONE);
    if (scan.expectTrue())
        return setFpuType(FPU_SH2E);
    const auto *t = Fpu::search(scan, ARRAY_RANGE(FPU_TABLE));
    if (t == nullptr)
        return UNKNOWN_OPERAND;
    return setFpuType(t->readCpuType());
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
