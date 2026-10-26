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

#ifndef __LIBASM_PIO_EXAMPLE_H__
#define __LIBASM_PIO_EXAMPLE_H__

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "asm_base.h"
#include "config_base.h"
#include "dis_base.h"

namespace libasm {
namespace pio {

/**
 * Interactive assembler and disassembler example on standard input/output.
 */
struct Example {
    Example(Assembler &current)
        : _asm(&current),
          _dis(nullptr),
          _abegin(&_asm),
          _aend(_abegin + 1),
          _dbegin(nullptr),
          _dend(nullptr),
          _current(ASM) {}
    Example(Disassembler &current)
        : _asm(nullptr),
          _dis(&current),
          _abegin(nullptr),
          _aend(nullptr),
          _dbegin(&_dis),
          _dend(_dbegin + 1),
          _current(DIS) {}
    Example(Assembler **abegin, Assembler **aend)
        : _asm(*abegin),
          _dis(nullptr),
          _abegin(abegin),
          _aend(aend),
          _dbegin(nullptr),
          _dend(nullptr),
          _current(ASM) {}
    Example(Disassembler **dbegin, Disassembler **dend)
        : _asm(nullptr),
          _dis(*dbegin),
          _abegin(nullptr),
          _aend(nullptr),
          _dbegin(dbegin),
          _dend(dend),
          _current(DIS) {}
    Example(Assembler &assembler, Disassembler &disassembler)
        : _asm(&assembler),
          _dis(&disassembler),
          _abegin(&_asm),
          _aend(_abegin + 1),
          _dbegin(&_dis),
          _dend(_dbegin + 1),
          _current(ASM) {}
    Example(Assembler **abegin, Assembler **aend, Disassembler **dbegin, Disassembler **dend)
        : _asm(*abegin),
          _dis(*dbegin),
          _abegin(abegin),
          _aend(aend),
          _dbegin(dbegin),
          _dend(dend),
          _current(ASM) {}

    int run() {
        char line[80];
        printPrompt();
        while (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\r\n")] = 0;
            processLine(line);
            printPrompt();
        }
        printf("\n");
        return 0;
    }

protected:
    Assembler *_asm;
    Disassembler *_dis;

    bool isAsm() const { return _current == ASM; }
    const char *cpu_P() const { return isAsm() ? _asm->config().cpu_P() : _dis->config().cpu_P(); }

