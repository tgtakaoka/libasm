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

#include "asm_i8086.h"
#include "table_i8086.h"
#include "text_i8086.h"

namespace libasm {
namespace i8086 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::i8086;
using namespace text::option;

// clang-format off
namespace {
constexpr char OPT_BOOL_OPTIMIZE_SEGMENT[] PROGMEM = "optimize-segment";
constexpr char OPT_DESC_OPTIMIZE_SEGMENT[] PROGMEM = "optimize segment override";
constexpr char OPT_BOOL_GNU_AS[] PROGMEM = "gnu-as";
constexpr char OPT_DESC_GNU_AS[] PROGMEM = "GNU assembler compatible encoding";

constexpr char TEXT_DQ[]   PROGMEM = "dq";
constexpr char TEXT_DT[]   PROGMEM = "dt";
constexpr char TEXT_RESB[] PROGMEM = "resb";
constexpr char TEXT_RESD[] PROGMEM = "resd";
constexpr char TEXT_RESW[] PROGMEM = "resw";
}  // namespace

constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_DB,   &Assembler::defineDataConstant, Assembler::DATA_BYTE },
    { TEXT_DS,   &Assembler::allocateSpaces,     Assembler::DATA_BYTE },
    { TEXT_DW,   &Assembler::defineDataConstant, Assembler::DATA_WORD },
    { TEXT_RESB, &Assembler::allocateSpaces,     Assembler::DATA_BYTE },
    { TEXT_RESD, &Assembler::allocateSpaces,     Assembler::DATA_LONG },
    { TEXT_RESW, &Assembler::allocateSpaces,     Assembler::DATA_WORD },
};
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

constexpr AsmI8086::PseudoI8086 AsmI8086::PSEUDO_I8086_TABLE[] PROGMEM = {
    { TEXT_DD,    &AsmI8086::defineConstant, DATA_DD       },
    { TEXT_DQ,    &AsmI8086::defineConstant, DATA_DQ       },
    { TEXT_DT,    &AsmI8086::defineConstant, DATA_DT       },
    { TEXT_FPU,   &AsmI8086::setCoprocessor },
};
PROGMEM constexpr AsmI8086::PseudosI8086 AsmI8086::PSEUDOS_I8086{ARRAY_RANGE(PSEUDO_I8086_TABLE)};

constexpr AsmI8086::PseudoI8086 AsmI8086::PREFIX_TABLE[] PROGMEM = {
    { TEXT_PRE_ADDR32, &AsmI8086::encodePrefix,  AsmInsn::ADDR32      },
    { TEXT_PRE_DATA32, &AsmI8086::encodePrefix,  AsmInsn::DATA32      },
    { TEXT_LOCK,       &AsmI8086::encodePrefix,  AsmInsn::LOCK_PREFIX },
    { TEXT_REP,        &AsmI8086::encodePrefix,  0xF3                 },
    { TEXT_REPC,       &AsmI8086::encodePrefix,  0x65,                },  //V30
    { TEXT_REPE,       &AsmI8086::encodePrefix,  0xF3                 },
    { TEXT_REPNC,      &AsmI8086::encodePrefix,  0x64,                },  //V30
    { TEXT_REPNE,      &AsmI8086::encodePrefix,  0xF2                 },
    { TEXT_REPNZ,      &AsmI8086::encodePrefix,  0xF2                 },
    { TEXT_REPZ,       &AsmI8086::encodePrefix,  0xF3                 },
};
PROGMEM constexpr AsmI8086::PseudosI8086 AsmI8086::PREFIXES{ARRAY_RANGE(PREFIX_TABLE)};

// clang-format on

struct I8086SymbolParser final : SimpleSymbolParser, Singleton<I8086SymbolParser> {
    I8086SymbolParser() : SimpleSymbolParser(PSTR_UNDER_AT_QUESTION) {}
};

const ValueParser::Plugins &AsmI8086::defaultPlugins() {
    static struct final : ValueParser::IntelPlugins {
        const SymbolParser &symbol() const { return I8086SymbolParser::singleton(); }
    } PLUGINS;
    return PLUGINS;
}

AsmI8086::AsmI8086(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_fpu),
      Config(TABLE),
      _opt_fpu(this, &Config::setFpuName, OPT_TEXT_FPU, OPT_DESC_FPU, &_opt_use16),
      _opt_use16(this, &Config::setUse16, OPT_BOOL_USE16, OPT_DESC_USE16, &_opt_use32),
      _opt_use32(this, &Config::setUse32, OPT_BOOL_USE32, OPT_DESC_USE32, &_opt_optimizeSegment),
      _opt_optimizeSegment(this, &AsmI8086::setOptimizeSegment, OPT_BOOL_OPTIMIZE_SEGMENT,
              OPT_DESC_OPTIMIZE_SEGMENT, &_opt_gnuAs),
      _opt_gnuAs(this, &AsmI8086::setGnuAs, OPT_BOOL_GNU_AS, OPT_DESC_GNU_AS) {
    reset();
}

void AsmI8086::reset() {
    Assembler::reset();
    setFpuType(FPU_NONE);
    setOptimizeSegment(true);
    setGnuAs(false);
}

Error AsmI8086::setOptimizeSegment(bool enable) {
    _optimizeSegment = enable;
    return OK;
}

Error AsmI8086::setGnuAs(bool enable) {
    _gnuAs = enable;
    return OK;
}

Error AsmI8086::parsePointerSize(StrScanner &scan, Operand &op) const {
    auto p = scan;
    const auto prefix = parsePrefixName(p, parser());
    if (prefix != PRE_UNDEF) {
        // Pointer size override
        if (parsePrefixName(p.skipSpaces(), parser()) == PRE_PTR) {
            op.ptr = prefix;
            scan = p.skipSpaces();
            return OK;
        }
        return op.setError(UNKNOWN_OPERAND);
    }
    return OK;
}

