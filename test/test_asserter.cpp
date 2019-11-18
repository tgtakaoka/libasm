#include "test_asserter.h"

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstring>

bool TestAsserter::check() const {
    if (_fail_count == 0) {
        printf("  PASS (%4d)     %s\n", _pass_count, _test_name);
    } else {
        printf("  FAIL (%4d/%4d) %s\n",
               _fail_count, (_fail_count + _pass_count), _test_name);
    }
    return _fail_count == 0;
}

void TestAsserter::equals(
    const char *message, const int expected, const int actual) {
    if (expected == actual) {
        _pass_count++;
        return;
    }
    _fail_count++;
    printf("%s: expected Error '%d': actual '%d'\n", message, expected, actual);
}

void TestAsserter::equals(
    const char *message, const char *expected, const char *actual) {
    if (strcmp(expected, actual) == 0) {
        _pass_count++;
        return;
    }
    _fail_count++;
    printf("%s: expected '%s': actual '%s'\n", message, expected, actual);
}

void TestAsserter::equals(
    const char *message,
    const uint8_t expected[], std::size_t expected_len,
    const uint8_t actual[], std::size_t actual_len) {
    if (expected_len == actual_len) {
        std::size_t i;
        for (i = 0; i < expected_len; i++)
            if (expected[i] != actual[i])
                break;
        if (i == expected_len) {
            _pass_count++;
            return;
        }
    }
    _fail_count++;
    printf("%s: expected ", message);
    for (std::size_t i = 0; i < expected_len; i++) {
        printf(i == 0 ?  "[" : " ");
        printf("%02" PRIX8, expected[i]);
    }
    printf("]: actual ");
    for (std::size_t i = 0; i < actual_len; i++) {
        printf(i == 0 ? "[" : " ");
        printf("%02" PRIX8, actual[i]);
    }
    printf("\n");
}
