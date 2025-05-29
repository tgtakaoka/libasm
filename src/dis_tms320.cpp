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

#include "dis_tms320.h"
#include "reg_tms320.h"
#include "table_tms320.h"
#include "text_tms320.h"

namespace libasm {
namespace tms320 {

using namespace reg;
using namespace text::common;
using namespace text::tms320;

namespace {

const char OPT_BOOL_USE_REG_NAME[] PROGMEM = "use-aux-name";
const char OPT_DESC_USE_REG_NAME[] PROGMEM = "use aux register name ARn";
const char OPT_BOOL_USE_PORT_NAME[] PROGMEM = "use-port-name";
const char OPT_DESC_USE_PORT_NAME[] PROGMEM = "use port name PAn";

}  // namespace

const ValueFormatter::Plugins &DisTms320::defaultPlugins() {
    static const struct final : ValueFormatter::Plugins {
        const HexFormatter &hex() const override { return _hex; }
        char locationSymbol() const override { return '$'; }
        const /*PROGMEM*/ char *lineComment_P() const override { return PSTR_STAR; }
        const SuffixHexFormatter _hex{'h'};
    } PLUGINS{};
    return PLUGINS;
}

DisTms320::DisTms320(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_useAuxName),
      Config(TABLE),
      _opt_useAuxName(this, &DisTms320::setUseAuxName, OPT_BOOL_USE_REG_NAME, OPT_DESC_USE_REG_NAME,
              &_opt_usePortName),
      _opt_usePortName(
              this, &DisTms320::setUsePortName, OPT_BOOL_USE_PORT_NAME, OPT_DESC_USE_PORT_NAME) {
    reset();
}

void DisTms320::reset() {
    Disassembler::reset();
    setUseAuxName(true);
    setUsePortName(true);
}

Error DisTms320::setUseAuxName(bool enable) {
    _useAuxName = enable;
    return OK;
}

Error DisTms320::setUsePortName(bool enable) {
    _usePortName = enable;
    return OK;
}

StrBuffer &DisTms320::outDirect(StrBuffer &out, DisInsn &insn) const {
    const auto dma = toDmAddr(insn.opCode());
    if (!validDmAddr(insn.opCode(), dma))
        insn.setErrorIf(out, OVERFLOW_RANGE);
    outAbsAddr(out, dma, 8);
    return out;
}

StrBuffer &DisTms320::outIndirect(StrBuffer &out, uint8_t mam) const {
    const auto modify = (mam >> 4) & 7;
    out.letter('*');
    if (modify == 4 || modify == 7)
        out.text_P(PSTR("BR0"));
    if (modify == 5 || modify == 6)
        out.letter('0');
    if (modify == 1 || modify == 4 || modify == 5)
        return out.letter('-');
    if (modify == 2 || modify == 6 || modify == 7)
        return out.letter('+');
    return out;
}

StrBuffer &DisTms320::outAuxiliary(StrBuffer &out, uint_fast8_t no) const {
    no &= maxAR();
    if (_useAuxName)
        return outRegName(out, decodeAR(no));
    return outDec(out, no, 3);
}

StrBuffer &DisTms320::outPort(StrBuffer &out, uint_fast8_t no) const {
    no &= maxPA();
    if (_usePortName)
        return outRegName(out, decodePA(no));
    return outDec(out, no, 4);
};

StrBuffer &DisTms320::outProgramAddress(StrBuffer &out, DisInsn &insn) const {
    const auto pma = insn.readUint16();
    const auto error = insn.setErrorIf(out, checkAddr(pma));
    outAbsAddr(out, pma, error ? ADDRESS_16BIT : 0);
    return out;
}

bool DisTms320::hasValue(DisInsn &insn, AddrMode mode) const {
    const auto opc = insn.opCode();
    if (mode == M_NARP) {
        return is3201x() ? (opc & 0x88) == 0x80 : (opc & 0x88) == 0x88;
    } else if (mode == M_LS3) {
        return ((opc >> 8) & 7) != 0;
    } else if (mode == M_LS4) {
        return ((opc >> 8) & 0xF) != 0;
    } else if (mode == M_LS4L) {
        return (opc & 0xF) != 0;
    } else if (mode == M_MAR) {
        return (opc & 0xF0) != 0x80;
    } else if (mode == M_LS0) {
        return false;
    }
    return mode != M_NONE;
}

void DisTms320::decodeConditionCode(DisInsn &insn, StrBuffer &out) const {
    const auto opc = insn.opCode();
    const auto tp = (opc >> 8) & 3;
    auto sep = ',';
    if (tp == 0) {
        out.text_P(TEXT_CC_BIO);
    } else if (tp == 1) {
        out.text_P(TEXT_CC_TC);
    } else if (tp == 2) {
        out.text_P(TEXT_CC_NTC);
    } else {
        sep = 0;
    }
    const auto mask = opc & 0xF;
    if (mask == 0) {
        if (tp == 3)
            out.text_P(TEXT_CC_UNC);
        return;
    }
    if (sep)
        out.letter(sep);
    sep = ',';
    if ((mask & 0xC) == 0xC) {
        const auto val = (opc >> 4) & 0xC;
        if (val == 0xC) {
            out.text_P(TEXT_CC_LEQ);  // ZL
        } else if (val == 0x8) {
            out.text_P(TEXT_CC_GEQ);  // Z_
        } else if (val == 0x4) {
            out.text_P(TEXT_CC_LT);  // _L
        } else if (val == 0x0) {
            out.text_P(TEXT_CC_GT);  // __
        }
    } else if (mask & 8) {
        const auto val = (opc >> 4) & 8;
        out.text_P(val ? TEXT_CC_EQ : TEXT_CC_NEQ);
    } else if (mask & 4) {
        const auto val = (opc >> 4) & 4;
        out.text_P(val ? TEXT_CC_LT : TEXT_CC_GEQ);
    } else {
        sep = 0;
    }
    if (mask & 2) {  // V
        const auto val = (opc >> 4) & 2;
        if (sep)
            out.letter(sep);
        out.text_P(val ? TEXT_CC_OV : TEXT_CC_NOV);
        sep = ',';
    }
    if (mask & 1) {  // C
        const auto val = (opc >> 4) & 1;
        if (sep)
            out.letter(sep);
        out.text_P(val ? TEXT_CC_C : TEXT_CC_NC);
    }
}

void DisTms320::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto opc = insn.opCode();
    switch (mode) {
    case M_MAM:
        if (opc & 0x80) {
            outIndirect(out, opc);
        } else {
            outDirect(out, insn);
        }
        break;
    case M_IND:
    case M_MAR:
        outIndirect(out, opc);
        break;
    case M_NARP:
    case M_ARK:
        outAuxiliary(out, opc);
        break;
    case M_LS4:
    case M_BIT:
        outDec(out, opc >> 8, 4);
        break;
    case M_LS4L:
        outDec(out, opc, 4);
        break;
    case M_LS3:
    case M_LS0:
        outDec(out, opc >> 8, 3);
        break;
    case M_LS16:
        outDec(out, 16, 5);
        break;
    case M_AR:
        outAuxiliary(out, opc >> 8);
        break;
    case M_PA:
        outPort(out, opc >> 8);
        break;
    case M_STN:
        out.letter('#').letter((opc & 0x100) ? '1' : '0');
        break;
    case M_UI1:
        out.letter((opc & 1) == 0 ? '0' : '1');
        break;
    case M_UI2:
        outHex(out, opc & 3, 2);
        break;
    case M_UI5:
        outDec(out, opc & 0x1F, 5);
        break;
    case M_IMU8:
        out.letter('#');
        // Fall-through
    case M_UI8:
        outDec(out, static_cast<uint8_t>(opc), 8);
        break;
    case M_IMU9:
        out.letter('#');
        // Fall-through
    case M_UI9:
        outHex(out, opc & 0x1FF, 9, false);
        break;
    case M_IM13:
        out.letter('#');
        // Fall-through
    case M_SI13:
        // Sign extends 13-bit number as 0x1000 is a sign bit.
        outDec(out, signExtend(opc, 13), -13);
        break;
    case M_PM12:
        outProgramAddress(out, insn);
        break;
    case M_IM16:
    case M_IMU16:
        out.letter('#');
        // Fall-through
    case M_SI16:
    case M_UI16:
        outHex(out, insn.readUint16(), 16);
        break;
    case M_PA16:
    case M_PM16:
        outAbsAddr(out, insn.readUint16());
        break;
    case M_CC:
        decodeConditionCode(insn, out);
        break;
    case M_CTL: {
        const auto reg = decodeControlNum(opc >> 1);
        if (reg == REG_HM && is320C20x())
            insn.setErrorIf(out, ILLEGAL_REGISTER);
        outRegName(out, reg);
        break;
    }
    default:
        break;
    }
}

Error DisTms320::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    insn.setOpCode(insn.readUint16());
    if (searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    decodeOperand(insn, out, insn.mode1());
    const auto mode2 = insn.mode2();
    if (mode2 != M_NONE) {
        const auto mode3 = insn.mode3();
        if (hasValue(insn, mode2) || hasValue(insn, mode3)) {
            decodeOperand(insn, out.comma(), mode2);
            if (mode3 != M_NONE && hasValue(insn, mode3))
                decodeOperand(insn, out.comma(), mode3);
        }
    }
    return _insn.setError(insn);
}

}  // namespace tms320
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