void AsmI8086::parseSegmentOverride(StrScanner &scan, Operand &op) const {
    auto p = scan;
    const auto reg = parseRegName(p, _cpuSpec, parser());
    if (isSegmentReg(reg, _cpuSpec)) {
        // Segment Override
        if (p.skipSpaces().expect(':')) {
            op.seg = reg;
            scan = p;
        }
    }
}

void AsmI8086::parseBaseRegister(StrScanner &scan, Operand &op) const {
    auto p = scan;
    const auto reg = parseRegName(p, _cpuSpec, parser());
    if (reg == REG_BX || reg == REG_BP) {
        op.reg = reg;
        scan = p.skipSpaces();
    } else if (reg >= REG_EAX && reg <= REG_EDI) {
        // 32-bit register: accept as base unless followed by '*' (scale factor means it's an index)
        auto q = p;
        if (*q.skipSpaces() != '*') {
            op.reg = reg;
            scan = p.skipSpaces();
        }
    }
}

void AsmI8086::parseIndexRegister(StrScanner &scan, Operand &op) const {
    auto p = scan;
    if (op.reg != REG_UNDEF) {
        if (!p.expect('+'))
            return;
        p.skipSpaces();
    }
    const auto reg = parseRegName(p, _cpuSpec, parser());
    if (reg == REG_SI || reg == REG_DI) {
        op.index = reg;
        scan = p.skipSpaces();
    } else if (reg >= REG_EAX && reg <= REG_EDI && reg != REG_ESP) {
        // 32-bit index register (ESP cannot be used as index)
        op.index = reg;
        auto q = p.skipSpaces();
        if (*q == '*') {
            q.expect('*');
            q.skipSpaces();
            const auto scaleP = q;
            // Parse scale as a single decimal integer literal (1, 2, 4, or 8).
            // Cannot use parseInteger() here: it evaluates expressions like "4+disp".
            const auto c = *q;
            const uint8_t s = c - '0';
            ++q;
            if (c < '1' || c > '9' || (s != 1 && s != 2 && s != 4 && s != 8)) {
                op.setError(scaleP, ILLEGAL_CONSTANT);
            } else {
                op.scale = s;
            }
            p = q;
        }
        scan = p.skipSpaces();
    }
}

Error AsmI8086::parseDisplacement(StrScanner &scan, Operand &op) const {
    auto p = scan;
    if (endOfLine(p) || *p == ']')
        return OK;
    if (op.reg != REG_UNDEF || op.index != REG_UNDEF) {
        if (*p != '+' && *p != '-')
            return op.setError(UNKNOWN_OPERAND);
    }
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.hasDisp = true;
    scan = p.skipSpaces();
    return OK;
}

namespace {
AddrMode pointerMode(PrefixName ptr, AddrMode undef, AddrMode byte, AddrMode word) {
    if (ptr == PRE_UNDEF)
        return undef;
    if (ptr == PRE_BYTE)
        return byte;
    if (ptr == PRE_WORD)
        return word;
    if (ptr == PRE_DWORD)
        return M_DMEM;
    // QWORD, TBYTE
    return M_FMEM;
}
}  // namespace

Error AsmI8086::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (parsePointerSize(p, op))
        return op.getError();
    parseSegmentOverride(p, op);
    if (p.skipSpaces().expect('[')) {
        parseBaseRegister(p.skipSpaces(), op);
        parseIndexRegister(p, op);
        if (parseDisplacement(p, op))
            return op.getError();
        if (!p.skipSpaces().expect(']'))
            return op.setError(p, MISSING_CLOSING_BRACKET);
        scan = p;
        if (op.reg == REG_UNDEF && op.index == REG_UNDEF) {
            if (!op.hasDisp)
                return op.setError(UNKNOWN_OPERAND);
            op.mode = pointerMode(op.ptr, M_DIR, M_BDIR, M_WDIR);
            return OK;
        }
        op.mode = pointerMode(op.ptr, M_MEM, M_BMEM, M_WMEM);
        return OK;
    }
    if (op.ptr != PRE_UNDEF || op.seg != REG_UNDEF)
        return op.setError(UNKNOWN_OPERAND);

    auto a = p;
    const auto reg = parseRegName(a, _cpuSpec, parser());
    if (isGeneralReg(reg, _cpuSpec)) {
        op.reg = reg;
        switch (reg) {
        case REG_AL:
            op.mode = M_AL;
            break;
        case REG_CL:
            op.mode = M_CL;
            break;
        case REG_AX:
            op.mode = M_AX;
            break;
        case REG_DX:
            op.mode = M_DX;
            break;
        default:
            op.mode = (generalRegSize(reg) == SZ_BYTE) ? M_BREG
                      : (generalRegSize(reg) == SZ_DWORD) ? M_DREG
                      : M_WREG;
            break;
        }
        scan = a;
        return OK;
    }
    if (isSegmentReg(reg, _cpuSpec)) {
        op.reg = reg;
        op.mode = (reg == REG_CS) ? M_CS
                : (reg == REG_FS) ? M_FS
                : (reg == REG_GS) ? M_GS
                                  : M_SREG;
        scan = a;
        return OK;
    }
    if (reg == REG_ST) {
        if (a.skipSpaces().expect('(')) {
            const auto valp = a;
            op.val = parseInteger(a, op, ')');
            if (op.hasError())
                return op.getError();
            if (a.skipSpaces().expect(')')) {
                if (op.val.overflow(7))
                    op.setErrorIf(valp, OVERFLOW_RANGE);
                op.mode = (op.isOK() && op.val.isZero()) ? M_ST0 : M_STI;
                scan = a;
                return OK;
            }
            return op.setErrorIf(a, MISSING_CLOSING_PAREN);
        }
        op.mode = M_ST0;
        scan = a;
        return OK;
    }
    if (reg >= REG_CR_BASE && reg < REG_CR_BASE + 4) {
        op.reg = reg;
        op.mode = M_CTLR;
        scan = a;
        return OK;
    }
    if (reg >= REG_DR_BASE && reg < REG_DR_BASE + 8) {
        op.reg = reg;
        op.mode = M_DBGR;
        scan = a;
        return OK;
    }
    if (reg >= REG_TR_BASE && reg < REG_TR_BASE + 8) {
        op.reg = reg;
        op.mode = M_TSTR;
        scan = a;
        return OK;
    }
    if (reg != REG_UNDEF)
        return op.setError(UNKNOWN_OPERAND);

    const auto valp = p.skipSpaces();
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    if (p.skipSpaces().expect(':')) {
        if (op.val.overflow(UINT16_MAX))
            return op.setError(valp, OVERFLOW_RANGE);
        op.segval = op.val;
        ErrorAt offset;
        offset.setAt(p);
        op.val = parseInteger(p, offset);
        if (op.hasError())
            return offset.getError();
        op.setErrorIf(offset);
        op.mode = M_FAR;
        scan = p;
        return OK;
    }
    op.mode = op.immediateMode();
    scan = p;
    return OK;
}

