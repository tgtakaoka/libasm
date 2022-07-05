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

void FileFactory::closeCurrent() {
    _sources.pop_back();
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

AsmDriver::AsmDriver(AsmDirective **begin, AsmDirective **end) : _commonDir(begin, end, _sources) {}

int AsmDriver::usage() {
    std::string cpuList;
    AsmDirective *directive = defaultDirective();
    if (directive) {
        cpuList = ": ";
        cpuList += _commonDir.listCpu("");
    } else {
        const char *cpuSep = "\n                ";
        cpuList = _commonDir.listCpu(cpuSep);
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
            _progname, cpuList.c_str());
    return 2;
}

int AsmDriver::assemble() {
    _commonDir.setSymbolMode(REPORT_DUPLICATE);
    int pass = 0;
    CliMemory memory;
    if (_verbose) {
        fprintf(stderr, "libasm assembler (version " LIBASM_VERSION_STRING ")\n");
        fprintf(stderr, "%s: Pass %d\n", _input_name, ++pass);
    }
    FilePrinter listout;
    (void)assemble(memory, listout);

    do {
        _commonDir.setSymbolMode(REPORT_UNDEFINED);
        CliMemory next;
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
        AsmDirective *directive = _commonDir.current();
        const AddressWidth addrWidth = _commonDir.config().addressWidth();
        BinFormatter *formatter = &directive->binFormatter();
        MotoSrec srecord{addrWidth};
        IntelHex intelHex{addrWidth};
        if (_formatter == 'S') {
            formatter = &srecord;
        } else if (_formatter == 'H') {
            formatter = &intelHex;
        }

        formatter->begin(&output);
        memory.dump([this, formatter](uint32_t addr, const uint8_t *data, size_t data_size) {
            if (_verbose) {
                const uint8_t addrUnit = _commonDir.addrUnit();
                fprintf(stderr, "%s: Write %4zu bytes %04x-%04x\n", _output_name, data_size,
                        addr / addrUnit, (uint32_t)(addr + data_size - 1) / addrUnit);
            }
            for (size_t i = 0; i < data_size; i += _record_bytes) {
                auto size = std::min(_record_bytes, data_size - i);
                formatter->encode(addr + i, data + i, size);
            }
        });
        formatter->end();
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

int AsmDriver::assemble(CliMemory &memory, TextPrinter &out, bool reportError) {
    if (_sources.open(_input_name)) {
        fprintf(stderr, "Can't open input file %s\n", _input_name);
        return 1;
    }

    int errors = 0;
    _commonDir.reset();
    StrScanner *scan;
    while ((scan = _sources.readLine()) != nullptr) {
        const auto error = _commonDir.assembleLine(*scan, memory);
        if (error == OK || error == END_ASSEMBLE) {
            printListing(memory, out);
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
                out.format("%s:%d: error: %s at '%s'\n", filename, lineno,
                        _commonDir.errorText_P(), at);
            }
            out.format("%s:%d %s\n", filename, lineno, line);
            errors++;
        }
    }
    while (_sources.size())
        _sources.closeCurrent();
    return errors;
}

void AsmDriver::printListing(CliMemory &memory, TextPrinter &out) {
    _listing.setUppercase(_uppercase);
    _listing.enableLineNumber(_line_number);
    _listing.reset(_commonDir);
    do {
        out.println(_listing.getLine());
    } while (_listing.hasNext());
}

AsmDirective *AsmDriver::defaultDirective() {
    const size_t prefix_len = strlen(PROG_PREFIX);
    AsmDirective *directive = nullptr;
    if (_progname && strncmp(_progname, PROG_PREFIX, prefix_len) == 0) {
        const char *cpu = _progname + prefix_len;
        directive = _commonDir.restrictCpu(cpu);
    }
    return directive;
}

int AsmDriver::parseOption(int argc, const char **argv) {
    _progname = basename(argv[0]);
    _input_name = nullptr;
    _output_name = nullptr;
    _list_name = nullptr;
    _record_bytes = 32;
    _formatter = 0;
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
                _formatter = *opt++;
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
                if (_commonDir.setCpu(argv[i]) == nullptr) {
                    fprintf(stderr, "unknown CPU '%s'\n", argv[i]);
                    return 4;
                }
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
    if (!_input_name) {
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

const char *AsmDriver::basename(const char *str, char sep_char) {
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
