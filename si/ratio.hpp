#pragma once
#include <ratio>
#include <type_traits>
#include <string>

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

using Zero = std::ratio<0>;
using One = std::ratio<1>;
template <typename R> using Square = std::ratio_multiply<R, R>;

// Find the largest integer N such that Predicate<N>::value is true.
template <template <intmax_t N> class Predicate, typename Enabled = void>
struct BinarySearch
{
    template <intmax_t N>
    struct SafeDouble_
    {
        constexpr intmax_t static value = 2 * N;
        static_assert(value > 0, "Overflows when computing 2 * N");
    };

    template <intmax_t Lower, intmax_t Upper, typename Enabled1 = void>
    struct DoubleSidedSearch_ : DoubleSidedSearch_<Lower, Lower+(Upper-Lower)/2>
    {
    };

    template <intmax_t Lower, intmax_t Upper>
    struct DoubleSidedSearch_<Lower, Upper, typename std::enable_if<Upper-Lower==1>::type> : std::integral_constant<intmax_t, Lower>
    {
    };

    template <intmax_t Lower, intmax_t Upper>
    struct DoubleSidedSearch_<Lower, Upper, typename std::enable_if<(Upper-Lower>1 && Predicate<Lower+(Upper-Lower)/2>::value)>::type>
      : DoubleSidedSearch_<Lower+(Upper-Lower)/2, Upper>
    {
    };

    template <intmax_t Lower, typename Enabled1 = void>
    struct SingleSidedSearch_ : DoubleSidedSearch_<Lower, SafeDouble_<Lower>::value>
    {
    };

    template <intmax_t Lower>
    struct SingleSidedSearch_<Lower, typename std::enable_if<Predicate<SafeDouble_<Lower>::value>::value>::type>
      : SingleSidedSearch_<SafeDouble_<Lower>::value>
    {
    };

    static constexpr intmax_t value = SingleSidedSearch_<1>::value;
};

template <template <intmax_t N> class Predicate>
struct BinarySearch<Predicate, typename std::enable_if<!Predicate<1>::value>::type> : std::integral_constant<intmax_t, 0>
{
};

// Find largest integer N such that N<=sqrt(R)
template <typename R>
struct Integer
{
    template <intmax_t N>
    using Predicate_ = std::ratio_less_equal<std::ratio<N>, std::ratio_divide<R, std::ratio<N>>>;

    static constexpr intmax_t value = BinarySearch<Predicate_>::value;
};

template <typename R>
struct IsPerfectSquare
{
    static constexpr intmax_t DenSqrt_ = Integer<std::ratio<R::den>>::value;
    static constexpr intmax_t NumSqrt_ = Integer<std::ratio<R::num>>::value;
    static constexpr bool value = DenSqrt_ * DenSqrt_ == R::den && NumSqrt_ * NumSqrt_ == R::num;
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
    static constexpr intmax_t I_ = (A_::num + Integer<std::ratio_multiply<B_, Square<std::ratio<A_::den>>>>::value) / A_::den;
    using I = std::ratio<I_>;
    using Rem = Remainder<B_, std::ratio_subtract<I, A_>>;
};

// Expands sqrt(R) to continued fraction:
// f(x)=C1+1/(C2+1/(C3+1/(...+1/(Cn+x)))) = (U*x+V)/(W*x+1) and sqrt(R)=f(Rem).
// The error |f(Rem)-V| = |(U-W*V)x/(W*x+1)| <= |U-W*V|*Rem <= |U-W*V|/I' where
// I' is the integer part of reciprocal of Rem.
template <typename aRatio, intmax_t N>
struct continued_fraction
{
    template <typename T>
    using Abs_ = typename std::conditional
    <
        std::ratio_less<T, Zero>::value,
        std::ratio_subtract<Zero, T>,
        T
    >::type;

    using Last_ = continued_fraction<aRatio, N-1>;
    using Reciprocal_ = Reciprocal<typename Last_::Rem>;
    using Rem = typename Reciprocal_::Rem;
    using I_ = typename Reciprocal_::I;
    using Den_ = std::ratio_add<typename Last_::W, I_>;
    using U = std::ratio_divide<typename Last_::V, Den_>;
    using V = std::ratio_divide<std::ratio_add<typename Last_::U, std::ratio_multiply<typename Last_::V, I_>>, Den_>;
    using W = std::ratio_divide<One, Den_>;
    using Error = Abs_<std::ratio_divide<std::ratio_subtract<U, std::ratio_multiply<V, W>>, typename Reciprocal<Rem>::I>>;
};

