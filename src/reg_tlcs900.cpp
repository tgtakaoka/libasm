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

#include "reg_tlcs900.h"
#include "reg_base.h"
#include "text_tlcs900.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::tlcs900;

namespace libasm {
namespace tlcs900 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_A,    REG_A    },
    { TEXT_REG_B,    REG_B    },
    { TEXT_REG_BC,   REG_BC   },
    { TEXT_REG_C,    REG_C    },
    { TEXT_REG_D,    REG_D    },
    { TEXT_REG_DE,   REG_DE   },
    { TEXT_REG_E,    REG_E    },
    { TEXT_REG_H,    REG_H    },
    { TEXT_REG_HL,   REG_HL   },
    { TEXT_REG_IX,   REG_IX   },
    { TEXT_REG_IY,   REG_IY   },
    { TEXT_REG_IZ,   REG_IZ   },
    { TEXT_REG_L,    REG_L    },
    { TEXT_REG_NSP,  REG_NSP  },
    { TEXT_REG_SP,   REG_SP   },
    { TEXT_REG_SR,   REG_SR   },
    { TEXT_REG_W,    REG_W    },
    { TEXT_REG_WA,   REG_WA   },
    { TEXT_REG_XBC,  REG_XBC  },
    { TEXT_REG_XDE,  REG_XDE  },
    { TEXT_REG_XHL,  REG_XHL  },
    { TEXT_REG_XIX,  REG_XIX  },
    { TEXT_REG_XIY,  REG_XIY  },
    { TEXT_REG_XIZ,  REG_XIZ  },
    { TEXT_REG_XNSP, REG_XNSP },
    { TEXT_REG_XSP,  REG_XSP  },
    { TEXT_REG_XWA,  REG_XWA  },
};

constexpr NameEntry CC_ENTRIES[] PROGMEM = {
    { TEXT_CC_C,   CC_C   },
    { TEXT_CC_EQ,  CC_EQ  },
    { TEXT_CC_F,   CC_F   },
    { TEXT_CC_GE,  CC_GE  },
    { TEXT_CC_GT,  CC_GT  },
    { TEXT_CC_LE,  CC_LE  },
    { TEXT_CC_LT,  CC_LT  },
    { TEXT_CC_M,   CC_M   },
    { TEXT_CC_MI,  CC_MI  },
    { TEXT_CC_NC,  CC_NC  },
    { TEXT_CC_NE,  CC_NE  },
    { TEXT_CC_NOV, CC_NOV },
    { TEXT_CC_NZ,  CC_NZ  },
    { TEXT_CC_OV,  CC_OV  },
    { TEXT_CC_P,   CC_P   },
    { TEXT_CC_PE,  CC_PE  },
    { TEXT_CC_PL,  CC_PL  },
    { TEXT_CC_PO,  CC_PO  },
    { TEXT_CC_T,   CC_T   },
    { TEXT_CC_UGE, CC_UGE },
    { TEXT_CC_UGT, CC_UGT },
    { TEXT_CC_ULE, CC_ULE },
    { TEXT_CC_ULT, CC_ULT },
    { TEXT_CC_Z,   CC_Z   },
};

PROGMEM constexpr NameTable REG_TABLE{ARRAY_RANGE(REG_ENTRIES)};
PROGMEM constexpr NameTable CC_TABLE{ARRAY_RANGE(CC_ENTRIES)};

// Absolute-bank register tables. FIXED tables hold the byte-halves and the
// Q-variants of IX/IY/IZ/SP whose encoded code is 0xF0-0xFF and which never
// take a bank-digit or '\'' suffix. REL tables hold the bank-relative
// registers whose name field is the bank offset (0x00-0x0F), and the caller
// composes the final byte by OR-ing in (bank<<4), 0xD0 (prev), or 0xE0
// (current). Sorted by text for binary search.

