/*
 * Copyright 2020 Tadashi G. Takaoka
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

#include "config_mc6800.h"

#include "entry_mc6800.h"
#include "table_mc6800.h"
#include "text_mc6800.h"

#include <string.h>

namespace libasm {
namespace mc6800 {

#define E3(_opc, _name, _size, _op1, _op2, _op3)                        \
    { _opc, Entry::Flags::create(_op1, _op2, _op3, SZ_##_size), TEXT_##_name },
#define E2(_opc, _name, _size, _op1, _op2)      \
    E3(_opc, _name, _size, _op1, _op2, M_NO)
#define E1(_opc, _name, _size, _op1)            \
    E3(_opc, _name, _size, _op1, M_NO, M_NO)
#define E0(_opc, _name)                         \
    E3(_opc, _name, NONE, M_NO, M_NO, M_NO)

static constexpr Entry MC6800_TABLE[] PROGMEM = {
    E0(0x01, NOP)
    E0(0x06, TAP)
    E0(0x07, TPA)
    E0(0x08, INX)
    E0(0x09, DEX)
    E0(0x0A, CLV)
    E0(0x0B, SEV)
    E0(0x0C, CLC)
    E0(0x0D, SEC)
    E0(0x0E, CLI)
    E0(0x0F, SEI)
    E0(0x10, SBA)
    E0(0x11, CBA)
    E0(0x16, TAB)
    E0(0x17, TBA)
    E0(0x19, DAA)
    E0(0x1B, ABA)
    E1(0x20, BRA,  BYTE, M_REL)
    E1(0x22, BHI,  BYTE, M_REL)
    E1(0x23, BLS,  BYTE, M_REL)
    E1(0x24, BHS,  BYTE, M_REL)
    E1(0x24, BCC,  BYTE, M_REL)
    E1(0x25, BLO,  BYTE, M_REL)
    E1(0x25, BCS,  BYTE, M_REL)
    E1(0x26, BNE,  BYTE, M_REL)
    E1(0x27, BEQ,  BYTE, M_REL)
    E1(0x28, BVC,  BYTE, M_REL)
    E1(0x29, BVS,  BYTE, M_REL)
    E1(0x2A, BPL,  BYTE, M_REL)
    E1(0x2B, BMI,  BYTE, M_REL)
    E1(0x2C, BGE,  BYTE, M_REL)
    E1(0x2D, BLT,  BYTE, M_REL)
    E1(0x2E, BGT,  BYTE, M_REL)
    E1(0x2F, BLE,  BYTE, M_REL)
    E0(0x30, TSX)
    E0(0x31, INS)
    E0(0x32, PULA)
    E0(0x33, PULB)
    E0(0x34, DES)
    E0(0x35, TXS)
    E0(0x36, PSHA)
    E0(0x37, PSHB)
    E0(0x39, RTS)
    E0(0x3B, RTI)
    E0(0x3E, WAI)
    E0(0x3F, SWI)
    E0(0x40, NEGA)
    E0(0x50, NEGB)
    E0(0x43, COMA)
    E0(0x53, COMB)
    E0(0x44, LSRA)
    E0(0x54, LSRB)
    E0(0x46, RORA)
    E0(0x56, RORB)
    E0(0x47, ASRA)
    E0(0x57, ASRB)
    E0(0x48, ASLA)
    E0(0x58, ASLB)
    E0(0x48, LSLA)
    E0(0x58, LSLB)
    E0(0x49, ROLA)
    E0(0x59, ROLB)
    E0(0x4A, DECA)
    E0(0x5A, DECB)
    E0(0x4C, INCA)
    E0(0x5C, INCB)
    E0(0x4D, TSTA)
    E0(0x5D, TSTB)
    E0(0x4F, CLRA)
    E0(0x5F, CLRB)
    E1(0x60, NEG,  BYTE, M_IDX)
    E1(0x63, COM,  BYTE, M_IDX)
    E1(0x64, LSR,  BYTE, M_IDX)
    E1(0x66, ROR,  BYTE, M_IDX)
    E1(0x67, ASR,  BYTE, M_IDX)
    E1(0x68, ASL,  BYTE, M_IDX)
    E1(0x68, LSL,  BYTE, M_IDX)
    E1(0x69, ROL,  BYTE, M_IDX)
    E1(0x6A, DEC,  BYTE, M_IDX)
    E1(0x6C, INC,  BYTE, M_IDX)
    E1(0x6D, TST,  BYTE, M_IDX)
    E1(0x6E, JMP,  BYTE, M_IDX)
    E1(0x6F, CLR,  BYTE, M_IDX)
    E1(0x70, NEG,  WORD, M_EXT)
    E1(0x73, COM,  WORD, M_EXT)
    E1(0x74, LSR,  WORD, M_EXT)
    E1(0x76, ROR,  WORD, M_EXT)
    E1(0x77, ASR,  WORD, M_EXT)
    E1(0x78, ASL,  WORD, M_EXT)
    E1(0x78, LSL,  WORD, M_EXT)
    E1(0x79, ROL,  WORD, M_EXT)
    E1(0x7A, DEC,  WORD, M_EXT)
    E1(0x7C, INC,  WORD, M_EXT)
    E1(0x7D, TST,  WORD, M_EXT)
    E1(0x7E, JMP,  WORD, M_EXT)
    E1(0x7F, CLR,  WORD, M_EXT)
    E1(0x80, SUBA, BYTE, M_IMM)
    E1(0xC0, SUBB, BYTE, M_IMM)
    E1(0x81, CMPA, BYTE, M_IMM)
    E1(0xC1, CMPB, BYTE, M_IMM)
    E1(0x82, SBCA, BYTE, M_IMM)
    E1(0xC2, SBCB, BYTE, M_IMM)
    E1(0x84, ANDA, BYTE, M_IMM)
    E1(0xC4, ANDB, BYTE, M_IMM)
    E1(0x85, BITA, BYTE, M_IMM)
    E1(0xC5, BITB, BYTE, M_IMM)
    E1(0x86, LDAA, BYTE, M_IMM)
    E1(0xC6, LDAB, BYTE, M_IMM)
    E1(0x88, EORA, BYTE, M_IMM)
    E1(0xC8, EORB, BYTE, M_IMM)
    E1(0x89, ADCA, BYTE, M_IMM)
    E1(0xC9, ADCB, BYTE, M_IMM)
    E1(0x8A, ORAA, BYTE, M_IMM)
    E1(0xCA, ORAB, BYTE, M_IMM)
    E1(0x8B, ADDA, BYTE, M_IMM)
    E1(0xCB, ADDB, BYTE, M_IMM)
    E1(0x8C, CPX,  WORD, M_IMM)
    E1(0x8D, BSR,  BYTE, M_REL)
    E1(0x8E, LDS,  WORD, M_IMM)
    E1(0x90, SUBA, BYTE, M_DIR)
    E1(0xD0, SUBB, BYTE, M_DIR)
    E1(0x91, CMPA, BYTE, M_DIR)
    E1(0xD1, CMPB, BYTE, M_DIR)
    E1(0x92, SBCA, BYTE, M_DIR)
    E1(0xD2, SBCB, BYTE, M_DIR)
    E1(0x94, ANDA, BYTE, M_DIR)
    E1(0xD4, ANDB, BYTE, M_DIR)
    E1(0x95, BITA, BYTE, M_DIR)
    E1(0xD5, BITB, BYTE, M_DIR)
    E1(0x96, LDAA, BYTE, M_DIR)
    E1(0xD6, LDAB, BYTE, M_DIR)
    E1(0x97, STAA, BYTE, M_DIR)
    E1(0xD7, STAB, BYTE, M_DIR)
    E1(0x98, EORA, BYTE, M_DIR)
    E1(0xD8, EORB, BYTE, M_DIR)
    E1(0x99, ADCA, BYTE, M_DIR)
    E1(0xD9, ADCB, BYTE, M_DIR)
    E1(0x9A, ORAA, BYTE, M_DIR)
    E1(0xDA, ORAB, BYTE, M_DIR)
    E1(0x9B, ADDA, BYTE, M_DIR)
    E1(0xDB, ADDB, BYTE, M_DIR)
    E1(0x9C, CPX,  BYTE, M_DIR)
    E1(0x9E, LDS,  BYTE, M_DIR)
    E1(0x9F, STS,  BYTE, M_DIR)
    E1(0xA0, SUBA, BYTE, M_IDX)
    E1(0xE0, SUBB, BYTE, M_IDX)
    E1(0xA1, CMPA, BYTE, M_IDX)
    E1(0xE1, CMPB, BYTE, M_IDX)
    E1(0xA2, SBCA, BYTE, M_IDX)
    E1(0xE2, SBCB, BYTE, M_IDX)
    E1(0xA4, ANDA, BYTE, M_IDX)
    E1(0xE4, ANDB, BYTE, M_IDX)
    E1(0xA5, BITA, BYTE, M_IDX)
    E1(0xE5, BITB, BYTE, M_IDX)
    E1(0xA6, LDAA, BYTE, M_IDX)
    E1(0xE6, LDAB, BYTE, M_IDX)
    E1(0xA7, STAA, BYTE, M_IDX)
    E1(0xE7, STAB, BYTE, M_IDX)
    E1(0xA8, EORA, BYTE, M_IDX)
    E1(0xE8, EORB, BYTE, M_IDX)
    E1(0xA9, ADCA, BYTE, M_IDX)
    E1(0xE9, ADCB, BYTE, M_IDX)
    E1(0xAA, ORAA, BYTE, M_IDX)
    E1(0xEA, ORAB, BYTE, M_IDX)
    E1(0xAB, ADDA, BYTE, M_IDX)
    E1(0xEB, ADDB, BYTE, M_IDX)
    E1(0xAC, CPX,  BYTE, M_IDX)
    E1(0xAD, JSR,  BYTE, M_IDX)
    E1(0xAE, LDS,  BYTE, M_IDX)
    E1(0xAF, STS,  BYTE, M_IDX)
    E1(0xB0, SUBA, WORD, M_EXT)
    E1(0xF0, SUBB, WORD, M_EXT)
    E1(0xB1, CMPA, WORD, M_EXT)
    E1(0xF1, CMPB, WORD, M_EXT)
    E1(0xB2, SBCA, WORD, M_EXT)
    E1(0xF2, SBCB, WORD, M_EXT)
    E1(0xB4, ANDA, WORD, M_EXT)
    E1(0xF4, ANDB, WORD, M_EXT)
    E1(0xB5, BITA, WORD, M_EXT)
    E1(0xF5, BITB, WORD, M_EXT)
    E1(0xB6, LDAA, WORD, M_EXT)
    E1(0xF6, LDAB, WORD, M_EXT)
    E1(0xB7, STAA, WORD, M_EXT)
    E1(0xF7, STAB, WORD, M_EXT)
    E1(0xB8, EORA, WORD, M_EXT)
    E1(0xF8, EORB, WORD, M_EXT)
    E1(0xB9, ADCA, WORD, M_EXT)
    E1(0xF9, ADCB, WORD, M_EXT)
    E1(0xBA, ORAA, WORD, M_EXT)
    E1(0xFA, ORAB, WORD, M_EXT)
    E1(0xBB, ADDA, WORD, M_EXT)
    E1(0xFB, ADDB, WORD, M_EXT)
    E1(0xBC, CPX,  WORD, M_EXT)
    E1(0xBD, JSR,  WORD, M_EXT)
    E1(0xBE, LDS,  WORD, M_EXT)
    E1(0xBF, STS,  WORD, M_EXT)
    E1(0xCE, LDX,  WORD, M_IMM)
    E1(0xDE, LDX,  BYTE, M_DIR)
    E1(0xDF, STX,  BYTE, M_DIR)
    E1(0xEE, LDX,  BYTE, M_IDX)
    E1(0xEF, STX,  BYTE, M_IDX)
    E1(0xFE, LDX,  WORD, M_EXT)
    E1(0xFF, STX,  WORD, M_EXT)
};

static constexpr Entry MC6801_TABLE[] PROGMEM = {
    E0(0x04, LSRD)
    E0(0x05, ASLD)
    E0(0x05, LSLD)
    E1(0x21, BRN,  BYTE, M_REL)
    E0(0x38, PULX)
    E0(0x3A, ABX)
    E0(0x3C, PSHX)
    E0(0x3D, MUL)
    E1(0x83, SUBD, WORD, M_IMM)
    E1(0x93, SUBD, BYTE, M_DIR)
    E1(0x9D, JSR,  NONE, M_DIR)
    E1(0xA3, SUBD, BYTE, M_IDX)
    E1(0xB3, SUBD, WORD, M_EXT)
    E1(0xC3, ADDD, WORD, M_IMM)
    E1(0xCC, LDD,  WORD, M_IMM)
    E1(0xD3, ADDD, BYTE, M_DIR)
    E1(0xDC, LDD,  BYTE, M_DIR)
    E1(0xDD, STD,  BYTE, M_DIR)
    E1(0xE3, ADDD, BYTE, M_IDX)
    E1(0xEC, LDD,  BYTE, M_IDX)
    E1(0xED, STD,  BYTE, M_IDX)
    E1(0xF3, ADDD, WORD, M_EXT)
    E1(0xFC, LDD,  WORD, M_EXT)
    E1(0xFD, STD,  WORD, M_EXT)
};

static constexpr Entry HD6301_TABLE[] PROGMEM = {
    E0(0x18, XGDX)
    E0(0x1A, SLP)
    E2(0x61, AIM,  BYTE, M_BMM, M_IDX)
    E2(0x61, BCLR, BYTE, M_BIT, M_IDX)
    E2(0x62, OIM,  BYTE, M_BMM, M_IDX)
    E2(0x62, BSET, BYTE, M_BIT, M_IDX)
    E2(0x65, EIM,  BYTE, M_BMM, M_IDX)
    E2(0x65, BTGL, BYTE, M_BIT, M_IDX)
    E2(0x6B, TIM,  BYTE, M_BMM, M_IDX)
    E2(0x6B, BTST, BYTE, M_BIT, M_IDX)
    E2(0x71, AIM,  BYTE, M_BMM, M_DIR)
    E2(0x71, BCLR, BYTE, M_BIT, M_DIR)
    E2(0x72, OIM,  BYTE, M_BMM, M_DIR)
    E2(0x72, BSET, BYTE, M_BIT, M_DIR)
    E2(0x75, EIM,  BYTE, M_BMM, M_DIR)
    E2(0x75, BTGL, BYTE, M_BIT, M_DIR)
    E2(0x7B, TIM,  BYTE, M_BMM, M_DIR)
    E2(0x7B, BTST, BYTE, M_BIT, M_DIR)
};

static constexpr Entry MC68HC11_P00[] PROGMEM = {
    E0(0x02, IDIV)
    E0(0x03, FDIV)
    E3(0x12, BRSET, BYTE, M_DIR, M_IMM, M_REL)
    E3(0x13, BRCLR, BYTE, M_DIR, M_IMM, M_REL)
    E2(0x14, BSET,  BYTE, M_DIR, M_IMM)
    E2(0x15, BCLR,  BYTE, M_DIR, M_IMM)
    E2(0x1C, BSET,  BYTE, M_IDX, M_IMM)
    E2(0x1D, BCLR,  BYTE, M_IDX, M_IMM)
    E3(0x1E, BRSET, BYTE, M_IDX, M_IMM, M_REL)
    E3(0x1F, BRCLR, BYTE, M_IDX, M_IMM, M_REL)
    E0(0x8F, XGDX)
    E0(0xCF, STOP)
};

static constexpr Entry MC68HC11_P18[] PROGMEM = {
    E0(0x08, INY)
    E0(0x09, DEY)
    E2(0x1C, BSET,  BYTE, M_IDY, M_IMM)
    E2(0x1D, BCLR,  BYTE, M_IDY, M_IMM)
    E3(0x1E, BRSET, BYTE, M_IDY, M_IMM, M_REL)
    E3(0x1F, BRCLR, BYTE, M_IDY, M_IMM, M_REL)
    E0(0x30, TSY)
    E0(0x35, TYS)
    E0(0x38, PULY)
    E0(0x3A, ABY)
    E0(0x3C, PSHY)
    E1(0x60, NEG,   BYTE, M_IDY)
    E1(0x63, COM,   BYTE, M_IDY)
    E1(0x64, LSR,   BYTE, M_IDY)
    E1(0x66, ROR,   BYTE, M_IDY)
    E1(0x67, ASR,   BYTE, M_IDY)
    E1(0x68, ASL,   BYTE, M_IDY)
    E1(0x68, LSL,   BYTE, M_IDY)
    E1(0x69, ROL,   BYTE, M_IDY)
    E1(0x6A, DEC,   BYTE, M_IDY)
    E1(0x6C, INC,   BYTE, M_IDY)
    E1(0x6D, TST,   BYTE, M_IDY)
    E1(0x6E, JMP,   BYTE, M_IDY)
    E1(0x6F, CLR,   BYTE, M_IDY)
    E1(0x8C, CPY,   WORD, M_IMM)
    E0(0x8F, XGDY)
    E1(0x9C, CPY,   BYTE, M_DIR)
    E1(0xA0, SUBA,  BYTE, M_IDY)
    E1(0xE0, SUBB,  BYTE, M_IDY)
    E1(0xA1, CMPA,  BYTE, M_IDY)
    E1(0xE1, CMPB,  BYTE, M_IDY)
    E1(0xA2, SBCA,  BYTE, M_IDY)
    E1(0xE2, SBCB,  BYTE, M_IDY)
    E1(0xA3, SUBD,  BYTE, M_IDY)
    E1(0xE3, ADDD,  BYTE, M_IDY)
    E1(0xA4, ANDA,  BYTE, M_IDY)
    E1(0xE4, ANDB,  BYTE, M_IDY)
    E1(0xA5, BITA,  BYTE, M_IDY)
    E1(0xE5, BITB,  BYTE, M_IDY)
    E1(0xA6, LDAA,  BYTE, M_IDY)
    E1(0xE6, LDAB,  BYTE, M_IDY)
    E1(0xA7, STAA,  BYTE, M_IDY)
    E1(0xE7, STAB,  BYTE, M_IDY)
    E1(0xA8, EORA,  BYTE, M_IDY)
    E1(0xE8, EORB,  BYTE, M_IDY)
    E1(0xA9, ADCA,  BYTE, M_IDY)
    E1(0xE9, ADCB,  BYTE, M_IDY)
    E1(0xAA, ORAA,  BYTE, M_IDY)
    E1(0xEA, ORAB,  BYTE, M_IDY)
    E1(0xAB, ADDA,  BYTE, M_IDY)
    E1(0xEB, ADDB,  BYTE, M_IDY)
    E1(0xAC, CPY,   BYTE, M_IDY)
    E1(0xAD, JSR,   BYTE, M_IDY)
    E1(0xAE, LDS,   BYTE, M_IDY)
    E1(0xAF, STS,   BYTE, M_IDY)
    E1(0xBC, CPY,   WORD, M_EXT)
    E1(0xCE, LDY,   WORD, M_IMM)
    E1(0xDE, LDY,   BYTE, M_DIR)
    E1(0xDF, STY,   BYTE, M_DIR)
    E1(0xEC, LDD,   BYTE, M_IDY)
    E1(0xED, STD,   BYTE, M_IDY)
    E1(0xEE, LDY,   BYTE, M_IDY)
    E1(0xEF, STY,   BYTE, M_IDY)
    E1(0xFE, LDY,   WORD, M_EXT)
    E1(0xFF, STY,   WORD, M_EXT)
};

static constexpr Entry MC68HC11_P1A[] PROGMEM = {
    E1(0x83, CPD,   WORD, M_IMM)
    E1(0x93, CPD,   BYTE, M_DIR)
    E1(0xA3, CPD,   BYTE, M_IDX)
    E1(0xAC, CPY,   BYTE, M_IDX)
    E1(0xB3, CPD,   BYTE, M_EXT)
    E1(0xEE, LDY,   BYTE, M_IDX)
    E1(0xEF, STY,   BYTE, M_IDX)
};

static constexpr Entry MC68HC11_PCD[] PROGMEM = {
    E1(0xA3, CPD,   BYTE, M_IDY)
    E1(0xAC, CPX,   BYTE, M_IDY)
    E1(0xEE, LDX,   BYTE, M_IDY)
    E1(0xEF, STX,   BYTE, M_IDY)
};

static constexpr Config::opcode_t PREFIX_P00 = 0x00;
static constexpr Config::opcode_t PREFIX_P18 = 0x18;
static constexpr Config::opcode_t PREFIX_P1A = 0x1A;
static constexpr Config::opcode_t PREFIX_PCD = 0xCD;

bool TableMc6800::isPrefix(Config::opcode_t opCode) const {
    if (_cpuType != MC68HC11) return false;
    return opCode == PREFIX_P18 || opCode == PREFIX_P1A || opCode == PREFIX_PCD;
}

class TableMc6800::EntryPage : public EntryPageBase<Entry> {
public:
    constexpr EntryPage(
        Config::opcode_t prefix, const Entry *table, const Entry *end)
        : EntryPageBase(table, end), _prefix(prefix) {}

    Config::opcode_t prefix() const { return pgm_read_byte(&_prefix); }

private:
    Config::opcode_t _prefix;
};

static constexpr TableMc6800::EntryPage MC6800_PAGES[] PROGMEM = {
    { PREFIX_P00, ARRAY_RANGE(MC6800_TABLE) },
};

static constexpr TableMc6800::EntryPage MC6801_PAGES[] PROGMEM = {
    { PREFIX_P00, ARRAY_RANGE(MC6801_TABLE) },
    { PREFIX_P00, ARRAY_RANGE(MC6800_TABLE) },
};

static constexpr TableMc6800::EntryPage HD6301_PAGES[] PROGMEM = {
    { PREFIX_P00, ARRAY_RANGE(HD6301_TABLE) },
    { PREFIX_P00, ARRAY_RANGE(MC6801_TABLE) },
    { PREFIX_P00, ARRAY_RANGE(MC6800_TABLE) },
};

static constexpr TableMc6800::EntryPage MC68HC11_PAGES[] PROGMEM = {
    { PREFIX_P00, ARRAY_RANGE(MC6801_TABLE) },
    { PREFIX_P00, ARRAY_RANGE(MC6800_TABLE) },
    { PREFIX_P00, ARRAY_RANGE(MC68HC11_P00) },
    { PREFIX_P18, ARRAY_RANGE(MC68HC11_P18) },
    { PREFIX_P1A, ARRAY_RANGE(MC68HC11_P1A) },
    { PREFIX_PCD, ARRAY_RANGE(MC68HC11_PCD) },
};

static bool acceptAddrMode(AddrMode opr, AddrMode table) {
    if (opr == table) return true;
    if (opr == M_EXT) return table == M_REL;
    if (opr == M_DIR) return table == M_REL || table == M_EXT;
    if (opr == M_BIT) return table == M_REL || table == M_EXT || table == M_DIR;
    if (opr == M_IMM) return table == M_BMM;
    return false;
}

static bool acceptAddrMode(Entry::Flags flags, const Entry *entry) {
    const Entry::Flags table = entry->flags();
    return acceptAddrMode(flags.mode1(), table.mode1())
        && acceptAddrMode(flags.mode2(), table.mode2())
        && acceptAddrMode(flags.mode3(), table.mode3());
}

Error TableMc6800::searchName(
    InsnMc6800 &insn, const EntryPage *pages, const EntryPage *end) const{
    uint8_t count = 0;
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *entry = TableBase::searchName<Entry, Entry::Flags>(
            insn.name(), insn.flags(), page->table(), page->end(), acceptAddrMode, count);
        if (entry) {
            insn.setOpCode(entry->opCode(), page->prefix());
            insn.setFlags(entry->flags());
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

const Entry *TableMc6800::searchOpCode(
    InsnMc6800 &insn, const EntryPage *pages, const EntryPage *end) const {
    for (const EntryPage *page = pages; page < end; page++) {
        const Config::opcode_t prefix = page->prefix();
        if (insn.prefix() != prefix) continue;
        const Entry *entry = TableBase::searchCode<Entry, Config::opcode_t>(
            insn.opCode(), page->table(), page->end());
        if (entry) {
            insn.setFlags(entry->flags());
            insn.setName_P(entry->name());
            return entry;
        }
    }
    return nullptr;
}

Error TableMc6800::searchName(InsnMc6800 &insn) const {
    return _error.setError(searchName(insn, _table, _end));
}

Error TableMc6800::searchOpCode(InsnMc6800 &insn) const {
    const Entry *entry = searchOpCode(insn, _table, _end);
    return _error.setError(entry ? OK : UNKNOWN_INSTRUCTION);
}

Error TableMc6800::searchOpCodeAlias(InsnMc6800 &insn) const {
    const Entry *entry = searchOpCode(insn, _table, _end);
    if (!entry) return _error.setError(INTERNAL_ERROR);
    entry += 1;
    if (entry->opCode() != insn.opCode())
        return _error.setError(INTERNAL_ERROR);
    insn.setFlags(entry->flags());
    insn.setName_P(entry->name());
    return _error.setOK();
}

TableMc6800::TableMc6800() {
    setCpu(MC6800);
}

bool TableMc6800::setCpu(CpuType cpuType) {
    _cpuType = cpuType;
    if (cpuType == MC6800) {
        _table = ARRAY_BEGIN(MC6800_PAGES);
        _end = ARRAY_END(MC6800_PAGES);
    } else if (cpuType == MC6801) {
        _table = ARRAY_BEGIN(MC6801_PAGES);
        _end = ARRAY_END(MC6801_PAGES);
    } else if (cpuType == MC68HC11) {
        _table = ARRAY_BEGIN(MC68HC11_PAGES);
        _end = ARRAY_END(MC68HC11_PAGES);
    } else {
        _table = ARRAY_BEGIN(HD6301_PAGES);
        _end = ARRAY_END(HD6301_PAGES);
    }
    return true;
}

const char *TableMc6800::listCpu() const {
    return TEXT_CPU_LIST;
}

const char *TableMc6800::getCpu() const {
    if (_cpuType == MC6800)
        return TEXT_CPU_6800;
    if (_cpuType == MC68HC11)
        return TEXT_CPU_6811;
    return _cpuType == MC6801 ? TEXT_CPU_6801 : TEXT_CPU_6301;
}

bool TableMc6800::setCpu(const char *cpu) {
    const char *p;
    p = cpu;
    if (strncasecmp_P(p, TEXT_CPU_MC, 2) == 0)
        p += 2;
    if (strcasecmp_P(p, TEXT_CPU_6800) == 0)
        return setCpu(MC6800);
    if (strcasecmp_P(p, TEXT_CPU_6801) == 0)
        return setCpu(MC6801);
    if (strcasecmp_P(p, TEXT_CPU_6811) == 0
        || strcasecmp_P(p, TEXT_CPU_HC11) == 0)
        return setCpu(MC68HC11);
    p = cpu;
    if (strncasecmp_P(p, TEXT_CPU_HD, 2) == 0)
        p += 2;
    if (strcasecmp_P(p, TEXT_CPU_6301) == 0)
        return setCpu(HD6301);
    return false;
}

class TableMc6800 TableMc6800;

} // namespace mc6800
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
