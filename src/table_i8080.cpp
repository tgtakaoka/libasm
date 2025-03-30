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

#include "table_i8080.h"
#include "entry_i8080.h"
#include "entry_table.h"
#include "text_i8080.h"

using namespace libasm::text::i8080;

namespace libasm {
namespace i8080 {

#define E2(_opc, _cf, _name, _dst, _src) {_opc, Entry::Flags::create(_cf, _dst, _src), _name}
#define E1(_opc, _cf, _name, _dst) E2(_opc, _cf, _name, _dst, M_NONE)
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE)
#define U2(_opc, _cf, _name, _dst, _src) {_opc, Entry::Flags::undef(_cf, _dst, _src), _name}

// clang-format off
constexpr Entry TABLE_I8080[] PROGMEM = {
    E0(0x00, CF_00, TEXT_NOP),
    E2(0x01, CF_30, TEXT_LXI,  M_PTR,  M_IM16),
    E1(0x09, CF_30, TEXT_DAD,  M_PTR),
    E1(0x02, CF_10, TEXT_STAX, M_IDX),
    E1(0x22, CF_00, TEXT_SHLD, M_ABS),
    E1(0x32, CF_00, TEXT_STA,  M_ABS),
    E1(0x0A, CF_10, TEXT_LDAX, M_IDX),
    E1(0x2A, CF_00, TEXT_LHLD, M_ABS),
    E1(0x3A, CF_00, TEXT_LDA,  M_ABS),
    E1(0x03, CF_30, TEXT_INX,  M_PTR),
    E1(0x0B, CF_30, TEXT_DCX,  M_PTR),
    E1(0x04, CF_38, TEXT_INR,  M_DST),
    E1(0x05, CF_38, TEXT_DCR,  M_DST),
    E2(0x06, CF_38, TEXT_MVI,  M_DST,  M_IM8),
    E0(0x07, CF_00, TEXT_RLC),
    E0(0x0F, CF_00, TEXT_RRC),
    E0(0x17, CF_00, TEXT_RAL),
    E0(0x1F, CF_00, TEXT_RAR),
    E0(0x27, CF_00, TEXT_DAA),
    E0(0x2F, CF_00, TEXT_CMA),
    E0(0x37, CF_00, TEXT_STC),
    E0(0x3F, CF_00, TEXT_CMC),
    E0(0x76, CF_00, TEXT_HLT),
    E2(0x40, CF_3F, TEXT_MOV,  M_DST,  M_SRC),
    E1(0x80, CF_07, TEXT_ADD,  M_SRC),
    E1(0x88, CF_07, TEXT_ADC,  M_SRC),
    E1(0x90, CF_07, TEXT_SUB,  M_SRC),
    E1(0x98, CF_07, TEXT_SBB,  M_SRC),
    E1(0xA0, CF_07, TEXT_ANA,  M_SRC),
    E1(0xA8, CF_07, TEXT_XRA,  M_SRC),
    E1(0xB0, CF_07, TEXT_ORA,  M_SRC),
    E1(0xB8, CF_07, TEXT_CMP,  M_SRC),
    E0(0xC0, CF_00, TEXT_RNZ),
    E0(0xC8, CF_00, TEXT_RZ),
    E0(0xD0, CF_00, TEXT_RNC),
    E0(0xD8, CF_00, TEXT_RC),
    E0(0xE0, CF_00, TEXT_RPO),
    E0(0xE8, CF_00, TEXT_RPE),
    E0(0xF0, CF_00, TEXT_RP),
    E0(0xF8, CF_00, TEXT_RM),
    E1(0xC1, CF_30, TEXT_POP,  M_STK),
    E0(0xC9, CF_00, TEXT_RET),
    E0(0xE9, CF_00, TEXT_PCHL),
    E0(0xF9, CF_00, TEXT_SPHL),
    E1(0xC2, CF_00, TEXT_JNZ,  M_ABS),
    E1(0xCA, CF_00, TEXT_JZ,   M_ABS),
    E1(0xD2, CF_00, TEXT_JNC,  M_ABS),
    E1(0xDA, CF_00, TEXT_JC,   M_ABS),
    E1(0xE2, CF_00, TEXT_JPO,  M_ABS),
    E1(0xEA, CF_00, TEXT_JPE,  M_ABS),
    E1(0xF2, CF_00, TEXT_JP,   M_ABS),
    E1(0xFA, CF_00, TEXT_JM,   M_ABS),
    E1(0xC3, CF_00, TEXT_JMP,  M_ABS),
    E1(0xD3, CF_00, TEXT_OUT,  M_IOA),
    E1(0xDB, CF_00, TEXT_IN,   M_IOA),
    E0(0xE3, CF_00, TEXT_XTHL),
    E0(0xEB, CF_00, TEXT_XCHG),
    E0(0xF3, CF_00, TEXT_DI),
    E0(0xFB, CF_00, TEXT_EI),
    E1(0xC4, CF_00, TEXT_CNZ,  M_ABS),
    E1(0xCC, CF_00, TEXT_CZ,   M_ABS),
    E1(0xD4, CF_00, TEXT_CNC,  M_ABS),
    E1(0xDC, CF_00, TEXT_CC,   M_ABS),
    E1(0xE4, CF_00, TEXT_CPO,  M_ABS),
    E1(0xEC, CF_00, TEXT_CPE,  M_ABS),
    E1(0xF4, CF_00, TEXT_CP,   M_ABS),
    E1(0xFC, CF_00, TEXT_CM,   M_ABS),
    E1(0xC5, CF_30, TEXT_PUSH, M_STK),
    E1(0xCD, CF_00, TEXT_CALL, M_ABS),
    E1(0xC6, CF_00, TEXT_ADI,  M_IM8),
    E1(0xCE, CF_00, TEXT_ACI,  M_IM8),
    E1(0xD6, CF_00, TEXT_SUI,  M_IM8),
    E1(0xDE, CF_00, TEXT_SBI,  M_IM8),
    E1(0xE6, CF_00, TEXT_ANI,  M_IM8),
    E1(0xEE, CF_00, TEXT_XRI,  M_IM8),
    E1(0xF6, CF_00, TEXT_ORI,  M_IM8),
    E1(0xFE, CF_00, TEXT_CPI,  M_IM8),
    E1(0xC7, CF_38, TEXT_RST,  M_VEC),
};

constexpr uint8_t INDEX_I8080[] PROGMEM = {
     70,  // TEXT_ACI
     25,  // TEXT_ADC
     24,  // TEXT_ADD
     69,  // TEXT_ADI
     28,  // TEXT_ANA
     73,  // TEXT_ANI
     68,  // TEXT_CALL
     62,  // TEXT_CC
     66,  // TEXT_CM
     19,  // TEXT_CMA
     21,  // TEXT_CMC
     31,  // TEXT_CMP
     61,  // TEXT_CNC
     59,  // TEXT_CNZ
     65,  // TEXT_CP
     64,  // TEXT_CPE
     76,  // TEXT_CPI
     63,  // TEXT_CPO
     60,  // TEXT_CZ
     18,  // TEXT_DAA
      2,  // TEXT_DAD
     12,  // TEXT_DCR
     10,  // TEXT_DCX
     57,  // TEXT_DI
     58,  // TEXT_EI
     22,  // TEXT_HLT
     54,  // TEXT_IN
     11,  // TEXT_INR
      9,  // TEXT_INX
     47,  // TEXT_JC
     51,  // TEXT_JM
     52,  // TEXT_JMP
     46,  // TEXT_JNC
     44,  // TEXT_JNZ
     50,  // TEXT_JP
     49,  // TEXT_JPE
     48,  // TEXT_JPO
     45,  // TEXT_JZ
      8,  // TEXT_LDA
      6,  // TEXT_LDAX
      7,  // TEXT_LHLD
      1,  // TEXT_LXI
     23,  // TEXT_MOV
     13,  // TEXT_MVI
      0,  // TEXT_NOP
     30,  // TEXT_ORA
     75,  // TEXT_ORI
     53,  // TEXT_OUT
     42,  // TEXT_PCHL
     40,  // TEXT_POP
     67,  // TEXT_PUSH
     16,  // TEXT_RAL
     17,  // TEXT_RAR
     35,  // TEXT_RC
     41,  // TEXT_RET
     14,  // TEXT_RLC
     39,  // TEXT_RM
     34,  // TEXT_RNC
     32,  // TEXT_RNZ
     38,  // TEXT_RP
     37,  // TEXT_RPE
     36,  // TEXT_RPO
     15,  // TEXT_RRC
     77,  // TEXT_RST
     33,  // TEXT_RZ
     27,  // TEXT_SBB
     72,  // TEXT_SBI
      4,  // TEXT_SHLD
     43,  // TEXT_SPHL
      5,  // TEXT_STA
      3,  // TEXT_STAX
     20,  // TEXT_STC
     26,  // TEXT_SUB
     71,  // TEXT_SUI
     56,  // TEXT_XCHG
     29,  // TEXT_XRA
     74,  // TEXT_XRI
     55,  // TEXT_XTHL
};

constexpr Entry TABLE_Z8080[] PROGMEM = {
    E0(0x00, CF_00, TEXT_NOP),
    E2(0x01, CF_30, TEXT_LD,   M_PTR, M_IM16),
    E2(0x09, CF_30, TEXT_ADD,  R_HL,  M_PTR),
    E2(0x02, CF_10, TEXT_LD,   M_IDX, R_A),
    E2(0x0A, CF_10, TEXT_LD,   R_A,   M_IDX),
    E2(0x22, CF_00, TEXT_LD,   M_ABS, R_HL),
    E2(0x2A, CF_00, TEXT_LD,   R_HL,  M_ABS),
    E2(0x32, CF_00, TEXT_LD,   M_ABS, R_A),
    E2(0x3A, CF_00, TEXT_LD,   R_A,   M_ABS),
    E1(0x03, CF_30, TEXT_INC,  M_PTR),
    E1(0x0B, CF_30, TEXT_DEC,  M_PTR),
    E1(0x04, CF_38, TEXT_INC,  M_DST),
    E1(0x05, CF_38, TEXT_DEC,  M_DST),
    E2(0x06, CF_38, TEXT_LD,   M_DST, M_IM8),
    E0(0x07, CF_00, TEXT_RLCA),
    E0(0x0F, CF_00, TEXT_RRCA),
    E0(0x17, CF_00, TEXT_RLA),
    E0(0x1F, CF_00, TEXT_RRA),
    E0(0x27, CF_00, TEXT_DAA),
    E0(0x2F, CF_00, TEXT_CPL),
    E0(0x37, CF_00, TEXT_SCF),
    E0(0x3F, CF_00, TEXT_CCF),
    E0(0x76, CF_00, TEXT_HALT),
    U2(0x76, CF_00, TEXT_LD,   I_HL,  I_HL),
    E2(0x40, CF_3F, TEXT_LD,   M_DST, M_SRC),
    E2(0x80, CF_07, TEXT_ADD,  R_A,   M_SRC),
    E2(0x88, CF_07, TEXT_ADC,  R_A,   M_SRC),
    E2(0x90, CF_07, TEXT_SUB,  R_A,   M_SRC),
    E1(0x90, CF_07, TEXT_SUB,  M_SRC),
    E2(0x98, CF_07, TEXT_SBC,  R_A,   M_SRC),
    E2(0xA0, CF_07, TEXT_AND,  R_A,   M_SRC),
    E1(0xA0, CF_07, TEXT_AND,  M_SRC),
    E2(0xA8, CF_07, TEXT_XOR,  R_A,   M_SRC),
    E1(0xA8, CF_07, TEXT_XOR,  M_SRC),
    E2(0xB0, CF_07, TEXT_OR,   R_A,   M_SRC),
    E1(0xB0, CF_07, TEXT_OR,   M_SRC),
    E2(0xB8, CF_07, TEXT_CP,   R_A,   M_SRC),
    E1(0xB8, CF_07, TEXT_CP,   M_SRC),
    E1(0xC0, CF_38, TEXT_RET,  M_CC),
    E1(0xC1, CF_30, TEXT_POP,  M_STK),
    E0(0xC9, CF_00, TEXT_RET),
    E2(0xF9, CF_00, TEXT_LD,   R_SP,  R_HL),
    E2(0xC2, CF_38, TEXT_JP,   M_CC,  M_IM16),
    E1(0xC3, CF_00, TEXT_JP,   M_IM16),
    E1(0xE9, CF_00, TEXT_JP,   I_HL),
    E2(0xD3, CF_00, TEXT_OUT,  M_IOA, R_A),
    E2(0xDB, CF_00, TEXT_IN,   R_A,   M_IOA),
    E2(0xE3, CF_00, TEXT_EX,   I_SP,  R_HL),
    E2(0xE3, CF_00, TEXT_EX,   R_HL,  I_SP),
    E2(0xEB, CF_00, TEXT_EX,   R_DE,  R_HL),
    E2(0xEB, CF_00, TEXT_EX,   R_HL,  R_DE),
    E0(0xF3, CF_00, TEXT_DI),
    E0(0xFB, CF_00, TEXT_EI),
    E2(0xC4, CF_38, TEXT_CALL, M_CC,  M_IM16),
    E1(0xC5, CF_30, TEXT_PUSH, M_STK),
    E1(0xCD, CF_00, TEXT_CALL, M_IM16),
    E2(0xC6, CF_00, TEXT_ADD,  R_A,   M_IM8),
    E2(0xCE, CF_00, TEXT_ADC,  R_A,   M_IM8),
    E2(0xD6, CF_00, TEXT_SUB,  R_A,   M_IM8),
    E1(0xD6, CF_00, TEXT_SUB,  M_IM8),
    E2(0xDE, CF_00, TEXT_SBC,  R_A,   M_IM8),
    E2(0xE6, CF_00, TEXT_AND,  R_A,   M_IM8),
    E1(0xE6, CF_00, TEXT_AND,  M_IM8),
    E2(0xEE, CF_00, TEXT_XOR,  R_A,   M_IM8),
    E1(0xEE, CF_00, TEXT_XOR,  M_IM8),
    E2(0xF6, CF_00, TEXT_OR,   R_A,   M_IM8),
    E1(0xF6, CF_00, TEXT_OR,   M_IM8),
    E2(0xFE, CF_00, TEXT_CP,   R_A,   M_IM8),
    E1(0xFE, CF_00, TEXT_CP,   M_IM8),
    E1(0xC7, CF_38, TEXT_RST,  M_VEC),
};

constexpr uint8_t INDEX_Z8080[] PROGMEM = {
     26,  // TEXT_ADC
     57,  // TEXT_ADC
      2,  // TEXT_ADD
     25,  // TEXT_ADD
     56,  // TEXT_ADD
     30,  // TEXT_AND
     31,  // TEXT_AND
     61,  // TEXT_AND
     62,  // TEXT_AND
     53,  // TEXT_CALL
     55,  // TEXT_CALL
     21,  // TEXT_CCF
     36,  // TEXT_CP
     37,  // TEXT_CP
     67,  // TEXT_CP
     68,  // TEXT_CP
     19,  // TEXT_CPL
     18,  // TEXT_DAA
     10,  // TEXT_DEC
     12,  // TEXT_DEC
     51,  // TEXT_DI
     52,  // TEXT_EI
     47,  // TEXT_EX
     48,  // TEXT_EX
     49,  // TEXT_EX
     50,  // TEXT_EX
     22,  // TEXT_HALT
     46,  // TEXT_IN
      9,  // TEXT_INC
     11,  // TEXT_INC
     42,  // TEXT_JP
     43,  // TEXT_JP
     44,  // TEXT_JP
      1,  // TEXT_LD
      3,  // TEXT_LD
      4,  // TEXT_LD
      5,  // TEXT_LD
      6,  // TEXT_LD
      7,  // TEXT_LD
      8,  // TEXT_LD
     13,  // TEXT_LD
     23,  // TEXT_LD
     24,  // TEXT_LD
     41,  // TEXT_LD
      0,  // TEXT_NOP
     34,  // TEXT_OR
     35,  // TEXT_OR
     65,  // TEXT_OR
     66,  // TEXT_OR
     45,  // TEXT_OUT
     39,  // TEXT_POP
     54,  // TEXT_PUSH
     38,  // TEXT_RET
     40,  // TEXT_RET
     16,  // TEXT_RLA
     14,  // TEXT_RLCA
     17,  // TEXT_RRA
     15,  // TEXT_RRCA
     69,  // TEXT_RST
     29,  // TEXT_SBC
     60,  // TEXT_SBC
     20,  // TEXT_SCF
     27,  // TEXT_SUB
     28,  // TEXT_SUB
     58,  // TEXT_SUB
     59,  // TEXT_SUB
     32,  // TEXT_XOR
     33,  // TEXT_XOR
     63,  // TEXT_XOR
     64,  // TEXT_XOR
};

constexpr Entry TABLE_I8085[] PROGMEM = {
    E0(0x20, CF_00, TEXT_RIM),
    E0(0x30, CF_00, TEXT_SIM),
};

constexpr uint8_t INDEX_I8085[] PROGMEM = {
      0,  // TEXT_RIM
      1,  // TEXT_SIM
};

constexpr Entry TABLE_Z8085[] PROGMEM = {
    E2(0x20, CF_00, TEXT_LD, R_A,  R_IM),
    E2(0x30, CF_00, TEXT_LD, R_IM, R_A),
};

constexpr uint8_t INDEX_Z8085[] PROGMEM = {
      0,  // TEXT_LD
      1,  // TEXT_LD
};

constexpr Entry TABLE_V30EMU[] PROGMEM = {
    E1(0xED, CF_00, TEXT_CALLN, M_IM8),
    E0(0xFD, CF_00, TEXT_RETEM),
};

constexpr uint8_t INDEX_V30EMU[] PROGMEM = {
      0,  // TEXT_CALLN
      1,  // TEXT_RETEM
};

// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage I8080_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_I8080), ARRAY_RANGE(INDEX_I8080)},
};

