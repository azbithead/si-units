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
    typename IntervalT,
    typename QuantityT
>
inline
std::basic_string<CharT>
basic_string_from
(
    units_t<ValueT, IntervalT, QuantityT> aUnits
)
{
    std::basic_string<CharT> theResult;
    if( aUnits.interval.num != aUnits.interval.den )
    {
        theResult = basic_string_from<CharT>(aUnits.interval);
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
    typename IntervalT,
    typename QuantityT
>
inline
std::string
string_from
(
    units_t<ValueT, IntervalT, QuantityT> aUnits
)
{
    return basic_string_from<char>(aUnits);
}

template
<
    typename ValueT,
    typename IntervalT,
    typename QuantityT
>
inline
std::wstring
wstring_from
(
    units_t<ValueT, IntervalT, QuantityT> aUnits
)
{
    return basic_string_from<wchar_t>(aUnits);
}

} // end of namespace si

