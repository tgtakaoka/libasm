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

#include "asm_driver.h"

#include <stdio.h>
#include <string.h>

namespace libasm {
namespace cli {

AsmDriver::AsmDriver(std::vector<AsmDirective *> &directives)
    : _commonDir(directives) {}

int AsmDriver::usage() {
    const char *cpuSep = "\n                ";
    std::string cpuList(cpuSep + _commonDir.listCpu(cpuSep));
    AsmDirective *directive = defaultDirective();
    if (directive) {
        cpuList = ": ";
        cpuList += directive->assembler().listCpu();
    } else {
        const char *cpuSep = "\n                ";
        cpuList = _commonDir.listCpu(cpuSep);
    }
    fprintf(stderr,
            "libasm assembler (version " LIBASM_VERSION_STRING
            ")\n"
            "usage: %s [-o <output>] [-l <list>] <input>\n"
            "  -C <CPU>    : target CPU%s\n"
            "  -o <output> : output file\n"
            "  -l <list>   : list file\n"
            "  -S[<bytes>] : output Motorola S-Record format\n"
            "  -H[<bytes>] : output Intel HEX format\n"
            "              : optional <bytes> specifies data record length "
            "(max 32)\n"
            "  -u          : use uppercase letter for listing\n"
            "  -n          : output line number to list file\n"
            "  -v          : print progress verbosely\n",
            _progname, cpuList.c_str());
    return 2;
}

int AsmDriver::assemble() {
    _commonDir.setSymbolMode(false, true);
    int pass = 0;
    CliMemory memory;
    if (_verbose) {
        fprintf(stderr,
                "libasm assembler (version " LIBASM_VERSION_STRING ")\n");
        fprintf(stderr, "%s: Pass %d\n", _input_name, ++pass);
    }
    (void)assemble(memory);

    do {
        _commonDir.setSymbolMode(true, false);
        CliMemory next;
        if (_verbose)
            fprintf(stderr, "%s: Pass %d\n", _input_name, ++pass);
        (void)assemble(next);
        if (memory.equals(next))
            break;
        memory.swap(next);
    } while (true);

    if (_output_name) {
        FILE *output = fopen(_output_name, "w");
        if (output == nullptr) {
            fprintf(stderr, "Can't open output file %s\n", _output_name);
            return 1;
        }
        AsmDirective *directive = _commonDir.currentDirective();
        const AddressWidth addrWidth = directive->assembler().addressWidth();
        BinFormatter *formatter;
        if (_formatter == 'S') {
            formatter = new MotoSrec(addrWidth);
        } else if (_formatter == 'H') {
            formatter = new IntelHex(addrWidth);
        } else {
            formatter = directive->defaultFormatter();
        }
        const char *begin = formatter->begin();
        if (begin)
            fprintf(output, "%s\n", begin);
        memory.dump([this, output, formatter](uint32_t addr,
                            const uint8_t *data, size_t data_size) {
            if (_verbose)
                fprintf(stderr, "%s: Write %4zu bytes %04x-%04x\n",
                        _output_name, data_size, addr,
                        (uint32_t)(addr + data_size - 1));
            for (size_t i = 0; i < data_size; i += _record_bytes) {
                auto size = std::min(_record_bytes, data_size - i);
                const char *line = formatter->prepare(addr + i);
                if (line)
                    fprintf(output, "%s\n", line);
                line = formatter->encode(addr + i, data + i, size);
                fprintf(output, "%s\n", line);
                fflush(output);
            }
        });
        const char *end = formatter->end();
        if (end)
            fprintf(output, "%s\n", end);
        fclose(output);
        delete formatter;
    }
    FILE *list = nullptr;
    if (_list_name) {
        list = fopen(_list_name, "w");
        if (list == nullptr) {
            fprintf(stderr, "Can't open list file %s\n", _list_name);
            return 1;
        }
        if (_verbose)
            fprintf(stderr, "%s: Opened for listing\n", _list_name);
    }
    if (_verbose)
        fprintf(stderr, "%s: Pass listing\n", _input_name);
    assemble(memory, list, true);
    if (list)
        fclose(list);

    return 0;
}

int AsmDriver::assemble(CliMemory &memory, FILE *list, bool reportError) {
    if (_commonDir.openSource(_input_name)) {
        fprintf(stderr, "Can't open input file %s\n", _input_name);
        return 1;
    }

    int errors = 0;
    _commonDir.currentDirective()->assembler().reset();
    _commonDir.setOrigin(0);
    const char *line;
    while ((line = _commonDir.readSourceLine()) != nullptr) {
        if (_commonDir.assembleLine(line, memory) && reportError) {
            const char *filename = _commonDir.currentSource();
            const int lineno = _commonDir.currentLineno();
            const int column = _commonDir.errorAt() - line + 1;
            fprintf(stderr, "%s:%d:%d: error: %s\n", filename, lineno, column,
                    _commonDir.errorText());
            fprintf(stderr, "%s:%d %s\n", filename, lineno, line);
            if (list) {
                fprintf(list, "%s:%d:%d: error: %s\n", filename, lineno, column,
                        _commonDir.errorText());
                fprintf(list, "%s:%d %s\n", filename, lineno, line);
            }
            errors++;
            continue;
        }
        if (list)
            printListing(memory, list);
    }
    return errors;
}

void AsmDriver::printListing(CliMemory &memory, FILE *out) {
    _listing.setUppercase(_uppercase);
    _listing.enableLineNumber(_line_number);
    _listing.reset(_commonDir);
    do {
        fprintf(out, "%s\n", _listing.getLine());
    } while (_listing.hasNext());
    fflush(out);
}

AsmDirective *AsmDriver::defaultDirective() {
    const size_t prefix_len = strlen(PROG_PREFIX);
    AsmDirective *directive = nullptr;
    if (_progname && strncmp(_progname, PROG_PREFIX, prefix_len) == 0) {
        const char *cpu = _progname + prefix_len;
        directive = _commonDir.restrictCpu(cpu);
    }
    return directive;
}

int AsmDriver::parseOption(int argc, const char **argv) {
    _progname = basename(argv[0]);
    _input_name = nullptr;
    _output_name = nullptr;
    _list_name = nullptr;
    _record_bytes = 32;
    _formatter = 0;
    _uppercase = false;
    _line_number = false;
    _verbose = false;
    AsmDirective *directive = defaultDirective();
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
            case 'S':
            case 'H':
                _formatter = *opt++;
                if (*opt) {
                    char *end;
                    unsigned long v = strtoul(opt, &end, 10);
                    if (*end || v > 64) {
                        fprintf(stderr, "invalid record length: %s\n", argv[i]);
                        return 3;
                    }
                    _record_bytes = v;
                }
                break;
            case 'C':
                if (++i >= argc) {
                    fprintf(stderr, "-C requires CPU name\n");
                    return 1;
                }
                directive = _commonDir.setCpu(argv[i]);
                if (directive == nullptr) {
                    fprintf(stderr, "unknown CPU '%s'\n", argv[i]);
                    return 4;
                }
                break;
            case 'u':
                _uppercase = true;
                break;
            case 'n':
                _line_number = true;
                break;
            case 'v':
                _verbose = true;
                break;
            default:
                fprintf(stderr, "unknown option: %s\n", opt);
                return 1;
            }
        } else {
            if (_input_name) {
                fprintf(stderr, "multiple input files specified: %s and %s\n",
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

const char *AsmDriver::basename(const char *str, char sep_char) {
    const char *sep = strrchr(str, sep_char);
    return sep ? sep + 1 : str;
}

}  // namespace cli
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
