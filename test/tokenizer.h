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

#ifndef __LIBASM_TOKENIZER_H__
#define __LIBASM_TOKENIZER_H__

#include <cctype>
#include <initializer_list>
#include <string>
#include <vector>

namespace libasm {
namespace gen {

// A pluggable filter for TokenizedText.  Each Tokenizer recognizes a class of
// variable operand text (a number, a register/index/scale variant, a relative
// target, ...) and collapses it to a canonical template token so the test
// generator dedups near-duplicate instructions.  An arch composes the ordered
// list it needs in gen_<arch>.cpp (see standardTokenizers) and injects it via
// TestGenerator.
struct Tokenizer {
    virtual ~Tokenizer() = default;
    // Try to match a token at |p|.  On match, append the replacement token to
    // |out| and return the pointer just past the consumed input.  On no match,
    // return nullptr and leave |out| untouched.
    virtual const char *tokenize(const char *p, std::string &out) const = 0;
};

// An ordered list of tokenizers.  TokenizedText tries each in order at every
// input position; the first match wins.
using TokenizerList = std::vector<const Tokenizer *>;

// Shared low-level matchers.  Each returns true on match and sets |r| just past
// the match.
bool isXdigits(const char *&r, const char *p);
bool isDigits(const char *&r, const char *p);
bool isFloat(const char *p, const char *&r);

// Number policies: each recognizes ONE assembler family's integer literal.  They
// are PURE (no bare-decimal fallback) -- an arch's hex form is always matched
// before decimal (a decimal-first order would split "0x1F"/"1234H" at the
// leading digit).  DecimalNumber is the shared fallback.  A number-consuming
// tokenizer templated on a hex policy tries it then DecimalNumber (matchNum).
struct IntelNumber {  // 1234H, 0FFH, 377Q
    static bool match(const char *p, const char *&r);
};
struct MotorolaNumber {  // $1234
    static bool match(const char *p, const char *&r);
};
struct NationalNumber {  // X'1234'/H'..' (IBM/National), 0x1234
    static bool match(const char *p, const char *&r);
};
struct CstyleNumber {  // 0x1F, 0b1010
    static bool match(const char *p, const char *&r);
};
struct DecimalNumber {  // 1234 (bare decimal / octal digit run)
    static bool match(const char *p, const char *&r) { return isDigits(r, p); }
};

// A displacement/offset value is "the arch's hex OR bare decimal"; try hex first.
template <class Hex>
inline bool matchNum(const char *p, const char *&r) {
    return Hex::match(p, r) || DecimalNumber::match(p, r);
}

// Reduce a float-constant variant (n.nE-e, n.nE+e, NaN, Inf) to one token.
struct FloatTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override;
};

// Reduce one number family's integer literal to a single 'n' token.  A leading
// sign is NOT consumed here -- it is left literal so a standalone "-5" and "5"
// stay distinct; index displacements drop their sign via IndexDisp/RegDisp.
// Register digits are handled by RegisterTokenizer, which is placed before the
// number tokenizers and so consumes "R0" whole before the number matcher reaches
// its digit; an arch without a RegisterTokenizer for a family (e.g. pdp11, whose
// generator is too exploration-fragile for one) falls back to collapsing the
// digit here, as before.
template <class Number>
struct NumberTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        const char *r;
        if (Number::match(p, r)) {
            out.push_back('n');
            return r;
        }
        return nullptr;
    }
};

// Collapse a register family "<prefix><decimal>" (R0..R15, AR0..AR7, D0..D7) to a
// single class token.  |prefix| is matched case-insensitively at a word boundary;
// the register number must be <= |maxReg| and end at a non-alphanumeric boundary
// (so "R1" matches but "R1L"/"MR0" do not).  Each gen_<arch>.cpp instantiates one
// per family it wants collapsed -- register collapse is now explicit and
// per-arch, not a side effect of the number matcher.  Order longer prefixes first
// (RR/RQ before R).
// |suffix| (optional) lets a byte/half register keep a trailing letter: with
// suffix "HL", "R0H"->"RnH", "R0L"->"RnL", and plain "R0"->"Rn".
struct RegisterTokenizer : Tokenizer {
    RegisterTokenizer(const char *prefix, int maxReg, const char *classToken,
            const char *suffix = nullptr);
    const char *tokenize(const char *p, std::string &out) const override;

private:
    const char *const _prefix;
    const std::size_t _plen;
    const int _maxReg;
    const char *const _class;
    const char *const _suffix;
};

// Reduce PC-relative target variants of one location-counter symbol to
// "<sym>+r": "<sym>", "<sym>+n" and "<sym>-n".  |Hex| is the arch's hex number
// policy; the offset also accepts bare decimal.
template <class Hex>
struct RelativeTokenizer : Tokenizer {
    explicit RelativeTokenizer(char locationSymbol) : _loc(locationSymbol) {}
    const char *tokenize(const char *p, std::string &out) const override {
        if (*p != _loc)
            return nullptr;
        const auto n = *(p + 1);
        const char *r;
        if ((n == '+' || n == '-') && matchNum<Hex>(p + 2, r)) {
            out.push_back(_loc);
            out.push_back('+');
            out.push_back('r');
            return r;
        }
        // Standalone location symbol (no +/-n offset): the whole target only when
        // the next char is neither alphanumeric nor a '+'/'-' sign.
        if (!isalnum(static_cast<unsigned char>(n)) && n != '+' && n != '-') {
            out.push_back(_loc);
            out.push_back('+');
            out.push_back('r');
            return p + 1;
        }
        return nullptr;
    }

private:
    const char _loc;
};

