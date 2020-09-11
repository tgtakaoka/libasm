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

#define ARRAY_BEGIN(array) (&(array)[0])
#define ARRAY_END(array)   (&(array)[sizeof(array) / sizeof(array[0])])
#define ARRAY_RANGE(array) ARRAY_BEGIN(array), ARRAY_END(array)

#ifdef AVR
#include <avr/pgmspace.h>

#else  // AVR

// Teensy (ARM) predefined following macros.
#if !defined(pgm_read_byte)
#include <string.h>

#define PROGMEM
namespace libasm {
namespace host {
typedef union {
    void *p;
    uintptr_t i;
} read_ptr_t;
} // host
} //libasm

#define pgm_read_byte(p) *(p)
#define pgm_read_word(p) *(p)
#define pgm_read_dword(p) *(p)
#define pgm_read_ptr(p) ((libasm::host::read_ptr_t *)(p))->i
#define PSTR(text) (text)
#define F(text) (text)
#define strcpy_P(d, s) strcpy((d), (s))
#define strncpy_P(d, s, n) strncpy((d), (s), (n))
#define strcmp_P(d, s) strcmp((d), (s))
#define strcasecmp_P(d, s) strcasecmp((d), (s))
#define strncasecmp_P(d, s, n) strncasecmp((d), (s), (n))

#endif // !defined(pgm_read_byte)

#endif // AVR

#endif // __CONFIG_HOST_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
