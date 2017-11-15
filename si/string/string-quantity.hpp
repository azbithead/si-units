#pragma once
#include <string>
#include <cstdint>
#include <ratio>
#include "quantity.hpp"
#include "string/string-units.hpp"

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
typename std::enable_if<ratio::is_ratio<RatioT>, std::basic_string<CharT>>::type
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
    typename QuantityT
>
std::basic_string<CharT>
basic_string_from_quantity_suffix
(
)
{
    using Quantity_t = std::decay_t<QuantityT>;

    auto theResult = basic_string_from_ratio<CharT,typename Quantity_t::ratio_t>();
    if( theResult == one<CharT> )
    {
        theResult.clear();
    }

    const auto theUnitsString = basic_string_from_units<CharT,typename Quantity_t::units_t>();
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
    typename QuantityT
>
std::string
string_from_quantity_suffix
(
)
{
    return basic_string_from_quantity_suffix<char,QuantityT>();
}

template
<
    typename QuantityT
>
std::wstring
wstring_from_quantity_suffix
(
)
{
    return basic_string_from_quantity_suffix<wchar_t,QuantityT>();
}

template
<
    typename StorageT,
    typename RatioT,
    typename UnitsT
>
std::string
string_from_quantity
(
    const quantity<StorageT, RatioT, UnitsT>& aQuantity
)
{
    return std::to_string(aQuantity.value()) + dot<char> + string_from_quantity_suffix<decltype(aQuantity)>();
}

template
<
    typename StorageT,
    typename RatioT,
    typename UnitsT
>
std::wstring
wstring_from_quantity
(
    const quantity<StorageT, RatioT, UnitsT>& aQuantity
)
{
    return std::to_wstring(aQuantity.value()) + dot<wchar_t> + wstring_from_quantity_suffix<decltype(aQuantity)>();
}

} // end of namespace string
} // end of namespace si

