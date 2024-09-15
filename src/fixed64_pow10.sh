#!/bin/bash

progname=$(basename $0 .sh)
gcc -o ${progname} -x c - -lquadmath <<EOF
#include <quadmath.h>
#include <stdint.h>
#include <stdio.h>

void print_q(const char *str, __float128 q, int16_t exp10) {
    union {
        __float128 r;
        uint8_t b[16];
    } x;
    x.r = q;
    int_fast16_t n = printf("        {__fixed64(");
    uint_fast8_t c = 0x80;  // hidden 1
    char *sep = "0x";
    for (int_fast8_t i = 0; i < 10; i++) {
        uint8_t v = x.b[13 - i];
        if (i == 9 && (v & 1))
            v += 2;  // round
        n += printf("%s%02X", sep, (v >> 1) | c);
        c = (v & 1) << 7;
        if (i % 8 == 7) {
            sep = ", 0x";
        } else {
            sep = "";
        }
    }
    int exp2 = log2q(q);
    n += printf("), %d, %d},", exp2, exp10);
    while (n < 61) {
        printf(" ");
        n++;
    }
    printf("// %s\n", str);
}

#define print(q, exp10) print_q(#q, q, exp10)

int main() {
    printf(R"(
const struct {
    const __fixed64 sig    // siginificand of 10^|exp10|
    const int16_t exp2;    // exponent of 2
    const uint16_t exp10;  // exponent of 10
} P10[] = {
)");
    print(1E4096Q, 4096);
    print(1E2048Q, 2048);
    print(1E1024Q, 1024);
    print(1E512Q, 512);
    print(1E256Q, 256);
    print(1E128Q, 128);
    print(1E64Q, 64);
    print(1E32Q, 32);
    print(1E16Q, 16);
    print(1E8Q, 8);
    print(1E4Q, 4);
    print(1E2Q, 2);
    print(1E1Q, 1);
    printf("};\n");
    return 0;
}
EOF

./${progname}
rm -f ${progname}
