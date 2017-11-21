#pragma once
#include <string>
#include <utility>
#include "string/constants.hpp"
#include "quantity.hpp"

namespace si
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
            aString += multiply_operator<CharT>;
        }

        aString += aAbbreviation;

        if( aExp > 1 )
        {
            std::basic_string<CharT> theSuperscript;

            do
            {
                theSuperscript = superscript_digit<CharT>[aExp % 10] + theSuperscript;
                aExp /= 10;
            }
            while( aExp > 0 );

            aString += theSuperscript;
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
    if( abbrev<CharT, quantity_t<M,L,T,C,Temp,Lum,S,A>> != nullptr )
    {
        return abbrev<CharT, quantity_t<M,L,T,C,Temp,Lum,S,A>>;
    }

    auto theNum = basic_string_from_exp( abbrev<CharT,mass>, aQuantity.mass_exp );
    theNum = basic_string_from_exp( abbrev<CharT,length>, aQuantity.length_exp, std::move(theNum) );
    theNum = basic_string_from_exp( abbrev<CharT,time>, aQuantity.time_exp, std::move(theNum) );
    theNum = basic_string_from_exp( abbrev<CharT,current>, aQuantity.current_exp, std::move(theNum) );
    theNum = basic_string_from_exp( abbrev<CharT,temperature>, aQuantity.temperature_exp, std::move(theNum) );
    theNum = basic_string_from_exp( abbrev<CharT,luminous_intensity>, aQuantity.luminous_intensity_exp, std::move(theNum) );
    theNum = basic_string_from_exp( abbrev<CharT,substance>, aQuantity.substance_exp, std::move(theNum) );
    theNum = basic_string_from_exp( abbrev<CharT,angle>, aQuantity.angle_exp, std::move(theNum) );

    auto theDen = basic_string_from_exp( abbrev<CharT,mass>, -aQuantity.mass_exp );
    theDen = basic_string_from_exp( abbrev<CharT,length>, -aQuantity.length_exp, std::move(theDen) );
    theDen = basic_string_from_exp( abbrev<CharT,time>, -aQuantity.time_exp, std::move(theDen) );
    theDen = basic_string_from_exp( abbrev<CharT,current>, -aQuantity.current_exp, std::move(theDen) );
    theDen = basic_string_from_exp( abbrev<CharT,temperature>, -aQuantity.temperature_exp, std::move(theDen) );
    theDen = basic_string_from_exp( abbrev<CharT,luminous_intensity>, -aQuantity.luminous_intensity_exp, std::move(theDen) );
    theNum = basic_string_from_exp( abbrev<CharT,substance>, -aQuantity.substance_exp, std::move(theNum) );
    theDen = basic_string_from_exp( abbrev<CharT,angle>, -aQuantity.angle_exp, std::move(theDen) );

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

        return theNum + divide_operator<CharT> + theDen;
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

} // end of namespace si
