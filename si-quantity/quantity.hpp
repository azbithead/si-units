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
template <typename STORAGE, typename RATIO, typename UNITS > class quantity;

} // end of namespace si

//------------------------------------------------------------------------------
/// Specialization of std::common_type for quantity.
template
<
    typename UNITS,
    typename STORAGE1,
    typename RATIO1,
    typename STORAGE2,
    typename RATIO2
>
struct std::common_type
<
    si::quantity<STORAGE1, RATIO1, UNITS>,
    si::quantity<STORAGE2, RATIO2, UNITS>
>
{
    using type = si::quantity
    <
        typename std::common_type<STORAGE1, STORAGE2>::type,
        si::ratio_gcd<RATIO1, RATIO2>,
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
            static_cast<typename ToQ::storage_t>(aFromQuantity.value())
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
        using ResultStorage_t = typename std::common_type
        <
            typename ToQ::storage_t,
            typename FromQ::storage_t,
            intmax_t
        >::type;
        return ToQ
        (
            static_cast<typename ToQ::storage_t>
            (
                static_cast<ResultStorage_t>(aFromQuantity.value()) /
                static_cast<ResultStorage_t>(RATIO::den)
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
        using ResultStorage_t = typename std::common_type
        <
            typename ToQ::storage_t,
            typename FromQ::storage_t,
            intmax_t
        >::type;
        return ToQ
        (
            static_cast<typename ToQ::storage_t>
            (
                static_cast<ResultStorage_t>(aFromQuantity.value()) *
                static_cast<ResultStorage_t>(RATIO::num)
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
        using ResultStorage_t = typename std::common_type
        <
            typename ToQ::storage_t,
            typename FromQ::storage_t,
            intmax_t
        >::type;
        return ToQ
        (
            static_cast<typename ToQ::storage_t>
            (
                static_cast<ResultStorage_t>(aFromQuantity.value()) *
                static_cast<ResultStorage_t>(RATIO::num) /
                static_cast<ResultStorage_t>(RATIO::den)
            )
        );
    }
};

template <typename aType>
struct is_quantity_impl : std::false_type {};

template <typename UNITS, typename STORAGE, typename RATIO>
struct is_quantity_impl<quantity<STORAGE, RATIO, UNITS>> : std::true_type {};

//------------------------------------------------------------------------------
/// @param aType a type to be tested
/// @return true if aType is an si::quantity, false otherwise
template <typename aType>
constexpr bool is_quantity = is_quantity_impl<typename std::remove_cv<aType>::type>::value;

//------------------------------------------------------------------------------
/// Convert an si::quantity to another si:quantity type.
/// Both types must have the same si::units type.
template <typename ToQ, typename UNITS, typename STORAGE, typename RATIO>
inline
constexpr
typename std::enable_if
<
    is_quantity<ToQ> && std::is_same<typename ToQ::units_t,UNITS>::value,
    ToQ
>::type
quantity_cast(const quantity<STORAGE, RATIO, UNITS>& aFromQuantity)
{
    return quantity_cast_impl
    <
        quantity<STORAGE, RATIO, UNITS>,
        ToQ
    >()(aFromQuantity);
}

// some special quantity values
template <typename STORAGE>
struct quantity_values
{
public:
    static constexpr STORAGE zero() {return STORAGE(0);}
    static constexpr STORAGE max()  {return std::numeric_limits<STORAGE>::max();}
    static constexpr STORAGE min()  {return std::numeric_limits<STORAGE>::lowest();}
};

//------------------------------------------------------------------------------
/// Class si::quantity represents a numeric value with associated SI units.
template <typename STORAGE, typename RATIO, typename UNITS>
class quantity
{
    static_assert(std::is_arithmetic<STORAGE>::value, "STORAGE must be an arithmetic type");
    static_assert(is_ratio<RATIO>, "RATIO must be of type std::ratio");
    static_assert(std::ratio_greater<RATIO, std::ratio<0>>::value, "RATIO must be positive");
    static_assert(is_units<UNITS>, "UNITS must be of type si::units" );

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
    using units_t = UNITS;
    using storage_t = STORAGE;
    using ratio_t = typename RATIO::type;


    //--------------------------------------------------------------------------
    /// Static member constants
    static constexpr auto ratio = RATIO{};
    static constexpr auto units = UNITS{};

private:

    storage_t mValue;

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
    /// @param aValue the unitless value that will be stored in this object
    template <typename STORAGE2>
    constexpr
    explicit
    quantity
    (
        const STORAGE2& aValue,
        typename std::enable_if
        <
            std::is_convertible<STORAGE2, storage_t>::value &&
            (
                std::is_floating_point<storage_t>::value ||
                !std::is_floating_point<STORAGE2>::value
            )
        >::type* = 0
    )
    : mValue{aValue}
    {
    }

    //--------------------------------------------------------------------------
    /// Initialize a quantity from another quantity possibly having different storage_t and ratio_t types but the same units_t type.
    /// This constructor will not be chosen by the compiler if it would result in overflow or loss of precision.
    /// @param aQuantity the quantity that will be converted to this quantity
    template <typename STORAGE2, typename RATIO2>
    constexpr
    quantity
    (
        const quantity<STORAGE2, RATIO2, UNITS>& aQuantity,
        typename std::enable_if
        <
            no_overflow<RATIO2, ratio_t>::value &&
            (
                std::is_floating_point<storage_t>::value ||
                (
                    no_overflow<RATIO2, ratio_t>::type::den == 1 &&
                    !std::is_floating_point<STORAGE2>::value
                )
            )
        >::type* = 0
    )
    : mValue{quantity_cast<quantity>(aQuantity).value()}
    {
    }

    //--------------------------------------------------------------------------
    // Accessor function
    constexpr storage_t value() const {return mValue;}

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
    quantity& operator*=(const storage_t& rhs) {mValue *= rhs; return *this;}
    quantity& operator/=(const storage_t& rhs) {mValue /= rhs; return *this;}
    quantity& operator%=(const storage_t& rhs) {mValue %= rhs; return *this;}
    quantity& operator%=(const quantity& rhs) {mValue %= rhs.value(); return *this;}

    //--------------------------------------------------------------------------
    // Special values
    static constexpr quantity zero() {return quantity{quantity_values<storage_t>::zero()};}
    static constexpr quantity min() {return quantity{quantity_values<storage_t>::min()};}
    static constexpr quantity max() {return quantity{quantity_values<storage_t>::max()};}
}; // end of class quantity

template <typename LhsQ, typename RhsQ>
struct quantity_eq_impl
{
    constexpr
    bool operator()(const LhsQ& aLHS, const RhsQ& aRHS) const
    {
        using CommonQuantity_t = typename std::common_type<LhsQ, RhsQ>::type;
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
        using CommonQuantity_t = typename std::common_type<LhsQ, RhsQ>::type;
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
template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
constexpr
bool
operator ==
(
    const quantity<STORAGE1, RATIO1, UNITS>& aLHS,
    const quantity<STORAGE2, RATIO2, UNITS>& aRHS
)
{
    return quantity_eq_impl
    <
        quantity<STORAGE1, RATIO1, UNITS>,
        quantity<STORAGE2, RATIO2, UNITS>
    >()(aLHS, aRHS);
}

//------------------------------------------------------------------------------
// quantity !=
template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
constexpr
bool
operator !=
(
    const quantity<STORAGE1, RATIO1, UNITS>& aLHS,
    const quantity<STORAGE2, RATIO2, UNITS>& aRHS
)
{
    return !(aLHS == aRHS);
}

//------------------------------------------------------------------------------
// quantity <
template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
constexpr
bool
operator <
(
    const quantity<STORAGE1, RATIO1, UNITS>& aLHS,
    const quantity<STORAGE2, RATIO2, UNITS>& aRHS)
{
    return quantity_lt_impl
    <
        quantity<STORAGE1, RATIO1, UNITS>,
        quantity<STORAGE2, RATIO2, UNITS>
    >()(aLHS, aRHS);
}

//------------------------------------------------------------------------------
// quantity >
template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
constexpr
bool
operator >
(
    const quantity<STORAGE1, RATIO1, UNITS>& aLHS,
    const quantity<STORAGE2, RATIO2, UNITS>& aRHS
)
{
    return aRHS < aLHS;
}

//------------------------------------------------------------------------------
// quantity <=
template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
constexpr
bool
operator <=
(
    const quantity<STORAGE1, RATIO1, UNITS>& aLHS,
    const quantity<STORAGE2, RATIO2, UNITS>& aRHS
)
{
    return !(aRHS < aLHS);
}

//------------------------------------------------------------------------------
// quantity >=
template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
constexpr
bool
operator >=
(
    const quantity<STORAGE1, RATIO1, UNITS>& aLHS,
    const quantity<STORAGE2, RATIO2, UNITS>& aRHS
)
{
    return !(aLHS < aRHS);
}

//------------------------------------------------------------------------------
// quantity +
template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
constexpr
auto
operator +
(
    const quantity<STORAGE1, RATIO1, UNITS>& aLHS,
    const quantity<STORAGE2, RATIO2, UNITS>& aRHS
)
{
    using CommonQuantity_t = typename std::common_type
    <
        quantity<STORAGE1, RATIO1, UNITS>,
        quantity<STORAGE2, RATIO2, UNITS>
    >::type;
    return CommonQuantity_t{CommonQuantity_t{aLHS}.value() + CommonQuantity_t{aRHS}.value()};
}

//------------------------------------------------------------------------------
// quantity -
template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
constexpr
auto
operator -
(
    const quantity<STORAGE1, RATIO1, UNITS>& aLHS,
    const quantity<STORAGE2, RATIO2, UNITS>& aRHS
)
{
    using CommonQuantity_t = typename std::common_type
    <
        quantity<STORAGE1, RATIO1, UNITS>,
        quantity<STORAGE2, RATIO2, UNITS>
    >::type;
    return CommonQuantity_t{CommonQuantity_t{aLHS}.value() - CommonQuantity_t{aRHS}.value()};
}

//------------------------------------------------------------------------------
// quantity * quantity
template
<
    typename UNITS1,
    typename STORAGE1,
    typename RATIO1,
    typename UNITS2,
    typename STORAGE2,
    typename RATIO2
>
inline
constexpr
auto
operator *
(
    const quantity<STORAGE1, RATIO1, UNITS1>& aLHS,
    const quantity<STORAGE2, RATIO2, UNITS2>& aRHS
)
{
    using ResultStorage_t = typename std::common_type<STORAGE1, STORAGE2>::type;
    using Result_t = quantity
    <
        ResultStorage_t,
        std::ratio_multiply<RATIO1, RATIO2>,
        multiply_units<UNITS1, UNITS2>
    >;

    return Result_t
    (
        static_cast<ResultStorage_t>( aLHS.value() )
        *
        static_cast<ResultStorage_t>( aRHS.value() )
    );
}

//------------------------------------------------------------------------------
// quantity * scalar
template <typename STORAGE1, typename RATIO, typename UNITS, typename STORAGE2>
inline
constexpr
typename std::enable_if
<
    !is_quantity<STORAGE2> &&
    std::is_convertible
    <
        STORAGE2,
        typename std::common_type<STORAGE1, STORAGE2>::type
    >::value,
    quantity
    <
        typename std::common_type<STORAGE1, STORAGE2>::type,
        RATIO,
        UNITS
    >
>::type
operator *
(
    const quantity<STORAGE1, RATIO, UNITS>& aQuantity,
    const STORAGE2& aScalar
)
{
    return aQuantity * quantity<STORAGE2, std::ratio<1>, scalar>{aScalar};
}

//------------------------------------------------------------------------------
// scalar * quantity
template <typename STORAGE1, typename RATIO, typename UNITS, typename STORAGE2>
inline
constexpr
typename std::enable_if
<
    !is_quantity<STORAGE2> &&
    std::is_convertible
    <
        STORAGE2,
        typename std::common_type<STORAGE1, STORAGE2>::type
    >::value,
    quantity
    <
        typename std::common_type<STORAGE1, STORAGE2>::type,
        RATIO,
        UNITS
    >
>::type
operator *
(
    const STORAGE2& aScalar,
    const quantity<STORAGE1, RATIO, UNITS>& aQuantity
)
{
    return aQuantity * aScalar;
}

template
<
    typename QUANTITY,
    typename STORAGE,
    bool = std::is_convertible
    <
        STORAGE,
        typename std::common_type<typename QUANTITY::storage_t, STORAGE>::type
    >::value
>
struct quantity_divide_helper
{
};

template
<
    typename STORAGE1,
    typename RATIO,
    typename UNITS,
    typename STORAGE2
>
struct quantity_divide_helper
<
    quantity<STORAGE1, RATIO, UNITS>,
    STORAGE2,
    true
>
{
    using type = quantity
    <
        typename std::common_type<STORAGE1, STORAGE2>::type,
        RATIO,
        UNITS
    >;
};

template
<
    typename QUANTITY,
    typename STORAGE,
    bool = is_quantity<STORAGE>
>
struct quantity_divide_result_t
{
};

template
<
    typename STORAGE1,
    typename RATIO,
    typename UNITS,
    typename STORAGE2
>
struct quantity_divide_result_t
<
    quantity<STORAGE1, RATIO, UNITS>,
    STORAGE2,
    false
>
: quantity_divide_helper<quantity<STORAGE1, RATIO, UNITS>, STORAGE2>
{
};

//------------------------------------------------------------------------------
// divide quantity by scalar
template
<
    typename STORAGE1,
    typename RATIO,
    typename UNITS,
    typename STORAGE2
>
inline
constexpr
typename quantity_divide_result_t<quantity<STORAGE1, RATIO, UNITS>, STORAGE2>::type
operator /
(
    const quantity<STORAGE1, RATIO, UNITS>& aQuantity,
    const STORAGE2& aScalar
)
{
    using ResultStorage_t = typename std::common_type<STORAGE1, STORAGE2>::type;
    using Result_t = quantity<ResultStorage_t, RATIO, UNITS>;
    return Result_t(Result_t(aQuantity).value() / static_cast<ResultStorage_t>(aScalar));
}

//------------------------------------------------------------------------------
// divide quantity by quantity, same units
template
<
    typename UNITS,
    typename STORAGE1,
    typename RATIO1,
    typename STORAGE2,
    typename RATIO2
>
inline
constexpr
auto
operator /
(
    const quantity<STORAGE1, RATIO1, UNITS>& aLHS,
    const quantity<STORAGE2, RATIO2, UNITS>& aRHS
)
{
    using CommonQuantity_t = typename std::common_type
    <
        quantity<STORAGE1, RATIO1, UNITS>,
        quantity<STORAGE2, RATIO2, UNITS>
    >::type;
    return CommonQuantity_t(aLHS).value() / CommonQuantity_t(aRHS).value();
}

template
<
    typename STORAGE1,
    typename RATIO1,
    typename UNITS1,
    typename STORAGE2,
    typename RATIO2,
    typename UNITS2
>
using diff_units_result_t = quantity
<
    typename std::common_type<STORAGE1, STORAGE2>::type,
    std::ratio_divide<RATIO1, RATIO2>,
    divide_units<UNITS1, UNITS2>
>;

//------------------------------------------------------------------------------
// divide quantity by quantity, different units
template
<
    typename UNITS1,
    typename STORAGE1,
    typename RATIO1,
    typename UNITS2,
    typename STORAGE2,
    typename RATIO2
>
inline
constexpr
typename std::enable_if
<
    !std::is_same<UNITS1,UNITS2>::value,
    diff_units_result_t<STORAGE1, RATIO1, UNITS1, STORAGE2, RATIO2, UNITS2>
>::type
operator /
(
    const quantity<STORAGE1, RATIO1, UNITS1>& aLHS,
    const quantity<STORAGE2, RATIO2, UNITS2>& aRHS
)
{
    using Result_t = diff_units_result_t<STORAGE1, RATIO1, UNITS1, STORAGE2, RATIO2, UNITS2>;
    return Result_t( aLHS.value() / aRHS.value() );
}

//------------------------------------------------------------------------------
// divide scalar by quantity
template
<
    typename STORAGE1,
    typename RATIO,
    typename UNITS,
    typename STORAGE2
>
inline
constexpr
auto
operator /
(
    const STORAGE2& aScalar,
    const quantity<STORAGE1, RATIO, UNITS>& aQuantity
)
{
    using ResultStorage_t = typename std::common_type<STORAGE1, STORAGE2>::type;
    return quantity<ResultStorage_t, std::ratio<1>, scalar>{aScalar} / aQuantity;
}

//------------------------------------------------------------------------------
// modulo quantity by scalar
template
<
    typename UNITS,
    typename STORAGE1,
    typename RATIO,
    typename STORAGE2
>
inline
constexpr
typename quantity_divide_result_t<quantity<STORAGE1, RATIO, UNITS>, STORAGE2>::type
operator%
(
    const quantity<STORAGE1, RATIO, UNITS>& aQuantity,
    const STORAGE2& aScalar
)
{
    using ResultStorage_t = typename std::common_type<STORAGE1, STORAGE2>::type;
    using Result_t = quantity<ResultStorage_t, RATIO, UNITS>;
    return Result_t(Result_t(aQuantity).value() % static_cast<ResultStorage_t>(aScalar));
}

#if 0
template
<
    typename STORAGE1,
    typename RATIO1,
    typename UNITS1,
    typename STORAGE2,
    typename RATIO2
>
using mod_result = quantity
<
    typename std::common_type<STORAGE1, STORAGE2>::type,
    std::ratio_divide<RATIO1, RATIO2>,
    UNITS1
>;

//------------------------------------------------------------------------------
// modulo quantity by quantity
template
<
    typename STORAGE1,
    typename RATIO1,
    typename UNITS1,
    typename STORAGE2,
    typename RATIO2,
    typename UNITS2
>
inline
constexpr
mod_result<STORAGE1, RATIO1, UNITS1, STORAGE2, RATIO2>
operator%
(
    const quantity<STORAGE1, RATIO1, UNITS1>& aLHS,
    const quantity<STORAGE2, RATIO2, UNITS2>& aRHS
)
{
    using Result_t = mod_result<STORAGE1, RATIO1, UNITS1, STORAGE2, RATIO2>;
    using ResultStorage_t = typename Result_t::storage_t;
    auto x =
    return Result_t(static_cast<ResultStorage_t>(Result_t{aLHS.value()}.value % static_cast<ResultStorage_t>(Result_t(aRHS).value()));
}
#endif

template
<
    typename STORAGE,
    typename RATIO,
    typename UNITS,
    typename EPSILON
>
using sqrt_result_t = typename std::enable_if
<
    std::is_floating_point<STORAGE>::value,
    quantity
    <
        STORAGE,
        typename ratio_sqrt<RATIO, EPSILON>::type,
        root_units<UNITS, 2>
    >
>::type;

//------------------------------------------------------------------------------
// square root of a quantity
template
<
    typename STORAGE,
    typename RATIO,
    typename UNITS,
    typename EPSILON = std::ratio<1,10000000000000>
>
sqrt_result_t<STORAGE, RATIO, UNITS, EPSILON>
sqrt
(
    const quantity<STORAGE, RATIO, UNITS>& aQuantity
)
{
    using Result_t = sqrt_result_t<STORAGE, RATIO, UNITS, EPSILON>;
    return Result_t{ std::sqrt(aQuantity.value()) };
}

//==============================================================================
// Some useful si::quantity types
template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using meters = quantity<STORAGE, RATIO, distance>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using kilograms = quantity<STORAGE, RATIO, mass>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using seconds = quantity<STORAGE, RATIO, time>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using amperes = quantity<STORAGE, RATIO, current>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using kelvins = quantity<STORAGE, RATIO, temperature>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using candelas = quantity<STORAGE, RATIO, luminance>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using radians = quantity<STORAGE, RATIO, angle>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using steradians = quantity<STORAGE, RATIO, power_units<angle,2>>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using hertz = quantity<STORAGE, RATIO, reciprocal_units<time>>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using newtons = quantity<STORAGE, RATIO, divide_units<multiply_units<mass, distance>, power_units<time,2>>>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using coulombs = quantity<STORAGE, RATIO, multiply_units<current, time>>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using lux = quantity<STORAGE, RATIO, divide_units<luminance, power_units<distance,2>>>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using pascals = quantity<STORAGE, RATIO, divide_units<typename newtons<>::units_t, power_units<distance,2>>>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using joules = quantity<STORAGE, RATIO, multiply_units<typename newtons<>::units_t, distance>>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using watts = quantity<STORAGE, RATIO, divide_units<typename joules<>::units_t, time>>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using volts = quantity<STORAGE, RATIO, divide_units<typename watts<>::units_t, current>>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using farads = quantity<STORAGE, RATIO, divide_units<typename coulombs<>::units_t, typename volts<>::units_t>>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using ohms = quantity<STORAGE, RATIO, divide_units<typename volts<>::units_t, current>>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using webers = quantity<STORAGE, RATIO, multiply_units<typename volts<>::units_t, time>>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using teslas = quantity<STORAGE, RATIO, divide_units<typename webers<>::units_t, power_units<distance,2>>>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using henries = quantity<STORAGE, RATIO, divide_units<typename webers<>::units_t, current>>;

template< typename RATIO = std::ratio<1>, typename STORAGE = double >
using lumens = quantity<STORAGE, RATIO, multiply_units<luminance, typename steradians<>::units_t>>;

} // end of namespace si
