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

// Every row carries its access size: ISZ_NONE (no suffix), a fixed
// ISZ_BYTE/WORD/LONG, or ISZ_DATA (size lives in the opcode; sizeFieldOf(src,dst)
// locates the bits and the stored opcode is the default-size template).
#define E2(opc, name, isz, src, dst) \
    {opc, Entry::Flags::create(isz, src, dst), name}
#define E1(opc, name, isz, src) E2(opc, name, isz, src, M_NONE)
#define E0(opc, name, isz)      E1(opc, name, isz, M_NONE)

// Common SH-1/SH-2 instructions.
// Each Entry opcode is the 16-bit template with all variable bits zero;
// matchOpCode() masks those bits out (per opcodeMask) before comparing.
constexpr Entry TABLE_COMMON[] PROGMEM = {
    E2(0x0002, TEXT_STC,     ISZ_NONE, M_SR,    M_RN),    // STC SR,Rn
    E2(0x0012, TEXT_STC,     ISZ_NONE, M_GBR,   M_RN),    // STC GBR,Rn
    E2(0x0022, TEXT_STC,     ISZ_NONE, M_VBR,   M_RN),    // STC VBR,Rn
    E2(0x0006, TEXT_MOV,     ISZ_DATA, M_RM,    M_IDXN),  // MOV.B/W/L Rm,@(R0,Rn)
    E0(0x0009, TEXT_NOP,     ISZ_NONE),                   // NOP
    E2(0x000A, TEXT_STS,     ISZ_NONE, M_MACH,  M_RN),    // STS MACH,Rn
    E0(0x000B, TEXT_RTS,     ISZ_NONE),                   // RTS
    E2(0x000E, TEXT_MOV,     ISZ_DATA, M_IDXM,  M_RN),    // MOV.B/W/L @(R0,Rm),Rn
    E0(0x0008, TEXT_CLRT,    ISZ_NONE),                   // CLRT
    E0(0x0018, TEXT_SETT,    ISZ_NONE),                   // SETT
    E0(0x0019, TEXT_DIV0U,   ISZ_NONE),                   // DIV0U
    E2(0x001A, TEXT_STS,     ISZ_NONE, M_MACL,  M_RN),    // STS MACL,Rn
    E0(0x001B, TEXT_SLEEP,   ISZ_NONE),                   // SLEEP
    E0(0x0028, TEXT_CLRMAC,  ISZ_NONE),                   // CLRMAC
    E1(0x0029, TEXT_MOVT,    ISZ_NONE, M_RN),             // MOVT Rn
    E2(0x002A, TEXT_STS,     ISZ_NONE, M_PR,    M_RN),    // STS PR,Rn
    E0(0x002B, TEXT_RTE,     ISZ_NONE),                   // RTE
    E2(0x1000, TEXT_MOV,     ISZ_LONG, M_RM,    M_D4N),   // MOV.L Rm,@(disp,Rn)
    E2(0x2002, TEXT_MOV,     ISZ_DATA, M_RM,    M_IRN),   // MOV.B/W/L Rm,@Rn
    E2(0x2006, TEXT_MOV,     ISZ_DATA, M_RM,    M_DECN),  // MOV.B/W/L Rm,@-Rn
    E2(0x2007, TEXT_DIV0S,   ISZ_NONE, M_RM,    M_RN),    // DIV0S Rm,Rn
    E2(0x2008, TEXT_TST,     ISZ_NONE, M_RM,    M_RN),    // TST Rm,Rn
    E2(0x2009, TEXT_AND,     ISZ_NONE, M_RM,    M_RN),    // AND Rm,Rn
    E2(0x200A, TEXT_XOR,     ISZ_NONE, M_RM,    M_RN),    // XOR Rm,Rn
    E2(0x200B, TEXT_OR,      ISZ_NONE, M_RM,    M_RN),    // OR Rm,Rn
    E2(0x200C, TEXT_CMP_STR, ISZ_NONE, M_RM,    M_RN),    // CMP/STR Rm,Rn
    E2(0x200D, TEXT_XTRCT,   ISZ_NONE, M_RM,    M_RN),    // XTRCT Rm,Rn
    E2(0x200E, TEXT_MULU,    ISZ_WORD, M_RM,    M_RN),    // MULU.W Rm,Rn
    E2(0x200F, TEXT_MULS,    ISZ_WORD, M_RM,    M_RN),    // MULS.W Rm,Rn
    E2(0x3000, TEXT_CMP_EQ,  ISZ_NONE, M_RM,    M_RN),    // CMP/EQ Rm,Rn
    E2(0x3002, TEXT_CMP_HS,  ISZ_NONE, M_RM,    M_RN),    // CMP/HS Rm,Rn
    E2(0x3003, TEXT_CMP_GE,  ISZ_NONE, M_RM,    M_RN),    // CMP/GE Rm,Rn
    E2(0x3004, TEXT_DIV1,    ISZ_NONE, M_RM,    M_RN),    // DIV1 Rm,Rn
    E2(0x3006, TEXT_CMP_HI,  ISZ_NONE, M_RM,    M_RN),    // CMP/HI Rm,Rn
    E2(0x3007, TEXT_CMP_GT,  ISZ_NONE, M_RM,    M_RN),    // CMP/GT Rm,Rn
    E2(0x3008, TEXT_SUB,     ISZ_NONE, M_RM,    M_RN),    // SUB Rm,Rn
    E2(0x300A, TEXT_SUBC,    ISZ_NONE, M_RM,    M_RN),    // SUBC Rm,Rn
    E2(0x300B, TEXT_SUBV,    ISZ_NONE, M_RM,    M_RN),    // SUBV Rm,Rn
    E2(0x300C, TEXT_ADD,     ISZ_NONE, M_RM,    M_RN),    // ADD Rm,Rn
    E2(0x300E, TEXT_ADDC,    ISZ_NONE, M_RM,    M_RN),    // ADDC Rm,Rn
    E2(0x300F, TEXT_ADDV,    ISZ_NONE, M_RM,    M_RN),    // ADDV Rm,Rn
    E1(0x4000, TEXT_SHLL,    ISZ_NONE, M_RN),             // SHLL Rn
    E1(0x4001, TEXT_SHLR,    ISZ_NONE, M_RN),             // SHLR Rn
    E2(0x4002, TEXT_STS,     ISZ_LONG, M_MACH,  M_DECN),  // STS.L MACH,@-Rn
    E2(0x4003, TEXT_STC,     ISZ_LONG, M_SR,    M_DECN),  // STC.L SR,@-Rn
    E1(0x4004, TEXT_ROTL,    ISZ_NONE, M_RN),             // ROTL Rn
    E1(0x4005, TEXT_ROTR,    ISZ_NONE, M_RN),             // ROTR Rn
    E2(0x4006, TEXT_LDS,     ISZ_LONG, M_INCN,  M_MACH),  // LDS.L @Rm+,MACH
    E2(0x4007, TEXT_LDC,     ISZ_LONG, M_INCN,  M_SR),    // LDC.L @Rm+,SR
    E1(0x4008, TEXT_SHLL2,   ISZ_NONE, M_RN),             // SHLL2 Rn
    E1(0x4009, TEXT_SHLR2,   ISZ_NONE, M_RN),             // SHLR2 Rn
    E2(0x400A, TEXT_LDS,     ISZ_NONE, M_RN,    M_MACH),  // LDS Rm,MACH
    E1(0x400B, TEXT_JSR,     ISZ_NONE, M_IRN),            // JSR @Rm
    E2(0x400E, TEXT_LDC,     ISZ_NONE, M_RN,    M_SR),    // LDC Rm,SR
    E2(0x400F, TEXT_MAC_W,   ISZ_NONE, M_INCM,  M_INCN),  // MAC.W @Rm+,@Rn+
    E1(0x4011, TEXT_CMP_PZ,  ISZ_NONE, M_RN),             // CMP/PZ Rn
    E2(0x4012, TEXT_STS,     ISZ_LONG, M_MACL,  M_DECN),  // STS.L MACL,@-Rn
    E2(0x4013, TEXT_STC,     ISZ_LONG, M_GBR,   M_DECN),  // STC.L GBR,@-Rn
    E1(0x4015, TEXT_CMP_PL,  ISZ_NONE, M_RN),             // CMP/PL Rn
    E2(0x4016, TEXT_LDS,     ISZ_LONG, M_INCN,  M_MACL),  // LDS.L @Rm+,MACL
    E2(0x4017, TEXT_LDC,     ISZ_LONG, M_INCN,  M_GBR),   // LDC.L @Rm+,GBR
    E1(0x4018, TEXT_SHLL8,   ISZ_NONE, M_RN),             // SHLL8 Rn
    E1(0x4019, TEXT_SHLR8,   ISZ_NONE, M_RN),             // SHLR8 Rn
    E2(0x401A, TEXT_LDS,     ISZ_NONE, M_RN,    M_MACL),  // LDS Rm,MACL
    E1(0x401B, TEXT_TAS,     ISZ_BYTE, M_IRN),            // TAS.B @Rn
    E2(0x401E, TEXT_LDC,     ISZ_NONE, M_RN,    M_GBR),   // LDC Rm,GBR
    E1(0x4020, TEXT_SHAL,    ISZ_NONE, M_RN),             // SHAL Rn
    E1(0x4021, TEXT_SHAR,    ISZ_NONE, M_RN),             // SHAR Rn
    E2(0x4022, TEXT_STS,     ISZ_LONG, M_PR,    M_DECN),  // STS.L PR,@-Rn
    E2(0x4023, TEXT_STC,     ISZ_LONG, M_VBR,   M_DECN),  // STC.L VBR,@-Rn
    E1(0x4024, TEXT_ROTCL,   ISZ_NONE, M_RN),             // ROTCL Rn
    E1(0x4025, TEXT_ROTCR,   ISZ_NONE, M_RN),             // ROTCR Rn
    E2(0x4026, TEXT_LDS,     ISZ_LONG, M_INCN,  M_PR),    // LDS.L @Rm+,PR
    E2(0x4027, TEXT_LDC,     ISZ_LONG, M_INCN,  M_VBR),   // LDC.L @Rm+,VBR
    E1(0x4028, TEXT_SHLL16,  ISZ_NONE, M_RN),             // SHLL16 Rn
    E1(0x4029, TEXT_SHLR16,  ISZ_NONE, M_RN),             // SHLR16 Rn
    E2(0x402A, TEXT_LDS,     ISZ_NONE, M_RN,    M_PR),    // LDS Rm,PR
    E1(0x402B, TEXT_JMP,     ISZ_NONE, M_IRN),            // JMP @Rm
    E2(0x402E, TEXT_LDC,     ISZ_NONE, M_RN,    M_VBR),   // LDC Rm,VBR
    E2(0x5000, TEXT_MOV,     ISZ_LONG, M_D4M,   M_RN),    // MOV.L @(disp,Rm),Rn
    E2(0x6002, TEXT_MOV,     ISZ_DATA, M_IRM,   M_RN),    // MOV.B/W/L @Rm,Rn
    E2(0x6003, TEXT_MOV,     ISZ_NONE, M_RM,    M_RN),    // MOV Rm,Rn
    E2(0x6006, TEXT_MOV,     ISZ_DATA, M_INCM,  M_RN),    // MOV.B/W/L @Rm+,Rn
    E2(0x6007, TEXT_NOT,     ISZ_NONE, M_RM,    M_RN),    // NOT Rm,Rn
    E2(0x6009, TEXT_SWAP,    ISZ_DATA, M_RM,    M_RN),    // SWAP.B/W Rm,Rn
    E2(0x600A, TEXT_NEGC,    ISZ_NONE, M_RM,    M_RN),    // NEGC Rm,Rn
    E2(0x600B, TEXT_NEG,     ISZ_NONE, M_RM,    M_RN),    // NEG Rm,Rn
    E2(0x600D, TEXT_EXTU,    ISZ_DATA, M_RM,    M_RN),    // EXTU.B/W Rm,Rn
    E2(0x600F, TEXT_EXTS,    ISZ_DATA, M_RM,    M_RN),    // EXTS.B/W Rm,Rn
    E2(0x7000, TEXT_ADD,     ISZ_NONE, M_IMM8,  M_RN),    // ADD #imm,Rn
    E2(0x8100, TEXT_MOV,     ISZ_DATA, M_R0,    M_D4M),   // MOV.B/W R0,@(disp,Rn)
    E2(0x8500, TEXT_MOV,     ISZ_DATA, M_D4M,   M_R0),    // MOV.B/W @(disp,Rm),R0
    E2(0x8800, TEXT_CMP_EQ,  ISZ_NONE, M_IMM8,  M_R0),    // CMP/EQ #imm,R0
    E1(0x8900, TEXT_BT,      ISZ_NONE, M_REL8),           // BT label
    E1(0x8B00, TEXT_BF,      ISZ_NONE, M_REL8),           // BF label
    E1(0xA000, TEXT_BRA,     ISZ_NONE, M_REL12),          // BRA label
    E1(0xB000, TEXT_BSR,     ISZ_NONE, M_REL12),          // BSR label
    E2(0xC200, TEXT_MOV,     ISZ_DATA, M_R0,    M_D8),    // MOV.B/W/L R0,@(disp,GBR)
    E1(0xC300, TEXT_TRAPA,   ISZ_NONE, M_TVEC),           // TRAPA #imm
    E2(0xC600, TEXT_MOV,     ISZ_DATA, M_D8,    M_R0),    // MOV.B/W/L @(disp,GBR),R0
    E2(0xC700, TEXT_MOVA,    ISZ_NONE, M_PCREL, M_R0),    // MOVA @(disp,PC),R0
    E2(0xC800, TEXT_TST,     ISZ_NONE, M_IMM8,  M_R0),    // TST #imm,R0
    E2(0xC900, TEXT_AND,     ISZ_NONE, M_IMM8,  M_R0),    // AND #imm,R0
    E2(0xCA00, TEXT_XOR,     ISZ_NONE, M_IMM8,  M_R0),    // XOR #imm,R0
    E2(0xCB00, TEXT_OR,      ISZ_NONE, M_IMM8,  M_R0),    // OR #imm,R0
    E2(0xCC00, TEXT_TST,     ISZ_BYTE, M_IMM8,  M_IGBR),  // TST.B #imm,@(R0,GBR)
    E2(0xCD00, TEXT_AND,     ISZ_BYTE, M_IMM8,  M_IGBR),  // AND.B #imm,@(R0,GBR)
    E2(0xCE00, TEXT_XOR,     ISZ_BYTE, M_IMM8,  M_IGBR),  // XOR.B #imm,@(R0,GBR)
    E2(0xCF00, TEXT_OR,      ISZ_BYTE, M_IMM8,  M_IGBR),  // OR.B #imm,@(R0,GBR)
    E2(0xD000, TEXT_MOV,     ISZ_DATA, M_PCREL, M_RN),    // MOV.W/L @(disp,PC),Rn
    E2(0xE000, TEXT_MOV,     ISZ_NONE, M_IMM8,  M_RN),    // MOV #imm,Rn
};

