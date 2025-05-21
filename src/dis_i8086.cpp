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

#include "dis_i8086.h"
#include <ctype.h>
#include "reg_i8086.h"
#include "table_i8086.h"
#include "text_i8086.h"

#include <stdio.h>

namespace libasm {
namespace i8086 {

using namespace reg;
using namespace text::i8086;
using namespace text::option;

namespace {

const char OPT_BOOL_SEGMENT_INSN[] PROGMEM = "segment-insn";
const char OPT_DESC_SEGMENT_INSN[] PROGMEM = "segment override as instruction";
const char OPT_BOOL_STRING_INSN[] PROGMEM = "string-insn";
const char OPT_DESC_STRING_INSN[] PROGMEM = "string instruction as repeat operand";
const char OPT_BOOL_FAR_SUFFIX[] PROGMEM = "far-suffix";
const char OPT_DESC_FAR_SUFFIX[] PROGMEM = "use callf/jmpf for far call/jump";

}  // namespace

const ValueFormatter::Plugins &DisI8086::defaultPlugins() {
    return ValueFormatter::Plugins::intel();
}

DisI8086::DisI8086(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_fpu),
      Config(TABLE),
      _opt_fpu(this, &Config::setFpuName, OPT_TEXT_FPU, OPT_DESC_FPU, &_opt_segmentInsn),
      _opt_segmentInsn(this, &DisI8086::setSegmentInsn, OPT_BOOL_SEGMENT_INSN,
              OPT_DESC_SEGMENT_INSN, &_opt_stringInsn),
      _opt_stringInsn(this, &DisI8086::setStringInsn, OPT_BOOL_STRING_INSN, OPT_DESC_STRING_INSN,
              &_opt_use16),
      _opt_use16(this, &Config::setUse16, OPT_BOOL_USE16, OPT_DESC_USE16, &_opt_use32),
      _opt_use32(this, &Config::setUse32, OPT_BOOL_USE32, OPT_DESC_USE32, &_opt_farSuffix),
      _opt_farSuffix(this, &DisI8086::setFarSuffix, OPT_BOOL_FAR_SUFFIX, OPT_DESC_FAR_SUFFIX) {
    reset();
}

void DisI8086::reset() {
    Disassembler::reset();
    setFpuType(FPU_NONE);
#if !defined(LIBASM_I8086_NOFPU)
    setFpuType(FPU_ON);
#endif
    setSegmentInsn(false);
    setStringInsn(false);
    setFarSuffix(false);
    setModel(_cpuSpec.has32bit());
}

Error DisI8086::setSegmentInsn(bool enable) {
    _segOverrideInsn = enable;
    return OK;
}

Error DisI8086::setStringInsn(bool enable) {
    _repeatHasStringInst = enable;
    return OK;
}

Error DisI8086::setFarSuffix(bool enable) {
    _farSuffix = enable;
    return OK;
}

StrBuffer &DisI8086::outRegister(StrBuffer &out, RegName name, const char prefix) const {
    if (name != REG_UNDEF) {
        if (prefix)
            out.letter(prefix);
        outRegName(out, name);
    }
    return out;
}

RegName DisI8086::decodeRegister(DisInsn &insn, AddrMode mode, OprPos pos) const {
    uint8_t num = 0;
    switch (pos) {
    case P_OREG:
        num = insn.opCode();
        break;
    case P_OSEG:
        num = insn.opCode() >> 3;
        break;
    case P_MOD:
    case P_OMOD:
        num = insn.modReg();
        break;
    case P_REG:
        num = insn.modReg() >> 3;
        break;
    default:
        return REG_UNDEF;
    }
    switch (mode) {
    case M_BREG:
        return decodeByteReg(num);
    case M_WREG:
        return insn.useData32() ? decodeDwordReg(num) : decodeWordReg(num);
    case M_DREG:
        return decodeDwordReg(num);
    case M_SREG:
        return decodeSegReg(num, _cpuSpec);
    case M_CTLR:
        return decodeCtlReg(num, _cpuSpec);
    case M_DBGR:
        return decodeDbgReg(num, _cpuSpec);
    case M_TSTR:
        return decodeTstReg(num, _cpuSpec);
    default:
        return REG_UNDEF;
    }
}

int32_t readDelta(DisInsn &insn, AddrMode mode) {
    if (mode == M_REL8)
        return static_cast<int8_t>(insn.readByte());
    if (insn.useData32())
        return static_cast<int32_t>(insn.readUint32());
    return static_cast<int16_t>(insn.readUint16());
}

void DisI8086::decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto delta = readDelta(insn, mode);
    const auto base = insn.address() + insn.length();
    const auto target = base + delta;
    if (addressWidth() == ADDRESS_32BIT) {
        if ((delta >= 0 && target < base) || (delta < 0 && target >= base))
            insn.setErrorIf(out, OVERFLOW_RANGE);
        const auto bits = (mode == M_REL8) ? 8 : (insn.data32() ? 32 : 16);
        outRelAddr(out, target, insn.address(), bits);
    } else {
        insn.setErrorIf(out, checkAddr(target, insn.address(), 16));
        outRelAddr(out, target, insn.address(), mode == M_REL8 ? 8 : 16);
    }
}

void DisI8086::decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    if (mode == M_WIMM && insn.size() == SZ_WORD) {
        if (insn.useData32()) {
            outHex(out, insn.readUint32(), 32);
        } else {
            outHex(out, insn.readUint16(), 16);
        }
    } else if ((mode == M_WIMM && insn.size() == SZ_BYTE) || mode == M_IOA) {
        outDec(out, insn.readByte(), 8);
    } else if (mode == M_BIMM) {
        outDec(out, insn.readByte(), -8);
    } else if (mode == M_UI16) {
        outHex(out, insn.readUint16(), 16);
    } else if (mode == M_UI8) {
        outDec(out, insn.readByte(), 8);
    } else if (mode == M_BIT) {
        const auto bit = insn.readByte();
        if (insn.size() == SZ_BYTE) {
            if (bit >= 8)
                insn.setErrorIf(out, OVERFLOW_RANGE);
        } else if (insn.data32()) {
            if (bit >= 32)
                insn.setErrorIf(out, OVERFLOW_RANGE);
        } else if (bit >= 16) {
            insn.setErrorIf(out, OVERFLOW_RANGE);
        }
        outDec(out, bit, 8);
    } else {
        // M_FAR
        const uint32_t offset = insn.useData32() ? insn.readUint32() : insn.readUint16();
        const auto offsetError = insn.getError();
        const auto segment = insn.readUint16();
        outHex(out, segment, 16);
        if (_gnuAs) {
            out.comma();
        } else {
            out.letter(':');
        }
        if (offsetError)
            insn.setErrorIf(out, offsetError);
        outAbsAddr(out, offset, insn.data32() ? 32 : 16);
    }
}

