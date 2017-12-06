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
template <typename VALUE, typename INTERVAL, typename QUANTITY > class units_t;

} // end of namespace si

//------------------------------------------------------------------------------
/// Specialization of std::common_type for units_t.
template
<
    typename QUANTITY,
    typename VALUE1,
    typename INTERVAL1,
    typename VALUE2,
    typename INTERVAL2
>
struct std::common_type
<
    si::units_t<VALUE1, INTERVAL1, QUANTITY>,
    si::units_t<VALUE2, INTERVAL2, QUANTITY>
>
{
    using type = si::units_t
    <
        std::common_type_t<VALUE1, VALUE2>,
        si::ratio_gcd<INTERVAL1, INTERVAL2>,
        QUANTITY
    >;
};

namespace si
{

template
<
    typename FromUnitsT,
    typename ToUnitsT,
    typename INTERVAL = typename std::ratio_divide
    <
        typename FromUnitsT::interval_t,
        typename ToUnitsT::interval_t
    >::type,
    bool = INTERVAL::num == 1,
    bool = INTERVAL::den == 1>
struct units_cast_impl;

template
<
    typename FromUnitsT,
    typename ToUnitsT,
    typename INTERVAL
>
struct units_cast_impl<FromUnitsT, ToUnitsT, INTERVAL, true, true>
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
    typename INTERVAL
>
struct units_cast_impl<FromUnitsT, ToUnitsT, INTERVAL, true, false>
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
                static_cast<ResultValue_t>(INTERVAL::den)
            )
        };
    }
};

template
<
    typename FromUnitsT,
    typename ToUnitsT,
    typename INTERVAL
>
struct units_cast_impl<FromUnitsT, ToUnitsT, INTERVAL, false, true>
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
                static_cast<ResultValue_t>(INTERVAL::num)
            )
        };
    }
};

template
<
    typename FromUnitsT,
    typename ToUnitsT,
    typename INTERVAL
>
struct units_cast_impl<FromUnitsT, ToUnitsT, INTERVAL, false, false>
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
                static_cast<ResultValue_t>(INTERVAL::num) /
                static_cast<ResultValue_t>(INTERVAL::den)
            )
        };
    }
};

template <typename aType>
struct is_units_impl : std::false_type {};

template <typename QUANTITY, typename VALUE, typename INTERVAL>
struct is_units_impl<units_t<VALUE, INTERVAL, QUANTITY>> : std::true_type {};

//------------------------------------------------------------------------------
/// true if aType is a units_t, false otherwise
/// @tparam aType a type to be tested
template <typename aType>
constexpr bool is_units_t = is_units_impl<typename std::decay<aType>::type>::value;

//------------------------------------------------------------------------------
/// Convert a units_t to another units_t type.
/// Both types must have the same quantity_t type.
template <typename ToUnitsT, typename QUANTITY, typename VALUE, typename INTERVAL>
inline
constexpr
typename std::enable_if
<
    is_units_t<ToUnitsT> && std::is_same<typename ToUnitsT::quantity_t,QUANTITY>::value,
    ToUnitsT
