/* -*- mode: c++; -*- */
#ifndef __TEST_ASSERTER_H__
#define __TEST_ASSERTER_H__

#include <cstdint>

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
        const char *message, const int expected, const int actual);
    void equals(
        const char *message, const char *expected, const char *actual);
    void equals(
        const char *message,
        const uint8_t expected[], std::size_t expected_len,
        const uint8_t actual[], std::size_t actual_len);

private:
    const char *_test_name;
    int _pass_count;
    int _fail_count;
};

#endif