// SH-2-only additions.
constexpr Entry TABLE_SH2[] PROGMEM = {
    E1(0x0003, TEXT_BSRF,    ISZ_NONE, M_RN),            // BSRF Rm
    E2(0x0007, TEXT_MUL,     ISZ_LONG, M_RM,    M_RN),   // MUL.L Rm,Rn
    E2(0x000F, TEXT_MAC_L,   ISZ_NONE, M_INCM,  M_INCN), // MAC.L @Rm+,@Rn+
    E1(0x0023, TEXT_BRAF,    ISZ_NONE, M_RN),            // BRAF Rm
    E2(0x3005, TEXT_DMULU,   ISZ_LONG, M_RM,    M_RN),   // DMULU.L Rm,Rn
    E2(0x300D, TEXT_DMULS,   ISZ_LONG, M_RM,    M_RN),   // DMULS.L Rm,Rn
    E1(0x4010, TEXT_DT,      ISZ_NONE, M_RN),            // DT Rn
    E1(0x8D00, TEXT_BT_S,    ISZ_NONE, M_REL8),          // BT/S label
    E1(0x8F00, TEXT_BF_S,    ISZ_NONE, M_REL8),          // BF/S label
};

// SH-DSP additions: DSP register file (DSR/A0/X0/X1/Y0/Y1), repeat control
// (MOD/RS/RE), and the LDRS/LDRE/SETRC repeat instructions.
constexpr Entry TABLE_DSP[] PROGMEM = {
    E2(0x0052, TEXT_STC,     ISZ_NONE, M_MOD,   M_RN),   // STC MOD,Rn
    E2(0x0062, TEXT_STC,     ISZ_NONE, M_RS,    M_RN),   // STC RS,Rn
    E2(0x006A, TEXT_STS,     ISZ_NONE, M_DSR,   M_RN),   // STS DSR,Rn
    E2(0x0072, TEXT_STC,     ISZ_NONE, M_RE,    M_RN),   // STC RE,Rn
    E2(0x007A, TEXT_STS,     ISZ_NONE, M_A0,    M_RN),   // STS A0,Rn
    E2(0x008A, TEXT_STS,     ISZ_NONE, M_X0,    M_RN),   // STS X0,Rn
    E2(0x009A, TEXT_STS,     ISZ_NONE, M_X1,    M_RN),   // STS X1,Rn
    E2(0x00AA, TEXT_STS,     ISZ_NONE, M_Y0,    M_RN),   // STS Y0,Rn
    E2(0x00BA, TEXT_STS,     ISZ_NONE, M_Y1,    M_RN),   // STS Y1,Rn
    E1(0x4014, TEXT_SETRC,   ISZ_NONE, M_RN),            // SETRC Rm  (n-field used for Rm)
    E2(0x4053, TEXT_STC,     ISZ_LONG, M_MOD,   M_DECN), // STC.L MOD,@-Rn
    E2(0x4057, TEXT_LDC,     ISZ_LONG, M_INCN,  M_MOD),  // LDC.L @Rm+,MOD
    E2(0x405E, TEXT_LDC,     ISZ_NONE, M_RN,    M_MOD),  // LDC Rm,MOD
    E2(0x4062, TEXT_STS,     ISZ_LONG, M_DSR,   M_DECN), // STS.L DSR,@-Rn
    E2(0x4063, TEXT_STC,     ISZ_LONG, M_RS,    M_DECN), // STC.L RS,@-Rn
    E2(0x4066, TEXT_LDS,     ISZ_LONG, M_INCN,  M_DSR),  // LDS.L @Rm+,DSR
    E2(0x4067, TEXT_LDC,     ISZ_LONG, M_INCN,  M_RS),   // LDC.L @Rm+,RS
    E2(0x406A, TEXT_LDS,     ISZ_NONE, M_RN,    M_DSR),  // LDS Rm,DSR
    E2(0x406E, TEXT_LDC,     ISZ_NONE, M_RN,    M_RS),   // LDC Rm,RS
    E2(0x4072, TEXT_STS,     ISZ_LONG, M_A0,    M_DECN), // STS.L A0,@-Rn
    E2(0x4073, TEXT_STC,     ISZ_LONG, M_RE,    M_DECN), // STC.L RE,@-Rn
    E2(0x4076, TEXT_LDS,     ISZ_LONG, M_INCN,  M_A0),   // LDS.L @Rm+,A0
    E2(0x4077, TEXT_LDC,     ISZ_LONG, M_INCN,  M_RE),   // LDC.L @Rm+,RE
    E2(0x407A, TEXT_LDS,     ISZ_NONE, M_RN,    M_A0),   // LDS Rm,A0
    E2(0x407E, TEXT_LDC,     ISZ_NONE, M_RN,    M_RE),   // LDC Rm,RE
    E2(0x4082, TEXT_STS,     ISZ_LONG, M_X0,    M_DECN), // STS.L X0,@-Rn
    E2(0x4086, TEXT_LDS,     ISZ_LONG, M_INCN,  M_X0),   // LDS.L @Rm+,X0
    E2(0x408A, TEXT_LDS,     ISZ_NONE, M_RN,    M_X0),   // LDS Rm,X0
    E2(0x4092, TEXT_STS,     ISZ_LONG, M_X1,    M_DECN), // STS.L X1,@-Rn
    E2(0x4096, TEXT_LDS,     ISZ_LONG, M_INCN,  M_X1),   // LDS.L @Rm+,X1
    E2(0x409A, TEXT_LDS,     ISZ_NONE, M_RN,    M_X1),   // LDS Rm,X1
    E2(0x40A2, TEXT_STS,     ISZ_LONG, M_Y0,    M_DECN), // STS.L Y0,@-Rn
    E2(0x40A6, TEXT_LDS,     ISZ_LONG, M_INCN,  M_Y0),   // LDS.L @Rm+,Y0
    E2(0x40AA, TEXT_LDS,     ISZ_NONE, M_RN,    M_Y0),   // LDS Rm,Y0
    E2(0x40B2, TEXT_STS,     ISZ_LONG, M_Y1,    M_DECN), // STS.L Y1,@-Rn
    E2(0x40B6, TEXT_LDS,     ISZ_LONG, M_INCN,  M_Y1),   // LDS.L @Rm+,Y1
    E2(0x40BA, TEXT_LDS,     ISZ_NONE, M_RN,    M_Y1),   // LDS Rm,Y1
    E1(0x8200, TEXT_SETRC,   ISZ_NONE, M_TVEC),          // SETRC #imm
    E1(0x8C00, TEXT_LDRS,    ISZ_NONE, M_REL8P),         // LDRS @(disp,PC)
    E1(0x8E00, TEXT_LDRE,    ISZ_NONE, M_REL8P),         // LDRE @(disp,PC)
};

