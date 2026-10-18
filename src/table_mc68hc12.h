/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_TABLE_MC68HC12_H__
#define __LIBASM_TABLE_MC68HC12_H__

#include "config_mc68hc12.h"
#include "insn_mc68hc12.h"

namespace libasm {
namespace mc68hc12 {

struct TableMc68HC12 final : InsnTable<CpuType> {
    const /*PROGMEM*/ char *listCpu_P() const override;
    const /*PROGMEM*/ char *cpuName_P(CpuType cpuType) const override;
    Error searchCpuName(StrScanner &name, CpuType &cpuType) const override;
};

extern const TableMc68HC12 TABLE;

bool hasOperand(CpuType, AsmInsn &insn);
Error searchName(CpuType, AsmInsn &insn);
Error searchOpCode(CpuType, DisInsn &insn, StrBuffer &out);
bool isPrefix(CpuType, Config::opcode_t code);
/** The page register which switches the window at CPU address |addr|, or PAGE_NONE */
PageReg pageWindow(CpuType, uint16_t addr);
/** The last CPU address of the window at |addr| */
uint16_t pageWindowLast(CpuType, uint16_t addr);
/** The number of implemented bits of page register |reg|, or 0 when it doesn't exist */
uint_fast8_t pageRegisterBits(CpuType, PageReg reg);
/** The physical address of CPU address |addr| when its window is on |page| */
uint32_t toPhysical(CpuType, uint16_t addr, uint8_t page);
/** The CPU address |cpu| of physical |addr| through the program page window on |page| */
bool programPage(CpuType, uint32_t addr, uint16_t &cpu, int16_t &page);
/**
 * The CPU address |cpu| of physical |addr| in program space, a fixed region or the program page
 * window on |page| (-1 when fixed).
 */
bool programAddress(CpuType, uint32_t addr, uint16_t &cpu, int16_t &page);
/**
 * The CPU address |cpu| of physical |addr|, through a fixed region or a window whose page is
 * |pages| (-1 when not assumed).  OVERFLOW_RANGE when no window maps it, OPERAND_NOT_ALLOWED
 * when two windows do.
 */
Error cpuAddress(CpuType, uint32_t addr, const int16_t *pages, uint16_t &cpu);
/** Encode MC68HCS12X transfer/exchange postbyte of |src|,|dst| into |opc| */
bool encodeXferPair(AddrMode mode, Config::opcode_t &opc, RegName src, RegName dst);
/** Decode MC68HCS12X transfer/exchange postbyte |opc| */
bool decodeXferPair(AddrMode mode, Config::opcode_t opc, RegName &src, RegName &dst);
/** Whether page 2 code |opc| is an unimplemented opcode trap */
bool isTrapCode(CpuType, Config::opcode_t opc);

}  // namespace mc68hc12
}  // namespace libasm

#endif  // __LIBASM_TABLE_MC68HC12_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
