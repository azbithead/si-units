#pragma once
#include <type_traits>
#include <ratio>
#include <limits>
#include <cmath>
#include <chrono>
#include <string>
#include <ostream>

#include "quantity.hpp"
#include "ratio.hpp"

namespace si
{

// Forward declaration
template <typename ValueT, typename IntervalT, typename QuantityT > class units_t;

} // end of namespace si

//------------------------------------------------------------------------------
/// Specialization of std::common_type for units_t.
template
<
    typename QuantityT,
    typename ValueT1,
    typename IntervalT1,
    typename ValueT2,
    typename IntervalT2
>
struct std::common_type
<
    si::units_t<ValueT1, IntervalT1, QuantityT>,
    si::units_t<ValueT2, IntervalT2, QuantityT>
>
{
    using type = si::units_t
    <
        std::common_type_t<ValueT1, ValueT2>,
        si::ratio_gcd<IntervalT1, IntervalT2>,
        QuantityT
    >;
};

namespace si
{

template
<
    typename FromUnitsT,
    typename ToUnitsT,
    typename IntervalT = typename std::ratio_divide
    <
        typename FromUnitsT::interval_t,
        typename ToUnitsT::interval_t
    >::type,
    bool = IntervalT::num == 1,
    bool = IntervalT::den == 1>
struct units_cast_impl;

template
<
    typename FromUnitsT,
    typename ToUnitsT,
    typename IntervalT
>
struct units_cast_impl<FromUnitsT, ToUnitsT, IntervalT, true, true>
{
    constexpr
    ToUnitsT operator()(FromUnitsT aFromUnits) const
    {
        return ToUnitsT
        {
            static_cast<typename ToUnitsT::value_t>(aFromUnits.value())
        };
    }
};

template
<
    typename FromUnitsT,
    typename ToUnitsT,
    typename IntervalT
>
struct units_cast_impl<FromUnitsT, ToUnitsT, IntervalT, true, false>
{
    constexpr
    ToUnitsT operator()(FromUnitsT aFromUnits) const
    {
        using ResultValue_t = std::common_type_t
        <
            typename ToUnitsT::value_t,
            typename FromUnitsT::value_t,
            intmax_t
        >;
        return ToUnitsT
        {
            static_cast<typename ToUnitsT::value_t>
            (
                static_cast<ResultValue_t>(aFromUnits.value()) /
                static_cast<ResultValue_t>(IntervalT::den)
            )
        };
    }
};

template
<
    typename FromUnitsT,
    typename ToUnitsT,
    typename IntervalT
>
struct units_cast_impl<FromUnitsT, ToUnitsT, IntervalT, false, true>
{
    constexpr
    ToUnitsT operator()(FromUnitsT aFromUnits) const
    {
        using ResultValue_t = std::common_type_t
        <
            typename ToUnitsT::value_t,
            typename FromUnitsT::value_t,
            intmax_t
        >;
        return ToUnitsT
        {
            static_cast<typename ToUnitsT::value_t>
            (
                static_cast<ResultValue_t>(aFromUnits.value()) *
                static_cast<ResultValue_t>(IntervalT::num)
            )
        };
    }
};

template
<
    typename FromUnitsT,
    typename ToUnitsT,
    typename IntervalT
>
struct units_cast_impl<FromUnitsT, ToUnitsT, IntervalT, false, false>
{
    constexpr
    ToUnitsT operator()(FromUnitsT aFromUnits) const
    {
        using ResultValue_t = std::common_type_t
        <
            typename ToUnitsT::value_t,
            typename FromUnitsT::value_t,
            intmax_t
        >;
        return ToUnitsT
        {
            static_cast<typename ToUnitsT::value_t>
            (
                static_cast<ResultValue_t>(aFromUnits.value()) *
                static_cast<ResultValue_t>(IntervalT::num) /
                static_cast<ResultValue_t>(IntervalT::den)
            )
        };
    }
};

template <typename aType>
struct is_units_impl : std::false_type {};

template <typename QuantityT, typename ValueT, typename IntervalT>
struct is_units_impl<units_t<ValueT, IntervalT, QuantityT>> : std::true_type {};

//------------------------------------------------------------------------------
/// true if aType is a units_t, false otherwise
template <typename aType>
constexpr bool is_units_t = is_units_impl<typename std::decay<aType>::type>::value;

//------------------------------------------------------------------------------
/// Convert a units_t to another units_t type.
/// Both types must have the same quantity_t type.
template <typename ToUnitsT, typename QuantityT, typename ValueT, typename IntervalT>
inline
constexpr
typename std::enable_if
<
    is_units_t<ToUnitsT> && std::is_same<typename ToUnitsT::quantity_t,QuantityT>::value,
    ToUnitsT
>::type
units_cast
(
    units_t<ValueT, IntervalT, QuantityT> aFromUnits
)
{
    return units_cast_impl
    <
        decltype(aFromUnits),
        ToUnitsT
    >{}(aFromUnits);
}

//------------------------------------------------------------------------------
/// Convert a std::chrono::duration to si::seconds.
template <typename ToUnitsT, typename REP, typename PERIOD>
inline
constexpr
typename std::enable_if
<
    is_units_t<ToUnitsT> && std::is_same<typename ToUnitsT::quantity_t,si::time>::value,
    ToUnitsT
>::type
units_cast
(
    std::chrono::duration<REP, PERIOD> aFromDuration
)
{
    using FromUnitsT = units_t<REP, PERIOD, si::time>;
    return units_cast_impl
    <
        FromUnitsT,
        ToUnitsT
    >{}(FromUnitsT{aFromDuration.count()});
}

template <typename aType>
struct is_duration : std::false_type {};

template <typename REP, typename PERIOD>
struct is_duration<std::chrono::duration<REP, PERIOD>> : std::true_type {};

//------------------------------------------------------------------------------
/// Convert an si::seconds to a std::chrono::duration.
template<typename ToDurationT, typename ValueT, typename IntervalT>
inline
constexpr
typename std::enable_if
<
    is_duration<ToDurationT>::value,
    ToDurationT
>::type
duration_cast
(
    units_t<ValueT, IntervalT, si::time> aUnits
)
{
    using Result_t = units_t<typename ToDurationT::rep, typename ToDurationT::period, si::time>;
    return ToDurationT{units_cast<Result_t>(aUnits).value()};
}

// some special units_t values
template <typename ValueT>
struct units_values
{
public:
    static constexpr ValueT zero() {return ValueT(0);}
    static constexpr ValueT max()  {return std::numeric_limits<ValueT>::max();}
    static constexpr ValueT min()  {return std::numeric_limits<ValueT>::lowest();}
};

// This is coming in c++ 17 but we don't have that yet
template< class From, class To >
constexpr bool is_convertible_v = std::is_convertible<From, To>::value;

//------------------------------------------------------------------------------
/// Class units_t represents a numeric value with associated SI units.
template <typename ValueT, typename IntervalT, typename QuantityT>
class units_t
{
    static_assert(std::is_arithmetic<ValueT>::value, "ValueT must be an arithmetic type");
    static_assert(is_ratio<IntervalT>, "IntervalT must be of type std::ratio");
    static_assert(std::ratio_greater<IntervalT, r_zero>::value, "IntervalT must be positive");
    static_assert(is_quantity<QuantityT>, "QuantityT must be of type si::quantity_t" );

