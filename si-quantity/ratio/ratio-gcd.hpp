#pragma once
#include <ratio>

namespace si
{

template <intmax_t aX, intmax_t aY>
struct gcd_impl
{
    static constexpr intmax_t value = gcd_impl<aY, aX % aY>::value;
};

template <intmax_t aX>
struct gcd_impl<aX, 0>
{
    static constexpr intmax_t value = aX;
};

template <>
struct gcd_impl<0, 0>
{
    static constexpr intmax_t value = 1;
};

//------------------------------------------------------------------------------
/// greatest common divisor (GCD)
template <intmax_t aX, intmax_t aY>
constexpr intmax_t gcd = gcd_impl<aX,aY>::value;

template <intmax_t aX, intmax_t aY>
struct lcm_impl
{
    static constexpr intmax_t value = aX / gcd<aX, aY> * aY;
};

//------------------------------------------------------------------------------
/// least common multiple (LCM)
template <intmax_t aX, intmax_t aY>
constexpr intmax_t lcm = lcm_impl<aX,aY>::value;

//------------------------------------------------------------------------------
/// the std::ratio that is the greatest common divisor (GCD) of aRatioX and aRatioY
/// @tparam aRatioX a std::ratio
/// @tparam aRatioY a std::ratio
template <typename aRatioX, typename aRatioY>
using ratio_gcd = std::ratio
<
    gcd
    <
        aRatioX::num,
        aRatioY::num
    >,
    lcm
    <
        aRatioX::den,
        aRatioY::den
    >
>;

} // end of namespace si
