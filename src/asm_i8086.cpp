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

#include <stdio.h>

#include "asm_i8086.h"
#include "table_i8086.h"
#include "reg_i8086.h"

namespace libasm {
namespace i8086 {

bool AsmI8086::parseStringInst(const char *scan, Operand &op) {
    Insn _insn;
    InsnI8086 insn(_insn);
    const char *endName = _parser.scanSymbol(scan);
    insn.setName(scan, endName);
    insn.setAddrMode(M_NONE, M_NONE);
    if (TableI8086.searchName(insn)) return false;
    if (!insn.stringInst()) return false;
    _scan = skipSpaces(endName);
    op.val32 = insn.opCode();
    op.mode = M_ISTR;
    return true;
}

const char *AsmI8086::parsePointerSize(const char *scan, Operand &op) {
    const char *p = scan;
    const RegName reg = _regs.parseRegName(p);
    if (reg == REG_BYTE || reg == REG_WORD) {
        p = skipSpaces(p + _regs.regNameLen(reg));
        // Pointer size override
        if (_regs.parseRegName(p) == REG_PTR) {
            op.ptr = reg;
            return skipSpaces(p + _regs.regNameLen(REG_PTR));
        }
        setError(UNKNOWN_OPERAND);
        return nullptr;
    }
    return scan;
}

const char *AsmI8086::parseSegmentOverride(const char *scan, Operand &op) {
    const char *p = scan;
    const RegName reg = _regs.parseRegName(p);
    if (_regs.isSegmentReg(reg)) {
        p += _regs.regNameLen(reg);
        // Segment Override
        if (*p == ':') {
            op.seg = reg;
            return p + 1;
        }
    }
    return scan;
}

const char *AsmI8086::parseBaseRegister(const char *scan, Operand &op) {
    const char *p = scan;
    const RegName reg = _regs.parseRegName(p);
    if (reg == REG_BX || reg == REG_BP) {
        op.reg = reg;
        return p + _regs.regNameLen(reg);
    }
    return scan;
}

const char *AsmI8086::parseIndexRegister(const char *scan, Operand &op) {
    const char *p = scan;
    if (op.reg != REG_UNDEF) {
        if (*p != '+') return scan;
        p = skipSpaces(p + 1);
    }
    const RegName reg = _regs.parseRegName(p);
    if (reg == REG_SI || reg == REG_DI) {
        op.index = reg;
        return p + _regs.regNameLen(reg);
    }
    return scan;
}

const char *AsmI8086::parseDisplacement(const char *scan, Operand &op) {
    const char *p = scan;
    if (endOfLine(p) || *p == ']') return scan;
    if (op.reg != REG_UNDEF || op.index != REG_UNDEF) {
        if (*p != '+' && *p != '-') {
            setError(UNKNOWN_OPERAND);
            return nullptr;
        }
    }
    _scan = p;
    if (getOperand(op.val32)) return nullptr;
    op.setError(getError());
    op.hasVal = true;
    return _scan;
}

Error AsmI8086::parseOperand(Operand &op) {
    const char *p = _scan;
    if (endOfLine(p)) return OK;

    if (parseStringInst(p, op))
        return OK;
    p = parsePointerSize(p, op);
    if (p == nullptr) return getError();
    p = parseSegmentOverride(p, op);
    if (*p == '[') {
        p = skipSpaces(p + 1);
        p = parseBaseRegister(p, op);
        p = parseIndexRegister(p, op);
        p = parseDisplacement(p, op);
        if (p == nullptr) return getError();
        if (*p == ']') {
            _scan = p + 1;
            if (op.reg == REG_UNDEF && op.index == REG_UNDEF) {
                if (op.hasVal) {
                    op.mode = (op.ptr == REG_UNDEF) ? M_DIR
                        : (op.ptr == REG_BYTE ? M_BDIR : M_WDIR);
                    return OK;
                }
                return setError(UNKNOWN_OPERAND);
            }
            op.mode = (op.ptr == REG_UNDEF) ? M_MEM
                : (op.ptr == REG_BYTE ? M_BMEM : M_WMEM);
            return OK;
        }
        return setError(MISSING_CLOSING_PAREN);
    }
    if (op.ptr != REG_UNDEF || op.seg != REG_UNDEF)
        return setError(UNKNOWN_OPERAND);

    const RegName reg = _regs.parseRegName(p);
    if (_regs.isGeneralReg(reg)) {
        _scan = p + _regs.regNameLen(reg);
        op.reg = reg;
        switch (reg) {
        case REG_AL: op.mode = M_AL; break;
        case REG_CL: op.mode = M_CL; break;
        case REG_AX: op.mode = M_AX; break;
        case REG_DX: op.mode = M_DX; break;
        default:
            op.mode = (_regs.regSize(reg) == SZ_BYTE) ? M_BREG : M_WREG;
            break;
        }
        return OK;
    }
    if (_regs.isSegmentReg(reg)) {
        _scan = p + _regs.regNameLen(reg);
        op.reg = reg;
        op.mode = M_SREG;
        return OK;
    }
    if (reg != REG_UNDEF) return setError(UNKNOWN_OPERAND);

    if (getOperand(op.val32)) return getError();
    op.setError(getError());
    if (*_scan == ':') {
        op.seg16 = op.val32;
        _scan++;
        if (getOperand(op.val32)) return getError();
        op.setErrorIf(getError());
        op.mode = M_FAR;
        return OK;
    }
    op.mode = op.immediateMode();
    return OK;
}

AddrMode AsmI8086::Operand::immediateMode() const {
    if (getError()) return M_IMM;
    if (val32 == 1) return M_VAL1;
    if (val32 == 3) return M_VAL3;
    const int32_t val = static_cast<int32_t>(val32);
    return (val >= -0x80 && val < 0x80) ? M_IMM8 : M_IMM;
}

Error AsmI8086::emitImmediate(InsnI8086 &insn, OprSize size, uint16_t val) {
    if (size == SZ_BYTE) insn.emitOperand8(val);
    if (size == SZ_WORD) insn.emitOperand16(val);
    return OK;
}

Error AsmI8086::emitRelative(InsnI8086 &insn, const Operand &op, AddrMode mode) {
    const Config::uintptr_t base = insn.address() + (mode == M_REL8 ? 2 : 3);
    const Config::uintptr_t target = op.getError() ? base : op.val32;
    const Config::ptrdiff_t delta = target - base;
    if (mode == M_REL8) {
        const bool overflow = (delta < -0x80 || delta >= 0x80);
        if (insn.opCode() == 0xEB && (overflow || op.getError())) {
            insn.setOpCode(0xE9, 0);
            return emitRelative(insn, op, M_REL);
        }
        if (overflow) return setError(OPERAND_TOO_FAR);
        insn.emitOperand8(static_cast<uint8_t>(delta));
        return OK;
    }
    // M_REL
    if (delta < -0x8000 || delta >= 0x8000) return setError(OPERAND_TOO_FAR);
    insn.emitOperand16(static_cast<uint16_t>(delta));
    return OK;
}

Error AsmI8086::emitRegister(InsnI8086 &insn, const Operand &op, OprPos pos) {
    const uint8_t num = _regs.encodeRegNum(op.reg);
    switch (pos) {
    case P_OREG: insn.embed(num); break;
    case P_OSEG: insn.embed(num << 3); break;
    case P_OMOD: insn.embed(0300 | num); break;
    case P_REG:  insn.embedModReg(num << 3); break;
    case P_MOD:  insn.embedModReg(0300 | num); break;
    default: break;
    }
    return OK;
}

uint8_t AsmI8086::Operand::encodeMod() const {
    const bool needDisp = (reg == REG_BP && index == REG_UNDEF)
        || (hasVal && (val32 || getError()));
    if (needDisp) {
        const int32_t val = static_cast<int32_t>(val32);
        return (val < -0x80 || val >= 0x80 || getError()) ? 2 : 1;
    }
    return 0;
}

uint8_t AsmI8086::Operand::encodeR_m() const {
    uint8_t r_m = 0;
    if (reg == REG_UNDEF) {
        r_m = (index == REG_SI) ? 4 : 5;
    } else if (index == REG_UNDEF) {
        r_m = (reg == REG_BP) ? 6 : 7;
    } else {
        if (reg == REG_BP) r_m |= 2;
        if (index == REG_DI) r_m |= 1;
    }
    return r_m;
}

Error AsmI8086::emitModReg(InsnI8086 &insn, const Operand &op, OprPos pos) {
    uint8_t mod;
    uint8_t modReg;
    switch (op.mode) {
    case M_AL: case M_CL: case M_AX: case M_DX:
    case M_BREG: case M_WREG:
        return emitRegister(insn, op, pos);
    case M_BDIR: case M_WDIR: case M_DIR:
        return emitDirect(insn, op, pos);
    case M_BMEM: case M_WMEM: case M_MEM:
        insn.setSegment(TableI8086.segOverridePrefix(op.seg));
        mod = op.encodeMod();
        modReg = mod << 6;
        modReg |= op.encodeR_m();
        if (pos == P_OMOD) {
            insn.embed(modReg);
        } else {
            insn.embedModReg(modReg);
        }
        if (mod == 1) insn.emitOperand8(op.val32);
        if (mod == 2) insn.emitOperand16(op.val32);
        break;
    default: break;
    }
    return OK;
}

Error AsmI8086::emitDirect(InsnI8086 &insn, const Operand &op, OprPos pos) {
    insn.setSegment(TableI8086.segOverridePrefix(op.seg));
    if (pos == P_MOD)  insn.embedModReg(0006);
    if (pos == P_OMOD) insn.embed(0006);
    return emitImmediate(insn, SZ_WORD, op.val32);
}

Error AsmI8086::emitOperand(
    InsnI8086 &insn, AddrMode mode, const Operand &op, OprPos pos) {
    switch (mode) {
    case M_BREG:
    case M_WREG:
    case M_SREG:
        return emitRegister(insn, op, pos);
    case M_BMOD:
    case M_WMOD:
    case M_BMEM:
    case M_WMEM:
        return emitModReg(insn, op, pos);
    case M_BDIR:
    case M_WDIR:
        return emitDirect(insn, op, P_OPR);
    case M_IMM:
        return emitImmediate(insn, insn.oprSize(), op.val32);
    case M_IOA:
        if (op.val32 >= 0x100) return setError(OVERFLOW_RANGE);
        /* Fall-through */
    case M_IMM8:
        return emitImmediate(insn, SZ_BYTE, op.val32);
    case M_REL:
    case M_REL8:
        return emitRelative(insn, op, mode);
    case M_FAR:
        if (op.val32 >= 0x10000) return setError(OVERFLOW_RANGE);
        emitImmediate(insn, SZ_WORD, op.val32);
        return emitImmediate(insn, SZ_WORD, op.seg16);
    case M_ISTR:
        insn.emitOperand8(op.val32);
        return OK;
    default:
        return OK;
    }
}

Error AsmI8086::emitStringOperand(
    InsnI8086 &insn, const Operand &op, RegName seg, RegName index) {
    if (op.mode == M_NONE) return OK;
    if (op.reg != REG_UNDEF || op.index != index || op.hasVal)
        return setError(ILLEGAL_OPERAND);
    if (seg == REG_ES && op.seg != REG_ES)
        return setError(ILLEGAL_SEGMENT);
    if (seg == REG_DS && op.seg != REG_UNDEF && op.seg != REG_DS)
        insn.setSegment(TableI8086.segOverridePrefix(op.seg));
    return OK;
}

Error AsmI8086::encodeStringInst(
    InsnI8086 &insn, const Operand &dst, const Operand &src) {
    switch (insn.opCode() & ~1) {
    case 0xA4: // MOVS ES:[DI],DS:[SI]
        if (emitStringOperand(insn, src, REG_DS, REG_SI)) return getError();
        /* Fall-through */
    case 0xAA: // STOS ES:[DI]
    case 0xAE: // SCAS ES:[DI]
        if (emitStringOperand(insn, dst, REG_ES, REG_DI)) return getError();
        break;
    case 0xA6: // CMPS DS:[SI],ES:[DI]
        if (emitStringOperand(insn, src, REG_ES, REG_DI)) return getError();
    case 0xAC: // LODS DS:[SI]
        if (emitStringOperand(insn, dst, REG_DS, REG_SI)) return getError();
        break;
    }
    insn.emitInsn();
    return setOK();
}

#if 0
void AsmI8086::Operand::print(const char *title) const {
    printf("%s%d", title, mode);
    if (ptr) printf(":p=%d", ptr);
    if (seg) printf(":s=%d", seg);
    if (reg) printf(":r=%d", reg);
    if (index) printf(":i=%d", index);
    if (hasVal) {
        if (mode == M_FAR) printf(":d=%04X:%04X", seg16, val32);
        else printf(":d=%04X", val32);
    }
    if (getError()) printf(":e=%d", getError());
}
#endif

Error AsmI8086::encode(Insn &_insn) {
    InsnI8086 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    Operand dstOp, srcOp;
    _scan = skipSpaces(endName);
    if (parseOperand(dstOp)) return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        _scan = skipSpaces(p + 1);
        if (parseOperand(srcOp)) return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p)) return setError(GARBAGE_AT_END);
    setError(dstOp.getError());
    setErrorIf(srcOp.getError());

    insn.setAddrMode(dstOp.mode, srcOp.mode);
#if 0
    printf("@@ search: name=%-6s", insn.name());
    dstOp.print(" dst=");
    srcOp.print(" src=");
    printf(" %s\n", endName);
#endif
    if (TableI8086.searchName(insn))
        return setError(TableI8086.getError());
#if 0
    printf("@@  found: code=");
    if (insn.first()) printf("%02X:", insn.first());
    printf("%02X dst=%d:%d src=%d:%d size=%d\n", insn.opCode(), insn.dstMode(), insn.dstPos(), insn.srcMode(), insn.srcPos(), insn.oprSize());
#endif
    insn.prepairModReg();

    if (insn.stringInst())
        return encodeStringInst(insn, dstOp, srcOp);
    const AddrMode dst = insn.dstMode();
    if (dst != M_NONE) {
        if (emitOperand(insn, dst, dstOp, insn.dstPos()))
            return getError();
    }
    const AddrMode src = insn.srcMode();
    if (src != M_NONE) {
        if (emitOperand(insn, src, srcOp, insn.srcPos()))
            return getError();
    }
    insn.emitInsn();
    return getError();
}

} // namespace i8086
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