namespace {

RegName getBaseReg(uint8_t mod, uint8_t r_m) {
    if (r_m <= 1 || r_m == 7)
        return REG_BX;
    if (r_m <= 3)
        return REG_BP;
    if (r_m == 6 && mod != 0)
        return REG_BP;
    return REG_UNDEF;
}

RegName getIndexReg(uint8_t r_m) {
    if (r_m >= 6)
        return REG_UNDEF;
    return r_m % 2 == 0 ? REG_SI : REG_DI;
}

OprSize operandSize(const DisInsn &insn, AddrMode mode) {
    switch (mode) {
    case M_AL:
    case M_BREG:
        return SZ_BYTE;
    case M_AX:
    case M_WREG:
        return insn.data32() ? SZ_DWORD : SZ_WORD;
    case M_SREG:
        return SZ_WORD;
    case M_BMOD:
        return (insn.modReg() >> 6) == 3 ? insn.size() : SZ_NONE;
    case M_WMOD:
        return (insn.modReg() >> 6) == 3 ? (insn.data32() ? SZ_DWORD : insn.size()) : SZ_NONE;
    default:
        return SZ_NONE;
    }
}

OprSize operandSize(const DisInsn &insn) {
    if (insn.stringInst())
        return insn.size();
    OprSize size;
    if ((size = operandSize(insn, insn.dst())) != SZ_NONE)
        return size;
    if ((size = operandSize(insn, insn.src())) != SZ_NONE)
        return size;
    return SZ_NONE;
}

OprSize pointerSize(DisInsn &insn, AddrMode mode) {
    const auto size = insn.size();
    switch (mode) {
    case M_BMOD:
    case M_BMEM:
        return size;
    case M_WMOD:
    case M_WMEM:
        if (size == SZ_NONE) {
            if (insn.fpuInsn())
                return SZ_NONE;
            if (insn.farInsn()) {
                if (insn.gnuAs()) {
                    //                    printf("@@ farInsn & GNU\n");
                    return SZ_NONE;
                }
                return insn.useData32() ? SZ_FWORD : SZ_NONE;
            }
            return insn.useData32() ? SZ_DWORD : SZ_NONE;
        }
        return insn.useData32() ? SZ_DWORD : SZ_WORD;
    default:
        return size;
    }
}

PrefixName pointerReg(DisInsn &insn) {
    OprSize size;
    if ((size = pointerSize(insn, insn.dst())) == SZ_NONE &&
            (size = pointerSize(insn, insn.src())) == SZ_NONE)
        return PRE_UNDEF;
    return PrefixName(size + PRE_PTR);
}

}  // namespace

