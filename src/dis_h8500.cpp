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

using namespace libasm::text::h8500;

namespace libasm {
namespace h8500 {

using namespace reg;

namespace {

// Returns the number of EA extension bytes for the given EA byte.
uint8_t eaExtCount(uint8_t ea) {
    if ((ea >> 4) == 0xE) return 1;  // @(d:8,Rn)
    if ((ea >> 4) == 0xF) return 2;  // @(d:16,Rn)
    if (ea == 0x04 || ea == 0x05 || ea == 0x0D) return 1;
    if (ea == 0x0C || ea == 0x15 || ea == 0x1D) return 2;
    return 0;
}

}  // namespace

const ValueFormatter::Plugins &DisH8500::defaultPlugins() {
    return ValueFormatter::Plugins::motorola();
}

DisH8500::DisH8500(const ValueFormatter::Plugins &plugins) : Disassembler(plugins), Config(TABLE) {
    reset();
}

void DisH8500::outReg(StrBuffer &out, RegName reg) const {
    outRegName(out, reg);
}

void DisH8500::outCr(StrBuffer &out, CrName cr) const {
    outCrName(out, cr);
}

void DisH8500::outImm8(StrBuffer &out, uint8_t val) const {
    out.letter('#');
    outHex(out, val, 8);
}

void DisH8500::outImm16(DisInsn &insn, StrBuffer &out) const {
    out.letter('#');
    outHex(out, insn.readUint16Be(), 16);
}

void DisH8500::outAbs8(StrBuffer &out, uint8_t addr) const {
    out.letter('@');
    outAbsAddr(out, addr, 8);
    if (!_gnuAs)
        out.text_P(PSTR(":8"));
}

void DisH8500::outAbs16(DisInsn &insn, StrBuffer &out) const {
    out.letter('@');
    outAbsAddr(out, insn.readUint16Be());
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

// Decode and output the EA operand. EA extensions are pre-buffered in insn.eaExt1/eaExt2.
void DisH8500::outEaOperand(DisInsn &insn, StrBuffer &out, uint8_t ea) const {
    const auto hi = ea >> 4;
    const auto rrr = ea & 0x07;
    const auto reg = decodeReg(rrr);

    switch (hi) {
    case 0xA:  // Rn direct
        outReg(out, reg);
        break;
    case 0xB:  // @-Rn pre-decrement
        out.text_P(PSTR("@-"));
        outReg(out, reg);
        break;
    case 0xC:  // @Rn+ post-increment
        out.letter('@');
        outReg(out, reg);
        out.letter('+');
        break;
    case 0xD:  // @Rn indirect
        out.letter('@');
        outReg(out, reg);
        break;
    case 0xE:  // @(d:8,Rn)
        out.letter('@').letter('(');
        outHex(out, static_cast<int8_t>(insn.eaExt1), -8);
        out.comma();
        outReg(out, reg);
        out.letter(')');
        break;
    case 0xF:  // @(d:16,Rn)
        out.letter('@').letter('(');
        outHex(out, static_cast<int16_t>((insn.eaExt1 << 8) | insn.eaExt2), -16);
        out.comma();
        outReg(out, reg);
        out.letter(')');
        break;
    default:
        // Low-range EA bytes
        switch (ea) {
        case 0x04:  // #xx:8 immediate
            outImm8(out, insn.eaExt1);
            break;
        case 0x0C:  // #xx:16 immediate
            out.letter('#');
            outHex(out, static_cast<uint16_t>((insn.eaExt1 << 8) | insn.eaExt2), 16);
            break;
        case 0x05:  // @aa:8 short absolute (byte)
        case 0x0D:  // @aa:8 short absolute (word)
            outAbs8(out, insn.eaExt1);
            break;
        case 0x15:  // @aa:16 absolute (byte)
        case 0x1D: {  // @aa:16 absolute (word)
            const auto addr = static_cast<uint16_t>((insn.eaExt1 << 8) | insn.eaExt2);
            out.letter('@');
            outAbsAddr(out, addr);
            if (!_gnuAs)
                out.text_P(PSTR(":16"));
            break;
        }
        default:
            insn.setError(UNKNOWN_INSTRUCTION);
            break;
        }
        break;
    }
}

// Decode GEN (Format A) or EXT (Format C) instruction.
// insn.insnFmt, insn.eaByte, insn.opByte, insn.eaExt1/eaExt2 must be set.
Error DisH8500::decodeGenFmt(DisInsn &insn, StrBuffer &out) const {
    searchOpCode(cpuType(), insn, out);
    if (insn.getError())
        return insn.getError();

    const auto flags = insn.flags();
    const auto ea = insn.eaByte;
    const auto op = insn.opByte;
    const auto esrc = flags.src();
    const auto edst = flags.dst();

    if (esrc == M_EA && edst == M_NONE) {
        outEaOperand(insn, out, ea);
    } else if (esrc == M_EA && edst == M_REG) {
        outEaOperand(insn, out, ea);
        out.comma();
        outReg(out, decodeReg(op & 0x07));
    } else if (esrc == M_EA && edst == M_CR) {
        outEaOperand(insn, out, ea);
        out.comma();
        const auto cr = decodeCr(op & 0x07);
        if (cr == CR_UNDEF)
            return insn.setError(UNKNOWN_INSTRUCTION);
        outCr(out, cr);
    } else if (esrc == M_REG && edst == M_EA) {
        outReg(out, decodeReg(op & 0x07));
        out.comma();
        outEaOperand(insn, out, ea);
    } else if (esrc == M_CR && edst == M_EA) {
        const auto cr = decodeCr(op & 0x07);
        if (cr == CR_UNDEF)
            return insn.setError(UNKNOWN_INSTRUCTION);
        outCr(out, cr);
        out.comma();
        outEaOperand(insn, out, ea);
    } else if (esrc == M_BIT && edst == M_EA) {
        outBit(out, op & 0x0F);
        out.comma();
        outEaOperand(insn, out, ea);
    } else if (esrc == M_IMM8 && edst == M_CR) {
        // ANDC/ORC/XORC: EA=0x04 (imm8 in eaExt1)
        outImm8(out, insn.eaExt1);
        out.comma();
        const auto cr = decodeCr(op & 0x07);
        if (cr == CR_UNDEF)
            return insn.setError(UNKNOWN_INSTRUCTION);
        outCr(out, cr);
    }

    return insn.getError();
}

// Write a static name to nameBuffer and restore out to operand buffer.
static void writeName(DisInsn &insn, StrBuffer &out, const char *name_P) {
    auto save{out};
    insn.nameBuffer().reset().over(out).text_P(name_P).over(insn.nameBuffer());
    save.over(out);
}

// Output register list byte as {R7, R6, ...} (high-numbered first).
static void outRegList(DisInsn &insn, StrBuffer &out) {
    const auto list = insn.readByte();
    out.letter('{');
    bool first = true;
    for (int i = 7; i >= 0; i--) {
        if (list & (1 << i)) {
            if (!first)
                out.comma();
            first = false;
            outRegName(out, decodeReg(i));
        }
    }
    out.letter('}');
}

// Decode secondary 0x11-prefix instructions.
Error DisH8500::decodeSecondary(DisInsn &insn, StrBuffer &out) const {
    const auto b1 = insn.readByte();

    switch (b1) {
    case 0x14:  // PRTD #xx:8 (H8/532)
        if (cpuType() != H8_532)
            return insn.setError(UNKNOWN_INSTRUCTION);
        writeName(insn, out, TEXT_PRTD);
        outImm8(out, insn.readByte());
        return insn.getError();
    case 0x19:  // PRTS (H8/532)
        if (cpuType() != H8_532)
            return insn.setError(UNKNOWN_INSTRUCTION);
        writeName(insn, out, TEXT_PRTS);
        return insn.getError();
    case 0x1C:  // PRTD #xx:16 (H8/532)
        if (cpuType() != H8_532)
            return insn.setError(UNKNOWN_INSTRUCTION);
        writeName(insn, out, TEXT_PRTD);
        outImm16(insn, out);
        return insn.getError();
    default:
        break;
    }

    // JMP/JSR indirect and displacement forms (0xC0-0xFF)
    if (b1 >= 0xC0) {
        const auto reg = decodeReg(b1 & 0x07);
        if ((b1 & 0xF8) == 0xC0 && cpuType() == H8_532) {
            writeName(insn, out, TEXT_PJMP);
            out.letter('@');
            outReg(out, reg);
            return insn.getError();
        }
        if ((b1 & 0xF8) == 0xC8 && cpuType() == H8_532) {
            writeName(insn, out, TEXT_PJSR);
            out.letter('@');
            outReg(out, reg);
            return insn.getError();
        }
        if ((b1 & 0xF8) == 0xD0) {
            writeName(insn, out, TEXT_JMP);
            out.letter('@');
            outReg(out, reg);
            return insn.getError();
        }
        if ((b1 & 0xF8) == 0xD8) {
            writeName(insn, out, TEXT_JSR);
            out.letter('@');
            outReg(out, reg);
            return insn.getError();
        }
        if ((b1 & 0xF8) == 0xE0) {
            writeName(insn, out, TEXT_JMP);
            const auto disp = static_cast<int8_t>(insn.readByte());
            out.letter('@').letter('(');
            outHex(out, disp, -8);
            out.comma();
            outReg(out, reg);
            out.letter(')');
            return insn.getError();
        }
        if ((b1 & 0xF8) == 0xE8) {
            writeName(insn, out, TEXT_JSR);
            const auto disp = static_cast<int8_t>(insn.readByte());
            out.letter('@').letter('(');
            outHex(out, disp, -8);
            out.comma();
            outReg(out, reg);
            out.letter(')');
            return insn.getError();
        }
        if ((b1 & 0xF8) == 0xF0) {
            writeName(insn, out, TEXT_JMP);
            const auto disp = static_cast<int16_t>(insn.readUint16Be());
            out.letter('@').letter('(');
            outHex(out, disp, -16);
            out.comma();
            outReg(out, reg);
            out.letter(')');
            return insn.getError();
        }
        if ((b1 & 0xF8) == 0xF8) {
            writeName(insn, out, TEXT_JSR);
            const auto disp = static_cast<int16_t>(insn.readUint16Be());
            out.letter('@').letter('(');
            outHex(out, disp, -16);
            out.comma();
            outReg(out, reg);
            out.letter(')');
            return insn.getError();
        }
    }

    return insn.setError(UNKNOWN_INSTRUCTION);
}

// Decode special format (Format B) instruction.
Error DisH8500::decodeSpcFmt(DisInsn &insn, StrBuffer &out) const {
    if (insn.opByte == 0x11)
        return decodeSecondary(insn, out);

    // searchOpCode writes the instruction name and sets insn.flags().
    searchOpCode(cpuType(), insn, out);
    if (insn.getError())
        return insn.getError();

    const auto flags = insn.flags();
    const auto esrc = flags.src();
    const auto edst = flags.dst();
    const auto b0 = insn.opByte;

    if (esrc == M_PCREL8) {
        outPcRel8(insn, out);
    } else if (esrc == M_PCREL16) {
        outPcRel16(insn, out);
    } else if (esrc == M_IMM8 && edst == M_NONE) {
        outImm8(out, insn.readByte());
    } else if (esrc == M_IMM8 && edst == M_REG) {
        outImm8(out, insn.readByte());
        out.comma();
        outReg(out, decodeReg(b0 & 0x07));
    } else if (esrc == M_IMM16 && edst == M_NONE) {
        outImm16(insn, out);
    } else if (esrc == M_IMM16 && edst == M_REG) {
        outImm16(insn, out);
        out.comma();
        outReg(out, decodeReg(b0 & 0x07));
    } else if (esrc == M_ABS16) {
        outAbs16(insn, out);
    } else if (esrc == M_ABS24) {
        const auto page = insn.readByte();
        const auto addr = insn.readUint16Be();
        out.letter('@');
        outHex(out, (static_cast<uint32_t>(page) << 16) | addr, 24);
    } else if (esrc == M_REGLIST) {
        outRegList(insn, out);
    } else if (esrc == M_TRAPV) {
        const auto b1 = insn.readByte();
        if ((b1 & 0xFC) != 0x10)
            return insn.setError(UNKNOWN_INSTRUCTION);
        outImm8(out, b1 & 0x03);
    } else if (esrc == M_SCB) {
        const auto b1 = insn.readByte();
        if ((b1 & 0xF8) != 0xB8)
            return insn.setError(UNKNOWN_INSTRUCTION);
        outReg(out, decodeReg(b1 & 0x07));
        out.comma();
        outPcRel8(insn, out);
    } else if (esrc == M_ABS8 && edst == M_REG) {
        outAbs8(out, insn.readByte());
        out.comma();
        outReg(out, decodeReg(b0 & 0x07));
    } else if (esrc == M_REG && edst == M_ABS8) {
        outReg(out, decodeReg(b0 & 0x07));
        out.comma();
        outAbs8(out, insn.readByte());
    } else if (esrc == M_DISP8F && edst == M_REG) {
        const auto disp = static_cast<int8_t>(insn.readByte());
        out.letter('@').letter('(');
        outHex(out, disp, -8);
        out.comma();
        outReg(out, REG_FP);
        out.letter(')');
        out.comma();
        outReg(out, decodeReg(b0 & 0x07));
    } else if (esrc == M_REG && edst == M_DISP8F) {
        outReg(out, decodeReg(b0 & 0x07));
        out.comma();
        const auto disp = static_cast<int8_t>(insn.readByte());
        out.letter('@').letter('(');
        outHex(out, disp, -8);
        out.comma();
        outReg(out, REG_FP);
        out.letter(')');
    } else if (esrc == M_REG && edst == M_NONE) {
        outReg(out, REG_FP);  // UNLK FP
    } else if (esrc == M_REG && edst == M_IMM8) {
        outReg(out, REG_FP);  // LINK FP, #imm8
        out.comma();
        outImm8(out, insn.readByte());
    } else if (esrc == M_REG && edst == M_IMM16) {
        outReg(out, REG_FP);  // LINK FP, #imm16
        out.comma();
        outImm16(insn, out);
    }
    // M_NONE, M_NONE: no operand output (NOP, RTE, RTS, SLEEP, TRAP/VS)

    return insn.getError();
}

Error DisH8500::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto b0 = insn.readByte();
    insn.setOpCode(b0);

    if (isEaByte(b0)) {
        insn.eaByte = b0;
        const auto cnt = eaExtCount(b0);
        if (cnt >= 1)
            insn.eaExt1 = insn.readByte();
        if (cnt >= 2)
            insn.eaExt2 = insn.readByte();
        const auto bN = insn.readByte();
        if (bN == 0x00) {
            insn.opByte = insn.readByte();
            insn.insnFmt = FMT_EXT;
        } else {
            insn.opByte = bN;
            insn.insnFmt = FMT_GEN;
        }
        insn.setOpCode(insn.opByte);
        return _insn.setError(decodeGenFmt(insn, out));
    }
    insn.opByte = b0;
    insn.insnFmt = FMT_SPC;
    return _insn.setError(decodeSpcFmt(insn, out));
}

}  // namespace h8500
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
