#pragma once
#include <string>
#include <cstdint>
#include <ratio>
#include "units.hpp"
#include "string/string-quantity.hpp"

namespace si
{
namespace string
{

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
        theResult += forward_slash<CharT> + basic_string_from<CharT>(aRatio.den);
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

template
<
    typename CharT,
    typename RatioT,
    typename QuantityT
>
inline
std::basic_string<CharT>
basic_string_from
(
    const units_suffix_t<RatioT, QuantityT>& aUnitsSuffix
)
{
    auto theResult = basic_string_from<CharT>(aUnitsSuffix.ratio);
    if( theResult == one<CharT> )
    {
        theResult.clear();
    }

    const auto theQuantityString = basic_string_from<CharT>(aUnitsSuffix.quantity);
    if( !theQuantityString.empty() )
    {
        if( !theResult.empty() )
        {
            theResult += dot<CharT>;
        }

        theResult += theQuantityString;
    }

    return theResult;
}

template
<
    typename RatioT,
    typename QuantityT
>
inline
std::string
string_from
(
    const units_suffix_t<RatioT, QuantityT>& aUnitsSuffix
)
{
    return basic_string_from<char>(aUnitsSuffix);
}

template
<
    typename RatioT,
    typename QuantityT
>
inline
std::wstring
wstring_from
(
    const units_suffix_t<RatioT, QuantityT>& aUnitsSuffix
)
{
    return basic_string_from<wchar_t>(aUnitsSuffix);
}

} // end of namespace string
} // end of namespace si

