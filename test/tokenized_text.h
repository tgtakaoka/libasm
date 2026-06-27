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

#ifndef __LIBASM_TOKENIZED_TEXT_H__
#define __LIBASM_TOKENIZED_TEXT_H__

#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_set>

namespace libasm {
namespace gen {

struct TokenizedText {
    TokenizedText(const char *text);

    std::size_t hash() const;
    uint32_t increment() const { return ++_count; }
    uint32_t count() const { return _count; }
    const std::string &tokens() const { return _tokens; }

    struct hashOp {
        std::size_t operator()(const TokenizedText &t) const { return t.hash(); }
    };
    struct eqOp {
        bool operator()(const TokenizedText &a, const TokenizedText &b) const {
            return a._tokens == b._tokens;
        }
    };

    typedef std::unordered_set<TokenizedText, TokenizedText::hashOp, TokenizedText::eqOp> Set;

    // When set, a ":N" size suffix on a number is collapsed into the bare value
    // (H16's :8/:16/:32 forced displacement/immediate/absolute widths), so a
    // forced-width operand dedups with its natural-width form.  Off by default;
    // a generator opts in (only gen_h16 does -- other CPUs share the ":N"
    // syntax for distinct addressing modes that must NOT collapse).
    static bool reduceSizeSuffix;

private:
    const std::string _tokens;
    mutable uint32_t _count;

    static std::string tokenize(const char *text);
};

}  // namespace gen
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
