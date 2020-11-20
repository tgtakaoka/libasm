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

#include "table_mc6800.h"

#include <string.h>

#include "config_mc6800.h"
#include "entry_mc6800.h"
#include "text_mc6800.h"

namespace libasm {
namespace mc6800 {

#define E3(_opc, _name, _size, _op1, _op2, _op3) \
    { _opc, Entry::Flags::create(_op1, _op2, _op3, SZ_##_size), _name }
#define E2(_opc, _name, _size, _op1, _op2) \
    E3(_opc, _name, _size, _op1, _op2, M_NO)
#define E1(_opc, _name, _size, _op1) E3(_opc, _name, _size, _op1, M_NO, M_NO)
#define E0(_opc, _name) E3(_opc, _name, NONE, M_NO, M_NO, M_NO)

// clang-format off
static constexpr Entry MC6800_TABLE[] PROGMEM = {
    E0(0x01, TEXT_NOP),
    E0(0x06, TEXT_TAP),
    E0(0x07, TEXT_TPA),
    E0(0x08, TEXT_INX),
    E0(0x09, TEXT_DEX),
    E0(0x0A, TEXT_CLV),
    E0(0x0B, TEXT_SEV),
    E0(0x0C, TEXT_CLC),
    E0(0x0D, TEXT_SEC),
    E0(0x0E, TEXT_CLI),
    E0(0x0F, TEXT_SEI),
    E0(0x10, TEXT_SBA),
    E0(0x11, TEXT_CBA),
    E0(0x16, TEXT_TAB),
    E0(0x17, TEXT_TBA),
    E0(0x19, TEXT_DAA),
    E0(0x1B, TEXT_ABA),
    E1(0x20, TEXT_BRA,  BYTE, M_REL),
    E1(0x22, TEXT_BHI,  BYTE, M_REL),
    E1(0x23, TEXT_BLS,  BYTE, M_REL),
    E1(0x24, TEXT_BHS,  BYTE, M_REL),
    E1(0x24, TEXT_BCC,  BYTE, M_REL),
    E1(0x25, TEXT_BLO,  BYTE, M_REL),
    E1(0x25, TEXT_BCS,  BYTE, M_REL),
    E1(0x26, TEXT_BNE,  BYTE, M_REL),
    E1(0x27, TEXT_BEQ,  BYTE, M_REL),
    E1(0x28, TEXT_BVC,  BYTE, M_REL),
    E1(0x29, TEXT_BVS,  BYTE, M_REL),
    E1(0x2A, TEXT_BPL,  BYTE, M_REL),
    E1(0x2B, TEXT_BMI,  BYTE, M_REL),
    E1(0x2C, TEXT_BGE,  BYTE, M_REL),
    E1(0x2D, TEXT_BLT,  BYTE, M_REL),
    E1(0x2E, TEXT_BGT,  BYTE, M_REL),
    E1(0x2F, TEXT_BLE,  BYTE, M_REL),
    E0(0x30, TEXT_TSX),
    E0(0x31, TEXT_INS),
    E0(0x32, TEXT_PULA),
    E0(0x33, TEXT_PULB),
    E0(0x34, TEXT_DES),
    E0(0x35, TEXT_TXS),
    E0(0x36, TEXT_PSHA),
    E0(0x37, TEXT_PSHB),
    E0(0x39, TEXT_RTS),
    E0(0x3B, TEXT_RTI),
    E0(0x3E, TEXT_WAI),
    E0(0x3F, TEXT_SWI),
    E0(0x40, TEXT_NEGA),
    E0(0x50, TEXT_NEGB),
    E0(0x43, TEXT_COMA),
    E0(0x53, TEXT_COMB),
    E0(0x44, TEXT_LSRA),
    E0(0x54, TEXT_LSRB),
    E0(0x46, TEXT_RORA),
    E0(0x56, TEXT_RORB),
    E0(0x47, TEXT_ASRA),
    E0(0x57, TEXT_ASRB),
    E0(0x48, TEXT_ASLA),
    E0(0x58, TEXT_ASLB),
    E0(0x48, TEXT_LSLA),
    E0(0x58, TEXT_LSLB),
    E0(0x49, TEXT_ROLA),
    E0(0x59, TEXT_ROLB),
    E0(0x4A, TEXT_DECA),
    E0(0x5A, TEXT_DECB),
    E0(0x4C, TEXT_INCA),
    E0(0x5C, TEXT_INCB),
    E0(0x4D, TEXT_TSTA),
    E0(0x5D, TEXT_TSTB),
    E0(0x4F, TEXT_CLRA),
    E0(0x5F, TEXT_CLRB),
    E1(0x60, TEXT_NEG,  BYTE, M_IDX),
    E1(0x63, TEXT_COM,  BYTE, M_IDX),
    E1(0x64, TEXT_LSR,  BYTE, M_IDX),
    E1(0x66, TEXT_ROR,  BYTE, M_IDX),
    E1(0x67, TEXT_ASR,  BYTE, M_IDX),
    E1(0x68, TEXT_ASL,  BYTE, M_IDX),
    E1(0x68, TEXT_LSL,  BYTE, M_IDX),
    E1(0x69, TEXT_ROL,  BYTE, M_IDX),
    E1(0x6A, TEXT_DEC,  BYTE, M_IDX),
    E1(0x6C, TEXT_INC,  BYTE, M_IDX),
    E1(0x6D, TEXT_TST,  BYTE, M_IDX),
    E1(0x6E, TEXT_JMP,  BYTE, M_IDX),
    E1(0x6F, TEXT_CLR,  BYTE, M_IDX),
    E1(0x70, TEXT_NEG,  WORD, M_EXT),
    E1(0x73, TEXT_COM,  WORD, M_EXT),
    E1(0x74, TEXT_LSR,  WORD, M_EXT),
    E1(0x76, TEXT_ROR,  WORD, M_EXT),
    E1(0x77, TEXT_ASR,  WORD, M_EXT),
    E1(0x78, TEXT_ASL,  WORD, M_EXT),
    E1(0x78, TEXT_LSL,  WORD, M_EXT),
    E1(0x79, TEXT_ROL,  WORD, M_EXT),
    E1(0x7A, TEXT_DEC,  WORD, M_EXT),
    E1(0x7C, TEXT_INC,  WORD, M_EXT),
    E1(0x7D, TEXT_TST,  WORD, M_EXT),
    E1(0x7E, TEXT_JMP,  WORD, M_EXT),
    E1(0x7F, TEXT_CLR,  WORD, M_EXT),
    E1(0x80, TEXT_SUBA, BYTE, M_IMM),
    E1(0xC0, TEXT_SUBB, BYTE, M_IMM),
    E1(0x81, TEXT_CMPA, BYTE, M_IMM),
    E1(0xC1, TEXT_CMPB, BYTE, M_IMM),
    E1(0x82, TEXT_SBCA, BYTE, M_IMM),
    E1(0xC2, TEXT_SBCB, BYTE, M_IMM),
    E1(0x84, TEXT_ANDA, BYTE, M_IMM),
    E1(0xC4, TEXT_ANDB, BYTE, M_IMM),
    E1(0x85, TEXT_BITA, BYTE, M_IMM),
    E1(0xC5, TEXT_BITB, BYTE, M_IMM),
    E1(0x86, TEXT_LDAA, BYTE, M_IMM),
    E1(0xC6, TEXT_LDAB, BYTE, M_IMM),
    E1(0x88, TEXT_EORA, BYTE, M_IMM),
    E1(0xC8, TEXT_EORB, BYTE, M_IMM),
    E1(0x89, TEXT_ADCA, BYTE, M_IMM),
    E1(0xC9, TEXT_ADCB, BYTE, M_IMM),
    E1(0x8A, TEXT_ORAA, BYTE, M_IMM),
    E1(0xCA, TEXT_ORAB, BYTE, M_IMM),
    E1(0x8B, TEXT_ADDA, BYTE, M_IMM),
    E1(0xCB, TEXT_ADDB, BYTE, M_IMM),
    E1(0x8C, TEXT_CPX,  WORD, M_IMM),
    E1(0x8D, TEXT_BSR,  BYTE, M_REL),
    E1(0x8E, TEXT_LDS,  WORD, M_IMM),
    E1(0x90, TEXT_SUBA, BYTE, M_DIR),
    E1(0xD0, TEXT_SUBB, BYTE, M_DIR),
    E1(0x91, TEXT_CMPA, BYTE, M_DIR),
    E1(0xD1, TEXT_CMPB, BYTE, M_DIR),
    E1(0x92, TEXT_SBCA, BYTE, M_DIR),
    E1(0xD2, TEXT_SBCB, BYTE, M_DIR),
    E1(0x94, TEXT_ANDA, BYTE, M_DIR),
    E1(0xD4, TEXT_ANDB, BYTE, M_DIR),
    E1(0x95, TEXT_BITA, BYTE, M_DIR),
    E1(0xD5, TEXT_BITB, BYTE, M_DIR),
    E1(0x96, TEXT_LDAA, BYTE, M_DIR),
    E1(0xD6, TEXT_LDAB, BYTE, M_DIR),
    E1(0x97, TEXT_STAA, BYTE, M_DIR),
    E1(0xD7, TEXT_STAB, BYTE, M_DIR),
    E1(0x98, TEXT_EORA, BYTE, M_DIR),
    E1(0xD8, TEXT_EORB, BYTE, M_DIR),
    E1(0x99, TEXT_ADCA, BYTE, M_DIR),
    E1(0xD9, TEXT_ADCB, BYTE, M_DIR),
    E1(0x9A, TEXT_ORAA, BYTE, M_DIR),
    E1(0xDA, TEXT_ORAB, BYTE, M_DIR),
    E1(0x9B, TEXT_ADDA, BYTE, M_DIR),
    E1(0xDB, TEXT_ADDB, BYTE, M_DIR),
    E1(0x9C, TEXT_CPX,  BYTE, M_DIR),
    E1(0x9E, TEXT_LDS,  BYTE, M_DIR),
    E1(0x9F, TEXT_STS,  BYTE, M_DIR),
    E1(0xA0, TEXT_SUBA, BYTE, M_IDX),
    E1(0xE0, TEXT_SUBB, BYTE, M_IDX),
    E1(0xA1, TEXT_CMPA, BYTE, M_IDX),
    E1(0xE1, TEXT_CMPB, BYTE, M_IDX),
    E1(0xA2, TEXT_SBCA, BYTE, M_IDX),
    E1(0xE2, TEXT_SBCB, BYTE, M_IDX),
    E1(0xA4, TEXT_ANDA, BYTE, M_IDX),
    E1(0xE4, TEXT_ANDB, BYTE, M_IDX),
    E1(0xA5, TEXT_BITA, BYTE, M_IDX),
    E1(0xE5, TEXT_BITB, BYTE, M_IDX),
    E1(0xA6, TEXT_LDAA, BYTE, M_IDX),
    E1(0xE6, TEXT_LDAB, BYTE, M_IDX),
    E1(0xA7, TEXT_STAA, BYTE, M_IDX),
    E1(0xE7, TEXT_STAB, BYTE, M_IDX),
    E1(0xA8, TEXT_EORA, BYTE, M_IDX),
    E1(0xE8, TEXT_EORB, BYTE, M_IDX),
    E1(0xA9, TEXT_ADCA, BYTE, M_IDX),
    E1(0xE9, TEXT_ADCB, BYTE, M_IDX),
    E1(0xAA, TEXT_ORAA, BYTE, M_IDX),
    E1(0xEA, TEXT_ORAB, BYTE, M_IDX),
    E1(0xAB, TEXT_ADDA, BYTE, M_IDX),
    E1(0xEB, TEXT_ADDB, BYTE, M_IDX),
    E1(0xAC, TEXT_CPX,  BYTE, M_IDX),
    E1(0xAD, TEXT_JSR,  BYTE, M_IDX),
    E1(0xAE, TEXT_LDS,  BYTE, M_IDX),
    E1(0xAF, TEXT_STS,  BYTE, M_IDX),
    E1(0xB0, TEXT_SUBA, WORD, M_EXT),
    E1(0xF0, TEXT_SUBB, WORD, M_EXT),
    E1(0xB1, TEXT_CMPA, WORD, M_EXT),
    E1(0xF1, TEXT_CMPB, WORD, M_EXT),
    E1(0xB2, TEXT_SBCA, WORD, M_EXT),
    E1(0xF2, TEXT_SBCB, WORD, M_EXT),
    E1(0xB4, TEXT_ANDA, WORD, M_EXT),
    E1(0xF4, TEXT_ANDB, WORD, M_EXT),
    E1(0xB5, TEXT_BITA, WORD, M_EXT),
    E1(0xF5, TEXT_BITB, WORD, M_EXT),
    E1(0xB6, TEXT_LDAA, WORD, M_EXT),
    E1(0xF6, TEXT_LDAB, WORD, M_EXT),
    E1(0xB7, TEXT_STAA, WORD, M_EXT),
    E1(0xF7, TEXT_STAB, WORD, M_EXT),
    E1(0xB8, TEXT_EORA, WORD, M_EXT),
    E1(0xF8, TEXT_EORB, WORD, M_EXT),
    E1(0xB9, TEXT_ADCA, WORD, M_EXT),
    E1(0xF9, TEXT_ADCB, WORD, M_EXT),
    E1(0xBA, TEXT_ORAA, WORD, M_EXT),
    E1(0xFA, TEXT_ORAB, WORD, M_EXT),
    E1(0xBB, TEXT_ADDA, WORD, M_EXT),
    E1(0xFB, TEXT_ADDB, WORD, M_EXT),
    E1(0xBC, TEXT_CPX,  WORD, M_EXT),
    E1(0xBD, TEXT_JSR,  WORD, M_EXT),
    E1(0xBE, TEXT_LDS,  WORD, M_EXT),
    E1(0xBF, TEXT_STS,  WORD, M_EXT),
    E1(0xCE, TEXT_LDX,  WORD, M_IMM),
    E1(0xDE, TEXT_LDX,  BYTE, M_DIR),
    E1(0xDF, TEXT_STX,  BYTE, M_DIR),
    E1(0xEE, TEXT_LDX,  BYTE, M_IDX),
    E1(0xEF, TEXT_STX,  BYTE, M_IDX),
    E1(0xFE, TEXT_LDX,  WORD, M_EXT),
    E1(0xFF, TEXT_STX,  WORD, M_EXT),
};

static constexpr Entry MC6801_TABLE[] PROGMEM = {
    E0(0x04, TEXT_LSRD),
    E0(0x05, TEXT_ASLD),
    E0(0x05, TEXT_LSLD),
    E1(0x21, TEXT_BRN,  BYTE, M_REL),
    E0(0x38, TEXT_PULX),
    E0(0x3A, TEXT_ABX),
    E0(0x3C, TEXT_PSHX),
    E0(0x3D, TEXT_MUL),
    E1(0x83, TEXT_SUBD, WORD, M_IMM),
    E1(0x93, TEXT_SUBD, BYTE, M_DIR),
    E1(0x9D, TEXT_JSR,  NONE, M_DIR),
    E1(0xA3, TEXT_SUBD, BYTE, M_IDX),
    E1(0xB3, TEXT_SUBD, WORD, M_EXT),
    E1(0xC3, TEXT_ADDD, WORD, M_IMM),
    E1(0xCC, TEXT_LDD,  WORD, M_IMM),
    E1(0xD3, TEXT_ADDD, BYTE, M_DIR),
    E1(0xDC, TEXT_LDD,  BYTE, M_DIR),
    E1(0xDD, TEXT_STD,  BYTE, M_DIR),
    E1(0xE3, TEXT_ADDD, BYTE, M_IDX),
    E1(0xEC, TEXT_LDD,  BYTE, M_IDX),
    E1(0xED, TEXT_STD,  BYTE, M_IDX),
    E1(0xF3, TEXT_ADDD, WORD, M_EXT),
    E1(0xFC, TEXT_LDD,  WORD, M_EXT),
    E1(0xFD, TEXT_STD,  WORD, M_EXT),
};

static constexpr Entry HD6301_TABLE[] PROGMEM = {
    E0(0x18, TEXT_XGDX),
    E0(0x1A, TEXT_SLP),
    E2(0x61, TEXT_AIM,  BYTE, M_BMM, M_IDX),
    E2(0x61, TEXT_BCLR, BYTE, M_BIT, M_IDX),
    E2(0x62, TEXT_OIM,  BYTE, M_BMM, M_IDX),
    E2(0x62, TEXT_BSET, BYTE, M_BIT, M_IDX),
    E2(0x65, TEXT_EIM,  BYTE, M_BMM, M_IDX),
    E2(0x65, TEXT_BTGL, BYTE, M_BIT, M_IDX),
    E2(0x6B, TEXT_TIM,  BYTE, M_BMM, M_IDX),
    E2(0x6B, TEXT_BTST, BYTE, M_BIT, M_IDX),
    E2(0x71, TEXT_AIM,  BYTE, M_BMM, M_DIR),
    E2(0x71, TEXT_BCLR, BYTE, M_BIT, M_DIR),
    E2(0x72, TEXT_OIM,  BYTE, M_BMM, M_DIR),
    E2(0x72, TEXT_BSET, BYTE, M_BIT, M_DIR),
    E2(0x75, TEXT_EIM,  BYTE, M_BMM, M_DIR),
    E2(0x75, TEXT_BTGL, BYTE, M_BIT, M_DIR),
    E2(0x7B, TEXT_TIM,  BYTE, M_BMM, M_DIR),
    E2(0x7B, TEXT_BTST, BYTE, M_BIT, M_DIR),
};

static constexpr Entry MC68HC11_P00[] PROGMEM = {
    E0(0x02, TEXT_IDIV),
    E0(0x03, TEXT_FDIV),
    E3(0x12, TEXT_BRSET, BYTE, M_DIR, M_IMM, M_REL),
    E3(0x13, TEXT_BRCLR, BYTE, M_DIR, M_IMM, M_REL),
    E2(0x14, TEXT_BSET,  BYTE, M_DIR, M_IMM),
    E2(0x15, TEXT_BCLR,  BYTE, M_DIR, M_IMM),
    E2(0x1C, TEXT_BSET,  BYTE, M_IDX, M_IMM),
    E2(0x1D, TEXT_BCLR,  BYTE, M_IDX, M_IMM),
    E3(0x1E, TEXT_BRSET, BYTE, M_IDX, M_IMM, M_REL),
    E3(0x1F, TEXT_BRCLR, BYTE, M_IDX, M_IMM, M_REL),
    E0(0x8F, TEXT_XGDX),
    E0(0xCF, TEXT_STOP),
};

static constexpr Entry MC68HC11_P18[] PROGMEM = {
    E0(0x08, TEXT_INY),
    E0(0x09, TEXT_DEY),
    E2(0x1C, TEXT_BSET,  BYTE, M_IDY, M_IMM),
    E2(0x1D, TEXT_BCLR,  BYTE, M_IDY, M_IMM),
    E3(0x1E, TEXT_BRSET, BYTE, M_IDY, M_IMM, M_REL),
    E3(0x1F, TEXT_BRCLR, BYTE, M_IDY, M_IMM, M_REL),
    E0(0x30, TEXT_TSY),
    E0(0x35, TEXT_TYS),
    E0(0x38, TEXT_PULY),
    E0(0x3A, TEXT_ABY),
    E0(0x3C, TEXT_PSHY),
    E1(0x60, TEXT_NEG,   BYTE, M_IDY),
    E1(0x63, TEXT_COM,   BYTE, M_IDY),
    E1(0x64, TEXT_LSR,   BYTE, M_IDY),
    E1(0x66, TEXT_ROR,   BYTE, M_IDY),
    E1(0x67, TEXT_ASR,   BYTE, M_IDY),
    E1(0x68, TEXT_ASL,   BYTE, M_IDY),
    E1(0x68, TEXT_LSL,   BYTE, M_IDY),
    E1(0x69, TEXT_ROL,   BYTE, M_IDY),
    E1(0x6A, TEXT_DEC,   BYTE, M_IDY),
    E1(0x6C, TEXT_INC,   BYTE, M_IDY),
    E1(0x6D, TEXT_TST,   BYTE, M_IDY),
    E1(0x6E, TEXT_JMP,   BYTE, M_IDY),
    E1(0x6F, TEXT_CLR,   BYTE, M_IDY),
    E1(0x8C, TEXT_CPY,   WORD, M_IMM),
    E0(0x8F, TEXT_XGDY),
    E1(0x9C, TEXT_CPY,   BYTE, M_DIR),
    E1(0xA0, TEXT_SUBA,  BYTE, M_IDY),
    E1(0xE0, TEXT_SUBB,  BYTE, M_IDY),
    E1(0xA1, TEXT_CMPA,  BYTE, M_IDY),
    E1(0xE1, TEXT_CMPB,  BYTE, M_IDY),
    E1(0xA2, TEXT_SBCA,  BYTE, M_IDY),
    E1(0xE2, TEXT_SBCB,  BYTE, M_IDY),
    E1(0xA3, TEXT_SUBD,  BYTE, M_IDY),
    E1(0xE3, TEXT_ADDD,  BYTE, M_IDY),
    E1(0xA4, TEXT_ANDA,  BYTE, M_IDY),
    E1(0xE4, TEXT_ANDB,  BYTE, M_IDY),
    E1(0xA5, TEXT_BITA,  BYTE, M_IDY),
    E1(0xE5, TEXT_BITB,  BYTE, M_IDY),
    E1(0xA6, TEXT_LDAA,  BYTE, M_IDY),
    E1(0xE6, TEXT_LDAB,  BYTE, M_IDY),
    E1(0xA7, TEXT_STAA,  BYTE, M_IDY),
    E1(0xE7, TEXT_STAB,  BYTE, M_IDY),
    E1(0xA8, TEXT_EORA,  BYTE, M_IDY),
    E1(0xE8, TEXT_EORB,  BYTE, M_IDY),
    E1(0xA9, TEXT_ADCA,  BYTE, M_IDY),
    E1(0xE9, TEXT_ADCB,  BYTE, M_IDY),
    E1(0xAA, TEXT_ORAA,  BYTE, M_IDY),
    E1(0xEA, TEXT_ORAB,  BYTE, M_IDY),
    E1(0xAB, TEXT_ADDA,  BYTE, M_IDY),
    E1(0xEB, TEXT_ADDB,  BYTE, M_IDY),
    E1(0xAC, TEXT_CPY,   BYTE, M_IDY),
    E1(0xAD, TEXT_JSR,   BYTE, M_IDY),
    E1(0xAE, TEXT_LDS,   BYTE, M_IDY),
    E1(0xAF, TEXT_STS,   BYTE, M_IDY),
    E1(0xBC, TEXT_CPY,   WORD, M_EXT),
    E1(0xCE, TEXT_LDY,   WORD, M_IMM),
    E1(0xDE, TEXT_LDY,   BYTE, M_DIR),
    E1(0xDF, TEXT_STY,   BYTE, M_DIR),
    E1(0xEC, TEXT_LDD,   BYTE, M_IDY),
    E1(0xED, TEXT_STD,   BYTE, M_IDY),
    E1(0xEE, TEXT_LDY,   BYTE, M_IDY),
    E1(0xEF, TEXT_STY,   BYTE, M_IDY),
    E1(0xFE, TEXT_LDY,   WORD, M_EXT),
    E1(0xFF, TEXT_STY,   WORD, M_EXT),
};

static constexpr Entry MC68HC11_P1A[] PROGMEM = {
    E1(0x83, TEXT_CPD,   WORD, M_IMM),
    E1(0x93, TEXT_CPD,   BYTE, M_DIR),
    E1(0xA3, TEXT_CPD,   BYTE, M_IDX),
    E1(0xAC, TEXT_CPY,   BYTE, M_IDX),
    E1(0xB3, TEXT_CPD,   BYTE, M_EXT),
    E1(0xEE, TEXT_LDY,   BYTE, M_IDX),
    E1(0xEF, TEXT_STY,   BYTE, M_IDX),
};

static constexpr Entry MC68HC11_PCD[] PROGMEM = {
    E1(0xA3, TEXT_CPD,   BYTE, M_IDY),
    E1(0xAC, TEXT_CPX,   BYTE, M_IDY),
    E1(0xEE, TEXT_LDX,   BYTE, M_IDY),
    E1(0xEF, TEXT_STX,   BYTE, M_IDY),
};
// clang-format on

static constexpr Config::opcode_t PREFIX_P00 = 0x00;
static constexpr Config::opcode_t PREFIX_P18 = 0x18;
static constexpr Config::opcode_t PREFIX_P1A = 0x1A;
static constexpr Config::opcode_t PREFIX_PCD = 0xCD;

bool TableMc6800::isPrefix(Config::opcode_t opCode) const {
    if (_cpuType != MC68HC11)
        return false;
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
        {PREFIX_P00, ARRAY_RANGE(MC6800_TABLE)},
};

static constexpr TableMc6800::EntryPage MC6801_PAGES[] PROGMEM = {
        {PREFIX_P00, ARRAY_RANGE(MC6801_TABLE)},
        {PREFIX_P00, ARRAY_RANGE(MC6800_TABLE)},
};

static constexpr TableMc6800::EntryPage HD6301_PAGES[] PROGMEM = {
        {PREFIX_P00, ARRAY_RANGE(HD6301_TABLE)},
        {PREFIX_P00, ARRAY_RANGE(MC6801_TABLE)},
        {PREFIX_P00, ARRAY_RANGE(MC6800_TABLE)},
};

static constexpr TableMc6800::EntryPage MC68HC11_PAGES[] PROGMEM = {
        {PREFIX_P00, ARRAY_RANGE(MC6801_TABLE)},
        {PREFIX_P00, ARRAY_RANGE(MC6800_TABLE)},
        {PREFIX_P00, ARRAY_RANGE(MC68HC11_P00)},
        {PREFIX_P18, ARRAY_RANGE(MC68HC11_P18)},
        {PREFIX_P1A, ARRAY_RANGE(MC68HC11_P1A)},
        {PREFIX_PCD, ARRAY_RANGE(MC68HC11_PCD)},
};

static bool acceptAddrMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_EXT)
        return table == M_REL;
    if (opr == M_DIR)
        return table == M_REL || table == M_EXT;
    if (opr == M_BIT)
        return table == M_REL || table == M_EXT || table == M_DIR;
    if (opr == M_IMM)
        return table == M_BMM;
    return false;
}

static bool acceptAddrMode(Entry::Flags flags, const Entry *entry) {
    const Entry::Flags table = entry->flags();
    return acceptAddrMode(flags.mode1(), table.mode1()) &&
           acceptAddrMode(flags.mode2(), table.mode2()) &&
           acceptAddrMode(flags.mode3(), table.mode3());
}

Error TableMc6800::searchName(
        InsnMc6800 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *entry = TableBase::searchName<Entry, Entry::Flags>(
                insn.name(), insn.flags(), page->table(), page->end(),
                acceptAddrMode, count);
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
        if (insn.prefix() != prefix)
            continue;
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
    if (!entry)
        return _error.setError(INTERNAL_ERROR);
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
    if (strcasecmp_P(p, TEXT_CPU_6811) == 0 ||
            strcasecmp_P(p, TEXT_CPU_HC11) == 0)
        return setCpu(MC68HC11);
    p = cpu;
    if (strncasecmp_P(p, TEXT_CPU_HD, 2) == 0)
        p += 2;
    if (strcasecmp_P(p, TEXT_CPU_6301) == 0)
        return setCpu(HD6301);
    return false;
}

class TableMc6800 TableMc6800;

}  // namespace mc6800
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
