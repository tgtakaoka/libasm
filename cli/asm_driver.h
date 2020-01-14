/* -*- mode: c++; -*- */
#ifndef __ASM_DRIVER_H__
#define __ASM_DRIVER_H__

#include "cli_memory.h"
#include "asm_directive.h"

#include <stdio.h>
#include <string.h>

template<typename Asm>
class AsmDriver {
public:
    typedef typename Asm::addr_t Addr;

    AsmDriver(AsmDirective<Asm> &directive) : _directive(directive) {}
    virtual ~AsmDriver() {
        if (_formatter) delete _formatter;
    }

    int main(int argc, const char **argv) {
        _progname = basename(argv[0]);
        if (parseOption(argc, argv))
            return usage();

        _directive.setSymbolMode(false, true);
        CliMemory<Addr> memory;
        if (assemble(memory, nullptr) != 0)
            return 1;

        do {
            _directive.setSymbolMode(true, false);
            CliMemory<Addr> next;
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
            memory.dump(
                [this, output]
                (Addr addr, const uint8_t *data, size_t data_size) {
                    for (size_t i = 0; i < data_size; i += _record_bytes) {
                        auto size = std::min(_record_bytes, data_size - i);
                        const char *line =
                            _formatter->encode(addr + i, data + i, size);
                        fprintf(output, "%s\n", line);
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
    AsmDirective<Asm> &_directive;
    const char *_progname;
    const char *_input_name;
    const char *_output_name;
    const char *_list_name;
    size_t _record_bytes;
    BinFormatter<Addr> *_formatter;

    int assemble(CliMemory<Addr> &memory, FILE *list) {
        if (_directive.openSource(_input_name)) {
            fprintf(stderr, "Can't open input file %s\n", _input_name);
            return 1;
        }

        int errors = 0;
        _directive.setOrigin(0);
        const char *line;
        while ((line = _directive.readSourceLine()) != nullptr) {
            typename AsmDirective<Asm>::Listing listing{};
            if (_directive.assembleLine(line, memory, listing)) {
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
                printListing(listing, memory, list);
        }
        return errors;
    }

    void printListing(
        typename AsmDirective<Asm>::Listing &listing, CliMemory<Addr> &memory,
        FILE *out) {
        if (sizeof(Addr) == 2) {
            fprintf(out, "%04x:", listing.address);
        } else {
            fprintf(out, "%08x:", listing.address);
        }
        int i = 0;
        while (i < listing.length && i < 6) {
            uint8_t val = 0;
            memory.readByte(listing.address + i, val);
            fprintf(out, " %02x", val);
            i++;
        }
        listing.address += i;
        while (i < 6) {
            fprintf(out, "   ");
            i++;
        }
        if (!listing.label && !listing.instruction) {
            fprintf(out, "%s\n", listing.comment);
            return;
        }
        if (listing.label) {
            fprintf(out, " %-8.*s ", listing.label_len, listing.label);
        } else if (listing.instruction) {
            fprintf(out, "          ");
        }
        fprintf(out, "%-6.*s", listing.instruction_len, listing.instruction);
        fprintf(out, " %-8.*s", listing.operand_len, listing.operand);
        fprintf(out, " %s\n", listing.comment);
        while (i < listing.length) {
            if (sizeof(Addr) == 2) {
                fprintf(out, "%04x:", listing.address);
            } else {
                fprintf(out, "%08x:", listing.address);
            }
            int j = 0;
            while (i + j < listing.length && j < 6) {
                uint8_t val = 0;
                memory.readByte(listing.address + j, val);
                fprintf(out, " %02x", val);
                j++;
            }
            i += j;
            listing.address += j;
            fprintf(out, "\n");
        }
    }

    int parseOption(int argc, const char **argv) {
        _input_name = nullptr;
        _output_name = nullptr;
        _list_name = nullptr;
        _record_bytes = 32;
        _formatter = nullptr;
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
                _formatter = new SRecord<Addr>();
            } else if (formatter == 'H') {
                _formatter = new IntelHex<Addr>();
            } else {
                fprintf(stderr, "No output format specified\n");
                return 3;
            }
        }
        return 0;
    }

    int usage() {
        fprintf(stderr,
                "usage: %s -(S|H[<bytes>]] [-o <output>] [-l <list>] <input>\n"
                "  -S : output Motorola SREC format\n"
                "  -H : output Intel HEX format\n"
                "     : optional <bytes> specifies data record length (max 32)\n",
                _progname);
        return 2;
    }

    static const char *basename(const char *str, char sep_char = '/') {
        const char *sep = strrchr(str, sep_char);
        return sep ? sep + 1 : str;
    }
};

#endif
