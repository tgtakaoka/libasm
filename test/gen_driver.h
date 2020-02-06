/* -*- mode: c++; -*- */
#ifndef __GEN_DRIVER_H__
#define __GEN_DRIVER_H__

#include "asm_listing.h"
#include "dis_interface.h"
#include "test_generator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

template<typename Addr>
class GenDriver : private AsmLine<Addr>,
                  private TestGenerator<Addr>::Printer {
public:
    GenDriver(Disassembler<Addr> &disassembler)
        : _disassembler(disassembler),
          _generator(disassembler),
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
        }
        _list = nullptr;
        if (_list_name) {
            _list = fopen(_list_name, "w");
            if (_list == nullptr) {
                fprintf(stderr, "Can't open list file %s\n", _list_name);
                return 1;
            }
        }

        _generator.generate(this);
        if (_output) fclose(_output);
        if (_list) fclose(_list);

        return 0;
    }

private:
    Disassembler<Addr> &_disassembler;
    TestGenerator<Addr> _generator;
    AsmListing<Addr> _listing;
    const char *_progname;
    const char *_output_name;
    const char *_list_name;
    FILE *_output;
    FILE *_list;
    const Insn *_insn;
    const char *_operands;

    // TestGenerator<Addr>::Printer
    void print(const Insn &insn, const char *operands) override {
        _insn = &insn;
        _operands = operands;
        _listing.reset(*this, false, false);
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
    Addr startAddress() const override { return _insn->address(); }
    int generatedSize() const override { return _insn->insnLen(); }
    uint8_t getByte(int offset) const override { return _insn->bytes()[offset]; }
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
    int instructionWidth() const override { return Insn::getMaxName() + 1; }
    int operandWidth() const override { return 40; }

    int parseOption(int argc, const char **argv) {
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
                "usage: %s [-C <cpu>] [-o <output>] [-l <list>]\n"
                "  -C : CPU variant: 6809, 6309, 6502, 65c02, 8080, z80, 9995\n",
                _progname);
        return 2;
    }

    static const char *basename(const char *str, char sep_char = '/') {
        const char *sep = strrchr(str, sep_char);
        return sep ? sep + 1 : str;
    }
};

#endif