constexpr NameEntry ABREG8_FIXED_ENTRIES[] PROGMEM = {
    { TEXT_REG_IXH,  int8_t(0xF1) },
    { TEXT_REG_IXL,  int8_t(0xF0) },
    { TEXT_REG_IYH,  int8_t(0xF5) },
    { TEXT_REG_IYL,  int8_t(0xF4) },
    { TEXT_REG_IZH,  int8_t(0xF9) },
    { TEXT_REG_IZL,  int8_t(0xF8) },
    { TEXT_REG_QIXH, int8_t(0xF3) },
    { TEXT_REG_QIXL, int8_t(0xF2) },
    { TEXT_REG_QIYH, int8_t(0xF7) },
    { TEXT_REG_QIYL, int8_t(0xF6) },
    { TEXT_REG_QIZH, int8_t(0xFB) },
    { TEXT_REG_QIZL, int8_t(0xFA) },
    { TEXT_REG_QSPH, int8_t(0xFF) },
    { TEXT_REG_QSPL, int8_t(0xFE) },
    { TEXT_REG_SPH,  int8_t(0xFD) },
    { TEXT_REG_SPL,  int8_t(0xFC) },
};
constexpr NameEntry ABREG8_REL_ENTRIES[] PROGMEM = {
    // In-bank layout: A=0,W=1,QA=2,QW=3,C=4,B=5,QC=6,QB=7,E=8,D=9,QE=A,QD=B,L=C,H=D,QL=E,QH=F
    { TEXT_REG_A,  0x0 },
    { TEXT_REG_B,  0x5 },
    { TEXT_REG_C,  0x4 },
    { TEXT_REG_D,  0x9 },
    { TEXT_REG_E,  0x8 },
    { TEXT_REG_H,  0xD },
    { TEXT_REG_L,  0xC },
    { TEXT_REG_QA, 0x2 },
    { TEXT_REG_QB, 0x7 },
    { TEXT_REG_QC, 0x6 },
    { TEXT_REG_QD, 0xB },
    { TEXT_REG_QE, 0xA },
    { TEXT_REG_QH, 0xF },
    { TEXT_REG_QL, 0xE },
    { TEXT_REG_QW, 0x3 },
    { TEXT_REG_W,  0x1 },
};
// 16/32-bit FIXED hold only Q-variants. Plain IX/IY/IZ/SP and
// XIX/XIY/XIZ/XSP stay as M_REG16/M_REG32 on input; outAbsReg falls back
// to outRegName for their codes (0xF0/0xF4/0xF8/0xFC).
constexpr NameEntry ABREG16_FIXED_ENTRIES[] PROGMEM = {
    { TEXT_REG_QIX, int8_t(0xF2) },
    { TEXT_REG_QIY, int8_t(0xF6) },
    { TEXT_REG_QIZ, int8_t(0xFA) },
    { TEXT_REG_QSP, int8_t(0xFE) },
};
constexpr NameEntry ABREG16_REL_ENTRIES[] PROGMEM = {
    { TEXT_REG_BC,  0x4 },
    { TEXT_REG_DE,  0x8 },
    { TEXT_REG_HL,  0xC },
    { TEXT_REG_QBC, 0x6 },
    { TEXT_REG_QDE, 0xA },
    { TEXT_REG_QHL, 0xE },
    { TEXT_REG_QWA, 0x2 },
    { TEXT_REG_WA,  0x0 },
};
// 32-bit FIXED has no parse-eligible entries (no Q-variants exist for 32-bit).
// Decoding codes 0xF0/0xF4/0xF8/0xFC at size 2 falls back to outRegName.
constexpr NameEntry ABREG32_REL_ENTRIES[] PROGMEM = {
    { TEXT_REG_XBC, 0x4 },
    { TEXT_REG_XDE, 0x8 },
    { TEXT_REG_XHL, 0xC },
    { TEXT_REG_XWA, 0x0 },
};
PROGMEM constexpr NameTable ABREG8_FIXED_TABLE{ARRAY_RANGE(ABREG8_FIXED_ENTRIES)};
PROGMEM constexpr NameTable ABREG8_REL_TABLE{ARRAY_RANGE(ABREG8_REL_ENTRIES)};
PROGMEM constexpr NameTable ABREG16_FIXED_TABLE{ARRAY_RANGE(ABREG16_FIXED_ENTRIES)};
PROGMEM constexpr NameTable ABREG16_REL_TABLE{ARRAY_RANGE(ABREG16_REL_ENTRIES)};
PROGMEM constexpr NameTable ABREG32_REL_TABLE{ARRAY_RANGE(ABREG32_REL_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = REG_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return RegName(entry->name());
    }
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto *entry = REG_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

bool isReg8(RegName name) {
    const auto n = int8_t(name);
    return n >= int8_t(REG_W) && n <= int8_t(REG_L);
}

bool isReg16(RegName name) {
    const auto n = int8_t(name);
    return n >= int8_t(REG_WA) && n <= int8_t(REG_SP);
}

bool isReg32(RegName name) {
    const auto n = int8_t(name);
    return n >= int8_t(REG_XWA) && n <= int8_t(REG_XSP);
}

bool isIndexReg(RegName name) {
    // XIX..XSP: usable as base register in indirect/indexed addressing
    const auto n = int8_t(name);
    return n >= int8_t(REG_XIX) && n <= int8_t(REG_XSP);
}

// Current-bank 8-bit register index (0=W,1=A,2=B,...,7=L)
uint8_t encodeReg8(RegName name) {
    return uint8_t(name);  // REG_W=0..REG_L=7
}

RegName decodeReg8(uint8_t num) {
    return RegName(num & 7);  // 0..7 → REG_W..REG_L
}

// Current-bank 16-bit register index (0=WA,1=BC,...,7=SP)
uint8_t encodeReg16(RegName name) {
    return uint8_t(name) - 8;
}

RegName decodeReg16(uint8_t num) {
    return RegName((num & 7) + 8);
}

// Current-bank 32-bit register index (0=XWA,1=XBC,...,7=XSP)
uint8_t encodeReg32(RegName name) {
    return uint8_t(name) - 16;
}

RegName decodeReg32(uint8_t num) {
    return RegName((num & 7) + 16);
}

// Encode 32-bit index register for indirect addressing (0=XIX,1=XIY,2=XIZ,3=XSP)
// Prefix byte = 0x84 + encodeIndReg (for byte-source indirect)
uint8_t encodeIndReg(RegName name) {
    return uint8_t(name) - uint8_t(REG_XIX);
}

RegName decodeIndReg(uint8_t num) {
    return RegName((num & 3) + uint8_t(REG_XIX));
}

bool isCtrlReg(RegName name) {
    return name == REG_NSP || name == REG_XNSP || name == REG_INTNEST ||
           (uint8_t(name) >= uint8_t(REG_DMAS0) && uint8_t(name) <= uint8_t(REG_DMAM3));
}

uint8_t ctrlRegCode(RegName name) {
    if (name == REG_NSP || name == REG_XNSP || name == REG_INTNEST)
        return 0x3C;
    return uint8_t(name) - uint8_t(REG_DMAS0);  // = uint8_t(name) - 0x40
}

Size ctrlRegSize(RegName name) {
    if (name == REG_XNSP)
        return SZ_QUAD;
    if (name == REG_NSP)
        return SZ_WORD;
    if (name == REG_INTNEST)
        return SZ_WORD;
    const uint8_t sub = ctrlRegCode(name);
    if (sub <= 0x1C)
        return SZ_QUAD;                    // DMAS0-3, DMAD0-3: 32-bit
    return (sub & 2) ? SZ_BYTE : SZ_WORD;  // DMAM (bit1=1): 8-bit; DMAC (bit1=0): 16-bit
}

// sub 0x3C is shared by NSP/XNSP (base only) and INTNEST (non-base only); disambiguate by CPU.
RegName decodeCtrlReg(uint8_t sub, bool is32, bool isBase) {
    if (sub == 0x3C) {
        if (is32)
            return isBase ? REG_XNSP : REG_UNDEF;
        return isBase ? REG_NSP : REG_INTNEST;
    }
    if (sub & 1)
        return REG_UNDEF;  // odd sub-bytes invalid
    if (sub <= 0x1C)
        return (sub & 3) ? REG_UNDEF : RegName(sub + uint8_t(REG_DMAS0));
    if (sub <= 0x2E)
        return RegName(sub + uint8_t(REG_DMAS0));
    return REG_UNDEF;
}

RegName parseCtrlRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto token = parser.readRegName(p);
    if (token.size() == 0)
        return REG_UNDEF;
    // NSP / XNSP / INTNEST (all share ctrl sub-byte 0x3C; CPU gates which name is valid).
    if (token.iequals_P(TEXT_REG_NSP)) {
        scan = p;
        return REG_NSP;
    }
    if (token.iequals_P(TEXT_REG_XNSP)) {
        scan = p;
        return REG_XNSP;
    }
    if (token.iequals_P(TEXT_REG_INTNEST)) {
        scan = p;
        return REG_INTNEST;
    }
    // DMA[SDCM][0-3]: token must be exactly 5 chars "DMAxy"
    if (token.size() != 5)
        return REG_UNDEF;
    const char *t = token.str();
    if ((t[0] | 0x20) != 'd' || (t[1] | 0x20) != 'm' || (t[2] | 0x20) != 'a')
        return REG_UNDEF;
    if (t[4] < '0' || t[4] > '3')
        return REG_UNDEF;
    const uint8_t idx = uint8_t(t[4] - '0');
    uint8_t sub;
    switch (t[3] | 0x20) {
    case 's':
        sub = uint8_t(idx * 4);
        break;  // DMAS: 0x00..0x0C
    case 'd':
        sub = uint8_t(0x10 + idx * 4);
        break;  // DMAD: 0x10..0x1C
    case 'c':
        sub = uint8_t(0x20 + idx * 4);
        break;  // DMAC: 0x20..0x2C
    case 'm':
        sub = uint8_t(0x22 + idx * 4);
        break;  // DMAM: 0x22..0x2E
    default:
        return REG_UNDEF;
    }
    scan = p;
    return RegName(sub + uint8_t(REG_DMAS0));
}

