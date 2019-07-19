/* -*- mode: c++; -*- */
#ifndef _STRING_UTILS_H__
#define _STRING_UTILS_H__

#include <stdint.h>

char *outStr(char *out, const char *str);
char *outStr(char *out, uint8_t width, const char *str); /* right adjust */
char *outStr(char *out, const char *str, uint8_t width); /* left adjust */
char *outHex8(char *out, uint8_t u8);
char *outHex16(char *out, uint16_t u16);
char *outUint16(char *out, uint16_t u16);
char *outInt16(char *out, int16_t i16);
char *outHex32(char *out, uint32_t u32);
char *outUint32(char *out, uint32_t u32);
char *outInt32(char *out, int32_t i32);

bool getInt16(const char *&in, uint16_t &out);
bool getUint32(const char *&in, uint32_t &out);

#endif
