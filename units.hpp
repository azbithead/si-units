#include <type_traits>

namespace si
{

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
};

/// is_units
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

template <typename T>
constexpr bool is_units = is_units_impl<T>::value;

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

template< typename Units1, typename Units2 >
using multiply_units = typename multiply_units_impl< Units1, Units2 >::type;

template< typename Units >
struct reciprocal_units_impl
{
    using type = units
    <
        -Units::mass::value,
        -Units::length::value,
        -Units::time::value,
        -Units::current::value,
        -Units::temperature::value,
        -Units::light::value,
        -Units::angle::value
    >;
};

template< typename Units >
using reciprocal_units = typename reciprocal_units_impl< Units >::type;

template< typename Units1, typename Units2 >
using divide_units = multiply_units< Units1, reciprocal_units< Units2 > >;

using scalar =    units<>; // unitless
using kilograms = units<1>;
using meters =    units<0,1>;
using seconds =   units<0,0,1>;
using amperes =   units<0,0,0,1>;
using degreesk =  units<0,0,0,0,1>;
using candelas =  units<0,0,0,0,0,1>;
using radians =   units<0,0,0,0,0,0,1>;

} // end of namespace si
