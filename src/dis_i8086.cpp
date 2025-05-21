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
#include "reg_i8086.h"
#include "table_i8086.h"
#include "text_i8086.h"

#include <stdio.h>

namespace libasm {
namespace i8086 {

using namespace reg;
using namespace text::common;
using namespace text::i8086;
using namespace text::option;

namespace {
const char OPT_BOOL_SEG_INSN[] PROGMEM = "segment-insn";
const char OPT_DESC_SEG_INSN[] PROGMEM = "segment override as instruction";
const char OPT_BOOL_REP_INSN[] PROGMEM = "repeat-insn";
const char OPT_DESC_REP_INSN[] PROGMEM = "repeat prefix as instruction";
const char OPT_BOOL_LOCK_INSN[] PROGMEM = "lock-insn";
const char OPT_DESC_LOCK_INSN[] PROGMEM = "lock prefix as instruction";
const char OPT_BOOL_FAR_INSN[] PROGMEM = "far-insn";
const char OPT_DESC_FAR_INSN[] PROGMEM = "use callf/jmpf for far call/jump";
const char OPT_BOOL_PTR_SUFFIX[] PROGMEM = "ptr-suffix";
const char OPT_DESC_PTR_SUFFIX[] PROGMEM = "add ptr suffix for memory reference";
}  // namespace

const ValueFormatter::Plugins &DisI8086::defaultPlugins() {
    return ValueFormatter::Plugins::intel();
}

DisI8086::DisI8086(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_fpu),
      Config(TABLE),
      _opt_fpu(this, &Config::setFpuName, OPT_TEXT_FPU, OPT_DESC_FPU, &_opt_segInsn),
      _opt_segInsn(
              this, &DisI8086::setSegmentInsn, OPT_BOOL_SEG_INSN, OPT_DESC_SEG_INSN, &_opt_repInsn),
      _opt_repInsn(
              this, &DisI8086::setRepeatInsn, OPT_BOOL_REP_INSN, OPT_DESC_REP_INSN, &_opt_lockInsn),
      _opt_lockInsn(
              this, &DisI8086::setLockInsn, OPT_BOOL_LOCK_INSN, OPT_DESC_LOCK_INSN, &_opt_farInsn),
      _opt_farInsn(
              this, &DisI8086::setFarInsn, OPT_BOOL_FAR_INSN, OPT_DESC_FAR_INSN, &_opt_ptrSuffix),
      _opt_ptrSuffix(
              this, &DisI8086::setPtrSuffix, OPT_BOOL_PTR_SUFFIX, OPT_DESC_PTR_SUFFIX, &_opt_use16),
      _opt_use16(this, &Config::setUse16, OPT_BOOL_USE16, OPT_DESC_USE16, &_opt_use32),
      _opt_use32(this, &Config::setUse32, OPT_BOOL_USE32, OPT_DESC_USE32) {
    reset();
}

void DisI8086::reset() {
    Disassembler::reset();
    setFpuType(FPU_NONE);
#if !defined(LIBASM_I8086_NOFPU)
    setFpuType(FPU_ON);
#endif
    setSegmentInsn(false);
    setRepeatInsn(false);
    setLockInsn(false);
    setFarInsn(false);
    setModel(_cpuSpec.has32bit());
}

Error DisI8086::setSegmentInsn(bool enable) {
    _segInsn = enable;
    printf("@@@@@ segment-insn=%d\n", enable);
    return OK;
}

Error DisI8086::setRepeatInsn(bool enable) {
    _repInsn = enable;
    return OK;
}

Error DisI8086::setLockInsn(bool enable) {
    _lockInsn = enable;
    return OK;
}

Error DisI8086::setGnuAs(bool enable) {
    _farMode = enable ? FMODE_L : FMODE_FAR;
    return Disassembler::setGnuAs(enable);
}

Error DisI8086::setFarInsn(bool enable) {
    _farMode = enable ? FMODE_F : (_gnuAs ? FMODE_L : FMODE_FAR);
    return OK;
}

Error DisI8086::setPtrSuffix(bool enable) {
    _ptrSuffix = enable;
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
        num = insn.r_m();
        break;
    case P_REG:
        num = insn.reg();
        break;
    default:
        return REG_UNDEF;
    }
    switch (mode) {
    case M_BREG:
        return decodeByteReg(num);
    case M_WREG:
        return insn.size() == SZ_DATA && insn.useData32() ? decodeDwordReg(num)
                                                          : decodeWordReg(num);
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
    // M_REL
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
        if (_gnuAs && mode == M_REL && insn.hasData32())
            insn.addPrefix(TEXT_PRE_DATA32, out);
    } else {
        insn.setErrorIf(out, checkAddr(target, insn.address(), 16));
        outRelAddr(out, target, insn.address(), mode == M_REL8 ? 8 : 16);
    }
}

void DisI8086::decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    if (mode == M_WIMM && insn.size() == SZ_DATA) {
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
    } else if (mode == M_FAR) {
        const uint32_t offset = insn.useData32() ? insn.readUint32() : insn.readUint16();
        const auto offsetError = insn.getError();
        const auto segment = insn.readUint16();
        outHex(out, segment, 16).letter(':');
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
    case M_SREG:
        return SZ_WORD;
    case M_BMOD:
    case M_WMOD:
        return insn.mod() == 3 ? insn.size() : SZ_NONE;
    default:
        return SZ_NONE;
    }
}