    template <typename _R1, typename _R2>
    struct no_overflow
    {
    private:
        static constexpr intmax_t num_gcd = gcd<_R1::num, _R2::num>;
        static constexpr intmax_t den_gcd = gcd<_R1::den, _R2::den>;
        static constexpr intmax_t num1 = _R1::num / num_gcd;
        static constexpr intmax_t den1 = _R1::den / den_gcd;
        static constexpr intmax_t num2 = _R2::num / num_gcd;
        static constexpr intmax_t den2 = _R2::den / den_gcd;
        static constexpr intmax_t max = -((intmax_t(1) << (sizeof(intmax_t) * CHAR_BIT - 1)) + 1);

        template <intmax_t aX, intmax_t aY, bool isOverflow>
        struct multiply    // isOverflow == false
        {
            static constexpr intmax_t value = aX * aY;
        };

        template <intmax_t aX, intmax_t aY>
        struct multiply<aX, aY, true>
        {
            static constexpr intmax_t value = 1;
        };

    public:
        static constexpr bool value = (num1 <= max / den2) && (num2 <= max / den1);
        using type = std::ratio
        <
            multiply<num1, den2, !value>::value,
            multiply<num2, den1, !value>::value
        >;
    };

public:

    //--------------------------------------------------------------------------
    /// Type aliases
    using quantity_t = QuantityT;
    using value_t = ValueT;
    using interval_t = typename IntervalT::type;


    //--------------------------------------------------------------------------
    /// Static member constants
    static constexpr auto interval = interval_t{};
    static constexpr auto quantity = quantity_t{};

    //--------------------------------------------------------------------------
    constexpr
    units_t
    (
    ) = default;

    //--------------------------------------------------------------------------
    constexpr
    units_t
    (
        const units_t&
    ) = default;

