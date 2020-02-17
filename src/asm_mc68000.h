/* -*- mode: c++; -*- */
#ifndef __ASM_MC68000_H__
#define __ASM_MC68000_H__

#include "config_mc68000.h"

#include "insn_mc68000.h"
#include "table_mc68000.h"
#include "asm_interface.h"

class AsmMc68000 : public Assembler<target::uintptr_t> {
public:
    AsmOperand &getParser() override { return _parser; }
    bool acceptCpu(const char *cpu) override;

private:
    AsmMotoOperand _parser;

public:
    struct Operand : public ErrorReporter {
        EaMode mode;
        RegName reg;
        RegName index;          // index register
        EaSize size;            // index size
        uint32_t val32;
        bool satisfy(host::uint_t categories) const {
            return EaMc68000::satisfy(mode, categories);
        }
        void reset() {
            setError(OK);
            mode = M_NONE;
            reg = index = REG_UNDEF;
            size = SZ_NONE;
            val32 = 0;
        }
    };
private:

    Error checkSize(Insn& insn, const EaSize size);
    Error checkSize(const uint32_t val32, const EaSize size, bool uint);
    Error parseOperand(Operand &opr);
    Error parseMoveMultiRegList(Operand &opr);

    Error emitImmediateData(
        Insn &insn, EaSize size, uint32_t val, Error error);
    Error emitEffectiveAddr(
        Insn &insn,
        const Operand &ea,
        host::int_t size_gp = 6,
        host::int_t mode_gp = 3,
        host::uint_t reg_gp = 0);

    Error encodeImplied(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeDestSiz(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeAddrReg(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeDataReg(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeTrapVec(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeDataDst(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeDestOpr(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeSignExt(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeRelative(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeMoveMlt(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeMoveQic(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeMovePer(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeAregSiz(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeDregDst(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeDataQic(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeDmemOpr(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeDmemSiz(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeRegsExg(Insn &insn, const Operand &op1, const Operand &op2);
    Error encodeMoveOpr(Insn &insn, const Operand &op1, const Operand &op2);

    Error encode(Insn &insn) override;
};

#endif // __ASM_MC68000_H__
