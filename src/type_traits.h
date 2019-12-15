/* -*- mode: c++; -*- */
#ifndef __MAKE_SIGNED_H__
#define __MAKE_SIGNED_H__

#include <stdint.h>

// Extracted from <type_traits>

template<bool v>
struct __bool_constant {
    static constexpr bool value = v;
    typedef bool value_type;
};
typedef __bool_constant<true>  true_type;
typedef __bool_constant<false> false_type;

template<typename T>
struct __type_traits {
    typedef T __signed_type;
    typedef T __unsigned_type;
    typedef T __is_signed;
    typedef T __is_unsigned;
};

template<>
struct __type_traits<int8_t> {
    typedef int8_t  __signed_type;
    typedef uint8_t __unsigned_type;
    typedef true_type  __is_signed;
    typedef false_type __is_unsigned;
};

template<>
struct __type_traits<uint8_t> {
    typedef int8_t  __signed_type;
    typedef uint8_t __unsigned_type;
    typedef false_type __is_signed;
    typedef true_type  __is_unsigned;
};

template<>
struct __type_traits<int16_t> {
    typedef int16_t  __signed_type;
    typedef uint16_t __unsigned_type;
    typedef true_type  __is_signed;
    typedef false_type __is_unsigned;
};

template<>
struct __type_traits<uint16_t> {
    typedef int16_t  __signed_type;
    typedef uint16_t __unsigned_type;
    typedef false_type __is_signed;
    typedef true_type  __is_unsigned;
};

template<>
struct __type_traits<int32_t> {
    typedef int32_t  __signed_type;
    typedef uint32_t __unsigned_type;
    typedef true_type  __is_signed;
    typedef false_type __is_unsigned;
};

template<>
struct __type_traits<uint32_t> {
    typedef int32_t  __signed_type;
    typedef uint32_t __unsigned_type;
    typedef false_type __is_signed;
    typedef true_type  __is_unsigned;
};

template<typename T>
struct make_signed {
    typedef typename __type_traits<T>::__signed_type type;
};

template<typename T>
struct make_unsigned {
    typedef typename __type_traits<T>::__unsigned_type type;
};

template<typename T>
struct is_signed : public __type_traits<T>::__is_signed {
};

template<typename T>
struct is_unsigned : public __type_traits<T>::__is_unsigned {
};

#endif
