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

#include "entry_ns32000.h"
#include "reg_base.h"

namespace libasm {
namespace ns32000 {

enum RegName : char {
    REG_UNDEF = 0,
    REG_R0 = '0',  // R0
    REG_R1 = '1',  // R1
    REG_R2 = '2',  // R2
    REG_R3 = '3',  // R3
    REG_R4 = '4',  // R4
    REG_R5 = '5',  // R5
    REG_R6 = '6',  // R6
    REG_R7 = '7',  // R7
#ifdef ENABLE_FLOAT
    REG_F0 = 'A',  // F0
    REG_F1 = 'B',  // F1
    REG_F2 = 'C',  // F2
    REG_F3 = 'D',  // F3
    REG_F4 = 'E',  // F4
    REG_F5 = 'F',  // F5
    REG_F6 = 'G',  // F6
    REG_F7 = 'H',  // F7
#endif
    REG_FP = 'f',  // FP
    REG_SP = 's',  // SP
    REG_SB = 'b',  // SB
    REG_PC = 'p',  // PC
    REG_TOS = 't', // TOS
    REG_EXT = 'e', // EXT
};

enum PregName : uint8_t {
    PREG_UPSR    = 0,  // UPSR/US
    PREG_FP      = 8,  // FP
    PREG_SP      = 9,  // SP
    PREG_SB      = 10, // SB
    PREG_PSR     = 13, // PSR
    PREG_INTBASE = 14, // INTBASE
    PREG_MOD     = 15, // MOD
    PREG_UNDEF   = 16,
};

#ifdef ENABLE_MMU
enum MregName : uint8_t {
    MREG_BPR0  = 0,  // BPR0
    MREG_BPR1  = 1,  // BPR1
    MREG_MSR   = 10, // MSR
    MREG_BCNT  = 11, // BCNT
    MREG_PTB0  = 12, // PTB0
    MREG_PTB1  = 13, // PTB1
    MREG_EIA   = 15, // EIA
    MREG_UNDEF = 16,
};
#endif

enum ConfigName : uint8_t {
    CONFIG_UNDEF = 0,
    CONFIG_I = 1, // Vectored Interrupt
    CONFIG_F = 2, // Floating Point
    CONFIG_M = 4, // Memory Management
    CONFIG_C = 8, // Custom
};

enum StrOptName : uint8_t {
    STROPT_UNDEF = 0,
    STROPT_B = 2,  // Backward
    STROPT_W = 4,  // While Match
    STROPT_U = 12, // Until Match
};

class RegNs32000 : public RegBase {
public:
    RegName parseRegName(const char *line) const;
    RegName decodeRegName(uint8_t num, bool floating = false) const;
    int8_t encodeRegName(RegName name) const;
    uint8_t regNameLen(RegName name) const;
    char *outRegName(char *out, const RegName name) const;
    bool isGeneric(RegName name) const;
#ifdef ENABLE_FLOAT
    bool isFloat(RegName name) const;
#endif

    PregName parsePregName(const char *line) const;
    PregName decodePregName(uint8_t num) const;
    int8_t encodePregName(PregName name) const;
    uint8_t pregNameLen(PregName name) const;
    char *outPregName(char *out, PregName name) const;

#ifdef ENABLE_MMU
    MregName parseMregName(const char *line) const;
    MregName decodeMregName(uint8_t num) const;
    int8_t encodeMregName(MregName name) const;
    uint8_t mregNameLen(MregName name) const;
    char *outMregName(char *out, MregName name) const;
#endif

    ConfigName parseConfigName(const char *line) const;
    ConfigName decodeConfigName(uint8_t num) const;
    uint8_t configNameLen(ConfigName name) const;
    char *outConfigName(char *out, ConfigName name) const;

    StrOptName parseStrOptName(const char *line) const;
    StrOptName decodeStrOptName(uint8_t num) const;
    uint8_t strOptNameLen(StrOptName name) const;
    char *outStrOptName(char *out, StrOptName name) const;

    OprSize parseIndexSize(const char *line) const;
    uint8_t indexSizeLen(OprSize size) const;
    char indexSizeChar(OprSize size) const;

private:
    char *outChar(char *out, char c) const;
    char *outText(char *out, const char *text) const;
};

} // namespace ns32000
} // namespace libasm

#endif // __REG_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
