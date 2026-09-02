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

#ifndef __LIBASM_CONFIG_I960_H__
#define __LIBASM_CONFIG_I960_H__

#include "config_base.h"

namespace libasm {
namespace i960 {

// The 80960KA and 80960KB share one instruction set; the KB adds the
// floating-point and decimal instructions on top of it.  Both are spelled
// 80960 here, and the extension will be selected by an option rather than by
// a CPU of its own.
enum CpuType : uint8_t {
    I80960,
};

// Where an instruction leaves an operand field unused, the mode bit of that
// field may read either way; the processor does not look at it.  GNU as and
// the original Intel assembler set it, ASL leaves it clear, and the two
// therefore encode the same instruction differently.  This selects which of
// them an encoding is expected to be in.
enum ModeBits : uint8_t {
    MODE_BITS_CLEAR = 0,  // as ASL encodes an unused field
    MODE_BITS_SET = 1,    // as GNU as and the Intel assembler encode one
};

// What the operand of the IP-relative addressing mode means.  Table 5-1 of the
// manual gives the syntax as "exp (IP)" and 5-8 says "the displacement plus a
// constant of 8 is added to the IP of the instruction", which is how GNU as
// reads it too.  ASL instead takes the operand as the address to be reached and
// works the displacement out itself, so a source written for one assembler does
// not encode the same under the other.
enum IpMode : uint8_t {
    IP_OFFSET = 0,  // the operand is the offset from IP + 8, as the manual has it
    IP_TARGET = 1,  // the operand is the address to be reached, as ASL has it
};

// The 80960KB adds the floating-point instructions and the three decimal ones
// to the base set; GNU as gathers exactly those under its -AKB selector, where
// -AKA is the base alone.  The disassembler enables them by default and the
// assembler does not; each class sets its own in reset().
enum FpuType : uint8_t { FPU_NONE = 0, FPU_ON = 1 };

struct Config
    : ConfigImpl<CpuType, ADDRESS_32BIT, ADDRESS_BYTE, OPCODE_32BIT, ENDIAN_LITTLE, 8, 8> {
    Config(const InsnTable<CpuType> &table)
        : ConfigImpl(table, I80960),
          _modeBits(MODE_BITS_SET),
          _ipMode(IP_OFFSET),
          _fpuType(FPU_NONE) {}

    bool hasFpu() const { return _fpuType == FPU_ON; }

    Error setFpu(bool enable) {
        _fpuType = enable ? FPU_ON : FPU_NONE;
        return OK;
    }

    ModeBits modeBits() const { return _modeBits; }

    void setModeBits(ModeBits modeBits) { _modeBits = modeBits; }

    // The value the bit is to carry, so the option reads as the bit itself.
    Error setModeBits(int32_t value) {
        if (value != 0 && value != 1)
            return OVERFLOW_RANGE;
        _modeBits = value ? MODE_BITS_SET : MODE_BITS_CLEAR;
        return OK;
    }

    bool ipOffset() const { return _ipMode == IP_OFFSET; }

    void setIpMode(IpMode ipMode) { _ipMode = ipMode; }

    Error setIpOperand(StrScanner &scan) {
        if (scan.iequals_P(PSTR("offset"))) {
            _ipMode = IP_OFFSET;
        } else if (scan.iequals_P(PSTR("target"))) {
            _ipMode = IP_TARGET;
        } else {
            return UNKNOWN_OPERAND;
        }
        return OK;
    }

private:
    ModeBits _modeBits;
    IpMode _ipMode;
    FpuType _fpuType;
};

}  // namespace i960
}  // namespace libasm

#endif  // __LIBASM_CONFIG_I960_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
