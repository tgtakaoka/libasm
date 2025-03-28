/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "table_mc6805.h"
#include "entry_mc6805.h"
#include "entry_table.h"
#include "text_mc6805.h"

using namespace libasm::text::mc6805;

namespace libasm {
namespace mc6805 {

#define E3(_opc, _name, _opr1, _opr2, _opr3) \
    {_opc, Entry::Flags::create(_opr1, _opr2, _opr3), _name}
#define E2(_opc, _name, _opr1, _opr2) E3(_opc, _name, _opr1, _opr2, M_NONE)
#define E1(_opc, _name, _opr1) E2(_opc, _name, _opr1, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)
#define U1(_opc, _name, _opr1) {_opc, Entry::Flags::undef(_opr1), _name}

// clang-format off
constexpr Entry MC6805_TABLE[] PROGMEM = {
    E1(0x20, TEXT_BRA,  M_REL),
    E1(0x21, TEXT_BRN,  M_REL),
    E1(0x22, TEXT_BHI,  M_REL),
    E1(0x23, TEXT_BLS,  M_REL),
    E1(0x24, TEXT_BHS,  M_REL),
    E1(0x24, TEXT_BCC,  M_REL),
    E1(0x25, TEXT_BLO,  M_REL),
    E1(0x25, TEXT_BCS,  M_REL),
    E1(0x26, TEXT_BNE,  M_REL),
    E1(0x27, TEXT_BEQ,  M_REL),
    E1(0x28, TEXT_BHCC, M_REL),
    E1(0x29, TEXT_BHCS, M_REL),
    E1(0x2A, TEXT_BPL,  M_REL),
    E1(0x2B, TEXT_BMI,  M_REL),
    E1(0x2C, TEXT_BMC,  M_REL),
    E1(0x2D, TEXT_BMS,  M_REL),
    E1(0x2E, TEXT_BIL,  M_REL),
    E1(0x2F, TEXT_BIH,  M_REL),
    E1(0x30, TEXT_NEG,  M_MEM),
    E1(0x33, TEXT_COM,  M_MEM),
    E1(0x34, TEXT_LSR,  M_MEM),
    E1(0x36, TEXT_ROR,  M_MEM),
    E1(0x37, TEXT_ASR,  M_MEM),
    E1(0x38, TEXT_ASL,  M_MEM),
    E1(0x38, TEXT_LSL,  M_MEM),
    E1(0x39, TEXT_ROL,  M_MEM),
    E1(0x3A, TEXT_DEC,  M_MEM),
    E1(0x3C, TEXT_INC,  M_MEM),
    E1(0x3D, TEXT_TST,  M_MEM),
    E1(0x3F, TEXT_CLR,  M_MEM),
    E0(0x40, TEXT_NEGA),
    E0(0x43, TEXT_COMA),
    E0(0x44, TEXT_LSRA),
    E0(0x46, TEXT_RORA),
    E0(0x47, TEXT_ASRA),
    E0(0x48, TEXT_ASLA),
    E0(0x48, TEXT_LSLA),
    E0(0x49, TEXT_ROLA),
    E0(0x4A, TEXT_DECA),
    E0(0x4C, TEXT_INCA),
    E0(0x4D, TEXT_TSTA),
    E0(0x4F, TEXT_CLRA),
    E0(0x50, TEXT_NEGX),
    E0(0x53, TEXT_COMX),
    E0(0x54, TEXT_LSRX),
    E0(0x56, TEXT_RORX),
    E0(0x57, TEXT_ASRX),
    E0(0x58, TEXT_ASLX),
    E0(0x58, TEXT_LSLX),
    E0(0x59, TEXT_ROLX),
    E0(0x5A, TEXT_DECX),
    E0(0x5C, TEXT_INCX),
    E0(0x5D, TEXT_TSTX),
    E0(0x5F, TEXT_CLRX),
    E1(0xA0, TEXT_SUB,  M_GEN),
    E1(0xA1, TEXT_CMP,  M_GEN),
    E1(0xA2, TEXT_SBC,  M_GEN),
    E1(0xA3, TEXT_CPX,  M_GEN),
    E1(0xA4, TEXT_AND,  M_GEN),
    E1(0xA5, TEXT_BIT,  M_GEN),
    E1(0xA6, TEXT_LDA,  M_GEN),
    U1(0xA7, TEXT_STA,  M_IMM),
    E1(0xA7, TEXT_STA,  M_GEN),
    E1(0xA8, TEXT_EOR,  M_GEN),
    E1(0xA9, TEXT_ADC,  M_GEN),
    E1(0xAA, TEXT_ORA,  M_GEN),
    E1(0xAB, TEXT_ADD,  M_GEN),
    U1(0xAC, TEXT_JMP,  M_IMM),
    E1(0xAC, TEXT_JMP,  M_GEN),
    E1(0xAE, TEXT_LDX,  M_GEN),
    U1(0xAF, TEXT_STX,  M_IMM),
    E1(0xAF, TEXT_STX,  M_GEN),
    E1(0xAD, TEXT_BSR,  M_REL),
    U1(0xAD, TEXT_JSR,  M_IMM),
    E1(0xAD, TEXT_JSR,  M_GEN),
    E0(0x80, TEXT_RTI),
    E0(0x81, TEXT_RTS),
    E0(0x83, TEXT_SWI),
    E0(0x97, TEXT_TAX),
    E0(0x98, TEXT_CLC),
    E0(0x99, TEXT_SEC),
    E0(0x9A, TEXT_CLI),
    E0(0x9B, TEXT_SEI),
    E0(0x9C, TEXT_RSP),
    E0(0x9D, TEXT_NOP),
    E0(0x9F, TEXT_TXA),
    E2(0x10, TEXT_BSET,  M_BNO, M_DIR),
    E2(0x11, TEXT_BCLR,  M_BNO, M_DIR),
    E3(0x00, TEXT_BRSET, M_BNO, M_DIR, M_REL),
    E3(0x01, TEXT_BRCLR, M_BNO, M_DIR, M_REL),
};

constexpr uint8_t MC6805_INDEX[] PROGMEM = {
     64,  // TEXT_ADC
     66,  // TEXT_ADD
     58,  // TEXT_AND
     23,  // TEXT_ASL
     35,  // TEXT_ASLA
     47,  // TEXT_ASLX
     22,  // TEXT_ASR
     34,  // TEXT_ASRA
     46,  // TEXT_ASRX
      5,  // TEXT_BCC
     87,  // TEXT_BCLR
      7,  // TEXT_BCS
      9,  // TEXT_BEQ
     10,  // TEXT_BHCC
     11,  // TEXT_BHCS
      2,  // TEXT_BHI
      4,  // TEXT_BHS
     17,  // TEXT_BIH
     16,  // TEXT_BIL
     59,  // TEXT_BIT
      6,  // TEXT_BLO
      3,  // TEXT_BLS
     14,  // TEXT_BMC
     13,  // TEXT_BMI
     15,  // TEXT_BMS
      8,  // TEXT_BNE
     12,  // TEXT_BPL
      0,  // TEXT_BRA
     89,  // TEXT_BRCLR
      1,  // TEXT_BRN
     88,  // TEXT_BRSET
     86,  // TEXT_BSET
     72,  // TEXT_BSR
     79,  // TEXT_CLC
     81,  // TEXT_CLI
     29,  // TEXT_CLR
     41,  // TEXT_CLRA
     53,  // TEXT_CLRX
     55,  // TEXT_CMP
     19,  // TEXT_COM
     31,  // TEXT_COMA
     43,  // TEXT_COMX
     57,  // TEXT_CPX
     26,  // TEXT_DEC
     38,  // TEXT_DECA
     50,  // TEXT_DECX
     63,  // TEXT_EOR
     27,  // TEXT_INC
     39,  // TEXT_INCA
     51,  // TEXT_INCX
     67,  // TEXT_JMP
     68,  // TEXT_JMP
     73,  // TEXT_JSR
     74,  // TEXT_JSR
     60,  // TEXT_LDA
     69,  // TEXT_LDX
     24,  // TEXT_LSL
     36,  // TEXT_LSLA
     48,  // TEXT_LSLX
     20,  // TEXT_LSR
     32,  // TEXT_LSRA
     44,  // TEXT_LSRX
     18,  // TEXT_NEG
     30,  // TEXT_NEGA
     42,  // TEXT_NEGX
     84,  // TEXT_NOP
     65,  // TEXT_ORA
     25,  // TEXT_ROL
     37,  // TEXT_ROLA
     49,  // TEXT_ROLX
     21,  // TEXT_ROR
     33,  // TEXT_RORA
     45,  // TEXT_RORX
     83,  // TEXT_RSP
     75,  // TEXT_RTI
     76,  // TEXT_RTS
     56,  // TEXT_SBC
     80,  // TEXT_SEC
     82,  // TEXT_SEI
     61,  // TEXT_STA
     62,  // TEXT_STA
     70,  // TEXT_STX
     71,  // TEXT_STX
     54,  // TEXT_SUB
     77,  // TEXT_SWI
     78,  // TEXT_TAX
     28,  // TEXT_TST
     40,  // TEXT_TSTA
     52,  // TEXT_TSTX
     85,  // TEXT_TXA
};

constexpr Entry MC146805_TABLE[] PROGMEM = {
    E0(0x8E, TEXT_STOP),
    E0(0x8F, TEXT_WAIT),
};

constexpr uint8_t MC146805_INDEX[] PROGMEM = {
      0,  // TEXT_STOP
      1,  // TEXT_WAIT
};

constexpr Entry MC68HC05_TABLE[] PROGMEM = {
    E0(0x42, TEXT_MUL),
};

constexpr uint8_t MC68HC05_INDEX[] PROGMEM = {
      0,  // TEXT_MUL
};

constexpr Entry MC68HC08_TABLE[] PROGMEM = {
    E1(0x35, TEXT_STHX,  M_DIR),
    E1(0x45, TEXT_LDHX,  M_IM16),
    E1(0x55, TEXT_LDHX,  M_DIR),
    E1(0x65, TEXT_CPHX,  M_IM16),
    E1(0x75, TEXT_CPHX,  M_DIR),
    E2(0x4E, TEXT_MOV,   M_DIR,  M_DIR),
    E2(0x5E, TEXT_MOV,   M_DIR,  M_IX0P), // for disassembler
    E1(0x5E, TEXT_MOV,   M_IX1P),         // for assembler
    E2(0x6E, TEXT_MOV,   M_IMM,  M_DIR),
    E2(0x7E, TEXT_MOV,   M_IX0P, M_DIR),
    E1(0xA7, TEXT_AIS,   M_SIM8),
    E1(0xAF, TEXT_AIX,   M_SIM8),
    E0(0x52, TEXT_DIV),
    E0(0x62, TEXT_NSA),
    E0(0x72, TEXT_DAA),
    E0(0x84, TEXT_TAP),
    E0(0x85, TEXT_TPA),
    E0(0x86, TEXT_PULA),
    E0(0x87, TEXT_PSHA),
    E0(0x88, TEXT_PULX),
    E0(0x89, TEXT_PSHX),
    E0(0x8A, TEXT_PULH),
    E0(0x8B, TEXT_PSHH),
    E0(0x8C, TEXT_CLRH),
    E0(0x94, TEXT_TXS),
    E0(0x95, TEXT_TSX),
    E1(0x90, TEXT_BGE,   M_REL),
    E1(0x91, TEXT_BLT,   M_REL),
    E1(0x92, TEXT_BGT,   M_REL),
    E1(0x93, TEXT_BLE,   M_REL),
    E2(0x3B, TEXT_DBNZ,  M_DIR,  M_REL),
    E1(0x4B, TEXT_DBNZA, M_REL),
    E1(0x5B, TEXT_DBNZX, M_REL),
    E2(0x6B, TEXT_DBNZ,  M_IX1,  M_REL),
    E2(0x7B, TEXT_DBNZ,  M_IX0,  M_REL),
    E2(0x7B, TEXT_DBNZ,  M_REGX, M_REL),
    E2(0x31, TEXT_CBEQ,  M_DIR,  M_REL),
    E2(0x41, TEXT_CBEQA, M_IMM,  M_REL),
    E2(0x51, TEXT_CBEQX, M_IMM,  M_REL),
    E2(0x61, TEXT_CBEQ,  M_IX1P, M_REL),
    E2(0x71, TEXT_CBEQ,  M_IX0P, M_REL),
};

constexpr uint8_t MC68HC08_INDEX[] PROGMEM = {
     10,  // TEXT_AIS
     11,  // TEXT_AIX
     26,  // TEXT_BGE
     28,  // TEXT_BGT
     29,  // TEXT_BLE
     27,  // TEXT_BLT
     36,  // TEXT_CBEQ
     39,  // TEXT_CBEQ
     40,  // TEXT_CBEQ
     37,  // TEXT_CBEQA
     38,  // TEXT_CBEQX
     23,  // TEXT_CLRH
      3,  // TEXT_CPHX
      4,  // TEXT_CPHX
     14,  // TEXT_DAA
     30,  // TEXT_DBNZ
     33,  // TEXT_DBNZ
     34,  // TEXT_DBNZ
     35,  // TEXT_DBNZ
     31,  // TEXT_DBNZA
     32,  // TEXT_DBNZX
     12,  // TEXT_DIV
      1,  // TEXT_LDHX
      2,  // TEXT_LDHX
      5,  // TEXT_MOV
      6,  // TEXT_MOV
      7,  // TEXT_MOV
      8,  // TEXT_MOV
      9,  // TEXT_MOV
     13,  // TEXT_NSA
     18,  // TEXT_PSHA
     22,  // TEXT_PSHH
     20,  // TEXT_PSHX
     17,  // TEXT_PULA
     21,  // TEXT_PULH
     19,  // TEXT_PULX
      0,  // TEXT_STHX
     15,  // TEXT_TAP
     16,  // TEXT_TPA
     25,  // TEXT_TSX
     24,  // TEXT_TXS
};

constexpr Entry MC68HC08_P9E[] PROGMEM = {
    E1(0x60, TEXT_NEG,  M_SP1),
    E2(0x61, TEXT_CBEQ, M_SP1,  M_REL),
    E1(0x63, TEXT_COM,  M_SP1),
    E1(0x64, TEXT_LSR,  M_SP1),
    E1(0x66, TEXT_ROR,  M_SP1),
    E1(0x67, TEXT_ASR,  M_SP1),
    E1(0x68, TEXT_ASL,  M_SP1),
    E1(0x68, TEXT_LSL,  M_SP1),
    E1(0x69, TEXT_ROL,  M_SP1),
    E1(0x6A, TEXT_DEC,  M_SP1),
    E2(0x6B, TEXT_DBNZ, M_SP1,  M_REL),
    E1(0x6C, TEXT_INC,  M_SP1),
    E1(0x6D, TEXT_TST,  M_SP1),
    E1(0x6F, TEXT_CLR,  M_SP1),
    E1(0xE0, TEXT_SUB,  M_SP1),
    E1(0xE1, TEXT_CMP,  M_SP1),
    E1(0xE2, TEXT_SBC,  M_SP1),
    E1(0xE3, TEXT_CPX,  M_SP1),
    E1(0xE4, TEXT_AND,  M_SP1),
    E1(0xE5, TEXT_BIT,  M_SP1),
    E1(0xE6, TEXT_LDA,  M_SP1),
    E1(0xE7, TEXT_STA,  M_SP1),
    E1(0xE8, TEXT_EOR,  M_SP1),
    E1(0xE9, TEXT_ADC,  M_SP1),
    E1(0xEA, TEXT_ORA,  M_SP1),
    E1(0xEB, TEXT_ADD,  M_SP1),
    E1(0xEE, TEXT_LDX,  M_SP1),
    E1(0xEF, TEXT_STX,  M_SP1),
    E1(0xD0, TEXT_SUB,  M_SP2),
    E1(0xD1, TEXT_CMP,  M_SP2),
    E1(0xD2, TEXT_SBC,  M_SP2),
    E1(0xD3, TEXT_CPX,  M_SP2),
    E1(0xD4, TEXT_AND,  M_SP2),
    E1(0xD5, TEXT_BIT,  M_SP2),
    E1(0xD6, TEXT_LDA,  M_SP2),
    E1(0xD7, TEXT_STA,  M_SP2),
    E1(0xD8, TEXT_EOR,  M_SP2),
    E1(0xD9, TEXT_ADC,  M_SP2),
    E1(0xDA, TEXT_ORA,  M_SP2),
    E1(0xDB, TEXT_ADD,  M_SP2),
    E1(0xDE, TEXT_LDX,  M_SP2),
    E1(0xDF, TEXT_STX,  M_SP2),
};

constexpr uint8_t MC68HC08_I9E[] PROGMEM = {
     23,  // TEXT_ADC
     37,  // TEXT_ADC
     25,  // TEXT_ADD
     39,  // TEXT_ADD
     18,  // TEXT_AND
     32,  // TEXT_AND
      6,  // TEXT_ASL
      5,  // TEXT_ASR
     19,  // TEXT_BIT
     33,  // TEXT_BIT
      1,  // TEXT_CBEQ
     13,  // TEXT_CLR
     15,  // TEXT_CMP
     29,  // TEXT_CMP
      2,  // TEXT_COM
     17,  // TEXT_CPX
     31,  // TEXT_CPX
     10,  // TEXT_DBNZ
      9,  // TEXT_DEC
     22,  // TEXT_EOR
     36,  // TEXT_EOR
     11,  // TEXT_INC
     20,  // TEXT_LDA
     34,  // TEXT_LDA
     26,  // TEXT_LDX
     40,  // TEXT_LDX
      7,  // TEXT_LSL
      3,  // TEXT_LSR
      0,  // TEXT_NEG
     24,  // TEXT_ORA
     38,  // TEXT_ORA
      8,  // TEXT_ROL
      4,  // TEXT_ROR
     16,  // TEXT_SBC
     30,  // TEXT_SBC
     21,  // TEXT_STA
     35,  // TEXT_STA
     27,  // TEXT_STX
     41,  // TEXT_STX
     14,  // TEXT_SUB
     28,  // TEXT_SUB
     12,  // TEXT_TST
};

constexpr Entry MC68HCS08_TABLE[] PROGMEM = {
    E1(0x35, TEXT_STHX, M_DIR),
    E1(0x55, TEXT_LDHX, M_DIR),
    E1(0x75, TEXT_CPHX, M_DIR),
    E1(0x32, TEXT_LDHX, M_EXT),
    E1(0x3E, TEXT_CPHX, M_EXT),
    E1(0x96, TEXT_STHX, M_EXT),
    E0(0x82, TEXT_BGND),
};

constexpr uint8_t MC68HCS08_INDEX[] PROGMEM = {
      6,  // TEXT_BGND
      2,  // TEXT_CPHX
      4,  // TEXT_CPHX
      1,  // TEXT_LDHX
      3,  // TEXT_LDHX
      0,  // TEXT_STHX
      5,  // TEXT_STHX
};

constexpr Entry MC68HCS08_P9E[] PROGMEM = {
    E1(0xAE, TEXT_LDHX, M_IX0),
    E1(0xBE, TEXT_LDHX, M_IX2),
    E1(0xCE, TEXT_LDHX, M_IX1),
    E1(0xFE, TEXT_LDHX, M_SP1),
    E1(0xFF, TEXT_STHX, M_SP1),
    E1(0xF3, TEXT_CPHX, M_SP1),
};

constexpr uint8_t MC68HCS08_I9E[] PROGMEM = {
      5,  // TEXT_CPHX
      0,  // TEXT_LDHX
      1,  // TEXT_LDHX
      2,  // TEXT_LDHX
      3,  // TEXT_LDHX
      4,  // TEXT_STHX
};

// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage MC6805_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC6805_TABLE), ARRAY_RANGE(MC6805_INDEX)},
};

constexpr EntryPage MC146805_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC6805_TABLE), ARRAY_RANGE(MC6805_INDEX)},
        {0x00, ARRAY_RANGE(MC146805_TABLE), ARRAY_RANGE(MC146805_INDEX)},
};

