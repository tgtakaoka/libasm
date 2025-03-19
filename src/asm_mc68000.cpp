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

#include "asm_mc68000.h"
#include <stdlib.h>
#include "table_mc68000.h"
#include "text_mc68000.h"

namespace libasm {
namespace mc68000 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::option;

using text::mc68000::TEXT_FMOVE;
using text::mc68000::TEXT_FPU_68881;
using text::mc68000::TEXT_FPU_MC68881;

namespace {

// clang-format off
constexpr char TEXT_DC_B[] PROGMEM = "dc.b";
constexpr char TEXT_DC_D[] PROGMEM = "dc.d";
constexpr char TEXT_DC_L[] PROGMEM = "dc.l";
constexpr char TEXT_DC_P[] PROGMEM = "dc.p";
constexpr char TEXT_DC_S[] PROGMEM = "dc.s";
constexpr char TEXT_DC_W[] PROGMEM = "dc.w";
constexpr char TEXT_DC_X[] PROGMEM = "dc.x";
constexpr char TEXT_DS_B[] PROGMEM = "ds.b";
constexpr char TEXT_DS_L[] PROGMEM = "ds.l";
constexpr char TEXT_DS_W[] PROGMEM = "ds.w";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_dALIGN, &Assembler::alignOrigin},
    {TEXT_DC,     &Assembler::defineDataConstant, Assembler::DATA_WORD|Assembler::DATA_ALIGN2},
    {TEXT_DC_B,   &Assembler::defineDataConstant, Assembler::DATA_BYTE},
#if !defined(LIBASM_MC68000_NOFPU)
    {TEXT_DC_D,   &Assembler::defineDataConstant, Assembler::DATA_FLOAT64|Assembler::DATA_ALIGN2},
#endif
    {TEXT_DC_L,   &Assembler::defineDataConstant, Assembler::DATA_LONG|Assembler::DATA_ALIGN2},
#if !defined(LIBASM_MC68000_NOFPU)
    {TEXT_DC_S,   &Assembler::defineDataConstant, Assembler::DATA_FLOAT32|Assembler::DATA_ALIGN2},
#endif
    {TEXT_DC_W,   &Assembler::defineDataConstant, Assembler::DATA_WORD|Assembler::DATA_ALIGN2},
    {TEXT_DS,     &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DS_B,   &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DS_L,   &Assembler::allocateSpaces,     Assembler::DATA_LONG|Assembler::DATA_ALIGN2},
    {TEXT_DS_W,   &Assembler::allocateSpaces,     Assembler::DATA_WORD|Assembler::DATA_ALIGN2},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmMc68000::defaultPlugins() {
    static const struct final : ValueParser::MotorolaPlugins {
        const SymbolParser &symbol() const override { return Mc68000SymbolParser::singleton(); }
        const LetterParser &letter() const override { return Plugins::letter(); }
        const OperatorParser &operators() const override {
            return CStyleOperatorParser::singleton();
        }
    } PLUGINS{};
    return PLUGINS;
}

AsmMc68000::AsmMc68000(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_fpu),
      Config(TABLE),
      _opt_fpu(this, &Config::setFpuName, OPT_TEXT_FPU, OPT_DESC_FPU) {
    reset();
}

void AsmMc68000::reset() {
    Assembler::reset();
    setFpuType(FPU_NONE);
}

namespace {

int8_t modePos(OprPos pos) {
    static constexpr int8_t BITNO[] PROGMEM = {
            3,   // OP_10 = 0: __|___|___|mmm|rrr
            6,   // OP_23 = 1: __|rrr|mmm|___|___
            -1,  // OP__0 = 2: __|___|___|___|rrr
            -1,  // OP__3 = 3: __|rrr|___|___|___
    };
    if (pos < OP___)
        return static_cast<int8_t>(pgm_read_byte(&BITNO[pos]));
    return -1;
}

int8_t regPos(OprPos pos) {
    static constexpr int8_t BITNO[] PROGMEM = {
            0,  // OP_10 = 0: __|___|___|mmm|rrr
            9,  // OP_23 = 1: __|rrr|mmm|___|___
            0,  // OP__0 = 2: __|___|___|___|rrr
            9,  // OP__3 = 3: __|rrr|___|___|___
    };
    if (pos < OP___)
        return static_cast<int8_t>(pgm_read_byte(&BITNO[pos]));
    return -1;
}

int8_t postPos(OprPos pos) {
    static constexpr int8_t BITNO[] PROGMEM = {
            10,  // EX_RX = 5:   __|xxx|___|_______ : format or source
            7,   // EX_RY = 6:   __|___|yyy|_______ : destination regi
            7,   // EX_SC = 7:   __|___|sss|____ccc : FPSINCOS
            0,   // EX_SL = 8:   __|___|__|ffffffff : static register
            4,   // EX_DL = 9:   __|___|___|rrr____ : dynamic register
            0,   // EX_SK = 10:  __|___|___|kkkkkkk : static k-factor
            4,   // EX_DK = 11:  __|___|___|rrr____ : dynamic k-factor
            12,  // EX_GR = 12:  a|rrr|____________ : Dn/An
            0,   // EX_RC = 13:  _____|cccccccccccc : Control register
            0,   // EX_OW = 14:  _____|dooooodwwwww : Bitfield offset
            0,   // EX_DC = 15:  _______________ccc : Dc
            6,   // EX_DU = 16:  _______uuu|___|___ : Du
            12,  // EX_DR = 17:  _|ddd|____________ : Dn
            0,   // EX_QR = 18:  _|qqq|________|rrr : Dr:Dq
            0,   // EX_QQ = 19:  _|qqq|________|qqq : Dq
            0,   // EX_DCP = 20: ______________|ccc : Dc1:Dc2 for CAS2
            6,   // EX_DUP = 21: _______uuu|___|___ : Du1:Du2 for CAS2
    };
    if (pos < EX_RX)
        return -1;
    return static_cast<int8_t>(pgm_read_byte(&BITNO[pos - EX_RX]));
}

bool checkFloatControlMode(AsmInsn &insn, const Operand &op) {
    const auto src = insn.src();
    const auto &ea = (src == op.mode || src == M_FCMLT) ? insn.dstOp : insn.srcOp;
    if (op.mode != M_FPIAR && ea.mode == M_AREG)
        insn.setErrorIf(ea, ILLEGAL_OPERAND_MODE);
    return src == M_FCMLT || insn.dst() == M_FCMLT;
}

}  // namespace

void emitOprSize(AsmInsn &insn, InsnSize isize) {
    const auto osize = insn.oprSize();
    if (osize == SZ_DATA || osize == SZ_DATH) {
        const auto pos = (osize == SZ_DATA) ? 6 : 9;
        switch (isize) {
        case ISZ_NONE:
        case ISZ_WORD:
            insn.embed(1 << pos);
            break;
        case ISZ_LONG:
            insn.embed(2 << pos);
            break;
        default:  // ISZ_BYTE
            break;
        }
        return;
    }
    if (osize == SZ_ADDR) {
        if (isize == ISZ_LONG)
            insn.embed(1 << 6);
        return;
    }
    if (osize == SZ_ADR8) {
        if (isize == ISZ_LONG)
            insn.embed(1 << 8);
        return;
    }
    if (osize == SZ_FDAT) {
        // clang-format off
        static constexpr uint8_t FSIZES[] PROGMEM = {
            // B  W  L  _  _  _  _  S  D  X  P  _
            0, 6, 4, 0, 0, 0, 0, 0, 1, 5, 2, 3, 0,
        };
        // clang-format on
        const auto fsize = pgm_read_byte(&FSIZES[isize]);
        insn.embedPostfix(fsize << 10);
        return;
    }
    if (osize == SZ_CAS1 || osize == SZ_CAS2) {
        switch (isize) {
        case ISZ_BYTE:
            insn.embed(1 << 9);
            break;
        default:
            insn.embed(2 << 9);
            break;
        case ISZ_LONG:
            insn.embed(3 << 9);
            break;
        }
        return;
    }
}

Error AsmMc68000::checkAlignment(
        AsmInsn &insn, OprSize size, const Value &value, const ErrorAt &at) const {
    const auto val32 = value.getUnsigned();
    if (size == SZ_WORD && (val32 % 2))
        return insn.setError(at, OPERAND_NOT_ALIGNED);
    if (size == SZ_LONG && (val32 % 2))
        return insn.setError(at, OPERAND_NOT_ALIGNED);
    return OK;
}

namespace {
Value calcDisplacement(const AsmInsn &insn, RegName baseReg, const Displacement &disp) {
    if (baseReg != REG_PC)
        return disp.value;
    auto len = insn.length();
    if (len == 0)
        len = sizeof(Config::opcode_t);
    if (insn.hasPostVal() && len < 4)
        len = 4;
    Value base;
    base.setUnsigned(insn.address() + len);
    return disp.value.isUndefined() ? Value() : disp.value - base;
}
}  // namespace

void AsmMc68000::encodeBriefExtension(AsmInsn &insn, const Addressing &addr) const {
    const auto disp = calcDisplacement(insn, addr.base, addr.disp);
    if (disp.overflowInt8())
        insn.setErrorIf(addr.disp.at, OVERFLOW_RANGE);
    uint16_t ext = static_cast<uint8_t>(disp.getSigned());
    auto index = addr.index.reg;
    if (index == REG_UNDEF)
        index = REG_D0;
    ext |= encodeGeneralRegNo(index) << 12;
    if (isAddrReg(addr.index.reg))
        ext |= (1 << 15);
    if (addr.index.size == ISZ_LONG)
        ext |= (1 << 11);
    if (addr.index.scale >= SCALE_1) {
        if (firstGen())
            insn.setErrorIf(addr.index.scaleAt, ADDRESSING_NOT_ALLOWED);
        ext |= (addr.index.scale << 9);
    }
    insn.emitOperand16(ext);
}

namespace {
uint_fast8_t displacementSize(const Displacement &disp) {
    if (disp.none)
        return 1;  // null displacement
    return disp.value.overflowInt16() ? 3 : 2;
}
}  // namespace

void AsmMc68000::encodeFullExtension(AsmInsn &insn, const Addressing &addr) const {
    auto ext = UINT16_C(0x0100);
    if (addr.base == REG_UNDEF)
        ext |= 0x80;  // base suppress
    if (addr.index.reg == REG_UNDEF) {
        ext |= 0x40;  // index suppress
    } else {
        if (addr.base == REG_UNDEF && firstGen())
            insn.setErrorIf(addr.index.at, ADDRESSING_NOT_ALLOWED);
        ext |= encodeGeneralRegNo(addr.index.reg) << 12;
        if (isAddrReg(addr.index.reg))
            ext |= (1 << 15);
        if (addr.index.size == ISZ_LONG)
            ext |= (1 << 11);
        if (addr.index.scale >= SCALE_1) {
            if (firstGen())
                insn.setErrorIf(addr.index.scaleAt, ADDRESSING_NOT_ALLOWED);
            ext |= (addr.index.scale << 9);
        }
    }
    auto baseDisp = addr.disp;
    if (!baseDisp.none) {
        baseDisp.value = calcDisplacement(insn, addr.base, addr.disp);
        if (baseDisp.value.isZero())
            baseDisp.none = true;
    }
    const auto bdSize = displacementSize(baseDisp);
    const auto odSize = displacementSize(addr.outer);
    ext |= bdSize << 4;
    if (addr.indirect) {
        ext |= odSize;
        if (addr.index.post)
            ext |= 4;
    }
    insn.emitOperand16(ext);
    if (bdSize == 2) {
        insn.emitOperand16(baseDisp.value.getUnsigned());
    } else if (bdSize == 3) {
        insn.emitOperand32(baseDisp.value.getUnsigned());
    }
    if (odSize == 2) {
        insn.emitOperand16(addr.outer.value.getUnsigned());
    } else if (odSize == 3) {
        insn.emitOperand32(addr.outer.value.getUnsigned());
    }
}

int_fast8_t AsmMc68000::encodeModeReg(AsmInsn &insn, AddrMode mode, RegName reg, OprPos pos) const {
    const auto mode_gp = modePos(pos);
    if (mode_gp >= 0) {
        const auto m = encodeAddrMode(mode);
        insn.embed(m << mode_gp);
    }

    const auto reg_gp = regPos(pos);
    if (reg_gp >= 0) {
        if (reg == REG_UNDEF)  // if base register is suppressed
            reg = REG_A0;
        const auto r = encodeRegNo(mode, reg);
        insn.embed(r << reg_gp);
    }

    const auto post_gp = postPos(pos);
    if (post_gp >= 0) {
        insn.embedPostfix(encodeGeneralRegNo(reg) << post_gp);
        if (pos == EX_GR && isAddrReg(reg))
            insn.embedPostfix(0x8000);
        if (pos == EX_QQ)
            insn.embedPostfix(reg << 12);
    }
    return post_gp;
}

void AsmMc68000::encodeEffectiveAddress(AsmInsn &insn, const Operand &op, OprPos pos) const {
    if (op.mode == M_DISP || op.mode == M_PCDSP) {
        const auto disp = calcDisplacement(insn, op.addr.base, op.addr.disp);
        if (firstGen() || !disp.overflowInt16()) {
            encodeModeReg(insn, op.mode, op.addr.base, pos);
            if (disp.overflowInt16())
                insn.setErrorIf(op.addr.disp.at, OVERFLOW_RANGE);
            insn.emitOperand16(disp.getUnsigned());
            return;
        }
    }
    if (firstGen() || isAddrReg(op.addr.base) || op.addr.base == REG_PC) {
        const auto disp = calcDisplacement(insn, op.addr.base, op.addr.disp);
        if (firstGen() || (!op.addr.indirect && !disp.overflowInt8())) {
            if (op.addr.indirect)
                insn.setErrorIf(op.addr.indirectAt, ADDRESSING_NOT_ALLOWED);
            if (op.addr.base == REG_UNDEF)
                insn.setErrorIf(op.addr.index.at, ADDRESSING_NOT_ALLOWED);
            encodeModeReg(insn, op.mode, op.addr.base, pos);
            encodeBriefExtension(insn, op.addr);
            return;
        }
    }
    const auto mode = (op.addr.base == REG_PC) ? M_PCIDX : M_INDX;
    encodeModeReg(insn, mode, op.addr.base, pos);
    encodeFullExtension(insn, op.addr);
}

AddrMode AsmMc68000::branchType(AddrMode mode, InsnSize size, Config::ptrdiff_t delta) const {
    if (mode == M_REL8) {
        if (size == ISZ_NONE) {
            if (!overflowDelta(delta, 8))
                return M_REL8;
            if (!overflowDelta(delta, 16))
                return M_REL16;
            return hasLongBranch() ? M_REL32 : M_REL8;
        }
        // Mnemonics has suffix 'B' or 'S'
        if (size == ISZ_BYTE || size == ISZ_SNGL)
            return M_REL8;
        // Mnemonics has suffix 'W' or 'L'
        if (size == ISZ_WORD || size == ISZ_LONG)
            return M_REL16;
        // Mnemonics has suffix 'X'
        if (size == ISZ_XTND && hasLongBranch())
            return M_REL32;
    } else if (mode == M_REL16) {
        // Mnemonics has suffix 'W' or 'L'
        if (size == ISZ_NONE || size == ISZ_WORD || size == ISZ_LONG)
            return M_REL16;
    } else {  // mode == M_REL32
        if (size == ISZ_NONE)
            return overflowDelta(delta, 16) ? M_REL32 : M_REL16;
        // Mnemonics has suffix 'W' or 'L'
        if (size == ISZ_WORD || size == ISZ_LONG)
            return M_REL16;
        // Mnemonics has suffix ''X'
        if (size == ISZ_XTND)
            return M_REL32;
    }
    return M_NONE;
}

void AsmMc68000::encodeRelativeAddr(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    // FDBcc has different base address
    const auto FDBcc = insn.hasPostVal();
    const Config::uintptr_t base = insn.address() + (FDBcc ? 4 : 2);
    const Config::uintptr_t target = op.getError() ? base : op.val.getUnsigned();
    insn.setErrorIf(op, checkAddr(target, true));
    Config::ptrdiff_t delta = target - base;
    // Catch overwrap case
    if ((delta < 0 && target >= base) || (delta >= 0 && target < base))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    auto type = branchType(mode, insn.insnSize(), delta);
    if (type == M_NONE) {
        insn.setErrorIf(op, ILLEGAL_SIZE);
        type = mode;
    }
    if (type == M_REL8) {
        if (overflowDelta(delta, 8))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        if (!firstGen() && delta == -1)
            delta = 0;
        if (delta == 0) {
            insn.emitOperand16(delta);
        } else {
            insn.embed(static_cast<uint8_t>(delta));
        }
    } else if (type == M_REL16) {
        if (overflowDelta(delta, 16))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand16(delta);
    } else {  // type == M_REL32
        insn.emitOperand32(delta);
        if (mode == M_REL8) {
            insn.embed(0xFF);
        } else if (mode == M_REL32) {
            insn.embed(1 << 6);  // set SIZE bit
        }
    }
}

void AsmMc68000::encodeImmediate(AsmInsn &insn, const Operand &op, OprSize size) const {
    const auto val32 = op.val.getUnsigned();
    switch (size) {
    case SZ_LONG:
        if (op.val.overflowUint32())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand32(val32);
        break;
    case SZ_WORD:
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(val32);
        break;
    case SZ_BYTE:
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(static_cast<uint8_t>(val32));
        break;
#if defined(LIBASM_ASM_NOFLOAT) || defined(LIBASM_MC68000_NOFPU)
    case SZ_SNGL:
        insn.setErrorIf(op, FLOAT_NOT_SUPPORTED);
        insn.emitUint32(0, insn.operandPos());
        break;
    case SZ_DUBL:
        insn.setErrorIf(op, FLOAT_NOT_SUPPORTED);
        insn.emitUint64(0, insn.operandPos());
        break;
    case SZ_XTND:
    case SZ_PBCD:
        insn.setErrorIf(op, FLOAT_NOT_SUPPORTED);
        insn.emitUint32(0, insn.operandPos());
        insn.emitUint64(0, insn.operandPos());
        break;
#else
    case SZ_SNGL:
        insn.emitFloat32(op.val.getFloat(), insn.operandPos());
        break;
    case SZ_DUBL:
        insn.emitFloat64(op.val.getFloat(), insn.operandPos());
        break;
    case SZ_XTND:
        insn.emitExtendedReal(op.val.getFloat(), insn.operandPos());
        break;
    case SZ_PBCD: {
        insn.emitDecimalString(op.val.getFloat(), insn.operandPos());
        break;
    }
#endif
    default:
        break;
    }
}

void AsmMc68000::encodeAbsoluteMode(
        AsmInsn &insn, OprSize size, OprPos pos, const Addressing &addr, const ErrorAt &at) const {
    auto addrSize = addr.absSize;
    auto abs = addr.disp.value.getSigned();
    const auto max = (UINT32_C(1) << addressWidth()) - 1;
    if (!addr.disp.value.overflow(max))
        abs = signExtend(addr.disp.value.getUnsigned(), addressWidth());
    if (addrSize == ISZ_NONE)
        addrSize = overflowDelta(abs, 16) ? ISZ_LONG : ISZ_WORD;
    if (addrSize == ISZ_WORD && overflowDelta(abs, 16))
        insn.setErrorIf(at, OVERFLOW_RANGE);
    encodeModeReg(insn, addrSize == ISZ_WORD ? M_AWORD : M_ALONG, REG_UNDEF, pos);
    insn.setErrorIf(at, checkAlignment(insn, size, addr.disp.value, at));
    if (addrSize == ISZ_WORD) {
        insn.emitOperand16(abs);
    } else {
        insn.emitOperand32(addr.disp.value.getUnsigned());
    }
}

void AsmMc68000::encodeBitField(AsmInsn &insn, const Operand &op) const {
    if (op.reg != REG_UNDEF) {
        insn.embedPostfix((040 | encodeGeneralRegNo(op.reg)) << 6);
    } else {
        insn.embedPostfix((op.val.getUnsigned() & 037) << 6);
    }
    if (op.reg2 != REG_UNDEF) {
        insn.embedPostfix(040 | encodeGeneralRegNo(op.reg2));
    } else {
        insn.embedPostfix(op.val2.getUnsigned() & 037);
    }
}

void AsmMc68000::encodeDataRegPair(AsmInsn &insn, const Operand &op, OprPos pos) const {
    const auto reg1 = encodeGeneralRegNo(op.reg);
    const auto reg2 = encodeGeneralRegNo(op.reg2);
    if (pos == EX_QR) {
        insn.embedPostfix(reg1);
        insn.embedPostfix(reg2 << 12);
    } else if (pos == EX_DCP) {
        insn.embedPostfix(reg1);
        insn.post2 = reg2;
    } else if (pos == EX_DUP) {
        insn.embedPostfix(reg1 << 6);
        insn.post2 |= reg2 << 6;
    }
};

void AsmMc68000::encodePointerPair(AsmInsn &insn, const Operand &op) const {
    auto reg1 = encodeGeneralRegNo(op.reg);
    if (isAddrReg(op.reg))
        reg1 |= 8;
    auto reg2 = encodeGeneralRegNo(op.reg2);
    if (isAddrReg(op.reg2))
        reg2 |= 8;
    insn.embedPostfix(reg1 << 12);
    insn.post2 |= reg2 << 12;
    insn.emitOperand16(insn.post2);
};

namespace {
bool inFloatOperand(const AsmInsn &insn, AddrMode mode) {
    if (mode == M_RDATA) {
        const auto dst = insn.dst();
        // Dn,FPn || Dn,FPc:FPs || FTST Dn
        return dst == M_FPREG || dst == M_FSICO || dst == M_NONE;
    }
    if (mode == M_WDATA) {
        // FPn,Dn
        return insn.src() == M_FPREG;
    }
    return false;
}
}  // namespace

Error AsmMc68000::encodeOperand(
        AsmInsn &insn, OprSize size, const Operand &op, AddrMode mode, OprPos pos) const {
    if (mode == M_MULT)
        return insn.getError();
    switch (op.mode) {
    case M_DREG:
        if (mode >= M_RMEM) {
            insn.reset();
            return insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        }
        if (inFloatOperand(insn, mode)) {
            if (size == SZ_DUBL || size == SZ_XTND || size == SZ_PBCD)
                insn.setErrorIf(op, ILLEGAL_SIZE);
        }
        encodeModeReg(insn, op.mode, op.reg, pos);
        if (pos == EX_DL) {
            if (insn.dstPos() == EX_DL || insn.dstOp.mode != M_PDEC)
                insn.embedPostfix(0x1000);
        }
        break;
    case M_AREG: {
        if (mode == M_RDATA || mode >= M_WDATA) {
            insn.reset();
            return insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        }
        const auto post_gp = encodeModeReg(insn, op.mode, op.reg, pos);
        if (post_gp < 0 && size == SZ_BYTE) {
            // Address register can't be accessed as BYTE.
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        } else {
            // But "MOVES.B An,<ea>" is OK
        }
        break;
    }
    case M_AIND:
        encodeModeReg(insn, op.mode, op.addr.base, pos);
        break;
    case M_PINC:
        if (mode == M_JADDR || mode == M_DADDR || mode == M_BITFR || mode == M_BITFW) {
            insn.reset();
            return insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        }
        encodeModeReg(insn, op.mode, op.addr.base, pos);
        break;
    case M_PDEC:
        if (mode == M_JADDR || mode == M_IADDR || mode == M_BITFR || mode == M_BITFW) {
            insn.reset();
            return insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        }
        encodeModeReg(insn, op.mode, op.addr.base, pos);
        break;
    case M_PCDSP:
        if (mode == M_WADDR || mode == M_WDATA || mode == M_WMEM || mode == M_DADDR ||
                mode == M_BITFW) {
            insn.reset();
            return insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        }
        insn.setErrorIf(op, checkAlignment(insn, size, op.addr.disp.value, op));
        // Fall-through
    case M_DISP:
        encodeEffectiveAddress(insn, op, pos);
        break;
    case M_PCIDX:
        if (mode == M_WADDR || mode == M_WDATA || mode == M_WMEM || mode == M_DADDR ||
                mode == M_BITFW) {
            insn.reset();
            return insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        }
        // Fall-through
    case M_INDX:
        encodeEffectiveAddress(insn, op, pos);
        break;
    case M_AWORD:
    case M_ALONG:
        encodeAbsoluteMode(insn, size, pos, op.addr, op);
        break;
    case M_IMDAT:
        if (mode == M_IMBIT) {
            uint_fast8_t bit = op.val.getUnsigned();
            if (size == SZ_BYTE && op.val.overflow(7)) {
                insn.setErrorIf(insn.srcOp, ILLEGAL_BIT_NUMBER);
                bit &= 7;
            }
            if (insn.oprSize() == SZ_LONG && op.val.overflow(31)) {
                insn.setErrorIf(insn.srcOp, ILLEGAL_BIT_NUMBER);
                bit &= 0x1F;
            }
            insn.emitOperand16(bit);
            break;
        }
        if (mode == M_IM3) {
            if (pos == OP__3) {
                // ADDQ/SUBQ/shift/rotate
                // "Zero means 2^3" unsigned 3-bit.
                uint_fast8_t data = op.val.getUnsigned();
                if (op.val.overflow(8))
                    insn.setErrorIf(op, OVERFLOW_RANGE);
                if (op.val.isZero())
                    insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
                insn.embed((data & 7) << 9);  // 8 is encoded to 0.
            } else if (pos == OP__0) {
                // BKPT
                uint_fast8_t vector = op.val.getUnsigned();
                if (op.val.overflow(7))
                    insn.setErrorIf(op, OVERFLOW_RANGE);
                insn.embed(vector & 7);
            }
            break;
        }
        if (mode == M_IM8) {
            // Signed 8-bit.
            if (op.val.overflowInt8())
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.embed(static_cast<uint8_t>(op.val.getUnsigned()));
            break;
        }
        if (mode == M_IMROM) {
            uint_fast8_t crom = op.val.getUnsigned();
            if (op.val.overflow(0x7F))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.embedPostfix(crom & 0x7F);
            break;
        }
        if (mode == M_IMVEC) {
            uint_fast8_t vector = op.val.getUnsigned();
            if (op.val.overflow(15))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.embed(vector & 0xF);
            break;
        }
        if (mode == M_IMDSP) {
            if (op.val.overflowInt16())
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand16(op.val.getUnsigned());
            break;
        }
        /* Fall-through */
    case M_IMFLT:
        if (mode >= M_WADDR) {
            insn.reset();
            return insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        }
        encodeModeReg(insn, op.mode, REG_UNDEF, pos);
        encodeImmediate(insn, op, size);
        break;
    case M_LABEL:
        encodeRelativeAddr(insn, mode, op);
        break;
    case M_FPREG:
        if (mode == M_FPMLT) {
            encodeFloatRegisterList(insn, op);
        } else {
            const auto post_gp = encodeModeReg(insn, op.mode, op.reg, pos);
            if (post_gp >= 0 && insn.dst() == M_NONE) {
                // Float instruction which omits destination register
                const auto regy = encodeGeneralRegNo(op.reg) << postPos(EX_RY);
                insn.embedPostfix(regy);
            }
        }
        break;
    case M_FSICO:
        insn.embedPostfix(encodeGeneralRegNo(op.reg));                     // FPc
        insn.embedPostfix(encodeGeneralRegNo(op.reg2) << postPos(EX_SC));  // FPs
        break;
    case M_FPMLT:
        encodeFloatRegisterList(insn, op);
        break;
    case M_FPSR:
    case M_FPCR:
    case M_FPIAR:
        if (!checkFloatControlMode(insn, op))
            break;
        /* Fall-through */
    case M_FCMLT:
        encodeFloatControlList(insn, op);
        break;
    case M_USP:
    case M_CREG:
        if (postPos(pos) == 0)
            insn.embedPostfix(encodeControlRegNo(op.reg));
        break;
    case M_KFACT:
        insn.embedPostfix(op.val.getUnsigned() & 0x7F);
        break;
    case M_KDREG:
        insn.embedPostfix(0x1000 | (encodeGeneralRegNo(op.reg) << 4));
        break;
    case M_BITOW:
        encodeBitField(insn, op);
        break;
    case M_DPAIR:
        encodeDataRegPair(insn, op, pos);
        break;
    case M_PPAIR:
        encodePointerPair(insn, op);
        break;
    case M_NONE:
        if (mode == M_KFACT && insn.isOK())
            insn.embedPostfix(17);
        break;
    default:
        break;
    }
    return insn.getError();
}

void AsmMc68000::encodeFloatControlList(AsmInsn &insn, const Operand &op) const {
    auto p = op.list;
    uint_fast8_t n = 0;
    auto reg = REG_UNDEF;
    for (;;) {
        auto a = p;
        reg = parseRegName(a, parser());
        if (!isFloatControlReg(reg))
            insn.setErrorIf(p, REGISTER_NOT_ALLOWED);
        insn.embedPostfix(1 << encodeFloatControlRegPos(reg));
        ++n;
        if (a.skipSpaces().expect('/')) {
            p = a;
            continue;
        }
        if (*a != ',' && !endOfLine(a))
            insn.setErrorIf(a, UNKNOWN_OPERAND);
        break;
    }
    const auto &ea = insn.src() == M_FCMLT ? insn.dstOp : insn.srcOp;
    if (ea.mode == M_DREG || ea.mode == M_AREG || ea.mode == M_IMDAT)
        insn.setError(ea, ILLEGAL_OPERAND_MODE);
}

namespace {

void swapRegPos(uint_fast8_t &s, uint_fast8_t &e) {
    const auto t = s;
    s = e;
    e = t;
}

uint_fast8_t reverseUint8(uint_fast8_t bits) {
    bits = (bits & 0x55) << 1 | (bits & 0xAA) >> 1;
    bits = (bits & 0x33) << 2 | (bits & 0xCC) >> 2;
    return bits << 4 | bits >> 4;
}

uint_fast16_t reverseUint16(uint_fast16_t bits) {
    bits = (bits & 0x5555) << 1 | (bits & 0xAAAA) >> 1;
    bits = (bits & 0x3333) << 2 | (bits & 0xCCCC) >> 2;
    bits = (bits & 0x0F0F) << 4 | (bits & 0xF0F0) >> 4;
    return bits << 8 | bits >> 8;
}

}  // namespace

void AsmMc68000::encodeFloatRegisterList(AsmInsn &insn, const Operand &op) const {
    uint_fast8_t bits = 0;
    if (op.mode == M_FPREG) {
        bits = 1 << encodeFloatRegPos(op.reg);
    } else {
        auto p = op.list;
        for (;;) {
            auto a = p;
            const auto start = parseRegName(a, parser());
            if (!isFloatReg(start))
                insn.setErrorIf(p, REGISTER_NOT_ALLOWED);
            auto s = encodeFloatRegPos(start);
            auto e = s;
            if (a.skipSpaces().expect('-')) {
                p = a.skipSpaces();
                const auto last = parseRegName(a, parser());
                if (!isFloatReg(last))
                    insn.setErrorIf(p, REGISTER_NOT_ALLOWED);
                e = encodeFloatRegPos(last);
            }
            if (s > e)
                swapRegPos(s, e);
            for (auto i = s; i <= e; i++) {
                const uint_fast8_t bm = 1 << i;
                if (bits & bm)
                    insn.setErrorIf(p, DUPLICATE_REGISTER);
                bits |= bm;
            }
            if (a.skipSpaces().expect('/')) {
                p = a;
                continue;
            }
            if (*a != ',' && !endOfLine(a))
                insn.setErrorIf(a, UNKNOWN_OPERAND);
            break;
        }
    }
    const auto dstMode = insn.dstOp.mode;
    if (insn.src() == M_FPMLT && dstMode == M_PDEC)
        bits = reverseUint8(bits);
    insn.embedPostfix(bits);
    if (insn.dst() == M_FPMLT || dstMode != M_PDEC)
        insn.embedPostfix(0x1000);
}

void AsmMc68000::encodeRegisterList(AsmInsn &insn, const Operand &op, bool reverse) const {
    auto p = op.list;
    uint_fast16_t bits = 0;
    for (;;) {
        auto a = p;
        const auto start = parseRegName(a, parser());
        if (!isGeneralReg(start))
            insn.setErrorIf(p, REGISTER_NOT_ALLOWED);
        auto s = encodeGeneralRegPos(start);
        auto e = s;
        if (a.skipSpaces().expect('-')) {
            p = a.skipSpaces();
            const auto last = parseRegName(a, parser());
            if (!isGeneralReg(last))
                insn.setErrorIf(p, REGISTER_NOT_ALLOWED);
            e = encodeGeneralRegPos(last);
        }
        if (s > e)
            swapRegPos(s, e);
        for (auto i = s; i <= e; i++) {
            const uint_fast16_t bm = 1 << i;
            if (bits & bm)
                insn.setErrorIf(p, DUPLICATE_REGISTER);
            bits |= bm;
        }
        if (a.skipSpaces().expect('/')) {
            p = a;
            continue;
        }
        if (*a != ',' && !endOfLine(a))
            insn.setErrorIf(a, UNKNOWN_OPERAND);
        break;
    }
    if (reverse)
        bits = reverseUint16(bits);
    insn.emitOperand16(bits);
}

Error AsmMc68000::parseBitField(StrScanner &scan, Operand &op) const {
    auto p = scan;
    auto at = p.skipSpaces();
    if (p.expect('#')) {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        if (op.val.isUndefined()) {
            op.val.setSigned(17);
        } else if (op.val.overflow(63, -64)) {
            op.setErrorIf(at, OVERFLOW_RANGE);
            op.val.setSigned(17);
        }
        op.mode = M_KFACT;
    } else {
        op.reg = parseRegName(p, parser());
        if (isDataReg(op.reg)) {
            op.mode = M_KDREG;
        } else if (op.reg != REG_UNDEF) {
            return op.setErrorIf(at, REGISTER_NOT_ALLOWED);
        } else {
            op.val = parseInteger(p, op);
            if (op.hasError())
                return op.getError();
            if (op.val.overflow(31))
                return op.setErrorIf(p, ILLEGAL_BIT_NUMBER);
            op.mode = M_BITOW;
        }
        if (p.skipSpaces().expect(':')) {
            at = p.skipSpaces();
            op.reg2 = parseRegName(p, parser());
            if (isDataReg(op.reg2)) {
                op.mode = M_BITOW;
            } else if (op.reg2 != REG_UNDEF) {
                return op.setErrorIf(at, REGISTER_NOT_ALLOWED);
            } else {
                op.val2 = parseInteger(p, op);
                if (op.hasError())
                    return op.getError();
                if (op.val2.isUndefined())
                    op.val2.setUnsigned(32);
                if (op.val2.overflow(32, 1))
                    op.setErrorIf(at, OVERFLOW_RANGE);
                op.mode = M_BITOW;
            }
        } else if (op.mode == M_BITOW) {
            return op.setErrorIf(p, UNKNOWN_OPERAND);
        }
    }
    if (!p.skipSpaces().expect('}'))
        return op.setErrorIf(p, MISSING_CLOSING_PAREN);
    scan = p;
    return OK;
}

Error AsmMc68000::parseDisplacement(
        StrScanner &scan, Displacement &disp, ErrorAt &error, char delim) const {
    auto p = scan;
    disp.value = parseInteger(p, error, delim);
    if (error.hasError())
        return error.getError();
    scan = p;
    return OK;
}

Error AsmMc68000::parseIndexScale(
        StrScanner &scan, Indexing &index, ErrorAt &error, char delim) const {
    auto p = scan.skipSpaces();
    if (p.expect('*')) {
        ErrorAt err;
        const auto value = parseInteger(p, err, delim);
        if (err.getError())  // undefined sysmbol is error
            return error.setError(err);
        const auto scale = value.getUnsigned();
        if (scale == 1) {
            index.scale = SCALE_1;
        } else if (scale == 2) {
            index.scale = SCALE_2;
        } else if (scale == 4) {
            index.scale = SCALE_4;
        } else if (scale == 8) {
            index.scale = SCALE_8;
        } else {
            return error.setErrorIf(scan, ADDRESSING_NOT_ALLOWED);
        }
        index.scaleAt = scan;
        scan = p;
    }
    return OK;
}

Error AsmMc68000::parseIndexSuffix(
        StrScanner &scan, Indexing &index, ErrorAt &error, char delim) const {
    auto p = scan;
    index.size = parseIndexSize(p);
    if (index.size == ISZ_ERROR)
        return error.setErrorIf(p, ADDRESSING_NOT_ALLOWED);
    if (parseIndexScale(p, index, error, delim))
        return error.getError();
    scan = p;
    return OK;
}

#if defined(LIBASM_DEBUG_VALUE)
const char *Indexing::str() const {
    static char buf[10];
    StrBuffer out(buf, sizeof(buf));
    if (reg != REG_UNDEF) {
        outRegName(out, reg);
        if (size != ISZ_NONE)
            out.letter('.').letter(sizeSuffix(OprSize(size)));
        if (scale != SCALE_NONE) {
            const auto sc = UINT8_C(1) << scale;
            out.letter('*').dec(sc);
        }
    }
    return out.str();
}

const char *Displacement::str() const {
    static char buf[10];
    StrBuffer out(buf, sizeof(buf));
    if (none) {
        out.text("none");
    } else {
        out.hex(value.getUnsigned(), 8);
    }
    return out.str();
}

const char *Addressing::str() const {
    static char buf[128];
    StrBuffer out(buf, sizeof(buf));
    out.letter('(');
    if (indirect)
        out.letter('[');
    out.text("disp=").text(disp.str());
    if (base != REG_UNDEF)
        outRegName(out.letter(',').text("base="), base);
    if (index.reg != REG_UNDEF && !index.post)
        out.letter(',').text("pre=").text(index.str());
    if (indirect) {
        out.letter(']');
        if (index.reg != REG_UNDEF && index.post)
            out.letter(',').text("post=").text(index.str());
        out.letter(',').text("outer=").text(outer.str());
    } else {
        if (index.reg != REG_UNDEF && index.post)
            out.letter(',').text("index=").text(index.str());
    }
    out.letter(')');
    if (absSize != ISZ_NONE)
        out.letter('.').letter(sizeSuffix(OprSize(absSize)));
    return out.str();
}
#else
const char *Indexing::str() const {
    return TEXT_null;
}

const char *Displacement::str() const {
    return TEXT_null;
}

const char *Addressing::str() const {
    return TEXT_null;
}
#endif

Error AsmMc68000::parseAddressing(
        StrScanner &scan, Addressing &addr, ErrorAt &error, char delim) const {
    const auto indirect = (delim == ']');
    auto p = scan;
    do {
        const auto at = p.skipSpaces();
        const auto reg = parseRegName(p, parser());
        if (isAddrReg(reg)) {
            Indexing index(reg);
            if (parseIndexSuffix(p, index, error, delim))
                return error.getError();
            if (!addr.indirect && addr.base == REG_UNDEF && index.size != ISZ_WORD &&
                    index.scale == SCALE_NONE) {
                addr.base = reg;
            } else if (addr.index.reg == REG_UNDEF) {
                index.at = at;
                index.post = !indirect;
                addr.index = index;
            } else {
                return error.setError(at, REGISTER_NOT_ALLOWED);
            }
        } else if (isDataReg(reg)) {
            if (addr.index.reg == REG_UNDEF) {
                addr.index.at = at;
                addr.index.post = !indirect;
                addr.index.reg = reg;
                if (parseIndexSuffix(p, addr.index, error, delim))
                    return error.getError();
            } else {
                return error.setError(at, REGISTER_NOT_ALLOWED);
            }
        } else if (reg == REG_PC) {
            if (addr.base == REG_UNDEF) {
                addr.base = reg;
            } else {
                return error.setError(at, REGISTER_NOT_ALLOWED);
            }
        } else if (reg != REG_UNDEF) {
            return error.setError(at, REGISTER_NOT_ALLOWED);
        } else if (!indirect && !addr.indirect && p.expect('[')) {
            if (parseAddressing(p, addr, error, ']'))
                return error.getError();
            addr.indirectAt = at;
            addr.indirect = true;
        } else {
            auto &disp = addr.indirect ? addr.outer : addr.disp;
            if (!disp.none)
                return error.setErrorIf(at, UNKNOWN_OPERAND);
            if (parseDisplacement(p, disp, error, delim))
                return error.getError();
            disp.at = at;
            disp.none = false;
        }
    } while (p.skipSpaces().expect(','));
    if (!p.expect(delim)) {
        const auto code = indirect ? MISSING_CLOSING_BRACKET : MISSING_CLOSING_PAREN;
        return error.setErrorIf(p, code);
    }
    if (!indirect && !addr.indirect && addr.base == REG_UNDEF && addr.index.reg == REG_UNDEF &&
            !addr.disp.none) {
        addr.absSize = parseIndexSize(p.skipSpaces());
        if (addr.absSize == ISZ_ERROR)
            return error.setErrorIf(p, ADDRESSING_NOT_ALLOWED);
    }
    scan = p;
    return OK;
}

Error AsmMc68000::parsePointerPair(StrScanner &scan, Operand &op) const {
    auto p = scan;
    op.reg = parseRegName(p.skipSpaces(), parser());
    if (isGeneralReg(op.reg) && p.skipSpaces().expect(')') && p.skipSpaces().expect(':') &&
            p.skipSpaces().expect('(')) {
        auto at = p.skipSpaces();
        op.reg2 = parseRegName(p, parser());
        if (isGeneralReg(op.reg2)) {
            if (!p.skipSpaces().expect(')'))
                return op.setErrorIf(p, MISSING_CLOSING_PAREN);
            op.mode = M_PPAIR;
            scan = p;
            return OK;
        }
        return op.setErrorIf(at, REGISTER_NOT_ALLOWED);
    }
    return NOT_AN_EXPECTED;
}

namespace {
bool preDecrement(StrScanner &scan) {
    auto p = scan;
    if (p.expect('-') && p.expect('(')) {
        scan = --p;
        return true;
    }
    return false;
}
}  // namespace

Error AsmMc68000::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(op.list = p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#')) {
        op.val = parseExpr(p, op);
        if (op.hasError())
            return op.getError();
        if (op.val.isFloat()) {
            op.mode = M_IMFLT;
            scan = p;
            return op.setOK();
        }
        op.mode = M_IMDAT;
        scan = p;
        return OK;
    }
    if (p.expect('{')) {
        if (parseBitField(p, op))
            return op.getError();
        scan = p;
        return OK;
    }

