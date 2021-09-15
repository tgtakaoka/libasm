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

#ifndef __ARDUINO_EXAMPLE_H__
#define __ARDUINO_EXAMPLE_H__

#include <libcli.h>

#include "asm_base.h"
#include "dis_base.h"
#include "str_memory.h"

namespace libasm {
namespace arduino {

using libcli::Cli;

class BaseExample : protected SymbolTable {
public:
    virtual void begin(Stream &console) { _cli.begin(console); }

    void loop() { _cli.loop(); }

protected:
    Cli _cli;
    uint32_t _origin;

    BaseExample(ConfigBase *config) : _cli(), _config(config) {}

    virtual const /*PROGMEM*/ char *getCpu() const = 0;
    virtual bool setCpu(const char *) = 0;

    void printAddress(uint32_t addr) {
        const AddressWidth width = _config->addressWidth();
        if (width == ADDRESS_16BIT || width == ADDRESS_12BIT)
            _cli.printHex16(addr);
        if (width == ADDRESS_20BIT)
            _cli.printHex20(addr);
        if (width == ADDRESS_24BIT)
            _cli.printHex24(addr);
        if (width == ADDRESS_32BIT)
            _cli.printHex32(addr);
    }

    void printBytes(const uint8_t *bytes, uint8_t length) {
        const OpCodeWidth width = _config->opCodeWidth();
        const Endian endian = _config->endian();
        for (uint8_t i = 0; i < length;) {
            _cli.print(' ');
            if (width == OPCODE_8BIT)
                _cli.printHex8(bytes[i++]);
            if (width == OPCODE_16BIT && endian == ENDIAN_BIG) {
                uint16_t val = static_cast<uint16_t>(bytes[i++]) << 8;
                val |= bytes[i++];
                _cli.printHex16(val);
            }
            if (width == OPCODE_16BIT && endian == ENDIAN_LITTLE) {
                uint16_t val = bytes[i++];
                val = static_cast<uint16_t>(bytes[i++]) << 8;
                _cli.printHex16(val);
            }
        }
        const uint8_t codeMax = _config->codeMax();
        for (uint8_t i = length; i < codeMax;) {
            if (width == OPCODE_8BIT) {
                _cli.print(F("   "));
                i += 1;
            }
            if (width == OPCODE_16BIT) {
                _cli.print(F("     "));
                i += 2;
            }
        }
    }

    void printInsn(const Insn &insn, const char *operands) {
        const uint8_t nameMax = _config->nameMax();
        uint8_t n = _cli.print(insn.name());
        if (operands && *operands) {
            while (n++ <= nameMax)
                _cli.print(' ');
            _cli.print(operands);
        }
    }

    bool processPseudo(const char *line) {
        if (strncasecmp_P(line, PSTR("CPU "), 4) == 0) {
            const char *cpu = skipSpaces(line + 4);
            if (!setCpu(cpu))
                _cli.println(F("unknown CPU"));
            return true;
        }
        if (strncasecmp_P(line, PSTR("ORG "), 4) == 0) {
            const char *org = line + 4;
            const char *p = StrMemory::readNumber(org, &_origin);
            if (p == org)
                _cli.println(F("unknown ORG"));
            return true;
        }
        return false;
    }

    // SymbolTable
    const char *lookupValue(uint32_t value) override {
        (void)value;
        return nullptr;
    }

    bool hasSymbol(const char *symbol, const char *end) override {
        (void)symbol;
        (void)end;
        return false;
    }

    uint32_t lookupSymbol(const char *symbol, const char *end) override {
        (void)symbol;
        (void)end;
        return 0;
    }
    uint32_t currentOrigin() override { return _origin; }

    static bool isSpace(char c) { return c == ' ' || c == '\t'; }

    static const char *skipSpaces(const char *p) {
        while (isSpace(*p))
            p++;
        return p;
    }

    static const __FlashStringHelper *FSTR(const /*PROGMEM*/ char *pstr) {
        return reinterpret_cast<const __FlashStringHelper *>(pstr);
    }

private:
    ConfigBase *_config;
};

class AsmExample : public BaseExample {
public:
    AsmExample(Assembler &assembler) : BaseExample(&assembler), _assembler(assembler) {}

    void begin(Stream &console) override {
        BaseExample::begin(console);
        printPrompt();
    }

protected:
    const /*PROGMEM */ char *getCpu() const override { return _assembler.getCpu(); }
    bool setCpu(const char *cpu) override { return _assembler.setCpu(cpu); }

private:
    Assembler &_assembler;

    void assemble(const char *line) {
        Insn insn;
        if (_assembler.encode(line, insn, _origin, this)) {
            _cli.print(F("Error: "));
            _cli.println(FSTR(_assembler.errorText()));
        } else {
            printAddress(insn.address());
            _cli.print(':');
            printBytes(insn.bytes(), insn.length());
            _cli.println();
            _origin += insn.length();
        }
    }

    void printPrompt() {
        _cli.print(F("ASM:"));
        _cli.print(FSTR(_assembler.getCpu()));
        _cli.print(F("> "));
        _cli.readString(handleLine, reinterpret_cast<uintptr_t>(this));
    }

    static void handleLine(char *line, uintptr_t extra, Cli::State state) {
        (void)state;
        AsmExample *example = reinterpret_cast<AsmExample *>(extra);
        const char *scan = skipSpaces(line);
        if (*scan) {
            if (!example->processPseudo(scan))
                example->assemble(scan);
        }
        example->printPrompt();
    }
};

class DisExample : public BaseExample {
public:
    DisExample(Disassembler &disassembler)
        : BaseExample(&disassembler), _disassembler(disassembler) {
        _disassembler.setUppercase(true);
    }

    void begin(Stream &console) override {
        BaseExample::begin(console);
        printPrompt();
    }

protected:
    const /*PROGMEM*/ char *getCpu() const override { return _disassembler.getCpu(); }
    bool setCpu(const char *cpu) override { return _disassembler.setCpu(cpu); }

private:
    Disassembler &_disassembler;

    void disassemble(DisMemory &memory) {
        char operands[80];
        Insn insn;
        while (memory.hasNext()) {
            if (_disassembler.decode(memory, insn, operands, sizeof(operands), this)) {
                _cli.print(F("Error: "));
                _cli.println(FSTR(_disassembler.errorText()));
            } else {
                printAddress(insn.address());
                _cli.print(':');
                printBytes(insn.bytes(), insn.length());
                _cli.print(' ');
                printInsn(insn, operands);
                _cli.println();
                _origin += insn.length();
            }
        }
    }

    void printPrompt() {
        _cli.print(F("DIS:"));
        _cli.print(FSTR(_disassembler.getCpu()));
        _cli.print(F("> "));
        _cli.readString(handleLine, reinterpret_cast<uintptr_t>(this));
    }

    static void handleLine(char *line, uintptr_t extra, Cli::State state) {
        (void)state;
        DisExample *example = reinterpret_cast<DisExample *>(extra);
        const char *scan = skipSpaces(line);
        if (*scan) {
            if (!example->processPseudo(scan)) {
                StrMemory memory(example->_origin, scan);
                example->disassemble(memory);
            }
        }
        example->printPrompt();
    }
};

}  // namespace arduino
}  // namespace libasm

#endif  // __ARDUINO_EXAMPLE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
