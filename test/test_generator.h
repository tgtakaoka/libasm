/* -*- mode: c++; -*- */
#ifndef __TEST_GENERATOR_H__
#define __TEST_GENERATOR_H__

#include "dis_interface.h"
#include "test_memory.h"
#include "levenshtein.h"

template<typename Addr>
class TestGenerator {
public:
    TestGenerator(Disassembler<Addr> &disassembler)
        : _disassembler(disassembler),
          _bytes(new uint8_t(Insn::getMaxBytes()))
    {
        _levenshtein.setInsertCost(3);
        _levenshtein.setDeleteCost(3);
    }

    virtual ~TestGenerator() {
        delete _bytes;
    }

    class Printer {
    public:
        virtual void print(const Insn &insn, const char *operands) = 0;
    };
    void generate(Printer *printer) {
        _prevName = new char[Insn::getMaxName() + 1];
        _addr = 0;
        generate(0, printer);
        delete _prevName;
    }

private:
    Disassembler<Addr> &_disassembler;
    uint8_t *_bytes;
    TestMemory _memory;
    target::uintptr_t _addr;
    Levenshtein<int> _levenshtein;
    char *_prevName;
    char _operands[40];
    char _prevOperands[40];
    int _sameCount;
    int _nearCount;

    void printInsn(const Insn &insn, const char *operands, Printer *printer) {
        printer->print(insn, operands);
        _addr += insn.insnLen();
    }

    template<typename Printer>
    void generate(size_t pos, Printer *printer) {
        for (size_t b = pos + 1; b < Insn::getMaxBytes(); b++)
            _bytes[b] = 0;

        uint8_t i = 0;
        do {
            _bytes[pos] = i;
            _memory.setAddress(_addr);
            _memory.setBytes(_bytes, Insn::getMaxBytes());
            Insn insn;
            _disassembler.decode(_memory, insn, _operands, nullptr);
            if (_disassembler.getError() == OK) {
                const bool sameName = (strcmp(_prevName, insn.name()) == 0);
                if (sameName) {
                    _sameCount++;
                    const int diff = _levenshtein.distance(_prevOperands, _operands);
                    bool near = false;
                    if (_sameCount < 4 && diff < 1) near = true;
                    else if (_sameCount < 20 && diff < 2) near = true;
                    else if (_sameCount < 256 && diff < 3) near = true;
                    else if (_sameCount < 65536L && diff < 6) near = true;
                    if (near) {
                        _nearCount++;
                    } else {
                        printInsn(insn, _operands, printer);
                        strcpy(_prevOperands, _operands);
                    }
                } else {
                    _sameCount = 0;
                    _nearCount = 0;
                    strcpy(_prevName, insn.name());
                    strcpy(_prevOperands, _operands);
                    printInsn(insn, _operands, printer);
                    if (pos + 1 < insn.insnLen())
                        generate(pos + 1, printer);
                }
            }
        } while (++i);
        _bytes[pos] = 0;
    }
};

#endif