// SH-2A 16-bit CPU additions.
constexpr Entry TABLE_SH2A[] PROGMEM = {
    E1(0x0039, TEXT_MOVRT,   ISZ_NONE, M_RN),            // MOVRT Rn        0000nnnn00111001
    E0(0x005B, TEXT_RESBANK, ISZ_NONE),                  // RESBANK         0000000001011011
    E0(0x0068, TEXT_NOTT,    ISZ_NONE),                  // NOTT            0000000001101000
    E0(0x006B, TEXT_RTS_N,   ISZ_NONE),                  // RTS/N           0000000001101011
    E1(0x007B, TEXT_RTV_N,   ISZ_NONE, M_RN),            // RTV/N Rm        0000mmmm01111011  (m in n slot per encoding)
    E1(0x0083, TEXT_PREF,    ISZ_NONE, M_IRN),           // PREF @Rn        0000nnnn10000011
    E1(0x404B, TEXT_JSR_N,   ISZ_NONE, M_IRN),           // JSR/N @Rn       0100nnnn01001011  (manual labels n as "m")
    E2(0x400C, TEXT_SHAD,    ISZ_NONE, M_RM,    M_RN),   // SHAD Rm,Rn      0100nnnnmmmm1100
    E2(0x400D, TEXT_SHLD,    ISZ_NONE, M_RM,    M_RN),   // SHLD Rm,Rn      0100nnnnmmmm1101
    E1(0x4085, TEXT_CLIPU,   ISZ_DATA, M_RN),            // CLIPU.B/W Rn    0100nnnn1000 0w 01
    E2(0x4084, TEXT_DIVU,    ISZ_NONE, M_R0,    M_RN),   // DIVU R0,Rn      0100nnnn10000100
    E2(0x4080, TEXT_MULR,    ISZ_NONE, M_R0,    M_RN),   // MULR R0,Rn      0100nnnn10000000
    E1(0x4095, TEXT_CLIPS,   ISZ_DATA, M_RN),            // CLIPS.B/W Rn    0100nnnn1001 0w 01
    E2(0x4094, TEXT_DIVS,    ISZ_NONE, M_R0,    M_RN),   // DIVS R0,Rn      0100nnnn10010100
    E2(0x40E1, TEXT_STBANK,  ISZ_NONE, M_R0,    M_IRN),  // STBANK R0,@Rn   0100nnnn11100001
    E2(0x40E5, TEXT_LDBANK,  ISZ_NONE, M_IRN,   M_R0),   // LDBANK @Rn,R0   0100nnnn11100101  (manual labels n as "m")
    E2(0x40F0, TEXT_MOVMU,   ISZ_LONG, M_RN,    M_PUSH), // MOVMU.L Rn,@-R15
    E2(0x40F1, TEXT_MOVML,   ISZ_LONG, M_RN,    M_PUSH), // MOVML.L Rn,@-R15
    E2(0x40F4, TEXT_MOVMU,   ISZ_LONG, M_PULL,  M_RN),   // MOVMU.L @R15+,Rn
    E2(0x40F5, TEXT_MOVML,   ISZ_LONG, M_PULL,  M_RN),   // MOVML.L @R15+,Rn
    E2(0x8600, TEXT_BCLR,    ISZ_NONE, M_IMM3,  M_RM),   // BCLR #imm3,Rn   10000110nnnniii0  (n in bits[7:4])
    E2(0x8601, TEXT_BSET,    ISZ_NONE, M_IMM3,  M_RM),   // BSET #imm3,Rn   10000110nnnniii1
    E2(0x8700, TEXT_BST,     ISZ_NONE, M_IMM3,  M_RM),   // BST #imm3,Rn    10000111nnnniii0
    E2(0x8701, TEXT_BLD,     ISZ_NONE, M_IMM3,  M_RM),   // BLD #imm3,Rn    10000111nnnniii1
};