AddrMode Operand::immediateMode() const {
    if (getError())
        return M_WIMM;
    const auto v = val.getUnsigned();
    if (v == 1)
        return M_VAL1;
    if (v == 3)
        return M_VAL3;
    return val.overflowInt8() ? M_WIMM : M_BIMM;
}

void AsmI8086::emitImmediate(
        AsmInsn &insn, AddrMode mode, OprSize size, const Value &val, const ErrorAt &at) const {
    const auto imm = (mode == M_BIMM || mode == M_WIMM);
    if (size == SZ_BYTE) {
        if (imm && val.overflowUint8())
            insn.setErrorIf(at, OVERFLOW_RANGE);
        if (!imm && val.overflow(UINT8_MAX))
            insn.setErrorIf(at, OVERFLOW_RANGE);
        insn.emitOperand8(val.getUnsigned());
    } else if (size == SZ_DWORD ||
               (size == SZ_DATA && (insn.model32() ^ insn.hasData32Prefix()))) {
        if (imm && val.overflowUint32())
            insn.setErrorIf(at, OVERFLOW_RANGE);
        if (!imm && val.overflow(UINT32_MAX))
            insn.setErrorIf(at, OVERFLOW_RANGE);
        insn.emitOperand32(val.getUnsigned());
    } else if (size == SZ_WORD || size == SZ_DATA) {
        if (imm && val.overflowUint16())
            insn.setErrorIf(at, OVERFLOW_RANGE);
        if (!imm && val.overflow(UINT16_MAX))
            insn.setErrorIf(at, OVERFLOW_RANGE);
        insn.emitOperand16(val.getUnsigned());
    }
}

namespace {

constexpr Config::opcode_t JMP_rel8 = 0xEB;
constexpr Config::opcode_t JMP_rel16 = 0xE9;

bool is16BitReg(AddrMode m) {
    return m == M_WREG || m == M_AX || m == M_DX;
}

bool is16BitMem(AddrMode m) {
    return m == M_WMEM || m == M_WDIR;
}

bool isMemOperand(const Operand &op) {
    return op.mode == M_MEM || op.mode == M_BMEM || op.mode == M_WMEM ||
           op.mode == M_DMEM || op.mode == M_FMEM;
}

bool is16BitAddrReg(RegName r) {
    return r == REG_BX || r == REG_BP || r == REG_SI || r == REG_DI;
}

bool uses16BitAddressing(const Operand &op) {
    return isMemOperand(op) && (is16BitAddrReg(op.reg) || is16BitAddrReg(op.index));
}

bool uses32BitAddressing(const Operand &op) {
    return isMemOperand(op) && ((op.reg >= REG_EAX && op.reg <= REG_EDI) ||
                                       (op.index >= REG_EAX && op.index <= REG_EDI));
}

constexpr bool maySmartBranch(Config::opcode_t opc) {
    return opc == JMP_rel8;
}

constexpr bool isShortJcc(Config::opcode_t opc) {
    return (opc & 0xF0) == 0x70;
}

void shortBranch(AsmInsn &insn) {
    insn.setOpCode(JMP_rel8);
}

void longBranch(AsmInsn &insn, const Operand &op) {
    insn.setOpCode(JMP_rel16);
    insn.setError(op);
}

void longJcc(AsmInsn &insn) {
    insn.setPrefix(0x0F);
    insn.setOpCode(insn.opCode() + 0x10);  // 0x7n -> 0x0F 0x8n
}

constexpr Config::uintptr_t segment(Config::uintptr_t addr) {
    return addr & ~0xFFFF;
}

}  // namespace