>::type
units_cast
(
    units_t<VALUE, INTERVAL, QUANTITY> aFromUnits
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
template<typename ToDurationT, typename VALUE, typename INTERVAL>
inline
constexpr
typename std::enable_if
<
    is_duration<ToDurationT>::value,
    ToDurationT
>::type
duration_cast
(
    units_t<VALUE, INTERVAL, si::time> aUnits
)
{
    using Result_t = units_t<typename ToDurationT::rep, typename ToDurationT::period, si::time>;
    return ToDurationT{units_cast<Result_t>(aUnits).value()};
}

// some special units_t values
template <typename VALUE>
struct units_values
{
public:
    static constexpr VALUE zero() {return VALUE(0);}
    static constexpr VALUE max()  {return std::numeric_limits<VALUE>::max();}
    static constexpr VALUE min()  {return std::numeric_limits<VALUE>::lowest();}
};

// This is coming in c++ 17 but we don't have that yet
template< class From, class To >
constexpr bool is_convertible_v = std::is_convertible<From, To>::value;

//------------------------------------------------------------------------------
/// Class units_t represents a numeric value with associated SI units.
template <typename VALUE, typename INTERVAL, typename QUANTITY>
class units_t
{
    static_assert(std::is_arithmetic<VALUE>::value, "VALUE must be an arithmetic type");
    static_assert(is_ratio<INTERVAL>, "INTERVAL must be of type std::ratio");
    static_assert(std::ratio_greater<INTERVAL, Zero>::value, "INTERVAL must be positive");
    static_assert(is_quantity<QUANTITY>, "QUANTITY must be of type si::quantity_t" );

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
    using quantity_t = QUANTITY;
    using value_t = VALUE;
    using interval_t = typename INTERVAL::type;


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
    template <typename VALUE2>
    constexpr
    explicit
    units_t
    (
        VALUE2 aValue,
        typename std::enable_if
        <
            std::is_arithmetic<VALUE2>::value &&
            (
                std::is_floating_point<value_t>::value ||
                !std::is_floating_point<VALUE2>::value
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
    template <typename VALUE2, typename INTERVAL2>
    constexpr
    units_t
    (
        units_t<VALUE2, INTERVAL2, QUANTITY> aUnits,
        typename std::enable_if
        <
            no_overflow<INTERVAL2, interval_t>::value &&
            (
                std::is_floating_point<value_t>::value ||
                (
                    no_overflow<INTERVAL2, interval_t>::type::den == 1 &&
                    !std::is_floating_point<VALUE2>::value
                )
            )
        >::type* = nullptr
    )
    : mValue{units_cast<units_t>(aUnits).value()}
    {
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

//==============================================================================
// Some useful units_t types
template< typename INTERVAL = One, typename VALUE = double >
using scalar = units_t<VALUE, INTERVAL, none>;

template< typename INTERVAL = One, typename VALUE = double >
using meters = units_t<VALUE, INTERVAL, length>;

template< typename INTERVAL = One, typename VALUE = double >
using kilograms = units_t<VALUE, INTERVAL, mass>;

template< typename INTERVAL = One, typename VALUE = double >
using seconds = units_t<VALUE, INTERVAL, time>;

template< typename VALUE = double >
using minutes = seconds<std::ratio<60>, VALUE>;

template< typename VALUE = double >
using hours = seconds<std::ratio<60*60>, VALUE>;

template< typename VALUE = double >
using days = seconds<std::ratio<24*60*60>, VALUE>;

template< typename VALUE = double >
using milliseconds = seconds<std::milli, VALUE>;

template< typename VALUE = double >
using microseconds = seconds<std::micro, VALUE>;

template< typename VALUE = double >
using nanoseconds = seconds<std::nano, VALUE>;

template< typename INTERVAL = One, typename VALUE = double >
using amperes = units_t<VALUE, INTERVAL, current>;

template< typename INTERVAL = One, typename VALUE = double >
using kelvins = units_t<VALUE, INTERVAL, temperature>;

template< typename INTERVAL = One, typename VALUE = double >
using candelas = units_t<VALUE, INTERVAL, luminous_intensity>;

template< typename INTERVAL = One, typename VALUE = double >
using moles = units_t<VALUE, INTERVAL, substance>;

template< typename INTERVAL = One, typename VALUE = double >
using radians = units_t<VALUE, INTERVAL, angle>;

template< typename INTERVAL = One, typename VALUE = double >
using steradians = units_t<VALUE, INTERVAL, solid_angle>;

template< typename INTERVAL = One, typename VALUE = double >
using hertz = units_t<VALUE, INTERVAL, frequency>;

template< typename INTERVAL = One, typename VALUE = double >
using newtons = units_t<VALUE, INTERVAL, force>;

template< typename INTERVAL = One, typename VALUE = double >
using coulombs = units_t<VALUE, INTERVAL, charge>;

template< typename INTERVAL = One, typename VALUE = double >
using lux = units_t<VALUE, INTERVAL, illuminance>;

template< typename INTERVAL = One, typename VALUE = double >
using pascals = units_t<VALUE, INTERVAL, pressure>;

template< typename INTERVAL = One, typename VALUE = double >
using joules = units_t<VALUE, INTERVAL, energy>;

template< typename INTERVAL = One, typename VALUE = double >
using watts = units_t<VALUE, INTERVAL, power>;

template< typename INTERVAL = One, typename VALUE = double >
using volts = units_t<VALUE, INTERVAL, voltage>;

template< typename INTERVAL = One, typename VALUE = double >
using farads = units_t<VALUE, INTERVAL, capacitance>;

template< typename INTERVAL = One, typename VALUE = double >
using ohms = units_t<VALUE, INTERVAL, impedance>;

template< typename INTERVAL = One, typename VALUE = double >
using siemens = units_t<VALUE, INTERVAL, conductance>;

template< typename INTERVAL = One, typename VALUE = double >
using webers = units_t<VALUE, INTERVAL, magnetic_flux>;

template< typename INTERVAL = One, typename VALUE = double >
using teslas = units_t<VALUE, INTERVAL, magnetic_flux_density>;

template< typename INTERVAL = One, typename VALUE = double >
using henries = units_t<VALUE, INTERVAL, inductance>;

template< typename INTERVAL = One, typename VALUE = double >
using lumens = units_t<VALUE, INTERVAL, luminous_flux>;

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
template <typename QUANTITY, typename VALUE1, typename INTERVAL1, typename VALUE2, typename INTERVAL2>
inline
constexpr
bool
operator ==
(
    units_t<VALUE1, INTERVAL1, QUANTITY> aLHS,
    units_t<VALUE2, INTERVAL2, QUANTITY> aRHS
)
{
    return units_eq_impl<decltype(aLHS), decltype(aRHS)>{}(aLHS, aRHS);
}

//------------------------------------------------------------------------------
// units_t !=
template <typename QUANTITY, typename VALUE1, typename INTERVAL1, typename VALUE2, typename INTERVAL2>
inline
constexpr
bool
operator !=
(
    units_t<VALUE1, INTERVAL1, QUANTITY> aLHS,
    units_t<VALUE2, INTERVAL2, QUANTITY> aRHS
)
{
    return !(aLHS == aRHS);
}

//------------------------------------------------------------------------------
// units_t <
template <typename QUANTITY, typename VALUE1, typename INTERVAL1, typename VALUE2, typename INTERVAL2>
inline
constexpr
bool
operator <
(
    units_t<VALUE1, INTERVAL1, QUANTITY> aLHS,
    units_t<VALUE2, INTERVAL2, QUANTITY> aRHS
)
{
    return units_lt_impl<decltype(aLHS),decltype(aRHS)>{}(aLHS, aRHS);
}

//------------------------------------------------------------------------------
// units_t >
template <typename QUANTITY, typename VALUE1, typename INTERVAL1, typename VALUE2, typename INTERVAL2>
inline
constexpr
bool
operator >
(
    units_t<VALUE1, INTERVAL1, QUANTITY> aLHS,
    units_t<VALUE2, INTERVAL2, QUANTITY> aRHS
)
{
    return aRHS < aLHS;
}

//------------------------------------------------------------------------------
// units_t <=
template <typename QUANTITY, typename VALUE1, typename INTERVAL1, typename VALUE2, typename INTERVAL2>
inline
constexpr
bool
operator <=
(
    units_t<VALUE1, INTERVAL1, QUANTITY> aLHS,
    units_t<VALUE2, INTERVAL2, QUANTITY> aRHS
)
{
    return !(aRHS < aLHS);
}

//------------------------------------------------------------------------------
// units_t >=
template <typename QUANTITY, typename VALUE1, typename INTERVAL1, typename VALUE2, typename INTERVAL2>
inline
constexpr
bool
operator >=
(
    units_t<VALUE1, INTERVAL1, QUANTITY> aLHS,
    units_t<VALUE2, INTERVAL2, QUANTITY> aRHS
)
{
    return !(aLHS < aRHS);
}

//------------------------------------------------------------------------------
// units_t +
template <typename QUANTITY, typename VALUE1, typename INTERVAL1, typename VALUE2, typename INTERVAL2>
inline
constexpr
auto
operator +
(
    units_t<VALUE1, INTERVAL1, QUANTITY> aLHS,
    units_t<VALUE2, INTERVAL2, QUANTITY> aRHS
)
{
    using CommonUnits_t = std::common_type_t<decltype(aLHS), decltype(aRHS)>;
    return CommonUnits_t{CommonUnits_t{aLHS}.value() + CommonUnits_t{aRHS}.value()};
}

//------------------------------------------------------------------------------
// units_t -
template <typename QUANTITY, typename VALUE1, typename INTERVAL1, typename VALUE2, typename INTERVAL2>
inline
constexpr
auto
operator -
(
    units_t<VALUE1, INTERVAL1, QUANTITY> aLHS,
    units_t<VALUE2, INTERVAL2, QUANTITY> aRHS
)
{
    return aLHS + (-aRHS);
}

//------------------------------------------------------------------------------
// units_t * units_t
template
<
    typename QUANTITY1,
    typename VALUE1,
    typename INTERVAL1,
    typename QUANTITY2,
    typename VALUE2,
    typename INTERVAL2
>
inline
constexpr
auto
operator *
(
    units_t<VALUE1, INTERVAL1, QUANTITY1> aLHS,
    units_t<VALUE2, INTERVAL2, QUANTITY2> aRHS
)
{
    using ResultValue_t = std::common_type_t<VALUE1, VALUE2>;
    using Result_t = units_t
    <
        ResultValue_t,
        std::ratio_multiply<INTERVAL1, INTERVAL2>,
        multiply_quantity<QUANTITY1, QUANTITY2>
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
template <typename VALUE1, typename INTERVAL, typename QUANTITY, typename VALUE2>
inline
constexpr
typename std::enable_if
<
    !is_units_t<VALUE2> &&
    is_convertible_v
    <
        VALUE2,
        std::common_type_t<VALUE1, VALUE2>
    >,
    units_t
    <
        std::common_type_t<VALUE1, VALUE2>,
        INTERVAL,
        QUANTITY
    >
>::type
operator *
(
    units_t<VALUE1, INTERVAL, QUANTITY> aUnits,
    VALUE2 aScalar
)
{
    return aUnits * scalar<One, VALUE2>{aScalar};
}

//------------------------------------------------------------------------------
// scalar * units_t
template <typename VALUE1, typename INTERVAL, typename QUANTITY, typename VALUE2>
inline
constexpr
typename std::enable_if
<
    !is_units_t<VALUE2> &&
    is_convertible_v
    <
        VALUE2,
        std::common_type_t<VALUE1, VALUE2>
    >,
    units_t
    <
        std::common_type_t<VALUE1, VALUE2>,
        INTERVAL,
        QUANTITY
    >
>::type
operator *
(
    VALUE2 aScalar,
    units_t<VALUE1, INTERVAL, QUANTITY> aUnits
)
{
    return aUnits * aScalar;
}

template
<
    typename UNITS,
    typename VALUE,
    bool = is_convertible_v
    <
        VALUE,
        std::common_type_t<typename UNITS::value_t, VALUE>
    >
>
struct units_divide_helper
{
};

template
<
    typename VALUE1,
    typename INTERVAL,
    typename QUANTITY,
    typename VALUE2
>
struct units_divide_helper
<
    units_t<VALUE1, INTERVAL, QUANTITY>,
    VALUE2,
    true
>
{
    using type = units_t
    <
        std::common_type_t<VALUE1, VALUE2>,
        INTERVAL,
        QUANTITY
    >;
};

template
<
    typename QUANTITY,
    typename VALUE,
    bool = is_units_t<VALUE>
>
struct units_divide_result_t
{
};

template
<
    typename VALUE1,
    typename INTERVAL,
    typename QUANTITY,
    typename VALUE2
>
struct units_divide_result_t
<
    units_t<VALUE1, INTERVAL, QUANTITY>,
    VALUE2,
    false
>
: units_divide_helper<units_t<VALUE1, INTERVAL, QUANTITY>, VALUE2>
{
};

//------------------------------------------------------------------------------
// divide units_t by scalar
template
<
    typename VALUE1,
    typename INTERVAL,
    typename QUANTITY,
    typename VALUE2
>
inline
constexpr
typename units_divide_result_t<units_t<VALUE1, INTERVAL, QUANTITY>, VALUE2>::type
operator /
(
    units_t<VALUE1, INTERVAL, QUANTITY> aUnits,
    VALUE2 aScalar
)
{
    using ResultValue_t = std::common_type_t<VALUE1, VALUE2>;
    using Result_t = units_t<ResultValue_t, INTERVAL, QUANTITY>;
    return Result_t{Result_t{aUnits}.value() / static_cast<ResultValue_t>(aScalar)};
}

//------------------------------------------------------------------------------
// divide units_t by units_t, same quantity_t
template
<
    typename QUANTITY,
    typename VALUE1,
    typename INTERVAL1,
    typename VALUE2,
    typename INTERVAL2
>
inline
constexpr
auto
operator /
(
    units_t<VALUE1, INTERVAL1, QUANTITY> aLHS,
    units_t<VALUE2, INTERVAL2, QUANTITY> aRHS
)
{
    using CommonUnits_t = std::common_type_t
    <
        units_t<VALUE1, INTERVAL1, QUANTITY>,
        units_t<VALUE2, INTERVAL2, QUANTITY>
    >;
    return CommonUnits_t{aLHS}.value() / CommonUnits_t{aRHS}.value();
}

template
<
    typename VALUE1,
    typename INTERVAL1,
    typename QUANTITY1,
    typename VALUE2,
    typename INTERVAL2,
    typename QUANTITY2
>
using diff_units_result_t = units_t
<
    std::common_type_t<VALUE1, VALUE2>,
    std::ratio_divide<INTERVAL1, INTERVAL2>,
    divide_quantity<QUANTITY1, QUANTITY2>
>;

//------------------------------------------------------------------------------
// divide units_t by units_t, different quantity_t
template
<
    typename QUANTITY1,
    typename VALUE1,
    typename INTERVAL1,
    typename QUANTITY2,
    typename VALUE2,
    typename INTERVAL2
>
inline
constexpr
typename std::enable_if
<
    !std::is_same<QUANTITY1,QUANTITY2>::value,
    diff_units_result_t<VALUE1, INTERVAL1, QUANTITY1, VALUE2, INTERVAL2, QUANTITY2>
>::type
operator /
(
    units_t<VALUE1, INTERVAL1, QUANTITY1> aLHS,
    units_t<VALUE2, INTERVAL2, QUANTITY2> aRHS
)
{
    using Result_t = diff_units_result_t<VALUE1, INTERVAL1, QUANTITY1, VALUE2, INTERVAL2, QUANTITY2>;
    return Result_t{aLHS.value() / aRHS.value()};
}

//------------------------------------------------------------------------------
// divide scalar by units_t
template
<
    typename VALUE1,
    typename INTERVAL,
    typename QUANTITY,
    typename VALUE2
>
inline
constexpr
auto
operator /
(
    VALUE2 aScalar,
    units_t<VALUE1, INTERVAL, QUANTITY> aUnits
)
{
    using ResultValue_t = std::common_type_t<VALUE1, VALUE2>;
    return scalar<One, ResultValue_t>{aScalar} / aUnits;
}

//------------------------------------------------------------------------------
// modulo units_t by scalar
template
<
    typename QUANTITY,
    typename VALUE1,
    typename INTERVAL,
    typename VALUE2
>
inline
constexpr
typename units_divide_result_t<units_t<VALUE1, INTERVAL, QUANTITY>, VALUE2>::type
operator%
(
    units_t<VALUE1, INTERVAL, QUANTITY> aUnits,
    VALUE2 aScalar
)
{
    using ResultValue_t = std::common_type_t<VALUE1, VALUE2>;
    using Result_t = units_t<ResultValue_t, INTERVAL, QUANTITY>;
    return Result_t{Result_t{aUnits}.value() % static_cast<ResultValue_t>(aScalar)};
}

template
<
    typename VALUE1,
    typename INTERVAL1,
    typename QUANTITY1,
    typename VALUE2,
    typename INTERVAL2
>
using mod_result = units_t
<
    std::common_type_t<VALUE1, VALUE2>,
    std::ratio_divide<INTERVAL1, INTERVAL2>,
    QUANTITY1
>;

//------------------------------------------------------------------------------
// modulo units_t by units_t
template
<
    typename VALUE1,
    typename INTERVAL1,
    typename QUANTITY1,
    typename VALUE2,
    typename INTERVAL2,
    typename QUANTITY2
>
inline
constexpr
mod_result<VALUE1, INTERVAL1, QUANTITY1, VALUE2, INTERVAL2>
operator%
(
    units_t<VALUE1, INTERVAL1, QUANTITY1> aLHS,
    units_t<VALUE2, INTERVAL2, QUANTITY2> aRHS
)
{
    return aLHS - (aRHS * (aLHS / aRHS));
}

//------------------------------------------------------------------------------
// absolute value of a units_t
template
<
    typename VALUE,
    typename INTERVAL,
    typename QUANTITY
>
inline
constexpr
units_t<VALUE, INTERVAL, QUANTITY>
absolute
(
    units_t<VALUE, INTERVAL, QUANTITY> aUnits
)
{
    return units_t<VALUE, INTERVAL, QUANTITY>{std::abs(aUnits.value())};
}

//------------------------------------------------------------------------------
// floor of a units_t
template
<
    typename RESULT,
    typename VALUE,
    typename INTERVAL,
    typename QUANTITY,
    typename = std::enable_if_t<is_units_t<RESULT>>
>
inline
constexpr
RESULT
floor
(
    units_t<VALUE, INTERVAL, QUANTITY> aUnits
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
    typename VALUE,
    typename INTERVAL,
    typename QUANTITY,
    typename = std::enable_if_t<is_units_t<RESULT>>
>
inline
constexpr
RESULT
ceiling
(
    units_t<VALUE, INTERVAL, QUANTITY> aUnits
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
    typename INTERVAL,
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
    units_t<VALUE, INTERVAL, QUANTITY> aUnits
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
    typename VALUE,
    typename INTERVAL,
    typename QUANTITY,
    typename = std::enable_if_t<is_units_t<RESULT>>
>
inline
constexpr
RESULT
truncate
(
    units_t<VALUE, INTERVAL, QUANTITY> aUnits
)
{
    auto theResult = units_cast<RESULT>(aUnits);
    return RESULT{static_cast<typename RESULT::value_t>(std::trunc(theResult.value()))};
}

template
<
    typename VALUE,
    typename INTERVAL,
    typename QUANTITY,
    typename EPSILON
>
using sqrt_result_t = typename std::enable_if
<
    std::is_floating_point<VALUE>::value,
    units_t
    <
        VALUE,
        typename ratio_sqrt<INTERVAL, EPSILON>::type,
        root_quantity<QUANTITY, 2>
    >
>::type;

//------------------------------------------------------------------------------
// square root of a units_t
template
<
    typename VALUE,
    typename INTERVAL,
    typename QUANTITY,
    typename EPSILON = std::ratio<1,10000000000000>
>
inline
sqrt_result_t<VALUE, INTERVAL, QUANTITY, EPSILON>
square_root
(
    units_t<VALUE, INTERVAL, QUANTITY> aQuantity
)
{
    using Result_t = sqrt_result_t<VALUE, INTERVAL, QUANTITY, EPSILON>;
    return Result_t{std::sqrt(aQuantity.value())};
}

template
<
    typename VALUE,
    typename INTERVAL,
    typename QUANTITY,
    std::intmax_t EXPONENT
>
struct power_result_impl;

template
<
    typename VALUE,
    typename INTERVAL,
    typename QUANTITY
>
struct power_result_impl<VALUE, INTERVAL, QUANTITY, 0>
{
    using type = scalar<One, VALUE>;
};

template
<
    typename VALUE,
    typename INTERVAL,
    typename QUANTITY,
    std::intmax_t EXPONENT
>
struct power_result_impl
{
    using temp = typename power_result_impl<VALUE, INTERVAL, QUANTITY, EXPONENT-1>::type;
    using type = units_t
    <
        VALUE,
        std::ratio_multiply<INTERVAL, typename temp::interval_t>,
        multiply_quantity<QUANTITY, typename temp::quantity_t>
    >;
};

template
<
    typename VALUE,
    typename INTERVAL,
    typename QUANTITY,
    std::intmax_t EXPONENT
>
using power_result_t = typename power_result_impl<VALUE, INTERVAL, QUANTITY, EXPONENT>::type;

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
    typename INTERVAL,
    typename QUANTITY
>
constexpr
inline
power_result_t<VALUE, INTERVAL, QUANTITY, EXPONENT>
exponentiate
(
    const units_t<VALUE, INTERVAL, QUANTITY>& aQuantity
)
{
    using Result_t = power_result_t<VALUE, INTERVAL, QUANTITY, EXPONENT>;
    return Result_t{value_pow(aQuantity.value(), EXPONENT)};
}

//------------------------------------------------------------------------------
// sine of radians
template
<
    typename VALUE,
    typename INTERVAL
>
inline
scalar<>
sine
(
    radians<INTERVAL, VALUE> aRadians
)
{
    const auto theBaseRadians = units_cast<radians<>>(aRadians);
    return scalar<>{std::sin(theBaseRadians.value())};
}

//------------------------------------------------------------------------------
// cosine of radians
template
<
    typename VALUE,
    typename INTERVAL
>
inline
scalar<>
cosine
(
    radians<INTERVAL, VALUE> aRadians
)
{
    const auto theBaseRadians = units_cast<radians<>>(aRadians);
    return scalar<>{std::cos(theBaseRadians.value())};
}

//------------------------------------------------------------------------------
// tangent of radians
template
<
    typename VALUE,
    typename INTERVAL
>
inline
scalar<>
tangent
(
    radians<INTERVAL, VALUE> aRadians
)
{
    const auto theBaseRadians = units_cast<radians<>>(aRadians);
    return scalar<>{std::tan(theBaseRadians.value())};
}

//------------------------------------------------------------------------------
// arc sine of scalar
template
<
    typename VALUE,
    typename INTERVAL
>
inline
radians<>
arc_sine
(
    scalar<INTERVAL, VALUE> aScalar
)
{
    const auto theBaseScalar = units_cast<scalar<>>(aScalar);
    return radians<>{std::asin(theBaseScalar.value())};
}

//------------------------------------------------------------------------------
// arc cosine of scalar
template
<
    typename VALUE,
    typename INTERVAL
>
inline
radians<>
arc_cosine
(
    scalar<INTERVAL, VALUE> aScalar
)
{
    const auto theBaseScalar = units_cast<scalar<>>(aScalar);
    return radians<>{std::acos(theBaseScalar.value())};
}

//------------------------------------------------------------------------------
// arc tangent of scalar
template
<
    typename VALUE,
    typename INTERVAL
>
inline
radians<>
arc_tangent
(
    scalar<INTERVAL, VALUE> aScalar
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
struct basic_string_from_impl<CharT, units_t<ValueT, IntervalT, QuantityT>>
{
    std::basic_string<CharT>
    operator()
    (
        units_t<ValueT, IntervalT, QuantityT> aUnits
    )
    {
        std::basic_string<CharT> theResult;
        if( aUnits.interval.num != aUnits.interval.den )
        {
            theResult = basic_string_from_impl<CharT,IntervalT>{}(aUnits.interval);
        }

        const auto theQuantityString = basic_string_from_impl<CharT,QuantityT>{}(aUnits.quantity);
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
};

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
    using Input_t = decltype(aUnits);

    aStream << aUnits.value();

    if( aUnits.interval.num != aUnits.interval.den )
    {
        aStream << multiply_operator<CharT> << basic_string_from<CharT>(IntervalT{});
    }

    const auto theQuantityStr = basic_string_from<CharT>(QuantityT{});
    if( !theQuantityStr.empty() )
    {
        aStream << space<CharT> << theQuantityStr;
    }

    return aStream;
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
constexpr type<si::One,long long> operator "" _##str \
( \
    unsigned long long aValue \
) \
{ \
    return type<si::One,long long>{aValue}; \
}

#define time_literal( type, str ) \
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
literal(seconds, s);
time_literal(minutes, min);
time_literal(hours, h);
time_literal(days, d);
time_literal(milliseconds, ms);
time_literal(microseconds, us);
time_literal(nanoseconds, ns);
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

namespace std
{

template
<
    typename VALUE,
    typename INTERVAL,
    typename QUANTITY
>
struct hash<si::units_t<VALUE,INTERVAL,QUANTITY>>
{
    size_t
    operator()
    (
        si::units_t<VALUE,INTERVAL,QUANTITY> aUnits
    ) const
    {
        return hash<VALUE>{}(aUnits.value());
    }
};

} // end of namespace std