StrBuffer &outCtrlRegName(StrBuffer &out, RegName name) {
    if (name == REG_XNSP)
        return out.text_P(TEXT_REG_XNSP);
    if (name == REG_NSP)
        return out.text_P(TEXT_REG_NSP);
    if (name == REG_INTNEST)
        return out.text_P(TEXT_REG_INTNEST);
    const uint8_t sub = ctrlRegCode(name);
    const uint8_t idx = (sub >> 2) & 3;
    char type;
    if (sub < 0x10)
        type = 'S';
    else if (sub < 0x20)
        type = 'D';
    else
        type = (sub & 2) ? 'M' : 'C';
    return out.letter('D').letter('M').letter('A').letter(type).letter(char('0' + idx));
}

CcName parseCcName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = CC_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return CcName(entry->name());
    }
    return CC_UNDEF;
}

StrBuffer &outCcName(StrBuffer &out, CcName name) {
    // Output canonical (non-alias) name
    const auto canonical = (name >= CC_ALIAS) ? CcName(int8_t(name) - CC_ALIAS) : name;
    const auto *entry = CC_TABLE.searchName(canonical);
    return entry ? entry->outText(out) : out;
}

uint8_t encodeCcName(CcName name) {
    if (name >= CC_ALIAS)
        return uint8_t(name) - CC_ALIAS;
    return uint8_t(name);
}

