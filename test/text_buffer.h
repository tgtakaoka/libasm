/* -*- mode: c++; -*- */
#ifndef __TEXT_BUFFER_H__
#define __TEXT_BUFFER_H__

#include <stdint.h>

class TextBuffer {
public:
    TextBuffer(int max) {
        _buffer = new char[max + 1];
        _tokens = new uint8_t[max + 1];
    }
    virtual ~TextBuffer() {
        delete[] _buffer;
        delete[] _tokens;
    }

    char *buffer() { return _buffer; }
    const char *buffer() const { return _buffer; }

    static void analyze(TextBuffer &a, TextBuffer &b);
    int length() const { return _len; }
    int prefixLen() const { return _prefixLen; }
    int digitsInPrefix() const { return _digitsInPrefix; }
    int suffixLen() const { return _suffixLen; }
    int digitsInSuffix() const { return _digitsInSuffix; }
    int trunkLen() const { return _len - _prefixLen - _suffixLen; }
    const char *prefix() const { return _buffer; }
    const char *suffix() const { return _buffer + _len - _suffixLen; }
    const char *trunk() const { return _buffer + _prefixLen; }

private:
    char *_buffer;
    uint8_t *_tokens;
    int _len;
    int _tokenLen;
    int _prefixLen;
    int _digitsInPrefix;
    int _suffixLen;
    int _digitsInSuffix;
    void toTokens();
};

#endif