constexpr EntryPage MC68HC05_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC6805_TABLE), ARRAY_RANGE(MC6805_INDEX)},
        {0x00, ARRAY_RANGE(MC146805_TABLE), ARRAY_RANGE(MC146805_INDEX)},
        {0x00, ARRAY_RANGE(MC68HC05_TABLE), ARRAY_RANGE(MC68HC05_INDEX)},
};

constexpr EntryPage MC68HC08_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC68HC08_TABLE), ARRAY_RANGE(MC68HC08_INDEX)},
        // Above definitions overrides unknown instructions defined below.
        {0x00, ARRAY_RANGE(MC6805_TABLE), ARRAY_RANGE(MC6805_INDEX)},
        {0x00, ARRAY_RANGE(MC146805_TABLE), ARRAY_RANGE(MC146805_INDEX)},
        {0x00, ARRAY_RANGE(MC68HC05_TABLE), ARRAY_RANGE(MC68HC05_INDEX)},
        {0x9E, ARRAY_RANGE(MC68HC08_P9E), ARRAY_RANGE(MC68HC08_I9E)},
};

constexpr EntryPage MC68HCS08_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC68HCS08_TABLE), ARRAY_RANGE(MC68HCS08_INDEX)},
        {0x00, ARRAY_RANGE(MC68HC08_TABLE), ARRAY_RANGE(MC68HC08_INDEX)},
        // Above definitions overrides unknown instructions defined below.
        {0x00, ARRAY_RANGE(MC6805_TABLE), ARRAY_RANGE(MC6805_INDEX)},
        {0x00, ARRAY_RANGE(MC146805_TABLE), ARRAY_RANGE(MC146805_INDEX)},
        {0x00, ARRAY_RANGE(MC68HC05_TABLE), ARRAY_RANGE(MC68HC05_INDEX)},
        {0x9E, ARRAY_RANGE(MC68HCS08_P9E), ARRAY_RANGE(MC68HCS08_I9E)},
        {0x9E, ARRAY_RANGE(MC68HC08_P9E), ARRAY_RANGE(MC68HC08_I9E)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {MC6805, TEXT_CPU_6805, ARRAY_RANGE(MC6805_PAGES)},
        {MC146805, TEXT_CPU_146805, ARRAY_RANGE(MC146805_PAGES)},
        {MC68HC05, TEXT_CPU_68HC05, ARRAY_RANGE(MC68HC05_PAGES)},
        {MC68HC08, TEXT_CPU_68HC08, ARRAY_RANGE(MC68HC08_PAGES)},
        {MC68HCS08, TEXT_CPU_68HCS08, ARRAY_RANGE(MC68HCS08_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool hasOperand(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, Cpu::acceptAll<AsmInsn, Entry>);
    return insn.isOK() && insn.mode1() != M_NONE;
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_EXT)
        return table == M_REL || table == M_GEN;
    if (opr == M_DIR)
        return table == M_REL || table == M_EXT || table == M_GEN || table == M_MEM;
    if (opr == M_IMM)
        return table == M_IM16 || table == M_SIM8 || table == M_GEN;
    if (opr == M_BNO)
        return table == M_REL || table == M_DIR || table == M_EXT || table == M_GEN ||
               table == M_MEM;
    if (opr == M_IX0 || opr == M_IX1)
        return table == M_GEN || table == M_MEM;
    if (opr == M_IX2)
        return table == M_GEN;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    if (acceptMode(insn.op1.mode, table.mode1()) && acceptMode(insn.op2.mode, table.mode2()) &&
            acceptMode(insn.op3.mode, table.mode3())) {
        if (table.undefined())
            insn.setErrorIf(OPERAND_NOT_ALLOWED);
        return true;
    }
    return false;
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opCode = insn.opCode();
    const auto flags = entry->readFlags();
    const auto mode1 = flags.mode1();
    if (mode1 == M_MEM) {
        const auto opc = opCode & 0xF0;
        if (opc == 0x30 || opc == 0x60 || opc == 0x70)
            opCode = (opCode & ~0xF0) | 0x30;
    } else if (mode1 == M_GEN && (opCode & 0xF0) >= 0xA0) {
        opCode = (opCode & ~0xF0) | 0xA0;
    } else if (mode1 == M_BNO) {
        opCode &= ~0x0E;
    }
    return opCode == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    auto entry = cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    if (entry && entry->readFlags().undefined()) {
        insn.nameBuffer().reset();
        insn.setErrorIf(UNKNOWN_INSTRUCTION);
    }
    return insn.getError();
}

bool isPrefix(CpuType cpuType, Config::opcode_t code) {
    return cpu(cpuType)->isPrefix(code);
}

const /*PROGMEM*/ char *TableMc6805::listCpu_P() const {
    return TEXT_MC6805_LIST;
}

const /*PROGMEM*/ char *TableMc6805::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableMc6805::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    name.iexpectText_P(TEXT_MC6805_LIST, 2);
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableMc6805 TABLE;

}  // namespace mc6805
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
