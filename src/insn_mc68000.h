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

#ifndef __INSN_MC68000_H__
#define __INSN_MC68000_H__

#include "insn_base.h"
#include "entry_mc68000.h"
#include "reg_mc68000.h"

using libasm::mc68000::Entry;
using libasm::mc68000::EaSize;
using libasm::mc68000::RegMc68000;

class Insn : public InsnBase {
public:
    InsnFormat insnFormat() const { return Entry::_insnFormat(_flags); }

    void setFlags(host::uint_t flags) {
        _flags = flags;
    }

    target::insn_t embed(target::insn_t data, host::uint_t gp = 0) {
        return (_insnCode |= (data << gp));
    }

    void setSize(EaSize size) { _size = size; }
    EaSize size() const { return _size; }
    void appendSize(EaSize size, RegMc68000 regs) {
        _size = size;
        regs.outEaSize(_name + strlen(_name), size);
    }

    void emitInsn() {
        emitUint16(_insnCode, 0);
        if (_length == 0) _length = 2;
    }

    void emitOperand16(uint16_t val16) {
        if (_length == 0) _length = 2;
        emitUint16(val16, _length);
        _length += 2;
    }

    void emitOperand32(uint32_t val32) {
        emitOperand16(static_cast<uint16_t>(val32 >> 16));
        emitOperand16(static_cast<uint16_t>(val32));
    }

protected:
    Endian endian() override { return ENDIAN_BIG; }

private:
    host::uint_t _flags;
    EaSize _size;

    void emitUint16(uint16_t val16, host::uint_t pos) {
        _bytes[pos++] = static_cast<uint8_t>(val16 >> 8);
        _bytes[pos] = static_cast<uint8_t>(val16);
    }
};

static const char *insnFormat(const Insn &insn) __attribute__((unused));
static const char *insnFormat(const Insn &insn) {
    switch (insn.insnFormat()) {
    case IMPLIED: return "IMPLIED";
    case ADDR_REG: return "ADDR_REG";
    case DATA_REG: return "DATA_REG";
    case MOVE_USP: return "MOVE_USP";
    case TRAP_VEC: return "TRAP_VEC";
    case DATA_DST: return "DATA_DST";
    case DEST_OPR: return "DEST_OPR";
    case SIGN_EXT: return "SIGN_EXT";
    case RELATIVE: return "RELATIVE";
    case DEST_SIZ: return "DEST_SIZ";
    case MOVE_MLT: return "MOVE_MLT";
    case MOVE_SR:  return "MOVE_SR";
    case AREG_LNG: return "AREG_LNG";
    case AREG_SIZ: return "AREG_SIZ";
    case DREG_DST: return "DREG_DST";
    case MOVE_QIC: return "MOVE_QIC";
    case MOVE_PER: return "MOVE_PER";
    case DATA_QIC: return "DATA_QIC";
    case DREG_SIZ: return "DREG_SIZ";
    case DMEM_SIZ: return "DMEM_SIZ";
    case DREG_ROT: return "DREG_ROT";
    case DMEM_DST: return "DMEM_DST";
    case REGS_EXG: return "REGS_EXG";
    case CMPM_SIZ: return "CMPM_SIZ";
    case DMEM_OPR: return "DMEM_OPR";
    case MOVA_OPR: return "MOVA_OPR";
    case MOVE_OPR: return "MOVE_OPR";
    default: return "unkn_fmt";
    }
}

#endif // __INSN_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
