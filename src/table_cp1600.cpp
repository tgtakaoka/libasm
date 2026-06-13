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

#include "table_cp1600.h"
#include "entry_cp1600.h"
#include "entry_table.h"
#include "text_cp1600.h"

using namespace libasm::text::cp1600;

namespace libasm {
namespace cp1600 {

#define E2(_opc, _name, _src, _dst) {_opc, Entry::Flags::create(_src, _dst), _name}
#define E1(_opc, _name, _src) E2(_opc, _name, _src, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
constexpr Entry TABLE_CP1600[] PROGMEM = {
    E1(00200, TEXT_TSTR, M_XREG),
    E1(00700, TEXT_CLRR, M_XREG),
    E1(00207, TEXT_JR,   M_SREG),
    E2(00200, TEXT_MOVR, M_SREG, M_DREG),
    E2(00300, TEXT_ADDR, M_SREG, M_DREG),
    E2(00400, TEXT_SUBR, M_SREG, M_DREG),
    E2(00500, TEXT_CMPR, M_SREG, M_DREG),
    E2(00600, TEXT_ANDR, M_SREG, M_DREG),
    E2(00700, TEXT_XORR, M_SREG, M_DREG),
    E1(00010, TEXT_INCR, M_DREG),
    E1(00020, TEXT_DECR, M_DREG),
    E1(00030, TEXT_COMR, M_DREG),
    E1(00040, TEXT_NEGR, M_DREG),
    E1(00050, TEXT_ADCR, M_DREG),
    E1(00060, TEXT_GSWD, M_LREG),
    E1(00064, TEXT_NOP,  M_BIT0),
    E1(00066, TEXT_SIN,  M_BIT0),
    E1(00070, TEXT_RSWD, M_DREG),
    E1(00100, TEXT_SWAP, M_LREG),
    E2(00100, TEXT_SWAP, M_LREG, M_SHCNT),
    E1(00110, TEXT_SLL,  M_LREG),
    E2(00110, TEXT_SLL,  M_LREG, M_SHCNT),
    E1(00120, TEXT_RLC,  M_LREG),
    E2(00120, TEXT_RLC,  M_LREG, M_SHCNT),
    E1(00130, TEXT_SLLC, M_LREG),
    E2(00130, TEXT_SLLC, M_LREG, M_SHCNT),
    E1(00140, TEXT_SLR,  M_LREG),
    E2(00140, TEXT_SLR,  M_LREG, M_SHCNT),
    E1(00150, TEXT_SAR,  M_LREG),
    E2(00150, TEXT_SAR,  M_LREG, M_SHCNT),
    E1(00160, TEXT_RRC,  M_LREG),
    E2(00160, TEXT_RRC,  M_LREG, M_SHCNT),
    E1(00170, TEXT_SARC, M_LREG),
    E2(00170, TEXT_SARC, M_LREG, M_SHCNT),
    E2(01100, TEXT_MVO,   M_DREG, M_DADDR),
    E1(01160, TEXT_PSHR,  M_DREG),
    E2(01170, TEXT_MVOI,  M_DREG, M_IMM16),
    E2(01100, TEXT_MVOAT, M_DREG, M_INDIR),
    E2(01200, TEXT_MVI,   M_DADDR, M_DREG),
    E1(01260, TEXT_PULR,  M_DREG),
    E2(01270, TEXT_MVII,  M_IMM16, M_DREG),
    E2(01200, TEXT_MVIAT, M_INDIR, M_DREG),
    E2(01300, TEXT_ADD,   M_DADDR, M_DREG),
    E2(01370, TEXT_ADDI,  M_IMM16, M_DREG),
    E2(01360, TEXT_ADDAT, M_STACK, M_DREG),
    E2(01300, TEXT_ADDAT, M_INDIR, M_DREG),
    E2(01400, TEXT_SUB,   M_DADDR, M_DREG),
    E2(01470, TEXT_SUBI,  M_IMM16, M_DREG),
    E2(01460, TEXT_SUBAT, M_STACK, M_DREG),
    E2(01400, TEXT_SUBAT, M_INDIR, M_DREG),
    E2(01500, TEXT_CMP,   M_DADDR, M_DREG),
    E2(01570, TEXT_CMPI,  M_IMM16, M_DREG),
    E2(01560, TEXT_CMPAT, M_STACK, M_DREG),
    E2(01500, TEXT_CMPAT, M_INDIR, M_DREG),
    E2(01600, TEXT_AND,   M_DADDR, M_DREG),
    E2(01670, TEXT_ANDI,  M_IMM16, M_DREG),
    E2(01660, TEXT_ANDAT, M_STACK, M_DREG),
    E2(01600, TEXT_ANDAT, M_INDIR, M_DREG),
    E2(01700, TEXT_XOR,   M_DADDR, M_DREG),
    E2(01770, TEXT_XORI,  M_IMM16, M_DREG),
    E2(01760, TEXT_XORAT, M_STACK, M_DREG),
    E2(01700, TEXT_XORAT, M_INDIR, M_DREG),
    E1(01000, TEXT_B,     M_BDISP),
    E1(01010, TEXT_NOPP,  M_NOPP),
    E1(01001, TEXT_BC,    M_BDISP),
    E1(01011, TEXT_BNC,   M_BDISP),
    E1(01001, TEXT_BLGE,  M_BDISP),
    E1(01011, TEXT_BLLT,  M_BDISP),
    E1(01002, TEXT_BOV,   M_BDISP),
    E1(01012, TEXT_BNOV,  M_BDISP),
    E1(01003, TEXT_BPL,   M_BDISP),
    E1(01013, TEXT_BMI,   M_BDISP),
    E1(01004, TEXT_BZE,   M_BDISP),
    E1(01014, TEXT_BNZE,  M_BDISP),
    E1(01004, TEXT_BEQ,   M_BDISP),
    E1(01014, TEXT_BNEQ,  M_BDISP),
    E1(01005, TEXT_BLT,   M_BDISP),
    E1(01015, TEXT_BGE,   M_BDISP),
    E1(01006, TEXT_BLE,   M_BDISP),
    E1(01016, TEXT_BGT,   M_BDISP),
    E1(01007, TEXT_BUSC,  M_BDISP),
    E1(01017, TEXT_BESC,  M_BDISP),
    E2(01020, TEXT_BEXT,  M_BDISP, M_BCOND),
    E0(00000, TEXT_HLT),
    E0(00001, TEXT_SDBD),
    E0(00002, TEXT_EIS),
    E0(00003, TEXT_DIS),
    E0(00005, TEXT_TCI),
    E0(00006, TEXT_CLRC),
    E0(00007, TEXT_SETC),
};

static constexpr uint8_t INDEX_CP1600[] PROGMEM = {
     13,  // TEXT_ADCR
     42,  // TEXT_ADD
     44,  // TEXT_ADDAT
     45,  // TEXT_ADDAT
     43,  // TEXT_ADDI
      4,  // TEXT_ADDR
     54,  // TEXT_AND
     56,  // TEXT_ANDAT
     57,  // TEXT_ANDAT
     55,  // TEXT_ANDI
      7,  // TEXT_ANDR
     62,  // TEXT_B
     64,  // TEXT_BC
     74,  // TEXT_BEQ
     81,  // TEXT_BESC
     82,  // TEXT_BEXT
     77,  // TEXT_BGE
     79,  // TEXT_BGT
     78,  // TEXT_BLE
     66,  // TEXT_BLGE
     67,  // TEXT_BLLT
     76,  // TEXT_BLT
     71,  // TEXT_BMI
     65,  // TEXT_BNC
     75,  // TEXT_BNEQ
     69,  // TEXT_BNOV
     73,  // TEXT_BNZE
     68,  // TEXT_BOV
     70,  // TEXT_BPL
     80,  // TEXT_BUSC
     72,  // TEXT_BZE
     88,  // TEXT_CLRC
      1,  // TEXT_CLRR
     50,  // TEXT_CMP
     52,  // TEXT_CMPAT
     53,  // TEXT_CMPAT
     51,  // TEXT_CMPI
      6,  // TEXT_CMPR
     11,  // TEXT_COMR
     10,  // TEXT_DECR
     86,  // TEXT_DIS
     85,  // TEXT_EIS
     14,  // TEXT_GSWD
     83,  // TEXT_HLT
      9,  // TEXT_INCR
      2,  // TEXT_JR
      3,  // TEXT_MOVR
     38,  // TEXT_MVI
     41,  // TEXT_MVIAT
     40,  // TEXT_MVII
     34,  // TEXT_MVO
     37,  // TEXT_MVOAT
     36,  // TEXT_MVOI
     12,  // TEXT_NEGR
     15,  // TEXT_NOP
     63,  // TEXT_NOPP
     35,  // TEXT_PSHR
     39,  // TEXT_PULR
     22,  // TEXT_RLC
     23,  // TEXT_RLC
     30,  // TEXT_RRC
     31,  // TEXT_RRC
     17,  // TEXT_RSWD
     28,  // TEXT_SAR
     29,  // TEXT_SAR
     32,  // TEXT_SARC
     33,  // TEXT_SARC
     84,  // TEXT_SDBD
     89,  // TEXT_SETC
     16,  // TEXT_SIN
     20,  // TEXT_SLL
     21,  // TEXT_SLL
     24,  // TEXT_SLLC
     25,  // TEXT_SLLC
     26,  // TEXT_SLR
     27,  // TEXT_SLR
     46,  // TEXT_SUB
     48,  // TEXT_SUBAT
     49,  // TEXT_SUBAT
     47,  // TEXT_SUBI
      5,  // TEXT_SUBR
     18,  // TEXT_SWAP
     19,  // TEXT_SWAP
     87,  // TEXT_TCI
      0,  // TEXT_TSTR
     58,  // TEXT_XOR
     60,  // TEXT_XORAT
     61,  // TEXT_XORAT
     59,  // TEXT_XORI
      8,  // TEXT_XORR
};

constexpr Entry TABLE_JUMP[] = {
    E1(01400, TEXT_J,    M_JADDR),
    E1(01401, TEXT_JE,   M_JADDR),
    E1(01402, TEXT_JD,   M_JADDR),
    E2(00000, TEXT_JSR,  M_BREG, M_JADDR),
    E2(00001, TEXT_JSRE, M_BREG, M_JADDR),
    E2(00002, TEXT_JSRD, M_BREG, M_JADDR),
};

static constexpr uint8_t INDEX_JUMP[] PROGMEM = {
      0,  // TEXT_J
      2,  // TEXT_JD
      1,  // TEXT_JE
      3,  // TEXT_JSR
      5,  // TEXT_JSRD
      4,  // TEXT_JSRE
};

// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage CP1600_PAGES[] PROGMEM = {
        {0x0000, ARRAY_RANGE(TABLE_CP1600), ARRAY_RANGE(INDEX_CP1600)},
        {0x0004, ARRAY_RANGE(TABLE_JUMP), ARRAY_RANGE(INDEX_JUMP)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {CP1600, TEXT_CPU_CP1600, ARRAY_RANGE(CP1600_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_REG)
        return table == M_DREG || table == M_SREG || table == M_LREG ||
               table == M_BREG || table == M_INDIR || table == M_XREG;
    if (opr == M_DADDR)
        return table == M_IMM16 || table == M_BDISP || table == M_JADDR ||
               table == M_BCOND || table == M_SHCNT || table == M_BIT0;
    if (opr == M_NONE)
        return table == M_SHCNT || table == M_BIT0 || table == M_NOPP;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto flags = entry->readFlags();
    return acceptMode(insn.srcOp.mode, flags.src()) &&
           acceptMode(insn.dstOp.mode, flags.dst());
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    if (flags.src() == M_XREG) {
        const auto sreg = (opc >> 3) & 7;
        const auto dreg = opc & 7;
        if (sreg != dreg)
            return false;
    }
    opc &= ~flags.opcodeMask();
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

bool isPrefix(CpuType cpuType, Config::opcode_t code) {
    return cpu(cpuType)->isPrefix(code);
}

const /*PROGMEM*/ char *TableCp1600::listCpu_P() const {
    return TEXT_CP1600_LIST;
}

const /*PROGMEM*/ char *TableCp1600::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableCp1600::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto p = name;
    if (p.iexpectText_P(TEXT_CPU_CP1600) && p.size() == 0) {
        name = p;
        cpuType = CP1600;
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableCp1600 TABLE;

}  // namespace cp1600
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
