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

#include "config_z8.h"

#include "entry_z8.h"
#include "table_z8.h"
#include "text_z8.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace z8 {

#define P(_opc, _name, _post, _dst, _src, _dstSrc, _ext) \
    { _opc, Entry::Flags::create(_dst, _src, _ext, _dstSrc, _post), _name }
#define E(_opc, _name, _dst, _src, _dstSrc) P(_opc, _name, P0, _dst, _src, _dstSrc, M_NO)

// clang-format off
static constexpr Entry TABLE_COMMON[] PROGMEM = {
    E(0x08, TEXT_LD,   M_r,   M_R,  DST_SRC),
    E(0x09, TEXT_LD,   M_R,   M_r,  SRC_DST),
    E(0x0C, TEXT_LD,   M_r,   M_IM, DST_SRC),
    E(0xE4, TEXT_LD,   M_R,   M_R,  SRC_DST),
    E(0xE5, TEXT_LD,   M_R,   M_IR, SRC_DST),
    E(0xE6, TEXT_LD,   M_R,   M_IM, DST_SRC),
    E(0xF5, TEXT_LD,   M_IR,  M_R,  SRC_DST),
    E(0x0A, TEXT_DJNZ, M_r,   M_RA, DST_SRC),
    E(0x8B, TEXT_JR,   M_RA,  M_NO, DS_NO),
    E(0x0B, TEXT_JR,   M_cc,  M_RA, DST_SRC),
    E(0x8D, TEXT_JP,   M_DA,  M_NO, DS_NO),
    E(0x0D, TEXT_JP,   M_cc,  M_DA, DST_SRC),
    E(0x30, TEXT_JP,   M_IRR, M_NO, DS_NO),
    E(0x02, TEXT_ADD,  M_r,   M_r,  DST_SRC),
    E(0x03, TEXT_ADD,  M_r,   M_Ir, DST_SRC),
    E(0x04, TEXT_ADD,  M_R,   M_R,  SRC_DST),
    E(0x05, TEXT_ADD,  M_R,   M_IR, SRC_DST),
    E(0x06, TEXT_ADD,  M_R,   M_IM, DST_SRC),
    E(0x12, TEXT_ADC,  M_r,   M_r,  DST_SRC),
    E(0x13, TEXT_ADC,  M_r,   M_Ir, DST_SRC),
    E(0x14, TEXT_ADC,  M_R,   M_R,  SRC_DST),
    E(0x15, TEXT_ADC,  M_R,   M_IR, SRC_DST),
    E(0x16, TEXT_ADC,  M_R,   M_IM, DST_SRC),
    E(0x22, TEXT_SUB,  M_r,   M_r,  DST_SRC),
    E(0x23, TEXT_SUB,  M_r,   M_Ir, DST_SRC),
    E(0x24, TEXT_SUB,  M_R,   M_R,  SRC_DST),
    E(0x25, TEXT_SUB,  M_R,   M_IR, SRC_DST),
    E(0x26, TEXT_SUB,  M_R,   M_IM, DST_SRC),
    E(0x32, TEXT_SBC,  M_r,   M_r,  DST_SRC),
    E(0x33, TEXT_SBC,  M_r,   M_Ir, DST_SRC),
    E(0x34, TEXT_SBC,  M_R,   M_R,  SRC_DST),
    E(0x35, TEXT_SBC,  M_R,   M_IR, SRC_DST),
    E(0x36, TEXT_SBC,  M_R,   M_IM, DST_SRC),
    E(0x42, TEXT_OR,   M_r,   M_r,  DST_SRC),
    E(0x43, TEXT_OR,   M_r,   M_Ir, DST_SRC),
    E(0x44, TEXT_OR,   M_R,   M_R,  SRC_DST),
    E(0x45, TEXT_OR,   M_R,   M_IR, SRC_DST),
    E(0x46, TEXT_OR,   M_R,   M_IM, DST_SRC),
    E(0x52, TEXT_AND,  M_r,   M_r,  DST_SRC),
    E(0x53, TEXT_AND,  M_r,   M_Ir, DST_SRC),
    E(0x54, TEXT_AND,  M_R,   M_R,  SRC_DST),
    E(0x55, TEXT_AND,  M_R,   M_IR, SRC_DST),
    E(0x56, TEXT_AND,  M_R,   M_IM, DST_SRC),
    E(0x62, TEXT_TCM,  M_r,   M_r,  DST_SRC),
    E(0x63, TEXT_TCM,  M_r,   M_Ir, DST_SRC),
    E(0x64, TEXT_TCM,  M_R,   M_R,  SRC_DST),
    E(0x65, TEXT_TCM,  M_R,   M_IR, SRC_DST),
    E(0x66, TEXT_TCM,  M_R,   M_IM, DST_SRC),
    E(0x72, TEXT_TM,   M_r,   M_r,  DST_SRC),
    E(0x73, TEXT_TM,   M_r,   M_Ir, DST_SRC),
    E(0x74, TEXT_TM,   M_R,   M_R,  SRC_DST),
    E(0x75, TEXT_TM,   M_R,   M_IR, SRC_DST),
    E(0x76, TEXT_TM,   M_R,   M_IM, DST_SRC),
    E(0xA2, TEXT_CP,   M_r,   M_r,  DST_SRC),
    E(0xA3, TEXT_CP,   M_r,   M_Ir, DST_SRC),
    E(0xA4, TEXT_CP,   M_R,   M_R,  SRC_DST),
    E(0xA5, TEXT_CP,   M_R,   M_IR, SRC_DST),
    E(0xA6, TEXT_CP,   M_R,   M_IM, DST_SRC),
    E(0xB2, TEXT_XOR,  M_r,   M_r,  DST_SRC),
    E(0xB3, TEXT_XOR,  M_r,   M_Ir, DST_SRC),
    E(0xB4, TEXT_XOR,  M_R,   M_R,  SRC_DST),
    E(0xB5, TEXT_XOR,  M_R,   M_IR, SRC_DST),
    E(0xB6, TEXT_XOR,  M_R,   M_IM, DST_SRC),
    E(0x00, TEXT_DEC,  M_R,   M_NO, DS_NO),
    E(0x01, TEXT_DEC,  M_IR,  M_NO, DS_NO),
    E(0x10, TEXT_RLC,  M_R,   M_NO, DS_NO),
    E(0x11, TEXT_RLC,  M_IR,  M_NO, DS_NO),
    E(0x0E, TEXT_INC,  M_r,   M_NO, DS_NO),
    E(0x20, TEXT_INC,  M_R,   M_NO, DS_NO),
    E(0x21, TEXT_INC,  M_IR,  M_NO, DS_NO),
    E(0x40, TEXT_DA,   M_R,   M_NO, DS_NO),
    E(0x41, TEXT_DA,   M_IR,  M_NO, DS_NO),
    E(0x50, TEXT_POP,  M_R,   M_NO, DS_NO),
    E(0x51, TEXT_POP,  M_IR,  M_NO, DS_NO),
    E(0x60, TEXT_COM,  M_R,   M_NO, DS_NO),
    E(0x61, TEXT_COM,  M_IR,  M_NO, DS_NO),
    E(0x70, TEXT_PUSH, M_R,   M_NO, DS_NO),
    E(0x71, TEXT_PUSH, M_IR,  M_NO, DS_NO),
    E(0x80, TEXT_DECW, M_RR,  M_NO, DS_NO),
    E(0x81, TEXT_DECW, M_IR,  M_NO, DS_NO),
    E(0x90, TEXT_RL,   M_R,   M_NO, DS_NO),
    E(0x91, TEXT_RL,   M_IR,  M_NO, DS_NO),
    E(0xA0, TEXT_INCW, M_RR,  M_NO, DS_NO),
    E(0xA1, TEXT_INCW, M_IR,  M_NO, DS_NO),
    E(0xB0, TEXT_CLR,  M_R,   M_NO, DS_NO),
    E(0xB1, TEXT_CLR,  M_IR,  M_NO, DS_NO),
    E(0xC0, TEXT_RRC,  M_R,   M_NO, DS_NO),
    E(0xC1, TEXT_RRC,  M_IR,  M_NO, DS_NO),
    E(0xD0, TEXT_SRA,  M_R,   M_NO, DS_NO),
    E(0xD1, TEXT_SRA,  M_IR,  M_NO, DS_NO),
    E(0xE0, TEXT_RR,   M_R,   M_NO, DS_NO),
    E(0xE1, TEXT_RR,   M_IR,  M_NO, DS_NO),
    E(0xF0, TEXT_SWAP, M_R,   M_NO, DS_NO),
    E(0xF1, TEXT_SWAP, M_IR,  M_NO, DS_NO),
    E(0x8F, TEXT_DI,   M_NO,  M_NO, DS_NO),
    E(0x9F, TEXT_EI,   M_NO,  M_NO, DS_NO),
    E(0xAF, TEXT_RET,  M_NO,  M_NO, DS_NO),
    E(0xBF, TEXT_IRET, M_NO,  M_NO, DS_NO),
    E(0xCF, TEXT_RCF,  M_NO,  M_NO, DS_NO),
    E(0xDF, TEXT_SCF,  M_NO,  M_NO, DS_NO),
    E(0xEF, TEXT_CCF,  M_NO,  M_NO, DS_NO),
    E(0xFF, TEXT_NOP,  M_NO,  M_NO, DS_NO),
};

