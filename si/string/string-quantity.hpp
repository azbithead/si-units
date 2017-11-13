#pragma once
#include <string>
#include <cstdint>
#include <ratio>
#include "quantity.hpp"
#include "string-units.hpp"

namespace si
{
namespace string
{

template<typename CharT>
std::basic_string<CharT>
basic_string_from_intmax
(
    std::intmax_t aValue
);

template<>
std::basic_string<char>
basic_string_from_intmax<char>
(
    std::intmax_t aValue
)
{
    return std::to_string(aValue);
}

template<>
std::basic_string<wchar_t>
basic_string_from_intmax<wchar_t>
(
    std::intmax_t aValue
)
{
    return std::to_wstring(aValue);
}

template
<
    typename CharT,
    typename RatioT
>
std::basic_string<CharT>
basic_string_from_ratio
(
)
{
    auto theResult = basic_string_from_intmax<CharT>(RatioT::num);
    if( RatioT::den != 1 )
    {
        theResult += forward_slash<CharT> + basic_string_from_intmax<CharT>(RatioT::den);
    }

    return theResult;
}

template
<
    typename RatioT
>
std::string
string_from_ratio
(
)
{
    return basic_string_from_ratio<char, RatioT>();
}

template
<
    typename RatioT
>
std::wstring
wstring_from_ratio
(
)
{
    return basic_string_from_ratio<wchar_t, RatioT>();
}

template
<
    typename CharT,
    typename Storage,
    typename Ratio,
    typename Units
>
std::basic_string<CharT>
basic_string_from_quantity_suffix
(
    const si::quantity<Storage, Ratio, Units>& aQuantity
)
{
    auto theResult = basic_string_from_ratio<CharT,Ratio>();
    if( theResult == one<CharT> )
    {
        theResult.clear();
    }

    const auto theUnitsString = basic_string_from_units<CharT,Units>();
    if( !theUnitsString.empty() )
    {
        if( !theResult.empty() )
        {
            theResult += dot<CharT>;
        }

        theResult += theUnitsString;
    }

    return theResult;
}

template
<
    typename Storage,
    typename Ratio,
    typename Units
>
std::string
string_from_quantity_suffix
(
    const si::quantity<Storage, Ratio, Units>& aQuantity
)
{
    return basic_string_from_quantity_suffix<char>(aQuantity);
}

template
<
    typename Storage,
    typename Ratio,
    typename Units
>
std::wstring
wstring_from_quantity_suffix
(
    const si::quantity<Storage, Ratio, Units>& aQuantity
)
{
    return basic_string_from_quantity_suffix<wchar_t>(aQuantity);
}

} // end of namespace string
} // end of namespace si

