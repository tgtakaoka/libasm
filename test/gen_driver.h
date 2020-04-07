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

#include "asm_listing.h"
#include "dis_interface.h"
#include "test_generator.h"

#include <stdio.h>
#include <string.h>

namespace libasm {
namespace test {

using libasm::cli::AsmLine;
using libasm::cli::AsmListing;

template<typename Conf>
class GenDriver : public TestGenerator<Conf>::Printer,
                  private AsmLine<typename Conf::uintptr_t> {
    typedef typename Conf::uintptr_t addr_t;

public:
    GenDriver(Disassembler<Conf> &disassembler)
        : _disassembler(disassembler),
          _listing()
    {}

    int main(int argc, const char **argv) {
        _progname = basename(argv[0]);
        if (parseOption(argc, argv))
            return usage();

        _output = nullptr;
        if (_output_name) {
            _output = fopen(_output_name, "w");
            if (_output == nullptr) {
                fprintf(stderr, "Can't open output file %s\n", _output_name);
                return 1;
            }
            printCommandLine(_output, _progname, argc, argv);
        }
        _list = nullptr;
        if (_list_name) {
            _list = fopen(_list_name, "w");
            if (_list == nullptr) {
                fprintf(stderr, "Can't open list file %s\n", _list_name);
                return 1;
            }
            printCommandLine(_list, _progname, argc, argv);
        }
        return 0;
    }

    bool uppercase() const { return _uppercase; }

    int close() {
        if (_output) fclose(_output);
        if (_list) fclose(_list);
        return 0;
    }

private:
    Disassembler<Conf> &_disassembler;
    AsmListing<Conf> _listing;
    const char *_progname;
    const char *_output_name;
    const char *_list_name;
    bool _uppercase;
    FILE *_output;
    FILE *_list;
    const Insn<Conf> *_insn;
    const char *_operands;

    // TestGenerator<Addr>::Printer
    void print(const Insn<Conf> &insn, const char *operands) override {
        _insn = &insn;
        _operands = operands;
        _listing.reset(*this, _uppercase, false);
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

    // AsmLine<Addr>
    uint16_t lineNumber() const override { return 0; }
    uint16_t includeNest() const override { return 0; }
    addr_t startAddress() const override { return _insn->address(); }
    int generatedSize() const override { return _insn->length(); }
    uint8_t getByte(int offset) const override { return _insn->bytes()[offset]; }
    bool hasValue() const override { return false; }
    uint32_t value() const override { return 0; }
    bool hasLabel() const override { return false; }
    bool hasInstruction() const override { return true; }
    bool hasOperand() const override { return *_operands; }
    bool hasComment() const override { return false; }
    std::string getLabel() const override { return ""; }
    std::string getInstruction() const override { return std::string(_insn->name()); }
    std::string getOperand() const override { return std::string(_operands); }
    std::string getComment() const override { return ""; }
    int maxBytes() const override { return 6; }
    int labelWidth() const override { return 6; }
    int instructionWidth() const override { return Conf::name_max + 1; }
    int operandWidth() const override { return 40; }

    int parseOption(int argc, const char **argv) {
        _output_name = nullptr;
        _list_name = nullptr;
        _uppercase = false;
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
                    break;
                case 'u':
                    _uppercase = true;
                    break;
                default:
                    fprintf(stderr, "unknown option: %s\n", opt);
                    return 1;
                }
            }
        }
        return 0;
    }

    int usage() {
        fprintf(stderr,
                "usage: %s [-o <output>] [-l <list>]\n"
                " options:\n"
                "  -o <output> : output file\n"
                "  -l <list>   : list file\n"
                "  -C          : CPU variant: %s\n"
                "  -u          : use uppercase letter for output\n",
                _progname, _disassembler.listCpu());
        return 2;
    }

    static const char *basename(const char *str, char sep_char = '/') {
        const char *sep = strrchr(str, sep_char);
        return sep ? sep + 1 : str;
    }

    static void printCommandLine(
        FILE *out, const char *progname, int argc, const char **argv) {
        fprintf(out, ";;; AUTO GENERATED FILE\n");
        fprintf(out, ";;; generated by: %s", progname);
        for (int i = 1; i < argc; i++) fprintf(out, " %s", argv[i]);
        fprintf(out, "\n\n");
    }
};

} // namespace test
} // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
