#pragma once
#include <string>
#include "units.hpp"
#include "string/string-ratio.hpp"
#include "string/string-quantity.hpp"

namespace si
{

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
    units_t<ValueT, RatioT, QuantityT> aUnits
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
            theResult += space<CharT>;
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
    units_t<ValueT, RatioT, QuantityT> aUnits
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
    units_t<ValueT, RatioT, QuantityT> aUnits
)
{
    return basic_string_from<wchar_t>(aUnits);
}

} // end of namespace si

