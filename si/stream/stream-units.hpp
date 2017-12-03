#pragma once
#include "string/string-units.hpp"

namespace si
{

template
<
    typename CharT,
    typename ValueT,
    typename IntervalT,
    typename QuantityT
>
std::basic_ostream<CharT>&
operator <<
(
    std::basic_ostream<CharT>& aStream,
    units_t<ValueT, IntervalT, QuantityT> aUnits
)
{
    aStream << aUnits.value();

    if( aUnits.interval.num != aUnits.interval.den )
    {
        aStream << si::multiply_operator<CharT>;
    }

    return aStream << si::basic_string_from<CharT>(aUnits);
}

} // end of namespace si

