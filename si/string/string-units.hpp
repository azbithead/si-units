#pragma once
#include <string>
#include <utility>
#include "string/constants.hpp"
#include "units.hpp"

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
    typename Units
>
inline
std::basic_string<CharT>
basic_string_from_units
(
)
{
    auto theNum = basic_string_from_exp( mass_abbrev<CharT>, Units::mass::value );
    theNum = basic_string_from_exp( length_abbrev<CharT>, Units::length::value, std::move(theNum) );
    theNum = basic_string_from_exp( time_abbrev<CharT>, Units::time::value, std::move(theNum) );
    theNum = basic_string_from_exp( current_abbrev<CharT>, Units::current::value, std::move(theNum) );
    theNum = basic_string_from_exp( temperature_abbrev<CharT>, Units::temperature::value, std::move(theNum) );
    theNum = basic_string_from_exp( luminance_abbrev<CharT>, Units::luminance::value, std::move(theNum) );
    theNum = basic_string_from_exp( substance_abbrev<CharT>, Units::substance::value, std::move(theNum) );
    theNum = basic_string_from_exp( angle_abbrev<CharT>, Units::angle::value, std::move(theNum) );

    auto theDen = basic_string_from_exp( mass_abbrev<CharT>, -Units::mass::value );
    theDen = basic_string_from_exp( length_abbrev<CharT>, -Units::length::value, std::move(theDen) );
    theDen = basic_string_from_exp( time_abbrev<CharT>, -Units::time::value, std::move(theDen) );
    theDen = basic_string_from_exp( current_abbrev<CharT>, -Units::current::value, std::move(theDen) );
    theDen = basic_string_from_exp( temperature_abbrev<CharT>, -Units::temperature::value, std::move(theDen) );
    theDen = basic_string_from_exp( luminance_abbrev<CharT>, -Units::luminance::value, std::move(theDen) );
    theNum = basic_string_from_exp( substance_abbrev<CharT>, -Units::substance::value, std::move(theNum) );
    theDen = basic_string_from_exp( angle_abbrev<CharT>, -Units::angle::value, std::move(theDen) );

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
    typename Units
>
inline
std::string
string_from_units
(
)
{
    return basic_string_from_units<char, Units>();
}

template
<
    typename Units
>
inline
std::wstring
wstring_from_units
(
)
{
    return basic_string_from_units<wchar_t, Units>();
}

} // end of namespace string
} // end of namespace si