constexpr EntryPage Z8080_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_Z8080), ARRAY_RANGE(INDEX_Z8080)},
};

constexpr EntryPage I8085_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_I8080), ARRAY_RANGE(INDEX_I8080)},
        {0x00, ARRAY_RANGE(TABLE_I8085), ARRAY_RANGE(INDEX_I8085)},
};

constexpr EntryPage Z8085_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_Z8080), ARRAY_RANGE(INDEX_Z8080)},
        {0x00, ARRAY_RANGE(TABLE_Z8085), ARRAY_RANGE(INDEX_Z8085)},
};

constexpr EntryPage V30EMU_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_I8080), ARRAY_RANGE(INDEX_I8080)},
        {0xED, ARRAY_RANGE(TABLE_V30EMU), ARRAY_RANGE(INDEX_V30EMU)},
};

constexpr EntryPage Z30EMU_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_Z8080), ARRAY_RANGE(INDEX_Z8080)},
        {0xED, ARRAY_RANGE(TABLE_V30EMU), ARRAY_RANGE(INDEX_V30EMU)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu INTEL_TABLE[] PROGMEM = {
        {I8080, TEXT_CPU_8080, ARRAY_RANGE(I8080_PAGES)},
        {I8085, TEXT_CPU_8085, ARRAY_RANGE(I8085_PAGES)},
        {V30EMU, TEXT_CPU_V30EMU, ARRAY_RANGE(V30EMU_PAGES)},
};

constexpr Cpu ZILOG_TABLE[] PROGMEM = {
        {I8080, TEXT_CPU_8080, ARRAY_RANGE(Z8080_PAGES)},
        {I8085, TEXT_CPU_8085, ARRAY_RANGE(Z8085_PAGES)},
        {V30EMU, TEXT_CPU_V30EMU, ARRAY_RANGE(Z30EMU_PAGES)},
};

const Cpu *cpu(CpuType cpuType, bool zilog) {
    return zilog ? Cpu::search(cpuType, ARRAY_RANGE(ZILOG_TABLE))
                 : Cpu::search(cpuType, ARRAY_RANGE(INTEL_TABLE));
}

bool acceptIntelMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_SRC)
        return table == M_DST;
    if (opr == M_IDX || opr == R_H)
        return table == M_PTR || table == M_STK || table == M_SRC || table == M_DST;
    if (opr == M_IM16)
        return table == M_IM8 || table == M_ABS || table == M_IOA || table == M_VEC;
    return false;
}

