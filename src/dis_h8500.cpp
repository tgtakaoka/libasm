/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "dis_h8500.h"
#include "reg_h8500.h"
#include "table_h8500.h"
#include "text_h8500.h"

namespace libasm {
namespace h8500 {

using namespace reg;
using namespace text::h8500;

const ValueFormatter::Plugins &DisH8500::defaultPlugins() {
    return ValueFormatter::Plugins::hitachi();
}

DisH8500::DisH8500(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_maxMode),
      Config(TABLE),
      _opt_maxMode(this, &Config::setMaxMode, OPT_BOOL_MAX_MODE, OPT_DESC_MAX_MODE, &_opt_fpAlias),
      _opt_fpAlias(this, &Config::setFpAlias, OPT_BOOL_FP_ALIAS, OPT_DESC_FP_ALIAS, &_opt_spAlias),
      _opt_spAlias(this, &Config::setSpAlias, OPT_BOOL_SP_ALIAS, OPT_DESC_SP_ALIAS) {
    reset();
}

void DisH8500::reset() {
    Disassembler::reset();
    setMaxMode(false);
    setFpAlias(true);
    setSpAlias(true);
}

void DisH8500::outReg(StrBuffer &out, RegName reg) const {
    if (reg == REG_R6 && !fpAlias()) {
        out.text_P(TEXT_REG_R6);
        return;
    }
    outRegName(out, reg);
}

void DisH8500::outAddrReg(StrBuffer &out, RegName reg) const {
    if (reg == REG_R7 && spAlias()) {
        out.text_P(TEXT_REG_SP);
        return;
    }
    outReg(out, reg);
}

void DisH8500::outCr(StrBuffer &out, CrName cr) const {
    outCrName(out, cr);
}

void DisH8500::outImm8(StrBuffer &out, uint8_t val) const {
    out.letter('#');
    outHex(out, val, 8);
}

void DisH8500::outAbs8(StrBuffer &out, uint8_t addr) const {
    out.letter('@');
    outAbsAddr(out, addr, 8);
    if (!_gnuAs)
        out.text_P(PSTR(":8"));
}

void DisH8500::outAbs16(DisInsn &insn, StrBuffer &out) const {
    out.letter('@');
    // @aa:16 encodes only the 16-bit field; its high byte comes from a page
    // register at run time, so print it at its true 16-bit width rather than
    // zero-padded to the (24-bit) address width in maximum mode.
    outAbsAddr(out, insn.readUint16Be(), 16);
    if (!_gnuAs)
        out.text_P(PSTR(":16"));
}

void DisH8500::outPcRel8(DisInsn &insn, StrBuffer &out) const {
    const auto delta = static_cast<int8_t>(insn.readByte());
    const auto base = insn.address() + insn.length();
    const auto target = static_cast<Config::uintptr_t>(base + delta);
    insn.setErrorIf(out, checkAddr(target));
    outRelAddr(out, target, insn.address(), 8);
}

void DisH8500::outPcRel16(DisInsn &insn, StrBuffer &out) const {
    const auto delta = static_cast<int16_t>(insn.readUint16Be());
    const auto base = insn.address() + insn.length();
    const auto target = static_cast<Config::uintptr_t>(base + delta);
    insn.setErrorIf(out, checkAddr(target));
    outRelAddr(out, target, insn.address(), 16);
}

void DisH8500::outBit(StrBuffer &out, uint8_t bit) const {
    out.letter('#');
    outHex(out, bit, 4);
}

// Emit the EA operand resolved in decodeImpl. eaVal holds the displacement /
// absolute address / immediate when the mode needs one.
void DisH8500::outEa(DisInsn &insn, StrBuffer &out) const {
    switch (insn.eaMode) {
    case M_REG:
        outReg(out, insn.eaReg);
        break;
    case M_IND:
        out.letter('@');
        outAddrReg(out, insn.eaReg);
        break;
    case M_PDEC:
        out.text_P(PSTR("@-"));
        outAddrReg(out, insn.eaReg);
        break;
    case M_PINC:
        out.letter('@');
        outAddrReg(out, insn.eaReg);
        out.letter('+');
        break;
    case M_IDX8:
        out.letter('@').letter('(');
        outHex(out, static_cast<int8_t>(insn.eaVal), -8);
        out.letter(',');
        outAddrReg(out, insn.eaReg);
        out.letter(')');
        break;
    case M_IDX16:
        out.letter('@').letter('(');
        outHex(out, static_cast<int16_t>(insn.eaVal), -16);
        out.letter(',');
        outAddrReg(out, insn.eaReg);
        out.letter(')');
        break;
    case M_ABS8:
        outAbs8(out, static_cast<uint8_t>(insn.eaVal));
        break;
    case M_ABS16:
        out.letter('@');
        outAbsAddr(out, insn.eaVal, 16);  // 16-bit field; high byte is the DP page
        if (!_gnuAs)
            out.text_P(PSTR(":16"));
        break;
    case M_IMM8:
        outImm8(out, static_cast<uint8_t>(insn.eaVal));
        break;
    case M_IMM16:
        out.letter('#');
        outHex(out, insn.eaVal, 16);
        break;
    default:
        insn.setError(UNKNOWN_INSTRUCTION);
        break;
    }
}

// Output register list byte as (R0,R2-R4): low-numbered first, with runs of
// consecutive registers collapsed to Rm-Rn (the manual / ASL syntax).
void DisH8500::outRegList(DisInsn &insn, StrBuffer &out) const {
    const auto list = insn.readByte();
    const auto at(out);  // mark the '(' position before emitting the list
    out.letter('(');
    bool first = true;
    for (int i = 0; i < 8;) {
        if ((list & (1 << i)) == 0) {
            i++;
            continue;
        }
        int j = i;
        while (j + 1 < 8 && (list & (1 << (j + 1))))
            j++;
        if (!first)
            out.letter(',');
        first = false;
        outReg(out, decodeReg(i));
        if (j > i) {
            out.letter('-');
            outReg(out, decodeReg(j));
        }
        i = j + 1;
    }
    out.letter(')');
    if (list == 0)
        insn.setErrorIf(at, OPCODE_HAS_NO_EFFECT);
}

static bool isEaPlaceholder(AddrMode mode) {
    return mode == M_EASRC || mode == M_EADST || mode == M_EAREG || mode == M_EAMEM;
}

// In a GEN/EXT entry whose neither slot is an EA placeholder, the immediate slot
// is itself the resolved EA (ANDC/ORC/XORC #imm,CR). Otherwise (MOV:G #imm,<EAd>)
// the immediate is trailing data after the OP byte.
static bool immIsEa(const DisInsn &insn) {
    const auto pm = insn.prefixMode();
    return (pm == PM_GEN || pm == PM_EXT) && !isEaPlaceholder(insn.src()) &&
           !isEaPlaceholder(insn.dst());
}

void DisH8500::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    switch (mode) {
    case M_NONE:
        break;
    case M_EASRC:
    case M_EADST:
    case M_EAREG:
    case M_EAMEM:
        outEa(insn, out);
        break;
    case M_REG:
        outReg(out, decodeReg(insn.opByte & 0x07));
        break;
    case M_REGP: {
        // MULXU.W/DIVXU.W place the 32-bit result in the pair Rd:Rd+1, so Rd
        // must be even. Only the word form (EA Sz bit set) forms a pair; the
        // byte form uses a single register.
        const auto regno = insn.opByte & 0x07;
        if (((insn.eaByte >> 3) & 1) && (regno & 1))
            insn.setErrorIf(out, REGISTER_NOT_ALIGNED);
        outReg(out, decodeReg(regno));
        break;
    }
    case M_CR: {
        const auto cr = decodeCr(insn.opByte & 0x07);
        if (cr == CR_UNDEF) {
            insn.setError(UNKNOWN_INSTRUCTION);
            break;
        }
        outCr(out, cr);
        break;
    }
    case M_BIT: {
        const auto bit = insn.opByte & 0x0F;
        // A byte-size target (EA Sz bit = 0) only has bits 0-7.
        if (((insn.eaByte >> 3) & 1) == 0 && bit > 7) {
            insn.setError(ILLEGAL_BIT_NUMBER);
            break;
        }
        outBit(out, bit);
        break;
    }
    case M_IMM2: {
        // ADD:Q quick immediate encoded in the OP byte: 08=+1 09=+2 0C=-1 0D=-2.
        int8_t v;
        switch (insn.opByte) {
        case 0x08:
            v = 1;
            break;
        case 0x09:
            v = 2;
            break;
        case 0x0C:
            v = -1;
            break;
        case 0x0D:
            v = -2;
            break;
        default:
            insn.setError(UNKNOWN_INSTRUCTION);
            return;
        }
        out.letter('#');
        outDec(out, static_cast<uint32_t>(v), -8);
        break;
    }
    case M_FP:
        outReg(out, REG_FP);
        break;
    case M_REL8:
        outPcRel8(insn, out);
        break;
    case M_REL16:
        outPcRel16(insn, out);
        break;
    case M_IMM8:
        if (immIsEa(insn))
            outEa(insn, out);
        else
            outImm8(out, insn.readByte());
        break;
    case M_IMM16:
        if (immIsEa(insn)) {
            outEa(insn, out);
        } else {
            const auto val = insn.readUint16Be();
            out.letter('#');
            outHex(out, val, 16);
            // RTD/LINK/PRTD (no class) also have a #xx:8 form; a value that fits
            // a byte needs an explicit :16 to round-trip to this long form. The
            // :I forms (MOV:I/CMP:I) are unambiguous, so they take no suffix.
            if (insn.insnClass() == IC_N && val <= 0xFF)
                out.text_P(PSTR(":16"));
        }
        break;
    case M_ABS8:
        outAbs8(out, insn.readByte());
        break;
    case M_ABS16:
        outAbs16(insn, out);
        break;
    case M_ABS24: {
        const auto page = insn.readByte();
        const auto addr = insn.readUint16Be();
        const auto target = (static_cast<uint32_t>(page) << 16) | addr;
        // A page-jump address beyond the chip's physical space (20-bit on the
        // pin-limited H8/520/530) is a non-canonical encoding. The physical
        // width applies regardless of operating mode (page jumps are max-mode).
        const auto max = (UINT32_C(1) << physicalWidth()) - 1;
        if (target > max)
            insn.setError(out, OVERFLOW_RANGE);
        out.letter('@');
        outHex(out, target, 24);
        // PJMP/PJSR have only the @aa:24 form; a page-0 (<=0xFFFF) address would
        // otherwise re-parse as @aa:16 (which they reject), so make it explicit.
        if (!_gnuAs)
            out.text_P(PSTR(":24"));
        break;
    }
    case M_REGLIST:
        outRegList(insn, out);
        break;
    case M_SP:
        // Implicit stack operand: @SP+ when it is the source (LDM pop), @-SP
        // when the destination (STM push).
        if (insn.dst() == M_SP)
            out.text_P(PSTR("@-"));
        else
            out.letter('@');
        outAddrReg(out, REG_SP);
        if (insn.src() == M_SP)
            out.letter('+');
        break;
    case M_FPX8: {
        const auto disp = static_cast<int8_t>(insn.readByte());
        out.letter('@').letter('(');
        outHex(out, disp, -8);
        out.letter(',');
        outReg(out, REG_FP);
        out.letter(')');
        break;
    }
    case M_TRAPV: {
        const auto b1 = insn.readByte();
        if ((b1 & 0xF0) != 0x10) {
            insn.setError(UNKNOWN_INSTRUCTION);
            break;
        }
        outImm8(out, b1 & 0x0F);
        break;
    }
    case M_SCB: {
        const auto b1 = insn.readByte();
        if ((b1 & 0xF8) != 0xB8) {
            insn.setError(UNKNOWN_INSTRUCTION);
            break;
        }
        outReg(out, decodeReg(b1 & 0x07));
        break;
    }
    case M_IND:
        out.letter('@');
        outAddrReg(out, decodeReg(insn.opByte & 0x07));
        break;
    case M_INDP: {
        // PJMP/PJSR @Rn copy Rn->CP and Rn+1->PC, so Rn must be the even base
        // of a register pair.
        const auto regno = insn.opByte & 0x07;
        if (regno & 1)
            insn.setErrorIf(out, REGISTER_NOT_ALIGNED);
        out.letter('@');
        outAddrReg(out, decodeReg(regno));
        break;
    }
    case M_IDX8: {
        const auto disp = static_cast<int8_t>(insn.readByte());
        out.letter('@').letter('(');
        outHex(out, disp, -8);
        out.letter(',');
        outAddrReg(out, decodeReg(insn.opByte & 0x07));
        out.letter(')');
        break;
    }
    case M_IDX16: {
        const auto disp = static_cast<int16_t>(insn.readUint16Be());
        out.letter('@').letter('(');
        outHex(out, disp, -16);
        out.letter(',');
        outAddrReg(out, decodeReg(insn.opByte & 0x07));
        out.letter(')');
        break;
    }
    default:
        break;
    }
}

// Read the lead byte(s): an EA prefix (+ext) then the OP byte (PM_GEN/EXT), the
// 0x11 prefix then OP byte (PM_SEC), or a bare OP byte (PM_SPC). The EA operand
// is resolved once into insn.eaMode/eaReg/eaVal. Reading past the end sets
// NO_MEMORY, which the caller surfaces before searchOpCode's setOK() drops it.
Error DisH8500::readLead(DisInsn &insn) const {
    const auto b0 = insn.readByte();
    insn.setOpCode(b0);

    AddrMode eaMode = M_NONE;
    const auto ext = classifyLead(cpuType(), b0, eaMode);
    PrefixMode pm;
    if (ext >= 0) {
        insn.eaByte = b0;
        insn.eaMode = eaMode;
        insn.eaReg = decodeReg(b0 & 0x07);
        if (ext == 1)
            insn.eaVal = insn.readByte();
        else if (ext == 2)
            insn.eaVal = insn.readUint16Be();
        const auto bN = insn.readByte();
        if (bN == 0x00) {
            insn.opByte = insn.readByte();
            pm = PM_EXT;
        } else {
            insn.opByte = bN;
            pm = PM_GEN;
        }
    } else if (isPrefix(cpuType(), b0)) {
        insn.setPrefix(b0);
        insn.opByte = insn.readByte();
        pm = PM_SEC;
    } else {
        insn.opByte = b0;
        pm = PM_SPC;
    }
    insn.setPrefixMode(pm);
    insn.setOpCode(insn.opByte);
    return insn.getError();
}

// Append the .B/.W size suffix from the live Sz bit (the EA byte for GEN/EXT, the
// OP byte for the SPC MOV:S/F/L forms) and reject encodings whose Sz bit
// contradicts a fixed or control-register-determined size.
void DisH8500::decodeSize(DisInsn &insn, StrBuffer &out) const {
    const auto src = insn.src();
    const auto dst = insn.dst();
    const auto pm = insn.prefixMode();

    // LDC/STC/ANDC/ORC/XORC <imm|EA>,CR: the operand size (EA Sz bit) must match
    // the control register width (SR is word, CCR/BR/EP/DP/TP are byte). A
    // mismatch is a non-canonical encoding, not this instruction.
    if ((pm == PM_GEN || pm == PM_EXT) && (src == M_CR || dst == M_CR)) {
        const bool word = (insn.eaByte >> 3) & 1;
        if (word != (decodeCr(insn.opByte & 0x07) == CR_SR)) {
            insn.setError(UNKNOWN_INSTRUCTION);
            return;
        }
    }

    // A fixed-size GEN/EXT op carries no .B/.W suffix (TAS/SWAP/EXTS/EXTU/DADD/
    // DSUB/MOVFPE/MOVTPE are byte; XCH is word). Its live EA Sz bit must match
    // the fixed size; a contradicting Sz bit is a non-canonical encoding.
    if ((pm == PM_GEN || pm == PM_EXT) && insn.insnSize() == ISZ_NONE) {
        const auto osz = insn.oprSize();
        if ((osz == SZ_BYTE || osz == SZ_WORD) && (((insn.eaByte >> 3) & 1) != (osz == SZ_WORD))) {
            insn.setError(UNKNOWN_INSTRUCTION);
            return;
        }
    }

    // Emit the ":x" class suffix (MOV/ADD/CMP carry only the stem as the name).
    static const char CLASS_LETTER[] = {0, 'G', 'E', 'F', 'I', 'L', 'S', 'Q'};
    // CLASS_LETTER is indexed by the InsnClass value; keep the two in lockstep.
    static_assert(IC_N == 0 && IC_G == 1 && IC_E == 2 && IC_F == 3 && IC_I == 4 && IC_L == 5 &&
                          IC_S == 6 && IC_Q == 7 && sizeof(CLASS_LETTER) == IC_Q + 1,
            "CLASS_LETTER[] must track the InsnClass enum order");
    const auto cls = insn.insnClass();
    if (cls != IC_N) {
        insn.nameBuffer().letter(':');
        insn.appendName(out, CLASS_LETTER[cls]);
    }

    // The .B/.W suffix follows the live Sz bit (the EA byte, or the OP byte for
    // the SPC short forms). For MOV:G #imm,<EAd> the OP byte (0x06 8-bit data /
    // 0x07 16-bit data) is the data length, independent of the operation size:
    // 0x06 with a word EA is the legal sign-extended form (manual 2.2.24 note 3),
    // so only the impossible 0x07-with-byte-EA (16-bit data, byte op) is rejected.
    if (insn.insnSize() == ISZ_DATA) {
        const bool word = ((pm == PM_SPC ? insn.opByte : insn.eaByte) >> 3) & 1;
        if (insn.oprSize() == SZ_WORD && !word) {
            insn.setError(UNKNOWN_INSTRUCTION);
            return;
        }
        insn.nameBuffer().letter('.');
        insn.appendName(out, word ? 'W' : 'B');
    }
}

Error DisH8500::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    // Gate page jumps by mode (see pageMatcher), like the assembler.
    insn.maxMode = maxMode();

    // NO_MEMORY from reading past the end must surface before searchOpCode below.
    if (readLead(insn))
        return _insn.setErrorIf(insn);

    if (searchOpCode(cpuType(), insn, out) == OK)
        decodeSize(insn, out);

    if (insn.isOK()) {
        decodeOperand(insn, out, insn.src());
        if (insn.dst() != M_NONE && insn.isOK())
            decodeOperand(insn, out.comma(), insn.dst());
    }

    // A mid-decode failure (unknown opcode / non-canonical encoding) would leak a
    // partial mnemonic or operand; reset so the caller sees a clean line.
    if (insn.getError() == UNKNOWN_INSTRUCTION) {
        insn.nameBuffer().reset();
        out.reset();
    }
    return _insn.setErrorIf(insn);
}

}  // namespace h8500
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
