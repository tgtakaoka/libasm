/* -*- mode: c++; -*- */
#ifndef __INSN_MC68000_H__
#define __INSN_MC68000_H__

#include "insn_base.h"
#include "entry_mc68000.h"
#include "reg_mc68000.h"

class Insn : public InsnBase<target::uintptr_t, target::insn_t, 10, 7> {
public:
    InsnFormat insnFormat() const { return _insnFormat(_flags); }

    void setFlags(host::uint_t flags) {
        _flags = flags;
    }

    void appendSize(EaSize size, RegMc68000 regs) {
        regs.outEaSize(_name + strlen(_name), size);
    }

    Error readUint16(DisMemory<target::uintptr_t> &memory, uint16_t &val) {
        uint8_t high, low;
        if (readByte(memory, high)) return NO_MEMORY;
        if (readByte(memory, low))  return NO_MEMORY;
        val = static_cast<uint16_t>(high) << 8 | low;
        return OK;
    }

    Error readUint32(DisMemory<target::uintptr_t> &memory, uint32_t &val) {
        uint16_t high, low;
        if (readUint16(memory, high)) return NO_MEMORY;
        if (readUint16(memory, low))  return NO_MEMORY;
        val = static_cast<uint32_t>(high) << 16 | low;
        return OK;
    }

    void emitUint16(uint16_t val) {
        emitByte(static_cast<uint8_t>(val >> 8));
        emitByte(static_cast<uint8_t>(val & 0xff));
    }

    void emitUint32(uint32_t val) {
        emitUint16(static_cast<uint16_t>(val >> 16));
        emitUint16(static_cast<uint16_t>(val & 0xffff));
    }

private:
    host::uint_t _flags;
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
