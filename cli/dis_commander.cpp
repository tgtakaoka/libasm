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

#include <algorithm>
#include <cstring>
#include <string>

#include "bin_decoder.h"
#include "file_printer.h"
#include "file_reader.h"

namespace libasm {
namespace cli {

using namespace libasm::driver;

DisCommander::DisCommander(std::initializer_list<Disassembler *> disassemblers)
    : _driver(disassemblers) {}

int DisCommander::disassemble() {
    if (_verbose)
        fprintf(stderr, "libasm disassembler (version " LIBASM_VERSION_STRING ")\n");
    if (defaultDisassembler() == nullptr && _cpu == nullptr) {
        fprintf(stderr, "No target CPU specified\n");
        return 1;
    }
    if (_driver.setCpu(_cpu) == nullptr) {
        fprintf(stderr, "Unknown target CPU %s\n", _cpu);
        return 1;
    }

    FileReader input(_input_name);
    if (!input.open()) {
        fprintf(stderr, "Can't open input file %s\n", _input_name);
        return 1;
    }
    BinMemory memory;
    if (readBinary(memory, input) < 0)
        return 1;
    input.close();

    const auto unit = _driver.current()->config().addressUnit();
    const auto mem_start = memory.startAddress() / unit;
    const auto mem_end = memory.endAddress() / unit;
    if (_dis_start > mem_end || _dis_end < mem_start) {
        fprintf(stderr, "Input file has address range: 0x%04X,0x%04X\n", mem_start, mem_end);
        fprintf(stderr, "-A range has no intersection: 0x%04X,0x%04X\n", _dis_start, _dis_end);
        return 1;
    }
    if (_range_start) {
        const auto start = _dis_start * unit;
        const auto end = _range_end ? _dis_end * unit : memory.endAddress();
        memory.setRange(start, end);
    }

    FilePrinter output;
    if (_output_name) {
        if (!output.open(_output_name)) {
            fprintf(stderr, "Can't open output file %s\n", _output_name);
            return 1;
        }
        if (_verbose)
            fprintf(stderr, "%s: Opened for output\n", output.name().c_str());
    }
    FilePrinter listout;
    if (_list_name) {
        if (!listout.open(_list_name)) {
            fprintf(stderr, "Can't open list file %s\n", _list_name);
            return 1;
        }
        if (_verbose)
            fprintf(stderr, "%s: Opened for listing\n", listout.name().c_str());
    }

    _driver.setUpperHex(_upper_hex);
    _driver.setUppercase(_uppercase);
    for (auto &opt : _options) {
        _driver.setOption(opt.first.c_str(), opt.second.c_str());
    }

    _driver.disassemble(memory, _input_name, output, listout, FilePrinter::STDERR);

    return 0;
}

int DisCommander::readBinary(BinMemory &memory, TextReader &input) {
    const auto filename = input.name().c_str();
    const auto unit = static_cast<uint8_t>(_driver.current()->config().addressUnit());
    const auto size = BinDecoder::decode(input, memory);
    if (size < 0) {
        fprintf(stderr, "%s:%d: Unrecognizable binary format\n", filename, input.lineno());
        return size;
    }
    if (_verbose) {
        for (auto block = memory.begin(); block != nullptr; block = block->next()) {
            const auto start = block->base() / unit;
            const auto size = block->size();
            const auto end = (block->base() + size - 1) / unit;
            fprintf(stderr, "%s: Read %4u bytes 0x%04X-0x%04X\n", filename, size, start, end);
        }
    }
    return size;
}

Disassembler *DisCommander::defaultDisassembler() {
    Disassembler *dis = nullptr;
    const auto *prefix = strstr(_prog_name, PROG_PREFIX);
    if (prefix) {
        const char *cpu = prefix + strlen(PROG_PREFIX);
        dis = _driver.restrictCpu(cpu);
    }
    return dis;
}

int DisCommander::usage() {
    if (defaultDisassembler() == nullptr && _cpu && _driver.restrictCpu(_cpu) == nullptr)
        fprintf(stderr, "Unknown CPU specified\n");
    const auto cpuList = _driver.listCpu();
    const auto cpuOption = cpuList.size() == 1 ? "" : " -C <CPU>";
    std::string list;
    const auto cpuSep = "\n     ";
    std::string buf;
    for (auto &cpu : cpuList) {
        if (buf.size())
            buf += " ";
        if (buf.size() + cpu.size() < 70) {
            buf += cpu;
        } else {
            list += cpuSep;
            list += buf;
            buf = cpu;
        }
    }
    if (list.size() + buf.size() < 70) {
        list += ": " + buf;
    } else {
        list += cpuSep + buf;
    }
    fprintf(stderr,
            R"(libasm disassembler (version %s)
usage: %s%s [-o <output>] [-l <list>] <input>
  -C <CPU>         : target CPU%s
  -o <output>      : output file
  -l <list>        : list file
  <input>          : file can be Motorola S-Record or Intel HEX format
  -A start[,end]   : disassemble start address and optional end address
  -r               : use program counter relative notation
  -h               : use lower case letter for hexadecimal
  -u               : use upper case letter for output
  -v               : print progress verbosely
)",
            LIBASM_VERSION_STRING, _prog_name, cpuOption, list.c_str());
    bool longOptions = false;
    for (const auto *dis : _driver)
        longOptions |= (dis->commonOptions().head() || dis->options().head());
    if (longOptions) {
        fprintf(stderr,
                "  --<name>=<vale>  : extra options (<type> [, <CPU>])\n");
        const auto dis = *_driver.begin();
        for (const auto *opt = dis->commonOptions().head(); opt; opt = opt->next()) {
            fprintf(stderr, "  --%-15s: %s (%s)\n", opt->name_P(), opt->description_P(),
                    Options::nameof(opt->spec()));
        }
        std::map<std::string, std::string> cpus;
        std::map<std::string, std::string> descs;
        std::map<std::string, OptionBase::OptionSpec> specs;
        for (const auto *dir : _driver) {
            for (const auto *opt = dir->options().head(); opt; opt = opt->next()) {
                const std::string name{opt->name_P()};
                if (descs.find(name) == descs.end()) {
                    descs[name] = opt->description_P();
                    specs[name] = opt->spec();
                    cpus[name] = dir->config().cpu_P();
                } else {
                    cpus[name] += ", ";
                    cpus[name] += dir->config().cpu_P();
                }
            }
        }
        for (const auto &opt : descs) {
            const auto &name = opt.first;
            const auto &desc = opt.second;
            fprintf(stderr, "  --%-15s: %s (%s: %s)\n", name.c_str(), desc.c_str(),
                    Options::nameof(specs[name]), cpus[name].c_str());
        }
    }
    return 2;
}

static const char *basename(const char *str, char sep_char = '/') {
    const auto sep = strrchr(str, sep_char);
    return sep ? sep + 1 : str;
}

void DisCommander::parseOptionValue(const char *option) {
    const auto equ = strchr(option, '=');
    if (equ) {
        // --<name>=<value>
        _options.emplace(std::string(option, equ), std::string(equ + 1));
    } else if (strncmp(option, "no-", 3) == 0) {
        // --no-<name>
        _options.emplace(option + 3, "false");
    } else {
        // --<name>
        _options.emplace(option, "true");
    }
}

int DisCommander::parseArgs(int argc, const char **argv) {
    _prog_name = basename(argv[0]);
    _input_name = nullptr;
    _output_name = nullptr;
    _list_name = nullptr;
    _cpu = nullptr;
    _upper_hex = true;
    _uppercase = false;
    _verbose = false;
    _range_start = _range_end = false;
    _dis_start = 0;
    _dis_end = UINT32_MAX;
    for (auto i = 1; i < argc; i++) {
        const auto *opt = argv[i];
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
                _cpu = argv[i];
                break;
            }
            case 'r':
                _options.emplace("relative", "on");
                break;
            case 'h':
                _upper_hex = false;
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
                    _dis_start = strtoul(argv[i], &end_start, 0);
                    if (end_start == argv[i]) {
                    format_error:
                        fprintf(stderr, "invalid address format for -A: %s\n", argv[i]);
                        return 1;
                    }
                    _range_start = true;
                    if (*end_start == 0)
                        break;
                    if (*end_start++ != ',') {
                    form_error:
                        fprintf(stderr, "address must be start[,end] form: %s\n", argv[i]);
                        return 1;
                    }
                    char *end_end;
                    _dis_end = strtoul(end_start, &end_end, 0);
                    if (end_end == end_start)
                        goto format_error;
                    if (*end_end)
                        goto form_error;
                    if (_dis_end < _dis_start) {
                        fprintf(stderr, "end address must be less than start: %s\n", argv[i]);
                        return 1;
                    }
                    _range_end = true;
                }
                break;
            case '-':
                parseOptionValue(++opt);
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
    if (_input_name == nullptr) {
        fprintf(stderr, "no input file\n");
        return 1;
    }
    if (_output_name && strcmp(_output_name, _input_name) == 0) {
        fprintf(stderr, "output file overwrite input file\n");
        return 2;
    }
    if (_list_name && strcmp(_list_name, _input_name) == 0) {
        fprintf(stderr, "listing file overwrite input file\n");
        return 2;
    }
    for (auto &option : _options) {
        auto valid = false;
        for (const auto *dis : _driver) {
            for (const auto *opt = dis->options().head(); opt; opt = opt->next()) {
                if (strcmp_P(option.first.c_str(), opt->name_P()) == 0) {
                    valid = true;
                    break;
                }
            }
            if (valid)
                continue;
            for (const auto *opt = dis->commonOptions().head(); opt; opt = opt->next()) {
                if (strcmp_P(option.first.c_str(), opt->name_P()) == 0) {
                    valid = true;
                    break;
                }
            }
        }
        if (!valid) {
            fprintf(stderr, "unknown option --%s=%s\n", option.first.c_str(),
                    option.second.c_str());
            return 1;
        }
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
