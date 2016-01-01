#pragma once
#include <ostream>
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
std::basic_ostream<CharT>&
operator <<
(
    std::basic_ostream<CharT>& aStream,
    const std::ratio<Num, Den>& aRatio
)
{
    return aStream << aRatio.num << divide_char<CharT> << aRatio.den;
}

template
<
    typename CharT,
    typename Storage,
    typename Ratio,
    typename Units
>
std::basic_ostream<CharT>&
operator <<
(
    std::basic_ostream<CharT>& aStream,
    const si::quantity<Storage, Ratio, Units>& aQuantity
)
{
    aStream << aQuantity.count();

    if( !(aQuantity.ratio().num == 1 && aQuantity.ratio().den == 1) )
    {
        aStream << multiply_char<CharT> << aQuantity.ratio();
    }

    aStream << aQuantity.units();

    return aStream;
}

} // end of namespace si
