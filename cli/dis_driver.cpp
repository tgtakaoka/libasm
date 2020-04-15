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

#include "dis_driver.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace libasm {
namespace cli {

DisDriver::~DisDriver() {
    if (_formatter) delete _formatter;
}

int DisDriver::usage() {
    const char *cpuSep = "\n                ";
    std::string cpuList;
    for (auto dis : *_disassemblers) {
        cpuList += cpuSep;
        cpuList += dis->listCpu();
    }
    fprintf(stderr,
            "usage: %s -C <cpu> [-u] [-o <output>] [-l <list>] <input>\n"
            " options:\n"
            "  -C          : CPU variant%s\n"
            "  -o <output> : output file\n"
            "  -l <list>   : list file\n"
            "  -u          : use uppercase letter for output\n"
            "  <input>     : file can be Motorola SREC or Intel HEX format\n",
            _progname, cpuList.c_str());
    return 2;
}

int DisDriver::parseOption(
    int argc, const char **argv, Disassembler &disassembler) {
    std::vector<Disassembler *> disassemblers = { &disassembler };
    return parseOption(argc, argv, disassemblers);
}

int DisDriver::parseOption(
    int argc, const char **argv, std::vector<Disassembler *> &disassemblers) {
    _disassemblers = &disassemblers;
    _progname = basename(argv[0]);
    return parseOption(argc, argv);
}

int DisDriver::disassemble() {
    FILE *input = fopen(_input_name, "r");
    if (input == nullptr) {
        fprintf(stderr, "Can't open input file %s\n", _input_name);
        return 1;
    }
    CliMemory memory;
    if (readInput(input, _input_name, memory) != 0)
        return 1;
    fclose(input);

    FILE *output = nullptr;
    if (_output_name) {
        output = fopen(_output_name, "w");
        if (output == nullptr) {
            fprintf(stderr, "Can't open output file %s\n", _output_name);
            return 1;
        }
    }
    FILE *list = nullptr;
    if (_list_name) {
        list = fopen(_list_name, "w");
        if (list == nullptr) {
            fprintf(stderr, "Can't open list file %s\n", _list_name);
            return 1;
        }
    }
    memory.dump(
        [this, output, list, &memory]
        (uint32_t base, const uint8_t *data, size_t size) {
            DisDirective listing(*_disassembler, memory, _uppercase);
            if (list) {
                fprintf(list, "%s\n", listing.origin(base, true));
                fflush(list);
            }
            if (output) {
                fprintf(output, "%s\n", listing.origin(base));
                fflush(output);
            }
            Insn insn;
            for (size_t pc = 0; pc < size; pc += insn.length()) {
                uint32_t address = base + pc;
                listing.disassemble(address, insn);
                if (list) {
                    if (_disassembler->getError())
                        fprintf(list, "Error %d\n", _disassembler->getError());
                    do {
                        fprintf(list, "%s\n", listing.getLine());
                    } while (listing.hasNext());
                    fflush(list);
                }
                if (output) {
                    if (_disassembler->getError())
                        fprintf(output, "Error %d\n", _disassembler->getError());
                    do {
                        fprintf(output, "%s\n", listing.getContent());
                    } while (listing.hasNext());
                    fflush(output);
                }
            }
        });
    if (output) fclose(output);
    if (list) fclose(list);

    return 0;
}

int DisDriver::readInput(
    FILE *input, const char *filename, CliMemory &memory) {
    int lineno = 0;
    int errors = 0;
    size_t line_len = 128;
    char *line = static_cast<char *>(malloc(line_len));
    int len;
    while ((len = getLine(line, line_len, input)) > 0) {
        lineno++;
        uint32_t addr;
        host::uint_t size;
        uint8_t *data = _formatter->decode(line, addr, size);
        if (data == nullptr) {
            if (errors < 3) {
                fprintf(stderr, "%s:%d: format error: %s\n",
                        filename, lineno, line);
            } else if (errors == 3) {
                fprintf(stderr, "%s: too many errors, wrong format?\n",
                        filename);
            }
            errors++;
            continue;
        }
        if (size)
            memory.writeBytes(addr, data, size);
    }
    free(line);
    return errors;
}

BinFormatter *DisDriver::determineInputFormat(const char *input_name) {
    FILE *input = fopen(input_name, "r");
    if (input == nullptr) {
        fprintf(stderr, "Can't open input file %s\n", input_name);
        return nullptr;
    }
            
    size_t line_len = 128;
    char *line = static_cast<char *>(malloc(line_len));
    const int len = getLine(line, line_len, input);
    fclose(input);
    const char c = (len > 0) ? *line : 0;
    free(line);

    const AddressWidth addrWidth = _disassembler->addressWidth();
    if (c == 'S') return new MotoSrec(addrWidth);
    if (c == ':') return new IntelHex(addrWidth);
    return nullptr;
}
    
int DisDriver::parseOption(int argc, const char **argv) {
    _uppercase = false;
    _input_name = nullptr;
    _output_name = nullptr;
    _list_name = nullptr;
    _disassembler = nullptr;
    _formatter = nullptr;
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
            case 'C': {
                if (++i >= argc) {
                    fprintf(stderr, "-C requires CPU name\n");
                    return 1;
                }
                Disassembler *disassembler = nullptr;
                for (auto dis : *_disassemblers) {
                    if (dis->setCpu(argv[i])) {
                        disassembler = dis;
                        break;
                    }
                }
                if (disassembler == nullptr) {
                    fprintf(stderr, "unknown CPU '%s'\n", argv[i]);
                    return 4;
                }
                _disassembler = disassembler;
                break;
            }
            case 'u':
                _uppercase = true;
                break;
            default:
                fprintf(stderr, "unknown option: %s\n", opt);
                return 1;
            }
        } else {
            if (_input_name) {
                fprintf(stderr,
                        "multiple input files specified: %s and %s\n",
                        _input_name, opt);
                return 1;
            }
            _input_name = opt;
        }
    }
    if (!_input_name) {
        fprintf(stderr, "no input file\n");
        return 1;
    }
    _formatter = determineInputFormat(_input_name);
    if (_formatter == nullptr) {
        fprintf(stderr, "Can't determine format of input file %s\n",
                _input_name);
        return 2;
    }
    if (_output_name && strcmp(_output_name, _input_name) == 0) {
        fprintf(stderr, "output file overwrite input file\n");
        return 2;
    }
    if (_list_name && strcmp(_list_name, _input_name) == 0) {
        fprintf(stderr, "listing file overwrite input file\n");
        return 2;
    }
    return 0;
}

const char *DisDriver::basename(const char *str, char sep_char) {
    const char *sep = strrchr(str, sep_char);
    return sep ? sep + 1 : str;
}

} // namespace cli
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4: