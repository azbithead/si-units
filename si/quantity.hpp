#pragma once
#include <type_traits>
#include <ratio>
#include <limits>
#include <cmath>

#include "units.hpp"
#include "ratio/is-ratio.hpp"
#include "ratio/ratio-gcd.hpp"
#include "ratio/ratio-sqrt.hpp"

namespace si
{

// Forward declaration
template <typename VALUE, typename RATIO, typename UNITS > class quantity;

} // end of namespace si

//------------------------------------------------------------------------------
/// Specialization of std::common_type for quantity.
template
<
    typename UNITS,
    typename VALUE1,
    typename RATIO1,
    typename VALUE2,
    typename RATIO2
>
struct std::common_type
<
    si::quantity<VALUE1, RATIO1, UNITS>,
    si::quantity<VALUE2, RATIO2, UNITS>
>
{
    using type = si::quantity
    <
        std::common_type_t<VALUE1, VALUE2>,
        si::ratio::ratio_gcd<RATIO1, RATIO2>,
        UNITS
    >;
};

namespace si
{

template
<
    typename FromQ,
    typename ToQ,
    typename RATIO = typename std::ratio_divide
    <
        typename FromQ::ratio_t,
        typename ToQ::ratio_t
    >::type,
    bool = RATIO::num == 1,
    bool = RATIO::den == 1>
struct quantity_cast_impl;

template
<
    typename FromQ,
    typename ToQ,
    typename RATIO
>
struct quantity_cast_impl<FromQ, ToQ, RATIO, true, true>
{
    constexpr
    ToQ operator()(const FromQ& aFromQuantity) const
    {
        return ToQ
        (
            static_cast<typename ToQ::value_t>(aFromQuantity.value())
        );
    }
};

template
<
    typename FromQ,
    typename ToQ,
    typename RATIO
>
struct quantity_cast_impl<FromQ, ToQ, RATIO, true, false>
{
    constexpr
    ToQ operator()(const FromQ& aFromQuantity) const
    {
        using ResultValue_t = std::common_type_t
        <
            typename ToQ::value_t,
            typename FromQ::value_t,
            intmax_t
        >;
        return ToQ
        (
            static_cast<typename ToQ::value_t>
            (
                static_cast<ResultValue_t>(aFromQuantity.value()) /
                static_cast<ResultValue_t>(RATIO::den)
            )
        );
    }
};

template
<
    typename FromQ,
    typename ToQ,
    typename RATIO
>
struct quantity_cast_impl<FromQ, ToQ, RATIO, false, true>
{
    constexpr
    ToQ operator()(const FromQ& aFromQuantity) const
    {
        using ResultValue_t = std::common_type_t
        <
            typename ToQ::value_t,
            typename FromQ::value_t,
            intmax_t
        >;
        return ToQ
        (
            static_cast<typename ToQ::value_t>
            (
                static_cast<ResultValue_t>(aFromQuantity.value()) *
                static_cast<ResultValue_t>(RATIO::num)
            )
        );
    }
};

template
<
    typename FromQ,
    typename ToQ,
    typename RATIO
>
struct quantity_cast_impl<FromQ, ToQ, RATIO, false, false>
{
    constexpr
    ToQ operator()(const FromQ& aFromQuantity) const
    {
        using ResultValue_t = std::common_type_t
        <
            typename ToQ::value_t,
            typename FromQ::value_t,
            intmax_t
        >;
        return ToQ
        (
            static_cast<typename ToQ::value_t>
            (
                static_cast<ResultValue_t>(aFromQuantity.value()) *
                static_cast<ResultValue_t>(RATIO::num) /
                static_cast<ResultValue_t>(RATIO::den)
            )
        );
    }
};

template <typename aType>
struct is_quantity_impl : std::false_type {};

template <typename UNITS, typename VALUE, typename RATIO>
struct is_quantity_impl<quantity<VALUE, RATIO, UNITS>> : std::true_type {};

//------------------------------------------------------------------------------
/// true if aType is an si::quantity, false otherwise
/// @tparam aType a type to be tested
template <typename aType>
constexpr bool is_quantity = is_quantity_impl<typename std::decay<aType>::type>::value;

//------------------------------------------------------------------------------
/// Convert an si::quantity to another si:quantity type.
/// Both types must have the same si::units type.
template <typename ToQ, typename UNITS, typename VALUE, typename RATIO>
inline
constexpr
typename std::enable_if
<
    is_quantity<ToQ> && std::is_same<typename ToQ::units_t,UNITS>::value,
    ToQ
>::type
quantity_cast(const quantity<VALUE, RATIO, UNITS>& aFromQuantity)
{
    return quantity_cast_impl
    <
        quantity<VALUE, RATIO, UNITS>,
        ToQ
    >()(aFromQuantity);
}

// some special quantity values
template <typename VALUE>
struct quantity_values
{
public:
    static constexpr VALUE zero() {return VALUE(0);}
    static constexpr VALUE max()  {return std::numeric_limits<VALUE>::max();}
    static constexpr VALUE min()  {return std::numeric_limits<VALUE>::lowest();}
};

//------------------------------------------------------------------------------
/// Class si::quantity represents a numeric value with associated SI units.
template <typename VALUE, typename RATIO, typename UNITS>
class quantity
{
    static_assert(std::is_arithmetic<VALUE>::value, "VALUE must be an arithmetic type");
    static_assert(ratio::is_ratio<RATIO>, "RATIO must be of type std::ratio");
    static_assert(std::ratio_greater<RATIO, std::ratio<0>>::value, "RATIO must be positive");
    static_assert(is_units<UNITS>, "UNITS must be of type si::units" );

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
    using units_t = UNITS;
    using value_t = VALUE;
    using ratio_t = typename RATIO::type;