CcName decodeCcName(uint8_t num) {
    return CcName(num & 0xF);
}

// Returns the FIXED or REL table for the given access size, or nullptr when
// no such table exists (only 32-bit FIXED is empty -- decode falls back to
// outRegName for plain XIX/XIY/XIZ/XSP and parse rejects 32-bit fixed).
static const NameTable *abregTable(uint8_t size, bool fixed) {
    switch (size) {
    case 0:
        return fixed ? &ABREG8_FIXED_TABLE : &ABREG8_REL_TABLE;
    case 1:
        return fixed ? &ABREG16_FIXED_TABLE : &ABREG16_REL_TABLE;
    case 2:
        return fixed ? nullptr : &ABREG32_REL_TABLE;
    default:
        return nullptr;
    }
}

bool parseAbsRegCode(StrScanner &scan, const ValueParser &parser, uint8_t &code, uint8_t &size) {
    auto p = scan;
    const auto token = parser.readRegName(p);
    if (token.size() == 0)
        return false;

    // Strip optional bank-digit suffix (bank-N register) or prev-bank '\''.
    const char lastChar = token.str()[token.size() - 1];
    const bool hasBankDigit = isdigit(lastChar) != 0;
    const StrScanner base{token.str(),
            hasBankDigit ? token.str() + token.size() - 1 : token.str() + token.size()};
    const uint8_t bankNum = hasBankDigit ? uint8_t(lastChar - '0') : 0;
    bool isPrev = false;
    if (!hasBankDigit && *p == '\'') {
        isPrev = true;
        ++p;
    }
    const bool hasSuffix = hasBankDigit || isPrev;
    // Q-prefixed names (QA, QW, ..., QWA, QBC, ...) name Q-variants that
    // only exist as absolute-bank registers. Without a suffix they default
    // to the current-bank form (0xE0|offset). Plain names that also exist
    // in REG_TABLE (A, WA, XWA, ...) require a suffix to become ABREG.
    const bool isQ = (base.str()[0] == 'Q' || base.str()[0] == 'q');

    for (uint8_t sz = 0; sz < 3; sz++) {
        // FIXED entries (byte-halves and Q-variants of IX/IY/IZ/SP): the
        // name byte is the encoded code 0xF0-0xFF and no suffix is allowed.
        if (!hasSuffix) {
            const auto *table = abregTable(sz, /*fixed=*/true);
            if (table) {
                if (const auto *entry = table->searchText(base)) {
                    code = static_cast<uint8_t>(entry->name());
                    size = sz;
                    scan = p;
                    return true;
                }
            }
        }
        // REL entries: the name byte is the bank-relative offset 0x0-0xF.
        if (const auto *entry = abregTable(sz, /*fixed=*/false)->searchText(base)) {
            const uint8_t off = static_cast<uint8_t>(entry->name());
            if (hasBankDigit) {
                code = uint8_t((bankNum << 4) | off);
            } else if (isPrev) {
                code = uint8_t(0xD0 | off);
            } else if (isQ) {
                code = uint8_t(0xE0 | off);
            } else {
                return false;  // plain register without suffix -> not ABREG
            }
            size = sz;
            scan = p;
            return true;
        }
    }
    return false;
}