    //--------------------------------------------------------------------------
    /// Initialize a units_t from a unitless value.
    /// This constructor will not be chosen by the compiler if it would result in loss of precision.
    /// @param aValue the scalar value that will be stored in this object
    template <typename ValueT2>
    constexpr
    explicit
    units_t
    (
        ValueT2 aValue,
        typename std::enable_if
        <
            std::is_arithmetic<ValueT2>::value &&
            (
                std::is_floating_point<value_t>::value ||
                !std::is_floating_point<ValueT2>::value
            )
        >::type* = nullptr
    )
    : mValue{static_cast<value_t>(aValue)}
    {
    }

    //--------------------------------------------------------------------------
    /// Initialize a units_t from another units_t possibly having different value_t and interval_t types but the same quantity_t type.
    /// This constructor will not be chosen by the compiler if it would result in overflow or loss of precision.
    /// @param aUnits the units_t that will be converted to this units_t
    template <typename ValueT2, typename IntervalT2>
    constexpr
    units_t
    (
        units_t<ValueT2, IntervalT2, QuantityT> aUnits,
        typename std::enable_if
        <
            no_overflow<IntervalT2, interval_t>::value &&
            (
                std::is_floating_point<value_t>::value ||
                (
                    no_overflow<IntervalT2, interval_t>::type::den == 1 &&
                    !std::is_floating_point<ValueT2>::value
                )
            )
        >::type* = nullptr
    )
    : mValue{units_cast<units_t>(aUnits).value()}
    {
    }

    //--------------------------------------------------------------------------
    constexpr
    auto
    scalar
    (
    )
    {
        return units_t<ValueT, IntervalT, none>{mValue};
    }

    //--------------------------------------------------------------------------
    // Accessor function
    constexpr value_t value() const {return mValue;}

    //--------------------------------------------------------------------------
    // Arithmetic functions
    constexpr units_t operator+() const {return *this;}
    constexpr units_t operator-() const {return units_t{-mValue};}
    constexpr units_t& operator++() {++mValue; return *this;}
    constexpr units_t operator++(int) {return units_t{mValue++};}
    constexpr units_t& operator--() {--mValue; return *this;}
    constexpr units_t operator--(int) {return units_t{mValue--};}
    constexpr units_t& operator+=(units_t rhs) {mValue += rhs.value(); return *this;}
    constexpr units_t& operator-=(units_t rhs) {mValue -= rhs.value(); return *this;}
    constexpr units_t& operator*=(value_t rhs) {mValue *= rhs; return *this;}
    constexpr units_t& operator/=(value_t rhs) {mValue /= rhs; return *this;}
    constexpr units_t& operator%=(value_t rhs) {mValue %= rhs; return *this;}
    constexpr units_t& operator%=(units_t rhs) {mValue %= rhs.value(); return *this;}

    //--------------------------------------------------------------------------
    // Special values
    static constexpr units_t zero() {return units_t{units_values<value_t>::zero()};}
    static constexpr units_t min() {return units_t{units_values<value_t>::min()};}
    static constexpr units_t max() {return units_t{units_values<value_t>::max()};}

private:

    value_t mValue;

}; // end of class units_t

template< typename... >
struct multiply_units_impl;

template< typename First, typename... Rest >
struct multiply_units_impl< First, Rest... >
{
    using previous = multiply_units_impl<Rest...>;