template <typename aRatio>
struct continued_fraction<aRatio, 1>
{
    using U = One;
    using V = std::ratio<Integer<aRatio>::value>;
    using W = Zero;
    using Rem = Remainder<aRatio, V>;
    using Error = std::ratio_divide<One, typename Reciprocal<Rem>::I>;
};

template <typename aRatio, typename aEpsilon, intmax_t N=1, typename Enabled = void>
struct ratio_sqrt_impl : ratio_sqrt_impl<aRatio, aEpsilon, N+1>
{
};

template <typename aRatio, typename aEpsilon, intmax_t N>
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
    static_assert(std::ratio_greater_equal<aRatio, Zero>::value, "R can't be negative");
};

template <typename aRatio, typename aEpsilon>
struct ratio_sqrt
<
    aRatio,
    aEpsilon,
    typename std::enable_if
    <
        std::ratio_greater_equal<aRatio, Zero>::value && IsPerfectSquare<aRatio>::value
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
        std::ratio_greater_equal<aRatio, Zero>::value && !IsPerfectSquare<aRatio>::value
    >::type
> : ratio_sqrt_impl<aRatio, aEpsilon>
{
};

template< typename CharT >
inline
std::basic_string<CharT>
basic_string_from_exponent
(
    std::intmax_t aExponent
)
{
    std::basic_string<CharT> theResult;
    std::basic_string<CharT> theSign;

    if( aExponent < 0 )
    {
        theSign = superscript_minus<CharT>;
        aExponent = -aExponent;
    }

    do
    {
        theResult = superscript_digit<CharT>[aExponent % 10] + theResult;
        aExponent /= 10;
    }
    while( aExponent > 0 );

    theResult = theSign + theResult;

    return theResult;
}

struct scientific
{
    constexpr
    scientific
    (
        std::intmax_t aValue
    )
    : mantissa{aValue}, exponent{0}
    {
        if( mantissa > 0 )
        {
            while( (mantissa % 10) == 0 )
            {
                mantissa /= 10;
                ++exponent;
            }
        }
    }

    std::intmax_t mantissa;
    std::intmax_t exponent;
};

template< typename CharT >
struct basic_string_from_impl<CharT, scientific>
{
    std::basic_string<CharT>
    operator()
    (
        scientific aScientific
    )
    {
        std::basic_string<CharT> theResult = basic_string_from<CharT>(aScientific.mantissa);

        if( aScientific.exponent )
        {
            theResult += multiply_operator<CharT> + basic_string_from<CharT>(10) + basic_string_from_exponent<CharT>(aScientific.exponent);
        }

        return theResult;
    }
};

template
<
    typename CharT,
    std::intmax_t Num,
    std::intmax_t Den
>
struct basic_string_from_impl<CharT, std::ratio<Num,Den>>
{
    std::basic_string<CharT>
    operator()
    (
        std::ratio<Num,Den> aRatio
    )
    {
        std::basic_string<CharT> theResult;

        if( aRatio.num != aRatio.den )
        {
            constexpr auto theNum = scientific{aRatio.num};
            auto theDen = scientific{aRatio.den};
            theDen.exponent = -theDen.exponent;

            if( theNum.mantissa != theDen.mantissa )
            {
                theResult = basic_string_from<CharT>(theNum.mantissa);
            }

            if( theDen.mantissa != 1 )
            {
                theResult += divide_operator<CharT> + basic_string_from<CharT>(theDen.mantissa);
            }

            if( theNum.exponent != 0 )
            {
                if( !theResult.empty() )
                {
                    theResult += multiply_operator<CharT>;
                }

                theResult += basic_string_from<CharT>(10) + basic_string_from_exponent<CharT>(theNum.exponent);
            }

            if( theDen.exponent != 0 )
            {
                if( !theResult.empty() )
                {
                    theResult += multiply_operator<CharT>;
                }

                theResult += basic_string_from<CharT>(10) + basic_string_from_exponent<CharT>(theDen.exponent);
            }
        }

        return theResult;
    }
};

} // end of namespace si
