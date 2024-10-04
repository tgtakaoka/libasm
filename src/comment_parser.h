/*
 * Copyright 2024 Tadashi G. Takaoka
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

#ifndef __LIBASM_COMMENT_PARSER_H__
#define __LIBASM_COMMENT_PARSER_H__

#include "config_host.h"
#include "str_scanner.h"

namespace libasm {

/**
 * Comment parser
 */
struct CommentParser {
    virtual bool commentLine(StrScanner &scan) const { return endOfLine(scan); }
    virtual bool endOfLine(StrScanner &scan) const = 0;
};

struct SemicolonCommentParser final : CommentParser, Singleton<SemicolonCommentParser> {
    bool endOfLine(StrScanner &scan) const override {
        return *scan.skipSpaces() == 0 || *scan == ';';
    }
};

struct SharpCommentParser final : CommentParser, Singleton<SharpCommentParser> {
    bool endOfLine(StrScanner &scan) const override {
        return SemicolonCommentParser::singleton().endOfLine(scan) || *scan == '#';
    }
};

struct StarCommentParser final : CommentParser, Singleton<StarCommentParser> {
    bool commentLine(StrScanner &scan) const override { return *scan == '*' || *scan == ';'; }
    bool endOfLine(StrScanner &scan) const override {
        const auto end = (*scan == 0 || *scan == ' ');
        scan.skipSpaces();
        return end || SemicolonCommentParser::singleton().endOfLine(scan);
    }
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
