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

#include "dis_driver.h"

#include "file_printer.h"
#include "file_reader.h"

#include <string.h>

#include <algorithm>
#include <list>
#include <string>

namespace libasm {
namespace cli {

DisDriver::DisDriver(Disassembler **begin, Disassembler **end) {
    _disassemblers.insert(_disassemblers.end(), begin, end);
}

DisDriver::~DisDriver() {
    if (_formatter)
        delete _formatter;
}

static void appendTo(const std::string &cpu, std::list<std::string> &list) {
    if (std::find(list.begin(), list.end(), cpu) == list.end())
        list.push_back(cpu);
}

static void filter(const char *text, std::list<std::string> &list) {
    while (*text) {
        const char *del = strchr(text, ',');
        if (del == nullptr) {
            appendTo(std::string(text), list);
            return;
        }
        const std::string cpu(text, del);
        appendTo(cpu, list);
        for (text = del + 1; *text == ' '; text++)
            ;
    }
}

static std::string listCpu(const char *separator, const std::list<Disassembler *> &disassemblers) {
    std::list<std::string> list;
    for (auto dis : disassemblers) {
        const /* PROGMEM */ char *list_P = dis->listCpu_P();
        char listCpu[strlen_P(list_P) + 1];
        strcpy_P(listCpu, list_P);
        filter(listCpu, list);
    }
    std::string cpuList;
    std::string buf;
    for (auto &cpu : list) {
        if (buf.size())
            buf += " ";
        if (buf.size() + cpu.size() < 60) {
            buf += cpu;
        } else {
            cpuList += separator;
            cpuList += buf;
            buf = cpu;
        }
    }
    cpuList += separator + buf;
    return cpuList;
}

int DisDriver::usage() {
    const char *cpuOption = "";
    std::string cpuList;
    Disassembler *dis = defaultDisassembler();
    if (dis) {
        const /* PROGMEM */ auto list_P = dis->listCpu_P();
        char listCpu[strlen_P(list_P) + 1];
        strcpy(listCpu, list_P);
        cpuList = ": ";
        cpuList += listCpu;
    } else {
        cpuOption = " -C <CPU>";
        const char *cpuSep = "\n                ";
        cpuList = listCpu(cpuSep, _disassemblers);
    }
    fprintf(stderr,
            "libasm disassembler (version " LIBASM_VERSION_STRING
            ")\n"
            "usage: %s%s [-o <output>] [-l <list>] <input>\n"
            "  -C <CPU>    : target CPU%s\n"
            "  -o <output> : output file\n"
            "  -l <list>   : list file\n"
            "  <input>     : file can be Motorola S-Record or Intel HEX format\n"
            "  -A start[,end]\n"
            "              : disassemble start address and optional end address\n"
            "  -r          : use program counter relative notation\n"
            "  -u          : use uppercase letter for output\n"
            "  -v          : print progress verbosely\n",
            _progname, cpuOption, cpuList.c_str());
    return 2;
}

int DisDriver::disassemble() {
    if (_verbose)
        fprintf(stderr, "libasm disassembler (version " LIBASM_VERSION_STRING ")\n");
    FileReader input(_input_name);
    if (!input.open()) {
        fprintf(stderr, "Can't open input file %s\n", _input_name);
        return 1;
    }
    BinMemory memory;
    if (readInput(input, memory) != 0)
        return 1;
    input.close();
    const uint8_t addrUnit = static_cast<uint8_t>(_disassembler->config().addressUnit());
    const uint32_t mem_start = memory.startAddress() / addrUnit;
    const uint32_t mem_end = memory.endAddress() / addrUnit;
    if ((_addr_start != 0 || _addr_end != UINT32_MAX) &&
            (mem_end < _addr_start || mem_start > _addr_end)) {
        fprintf(stderr, "Input file has address range: 0x%04X,0x%04X\n", mem_start, mem_end);
        fprintf(stderr, "-A range has no intersection: 0x%04X,0x%04X\n", _addr_start, _addr_end);
        return 1;
    }

    _disassembler->setRelativeTarget(_relativeTarget);
    DisDirective listing(*_disassembler, memory, _uppercase);
    FilePrinter output;
    if (_output_name) {
        if (!output.open(_output_name)) {
            fprintf(stderr, "Can't open output file %s\n", _output_name);
            return 1;
        }
        if (_verbose)
            fprintf(stderr, "%s: Opened for output\n", _output_name);
        output.println(listing.getCpu());
    }
    FilePrinter listout;
    if (_list_name) {
        if (!listout.open(_list_name)) {
            fprintf(stderr, "Can't open list file %s\n", _list_name);
            return 1;
        }
        if (_verbose)
            fprintf(stderr, "%s: Opened for listing\n", _list_name);
        listout.println(listing.getCpu(true));
    }
    for (const auto &it : memory) {
        auto base = it.first;
        auto size = it.second.size();
        const uint8_t addrUnit = static_cast<uint8_t>(_disassembler->config().addressUnit());
        uint32_t start = base / addrUnit;
        const uint32_t end = start + (size - 1) / addrUnit;
        if (base > _addr_end || end < _addr_start)
            continue;
        if (base < _addr_start) {
            size -= (_addr_start - base) * addrUnit;
            base = _addr_start;
            start = base / addrUnit;
        }
        if (end > _addr_end)
            size -= (end - _addr_end) * addrUnit;
        listout.println(listing.origin(start, true));
        output.println(listing.origin(start));
        for (size_t pc = 0; pc < size;) {
            const uint32_t address = start + pc / addrUnit;
            Insn insn(address);
            listing.disassemble(base + pc, insn);
            const Error error = _disassembler->getError();
            pc += insn.length();
            if (error)
                fprintf(stderr, "%s:0x%04x: error: %s\n", _input_name, insn.address(),
                        _disassembler->errorText_P());
            if (error)
                listout.format("%s:0x%04x: error: %s\n", _input_name, insn.address(),
                        _disassembler->errorText_P());
            do {
                listout.println(listing.getLine());
            } while (listing.hasNext());
            if (error)
                output.format("; %s:0x%04x: error: %s\n", _input_name, insn.address(),
                        _disassembler->errorText_P());
            do {
                output.println(listing.getContent());
            } while (listing.hasNext());
        }
    };

    return 0;
}

int DisDriver::readInput(FileReader &input, BinMemory &memory) {
    const char *filename = input.name().c_str();
    int lineno = 0;
    int errors = 0;
    const uint8_t addrUnit = static_cast<uint8_t>(_disassembler->config().addressUnit());
    uint32_t start = 0, end = 0;
    StrScanner *line;
    while ((line = input.readLine()) != nullptr) {
        lineno++;
        uint32_t addr;
        uint8_t size;
        uint8_t *data = _formatter->decode(*line, addr, size);
        if (data == nullptr) {
            if (errors < 3) {
                fprintf(stderr, "%s:%d: format error: %s\n", filename, lineno,
                        (const char *)(*line));
            } else if (errors == 3) {
                fprintf(stderr, "%s: too many errors, wrong format?\n", filename);
            }
            errors++;
            continue;
        }
        if (size) {
            if (start == end)
                start = end = addr;
            if (addr != end) {
                if (_verbose)
                    fprintf(stderr, "%s: Read %4d bytes 0x%04X,0x%04X\n", filename, end - start,
                            start / addrUnit, (end - 1) / addrUnit);
                start = addr;
            }
            end = addr + size;
            memory.writeBytes(addr, data, size);
        }
    }
    if (start != end && _verbose)
        fprintf(stderr, "%s: Read %4d bytes 0x%04X,0x%04X\n", filename, end - start,
                start / addrUnit, (end - 1) / addrUnit);
    return errors;
}

BinFormatter *DisDriver::determineInputFormat(const char *input_name) const {
    FileReader input(input_name);
    if (!input.open()) {
        fprintf(stderr, "Can't open input file %s\n", input_name);
        return nullptr;
    }
    StrScanner *line = input.readLine();
    const auto c = **line;
    input.close();

    const AddressWidth addrWidth = _disassembler->config().addressWidth();
    if (c == 'S')
        return new MotoSrec(addrWidth);
    if (c == ':')
        return new IntelHex(addrWidth);
    return nullptr;
}

Disassembler *DisDriver::defaultDisassembler() {
    const size_t prefix_len = strlen(PROG_PREFIX);
    if (_progname && strncmp(_progname, PROG_PREFIX, prefix_len) == 0) {
        const char *cpu = _progname + prefix_len;
        for (auto dis : _disassemblers) {
            if (dis->setCpu(cpu)) {
                _disassemblers.clear();
                _disassemblers.push_back(dis);
                return dis;
            }
        }
    }
    return nullptr;
}

int DisDriver::parseOption(int argc, const char **argv) {
    _progname = basename(argv[0]);
    _relativeTarget = false;
    _uppercase = false;
    _verbose = false;
    _addr_start = 0;
    _addr_end = UINT32_MAX;
    _input_name = nullptr;
    _output_name = nullptr;
    _list_name = nullptr;
    _disassembler = defaultDisassembler();
    _formatter = nullptr;
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
            case 'C': {
                if (++i >= argc) {
                    fprintf(stderr, "-C requires CPU name\n");
                    return 1;
                }
                Disassembler *disassembler = nullptr;
                for (auto dis : _disassemblers) {
                    if (dis->setCpu(argv[i])) {
                        disassembler = dis;
                        break;
                    }
                }
                if (disassembler == nullptr) {
                    fprintf(stderr, "unknown CPU '%s'\n", argv[i]);
                    return 4;
                }
                _disassembler = disassembler;
                break;
            }
            case 'r':
                _relativeTarget = true;
                break;
            case 'u':
                _uppercase = true;
                break;
            case 'v':
                _verbose = true;
                break;
            case 'A':
                if (++i >= argc) {
                    fprintf(stderr, "-A requires start[,end] address\n");
                    return 1;
                } else {
                    char *end_start;
                    _addr_start = strtoul(argv[i], &end_start, 0);
                    if (end_start == argv[i]) {
                    format_error:
                        fprintf(stderr, "invalid address format for -A: %s\n", argv[i]);
                        return 1;
                    }
                    if (*end_start == 0)
                        break;
                    if (*end_start++ != ',') {
                    form_error:
                        fprintf(stderr, "address must be start[,end] form: %s\n", argv[i]);
                        return 1;
                    }
                    char *end_end;
                    _addr_end = strtoul(end_start, &end_end, 0);
                    if (end_end == end_start)
                        goto format_error;
                    if (*end_end)
                        goto form_error;
                    if (_addr_end < _addr_start) {
                        fprintf(stderr, "end address must be less than start: %s\n", argv[i]);
                        return 1;
                    }
                }
                break;
            default:
                fprintf(stderr, "unknown option: %s\n", opt);
                return 1;
            }
        } else {
            if (_input_name) {
                fprintf(stderr, "multiple input files specified: %s and %s\n", _input_name, opt);
                return 1;
            }
            _input_name = opt;
        }
    }
    if (!_input_name) {
        fprintf(stderr, "no input file\n");
        return 1;
    }
    if (_disassembler == nullptr) {
        fprintf(stderr, "no target CPU specified\n");
        return 1;
    }
    _formatter = determineInputFormat(_input_name);
    if (_formatter == nullptr) {
        fprintf(stderr, "Can't determine format of input file %s\n", _input_name);
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

const char *DisDriver::basename(const char *str, char sep_char) {
    const char *sep = strrchr(str, sep_char);
    return sep ? sep + 1 : str;
}

}  // namespace cli
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