    auto at = p;
    const auto pdec = preDecrement(p);
    if (p.expect('(')) {
        if (parsePointerPair(p, op) != NOT_AN_EXPECTED) {
            scan = p;
            return op.getError();
        }
        parseAddressing(p, op.addr, op, ')');
        if (op.hasError())
            return op.getError();
        if (!pdec)
            at = p;
        const auto pinc = p.expect('+');
        if (pdec || pinc) {
            if (pdec && pinc)
                return op.setErrorIf(at, UNKNOWN_OPERAND);
            if (op.addr.base == REG_UNDEF || op.addr.index.reg != REG_UNDEF || !op.addr.disp.none)
                return op.setErrorIf(at, UNKNOWN_OPERAND);
        }
        scan = p;
        if (!op.addr.indirect && op.addr.index.reg == REG_UNDEF) {
            if (op.addr.disp.none) {
                if (isAddrReg(op.addr.base)) {
                    if (pdec) {
                        op.mode = M_PDEC;
                    } else if (pinc) {
                        op.mode = M_PINC;
                    } else {
                        op.mode = M_AIND;
                    }
                    return OK;
                }
            } else if (op.addr.base == REG_UNDEF) {
                op.mode = M_ALONG;
                return OK;
            } else if (op.addr.base == REG_PC) {
                op.mode = M_PCDSP;
                return OK;
            } else if (isAddrReg(op.addr.base)) {
                op.mode = M_DISP;
                return OK;
            }
        }
        op.mode = (op.addr.base == REG_PC) ? M_PCIDX : M_INDX;
        scan = p;
        return OK;
    }

