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
template <typename Base>
class BaseExample {
public:
    virtual void begin(Stream &console) { _cli.begin(console); }

    void loop() { _cli.loop(); }

protected:
    /* command line interface: libcli */
    libcli::Cli &_cli;
    Base *_current;
    Base ** _begin;
    Base ** _end;
    uint32_t _origin;

    BaseExample(Base &current)
        : _cli(libcli::Cli::instance()), _current(&current), _begin(&_current), _end(_begin + 1) {}
    BaseExample(Base **begin, Base **end)
        : _cli(libcli::Cli::instance()), _current(*begin), _begin(begin), _end(end) {}

    virtual const /*PROGMEM*/ char *prompt() const = 0;

    const ConfigBase &config() const { return _current->config(); }
    uint32_t addrMax() const { return 1UL << uint8_t(config().addressWidth()); }
    uint8_t addrUnit() const { return uint8_t(config().addressUnit()); }
    uint8_t addrDigits() const { return ((uint8_t(config().addressWidth()) + 3) & -4) / 4; }

    void printPrompt(void (*callback)(const char *, uintptr_t, State)) {
        _cli.print(FSTR(prompt()));
        _cli.print(FSTR(_current->getCpu()));
        _cli.print(':');
        printAddress(_origin);
        _cli.print(F("> "));
        _cli.readLine(callback, reinterpret_cast<uintptr_t>(this));
    }

    void printAddress(uint32_t addr) { _cli.printHex(addr, addrDigits()); }

    void printBytes(const uint8_t *bytes, uint8_t length) {
        const auto width = config().opCodeWidth();
        for (auto i = 0; i < length;) {
            _cli.print(' ');
            if (width == OPCODE_8BIT) {
                _cli.printHex(bytes[i], 2);
                i += 1;
            } else {  // OPCODE_16BIT
                const auto hi = uint8_t(config().endian());
                const auto lo = 1 - hi;
                const auto val = (static_cast<uint16_t>(bytes[i + hi]) << 8) | bytes[i + lo];
                _cli.printHex(val, 4);
                i += 2;
            }
        }
        const auto codeMax = config().codeMax();
        for (auto i = length; i < codeMax;) {
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
        const auto nameMax = config().nameMax();
        auto n = _cli.print(insn.name());
        if (operands && *operands) {
            while (n++ <= nameMax)
                _cli.print(' ');
            _cli.print(operands);
        }
    }

    bool processPseudo(const char *line) {
        if (strcasecmp_P(line, PSTR("CPU")) == 0 || strncasecmp_P(line, PSTR("CPU "), 4) == 0) {
            const auto cpu = skipSpaces(line + 3);
            if (*cpu == 0) {
                for (auto p = _begin; p < _end; p++) {
                    if (p != _begin)
                        _cli.print(F(", "));
                    _cli.print(FSTR((*p)->listCpu()));
                }
                _cli.println();
                return true;
            }
            if (_current->setCpu(cpu))
                return true;
            for (auto p = _begin; p < _end; p++) {
                if ((*p)->setCpu(cpu)) {
                    _current = *p;
                    return true;
                }
            }
            _cli.println(F("unknown CPU"));
            return true;
        }
        if (strcasecmp_P(line, PSTR("ORG")) == 0 || strncasecmp_P(line, PSTR("ORG "), 4) == 0) {
            const auto org = skipSpaces(line + 3);
            if (*org) {
                uint32_t addr;
                const auto p = StrMemory::readNumber(org, &addr);
                if (p == org) {
                    _cli.println(F("illegal ORG address"));
                    return true;
                }
                const auto max = addrMax();
                if (max && addr >= max) {
                    _cli.println(F("ORG overflow range"));
                    return true;
                }
                _origin = addr;
            }
            printAddress(_origin);
            _cli.println();
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
            const auto p = parseNumber(_next, &val32);
            if (_config.opCodeWidth() == OPCODE_8BIT) {
                _next = p;
                return val32;
            } else {  // OPCODE_16BIT
                const uint8_t hi = val32 >> 8;
                const uint8_t lo = val32;
                if (address() % 2 == 0)
                    return _config.endian() == ENDIAN_BIG ? hi : lo;
                _next = p;
                return _config.endian() == ENDIAN_BIG ? lo : hi;
            }
        }

    private:
        const uint32_t _origin;
        const char *const _line;
        const char *_next;
        const ConfigBase &_config;

        static const char *parseNumber(const char *scan, uint32_t *val = nullptr) {
            auto p = skipSpaces(scan);
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
class AsmExample : public BaseExample<Assembler> {
public:
    AsmExample(Assembler &assembler) : BaseExample(assembler) {}
    AsmExample(Assembler **begin, Assembler **end) : BaseExample(begin, end) {}

    void begin(Stream &console) override {
        BaseExample::begin(console);
        printPrompt(handleLine);
    }

protected:
    const /*PROGMEM*/ char *prompt() const override { return PSTR("ASM:"); }

private:
    void assemble(const char *line) {
        const auto max = addrMax();
        Insn insn(_origin);
        if (_current->encode(line, insn)) {
            _cli.print(F("Error: "));
            _cli.println(FSTR(_current->errorText()));
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

    static void handleLine(const char *line, uintptr_t extra, State state) {
        (void)state;
        auto example = reinterpret_cast<AsmExample *>(extra);
        const auto scan = skipSpaces(line);
        if (*scan && !example->processPseudo(scan))
            example->assemble(scan);
        example->printPrompt(handleLine);
    }
};

/**
 * Disassembler example class.
 */
class DisExample : public BaseExample<Disassembler> {
public:
    DisExample(Disassembler &disassembler) : BaseExample(disassembler) {
        _current->setUppercase(true);
    }
    DisExample(Disassembler **begin, Disassembler **end) : BaseExample(begin, end) {
        _current->setUppercase(true);
    }

    void begin(Stream &console) override {
        BaseExample::begin(console);
        printPrompt(handleLine);
    }

protected:
    const /*PROGMEM*/ char *prompt() const override { return PSTR("DIS:"); }

private:
    void disassemble(StrMemory &memory) {
        const auto max = addrMax();
        const auto origin = memory.address();
        while (memory.hasNext()) {
            const auto delta = (memory.address() - origin) / addrUnit();
            if (max && origin + delta >= max) {
                _cli.println(F("address range overflow"));
                return;
            }
            memory.readByte();
        }
        memory.rewind();

        char operands[80];
        while (memory.hasNext()) {
            const auto delta = (memory.address() - origin) / addrUnit();
            Insn insn(origin + delta);
            if (_current->decode(memory, insn, operands, sizeof(operands))) {
                _cli.print(F("Error: "));
                _cli.println(FSTR(_current->errorText()));
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

    static void handleLine(const char *line, uintptr_t extra, State state) {
        (void)state;
        auto example = reinterpret_cast<DisExample *>(extra);
        const auto scan = skipSpaces(line);
        if (*scan && !example->processPseudo(scan)) {
            StrMemory memory(example->_origin, scan, example->config());
            example->disassemble(memory);
        }
        example->printPrompt(handleLine);
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
