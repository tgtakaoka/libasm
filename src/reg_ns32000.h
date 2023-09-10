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

#ifndef __REG_NS32000_H__
#define __REG_NS32000_H__

#include <stdint.h>

#include "entry_ns32000.h"
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {
namespace ns32000 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // General registers.
    REG_R0 = 0 + 0,
    REG_R1 = 1 + 0,
    REG_R2 = 2 + 0,
    REG_R3 = 3 + 0,
    REG_R4 = 4 + 0,
    REG_R5 = 5 + 0,
    REG_R6 = 6 + 0,
    REG_R7 = 7 + 0,
    // NS32081 Floating point registers.
    REG_F0 = 0 + 8,
    REG_F1 = 1 + 8,
    REG_F2 = 2 + 8,
    REG_F3 = 3 + 8,
    REG_F4 = 4 + 8,
    REG_F5 = 5 + 8,
    REG_F6 = 6 + 8,
    REG_F7 = 7 + 8,
    REG_FP = 'F',   // FP
    REG_SP = 'S',   // SP
    REG_SB = 'B',   // SB
    REG_PC = 'P',   // PC
    REG_TOS = 'T',  // TOS
    REG_EXT = 'E',  // EXT
};

enum PregName : int8_t {
    PREG_UNDEF = -1,
    PREG_UPSR = 0,      // UPSR/US
    PREG_FP = 8,        // FP
    PREG_SP = 9,        // SP
    PREG_SB = 10,       // SB
    PREG_PSR = 13,      // PSR
    PREG_INTBASE = 14,  // INTBASE
    PREG_MOD = 15,      // MOD
};

enum MregName : int8_t {
    MREG_UNDEF = -1,
    MREG_BPR0 = 0,   // BPR0
    MREG_BPR1 = 1,   // BPR1
    MREG_MSR = 10,   // MSR
    MREG_BCNT = 11,  // BCNT
    MREG_PTB0 = 12,  // PTB0
    MREG_PTB1 = 13,  // PTB1
    MREG_EIA = 15,   // EIA
};

enum ConfigName : int8_t {
    CONFIG_UNDEF = 0,
    CONFIG_I = 1,  // Vectored Interrupt
    CONFIG_F = 2,  // Floating Point
    CONFIG_M = 4,  // Memory Management
    CONFIG_C = 8,  // Custom
};

enum StrOptName : int8_t {
    STROPT_UNDEF = 0,
    STROPT_B = 0x2,  // Backward
    STROPT_W = 0x4,  // While Match
    STROPT_U = 0xC,  // Until Match
};

namespace reg {

RegName parseRegName(StrScanner &scan);
StrBuffer &outRegName(StrBuffer &out, RegName name);
uint8_t encodeRegName(RegName name);
bool isGeneric(RegName name);
RegName decodeRegName(uint8_t num, bool floating = false);
bool isFloat(RegName name);
bool isRegPair(RegName name);

PregName parsePregName(StrScanner &scan);
StrBuffer &outPregName(StrBuffer &out, PregName name);
PregName decodePregName(uint8_t num);
uint8_t encodePregName(PregName name);

MregName parseMregName(StrScanner &scan);
StrBuffer &outMregName(StrBuffer &out, MregName name);
MregName decodeMregName(uint8_t num);
uint8_t encodeMregName(MregName name);

ConfigName parseConfigName(StrScanner &scan);
StrBuffer &outConfigNames(StrBuffer &out, uint8_t configs);

StrOptName parseStrOptName(StrScanner &scan);
StrBuffer &outStrOptNames(StrBuffer &out, uint8_t strOpts);

OprSize parseIndexSize(StrScanner &scan);
char indexSizeChar(OprSize size);

}  // namespace reg
}  // namespace ns32000
}  // namespace libasm

#endif  // __REG_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