void AsmI8086::emitRelative(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    // PC at end of instruction = address + prefix bytes + opcode + disp.
    const auto prefixLen =
            (insn.hasAddr32Prefix() ? 1 : 0) + (insn.hasData32Prefix() ? 1 : 0);
    const auto base = insn.address() + prefixLen + 2;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    // Segment-boundary check is meaningful only in use16 mode. Apply it
    // BEFORE branchDelta() — that helper also reports OVERFLOW_RANGE on
    // signed/unsigned wrap, and setErrorIf only honours the first error.
    if (!insn.model32())
        insn.setErrorIf(op, checkAddr(target, insn.address(), 16));
    const auto delta = branchDelta(base, target, insn, op);
    const auto smartBranch = maySmartBranch(insn.opCode());
    if (mode == M_REL8 && !smartBranch) {
        if (overflowDelta(delta, 8)) {
            if (_cpuSpec.has32bit() && isShortJcc(insn.opCode())) {
                longJcc(insn);
                const auto data32 = insn.model32() ^ insn.hasData32Prefix();
                const auto bits = data32 ? 32 : 16;
                // [DATA32?][0x0F][0x8n][dispN]: base is PC after instruction
                const auto lbase =
                        insn.address() + (insn.hasData32Prefix() ? 1 : 0) + 2 + bits / 8;
                const auto ltarget = op.getError() ? lbase : op.val.getUnsigned();
                const auto ldelta = branchDelta(lbase, ltarget, insn, op);
                if (overflowDelta(ldelta, bits))
                    insn.setErrorIf(op, OPERAND_TOO_FAR);
                if (bits == 32)
                    insn.emitOperand32(ldelta);
                else
                    insn.emitOperand16(ldelta);
                return;
            }
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        }
    short_branch:
        insn.emitOperand8(delta);
        return;
    }
    if (mode == M_REL && !smartBranch) {
    long_branch:
        // In use32 mode the relative displacement is 32-bit by default
        // (toggled to 16-bit by an explicit DATA32 prefix). In use16 the
        // reverse holds. Instruction length = prefix + 1 (opcode) + N/8 disp.
        const auto data32 = insn.model32() ^ insn.hasData32Prefix();
        const auto bits = data32 ? 32 : 16;
        const auto base = insn.address() + prefixLen + 1 + bits / 8;
        const auto target = op.getError() ? base : op.val.getUnsigned();
        const auto delta = branchDelta(base, target, insn, op);
        if (overflowDelta(delta, bits))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        if (bits == 32)
            insn.emitOperand32(delta);
        else
            insn.emitOperand16(delta);
        return;
    }
    if (op.getError() || overflowDelta(delta, 8)) {
        longBranch(insn, op);  // resets insn error to op's; re-check segment.
        if (!insn.model32())
            insn.setErrorIf(op, checkAddr(target, insn.address(), 16));
        goto long_branch;
    }
    shortBranch(insn);
    goto short_branch;
}

void AsmI8086::emitRegister(AsmInsn &insn, const Operand &op, OprPos pos) const {
    const auto num = encodeRegNum(op.reg);
    switch (pos) {
    case P_OREG:
        insn.embed(num);
        break;
    case P_OSEG:
        insn.embed(num << 3);
        break;
    case P_OMOD:
        insn.embed(0300 | num);
        break;
    case P_REG:
        insn.embedModReg(num << 3);
        break;
    case P_MOD:
        insn.embedModReg(0300 | num);
        break;
    case P_MREG:
        insn.embedModReg(0300 | (num << 3) | num);
        break;
    default:
        break;
    }
}

uint8_t Operand::encodeMod() const {
    const auto needDisp =
            (reg == REG_BP && index == REG_UNDEF) || (hasDisp && (!val.isZero() || getError()));
    if (needDisp)
        return val.overflowInt8() || getError() ? 2 : 1;
    return 0;
}

uint8_t Operand::encodeR_m() const {
    uint8_t r_m = 0;
    if (reg == REG_UNDEF) {
        r_m = (index == REG_SI) ? 4 : 5;
    } else if (index == REG_UNDEF) {
        r_m = (reg == REG_BP) ? 6 : 7;
    } else {
        if (reg == REG_BP)
            r_m |= 2;
        if (index == REG_DI)
            r_m |= 1;
    }
    return r_m;
}

uint8_t Operand::encodeMod32() const {
    if (reg == REG_UNDEF)
        return 0;  // no base: mod=0 (SIB.base=5 handles disp32)
    // EBP as base (with or without index): SIB.base=5 with mod=0 means disp32-only, so force mod>=1
    const auto ebpBase = (reg == REG_EBP);
    const auto needDisp = ebpBase || (hasDisp && (!val.isZero() || getError()));
    if (needDisp)
        return val.overflowInt8() || getError() ? 2 : 1;
    return 0;
}

uint8_t Operand::encodeR_m32() const {
    if (reg == REG_UNDEF && index == REG_UNDEF)
        return 5;  // mod=0+r/m=5: disp32 direct (no SIB)
    if (index != REG_UNDEF || reg == REG_ESP)
        return 4;  // SIB byte follows
    return encodeRegNum(reg);
}

uint8_t Operand::encodeSib32() const {
    const auto ss = (scale == 8) ? 3 : (scale == 4) ? 2 : (scale == 2) ? 1 : 0;
    const auto idx = (index == REG_UNDEF) ? 4 : encodeRegNum(index);
    const auto bas = (reg == REG_UNDEF) ? 5 : encodeRegNum(reg);
    return (ss << 6) | (idx << 3) | bas;
}

Config::opcode_t AsmI8086::encodeSegmentOverride(RegName seg, RegName base) const {
    if (seg == REG_UNDEF)
        return 0;
    const Config::opcode_t segPrefix = segOverridePrefix(_cpuSpec, seg);
    if (_optimizeSegment) {
        if (base == REG_BP || base == REG_SP || base == REG_EBP || base == REG_ESP)
            return seg == REG_SS ? 0 : segPrefix;
        return seg == REG_DS ? 0 : segPrefix;
    }
    return segPrefix;
}

