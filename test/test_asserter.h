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

#ifndef __TEST_ASSERTER_H__
#define __TEST_ASSERTER_H__

#include <stdint.h>

class TestAsserter {
public:
    TestAsserter() : _fail_count(0) {}
    void clear(const char *test_name) {
        _test_name = test_name;
        _pass_count = 0;
        _fail_count = 0;
    }
    bool check() const;
    void equals(
        const char *message, const uint32_t expected, const uint32_t actual);
    void equals16(
        const char *message, const uint16_t expected, const uint16_t actual) {
        equals(message,
               static_cast<uint32_t>(expected),
               static_cast<uint32_t>(actual));
    }
    void equals8(
        const char *message, const uint8_t expected, const uint8_t actual) {
        equals(message,
               static_cast<uint32_t>(expected),
               static_cast<uint32_t>(actual));
    }
    void equals(
        const char *message, const char *expected, const char *actual);
    void equals(
        const char *message,
        const uint8_t expected[], size_t expected_len,
        const uint8_t actual[], size_t actual_len);
    void equals(
        const char *message,
        const uint16_t expected[], size_t expected_len,
        const uint8_t actual[], size_t actual_len);
    void not_equals(
        const char *message, const char *expected, const char *actual);

private:
    const char *_test_name;
    int _pass_count;
    int _fail_count;
};

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
