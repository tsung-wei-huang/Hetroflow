#pragma once

#include <tuple>

// nonstd
namespace nonstd {

template <typename T, typename U>
inline constexpr bool is_same_v = std::is_same<T, U>::value;

template <typename Base, typename Derived>
inline constexpr bool is_base_of_v = std::is_base_of<Base, Derived>::value;

template <size_t I, typename T>
using tuple_element_t = typename std::tuple_element<I, T>::type;

template< class T >
inline constexpr size_t tuple_size_v = std::tuple_size<T>::value;

template< class T >
using remove_reference_t = typename std::remove_reference<T>::type;

template< bool B, class T = void >
using enable_if_t = typename std::enable_if<B,T>::type;

template< class T >
using decay_t = typename std::decay<T>::type;

};  // end of nonstd ----------------------------------------------------------

namespace hf {

template<typename F>
struct function_traits;
 
// function pointer
template<typename R, typename... Args>
struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)> {
};

// function reference
template<typename R, typename... Args>
struct function_traits<R(&)(Args...)> : public function_traits<R(Args...)> {
};
 
 // function_traits
template<typename R, typename... Args>
struct function_traits<R(Args...)> {

  using return_type = R;
 
  static constexpr size_t arity = sizeof...(Args);
 
  template <size_t N>
  struct argument {
    static_assert(N < arity, "error: invalid parameter index.");
    using type = std::tuple_element_t<N,std::tuple<Args...>>;
  };

  template <size_t N>
  using argument_t = typename argument<N>::type;
};




}  // end of namespace hf -----------------------------------------------------




