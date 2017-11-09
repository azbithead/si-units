#pragma once
#include <string>
#include "quantity.hpp"
#include "units-io.hpp"

namespace
{

template< typename CharT >
constexpr CharT divide_char = 0;

template<>
constexpr char divide_char<char> = '/';

template<>
constexpr wchar_t divide_char<wchar_t> = L'/';

template< typename CharT >
constexpr CharT multiply_char = 0;

template<>
constexpr char multiply_char<char> = 'x';

template<>
constexpr wchar_t multiply_char<wchar_t> = L'x';

} // end of anonymous namespace

namespace si
{

template
<
    typename CharT,
    std::intmax_t Num,
    std::intmax_t Den
>
std::basic_string<CharT>
string_from_ratio
(
    std::ratio<Num, Den> aRatio
)
{
    return string_from_scalar<CharT,std::intmax_t>(aRatio.num) + divide_char<CharT> + string_from_scalar<CharT,std::intmax_t>(aRatio.den);
}

template
<
    typename CharT,
    typename Storage,
    typename Ratio,
    typename Units
>
std::basic_string<CharT>
string_from_quantity
(
    const si::quantity<Storage, Ratio, Units>& aQuantity
)
{
    auto theResult = string_from_scalar<CharT,Storage>(aQuantity.count());

    if( !(aQuantity.ratio().num == 1 && aQuantity.ratio().den == 1) )
    {
        theResult += multiply_char<CharT> + string_from_ratio<CharT>(aQuantity.ratio());
    }

    theResult += string_from_units<CharT>(aQuantity.units());

    return theResult;
}

} // end of namespace si
