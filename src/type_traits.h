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

// is_unsigned
template<typename T> struct __is_unsigned_helper;;
template<> struct __is_unsigned_helper<int8_t>   : public false_type {};
template<> struct __is_unsigned_helper<uint8_t>  : public true_type  {};
template<> struct __is_unsigned_helper<int16_t>  : public false_type {};
template<> struct __is_unsigned_helper<uint16_t> : public true_type  {};
template<> struct __is_unsigned_helper<int32_t>  : public false_type {};
template<> struct __is_unsigned_helper<uint32_t> : public true_type  {};
template<typename T>
struct is_unsigned : public __is_unsigned_helper<T> {};

// is_signed
template<typename T> struct __is_signed_helper;;
template<> struct __is_signed_helper<int8_t>   : public true_type  {};
template<> struct __is_signed_helper<uint8_t>  : public false_type {};
template<> struct __is_signed_helper<int16_t>  : public true_type  {};
template<> struct __is_signed_helper<uint16_t> : public false_type {};
template<> struct __is_signed_helper<int32_t>  : public true_type  {};
template<> struct __is_signed_helper<uint32_t> : public false_type {};
template<typename T>
struct is_signed : public __is_signed_helper<T> {};

// make_unsigned
template<typename T> struct __make_unsigned { typedef T __type; };
template<> struct __make_unsigned<int8_t>   { typedef uint8_t __type;  };
template<> struct __make_unsigned<uint8_t>  { typedef uint8_t __type;  };
template<> struct __make_unsigned<int16_t>  { typedef uint16_t __type; };
template<> struct __make_unsigned<uint16_t> { typedef uint16_t __type; };
template<> struct __make_unsigned<int32_t>  { typedef uint32_t __type; };
template<> struct __make_unsigned<uint32_t> { typedef uint32_t __type; };
template<typename T>
struct make_unsigned {
    typedef typename __make_unsigned<T>::__type type;
};

// make_signed
template<typename T> struct __make_signed { typedef T __type; };
template<> struct __make_signed<int8_t>   { typedef int8_t __type;  };
template<> struct __make_signed<uint8_t>  { typedef int8_t __type;  };
template<> struct __make_signed<int16_t>  { typedef int16_t __type; };
template<> struct __make_signed<uint16_t> { typedef int16_t __type; };
template<> struct __make_signed<int32_t>  { typedef int32_t __type; };
template<> struct __make_signed<uint32_t> { typedef int32_t __type; };
template<typename T>
struct make_signed {
    typedef typename __make_signed<T>::__type type;
};

#endif
