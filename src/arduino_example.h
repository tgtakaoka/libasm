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
#include "config_base.h"
#include "dis_base.h"

typedef libcli::Cli::State State;

namespace libasm {
namespace arduino {

/**
 * Base class for assembler and disassembler example for Arduino.
 */
class BaseExample {
public:
    virtual void begin(Stream &console) { _cli.begin(console); }

    void loop() { _cli.loop(); }

protected:
    /* command line interface: libcli */
    libcli::Cli _cli;
    const ConfigBase &_config;
    uint32_t _origin;

    BaseExample(ConfigBase &config) : _cli(), _config(config) {}

    virtual const /*PROGMEM*/ char *prompt() const = 0;
    virtual const /*PROGMEM*/ char *getCpu() const = 0;
    virtual bool setCpu(const char *) = 0;

    uint32_t addrMax() const { return 1UL << uint8_t(_config.addressWidth()); }
    uint8_t addrUnit() const { return uint8_t(_config.addressUnit()); }

    void printPrompt(void (*handler)(char *, uintptr_t, State)) {
        _cli.print(FSTR(prompt()));
        _cli.print(FSTR(getCpu()));
        _cli.print(':');
        printAddress(_origin);
        _cli.print(F("> "));
        _cli.readString(handler, reinterpret_cast<uintptr_t>(this));
    }

    void printAddress(uint32_t addr) {
        const AddressWidth width = _config.addressWidth();
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
        const OpCodeWidth width = _config.opCodeWidth();
        const Endian endian = _config.endian();
        for (uint8_t i = 0; i < length;) {
            _cli.print(' ');
            if (width == OPCODE_8BIT) {
                _cli.printHex8(bytes[i]);
                i += 1;
            } else {  // OPCODE_16BIT
                const uint8_t hi = (endian == ENDIAN_BIG) ? 0 : 1;
                const uint8_t lo = (endian == ENDIAN_BIG) ? 1 : 0;
                uint16_t val = static_cast<uint16_t>(bytes[i + hi]) << 8;
                val |= bytes[i + lo];
                _cli.printHex16(val);
                i += 2;
            }
        }
        const uint8_t codeMax = _config.codeMax();
        for (uint8_t i = length; i < codeMax;) {
            if (width == OPCODE_8BIT) {
                _cli.print(F("   "));
                i += 1;
            } else {  // OPCODE_16BIT
                _cli.print(F("     "));
                i += 2;
            }
        }
    }

    void printInsn(const Insn &insn, const char *operands) {
        const uint8_t nameMax = _config.nameMax();
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
            uint32_t addr;
            const char *p = StrMemory::readNumber(org, &addr);
            if (p == org) {
                _cli.println(F("illegal ORG address"));
                return true;
            }
            const uint32_t max = addrMax();
            if (max && addr >= max) {
                _cli.println(F("ORG overflow range"));
                return true;
            }
            _origin = addr;
            return true;
        }
        return false;
    }

    static const char *skipSpaces(const char *p) {
        while (isSpace(*p))
            p++;
        return p;
    }

    static constexpr const __FlashStringHelper *FSTR(const /*PROGMEM*/ char *pstr) {
        return reinterpret_cast<const __FlashStringHelper *>(pstr);
    }

    /**
     * Memory interface for disassembler on string of hexadecimal number list.
     */
    class StrMemory : public DisMemory {
    public:
        StrMemory(uint32_t origin, const char *line, const ConfigBase &config)
            : DisMemory(origin), _origin(origin), _line(line), _next(line), _config(config) {}

        bool hasNext() const override { return parseNumber(_next) != _next; }
        void rewind() {
            resetAddress(_origin);
            _next = _line;
        }

        static const char *readNumber(const char *scan, uint32_t *val) {
            return parseNumber(scan, val);
        }

    protected:
        uint8_t nextByte() override {
            uint32_t val32 = 0;
            const char *p = parseNumber(_next, &val32);
            if (_config.opCodeWidth() == OPCODE_8BIT) {
                _next = p;
                return val32;
            } else {  // OPCODE_16BIT
                if (address() % 2 == 0)
                    return _config.endian() == ENDIAN_BIG ? val32 >> 8 : val32;
                _next = p;
                return _config.endian() == ENDIAN_BIG ? val32 : val32 >> 8;
            }
        }

