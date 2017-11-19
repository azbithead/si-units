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
    typename Quantity
>
inline
std::basic_string<CharT>
basic_string_from_quantity
(
)
{
    auto theNum = basic_string_from_exp( mass_abbrev<CharT>, Quantity::mass_exp );
    theNum = basic_string_from_exp( length_abbrev<CharT>, Quantity::length_exp, std::move(theNum) );
    theNum = basic_string_from_exp( time_abbrev<CharT>, Quantity::time_exp, std::move(theNum) );
    theNum = basic_string_from_exp( current_abbrev<CharT>, Quantity::current_exp, std::move(theNum) );
    theNum = basic_string_from_exp( temperature_abbrev<CharT>, Quantity::temperature_exp, std::move(theNum) );
    theNum = basic_string_from_exp( luminous_intensity_abbrev<CharT>, Quantity::luminous_intensity_exp, std::move(theNum) );
    theNum = basic_string_from_exp( substance_abbrev<CharT>, Quantity::substance_exp, std::move(theNum) );
    theNum = basic_string_from_exp( angle_abbrev<CharT>, Quantity::angle_exp, std::move(theNum) );

    auto theDen = basic_string_from_exp( mass_abbrev<CharT>, -Quantity::mass_exp );
    theDen = basic_string_from_exp( length_abbrev<CharT>, -Quantity::length_exp, std::move(theDen) );
    theDen = basic_string_from_exp( time_abbrev<CharT>, -Quantity::time_exp, std::move(theDen) );
    theDen = basic_string_from_exp( current_abbrev<CharT>, -Quantity::current_exp, std::move(theDen) );
    theDen = basic_string_from_exp( temperature_abbrev<CharT>, -Quantity::temperature_exp, std::move(theDen) );
    theDen = basic_string_from_exp( luminous_intensity_abbrev<CharT>, -Quantity::luminous_intensity_exp, std::move(theDen) );
    theNum = basic_string_from_exp( substance_abbrev<CharT>, -Quantity::substance_exp, std::move(theNum) );
    theDen = basic_string_from_exp( angle_abbrev<CharT>, -Quantity::angle_exp, std::move(theDen) );

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
    typename Quantity
>
inline
std::string
string_from_quantity
(
)
{
    return basic_string_from_quantity<char, Quantity>();
}

template
<
    typename Quantity
>
inline
std::wstring
wstring_from_quantity
(
)
{
    return basic_string_from_quantity<wchar_t, Quantity>();
}

} // end of namespace string
} // end of namespace si
