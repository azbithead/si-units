#pragma once
#include <cstdint>
#include <type_traits>
#include <ratio>
#include <string>
#include "exponent.hpp"
#include "constants.hpp"

namespace si
{

//------------------------------------------------------------------------------
/// quantity_t
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
    using mass = exponent_t<aMassExponent>;
    using length = exponent_t<aLengthExponent>;
    using time = exponent_t<aTimeExponent>;
    using current = exponent_t<aCurrentExponent>;
    using temperature = exponent_t<aTemperatureExponent>;
    using luminous_intensity = exponent_t<aLuminousIntensityExponent>;
    using substance = exponent_t<aSubstanceExponent>;
    using angle = exponent_t<aAngleExponent>;
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
/// true if aType is an quantity_t type, false otherwise
/// @tparam aType the type to be tested
template <typename aType>
constexpr bool is_quantity = is_quantity_impl<typename std::remove_cv<aType>::type>::value;

template< typename... >
struct multiply_quantity_impl;

template< typename First, typename... Rest >
struct multiply_quantity_impl< First, Rest... >
{
    using previous = multiply_quantity_impl<Rest...>;

    using type = quantity_t
    <
        multiply_exponent<typename First::mass,typename previous::type::mass>::value,
        multiply_exponent<typename First::length,typename previous::type::length>::value,
        multiply_exponent<typename First::time,typename previous::type::time>::value,
        multiply_exponent<typename First::current,typename previous::type::current>::value,
        multiply_exponent<typename First::temperature,typename previous::type::temperature>::value,
        multiply_exponent<typename First::luminous_intensity,typename previous::type::luminous_intensity>::value,
        multiply_exponent<typename First::substance,typename previous::type::substance>::value,
        multiply_exponent<typename First::angle,typename previous::type::angle>::value
    >;
};

template<>
struct multiply_quantity_impl<>
{
    using type = quantity_t<>;
};

//------------------------------------------------------------------------------
/// the quantity_t type that is the product of a list of quantity_t's
template< typename... Quantities >
using multiply_quantity = typename multiply_quantity_impl< Quantities... >::type;

template< typename Quantity, typename aPower >
struct exponentiate_quantity_impl
{
    using type = quantity_t
    <
        std::ratio_multiply<std::ratio<Quantity::mass::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Quantity::length::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Quantity::time::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Quantity::current::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Quantity::temperature::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Quantity::luminous_intensity::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Quantity::substance::value>, aPower>::num,
        std::ratio_multiply<std::ratio<Quantity::angle::value>, aPower>::num
    >;
};

//------------------------------------------------------------------------------
/// the quantity_t type that is aQuantity raised to aPower
/// @tparam aQuantity a quantity_t type
/// @tparam aPower the power to which aQuantity is raised
template< typename aQuantity, std::intmax_t aPower >
using power_quantity = typename exponentiate_quantity_impl<aQuantity, std::ratio<aPower>>::type;

//------------------------------------------------------------------------------
/// the quantity_t type that is the reciprocal of aQuantity
/// @tparam aQuantity a quantity_t type
template< typename aQuantity >
using reciprocal_quantity = typename exponentiate_quantity_impl<aQuantity, std::ratio<-1>>::type;

//------------------------------------------------------------------------------
/// the quantity_t type that is the quotient of dividing aDividend by aDivisor
/// @tparam aDividend a quantity_t type
/// @tparam aDivisor a quantity_t type
template< typename aDividend, typename aDivisor >
using divide_quantity = multiply_quantity< aDividend, reciprocal_quantity< aDivisor > >;

//------------------------------------------------------------------------------
/// the quantity_t type that is the root aRoot of aQuantity
/// @tparam aQuantity a quantity_t type
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

template< typename CharT, typename QuantityT >
constexpr const CharT* abbrev = nullptr;

#define ABBREV_CONST(qt, str) \
template<> constexpr const char* abbrev<char,qt> = str; \
template<> constexpr const wchar_t* abbrev<wchar_t,qt> = L##str; \
template<> constexpr const char16_t* abbrev<char16_t,qt> = u##str; \
template<> constexpr const char32_t* abbrev<char32_t,qt> = U##str

ABBREV_CONST(none, "");
ABBREV_CONST(mass, "kg");
ABBREV_CONST(length, "m");
ABBREV_CONST(time, "s");
ABBREV_CONST(current, "A");
ABBREV_CONST(temperature, "K");
ABBREV_CONST(luminous_intensity, "cd");
ABBREV_CONST(substance, "mol");
ABBREV_CONST(angle, "rad");
ABBREV_CONST(solid_angle, "sr");
ABBREV_CONST(frequency, "Hz");
ABBREV_CONST(force, "N");
ABBREV_CONST(pressure, "Pa");
ABBREV_CONST(energy, "J");
ABBREV_CONST(power, "W");
ABBREV_CONST(charge, "C");
ABBREV_CONST(voltage, "V");
ABBREV_CONST(capacitance, "F");
ABBREV_CONST(impedance, "\u2126");
ABBREV_CONST(conductance, "S");
ABBREV_CONST(magnetic_flux, "Wb");
ABBREV_CONST(magnetic_flux_density, "T");
ABBREV_CONST(inductance, "H");
ABBREV_CONST(luminous_flux, "lm");
ABBREV_CONST(illuminance, "lx");

template< typename Exp, typename CharT >
inline
std::basic_string<CharT>
basic_string_from_exp
(
    const CharT* const aAbbreviation,
    std::basic_string<CharT> aString = std::basic_string<CharT>{}
)
{
    if( Exp::value > 0 )
    {
        if( !aString.empty() )
        {
            aString += multiply_operator<CharT>;
        }

        aString += aAbbreviation;

        if( Exp::value > 1 )
        {
            aString += si::basic_string_from<CharT>(Exp{});
        }
    }

    return aString;
}

STRING_CONST(one, "1");

template
<
    typename CharT,
    std::intmax_t M,
    std::intmax_t L,
    std::intmax_t T,
    std::intmax_t C,
    std::intmax_t Temp,
    std::intmax_t Lum,
    std::intmax_t S,
    std::intmax_t A
>
struct basic_string_from_impl<CharT, quantity_t<M,L,T,C,Temp,Lum,S,A>>
{
    std::basic_string<CharT>
    operator()
    (
        quantity_t<M,L,T,C,Temp,Lum,S,A> aQuantity
    )
    {
        using Q_t = quantity_t<M,L,T,C,Temp,Lum,S,A>;
        if( abbrev<CharT, Q_t> != nullptr )
        {
            return abbrev<CharT, Q_t>;
        }

        auto theNum = basic_string_from_exp<typename Q_t::mass>( abbrev<CharT,mass> );
        theNum = basic_string_from_exp<typename Q_t::length>( abbrev<CharT,length>, std::move(theNum) );
        theNum = basic_string_from_exp<typename Q_t::time>( abbrev<CharT,time>, std::move(theNum) );
        theNum = basic_string_from_exp<typename Q_t::current>( abbrev<CharT,current>, std::move(theNum) );
        theNum = basic_string_from_exp<typename Q_t::temperature>( abbrev<CharT,temperature>, std::move(theNum) );
        theNum = basic_string_from_exp<typename Q_t::luminous_intensity>( abbrev<CharT,luminous_intensity>, std::move(theNum) );
        theNum = basic_string_from_exp<typename Q_t::substance>( abbrev<CharT,substance>, std::move(theNum) );
        theNum = basic_string_from_exp<typename Q_t::angle>( abbrev<CharT,angle>, std::move(theNum) );

        using Recip_t = reciprocal_quantity<Q_t>;

        auto theDen = basic_string_from_exp<typename Recip_t::mass>( abbrev<CharT,mass> );
        theDen = basic_string_from_exp<typename Recip_t::length>( abbrev<CharT,length>, std::move(theDen) );
        theDen = basic_string_from_exp<typename Recip_t::time>( abbrev<CharT,time>, std::move(theDen) );
        theDen = basic_string_from_exp<typename Recip_t::current>( abbrev<CharT,current>, std::move(theDen) );
        theDen = basic_string_from_exp<typename Recip_t::temperature>( abbrev<CharT,temperature>, std::move(theDen) );
        theDen = basic_string_from_exp<typename Recip_t::luminous_intensity>( abbrev<CharT,luminous_intensity>, std::move(theDen) );
        theNum = basic_string_from_exp<typename Recip_t::substance>( abbrev<CharT,substance>, std::move(theNum) );
        theDen = basic_string_from_exp<typename Recip_t::angle>( abbrev<CharT,angle>, std::move(theDen) );

        if( theDen.empty() )
        {
            return theNum;
        }
        else
        {
            if( theNum.empty() )
            {
                theNum = one<CharT>;
            }

            return theNum + divide_operator<CharT> + theDen;
        }

        return theNum;
    }
};

template
<
    typename CharT,
    std::intmax_t M,
    std::intmax_t L,
    std::intmax_t T,
    std::intmax_t C,
    std::intmax_t Te,
    std::intmax_t Li,
    std::intmax_t S,
    std::intmax_t A
>
inline
std::basic_ostream<CharT>&
operator <<
(
    std::basic_ostream<CharT>& aStream,
    quantity_t<M,L,T,C,Te,Li,S,A> aQuantity
)
{
    return aStream << basic_string_from<CharT>(aQuantity);
}

} // end of namespace si
