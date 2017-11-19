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
basic_string_from_intmax
(
    std::intmax_t aValue
);

template<>
inline
std::basic_string<char>
basic_string_from_intmax<char>
(
    std::intmax_t aValue
)
{
    return std::to_string(aValue);
}

template<>
inline
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
inline
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
inline
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
inline
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
    typename UnitsT
>
inline
std::basic_string<CharT>
basic_string_from_units_suffix
(
)
{
    using Units_t = std::decay_t<UnitsT>;

    auto theResult = basic_string_from_ratio<CharT,typename Units_t::ratio_t>();
    if( theResult == one<CharT> )
    {
        theResult.clear();
    }

    const auto theQuantityString = basic_string_from_quantity<CharT,typename Units_t::quantity_t>();
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
    typename UnitsT
>
inline
std::string
string_from_units_suffix
(
)
{
    return basic_string_from_units_suffix<char,UnitsT>();
}

template
<
    typename UnitsT
>
inline
std::wstring
wstring_from_units_suffix
(
)
{
    return basic_string_from_units_suffix<wchar_t,UnitsT>();
}

template
<
    typename StorageT,
    typename RatioT,
    typename QuantityT
>
inline
std::string
string_from_units
(
    const units_t<StorageT, RatioT, QuantityT>& aUnits
)
{
    return std::to_string(aUnits.value()) + dot<char> + string_from_units_suffix<decltype(aUnits)>();
}

template
<
    typename StorageT,
    typename RatioT,
    typename QuantityT
>
inline
std::wstring
wstring_from_units
(
    const units_t<StorageT, RatioT, QuantityT>& aUnits
)
{
    return std::to_wstring(aUnits.value()) + dot<wchar_t> + wstring_from_units_suffix<decltype(aUnits)>();
}

} // end of namespace string
} // end of namespace si

