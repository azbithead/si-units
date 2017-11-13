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
    int aLengthExponent = 0,
    int aTimeExponent = 0,
    int aCurrentExponent = 0,
    int aTemperatureExponent = 0,
    int aLuminanceExponent = 0,
    int aSubstanceExponent = 0,
    int aAngleExponent = 0
>
struct units
{
    using mass = std::integral_constant<int, aMassExponent>;
    using length = std::integral_constant<int, aLengthExponent>;
    using time = std::integral_constant<int, aTimeExponent>;
    using current = std::integral_constant<int, aCurrentExponent>;
    using temperature = std::integral_constant<int, aTemperatureExponent>;
    using luminance = std::integral_constant<int, aLuminanceExponent>;
    using substance = std::integral_constant<int, aSubstanceExponent>;
    using angle = std::integral_constant<int, aAngleExponent>;
};

template< typename aType >
struct is_units_impl : std::false_type {};

template
<
    int aMassExponent,
    int aLengthExponent,
    int aTimeExponent,
    int aCurrentExponent,
    int aTemperatureExponent,
    int aLuminanceExponent,
    int aSubstanceExponent,
    int aAngleExponent
>
struct is_units_impl
<
    units
    <
        aMassExponent,
        aLengthExponent,
        aTimeExponent,
        aCurrentExponent,
        aTemperatureExponent,
        aLuminanceExponent,
        aSubstanceExponent,
        aAngleExponent
    >
> : std::true_type {};

//------------------------------------------------------------------------------
/// true if aType is an si::units type, false otherwise
/// @tparam aType the type to be tested
template <typename aType>
constexpr bool is_units = is_units_impl<typename std::remove_cv<aType>::type>::value;

template< typename aLeft, typename aRight >
struct multiply_units_impl
{
    using type = units
    <
        aLeft::mass::value + aRight::mass::value,
        aLeft::length::value + aRight::length::value,
        aLeft::time::value + aRight::time::value,
        aLeft::current::value + aRight::current::value,
        aLeft::temperature::value + aRight::temperature::value,
        aLeft::luminance::value + aRight::luminance::value,
        aLeft::substance::value + aRight::substance::value,
        aLeft::angle::value + aRight::angle::value
    >;
};

//------------------------------------------------------------------------------
/// the si::units type that is the product of aLeft and aRight
/// @tparam aLeft an si::units type to be multiplied
/// @tparam aRight an si::units type to be multiplied
template< typename aLeft, typename aRight >
using multiply_units = typename multiply_units_impl< aLeft, aRight >::type;

template< typename Units, typename aPower >
struct exponentiate_units_impl
{
    using type = units
    <
        std::ratio_multiply<std::ratio<Units::mass::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::length::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::time::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::current::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::temperature::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::luminance::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::substance::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::angle::value>, aPower>::num
    >;
};

//------------------------------------------------------------------------------
/// the si::units type that is aUnits raised to aPower
/// Raise an si::units type to a power.
/// @tparam aUnits an si::units type
/// @tparam aPower the power to which aUnits is raised
template< typename aUnits, std::intmax_t aPower >
using power_units = typename exponentiate_units_impl<aUnits, std::ratio<aPower,1>>::type;

//------------------------------------------------------------------------------
/// the si::units type that is the reciprocal of aUnits
/// @tparam aUnits an si::units type
template< typename aUnits >
using reciprocal_units = typename exponentiate_units_impl<aUnits, std::ratio<-1,1>>::type;

//------------------------------------------------------------------------------
/// the si::units type that is the quotient of dividing aDividend by aDivisor
/// @tparam aDividend an si::units type
/// @tparam aDivisor an si::units type
template< typename aDividend, typename aDivisor >
using divide_units = multiply_units< aDividend, reciprocal_units< aDivisor > >;

//------------------------------------------------------------------------------
/// the si::units type that is the root aRoot of aUnits
/// @tparam aUnits an si::units type
/// @tparam aRoot the root to be gotten
template< typename aUnits, std::intmax_t aRoot >
using root_units = typename exponentiate_units_impl<aUnits, std::ratio<1,aRoot>>::type;

//------------------------------------------------------------------------------
/// Base units
using scalar = units<>; // unitless
using mass = units<1>;
using length = units<0,1>;
using distance = length;
using time = units<0,0,1>;
using current = units<0,0,0,1>; // electric current
using temperature = units<0,0,0,0,1>;
using luminance = units<0,0,0,0,0,1>; // luminous intensity
using substance = units<0,0,0,0,0,0,1>; // amount of substance
using angle = units<0,0,0,0,0,0,0,1>;

} // end of namespace si