    virtual bool processPseudo(const char *line) {
        if (strcasecmp(line, "ASM") == 0) {
            if (_abegin) {
                _current = ASM;
            } else {
                printf("No assembler\n");
            }
            return true;
        }
        if (strcasecmp(line, "DIS") == 0) {
            if (_dbegin) {
                _current = DIS;
            } else {
                printf("No disassembler\n");
            }
            return true;
        }
        if (isCommand(line, "CPU")) {
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
            printf("unknown CPU\n");
            return true;
        }
        if (!isAsm() && isCommand(line, "ORG")) {
            const auto org = skipSpaces(line + 3);
            if (*org) {
                uint32_t addr = 0;
                const auto p = StrMemory::readNumber(org, &addr);
                if (p == org) {
                    printf("illegal ORG address\n");
                    return true;
                }
                if (addr > addrMask()) {
                    printf("ORG overflow range\n");
                    return true;
                }
                _origin = addr;
            }
            printAddress(_origin);
            printf("\n");
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
    uint32_t _origin = 0;

    const ConfigBase &config() const { return isAsm() ? _asm->config() : _dis->config(); }
    uint32_t addrMask() const { return UINT32_MAX >> (32 - uint8_t(config().addressWidth())); }
    uint8_t addrUnit() const { return uint8_t(config().addressUnit()); }
    Radix listRadix() const { return isAsm() ? _asm->listRadix() : _dis->listRadix(); }
    int numDigits(uint8_t bits) const {
        const auto digit = (listRadix() == RADIX_8) ? 3 : 4;
        return bits / digit + ((bits % digit) == 0 ? 0 : 1);
    }

    void processLine(char *line) {
        auto *scan = skipSpaces(line);
        if (*scan && !processPseudo(scan)) {
            if (isAsm()) {
                assemble(_asm, scan);
            } else {
                disassemble(_dis, scan);
            }
        }
    }

    void printPrompt() {
        printf("%s:%s:", isAsm() ? "ASM" : "DIS", cpu_P());
        printAddress(_origin, '>');
        fflush(stdout);
    }

    void printNum(uint32_t val, int digits) const {
        printf(listRadix() == RADIX_8 ? "%0*o" : "%0*X", digits, val);
    }

    void printAddress(uint32_t addr, char suffix = 0) const {
        printNum(addr, numDigits(uint8_t(config().addressWidth())));
        if (suffix)
            printf("%c", suffix);
    }

    uint8_t printData(const uint8_t *bytes, uint8_t pos, uint8_t max) const {
        printf(" ");
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
            printNum(val, digits);
        } else {
            printf("%*s", digits, "");
        }
        return bits == OPCODE_8BIT ? 1 : 2;
    }

    void printBytes(const uint8_t *bytes, uint8_t length) const {
        for (auto i = 0; i < length;) {
            i += printData(bytes, i, length);
        }
    }

    void printBytes(const Insn &insn, const char *operands) const {
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
                printf("  %s", insn.name());
                if (*operands) {
                    const int pad = config().nameMax() - int(strlen(insn.name()));
                    printf("%*s%s", pad > 0 ? pad : 1, "", operands);
                }
            }
            printf("\n");
        }
    }

    void assemble(Assembler *assembler, const char *line) {
        const auto mask = addrMask();
        Insn insn(_origin);
        if (assembler->encode(line, insn)) {
            printf("Error: %s\n", insn.errorText_P());
        } else if (insn.length() && insn.address() + (insn.length() - 1) / addrUnit() > mask) {
            printf("address range overflow\n");
        } else {
            printAddress(insn.address(), ':');
            printBytes(insn.bytes(), insn.length());
            printf("\n");
            if (insn.address() == _origin) {
                _origin += insn.length() / addrUnit();
                _origin &= mask;
            } else {
                _origin = insn.address();
            }
        }
    }

    void disassemble(Disassembler *disassembler, const char *line) {
        StrMemory memory(_origin, line, config());
        const auto mask = addrMask();
        const auto origin = memory.address();
        while (memory.hasNext()) {
            const auto delta = (memory.address() - origin) / addrUnit();
            if (origin + delta > mask) {
                printf("address range overflow\n");
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
                printf("Error: %s\n", insn.errorText_P());
            } else {
                printBytes(insn, operands);
                _origin += insn.length() / addrUnit();
                _origin &= mask;
            }
        }
    }

    template <typename Prog>
    bool printCpuList(Prog **begin, Prog **end) const {
        for (auto p = begin; p < end; p++)
            printf("%s%s", p == begin ? "" : ", ", (*p)->config().listCpu_P());
        printf("\n");
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

    static bool isCommand(const char *line, const char *cmd) {
        const auto len = strlen(cmd);
        return strncasecmp(line, cmd, len) == 0 && (line[len] == 0 || isspace(line[len]));
    }

    static const char *skipSpaces(const char *p) {
        while (isspace(*p))
            p++;
        return p;
    }

    static char *skipSpaces(char *p) {
        while (isspace(*p))
            p++;
        return p;
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
            } else if (!isxdigit(*p)) {
                return scan;  // no valid hexadecimal number found.
            }

            uint32_t v = 0;
            char c;
            while (isxdigit(c = *p)) {
                if (octal) {
                    if (c >= '8')
                        break;
                    v <<= 3;
                } else {
                    v <<= 4;
                }
                uint8_t n;
                if (isdigit(c)) {
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

}  // namespace pio
}  // namespace libasm

#endif  // __LIBASM_PIO_EXAMPLE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
