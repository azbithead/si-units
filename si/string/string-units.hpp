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

template
<
    typename CharT,
    typename ValueT,
    typename RatioT,
    typename QuantityT
>
inline
std::basic_string<CharT>
basic_string_from
(
    const units_t<ValueT, RatioT, QuantityT>& aUnits
)
{
    std::basic_string<CharT> theResult;
    if( aUnits.ratio.num != aUnits.ratio.den )
    {
        theResult = basic_string_from<CharT>(aUnits.ratio);
    }

    const auto theQuantityString = basic_string_from<CharT>(aUnits.quantity);
    if( !theQuantityString.empty() )
    {
        if( !theResult.empty() )
        {
            theResult += multiply_operator<CharT>;
        }

        theResult += theQuantityString;
    }

    return theResult;
}

template
<
    typename ValueT,
    typename RatioT,
    typename QuantityT
>
inline
std::string
string_from
(
    const units_t<ValueT, RatioT, QuantityT>& aUnits
)
{
    return basic_string_from<char>(aUnits);
}

template
<
    typename ValueT,
    typename RatioT,
    typename QuantityT
>
inline
std::wstring
wstring_from
(
    const units_t<ValueT, RatioT, QuantityT>& aUnits
)
{
    return basic_string_from<wchar_t>(aUnits);
}

} // end of namespace string
} // end of namespace si

