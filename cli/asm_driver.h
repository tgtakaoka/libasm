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

#ifndef __ASM_DRIVER_H__
#define __ASM_DRIVER_H__

#include "asm_directive.h"
#include "cli_listing.h"
#include "bin_memory.h"
#include "file_reader.h"

namespace libasm {
namespace cli {

class FileFactory : public AsmSourceFactory {
public:
    Error open(const StrScanner &name) override;
    const TextReader *current() const override;
    void closeCurrent() override;
    size_t size() const override { return _sources.size(); }
    StrScanner *readLine() override;

private:
    static constexpr int max_includes = 4;
    std::list<FileReader> _sources;
};

class AsmDriver {
public:
    AsmDriver(AsmDirective **begin, AsmDirective **end);

    int usage();
    int parseOption(int argc, const char **argv);
    int assemble();

private:
    AsmCommonDirective _commonDir;
    FileFactory _sources;
    CliListing _listing;
    const char *_progname;
    const char *_input_name;
    const char *_output_name;
    const char *_list_name;
    char _encoder;
    size_t _record_bytes;
    bool _uppercase;
    bool _line_number;
    bool _verbose;

    static constexpr const char *PROG_PREFIX = "asm";
    AsmDirective *defaultDirective();
    int assemble(BinMemory &memory, TextPrinter &out, bool reportError = false);
    void printListing(BinMemory &memory, TextPrinter &out);

    static const char *basename(const char *str, char sep_char = '/');
};

}  // namespace cli
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