static constexpr Entry TABLE_Z8[] PROGMEM = {
    E(0x31, TEXT_SRP,  M_IM,  M_NO,  DS_NO),
    E(0x07, TEXT_ADD,  M_IR,  M_IM,  DST_SRC),
    E(0x17, TEXT_ADC,  M_IR,  M_IM,  DST_SRC),
    E(0x27, TEXT_SUB,  M_IR,  M_IM,  DST_SRC),
    E(0x37, TEXT_SBC,  M_IR,  M_IM,  DST_SRC),
    E(0x47, TEXT_OR,   M_IR,  M_IM,  DST_SRC),
    E(0x57, TEXT_AND,  M_IR,  M_IM,  DST_SRC),
    E(0x67, TEXT_TCM,  M_IR,  M_IM,  DST_SRC),
    E(0x77, TEXT_TM,   M_IR,  M_IM,  DST_SRC),
    E(0xA7, TEXT_CP,   M_IR,  M_IM,  DST_SRC),
    E(0xB7, TEXT_XOR,  M_IR,  M_IM,  DST_SRC),
    E(0xC7, TEXT_LD,   M_r,   M_X,   DST_SRC),
    E(0xD7, TEXT_LD,   M_X,   M_r,   SRC_DST),
    E(0xE3, TEXT_LD,   M_r,   M_Ir,  DST_SRC),
    E(0xE7, TEXT_LD,   M_IR,  M_IM,  DST_SRC),
    E(0xF3, TEXT_LD,   M_Ir,  M_r,   DST_SRC),
    E(0x82, TEXT_LDE,  M_r,   M_Irr, DST_SRC),
    E(0x92, TEXT_LDE,  M_Irr, M_r,   SRC_DST),
    E(0x83, TEXT_LDEI, M_Ir,  M_Irr, DST_SRC),
    E(0x93, TEXT_LDEI, M_Irr, M_Ir,  SRC_DST),
    E(0xC2, TEXT_LDC,  M_r,   M_Irr, DST_SRC),
    E(0xD2, TEXT_LDC,  M_Irr, M_r,   SRC_DST),
    E(0xC3, TEXT_LDCI, M_Ir,  M_Irr, DST_SRC),
    E(0xD3, TEXT_LDCI, M_Irr, M_Ir,  SRC_DST),
    E(0xD6, TEXT_CALL, M_DA,  M_NO,  DS_NO),
    E(0xD4, TEXT_CALL, M_IRR, M_NO,  DS_NO),
};