StrBuffer &DisI8086::outMemPrefix(DisInsn &insn, StrBuffer &out, RegName seg) const {
    auto opr = operandSize(insn);
    if ((opr == SZ_DWORD || opr == SZ_WORD) && insn.src() == M_BMOD) {
        insn.useData32();
        opr = SZ_NONE;  // MOVSX/MOVSZ r32,r/m8
    }
    if (opr == SZ_NONE) {
        const auto ptr = pointerReg(insn);
        if (ptr != PRE_UNDEF) {
            outPrefixName(out, ptr).letter(' ');
            outPrefixName(out, PRE_PTR).letter(' ');
        }
    }
    if (seg != REG_UNDEF)
        outRegister(out, seg).letter(':');
    return out;
}

StrBuffer &DisI8086::outMemReg(
        DisInsn &insn, StrBuffer &out, RegName seg, uint_fast8_t mod, uint_fast8_t r_m) const {
    outMemPrefix(insn, out, seg);
    const auto base = getBaseReg(mod, r_m);
    const auto index = getIndexReg(r_m);
    out.letter('[');
    char sep = 0;
    outRegister(out, base);
    if (base != REG_UNDEF)
        sep = '+';
    outRegister(out, index, sep);
    if (index != REG_UNDEF)
        sep = '+';
    if (mod == 1) {
        const auto disp8 = static_cast<int8_t>(insn.readByte());
        if (disp8 >= 0)
            out.letter(sep);
        outDec(out, disp8, -8);
    } else if (mod == 2 || (mod == 0 && r_m == 6)) {
        if (sep)
            out.letter(sep);
        outAbsAddr(out, insn.readUint16(), 16);
    }
    return out.letter(']');
}

StrBuffer &DisI8086::outDisplacement(DisInsn &insn, StrBuffer &out, uint_fast8_t mod) const {
    if (mod == 1) {
        const auto disp8 = static_cast<int8_t>(insn.readByte());
        if (disp8 >= 0)
            out.letter('+');
        outDec(out, disp8, -8);
    } else if (mod == 2) {
        if (_gnuAs && !model32())
            insn.addPrefix(TEXT_PRE_ADDR32, out);
        outAbsAddr(out.letter('+'), insn.readUint32(), 32);
    }
    return out;
}

