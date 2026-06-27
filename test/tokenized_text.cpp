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

#include "tokenized_text.h"

#include <cctype>
#include <cstring>

namespace libasm {
namespace gen {

inline bool isXdigits(const char *&r, const char *p) {
    if (!isxdigit(*p++))
        return false;
    while (isxdigit(*p))
        p++;
    r = p;
    return true;
}

inline bool isDigits(const char *&r, const char *p) {
    if (!isdigit(*p++))
        return false;
    while (isdigit(*p))
        p++;
    r = p;
    return true;
}

bool isFloat(const char *p, const char *&r) {
    const char *s = p;
    if (*s == '-')
        ++s;
    if (isDigits(s, s) && *s == '.' && isDigits(s, s + 1)) {
        const char *e = s + 1;
        if (toupper(*s) == 'E' && (*e == '-' || *e == '+') && isDigits(e, e + 1)) {
            r = e;
            return true;
        }
        r = s;
        return true;
    }
    if (strncasecmp(s, "nan", 3) == 0 || strncasecmp(s, "inf", 3) == 0) {
        r = s + 3;
        return true;
    }
    r = p;
    return false;
}

bool isNumber(const char *p, const char *&r) {
    const char *s;
    if (p[0] == '0' && toupper(p[1]) == 'X' && isXdigits(s, p + 2)) {
        r = s;
        return true;
    }
    if (isXdigits(s, p) && (toupper(*s) == 'H' || toupper(*s) == 'Q')) {
        r = s + 1;
        return true;
    }
    if (isDigits(s, p)) {
        r = s;
        return true;
    }
    if (*p == '$' && isXdigits(s, p + 1) && s - p >= 3) {
        r = s;
        return true;
    }
    r = p;
    return false;
}

bool isHexDigit(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

// Match Z8001 extern symbol seg_SS_off_XXXX (2 + 4 lowercase hex digits)
// emitted by DisZ8000 when extern-symbol option is enabled.  Treated as
// a single collapsed token so the generator dedups across addresses
// instead of producing one test per unique segmented address.
bool isZ8001ExternSym(const char *p, const char *&r) {
    if (p[0] != 's' || p[1] != 'e' || p[2] != 'g' || p[3] != '_')
        return false;
    if (!isHexDigit(p[4]) || !isHexDigit(p[5]))
        return false;
    if (p[6] != '_' || p[7] != 'o' || p[8] != 'f' || p[9] != 'f' || p[10] != '_')
        return false;
    if (!isHexDigit(p[11]) || !isHexDigit(p[12]) || !isHexDigit(p[13]) || !isHexDigit(p[14]))
        return false;
    r = p + 15;
    return true;
}

bool isRelative(const char *p, const char *&r) {
    const auto o = *p;
    if (o == '$' || o == '*' || o == '.') {
        const auto n = *++p;
        const char *tmp;
        if ((n == '+' || n == '-') && isNumber(p + 1, tmp)) {
            r = tmp;
            return true;
        }
        if (n == 0 || n == ',' || n == ')' || n == ']' || n == '[' || n == '(') {
            r = p;
            return true;
        }
    }
    return false;
}

bool isM68kIndex(const char *p, const char *&r) {
    const auto x = toupper(*p);
    const auto n = *++p;
    if ((x == 'A' || x == 'D') && (n >= '0' && n < '8') && *++p == '.') {
        // "[AD][0-7]."
        const auto sz = toupper(*++p);
        if (sz == 'W' || sz == 'L') {
            if (*++p != '*') {
                r = p;
                return true;
            }
            const auto sc = p[1];
            if (sc == '2' || sc == '4' || sc == '8') {
                r = p + 2;
                return true;
            }
        }
    }
    return false;
}

bool isM68kList(const char *p, const char *&r) {
    const auto x = toupper(*p++);
    const auto rn = *p++;
    if ((x == 'A' || x == 'D') && (rn >= '0' && rn < '8') && (*p == '/' || *p == '-')) {
        // "[AD][0-7][-/]"
        r = p + 1;
        return true;
    }
    const auto fpn = *p++;
    if (x == 'F' && toupper(rn) == 'P' && (fpn >= '0' && fpn < '8') && (*p == '/' || *p == '-')) {
        // "FP[0-7][-/]"
        r = p + 1;
        return true;
    }
    return false;
}

// H16 scaled-index / scaled-base operand.  Two forms collapse here:
//   index   "Rn.W"/"Rn.L" with optional "*1/2/4/8" (Xm is global R0-R15), and
//   base    "Rn"/"CRn"/"PRn" with a required "*1/2/4/8" scale (@(Rn*Sf)).
// A bare register (no size and no scale) is left alone -- it is not a scaled
// form and must not be collapsed.
bool isH16Index(const char *p, const char *&r) {
    const char *q = p;
    if (toupper(q[0]) == 'C' || toupper(q[0]) == 'P')
        q++;  // optional current/previous-bank prefix of a CRn/PRn base
    if (toupper(*q) != 'R' || !isdigit(q[1]))
        return false;
    q += 2;
    if (isdigit(*q))
        q++;  // Rn 10-15
    bool sized = false;
    if (*q == '.') {
        const auto sz = toupper(q[1]);
        if (sz != 'W' && sz != 'L')
            return false;
        q += 2;
        sized = true;
    }
    bool scaled = false;
    if (*q == '*' && (q[1] == '1' || q[1] == '2' || q[1] == '4' || q[1] == '8')) {
        q += 2;
        scaled = true;
    }
    if (!sized && !scaled)
        return false;
    r = q;
    return true;
}

// H16 register-list range "Rn-Rm" (STM/LDM/PGBN).  ASL spells a list as
// comma-separated elements, each a single Rn or a range Rn-Rm; single
// registers collapse via the generic number rule, so only the range needs a
// dedicated collapse here.
bool isH16List(const char *p, const char *&r) {
    if (toupper(*p) != 'R' || !isdigit(p[1]))
        return false;
    const char *q = p + 2;
    if (isdigit(*q))
        q++;  // R10-R15
    if (*q != '-')
        return false;
    ++q;
    if (toupper(*q) != 'R' || !isdigit(q[1]))
        return false;
    q += 2;
    if (isdigit(*q))
        q++;  // R10-R15
    r = q;
    return true;
}

// H16 register with an optional current/previous-bank prefix: Rn, CRn or PRn.
// Collapse the bank class to one register token (as MC680xx An/Dn collapse to
// Xn) so the src x dst bank cross product does not explode the generated set.
// Scaled bases (Rn*Sf, CRn*Sf) and list ranges (Rn-Rm) are matched earlier.
bool isH16Reg(const char *p, const char *&r) {
    const char *q = p;
    if (toupper(q[0]) == 'C' || toupper(q[0]) == 'P')
        q++;  // optional current/previous-bank prefix
    if (toupper(*q) != 'R' || !isdigit(q[1]))
        return false;
    q += 2;
    if (isdigit(*q))
        q++;  // Rn 10-15
    r = q;
    return true;
}

// H16 register list (STM/LDM/PGBN): a comma-separated sequence of Rn or Rn-Rm
// elements.  Collapse the whole list -- any length, any mix of singles and
// ranges -- to one token so these instructions do not emit a separate test per
// list shape (the 16-bit list mask otherwise explodes the generated set).
// Modeled on isM68kList (MC68000 MOVEM): that keys on the list-only '/'
// separator so it never fires on ordinary operands of other CPUs.  H16 lists
// use ", " (not list-specific), so key instead on a range element "Rn-Rm",
// which appears only inside an H16 register list -- never as a normal operand.
// Fires for a multi-element list containing a range; a single range is left to
// isH16List, and all-singleton lists collapse by count via the register rule.
bool isH16RegList(const char *p, const char *&r) {
    const char *q = p;
    int elements = 0;
    bool hasRange = false;
    for (;;) {
        if (toupper(q[0]) != 'R' || !isdigit(q[1]))
            break;
        q += 2;
        if (isdigit(*q))
            q++;  // Rn 10-15
        if (q[0] == '-' && toupper(q[1]) == 'R' && isdigit(q[2])) {
            q += 3;
            if (isdigit(*q))
                q++;  // -Rm 10-15
            hasRange = true;
        }
        elements++;
        if (q[0] == ',' && q[1] == ' ' && toupper(q[2]) == 'R' && isdigit(q[3])) {
            q += 2;  // skip ", " before the next element
            continue;
        }
        break;
    }
    if (elements < 2 || !hasRange)
        return false;  // not a range-bearing multi-element list
    r = q;
    return true;
}

bool isNs32kScale(const char *p) {
    if (p[1] == ']') {
        const auto c = toupper(*p);
        return c == 'B' || c == 'W' || c == 'D' || c == 'Q';
    }
    return false;
}

bool isX86Reg8(const char *p) {
    const auto c1 = toupper(p[0]);
    const auto c2 = toupper(p[1]);
    return (c1 >= 'A' && c1 <= 'D') && (c2 == 'L' || c2 == 'H');
}

bool isX86Reg16(const char *p) {
    const auto c1 = toupper(p[0]);
    const auto c2 = toupper(p[1]);
    if (c2 == 'X')
        return c1 >= 'B' && c1 <= 'D';  // BX/CX/DX
    if (c2 == 'P')
        return c1 == 'B' || c1 == 'S';  // BP/SP
    if (c2 == 'I')
        return c1 == 'D' || c1 == 'S';  // DI/SI
    return false;
}

bool isX86Seg(const char *p) {
    const auto s = toupper(p[1]);
    if (s == 'S' && p[2] == ':') {
        const auto c = toupper(*p);
        return (c >= 'C' && c <= 'G') || c == 'S';  // CS/DS/ES/FS/GS/SS
    }
    return false;
}

bool isX86Reg32(const char *p) {
    const auto c1 = toupper(p[0]);
    return c1 == 'E' && isX86Reg16(p + 1);
}

bool TokenizedText::reduceSizeSuffix = false;

TokenizedText::TokenizedText(const char *text) : _tokens(tokenize(text)), _count(0) {}

std::string TokenizedText::tokenize(const char *text) {
    std::string t;
    const char *b = text;
    while (*b) {
        const char *tmp;
        if (isFloat(b, tmp)) {
            // reduce float constant variants; n.nE-e, n.nE+e
            t.push_back('n');
            t.push_back('.');
            t.push_back('f');
            t.push_back('E');
            t.push_back('+');
            t.push_back('e');
            b = tmp;
        } else if (isNumber(b, tmp) || (*b == '-' && isNumber(b + 1, tmp))) {
            t.push_back('n');
            b = tmp;
        } else if (isZ8001ExternSym(b, tmp)) {
            // collapse seg_SS_off_XXXX to 's'
            t.push_back('s');
            b = tmp;
        } else if (isRelative(b, tmp)) {
            // reduce relative variant
            t.push_back(b[0]);
            t.push_back('+');
            t.push_back('r');
            b = tmp;
        } else if (isM68kIndex(b, tmp)) {
            // reduce index variants of MC680xx; [AD][0-7].[WL](*[248])?
            t.push_back('X');
            t.push_back('n');
            b = tmp;
        } else if (isM68kList(b, tmp)) {
            // reduce MOVEM variants of MC68000; Rn/Rn and Rn-Rn
            t.push_back('R');
            t.push_back('n');
            t.push_back('/');
            b = tmp;
        } else if (isH16RegList(b, tmp)) {
            // collapse H16 multi-element register-list shapes (STM/LDM/PGBN)
            t.push_back('R');
            t.push_back('L');
            b = tmp;
        } else if (isH16Index(b, tmp)) {
            // reduce scaled-index variants of H16; Rn.[WL] optionally *1/2/4/8
            t.push_back('X');
            t.push_back('n');
            b = tmp;
        } else if (isH16List(b, tmp)) {
            // reduce register-list range variants of H16; Rn-Rm
            t.push_back('R');
            t.push_back('n');
            t.push_back('-');
            t.push_back('R');
            t.push_back('n');
            b = tmp;
        } else if (isH16Reg(b, tmp)) {
            // reduce bank-class variants of H16; Rn/CRn/PRn collapse to one
            t.push_back('R');
            t.push_back('n');
            b = tmp;
        } else if ((b[0] == '(' || b[0] == '[') && b[1] == '-' && isNumber(b + 2, tmp)) {
            // reduce displacement variants of MC68000; (-n...) and (n...), [-n...] and [n...]
            t.push_back(b[0]);
            t.push_back('n');
            b = tmp;
        } else if (b[0] == '+' && b[1] == '(' && b[2] == '-' && isNumber(b + 3, tmp) &&
                   *tmp == ')') {
            // reduce displacement variants of NS32000; +(-n) and +n
            t.push_back('+');
            t.push_back('n');
            b = tmp + 1;
        } else if (b[0] == ':' && isNs32kScale(b + 1)) {
            // reduce index size variants of NS32000; [Rn:B], [Rn:W], [Rn:D] and [Rn:Q]
            t.push_back(':');
            t.push_back('s');
            t.push_back(b[2]);
            b += 3;
        } else if (isX86Reg16(b) && (b[2] == '+' || b[2] == '-') && isNumber(b + 3, tmp) && *tmp == ']') {
            // reduce indexing variants of 80X86; AX/BX/CX/DX+n, BP/SP+n, DI/SI+n
            t.push_back(b[1]);
            t.push_back('+');
            t.push_back('n');
            b = tmp + 1;
        } else if (isX86Reg32(b) && (b[3] == '+' || b[3] == '-') && isNumber(b + 4, tmp) && *tmp == ']') {
            // reduce indexing variants of 80X86; EAX/EBX/ECX/EDX+n, EBP/ESP+n, EDI/ESI+n
            t.push_back(b[2]);
            t.push_back('+');
            t.push_back('n');
            b = tmp + 1;
        } else if (b[0] == '*' && (b[1] == '2' || b[1] == '4' || b[1] == '8')) {
            // reduce index scaling variants of 80X86
            t.push_back('*');
            t.push_back('s');
            b += 2;
        } else if (isX86Reg8(b)) {
            // reduce 8 bit register variation of 80X86; AL/BL/CL/DL, AH/BH/CH/DH
            t.push_back(b[1]);
            b += 2;
        } else if (isX86Reg16(b)) {
            // reduce 16 bit register variation of 80X86; BX/CX/DX, BP/SP, DI/SI
            t.push_back(b[1]);
            b += 2;
        } else if (isX86Reg32(b)) {
            // reduce 32 bit register variation of 80X86; EBX/ECX/EDX, EBP/ESP, EDI/ESI
            t.push_back(b[2]);
            b += 3;
        } else if (isX86Seg(b)) {
            // reduce segment override of 80x86; ES:, CS:, SS:, DS:
            t.push_back('s');
            t.push_back('o');
            t.push_back(':');
            b += 3;
        } else {
            t.push_back(*b++);
        }
    }
    if (reduceSizeSuffix) {
        // Drop ":n" size-suffix tokens so a forced-width value (H16's :8/:16/:32
        // displacement/immediate/absolute) dedups with its natural-width form.
        // Done on the finished token string to catch every value branch (plain,
        // MC68000-style "(-n", double-indirect, ...).  Only gen_h16 enables this
        // (its operands' sole ":number" is the size suffix); NS32000's ":letter"
        // scale tokenizes to ":s" and is unaffected.
        for (std::string::size_type p; (p = t.find(":n")) != std::string::npos;)
            t.erase(p, 2);
    }
    return t;
}

std::size_t TokenizedText::hash() const {
    std::size_t seed = _tokens.size();
    for (auto t : _tokens) {
        seed ^= t + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

}  // namespace gen
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
