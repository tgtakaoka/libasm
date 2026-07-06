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

#include <string>
#include <vector>

namespace libasm {
namespace gen {

// A pluggable filter for TokenizedText.  Each Tokenizer recognizes a class of
// variable operand text (a number, a register/index/scale variant, a relative
// target, ...) and collapses it to a canonical template token so the test
// generator dedups near-duplicate instructions.  Standard tokenizers live
// here; an arch supplies its own in gen_<arch>.cpp and injects the ordered
// list it needs via TestGenerator.
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

// Shared low-level matchers, exposed so arch tokenizers in gen_<arch>.cpp can
// reuse them.  Each returns true on match and sets |r| just past the match.
bool isXdigits(const char *&r, const char *p);
bool isDigits(const char *&r, const char *p);
bool isFloat(const char *p, const char *&r);
// Any integer literal of any style (Intel ..H/Q, Motorola $.., National 0x../
// X'..', or decimal).  Used where an arch tokenizer just needs "a number"
// after a prefix (relative offset, size suffix, [reg+n], ...).
bool isNumber(const char *p, const char *&r);

// Reduce a float-constant variant (n.nE-e, n.nE+e, NaN, Inf) to one token.
struct FloatTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override;
};

// Number-style families: each reduces one assembler family's integer literal
// syntax to a single 'n' token (a leading '-' is folded in).  An arch's
// disassembler output uses one family (selected by gen options like
// intel-style / c-style); the standard list includes them all so any family
// the gen happens to emit is collapsed.
struct IntelNumberTokenizer : Tokenizer {  // 1234H, 0FFH, 377Q
    const char *tokenize(const char *p, std::string &out) const override;
};
struct MotorolaNumberTokenizer : Tokenizer {  // $1234
    const char *tokenize(const char *p, std::string &out) const override;
};
struct NationalNumberTokenizer : Tokenizer {  // 0x1234 (C-style), X'1234'/H'..' (IBM/National)
    const char *tokenize(const char *p, std::string &out) const override;
};
struct DecimalNumberTokenizer : Tokenizer {  // 1234 (shared by every family)
    const char *tokenize(const char *p, std::string &out) const override;
};

// Reduce PC-relative target variants of one location-counter symbol to
// "<sym>+r": "<sym>", "<sym>+n" and "<sym>-n".  The symbol is per-arch (the
// origin char the disassembler emits, e.g. '$', '*', '.'); pass it at
// construction so only that arch's symbol is collapsed.
struct RelativeTokenizer : Tokenizer {
    explicit RelativeTokenizer(char locationSymbol) : _loc(locationSymbol) {}
    const char *tokenize(const char *p, std::string &out) const override;

private:
    const char _loc;
};

// Shared stateless standard tokenizer instances.  Pick the ones an arch needs.
extern const FloatTokenizer FLOAT_TOKENIZER;
extern const IntelNumberTokenizer INTEL_NUMBER_TOKENIZER;
extern const MotorolaNumberTokenizer MOTOROLA_NUMBER_TOKENIZER;
extern const NationalNumberTokenizer NATIONAL_NUMBER_TOKENIZER;
extern const DecimalNumberTokenizer DECIMAL_NUMBER_TOKENIZER;

// The standard list every arch uses: {float, the number families, decimal,
// relative(originSymbol)}.  |originSymbol| is the location-counter char the
// disassembler emits (see Disassembler::curSym()).
TokenizerList standardTokenizers(char originSymbol);

}  // namespace gen
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
