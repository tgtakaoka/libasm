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

#ifndef __ENTRY_INS8070_H__
#define __ENTRY_INS8070_H__

#include <stdint.h>

#include "config_ins8070.h"
#include "entry_base.h"

namespace libasm {
namespace ins8070 {

enum OprSize : uint8_t {
    SZ_NONE = 0,  // unknown
    SZ_BYTE = 1,
    SZ_WORD = 2,
};

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_AR = 1,    // Accumulator: A
    M_ER = 2,    // Extension register: E
    M_EA = 3,    // Double precision Accumulator: EA
    M_TR = 4,    // Temporary register: T
    M_SR = 5,    // Status register: S
    M_P23 = 6,   // Pointer Register: P2/P3
    M_SP = 7,    // SP register
    M_PTR = 8,   // Base Register: PC/SP/P2/P3
    M_IMM = 9,   // Immediate constant
    M_ADR = 10,  // 16 bit constant
    M_VEC = 11,  // 4 bit constant
    M_IDX = 12,  // Pointer relative: nn,Pn
    M_PCR = 13,  // PC Relative
    M_GEN = 14,  // nnnn/nnnn,PC/nn,SP/nn,Pn/@nn,Pn/nn,@Pn/0xFFnn
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint16_t _attr;

        static constexpr Flags create(AddrMode dst, AddrMode src, OprSize size) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(src) << src_gp) |
                                               (static_cast<uint16_t>(size) << size_gp))};
        }

        static constexpr Flags exec(AddrMode dst) {
            return Flags{
                    static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                          (static_cast<uint16_t>(M_NONE) << src_gp) |
                                          (static_cast<uint16_t>(SZ_NONE) << size_gp) | exec_bm)};
        }

        static constexpr Flags undef(AddrMode dst, AddrMode src) {
            return Flags{
                    static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                          (static_cast<uint16_t>(src) << src_gp) |
                                          (static_cast<uint16_t>(SZ_NONE) << size_gp) | undef_bm)};
        }

        Flags read() const { return Flags{pgm_read_word(&_attr)}; }
        AddrMode dst() const { return AddrMode((_attr >> dst_gp) & mode_gm); }
        AddrMode src() const { return AddrMode((_attr >> src_gp) & mode_gm); }
        OprSize size() const { return OprSize((_attr >> size_gp) & size_gm); }
        bool execute() const { return _attr & exec_bm; }
        bool undefined() const { return _attr & undef_bm; }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr int dst_gp = 0;
    static constexpr int src_gp = 4;
    static constexpr int size_gp = 8;
    static constexpr int exec_bp = 14;
    static constexpr int undef_bp = 15;
    static constexpr uint8_t mode_gm = 0x0F;
    static constexpr uint8_t size_gm = 0x03;
    static constexpr uint16_t exec_bm = (1 << exec_bp);
    static constexpr uint16_t undef_bm = (1 << undef_bp);
};

}  // namespace ins8070
}  // namespace libasm

#endif  // __ENTRY_INS8070_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
