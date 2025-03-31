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

#include "asm_mc6805.h"
#include "reg_mc6805.h"
#include "table_mc6805.h"
#include "text_common.h"

namespace libasm {
namespace mc6805 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr char OPT_INT_PCBITS[]  PROGMEM = "pc-bits";
constexpr char OPT_DESC_PCBITS[] PROGMEM = "program counter width in bit, default 13";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_FCB, &Assembler::defineDataConstant, Assembler::DATA_BYTE_NO_STRING},
    {TEXT_FCC, &Assembler::defineString},
    {TEXT_FDB, &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING},
    {TEXT_RMB, &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmMc6805::defaultPlugins() {
    return ValueParser::Plugins::motorola();
}

AsmMc6805::AsmMc6805(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_pc_bits),
      Config(TABLE),
      _opt_pc_bits(this, &AsmMc6805::setPcBits, OPT_INT_PCBITS, OPT_DESC_PCBITS) {
    reset();
}

void AsmMc6805::reset() {
    Assembler::reset();
    setPcBits(0);
}

AddressWidth AsmMc6805::addressWidth() const {
    return is08() ? ADDRESS_16BIT : AddressWidth(_pc_bits == 0 ? 13 : _pc_bits);
}

Error AsmMc6805::setPcBits(int32_t value) {
    if (value >= 0 && value <= 16) {
        _pc_bits = value;
        return OK;
    }
    return OVERFLOW_RANGE;
}

Error AsmMc6805::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);

    if (p.expect('#')) {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_IMM;
        scan = p;
        return OK;
    }

    if (p.expect(',')) {
        if (parseRegName(p) == REG_X) {
            op.mode = M_IX0;
            scan = p;
            return OK;
        }
        return op.setError(scan, UNKNOWN_OPERAND);
    }
    auto r = p;
    if (parseRegName(r) == REG_X) {
        op.mode = r.expect('+') ? M_IX0P : M_REGX;
        scan = r;
        return OK;
    }

    op.size = 0;
    if (p.expect('<')) {
        op.size = 8;
    } else if (p.expect('>')) {
        op.size = 16;
    }
    const auto disp = p;
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();

    auto a = p;
    if (a.skipSpaces().expect(',')) {
        const auto reg = parseRegName(a.skipSpaces());
        if (reg == REG_X) {
            const auto plus = a.expect('+');
            if (op.size == 8) {
                op.mode = plus ? M_IX1P : M_IX1;
            } else if (op.size == 16) {
                op.mode = M_IX2;
            } else if (op.val.isZero()) {
                op.mode = plus ? M_IX0P : M_IX0;
            } else if (!op.val.overflow(UINT8_MAX)) {
                op.mode = plus ? M_IX1P : M_IX1;
            } else {
                op.mode = M_IX2;
            }
            if (plus && op.mode == M_IX2)
                return op.setError(disp, OVERFLOW_RANGE);
            scan = a;
            return OK;
        }
        if (reg == REG_SP) {
            if (op.size == 8) {
                op.mode = M_SP1;
            } else if (op.size == 16) {
                op.mode = M_SP2;
            } else {
                op.mode = op.val.overflow(UINT8_MAX) ? M_SP2 : M_SP1;
            }
            scan = a;
            return OK;
        }
    }
    if (op.size == 0) {
        if (op.val.overflow(UINT8_MAX)) {
            op.mode = M_EXT;
        } else if (op.val.overflow(7)) {
            op.mode = M_DIR;
        } else {
            op.mode = M_BNO;
        }
    } else {
        op.mode = (op.size == 8) ? M_DIR : M_EXT;
    }
    scan = p;
    return OK;
}

void AsmMc6805::emitRelative(AsmInsn &insn, const Operand &op) const {
    const auto base = insn.address() + insn.length() + (insn.length() == 0 ? 2 : 1);
    const auto target = op.getError() ? base : op.val.getUnsigned();
    const auto delta = branchDelta(base, target, insn, op);
    if (overflowDelta(delta, 8))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand8(delta);
}

void AsmMc6805::emitBitNumber(AsmInsn &insn, const Operand &op) const {
    const uint8_t imm = 1U << (op.val.getUnsigned() & 7);
    const auto aim = (insn.opCode() & 0xF) == 1;
    insn.emitOperand8(aim ? ~imm : imm);
}

void AsmMc6805::emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_IX10:
        if (op.mode == M_IX0) {
            insn.embed(0x10);
            break;
        }
        goto idx;
    case M_GEN1:
        if (op.mode == M_IMM)
            goto imm;
        insn.embed(0x10);
        goto dir;
    case M_GEN2:
        switch (op.mode) {
        case M_EXT:
            goto ext;
        case M_IX2:
            insn.embed(0x10);
            goto ix2;
        case M_IX1:
            insn.embed(0x20);
            goto idx;
        default:  // M_IX0
            insn.embed(0x30);
            break;
        }
        break;
    case M_DIR:
    dir:
    case M_IX1:
    case M_IX1P:
    case M_SP1:
    idx:
        if (op.val.overflow(UINT8_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_IX2:
    case M_SP2:
    case M_IM16:
    ix2:
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_EXT:
    ext:
        if (checkAddr(op.val.getUnsigned()))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_REL:
        return emitRelative(insn, op);
    case M_SIM8:
        if (op.val.overflowInt8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        // Fall-through
    case M_IMM:
    imm:
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_BNO:   // handled in encodeImpl(Insn)
    case M_IX0P:  // silently ignore
    case M_REGX:  // silently ignore
    default:
        break;
    }
}

Error AsmMc6805::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (hasOperand(cpuType(), insn)) {
        if (parseOperand(scan, insn.op1) && insn.op1.hasError())
            return _insn.setError(insn.op1);
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.op2) && insn.op2.hasError())
                return _insn.setError(insn.op2);
        }
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.op3) && insn.op3.hasError())
                return _insn.setError(insn.op3);
        }
    }
    scan.skipSpaces();

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.op1, insn);

    if (insn.mode1() == M_BNO)
        insn.embed((insn.op1.val.getUnsigned() & 7) << 1);
    emitOperand(insn, insn.mode1(), insn.op1);
    emitOperand(insn, insn.mode2(), insn.op2);
    emitOperand(insn, insn.mode3(), insn.op3);
    insn.emitInsn();
    return _insn.setError(insn);
}

void AsmInsn::emitInsn() {
    uint_fast8_t pos = 0;
    if (hasPrefix())
        emitByte(prefix(), pos++);
    emitByte(opCode(), pos++);
}

}  // namespace mc6805
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
