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

#ifndef __TEST_GENERATOR_H__
#define __TEST_GENERATOR_H__

#include "dis_interface.h"
#include "dis_memory.h"
#include "text_buffer.h"

#include <ctype.h>

#if DEBUG_TEXT || DEBUG_TRACE
#include <stdio.h>
#endif

class DataGenerator {
public:
    typedef bool (*Filter)(uint8_t);

    DataGenerator(uint8_t *buffer, Endian endian, int size, Filter filter = nullptr)
        : _buffer(buffer),
          _endian(endian),
          _pos(0),
          _size(size),
          _end(calcEnd(0, size)),
          _filter(filter),
          _i(0)
    {}

    DataGenerator(const DataGenerator &parent, int size, Filter filter = nullptr)
        : _buffer(parent._buffer),
          _endian(parent._endian),
          _pos(parent._pos + parent._size),
          _size(size),
          _end(calcEnd(parent._i, size)),
          _filter(filter),
          _i(parent._i)
    {}

    int pos() const { return _pos; }
    int size() const { return _size; }

    void next() {
        _i = outSizedUint(_i) + 1;
    }
    bool hasNext() {
        if (_i == _end) {
            outSizedUint(0);
            return false;
        }
        return true;
    }

    void outUint8(uint8_t val8, int offset = 0) {
        _buffer[_pos + offset] = val8;
    }

    void debugPrint(const char *msg, const uint8_t *memory) const {
#if DEBUG_TRACE
        printf("%s %d/%d%c: ", msg, _pos, _size, _endian == ENDIAN_BIG? 'B' : 'L');
        for (int i = 0; i < _pos; i++)
            printf(" %02X", memory[i]);
        for (int i = 0; i < _size; i++)
            printf("%c%02X", i == 0 ? '|' : ' ', memory[_pos + i]);
        printf("\n");
        fflush(stdout);
#endif
    }

private:
    uint8_t *const _buffer;
    const Endian _endian;
    int _pos;
    int _size;
    const uint32_t _end;
    Filter _filter;
    uint32_t _i;

    static uint32_t calcEnd(uint32_t start, int size) {
        if (size == 1) return start + 0x100;
        if (size == 2) return start + 0x10000L;
        return start;
    }

    uint32_t outSizedUint(uint32_t val) {
        if (_size == 1) {
            while (_filter && _filter(val))
                val++;
            outUint8(static_cast<uint8_t>(val));
        } else if (_size == 2) {
            outUint16(static_cast<uint16_t>(val));
        } else {
            outUint32(val);
        }
        return val;
    }

    void outUint16(uint16_t val16, int offset = 0) {
        if (_endian == ENDIAN_BIG) {
            outUint8(static_cast<uint8_t>(val16 >> 8), offset + 0);
            outUint8(static_cast<uint8_t>(val16 >> 0), offset + 1);
        } else {
            outUint8(static_cast<uint8_t>(val16 >> 0), offset + 0);
            outUint8(static_cast<uint8_t>(val16 >> 8), offset + 1);
        }
    }

    void outUint32(uint32_t val32, int offset = 0) {
        if (_endian == ENDIAN_BIG) {
            outUint16(static_cast<uint16_t>(val32 >> 16), offset + 0);
            outUint16(static_cast<uint16_t>(val32 >> 0),  offset + 2);
        } else {
            outUint16(static_cast<uint16_t>(val32 >> 0),  offset + 0);
            outUint16(static_cast<uint16_t>(val32 >> 16), offset + 2);
        }
    }
};

template<typename Addr>
class TestData : private DisMemory<Addr> {
public:
    TestData()
        : DisMemory<Addr>(0),
          _operands(60)
    {}

    const Insn &insn() const { return _insn; }
    TextBuffer &operands() { return _operands; }
    const TextBuffer &operands() const { return _operands; }
    Error tryGenerate(
        Disassembler<Addr> &dis,
        Addr addr,
        uint8_t *memory,
        int size,
        bool uppercase) {
        this->_address = addr;
        _memory = memory;
        _memorySize = size;
        _memoryIndex = 0;

        return dis.decode(
            *this, _insn, _operands.buffer(), nullptr, uppercase);
    }

private:
    TextBuffer _operands;
    uint8_t *_memory;
    int _memorySize;
    int _memoryIndex;
    Insn _insn;

    // DisMemory<Addr>
    bool hasNext() const override { return _memoryIndex < _memorySize; }
    uint8_t nextByte() override { return _memory[_memoryIndex++]; }
};

template<typename Addr>
class TestGenerator {
public:
    TestGenerator(
        Disassembler<Addr> &disassembler,
        size_t opcodeSize,
        bool uppercase)
        : _disassembler(disassembler),
          _memorySize(disassembler.maxBytes()),
          _endian(disassembler.endian()),
          _opcodeSize(opcodeSize),
          _uppercase(uppercase) {
        _memory = new uint8_t[_memorySize];
        _addr = 0;
    }

    virtual ~TestGenerator() {
        delete[] _memory;
    }

    class Printer {
    public:
        virtual void print(const Insn &insn, const char *operands) = 0;
    };
    typedef bool (*Filter)(uint8_t);

    TestGenerator<Addr> &generate(Printer &printer, Filter filter = nullptr) {
        DataGenerator gen(_memory, _endian, _opcodeSize, filter);
        return generate(printer, gen);
    }

    TestGenerator<Addr> &generate(
        Printer &printer, target::opcode_t opc1, Filter filter = nullptr) {
        DataGenerator parent(_memory, _endian, _opcodeSize);
        parent.outUint8(opc1);
        DataGenerator gen(parent, _opcodeSize, filter);
        return generate(printer, gen);
    }

