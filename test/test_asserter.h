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

#ifndef __LIBASM_TEST_ASSERTER_H__
#define __LIBASM_TEST_ASSERTER_H__

#include "array_memory.h"
#include "config_base.h"
#include "error_reporter.h"
#include "insn_base.h"
#include "str_scanner.h"

#include <cstddef>
#include <cstdint>

namespace libasm {
namespace test {

struct TestAsserter {
    TestAsserter() : _fail_count(0) {}
    void clear(const char *test_name) {
        _test_name = test_name;
        _pass_count = 0;
        _fail_count = 0;
    }
    bool check() const;

    /** assert bool */
    void isTrue(const char *file, int line, const char *message, bool actual);
    void isFalse(const char *file, int line, const char *message, bool actual);

    /** assert nullptr */
    void isNull(const char *file, int line, const char *message, const void *actual);
    void isNotNull(const char *file, int line, const char *message, const void *actual);

    /** assert uint64_t */
    void equals(
            const char *file, int line, const char *message, uint64_t expected, uint64_t actual);

    /** assert const char* */
    void equals(const char *file, int line, const char *message, const char *expected,
            const char *actual);

    /** assert const char* with StrScanner* */
    void equals(const char *file, int line, const char *message, const StrScanner *expected,
            const char *actual);

    /** assert StrScanner* */
    void equals(const char *file, int line, const char *message, const char *expected,
            const StrScanner *actual);

    /** assert StrScanner& */
    void equals(const char *file, int line, const char *message, const char *expected,
            const StrScanner &actual);

    /** assert Insn& */
    void equals(const char *file, int line, const char *message, const char *expected,
            const Insn &actual);

    /** assert const char with const PROGMEM char* */
    void equals_P(const char *file, int line, const char *message, const char *expected,
            const /* PROGMEM */ char *actual_P);

    /** assert ErrorReporter& */
    void equals(const char *file, int line, const char *message, Error expected,
            const ErrorReporter &actual);

    /** assert ErrorAt& */
    void equals(
            const char *file, int line, const char *message, Error expected, const ErrorAt &actual);

    /** assert uint8_t[] with ArrayMemory */
    void equals(const char *file, const int line, const char *message, const ArrayMemory &expected,
            const uint8_t actual[], size_t actual_len);

    /** assert const char* is not equal */
    void not_equals(const char *file, int line, const char *message, const char *expected,
            const char *actual);

private:
    const char *_test_name;
    int _pass_count;
    int _fail_count;
};

}  // namespace test
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
