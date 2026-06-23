/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "asm_h8500.h"
#include "reg_h8500.h"
#include "table_h8500.h"
#include "text_common.h"
#include "text_h8500.h"

using namespace libasm::text::common;
using namespace libasm::text::h8500;

namespace libasm {
namespace h8500 {

using namespace pseudo;
using namespace reg;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_FCB, &Assembler::defineDataConstant, Assembler::DATA_BYTE_NO_STRING },
    { TEXT_FDB, &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING },
    { TEXT_RMB, &Assembler::allocateSpaces,     Assembler::DATA_BYTE           },
};
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};
// clang-format on

// H8/500 instruction mnemonics include ':' (MOV:G, ADD:Q, ...), '/' (SCB/EQ,
// TRAP/VS) and a '.b'/'.w' size suffix in their names.
struct H8500SymbolParser final : SymbolParser {
    bool instructionLetter(char c) const override {
        return SymbolParser::instructionLetter(c) || c == ':' || c == '/' || c == '.';
    }
};

}  // namespace

const ValueParser::Plugins &AsmH8500::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return HitachiNumberParser::singleton(); }
        const CommentParser &comment() const override {
            return SemicolonCommentParser::singleton();
        }
        const SymbolParser &symbol() const override {
            static const H8500SymbolParser SYMBOL;
            return SYMBOL;
        }
        const LetterParser &letter() const override { return HitachiLetterParser::singleton(); }
        const OperatorParser &operators() const override {
            return HitachiOperatorParser::singleton();
        }
    } PLUGINS;
    return PLUGINS;
}

AsmH8500::AsmH8500(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

// The instruction encoder is added in a following commit.
Error AsmH8500::encodeImpl(StrScanner &, Insn &_insn) const {
    return _insn.setError(UNKNOWN_INSTRUCTION);
}

}  // namespace h8500
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
