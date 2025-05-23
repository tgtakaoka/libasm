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

#ifndef __LIBASM_ARDUINO_EXAMPLE_H__
#define __LIBASM_ARDUINO_EXAMPLE_H__

#include <libcli.h>

#include "asm_base.h"
#include "config_base.h"
#include "dis_base.h"

using State = libcli::State;

namespace libasm {
namespace arduino {

/**
 * Assembler and disassembler example for Arduino.
 */
struct Example {
    Example(Assembler &current)
        : _asm(&current),
          _dis(nullptr),
          _abegin(&_asm),
          _aend(_abegin + 1),
          _dbegin(nullptr),
          _dend(nullptr),
          _current(ASM),
          _cli() {}
    Example(Disassembler &current)
        : _asm(nullptr),
          _dis(&current),
          _abegin(nullptr),
          _aend(nullptr),
          _dbegin(&_dis),
          _dend(_dbegin + 1),
          _current(DIS),
          _cli() {}
    Example(Assembler **abegin, Assembler **aend)
        : _asm(*abegin),
          _dis(nullptr),
          _abegin(abegin),
          _aend(aend),
          _dbegin(nullptr),
          _dend(nullptr),
          _current(ASM),
          _cli() {}
    Example(Disassembler **dbegin, Disassembler **dend)
        : _asm(nullptr),
          _dis(*dbegin),
          _abegin(nullptr),
          _aend(nullptr),
          _dbegin(dbegin),
          _dend(dend),
          _current(DIS),
          _cli() {}
    Example(Assembler &assembler, Disassembler &disassembler)
        : _asm(&assembler),
          _dis(&disassembler),
          _abegin(&_asm),
          _aend(_abegin + 1),
          _dbegin(&_dis),
          _dend(_dbegin + 1),
          _current(ASM),
          _cli() {}
    Example(Assembler **abegin, Assembler **aend, Disassembler **dbegin, Disassembler **dend)
        : _asm(*abegin),
          _dis(*dbegin),
          _abegin(abegin),
          _aend(aend),
          _dbegin(dbegin),
          _dend(dend),
          _current(ASM),
          _cli() {}

    void begin(Stream &console) {
        _cli.begin(console);
        printPrompt();
    }

    void loop() { _cli.loop(); }

protected:
    Assembler *_asm;
    Disassembler *_dis;

    bool isAsm() const { return _current == ASM; }
    const char *cpu_P() const { return isAsm() ? _asm->config().cpu_P() : _dis->config().cpu_P(); }

