#pragma once
#include <type_traits>
#include <ratio>

namespace si
{

//------------------------------------------------------------------------------
/// Units
template
<
    int aMassExponent = 0,
    int aDistanceExponent = 0,
    int aTimeExponent = 0,
    int aCurrentExponent = 0,
    int aTemperatureExponent = 0,
    int aLuminanceExponent = 0,
    int aAngleExponent = 0
>
struct units
{
    using mass = std::integral_constant<int, aMassExponent>;
    using distance = std::integral_constant<int, aDistanceExponent>;
    using time = std::integral_constant<int, aTimeExponent>;
    using current = std::integral_constant<int, aCurrentExponent>;
    using temperature = std::integral_constant<int, aTemperatureExponent>;
    using light = std::integral_constant<int, aLuminanceExponent>;
    using angle = std::integral_constant<int, aAngleExponent>;
};

template< typename aType >
struct is_units_impl : std::false_type {};

template
<
    int aMassExponent,
    int aDistanceExponent,
    int aTimeExponent,
    int aCurrentExponent,
    int aTemperatureExponent,
    int aLuminanceExponent,
    int aAngleExponent
>
struct is_units_impl
<
    units
    <
        aMassExponent,
        aDistanceExponent,
        aTimeExponent,
        aCurrentExponent,
        aTemperatureExponent,
        aLuminanceExponent,
        aAngleExponent
    >
> : std::true_type {};

//------------------------------------------------------------------------------
/// Test a type to see if it is an si::units type.
/// @param aType the type to be tested
/// @return true if aType is an si::units type, false otherwise
template <typename aType>
constexpr bool is_units = is_units_impl<typename std::remove_cv<aType>::type>::value;

template< typename aLeft, typename aRight >
struct multiply_units_impl
{
    using type = units
    <
        aLeft::mass::value + aRight::mass::value,
        aLeft::distance::value + aRight::distance::value,
        aLeft::time::value + aRight::time::value,
        aLeft::current::value + aRight::current::value,
        aLeft::temperature::value + aRight::temperature::value,
        aLeft::light::value + aRight::light::value,
        aLeft::angle::value + aRight::angle::value
    >;
};

//------------------------------------------------------------------------------
/// The product of two si::units types.
/// @param aLeft an si::units type to be multiplied
/// @param aRight an si::units type to be multiplied
/// @return the si::units type that is the product of aLeft and aRight
template< typename aLeft, typename aRight >
using multiply_units = typename multiply_units_impl< aLeft, aRight >::type;

template< typename Units, typename aPower >
struct exponentiate_units_impl
{
    using type = units
    <
        std::ratio_multiply<std::ratio<Units::mass::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::distance::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::time::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::current::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::temperature::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::light::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::angle::value>, aPower>::num
    >;
};

//------------------------------------------------------------------------------
/// Raise an si::units type to a power.
/// @param Units an si::units type that is the base
/// @param Num the numerator of the power
/// @param Den the denominator of the power
/// @return the si::units type that is the exponentiation of Units by std::ratio<Num,Den>
template< typename aUnits, std::intmax_t Num, std::intmax_t Den = 1 >
using exponentiate_units = typename exponentiate_units_impl< aUnits, std::ratio<Num,Den> >::type;

//------------------------------------------------------------------------------
/// The reciprocal of an si::units type.
/// @param aUnits an si::units type
/// @return the si::units type that is the reciprocal of aUnits
template< typename aUnits >
using reciprocal_units = exponentiate_units< aUnits, -1>;

//------------------------------------------------------------------------------
/// The quotient of two si::units types.
/// @param aDividend an si::units type
/// @param aDivisor an si::units type
/// @return the si::units type that is the quotient of dividing aDividend by aDivisor
template< typename aDividend, typename aDivisor >
using divide_units = multiply_units< aDividend, reciprocal_units< aDivisor > >;

//------------------------------------------------------------------------------
/// The modulo of two si::units types.
/// @param aDividend an si::units type
/// @param aDivisor an si::units type
/// @return the si::units type that is the remainder of dividing aDividend by aDivisor
template< typename aDividend, typename aDivisor >
using modulo_units = aDividend;

//------------------------------------------------------------------------------
/// The square of an si::units type.
/// @param aUnits an si::units type
/// @return the si::units type that is the square of aUnits
template< typename aUnits >
using square_units = exponentiate_units<aUnits, 2>;

//------------------------------------------------------------------------------
/// Base units
using scalar = units<>; // unitless
using mass = units<1>;
using distance = units<0,1>;
using time = units<0,0,1>;
using current = units<0,0,0,1>;
using temperature = units<0,0,0,0,1>;
using luminance = units<0,0,0,0,0,1>;
using angle = units<0,0,0,0,0,0,1>;

} // end of namespace si