// SH-2A 32-bit CPU additions. Each entry's opcode is the FIRST 16-bit word
// template (with variable bits zero); the disassembler reads a 2nd word when
// matched. Long form is inferred from the M_IMM20/M_IMM20S/M_D12 operands;
// word 2 carries the displacement, immediate, or bit field.
constexpr Entry TABLE_SH2A_LONG[] PROGMEM = {
    E2(0x0000, TEXT_MOVI20,  ISZ_NONE, M_IMM20,  M_RN),  // MOVI20 #imm20,Rn
    E2(0x0001, TEXT_MOVI20S, ISZ_NONE, M_IMM20S, M_RN),  // MOVI20S #imm20<<8,Rn
};

#if !defined(LIBASM_SUPERH_NOFPU)
// SH-2A FPU additions.
constexpr Entry TABLE_FPU_SH2A[] PROGMEM = {
    E2(0xF0AD, TEXT_FCNVSD,  ISZ_NONE, M_FPUL,  M_DRN),  // FCNVSD FPUL,DRn 1111nnn010101101
    E2(0xF0BD, TEXT_FCNVDS,  ISZ_NONE, M_DRN,   M_FPUL), // FCNVDS DRn,FPUL 1111nnn010111101
};

// FPU-related CPU instructions (manual table 5.10): LDS/STS to the FPU system
// registers FPUL/FPSCR. They are System Control (CPU) instructions by encoding,
// but they access FPU registers that exist only when the FPU is present, so
// like the floating-point page they are gated by the FPU (mandatory on SH-2E,
// optional on SH-2A). On an SH-2A without FPU these are undefined code.
constexpr Entry TABLE_FPU_CPU[] PROGMEM = {
    E2(0x005A, TEXT_STS,     ISZ_NONE, M_FPUL,  M_RN),   // STS FPUL,Rn
    E2(0x006A, TEXT_STS,     ISZ_NONE, M_FPSCR, M_RN),   // STS FPSCR,Rn
    E2(0x4052, TEXT_STS,     ISZ_LONG, M_FPUL,  M_DECN), // STS.L FPUL,@-Rn
    E2(0x4056, TEXT_LDS,     ISZ_LONG, M_INCN,  M_FPUL), // LDS.L @Rm+,FPUL
    E2(0x405A, TEXT_LDS,     ISZ_NONE, M_RN,    M_FPUL), // LDS Rm,FPUL
    E2(0x4062, TEXT_STS,     ISZ_LONG, M_FPSCR, M_DECN), // STS.L FPSCR,@-Rn
    E2(0x4066, TEXT_LDS,     ISZ_LONG, M_INCN,  M_FPSCR),// LDS.L @Rm+,FPSCR
    E2(0x406A, TEXT_LDS,     ISZ_NONE, M_RN,    M_FPSCR),// LDS Rm,FPSCR
};