    auto a = p;
    op.reg = parseRegName(a, parser());
    if (op.reg != REG_UNDEF) {
        a.skipSpaces();
        if ((*a == '/' || *a == '-') &&
                (isGeneralReg(op.reg) || isFloatReg(op.reg) || isFloatControlReg(op.reg))) {
            while (*a != ',' && !endOfLine(a))
                ++a;
            if (isGeneralReg(op.reg)) {
                op.mode = M_MULT;
            } else if (isFloatReg(op.reg)) {
                op.mode = M_FPMLT;
            } else {
                op.mode = M_FCMLT;
            }
            scan = a;
            return op.getError();
        }
        if (isAddrReg(op.reg)) {
            op.mode = M_AREG;
        } else if (isDataReg(op.reg)) {
            if (a.skipSpaces().expect(':')) {
                op.reg2 = parseRegName(a, parser());
                if (!isDataReg(op.reg2))
                    return op.setErrorIf(p, UNKNOWN_OPERAND);
                op.mode = M_DPAIR;
            } else {
                op.mode = M_DREG;
            }
        } else if (isFloatReg(op.reg)) {
            if (a.skipSpaces().expect(':')) {
                op.reg2 = parseRegName(a, parser());
                if (!isFloatReg(op.reg2))
                    return op.setErrorIf(p, UNKNOWN_OPERAND);
                op.mode = M_FSICO;
            } else {
                op.mode = M_FPREG;
            }
        } else if (op.reg == REG_USP) {
            op.mode = M_USP;
        } else if (op.reg == REG_CCR) {
            op.mode = M_CCR;
        } else if (op.reg == REG_SR) {
            op.mode = M_SR;
        } else if (op.reg == REG_FPCR) {
            op.mode = M_FPCR;
        } else if (op.reg == REG_FPSR) {
            op.mode = M_FPSR;
        } else if (op.reg == REG_FPIAR) {
            op.mode = M_FPIAR;
        } else if (isControlReg(op.reg)) {
            op.mode = M_CREG;
        } else {
            return op.setError(p, REGISTER_NOT_ALLOWED);
        }
        scan = a;
        return OK;
    }
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_LABEL;
    scan = p;
    return OK;
}

