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

#ifndef __LIBASM_TYPE_TRAITS_H__
#define __LIBASM_TYPE_TRAITS_H__

#include <stdint.h>

namespace libasm {

// Extracted from <type_traits>

// true_type, false_type
template <bool v>
struct libasm_bool_constant {
    static constexpr bool value = v;
    typedef bool value_type;
};
typedef libasm_bool_constant<true> true_type;
typedef libasm_bool_constant<false> false_type;

// enable_if
template <bool, typename T = void>
struct enable_if {};
template <typename T>
struct enable_if<true, T> {
    typedef T type;
};

// declval
template <typename T, typename U = T &&>
U libasm_declval(int);
template <typename T>
T libasm_declval(long);
template <typename T>
auto declval() noexcept -> decltype(libasm_declval<T>(0));

// integral_cvonstant
template <class T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant<T, v>;
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

// is_class
template <typename T>
struct is_class : integral_constant<bool, __is_class(T)> {};

// is_base_of
template <typename BASE, typename DERIVED>
struct is_base_of : integral_constant<bool, __is_base_of(BASE, DERIVED)> {};

// is_base_of_template - is_base_of for template base class
template <template <typename...> class BASE, typename DERIVED>
struct libasm_is_base_of_template {
    template <typename... Ts>
    static constexpr true_type test(const BASE<Ts...> *);
    static constexpr false_type test(...);
    using type = decltype(test(declval<DERIVED *>()));
};
template <template <typename...> class BASE, typename DERIVED>
using is_base_of_template = typename libasm_is_base_of_template<BASE, DERIVED>::type;

// is_unsugned
template <typename T>
struct libasm_is_unsigned_helper;
template <>
struct libasm_is_unsigned_helper<int8_t> : public false_type {};
template <>
struct libasm_is_unsigned_helper<uint8_t> : public true_type {};
template <>
struct libasm_is_unsigned_helper<int16_t> : public false_type {};
template <>
struct libasm_is_unsigned_helper<uint16_t> : public true_type {};
template <>
struct libasm_is_unsigned_helper<int32_t> : public false_type {};
template <>
struct libasm_is_unsigned_helper<uint32_t> : public true_type {};
template <>
struct libasm_is_unsigned_helper<int64_t> : public false_type {};
template <>
struct libasm_is_unsigned_helper<uint64_t> : public true_type {};
template <typename T>
struct is_unsigned : public libasm_is_unsigned_helper<T> {};

// is_signed
template <typename T>
struct libasm_is_signed_helper;
template <>
struct libasm_is_signed_helper<int8_t> : public true_type {};
template <>
struct libasm_is_signed_helper<uint8_t> : public false_type {};
template <>
struct libasm_is_signed_helper<int16_t> : public true_type {};
template <>
struct libasm_is_signed_helper<uint16_t> : public false_type {};
template <>
struct libasm_is_signed_helper<int32_t> : public true_type {};
template <>
struct libasm_is_signed_helper<uint32_t> : public false_type {};
template <>
struct libasm_is_signed_helper<int64_t> : public true_type {};
template <>
struct libasm_is_signed_helper<uint64_t> : public false_type {};
template <typename T>
struct is_signed : public libasm_is_signed_helper<T> {};

// make_unsigned
template <typename T>
struct libasm_make_unsigned {
    typedef T libasm_type;
};
template <>
struct libasm_make_unsigned<int8_t> {
    typedef uint8_t libasm_type;
};
template <>
struct libasm_make_unsigned<uint8_t> {
    typedef uint8_t libasm_type;
};
template <>
struct libasm_make_unsigned<int16_t> {
    typedef uint16_t libasm_type;
};
template <>
struct libasm_make_unsigned<uint16_t> {
    typedef uint16_t libasm_type;
};
template <>
struct libasm_make_unsigned<int32_t> {
    typedef uint32_t libasm_type;
};
template <>
struct libasm_make_unsigned<uint32_t> {
    typedef uint32_t libasm_type;
};
template <>
struct libasm_make_unsigned<int64_t> {
    typedef uint64_t libasm_type;
};
template <>
struct libasm_make_unsigned<uint64_t> {
    typedef uint64_t libasm_type;
};
// make_unsigned
template <typename T>
struct make_unsigned {
    typedef typename libasm_make_unsigned<T>::libasm_type type;
};

// make_signed
template <typename T>
struct libasm_make_signed {
    typedef T libasm_type;
};
template <>
struct libasm_make_signed<int8_t> {
    typedef int8_t libasm_type;
};
template <>
struct libasm_make_signed<uint8_t> {
    typedef int8_t libasm_type;
};
template <>
struct libasm_make_signed<int16_t> {
    typedef int16_t libasm_type;
};
template <>
struct libasm_make_signed<uint16_t> {
    typedef int16_t libasm_type;
};
template <>
struct libasm_make_signed<int32_t> {
    typedef int32_t libasm_type;
};
template <>
struct libasm_make_signed<uint32_t> {
    typedef int32_t libasm_type;
};
template <>
struct libasm_make_signed<int64_t> {
    typedef int64_t libasm_type;
};
template <>
struct libasm_make_signed<uint64_t> {
    typedef int64_t libasm_type;
};
template <typename T>
struct make_signed {
    typedef typename libasm_make_signed<T>::libasm_type type;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
