/*
 * copyright 2025 Tadashi G. Takaoka
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

#include "dis_mc68hc12.h"
#include "reg_mc68hc12.h"
#include "table_mc68hc12.h"
#include "text_mc68hc12.h"

namespace libasm {
namespace mc68hc12 {

using namespace reg;
using namespace text::mc68hc12;

const ValueFormatter::Plugins &DisMc68HC12::defaultPlugins() {
    return ValueFormatter::Plugins::motorola();
}

DisMc68HC12::DisMc68HC12(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_mmc),
      Config(TABLE),
      _opt_mmc(this, &Config::setPagedModel, OPT_BOOL_MMC, OPT_DESC_MMC) {
    reset();
}

void DisMc68HC12::reset() {
    Disassembler::reset();
    setPagedModel(false);
}

bool DisMc68HC12::currentOption(const char *&name, const char *&value) const {
    if (pagedModel()) {
        name = OPT_BOOL_MMC;
        value = "on";
        return true;
    }
    return false;
}

// A CPU address is printed in 16 bits even when logical page:address is enabled.
uint8_t DisMc68HC12::localWidth(Config::uintptr_t addr) const {
    return (pagedModel() && addr <= UINT16_MAX) ? 16 : 0;
}

// On the memory map controller, a branch stays in the 64K CPU address space.  A target on the
// program page of the instruction is its physical address, any other is a CPU address.
Config::uintptr_t DisMc68HC12::branchTarget(
        DisInsn &insn, StrBuffer &out, Config::uintptr_t base, int32_t delta) const {
    if (!pagedModel())
        return base + delta;
    uint16_t pc = insn.address();
    int16_t page = -1;
    programAddress(cpuType(), insn.address(), pc, page);
    auto target = static_cast<int32_t>(pc + (base - insn.address())) + delta;
    if (target < 0 || target > UINT16_MAX) {
        insn.setErrorIf(out, OVERFLOW_RANGE);
        target &= UINT16_MAX;
    }
    if (page >= 0 && pageWindow(cpuType(), target) == PAGE_PPAGE)
        return toPhysical(cpuType(), target, page);
    return target;
}

Config::uintptr_t calculatePcRelBase(CpuType cpuType, DisInsn &insn) {
    const auto base = insn.address() + insn.length();
    if (insn.isMove()) {
        if (cpuType == MC68HC12)
            return insn.address() + insn.hc12MovePcBase(insn.pos);
        if (insn.pos == 2 && insn.moveDestFirst())
            return base;  // the source operand has been read
        return base + insn.moveTrailer(insn.pos);
    }
    const auto mode2 = insn.mode2();
    if (mode2 == M_IM8 || mode2 == M_PAG) {
        if (insn.mode3() == M_RL8)
            return base + 2;  // BRCLR/BRSET/CALL
        return base + 1;      // BLCR/BSET
    }
    return base;
}

int16_t readIndexExt(DisInsn &insn, Config::opcode_t post, AddrMode type) {
    const auto mode = post & 7;
    if (post < 0xE0 || mode >= 4 || type == M_NIDX)
        return 0;
    if (mode >= 2)
        return insn.readUint16();
    int16_t disp = insn.readByte();
    if (mode == 1)
        disp |= 0xFF00;
    return disp;
}

void DisMc68HC12::decodeIndexed(DisInsn &insn, StrBuffer &out, Config::opcode_t post,
        int16_t disp, AddrMode type) const {
    if (post < 0xC0) {
        const auto index = decodeRegNum((post >> 6) + 5);
        if ((post & 0x20) == 0) {
            const auto disp = signExtend(post & 0x1F, 5);
            outDec(out, disp, -5).letter(',');
            outRegName(out, index);
        } else {
            const auto delta = (post & 8) ? '-' : '+';
            const auto disp = (post & 8) ? 8 - (post & 7) : (post & 7) + 1;
            outDec(out, disp, 4).letter(',');
            if ((post & 0x10) == 0)
                out.letter(delta);
            outRegName(out, index);
            if (post & 0x10)
                out.letter(delta);
        }
    } else if (post < 0xE0) {
        const auto disp = signExtend(post & 0x1F, 5);
        if (_gnuAs) {
            outDec(out, disp, -5);
        } else {
            const auto base = calculatePcRelBase(cpuType(), insn);
            const auto target = branchTarget(insn, out, base, disp);
            outAbsAddr(out, target, localWidth(target));
        }
        outRegName(out.letter(','), REG_PC);
    } else {
        const auto num = ((post >> 3) & 3) + 5;
        const auto index = (num == 8) ? REG_PC : decodeRegNum(num);
        const auto mode = post & 7;
        const auto indirect = (mode & 3) == 3;
        if (indirect && (type == M_NIDX || type == M_DIDX))
            insn.setErrorIf(out, OPERAND_NOT_ALLOWED);
        if (mode < 4) {
            if (indirect)
                out.letter('[');
            if (type == M_NIDX) {
                insn.setErrorIf(out, OPERAND_NOT_ALLOWED);
            } else {
                if (index == REG_PC && !_gnuAs) {
                    const auto base = calculatePcRelBase(cpuType(), insn);
                    const auto target = branchTarget(insn, out, base, disp);
                    if (!indirect) {
                        if (disp >= -16 && disp < 16) {
                            out.letter(mode < 2 ? '<' : '>');
                        } else if (disp >= -0x100 && disp < 0x100 && mode >= 2) {
                            out.letter('>');
                        }
                    }
                    outAbsAddr(out, target, localWidth(target));
                } else {
                    const auto bits = (mode == 0) ? 8 : (mode == 1 ? -9 : 16);
                    outHex(out, disp, bits);
                }
            }
            outRegName(out.letter(','), index);
            if (indirect)
                out.letter(']');
        } else {
            const auto disp = (mode == 4) ? REG_A : (mode == 5 ? REG_B : REG_D);
            if (indirect)
                out.letter('[');
            outRegName(out, disp).letter(',');
            outRegName(out, index);
            if (indirect)
                out.letter(']');
        }
    }
}

void DisMc68HC12::decodeDirectPage(DisInsn &insn, StrBuffer &out) const {
    const uint8_t dir = insn.readByte();
    const auto label = lookup(dir);
    if (_gnuAs)
        out.letter('*');
    if (label) {
        if (!_gnuAs)
            out.letter('<');
        out.rtext(label);
    } else {
        outAbsAddr(out, dir, 8);
    }
}

void DisMc68HC12::decodeExtended(DisInsn &insn, StrBuffer &out) const {
    const Config::uintptr_t addr = insn.readUint16();
    const auto label = lookup(addr);
    if (label) {
        if (!_gnuAs)
            out.letter('>');
        out.rtext(label);
    } else {
        if (addr < 0x100 && !_gnuAs)
            out.letter('>');
        outAbsAddr(out, addr, localWidth(addr));
    }
}

void DisMc68HC12::decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto bits = (mode == M_RL16) ? 16 : (mode == M_RL9 ? 9 : 8);
    int16_t delta = (bits == 16) ? insn.readUint16() : static_cast<int8_t>(insn.readByte());
    if (mode == M_RL9) {
        if ((insn.opCode() & 0x10) == 0) {
            delta &= ~0xFF00;
        } else {
            delta |= 0xFF00;
        }
    }
    const auto base = insn.address() + insn.length();
    const auto target = branchTarget(insn, out, base, delta);
    insn.setErrorIf(out, checkAddr(target));
    if (_gnuAs && _relativeTarget)
        out.letter('(');
    if (_relativeTarget) {
        outRelAddr(out, target, insn.address(), bits);
    } else {
        outAbsAddr(out, target, localWidth(target));
    }
    if (_gnuAs && _relativeTarget)
        out.letter(')');
}

void DisMc68HC12::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    auto opc = insn.opCode();
    switch (mode) {
    case M_DIR:
    case M_GDIR:
    dir:
        decodeDirectPage(insn, out);
        break;
    case M_GMEM:
        if ((opc & 0x10) == 0)
            goto idx;
        // Fall-through
    case M_EXT:
    case M_GEXT:
    ext:
        decodeExtended(insn, out);
        break;
    case M_IDX:
    case M_DIDX:
    case M_NIDX:
    idx:
        if (insn.pos == 2 && insn.moveDestFirst()) {
            decodeIndexed(insn, out, insn.post, insn.postExt, mode);
        } else {
            const auto post = insn.readByte();
            decodeIndexed(insn, out, post, readIndexExt(insn, post, mode), mode);
        }
        break;
    case M_CIDX:
        decodeIndexed(insn, out, opc, readIndexExt(insn, opc, mode), mode);
        break;
    case M_RL8:
    case M_RL9:
    case M_RL16:
        decodeRelative(insn, out, mode);
        break;
    case M_GN8:
        if ((opc &= 0x30) == 0x10)
            goto dir;
        if (opc == 0x20)
            goto idx;
        if (opc == 0x30)
            goto ext;
        // Fall-through
    case M_IM8:
        out.letter('#');
        // Fall-through
    case M_PAG:
        outHex(out, insn.readByte(), 8);
        break;
    case M_GN16:
        if ((opc &= 0x30) == 0x10)
            goto dir;
        if (opc == 0x20)
            goto idx;
        if (opc == 0x30)
            goto ext;
        // Fall-through
    case M_IM16:
        out.letter('#');
        outHex(out, insn.readUint16(), 16);
        break;
    case M_R8H:
    case M_R16H:
        opc >>= 4;
        // Fall-through
    case M_R8L:
    case M_R16L:
        opc &= 7;
        if (opc == 3 || (insn.prefix() == 0x04 && opc == 2))
            insn.setErrorIf(out, ILLEGAL_REGISTER);
        outRegName(out, decodeRegNum(opc));
        break;
    case M_XPAIR:
    case M_XSEX: {
        RegName src, dst;
        decodeXferPair(mode, opc, src, dst);
        outRegName(outRegName(out, src).comma(), dst);
        break;
    }
    case M_TRAP:
        if (_gnuAs)
            out.letter('#');
        outHex(out, insn.opCode(), 8);
        break;
    default:
        break;
    }
}

Error DisMc68HC12::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readByte();
    insn.setOpCode(opc);
    if (isPrefix(cpuType(), opc)) {
        insn.setPrefix(opc);
        insn.setOpCode(insn.readByte());
        if (insn.getError())
            return _insn.setError(insn);
    }
    if (searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    if (insn.moveDestFirst()) {
        // Special index post byte order for MOVW/MOVB
        insn.post = insn.readByte();
        insn.postExt = readIndexExt(insn, insn.post, insn.mode2());
    }
    insn.pos = 1;
    decodeOperand(insn, out, insn.mode1());
    const auto mode2 = insn.mode2();
    if (mode2 != M_NONE) {
        out.comma();
        insn.pos = 2;
        decodeOperand(insn, out, mode2);
        const auto mode3 = insn.mode3();
        if (mode3 != M_NONE) {
            out.comma();
            insn.pos = 3;
            decodeOperand(insn, out, mode3);
        }
    }
    return _insn.setError(insn);
}

}  // namespace mc68hc12
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