// SH-2E floating-point instructions (shared with SH-2A when FPU is enabled).
// All encodings have the 0xF000 high nibble.
constexpr Entry TABLE_FPU[] PROGMEM = {
    E2(0xF000, TEXT_FADD,    ISZ_NONE, M_FRM,   M_FRN),  // FADD FRm,FRn
    E2(0xF001, TEXT_FSUB,    ISZ_NONE, M_FRM,   M_FRN),  // FSUB FRm,FRn
    E2(0xF002, TEXT_FMUL,    ISZ_NONE, M_FRM,   M_FRN),  // FMUL FRm,FRn
    E2(0xF003, TEXT_FDIV,    ISZ_NONE, M_FRM,   M_FRN),  // FDIV FRm,FRn
    E2(0xF004, TEXT_FCMP_EQ, ISZ_NONE, M_FRM,   M_FRN),  // FCMP/EQ FRm,FRn
    E2(0xF005, TEXT_FCMP_GT, ISZ_NONE, M_FRM,   M_FRN),  // FCMP/GT FRm,FRn
    E2(0xF006, TEXT_FMOV_S,  ISZ_NONE, M_IDXM,  M_FRN),  // FMOV.S @(R0,Rm),FRn
    E2(0xF007, TEXT_FMOV_S,  ISZ_NONE, M_FRM,   M_IDXN), // FMOV.S FRm,@(R0,Rn)
    E2(0xF008, TEXT_FMOV_S,  ISZ_NONE, M_IRM,   M_FRN),  // FMOV.S @Rm,FRn
    E2(0xF009, TEXT_FMOV_S,  ISZ_NONE, M_INCM,  M_FRN),  // FMOV.S @Rm+,FRn
    E2(0xF00A, TEXT_FMOV_S,  ISZ_NONE, M_FRM,   M_IRN),  // FMOV.S FRm,@Rn
    E2(0xF00B, TEXT_FMOV_S,  ISZ_NONE, M_FRM,   M_DECN), // FMOV.S FRm,@-Rn
    E2(0xF00C, TEXT_FMOV,    ISZ_NONE, M_FRM,   M_FRN),  // FMOV FRm,FRn
    E2(0xF00D, TEXT_FSTS,    ISZ_NONE, M_FPUL,  M_FRN),  // FSTS FPUL,FRn
    E2(0xF00E, TEXT_FMAC,    ISZ_NONE, M_FRM,   M_FRN),  // FMAC FR0,FRm,FRn
    E2(0xF01D, TEXT_FLDS,    ISZ_NONE, M_FRN,   M_FPUL), // FLDS FRm,FPUL  (FRm in n-slot)
    E2(0xF02D, TEXT_FLOAT,   ISZ_NONE, M_FPUL,  M_FRN),  // FLOAT FPUL,FRn
    E2(0xF03D, TEXT_FTRC,    ISZ_NONE, M_FRN,   M_FPUL), // FTRC FRm,FPUL  (FRm in n-slot)
    E1(0xF04D, TEXT_FNEG,    ISZ_NONE, M_FRN),           // FNEG FRn
    E1(0xF05D, TEXT_FABS,    ISZ_NONE, M_FRN),           // FABS FRn
    E1(0xF08D, TEXT_FLDI0,   ISZ_NONE, M_FRN),           // FLDI0 FRn
    E1(0xF09D, TEXT_FLDI1,   ISZ_NONE, M_FRN),           // FLDI1 FRn
};
#endif  // !LIBASM_SUPERH_NOFPU

