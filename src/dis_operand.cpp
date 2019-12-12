#include <ctype.h>
#include <string.h>

#include "config_host.h"
#include "dis_operand.h"

char *outStr(char *out, const char *str) {
    while ((*out = *str++) != 0)
        out++;
    return out;
}

char *outStr(char *out, uint8_t width, const char *str) {
    for (uint16_t w = strlen(str); w < width; w++)
        *out++ = ' ';
    return outStr(out, str);
}

char *outStr(char *out, const char *str, uint8_t width) {
    out = outStr(out, str);
    for (uint16_t w = strlen(str); w < width; w++)
        *out++ = ' ';
    *out = 0;
    return out;
}

static char *outHex4(char *out, uint8_t u8) {
    *out++ =  u8 + (u8 < 10 ? '0' : 'A' - 10);
    *out = 0;
    return out;
}

char *outHex8(char *out, uint8_t u8) {
    return outHex4(outHex4(out, u8 >> 4), u8 & 0xf);
}

char *outHex16(char *out, uint16_t u16) {
    return outHex8(outHex8(out, u16 >> 8), u16 & 0xff);
}

char *outUint16(char *out, uint16_t u16) {
    char *t = out;
    while (u16 != 0) {
        *out++ = (u16 % 10) + '0';
        u16 /= 10;
    }
    if (out == t) *out++ = '0';
    *out = 0;
    for (char *e = out - 1; t < e; ) {
        char c = *t;
        *t++ = *e;
        *e-- = c;
    }
    return out;
}

char *outInt16(char *out, int16_t i16) {
    if (i16 >= 0) return outUint16(out, i16);
    *out++ = '-';
    return outUint16(out, -i16);
}

char *outHex32(char *out, uint32_t u32) {
    return outHex16(outHex16(out, u32 >> 16), u32 & 0xffff);
}

char *outUint32(char*out, uint32_t u32) {
    char *t = out;
    while (u32 != 0) {
        *out++ = (u32 % 10) + '0';
        u32 /= 10;
    }
    if (out == t) *out++ = '0';
    *out = 0;
    for (char *e = out - 1; t < e; ) {
        char c = *t;
        *t++ = *e;
        *e-- = c;
    }
    return out;
}

char *outInt32(char *out, int32_t i32) {
    if (i32 >= 0) return outUint32(out, i32);
    *out++ = '-';
    return outUint32(out, -i32);
}
