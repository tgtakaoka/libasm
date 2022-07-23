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

#ifndef __GEN_DRIVER_H__
#define __GEN_DRIVER_H__

#include "dis_base.h"
#include "gen_formatter.h"
#include "test_generator.h"

#include <cstdio>
#include <string>

namespace libasm {
namespace gen {

class GenDriver : public TestGenerator::Formatter {
public:
    GenDriver(Disassembler &disassembler);

    int main(int argc, const char **argv);
    void pseudo(const char *name, const char *operands);
    int close();
    bool generateGas() const { return _generateGas; }

private:
    Disassembler &_disassembler;
    GenFormatter _listing;
    const char *_progname;
    const char *_output_name;
    const char *_list_name;
    bool _uppercase;
    std::string _cpu;
    bool _includeTarget;
    bool _generateGas;
    bool _dump;
    FILE *_output;
    FILE *_list;

    // TestGenerator::Formatter
    GenFormatter &listing() override final { return _listing; }
    void printList() override final;
    void setOrigin(uint32_t addr) override final;

    void info(const char *fmt, ...) override final;

    int parseOption(int argc, const char **argv);
    int usage() const;

    void printCommandLine(FILE *out, const char *commentStr, const char *progname, int argc,
            const char **argv) const;
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