    using type = units_t
    <
        std::common_type_t<typename First::value_t, typename previous::type::value_t>,
        std::ratio_multiply<typename First::interval_t, typename previous::type::interval_t>,
        multiply_quantity<typename First::quantity_t, typename previous::type::quantity_t>
    >;
};

template< typename UnitsT >
struct multiply_units_impl<UnitsT>
{
    using type = units_t
    <
        typename UnitsT::value_t,
        typename UnitsT::interval_t,
        typename UnitsT::quantity_t
    >;
};

//------------------------------------------------------------------------------
template< typename... UnitsT >
using multiply_units = typename multiply_units_impl< UnitsT... >::type;

//------------------------------------------------------------------------------
template< typename Units1, typename Units2 >
using divide_units = si::units_t
<
    std::common_type_t<typename Units1::value_t, typename Units2::value_t>,
    std::ratio_divide<typename Units1::interval_t, typename Units2::interval_t>,
    si::divide_quantity<typename Units1::quantity_t, typename Units2::quantity_t>
>;

template< typename RatioT, std::intmax_t Power >
struct power_ratio_impl
{
    using previous = power_ratio_impl<RatioT, Power - 1>;
    using type = std::ratio_multiply<RatioT, typename previous::type>;
};

template< typename RatioT >
struct power_ratio_impl<RatioT, 0>
{
    using type = r_one;
};

template< typename UnitsT, std::intmax_t Power >
struct power_units_impl
{
    using type = units_t
    <
        typename UnitsT::value_t,
        typename power_ratio_impl<typename UnitsT::interval_t, Power>::type,
        power_quantity<typename UnitsT::quantity_t, Power>
    >;
};

//------------------------------------------------------------------------------
template< typename UnitsT, std::intmax_t Power >
using power_units = typename power_units_impl<UnitsT, Power>::type;

//------------------------------------------------------------------------------
template< typename UnitsT >
using reciprocal_units = divide_units<units_t<typename UnitsT::value_t, r_one, none>, UnitsT>;

//==============================================================================
// Some useful units_t types
template< typename IntervalT = r_one, typename ValueT = double >
using scalar = units_t<ValueT, IntervalT, none>;

template< typename IntervalT = r_one, typename ValueT = double >
using meters = units_t<ValueT, IntervalT, length>;

template< typename IntervalT = r_one, typename ValueT = double >
using kilograms = units_t<ValueT, IntervalT, mass>;

template< typename ValueT = double >
using grams = units_t<ValueT, std::milli, mass>;

template< typename IntervalT = r_one, typename ValueT = double >
using seconds = units_t<ValueT, IntervalT, time>;

template< typename ValueT = double >
using minutes = seconds<std::ratio<60>, ValueT>;

template< typename ValueT = double >
using hours = seconds<std::ratio<60*60>, ValueT>;

template< typename ValueT = double >
using days = seconds<std::ratio<24*60*60>, ValueT>;

template< typename ValueT = double >
using milliseconds = seconds<std::milli, ValueT>;

template< typename ValueT = double >
using microseconds = seconds<std::micro, ValueT>;

template< typename ValueT = double >
using nanoseconds = seconds<std::nano, ValueT>;

template< typename IntervalT = r_one, typename ValueT = double >
using amperes = units_t<ValueT, IntervalT, current>;

template< typename IntervalT = r_one, typename ValueT = double >
using kelvins = units_t<ValueT, IntervalT, temperature>;

template< typename IntervalT = r_one, typename ValueT = double >
using candelas = units_t<ValueT, IntervalT, luminous_intensity>;

template< typename IntervalT = r_one, typename ValueT = double >
using moles = units_t<ValueT, IntervalT, substance>;

template< typename IntervalT = r_one, typename ValueT = double >
using radians = units_t<ValueT, IntervalT, angle>;

template< typename IntervalT = r_one, typename ValueT = double >
using steradians = units_t<ValueT, IntervalT, solid_angle>;

template< typename IntervalT = r_one, typename ValueT = double >
using hertz = units_t<ValueT, IntervalT, frequency>;

template< typename IntervalT = r_one, typename ValueT = double >
using newtons = units_t<ValueT, IntervalT, force>;

template< typename IntervalT = r_one, typename ValueT = double >
using coulombs = units_t<ValueT, IntervalT, charge>;

template< typename IntervalT = r_one, typename ValueT = double >
using lux = units_t<ValueT, IntervalT, illuminance>;

template< typename IntervalT = r_one, typename ValueT = double >
using pascals = units_t<ValueT, IntervalT, pressure>;

template< typename IntervalT = r_one, typename ValueT = double >
using joules = units_t<ValueT, IntervalT, energy>;

template< typename IntervalT = r_one, typename ValueT = double >
using watts = units_t<ValueT, IntervalT, power>;

template< typename IntervalT = r_one, typename ValueT = double >
using volts = units_t<ValueT, IntervalT, voltage>;

template< typename IntervalT = r_one, typename ValueT = double >
using farads = units_t<ValueT, IntervalT, capacitance>;

template< typename IntervalT = r_one, typename ValueT = double >
using ohms = units_t<ValueT, IntervalT, impedance>;

template< typename IntervalT = r_one, typename ValueT = double >
using siemens = units_t<ValueT, IntervalT, conductance>;

template< typename IntervalT = r_one, typename ValueT = double >
using webers = units_t<ValueT, IntervalT, magnetic_flux>;

template< typename IntervalT = r_one, typename ValueT = double >
using teslas = units_t<ValueT, IntervalT, magnetic_flux_density>;

template< typename IntervalT = r_one, typename ValueT = double >
using henries = units_t<ValueT, IntervalT, inductance>;

template< typename IntervalT = r_one, typename ValueT = double >
using lumens = units_t<ValueT, IntervalT, luminous_flux>;

template <typename LhsUnitsT, typename RhsUnitsT>
struct units_eq_impl
{
    constexpr
    bool operator()(LhsUnitsT aLHS, RhsUnitsT aRHS) const
    {
        using CommonUnits_t = std::common_type_t<LhsUnitsT, RhsUnitsT>;
        return CommonUnits_t{aLHS}.value() == CommonUnits_t{aRHS}.value();
    }
};

template <typename LhsUnitsT>
struct units_eq_impl<LhsUnitsT, LhsUnitsT>
{
    constexpr
    bool operator()(LhsUnitsT aLHS, LhsUnitsT aRHS) const
    {
        return aLHS.value() == aRHS.value();
    }
};

template <typename LhsUnitsT, typename RhsUnitsT>
struct units_lt_impl
{
    constexpr
    bool operator()(LhsUnitsT aLHS, RhsUnitsT aRHS) const
    {
        using CommonUnits_t = std::common_type_t<LhsUnitsT, RhsUnitsT>;
        return CommonUnits_t{aLHS}.value() < CommonUnits_t{aRHS}.value();
    }
};

template <typename LhsUnitsT>
struct units_lt_impl<LhsUnitsT, LhsUnitsT>
{
    constexpr
    bool operator()(LhsUnitsT aLHS, LhsUnitsT aRHS) const
    {
        return aLHS.value() < aRHS.value();
    }
};

//------------------------------------------------------------------------------
/// units_t ==
template <typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
inline
constexpr
bool
operator ==
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
)
{
    return units_eq_impl<decltype(aLHS), decltype(aRHS)>{}(aLHS, aRHS);
}