StrBuffer &DisI8086::outScaledIndex(DisInsn &insn, StrBuffer &out, uint_fast8_t mod) const {
    const auto sib = insn.readByte();
    const auto ss = (sib >> 6);
    const auto idx = (sib >> 3) & 7;
    const auto index = (idx == 4) ? REG_UNDEF : RegName(REG_EAX + idx);
    const auto bas = (sib & 7);
    auto base = RegName(REG_EAX + bas);
    if (mod == 0 && bas == 5)
        base = REG_UNDEF;
    out.letter('[');
    outRegister(out, base);
    auto sep = (base == REG_UNDEF) ? 0 : '+';
    if (index != REG_UNDEF) {
        outRegister(out, index, sep);
        if (ss)
            out.letter('*').letter('0' + (1 << ss));
        sep = '+';
    }
    if (base == REG_UNDEF) {
        if (sep)
            out.letter(sep);
        if (_gnuAs && !model32())
            insn.addPrefix(TEXT_PRE_ADDR32, out);
        outAbsAddr(out, insn.readUint32(), 32);
    } else {
        outDisplacement(insn, out, mod);
    }
    return out.letter(']');
}

StrBuffer &DisI8086::out32bitAddr(DisInsn &insn, StrBuffer &out, uint_fast8_t mod) const {
    outMemPrefix(insn, out, overrideSeg(_cpuSpec, insn.segment()));

    const auto r_m = insn.modReg() & 07;
    if (r_m == 4)
        return outScaledIndex(insn, out, mod);

    const auto base = RegName(REG_EAX + r_m);
    out.letter('[');
    if (mod == 0 && r_m == 5) {
        if (_gnuAs && !model32())
            insn.addPrefix(TEXT_PRE_ADDR32, out);
        outAbsAddr(out, insn.readUint32(), 32);
    } else {
        outRegister(out, base);
    }
    return outDisplacement(insn, out, mod).letter(']');
}

void DisI8086::decodeMemReg(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos) const {
    if (pos == P_NONE) {  // XLAT [BX]
        outRegName(out.letter('['), REG_BX).letter(']');
        return;
    }
    const auto mod = insn.modReg() >> 6;
    if (mod == 3) {
        if (mode >= M_BMEM && mode <= M_MEM)
            insn.setErrorIf(out, ILLEGAL_OPERAND);
        auto regMode = (mode == M_BMOD ? M_BREG : M_WREG);
        if (mode == M_DREG)
            regMode = mode;
        outRegister(out, decodeRegister(insn, regMode, pos));
        return;
    }
    if (insn.useAddr32()) {
        out32bitAddr(insn, out, mod);
        return;
    }
    const auto r_m = insn.modReg() & 07;
    outMemReg(insn, out, overrideSeg(_cpuSpec, insn.segment()), mod, r_m);
}

void DisI8086::decodeRepeatStr(DisInsn &insn, StrBuffer &out) const {
    if (_repeatHasStringInst) {
        Insn _istr(0);
        DisInsn istr(_istr, insn, out);
        const auto opc = insn.readByte();
        if (insn.getError())
            return;
        istr.setModel(model32());
        istr.setOpCode(opc);
        if (searchOpCode(_cpuSpec, istr, out))
            insn.setErrorIf(out, istr);
        if (!istr.stringInst())
            insn.setErrorIf(out, INVALID_INSTRUCTION);
        out.text(istr.name());
        insn.setUsage(istr);
    }
}

