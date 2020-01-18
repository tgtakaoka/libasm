/* -*- mode: c++; -*- */
#ifndef __DIS_DRIVER_H__
#define __DIS_DRIVER_H__

#include "cli_memory.h"
#include "dis_interface.h"
#include "dis_listing.h"
#include "file_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

template<typename Addr, bool wordBase = false>
class DisDriver {
public:
    DisDriver(Disassembler<Addr> &disassembler)
        : _disassembler(disassembler),
          _uppercase(false)
    {}

    virtual ~DisDriver() {
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
        CliMemory<Addr> memory;
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
            (Addr base, const uint8_t *data, size_t size) {
                DisListing<Addr, wordBase>
                    listing(_disassembler, memory, _uppercase);
                if (list) {
                    fprintf(list, "%s\n", listing.origin(base, true));
                }
                if (output) {
                    fprintf(output, "%s\n", listing.origin(base));
                }
                Insn insn;
                for (size_t pc = 0; pc < size; pc += insn.insnLen()) {
                    Addr address = base + pc;
                    listing.disassemble(address, insn);
                    if (list) {
                        if (_disassembler.getError())
                            fprintf(list, "Error %d\n", _disassembler.getError());
                        do {
                            fprintf(list, "%s\n", listing.getLine());
                        } while (listing.hasNext());
                    }
                    if (output) {
                        if (_disassembler.getError())
                            fprintf(output, "Error %d\n", _disassembler.getError());
                        do {
                            fprintf(output, "%s\n", listing.getContent());
                        } while (listing.hasNext());
                    }
                }
            });
        if (output) fclose(output);
        if (list) fclose(list);

        return 0;
    }

private:
    Disassembler<Addr> &_disassembler;
    bool _uppercase;
    const char *_progname;
    const char *_input_name;
    const char *_output_name;
    const char *_list_name;
    BinFormatter<Addr> *_formatter;

    int readInput(
        FILE *input, const char *filename, CliMemory<Addr> &memory) {
        int lineno = 0;
        int errors = 0;
        size_t line_len = 128;
        char *line = static_cast<char *>(malloc(line_len));
        int len;
        while ((len = getLine(line, line_len, input)) > 0) {
            lineno++;
            Addr addr;
            size_t size;
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

    BinFormatter<Addr> *determineInputFormat(const char *input_name) {
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

        if (c == 'S') return new SRecord<Addr>();
        if (c == ':') return new IntelHex<Addr>();
        return nullptr;
    }
    
    int parseOption(int argc, const char **argv) {
        _input_name = nullptr;
        _output_name = nullptr;
        _list_name = nullptr;
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
                    if (!_disassembler.acceptCpu(argv[i])) {
                        fprintf(stderr, "unknown CPU '%s'\n", argv[i]);
                        return 4;
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

    int usage() {
        fprintf(stderr,
                "usage: %s [-C <cpu>] [-u] [-o <output>] [-l <list>] <input>\n"
                "  -C : CPU variant: 6809, 6309, 6502, 65c02, 8080, z80, 9995\n"
                "  -u : use uppercase letter for output\n"
                "  <input> file can be Motorola SREC or Intel HEX format\n",
                _progname);
        return 2;
    }

    static const char *basename(const char *str, char sep_char = '/') {
        const char *sep = strrchr(str, sep_char);
        return sep ? sep + 1 : str;
    }
};

#endif
