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

#include "asm_driver.h"

#include "asm_formatter.h"
#include "file_printer.h"
#include "file_reader.h"

#include <string.h>

namespace libasm {
namespace cli {

Error FileFactory::open(const StrScanner &name) {
    if (_sources.size() >= max_includes)
        return TOO_MANY_INCLUDE;
    const auto *parent = _sources.empty() ? nullptr : &_sources.back();
    const auto pos = parent ? parent->name().find_last_of('/') : std::string::npos;
    if (pos == std::string::npos || *name == '/') {
        _sources.emplace_back(std::string(name, name.size()));
    } else {
        std::string path(parent->name().substr(0, pos + 1));
        path.append(name, name.size());
        _sources.push_back(path);
    }
    if (!_sources.back().open()) {
        _sources.pop_back();
        return NO_INCLUDE_FOUND;
    }
    return OK;
}

const TextReader *FileFactory::current() const {
    return _sources.empty() ? nullptr : &_sources.back();
}

Error FileFactory::closeCurrent() {
    _sources.pop_back();
    return OK;
}

StrScanner *FileFactory::readLine() {
    while (!_sources.empty()) {
        auto &source = _sources.back();
        auto *line = source.readLine();
        if (line)
            return line;
        closeCurrent();
    }
    return nullptr;
}

AsmDriver::AsmDriver(AsmDirective **begin, AsmDirective **end, AsmArgs &args)
    : _commonDir(begin, end, _sources), _args(args) {}

std::list<std::string> AsmDriver::listCpu() {
    defaultDirective();
    return _commonDir.listCpu();
}

int AsmDriver::assemble() {
    if (!_args.cpu.empty() && !_commonDir.setCpu(_args.cpu.c_str())) {
        fprintf(stderr, "unknown CPU '%s'\n", _args.cpu.c_str());
        return 4;
    }
    _commonDir.setSymbolMode(REPORT_DUPLICATE);
    int pass = 0;
    BinMemory memory;
    if (_args.verbose) {
        fprintf(stderr, "libasm assembler (version " LIBASM_VERSION_STRING ")\n");
        fprintf(stderr, "%s: Pass %d\n", _args.input_name.c_str(), ++pass);
    }
    FilePrinter listout;
    (void)assemble(memory, listout);

    do {
        _commonDir.setSymbolMode(REPORT_UNDEFINED);
        BinMemory next;
        if (_args.verbose)
            fprintf(stderr, "%s: Pass %d\n", _args.input_name.c_str(), ++pass);
        (void)assemble(next, listout);
        if (memory.equals(next))
            break;
        memory.swap(next);
    } while (true);

    if (!_args.output_name.empty()) {
        FilePrinter output;
        if (!output.open(_args.output_name)) {
            fprintf(stderr, "Can't open output file %s\n", _args.output_name.c_str());
            return 1;
        }

        auto &encoder =
                _args.encoder == 'S'
                        ? MotoSrec::encoder()
                        : (_args.encoder == 'H' ? IntelHex::encoder()
                                                 : _commonDir.current()->defaultEncoder());
        const AddressWidth addrWidth = _commonDir.current()->assembler().config().addressWidth();
        encoder.reset(addrWidth, _args.record_bytes);
        encoder.encode(memory, output);
        if (_args.verbose) {
            const uint8_t addrUnit = _commonDir.current()->assembler().config().addressUnit();
            for (const auto &it : memory) {
                const uint32_t start = it.first / addrUnit;
                const size_t size = it.second.size();
                const uint32_t end = (it.first + size - 1) / addrUnit;
                fprintf(stderr, "%s: Write %4zu bytes %04x-%04x\n", _args.output_name.c_str(), size,
                        start, end);
            }
        }
    }

    if (!_args.list_name.empty() && !listout.open(_args.list_name)) {
        fprintf(stderr, "Can't open list file %s\n", _args.list_name.c_str());
        return 1;
    }
    if (!_args.list_name.empty() && _args.verbose) {
        fprintf(stderr, "%s: Opened for listing\n", _args.list_name.c_str());
        fprintf(stderr, "%s: Pass listing\n", _args.input_name.c_str());
    }
    assemble(memory, listout, true);

    return 0;
}

int AsmDriver::assemble(BinMemory &memory, TextPrinter &out, bool reportError) {
    if (_sources.open(_args.input_name.c_str())) {
        fprintf(stderr, "Can't open input file %s\n", _args.input_name.c_str());
        return 1;
    }

    AsmFormatter listing(memory);
    listing.setUppercase(_args.uppercase);
    listing.enableLineNumber(_args.line_number);
    int errors = 0;
    _commonDir.reset();
    StrScanner *scan;
    while ((scan = _sources.readLine()) != nullptr) {
        listing.clear();
        const auto error = _commonDir.assembleLine(*scan, listing);
        if (error == OK || error == END_ASSEMBLE) {
            do {
                out.println(listing.getLine());
            } while (listing.hasNext());
            if (error != OK)
                break;
        } else if (reportError) {
            const char *filename = _sources.current()->name().c_str();
            const char *line = *scan;
            const int lineno = _sources.current()->lineno();
            const char *at = _commonDir.errorAt();
            const int column = (at >= line && at < line + scan->size()) ? at - line + 1 : -1;
            if (column >= 0) {
                fprintf(stderr, "%s:%d:%d: error: %s\n", filename, lineno, column,
                        _commonDir.errorText_P());
            } else {
                fprintf(stderr, "%s:%d: error: %s at '%s'\n", filename, lineno,
                        _commonDir.errorText_P(), at);
            }
            fprintf(stderr, "%s:%d %s\n", filename, lineno, line);
            if (column >= 0) {
                out.format("%s:%d:%d: error: %s\n", filename, lineno, column,
                        _commonDir.errorText_P());
            } else {
                out.format("%s:%d: error: %s at '%s'\n", filename, lineno, _commonDir.errorText_P(),
                        at);
            }
            out.format("%s:%d %s\n", filename, lineno, line);
            errors++;
        }
    }
    while (_sources.size())
        _sources.closeCurrent();
    return errors;
}

AsmDirective *AsmDriver::defaultDirective() {
    AsmDirective *directive = nullptr;
    if (_args.prog_name.find(PROG_PREFIX) == 0) {
        const size_t prefix_len = strlen(PROG_PREFIX);
        const char *cpu = _args.prog_name.c_str() + prefix_len;
        directive = _commonDir.restrictCpu(cpu);
    }
    return directive;
}

int AsmDriver::usage(const std::list<std::string> &cpuList) {
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
            _args.prog_name.c_str(), list.c_str());
    return 2;
}

static const char *basename(const char *str, char sep_char = '/') {
    const char *sep = strrchr(str, sep_char);
    return sep ? sep + 1 : str;
}

int AsmDriver::parseArgs(int argc, const char **argv) {
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
            case 'S':
            case 'H':
                _args.encoder = *opt++;
                if (*opt) {
                    char *end;
                    unsigned long v = strtoul(opt, &end, 10);
                    if (*end || v > 64) {
                        fprintf(stderr, "invalid record length: %s\n", argv[i]);
                        return 3;
                    }
                    _args.record_bytes = v;
                }
                break;
            case 'C':
                if (++i >= argc) {
                    fprintf(stderr, "-C requires CPU name\n");
                    return 1;
                }
                _args.cpu = argv[i];
                break;
            case 'u':
                _args.uppercase = true;
                break;
            case 'n':
                _args.line_number = true;
                break;
            case 'v':
                _args.verbose = true;
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
