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

#ifndef __ASM_DRIVER_H__
#define __ASM_DRIVER_H__

#include "cli_memory.h"
#include "asm_directive.h"

#include <stdio.h>
#include <string.h>

namespace libasm {
namespace cli {

template<typename Conf, typename Formatter>
class AsmDriver {
public:
    AsmDriver(AsmDirective<Conf> &directive)
        : _directive(directive)
    {}
    virtual ~AsmDriver() {
        if (_formatter) delete _formatter;
    }

    int main(int argc, const char **argv) {
        _progname = basename(argv[0]);
        if (parseOption(argc, argv))
            return usage();

        _directive.setSymbolMode(false, true);
        CliMemory<Conf> memory;
        if (assemble(memory, nullptr) != 0)
            return 1;

        do {
            _directive.setSymbolMode(true, false);
            CliMemory<Conf> next;
            if (assemble(next, nullptr) != 0)
                return 1;
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
            const char *begin = _formatter->begin();
            if (begin) fprintf(output, "%s\n", begin);
            typedef typename Conf::uintptr_t addr_t;
            memory.dump(
                [this, output]
                (addr_t addr, const uint8_t *data, size_t data_size) {
                    for (size_t i = 0; i < data_size; i += _record_bytes) {
                        auto size = std::min(_record_bytes, data_size - i);
                        const char *line =
                            _formatter->encode(addr + i, data + i, size);
                        fprintf(output, "%s\n", line);
                        fflush(output);
                    }
                });
            const char *end = _formatter->end();
            if (end) fprintf(output, "%s\n", end);
            fclose(output);
        }
        if (_list_name) {
            FILE *list = fopen(_list_name, "w");
            if (list == nullptr) {
                fprintf(stderr, "Can't open list file %s\n", _list_name);
                return 1;
            }
            assemble(memory, list);
            fclose(list);
        }

        return 0;
    }

private:
    AsmDirective<Conf> &_directive;
    AsmListing<Conf> _listing;
    const char *_progname;
    const char *_input_name;
    const char *_output_name;
    const char *_list_name;
    size_t _record_bytes;
    BinFormatter<Conf> *_formatter;
    bool _uppercase;
    bool _line_number;

    int assemble(CliMemory<Conf> &memory, FILE *list) {
        if (_directive.openSource(_input_name)) {
            fprintf(stderr, "Can't open input file %s\n", _input_name);
            return 1;
        }

        int errors = 0;
        _directive.setOrigin(0);
        const char *line;
        while ((line = _directive.readSourceLine()) != nullptr) {
            if (_directive.assembleLine(line, memory)) {
                const char *filename = _directive.currentSource();
                const int lineno = _directive.currentLineno();
                const int column = _directive.errorAt() - line;
                fprintf(stderr, "%s:%d:%d: error %d\n",
                        filename, lineno, column, _directive.getError());
                fprintf(stderr, "%s:%d %s\n",
                        filename, lineno, _directive.errorAt());
                errors++;
                continue;
            }
            if (list)
                printListing(memory, list);
        }
        return errors;
    }

    void printListing(CliMemory<Conf> &memory, FILE *out) {
        _listing.reset(_directive, _uppercase, _line_number);
        do {
            fprintf(out, "%s\n", _listing.getLine());
        } while (_listing.hasNext());
        fflush(out);
    }

    int parseOption(int argc, const char **argv) {
        _input_name = nullptr;
        _output_name = nullptr;
        _list_name = nullptr;
        _record_bytes = 32;
        _formatter = nullptr;
        _uppercase = false;
        _line_number = false;
        char formatter = 0;
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
                    formatter = *opt++;
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
                    if (!_directive.setCpu(argv[i])) {
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
        if (_output_name && strcmp(_output_name, _input_name) == 0) {
            fprintf(stderr, "output file overwrite input file\n");
            return 2;
        }
        if (_list_name && strcmp(_list_name, _input_name) == 0) {
            fprintf(stderr, "listing file overwrite input file\n");
            return 2;
        }
        if (_output_name) {
            if (formatter == 'S') {
                _formatter = new SRecord<Conf>();
            } else if (formatter == 'H') {
                _formatter = new IntelHex<Conf>();
            } else {
                _formatter = new Formatter();
            }
        }
        return 0;
    }

    int usage() {
        fprintf(stderr,
                "usage: %s [-o <output>] [-l <list>] <input>\n"
                " options:\n"
                "  -o <output> : output file\n"
                "  -l <list>   : list file\n"
                "  -S[<bytes>] : output Motorola SREC format\n"
                "  -H[<bytes>] : output Intel HEX format\n"
                "              : optional <bytes> specifies data record length (max 32)\n"
                "  -C          : CPU variant: %s\n"
                "  -u          : use uppercase letter for output\n"
                "  -n          : output line number to list file\n",
                _progname, _directive.listCpu());
        return 2;
    }

    static const char *basename(const char *str, char sep_char = '/') {
        const char *sep = strrchr(str, sep_char);
        return sep ? sep + 1 : str;
    }
};

} // namespace cli
} // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
