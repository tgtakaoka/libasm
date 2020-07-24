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

#ifndef __ASM_MC6809_H__
#define __ASM_MC6809_H__

#include "asm_base.h"
#include "config_mc6809.h"
#include "insn_mc6809.h"
#include "reg_mc6809.h"
#include "table_mc6809.h"

namespace libasm {
namespace mc6809 {

class AsmMc6809
    : public Assembler,
      public Config {
public:
    ValueParser *getParser() override { return &_parser; }

    // Config
    const char *listCpu() const override { return TableMc6809.listCpu(); }
    bool setCpu(const char *cpu) override { return TableMc6809.setCpu(cpu); }
    const char *getCpu() const override { return TableMc6809.getCpu(); }

    void reset() override { _direct_page = 0; }

private:
    MotoValueParser _parser;
    RegMc6809 _regs;
    uint8_t _direct_page;

    enum Token : char {
        EOL = 0,         // end of line
        ERROR = '?',
        COMMA = ',',
        LBRKT = '[',
        RBRKT = ']',
        IDX_PNTR = 'P',  // ,x where x=base register
                         // _reg=x
        IDX_AUTO = 'a',  // ,x+/,x++/,-x/,--x where x=base register
                         // _reg=x _extra=1/2/-1/-2
        IDX_DISP = 'D',  // disp,x where x=base/PC/PCR register
                         // _val32=disp _reg=x _extra=0/5/8/16
        TFM_MODE = 'T',  // x- where x=D/X/Y/U/S or x+ where x=D
                         // _reg=x _extra=TFM mode
        REG_BITP = 'B',  // r.b where r=A/B/CC
                         // _reg=r _extra=b
        DIR_BITP = 'b',  // nn.b where nn is direct page address
                         // _val32=nn _extra=b
        REG_LIST = 'L',  // r1,r2,... where rn is valid register
                         // _reg=r1 _reg2=r2, _val32=post, _extra=n
                         // _regs if there is a stack register in list
        VAL_IMM  = '#',  // immediate value
        VAL_ADDR = 'A'   // address value _extra=0/8/16
    } _token;
    RegName _reg;
    RegName _reg2;
    RegName _regStack;
    int8_t _extra;
    uint32_t _val32;
    Token nextToken(Token expect = EOL);
    Token _nextToken(Token expect = EOL);
    void printToken() const;
    bool tokenPointerIndex(const char *p);
    bool tokenTransferMemory(const char *p);
    bool tokenRegisterList(const char *p);
    bool tokenDisplacementIndex(const char *p, int8_t size);
    bool tokenBitPosition(const char *p);
    bool tokenConstant(const char *p, const char immediate);

    struct Operand : public ErrorReporter {
        AddrMode mode;
        IndexedSubMode sub;
        RegName index;
        RegName base;
        bool indir;
        int8_t extra;
        uint32_t val32;
        Operand()
            : ErrorReporter(),
              mode(NONE),
              sub(PNTR_IDX),
              index(REG_UNDEF),
              base(REG_UNDEF),
              indir(false),
              extra(0),
              val32(0)
        {}
    };
    void printOperand(const Operand &op, const Operand &extra);
    Error parseOperand(Operand &op);

    // MC6809
    Error encodePushPull(InsnMc6809 &insn, const Operand &op);
    Error encodeRegisters(InsnMc6809 &insn, Operand &op);
    Error encodeRelative(InsnMc6809 &insn, const Operand &op);
    Error encodeImmediate(
            InsnMc6809 &insn, const Operand &op, Operand &extra);
    Config::ptrdiff_t calculateDisplacement(
            const InsnMc6809 &insn, const Operand &op) const;
    Error encodeIndexed(
            InsnMc6809 &insn, Operand &op, bool emitInsn = true);
    // HD6309
    Error encodeBitOperation(
            InsnMc6809 &insn, const Operand &op, const Operand &extra);
    Error encodeTransferMemory(
            InsnMc6809 &insn, const Operand &op, const Operand &extra);

    Error processPseudo(InsnMc6809 &insn, const char *line);
    Error encode(Insn &insn) override;
};

} // namespace mc6809
} // namespace libasm

#endif // __ASM_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
