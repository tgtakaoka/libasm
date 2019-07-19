/* -*- mode: c++; -*- */
#ifndef _STRING_UTILS_H__
#define _STRING_UTILS_H__

#include <cstdint>

char *outStr(char *out, const char *str);
char *outStr(char *out, std::uint8_t width, const char *str); /* right adjust */
char *outStr(char *out, const char *str, std::uint8_t width); /* left adjust */
char *outHex8(char *out, std::uint8_t u8);
char *outHex16(char *out, std::uint16_t u16);
char *outUint16(char *out, std::uint16_t u16);
char *outInt16(char *out, std::int16_t i16);
char *outHex32(char *out, std::uint32_t u32);
char *outUint32(char *out, std::uint32_t u32);
char *outInt32(char *out, std::int32_t i32);

bool getInt16(const char *&in, std::uint16_t &out);
bool getUint32(const char *&in, std::uint32_t &out);

#endif
