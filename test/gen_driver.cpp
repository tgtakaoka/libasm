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
    : _disassembler(disassembler), _formatter(disassembler, "TestGenerator") {}

const char *basename(const char *str, char sep_char = '/') {
    const char *sep = strrchr(str, sep_char);
    return sep ? sep + 1 : str;
}

int GenDriver::main(int argc, const char **argv) {
    _progname = basename(argv[0]);
    if (parseOption(argc, argv))
        return usage();

    const char *commentStr = ";;;";
    _disassembler.setUpperHex(_upper_hex);
    _formatter.setUpperHex(_upper_hex);
    _disassembler.setUppercase(_uppercase);
    _formatter.setUppercase(_uppercase);
    if (_generateGas) {
        commentStr = ";###";
        _disassembler.setOption("c-style", "on");
        _formatter.setUpperHex(false);
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
    _disassembler.setCpu(_cpu.c_str());
    _formatter.setCpu(_cpu.c_str());
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
    char buffer[256];
    StrBuffer out{buffer, sizeof(buffer)};
    if (_output) {
        do {
            fprintf(_output, "%s\n", _formatter.getContent(out.reset()).str());
        } while (_formatter.hasNextContent());
    }
    if (_list) {
        do {
            fprintf(_list, "%s\n", _formatter.getLine(out.reset()).str());
        } while (_formatter.hasNextLine());
    }
}

void GenDriver::flush() {
    if (_output)
        fflush(_output);
    if (_list)
        fflush(_list);
}

void GenDriver::setOrigin(uint32_t addr) {
    _formatter.setOrigin(addr);
    if (!_includeTarget && !_generateGas) {
        printList();
    }
}

void GenDriver::info(const char *fmt, va_list args) {
    if (_list)
        vfprintf(_list, fmt, args);
}

int GenDriver::parseOption(int argc, const char **argv) {
    _output_name = nullptr;
    _list_name = nullptr;
    _upper_hex = true;
    _uppercase = false;
    _includeTarget = false;
    _generateGas = false;
    _generateZilog = false;
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
            case 'z':
                _generateZilog = true;
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
        const /* PROGMEM */ auto cpu_P = _disassembler.config().cpu_P();
        char cpu[strlen_P(cpu_P) + 1];
        strcpy_P(cpu, cpu_P);
        _cpu = cpu;
    }
    return 0;
}

int GenDriver::usage() const {
    const /* PROGMEM */ auto list_P = _disassembler.config().listCpu_P();
    char listCpu[strlen_P(list_P) + 1];
    strcpy_P(listCpu, list_P);
    fprintf(stderr, R"(usage: %s [-o <output>] [-l <list>]
options:
  -o <output> : output file
  -l <list>   : list file
  -C          : CPU variant: %s
  -i          : include target; no cpu and org directive
  -g          : output GNU as compatible
  -z          : output Zilog sytax
  -h          : use lower case letter for hexadecimal
  -u          : use upper case letter for output
  -d          : dump debug info
)",
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