static constexpr Entry TABLE_Z86C[] PROGMEM = {
    E(0x6F, TEXT_STOP, M_NO,  M_NO, DS_NO),
    E(0x7F, TEXT_HALT, M_NO,  M_NO, DS_NO),
};

static constexpr Entry TABLE_SUPER8[] PROGMEM = {
    E(0x0F, TEXT_NEXT, M_NO,  M_NO,  DS_NO),
    E(0x1F, TEXT_ENTER,M_NO,  M_NO,  DS_NO),
    E(0x2F, TEXT_EXIT, M_NO,  M_NO,  DS_NO),
    E(0x3F, TEXT_WFI,  M_NO,  M_NO,  DS_NO),
    E(0x4F, TEXT_SB0,  M_NO,  M_NO,  DS_NO),
    E(0x5F, TEXT_SB1,  M_NO,  M_NO,  DS_NO),
    E(0xF6, TEXT_CALL, M_DA,  M_NO,  DS_NO),
    E(0xF4, TEXT_CALL, M_IRR, M_NO,  DS_NO),
    E(0xD4, TEXT_CALL, M_IM,  M_NO,  DS_NO),
    E(0x87, TEXT_LD,   M_r,   M_X,   DST_SRC),
    E(0x97, TEXT_LD,   M_X,   M_r,   SRC_DST),
    E(0xC7, TEXT_LD,   M_r,   M_Ir,  DST_SRC),
    E(0xD7, TEXT_LD,   M_Ir,  M_r,   DST_SRC),
    E(0xD6, TEXT_LD,   M_IR,  M_IM,  DST_SRC),
    E(0xC4, TEXT_LDW,  M_RR,  M_RR,  SRC_DST),
    E(0xC5, TEXT_LDW,  M_RR,  M_IR,  SRC_DST),
    E(0xC6, TEXT_LDW,  M_RR,  M_IML, DST_SRC),
    E(0x94, TEXT_DIV,  M_RR,  M_R,   SRC_DST),
    E(0x95, TEXT_DIV,  M_RR,  M_IR,  SRC_DST),
    E(0x96, TEXT_DIV,  M_RR,  M_IM,  SRC_DST),
    E(0x84, TEXT_MULT, M_RR,  M_R,   SRC_DST),
    E(0x85, TEXT_MULT, M_RR,  M_IR,  SRC_DST),
    E(0x86, TEXT_MULT, M_RR,  M_IM,  SRC_DST),
    E(0x82, TEXT_PUSHUD, M_IR, M_R,  DST_SRC),
    E(0x83, TEXT_PUSHUI, M_IR, M_R,  DST_SRC),
    E(0x92, TEXT_POPUD,  M_R,  M_IR, SRC_DST),
    E(0x93, TEXT_POPUI,  M_R,  M_IR, SRC_DST),
    P(0xC2, TEXT_CPIJE,  P0, M_r, M_Ir, SRC_DST, M_RA),
    P(0xD2, TEXT_CPIJNE, P0, M_r, M_Ir, SRC_DST, M_RA),
};

