#pragma once
#include <string>
#include <utility>
#include "units.hpp"

namespace
{

template< typename CharT >
constexpr const CharT* one = nullptr;

template<>
constexpr const char* one<char> = "1";

template<>
constexpr const wchar_t* one<wchar_t> = L"1";

template< typename CharT >
constexpr CharT forward_slash = 0;

template<>
constexpr char forward_slash<char> = '/';

template<>
constexpr wchar_t forward_slash<wchar_t> = L'/';

template< typename CharT >
constexpr const CharT* dot = nullptr;

template<>
constexpr const char* dot<char> = "·";

template<>
constexpr const wchar_t* dot<wchar_t> = L"·";

template< typename CharT >
constexpr const CharT* superscript[8];

template<>
constexpr const char* superscript<char>[8] = {"\u00B2","\u00B3","\u2074","\u2075","\u2076","\u2077","\u2078","\u2079"};

template<>
constexpr const wchar_t* superscript<wchar_t>[8] = {L"\u00B2",L"\u00B3",L"\u2074",L"\u2075",L"\u2076",L"\u2077",L"\u2078",L"\u2079"};

} // end of anonymous namespace

namespace si
{
namespace string
{

template< typename CharT > constexpr const CharT* mass_abbrev = nullptr;
template<> constexpr const char* mass_abbrev<char> = "kg";
template<> constexpr const wchar_t* mass_abbrev<wchar_t> = L"kg";

template< typename CharT > constexpr const CharT* length_abbrev = nullptr;
template<> constexpr const char* length_abbrev<char> = "m";
template<> constexpr const wchar_t* length_abbrev<wchar_t> = L"m";

template< typename CharT > constexpr const CharT* time_abbrev = nullptr;
template<> constexpr const char* time_abbrev<char> = "s";
template<> constexpr const wchar_t* time_abbrev<wchar_t> = L"s";

template< typename CharT > constexpr const CharT* current_abbrev = nullptr;
template<> constexpr const char* current_abbrev<char> = "A";
template<> constexpr const wchar_t* current_abbrev<wchar_t> = L"A";

template< typename CharT > constexpr const CharT* temperature_abbrev = nullptr;
template<> constexpr const char* temperature_abbrev<char> = "K";
template<> constexpr const wchar_t* temperature_abbrev<wchar_t> = L"K";

template< typename CharT > constexpr const CharT* luminance_abbrev = nullptr;
template<> constexpr const char* luminance_abbrev<char> = "cd";
template<> constexpr const wchar_t* luminance_abbrev<wchar_t> = L"cd";

template< typename CharT > constexpr const CharT* substance_abbrev = nullptr;
template<> constexpr const char* substance_abbrev<char> = "mol";
template<> constexpr const wchar_t* substance_abbrev<wchar_t> = L"mol";

template< typename CharT > constexpr const CharT* angle_abbrev = nullptr;
template<> constexpr const char* angle_abbrev<char> = "rad";
template<> constexpr const wchar_t* angle_abbrev<wchar_t> = L"rad";

template< typename CharT >
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
std::wstring
wstring_from_units
(
)
{
    return basic_string_from_units<wchar_t, Units>();
}

} // end of namespace string
} // end of namespace si
