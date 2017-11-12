#pragma once
#include <ratio>

namespace si
{
namespace ratio
{

template <typename aType>
struct is_ratio_impl : std::false_type{};

template <std::intmax_t aNum, std::intmax_t aDen>
struct is_ratio_impl<std::ratio<aNum, aDen>> : std::true_type{};

//------------------------------------------------------------------------------
/// true if aType is a std::ratio, false otherwise
/// @tparam aType a type to be tested
template <typename aType>
constexpr bool is_ratio = is_ratio_impl<aType>::value;

} // end of namespace ratio
} // end of namespace si

