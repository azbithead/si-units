#pragma once
#include <ratio>
#include <type_traits>
#include <string>
#include "exponent.hpp"
#include "constants.hpp"

namespace si
{

template <typename aType>
struct is_ratio_impl : std::false_type{};

template <std::intmax_t aNum, std::intmax_t aDen>
struct is_ratio_impl<std::ratio<aNum, aDen>> : std::true_type{};

//------------------------------------------------------------------------------
/// true if aType is a std::ratio, false otherwise
/// @tparam aType a type to be tested
template <typename aType>
constexpr bool is_ratio = is_ratio_impl<aType>::value;

template <intmax_t aX, intmax_t aY>
struct gcd_impl
{
    static constexpr intmax_t value = gcd_impl<aY, aX % aY>::value;
};

template <intmax_t aX>
struct gcd_impl<aX, 0>
{
    static constexpr intmax_t value = aX;
};

template <>
struct gcd_impl<0, 0>
{
    static constexpr intmax_t value = 1;
};

//------------------------------------------------------------------------------
/// greatest common divisor (GCD)
template <intmax_t aX, intmax_t aY>
constexpr intmax_t gcd = gcd_impl<aX,aY>::value;

template <intmax_t aX, intmax_t aY>
struct lcm_impl
{
    static constexpr intmax_t value = aX / gcd<aX, aY> * aY;
};

//------------------------------------------------------------------------------
/// least common multiple (LCM)
template <intmax_t aX, intmax_t aY>
constexpr intmax_t lcm = lcm_impl<aX,aY>::value;

//------------------------------------------------------------------------------
/// the std::ratio that is the greatest common divisor (GCD) of aRatioX and aRatioY
/// @tparam aRatioX a std::ratio
/// @tparam aRatioY a std::ratio
template <typename aRatioX, typename aRatioY>
using ratio_gcd = std::ratio
<
    gcd
    <
        aRatioX::num,
        aRatioY::num
    >,
    lcm
    <
        aRatioX::den,
        aRatioY::den
    >
>;

using r_zero = std::ratio<0>;
using r_one = std::ratio<1>;

template <typename R> using Square = std::ratio_multiply<R, R>;

// Find the largest integer N such that Predicate<N>::value is true.
template <template <std::intmax_t N> class Predicate, typename Enabled = void>
struct binary_search
{
    template <std::intmax_t N>
    struct safe_double_
    {
        constexpr std::intmax_t static value = 2 * N;
        static_assert(value > 0, "Overflows when computing 2 * N");
    };

    template< std::intmax_t Lower, std::intmax_t Upper >
    struct mid : std::integral_constant<std::intmax_t, Lower + (Upper - Lower) / 2>
    {
    };

    template <std::intmax_t Lower, std::intmax_t Upper, typename Enabled1 = void>
    struct double_sided_search_ : double_sided_search_
    <
        Lower,
        mid<Lower, Upper>::value
    >
    {
    };

    template <std::intmax_t Lower, std::intmax_t Upper>
    struct double_sided_search_
    <
        Lower,
        Upper,
        typename std::enable_if<Upper-Lower==1>::type
    > : std::integral_constant<std::intmax_t, Lower>
    {
    };

    template <std::intmax_t Lower, std::intmax_t Upper>
    struct double_sided_search_
    <
        Lower,
        Upper,
        typename std::enable_if<(Upper-Lower>1 && Predicate<mid<Lower, Upper>::value>::value)>::type
    > : double_sided_search_<mid<Lower, Upper>::value, Upper>
    {
    };

    template <std::intmax_t Lower, typename Enabled1 = void>
    struct single_sided_search_ : double_sided_search_<Lower, safe_double_<Lower>::value>
    {
    };

    template <std::intmax_t Lower>
    struct single_sided_search_
    <
        Lower,
        typename std::enable_if<Predicate<safe_double_<Lower>::value>::value>::type
    > : single_sided_search_<safe_double_<Lower>::value>
    {
    };