void DisI8086::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos) const {
    RegName name;
    switch (mode) {
    case M_AL:
        outRegister(out, REG_AL);
        break;
    case M_CL:
        outRegister(out, REG_CL);
        break;
    case M_AX:
        outRegister(out, insn.useData32() ? REG_EAX : REG_AX);
        break;
    case M_DX:
        outRegister(out, REG_DX);
        break;
    case M_CS:
    case M_BREG:
    case M_WREG:
    case M_DREG:
    case M_SREG:
    case M_CTLR:
    case M_DBGR:
    case M_TSTR:
        name = decodeRegister(insn, mode, pos);
        if (mode == M_CS && pos == P_NONE) {  // POP CS
            name = REG_CS;
            insn.setErrorIf(out, REGISTER_NOT_ALLOWED);
        } else if (mode == M_DREG && pos == P_MOD) {
            if ((insn.modReg() >> 6) != 3)
                insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
        } else if (name == REG_UNDEF) {
            insn.setErrorIf(out, ILLEGAL_REGISTER);
        }
        outRegister(out, name);
        break;
    case M_FS:
        outRegister(out, REG_FS);
        break;
    case M_GS:
        outRegister(out, REG_GS);
        break;
    case M_BMEM:
    case M_WMEM:
        if (insn.leaInsn() && insn.segment())
            insn.setErrorIf(out, OPCODE_HAS_NO_EFFECT);
        // Fall-through
    case M_BMOD:
    case M_WMOD:
    case M_DMEM:
    case M_FMEM:
    case M_MEM:
        decodeMemReg(insn, out, mode, pos);
        break;
    case M_VAL1:
        outHex(out, 1, 3);
        break;
    case M_VAL3:
        outHex(out, 3, 3);
        break;
    case M_WIMM:
    case M_UI16:
    case M_UI8:
    case M_BIMM:
    case M_FAR:
    case M_IOA:
    case M_BIT:
        decodeImmediate(insn, out, mode);
        break;
    case M_SEG:
        insn.farseg = insn.readUint16();
        break;
    case M_OFF:
        outHex(out, insn.farseg, 16).comma();
        outAbsAddr(out, insn.useData32() ? insn.readUint32() : insn.readUint16());
        break;
    case M_BDIR:
    case M_WDIR:
        outMemReg(insn, out, overrideSeg(_cpuSpec, insn.segment()), 0, 6);
        break;
    case M_REL:
    case M_REL8:
        decodeRelative(insn, out, mode);
        break;
    case M_ISTR:
        decodeRepeatStr(insn, out);
        break;
    case M_ST0:
        outRegName(out, REG_ST);
        break;
    case M_STI:
        outRegName(out, REG_ST).letter('(').int16(insn.opCode() & 7).letter(')');
        break;
    default:
        break;
    }
}

namespace {

bool memoryOperand(AddrMode mode, uint_fast8_t mod) {
    if (mode == M_BMOD || mode == M_WMOD)
        return mod != 3;
    return mode >= M_BMEM && mode <= M_WDIR;
}

bool validSegOverride(const DisInsn &insn) {
    if (insn.segment() == 0)
        return true;
    if (insn.stringInst())
        return true;
    const auto mod = insn.modReg() >> 6;
    return memoryOperand(insn.dst(), mod) || memoryOperand(insn.src(), mod);
}

}  // namespace

void DisI8086::decodeStringInst(DisInsn &insn, StrBuffer &out) const {
    const auto opc = insn.opCode() & ~1;
    constexpr auto MOVS = 0xA4;   // ES:[DI], xS:[SI]
    constexpr auto CMPS = 0xA6;   // xS:[SI], ES:[DI]
    constexpr auto STOS = 0xAA;   // ES:[DI]
    constexpr auto LODS = 0xAC;   // xS:[SI]
    constexpr auto SCAS = 0xAE;   // ES:[DI]
    constexpr auto INS = 0x6C;    // ES:[DI], DX
    constexpr auto OUTS = 0x6E;   // DX. xS:[SI]
    constexpr auto ADD4S = 0x20;  // ES:[DI], xS:[SI]
    constexpr auto SUB4S = 0x22;  // ES:[DI], xS:[SI]
    constexpr auto CMP4S = 0x26;  // ES:[DI], xS:[SI]

    const auto seg = insn.segment();
    if (!_gnuAs) {
        if (seg == 0)
            return;
        insn.setErrorIf(out, ILLEGAL_SEGMENT);
    }

    switch (opc) {
    case MOVS:
    case ADD4S:
    case SUB4S:
    case CMP4S:
        // ES:[DI], xS:[SI]
        outMemReg(insn, out, REG_ES, 0, 5).comma();
        outMemReg(insn, out, overrideSeg(_cpuSpec, seg, REG_DS), 0, 4);
        break;
    case OUTS:
        outRegName(out, REG_DX).comma();
        // Fall-through
    case LODS:
        outMemReg(insn, out, overrideSeg(_cpuSpec, seg, REG_DS), 0, 4);
        break;
    case CMPS:
        // xS:[SI], ES:[DI]
        outMemReg(insn, out, overrideSeg(_cpuSpec, seg, REG_DS), 0, 4).comma();
        outMemReg(insn, out, REG_ES, 0, 5);
        break;
    case STOS:
    case SCAS:
    case INS:
        // ES:[DI]
        if (overrideSeg(_cpuSpec, seg) != REG_UNDEF)
            insn.setErrorIf(out, ILLEGAL_SEGMENT);
        outMemReg(insn, out, overrideSeg(_cpuSpec, seg, REG_ES), 0, 5);
        break;
    }
    if (opc == INS)
        outRegName(out.comma(), REG_DX);
}

