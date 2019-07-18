/* -*- mode: c++; -*- */
#ifndef __CONFIG_HOST_H__
#define __CONFIG_HOST_H__

#include <cstdint>

namespace host
{
    typedef std::int_least8_t int_t;
    typedef std::uint_least8_t uint_t;
    typedef std::int_fast16_t index_t;
    typedef std::uint_fast8_t uindex_t;
} // namespace host


#ifdef AVR

#include <avr/pgmspace.h>
#define pgm_strcpy(d, s) strcpy_P((d), pgm_read_word(&(s)))
#define pgm_strcasecmp(n, p) strcasecmp_P((n), pgm_read_word(&(p)))
class __FlashStringHelper;
char *outStr(char *out, const __FlashStringHelper *str);

#else  // AVR

#include <string.h>
#define PROGMEM
#define pgm_read_byte(p) *reinterpret_cast<const uint8_t *>(p)
#define pgm_read_word(p) *reinterpret_cast<const uint16_t *>(p)
#define F(text) (text)
#define strcpy_P(d, s) strcpy((d), (s))
#define pgm_strcpy(d, s) strcpy((d), (s))
#define pgm_strcasecmp(n, p) strcasecmp((n), (p))

#endif // AVR

#endif // __CONFIG_HOST_H__
