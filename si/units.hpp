#pragma once
#include <type_traits>
#include <ratio>
#include <limits>
#include <cmath>

#include "quantity.hpp"
#include "ratio/is-ratio.hpp"
#include "ratio/ratio-gcd.hpp"
#include "ratio/ratio-sqrt.hpp"

namespace si
{

// Forward declaration
template <typename VALUE, typename RATIO, typename QUANTITY > class units_t;

} // end of namespace si

//------------------------------------------------------------------------------
/// Specialization of std::common_type for units_t.
template
<
    typename QUANTITY,
    typename VALUE1,
    typename RATIO1,
    typename VALUE2,
    typename RATIO2
>
struct std::common_type
<
    si::units_t<VALUE1, RATIO1, QUANTITY>,
    si::units_t<VALUE2, RATIO2, QUANTITY>
>
{
    using type = si::units_t
    <
        std::common_type_t<VALUE1, VALUE2>,
        si::ratio::ratio_gcd<RATIO1, RATIO2>,
        QUANTITY
    >;
};

namespace si
{

template
<
    typename FromUnitsT,
    typename ToUnitsT,
    typename RATIO = typename std::ratio_divide
    <
        typename FromUnitsT::ratio_t,
        typename ToUnitsT::ratio_t
    >::type,
    bool = RATIO::num == 1,
    bool = RATIO::den == 1>
struct units_cast_impl;

template
<
    typename FromUnitsT,
    typename ToUnitsT,
    typename RATIO
>
struct units_cast_impl<FromUnitsT, ToUnitsT, RATIO, true, true>
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
    typename RATIO
>
struct units_cast_impl<FromUnitsT, ToUnitsT, RATIO, true, false>
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
                static_cast<ResultValue_t>(RATIO::den)
            )
        };
    }
};

template
<
    typename FromUnitsT,
    typename ToUnitsT,
    typename RATIO
>
struct units_cast_impl<FromUnitsT, ToUnitsT, RATIO, false, true>
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
                static_cast<ResultValue_t>(RATIO::num)
            )
        };
    }
};

template
<
    typename FromUnitsT,
    typename ToUnitsT,
    typename RATIO
>
struct units_cast_impl<FromUnitsT, ToUnitsT, RATIO, false, false>
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
                static_cast<ResultValue_t>(RATIO::num) /
                static_cast<ResultValue_t>(RATIO::den)
            )
        };
    }
};

template <typename aType>
struct is_units_impl : std::false_type {};

template <typename QUANTITY, typename VALUE, typename RATIO>
struct is_units_impl<units_t<VALUE, RATIO, QUANTITY>> : std::true_type {};

//------------------------------------------------------------------------------
/// true if aType is an si::units_t, false otherwise
/// @tparam aType a type to be tested
template <typename aType>
constexpr bool is_units_t = is_units_impl<typename std::decay<aType>::type>::value;

//------------------------------------------------------------------------------
/// Convert an si::units_t to another si::units_t type.
/// Both types must have the same si::quantity_t type.
template <typename ToUnitsT, typename QUANTITY, typename VALUE, typename RATIO>
inline
constexpr
typename std::enable_if
<
    is_units_t<ToUnitsT> && std::is_same<typename ToUnitsT::quantity_t,QUANTITY>::value,
    ToUnitsT
