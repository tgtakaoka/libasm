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

#ifndef __LIBASM_GEN_DRIVER_H__
#define __LIBASM_GEN_DRIVER_H__

#include <cstdio>
#include <string>

#include "dis_base.h"
#include "test_generator.h"

namespace libasm {
namespace gen {

struct GenDriver : TestGenerator::Formatter {
    GenDriver(Disassembler &disassembler);

    int main(int argc, const char **argv);
    void pseudo(const char *name, const char *operands);
    int close();
    bool generateGas() const { return _generateGas; }
    bool generateZilog() const { return _generateZilog; }
    const char *cpu() const { return _cpu.c_str(); }

private:
    Disassembler &_disassembler;
    driver::DisFormatter _formatter;
    const char *_progname;
    const char *_output_name;
    const char *_list_name;
    bool _upper_hex;
    bool _uppercase;
    std::string _cpu;
    bool _includeTarget;
    bool _generateGas;
    bool _generateZilog;
    bool _dump;
    FILE *_output;
    FILE *_list;

    // TestGenerator::Formatter
    driver::DisFormatter &formatter() override final { return _formatter; }
    void printList() override final;
    void flush() override final;
    void setOrigin(uint32_t addr) override final;

    void info(const char *fmt, va_list args) override final;

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
