#include <type_traits>

namespace si
{

// forward declaration
template< typename TAG, std::intmax_t EXP = 1 >
struct unit_t;

// is_unit_t
template< typename T >
struct is_unit_t : std::false_type {};

template< typename TAG, std::intmax_t EXP >
struct is_unit_t<unit_t<TAG, EXP>> : std::true_type {};

template< typename TAG, std::intmax_t EXP >
struct is_unit_t<const unit_t<TAG, EXP>> : std::true_type {};

template< typename TAG, std::intmax_t EXP >
struct is_unit_t<volatile unit_t<TAG, EXP>> : std::true_type {};

template< typename TAG, std::intmax_t EXP >
struct is_unit_t<const volatile unit_t<TAG, EXP>> : std::true_type {};

template
<
    typename TAG,
    std::intmax_t EXP
>
struct unit_t
{
    using type = TAG;
    static const std::intmax_t exponent = EXP;
};

template< typename U1, typename U2 >
struct same_tag_t : std::false_type {};

template< typename TAG, std::intmax_t EXP1, std::intmax_t EXP2 >
struct same_tag_t
<
    unit_t<TAG, EXP1>,
    unit_t<TAG, EXP2>
> : std::true_type {};

template
<
    typename U1,
    typename U2,
    typename = typename std::enable_if
    <
        is_unit_t<U1>::value &&
        is_unit_t<U2>::value &&
        same_tag_t
        <
            typename std::remove_cv<U1>::type,
            typename std::remove_cv<U2>::type
        >::value,
        void
    >::type
>
using multiply_units_t = unit_t
<
    typename U1::type,
    U1::exponent + U2::exponent
>;

template< typename U >
using reciprocal_unit_t = unit_t< typename U::type, -U::exponent >;

template< typename U1, typename U2 >
using divide_units_t = multiply_units_t< U1, reciprocal_unit_t<U2>>;

template< typename U, std::intmax_t E >
using exponentiate_unit_t = unit_t< typename U::type, E * U::exponent >;

} // end of namespace si