    virtual bool processPseudo(const char *line) {
        if (strcasecmp_P(line, PSTR("ASM")) == 0) {
            if (_abegin) {
                _current = ASM;
            } else {
                _cli.println(F("No assembler"));
            }
            return true;
        }
        if (strcasecmp_P(line, PSTR("DIS")) == 0) {
            if (_dbegin) {
                _current = DIS;
            } else {
                _cli.println(F("No disassembler"));
            }
            return true;
        }
        if (strcasecmp_P(line, PSTR("CPU")) == 0) {
            const auto cpu = skipSpaces(line + 3);
            if (*cpu == 0)
                return isAsm() ? printCpuList<>(_abegin, _aend) : printCpuList<>(_dbegin, _dend);
            if (isAsm() ? _asm->setCpu(cpu) : _dis->setCpu(cpu))
                return true;
            if (isAsm()) {
                auto *a = searchCpu<>(cpu, _abegin, _aend);
                if (a) {
                    _asm = a;
                    return true;
                }
            } else {
                auto *d = searchCpu<>(cpu, _dbegin, _dend);
                if (d) {
                    _dis = d;
                    return true;
                }
            }
            _cli.println(F("unknown CPU"));
            return true;
        }
        if (!isAsm() && strncasecmp_P(line, PSTR("ORG"), 3) == 0) {
            const auto org = skipSpaces(line + 3);
            if (*org) {
                uint32_t addr = 0;
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

private:
    Assembler **_abegin;
    Assembler **_aend;
    Disassembler **_dbegin;
    Disassembler **_dend;
    enum Prog : uint8_t { ASM, DIS } _current;
    /* command line interface: libcli */
    libcli::Cli _cli;
    uint32_t _origin;
    char buffer[80];

    const ConfigBase &config() const { return isAsm() ? _asm->config() : _dis->config(); }
    uint32_t addrMax() const { return 1UL << uint8_t(config().addressWidth()); }
    uint8_t addrUnit() const { return uint8_t(config().addressUnit()); }
    Radix listRadix() const { return isAsm() ? _asm->listRadix() : _dis->listRadix(); }
    uint8_t numDigits(uint8_t bits) const {
        const auto digit = (listRadix() == 8) ? 3 : 4;
        return bits / digit + ((bits % digit) == 0 ? 0 : 1);
    }
    uint8_t addrDigits() const { return numDigits(config().addressWidth()); }

    static void handleLine(char *line, uintptr_t extra, State state) {
        (void)state;
        reinterpret_cast<Example *>(extra)->processLine(line, state);
    }

    void processLine(char *line, State state) {
        (void)state;
        _cli.println();
        auto *scan = skipSpaces(line);
        if (*scan && !processPseudo(line)) {
            if (isAsm()) {
                assemble(_asm, scan);
            } else {
                disassemble(_dis, scan);
            }
        }
        printPrompt();
    }

    void printPrompt() {
        _cli.print(isAsm() ? F("ASM:") : F("DIS:"));
        _cli.print(FSTR(cpu_P()));
        _cli.print(':');
        printAddress(_origin, '>');
        _cli.readLine(handleLine, reinterpret_cast<uintptr_t>(this), buffer, sizeof(buffer));
    }

    void printAddress(uint32_t addr, char suffix = 0) {
        _cli.printNum(addr, listRadix(), addrDigits());
        if (suffix)
            _cli.print(suffix);
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

    uint8_t printData(const uint8_t *bytes, uint8_t pos, uint8_t max) {
        _cli.print(' ');
        const auto bits = config().opCodeWidth();
        const auto digits = numDigits(bits);
        if (pos < max) {
            uint16_t val = bytes[pos];
            if (bits > OPCODE_8BIT) {
                const uint16_t next = bytes[pos + 1];
                if (config().endian() == ENDIAN_BIG) {
                    val = (val << 8) | next;
                } else {
                    val |= (next << 8);
                }
            }
            _cli.printNum(val, listRadix(), digits);
        } else {
            for (auto i = 0; i < digits; i++)
                _cli.print(' ');
        }
        return bits == OPCODE_8BIT ? 1 : 2;
    }

    void printBytes(const uint8_t *bytes, uint8_t length) {
        for (auto i = 0; i < length;) {
            i += printData(bytes, i, length);
        }
    }

    void printBytes(const Insn &insn, const char *operands) {
        constexpr auto chunk = 6;
        const auto codeMax = config().codeMax();
        const auto step = codeMax < chunk ? codeMax : chunk;
        for (auto i = 0; i < insn.length(); i += step) {
            printAddress(insn.address() + i / addrUnit(), ':');
            auto len = insn.length() - i;
            if (len >= step)
                len = step;
            printBytes(insn.bytes() + i, len);
            if (i == 0) {
                _cli.print(F("  "));
                _cli.printStr(insn.name(), -config().nameMax());
                _cli.printStr(operands);
            }
            _cli.println();
        }
    }

    void assemble(Assembler *assembler, const char *line) {
        const auto max = addrMax();
        Insn insn(_origin);
        if (assembler->encode(line, insn)) {
            _cli.print(F("Error: "));
            _cli.println(FSTR(insn.errorText_P()));
        } else if (max && insn.address() + insn.length() / addrUnit() > max) {
            _cli.println(F("address range overflow"));
        } else {
            printAddress(insn.address(), ':');
            printBytes(insn.bytes(), insn.length());
            _cli.println();
            if (insn.address() == _origin) {
                _origin += insn.length() / addrUnit();
                _origin &= max;
            } else {
                _origin = insn.address();
            }
        }
    }

    void disassemble(Disassembler *disassembler, char *line) {
        StrMemory memory(_origin, line, config());
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
            if (disassembler->decode(memory, insn, operands, sizeof(operands))) {
                _cli.print(F("Error: "));
                _cli.println(FSTR(insn.errorText_P()));
            } else {
                printBytes(insn, operands);
                _origin += insn.length() / addrUnit();
                _origin &= max - 1;
            }
        }
    }

    template <typename Prog>
    bool printCpuList(Prog **begin, Prog **end) {
        for (auto p = begin; p < end; p++) {
            if (p != begin)
                _cli.print(F(", "));
            _cli.print(FSTR((*p)->config().listCpu_P()));
        }
        _cli.println();
        return true;
    }

    template <typename Prog>
    Prog *searchCpu(const char *cpu, Prog **begin, Prog **end) {
        for (auto p = begin; p < end; p++) {
            if ((*p)->setCpu(cpu))
                return *p;
        }
        return nullptr;
    }

    static const char *skipSpaces(const char *p) {
        while (isSpace(*p))
            p++;
        return p;
    }

    static char *skipSpaces(char *p) {
        while (isSpace(*p))
            p++;
        return p;
    }

    static const __FlashStringHelper *FSTR(const /*PROGMEM*/ char *pstr) {
        return reinterpret_cast<const __FlashStringHelper *>(pstr);
    }

    /**
     * Memory interface for disassmbeler on string of hexadecimal number list.
     */
    struct StrMemory : DisMemory {
        StrMemory(uint32_t origin, const char *line, const ConfigBase &config)
            : DisMemory(origin),
              _origin(origin),
              _line(line),
              _next(line),
              _config(config),
              _odd(false) {}

        bool hasNext() const override { return parseNumber(_next) != _next; }

        void rewind() {
            resetAddress(_origin);
            _next = _line;
            _odd = false;
        }

        static const char *readNumber(const char *scan, uint32_t *val) {
            return parseNumber(scan, val);
        }

    protected:
        uint8_t nextByte() override {
            uint32_t val32 = 0;
            const auto *p = parseNumber(_next, &val32);
            if (_config.opCodeWidth() == OPCODE_8BIT) {
                _next = p;
                return val32;
            }
            // OPCODE_16BIT
            const uint8_t hi = val32 >> 8;
            const uint8_t lo = val32;
            const uint8_t val =
                    (_config.endian() == ENDIAN_BIG) ? (_odd ? lo : hi) : (_odd ? hi : lo);
            if (_odd)
                _next = p;
            _odd = !_odd;
            return val;
        }

    private:
        const uint32_t _origin;
        const char *const _line;
        const char *_next;
        const ConfigBase &_config;
        bool _odd;

        static const char *parseNumber(const char *scan, uint32_t *val = nullptr) {
            auto p = skipSpaces(scan);
            if (*p == 0)
                return scan;
            const bool octal = (*p == '@');
            if (octal) {
                p++;
                if (*p < '0' || *p >= '8')
                    return scan;  // no valid octal number found.
            } else if (!isHexadecimalDigit(*p)) {
                return scan;  // no valid hexadecimal number found.
            }

            uint32_t v = 0;
            char c;
            while (isHexadecimalDigit(c = *p)) {
                if (octal) {
                    if (c >= '8')
                        break;
                    v <<= 3;
                } else {
                    v <<= 4;
                }
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

}  // namespace arduino
}  // namespace libasm

#endif  // __LIBASM_ARDUINO_EXAMPLE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