static constexpr Entry TABLE_SUPER8_POST[] PROGMEM {
    P(0xA7, TEXT_LDC,   P4_0, M_r,   M_DA,  DST_SRC, M_NO),
    P(0xA7, TEXT_LDE,   P4_1, M_r,   M_DA,  DST_SRC, M_NO),
    P(0xB7, TEXT_LDC,   P4_0, M_DA,  M_r,   SRC_DST, M_NO),
    P(0xB7, TEXT_LDE,   P4_1, M_DA,  M_r,   SRC_DST, M_NO),
    P(0xE7, TEXT_LDC,   P1_0, M_r,   M_XS,  DST_SRC, M_NO),
    P(0xE7, TEXT_LDE,   P1_1, M_r,   M_XS,  DST_SRC, M_NO),
    P(0xF7, TEXT_LDC,   P1_0, M_XS,  M_r,   SRC_DST, M_NO),
    P(0xF7, TEXT_LDE,   P1_1, M_XS,  M_r,   SRC_DST, M_NO),
    P(0xA7, TEXT_LDC,   P1_0, M_r,   M_XL,  DST_SRC, M_NO),
    P(0xA7, TEXT_LDE,   P1_1, M_r,   M_XL,  DST_SRC, M_NO),
    P(0xB7, TEXT_LDC,   P1_0, M_XL,  M_r,   SRC_DST, M_NO),
    P(0xB7, TEXT_LDE,   P1_1, M_XL,  M_r,   SRC_DST, M_NO),
    P(0x31, TEXT_SRP,   P2_0, M_IM,  M_NO,  DS_NO,   M_NO),
    P(0x31, TEXT_SRP0,  P2_2, M_IM,  M_NO,  DS_NO,   M_NO),
    P(0x31, TEXT_SRP1,  P2_1, M_IM,  M_NO,  DS_NO,   M_NO),
    P(0xC3, TEXT_LDC,   P1_0, M_r,   M_Irr, DST_SRC, M_NO),
    P(0xC3, TEXT_LDE,   P1_1, M_r,   M_Irr, DST_SRC, M_NO),
    P(0xD3, TEXT_LDC,   P1_0, M_Irr, M_r,   SRC_DST, M_NO),
    P(0xD3, TEXT_LDE,   P1_1, M_Irr, M_r,   SRC_DST, M_NO),
    P(0xE2, TEXT_LDCD,  P1_0, M_r,   M_Irr, DST_SRC, M_NO),
    P(0xE2, TEXT_LDED,  P1_1, M_r,   M_Irr, DST_SRC, M_NO),
    P(0xE3, TEXT_LDCI,  P1_0, M_r,   M_Irr, DST_SRC, M_NO),
    P(0xE3, TEXT_LDEI,  P1_1, M_r,   M_Irr, DST_SRC, M_NO),
    P(0xF2, TEXT_LDCPD, P1_0, M_Irr, M_r,   SRC_DST, M_NO),
    P(0xF2, TEXT_LDEPD, P1_1, M_Irr, M_r,   SRC_DST, M_NO),
    P(0xF3, TEXT_LDCPI, P1_0, M_Irr, M_r,   SRC_DST, M_NO),
    P(0xF3, TEXT_LDEPI, P1_1, M_Irr, M_r,   SRC_DST, M_NO),
    P(0x17, TEXT_BCP,   P1_0, M_r,   M_R,   DST_SRC, M_IMb),
    P(0x57, TEXT_BITC,  P1_0, M_r,   M_IMb, DS_NO,   M_NO),
    P(0x77, TEXT_BITR,  P1_0, M_r,   M_IMb, DS_NO,   M_NO),
    P(0x77, TEXT_BITS,  P1_1, M_r,   M_IMb, DS_NO,   M_NO),
    P(0x07, TEXT_BOR,   P1_0, M_r,   M_R,   DST_SRC, M_IMb),
    P(0x07, TEXT_BOR,   P1_1, M_R,   M_IMb, SRC_DST, M_r),
    P(0x27, TEXT_BXOR,  P1_0, M_r,   M_R,   DST_SRC, M_IMb),
    P(0x27, TEXT_BXOR,  P1_1, M_R,   M_IMb, SRC_DST, M_r),
    P(0x47, TEXT_LDB,   P1_0, M_r,   M_R,   DST_SRC, M_IMb),
    P(0x47, TEXT_LDB,   P1_1, M_R,   M_IMb, SRC_DST, M_r),
    P(0x67, TEXT_BAND,  P1_0, M_r,   M_R,   DST_SRC, M_IMb),
    P(0x67, TEXT_BAND,  P1_1, M_R,   M_IMb, SRC_DST, M_r),
    P(0x37, TEXT_BTJRF, P1_0, M_RA,  M_r,   SRC_DST, M_IMb),
    P(0x37, TEXT_BTJRT, P1_1, M_RA,  M_r,   SRC_DST, M_IMb),
};
// clang-format on