InsnSize AsmInsn::parseInsnSize() {
    StrScanner p{name()};
    p.trimStart([](char c) { return c != '.'; });
    auto eos = const_cast<char *>(p.str());
    const auto isize = parseSize(p);
    *eos = 0;
    return _isize = isize;
}

#if !defined(LIBASM_ASM_NOFLOAT) && !defined(LIBASM_MC68000_NOFPU)

namespace {

uint64_t convert2pbcd(uint64_t bin, uint_fast8_t digits) {
    uint64_t pbcd = 0;
    uint_fast8_t shift = 0;
    while (digits-- > 0) {
        pbcd |= (bin % 10) << shift;
        bin /= 10;
        shift += 4;
    }
    return pbcd;
}

}  // namespace

Error AsmInsn::emitExtendedReal(const float80_t &value, uint_fast8_t pos) {
    emitUint16(value.tag(), pos);
    emitUint16(0, pos + 2);
    return emitUint64(value.significand(), pos + 4);
}

Error AsmInsn::emitDecimalString(const float80_t &value, uint_fast8_t pos) {
    if (value.isInf()) {
    infinity:
        const auto tag = (value.tag() & float80_t::SGN_MASK) | float80_t::EXP_MASK;
        emitUint16(tag, pos);
        emitUint16(0, pos += 2);
        emitUint64(0, pos += 2);
    } else if (value.isNan()) {
    not_a_number:
        const auto tag = (value.tag() & float80_t::SGN_MASK) | float80_t::EXP_MASK;
        emitUint16(tag, pos);
        emitUint16(0, pos += 2);
        emitUint64(UINT64_C(0xC000000000000000), pos += 2);
    } else {
        uint_fast16_t sign = value.isNegative() ? 0x8000 : 0x0000;
        int_fast16_t exp10;
        fixed64_t sig;
        auto exp = value.decompose(sig, exp10);
        const auto ndigits = 17;
        uint8_t digits[ndigits];
        if (sig.decompose(exp, digits, ndigits))
            ++exp10;
        if (exp >= 1000) {
            setErrorIf(OVERFLOW_RANGE);
            goto infinity;
        }
        if (exp <= -1000) {
            setErrorIf(UNDERFLOW_RANGE);
            goto not_a_number;
        }
        if (exp10 < 0) {
            sign |= 0x4000;
            exp10 = -exp10;
        }
        emitUint16(sign | convert2pbcd(exp10, 3), pos);
        emitByte(0, pos += 2);
        uint_fast8_t data = 0;
        for (auto i = 0; i < ndigits; ++i) {
            if (i % 2 == 0) {
                data |= digits[i];
                emitByte(data, ++pos);
            } else {
                data = digits[i] << 4;
            }
        }
    }
    return getError();
}

