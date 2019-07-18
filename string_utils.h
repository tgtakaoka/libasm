/* -*- mode: c++; -*- */
#ifndef _STRING_UTILS_H__
#define _STRING_UTILS_H__

#include "config_host.h"

char *outStr(char *out, const char *str);
char *outStr(char *out, host::uint_t width, const char *str); /* right adjust */
char *outStr(char *out, const char *str, host::uint_t width); /* left adjust */
char *outHex8(char *out, target::byte_t u8);
char *outHex16(char *out, target::word_t u16);
char *outUint16(char *out, target::word_t u16);
char *outInt16(char *out, target::sword_t i16);
char *outHex32(char *out, target::dword_t u32);
char *outUint32(char *out, target::dword_t u32);
char *outInt32(char *out, target::sdword_t i32);

bool getInt16(const char *&in, target::word_t &out);
bool getUint32(const char *&in, target::dword_t &out);

#endif
