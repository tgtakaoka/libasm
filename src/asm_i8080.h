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

#ifndef __ASM_I8080_H__
#define __ASM_I8080_H__

#include "config_i8080.h"

#include "insn_i8080.h"
#include "reg_i8080.h"
#include "table_i8080.h"
#include "asm_interface.h"

namespace libasm {
namespace i8080 {

class AsmI8080 : public Assembler<Config::uintptr_t> {
public:
    AsmOperand &getParser() override { return _parser; }
    bool setCpu(const char *cpu) override { return TableI8080.setCpu(cpu); }
    const char *listCpu() const override { return TableI8080::listCpu(); }
    Endian endian() const override { return ENDIAN_LITTLE; }
    host::uint_t maxBytes() const override { return Entry::code_max; }
    host::uint_t maxName() const override { return Entry::name_max; }

private:
    AsmIntelOperand _parser;
    RegI8080 _regs;

    Error encodePointerReg(InsnI8080 &insn);
    Error encodeStackReg(InsnI8080 &insn);
    Error encodeIndexReg(InsnI8080 &insn);
    Error encodeDataReg(InsnI8080 &insn);
    Error encodeDataDataReg(InsnI8080 &insn);
    Error encodeVectorNo(InsnI8080 &insn);

    Error encodeImmediate(InsnI8080 &insn);
    Error encodeDirect(InsnI8080 &insn);
    Error encodeIoaddr(InsnI8080 &insn);

    Error encode(Insn<Config::uintptr_t> &insn) override;
};

} // namespace i8080
} // namespace libasm

#endif // __ASM_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
