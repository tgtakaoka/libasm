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
#include "cli_memory.h"

#include <vector>

namespace libasm {
namespace cli {

class AsmDriver {
public:
    AsmDriver();
    virtual ~AsmDriver();

    int usage() const;
    int parseOption(int argc, const char **argv, AsmDirective &directive);
    int parseOption(
        int argc, const char **argv, std::vector<AsmDirective *> &directives);
    int assemble();

private:
    AsmCommonDirective _commonDir;
    std::vector<AsmDirective *> *_directives;
    CliListing _listing;
    const char *_progname;
    const char *_input_name;
    const char *_output_name;
    const char *_list_name;
    size_t _record_bytes;
    BinFormatter *_formatter;
    bool _uppercase;
    bool _line_number;

    int assemble(CliMemory &memory, FILE *list);
    void printListing(CliMemory &memory, FILE *out);
    int parseOption(int argc, const char **argv);

    static const char *basename(const char *str, char sep_char = '/');
};

} // namespace cli
} // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