struct TableZ8::EntryPage : EntryPageBase<Entry> {
    constexpr EntryPage(const Entry *table, const Entry *end) : EntryPageBase(table, end) {}
};

static constexpr TableZ8::EntryPage Z8_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_Z8)},
        {ARRAY_RANGE(TABLE_COMMON)},
};

static constexpr TableZ8::EntryPage Z86C_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_Z8)},
        {ARRAY_RANGE(TABLE_COMMON)},
        {ARRAY_RANGE(TABLE_Z86C)},
};

static constexpr TableZ8::EntryPage SUPER8_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_SUPER8)},
        {ARRAY_RANGE(TABLE_SUPER8_POST)},
        {ARRAY_RANGE(TABLE_COMMON)},
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_IM)
        return table == M_IMb || table == M_IML;
    if (opr == M_DA)
        return table == M_RA;
    if (opr == M_R)
        return table == M_DA || table == M_RA;
    if (opr == M_RR)
        return table == M_DA || table == M_RA || table == M_R;
    if (opr == M_IRR)
        return table == M_IR;
    if (opr == M_r)
        return table == M_R;
    if (opr == M_rr)
        return table == M_RR;
    if (opr == M_Ir)
        return table == M_IR;
    if (opr == M_Irr)
        return table == M_IRR;
    if (opr == M_W)
        return table == M_DA || table == M_RA || table == M_r || table == M_R;
    if (opr == M_WW)
        return table == M_DA || table == M_RA || table == M_rr || table == M_RR || table == M_r ||
               table == M_R;
    if (opr == M_IW)
        return table == M_Ir || table == M_IR;
    if (opr == M_IWW)
        return table == M_Irr || table == M_IRR || table == M_Ir || table == M_IR;
    if (opr == M_XS)
        return table == M_X;
    if (opr == M_XL)
        return table == M_X;
    return false;
}

static bool acceptModes(Entry::Flags flags, const Entry *entry) {
    auto table = entry->flags();
    return acceptMode(flags.dstMode(), table.dstMode()) &&
           acceptMode(flags.srcMode(), table.srcMode()) &&
           acceptMode(flags.extMode(), table.extMode());
}

