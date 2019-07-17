#include <ctype.h>
#include <string.h>

#include "string_utils.h"

#ifdef SUPPORT_DISASM
#ifdef AVR
char *outStr(char *out, const __FlashStringHelper *str) {
  strcpy_P(out, reinterpret_cast<const char *>(str));
  while (*out) out++;
  return out;
}
#endif

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
#endif /* SUPPORT_DISASM */

#ifdef SUPPORT_ASM
static uint8_t getHex(const char c) {
    return isdigit(c) ? c - '0' : toupper(c) - 'A' + 10;
}

static const char *getSign(const char *p, int8_t &sign) {
    if (*p == '+') {
        sign = 1;
    } else if (*p == '-') {
        sign = -1;
    } else if (*p == '$') {
        sign = 2;
    } else {
        sign = 0;
        return p;
    }
    return ++p;
}

bool getInt16(const char *&in, uint16_t &out) {
    int8_t sign;
    const char *p = getSign(in, sign);
    uint16_t val = 0;
    if (sign == 2) {
        if (!isxdigit(*p)) return false;
        val = getHex(*p++);
        while (isxdigit(*p)) {
            val <<= 4;
            val |= getHex(*p++);
        }
        out = val;
        in = p;
        return true;
    }
    if (!isdigit(*p)) return false;
    val = getHex(*p++);
    while (isdigit(*p)) {
        val *= 10;
        val += getHex(*p++);
    }
    out = (sign < 0) ? -(int16_t)val : val;
    in = p;
    return true;
}

bool getUint32(const char *&in, uint32_t &out) {
    int8_t sign;
    const char *p = getSign(in, sign);
    uint32_t val = 0;
    if (sign == 2) {
        if (!isxdigit(*p)) return false;
        val = getHex(*p++);
        while (isxdigit(*p)) {
            val <<= 4;
            val |= getHex(*p++);
        }
        out = val;
        in = p;
        return true;
    }
    if (!isdigit(*p)) return false;
    val = getHex(*p++);
    while (isdigit(*p)) {
        val *= 10;
        val += getHex(*p++);
    }
    out = (sign < 0) ? -(int32_t)val : val;
    in = p;
    return true;
}
#endif /* SUPPORT_ASM */
