#pragma once
#include <cstdint>
#include <ratio>
#include <string>

namespace si
{

template<std::intmax_t Exp = 0>
struct exponent_t
{
    static constexpr auto value = Exp;
};

template< typename... >
struct multiply_exponent_impl;

template< typename First, typename... Rest >
struct multiply_exponent_impl< First, Rest... >
{
    using previous = multiply_exponent_impl<Rest...>;

    using type = exponent_t<First::value + previous::type::value>;
};

template<>
struct multiply_exponent_impl<>
{
    using type = exponent_t<>;
};

//------------------------------------------------------------------------------
/// the exponent_t type that is the product of a list of exponent_t's
template< typename... Exponents >
using multiply_exponent = typename multiply_exponent_impl< Exponents... >::type;

template< typename Exponent, typename aPower >
struct exponentiate_exponent_impl
{
    using type = exponent_t<std::ratio_multiply<std::ratio<Exponent::value>, aPower>::num>;
};

//------------------------------------------------------------------------------
/// the exponent_t type that is aExponent raised to aPower
/// @tparam aExponent a exponent_t type
/// @tparam aPower the power to which aExponent is raised
template< typename aExponent, std::intmax_t aPower >
using power_exponent = typename exponentiate_exponent_impl<aExponent, std::ratio<aPower>>::type;

//------------------------------------------------------------------------------
/// the exponent_t type that is the reciprocal of aExponent
/// @tparam aExponent a exponent_t type
template< typename aExponent >
using reciprocal_exponent = typename exponentiate_exponent_impl<aExponent, std::ratio<-1>>::type;

//------------------------------------------------------------------------------
/// the exponent_t type that is the quotient of dividing aDividend by aDivisor
/// @tparam aDividend a exponent_t type
/// @tparam aDivisor a exponent_t type
template< typename aDividend, typename aDivisor >
using divide_exponent = multiply_exponent< aDividend, reciprocal_exponent< aDivisor > >;

//------------------------------------------------------------------------------
/// the exponent_t type that is the root aRoot of aExponent
/// @tparam aExponent a exponent_t type
/// @tparam aRoot the root to be gotten
template< typename aExponent, std::intmax_t aRoot >
using root_exponent = typename exponentiate_exponent_impl<aExponent, std::ratio<1,aRoot>>::type;

#define STRING_CONST(aName, aString) \
template< typename CharT > constexpr const CharT* aName = nullptr; \
template<> constexpr const char* aName<char> = aString; \
template<> constexpr const wchar_t* aName<wchar_t> = L##aString; \
template<> constexpr const char16_t* aName<char16_t> = u##aString; \
template<> constexpr const char32_t* aName<char32_t> = U##aString

STRING_CONST(superscript_minus, "\u207B");

constexpr auto digit_count = 10;

template< typename CharT > constexpr const CharT* superscript_digit[digit_count];
template<> constexpr const char* superscript_digit<char>[digit_count] =
{
    "\u2070",
    "\u00B9",
    "\u00B2",
    "\u00B3",
    "\u2074",
    "\u2075",
    "\u2076",
    "\u2077",
    "\u2078",
    "\u2079"
};
template<> constexpr const wchar_t* superscript_digit<wchar_t>[digit_count] =
{
    L"\u2070",
    L"\u00B9",
    L"\u00B2",
    L"\u00B3",
    L"\u2074",
    L"\u2075",
    L"\u2076",
    L"\u2077",
    L"\u2078",
    L"\u2079"
};
template<> constexpr const char16_t* superscript_digit<char16_t>[digit_count] =
{
    u"\u2070",
    u"\u00B9",
    u"\u00B2",
    u"\u00B3",
    u"\u2074",
    u"\u2075",
    u"\u2076",
    u"\u2077",
    u"\u2078",
    u"\u2079"
};
template<> constexpr const char32_t* superscript_digit<char32_t>[digit_count] =
{
    U"\u2070",
    U"\u00B9",
    U"\u00B2",
    U"\u00B3",
    U"\u2074",
    U"\u2075",
    U"\u2076",
    U"\u2077",
    U"\u2078",
    U"\u2079"
};

template<typename CharT, std::intmax_t Exp>
inline
std::basic_string<CharT>
basic_string_from
(
    exponent_t<Exp> aExponent
)
{
    auto theExponent = aExponent.value;

    std::basic_string<CharT> theSign;

    if( theExponent < 0 )
    {
        theSign = superscript_minus<CharT>;
        theExponent = -theExponent;
    }

    std::basic_string<CharT> theResult;

    do
    {
        theResult = superscript_digit<CharT>[theExponent % 10] + theResult;
        theExponent /= 10;
    }
    while( theExponent > 0 );

    theResult = theSign + theResult;

    return theResult;
}

} // end of namespace si

#include "string-from.hpp"
