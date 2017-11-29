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
    std::basic_string<CharT> theSign;

    if( aExponent < 0 )
    {
        theSign = si::superscript_minus<CharT>;
        aExponent = -aExponent;
    }

    do
    {
        theResult = si::superscript_digit<CharT>[aExponent % 10] + theResult;
        aExponent /= 10;
    }
    while( aExponent > 0 );

    theResult = theSign + theResult;

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
inline
std::basic_string<CharT>
basic_string_from
(
    scientific aScientific
)
{
    std::basic_string<CharT> theResult = si::basic_string_from<CharT>(aScientific.mantissa);

    if( aScientific.exponent )
    {
        theResult += si::multiply_operator<CharT> + si::basic_string_from<CharT>(10) + basic_string_from_exponent<CharT>(aScientific.exponent);
    }

    return theResult;
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
            theResult += si::divide_operator<CharT> + basic_string_from<CharT>(theDen.mantissa);
        }

        if( theNum.exponent != 0 )
        {
            if( !theResult.empty() )
            {
                theResult += si::multiply_operator<CharT>;
            }

            theResult += si::basic_string_from<CharT>(10) + basic_string_from_exponent<CharT>(theNum.exponent);
        }

        if( theDen.exponent != 0 )
        {
            if( !theResult.empty() )
            {
                theResult += si::multiply_operator<CharT>;
            }

            theResult += si::basic_string_from<CharT>(10) + basic_string_from_exponent<CharT>(theDen.exponent);
        }
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