OprSize operandSize(const DisInsn &insn) {
    if (insn.stringInsn())
        return insn.size();
    OprSize size;
    if ((size = operandSize(insn, insn.dst())) != SZ_NONE)
        return size;
    if ((size = operandSize(insn, insn.src())) != SZ_NONE)
        return size;
    return SZ_NONE;
}

OprSize pointerSize(const DisInsn &insn, AddrMode mode) {
    const auto size = insn.size();
    if (size == SZ_NONE)
        return size;
    switch (mode) {
    case M_BMOD:
    case M_BMEM:
        return SZ_BYTE;
    case M_WMOD:
    case M_WMEM:
        return SZ_WORD;
    default:
        return size;
    }
}

PrefixName pointerReg(const DisInsn &insn) {
    OprSize size;
    if ((size = pointerSize(insn, insn.dst())) == SZ_NONE &&
            (size = pointerSize(insn, insn.src())) == SZ_NONE)
        return PRE_UNDEF;
    return PrefixName(size + PRE_PTR);
}

}  // namespace

StrBuffer &DisI8086::outMemPrefix(DisInsn &insn, StrBuffer &out, RegName seg) const {
    if (insn.dst() == M_FARM)
        outPrefixName(out, PRE_FAR).letter(' ');
    if (insn.needSize()) {
        auto size = insn.size();
        if (size == SZ_DATA && insn.src() == M_BMOD)
            size = SZ_BYTE;  // MOVSX/MOVSZ r16/r32, r/m8
        if (size == SZ_DATA)
            size = _cpuSpec.has32bit() && insn.useData32() ? SZ_DWORD : SZ_WORD;
        const auto ptr = decodePrefixName(size);
        if (ptr != PRE_UNDEF) {
            outPrefixName(out, ptr).letter(' ');
            if (_ptrSuffix || _gnuAs)
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

    const auto r_m = insn.r_m();
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
    const auto mod = insn.mod();
    if (mod == 3) {
        if (mode >= M_BMEM && mode <= M_MEM)
            insn.setErrorIf(out, ILLEGAL_OPERAND);
        auto regMode = (mode == M_BMOD ? M_BREG : M_WREG);
        if (mode == M_DREG)
            regMode = mode;
        return;
    }
    if (insn.useAddr32()) {
        out32bitAddr(insn, out, mod);
        return;
    }
    outMemReg(insn, out, overrideSeg(_cpuSpec, insn.segment()), mod, insn.r_m());
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
        name = insn.size() == SZ_DATA && insn.useData32() ? REG_EAX : REG_AX;
        outRegister(out, name);
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
            if (insn.mod() != 3)
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
    case M_DMEM:
    case M_FARM:
    case M_FMEM:
    case M_MEM:
        if (insn.leaInsn() && insn.segment())
            insn.setErrorIf(out, OPCODE_HAS_NO_EFFECT);
        // Fall-through
    case M_BMOD:
    case M_WMOD:
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
    case M_SEG:  // M_SEG, M_OFF; off16/off32,seg16
        insn.farOffset = insn.useData32() ? insn.readUint32() : insn.readUint16();
        break;
    case M_OFF:
        outHex(out, insn.readUint16(), 16).comma();
        outAbsAddr(out, insn.farOffset, insn.data32() ? 32 : 16);
        break;
    case M_BDIR:
    case M_WDIR:
        outMemReg(insn, out, overrideSeg(_cpuSpec, insn.segment()), 0, 6);
        break;
    case M_REL:
    case M_REL8:
        decodeRelative(insn, out, mode);
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
    if (insn.stringInsn())
        return true;
    const auto mod = insn.mod();
    return memoryOperand(insn.dst(), mod) || memoryOperand(insn.src(), mod);
}

}  // namespace

void DisI8086::decodeStringInsn(DisInsn &insn, StrBuffer &out) const {
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

    opc = insn.readPrefixCodes(
            opc, _cpuSpec, _segInsn && !_gnuAs, _repInsn && !_gnuAs, _lockInsn && !_gnuAs);
    if (insn.getError())
        return insn.getError();

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
    searchOpCode(_cpuSpec, insn, out);
    return insn.getError();
}

StrBuffer &DisI8086::prependLockPrefix(DisInsn &insn, StrBuffer &out) const {
    // Save main instruction name
    char name[Insn::MAX_NAME + 1];
    strncpy(name, insn.name(), Insn::MAX_NAME);
    name[Insn::MAX_NAME] = 0;

    auto save{out};
    insn.nameBuffer().reset().over(out).text_P(TEXT_LOCK).letter(' ');

    if (!insn.lockCapable() || insn.mod() == 3)
        insn.setErrorIf(out.mark(), ILLEGAL_COMBINATION);

    out.text(name).over(insn.nameBuffer());
    return save.over(out);
}

StrBuffer &DisI8086::prependRepeatInsn(DisInsn &insn, StrBuffer &out) const {
    // Save main instruction name
    char name[Insn::MAX_NAME + 1];
    strncpy(name, insn.name(), Insn::MAX_NAME);
    name[Insn::MAX_NAME] = 0;

    DisInsn rep{insn};
    rep.setPrefix(0);
    rep.setOpCode(insn.repeat());
    searchOpCode(_cpuSpec, rep, out);
    // Save repeat instruction name
    char rep_name[Insn::MAX_NAME + 1];
    strcpy(rep_name, rep.name());

    auto save{out};
    insn.nameBuffer().reset().over(out).text(rep_name).letter(' ');

    if (!insn.stringInsn())
        insn.setErrorIf(out.mark(), ILLEGAL_COMBINATION);

    out.text(name).over(insn.nameBuffer());
    return save.over(out);
}

namespace {

bool imulHasSameDstSrc(const DisInsn &insn) {
    return (insn.opCode() & ~2) == 0x69 &&  // IMUL
           insn.mod() == 3 &&               // regsiter
           insn.r_m() == insn.reg();        // dst==src
}

}  // namespace

Config::opcode_t DisInsn::readPrefixCodes(
        Config::opcode_t opc, const CpuSpec &cpuSpec, bool segInsn, bool repInsn, bool lockInsn) {
    while (true) {
        if (!lockInsn && opc == DisInsn::LOCK_PREFIX) {
            if (_lock) {
                setErrorIf(OPCODE_HAS_NO_EFFECT);
                break;
            }
            _lock = opc;
        } else if (!segInsn && isSegmentPrefix(cpuSpec, opc)) {
            if (_segment) {
                setErrorIf(PREFIX_HAS_NO_EFFECT);
                break;
            }
            _segment = opc;
        } else if (!repInsn && isRepeatPrefix(cpuSpec, opc)) {
            if (_repeat) {
                setErrorIf(PREFIX_HAS_NO_EFFECT);
                break;
            }
            _repeat = opc;
        } else if (opc == DATA32) {
            if (!cpuSpec.has32bit())
                break;
            if (_hasData32) {
                setErrorIf(PREFIX_HAS_NO_EFFECT);
                break;
            }
            _hasData32 = _hasUnusedData32 = true;
        } else if (opc == ADDR32) {
            if (!cpuSpec.has32bit())
                break;
            if (_hasAddr32) {
                setErrorIf(PREFIX_HAS_NO_EFFECT);
                break;
            }
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

static constexpr auto CALLF = 0x9A;   // CALL far
static constexpr auto JMPF = 0xEA;    // JMPF far
static constexpr auto PUSHiw = 0x68;  // PUSH iw

bool DisInsn::needData32() const {
    const auto opc = opCode();
    return data32() && prefix() == 0 && (opc == CALLF || opc == JMPF || opc == PUSHiw);
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
    insn.setFarMode(_farMode);

#if 0
    if (_cpuSpec.cpu == I80386) {
        printf("@@ search:");
        if (insn.segment())
            printf(" %02X", insn.segment());
        if (insn.prefix() >= 0x100) {
            printf(" %02X %02X", insn.prefix() >> 8, insn.prefix() & 0xFF);
        } else if (insn.prefix()) {
            printf(" %02X", insn.prefix());
        }
        printf(" %02X\n", insn.opCode());
    }
#endif
    if (searchCodes(insn, out))
        return _insn.setError(insn);
#if 0
    if (_cpuSpec.cpu == I80386) {
        printf("@@  found: %s dst=%d/%d src=%d/%d ext=%d/%d: ", insn.name(), insn.dst(),
                insn.dstPos(), insn.src(), insn.srcPos(), insn.ext(), insn.extPos());
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
        if (insn.needSize())
            printf(" needSize");
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

    if (_gnuAs && insn.farInsn()) {
        char name[Insn::MAX_NAME + 1];
        strcpy(name, insn.name());
        name[insn.nameBuffer().len() - 1] = 0;             // omit 'F' suffix
        insn.nameBuffer().reset().letter('L').text(name);  // add 'L' prefix
    }
    if (insn.lock())
        prependLockPrefix(insn, out);
    if (insn.repeat())
        prependRepeatInsn(insn, out);
    if (insn.stringInsn()) {
        decodeStringInsn(insn, out);
    } else {
        decodeOperand(insn, out, insn.dst(), insn.dstPos());
        if (_gnuAs && insn.needData32() && !model32())
            insn.addPrefix(TEXT_PRE_DATA32, out);
        if (insn.src() != M_NONE && !imulHasSameDstSrc(insn)) {
            if (insn.src() != M_OFF)
                out.comma();
            decodeOperand(insn, out, insn.src(), insn.srcPos());
        }
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
