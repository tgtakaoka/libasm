/*
 * Copyright 2026 Tadashi G. Takaoka
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

#ifndef __LIBASM_REG_H16_H__
#define __LIBASM_REG_H16_H__

#include <stdint.h>
#include "str_buffer.h"
#include "str_scanner.h"
#include "value_parser.h"

namespace libasm {
namespace h16 {
namespace reg {

// Registers.  Values are split into three 16-entry banks so the prefix
// (R/CR/PR) is encoded in the upper nibble and the register number in the
// low nibble.  This makes parsing (prefix + number) and encoding (low 4
// bits = Rn field in the EA byte) trivial.
//   R0..R15  : global bank (0x00..0x0F)
//   CR0..CR15: current bank (0x10..0x1F)  -- emitted as <CRn> EA prefix
//   PR0..PR15: previous bank (0x20..0x2F) -- emitted as <PRn> EA prefix
enum RegName : int8_t {
    REG_UNDEF = -1,
    REG_R0   = 0x00,
    REG_R15  = 0x0F,
    REG_CR0  = 0x10,
    REG_CR15 = 0x1F,
    REG_PR0  = 0x20,
    REG_PR15 = 0x2F,
};

// Helpers for the three banks (R/CR/PR).
static inline bool isGlobalReg(RegName r)  { return r >= REG_R0   && r <= REG_R15;  }
static inline bool isCurBankReg(RegName r) { return r >= REG_CR0  && r <= REG_CR15; }
static inline bool isPrevBankReg(RegName r){ return r >= REG_PR0  && r <= REG_PR15; }

// Extract the 4-bit register number (the Rn field encoded in the EA byte).
static inline uint8_t regNum(RegName r) { return uint8_t(r) & 0x0F; }

// Construct a RegName from a bank and register number.
static inline RegName makeGlobalReg(uint8_t n)   { return RegName(0x00 | (n & 0x0F)); }
static inline RegName makeCurBankReg(uint8_t n)  { return RegName(0x10 | (n & 0x0F)); }
static inline RegName makePrevBankReg(uint8_t n) { return RegName(0x20 | (n & 0x0F)); }

// Control registers.  Values are the CR-code bytes from Table 16-10, so
// encoding is just `uint8_t(name)` and decoding is a direct switch on the
// byte value (see crFromCode).  CR_UNDEF uses 0x00 which is not a valid
// CR code (the lowest used is VBNR=0x01).
enum CrName : uint8_t {
    CR_UNDEF = 0x00,
    CR_VBNR  = 0x01,  // byte
    CR_CCR   = 0x20,  // word
    CR_CBNR  = 0x40,  // long
    CR_BSP   = 0x41,  // long
    CR_BMR   = 0x80,  // byte  privileged
    CR_GBNR  = 0x81,  // byte  privileged
    CR_SR    = 0xA0,  // word  privileged
    CR_EBR   = 0xC0,  // long  privileged
    CR_RBR   = 0xC1,  // long  privileged
    CR_USP   = 0xC2,  // long  privileged
    CR_IBR   = 0xC3,  // long  privileged
};

// Condition codes used in Bcc:G, SET, SCB, TRAP instructions.  Values match
// the Table 16-8 "Code" column so encoding is `byte = name & 0x0F`.  HS and
// LO are offsetted aliases for CC and CS (low 4 bits identical so they
// encode identically).
enum CcName : int8_t {
    CC_UNDEF = -1,
    CC_T  = 0x00,  // always true
    CC_F  = 0x01,  // always false
    CC_HI = 0x02,  // high (unsigned >)
    CC_LS = 0x03,  // lower or same (unsigned <=)
    CC_CC = 0x04,  // carry clear / higher or same
    CC_CS = 0x05,  // carry set / lower
    CC_NE = 0x06,  // not equal
    CC_EQ = 0x07,  // equal
    CC_VC = 0x08,  // overflow clear
    CC_VS = 0x09,  // overflow set
    CC_PL = 0x0A,  // plus
    CC_MI = 0x0B,  // minus
    CC_GE = 0x0C,  // greater or equal (signed >=)
    CC_LT = 0x0D,  // less than (signed <)
    CC_GT = 0x0E,  // greater than (signed >)
    CC_LE = 0x0F,  // less or equal (signed <=)
    // Aliases: same low 4 bits as the canonical name; the 0x10 offset keeps
    // the enum value unique so disassembly can choose the canonical form.
    CC_HS = 0x14,  // alias for CC (higher-or-same)
    CC_LO = 0x15,  // alias for CS (lower)
};

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
CrName parseCrName(StrScanner &scan, const ValueParser &parser);
CcName parseCcName(StrScanner &scan, const ValueParser &parser);

void outRegName(StrBuffer &out, RegName reg);
void outCrName(StrBuffer &out, CrName cr);
void outCcName(StrBuffer &out, CcName cc);

// Returns the CR-code byte for a given control register.
uint8_t crCode(CrName cr);
// Returns the control register for a given CR-code byte, or CR_UNDEF.
CrName crFromCode(uint8_t code);

}  // namespace reg
}  // namespace h16
}  // namespace libasm

#endif  // __LIBASM_REG_H16_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
