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

#ifndef __LIBASM_INSN_PDP11_H__
#define __LIBASM_INSN_PDP11_H__

#include "config_pdp11.h"
#include "entry_pdp11.h"
#include "insn_base.h"
#include "reg_pdp11.h"

namespace libasm {
namespace pdp11 {

struct EntryInsn : EntryInsnPostfix<Config, Entry> {
    AddrMode src() const { return flags().src(); }
    AddrMode dst() const { return flags().dst(); }
    OprPos srcPos() const { return flags().srcPos(); }
    OprPos dstPos() const { return flags().dstPos(); }
    OprSize size() const { return flags().size(); }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}

    /** Read 32 bit mixed endian data */
    uint32_t readUint32Mix() {
        const auto msw = readUint16();
        const auto lsw = readUint16();
        return static_cast<uint32_t>(msw) << 16 | lsw;
    }

    /** Read 64 bit mixed endian data */
    uint64_t readUint64Mix() {
        const auto msw = readUint32Mix();
        const auto lsw = readUint32Mix();
        return static_cast<uint64_t>(msw) << 32 | lsw;
    }        
};

}  // namespace pdp11
}  // namespace libasm

#endif  // __LIBASM_INSN_PDP11_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
