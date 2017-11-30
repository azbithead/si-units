#pragma once
#include <type_traits>
#include <ratio>
#include <limits>
#include <cmath>

#include "units.hpp"

namespace si
{

//------------------------------------------------------------------------------
// absolute value of a units_t
template
<
    typename VALUE,
    typename RATIO,
    typename QUANTITY
>
inline
constexpr
units_t<VALUE, RATIO, QUANTITY>
absolute
(
    units_t<VALUE, RATIO, QUANTITY> aUnits
)
{
    return units_t<VALUE, RATIO, QUANTITY>{std::abs(aUnits.value())};
}

//------------------------------------------------------------------------------
// floor of a units_t
template
<
    typename RESULT,
    typename VALUE,
    typename RATIO,
    typename QUANTITY,
    typename = std::enable_if_t<is_units_t<RESULT>>
>
inline
constexpr
RESULT
floor
(
    units_t<VALUE, RATIO, QUANTITY> aUnits
)
{
    auto theResult = units_cast<RESULT>(aUnits);
    return RESULT{static_cast<typename RESULT::value_t>(std::floor(theResult.value()))};
}

//------------------------------------------------------------------------------
// ceiling of a units_t
template
<
    typename RESULT,
    typename VALUE,
    typename RATIO,
    typename QUANTITY,
    typename = std::enable_if_t<is_units_t<RESULT>>
>
inline
constexpr
RESULT
ceiling
(
    units_t<VALUE, RATIO, QUANTITY> aUnits
)
{
    auto theResult = units_cast<RESULT>(aUnits);
    if( theResult < aUnits )
    {
        theResult += RESULT{static_cast<typename RESULT::value_t>(1)};
    }
    return RESULT{static_cast<typename RESULT::value_t>(std::ceil(theResult.value()))};
}

//------------------------------------------------------------------------------
// round of a units_t
template
<
    typename RESULT,
    typename VALUE,
    typename RATIO,
    typename QUANTITY,
    typename = std::enable_if_t
    <
        is_units_t<RESULT> &&
        !std::is_floating_point<typename RESULT::value_t>::value
    >
>
inline
constexpr
RESULT
round
(
    units_t<VALUE, RATIO, QUANTITY> aUnits
)
{
    RESULT t0 = si::floor<RESULT>(aUnits);
    RESULT t1 = t0 + RESULT{1};
    auto diff0 = aUnits - t0;
    auto diff1 = t1 - aUnits;
    if (diff0 == diff1) {
        if (t0.value() & 1)
            return t1;
        return t0;
    } else if (diff0 < diff1) {
        return t0;
    }
    return t1;
}

template
<
    typename VALUE,
    typename RATIO,
    typename QUANTITY,
    typename EPSILON
>
using sqrt_result_t = typename std::enable_if
<
    std::is_floating_point<VALUE>::value,
    units_t
    <
        VALUE,
        typename ratio::ratio_sqrt<RATIO, EPSILON>::type,
        root_quantity<QUANTITY, 2>
    >
>::type;

//------------------------------------------------------------------------------
// square root of a units_t
template
<
    typename VALUE,
    typename RATIO,
    typename QUANTITY,
    typename EPSILON = std::ratio<1,10000000000000>
>
inline
sqrt_result_t<VALUE, RATIO, QUANTITY, EPSILON>
square_root
(
    units_t<VALUE, RATIO, QUANTITY> aQuantity
)
{
    using Result_t = sqrt_result_t<VALUE, RATIO, QUANTITY, EPSILON>;
    return Result_t{std::sqrt(aQuantity.value())};
}

template
<
    typename VALUE,
    typename RATIO,
    typename QUANTITY,
    std::intmax_t EXPONENT
>
struct power_result_impl;

template
<
    typename VALUE,
    typename RATIO,
    typename QUANTITY
>
struct power_result_impl<VALUE, RATIO, QUANTITY, 0>
{
    using type = units_t<VALUE, std::ratio<1>, none>;
};

template
<
    typename VALUE,
    typename RATIO,
    typename QUANTITY,
    std::intmax_t EXPONENT
>
struct power_result_impl
{
    using temp = typename power_result_impl<VALUE, RATIO, QUANTITY, EXPONENT-1>::type;
    using type = units_t
    <
        VALUE,
        std::ratio_multiply<RATIO, typename temp::ratio_t>,
        multiply_quantity<QUANTITY, typename temp::quantity_t>
    >;
};

template
<
    typename VALUE,
    typename RATIO,
    typename QUANTITY,
    std::intmax_t EXPONENT
>
using power_result_t = typename power_result_impl<VALUE, RATIO, QUANTITY, EXPONENT>::type;

template< typename VALUE >
constexpr
inline
VALUE
value_pow
(
    VALUE aBase,
    std::intmax_t aExponent
)
{
    if( aExponent > 0 )
    {
        return aBase * value_pow(aBase, aExponent - 1);
    }
    else
    {
        return 1;
    }
}

//------------------------------------------------------------------------------
// raise units_t to a power
template
<
    std::intmax_t EXPONENT,
    typename VALUE,
    typename RATIO,
    typename QUANTITY
>
constexpr
inline
power_result_t<VALUE, RATIO, QUANTITY, EXPONENT>
exponentiate
(
    const units_t<VALUE, RATIO, QUANTITY>& aQuantity
)
{
    using Result_t = power_result_t<VALUE, RATIO, QUANTITY, EXPONENT>;
    return Result_t{value_pow(aQuantity.value(), EXPONENT)};
}

//------------------------------------------------------------------------------
// sine of radians
template
<
    typename VALUE,
    typename RATIO
>
inline
scalar<>
sine
(
    radians<RATIO, VALUE> aRadians
)
{
    return scalar<>{std::sin(aRadians.expand())};
}

//------------------------------------------------------------------------------
// cosine of radians
template
<
    typename VALUE,
    typename RATIO
>
inline
scalar<>
cosine
(
    radians<RATIO, VALUE> aRadians
)
{
    return scalar<>{std::cos(aRadians.expand())};
}

//------------------------------------------------------------------------------
// tangent of radians
template
<
    typename VALUE,
    typename RATIO
>
inline
scalar<>
tangent
(
    radians<RATIO, VALUE> aRadians
)
{
    return scalar<>{std::tan(aRadians.expand())};
}

//------------------------------------------------------------------------------
// arc sine of scalar
template
<
    typename VALUE,
    typename RATIO
>
inline
radians<>
arc_sine
(
    scalar<RATIO, VALUE> aScalar
)
{
    return radians<>{std::asin(aScalar.expand())};
}

//------------------------------------------------------------------------------
// arc cosine of scalar
template
<
    typename VALUE,
    typename RATIO
>
inline
radians<>
arc_cosine
(
    scalar<RATIO, VALUE> aScalar
)
{
    return radians<>{std::acos(aScalar.expand())};
}

//------------------------------------------------------------------------------
// arc tangent of scalar
template
<
    typename VALUE,
    typename RATIO
>
inline
radians<>
arc_tangent
(
    scalar<RATIO, VALUE> aScalar
)
{
    return radians<>{std::atan(aScalar.expand())};
}

} // end of namespace si
