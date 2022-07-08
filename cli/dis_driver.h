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

#ifndef __DIS_COMMANDER_H__
#define __DIS_COMMANDER_H__

#include "bin_decoder.h"
#include "bin_memory.h"
#include "dis_args.h"
#include "dis_base.h"
#include "dis_directive.h"
#include "file_reader.h"

#include <list>
#include <string>

namespace libasm {
namespace cli {

class DisDriver {
public:
    DisDriver(Disassembler **begin, Disassembler **end, DisArgs &args);

    int parseArgs(int argc, const char **argv);
    int usage(const std::list<std::string> &cpuList);
    std::list<std::string> listCpu();
    int disassemble();

private:
    std::list<Disassembler *> _disassemblers;
    DisArgs &_args;
    Disassembler *_disassembler;

    static constexpr const char *PROG_PREFIX = "dis";
    Disassembler *defaultDisassembler();
    int readBinary(FileReader &input, BinMemory &memory);
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
