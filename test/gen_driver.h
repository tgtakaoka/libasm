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

#include <stdio.h>
#include <string.h>

#include "cli_listing.h"
#include "dis_base.h"
#include "test_generator.h"

namespace libasm {
namespace test {

using libasm::cli::CliListing;
using libasm::cli::ListingLine;

template <typename Conf>
class GenDriver : public TestGenerator<Conf>::Printer, private ListingLine {
public:
    GenDriver(Disassembler &disassembler) : _disassembler(disassembler), _listing() {}

    int main(int argc, const char **argv) {
        _progname = basename(argv[0]);
        if (parseOption(argc, argv))
            return usage();

        const char *commentStr = ";;;";
        if (_generateGas) {
            commentStr = ";###";
            _disassembler.getFormatter().setCStyleHex(true);
        }
        _disassembler.setUppercase(_uppercase);
        _listing.setUppercase(_uppercase);
        _output = nullptr;
        if (_output_name) {
            _output = fopen(_output_name, "w");
            if (_output == nullptr) {
                fprintf(stderr, "Can't open output file %s\n", _output_name);
                return 1;
            }
            printCommandLine(_output, commentStr, _progname, argc, argv);
        }
        _list = nullptr;
        if (_list_name) {
            _list = fopen(_list_name, "w");
            if (_list == nullptr) {
                fprintf(stderr, "Can't open list file %s\n", _list_name);
                return 1;
            }
            printCommandLine(_list, commentStr, _progname, argc, argv);
        }
        if (_generateCpu && !_generateGas)
            pseudo("CPU", _cpu);
        return 0;
    }

    void pseudo(const char *name, const char *operands) {
        _address = 0;
        _generated_size = 0;
        print(name, operands);
    }

    int close() {
        if (_output)
            fclose(_output);
        if (_list)
            fclose(_list);
        return 0;
    }

    bool generateGas() const { return _generateGas; }

private:
    Disassembler &_disassembler;
    CliListing _listing;
    const char *_progname;
    const char *_output_name;
    const char *_list_name;
    bool _uppercase;
    const char *_cpu;
    bool _generateCpu;
    bool _generateGas;
    bool _dump;
    FILE *_output;
    FILE *_list;
    const char *_name;
    const char *_operands;
    typename Conf::uintptr_t _address;
    const uint8_t *_bytes;
    size_t _generated_size;
    const char *_instruction;

    // TestGenerator<Addr>::Printer
    void print(const TestData<Conf> &data) override {
        _name = data.name();
        _address = data.address();
        _bytes = data.bytes();
        _generated_size = data.length();
        print(data.name(), data.operands());
    }

    void origin(typename Conf::uintptr_t addr) override {
        if (!_generateCpu || _generateGas)
            return;
        char operands[40];
        StrBuffer buffer(operands, sizeof(operands));
        _disassembler.getFormatter().formatHex(
                buffer, addr / addrUnit(), config().addressWidth(), false);
        _address = addr;
        _generated_size = 0;
        print("ORG", operands);
    }

    void print(const char *inst, const char *operands) {
        _listing.reset(*this);
        _instruction = inst;
        _operands = operands;
        if (_list) {
            do {
                fprintf(_list, "%s\n", _listing.getLine());
            } while (_listing.hasNext());
            fflush(_list);
        }
        if (_output) {
            do {
                fprintf(_output, "%s\n", _listing.getContent());
            } while (_listing.hasNext());
            fflush(_output);
        }
    }

    FILE *dumpOut() override { return _dump ? _list : nullptr; }

    // ListingLine
    uint16_t lineNumber() const override { return 0; }
    uint16_t includeNest() const override { return 0; }
    uint32_t startAddress() const override { return _address; }
    int generatedSize() const override { return _generated_size; }
    uint8_t getByte(int offset) const override { return _bytes[offset]; }
    bool hasValue() const override { return false; }
    uint32_t value() const override { return 0; }
    bool hasLabel() const override { return false; }
    bool hasInstruction() const override { return true; }
    bool hasOperand() const override { return *_operands; }
    bool hasComment() const override { return false; }
    std::string getLabel() const override { return ""; }
    std::string getInstruction() const override { return std::string(_instruction); }
    std::string getOperand() const override { return std::string(_operands); }
    std::string getComment() const override { return ""; }
    const ConfigBase &config() const override { return _disassembler.config(); }
    int labelWidth() const override { return 6; }
    int codeBytes() const override { return 6; }
    int nameWidth() const override { return config().nameMax() + 1; }
    int operandWidth() const override { return 40; }
    uint8_t addrUnit() const { return static_cast<uint8_t>(config().addressUnit()); }

    int parseOption(int argc, const char **argv) {
        _output_name = nullptr;
        _list_name = nullptr;
        _uppercase = false;
        _cpu = nullptr;
        _generateCpu = true;
        _generateGas = false;
        _dump = false;
        for (int i = 1; i < argc; i++) {
            const char *opt = argv[i];
            if (*opt == '-') {
                switch (*++opt) {
                case 'o':
                    if (++i >= argc) {
                        fprintf(stderr, "-o requires output file name\n");
                        return 1;
                    }
                    _output_name = argv[i];
                    break;
                case 'l':
                    if (++i >= argc) {
                        fprintf(stderr, "-l requires listing file name\n");
                        return 1;
                    }
                    _list_name = argv[i];
                    break;
                case 'C':
                    if (++i >= argc) {
                        fprintf(stderr, "-C requires CPU name\n");
                        return 1;
                    }
                    if (!_disassembler.setCpu(argv[i])) {
                        fprintf(stderr, "unknown CPU '%s'\n", argv[i]);
                        return 4;
                    }
                    _cpu = argv[i];
                    break;
                case 'c':
                    _generateCpu = false;
                    break;
                case 'g':
                    _generateGas = true;
                    break;
                case 'u':
                    _uppercase = true;
                    break;
                case 'd':
                    _dump = true;
                    break;
                default:
                    fprintf(stderr, "unknown option: %s\n", opt);
                    return 1;
                }
            }
        }
        if (_cpu == nullptr)
            _cpu = _disassembler.getCpu();
        return 0;
    }

    int usage() {
        fprintf(stderr,
                "usage: %s [-o <output>] [-l <list>]\n"
                " options:\n"
                "  -o <output> : output file\n"
                "  -l <list>   : list file\n"
                "  -C          : CPU variant: %s\n"
                "  -c          : do not output cpu directive\n"
                "  -g          : output GNU as compatible\n"
                "  -u          : use uppercase letter for output\n"
                "  -d          : dump debug info\n",
                _progname, _disassembler.listCpu());
        return 2;
    }

    static const char *basename(const char *str, char sep_char = '/') {
        const char *sep = strrchr(str, sep_char);
        return sep ? sep + 1 : str;
    }

    void printCommandLine(
            FILE *out, const char *commentStr, const char *progname, int argc, const char **argv) {
        fprintf(out, "%s AUTO GENERATED FILE\n", commentStr);
        fprintf(out, "%s generated by: %s", commentStr, progname);
        for (int i = 1; i < argc; i++)
            fprintf(out, " %s", argv[i]);
        fprintf(out, "\n");
    }
};

}  // namespace test
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
