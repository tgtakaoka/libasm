/* -*- mode: c++; -*- */
#ifndef __DIS_TMS9900_H__
#define __DIS_TMS9900_H__

#include "config_tms9900.h"

#include "insn_tms9900.h"
#include "dis_interface.h"

class DisTms9900 : public Disassembler<target::uintptr_t> {
public:
    DisOperand &getFormatter() override { return _formatter; }
    bool acceptCpu(const char *cpu) override;

private:
    DisIntelOperand _formatter;

    void outAddress(target::uintptr_t addr, bool relax = true);
    void outRegister(host::uint_t regno);

    Error decodeOperand(
        DisMemory<target::uintptr_t> &memory, Insn& insn, host::uint_t opr);
    Error decodeImmediate(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeRelative(Insn &insn);
    Error decode(
        DisMemory<target::uintptr_t> &memory, Insn& insn) override;
};

#endif // __DIS_TMS9900_H__
