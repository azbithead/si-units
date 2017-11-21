#pragma once
#include <string>
#include <cstdint>
#include <ratio>
#include "string/string-quantity.hpp"

namespace si
{

template< typename CharT >
inline
std::basic_string<CharT>
basic_string_from_exponent
(
    std::intmax_t aExponent
)
{
    std::basic_string<CharT> theResult;

    if( aExponent > 1 )
    {
        do
        {
            theResult = superscript_digit<CharT>[aExponent % 10] + theResult;
            aExponent /= 10;
        }
        while( aExponent > 0 );
    }

    return theResult;
}

template<typename CharT>
std::basic_string<CharT>
basic_string_from
(
    std::intmax_t aValue
);

template<>
inline
std::basic_string<char>
basic_string_from<char>
(
    std::intmax_t aValue
)
{
    return std::to_string(aValue);
}

template<>
inline
std::basic_string<wchar_t>
basic_string_from<wchar_t>
(
    std::intmax_t aValue
)
{
    return std::to_wstring(aValue);
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
    auto theResult = basic_string_from<CharT>(aRatio.num);
    if( aRatio.den != 1 )
    {
        theResult += divide_operator<CharT> + basic_string_from<CharT>(aRatio.den);
    }

    return theResult;
}

template
<
    std::intmax_t Num,
    std::intmax_t Den
>
inline
std::string
string_from
(
    std::ratio<Num,Den> aRatio
)
{
    return basic_string_from<char>(aRatio);
}

template
<
    std::intmax_t Num,
    std::intmax_t Den
>
inline
std::wstring
wstring_from
(
    std::ratio<Num,Den> aRatio
)
{
    return basic_string_from<wchar_t>(aRatio);
}

} // end of namespace si
