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

#include "table_z80.h"
#include "table_z180.hpp"

namespace libasm {
namespace z80 {

using namespace libasm::text::z80;
using namespace libasm::z80::common;

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {Z80, TEXT_CPU_Z80, ARRAY_RANGE(Z80_PAGES)},
        {Z180, TEXT_CPU_Z180, ARRAY_RANGE(Z180_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_SR8 || opr == R_A)
        return table == M_SRC || table == M_DST || table == M_SR8 || table == M_DR8 ||
               table == R_H || table == R_L;
    if (opr == I_HL)
        return table == M_SRC || table == M_DST;
    if (opr == R_C)
        return table == M_SRC || table == M_DST || table == M_SR8 || table == M_DR8 ||
               table == M_CC4 || table == M_CC8;
    if (opr == R_H || opr == R_L)
        return table == M_SRC || table == M_DST || table == M_SR8 || table == M_DR8;
    if (opr == R_BC || opr == R_DE)
        return table == M_R16 || table == M_R16X || table == R_PTRH;
    if (opr == R_HL)
        return table == M_R16 || table == M_R16X;
    if (opr == R_SP)
        return table == M_R16 || table == M_R16X;
    if (opr == R_IX || opr == R_IY)
        return table == R_IDX || table == M_R16X;
    if (opr == M_IDX8)
        return table == M_IDX;
    if (opr == M_IM16)
        return table == M_IM8 || table == M_REL8 || table == M_BIT || table == M_VEC ||
               table == M_IMMD || table == M_LM16 || table == M_JABS;
    if (opr == M_ABS)
        return table == M_IOA || table == M_DABS;
    if (opr == M_CC4)
        return table == M_CC8;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode(insn.dstOp.mode, table.dst()) && acceptMode(insn.srcOp.mode, table.src());
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    const auto dst = flags.dst();
    const auto src = flags.src();
    if (dst == M_DR8 || src == M_DR8) {
        const auto reg = (opc >> 3) & 7;
        if (reg == 6)
            return false;
    }
    if (src == M_SR8 || dst == M_SR8) {
        const auto reg = opc & 7;
        if (reg == 6)
            return false;
    }
    opc &= ~flags.mask();
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

bool isPrefix(CpuType cpuType, Config::prefix_t code) {
    return cpu(cpuType)->isPrefix(code);
}

const /*PROGMEM*/ char *TableZ80::listCpu_P() const {
    return TEXT_Z80_LIST;
}

const /*PROGMEM*/ char *TableZ80::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableZ80::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableZ80 TABLE;

}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
