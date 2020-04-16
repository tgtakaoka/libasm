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

#ifndef __DIS_DRIVER_H__
#define __DIS_DRIVER_H__

#include "cli_memory.h"
#include "dis_base.h"
#include "dis_directive.h"
#include "file_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace libasm {
namespace cli {

class DisDriver {
public:
    DisDriver(std::vector<Disassembler *> &disassemblers);
    virtual ~DisDriver();

    int usage();
    int parseOption(int argc, const char **argv);
    int disassemble();

private:
    bool _uppercase;
    std::vector<Disassembler *> _disassemblers;
    Disassembler *_disassembler;
    const char *_progname;
    const char *_input_name;
    const char *_output_name;
    const char *_list_name;
    BinFormatter *_formatter;

    static constexpr const char *PROG_PREFIX = "dis";
    Disassembler *defaultDisassembler() const;
    int readInput(FILE *input, const char *filename, CliMemory &memory);
    BinFormatter *determineInputFormat(const char *input_name);
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
