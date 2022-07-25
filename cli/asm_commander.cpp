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

#include "asm_formatter.h"
#include "file_printer.h"
#include "file_reader.h"

#include <string.h>

namespace libasm {
namespace cli {

using namespace libasm::driver;

Error AsmCommander::FileFactory::open(const StrScanner &name) {
    if (_sources.size() >= max_includes)
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

const TextReader *AsmCommander::FileFactory::current() const {
    return _sources.empty() ? nullptr : &_sources.back();
}

Error AsmCommander::FileFactory::closeCurrent() {
    _sources.pop_back();
    return OK;
}

StrScanner *AsmCommander::FileFactory::readLine() {
    while (!_sources.empty()) {
        auto &source = _sources.back();
        auto *line = source.readLine();
        if (line)
            return line;
        closeCurrent();
    }
    return nullptr;
}

TextPrinter &AsmCommander::FileFactory::errors() {
    return FilePrinter::STDERR;
}

AsmCommander::AsmCommander(AsmDirective **begin, AsmDirective **end)
    : _sources(), _driver(begin, end, _sources) {}

int AsmCommander::assemble() {
    if (_cpu && !_driver.setCpu(_cpu)) {
        fprintf(stderr, "unknown CPU '%s'\n", _cpu);
        return 4;
    }
    _driver.setSymbolMode(REPORT_DUPLICATE);
    int pass = 0;
    BinMemory memory;
    if (_verbose) {
        fprintf(stderr, "libasm assembler (version " LIBASM_VERSION_STRING ")\n");
        fprintf(stderr, "%s: Pass %d\n", _input_name, ++pass);
    }
    FilePrinter listout;
    (void)assemble(memory, listout);

    do {
        _driver.setSymbolMode(REPORT_UNDEFINED);
        BinMemory next;
        if (_verbose)
            fprintf(stderr, "%s: Pass %d\n", _input_name, ++pass);
        (void)assemble(next, listout);
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
                const uint32_t start = it.first / addrUnit;
                const size_t size = it.second.size();
                const uint32_t end = (it.first + size - 1) / addrUnit;
                fprintf(stderr, "%s: Write %4zu bytes %04x-%04x\n", _output_name, size, start, end);
            }
        }
    }

    if (_list_name && !listout.open(_list_name)) {
        fprintf(stderr, "Can't open list file %s\n", _list_name);
        return 1;
    }
    if (_list_name && _verbose) {
        fprintf(stderr, "%s: Opened for listing\n", _list_name);
        fprintf(stderr, "%s: Pass listing\n", _input_name);
    }
    assemble(memory, listout, true);

    return 0;
}

int AsmCommander::assemble(BinMemory &memory, TextPrinter &out, bool reportError) {
    if (_sources.open(_input_name)) {
        fprintf(stderr, "Can't open input file %s\n", _input_name);
        return 1;
    }

    AsmFormatter listing(memory);
    listing.setUppercase(_uppercase);
    listing.enableLineNumber(_line_number);
    int errors = 0;
    _driver.reset();
    StrScanner *scan;
    while ((scan = _sources.readLine()) != nullptr) {
        listing.clear();
        const auto error = _driver.assemble(*scan, listing);
        if (error == OK || error == END_ASSEMBLE) {
            do {
                out.println(listing.getLine());
            } while (listing.hasNextLine());
            if (error != OK)
                break;
        } else if (reportError) {
            const char *filename = _sources.current()->name().c_str();
            const char *line = scan->str();
            const int lineno = _sources.current()->lineno();
            const char *at = _driver.errorAt().str();
            const int column = (at >= line && at < line + scan->size()) ? at - line + 1 : -1;
            if (column >= 0) {
                fprintf(stderr, "%s:%d:%d: error: %s\n", filename, lineno, column,
                        _driver.errorText_P());
            } else {
                fprintf(stderr, "%s:%d: error: %s at '%s'\n", filename, lineno,
                        _driver.errorText_P(), at);
            }
            fprintf(stderr, "%s:%d: %s\n", filename, lineno, line);
            if (column >= 0) {
                out.format(
                        "%s:%d:%d: error: %s\n", filename, lineno, column, _driver.errorText_P());
            } else {
                out.format(
                        "%s:%d: error: %s at '%s'\n", filename, lineno, _driver.errorText_P(), at);
            }
            out.format("%s:%d: %s\n", filename, lineno, line);
            errors++;
        }
    }
    while (_sources.size())
        _sources.closeCurrent();
    return errors;
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
            "  -u          : use uppercase letter for listing\n"
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
    _uppercase = false;
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
            case 'u':
                _uppercase = true;
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
