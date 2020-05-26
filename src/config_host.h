/*
 * Copyright 2020 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CONFIG_HOST_H__
#define __CONFIG_HOST_H__

#include "version.h"

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
#define pgm_strncasecmp(n, p, l) strncasecmp_P((n), reinterpret_cast<const char *>(pgm_read_word(&(p))), (l))

#else  // AVR

#include <string.h>
#define PROGMEM
// Teensy 3.x (ARM) predefined following macros.
#if !defined(pgm_read_byte)
#define pgm_read_byte(p) *reinterpret_cast<const uint8_t *>(p)
#define pgm_read_word(p) *reinterpret_cast<const uint16_t *>(p)
#define pgm_read_ptr(p) *reinterpret_cast<const uintptr_t *>(p)
#define F(text) (text)
#define strcpy_P(d, s) strcpy((d), (s))
#define strncpy_P(d, s, n) strncpy((d), (s), (n))
#endif // !defined(pgm_read_byte)
#define pgm_strcpy(d, s) strcpy((d), (s))
#define pgm_strncpy(d, s, n) strncpy((d), (s), (n))
#define pgm_strcasecmp(n, p) strcasecmp((n), (p))
#define pgm_strncasecmp(n, p, l) strncasecmp((n), (p), (l))

#endif // AVR

#endif // __CONFIG_HOST_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