Error DisI8086::searchCodes(DisInsn &insn, StrBuffer &out) const {
    insn.setModel(model32());
    uint16_t opc = insn.readByte();
    const auto fwait = (_cpuSpec.fpu != FPU_NONE && opc == DisInsn::FWAIT);
    if (fwait)
        opc = insn.readByte();
    if (insn.getError()) {
        if (!fwait)
            return insn.getError();
        insn.setSegment(0);
        insn.setPrefix(0);
        insn.setOpCode(DisInsn::FWAIT);
        insn.resetLength(1);
        searchOpCode(_cpuSpec, insn, out);
        return insn.getError();
    }

    if (_cpuSpec.has32bit()) {
        opc = insn.readSizePrefix(opc);
        if (insn.getError())
            return insn.getError();
    }
    if ((!_segOverrideInsn || _gnuAs) && isSegmentPrefix(_cpuSpec, opc)) {
        insn.setSegment(opc);
        opc = insn.readByte();
        if (insn.getError())
            return insn.getError();
    }
    auto prefix = opc;
    if (fwait) {
        if (!DisInsn::escapeInsn(opc))
            return insn.setError(out, UNKNOWN_INSTRUCTION);
        prefix |= (DisInsn::FWAIT << 8);
    }
    if (isPrefix(_cpuSpec, prefix)) {
        insn.setPrefix(prefix);
        opc = insn.readByte();
        if (insn.getError())
            return insn.getError();
        if (prefix < 0x100) {
            prefix = (prefix << 8) | opc;
            if (isPrefix(_cpuSpec, prefix)) {
                insn.setPrefix(prefix);
                opc = insn.readByte();
                if (insn.getError())
                    return insn.getError();
            }
        }
    }

    insn.setOpCode(opc);
    // printf("@@ search:");
    // if (prefix >= 100) {
    //     printf(" %02X %02X", prefix >> 8, prefix & 0xFF);
    // } else if (prefix) {
    //     printf(" %02X", prefix);
    // }
    // printf(" %02x\n", opc);
    searchOpCode(_cpuSpec, insn, out);
    // if (insn.isOK()) {
    //     printf("@@  found: %s dst=%d/%d src=%d/%d ext=%d/%d", insn.name(), insn.dst(),
    //             insn.dstPos(), insn.src(), insn.srcPos(), insn.ext(), insn.extPos());
    // }
    return insn.getError();
}

namespace {

bool imulHasSameDstSrc(const DisInsn &insn) {
    const auto modreg = insn.modReg();
    const auto reg = (modreg >> 3) & 7;
    return (insn.opCode() & ~2) == 0x69 &&  // IMUL
           (modreg >> 6) == 3 &&            // regsiter
           (modreg & 7) == reg;             // dst==src
}

}  // namespace

Config::opcode_t DisInsn::readSizePrefix(Config::opcode_t opc) {
    while (true) {
        if (opc == DATA32 && !_hasData32) {
            _hasData32 = _hasUnusedData32 = true;
        } else if (opc == ADDR32 && !_hasAddr32) {
            _hasAddr32 = _hasUnusedAddr32 = true;
        } else {
            break;
        }
        opc = readByte();
    }
    return opc;
}

void DisInsn::readModReg() {
    const OprPos dst = dstPos();
    const OprPos src = srcPos();
    if (dst == P_MOD || dst == P_REG || src == P_MOD || src == P_REG)
        _modReg = readByte();
    else if (dst == P_OMOD || src == P_OMOD)
        _modReg = opCode();
}

