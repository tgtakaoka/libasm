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

#include "reg_z80.h"

#include "table_z80.h"
#include "text_z80.h"

using namespace libasm::text::z80;
using namespace libasm::reg;

namespace libasm {
namespace z80 {
namespace reg {

static constexpr NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_BC),
        NAME_ENTRY(REG_DE),
        NAME_ENTRY(REG_HL),
        NAME_ENTRY(REG_IX),
        NAME_ENTRY(REG_IY),
        NAME_ENTRY(REG_SP),
        NAME_ENTRY(REG_AFP),
        NAME_ENTRY(REG_AF),
        NAME_ENTRY(REG_A),
        NAME_ENTRY(REG_B),
        NAME_ENTRY(REG_C),
        NAME_ENTRY(REG_D),
        NAME_ENTRY(REG_E),
        NAME_ENTRY(REG_H),
        NAME_ENTRY(REG_L),
        NAME_ENTRY(REG_IM),
        NAME_ENTRY(REG_R),
        NAME_ENTRY(REG_I),
};

RegName parseRegName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out.text_P(entry->text_P());
    return out;
}

uint8_t encodeDataReg(RegName name) {
    // (HL) is parsed as I_HL, then looked up as M_REG(reg=REG_HL), so
    // we have to map REG_HL to register number 6.
    if (name == REG_HL)
        return 6;
    return int8_t(name) - 8;
}

RegName decodeDataReg(uint8_t num) {
    // REG_HL represents (HL).
    if ((num &= 7) == 6)
        return REG_HL;
    return RegName(num + 8);
}

uint8_t encodePointerReg(RegName name) {
    return int8_t(name);
}

uint8_t encodePointerRegIx(RegName name, RegName ix) {
    return encodePointerReg(name == ix ? REG_HL : name);
}

RegName decodePointerReg(uint8_t num, const InsnZ80 &insn) {
    const auto name = RegName(num & 3);
    if (name == REG_HL) {
        const auto ix = decodeIndexReg(insn);
        return ix == REG_UNDEF ? name : ix;
    }
    return name;
}

uint8_t encodeStackReg(RegName name) {
    if (name == REG_AF)
        return 3;
    return uint8_t(name);
}

RegName decodeStackReg(uint8_t num) {
    if ((num &= 3) == 3)
        return REG_AF;
    return RegName(num);
}

uint8_t encodeIndirectBase(RegName name) {
    return uint8_t(name);
}

RegName decodeIndirectBase(uint8_t num) {
    return RegName(num & 1);
}

void encodeIndexReg(InsnZ80 &insn, RegName ixReg) {
    const Config::opcode_t prefix = (ixReg == REG_IX) ? TableZ80::PREFIX_IX : TableZ80::PREFIX_IY;
    insn.setOpCode(insn.opCode(), prefix);
}

RegName decodeIndexReg(const InsnZ80 &insn) {
    const Config::opcode_t prefix = insn.prefix();
    if (prefix == TableZ80::PREFIX_IX)
        return REG_IX;
    if (prefix == TableZ80::PREFIX_IY)
        return REG_IY;
    return REG_UNDEF;
}

uint8_t encodeIrReg(RegName name) {
    return uint8_t(name) - 16;
}

RegName decodeIrReg(uint8_t num) {
    return RegName((num & 1) + 16);
}

static constexpr NameEntry CC_TABLE[] PROGMEM = {
        NAME_ENTRY(CC_NZ),
        NAME_ENTRY(CC_Z),
        NAME_ENTRY(CC_NC),
        NAME_ENTRY(CC_C),
        NAME_ENTRY(CC_PO),
        NAME_ENTRY(CC_PE),
        NAME_ENTRY(CC_P),
        NAME_ENTRY(CC_M),
};

CcName parseCcName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(CC_TABLE));
    return entry ? CcName(entry->name()) : CC_UNDEF;
}

StrBuffer &outCcName(StrBuffer &out, const CcName name) {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(CC_TABLE));
    if (entry)
        out.text_P(entry->text_P());
    return out;
}

bool isCc4Name(CcName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 4;
}

uint8_t encodeCcName(const CcName name) {
    return uint8_t(name);
}

CcName decodeCcName(uint8_t num) {
    return CcName(num & 7);
}

}  // namespace reg
}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
