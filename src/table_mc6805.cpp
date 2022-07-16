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

#include <string.h>

#include "config_mc6805.h"
#include "entry_mc6805.h"
#include "text_mc6800.h"
#include "text_mc6805.h"

using namespace libasm::mc6800;

namespace libasm {
namespace mc6805 {

#define E3(_opc, _name, _op1, _op2, _op3) \
    { _opc, Entry::Flags::create(_op1, _op2, _op3), _name }
#define E2(_opc, _name, _op1, _op2) E3(_opc, _name, _op1, _op2, M_NO)
#define E1(_opc, _name, _op1) E3(_opc, _name, _op1, M_NO, M_NO)
#define E0(_opc, _name) E3(_opc, _name, M_NO, M_NO, M_NO)
#define U1(_opc, _name, _op1) \
    { _opc, Entry::Flags::create(_op1, M_NO, M_NO, true), _name }

// clang-format off
static constexpr Entry MC6805_TABLE[] PROGMEM = {
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

static constexpr uint8_t MC6805_INDEX[] PROGMEM = {
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

static constexpr Entry MC146805_TABLE[] PROGMEM = {
    E0(0x8E, TEXT_STOP),
    E0(0x8F, TEXT_WAIT),
};

static constexpr uint8_t MC146805_INDEX[] PROGMEM = {
      0,  // TEXT_STOP
      1,  // TEXT_WAIT
};

static constexpr Entry MC68HC05_TABLE[] PROGMEM = {
    E0(0x42, TEXT_MUL),
};

static constexpr uint8_t MC68HC05_INDEX[] PROGMEM = {
      0,  // TEXT_MUL
};
// clang-format on

static constexpr TableMc6805::EntryPage MC6805_PAGES[] PROGMEM = {
        {ARRAY_RANGE(MC6805_TABLE), ARRAY_RANGE(MC6805_INDEX)},
};

static constexpr TableMc6805::EntryPage MC146805_PAGES[] PROGMEM = {
        {ARRAY_RANGE(MC6805_TABLE), ARRAY_RANGE(MC6805_INDEX)},
        {ARRAY_RANGE(MC146805_TABLE), ARRAY_RANGE(MC146805_INDEX)},
};

static constexpr TableMc6805::EntryPage MC68HC05_PAGES[] PROGMEM = {
        {ARRAY_RANGE(MC6805_TABLE), ARRAY_RANGE(MC6805_INDEX)},
        {ARRAY_RANGE(MC146805_TABLE), ARRAY_RANGE(MC146805_INDEX)},
        {ARRAY_RANGE(MC68HC05_TABLE), ARRAY_RANGE(MC68HC05_INDEX)},
};

static constexpr TableMc6805::Cpu CPU_TABLES[] PROGMEM = {
        {MC6805, TEXT_CPU_6805, ARRAY_RANGE(MC6805_PAGES)},
        {MC146805, TEXT_CPU_146805, ARRAY_RANGE(MC146805_PAGES)},
        {MC68HC05, TEXT_CPU_68HC05, ARRAY_RANGE(MC68HC05_PAGES)},
};

static bool acceptAddrMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (table == M_GEN)
        return opr == M_BNO || opr == M_IMM || opr == M_DIR || opr == M_EXT || opr == M_IDX ||
               opr == M_IX0 || opr == M_IX2;
    if (table == M_MEM)
        return opr == M_BNO || opr == M_DIR || opr == M_IDX || opr == M_IX0;
    if (opr == M_EXT)
        return table == M_REL;
    if (opr == M_DIR)
        return table == M_REL || table == M_EXT;
    if (opr == M_BNO)
        return table == M_REL || table == M_DIR || table == M_EXT;
    return false;
}

static bool acceptAddrMode(Entry::Flags flags, const Entry *entry) {
    auto table = entry->flags();
    return acceptAddrMode(flags.mode1(), table.mode1()) &&
           acceptAddrMode(flags.mode2(), table.mode2()) &&
           acceptAddrMode(flags.mode3(), table.mode3());
}

Error TableMc6805::searchName(InsnMc6805 &insn) {
    uint8_t count = 0;
    for (auto page = _cpu->table(); page < _cpu->end(); page++) {
        auto entry = searchEntry(insn.name(), insn.flags(), page, acceptAddrMode, count);
        if (entry) {
            insn.setOpCode(entry->opCode());
            insn.setFlags(entry->flags());
            if (insn.undefined())
                return setError(OPERAND_NOT_ALLOWED);
            return setOK();
        }
    }
    return setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t maskCode(Config::opcode_t code, const Entry *entry) {
    const auto &flags = entry->flags();
    const auto mode1 = flags.mode1();
    if (mode1 == M_GEN && (code & 0xF0) >= 0xA0)
        return (code & ~0xF0) | 0xA0;
    if (mode1 == M_MEM) {
        const auto opc = code & 0xF0;
        if (opc == 0x30 || opc == 0x60 || opc == 0x70)
            return (code & ~0xF0) | 0x30;
    }
    if (mode1 == M_BNO)
        return code & ~0x0E;
    return code;
}

Error TableMc6805::searchOpCode(InsnMc6805 &insn) {
    for (auto page = _cpu->table(); page < _cpu->end(); page++) {
        auto entry = searchEntry(insn.opCode(), page->table(), page->end(), maskCode);
        if (entry) {
            insn.setFlags(entry->flags());
            insn.nameBuffer().text_P(entry->name_P());
            if (insn.undefined())
                break;
            return setOK();
        }
    }
    return setError(UNKNOWN_INSTRUCTION);
}

TableMc6805::TableMc6805() {
    setCpu(MC6805);
}

bool TableMc6805::setCpu(CpuType cpuType) {
    auto t = Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLES));
    if (t == nullptr)
        return false;
    _cpu = t;
    return true;
}

const /* PROGMEM */ char *TableMc6805::listCpu_P() const {
    return TEXT_CPU_LIST_6805;
}

const /* PROGMEM */ char *TableMc6805::cpu_P() const {
    return _cpu->name_P();
}

bool TableMc6805::setCpu(const char *cpu) {
    auto p = cpu;
    if (strncasecmp_P(p, TEXT_CPU_6805_MC, 2) == 0)
        p += 2;
    auto t = Cpu::search(p, ARRAY_RANGE(CPU_TABLES));
    return t ? setCpu(t->cpuType()) : false;
}

TableMc6805 TableMc6805::TABLE;

}  // namespace mc6805
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
