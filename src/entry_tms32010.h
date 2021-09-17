/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __ENTRY_TMS32010_H__
#define __ENTRY_TMS32010_H__

#include "config_tms32010.h"
#include "entry_base.h"

namespace libasm {
namespace tms32010 {

enum AddrMode : uint8_t {
    M_NO = 0,
    M_MAM = 1,    // ---- ---- 0DDD DDDD Memory Addressing Mode: Direct addressing
                  // ---- ---- 10id n00a Memory Addressing Mode: Indirect addressing
    M_LS4 = 2,    // ---- SSSS ---- ---- 4-bit left-shift
    M_LS3 = 3,    // ---- -XXX ---- ---- 3-bit left-shift
    M_LS0 = 4,    // ---- -000 ---- ---- 0 left-shift
    M_PA = 5,     // ---- -AAA ---- ---- Port address
    M_AR = 6,     // ---- ---a ---- ---- Auxiliary register
    M_DPK = 7,    // ---- ---- ---- ---d Data memory page pointer immediate
    M_ARK = 8,    // ---- ---- ---- ---a Auxiliary register pointer immediate
    M_IM8 = 9,    // ---- ---- KKKK KKKK Immediate operand 8-bits
    M_REL = 10,   // ---- BBBB BBBB BBBB Branch address
    M_IM13 = 11,  // ---K KKKK KKKK KKKK Immediate operand 13-bits
    M_PMA = 12,   // Program memory address
    // Assembler
    M_DMA = 13,   // <dma>: Direct memory address
    M_ARP = 14,   // "*": Indirect addressing
    M_INC = 15,   // "*+": Indirect then auto increment addressing
    M_DEC = 16,   // "*-": Indirect then auto decrement addressing
    M_NARP = 17,  // Next auxiliary register pointer
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint16_t _attr;

        static constexpr Flags create(AddrMode op1, AddrMode op2, AddrMode op3) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(op1) << op1_gp) |
                                               (static_cast<uint16_t>(op2) << op2_gp) |
                                               (static_cast<uint16_t>(op3) << op3_gp))};
        }
        Flags read() const { return Flags{pgm_read_word(&_attr)}; }

        AddrMode op1() const { return AddrMode((_attr >> op1_gp) & mode_gm); }
        AddrMode op2() const { return AddrMode((_attr >> op2_gp) & mode_gm); }
        AddrMode op3() const { return AddrMode((_attr >> op3_gp) & mode_gm); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr uint8_t mode_gm = 0x1f;
    static constexpr int op1_gp = 0;
    static constexpr int op2_gp = 5;
    static constexpr int op3_gp = 10;
};

}  // namespace tms32010
}  // namespace libasm

#endif  // __ENTRY_TMS32010_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
