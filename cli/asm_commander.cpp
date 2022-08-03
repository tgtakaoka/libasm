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

#include "asm_directive.h"
#include "asm_formatter.h"
#include "asm_sources.h"
#include "file_printer.h"
#include "file_reader.h"
#include "intel_hex.h"
#include "moto_srec.h"

#include <string.h>

namespace libasm {
namespace cli {

using namespace libasm::driver;

class NullPrinter : public TextPrinter {
public:
    void println(const char *text) override {}
    void format(const char *fmt, ...) override {}
};

static NullPrinter STDNULL;

Error AsmCommander::FileSources::open(const StrScanner &name) {
    if (size() >= max_includes)
        return TOO_MANY_INCLUDE;
    const auto *parent = _sources.empty() ? nullptr : &_sources.back();
    const auto pos = parent ? parent->name().find_last_of('/') : std::string::npos;
    if (pos == std::string::npos || *name == '/') {
        _sources.emplace_back(std::string(name.str(), name.size()));
    } else {
        std::string path(parent->name().substr(0, pos + 1));
        path.append(name.str(), name.size());
        _sources.push_back(path);
    }
    if (!_sources.back().open()) {
        _sources.pop_back();
        return NO_INCLUDE_FOUND;
    }
    return OK;
}

driver::TextReader *AsmCommander::FileSources::last() {
    return _sources.empty() ? nullptr : &_sources.back();
}

driver::TextReader *AsmCommander::FileSources::secondToLast() {
    if (size() < 2)
        return nullptr;
    auto it = _sources.rbegin();
    return &(*++it);
}

Error AsmCommander::FileSources::closeCurrent() {
    auto &reader = _sources.back();
    reader.close();
    _sources.pop_back();
    return OK;
}

AsmCommander::AsmCommander(AsmDirective **begin, AsmDirective **end)
    : _sources(), _driver(begin, end, _sources) {}

int AsmCommander::assemble() {
    if (_cpu && !_driver.setCpu(_cpu)) {
        fprintf(stderr, "unknown CPU '%s'\n", _cpu);
        return 4;
    }

    int pass = 0;
    BinMemory memory;
    if (_verbose) {
        fprintf(stderr, "libasm assembler (version " LIBASM_VERSION_STRING ")\n");
        fprintf(stderr, "%s: Pass %d\n", _input_name, ++pass);
    }
    (void)assemble(memory, STDNULL, false);

    do {
        BinMemory next;
        if (_verbose)
            fprintf(stderr, "%s: Pass %d\n", _input_name, ++pass);
        (void)assemble(next, STDNULL, true);
        if (memory.equals(next))
            break;
        memory.swap(next);
    } while (true);

    if (_output_name) {
        FilePrinter output;
        if (!output.open(_output_name)) {
            fprintf(stderr, "Can't open output file %s\n", _output_name);
            return 1;
        }

        auto &encoder = _encoder == 'S' ? MotoSrec::encoder()
                                        : (_encoder == 'H' ? IntelHex::encoder()
                                                           : _driver.current()->defaultEncoder());
        const AddressWidth addrWidth = _driver.current()->assembler().config().addressWidth();
        encoder.reset(addrWidth, _record_bytes);
        encoder.encode(memory, output);
        if (_verbose) {
            const uint8_t addrUnit = _driver.current()->assembler().config().addressUnit();
            for (const auto &it : memory) {
                const uint32_t start = it.base / addrUnit;
                const size_t size = it.data.size();
                const uint32_t end = (it.base + size - 1) / addrUnit;
                fprintf(stderr, "%s: Write %4zu bytes %04x-%04x\n", _output_name, size, start, end);
            }
        }
    }

    if (_list_name) {
        FilePrinter listout;
        if (!listout.open(_list_name)) {
            fprintf(stderr, "Can't open list file %s\n", _list_name);
            return 1;
        }
        if (_verbose) {
            fprintf(stderr, "%s: Opened for listing\n", _list_name);
            fprintf(stderr, "%s: Pass listing\n", _input_name);
        }
        assemble(memory, listout, true);
    }

    return 0;
}

int AsmCommander::assemble(BinMemory &memory, TextPrinter &listout, bool reportError) {
    if (_sources.open(_input_name)) {
        fprintf(stderr, "Can't open input file %s\n", _input_name);
        return 1;
    }

    AsmFormatter formatter(_driver, _sources, memory);
    formatter.setUpperHex(_upper_hex);
    formatter.enableLineNumber(_line_number);
    if (_cpu)
        _driver.setCpu(_cpu);
    // TODO: call Assembler::setOptions()

    return _driver.assemble(_sources, memory, formatter, listout, FilePrinter::STDERR, reportError);
}

AsmDirective *AsmCommander::defaultDirective() {
    AsmDirective *directive = nullptr;
    const auto *prefix = strstr(_prog_name, PROG_PREFIX);
    if (prefix) {
        const char *cpu = prefix + strlen(PROG_PREFIX);
        directive = _driver.restrictCpu(cpu);
    }
    return directive;
}

int AsmCommander::usage() {
    defaultDirective();
    std::string list;
    const char *cpuSep = "\n                ";
    std::string buf;
    for (auto &cpu : _driver.listCpu()) {
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
            "libasm assembler (version " LIBASM_VERSION_STRING
            ")\n"
            "usage: %s [-o <output>] [-l <list>] <input>\n"
            "  -C <CPU>    : target CPU%s\n"
            "  -o <output> : output file\n"
            "  -l <list>   : list file\n"
            "  -S[<bytes>] : output Motorola S-Record format\n"
            "  -H[<bytes>] : output Intel HEX format\n"
            "              : optional <bytes> specifies data record length "
            "(max 32)\n"
            "  -h          : use lowe case letter for hexadecimal\n"
            "  -n          : output line number to list file\n"
            "  -v          : print progress verbosely\n",
            _prog_name, list.c_str());
    return 2;
}

static const char *basename(const char *str, char sep_char = '/') {
    const char *sep = strrchr(str, sep_char);
    return sep ? sep + 1 : str;
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
            case 'S':
            case 'H':
                _encoder = *opt++;
                if (*opt) {
                    char *end;
                    unsigned long v = strtoul(opt, &end, 10);
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
