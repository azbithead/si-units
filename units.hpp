#include <type_traits>

namespace si
{

//------------------------------------------------------------------------------
/// Units
template
<
    int Mass = 0,
    int Length = 0,
    int Time = 0,
    int Current = 0,
    int Temperature = 0,
    int Light = 0,
    int Angle = 0
>
struct units
{
    using mass = std::integral_constant<int, Mass>;
    using length = std::integral_constant<int, Length>;
    using time = std::integral_constant<int, Time>;
    using current = std::integral_constant<int, Current>;
    using temperature = std::integral_constant<int, Temperature>;
    using light = std::integral_constant<int, Light>;
    using angle = std::integral_constant<int, Angle>;

    static constexpr int mass_exp() { return mass::value; }
    static constexpr int length_exp() { return length::value; }
    static constexpr int time_exp() { return time::value; }
    static constexpr int current_exp() { return current::value; }
    static constexpr int temperature_exp() { return temperature::value; }
    static constexpr int light_exp() { return light::value; }
    static constexpr int angle_exp() { return angle::value; }
};

template< typename T >
struct is_units_impl : std::false_type {};

template
<
    int Mass,
    int Length,
    int Time,
    int Current,
    int Temperature,
    int Light,
    int Angle
>
struct is_units_impl
<
    units
    <
        Mass,
        Length,
        Time,
        Current,
        Temperature,
        Light,
        Angle
    >
> : std::true_type {};

//------------------------------------------------------------------------------
/// Test a type to see if it is a units type.
/// @param T the type to be tested
/// @return true if T is a units type, false otherwise
template <typename T>
constexpr bool is_units = is_units_impl<typename std::remove_cv<T>::type>::value;

template< typename Units1, typename Units2 >
struct multiply_units_impl
{
    using type = units
    <
        Units1::mass::value + Units2::mass::value,
        Units1::length::value + Units2::length::value,
        Units1::time::value + Units2::time::value,
        Units1::current::value + Units2::current::value,
        Units1::temperature::value + Units2::temperature::value,
        Units1::light::value + Units2::light::value,
        Units1::angle::value + Units2::angle::value
    >;
};

//------------------------------------------------------------------------------
/// The product of two si::units types is an si::units type where the members are memberwise summed.
/// @param Units1 an si::units type to be multiplied
/// @param Units2 an si::units type to be multiplied
/// @return the si::units type that is the product of Units1 and Units2
template< typename Units1, typename Units2 >
using multiply_units = typename multiply_units_impl< Units1, Units2 >::type;

template< typename Units, int Power >
struct exponentiate_units_impl
{
    using type = units
    <
        Units::mass::value * Power,
        Units::length::value * Power,
        Units::time::value * Power,
        Units::current::value * Power,
        Units::temperature::value * Power,
        Units::light::value * Power,
        Units::angle::value * Power
    >;
};

//------------------------------------------------------------------------------
/// The exponentiation of an si::unit type with an integer is an si::units type where the members are multiplied by the exponent.
/// @param Units an si::units type that is the base
/// @param Power an int that is the exponent
/// @param the si::units type that is the exponentiation of Units by Power
template< typename Units, int Power >
using exponentiate_units = typename exponentiate_units_impl< Units, Power >::type;

//------------------------------------------------------------------------------
/// The reciprocal of an si::units type is an si::units type where the members are negated.
/// @param Units an si::units type
/// @return the si::units type that is the reciprocal of Units
template< typename Units >
using reciprocal_units = exponentiate_units< Units, -1 >;

//------------------------------------------------------------------------------
/// The quotient of two si::units types is an si::units type where the members are memberwise subtracted.
/// @param Units1 an si::units type that is the dividend
/// @param Units2 an si::units type that is the divisor
/// @return the si::units type that is the quotient of dividing Units1 by Units2
template< typename Units1, typename Units2 >
using divide_units = multiply_units< Units1, reciprocal_units< Units2 > >;

//------------------------------------------------------------------------------
/// Base units
using scalar =    units<>; // unitless
using kilograms = units<1>;
using meters =    units<0,1>;
using seconds =   units<0,0,1>;
using amperes =   units<0,0,0,1>;
using degreesk =  units<0,0,0,0,1>;
using candelas =  units<0,0,0,0,0,1>;
using radians =   units<0,0,0,0,0,0,1>;

} // end of namespace si