>::type
units_cast(units_t<VALUE, RATIO, QUANTITY> aFromUnits)
{
    return units_cast_impl
    <
        units_t<VALUE, RATIO, QUANTITY>,
        ToUnitsT
    >()(aFromUnits);
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
/// Class si::units_t represents a numeric value with associated SI units.
template <typename VALUE, typename RATIO, typename QUANTITY>
class units_t
{
    static_assert(std::is_arithmetic<VALUE>::value, "VALUE must be an arithmetic type");
    static_assert(ratio::is_ratio<RATIO>, "RATIO must be of type std::ratio");
    static_assert(std::ratio_greater<RATIO, std::ratio<0>>::value, "RATIO must be positive");
    static_assert(is_quantity<QUANTITY>, "QUANTITY must be of type si::quantity_t" );

    template <typename _R1, typename _R2>
    struct no_overflow
    {
    private:
        static constexpr intmax_t num_gcd = ratio::gcd<_R1::num, _R2::num>;
        static constexpr intmax_t den_gcd = ratio::gcd<_R1::den, _R2::den>;
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
    using ratio_t = typename RATIO::type;


    //--------------------------------------------------------------------------
    /// Static member constants
    static constexpr auto ratio = ratio_t{};
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
        VALUE2 aValue
    )
    : mValue{aValue}
    {
    }

    //--------------------------------------------------------------------------
    /// Initialize a units_t from another units_t possibly having different value_t and ratio_t types but the same quantity_t type.
    /// This constructor will not be chosen by the compiler if it would result in overflow or loss of precision.
    /// @param aUnits the units_t that will be converted to this units_t
    template <typename VALUE2, typename RATIO2>
    constexpr
    units_t
    (
        units_t<VALUE2, RATIO2, QUANTITY> aUnits,
        typename std::enable_if
        <
            no_overflow<RATIO2, ratio_t>::value &&
            (
                std::is_floating_point<value_t>::value ||
                (
                    no_overflow<RATIO2, ratio_t>::type::den == 1 &&
                    !std::is_floating_point<VALUE2>::value
                )
            )
        >::type* = 0
    )
    : mValue{units_cast<units_t>(aUnits).value()}
    {
    }

    //--------------------------------------------------------------------------
    // Accessor function
    constexpr value_t value() const {return mValue;}
    constexpr value_t expand() const {return mValue * ratio.num / ratio.den;}

    //--------------------------------------------------------------------------
    // Arithmetic functions
    constexpr units_t operator+() const {return *this;}
    constexpr units_t operator-() const {return units_t{-mValue};}
    units_t& operator++() {++mValue; return *this;}
    units_t operator++(int) {return units_t{mValue++};}
    units_t& operator--() {--mValue; return *this;}
    units_t operator--(int) {return units_t{mValue--};}
    units_t& operator+=(units_t rhs) {mValue += rhs.value(); return *this;}
    units_t& operator-=(units_t rhs) {mValue -= rhs.value(); return *this;}
    units_t& operator*=(value_t rhs) {mValue *= rhs; return *this;}
    units_t& operator/=(value_t rhs) {mValue /= rhs; return *this;}
    units_t& operator%=(value_t rhs) {mValue %= rhs; return *this;}
    units_t& operator%=(units_t rhs) {mValue %= rhs.value(); return *this;}

    //--------------------------------------------------------------------------
    // Special values
    static constexpr units_t zero() {return units_t{units_values<value_t>::zero()};}
    static constexpr units_t min() {return units_t{units_values<value_t>::min()};}
    static constexpr units_t max() {return units_t{units_values<value_t>::max()};}

private:

    value_t mValue;

}; // end of class units_t

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
template <typename QUANTITY, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
bool
operator ==
(
    units_t<VALUE1, RATIO1, QUANTITY> aLHS,
    units_t<VALUE2, RATIO2, QUANTITY> aRHS
)
{
    return units_eq_impl
    <
        units_t<VALUE1, RATIO1, QUANTITY>,
        units_t<VALUE2, RATIO2, QUANTITY>
    >()(aLHS, aRHS);
}

//------------------------------------------------------------------------------
// units_t !=
template <typename QUANTITY, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
bool
operator !=
(
    units_t<VALUE1, RATIO1, QUANTITY> aLHS,
    units_t<VALUE2, RATIO2, QUANTITY> aRHS
)
{
    return !(aLHS == aRHS);
}

//------------------------------------------------------------------------------
// units_t <
template <typename QUANTITY, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
bool
operator <
(
    units_t<VALUE1, RATIO1, QUANTITY> aLHS,
    units_t<VALUE2, RATIO2, QUANTITY> aRHS
)
{
    return units_lt_impl
    <
        units_t<VALUE1, RATIO1, QUANTITY>,
        units_t<VALUE2, RATIO2, QUANTITY>
    >()(aLHS, aRHS);
}

//------------------------------------------------------------------------------
// units_t >
template <typename QUANTITY, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
bool
operator >
(
    units_t<VALUE1, RATIO1, QUANTITY> aLHS,
    units_t<VALUE2, RATIO2, QUANTITY> aRHS
)
{
    return aRHS < aLHS;
}

//------------------------------------------------------------------------------
// units_t <=
template <typename QUANTITY, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
bool
operator <=
(
    units_t<VALUE1, RATIO1, QUANTITY> aLHS,
    units_t<VALUE2, RATIO2, QUANTITY> aRHS
)
{
    return !(aRHS < aLHS);
}

//------------------------------------------------------------------------------
// units_t >=
template <typename QUANTITY, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
bool
operator >=
(
    units_t<VALUE1, RATIO1, QUANTITY> aLHS,
    units_t<VALUE2, RATIO2, QUANTITY> aRHS
)
{
    return !(aLHS < aRHS);
}

//------------------------------------------------------------------------------
// units_t +
template <typename QUANTITY, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
auto
operator +
(
    units_t<VALUE1, RATIO1, QUANTITY> aLHS,
    units_t<VALUE2, RATIO2, QUANTITY> aRHS
)
{
    using CommonUnits_t = std::common_type_t
    <
        units_t<VALUE1, RATIO1, QUANTITY>,
        units_t<VALUE2, RATIO2, QUANTITY>
    >;
    return CommonUnits_t{CommonUnits_t{aLHS}.value() + CommonUnits_t{aRHS}.value()};
}

//------------------------------------------------------------------------------
// units_t -
template <typename QUANTITY, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
auto
operator -
(
    units_t<VALUE1, RATIO1, QUANTITY> aLHS,
    units_t<VALUE2, RATIO2, QUANTITY> aRHS
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
    typename RATIO1,
    typename QUANTITY2,
    typename VALUE2,
    typename RATIO2
>
inline
constexpr
auto
operator *
(
    units_t<VALUE1, RATIO1, QUANTITY1> aLHS,
    units_t<VALUE2, RATIO2, QUANTITY2> aRHS
)
{
    using ResultValue_t = std::common_type_t<VALUE1, VALUE2>;
    using Result_t = units_t
    <
        ResultValue_t,
        std::ratio_multiply<RATIO1, RATIO2>,
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
template <typename VALUE1, typename RATIO, typename QUANTITY, typename VALUE2>
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
        RATIO,
        QUANTITY
    >
>::type
operator *
(
    units_t<VALUE1, RATIO, QUANTITY> aUnits,
    VALUE2 aScalar
)
{
    return aUnits * units_t<VALUE2, std::ratio<1>, none>{aScalar};
}

//------------------------------------------------------------------------------
// scalar * units_t
template <typename VALUE1, typename RATIO, typename QUANTITY, typename VALUE2>
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
        RATIO,
        QUANTITY
    >
>::type
operator *
(
    VALUE2 aScalar,
    units_t<VALUE1, RATIO, QUANTITY> aUnits
)
{
    return aUnits * aScalar;
}

template
<
    typename QUANTITY,
    typename VALUE,
    bool = is_convertible_v
    <
        VALUE,
        std::common_type_t<typename QUANTITY::value_t, VALUE>
    >
>
struct units_divide_helper
{
};

template
<
    typename VALUE1,
    typename RATIO,
    typename QUANTITY,
    typename VALUE2
>
struct units_divide_helper
<
    units_t<VALUE1, RATIO, QUANTITY>,
    VALUE2,
    true
>
{
    using type = units_t
    <
        std::common_type_t<VALUE1, VALUE2>,
        RATIO,
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
    typename RATIO,
    typename QUANTITY,
    typename VALUE2
>
struct units_divide_result_t
<
    units_t<VALUE1, RATIO, QUANTITY>,
    VALUE2,
    false
>
: units_divide_helper<units_t<VALUE1, RATIO, QUANTITY>, VALUE2>
{
};

//------------------------------------------------------------------------------
// divide units_t by scalar
template
<
    typename VALUE1,
    typename RATIO,
    typename QUANTITY,
    typename VALUE2
>
inline
constexpr
typename units_divide_result_t<units_t<VALUE1, RATIO, QUANTITY>, VALUE2>::type
operator /
(
    units_t<VALUE1, RATIO, QUANTITY> aUnits,
    VALUE2 aScalar
)
{
    using ResultValue_t = std::common_type_t<VALUE1, VALUE2>;
    using Result_t = units_t<ResultValue_t, RATIO, QUANTITY>;
    return Result_t{Result_t{aUnits}.value() / static_cast<ResultValue_t>(aScalar)};
}

//------------------------------------------------------------------------------
// divide units_t by units_t, same quantity_t
template
<
    typename QUANTITY,
    typename VALUE1,
    typename RATIO1,
    typename VALUE2,
    typename RATIO2
>
inline
constexpr
auto
operator /
(
    units_t<VALUE1, RATIO1, QUANTITY> aLHS,
    units_t<VALUE2, RATIO2, QUANTITY> aRHS
)
{
    using CommonUnits_t = std::common_type_t
    <
        units_t<VALUE1, RATIO1, QUANTITY>,
        units_t<VALUE2, RATIO2, QUANTITY>
    >;
    return CommonUnits_t{aLHS}.value() / CommonUnits_t{aRHS}.value();
}

template
<
    typename VALUE1,
    typename RATIO1,
    typename QUANTITY1,
    typename VALUE2,
    typename RATIO2,
    typename QUANTITY2
>
using diff_units_result_t = units_t
<
    std::common_type_t<VALUE1, VALUE2>,
    std::ratio_divide<RATIO1, RATIO2>,
    divide_quantity<QUANTITY1, QUANTITY2>
>;

//------------------------------------------------------------------------------
// divide units_t by units_t, different quantity_t
template
<
    typename QUANTITY1,
    typename VALUE1,
    typename RATIO1,
    typename QUANTITY2,
    typename VALUE2,
    typename RATIO2
>
inline
constexpr
typename std::enable_if
<
    !std::is_same<QUANTITY1,QUANTITY2>::value,
    diff_units_result_t<VALUE1, RATIO1, QUANTITY1, VALUE2, RATIO2, QUANTITY2>
>::type
operator /
(
    units_t<VALUE1, RATIO1, QUANTITY1> aLHS,
    units_t<VALUE2, RATIO2, QUANTITY2> aRHS
)
{
    using Result_t = diff_units_result_t<VALUE1, RATIO1, QUANTITY1, VALUE2, RATIO2, QUANTITY2>;
    return Result_t{aLHS.value() / aRHS.value()};
}

//------------------------------------------------------------------------------
// divide scalar by units_t
template
<
    typename VALUE1,
    typename RATIO,
    typename QUANTITY,
    typename VALUE2
>
inline
constexpr
auto
operator /
(
    VALUE2 aScalar,
    units_t<VALUE1, RATIO, QUANTITY> aUnits
)
{
    using ResultValue_t = std::common_type_t<VALUE1, VALUE2>;
    return units_t<ResultValue_t, std::ratio<1>, none>{aScalar} / aUnits;
}

//------------------------------------------------------------------------------
// modulo units_t by scalar
template
<
    typename QUANTITY,
    typename VALUE1,
    typename RATIO,
    typename VALUE2
>
inline
constexpr
typename units_divide_result_t<units_t<VALUE1, RATIO, QUANTITY>, VALUE2>::type
operator%
(
    units_t<VALUE1, RATIO, QUANTITY> aUnits,
    VALUE2 aScalar
)
{
    using ResultValue_t = std::common_type_t<VALUE1, VALUE2>;
    using Result_t = units_t<ResultValue_t, RATIO, QUANTITY>;
    return Result_t{Result_t{aUnits}.value() % static_cast<ResultValue_t>(aScalar)};
}

template
<
    typename VALUE1,
    typename RATIO1,
    typename QUANTITY1,
    typename VALUE2,
    typename RATIO2
>
using mod_result = units_t
<
    std::common_type_t<VALUE1, VALUE2>,
    std::ratio_divide<RATIO1, RATIO2>,
    QUANTITY1
>;

//------------------------------------------------------------------------------
// modulo units_t by units_t
template
<
    typename VALUE1,
    typename RATIO1,
    typename QUANTITY1,
    typename VALUE2,
    typename RATIO2,
    typename QUANTITY2
>
inline
constexpr
mod_result<VALUE1, RATIO1, QUANTITY1, VALUE2, RATIO2>
operator%
(
    units_t<VALUE1, RATIO1, QUANTITY1> aLHS,
    units_t<VALUE2, RATIO2, QUANTITY2> aRHS
)
{
    return aLHS - (aRHS * (aLHS / aRHS));
}

//==============================================================================
// Some useful si::units_t types
template< typename RATIO = std::ratio<1>, typename VALUE = double >
using scalar = units_t<VALUE, RATIO, none>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using meters = units_t<VALUE, RATIO, length>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using kilograms = units_t<VALUE, RATIO, mass>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using seconds = units_t<VALUE, RATIO, time>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using minutes = seconds<std::ratio_multiply<RATIO, std::ratio<60>>, VALUE>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using hours = minutes<std::ratio_multiply<RATIO, std::ratio<60>>, VALUE>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using days = hours<std::ratio_multiply<RATIO, std::ratio<24>>, VALUE>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using amperes = units_t<VALUE, RATIO, current>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using kelvins = units_t<VALUE, RATIO, temperature>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using candelas = units_t<VALUE, RATIO, luminous_intensity>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using radians = units_t<VALUE, RATIO, angle>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using steradians = units_t<VALUE, RATIO, solid_angle>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using hertz = units_t<VALUE, RATIO, frequency>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using newtons = units_t<VALUE, RATIO, force>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using coulombs = units_t<VALUE, RATIO, charge>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using lux = units_t<VALUE, RATIO, illuminance>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using pascals = units_t<VALUE, RATIO, pressure>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using joules = units_t<VALUE, RATIO, energy>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using watts = units_t<VALUE, RATIO, power>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using volts = units_t<VALUE, RATIO, voltage>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using farads = units_t<VALUE, RATIO, capacitance>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using ohms = units_t<VALUE, RATIO, impedance>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using siemens = units_t<VALUE, RATIO, conductance>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using webers = units_t<VALUE, RATIO, magnetic_flux>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using teslas = units_t<VALUE, RATIO, magnetic_flux_density>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using henries = units_t<VALUE, RATIO, inductance>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using lumens = units_t<VALUE, RATIO, luminous_flux>;

} // end of namespace si
