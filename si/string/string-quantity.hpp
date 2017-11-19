#pragma once
#include <string>
#include <utility>
#include "string/constants.hpp"
#include "quantity.hpp"

namespace si
{
namespace string
{

template< typename CharT >
inline
std::basic_string<CharT>
basic_string_from_exp
(
    const CharT* const aAbbreviation,
    int aExp,
    std::basic_string<CharT> aString = std::basic_string<CharT>{}
)
{
    if( aExp > 0 )
    {
        if( !aString.empty() )
        {
            aString += dot<CharT>;
        }

        aString += aAbbreviation;

        if( aExp > 1 )
        {
            aString += superscript<CharT>[aExp-2];
        }
    }

    return aString;
}

template
<
    typename CharT,
    std::intmax_t M,
    std::intmax_t L,
    std::intmax_t T,
    std::intmax_t C,
    std::intmax_t Temp,
    std::intmax_t Lum,
    std::intmax_t S,
    std::intmax_t A
>
inline
std::basic_string<CharT>
basic_string_from
(
    quantity_t<M,L,T,C,Temp,Lum,S,A> aQuantity
)
{
    auto theNum = basic_string_from_exp( mass_abbrev<CharT>, aQuantity.mass_exp );
    theNum = basic_string_from_exp( length_abbrev<CharT>, aQuantity.length_exp, std::move(theNum) );
    theNum = basic_string_from_exp( time_abbrev<CharT>, aQuantity.time_exp, std::move(theNum) );
    theNum = basic_string_from_exp( current_abbrev<CharT>, aQuantity.current_exp, std::move(theNum) );
    theNum = basic_string_from_exp( temperature_abbrev<CharT>, aQuantity.temperature_exp, std::move(theNum) );
    theNum = basic_string_from_exp( luminous_intensity_abbrev<CharT>, aQuantity.luminous_intensity_exp, std::move(theNum) );
    theNum = basic_string_from_exp( substance_abbrev<CharT>, aQuantity.substance_exp, std::move(theNum) );
    theNum = basic_string_from_exp( angle_abbrev<CharT>, aQuantity.angle_exp, std::move(theNum) );

    auto theDen = basic_string_from_exp( mass_abbrev<CharT>, -aQuantity.mass_exp );
    theDen = basic_string_from_exp( length_abbrev<CharT>, -aQuantity.length_exp, std::move(theDen) );
    theDen = basic_string_from_exp( time_abbrev<CharT>, -aQuantity.time_exp, std::move(theDen) );
    theDen = basic_string_from_exp( current_abbrev<CharT>, -aQuantity.current_exp, std::move(theDen) );
    theDen = basic_string_from_exp( temperature_abbrev<CharT>, -aQuantity.temperature_exp, std::move(theDen) );
    theDen = basic_string_from_exp( luminous_intensity_abbrev<CharT>, -aQuantity.luminous_intensity_exp, std::move(theDen) );
    theNum = basic_string_from_exp( substance_abbrev<CharT>, -aQuantity.substance_exp, std::move(theNum) );
    theDen = basic_string_from_exp( angle_abbrev<CharT>, -aQuantity.angle_exp, std::move(theDen) );

    if( theDen.empty() )
    {
        return theNum;
    }
    else
    {
        if( theNum.empty() )
        {
            theNum = one<CharT>;
        }

        return theNum + forward_slash<CharT> + theDen;
    }

    return theNum;
}

template
<
    std::intmax_t M,
    std::intmax_t L,
    std::intmax_t T,
    std::intmax_t C,
    std::intmax_t Temp,
    std::intmax_t Lum,
    std::intmax_t S,
    std::intmax_t A
>
inline
std::string
string_from
(
    quantity_t<M,L,T,C,Temp,Lum,S,A> aQuantity
)
{
    return basic_string_from<char>(aQuantity);
}

template
<
    std::intmax_t M,
    std::intmax_t L,
    std::intmax_t T,
    std::intmax_t C,
    std::intmax_t Temp,
    std::intmax_t Lum,
    std::intmax_t S,
    std::intmax_t A
>
inline
std::wstring
wstring_from
(
    quantity_t<M,L,T,C,Temp,Lum,S,A> aQuantity
)
{
    return basic_string_from<wchar_t>(aQuantity);
}

} // end of namespace string
} // end of namespace si