void AsmI8086::emitModReg(AsmInsn &insn, AddrMode mode, const Operand &op, OprPos pos) const {
    uint8_t mod;
    uint8_t modReg;
    switch (op.mode) {
    case M_AL:
    case M_CL:
    case M_AX:
    case M_DX:
    case M_BREG:
    case M_WREG:
    case M_DREG:
        if (insn.lock() && insn.lockCapable() && (mode == M_BMOD || mode == M_WMOD))
            insn.setErrorIf(op, ILLEGAL_COMBINATION);
        emitRegister(insn, op, pos);
        break;
    case M_BDIR:
    case M_WDIR:
    case M_DIR:
        if (insn.leaInsn() && op.seg != REG_UNDEF)
            insn.setErrorIf(op, OPCODE_HAS_NO_EFFECT);
        emitDirect(insn, op, pos);
        break;
    case M_DMEM:
    case M_FMEM:
    case M_MEM:
        if (op.reg == REG_UNDEF && op.index == REG_UNDEF) {
            emitDirect(insn, op, pos);
            break;
        }
        // Fall-through
    case M_BMEM:
    case M_WMEM:
        if (insn.leaInsn() && op.seg != REG_UNDEF)
            insn.setErrorIf(op, OPCODE_HAS_NO_EFFECT);
        insn.setSegment(encodeSegmentOverride(op.seg, op.reg));
        if (insn.useAddr32()) {
            const auto r_m = op.encodeR_m32();
            mod = op.encodeMod32();
            if (pos == P_NONE) {  // XLAT [EBX]
                if (op.reg != REG_EBX || op.index != REG_UNDEF || op.hasDisp)
                    insn.setErrorIf(op, ILLEGAL_OPERAND_MODE);
                mod = 0;
            } else if (pos == P_OMOD) {
                insn.embed((mod << 6) | r_m);
            } else {
                insn.embedModReg((mod << 6) | r_m);
            }
            if (r_m == 4)
                insn.emitOperand8(op.encodeSib32());
            const auto sibNoBase = (r_m == 4 && op.reg == REG_UNDEF);
            if (mod == 1) {
                if (op.val.overflowInt8())
                    insn.setErrorIf(op, OVERFLOW_RANGE);
                insn.emitOperand8(op.val.getUnsigned());
            } else if (mod == 2 || (mod == 0 && r_m == 5) || sibNoBase) {
                if (op.val.overflowUint32())
                    insn.setErrorIf(op, OVERFLOW_RANGE);
                insn.emitOperand32(op.hasDisp ? op.val.getUnsigned() : 0);
            }
        } else {
            mod = op.encodeMod();
            modReg = mod << 6;
            modReg |= op.encodeR_m();
            if (pos == P_NONE) {  // XLAT [BX]
                if (op.reg != REG_BX || op.index != REG_UNDEF || op.hasDisp)
                    insn.setErrorIf(op, ILLEGAL_OPERAND_MODE);
                mod = 0;
            } else if (pos == P_OMOD) {
                insn.embed(modReg);
            } else {
                insn.embedModReg(modReg);
            }
            if (mod == 1) {
                if (op.val.overflowInt8())
                    insn.setErrorIf(op, OVERFLOW_RANGE);
                insn.emitOperand8(op.val.getUnsigned());
            } else if (mod == 2) {
                if (op.val.overflowUint16())
                    insn.setErrorIf(op, OVERFLOW_RANGE);
                insn.emitOperand16(op.val.getUnsigned());
            }
        }
        break;
    default:
        break;
    }
}

void AsmI8086::emitDirect(AsmInsn &insn, const Operand &op, OprPos pos) const {
    insn.setSegment(encodeSegmentOverride(op.seg, REG_UNDEF));
    if (insn.useAddr32()) {
        if (pos == P_MOD)
            insn.embedModReg(0005);  // mod=0, r/m=5: 32-bit direct
        if (pos == P_OMOD)
            insn.embed(0005);
        if (op.val.overflowUint32())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand32(op.val.getUnsigned());
    } else {
        if (pos == P_MOD)
            insn.embedModReg(0006);  // mod=0, r/m=6: 16-bit direct
        if (pos == P_OMOD)
            insn.embed(0006);
        emitImmediate(insn, M_DIR, SZ_WORD, op.val, op);
    }
}

