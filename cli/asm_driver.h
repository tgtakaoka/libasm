/* -*- mode: c++; -*- */
#ifndef __ASM_DRIVER_H__
#define __ASM_DRIVER_H__

#include "asm_memory.h"
#include "asm_directive.h"
#include "file_util.h"

#include <stdio.h>
#include <stdlib.h>
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

        FILE *input = fopen(_input_name, "r");
        if (input == nullptr) {
            fprintf(stderr, "Can't open input file %s\n", _input_name);
            return 1;
        }

        _directive.setSymbolMode(false, true);
        AsmMemory<Addr> memory;
        if (assemble(input, _input_name, memory, nullptr) != 0)
            return 1;

        do {
            fseek(input, 0L, SEEK_SET);
            _directive.setSymbolMode(true, false);
            AsmMemory<Addr> next;
            if (assemble(input, _input_name, next, nullptr) != 0)
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
            memory.dump(_formatter,
                        [&output](const char *line) {
                            fprintf(output, "%s\n", line);
                        });
            fclose(output);
        }
        if (_list_name) {
            FILE *list = fopen(_list_name, "w");
            if (list == nullptr) {
                fprintf(stderr, "Can't open list file %s\n", _list_name);
                return 1;
            }
            fseek(input, 0L, SEEK_SET);
            assemble(input, _input_name, memory, list);
            fclose(list);
        }
        fclose(input);

        return 0;
    }

private:
    AsmDirective<Asm> &_directive;
    const char *_progname;
    const char *_input_name;
    const char *_output_name;
    const char *_list_name;
    BinFormatter<Addr> *_formatter;

    int assemble(
        FILE *input, const char *filename, AsmMemory<Addr> &memory,
        FILE *list) {
        int lineno = 0;
        int errors = 0;
        _directive.setOrigin(0);
        size_t line_len = 128;
        char *line = static_cast<char *>(malloc(line_len));
        int len;
        while ((len = getLine(line, line_len, input)) > 0) {
            lineno++;
            typename AsmDirective<Asm>::Listing listing{};
            if (_directive.assembleLine(line, memory, listing)) {
                int column = _directive.errorAt() - line;
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
        free(line);
        return errors;
    }

    void printListing(
        typename AsmDirective<Asm>::Listing &listing, AsmMemory<Addr> &memory,
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
        bool opt_list = false;
        for (int i = 1; i < argc; i++) {
            const char *opt = argv[i];
            if (*opt == '-') {
                if (strcmp(opt, "-o") == 0) {
                    if (++i >= argc) return 1;
                    const char *fmt = basename(argv[i], '.');
                    if (strcasecmp(fmt, "hex") == 0) {
                        _formatter = new IntelHex<Addr>();
                    } else if (strcasecmp(fmt, "s19") == 0) {
                        _formatter = new SRecord<Addr>();
                    } else {
                        fprintf(stderr, "Unknown output format: %s\n", fmt);
                        return 1;
                    }
                    _output_name = argv[i];
                } else if (strcmp(opt, "-l") == 0) {
                    opt_list = true;
                } else {
                    return 1;
                }
            } else {
                if (_input_name) return 1;
                _input_name = opt;
            }
        }
        if (!_input_name) return 1;
        if (opt_list) {
            char *name = static_cast<char *>(
                malloc(strlen(_input_name) + 5));
            strcpy(name, _input_name);
            char *suffix = strrchr(name, '.');
            if (!suffix) suffix = name + strlen(name);
            strcpy(suffix, ".lst");
            _list_name = name;
        }
        return 0;
    }

    int usage() {
        fprintf(stderr,
                "usage: %s [-o <output>.<fmt>] [-l] <input>\n"
                "  <fmt> can be\n"
                "        hex Intel Hex\n"
                "        s19 Motorola S-record\n",
                _progname);
        return 2;
    }

    static const char *basename(const char *str, char sep_char = '/') {
        const char *sep = strrchr(str, sep_char);
        return sep ? sep + 1 : str;
    }
};

#endif