    static constexpr std::intmax_t value = single_sided_search_<1>::value;
};

template <template <std::intmax_t N> class Predicate>
struct binary_search
<
    Predicate,
    typename std::enable_if<!Predicate<1>::value>::type
> : std::integral_constant<std::intmax_t, 0>
{
};

// Find largest integer N such that N<=sqrt(RatioT)
template <typename RatioT>
struct Integer
{
    template <std::intmax_t N>
    using Predicate_ = std::ratio_less_equal<std::ratio<N>, std::ratio_divide<RatioT, std::ratio<N>>>;

    static constexpr std::intmax_t value = binary_search<Predicate_>::value;
};

template <typename RatioT>
struct IsPerfectSquare
{
    static constexpr std::intmax_t DenSqrt_ = Integer<std::ratio<RatioT::den>>::value;
    static constexpr std::intmax_t NumSqrt_ = Integer<std::ratio<RatioT::num>>::value;
    static constexpr bool value = DenSqrt_ * DenSqrt_ == RatioT::den && NumSqrt_ * NumSqrt_ == RatioT::num;
    using Sqrt = std::ratio<NumSqrt_, DenSqrt_>;
};

// Represents sqrt(P)-Q.
template <typename aRatioP, typename aRatioQ>
struct Remainder
{
    using P = aRatioP;
    using Q = aRatioQ;
};

// Represents 1/R = I + Rem where R is a Remainder.
template <typename R>
struct Reciprocal
{
    using P_ = typename R::P;
    using Q_ = typename R::Q;
    using Den_ = std::ratio_subtract<P_, Square<Q_>>;
    using A_ = std::ratio_divide<Q_, Den_>;
    using B_ = std::ratio_divide<P_, Square<Den_>>;
    static constexpr std::intmax_t I_ = (A_::num + Integer<std::ratio_multiply<B_, Square<std::ratio<A_::den>>>>::value) / A_::den;
    using I = std::ratio<I_>;
    using Rem = Remainder<B_, std::ratio_subtract<I, A_>>;
};

// Expands sqrt(R) to continued fraction:
// f(x)=C1+1/(C2+1/(C3+1/(...+1/(Cn+x)))) = (U*x+V)/(W*x+1) and sqrt(R)=f(Rem).
// The error |f(Rem)-V| = |(U-W*V)x/(W*x+1)| <= |U-W*V|*Rem <= |U-W*V|/I' where
// I' is the integer part of reciprocal of Rem.
template <typename aRatio, std::intmax_t N>
struct continued_fraction
{
    template <typename T>
    using Abs_ = typename std::conditional
    <
        std::ratio_less<T, r_zero>::value,
        std::ratio_subtract<r_zero, T>,
        T
    >::type;

