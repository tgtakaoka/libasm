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

#ifndef __ASM_COMMANDER_H__
#define __ASM_COMMANDER_H__

#include <map>
#include <string>

#include "asm_driver.h"
#include "bin_memory.h"
#include "text_reader.h"

namespace libasm {
namespace cli {

struct AsmCommander final {
    AsmCommander(driver::AsmDirective **begin, driver::AsmDirective **end);

    int parseArgs(int argc, const char **argv);
    int usage();
    int assemble();

private:
    driver::AsmDriver _driver;
    // command line arguments
    const char *_prog_name;
    const char *_input_name;
    const char *_output_name;
    const char *_list_name;
    const char *_cpu;
    char _encoder;
    size_t _record_bytes;
    bool _upper_hex;

    bool _line_number;
    bool _verbose;
    std::map<std::string, std::string> _options;

    static constexpr const char *PROG_PREFIX = "asm";
    driver::AsmDirective *defaultDirective();
    int assemble(driver::BinMemory &memory, driver::TextPrinter &out, driver::TextPrinter &err,
            bool reportError = false);
    int parseOptionValue(const char *option);
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
