/* -*- mode: c++; -*- */
#ifndef __MAKE_SIGNED_H__
#define __MAKE_SIGNED_H__

#include <stdint.h>

// Extracted from <type_traits>

template<typename T>
struct __make_unsigned
{ typedef T __type; };

template<typename T>
struct __make_signed
{ typedef T __type; };

template<>
struct __make_signed<int8_t>
{ typedef int8_t __type; };

template<>
struct __make_unsigned<int8_t>
{ typedef uint8_t __type; };

template<>
struct __make_signed<uint8_t>
{ typedef int8_t __type; };

template<>
struct __make_unsigned<uint8_t>
{ typedef uint8_t __type; };

template<>
struct __make_signed<int16_t>
{ typedef int16_t __type; };

template<>
struct __make_unsigned<int16_t>
{ typedef uint16_t __type; };

template<>
struct __make_signed<uint16_t>
{ typedef int16_t __type; };

template<>
struct __make_unsigned<uint16_t>
{ typedef uint16_t __type; };

template<>
struct __make_signed<int32_t>
{ typedef int32_t __type; };

template<>
struct __make_unsigned<int32_t>
{ typedef uint32_t __type; };

template<>
struct __make_signed<uint32_t>
{ typedef int32_t __type; };

template<>
struct __make_unsigned<uint32_t>
{ typedef uint32_t __type; };

template<typename T>
struct make_signed
{ typedef typename __make_signed<T>::__type type; };

template<typename T>
struct make_unsigned
{ typedef typename __make_unsigned<T>::__type type; };

#endif
