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

#include "test_asserter.h"
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace libasm {
namespace test {

bool TestAsserter::check() const {
    if (_fail_count == 0) {
        printf("  PASS (%4d)     %s\n", _pass_count, _test_name);
    } else {
        printf("  FAIL (%4d/%4d) %s\n", _fail_count, (_fail_count + _pass_count), _test_name);
    }
    return _fail_count == 0;
}

void TestAsserter::isTrue(const char *file, const int line, const char *message, bool actual) {
    if (actual) {
        _pass_count++;
        return;
    }
    _fail_count++;
    printf("%s:%d: %s: expected 'true': actual 'false'\n", file, line, message);
}

void TestAsserter::isFalse(const char *file, const int line, const char *message, bool actual) {
    if (!actual) {
        _pass_count++;
        return;
    }
    _fail_count++;
    printf("%s:%d: %s: expected 'false': actual 'true'\n", file, line, message);
}

void TestAsserter::isNull(
        const char *file, const int line, const char *message, const void *actual) {
    if (actual == nullptr) {
        _pass_count++;
        return;
    }
    _fail_count++;
    printf("%s:%d: %s: expected 'nullptr': actual '%p'\n", file, line, message, actual);
}

void TestAsserter::isNotNull(
        const char *file, const int line, const char *message, const void *actual) {
    if (actual != nullptr) {
        _pass_count++;
        return;
    }
    _fail_count++;
    printf("%s:%d: %s: expected not 'nullptr': actual 'nullptr'\n", file, line, message);
}

void TestAsserter::equals(const char *file, const int line, const char *message, uint64_t expected,
        uint64_t actual, Radix radix) {
    if (expected == actual) {
        _pass_count++;
        return;
    }
    _fail_count++;
    if (radix == RADIX_10) {
        printf("%s:%d: %s: expected '%lu': actual '%lu'\n", file, line, message, expected, actual);
    } else if (radix == RADIX_16) {
        printf("%s:%d: %s: expected '%lx': actual '%lx'\n", file, line, message, expected, actual);
    }
}

void TestAsserter::equals(const char *file, const int line, const char *message,
        const char *expected, const Insn &insn) {
    const char *actual = insn.name();
    if (strcmp(expected, actual) == 0) {
        _pass_count++;
        return;
    }
    _fail_count++;
    printf("%s:%d: %s: expected '%s'\n", file, line, message, expected);
    printf("%s:%d: %s:   actual '%s'\n", file, line, message, actual);
}

void TestAsserter::equals(const char *file, const int line, const char *message,
        const char *expected, const char *actual) {
    if (expected == nullptr && actual == nullptr) {
        _pass_count++;
        return;
    }
    if (expected == nullptr) {
        _fail_count++;
        printf("%s:%d: %s: expected nullptr: actual '%s'\n", file, line, message, actual);
        return;
    }
    if (actual == nullptr) {
        _fail_count++;
        printf("%s:%d: %s: expected '%s': actual nullptr\n", file, line, message, expected);
        return;
    }
    if (strcmp(expected, actual) == 0) {
        _pass_count++;
        return;
    }
    _fail_count++;
    printf("%s:%d: %s: expected '%s'\n", file, line, message, expected);
    printf("%s:%d: %s:   actual '%s'\n", file, line, message, actual);
}

void TestAsserter::equals(const char *file, const int line, const char *message,
        const StrScanner *expected, const char *actual) {
    return expected ? equals(file, line, message, expected->str(), actual)
                    : equals(file, line, message, static_cast<const char *>(nullptr), actual);
}

void TestAsserter::equals(const char *file, const int line, const char *message,
        const char *expected, const StrScanner *actual) {
    return actual ? equals(file, line, message, expected, *actual)
                  : equals(file, line, message, expected, static_cast<const char *>(nullptr));
}

void TestAsserter::equals(const char *file, const int line, const char *message,
        const char *expected, const StrScanner &actual) {
    if (expected && strlen(expected) == actual.size() &&
            strncmp(expected, actual.str(), actual.size()) == 0) {
        _pass_count++;
        return;
    }
    _fail_count++;
    printf("%s:%d: %s: expected '%s'\n", file, line, message, expected);
    printf("%s:%d: %s:   actual '%.*s'\n", file, line, message, (int)actual.size(), actual.str());
}

void TestAsserter::equals_P(const char *file, const int line, const char *message,
        const char *expected, const /* PROGMEM */ char *actual_P) {
    char buffer[strlen_P(actual_P) + 1];
    strcpy_P(buffer, actual_P);
    return equals(file, line, message, expected, buffer);
}

void TestAsserter::equals(const char *file, const int line, const char *message, Error expected,
        const ErrorReporter &actual) {
    if (expected == actual.getError()) {
        _pass_count++;
        return;
    }
    _fail_count++;
    printf("%s:%d: %s: expected '%s'\n", file, line, message, ErrorReporter::errorText_P(expected));
    printf("%s:%d: %s:   actual '%s'\n", file, line, message, actual.errorText_P());
}

void TestAsserter::equals(const char *file, const int line, const char *message, Error expected,
        const ErrorAt &actual) {
    if (expected == actual.getError()) {
        _pass_count++;
        return;
    }
    _fail_count++;
    if (actual.getError() == OK) {
        printf("%s:%d: %s: expected '%s'\n", file, line, message, ErrorAt::errorText_P(expected));
        printf("%s:%d: %s:   actual '%s'\n", file, line, message, actual.errorText_P());
    } else {
        printf("%s:%d: %s: expected '%s'\n", file, line, message, ErrorAt::errorText_P(expected));
        printf("%s:%d: %s:   actual '%s' at '%s'\n", file, line, message, actual.errorText_P(),
                actual.errorAt());
    }
}

void TestAsserter::equals(const char *file, const int line, const char *message,
        const ArrayMemory &expected, const uint8_t actual[], size_t actual_len, Radix radix) {
    const auto endian = expected.endian();
    size_t i;
    auto it = expected.iterator(endian);
    for (i = 0, it.rewind(); i < actual_len && it.hasNext(); i++) {
        if (it.readByte() != actual[i])
            break;
    }
    if (i == actual_len && i == expected.size()) {
        _pass_count++;
        return;
    }
    _fail_count++;
    const auto unit = expected.unit();
    const auto hasByte = (unit == ADDRESS_BYTE) || (expected.hasByte());
    const auto hasWord = (unit == ADDRESS_WORD) || (expected.hasWord());
    const auto hasQuad = (unit == ADDRESS_QUAD) || (expected.hasQuad());
    const auto fmt8 = (radix == RADIX_8) ? "%03" PRIo8 : "%02" PRIX8;
    const auto fmt16 = (radix == RADIX_8) ? "%06" PRIo16 : "%04" PRIX16;
    const auto fmt32 = (radix == RADIX_8) ? "%011" PRIo16 : "%08" PRIX16;

    printf("%s:%d: %s: expected [", file, line, message);
    for (i = 0, it.rewind(); it.hasNext(); i++) {
        if (i)
            printf(" ");
        if (hasByte) {
            printf(fmt8, it.readByte());
        } else if (hasWord) {
            printf(fmt16, it.readUint16());
        } else if (hasQuad) {
            printf(fmt32, it.readUint32());
        }
    }
    printf("]\n");
    const ArrayMemory mem(0, actual, actual_len, endian, unit);
    auto m = mem.iterator(endian);
    printf("%s:%d: %s:   actual [", file, line, message);
    for (i = 0; m.hasNext(); i++) {
        if (i)
            printf(" ");
        if (hasByte) {
            printf(fmt8, m.readByte());
        } else if (hasWord) {
            printf(fmt16, m.readUint16());
        } else if (hasQuad) {
            printf(fmt32, m.readUint32());
        }
    }
    printf("]\n");
}

void TestAsserter::not_equals(const char *file, const int line, const char *message,
        const char *not_expected, const char *actual) {
    if (actual && (not_expected == nullptr || strcmp(not_expected, actual))) {
        _pass_count++;
        return;
    }
    if (actual == nullptr && not_expected == nullptr) {
        _fail_count++;
        printf("%s:%d: %s: not expected nullptr: actual nullptr\n", file, line, message);
        return;
    }
    _fail_count++;
    printf("%s:%d: %s: not expected '%s'\n", file, line, message, not_expected);
    printf("%s:%d: %s:       actual '%s'\n", file, line, message, actual);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
