#pragma once
#include <type_traits>
#include <ratio>

namespace si
{

//------------------------------------------------------------------------------
/// Units
template
<
    std::intmax_t aMassExponent = 0,
    std::intmax_t aLengthExponent = 0,
    std::intmax_t aTimeExponent = 0,
    std::intmax_t aCurrentExponent = 0,
    std::intmax_t aTemperatureExponent = 0,
    std::intmax_t aLuminousIntensityExponent = 0,
    std::intmax_t aSubstanceExponent = 0,
    std::intmax_t aAngleExponent = 0
>
struct units
{
    static constexpr std::intmax_t mass_exp = aMassExponent;
    static constexpr std::intmax_t length_exp = aLengthExponent;
    static constexpr std::intmax_t time_exp = aTimeExponent;
    static constexpr std::intmax_t current_exp = aCurrentExponent;
    static constexpr std::intmax_t temperature_exp = aTemperatureExponent;
    static constexpr std::intmax_t luminous_intensity_exp = aLuminousIntensityExponent;
    static constexpr std::intmax_t substance_exp = aSubstanceExponent;
    static constexpr std::intmax_t angle_exp = aAngleExponent;
};

template< typename aType >
struct is_units_impl : std::false_type {};

template
<
    std::intmax_t aMassExponent,
    std::intmax_t aLengthExponent,
    std::intmax_t aTimeExponent,
    std::intmax_t aCurrentExponent,
    std::intmax_t aTemperatureExponent,
    std::intmax_t aLuminousIntensityExponent,
    std::intmax_t aSubstanceExponent,
    std::intmax_t aAngleExponent
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
        aLuminousIntensityExponent,
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
        aLeft::mass_exp + aRight::mass_exp,
        aLeft::length_exp + aRight::length_exp,
        aLeft::time_exp + aRight::time_exp,
        aLeft::current_exp + aRight::current_exp,
        aLeft::temperature_exp + aRight::temperature_exp,
        aLeft::luminous_intensity_exp + aRight::luminous_intensity_exp,
        aLeft::substance_exp + aRight::substance_exp,
        aLeft::angle_exp + aRight::angle_exp
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
        std::ratio_multiply<std::ratio<Units::mass_exp>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::length_exp>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::time_exp>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::current_exp>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::temperature_exp>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::luminous_intensity_exp>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::substance_exp>, aPower>::num,
        std::ratio_multiply<std::ratio<Units::angle_exp>, aPower>::num
    >;
};

//------------------------------------------------------------------------------
/// the si::units type that is aUnits raised to aPower
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
using luminous_intensity = units<0,0,0,0,0,1>;
using substance = units<0,0,0,0,0,0,1>; // amount of substance
using angle = units<0,0,0,0,0,0,0,1>;

//------------------------------------------------------------------------------
/// Derived units
using solid_angle = power_units<angle,2>;
using frequency = reciprocal_units<time>;
using force = divide_units<multiply_units<mass, length>, power_units<time,2>>;
using weight = force;
using pressure = divide_units<force, power_units<length,2>>;
using stress = pressure;
using energy = multiply_units<force, length>;
using work = energy;
using power = divide_units<energy, time>;
using charge = multiply_units<current, time>;
using voltage = divide_units<power, current>;
using capacitance = divide_units<charge, voltage>;
using impedance = divide_units<voltage, current>;
using conductance = reciprocal_units<impedance>;
using magnetic_flux = divide_units<energy, current>;
using magnetic_flux_density = divide_units<magnetic_flux, power_units<length,2>>;
using inductance = divide_units<magnetic_flux, current>;
using luminous_flux = multiply_units<luminous_intensity, solid_angle>;
using illuminance = divide_units<luminous_intensity, power_units<length,2>>;

} // end of namespace si