void AsmI8086::emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op, OprPos pos) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_CS:
        if (pos == P_NONE)  // POP CS
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case M_BREG:
    case M_WREG:
    case M_DREG:
    case M_SREG:
    case M_CTLR:
    case M_DBGR:
    case M_TSTR:
        emitRegister(insn, op, pos);
        break;
    case M_BMOD:
    case M_WMOD:
    case M_BMEM:
    case M_WMEM:
    case M_DMEM:
    case M_FMEM:
    case M_MEM:
        emitModReg(insn, mode, op, pos);
        break;
    case M_BDIR:
    case M_WDIR:
        emitDirect(insn, op, P_OPR);
        break;
    case M_UI16:
        if (op.val.overflow(UINT16_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_UI8:
        if (op.val.overflow(UINT8_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_BIT:
        if (insn.size() == SZ_BYTE && op.val.overflow(7))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        if (insn.size() == SZ_WORD && op.val.overflow(15))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        if (insn.size() == SZ_DATA) {
            const auto max = (insn.model32() ^ insn.hasData32Prefix()) ? 31 : 15;
            if (op.val.overflow(max))
                insn.setErrorIf(op, OVERFLOW_RANGE);
        }
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_WIMM:
        emitImmediate(insn, mode, insn.size(), op.val, op);
        break;
    case M_IOA:
    case M_BIMM:
        emitImmediate(insn, mode, SZ_BYTE, op.val, op);
        break;
    case M_REL:
    case M_REL8:
        emitRelative(insn, op, mode);
        break;
    case M_FAR:
        emitImmediate(insn, M_OFF, SZ_WORD, op.val, op);
        emitImmediate(insn, M_SEG, SZ_WORD, op.segval, op);
        break;
    case M_SEG:
        break;  // emit with M_OFF
    case M_OFF:
        emitImmediate(insn, mode, SZ_WORD, op.val, op);
        emitImmediate(insn, M_SEG, SZ_WORD, insn.dstOp.val, op);
        break;
    case M_STI:
        insn.embed(op.val.getUnsigned() & 7);
        break;
    default:
        break;
    }
}

void AsmI8086::emitStringOperand(
        AsmInsn &insn, const Operand &op, RegName seg, RegName index) const {
    if (op.mode == M_NONE)
        return;
    if (op.reg != REG_UNDEF || op.index != index || op.hasDisp)
        insn.setErrorIf(op, ILLEGAL_OPERAND);
    if (seg == REG_ES && op.seg != REG_ES)
        insn.setErrorIf(op, ILLEGAL_SEGMENT);
    if (seg == REG_DS && op.seg != REG_UNDEF && op.seg != REG_DS)
        insn.setSegment(segOverridePrefix(_cpuSpec, op.seg));
}

void AsmI8086::emitStringInst(AsmInsn &insn, const Operand &dst, const Operand &src) const {
    insn.setErrorIf(dst);
    insn.setErrorIf(src);
    switch (insn.opCode() & ~1) {
    case 0xA4:  // MOVS ES:[DI],DS:[SI]
    case 0x20:  // ADD4S ES:[DI],DS:[SI]
    case 0x22:  // SUB4S ES:[DI],DS:[SI]
    case 0x26:  // CMP4S ES:[DI],DS:[SI]
        emitStringOperand(insn, src, REG_DS, REG_SI);
        /* Fall-through */
    case 0xAA:  // STOS ES:[DI]
    case 0xAE:  // SCAS ES:[DI]
    case 0x6C:  // INS ES:[DI]
        emitStringOperand(insn, dst, REG_ES, REG_DI);
        break;
    case 0xA6:  // CMPS DS:[SI],ES:[DI]
        emitStringOperand(insn, src, REG_ES, REG_DI);
        /* Fall-through */
    case 0xAC:  // LODS DS:[SI]
        emitStringOperand(insn, dst, REG_DS, REG_SI);
        break;
    case 0x6E:  // OUTS DS:[SI]
        emitStringOperand(insn, src, REG_DS, REG_SI);
        break;
    }
}

#if !defined(LIBASM_ASM_NOFLOAT)

Error AsmInsn::emitPackedDecimal(int64_t val64) {
    const auto sign = val64 < 0 ? 0x80 : 0;
    auto u64 = static_cast<uint64_t>(sign ? -val64 : val64);
    for (auto i = 0; i < 9; ++i) {
        uint8_t d = u64 % 10;
        u64 /= 10;
        d |= (u64 % 10) << 4;
        u64 /= 10;
        emitByte(d);
    }
    return emitByte(sign);
}

Error AsmInsn::emitTemporaryReal(const float80_t &val80) {
    emitUint64(val80.significand());
    return emitUint16(val80.tag());
}

#endif

Error AsmI8086::defineConstant(StrScanner &scan, AsmInsn &insn, uint16_t extra) {
    const auto type = static_cast<I8087Type>(extra);
    ErrorAt error;
    do {
        scan.skipSpaces();
        ErrorAt strErr;
        if (type == DATA_DD) {
            auto end = scan;
            const auto err = isString(end, strErr);
            if (err == OK) {
                generateString(scan, end, insn.insnBase(), DATA_LONG, strErr);
                if (error.setErrorIf(strErr) == NO_MEMORY)
                    break;
                continue;
            }
        }
        ErrorAt exprErr;
        auto p = scan;
        const auto val = parseExpr(p, exprErr);
        if (!endOfLine(p) && *p != ',') {
            if (strErr.getError()) {
                error.setErrorIf(strErr);
            } else {
                error.setErrorIf(scan, ILLEGAL_CONSTANT);
            }
            break;
        }
        if (exprErr.hasError()) {
            error.setErrorIf(strErr.getError() ? strErr : exprErr);
            break;
        }
        switch (type) {
        case DATA_DD:
            if (val.isInteger()) {
                if (val.overflowUint32()) {
                    exprErr.setErrorIf(scan, OVERFLOW_RANGE);
                } else {
                    exprErr.setErrorIf(scan, insn.emitUint32(val.getUnsigned()));
                }
            } else {
#if !defined(LIBASM_ASM_NOFLOAT)
                exprErr.setErrorIf(scan, insn.emitFloat32(val.getFloat()));
#else
                exprErr.setErrorIf(scan, FLOAT_NOT_SUPPORTED);
                insn.emitUint32(0);
#endif
            }
            break;
        case DATA_DQ:
#if defined(LIBASM_ASM_NOFLOAT)
            exprErr.setErrorIf(scan, FLOAT_NOT_SUPPORTED);
            insn.emitUint64(0);
#else
            if (val.isInteger()) {
                exprErr.setErrorIf(scan, insn.emitUint64(val.getInteger()));
            } else {
                exprErr.setErrorIf(scan, insn.emitFloat64(val.getFloat()));
            }
#endif
            break;
        case DATA_DT:
#if defined(LIBASM_ASM_NOFLOAT)
            exprErr.setErrorIf(scan, FLOAT_NOT_SUPPORTED);
            insn.emitUint16(0);
            insn.emitUint64(0);
#else
            if (val.isInteger()) {
                constexpr auto PDEC80_MAX = INT64_C(999'999'999'999'999'999);
                constexpr auto PDEC80_MIN = -INT64_C(999'999'999'999'999'999);
                const auto v = val.getInteger();
                if (v < PDEC80_MIN || v > PDEC80_MAX) {
                    exprErr.setErrorIf(scan, OVERFLOW_RANGE);
                } else {
                    exprErr.setErrorIf(scan, insn.emitPackedDecimal(v));
                }
            } else {
                exprErr.setErrorIf(scan, insn.emitTemporaryReal(val.getFloat()));
            }
#endif
            break;
        }
        scan = p;
        error.setErrorIf(exprErr);
        if (error.setErrorIf(exprErr) == NO_MEMORY)
            break;
    } while (scan.skipSpaces().expect(','));
    return insn.insnBase().setError(error);
}

Error AsmI8086::encodePrefix(StrScanner &scan, AsmInsn &insn, uint16_t opc) {
    if (endOfLine(scan))
        return encodeImpl(scan, insn);
    while (true) {
        if (opc == AsmInsn::LOCK_PREFIX) {
            if (insn.lock()) {
                insn.setErrorIf(insn.name(), ILLEGAL_COMBINATION);
                break;
            }
            insn.setLock(opc);
        } else if (AsmInsn::repeatInsn(opc, _cpuSpec.cpu)) {
            if (insn.repeat()) {
                insn.setErrorIf(insn.name(), ILLEGAL_COMBINATION);
                break;
            }
            insn.setRepeat(opc);
        } else if (opc == AsmInsn::ADDR32 && _cpuSpec.has32bit()) {
            insn.setAddr32();
        } else if (opc == AsmInsn::DATA32 && _cpuSpec.has32bit()) {
            insn.setData32();
        } else {
            break;
        }
        insn.saveAsPrefix();
        StrScanner symbol;
        _parser.readInstruction(scan.skipSpaces(), symbol);
        insn.nameBuffer().reset().rtext(symbol);
        const auto *p = PREFIXES.search(insn.name());
        if (p == nullptr)
            break;
        opc = p->readExtra();
    }
    return encodeImpl(scan.skipSpaces(), insn);
}

void AsmInsn::prependPrefix() {
    if (_prefixSave.len()) {
        _prefixSave.rtext(name());
        nameBuffer().reset().rtext(_prefixSave.str());
    }
}

void AsmInsn::applyAutoSizePrefix() {
    const auto dstMode = dstOp.mode;
    const auto srcMode = srcOp.mode;
    const auto extMode = extOp.mode;
    if (!model32()) {
        // use16: 32-bit register or 32-bit memory operand needs DATA32 to
        // promote, and 32-bit base/index needs ADDR32. SZ_DWORD entries
        // with an immediate slot (e.g. PUSHD imm32) also need DATA32 since
        // the opcode's operand width follows the mode in use16.
        if ((size() == SZ_DWORD && (dst() == M_WIMM || src() == M_WIMM)) ||
                (size() == SZ_WORD && dst() == M_DREG) ||  // MOVZX/MOVSX r32,r/m16
                (dstMode == M_DREG && (dst() == M_WREG || dst() == M_WMOD)) ||
                (srcMode == M_DREG && (src() == M_WREG || src() == M_WMOD)) ||
                (size() == SZ_DATA &&
                        (dstMode == M_DREG || srcMode == M_DREG || extMode == M_DREG)) ||
                (dstMode == M_DMEM && (dst() == M_WMOD || dst() == M_WMEM)) ||
                (srcMode == M_DMEM && (src() == M_WMOD || src() == M_WMEM)))
            setData32();
        if (!hasAddr32Prefix() && (uses32BitAddressing(dstOp) ||
                                          uses32BitAddressing(srcOp) ||
                                          uses32BitAddressing(extOp)))
            setAddr32();
    } else {
        // use32: 16-bit register or 16-bit memory operand needs DATA32 to
        // flip back to 16-bit, and 16-bit base/index needs ADDR32. Skip
        // entries whose operand width is intrinsically 16-bit so DATA32
        // would be meaningless: MOV Sreg,r/m (dst is Sreg, opcode 0x8E),
        // ARPL (0x63), and the 0F00 / 0F01 segment-register management
        // instructions that take only r/m16 (LLDT, LTR, VERR, VERW, LMSW).
        // FPU escape opcodes (D8..DF) carry operand size in the opcode
        // itself; DATA32 is never meaningful.
        const auto dstIsSreg = dst() == M_SREG;
        const auto opc = opCode();
        const auto pre = prefix();
        const auto isFpuEscape = pre >= 0xD8 && pre <= 0xDF;
        // SZ_WORD entries with a fixed-32-bit destination (M_DREG, e.g.
        // MOVZX/MOVSX r32,r/m16) and the segment-management forms
        // (LLDT/LTR/VERR/VERW/LMSW, ARPL) are intrinsically 16-bit on the
        // r/m side; DATA32 would do nothing or be wrong.
        const auto fixedWordEntry =
                (size() == SZ_WORD) &&
                (dst() == M_DREG ||
                        opc == 0x63 ||  // ARPL
                        (pre == 0x0F00 &&
                                (opc == 020 || opc == 030 || opc == 040 ||
                                        opc == 050)) ||  // LLDT, LTR, VERR, VERW
                        (pre == 0x0F01 && opc == 060));  // LMSW
        if (!dstIsSreg && !fixedWordEntry && !isFpuEscape &&
                ((is16BitReg(dstMode) && (dst() == M_WREG || dst() == M_WMOD)) ||
                        (is16BitReg(srcMode) && (src() == M_WREG || src() == M_WMOD)) ||
                        (size() == SZ_DATA &&
                                (is16BitReg(dstMode) || is16BitReg(srcMode) ||
                                        is16BitReg(extMode))) ||
                        (is16BitMem(dstMode) && (dst() == M_WMOD || dst() == M_WMEM)) ||
                        (is16BitMem(srcMode) && (src() == M_WMOD || src() == M_WMEM))))
            setData32();
        if (!hasAddr32Prefix() && (uses16BitAddressing(dstOp) ||
                                          uses16BitAddressing(srcOp) ||
                                          uses16BitAddressing(extOp)))
            setAddr32();
    }
}

Error AsmI8086::setCoprocessor(StrScanner &scan, AsmInsn &insn, uint16_t) {
    const auto at = scan;
    const auto error = _opt_fpu.set(scan);
    return error ? insn.insnBase().setErrorIf(at, error) : OK;
}

Error AsmI8086::processPseudo(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    insn.setGnuAs(_gnuAs);
    auto *p = PREFIXES.search(insn.name());
    if (p) {
        p->invoke(this, scan, insn);
        return _insn.setErrorIf(insn);
    }
    p = PSEUDOS_I8086.search(insn.name());
    return p ? p->invoke(this, scan, insn) : Assembler::processPseudo(scan, _insn);
}

void AsmInsn::prepareModReg() {
    if (hasPrefix())
        return;
    const auto dst = dstPos();
    const auto src = srcPos();
    if (dst == P_MOD || dst == P_REG || dst == P_MREG || src == P_MOD || src == P_REG)
        embedModReg(0);
}

void AsmInsn::embedModReg(Config::opcode_t data) {
    _modReg |= data;
    _hasModReg = true;
}

void AsmInsn::emitInsn() {
    uint_fast8_t pos = 0;
    auto pre = prefix();
    if ((pre >> 8) == FWAIT) {
        pre &= UINT8_MAX;
        emitByte(FWAIT, pos++);
    }
    // GAS order: segment override → addr32 (67) → data32 (66) → lock (F0)
    // → rep (F2/F3) → opcode. Intel SDM's recommended order has LOCK/REP
    // first and size prefixes last. Both decode identically; gnu-as flips
    // the emission so libasm output matches the committed GAS hex.
    if (_gnuAs) {
        if (_segment)
            emitByte(_segment, pos++);
        if (_addr32)
            emitByte(_addr32, pos++);
        if (_data32)
            emitByte(_data32, pos++);
        if (_lock)
            emitByte(_lock, pos++);
        if (_repeat)
            emitByte(_repeat, pos++);
    } else {
        if (_lock)
            emitByte(_lock, pos++);
        if (_repeat)
            emitByte(_repeat, pos++);
        if (_segment)
            emitByte(_segment, pos++);
        if (_addr32)
            emitByte(_addr32, pos++);
        if (_data32)
            emitByte(_data32, pos++);
    }
    if (pre) {
        if (pre > UINT8_MAX)
            emitByte(pre >> 8, pos++);
        emitByte(pre, pos++);
    }
    emitByte(opCode(), pos++);
    if (_hasModReg)
        emitByte(_modReg, pos);
}

uint_fast8_t AsmInsn::operandPos() const {
    uint_fast8_t pos = length();
    if (pos == 0) {
        if (_lock)
            pos++;
        if (_repeat)
            pos++;
        if (_segment)
            pos++;
        if (_addr32)
            pos++;
        if (_data32)
            pos++;
        if (hasPrefix()) {
            if (prefix() > UINT8_MAX)
                pos++;
            pos++;
        }
        pos++;
        if (_hasModReg)
            pos++;
    }
    return pos;
}

Error AsmI8086::encodeImpl(StrScanner &scan, AsmInsn &insn) const {
    if (insn.getError())
        return insn.getError();
    insn.setModel(model32());
    if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
        return insn.setError(insn.dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
            return insn.setError(insn.srcOp);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, insn.extOp) && insn.extOp.hasError())
            return insn.setError(insn.extOp);
        scan.skipSpaces();
    }

    if (searchName(_cpuSpec, insn)) {
        if (insn.getError() == OPERAND_NOT_ALLOWED)
            insn.setError(insn.dstOp, insn);
        return insn.getError();
    }
    insn.applyAutoSizePrefix();
    insn.prepareModReg();
    insn.prependPrefix();

    if (insn.lock() && !insn.lockCapable())
        insn.setErrorIf(insn.name(), ILLEGAL_COMBINATION);
    // gnu-as on i486+: rewrite shift-by-1 from short form (page D0/D1) to
    // the long form (page C0/C1, imm8=1) to match GAS -mtune=i486 output.
    // The page prefix is the actual opcode byte; insn.opCode() is the
    // reg-field of the modR/M.
    const auto pre = insn.prefix();
    const bool gnuShiftRewrite = insn.gnuAs() && _cpuSpec.cpu >= I80486 &&
                                 insn.srcOp.mode == M_VAL1 &&
                                 (pre == 0xD0 || pre == 0xD1);
    if (gnuShiftRewrite)
        insn.setPrefix(pre - 0x10);
    if (insn.stringInsn()) {
        emitStringInst(insn, insn.dstOp, insn.srcOp);
    } else {
        emitOperand(insn, insn.dst(), insn.dstOp, insn.dstPos());
        emitOperand(insn, insn.src(), insn.srcOp, insn.srcPos());
        emitOperand(insn, insn.ext(), insn.extOp, insn.extPos());
    }
    insn.emitInsn();
    if (gnuShiftRewrite)
        insn.emitOperand8(1);
    return insn.setError(insn);
}

Error AsmI8086::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    insn.setGnuAs(_gnuAs);
    encodeImpl(scan, insn);
    return _insn.setError(insn);
}

}  // namespace i8086
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
