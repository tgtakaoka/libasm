/* -*- mode: c++; -*- */
#ifndef __CONFIG_HOST_H__
#define __CONFIG_HOST_H__

#include <stdint.h>

namespace host
{
    typedef int_least8_t  int_t;
    typedef uint_least8_t uint_t;
} // namespace host

#define ARRAY_BEGIN(array) (&(array)[0])
#define ARRAY_END(array)   (&(array)[sizeof(array) / sizeof(array[0])])
#define ARRAY_RANGE(array) ARRAY_BEGIN(array), ARRAY_END(array)

#ifdef AVR

#include <avr/pgmspace.h>
#define pgm_strcpy(d, s) strcpy_P((d), reinterpret_cast<const char *>(pgm_read_word(&(s))))
#define pgm_strncpy(d, s, n) strncpy_P((d), reinterpret_cast<const char *>(pgm_read_word(&(s))), (n))
#define pgm_strcasecmp(n, p) strcasecmp_P((n), reinterpret_cast<const char *>(pgm_read_word(&(p))))

#else  // AVR

#include <string.h>
#define PROGMEM
#define pgm_read_byte(p) *reinterpret_cast<const uint8_t *>(p)
#define pgm_read_word(p) *reinterpret_cast<const uint16_t *>(p)
#define F(text) (text)
#define strcpy_P(d, s) strcpy((d), (s))
#define strncpy_P(d, s, n) strncpy((d), (s), (n))
#define pgm_strcpy(d, s) strcpy((d), (s))
#define pgm_strncpy(d, s, n) strncpy((d), (s), (n))
#define pgm_strcasecmp(n, p) strcasecmp((n), (p))

#endif // AVR

#endif // __CONFIG_HOST_H__