// Output an absolute bank register name from the register code byte.
// Register code layout (same for all access sizes; size selects which table):
//   0x00-0x3F  bank-N registers: N = code >> 4, offset = code & 0x0F
//   0xD0-0xDF  previous bank (offset = code & 0x0F), outputs name + '\''
//   0xE0-0xEF  current bank main registers (offset = code & 0x0F)
//   0xF0-0xFF  fixed: IX/IY/IZ/SP (byte halves for 8-bit, full for 16/32-bit)
bool outAbsReg(StrBuffer &out, uint8_t code, uint8_t size) {
    if (size > 2)
        return false;
    if (code >= 0xF0) {
        const auto *table = abregTable(size, /*fixed=*/true);
        if (table) {
            if (const auto *entry = table->searchName(static_cast<int8_t>(code))) {
                entry->outText(out);
                return true;
            }
        }
        // Plain IX/IY/IZ/SP (size 1) or XIX/XIY/XIZ/XSP (size 2). Their codes
        // are 0xF0/0xF4/0xF8/0xFC; any other low nibble in this range is
        // invalid for the given size.
        const uint8_t off = code - 0xF0;
        if (off % 4 != 0 || size == 0)
            return false;
        const auto base = (size == 1) ? REG_IX : REG_XIX;
        outRegName(out, RegName(int8_t(base) + (off / 4)));
        return true;
    }
    // Only 4 banks (0-3) are valid; codes 0x40-0xCF would address bank 4-12
    // which doesn't exist on TLCS-900.
    if (code >= 0x40 && code < 0xD0)
        return false;
    const auto *entry =
            abregTable(size, /*fixed=*/false)->searchName(static_cast<int8_t>(code & 0x0F));
    if (!entry)
        return false;
    entry->outText(out);
    if (code >= 0xD0 && code < 0xE0)
        out.letter('\'');
    else if (code < 0xD0)
        out.letter('0' + (code >> 4));
    return true;
}

}  // namespace reg
}  // namespace tlcs900
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
