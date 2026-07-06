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

namespace libasm {
namespace gen {

TokenizedText::TokenizedText(const char *text, const TokenizerList &tokenizers)
    : _tokens(tokenize(text, tokenizers)), _count(0) {}

std::string TokenizedText::tokenize(const char *text, const TokenizerList &tokenizers) {
    std::string t;
    const char *b = text;
    while (*b) {
        const char *next = nullptr;
        for (const auto *tokenizer : tokenizers) {
            next = tokenizer->tokenize(b, t);
            if (next) {
                b = next;
                break;
            }
        }
        if (!next)
            t.push_back(*b++);
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
