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

#include "dis_commander.h"

#include "bin_decoder.h"
#include "dis_formatter.h"
#include "file_printer.h"
#include "file_reader.h"

#include <string.h>

#include <algorithm>
#include <string>

namespace libasm {
namespace cli {

DisCommander::DisCommander(Disassembler **begin, Disassembler **end)
    : _driver(begin, end), _args() {}

int DisCommander::disassemble() {
    if (_args.verbose)
        fprintf(stderr, "libasm disassembler (version " LIBASM_VERSION_STRING ")\n");
    if (defaultDisassembler() == nullptr && _args.cpu.empty()) {
        fprintf(stderr, "No target CPU specified\n");
        return 1;
    }
    if (_driver.setCpu(_args.cpu.c_str()) == nullptr) {
        fprintf(stderr, "Unknown target CPU %s\n", _args.cpu.c_str());
        return 1;
    }

    BinMemory memory;
    FileReader input(_args.input_name);
    if (!input.open()) {
        fprintf(stderr, "Can't open input file %s\n", _args.input_name.c_str());
        return 1;
    }
    if (readBinary(input, memory) != 0)
        return 1;
    input.close();

    const uint8_t addrUnit = static_cast<uint8_t>(_driver.current()->config().addressUnit());
    const uint32_t mem_start = memory.startAddress() / addrUnit;
    const uint32_t mem_end = memory.endAddress() / addrUnit;
    if ((_args.addr_start != 0 || _args.addr_end != UINT32_MAX) &&
            (mem_end < _args.addr_start || mem_start > _args.addr_end)) {
        fprintf(stderr, "Input file has address range: 0x%04X,0x%04X\n", mem_start, mem_end);
        fprintf(stderr, "-A range has no intersection: 0x%04X,0x%04X\n", _args.addr_start,
                _args.addr_end);
        return 1;
    }

    _driver.current()->setOption("relative", _args.relative_target ? "on" : "off");
    DisFormatter list(*_driver.current());
    list.setUppercase(_args.uppercase);
    list.setCpu();
    FilePrinter output;
    if (!_args.output_name.empty()) {
        if (!output.open(_args.output_name)) {
            fprintf(stderr, "Can't open output file %s\n", _args.output_name.c_str());
            return 1;
        }
        if (_args.verbose)
            fprintf(stderr, "%s: Opened for output\n", output.name().c_str());
        output.println(list.getContent());
    }
    FilePrinter listout;
    if (!_args.list_name.empty()) {
        if (!listout.open(_args.list_name)) {
            fprintf(stderr, "Can't open list file %s\n", _args.list_name.c_str());
            return 1;
        }
        if (_args.verbose)
            fprintf(stderr, "%s: Opened for listing\n", listout.name().c_str());
        listout.println(list.getLine());
    }
    for (const auto &it : memory) {
        auto base = it.first;
        auto size = it.second.size();
        uint32_t start = base / addrUnit;
        const uint32_t end = start + (size - 1) / addrUnit;
        if (base > _args.addr_end || end < _args.addr_start)
            continue;
        if (base < _args.addr_start) {
            size -= (_args.addr_start - base) * addrUnit;
            base = _args.addr_start;
            start = base / addrUnit;
        }
        if (end > _args.addr_end)
            size -= (end - _args.addr_end) * addrUnit;
        list.setOrigin(start);
        output.println(list.getContent());
        listout.println(list.getLine());
        for (size_t pc = 0; pc < size;) {
            const uint32_t address = start + pc / addrUnit;
            Insn insn(address);
            list.disassemble(memory, base + pc, insn);
            const Error error = _driver.current()->getError();
            const /* PROGMEM */ char *error_P = _driver.current()->errorText_P();
            pc += insn.length();
            if (error)
                fprintf(stderr, "%s:0x%04x: error: %s\n", _args.input_name.c_str(), insn.address(),
                        error_P);
            if (error)
                listout.format("%s:0x%04x: error: %s\n", _args.input_name.c_str(), insn.address(),
                        error_P);
            do {
                listout.println(list.getLine());
            } while (list.hasNext());
            if (error)
                output.format("; %s:0x%04x: error: %s\n", _args.input_name.c_str(), insn.address(),
                        error_P);
            do {
                output.println(list.getContent());
            } while (list.hasNext());
        }
    };

    return 0;
}

int DisCommander::readBinary(FileReader &input, BinMemory &memory) {
    const char *filename = input.name().c_str();
    const uint8_t addrUnit = static_cast<uint8_t>(_driver.current()->config().addressUnit());
    const auto size = BinDecoder::decode(input, memory);
    if (size < 0) {
        fprintf(stderr, "%s:%d: Unrecognizable binary format\n", filename, input.lineno());
    } else if (_args.verbose) {
        for (const auto &it : memory) {
            const uint32_t start = it.first / addrUnit;
            const size_t size = it.second.size();
            const uint32_t end = (it.first + size - 1) / addrUnit;
            fprintf(stderr, "%s: Read %4zu bytes 0x%04X-0x%04X\n", filename, size, start, end);
        }
    }
    return size;
}

Disassembler *DisCommander::defaultDisassembler() {
    if (_args.prog_name.find(PROG_PREFIX) == 0) {
        const size_t prefix_len = strlen(PROG_PREFIX);
        const char *cpu = _args.prog_name.c_str() + prefix_len;
        Disassembler *dis = _driver.restrictCpu(cpu);
        if (dis)
            return dis;
    }
    return nullptr;
}

int DisCommander::usage() {
    const auto cpuList = _driver.listCpu();
    const char *cpuOption = cpuList.size() == 1 ? "" : " -C <CPU>";
    std::string list;
    const char *cpuSep = "\n                ";
    std::string buf;
    for (auto &cpu : cpuList) {
        if (buf.size())
            buf += " ";
        if (buf.size() + cpu.size() < 60) {
            buf += cpu;
        } else {
            list += cpuSep;
            list += buf;
            buf = cpu;
        }
    }
    if (list.size() + buf.size() < 60) {
        list += ": " + buf;
    } else {
        list += cpuSep + buf;
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
            _args.prog_name.c_str(), cpuOption, list.c_str());
    return 2;
}

static const char *basename(const char *str, char sep_char = '/') {
    const char *sep = strrchr(str, sep_char);
    return sep ? sep + 1 : str;
}

int DisCommander::parseArgs(int argc, const char **argv) {
    _args.prog_name = basename(argv[0]);
    for (int i = 1; i < argc; i++) {
        const char *opt = argv[i];
        if (*opt == '-') {
            switch (*++opt) {
            case 'o':
                if (++i >= argc) {
                    fprintf(stderr, "-o requires output file name\n");
                    return 1;
                }
                _args.output_name = argv[i];
                break;
            case 'l':
                if (++i >= argc) {
                    fprintf(stderr, "-l requires listing file name\n");
                    return 1;
                }
                _args.list_name = argv[i];
                break;
            case 'C': {
                if (++i >= argc) {
                    fprintf(stderr, "-C requires CPU name\n");
                    return 1;
                }
                _args.cpu = argv[i];
                break;
            }
            case 'r':
                _args.relative_target = true;
                break;
            case 'u':
                _args.uppercase = true;
                break;
            case 'v':
                _args.verbose = true;
                break;
            case 'A':
                if (++i >= argc) {
                    fprintf(stderr, "-A requires start[,end] address\n");
                    return 1;
                } else {
                    char *end_start;
                    _args.addr_start = strtoul(argv[i], &end_start, 0);
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
                    _args.addr_end = strtoul(end_start, &end_end, 0);
                    if (end_end == end_start)
                        goto format_error;
                    if (*end_end)
                        goto form_error;
                    if (_args.addr_end < _args.addr_start) {
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
            if (!_args.input_name.empty()) {
                fprintf(stderr, "multiple input files specified: %s and %s\n",
                        _args.input_name.c_str(), opt);
                return 1;
            }
            _args.input_name = opt;
        }
    }
    if (_args.input_name.empty()) {
        fprintf(stderr, "no input file\n");
        return 1;
    }
    if (_args.output_name == _args.input_name) {
        fprintf(stderr, "output file overwrite input file\n");
        return 2;
    }
    if (_args.list_name == _args.input_name) {
        fprintf(stderr, "listing file overwrite input file\n");
        return 2;
    }
    return 0;
}

}  // namespace cli
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