//------------------------------------------------------------------------------
// units_t !=
template <typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
inline
constexpr
bool
operator !=
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
)
{
    return !(aLHS == aRHS);
}

//------------------------------------------------------------------------------
// units_t <
template <typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
inline
constexpr
bool
operator <
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
)
{
    return units_lt_impl<decltype(aLHS),decltype(aRHS)>{}(aLHS, aRHS);
}

//------------------------------------------------------------------------------
// units_t >
template <typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
inline
constexpr
bool
operator >
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
)
{
    return aRHS < aLHS;
}

//------------------------------------------------------------------------------
// units_t <=
template <typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
inline
constexpr
bool
operator <=
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
)
{
    return !(aRHS < aLHS);
}

//------------------------------------------------------------------------------
// units_t >=
template <typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
inline
constexpr
bool
operator >=
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
)
{
    return !(aLHS < aRHS);
}

//------------------------------------------------------------------------------
// units_t +
template <typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
inline
constexpr
auto
operator +
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
)
{
    using CommonUnits_t = std::common_type_t<decltype(aLHS), decltype(aRHS)>;
    return CommonUnits_t{CommonUnits_t{aLHS}.value() + CommonUnits_t{aRHS}.value()};
}

//------------------------------------------------------------------------------
// units_t -
template <typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
inline
constexpr
auto
operator -
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
)
{
    return aLHS + (-aRHS);
}

//------------------------------------------------------------------------------
// units_t * units_t
template
<
    typename QuantityT1,
    typename ValueT1,
    typename IntervalT1,
    typename QuantityT2,
    typename ValueT2,
    typename IntervalT2
>
inline
constexpr
auto
operator *
(
    units_t<ValueT1, IntervalT1, QuantityT1> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT2> aRHS
)
{
    using ResultValue_t = std::common_type_t<ValueT1, ValueT2>;
    using Result_t = units_t
    <
        ResultValue_t,
        std::ratio_multiply<IntervalT1, IntervalT2>,
        multiply_quantity<QuantityT1, QuantityT2>
    >;

    return Result_t
    {
        static_cast<ResultValue_t>( aLHS.value() )
        *
        static_cast<ResultValue_t>( aRHS.value() )
    };
}

//------------------------------------------------------------------------------
// units_t * scalar
template <typename ValueT1, typename IntervalT, typename QuantityT, typename ValueT2>
inline
constexpr
auto
operator *
(
    units_t<ValueT1, IntervalT, QuantityT> aUnits,
    ValueT2 aScalar
)
{
    return aUnits * scalar<r_one, ValueT2>{aScalar};
}

//------------------------------------------------------------------------------
// scalar * units_t
template <typename ValueT1, typename IntervalT, typename QuantityT, typename ValueT2>
inline
constexpr
auto
operator *
(
    ValueT2 aScalar,
    units_t<ValueT1, IntervalT, QuantityT> aUnits
)
{
    return aUnits * aScalar;
}

template
<
    typename QuantityT1,
    typename ValueT1,
    typename IntervalT1,
    typename QuantityT2,
    typename ValueT2,
    typename IntervalT2
>
using diff_quantity_results_t = units_t
<
    std::common_type_t<ValueT1, ValueT2>,
    std::ratio_divide<IntervalT1, IntervalT2>,
    divide_quantity<QuantityT1, QuantityT2>
>;

//------------------------------------------------------------------------------
// divide units_t by units_t, different quantity_t
template
<
    typename QuantityT1,
    typename ValueT1,
    typename IntervalT1,
    typename QuantityT2,
    typename ValueT2,
    typename IntervalT2