#endif

Error AsmMc68000::defineDataConstant(
        AsmInsn &insn, StrScanner &scan, Mc68881Type type, ErrorAt &_error) const {
    insn.insnBase().align(2);
    ErrorAt error;
    do {
        scan.skipSpaces();
        ErrorAt exprErr;
        auto p = scan;
        const auto val = parseExpr(p, exprErr);
        if (!endOfLine(p) && *p != ',') {
            error.setErrorIf(scan, ILLEGAL_CONSTANT);
            break;
        }
        if (exprErr.hasError()) {
            error.setErrorIf(exprErr);
            break;
        }
#if defined(LIBASM_MC68000_NOFPU)
        (void)val;
#elif defined(LIBASM_ASM_NOFLOAT)
        (void)val;
        exprErr.setErrorIf(scan, FLOAT_NOT_SUPPORTED);
        insn.emitUint32(0, insn.length());
        insn.emitUint64(0, insn.length());
#else
        if (type == DATA_DCX)
            exprErr.setErrorIf(scan, insn.emitExtendedReal(val.getFloat(), insn.length()));
        if (type == DATA_DCP)
            exprErr.setErrorIf(scan, insn.emitDecimalString(val.getFloat(), insn.length()));
#endif
        scan = p;
        if (error.setErrorIf(exprErr) == NO_MEMORY)
            break;
    } while (scan.skipSpaces().expect(',') && error.getError() != NO_MEMORY);
    return _error.setError(error);
}

