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

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "test_asserter.h"

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

void TestAsserter::equals(const char *file, const int line, const char *message,
        const uint32_t expected, const uint32_t actual) {
    if (expected == actual) {
        _pass_count++;
        return;
    }
    _fail_count++;
    printf("%s:%d: %s: expected '%u': actual '%u'\n", file, line, message, expected, actual);
}

void TestAsserter::equals(const char *file, const int line, const char *message,
        const char *expected, const char *actual) {
    const char *e = expected;
    const char *a = actual;
    while (*e && (*e == *a || (*e == '_' && *a == '.'))) {
        e++;
        a++;
    }
    if (*e == 0 && *a == 0) {
        _pass_count++;
        return;
    }
    _fail_count++;
    printf("%s:%d: %s: expected '%s': actual '%s'\n", file, line, message, expected, actual);
}

void TestAsserter::equals(const char *file, const int line, const char *message,
        const char *expected, const StrScanner &actual) {
    if (strlen(expected) == actual.size() && strncmp(expected, actual, actual.size()) == 0) {
        _pass_count++;
        return;
    }
    _fail_count++;
    printf("%s:%d: %s: expected '%s': actual '%.*s'\n", file, line, message, expected,
            (int)actual.size(), (const char *)actual);
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
    printf("%s:%d: %s: expected '%s': actual '%s'\n", file, line, message,
            ErrorReporter::errorText(expected), actual.errorText());
}

void TestAsserter::equals(const char *file, const int line, const char *message,
        ArrayMemory &memory, const uint8_t actual[], size_t actual_len) {
    size_t i;
    for (i = 0, memory.rewind(); i < actual_len && memory.hasNext(); i++) {
        if (memory.readByte() != actual[i])
            break;
    }
    if (i == memory.size()) {
        _pass_count++;
        return;
    }
    _fail_count++;
    printf("%s:%d: %s: expected [", file, line, message);
    for (i = 0, memory.rewind(); memory.hasNext(); i++) {
        if (i)
            printf(" ");
        printf("%02" PRIX8, memory.readByte());
    }
    printf("]: actual [");
    for (i = 0; i < actual_len; i++) {
        if (i)
            printf(" ");
        printf("%02" PRIX8, actual[i]);
    }
    printf("]\n");
}

void TestAsserter::not_equals(const char *file, const int line, const char *message,
        const char *expected, const char *actual) {
    if (strcmp(expected, actual)) {
        _pass_count++;
        return;
    }
    _fail_count++;
    printf("%s:%d: %s: not expected '%s': actual '%s'\n", file, line, message, expected, actual);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