>
inline
constexpr
typename std::enable_if
<
    !std::is_same<QuantityT1, QuantityT2>::value,
    diff_quantity_results_t<QuantityT1, ValueT1, IntervalT1, QuantityT2, ValueT2, IntervalT2>
>::type
operator /
(
    units_t<ValueT1, IntervalT1, QuantityT1> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT2> aRHS
)
{
    using Result_t = diff_quantity_results_t<QuantityT1, ValueT1, IntervalT1, QuantityT2, ValueT2, IntervalT2>;
    return Result_t{aLHS.value() / aRHS.value()};
}

//------------------------------------------------------------------------------
// divide units_t by units_t, same quantity_t
template
<
    typename QuantityT,
    typename ValueT1,
    typename IntervalT1,
    typename ValueT2,
    typename IntervalT2
>
inline
constexpr
auto
operator /
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
)
{
    using CommonUnits_t = std::common_type_t
    <
        units_t<ValueT1, IntervalT1, QuantityT>,
        units_t<ValueT2, IntervalT2, QuantityT>
    >;
    return CommonUnits_t{aLHS}.value() / CommonUnits_t{aRHS}.value();
}

//------------------------------------------------------------------------------
// divide units_t by scalar
template
<
    typename ValueT1,
    typename IntervalT,
    typename QuantityT,
    typename ValueT2
>
inline
constexpr
auto
operator /
(
    units_t<ValueT1, IntervalT, QuantityT> aUnits,
    ValueT2 aScalar
)
{
    using ResultValue_t = std::common_type_t<ValueT1, ValueT2>;
    return aUnits / scalar<r_one, ResultValue_t>{aScalar};
}

//------------------------------------------------------------------------------
// divide scalar by units_t
template
<
    typename ValueT1,
    typename IntervalT,
    typename QuantityT,
    typename ValueT2
>
inline
constexpr
auto
operator /
(
    ValueT2 aScalar,
    units_t<ValueT1, IntervalT, QuantityT> aUnits
)
{
    using ResultValue_t = std::common_type_t<ValueT1, ValueT2>;
    return scalar<r_one, ResultValue_t>{aScalar} / aUnits;
}

//------------------------------------------------------------------------------
// modulo units_t by scalar
template
<
    typename QuantityT,
    typename ValueT1,
    typename IntervalT,
    typename ValueT2
>
inline
constexpr
auto
operator%
(
    units_t<ValueT1, IntervalT, QuantityT> aUnits,
    ValueT2 aScalar
)
{
    using ResultValue_t = std::common_type_t<ValueT1, ValueT2>;
    using Result_t = units_t<ResultValue_t, IntervalT, QuantityT>;
    return Result_t{Result_t{aUnits}.value() % static_cast<ResultValue_t>(aScalar)};
}

//------------------------------------------------------------------------------
// modulo units_t by units_t
template
<
    typename ValueT1,
    typename IntervalT1,
    typename ValueT2,
    typename IntervalT2,
    typename QuantityT
>
inline
constexpr
auto
operator%
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
)
{
    using Result_t = std::common_type_t<decltype(aLHS), decltype(aRHS)>;
    return Result_t{Result_t{aLHS}.value() % Result_t{aRHS}.value()};
}

//------------------------------------------------------------------------------
// absolute value of a units_t
template
<
    typename ValueT,
    typename IntervalT,
    typename QuantityT
>
inline
constexpr
units_t<ValueT, IntervalT, QuantityT>
absolute
(
    units_t<ValueT, IntervalT, QuantityT> aUnits
)
{
    return units_t<ValueT, IntervalT, QuantityT>{std::abs(aUnits.value())};
}

//------------------------------------------------------------------------------
// floor of a units_t
template
<
    typename RESULT,
    typename ValueT,
    typename IntervalT,
    typename QuantityT,
    typename = std::enable_if_t<is_units_t<RESULT>>
>
inline
constexpr
RESULT
floor
(
    units_t<ValueT, IntervalT, QuantityT> aUnits
)
{
    auto theResult = units_cast<RESULT>(aUnits);
    if( theResult > aUnits )
    {
        theResult -= RESULT{static_cast<typename RESULT::value_t>(1)};
    }

    return RESULT{static_cast<typename RESULT::value_t>(std::floor(theResult.value()))};
}

//------------------------------------------------------------------------------
// ceiling of a units_t
template
<
    typename RESULT,
    typename ValueT,
    typename IntervalT,
    typename QuantityT,
    typename = std::enable_if_t<is_units_t<RESULT>>