    TestGenerator<Addr> &generate(
        Printer &printer, target::opcode_t opc1, target::opcode_t opc2) {
        DataGenerator parent(_memory, _endian, _opcodeSize * 2);
        parent.outUint8(opc1, 0);
        parent.outUint8(opc2, 1);
        DataGenerator gen(parent, _opcodeSize);
        return generate(printer, gen);
    }

    TestGenerator<Addr> &generate(
        Printer &printer,
        target::opcode_t opc1, target::opcode_t opc2, target::opcode_t opc3) {
        DataGenerator parent(_memory, _endian, _opcodeSize * 3);
        parent.outUint8(opc1, 0);
        parent.outUint8(opc2, 1);
        parent.outUint8(opc3, 2);
        DataGenerator gen(parent, _opcodeSize);
        return generate(printer, gen);
    }

private:
    Disassembler<Addr> &_disassembler;
    const int _memorySize;
    const Endian _endian;
    const size_t _opcodeSize;
    const bool _uppercase;
    uint8_t *_memory;
    TestData<Addr> _data1;
    TestData<Addr> _data2;

    Addr _addr;
    Printer *_printer;
    TestData<Addr> *_data;
    TestData<Addr> *_prev;
    int _similarCount;

    TestGenerator<Addr> &generate(Printer &printer, DataGenerator &gen) {
        _printer = &printer;
        _data = &_data1;
        _prev = &_data2;
        generateTests(gen);

        return *this;
    }

    void printInsn(const TestData<Addr> *data) {
        _printer->print(data->insn(), data->operands().buffer());
        _addr += data->insn().length();
    }

    void swapBuffers() {
        if (_data == &_data1) {
            _prev = &_data1;
            _data = &_data2;
        } else {
            _prev = &_data2;
            _data = &_data1;
        }
    }

    int meaningfulTestData(const DataGenerator &gen) {
        if (strcmp(_data->insn().name(), _prev->insn().name())
            || _data->insn().length() != _prev->insn().length()) {
            _similarCount = 0;
#if DEBUG_TEXT
            printf("@@ %s : %s\n", _data->insn().name(), _data->operands().buffer());
#endif
            return -2;
        }

        TextBuffer::analyze(_data->operands(), _prev->operands());
        const TextBuffer &o = _data->operands();
        const TextBuffer &p = _prev->operands();
        int status = -1;
        if (o.prefixLen() == o.length() && p.prefixLen() == p.length()) {
            if (gen.size() == 4 && o.digitsInPrefix() >= 8) {
                status = 4;
            } else if (gen.size() == 2 && o.digitsInPrefix() >= 5
                       && _similarCount >= 300) {
                status = 2;
            } else if (gen.size() == 2 && _similarCount >= 1000) {
                status = 2;
            } else if (o.length() > 0 && o.prefixLen() == o.length()
                       && o.digitsInPrefix() > 0) {
                status = 0;
            } else if (o.prefixLen() > 0 && o.digitsInPrefix() >= 3) {
                status = 0;
            } else if (
                strcmp(_data->operands().buffer(), _prev->operands().buffer()) == 0) {
                status = 0;
            }
        }
#if DEBUG_TEXT
        bool print = true;
        if (status < 0) printf("@@ ");
        else if (status > 0)  printf("!! ");
        else if (status == 0) {
            if (_similarCount % 200 == 0)
                printf("__ ");
            else print = false;
        }
        if (print) {
            printf("%s : {%.*s}%.*s{%s} {%.*s}%.*s{%s} {%d:%d %d:%d} {%d:%d %d:%d} %d %d\n",
                   _data->insn().name(),
                   o.prefixLen(), o.prefix(), o.trunkLen(), o.trunk(), o.suffix(),
                   p.prefixLen(), p.prefix(), p.trunkLen(), p.trunk(), p.suffix(),
                   o.prefixLen(), o.digitsInPrefix(), o.suffixLen(), o.digitsInSuffix(),
                   p.prefixLen(), p.digitsInPrefix(), p.suffixLen(), p.digitsInSuffix(),
                   gen.size(), _similarCount
                );
            fflush(stdout);
        }
#endif
        if (status == 0) {
            _similarCount++;
        } else {
            _similarCount = 0;
        }
        return status;
    }

    void generateTests(DataGenerator &gen) {
        gen.debugPrint("@@ enter", _memory);
        do {
            gen.next();
            gen.debugPrint("@@  loop", _memory);
            _data->tryGenerate(
                _disassembler, _addr, _memory, _memorySize, _uppercase);
            if (_disassembler.getError() == OK) {
                int size = _data->insn().length() - (gen.pos() + gen.size());
                if (size > 0) {
                    if (size % 2 == 0 || size == 1) {
                        DataGenerator child(gen, size);
                        child.debugPrint("@@  long", _memory);
                        generateTests(child);
                        gen.debugPrint("@@  cont", _memory);
                    } else {
                        DataGenerator child(gen, 1);
                        child.debugPrint("@@  odd1", _memory);
                        generateTests(child);
                        gen.debugPrint("@@  cont", _memory);
                    }
                } else if (size < 0) {
                    gen.debugPrint("@@  shrk", _memory);
                    printInsn(_data);
                    swapBuffers();
                    return;
                } else {
                    int abort;
                    if ((abort = meaningfulTestData(gen)) < 0) {
                        printInsn(_data);
                        swapBuffers();
                    } else if (abort > 0) {
                        gen.debugPrint("@@ abort", _memory);
                        return;
                    }
                }
            }
        } while (gen.hasNext());
        gen.debugPrint("@@ leave", _memory);
    }
};

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