    private:
        const uint32_t _origin;
        const char *const _line;
        const char *_next;
        const ConfigBase &_config;

        static const char *parseNumber(const char *scan, uint32_t *val = nullptr) {
            const char *p = skipSpaces(scan);
            char c;
            if ((c = *p) == 0 || !isHexadecimalDigit(c))
                return scan;  // no valid hexadecimal number found.

            uint32_t v = 0;
            while (isHexadecimalDigit(c = *p)) {
                v <<= 4;
                uint8_t n;
                if (isDigit(c)) {
                    n = c - '0';
                } else {
                    n = (c & ~0x20) - 'A' + 10;
                }
                v += n;
                p++;
            }
            if (val)
                *val = v;
            return p;
        }
    };
};

/**
 * Assembler example class.
 */
class AsmExample : public BaseExample {
public:
    AsmExample(Assembler &assembler) : BaseExample(assembler.config()), _assembler(assembler) {}

    void begin(Stream &console) override {
        BaseExample::begin(console);
        printPrompt(handleLine);
    }

protected:
    const /*PROGMEM*/ char *prompt() const override { return PSTR("ASM:"); }
    const /*PROGMEM */ char *getCpu() const override { return _assembler.getCpu(); }
    bool setCpu(const char *cpu) override { return _assembler.setCpu(cpu); }

private:
    Assembler &_assembler;

    void assemble(const char *line) {
        const uint32_t max = addrMax();
        Insn insn(_origin);
        if (_assembler.encode(line, insn)) {
            _cli.print(F("Error: "));
            _cli.println(FSTR(_assembler.errorText()));
        } else if (max && insn.address() + insn.length() / addrUnit() > max) {
            _cli.println(F("address range overflow"));
        } else {
            printAddress(insn.address());
            _cli.print(':');
            printBytes(insn.bytes(), insn.length());
            _cli.println();
            _origin += insn.length() / addrUnit();
            _origin &= max - 1;
        }
    }

    static void handleLine(char *line, uintptr_t extra, State state) {
        (void)state;
        AsmExample &example = *reinterpret_cast<AsmExample *>(extra);
        const char *scan = skipSpaces(line);
        if (*scan && !example.processPseudo(scan))
            example.assemble(scan);
        example.printPrompt(handleLine);
    }
};

/**
 * Disassembler example class.
 */
class DisExample : public BaseExample {
public:
    DisExample(Disassembler &disassembler)
        : BaseExample(disassembler.config()), _disassembler(disassembler) {
        _disassembler.setUppercase(true);
    }

    void begin(Stream &console) override {
        BaseExample::begin(console);
        printPrompt(handleLine);
    }

protected:
    const /*PROGMEM*/ char *prompt() const override { return PSTR("DIS:"); }
    const /*PROGMEM*/ char *getCpu() const override { return _disassembler.getCpu(); }
    bool setCpu(const char *cpu) override { return _disassembler.setCpu(cpu); }

private:
    Disassembler &_disassembler;

    void disassemble(StrMemory &memory) {
        const uint32_t max = addrMax();
        const uint32_t origin = memory.address();
        while (memory.hasNext()) {
            const uint32_t delta = (memory.address() - origin) / addrUnit();
            if (max && origin + delta >= max) {
                _cli.println(F("address range overflow"));
                return;
            }
            memory.readByte();
        }
        memory.rewind();

        char operands[80];
        while (memory.hasNext()) {
            const uint32_t delta = (memory.address() - origin) / addrUnit();
            Insn insn(origin + delta);
            if (_disassembler.decode(memory, insn, operands, sizeof(operands))) {
                _cli.print(F("Error: "));
                _cli.println(FSTR(_disassembler.errorText()));
            } else {
                printAddress(insn.address());
                _cli.print(':');
                printBytes(insn.bytes(), insn.length());
                _cli.print(' ');
                printInsn(insn, operands);
                _cli.println();
                _origin += insn.length() / addrUnit();
                _origin &= max - 1;
            }
        }
    }

    static void handleLine(char *line, uintptr_t extra, State state) {
        (void)state;
        DisExample &example = *reinterpret_cast<DisExample *>(extra);
        const char *scan = skipSpaces(line);
        if (*scan && !example.processPseudo(scan)) {
            StrMemory memory(example._origin, scan, example._config);
            example.disassemble(memory);
        }
        example.printPrompt(handleLine);
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