bool acceptIntelModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptIntelMode(insn.dstOp.mode, table.dst()) &&
           acceptIntelMode(insn.srcOp.mode, table.src());
}

bool acceptZilogMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_SRC || opr == R_A)
        return table == M_DST || table == M_SRC || table == M_DST || table == M_SRC;
    if (opr == I_HL)
        return table == M_SRC || table == M_DST;
    if (opr == R_C)
        return table == M_SRC || table == M_DST || table == M_CC;
    if (opr == R_BC || opr == R_DE)
        return table == M_PTR || table == M_STK;
    if (opr == R_HL)
        return table == M_PTR || table == M_STK;
    if (opr == R_SP)
        return table == M_PTR;
    if (opr == R_AF)
        return table == M_STK;
    if (opr == M_IM16)
        return table == M_IM8 || table == M_VEC;
    if (opr == M_ABS)
        return table == M_IOA;
    return false;
}

bool acceptZilogModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    if (acceptZilogMode(insn.dstOp.mode, table.dst()) &&
            acceptZilogMode(insn.srcOp.mode, table.src())) {
        if (table.undefined())
            insn.setErrorIf(OPERAND_NOT_ALLOWED);
        return true;
    }
    return false;
}

Error searchName(CpuType cpuType, AsmInsn &insn, bool zilog) {
    auto acceptModes = zilog ? acceptZilogModes : acceptIntelModes;
    cpu(cpuType, zilog)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    opc &= ~entry->readFlags().mask();
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out, bool zilog) {
    cpu(cpuType, zilog)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

bool isPrefix(CpuType cpuType, Config::opcode_t code) {
    return cpuType == V30EMU && code == 0xED;
}

const /*PROGMEM*/ char *TableI8080::listCpu_P() const {
    return TEXT_I8080_LIST;
}

const /*PROGMEM*/ char *TableI8080::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType, false)->name_P();
}

Error TableI8080::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto t = Cpu::search(name, ARRAY_RANGE(INTEL_TABLE));
    if (t) {
        cpuType = t->readCpuType();
    } else {
        name.iexpect('i');
        if (name.iequals(TEXT_CPU_8080)) {
            cpuType = I8080;
        } else if (name.iequals(TEXT_CPU_8085)) {
            cpuType = I8085;
        } else {
            return UNSUPPORTED_CPU;
        }
    }
    return OK;
}

const TableI8080 TABLE;

}  // namespace i8080
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
