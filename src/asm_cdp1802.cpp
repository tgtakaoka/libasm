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

#include "asm_cdp1802.h"

#include "reg_cdp1802.h"
#include "table_cdp1802.h"

namespace libasm {
namespace cdp1802 {

using namespace reg;

static const char OPT_BOOL_USE_REGISTER[] PROGMEM = "use-register";
static const char OPT_DESC_USE_REGISTER[] PROGMEM = "enable register name Rn";
static const char OPT_BOOL_SMART_BRANCH[] PROGMEM = "smart-branch";
static const char OPT_DESC_SMART_BRANCH[] PROGMEM = "enable optimizing to short branch";

struct AsmCdp1802::Operand : public OperandBase {
    AddrMode mode;
    uint16_t val16;
    Operand() : mode(M_NONE), val16(0) {}
};

AsmCdp1802::AsmCdp1802()
    : Assembler(_parser, TableCdp1802::TABLE, _pseudos),
      _parser(_number, _comment, _symbol, _letter, _location),
      _pseudos() {
    reset();
}

AsmCdp1802::OptSmartBranch::OptSmartBranch(bool &var)
    : BoolOption(OPT_BOOL_SMART_BRANCH, OPT_DESC_SMART_BRANCH, var) {}

AsmCdp1802::OptUseRegister::OptUseRegister(bool &var, const OptionBase &next)
    : BoolOption(OPT_BOOL_USE_REGISTER, OPT_DESC_USE_REGISTER, var, next) {}

static Config::uintptr_t page(Config::uintptr_t addr) {
    return addr & ~0xFF;
}

void AsmCdp1802::encodePage(InsnCdp1802 &insn, AddrMode mode, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    if (mode == M_PAGE) {
        if (page(target) != page(base))
            setErrorIf(op, OVERWRAP_PAGE);
        insn.emitInsn();
        insn.emitByte(target);
        return;
    }
    if (_smartBranch && page(target) == page(base)) {
        const auto opc = insn.opCode();
        insn.setOpCode(0x30 | (opc & 0xF));  // convert to in-page branch
        insn.emitInsn();
        insn.emitByte(target);
        return;
    }
    insn.emitInsn();
    insn.emitUint16(op.val16);
}

void AsmCdp1802::emitOperand(InsnCdp1802 &insn, AddrMode mode, const Operand &op) {
    auto val16 = op.val16;
    switch (mode) {
    case M_REG1:
        if (op.getError())
            val16 = 7;  // default work register.
        if (val16 == 0) {
            setErrorIf(op, REGISTER_NOT_ALLOWED);
            val16 = 7;
        }
        /* Fall-through */
    case M_REGN:
        if (op.getError())
            val16 = 7;  // default work register.
        if (val16 >= 16) {
            setErrorIf(op, ILLEGAL_REGISTER);
            val16 = 7;
        }
        insn.embed(val16);
        insn.emitInsn();
        break;
    case M_IMM8:
        if (overflowUint8(val16))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitInsn();
        insn.emitByte(val16);
        break;
    case M_PAGE:
    case M_ADDR:
        encodePage(insn, mode, op);
        break;
    case M_IOAD:
        if (op.getError())
            val16 = 1;  // default IO address
        if (val16 == 0 || val16 >= 8) {
            setErrorIf(op, OPERAND_NOT_ALLOWED);
            val16 = 1;
        }
        insn.embed(val16);
        insn.emitInsn();
        break;
    default:
        insn.emitInsn();
        break;
    }
}

Error AsmCdp1802::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (_useReg) {
        const auto reg = parseRegName(p);
        if (reg != REG_UNDEF) {
            op.val16 = int8_t(reg);
            op.mode = M_REGN;
            scan = p;
            return OK;
        }
    }
    op.val16 = parseExpr16(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_ADDR;
    scan = p;
    return OK;
}

Error AsmCdp1802::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnCdp1802 insn(_insn);
    Operand op1, op2;
    if (parseOperand(scan, op1) && op1.hasError())
        return setError(op1);
    const auto op1p(scan);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, op2) && op2.hasError())
            return setError(op2);
        scan.skipSpaces();
    }
    if (!endOfLine(scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(op1);
    setErrorIf(op2);

    insn.setAddrMode(op1.mode, op2.mode);
    const auto error = TableCdp1802::TABLE.searchName(insn);
    if (error)
        return setError(op1, error);

    emitOperand(insn, insn.mode1(), op1);
    if (insn.mode2() == M_ADDR)
        insn.emitUint16(op2.val16);
    return setErrorIf(insn);
}

}  // namespace cdp1802
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