// INDEX arrays sorted alphabetically by the mnemonic STRING (not by the
// TEXT_X identifier), so the binary-search comparator (strcasecmp_P on
// the mnemonic) sees the index ordering match its sort. Regenerate via
// the awk pipeline in this file's commit history if TABLE_* changes.
constexpr uint8_t INDEX_COMMON[] PROGMEM = {
     38,  // TEXT_ADD
     89,  // TEXT_ADD
     39,  // TEXT_ADDC
     40,  // TEXT_ADDV
     22,  // TEXT_AND
    102,  // TEXT_AND
    106,  // TEXT_AND
     94,  // TEXT_BF
     95,  // TEXT_BRA
     96,  // TEXT_BSR
     93,  // TEXT_BT
     13,  // TEXT_CLRMAC
      8,  // TEXT_CLRT
     29,  // TEXT_CMP_EQ
     92,  // TEXT_CMP_EQ
     31,  // TEXT_CMP_GE
     34,  // TEXT_CMP_GT
     33,  // TEXT_CMP_HI
     30,  // TEXT_CMP_HS
     58,  // TEXT_CMP_PL
     55,  // TEXT_CMP_PZ
     25,  // TEXT_CMP_STR
     20,  // TEXT_DIV0S
     10,  // TEXT_DIV0U
     32,  // TEXT_DIV1
     88,  // TEXT_EXTS
     87,  // TEXT_EXTU
     77,  // TEXT_JMP
     52,  // TEXT_JSR
     48,  // TEXT_LDC
     53,  // TEXT_LDC
     60,  // TEXT_LDC
     65,  // TEXT_LDC
     73,  // TEXT_LDC
     78,  // TEXT_LDC
     47,  // TEXT_LDS
     51,  // TEXT_LDS
     59,  // TEXT_LDS
     63,  // TEXT_LDS
     72,  // TEXT_LDS
     76,  // TEXT_LDS
     54,  // TEXT_MAC_W
      3,  // TEXT_MOV
      7,  // TEXT_MOV
     17,  // TEXT_MOV
     18,  // TEXT_MOV
     19,  // TEXT_MOV
     79,  // TEXT_MOV
     80,  // TEXT_MOV
     81,  // TEXT_MOV
     82,  // TEXT_MOV
     90,  // TEXT_MOV
     91,  // TEXT_MOV
     97,  // TEXT_MOV
     99,  // TEXT_MOV
    109,  // TEXT_MOV
    110,  // TEXT_MOV
    100,  // TEXT_MOVA
     14,  // TEXT_MOVT
     28,  // TEXT_MULS
     27,  // TEXT_MULU
     86,  // TEXT_NEG
     85,  // TEXT_NEGC
      4,  // TEXT_NOP
     83,  // TEXT_NOT
     24,  // TEXT_OR
    104,  // TEXT_OR
    108,  // TEXT_OR
     70,  // TEXT_ROTCL
     71,  // TEXT_ROTCR
     45,  // TEXT_ROTL
     46,  // TEXT_ROTR
     16,  // TEXT_RTE
      6,  // TEXT_RTS
      9,  // TEXT_SETT
     66,  // TEXT_SHAL
     67,  // TEXT_SHAR
     41,  // TEXT_SHLL
     74,  // TEXT_SHLL16
     49,  // TEXT_SHLL2
     61,  // TEXT_SHLL8
     42,  // TEXT_SHLR
     75,  // TEXT_SHLR16
     50,  // TEXT_SHLR2
     62,  // TEXT_SHLR8
     12,  // TEXT_SLEEP
      0,  // TEXT_STC
      1,  // TEXT_STC
      2,  // TEXT_STC
     44,  // TEXT_STC
     57,  // TEXT_STC
     69,  // TEXT_STC
      5,  // TEXT_STS
     11,  // TEXT_STS
     15,  // TEXT_STS
     43,  // TEXT_STS
     56,  // TEXT_STS
     68,  // TEXT_STS
     35,  // TEXT_SUB
     36,  // TEXT_SUBC
     37,  // TEXT_SUBV
     84,  // TEXT_SWAP
     64,  // TEXT_TAS
     98,  // TEXT_TRAPA
     21,  // TEXT_TST
    101,  // TEXT_TST
    105,  // TEXT_TST
     23,  // TEXT_XOR
    103,  // TEXT_XOR
    107,  // TEXT_XOR
     26,  // TEXT_XTRCT
};

constexpr uint8_t INDEX_SH2[] PROGMEM = {
      8,  // TEXT_BF_S
      3,  // TEXT_BRAF
      0,  // TEXT_BSRF
      7,  // TEXT_BT_S
      5,  // TEXT_DMULS
      4,  // TEXT_DMULU
      6,  // TEXT_DT
      2,  // TEXT_MAC_L
      1,  // TEXT_MUL
};

constexpr uint8_t INDEX_SH2A[] PROGMEM = {
     20,  // TEXT_BCLR
     23,  // TEXT_BLD
     21,  // TEXT_BSET
     22,  // TEXT_BST
     12,  // TEXT_CLIPS
      9,  // TEXT_CLIPU
     13,  // TEXT_DIVS
     10,  // TEXT_DIVU
      6,  // TEXT_JSR_N
     15,  // TEXT_LDBANK
     17,  // TEXT_MOVML
     19,  // TEXT_MOVML
     16,  // TEXT_MOVMU
     18,  // TEXT_MOVMU
      0,  // TEXT_MOVRT
     11,  // TEXT_MULR
      2,  // TEXT_NOTT
      5,  // TEXT_PREF
      1,  // TEXT_RESBANK
      3,  // TEXT_RTS_N
      4,  // TEXT_RTV_N
      7,  // TEXT_SHAD
      8,  // TEXT_SHLD
     14,  // TEXT_STBANK
};
constexpr uint8_t INDEX_SH2A_LONG[] PROGMEM = {
      0,  // TEXT_MOVI20
      1,  // TEXT_MOVI20S
};
#if !defined(LIBASM_SUPERH_NOFPU)
constexpr uint8_t INDEX_FPU_SH2A[] PROGMEM = {
      1,  // TEXT_FCNVDS
      0,  // TEXT_FCNVSD
};

constexpr uint8_t INDEX_FPU_CPU[] PROGMEM = {
      4,  // TEXT_LDS
      7,  // TEXT_LDS
      3,  // TEXT_LDS
      6,  // TEXT_LDS
      0,  // TEXT_STS
      1,  // TEXT_STS
      2,  // TEXT_STS
      5,  // TEXT_STS
};

constexpr uint8_t INDEX_FPU[] PROGMEM = {
     19,  // TEXT_FABS
      0,  // TEXT_FADD
      4,  // TEXT_FCMP_EQ
      5,  // TEXT_FCMP_GT
      3,  // TEXT_FDIV
     20,  // TEXT_FLDI0
     21,  // TEXT_FLDI1
     15,  // TEXT_FLDS
     16,  // TEXT_FLOAT
     14,  // TEXT_FMAC
     12,  // TEXT_FMOV
      6,  // TEXT_FMOV_S
      7,  // TEXT_FMOV_S
      8,  // TEXT_FMOV_S
      9,  // TEXT_FMOV_S
     10,  // TEXT_FMOV_S
     11,  // TEXT_FMOV_S
      2,  // TEXT_FMUL
     18,  // TEXT_FNEG
     13,  // TEXT_FSTS
      1,  // TEXT_FSUB
     17,  // TEXT_FTRC
};
#endif  // !LIBASM_SUPERH_NOFPU

