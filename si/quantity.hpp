#pragma once
#include <type_traits>
#include <ratio>

namespace si
{

//------------------------------------------------------------------------------
/// si::quantity_t
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
struct quantity_t
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
struct is_quantity_impl : std::false_type {};

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
struct is_quantity_impl
<
    quantity_t
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
/// true if aType is an si::quantity_t type, false otherwise
/// @tparam aType the type to be tested
template <typename aType>
constexpr bool is_quantity = is_quantity_impl<typename std::remove_cv<aType>::type>::value;

template< typename aLeft, typename aRight >
struct multiply_quantity_impl
{
    using type = quantity_t
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
/// the si::quantity_t type that is the product of aLeft and aRight
/// @tparam aLeft an si::quantity_t type to be multiplied
/// @tparam aRight an si::quantity_t type to be multiplied
template< typename aLeft, typename aRight >
using multiply_quantity = typename multiply_quantity_impl< aLeft, aRight >::type;

template< typename Quantity, typename aPower >
struct exponentiate_quantity_impl
{
    using type = quantity_t
    <
        std::ratio_multiply<std::ratio<Quantity::mass_exp>, aPower>::num,
        std::ratio_multiply<std::ratio<Quantity::length_exp>, aPower>::num,
        std::ratio_multiply<std::ratio<Quantity::time_exp>, aPower>::num,
        std::ratio_multiply<std::ratio<Quantity::current_exp>, aPower>::num,
        std::ratio_multiply<std::ratio<Quantity::temperature_exp>, aPower>::num,
        std::ratio_multiply<std::ratio<Quantity::luminous_intensity_exp>, aPower>::num,
        std::ratio_multiply<std::ratio<Quantity::substance_exp>, aPower>::num,
        std::ratio_multiply<std::ratio<Quantity::angle_exp>, aPower>::num
    >;
};

//------------------------------------------------------------------------------
/// the si::quantity_t type that is aQuantity raised to aPower
/// @tparam aQuantity an si::quantity_t type
/// @tparam aPower the power to which aQuantity is raised
template< typename aQuantity, std::intmax_t aPower >
using power_quantity = typename exponentiate_quantity_impl<aQuantity, std::ratio<aPower,1>>::type;

//------------------------------------------------------------------------------
/// the si::quantity_t type that is the reciprocal of aQuantity
/// @tparam aQuantity an si::quantity_t type
template< typename aQuantity >
using reciprocal_quantity = typename exponentiate_quantity_impl<aQuantity, std::ratio<-1,1>>::type;

//------------------------------------------------------------------------------
/// the si::quantity_t type that is the quotient of dividing aDividend by aDivisor
/// @tparam aDividend an si::quantity_t type
/// @tparam aDivisor an si::quantity_t type
template< typename aDividend, typename aDivisor >
using divide_quantity = multiply_quantity< aDividend, reciprocal_quantity< aDivisor > >;

//------------------------------------------------------------------------------
/// the si::quantity_t type that is the root aRoot of aQuantity
/// @tparam aQuantity an si::quantity_t type
/// @tparam aRoot the root to be gotten
template< typename aQuantity, std::intmax_t aRoot >
using root_quantity = typename exponentiate_quantity_impl<aQuantity, std::ratio<1,aRoot>>::type;

//------------------------------------------------------------------------------
/// Base quantity
using none = quantity_t<>; // has no associated quantity
using mass = quantity_t<1>;
using length = quantity_t<0,1>;
using distance = length;
using time = quantity_t<0,0,1>;
using current = quantity_t<0,0,0,1>; // electric current
using temperature = quantity_t<0,0,0,0,1>;
using luminous_intensity = quantity_t<0,0,0,0,0,1>;
using substance = quantity_t<0,0,0,0,0,0,1>; // amount of substance
using angle = quantity_t<0,0,0,0,0,0,0,1>;

//------------------------------------------------------------------------------
/// Derived quantity
using solid_angle = power_quantity<angle,2>;
using frequency = reciprocal_quantity<time>;
using force = divide_quantity<multiply_quantity<mass, length>, power_quantity<time,2>>;
using weight = force;
using area = power_quantity<length,2>;
using pressure = divide_quantity<force, area>;
using stress = pressure;
using energy = multiply_quantity<force, length>;
using work = energy;
using power = divide_quantity<energy, time>;
using charge = multiply_quantity<current, time>;
using voltage = divide_quantity<power, current>;
using capacitance = divide_quantity<charge, voltage>;
using impedance = divide_quantity<voltage, current>;
using conductance = reciprocal_quantity<impedance>;
using magnetic_flux = divide_quantity<energy, current>;
using magnetic_flux_density = divide_quantity<magnetic_flux, area>;
using inductance = divide_quantity<magnetic_flux, current>;
using luminous_flux = multiply_quantity<luminous_intensity, solid_angle>;
using illuminance = divide_quantity<luminous_intensity, area>;

} // end of namespace si