Error AsmMc68000::processPseudo(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    const auto at = scan;
    if (strcasecmp_P(insn.name(), TEXT_FPU) == 0) {
        const auto error = _opt_fpu.set(scan);
        return error ? _insn.setErrorIf(at, error) : OK;
    }
#if !defined(LIBASM_MC68000_NOFPU)
    if (strcasecmp_P(insn.name(), TEXT_DC_X) == 0)
        return defineDataConstant(insn, scan, DATA_DCX, _insn);
    if (strcasecmp_P(insn.name(), TEXT_DC_P) == 0)
        return defineDataConstant(insn, scan, DATA_DCP, _insn);
#endif
    return Assembler::processPseudo(scan, _insn);
}

Error AsmMc68000::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    const auto isize = insn.parseInsnSize();
    if (isize == ISZ_ERROR)
        return _insn.setError(scan, ILLEGAL_SIZE);

    if (hasOperand(_cpuSpec, insn)) {
        if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
            return _insn.setError(insn.srcOp);
        if (scan.skipSpaces().expect(',') || *scan == '{') {
            if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
                return _insn.setError(insn.dstOp);
        }
        if (scan.skipSpaces().expect(',') || *scan == '{') {
            if (parseOperand(scan, insn.extOp) && insn.extOp.hasError())
                return _insn.setError(insn.extOp);
        }
    }
    scan.skipSpaces();

    if (searchName(_cpuSpec, insn))
        return _insn.setError(insn.srcOp, insn);

    insn.setErrorIf(insn.srcOp);
    insn.setErrorIf(insn.dstOp);
    insn.setErrorIf(insn.extOp);
    const auto src = insn.src();
    const auto dst = insn.dst();
    const auto ext = insn.ext();
    // Register list must be just after instruction code
    if (src == M_MULT)
        encodeRegisterList(insn, insn.srcOp, insn.dstOp.mode == M_PDEC);
    if (dst == M_MULT)
        encodeRegisterList(insn, insn.dstOp);
    const auto osize = (isize == ISZ_NONE) ? insn.oprSize() : OprSize(isize);
    emitOprSize(insn, isize);
    encodeOperand(insn, osize, insn.srcOp, src, insn.srcPos());
    encodeOperand(insn, osize, insn.dstOp, dst, insn.dstPos());
    encodeOperand(insn, osize, insn.extOp, ext, insn.extPos());
    insn.emitInsn();
    if (insn.getError() == ILLEGAL_SIZE)
        insn.setAt(_insn.errorAt());
    return _insn.setError(insn);
}

}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
