/*
 * Copyright 2024 Tadashi G. Takaoka
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

#include "table_pdp11.h"

#include "entry_pdp11.h"
#include "entry_table.h"
#include "text_pdp11.h"

using namespace libasm::text::pdp11;

namespace libasm {
namespace pdp11 {

#define E2(_opc, _size, _name, _src, _spos, _dst, _dpos) \
    { _opc, Entry::Flags::create(_size, _src, _spos, _dst, _dpos), _name }
#define E1(_opc, _size, _name, _src, _spos) E2(_opc, _size, _name, _src, _spos, M_NONE, P_NONE)
#define E0(_opc, _size, _name) E1(_opc, _size, _name, M_NONE, P_NONE)

// clang-format off
static constexpr Entry TABLE_BASE[] PROGMEM = {
    E2(0010000, SZ_WORD, TEXT_MOV,   M_GENS, P_7700, M_GEND, P_0077),
    E2(0110000, SZ_BYTE, TEXT_MOVB,  M_GENS, P_7700, M_GEND, P_0077),
    E2(0020000, SZ_WORD, TEXT_CMP,   M_GENS, P_7700, M_GENS, P_0077),
    E2(0120000, SZ_BYTE, TEXT_CMPB,  M_GENS, P_7700, M_GENS, P_0077),
    E2(0030000, SZ_WORD, TEXT_BIT,   M_GENS, P_7700, M_GEND, P_0077),
    E2(0130000, SZ_BYTE, TEXT_BITB,  M_GENS, P_7700, M_GEND, P_0077),
    E2(0040000, SZ_WORD, TEXT_BIC,   M_GENS, P_7700, M_GEND, P_0077),
    E2(0140000, SZ_BYTE, TEXT_BICB,  M_GENS, P_7700, M_GEND, P_0077),
    E2(0050000, SZ_WORD, TEXT_BIS,   M_GENS, P_7700, M_GEND, P_0077),
    E2(0150000, SZ_BYTE, TEXT_BISB,  M_GENS, P_7700, M_GEND, P_0077),
    E2(0060000, SZ_INTG, TEXT_ADD,   M_GENS, P_7700, M_GEND, P_0077),
    E2(0160000, SZ_INTG, TEXT_SUB,   M_GENS, P_7700, M_GEND, P_0077),
    E2(0074000, SZ_WORD, TEXT_XOR,   M_GENR, P_0700, M_GEND, P_0077),
    E1(0000100, SZ_WORD, TEXT_JMP,   M_GENA, P_0077),
    E1(0000300, SZ_WORD, TEXT_SWAB,  M_GEND, P_0077),
    E1(0005000, SZ_WORD, TEXT_CLR,   M_GEND, P_0077),
    E1(0105000, SZ_BYTE, TEXT_CLRB,  M_GEND, P_0077),
    E1(0005100, SZ_WORD, TEXT_COM,   M_GEND, P_0077),
    E1(0105100, SZ_BYTE, TEXT_COMB,  M_GEND, P_0077),
    E1(0005200, SZ_WORD, TEXT_INC,   M_GEND, P_0077),
    E1(0105200, SZ_BYTE, TEXT_INCB,  M_GEND, P_0077),
    E1(0005300, SZ_WORD, TEXT_DEC,   M_GEND, P_0077),
    E1(0105300, SZ_BYTE, TEXT_DECB,  M_GEND, P_0077),
    E1(0005400, SZ_WORD, TEXT_NEG,   M_GEND, P_0077),
    E1(0105400, SZ_BYTE, TEXT_NEGB,  M_GEND, P_0077),
    E1(0005500, SZ_WORD, TEXT_ADC,   M_GEND, P_0077),
    E1(0105500, SZ_BYTE, TEXT_ADCB,  M_GEND, P_0077),
    E1(0005600, SZ_WORD, TEXT_SBC,   M_GEND, P_0077),
    E1(0105600, SZ_BYTE, TEXT_SBCB,  M_GEND, P_0077),
    E1(0005700, SZ_WORD, TEXT_TST,   M_GENS, P_0077),
    E1(0105700, SZ_BYTE, TEXT_TSTB,  M_GENS, P_0077),
    E1(0006000, SZ_WORD, TEXT_ROR,   M_GEND, P_0077),
    E1(0106000, SZ_BYTE, TEXT_RORB,  M_GEND, P_0077),
    E1(0006100, SZ_WORD, TEXT_ROL,   M_GEND, P_0077),
    E1(0106100, SZ_BYTE, TEXT_ROLB,  M_GEND, P_0077),
    E1(0006200, SZ_WORD, TEXT_ASR,   M_GEND, P_0077),
    E1(0106200, SZ_BYTE, TEXT_ASRB,  M_GEND, P_0077),
    E1(0006300, SZ_WORD, TEXT_ASL,   M_GEND, P_0077),
    E1(0106300, SZ_BYTE, TEXT_ASLB,  M_GEND, P_0077),
    E1(0106400, SZ_BYTE, TEXT_MTPS,  M_GENS, P_0077),
    E1(0006700, SZ_WORD, TEXT_SXT,   M_GEND, P_0077),
    E1(0106700, SZ_BYTE, TEXT_MFPS,  M_GEND, P_0077),
    E1(0000400, SZ_NONE, TEXT_BR,    M_REL8, P_0377),
    E1(0001000, SZ_NONE, TEXT_BNE,   M_REL8, P_0377),
    E1(0001400, SZ_NONE, TEXT_BEQ,   M_REL8, P_0377),
    E1(0002000, SZ_NONE, TEXT_BGE,   M_REL8, P_0377),
    E1(0002400, SZ_NONE, TEXT_BLT,   M_REL8, P_0377),
    E1(0003000, SZ_NONE, TEXT_BGT,   M_REL8, P_0377),
    E1(0003400, SZ_NONE, TEXT_BLE,   M_REL8, P_0377),
    E1(0100000, SZ_NONE, TEXT_BPL,   M_REL8, P_0377),
    E1(0100400, SZ_NONE, TEXT_BMI,   M_REL8, P_0377),
    E1(0101000, SZ_NONE, TEXT_BHI,   M_REL8, P_0377),
    E1(0101400, SZ_NONE, TEXT_BLOS,  M_REL8, P_0377),
    E1(0102000, SZ_NONE, TEXT_BVC,   M_REL8, P_0377),
    E1(0102400, SZ_NONE, TEXT_BVS,   M_REL8, P_0377),
    E1(0103000, SZ_NONE, TEXT_BCC,   M_REL8, P_0377),
    E1(0103400, SZ_NONE, TEXT_BCS,   M_REL8, P_0377),
    E1(0103000, SZ_NONE, TEXT_BHIS,  M_REL8, P_0377),
    E1(0103400, SZ_NONE, TEXT_BLO,   M_REL8, P_0377),
    E2(0077000, SZ_NONE, TEXT_SOB,   M_GENR, P_0700, M_REL6, P_0077),
    E2(0004000, SZ_WORD, TEXT_JSR,   M_GENR, P_0700, M_GENA, P_0077),
    E1(0000200, SZ_NONE, TEXT_RTS,   M_GENR, P_0007),
    E1(0104000, SZ_NONE, TEXT_EMT,   M_IMM8, P_0377),
    E1(0104400, SZ_NONE, TEXT_TRAP,  M_IMM8, P_0377),
    E0(0000002, SZ_NONE, TEXT_RTI),
    E0(0000003, SZ_NONE, TEXT_BPT),
    E0(0000004, SZ_NONE, TEXT_IOT),
    E0(0000006, SZ_NONE, TEXT_RTT),
    E0(0000000, SZ_NONE, TEXT_HALT),
    E0(0000001, SZ_NONE, TEXT_WAIT),
    E0(0000005, SZ_NONE, TEXT_RESET),
    E0(0000007, SZ_NONE, TEXT_MFPT),
    E0(0000240, SZ_NONE, TEXT_NOP),
    E0(0000241, SZ_NONE, TEXT_CLC),
    E0(0000242, SZ_NONE, TEXT_CLV),
    E0(0000244, SZ_NONE, TEXT_CLZ),
    E0(0000250, SZ_NONE, TEXT_CLN),
    E0(0000257, SZ_NONE, TEXT_CCC),
    E0(0000260, SZ_NONE, TEXT_NOP),
    E0(0000261, SZ_NONE, TEXT_SEC),
    E0(0000262, SZ_NONE, TEXT_SEV),
    E0(0000264, SZ_NONE, TEXT_SEZ),
    E0(0000270, SZ_NONE, TEXT_SEN),
    E0(0000277, SZ_NONE, TEXT_SCC),
};

static constexpr uint8_t INDEX_BASE[] PROGMEM = {
};

static constexpr Entry TABLE_EIS[] PROGMEM = {
    E2(0070000, SZ_INTG, TEXT_MUL,   M_GENS, P_0077, M_GENR, P_0700),
    E2(0071000, SZ_INTG, TEXT_DIV,   M_GENS, P_0077, M_GENR, P_0700),
    E2(0072000, SZ_WORD, TEXT_ASH,   M_GENS, P_0077, M_GENR, P_0700),
    E2(0073000, SZ_WORD, TEXT_ASHC,  M_GENS, P_0077, M_GENR, P_0700),
};

static constexpr uint8_t INDEX_EIS[] PROGMEM = {
};

static constexpr Entry TABLE_MFPMTP[] PROGMEM = {
    E1(0006500, SZ_WORD, TEXT_MFPI,  M_GENA, P_0077),
    E1(0106500, SZ_WORD, TEXT_MFPD,  M_GENA, P_0077),
    E1(0006600, SZ_WORD, TEXT_MTPI,  M_GENA, P_0077),
    E1(0106600, SZ_WORD, TEXT_MTPD,  M_GENA, P_0077),
};

static constexpr uint8_t INDEX_MFPMTP[] PROGMEM = {
};

static constexpr Entry TABLE_J11[] PROGMEM = {
    E1(0006400, SZ_NONE, TEXT_MARK,  M_IMM6, P_0077),
    E1(0007000, SZ_WORD, TEXT_CSM,   M_GENS, P_0077),
    E1(0000230, SZ_NONE, TEXT_SPL,   M_IMM3, P_0007),
    E1(0007200, SZ_WORD, TEXT_TSTSET, M_GENA, P_0077),
    E1(0007300, SZ_WORD, TEXT_WRTLCK, M_GENA, P_0077),
};

static constexpr uint8_t INDEX_J11[] PROGMEM = {
};

static constexpr Entry TABLE_FP11[] PROGMEM = {
    E0(0170000, SZ_NONE, TEXT_CFCC),
    E0(0170011, SZ_NONE, TEXT_SETD),
    E0(0170001, SZ_NONE, TEXT_SETF),
    E0(0170002, SZ_NONE, TEXT_SETI),
    E0(0170012, SZ_NONE, TEXT_SETL),
    E1(0170100, SZ_WORD, TEXT_LDFPS, M_GENS, P_0077),
    E1(0170200, SZ_WORD, TEXT_STFPS, M_GEND, P_0077),
    E1(0170300, SZ_DWRD, TEXT_STST,  M_GEND, P_0077),
    E2(0176400, SZ_WORD, TEXT_LDEXP, M_GENS, P_0077, M_AC03, P_0300),
    E2(0175000, SZ_WORD, TEXT_STEXP, M_AC03, P_0300, M_GEND, P_0077),
    E1(0170600, SZ_DUBL, TEXT_ABSD,  M_GENG, P_0077),
    E2(0172000, SZ_DUBL, TEXT_ADDD,  M_GENF, P_0077, M_AC03, P_0300),
    E1(0170400, SZ_DUBL, TEXT_CLRD,  M_GENG, P_0077),
    E2(0173400, SZ_DUBL, TEXT_CMPD,  M_GENF, P_0077, M_AC03, P_0300),
    E2(0174400, SZ_DUBL, TEXT_DIVD,  M_GENF, P_0077, M_AC03, P_0300),
    E2(0177400, SZ_DUBL, TEXT_LDCDF, M_GENF, P_0077, M_AC03, P_0300),
    E2(0177000, SZ_LONG, TEXT_LDCLD, M_GENS, P_0077, M_AC03, P_0300),
    E2(0177000, SZ_INTG, TEXT_LDCID, M_GENS, P_0077, M_AC03, P_0300),
    E2(0172400, SZ_DUBL, TEXT_LDD,   M_GENF, P_0077, M_AC03, P_0300),
    E2(0171400, SZ_DUBL, TEXT_MODD,  M_GENF, P_0077, M_AC03, P_0300),
    E2(0171000, SZ_DUBL, TEXT_MULD,  M_GENF, P_0077, M_AC03, P_0300),
    E1(0170700, SZ_DUBL, TEXT_NEGD,  M_GENG, P_0077),
    E2(0176000, SZ_FLOT, TEXT_STCDF, M_AC03, P_0300, M_GENG, P_0077),
    E2(0174000, SZ_DUBL, TEXT_STD,   M_AC03, P_0300, M_GENA, P_0077),
    E2(0175400, SZ_LONG, TEXT_STCDL, M_AC03, P_0300, M_GEND, P_0077),
    E2(0175400, SZ_INTG, TEXT_STCDI, M_AC03, P_0300, M_GEND, P_0077),
    E2(0173000, SZ_DUBL, TEXT_SUBD,  M_GENF, P_0077, M_AC03, P_0300),
    E1(0170500, SZ_DUBL, TEXT_TSTD,  M_GENF, P_0077),
    E1(0170600, SZ_FLOT, TEXT_ABSF,  M_GENG, P_0077),
    E2(0172000, SZ_FLOT, TEXT_ADDF,  M_GENF, P_0077, M_AC03, P_0300),
    E1(0170400, SZ_FLOT, TEXT_CLRF,  M_GENG, P_0077),
    E2(0173400, SZ_FLOT, TEXT_CMPF,  M_GENF, P_0077, M_AC03, P_0300),
    E2(0174400, SZ_FLOT, TEXT_DIVF,  M_GENF, P_0077, M_AC03, P_0300),
    E2(0177400, SZ_FLOT, TEXT_LDCFD, M_GENF, P_0077, M_AC03, P_0300),
    E2(0177000, SZ_LONG, TEXT_LDCLF, M_GENS, P_0077, M_AC03, P_0300),
    E2(0177000, SZ_INTG, TEXT_LDCIF, M_GENS, P_0077, M_AC03, P_0300),
    E2(0172400, SZ_FLOT, TEXT_LDF,   M_GENF, P_0077, M_AC03, P_0300),
    E2(0171400, SZ_FLOT, TEXT_MODF,  M_GENF, P_0077, M_AC03, P_0300),
    E2(0171000, SZ_FLOT, TEXT_MULF,  M_GENF, P_0077, M_AC03, P_0300),
    E1(0170700, SZ_FLOT, TEXT_NEGF,  M_GENG, P_0077),
    E2(0176000, SZ_DUBL, TEXT_STCFD, M_AC03, P_0300, M_GENG, P_0077),
    E2(0174000, SZ_FLOT, TEXT_STF,   M_AC03, P_0300, M_GENA, P_0077),
    E2(0175400, SZ_LONG, TEXT_STCFL, M_AC03, P_0300, M_GEND, P_0077),
    E2(0175400, SZ_INTG, TEXT_STCFI, M_AC03, P_0300, M_GEND, P_0077),
    E2(0173000, SZ_FLOT, TEXT_SUBF,  M_GENF, P_0077, M_AC03, P_0300),
    E1(0170500, SZ_FLOT, TEXT_TSTF,  M_GENF, P_0077),
};

static constexpr uint8_t INDEX_FP11[] PROGMEM = {
};

// clang-format on

using EntryPage = entry::TableBase<Entry>;

static constexpr EntryPage DCJ11_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_BASE), ARRAY_RANGE(INDEX_BASE)},
        {ARRAY_RANGE(TABLE_EIS), ARRAY_RANGE(INDEX_EIS)},
        {ARRAY_RANGE(TABLE_J11), ARRAY_RANGE(INDEX_J11)},
        {ARRAY_RANGE(TABLE_FP11), ARRAY_RANGE(INDEX_FP11)},
        {ARRAY_RANGE(TABLE_MFPMTP), ARRAY_RANGE(INDEX_MFPMTP)},
};

static constexpr EntryPage DCT11_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_BASE), ARRAY_RANGE(INDEX_BASE)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

static constexpr Cpu CPU_TABLE[] PROGMEM = {
        {J11, TEXT_CPU_J11, ARRAY_RANGE(DCJ11_PAGES)},
        {T11, TEXT_CPU_T11, ARRAY_RANGE(DCT11_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    opc &= ~Entry::insnMask(flags.srcPos());
    opc &= ~Entry::insnMask(flags.dstPos());
    return opc == entry->readOpCode();
}

Error TablePdp11::searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) const {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TablePdp11::listCpu_P() const {
    return TEXT_PDP11_LIST;
}

const /*PROGMEM*/ char *TablePdp11::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TablePdp11::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    if (name.iequals_P(TEXT_CPU_DCJ11)) {
        cpuType = J11;
        return OK;
    }
    if (name.iequals_P(TEXT_CPU_DCT11)) {
        cpuType = T11;
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TablePdp11 TABLE;

}  // namespace pdp11
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