constexpr uint8_t INDEX_DSP[] PROGMEM = {
     11,  // TEXT_LDC
     12,  // TEXT_LDC
     16,  // TEXT_LDC
     18,  // TEXT_LDC
     22,  // TEXT_LDC
     24,  // TEXT_LDC
     39,  // TEXT_LDRE
     38,  // TEXT_LDRS
     15,  // TEXT_LDS
     17,  // TEXT_LDS
     21,  // TEXT_LDS
     23,  // TEXT_LDS
     26,  // TEXT_LDS
     27,  // TEXT_LDS
     29,  // TEXT_LDS
     30,  // TEXT_LDS
     32,  // TEXT_LDS
     33,  // TEXT_LDS
     35,  // TEXT_LDS
     36,  // TEXT_LDS
      9,  // TEXT_SETRC
     37,  // TEXT_SETRC
      0,  // TEXT_STC
      1,  // TEXT_STC
      3,  // TEXT_STC
     10,  // TEXT_STC
     14,  // TEXT_STC
     20,  // TEXT_STC
      2,  // TEXT_STS
      4,  // TEXT_STS
      5,  // TEXT_STS
      6,  // TEXT_STS
      7,  // TEXT_STS
      8,  // TEXT_STS
     13,  // TEXT_STS
     19,  // TEXT_STS
     25,  // TEXT_STS
     28,  // TEXT_STS
     31,  // TEXT_STS
     34,  // TEXT_STS
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

// SH-DSP is SH-2 plus the orthogonal DSP feature (DSP page comes from the DSP
// dispatch table below, enabled by DSP_ON).
constexpr EntryPage SH_DSP_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
        {ARRAY_RANGE(TABLE_SH2), ARRAY_RANGE(INDEX_SH2)},
};

// SH-2E inherits SH-2 instructions. The FPU page (floating-point plus the
// FPU-related CPU instructions) comes from the FPU dispatch table below
// (mandatory FPU_SH2E).
constexpr EntryPage SH2E_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
        {ARRAY_RANGE(TABLE_SH2), ARRAY_RANGE(INDEX_SH2)},
};

// SH-2A: SH-2 + SH-2A 16-bit + SH-2A 32-bit. The FPU page is optional via
// FPU_TABLE; without it the FPU system registers do not exist.
constexpr EntryPage SH2A_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
        {ARRAY_RANGE(TABLE_SH2), ARRAY_RANGE(INDEX_SH2)},
        {ARRAY_RANGE(TABLE_SH2A), ARRAY_RANGE(INDEX_SH2A)},
        {ARRAY_RANGE(TABLE_SH2A_LONG), ARRAY_RANGE(INDEX_SH2A_LONG)},
};

#if !defined(LIBASM_SUPERH_NOFPU)
constexpr EntryPage FPU_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_FPU_CPU), ARRAY_RANGE(INDEX_FPU_CPU)},
        {ARRAY_RANGE(TABLE_FPU), ARRAY_RANGE(INDEX_FPU)},
};

constexpr EntryPage FPU_SH2A_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_FPU_CPU), ARRAY_RANGE(INDEX_FPU_CPU)},
        {ARRAY_RANGE(TABLE_FPU), ARRAY_RANGE(INDEX_FPU)},
        {ARRAY_RANGE(TABLE_FPU_SH2A), ARRAY_RANGE(INDEX_FPU_SH2A)},
};
#endif  // !LIBASM_SUPERH_NOFPU

constexpr EntryPage DSP_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_DSP), ARRAY_RANGE(INDEX_DSP)},
};

#define EMPTY_RANGE(a) ARRAY_BEGIN(a), ARRAY_BEGIN(a)

using Cpu = entry::CpuBase<CpuType, EntryPage>;
#if !defined(LIBASM_SUPERH_NOFPU)
using Fpu = entry::CpuBase<FpuType, EntryPage>;
#endif
using Dsp = entry::CpuBase<DspType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {SH1, TEXT_CPU_SH1, ARRAY_RANGE(SH1_PAGES)},
        {SH2, TEXT_CPU_SH2, ARRAY_RANGE(SH2_PAGES)},
        {SH_DSP, TEXT_CPU_SH_DSP, ARRAY_RANGE(SH_DSP_PAGES)},
        {SH2E, TEXT_CPU_SH2E, ARRAY_RANGE(SH2E_PAGES)},
        {SH2A, TEXT_CPU_SH2A, ARRAY_RANGE(SH2A_PAGES)},
};

#if !defined(LIBASM_SUPERH_NOFPU)
constexpr Fpu FPU_TABLE[] PROGMEM = {
        {FPU_NONE, TEXT_FPU_NONE, EMPTY_RANGE(FPU_PAGES)},
        {FPU_SH2E, TEXT_FPU_SH2E, ARRAY_RANGE(FPU_PAGES)},
        {FPU_SH2A, TEXT_FPU_SH2A, ARRAY_RANGE(FPU_SH2A_PAGES)},
};
#endif  // !LIBASM_SUPERH_NOFPU