    //--------------------------------------------------------------------------
    /// Static member constants
    static constexpr auto ratio = ratio_t{};
    static constexpr auto units = units_t{};

private:

    value_t mValue;

public:

    //--------------------------------------------------------------------------
    constexpr
    quantity
    (
    ) = default;

    //--------------------------------------------------------------------------
    constexpr
    quantity
    (
        const quantity& aQuantity
    ) = default;

    //--------------------------------------------------------------------------
    /// Initialize a quantity from a unitless value.
    /// This constructor will not be chosen by the compiler if it would result in loss of precision.
    /// @param aValue the scalar value that will be stored in this object
    template <typename VALUE2>
    constexpr
    explicit
    quantity
    (
        const VALUE2& aValue,
        typename std::enable_if
        <
            std::is_convertible<VALUE2, value_t>::value &&
            (
                std::is_floating_point<value_t>::value ||
                !std::is_floating_point<VALUE2>::value
            )
        >::type* = 0
    )
    : mValue{aValue}
    {
    }

    //--------------------------------------------------------------------------
    /// Initialize a quantity from another quantity possibly having different value_t and ratio_t types but the same units_t type.
    /// This constructor will not be chosen by the compiler if it would result in overflow or loss of precision.
    /// @param aQuantity the quantity that will be converted to this quantity
    template <typename VALUE2, typename RATIO2>
    constexpr
    quantity
    (
        const quantity<VALUE2, RATIO2, UNITS>& aQuantity,
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
    : mValue{quantity_cast<quantity>(aQuantity).value()}
    {
    }

    //--------------------------------------------------------------------------
    // Accessor function
    constexpr value_t value() const {return mValue;}

    //--------------------------------------------------------------------------
    // Arithmetic functions
    constexpr quantity operator+() const {return *this;}
    constexpr quantity operator-() const {return quantity{-mValue};}
    quantity& operator++() {++mValue; return *this;}
    quantity operator++(int) {return quantity{mValue++};}
    quantity& operator--() {--mValue; return *this;}
    quantity operator--(int) {return quantity{mValue--};}
    quantity& operator+=(const quantity& aQuantity) {mValue += aQuantity.value(); return *this;}
    quantity& operator-=(const quantity& aQuantity) {mValue -= aQuantity.value(); return *this;}
    quantity& operator*=(const value_t& rhs) {mValue *= rhs; return *this;}
    quantity& operator/=(const value_t& rhs) {mValue /= rhs; return *this;}
    quantity& operator%=(const value_t& rhs) {mValue %= rhs; return *this;}
    quantity& operator%=(const quantity& rhs) {mValue %= rhs.value(); return *this;}

    //--------------------------------------------------------------------------
    // Special values
    static constexpr quantity zero() {return quantity{quantity_values<value_t>::zero()};}
    static constexpr quantity min() {return quantity{quantity_values<value_t>::min()};}
    static constexpr quantity max() {return quantity{quantity_values<value_t>::max()};}
}; // end of class quantity

template <typename LhsQ, typename RhsQ>
struct quantity_eq_impl
{
    constexpr
    bool operator()(const LhsQ& aLHS, const RhsQ& aRHS) const
    {
        using CommonQuantity_t = std::common_type_t<LhsQ, RhsQ>;
        return CommonQuantity_t{aLHS}.value() == CommonQuantity_t{aRHS}.value();
    }
};

template <typename LhsQ>
struct quantity_eq_impl<LhsQ, LhsQ>
{
    constexpr
    bool operator()(const LhsQ& aLHS, const LhsQ& aRHS) const
    {
        return aLHS.value() == aRHS.value();
    }
};

template <typename LhsQ, typename RhsQ>
struct quantity_lt_impl
{
    constexpr
    bool operator()(const LhsQ& aLHS, const RhsQ& aRHS) const
    {
        using CommonQuantity_t = std::common_type_t<LhsQ, RhsQ>;
        return CommonQuantity_t{aLHS}.value() < CommonQuantity_t{aRHS}.value();
    }
};

template <typename LhsQ>
struct quantity_lt_impl<LhsQ, LhsQ>
{
    constexpr
    bool operator()(const LhsQ& aLHS, const LhsQ& aRHS) const
    {
        return aLHS.value() < aRHS.value();
    }
};

//------------------------------------------------------------------------------
/// quantity ==
template <typename UNITS, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
bool
operator ==
(
    const quantity<VALUE1, RATIO1, UNITS>& aLHS,
    const quantity<VALUE2, RATIO2, UNITS>& aRHS
)
{
    return quantity_eq_impl
    <
        quantity<VALUE1, RATIO1, UNITS>,
        quantity<VALUE2, RATIO2, UNITS>
    >()(aLHS, aRHS);
}

//------------------------------------------------------------------------------
// quantity !=
template <typename UNITS, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
bool
operator !=
(
    const quantity<VALUE1, RATIO1, UNITS>& aLHS,
    const quantity<VALUE2, RATIO2, UNITS>& aRHS
)
{
    return !(aLHS == aRHS);
}

//------------------------------------------------------------------------------
// quantity <
template <typename UNITS, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
bool
operator <
(
    const quantity<VALUE1, RATIO1, UNITS>& aLHS,
    const quantity<VALUE2, RATIO2, UNITS>& aRHS)
{
    return quantity_lt_impl
    <
        quantity<VALUE1, RATIO1, UNITS>,
        quantity<VALUE2, RATIO2, UNITS>
    >()(aLHS, aRHS);
}

//------------------------------------------------------------------------------
// quantity >
template <typename UNITS, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
bool
operator >
(
    const quantity<VALUE1, RATIO1, UNITS>& aLHS,
    const quantity<VALUE2, RATIO2, UNITS>& aRHS
)
{
    return aRHS < aLHS;
}

//------------------------------------------------------------------------------
// quantity <=
template <typename UNITS, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
bool
operator <=
(
    const quantity<VALUE1, RATIO1, UNITS>& aLHS,
    const quantity<VALUE2, RATIO2, UNITS>& aRHS
)
{
    return !(aRHS < aLHS);
}

//------------------------------------------------------------------------------
// quantity >=
template <typename UNITS, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
bool
operator >=
(
    const quantity<VALUE1, RATIO1, UNITS>& aLHS,
    const quantity<VALUE2, RATIO2, UNITS>& aRHS
)
{
    return !(aLHS < aRHS);
}

//------------------------------------------------------------------------------
// quantity +
template <typename UNITS, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
auto
operator +
(
    const quantity<VALUE1, RATIO1, UNITS>& aLHS,
    const quantity<VALUE2, RATIO2, UNITS>& aRHS
)
{
    using CommonQuantity_t = std::common_type_t
    <
        quantity<VALUE1, RATIO1, UNITS>,
        quantity<VALUE2, RATIO2, UNITS>
    >;
    return CommonQuantity_t{CommonQuantity_t{aLHS}.value() + CommonQuantity_t{aRHS}.value()};
}

//------------------------------------------------------------------------------
// quantity -
template <typename UNITS, typename VALUE1, typename RATIO1, typename VALUE2, typename RATIO2>
inline
constexpr
auto
operator -
(
    const quantity<VALUE1, RATIO1, UNITS>& aLHS,
    const quantity<VALUE2, RATIO2, UNITS>& aRHS
)
{
    using CommonQuantity_t = std::common_type_t
    <
        quantity<VALUE1, RATIO1, UNITS>,
        quantity<VALUE2, RATIO2, UNITS>
    >;
    return CommonQuantity_t{CommonQuantity_t{aLHS}.value() - CommonQuantity_t{aRHS}.value()};
}

//------------------------------------------------------------------------------
// quantity * quantity
template
<
    typename UNITS1,
    typename VALUE1,
    typename RATIO1,
    typename UNITS2,
    typename VALUE2,
    typename RATIO2
>
inline
constexpr
auto
operator *
(
    const quantity<VALUE1, RATIO1, UNITS1>& aLHS,
    const quantity<VALUE2, RATIO2, UNITS2>& aRHS
)
{
    using ResultValue_t = std::common_type_t<VALUE1, VALUE2>;
    using Result_t = quantity
    <
        ResultValue_t,
        std::ratio_multiply<RATIO1, RATIO2>,
        multiply_units<UNITS1, UNITS2>
    >;

    return Result_t
    (
        static_cast<ResultValue_t>( aLHS.value() )
        *
        static_cast<ResultValue_t>( aRHS.value() )
    );
}

//------------------------------------------------------------------------------
// quantity * scalar
template <typename VALUE1, typename RATIO, typename UNITS, typename VALUE2>
inline
constexpr
typename std::enable_if
<
    !is_quantity<VALUE2> &&
    std::is_convertible
    <
        VALUE2,
        std::common_type_t<VALUE1, VALUE2>
    >::value,
    quantity
    <
        std::common_type_t<VALUE1, VALUE2>,
        RATIO,
        UNITS
    >
>::type
operator *
(
    const quantity<VALUE1, RATIO, UNITS>& aQuantity,
    const VALUE2& aScalar
)
{
    return aQuantity * quantity<VALUE2, std::ratio<1>, scalar>{aScalar};
}

//------------------------------------------------------------------------------
// scalar * quantity
template <typename VALUE1, typename RATIO, typename UNITS, typename VALUE2>
inline
constexpr
typename std::enable_if
<
    !is_quantity<VALUE2> &&
    std::is_convertible
    <
        VALUE2,
        std::common_type_t<VALUE1, VALUE2>
    >::value,
    quantity
    <
        std::common_type_t<VALUE1, VALUE2>,
        RATIO,
        UNITS
    >
>::type
operator *
(
    const VALUE2& aScalar,
    const quantity<VALUE1, RATIO, UNITS>& aQuantity
)
{
    return aQuantity * aScalar;
}

template
<
    typename QUANTITY,
    typename VALUE,
    bool = std::is_convertible
    <
        VALUE,
        std::common_type_t<typename QUANTITY::value_t, VALUE>
    >::value
>
struct quantity_divide_helper
{
};

template
<
    typename VALUE1,
    typename RATIO,
    typename UNITS,
    typename VALUE2
>
struct quantity_divide_helper
<
    quantity<VALUE1, RATIO, UNITS>,
    VALUE2,
    true
>
{
    using type = quantity
    <
        std::common_type_t<VALUE1, VALUE2>,
        RATIO,
        UNITS
    >;
};

template
<
    typename QUANTITY,
    typename VALUE,
    bool = is_quantity<VALUE>
>
struct quantity_divide_result_t
{
};

template
<
    typename VALUE1,
    typename RATIO,
    typename UNITS,
    typename VALUE2
>
struct quantity_divide_result_t
<
    quantity<VALUE1, RATIO, UNITS>,
    VALUE2,
    false
>
: quantity_divide_helper<quantity<VALUE1, RATIO, UNITS>, VALUE2>
{
};

//------------------------------------------------------------------------------
// divide quantity by scalar
template
<
    typename VALUE1,
    typename RATIO,
    typename UNITS,
    typename VALUE2
>
inline
constexpr
typename quantity_divide_result_t<quantity<VALUE1, RATIO, UNITS>, VALUE2>::type
operator /
(
    const quantity<VALUE1, RATIO, UNITS>& aQuantity,
    const VALUE2& aScalar
)
{
    using ResultValue_t = std::common_type_t<VALUE1, VALUE2>;
    using Result_t = quantity<ResultValue_t, RATIO, UNITS>;
    return Result_t(Result_t(aQuantity).value() / static_cast<ResultValue_t>(aScalar));
}

//------------------------------------------------------------------------------
// divide quantity by quantity, same units
template
<
    typename UNITS,
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
    const quantity<VALUE1, RATIO1, UNITS>& aLHS,
    const quantity<VALUE2, RATIO2, UNITS>& aRHS
)
{
    using CommonQuantity_t = std::common_type_t
    <
        quantity<VALUE1, RATIO1, UNITS>,
        quantity<VALUE2, RATIO2, UNITS>
    >;
    return CommonQuantity_t(aLHS).value() / CommonQuantity_t(aRHS).value();
}

template
<
    typename VALUE1,
    typename RATIO1,
    typename UNITS1,
    typename VALUE2,
    typename RATIO2,
    typename UNITS2
>
using diff_units_result_t = quantity
<
    std::common_type_t<VALUE1, VALUE2>,
    std::ratio_divide<RATIO1, RATIO2>,
    divide_units<UNITS1, UNITS2>
>;

//------------------------------------------------------------------------------
// divide quantity by quantity, different units
template
<
    typename UNITS1,
    typename VALUE1,
    typename RATIO1,
    typename UNITS2,
    typename VALUE2,
    typename RATIO2
>
inline
constexpr
typename std::enable_if
<
    !std::is_same<UNITS1,UNITS2>::value,
    diff_units_result_t<VALUE1, RATIO1, UNITS1, VALUE2, RATIO2, UNITS2>
>::type
operator /
(
    const quantity<VALUE1, RATIO1, UNITS1>& aLHS,
    const quantity<VALUE2, RATIO2, UNITS2>& aRHS
)
{
    using Result_t = diff_units_result_t<VALUE1, RATIO1, UNITS1, VALUE2, RATIO2, UNITS2>;
    return Result_t( aLHS.value() / aRHS.value() );
}

//------------------------------------------------------------------------------
// divide scalar by quantity
template
<
    typename VALUE1,
    typename RATIO,
    typename UNITS,
    typename VALUE2
>
inline
constexpr
auto
operator /
(
    const VALUE2& aScalar,
    const quantity<VALUE1, RATIO, UNITS>& aQuantity
)
{
    using ResultValue_t = std::common_type_t<VALUE1, VALUE2>;
    return quantity<ResultValue_t, std::ratio<1>, scalar>{aScalar} / aQuantity;
}

//------------------------------------------------------------------------------
// modulo quantity by scalar
template
<
    typename UNITS,
    typename VALUE1,
    typename RATIO,
    typename VALUE2
>
inline
constexpr
typename quantity_divide_result_t<quantity<VALUE1, RATIO, UNITS>, VALUE2>::type
operator%
(
    const quantity<VALUE1, RATIO, UNITS>& aQuantity,
    const VALUE2& aScalar
)
{
    using ResultValue_t = std::common_type_t<VALUE1, VALUE2>;
    using Result_t = quantity<ResultValue_t, RATIO, UNITS>;
    return Result_t(Result_t(aQuantity).value() % static_cast<ResultValue_t>(aScalar));
}

template
<
    typename VALUE1,
    typename RATIO1,
    typename UNITS1,
    typename VALUE2,
    typename RATIO2
>
using mod_result = quantity
<
    std::common_type_t<VALUE1, VALUE2>,
    std::ratio_divide<RATIO1, RATIO2>,
    UNITS1
>;

//------------------------------------------------------------------------------
// modulo quantity by quantity
template
<
    typename VALUE1,
    typename RATIO1,
    typename UNITS1,
    typename VALUE2,
    typename RATIO2,
    typename UNITS2
>
inline
constexpr
mod_result<VALUE1, RATIO1, UNITS1, VALUE2, RATIO2>
operator%
(
    const quantity<VALUE1, RATIO1, UNITS1>& aLHS,
    const quantity<VALUE2, RATIO2, UNITS2>& aRHS
)
{
    return aLHS - (aRHS * (aLHS / aRHS));
}

template
<
    typename VALUE,
    typename RATIO,
    typename UNITS,
    typename EPSILON
>
using sqrt_result_t = typename std::enable_if
<
    std::is_floating_point<VALUE>::value,
    quantity
    <
        VALUE,
        typename ratio::ratio_sqrt<RATIO, EPSILON>::type,
        root_units<UNITS, 2>
    >
>::type;

//------------------------------------------------------------------------------
// square root of a quantity
template
<
    typename VALUE,
    typename RATIO,
    typename UNITS,
    typename EPSILON = std::ratio<1,10000000000000>
>
inline
sqrt_result_t<VALUE, RATIO, UNITS, EPSILON>
sqrt
(
    const quantity<VALUE, RATIO, UNITS>& aQuantity
)
{
    using Result_t = sqrt_result_t<VALUE, RATIO, UNITS, EPSILON>;
    return Result_t{ std::sqrt(aQuantity.value()) };
}

//==============================================================================
// Some useful si::quantity types
template< typename RATIO = std::ratio<1>, typename VALUE = double >
using meters = quantity<VALUE, RATIO, length>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using kilograms = quantity<VALUE, RATIO, mass>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using seconds = quantity<VALUE, RATIO, time>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using minutes = seconds<std::ratio_multiply<RATIO, std::ratio<60>>, VALUE>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using hours = minutes<std::ratio_multiply<RATIO, std::ratio<60>>, VALUE>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using amperes = quantity<VALUE, RATIO, current>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using kelvins = quantity<VALUE, RATIO, temperature>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using candelas = quantity<VALUE, RATIO, luminance>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using radians = quantity<VALUE, RATIO, angle>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using steradians = quantity<VALUE, RATIO, power_units<angle,2>>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using hertz = quantity<VALUE, RATIO, reciprocal_units<time>>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using newtons = quantity<VALUE, RATIO, divide_units<multiply_units<mass, length>, power_units<time,2>>>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using coulombs = quantity<VALUE, RATIO, multiply_units<current, time>>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using lux = quantity<VALUE, RATIO, divide_units<luminance, power_units<length,2>>>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using pascals = quantity<VALUE, RATIO, divide_units<typename newtons<>::units_t, power_units<length,2>>>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using joules = quantity<VALUE, RATIO, multiply_units<typename newtons<>::units_t, length>>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using watts = quantity<VALUE, RATIO, divide_units<typename joules<>::units_t, time>>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using volts = quantity<VALUE, RATIO, divide_units<typename watts<>::units_t, current>>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using farads = quantity<VALUE, RATIO, divide_units<typename coulombs<>::units_t, typename volts<>::units_t>>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using ohms = quantity<VALUE, RATIO, divide_units<typename volts<>::units_t, current>>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using webers = quantity<VALUE, RATIO, multiply_units<typename volts<>::units_t, time>>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using teslas = quantity<VALUE, RATIO, divide_units<typename webers<>::units_t, power_units<length,2>>>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using henries = quantity<VALUE, RATIO, divide_units<typename webers<>::units_t, current>>;

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using lumens = quantity<VALUE, RATIO, multiply_units<luminance, typename steradians<>::units_t>>;

} // end of namespace si
