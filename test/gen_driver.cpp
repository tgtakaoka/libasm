/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "gen_driver.h"

#include <cstdarg>
#include <cstring>

namespace libasm {
namespace gen {

GenDriver::GenDriver(Disassembler &disassembler)
    : _disassembler(disassembler), _listing(disassembler) {}

static const char *basename(const char *str, char sep_char = '/') {
    const char *sep = strrchr(str, sep_char);
    return sep ? sep + 1 : str;
}

int GenDriver::main(int argc, const char **argv) {
    _progname = basename(argv[0]);
    if (parseOption(argc, argv))
        return usage();

    const char *commentStr = ";;;";
    _listing.setUpperHex(_upper_hex);
    _listing.setUppercase(_uppercase);
    if (_generateGas) {
        commentStr = ";###";
        _disassembler.setOption("c-style", "on");
        _listing.setUpperHex(false);
    }
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
    _listing.setCpu(_cpu.c_str());
    if (!_includeTarget && !_generateGas) {
        printList();
    }
    return 0;
}

int GenDriver::close() {
    if (_output)
        fclose(_output);
    if (_list)
        fclose(_list);
    return 0;
}

// TestGenerator::Printer
void GenDriver::printList() {
    if (_output) {
        do {
            fprintf(_output, "%s\n", _listing.getContent());
        } while (_listing.hasNextContent());
    }
    if (_list) {
        do {
            fprintf(_list, "%s\n", _listing.getLine());
        } while (_listing.hasNextLine());
    }
}

void GenDriver::setOrigin(uint32_t addr) {
    _listing.setOrigin(addr);
    if (!_includeTarget && !_generateGas) {
        printList();
    }
}

void GenDriver::info(const char *fmt, ...) {
#if DEBUG_TRACE
    if (_list) {
        va_list args;
        va_start(args, fmt);
        vfprintf(_list, fmt, args);
    }
#endif
}

int GenDriver::parseOption(int argc, const char **argv) {
    _output_name = nullptr;
    _list_name = nullptr;
    _upper_hex = true;
    _uppercase = false;
    _includeTarget = false;
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
            case 'i':
                _includeTarget = true;
                break;
            case 'g':
                _generateGas = true;
                break;
            case 'h':
                _upper_hex = false;
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
    if (_cpu.empty()) {
        const /* PROGMEM */ auto cpu_P = _disassembler.cpu_P();
        char cpu[strlen_P(cpu_P) + 1];
        strcpy_P(cpu, cpu_P);
        _cpu = cpu;
    }
    return 0;
}

int GenDriver::usage() const {
    const /* PROGMEM */ auto list_P = _disassembler.listCpu_P();
    char listCpu[strlen_P(list_P) + 1];
    strcpy_P(listCpu, list_P);
    fprintf(stderr,
            "usage: %s [-o <output>] [-l <list>]\n"
            " options:\n"
            "  -o <output> : output file\n"
            "  -l <list>   : list file\n"
            "  -C          : CPU variant: %s\n"
            "  -i          : include target; no cpu and org directive\n"
            "  -g          : output GNU as compatible\n"
            "  -h          : use lower case letter for hexadecimal\n"
            "  -u          : use upper case letter for output\n"
            "  -d          : dump debug info\n",
            _progname, listCpu);
    return 2;
}

void GenDriver::printCommandLine(FILE *out, const char *commentStr, const char *progname, int argc,
        const char **argv) const {
    fprintf(out, "%s AUTO GENERATED FILE\n", commentStr);
    fprintf(out, "%s generated by: %s", commentStr, progname);
    for (int i = 1; i < argc; i++)
        fprintf(out, " %s", argv[i]);
    fprintf(out, "\n");
}

}  // namespace gen
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