    using Last_ = continued_fraction<aRatio, N-1>;
    using Reciprocal_ = Reciprocal<typename Last_::Rem>;
    using Rem = typename Reciprocal_::Rem;
    using I_ = typename Reciprocal_::I;
    using Den_ = std::ratio_add<typename Last_::W, I_>;
    using U = std::ratio_divide<typename Last_::V, Den_>;
    using V = std::ratio_divide<std::ratio_add<typename Last_::U, std::ratio_multiply<typename Last_::V, I_>>, Den_>;
    using W = std::ratio_divide<r_one, Den_>;
    using Error = Abs_<std::ratio_divide<std::ratio_subtract<U, std::ratio_multiply<V, W>>, typename Reciprocal<Rem>::I>>;
};

template <typename aRatio>
struct continued_fraction<aRatio, 1>
{
    using U = r_one;
    using V = std::ratio<Integer<aRatio>::value>;
    using W = r_zero;
    using Rem = Remainder<aRatio, V>;
    using Error = std::ratio_divide<r_one, typename Reciprocal<Rem>::I>;
};

template <typename aRatio, typename aEpsilon, std::intmax_t N=1, typename Enabled = void>
struct ratio_sqrt_impl : ratio_sqrt_impl<aRatio, aEpsilon, N+1>
{
};

template <typename aRatio, typename aEpsilon, std::intmax_t N>
struct ratio_sqrt_impl
<
    aRatio,
    aEpsilon,
    N,
    typename std::enable_if
    <
        std::ratio_less_equal<typename continued_fraction<aRatio, N>::Error, aEpsilon>::value
    >::type
>
{
    using type = typename continued_fraction<aRatio, N>::V;
};

//------------------------------------------------------------------------------
/// the std::ratio that is the square root of aRatio
/// @tparam aRatio a ratio to take the square root of
/// @tparam aEpsilon a ratio expressing the greatest acceptable error
template <typename aRatio, typename aEpsilon, typename isEnabled = void>
struct ratio_sqrt
{
    static_assert(std::ratio_greater_equal<aRatio, r_zero>::value, "R can't be negative");
};

template <typename aRatio, typename aEpsilon>
struct ratio_sqrt
<
    aRatio,
    aEpsilon,
    typename std::enable_if
    <
        std::ratio_greater_equal<aRatio, r_zero>::value && IsPerfectSquare<aRatio>::value
    >::type
>
{
    using type = typename IsPerfectSquare<aRatio>::Sqrt;
};

template <typename aRatio, typename aEpsilon>
struct ratio_sqrt
<
    aRatio,
    aEpsilon,
    typename std::enable_if
    <
        std::ratio_greater_equal<aRatio, r_zero>::value && !IsPerfectSquare<aRatio>::value
    >::type
> : ratio_sqrt_impl<aRatio, aEpsilon>
{
};

template
<
    typename Ratio,
    std::intmax_t Exp = 0,
    bool NumMod = ((Ratio::num % 10) != 0),
    bool DenMod = ((Ratio::den % 10) != 0)
>
struct sci_t;

template
<
    typename Ratio,
    std::intmax_t Exp
>
struct sci_t<Ratio, Exp, true, true>
{
    using ratio = Ratio;
    using exponent = exponent_t<Exp>;
};

template
<
    typename Ratio,
    std::intmax_t Exp
>
struct sci_t<Ratio, Exp, false, true>
{
    using temp = sci_t<std::ratio<Ratio::num / 10, Ratio::den>, Exp + 1>;
    using ratio = std::ratio<temp::ratio::num,temp::ratio::den>;
    using exponent = exponent_t<temp::exponent::value>;
};

template
<
    typename Ratio,
    std::intmax_t Exp
>
struct sci_t<Ratio, Exp, true, false>
{
    using temp = sci_t<std::ratio<Ratio::num, Ratio::den / 10>, Exp - 1>;
    using ratio = std::ratio<temp::ratio::num,temp::ratio::den>;
    using exponent = exponent_t<temp::exponent::value>;
};

template<typename CharT>
inline
std::basic_string<CharT>
basic_string_from
(
    long aInt
);

template<>
inline
std::string
basic_string_from<char>
(
    long aInt
)
{
    return std::to_string(aInt);
}

template<>
inline
std::wstring
basic_string_from<wchar_t>
    (
    long aInt
)
{
    return std::to_wstring(aInt);
}

template
<
    typename CharT,
    std::intmax_t Num,
    std::intmax_t Den
>
inline
std::basic_string<CharT>
basic_string_from
(
    std::ratio<Num,Den> aRatio
)
{
    std::basic_string<CharT> theResult;

    using sci = sci_t<decltype(aRatio)>;

    if( sci::ratio::num != sci::ratio::den )
    {
        theResult = basic_string_from<CharT>(sci::ratio::num);
    }

    if( sci::ratio::den != 1 )
    {
        theResult += divide_operator<CharT> + basic_string_from<CharT>(sci::ratio::den);
    }

    if( sci::exponent::value != 0 )
    {
        if( !theResult.empty() )
        {
            theResult += multiply_operator<CharT>;
        }

        theResult += basic_string_from<CharT>(10) + basic_string_from<CharT>(typename sci::exponent{});
    }

    return theResult;
}

} // end of namespace si

#include "string-from.hpp"
