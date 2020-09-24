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

#include "config_z80.h"
#include "reg_z80.h"
#include "table_z80.h"
#include "reg_z80.h"

#include <ctype.h>

namespace libasm {
namespace z80 {

static const char TEXT_REG_BC[]  PROGMEM = "BC";
static const char TEXT_REG_DE[]  PROGMEM = "DE";
static const char TEXT_REG_HL[]  PROGMEM = "HL";
static const char TEXT_REG_IX[]  PROGMEM = "IX";
static const char TEXT_REG_IY[]  PROGMEM = "IY";
static const char TEXT_REG_SP[]  PROGMEM = "SP";
static const char TEXT_REG_AFP[] PROGMEM = "AF'";
static const char TEXT_REG_AF[]  PROGMEM = "AF";
static const char TEXT_REG_A[]   PROGMEM = "A";
static const char TEXT_REG_B[]   PROGMEM = "B";
static const char TEXT_REG_C[]   PROGMEM = "C";
static const char TEXT_REG_D[]   PROGMEM = "D";
static const char TEXT_REG_E[]   PROGMEM = "E";
static const char TEXT_REG_H[]   PROGMEM = "H";
static const char TEXT_REG_L[]   PROGMEM = "L";
static const char TEXT_REG_IM[]  PROGMEM = "IM";
static const char TEXT_REG_R[]   PROGMEM = "R";
static const char TEXT_REG_I[]   PROGMEM = "I";
static const RegBase::NameEntry REG_TABLE[] PROGMEM = {
     NAME_ENTRY(REG_BC)
     NAME_ENTRY(REG_DE)
     NAME_ENTRY(REG_HL)
     NAME_ENTRY(REG_IX)
     NAME_ENTRY(REG_IY)
     NAME_ENTRY(REG_SP)
     NAME_ENTRY(REG_AFP)
     NAME_ENTRY(REG_AF)
     NAME_ENTRY(REG_A)
     NAME_ENTRY(REG_B)
     NAME_ENTRY(REG_C)
     NAME_ENTRY(REG_D)
     NAME_ENTRY(REG_E)
     NAME_ENTRY(REG_H)
     NAME_ENTRY(REG_L)
     NAME_ENTRY(REG_IM)
     NAME_ENTRY(REG_R)
     NAME_ENTRY(REG_I)
};

RegName RegZ80::parseRegName(const char *line) {
    const NameEntry *entry = searchText(line, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

uint8_t RegZ80::regNameLen(RegName name) {
    return nameLen(uint8_t(name), ARRAY_RANGE(REG_TABLE));
}

char *RegZ80::outRegName(char *out, RegName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out = outText(out, entry->text());
    return out;
}

uint8_t RegZ80::encodeDataReg(RegName name) {
    // (HL) is parsed as I_HL, then looked up as M_REG(reg=REG_HL), so
    // we have to map REG_HL to register number 6.
    if (name == REG_HL) return 6;
    return int8_t(name) - 8;
}

RegName RegZ80::decodeDataReg(uint8_t num) {
    // REG_HL represents (HL).
    if ((num &= 7) == 6) return REG_HL;
    return RegName(num + 8);
}

uint8_t RegZ80::encodePointerReg(RegName name) {
    return int8_t(name);
}

uint8_t RegZ80::encodePointerRegIx(RegName name, RegName ix) {
    return encodePointerReg(name == ix ? REG_HL : name);
}

RegName RegZ80::decodePointerReg(uint8_t num, const InsnZ80 &insn) {
    const RegName name = RegName(num & 3);
    if (name == REG_HL) {
        const RegName ix = decodeIndexReg(insn);
        return ix == REG_UNDEF ? name : ix;
    }
    return name;
}

uint8_t RegZ80::encodeStackReg(RegName name) {
    if (name == REG_AF) return 3;
    return uint8_t(name);
}

RegName RegZ80::decodeStackReg(uint8_t num) {
    if ((num &= 3) == 3) return REG_AF;
    return RegName(num);
}

uint8_t RegZ80::encodeIndirectBase(RegName name) {
    return uint8_t(name);
}

RegName RegZ80::decodeIndirectBase(uint8_t num) {
    return RegName(num & 1);
}

void RegZ80::encodeIndexReg(InsnZ80 &insn, RegName ixReg) {
    const Config::opcode_t prefix =
        (ixReg == REG_IX) ? TableZ80::PREFIX_IX : TableZ80::PREFIX_IY;
    insn.setOpCode(insn.opCode(), prefix);
}

RegName RegZ80::decodeIndexReg(const InsnZ80 &insn) {
    const Config::opcode_t prefix = insn.prefix();
    if (prefix == TableZ80::PREFIX_IX) return REG_IX;
    if (prefix == TableZ80::PREFIX_IY) return REG_IY;
    return REG_UNDEF;
}

uint8_t RegZ80::encodeIrReg(RegName name) {
    return uint8_t(name) - 16;
}

RegName RegZ80::decodeIrReg(uint8_t num) {
    return RegName((num & 1) + 16);
}

static const char TEXT_CC_NZ[] PROGMEM = "NZ";
static const char TEXT_CC_Z[]  PROGMEM = "Z";
static const char TEXT_CC_NC[] PROGMEM = "NC";
static const char TEXT_CC_C[]  PROGMEM = "C";
static const char TEXT_CC_PO[] PROGMEM = "PO";
static const char TEXT_CC_PE[] PROGMEM = "PE";
static const char TEXT_CC_P[]  PROGMEM = "P";
static const char TEXT_CC_M[]  PROGMEM = "M";
static const RegBase::NameEntry CC_TABLE[] PROGMEM = {
    NAME_ENTRY(CC_NZ)
    NAME_ENTRY(CC_Z)
    NAME_ENTRY(CC_NC)
    NAME_ENTRY(CC_C)
    NAME_ENTRY(CC_PO)
    NAME_ENTRY(CC_PE)
    NAME_ENTRY(CC_P)
    NAME_ENTRY(CC_M)
};

CcName RegZ80::parseCcName(const char *line) {
    const NameEntry *entry = searchText(line, ARRAY_RANGE(CC_TABLE));
    return entry ? CcName(entry->name()) : CC_UNDEF;
}

uint8_t RegZ80::ccNameLen(const CcName name) {
    return nameLen(uint8_t(name), ARRAY_RANGE(CC_TABLE));
}

char *RegZ80::outCcName(char *out, const CcName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(CC_TABLE));
    if (entry)
        out = outText(out, entry->text());
    return out;
}

bool RegZ80::isCc4Name(CcName name) {
    const int8_t num = int8_t(name);
    return num >= 0 && num < 4;
}

uint8_t RegZ80::encodeCcName(const CcName name) {
    return uint8_t(name);
}

CcName RegZ80::decodeCcName(uint8_t num) {
    return CcName(num & 7);
}

} // namespace z80
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