>
inline
constexpr
RESULT
ceiling
(
    units_t<ValueT, IntervalT, QuantityT> aUnits
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
    typename ValueT,
    typename IntervalT,
    typename QuantityT,
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
    units_t<ValueT, IntervalT, QuantityT> aUnits
)
{
    RESULT t0 = floor<RESULT>(aUnits);
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

//------------------------------------------------------------------------------
// truncate a units_t
template
<
    typename RESULT,
    typename ValueT,
    typename IntervalT,
    typename QuantityT,
    typename = std::enable_if_t<is_units_t<RESULT>>
>
inline
constexpr
RESULT
truncate
(
    units_t<ValueT, IntervalT, QuantityT> aUnits
)
{
    auto theResult = units_cast<RESULT>(aUnits);
    return RESULT{static_cast<typename RESULT::value_t>(std::trunc(theResult.value()))};
}

template
<
    typename ValueT,
    typename IntervalT,
    typename QuantityT,
    typename EPSILON
>
using sqrt_result_t = typename std::enable_if
<
    std::is_floating_point<ValueT>::value,
    units_t
    <
        ValueT,
        typename ratio_sqrt<IntervalT, EPSILON>::type,
        root_quantity<QuantityT, 2>
    >
>::type;

//------------------------------------------------------------------------------
// square root of a units_t
template
<
    typename ValueT,
    typename IntervalT,
    typename QuantityT,
    typename EPSILON = std::ratio<1,10000000000000>
>
inline
sqrt_result_t<ValueT, IntervalT, QuantityT, EPSILON>
square_root
(
    units_t<ValueT, IntervalT, QuantityT> aQuantity
)
{
    using Result_t = sqrt_result_t<ValueT, IntervalT, QuantityT, EPSILON>;
    return Result_t{std::sqrt(aQuantity.value())};
}

template
<
    typename ValueT,
    typename IntervalT,
    typename QuantityT,
    std::intmax_t EXPONENT
>
struct power_result_impl;

template
<
    typename ValueT,
    typename IntervalT,
    typename QuantityT
>
struct power_result_impl<ValueT, IntervalT, QuantityT, 0>
{
    using type = scalar<r_one, ValueT>;
};

template
<
    typename ValueT,
    typename IntervalT,
    typename QuantityT,
    std::intmax_t EXPONENT
>
struct power_result_impl
{
    static_assert(EXPONENT >= 0, "si::exponentiate EXPONENT must be non-negative");
    using temp = typename power_result_impl<ValueT, IntervalT, QuantityT, EXPONENT-1>::type;
    using type = units_t
    <
        ValueT,
        std::ratio_multiply<IntervalT, typename temp::interval_t>,
        multiply_quantity<QuantityT, typename temp::quantity_t>
    >;
};

template
<
    typename ValueT,
    typename IntervalT,
    typename QuantityT,
    std::intmax_t EXPONENT
>
using power_result_t = typename power_result_impl<ValueT, IntervalT, QuantityT, EXPONENT>::type;

template< typename ValueT >
constexpr
inline
ValueT
value_pow
(
    ValueT aBase,
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
    typename ValueT,
    typename IntervalT,
    typename QuantityT
>
constexpr
inline
power_result_t<ValueT, IntervalT, QuantityT, EXPONENT>
exponentiate
(
    const units_t<ValueT, IntervalT, QuantityT>& aQuantity
)
{
    using Result_t = power_result_t<ValueT, IntervalT, QuantityT, EXPONENT>;
    return Result_t{value_pow(aQuantity.value(), EXPONENT)};
}

//------------------------------------------------------------------------------
// sine of radians
template
<
    typename ValueT,
    typename IntervalT
>
inline
scalar<>
sine
(
    radians<IntervalT, ValueT> aRadians
)
{
    const auto theBaseRadians = units_cast<radians<>>(aRadians);
    return scalar<>{std::sin(theBaseRadians.value())};
}

//------------------------------------------------------------------------------
// cosine of radians
template
<
    typename ValueT,
    typename IntervalT
>
inline
scalar<>
cosine
(
    radians<IntervalT, ValueT> aRadians
)
{
    const auto theBaseRadians = units_cast<radians<>>(aRadians);
    return scalar<>{std::cos(theBaseRadians.value())};
}

//------------------------------------------------------------------------------
// tangent of radians
template
<
    typename ValueT,
    typename IntervalT
>
inline
scalar<>
tangent
(
    radians<IntervalT, ValueT> aRadians
)
{
    const auto theBaseRadians = units_cast<radians<>>(aRadians);
    return scalar<>{std::tan(theBaseRadians.value())};
}

//------------------------------------------------------------------------------
// arc sine of scalar
template
<
    typename ValueT,
    typename IntervalT
>
inline
radians<>
arc_sine
(
    scalar<IntervalT, ValueT> aScalar
)
{
    const auto theBaseScalar = units_cast<scalar<>>(aScalar);
    return radians<>{std::asin(theBaseScalar.value())};
}

//------------------------------------------------------------------------------
// arc cosine of scalar
template
<
    typename ValueT,
    typename IntervalT
>
inline
radians<>
arc_cosine
(
    scalar<IntervalT, ValueT> aScalar
)
{
    const auto theBaseScalar = units_cast<scalar<>>(aScalar);
    return radians<>{std::acos(theBaseScalar.value())};
}

//------------------------------------------------------------------------------
// arc tangent of scalar
template
<
    typename ValueT,
    typename IntervalT
>
inline
radians<>
arc_tangent
(
    scalar<IntervalT, ValueT> aScalar
)
{
    const auto theBaseScalar = units_cast<scalar<>>(aScalar);
    return radians<>{std::atan(theBaseScalar.value())};
}

STRING_CONST(space, " ");

template
<
    typename CharT,
    typename ValueT,
    typename IntervalT,
    typename QuantityT
>
inline
std::basic_string<CharT>
basic_string_from
(
    units_t<ValueT, IntervalT, QuantityT> aUnits
)
{
    std::basic_string<CharT> theResult;
    if( aUnits.interval.num != aUnits.interval.den )
    {
        theResult = basic_string_from<CharT>(aUnits.interval);
    }

    const auto theQuantityString = basic_string_from<CharT>(aUnits.quantity);
    if( !theQuantityString.empty() )
    {
        if( !theResult.empty() )
        {
            theResult += space<CharT>;
        }

        theResult += theQuantityString;
    }

    return theResult;
}

//------------------------------------------------------------------------------
template
<
    typename CharT,
    typename ValueT,
    typename IntervalT
>
inline
std::basic_ostream<CharT>&
operator <<
(
    std::basic_ostream<CharT>& aStream,
    scalar<IntervalT, ValueT> aScalar
)
{
    if( aScalar.value() != 1 )
    {
        aStream << aScalar.value() << multiply_operator<CharT>;
    }

    return aStream << basic_string_from<CharT>(IntervalT{});
}

//------------------------------------------------------------------------------
template
<
    typename CharT,
    typename ValueT
>
inline
std::basic_ostream<CharT>&
operator <<
(
    std::basic_ostream<CharT>& aStream,
    scalar<si::r_one, ValueT> aScalar
)
{
    return aStream << aScalar.value();
}

//------------------------------------------------------------------------------
template
<
    typename CharT,
    typename ValueT,
    typename IntervalT,
    typename QuantityT
>
inline
std::basic_ostream<CharT>&
operator <<
(
    std::basic_ostream<CharT>& aStream,
    units_t<ValueT, IntervalT, QuantityT> aUnits
)
{
    return aStream << aUnits.scalar() << space<CharT> << QuantityT{};
}

namespace literals
{

#define literal( type, str ) \
constexpr type<> operator "" _##str \
( \
    long double aValue \
) \
{ \
    return type<>{aValue}; \
} \
 \
constexpr type<si::r_one,long long> operator "" _##str \
( \
    unsigned long long aValue \
) \
{ \
    return type<si::r_one,long long>{aValue}; \
}

#define literal2( type, str ) \
constexpr type<> operator "" _##str \
( \
    long double aValue \
) \
{ \
    return type<>{aValue}; \
} \
 \
constexpr type<long long> operator "" _##str \
( \
    unsigned long long aValue \
) \
{ \
    return type<long long>{aValue}; \
}

literal(meters,m);
literal(kilograms, kg);
literal2(grams, g);
literal(seconds, s);
literal2(minutes, min);
literal2(hours, h);
literal2(days, d);
literal2(milliseconds, ms);
literal2(microseconds, us);
literal2(nanoseconds, ns);
literal(amperes, A);
literal(kelvins, K);
literal(candelas, cd);
literal(radians, rad);
literal(moles, mol);
literal(steradians, sr);
literal(hertz, Hz);
literal(newtons, N);
literal(pascals, Pa);
literal(joules, J);
literal(watts, W);
literal(coulombs, C);
literal(volts, V);
literal(farads, F);
literal(ohms, O);
literal(siemens, S);
literal(webers, Wb);
literal(teslas, T);
literal(henries, H);
literal(lumens, lm);
literal(lux, lx);

} // end of namespace literals
} // end of namespace si

#include "string-from.hpp"

namespace std
{

template
<
    typename ValueT,
    typename IntervalT,
    typename QuantityT
>
struct hash<si::units_t<ValueT,IntervalT,QuantityT>>
{
    size_t
    operator()
    (
        si::units_t<ValueT,IntervalT,QuantityT> aUnits
    ) const
    {
        return hash<ValueT>{}(aUnits.value());
    }
};

} // end of namespace std