// DSP is enabled only by the SH-DSP CPU, so the names are internal (not parsed).
constexpr Dsp DSP_TABLE[] PROGMEM = {
        {DSP_NONE, TEXT_FPU_NONE, EMPTY_RANGE(DSP_PAGES)},
        {DSP_ON, TEXT_CPU_SH_DSP, ARRAY_RANGE(DSP_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

#if !defined(LIBASM_SUPERH_NOFPU)
static const Fpu *fpu(FpuType fpuType) {
    return Fpu::search(fpuType, ARRAY_RANGE(FPU_TABLE));
}
#endif

static const Dsp *dsp(DspType dspType) {
    return Dsp::search(dspType, ARRAY_RANGE(DSP_TABLE));
}

#if !defined(LIBASM_SUPERH_NOFPU)
// Resolve the on/off FPU intent to the current CPU's actual FPU page: SH-2E and
// SH-2A have (different) FPUs; SH-1/SH-2/SH-DSP have none, so the intent is
// gated off there.
static FpuType effectiveFpu(const CpuSpec &cpuSpec) {
    if (cpuSpec.fpu == FPU_NONE)
        return FPU_NONE;
    if (cpuSpec.cpu == SH2A)
        return FPU_SH2A;
    if (cpuSpec.cpu == SH2E)
        return FPU_SH2E;
    return FPU_NONE;
}
#endif  // !LIBASM_SUPERH_NOFPU

// matchOpCode: mask out variable bits (register/immediate fields) before comparing.
static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    const auto flags = entry->readFlags();
    // Mask both sides: an ISZ_DATA template stores its default size in the size
    // field, so those bits are non-zero in the stored opcode too.
    const auto mask = ~flags.opcodeMask();
    if ((insn.opCode() & mask) != (entry->readOpCode() & mask))
        return false;
    // An ISZ_DATA size field may have unused slots (e.g. the value-3 hole in a
    // 2-bit B/W/L field). Reject opcodes that land in one so a collapsed row
    // doesn't shadow a distinct instruction sharing the masked template
    // (e.g. MUL.L 0x0007 vs MOV @(R0,Rn), or TRAPA 0xC300 vs MOV @(disp,GBR)).
    if (flags.insnSize() == ISZ_DATA &&
            decodeDataSize(flags.src(), flags.dst(), insn.opCode()) == ISZ_NONE)
        return false;
    return true;
}

Error searchOpCode(const CpuSpec &cpuSpec, DisInsn &insn, StrBuffer &out) {
    cpu(cpuSpec.cpu)->searchOpCode(insn, out, matchOpCode);
#if !defined(LIBASM_SUPERH_NOFPU)
    if (insn.getError() == UNKNOWN_INSTRUCTION) {
        insn.nameBuffer().reset();
        out.reset();
        insn.setError(insn.errorAt(), OK);
        fpu(effectiveFpu(cpuSpec))->searchOpCode(insn, out, matchOpCode);
    }
#endif
    if (insn.getError() == UNKNOWN_INSTRUCTION) {
        insn.nameBuffer().reset();
        out.reset();
        insn.setError(insn.errorAt(), OK);
        dsp(cpuSpec.dsp)->searchOpCode(insn, out, matchOpCode);
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
    case M_PCREL:
        // MOV.W/MOV.L/MOVA also accept a bare symbol target (manual table 3-1,
        // "symbol" addressing mode); the parser tags it M_REL8.
        return op.mode == M_REL8;
    case M_REL8P:
        // LDRS/LDRE accept @(target,PC) or a bare symbol target.
        return op.mode == M_PCREL || op.mode == M_REL8;
    case M_TVEC:
        return op.mode == M_IMM8;
    case M_REL12:
        return op.mode == M_REL8;
    case M_FRM:
        return op.mode == M_FRN;  // parser tags FR registers as M_FRN
    case M_DRM:
        return op.mode == M_DRN;  // parser tags DR registers as M_DRN
    case M_IMM3:
        return op.mode == M_IMM8;  // unsigned 3-bit fits in IMM8
    case M_IMM20:
    case M_IMM20S:
        return op.mode == M_IMM8;  // 20/28-bit signed via integer parse
    case M_PUSH:
        return op.mode == M_DECN && op.reg == REG_R15;  // @-R15 implicit
    case M_PULL:
        return op.mode == M_INCN && op.reg == REG_R15;  // @R15+ implicit
    case M_BANK:
        return op.mode == M_RN && op.reg == REG_R0;  // STBANK/LDBANK R0
    default:
        return false;
    }
}

// Match the parsed .B/.W/.L suffix against the entry's size. ISZ_DATA accepts a
// bare mnemonic (the default lives in the opcode) or any size its field can
// hold; fixed entries accept their own size or a bare mnemonic; ISZ_NONE
// rejects any suffix.
static bool acceptSize(const AsmInsn &insn, const Entry::Flags &flags) {
    const auto parsed = insn.parsedSize();
    const auto isz = flags.insnSize();
    if (isz == ISZ_DATA) {
        if (parsed == ISZ_NONE)
            return true;
        const auto sf = sizeFieldOf(flags.src(), flags.dst());
        for (const auto s : sf.sizes) {
            if (s == parsed)
                return true;
        }
        return false;
    }
    if (isz == ISZ_NONE)
        return parsed == ISZ_NONE;
    return parsed == ISZ_NONE || parsed == isz;
}

static bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto flags = entry->readFlags();
    return acceptSize(insn, flags) && acceptMode(insn.srcOp, flags.src()) &&
            acceptMode(insn.dstOp, flags.dst());
}

Error searchName(const CpuSpec &cpuSpec, AsmInsn &insn) {
    cpu(cpuSpec.cpu)->searchName(insn, acceptModes);
    // LDS / STS / STC / LDC exist on the CPU page and the FPU and DSP pages
    // with different operands, so an operand-not-allowed can mean "try the next
    // page" rather than a real error. Fall back CPU -> FPU -> DSP.
    auto err = insn.getError();
#if !defined(LIBASM_SUPERH_NOFPU)
    if (err == UNKNOWN_INSTRUCTION || err == OPERAND_NOT_ALLOWED) {
        insn.setError(insn.errorAt(), OK);
        fpu(effectiveFpu(cpuSpec))->searchName(insn, acceptModes);
        if (insn.getError() == UNKNOWN_INSTRUCTION)
            insn.setError(insn.errorAt(), err);
    }
    err = insn.getError();
#endif
    if (err == UNKNOWN_INSTRUCTION || err == OPERAND_NOT_ALLOWED) {
        insn.setError(insn.errorAt(), OK);
        dsp(cpuSpec.dsp)->searchName(insn, acceptModes);
        if (insn.getError() == UNKNOWN_INSTRUCTION)
            insn.setError(insn.errorAt(), err);
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
    _cpuSpec.cpu = cpuType;
#if !defined(LIBASM_SUPERH_NOFPU)
    // SH-2E mandates its FPU; other CPUs keep the current on/off intent so a
    // setCpu (e.g. gen_driver's repeated setCpu) never clobbers it. effectiveFpu
    // gates the intent off on CPUs without an FPU.
    if (cpuType == SH2E)
        _cpuSpec.fpu = FPU_ON;
#endif
    // The DSP unit is present only on SH-DSP.
    _cpuSpec.dsp = (cpuType == SH_DSP) ? DSP_ON : DSP_NONE;
}

const /* PROGMEM */ char *Config::fpu_P() const {
#if defined(LIBASM_SUPERH_NOFPU)
    return TEXT_FPU_NONE;
#else
    return fpu(effectiveFpu(_cpuSpec))->name_P();
#endif
}

Error Config::setFpuType(FpuType fpuType) {
#if defined(LIBASM_SUPERH_NOFPU)
    // No FPU compiled in: the only FpuType is FPU_NONE.
    _cpuSpec.fpu = fpuType;
    return OK;
#else
    // Store only the on/off intent; effectiveFpu() resolves it per CPU. SH-2E
    // mandates its FPU, so it cannot be turned off there.
    if (fpuType == FPU_NONE) {
        if (cpuType() == SH2E)
            return UNKNOWN_OPERAND;
        _cpuSpec.fpu = FPU_NONE;
        return OK;
    }
    _cpuSpec.fpu = FPU_ON;
    return OK;
#endif
}

Error Config::setFpuName(StrScanner &scan) {
    if (scan.expectFalse())
        return setFpuType(FPU_NONE);
#if !defined(LIBASM_SUPERH_NOFPU)
    if (scan.expectTrue())
        return setFpuType(FPU_ON);
#endif
    return UNKNOWN_OPERAND;
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
