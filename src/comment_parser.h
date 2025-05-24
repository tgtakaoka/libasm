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
    /** Return if |scan| points line comment. */
    virtual bool commentLine(StrScanner &scan) const { return endOfLine(scan); }
    /** Return if |scan| points comment. */
    virtual bool endOfLine(StrScanner &scan) const = 0;
};

struct SemicolonCommentParser final : CommentParser, Singleton<SemicolonCommentParser> {
    /** A comment starts with ";". */
    bool endOfLine(StrScanner &scan) const override;
};

struct SharpCommentParser final : CommentParser, Singleton<SharpCommentParser> {
    /** A comment starts with ";" or "#". */
    bool endOfLine(StrScanner &scan) const override;
};

struct StarCommentParser final : CommentParser, Singleton<StarCommentParser> {
    /** A comment line starts with "*" or ";". */
    bool commentLine(StrScanner &scan) const override;
    /** If a comment starts with " " then skip spaces, otherwise starts with ";". */
    bool endOfLine(StrScanner &scan) const override;
};

struct RcaCommentParser final : CommentParser, Singleton<RcaCommentParser> {
    /** A comment line starts with ".." or ";". */
    bool commentLine(StrScanner &scan) const override;
    /** A comment starts with ";". */
    bool endOfLine(StrScanner &scan) const override {
        return SemicolonCommentParser::singleton().endOfLine(scan);
    }
};

struct MostekCommentParser final : CommentParser, Singleton<MostekCommentParser> {
    /** A comment line starts with ";". */
    bool commentLine(StrScanner &scan) const override {
        return SemicolonCommentParser::singleton().commentLine(scan);
    }
    /** If a comment starts with " " then skip spaces, otherwise starts with ";". */
    bool endOfLine(StrScanner &scan) const override {
        return StarCommentParser::singleton().endOfLine(scan);
    }
};

struct Pdp8CommentParser final : CommentParser, Singleton<Pdp8CommentParser> {
    /** A comment starts with "/" or ";". */
    bool endOfLine(StrScanner &scan) const override;
};

struct Tms320CommentParser final : CommentParser, Singleton<Tms320CommentParser> {
    bool commentLine(StrScanner &scan) const override { return *scan == '*'; }
    bool endOfLine(StrScanner &scan) const override {
        return SemicolonCommentParser::singleton().endOfLine(scan);
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
