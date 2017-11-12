#pragma once
#include <ratio>

namespace si
{

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

} // end of namespace si

void run();
