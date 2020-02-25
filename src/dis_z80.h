/* -*- mode: c++; -*- */
#ifndef __DIS_Z80_H__
#define __DIS_Z80_H__

#include "config_z80.h"

#include "insn_z80.h"
#include "reg_z80.h"
#include "table_z80.h"
#include "dis_interface.h"

class DisZ80 : public Disassembler<target::uintptr_t> {
public:
    DisOperand &getFormatter() override { return _formatter; }
    bool setCpu(const char *cpu) override { return TableZ80.setCpu(cpu); }
    const char *listCpu() const override { return TableZ80::listCpu(); }

private:
    DisIntelOperand _formatter;
    RegZ80 _regs;

    RegBase &getRegister() override { return _regs; }
    template<typename U>
    void outAddress(U addr, bool indir = true);
    void outIndexOffset(const Insn &insn, int8_t offset);
    void outRegister(RegName regName);
    void outPointer(RegName regName);
    void outDataRegister(RegName regName);
    void outConditionName(target::opcode_t cc, bool cc8 = true);

    Error decodeOperand(
        DisMemory<target::uintptr_t> &memory, Insn& insn);

    Error decodeInherent(Insn &insn);
    Error decodeImmediate8(Insn &insn, uint8_t val);
    Error decodeImmediate16(Insn &insn, uint16_t val);
    Error decodeDirect(Insn &insn, target::uintptr_t addr);
    Error decodeIoaddr(Insn &insn, uint8_t ioaddr);
    Error decodeRelative(Insn &insn, int8_t delta);
    Error decodeIndexed(Insn &insn, int8_t offset);
    Error decodeIndexedImmediate8(
        Insn &insn, int8_t offset, uint8_t val);
    Error decodeIndexedBitOp(
        Insn &insn, int8_t offset, target::opcode_t opCode);

    Error decode(
        DisMemory<target::uintptr_t> &memory, Insn& insn) override;
};

#endif // __DIS_Z80_H__