static constexpr auto CALLF = 0x9A;    // CALLF far
static constexpr auto JMPF = 0xEA;     // JMPF far
static constexpr auto RETFu16 = 0xCA;  // RETF u16
static constexpr auto RETF = 0xCB;     // RETF
static constexpr auto PUSHiw = 0x68;   // PUSH iw

bool DisInsn::farInsn() const {
    const auto pre = prefix();
    const auto opc = opCode();
    if (pre == 0)
        return opc == CALLF || opc == JMPF || opc == RETFu16 || opc == RETF;
    if (pre == 0xFF) {
        const auto reg = (opc >> 3) & 7;
        return reg == 3 || reg == 5;  // CALLF/JMPF
    }
    return false;
}

bool DisInsn::needData32() const {
    const auto opc = opCode();
    return data32() && prefix() == 0 && (opc == CALLF || opc == JMPF || opc == PUSHiw);
}

void DisInsn::addSuffix(char suffix, StrBuffer &out) {
    auto save{out};
    nameBuffer().over(out).letter(suffix).over(nameBuffer());
    save.over(out);
}

void DisInsn::addGnuPrefix(char prefix, StrBuffer &out) {
    char buffer[Insn::MAX_NAME + 1];
    strcpy(buffer, name());
    const auto pos = strlen(buffer) - 1;
    if (toupper(buffer[pos]) == 'F')
        buffer[pos] = 0;
    auto save{out};
    nameBuffer().reset().over(out).letter(prefix).text(buffer).over(nameBuffer());
    save.over(out);
}

void DisInsn::addPrefix(const /*PROGMEM*/ char *prefix_P, StrBuffer &out) {
    char buffer[Insn::MAX_NAME + 1];
    strcpy(buffer, name());
    auto save{out};
    nameBuffer().reset().over(out).text_P(prefix_P).letter(' ').text(buffer).over(nameBuffer());
    save.over(out);
}

Error DisI8086::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    insn.setGnuAs(_gnuAs);
    if (searchCodes(insn, out))
        return _insn.setError(insn);
#if 0
    if (_cpuSpec.cpu == I80386) {
        printf("@@ found: %s dst=%d src=%d ext=%d:", insn.name(), insn.dst(), insn.src(),
                insn.ext());
        if (insn.hasUnusedData32())
            printf(" DATA32");
        if (insn.hasUnusedAddr32())
            printf(" ADDR32");
        if (insn.data32())
            printf(" data32");
        if (insn.addr32())
            printf(" addr32");
        if (insn.farInsn())
            printf(" farInsn");
        for (auto i = 0; i < insn.length(); i++)
            printf(" %02X", insn.bytes()[i]);
        printf("\n");
    }
#endif

    insn.readModReg();
    if (_insn.setErrorIf(insn))
        return _insn.getError();
    if (!validSegOverride(insn))
        return _insn.setErrorIf(insn, ILLEGAL_SEGMENT);

    if (insn.farInsn()) {
        if (_gnuAs) {
            insn.addGnuPrefix('L', out);  // LCALL/LJMP
        } else if (_farSuffix) {
            insn.addSuffix('F', out);  // CALLF/JMPF
        }
    }
    if (insn.stringInst()) {
        decodeStringInst(insn, out);
    } else {
        decodeOperand(insn, out, insn.dst(), insn.dstPos());
        if (_gnuAs && insn.needData32() && !model32())
            insn.addPrefix(TEXT_PRE_DATA32, out);
        if (insn.src() != M_NONE && !imulHasSameDstSrc(insn))
            decodeOperand(insn, out.comma(), insn.src(), insn.srcPos());
        if (insn.ext() != M_NONE)
            decodeOperand(insn, out.comma(), insn.ext(), insn.extPos());
    }
    if (insn.hasUnusedData32() || insn.hasUnusedAddr32())
        insn.setErrorIf(out.str(), PREFIX_HAS_NO_EFFECT);
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
