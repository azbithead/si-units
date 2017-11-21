#pragma once
#include "string/string-units.hpp"

namespace si
{

template
<
    typename CharT,
    typename ValueT,
    typename RatioT,
    typename QuantityT
>
std::basic_ostream<CharT>&
operator <<
(
    std::basic_ostream<CharT>& aStream,
    si::units_t<ValueT, RatioT, QuantityT> aUnits
)
{
    aStream << aUnits.value();

    if( aUnits.ratio.num != aUnits.ratio.den )
    {
        aStream << si::multiply_operator<CharT> << si::basic_string_from<CharT>(aUnits.ratio);
    }

    return aStream << si::space<CharT> << si::basic_string_from<CharT>(aUnits.quantity);
}

} // end of namespace si

