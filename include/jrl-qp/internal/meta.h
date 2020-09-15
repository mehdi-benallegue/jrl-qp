/* Copyright 2020 CNRS-AIST JRL */

#pragma once

#include <type_traits>

namespace jrl
{
namespace qp
{
namespace internal
{
/** An helper struct used by derives_from.*/
template<template<typename...> class Base>
struct is_base
{
  /** Accept any class derived from Base<T...>.*/
  template<typename... T>
  static std::true_type check(Base<T...> const volatile &);
  /** Fallback function that will be used for type not deriving from Base<T...>. */
  static std::false_type check(...);
};

/** Check if class \t T derives from the templated class \t Base.
 *
 * This relies on jrl::qp::internal::is_base::check: if T derives from \t Base,
 * the overload returning \a std::true_type will be selected, otherwise, it
 * will be the one returning \a std::false_type.
 *
 * Adapted from https://stackoverflow.com/a/5998303/11611648
 */
template<typename T, template<typename...> class Base>
constexpr bool derives_from()
{
  return decltype(is_base<Base>::check(std::declval<const T &>()))::value;
}

/** Check if class \t T derives from the non-templated class \t Base
 * This returns \a false if \t Base is not a class.
 */
template<typename T, typename Base>
constexpr bool derives_from()
{
  return std::is_base_of<Base, T>::value;
}

/** Used to enable a function for a list of types.
 *
 * To have a function work for T equal or deriving from any B1, B2, ... or Bk
 * where Bi are types.
 * Use as template<typename T, enable_for_t<T,B1, B2, ..., ..., Bk>=0>
 */
template<typename T, typename Base>
using enable_for_t = typename std::enable_if<((std::is_same<T, Base>::value || derives_from<T, Base>())), int>::type;

/** Used to enable a function for a list of templated classes.
 *
 * To have a function work for T equal or deriving from any B1, B2, ... or Bk
 * where Bi are a templated classes.
 * Use as template<typename T, enable_for_templated_t<T,B1, B2, ..., ..., Bk>=0>
 */
template<typename T, typename Base>
using enable_for_templated_t = typename std::enable_if< derives_from<T, Base>(), int>::type;

/** A sink, whose value is always true. */
template<typename T>
class always_true : public std::true_type
{
};

/** A sink, whose value is always false. */
template<typename T>
class always_false : public std::false_type
{
};
} // namespace internal
} // namespace qp
   // namespace qp
} // namespace jrl