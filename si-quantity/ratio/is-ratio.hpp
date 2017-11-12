#pragma once
#include <ratio>

namespace si
{

template <typename aType>
struct is_ratio_impl : std::false_type{};

template <std::intmax_t aNum, std::intmax_t aDen>
struct is_ratio_impl<std::ratio<aNum, aDen>> : std::true_type{};

//------------------------------------------------------------------------------
/// @param aType a type to be tested
/// @return true if aType is a std::ratio, false otherwise
template <typename aType>
constexpr bool is_ratio = is_ratio_impl<aType>::value;

} // end of namespace si