Error TableZ8::searchName(InsnZ8 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (auto page = pages; page < end; page++) {
        auto entry = TableBase::searchName<Entry, Entry::Flags>(
                insn.name(), insn.flags(), page->table(), page->end(), acceptModes, count);
        if (entry) {
            insn.setOpCode(entry->opCode());
            insn.setFlags(entry->flags());
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

static Config::opcode_t maskCode(Config::opcode_t opCode, const Entry *entry) {
    auto table = entry->opCode();
    return InsnZ8::operandInOpCode(table) ? opCode & 0x0f : opCode;
}

static bool matchPostByte(const InsnZ8 &insn) {
    auto post = insn.post();
    switch (insn.postFormat()) {
    case P1_0:
        return (post & 1) == 0;
    case P1_1:
        return (post & 1) == 1;
    case P2_0:
        return (post & 3) == 0;
    case P2_1:
        return (post & 3) == 1;
    case P2_2:
        return (post & 3) == 2;
    case P4_0:
        return (post & 0xF) == 0;
    case P4_1:
        return (post & 0xF) == 1;
    default:
        return true;
    }
}

Error TableZ8::searchOpCode(
        InsnZ8 &insn, DisMemory &memory, const EntryPage *pages, const EntryPage *end) const {
    for (auto page = pages; page < end; page++) {
        auto end = page->end();
        for (auto entry = page->table();
                entry < end && (entry = TableBase::searchCode<Entry, Config::opcode_t>(
                                        insn.opCode(), entry, end, maskCode)) != nullptr;
                entry++) {
            insn.setFlags(entry->flags());
            if (insn.postFormat()) {
                if (insn.length() < 2) {
                    insn.readPost(memory);
                    if (insn.getError())
                        return NO_MEMORY;
                }
                if (!matchPostByte(insn))
                    continue;
            }
            insn.setName_P(entry->name_P());
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableZ8::searchName(InsnZ8 &insn) const {
    return setError(searchName(insn, _table, _end));
}

Error TableZ8::searchOpCode(InsnZ8 &insn, DisMemory &memory) const {
    return setError(searchOpCode(insn, memory, _table, _end));
}

class CpuTable : public CpuTableBase<CpuType, TableZ8::EntryPage> {
public:
    constexpr CpuTable(CpuType cpuType, const char *name, const TableZ8::EntryPage *table,
            const TableZ8::EntryPage *end)
        : CpuTableBase(cpuType, name, table, end) {}
};

static constexpr CpuTable CPU_TABLES[] PROGMEM = {
        {Z8, TEXT_CPU_Z8, ARRAY_RANGE(Z8_PAGES)},
        {Z86C, TEXT_CPU_Z86C, ARRAY_RANGE(Z86C_PAGES)},
        {SUPER8, TEXT_CPU_Z88, ARRAY_RANGE(SUPER8_PAGES)},
};

TableZ8::TableZ8() {
    setCpu(Z8);
}

bool TableZ8::isSuper8() const {
    return _cpuType == SUPER8;
}

bool TableZ8::setCpu(CpuType cpuType) {
    auto t = CpuTable::search(cpuType, ARRAY_RANGE(CPU_TABLES));
    if (t == nullptr)
        return false;
    _cpuType = cpuType;
    _table = t->table();
    _end = t->end();
    return true;
}

const /* PROGMEM */ char *TableZ8::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /* PROGMEM */ char *TableZ8::cpu_P() const {
    return CpuTable::search(_cpuType, ARRAY_RANGE(CPU_TABLES))->name_P();
}

bool TableZ8::setCpu(const char *cpu) {
    auto t = CpuTable::search(cpu, ARRAY_RANGE(CPU_TABLES));
    if (t)
        return setCpu(t->cpuType());
    if (strncasecmp_P(cpu, TEXT_CPU_Z86C, 4) == 0)
        return setCpu(Z86C);
    if (strncasecmp_P(cpu, TEXT_CPU_Z86, 3) == 0)
        return setCpu(Z8);
    if (strncasecmp_P(cpu, TEXT_CPU_Z88, 3) == 0 || strcasecmp_P(cpu, TEXT_CPU_SUPER8) == 0)
        return setCpu(SUPER8);
    return false;
}

class TableZ8 TableZ8;

}  // namespace z8
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