// Index-context predicates (2nd template arg of IndexDispTokenizer): given the
// char |t| just past the displacement number and the output |o| so far, is this
// an index/base displacement for this arch's syntax?
struct ParenIndex {  // n(Rn), @n(Rn), nested n(n(Rn))
    static bool ok(const char *t, const std::string &o) {
        (void)o;
        return *t == '(';
    }
};
struct CommaIndex {  // n,X ; (n,An) ; @(n,Rn)
    static bool ok(const char *t, const std::string &o) {
        (void)o;
        // ", " (comma+space) is the top-level operand separator; a comma with no
        // space is an index/base displacement inside an addressing mode.
        return *t == ',' && t[1] != ' ';
    }
};
struct OuterDisp {  // mc68020 memory-indirect outer displacement: ([...],od) / (...,od)
    static bool ok(const char *t, const std::string &o) {
        // the outer displacement closes the mode with ')' and follows a ','.
        return *t == ')' && !o.empty() && o.back() == ',';
    }
};
struct ColonDisp {  // h16 size-suffixed base displacement: @(disp:8/:16/:32, ...)
    static bool ok(const char *t, const std::string &o) {
        // a ":N"-sized displacement right after "@(" (an immediate after '#'
        // keeps its sign, so require the open-paren context).
        return *t == ':' && !o.empty() && o.back() == '(';
    }
};
struct LeadImm {  // leading bare immediate/quick operand: "<mnem> -8, <EA>"
    static bool ok(const char *t, const std::string &o) {
        // the FIRST operand (o empty) is a bare value ending at the operand
        // separator ", " -- collapse its sign so "0"/"-8" quicks conflate.  An
        // EA displacement ends at '(' and the EXT disp2 has a non-empty prefix,
        // so neither is matched here.
        return *t == ',' && t[1] == ' ' && o.empty();
    }
};

// Register-context predicates (2nd template arg of RegDispTokenizer): the disp
// follows a register and closes the index bracket.
struct ParenReg {  // (reg+n)
    static bool ok(const std::string &o, const char *t) {
        return !o.empty() && isalnum(static_cast<unsigned char>(o.back())) && *t == ')';
    }
};
struct BracketReg {  // [reg+n]
    static bool ok(const std::string &o, const char *t) {
        return !o.empty() && isalnum(static_cast<unsigned char>(o.back())) && *t == ']';
    }
};

// Family A -- displacement before the register.  Consolidate a signed index/base
// displacement to one 'n' (drop the sign, like RelativeTokenizer does for branch
// targets).  |Hex| = arch hex policy (decimal also accepted); |Index| = the
// index-context predicate.  Place before the number tokenizers so it wins on the
// whole displacement.
template <class Hex, class Index>
struct IndexDispTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        const char *q = p;
        if (*q == '+' || *q == '-')
            ++q;  // optional displacement sign
        const char *r;
        if (!matchNum<Hex>(q, r))
            return nullptr;
        if (!Index::ok(r, out))
            return nullptr;
        out.push_back('n');
        return r;
    }
};

// Family B -- register before the displacement.  Consolidate "<reg>+n"/"<reg>-n"
// to canonical "+n".
template <class Hex, class Reg>
struct RegDispTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        if (*p != '+' && *p != '-')
            return nullptr;
        const char *r;
        if (!matchNum<Hex>(p + 1, r))
            return nullptr;
        if (!Reg::ok(out, r))
            return nullptr;
        out.append("+n");
        return r;
    }
};

// Shared float instance (stateless, non-templated).
extern const FloatTokenizer FLOAT_TOKENIZER;

// The list an arch composes: { float, index/reg tokenizers, hex numbers, decimal
// numbers, relative(loc) }.  |Hex| is the arch's hex number policy; |indexToks|
// are the arch's IndexDisp/RegDisp instances, inserted BEFORE the number
// tokenizers so they win on the whole displacement.  |loc| is the
// location-counter char the disassembler emits (Disassembler::curSym()).
template <class Hex>
inline TokenizerList standardTokenizers(
        char loc, std::initializer_list<const Tokenizer *> indexToks = {}) {
    static const NumberTokenizer<Hex> hexNumber;
    static const NumberTokenizer<DecimalNumber> decNumber;
    static const RelativeTokenizer<Hex> relDollar('$');
    static const RelativeTokenizer<Hex> relStar('*');
    static const RelativeTokenizer<Hex> relDot('.');
    static const RelativeTokenizer<Hex> relBang('!');
    const Tokenizer *rel;
    switch (loc) {
    case '*':
        rel = &relStar;
        break;
    case '.':
        rel = &relDot;
        break;
    case '!':
        rel = &relBang;
        break;
    default:  // '$' and any other inherit the base default symbol
        rel = &relDollar;
        break;
    }
    TokenizerList list;
    list.push_back(&FLOAT_TOKENIZER);
    for (const auto *t : indexToks)
        list.push_back(t);
    list.push_back(&hexNumber);
    list.push_back(&decNumber);
    list.push_back(rel);
    return list;
}

}  // namespace gen
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
