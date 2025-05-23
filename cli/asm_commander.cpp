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

#include "asm_commander.h"
#include <cstring>
#include "asm_directive.h"
#include "file_printer.h"
#include "file_sources.h"
#include "intel_hex.h"
#include "moto_srec.h"
#include "stored_printer.h"

namespace libasm {
namespace cli {

using namespace libasm::driver;

AsmCommander::AsmCommander(std::initializer_list<AsmDirective *> directives)
    : _driver(directives) {}

int AsmCommander::assemble() {
    if (_cpu && !_driver.setCpu(_cpu)) {
        fprintf(stderr, "unknown CPU '%s'\n", _cpu);
        return 4;
    }

    int pass = 0;
    if (_verbose) {
        fprintf(stderr, "libasm assembler (version " LIBASM_VERSION_STRING ")\n");
        fprintf(stderr, "%s: Pass %d\n", _input_name, ++pass);
    }
    _driver.clearSymbols();

    BinMemory memory, prev;
    SymbolStoreImpl symbols;
    StoredPrinter listout, errorout;
    bool reportError = false;
    do {
        FileSources sources;
        if (sources.open(_input_name)) {
            fprintf(stderr, "Can't open input file %s\n", _input_name);
            return 1;
        }
        if (_verbose)
            fprintf(stderr, "%s: Pass %d\n", _input_name, ++pass);
        prev.swap(memory);
        symbols.copy(_driver.symbols());
        memory.clear();
        listout.clear();
        errorout.clear();
        _driver.setUpperHex(_upper_hex);
        _driver.setLineNumber(_line_number);
        _driver.reset();
        for (const auto &it : _options) {
            _driver.setOption(it.first.c_str(), it.second.c_str());
        }
        if (_cpu)
            _driver.setCpu(_cpu);
        _driver.assemble(sources, memory, listout, errorout, reportError);
        reportError = true;
    } while (errorout.size() == 0 && (memory != prev || symbols != _driver.symbols()));

    int errors = 0;
    for (size_t lineno = 1; lineno <= errorout.size(); lineno++) {
        fprintf(stderr, "%s\n", errorout.line(lineno));
        ++errors;
    }

    if (_list_name) {
        if (_verbose)
            fprintf(stderr, "%s: Opened for listing\n", _list_name);
        FILE *list = fopen(_list_name, "w");
        if (list == nullptr) {
            fprintf(stderr, "Can't open list file %s\n", _list_name);
            ++errors;
        } else {
            for (size_t lineno = 1; lineno <= listout.size(); lineno++)
                fprintf(list, "%s\n", listout.line(lineno));
            fclose(list);
        }
    }

    if (_output_name) {
        FilePrinter output;
        if (!output.open(_output_name)) {
            fprintf(stderr, "Can't open output file %s\n", _output_name);
            ++errors;
        } else {
            auto &encoder = _driver.current()->defaultEncoder();
            if (_encoder == 'S')
                encoder = MotoSrec::encoder();
            else if (_encoder == 'H')
                encoder = IntelHex::encoder();
            encoder.reset(_record_bytes);
            encoder.encode(memory, output);
            if (_verbose) {
                const auto unit = _driver.current()->config().addressUnit();
                for (auto block = memory.begin(); block != nullptr; block = block->next()) {
                    const auto start = block->base() / unit;
                    const auto size = block->size();
                    const auto end = (block->base() + size - 1) / unit;
                    fprintf(stderr, "%s: Write %4u bytes %04x-%04x\n", _output_name, size, start,
                            end);
                }
            }
        }
    }

    return errors;
}

bool AsmCommander::defaultDirective() {
    const auto prefix = strstr(_prog_name, PROG_PREFIX);
    if (prefix) {
        const auto cpu = prefix + strlen(PROG_PREFIX);
        return _driver.restrictCpu(cpu);
    }
    return false;
}

int AsmCommander::usage() {
    if (!defaultDirective() && _cpu && !_driver.restrictCpu(_cpu))
        fprintf(stderr, "Unknown CPU specified\n");
    std::string list;
    const auto cpuSep = "\n     ";
    std::string buf;
    for (auto &cpu : _driver.listCpu()) {
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
            R"(libasm assembler (version %s)
usage: %s [-o <output>] [-l <list>] <input>
  -C <CPU>          : target CPU%s
  -o <output>       : output file
  -l <list>         : list file
  -S[<bytes>]       : output Motorola S-Record format
  -H[<bytes>]       : output Intel HEX format
                    : optional <bytes> specifies data record length (max 32)
  -h                : use lower case letter for hexadecimal
  -n                : output line number to list file
  -v                : print progress verbosely
)",
            LIBASM_VERSION_STRING, _prog_name, list.c_str());
    bool longOptions = false;
    for (const auto *dir : _driver)
        longOptions |= (dir->commonOptions().head() || dir->options().head());
    if (longOptions) {
        fprintf(stderr, "  --<name>=<vale>   : extra options (<type> [, <CPU>])\n");
        const auto dir = *_driver.begin();
        for (const auto *opt = dir->commonOptions().head(); opt; opt = opt->next()) {
            fprintf(stderr, "  --%-16s: %s (%s)\n", opt->name_P(), opt->description_P(),
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
                    cpus[name] = dir->cpu_P();
                } else {
                    cpus[name] += ", ";
                    cpus[name] += dir->cpu_P();
                }
            }
        }
        for (const auto &opt : descs) {
            const auto &name = opt.first;
            const auto &desc = opt.second;
            fprintf(stderr, "  --%-16s: %s (%s: %s)\n", name.c_str(), desc.c_str(),
                    Options::nameof(specs[name]), cpus[name].c_str());
        }
    }
    return 2;
}

static const char *basename(const char *str, char sep_char = '/') {
    const auto sep = strrchr(str, sep_char);
    return sep ? sep + 1 : str;
}

int AsmCommander::parseOptionValue(const char *option) {
    const auto equ = strchr(option, '=');
    if (equ == nullptr)
        return 1;
    _options.emplace(std::string(option, equ), std::string(equ + 1));
    return 0;
}

int AsmCommander::parseArgs(int argc, const char **argv) {
    _prog_name = basename(argv[0]);
    _input_name = nullptr;
    _output_name = nullptr;
    _list_name = nullptr;
    _cpu = nullptr;
    _encoder = 0;
    _record_bytes = 32;
    _upper_hex = true;
    _line_number = false;
    _verbose = false;
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
            case 'S':
            case 'H':
                _encoder = *opt++;
                if (*opt) {
                    char *end;
                    const auto v = strtoul(opt, &end, 10);
                    if (*end || v > 64) {
                        fprintf(stderr, "invalid record length: %s\n", argv[i]);
                        return 3;
                    }
                    _record_bytes = v;
                }
                break;
            case 'C':
                if (++i >= argc) {
                    fprintf(stderr, "-C requires CPU name\n");
                    return 1;
                }
                _cpu = argv[i];
                break;
            case 'h':
                _upper_hex = false;
                break;
            case 'n':
                _line_number = true;
                break;
            case 'v':
                _verbose = true;
                break;
            case '-':
                if (parseOptionValue(++opt) == 0)
                    break;
                fprintf(stderr, "long option requires option=value\n");
                return 1;
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
        for (const auto *dir : _driver) {
            for (const auto *opt = dir->options().head(); opt; opt = opt->next()) {
                if (strcmp_P(option.first.c_str(), opt->name_P()) == 0) {
                    valid = true;
                    break;
                }
            }
            if (valid)
                continue;
            for (const auto *opt = dir->commonOptions().head(); opt; opt = opt->next()) {
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
