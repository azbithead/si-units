#pragma once
#include <string>
#include "units.hpp"

namespace
{

template< typename CharT >
constexpr CharT power_char = 0;

template<>
constexpr char power_char<char> = '^';

template<>
constexpr wchar_t power_char<wchar_t> = L'^';

} // end of anonymous namespace

namespace si
{
namespace string
{

template< typename CharT > constexpr CharT mass_abbrev[3] = {0,0,0};
template<> constexpr char mass_abbrev<char>[3] = "kg";
template<> constexpr wchar_t mass_abbrev<wchar_t>[3] = L"kg";

template< typename CharT > constexpr CharT distance_abbrev[2] = {0,0};
template<> constexpr char distance_abbrev<char>[2] = "m";
template<> constexpr wchar_t distance_abbrev<wchar_t>[2] = L"m";

template< typename CharT > constexpr CharT time_abbrev[2] = {0,0};
template<> constexpr char time_abbrev<char>[2] = "s";
template<> constexpr wchar_t time_abbrev<wchar_t>[2] = L"s";

template< typename CharT > constexpr CharT current_abbrev[2] = {0,0};
template<> constexpr char current_abbrev<char>[2] = "A";
template<> constexpr wchar_t current_abbrev<wchar_t>[2] = L"A";

template< typename CharT > constexpr CharT temperature_abbrev[2] = {0,0};
template<> constexpr char temperature_abbrev<char>[2] = "k";
template<> constexpr wchar_t temperature_abbrev<wchar_t>[2] = L"k";

template< typename CharT > constexpr CharT light_abbrev[3] = {0,0,0};
template<> constexpr char light_abbrev<char>[3] = "cd";
template<> constexpr wchar_t light_abbrev<wchar_t>[3] = L"cd";

template< typename CharT > constexpr CharT angle_abbrev[4] = {0,0,0,0};
template<> constexpr char angle_abbrev<char>[4] = "rad";
template<> constexpr wchar_t angle_abbrev<wchar_t>[4] = L"rad";

template< typename CharT, typename ValueT >
std::basic_string<CharT>
string_from_scalar
(
    ValueT aValue
);

template<>
std::basic_string<char>
string_from_scalar<char, int>
(
    int aValue
)
{
    return std::to_string(aValue);
}

template<>
std::basic_string<wchar_t>
string_from_scalar<wchar_t, int>
(
    int aValue
)
{
    return std::to_wstring(aValue);
}

template<>
std::basic_string<char>
string_from_scalar<char, std::intmax_t>
(
    std::intmax_t aValue
)
{
    return std::to_string(aValue);
}

template<>
std::basic_string<wchar_t>
string_from_scalar<wchar_t, std::intmax_t>
(
    std::intmax_t aValue
)
{
    return std::to_wstring(aValue);
}

template<>
std::basic_string<char>
string_from_scalar<char, double>
(
    double aValue
)
{
    return std::to_string(aValue);
}

template<>
std::basic_string<wchar_t>
string_from_scalar<wchar_t, double>
(
    double aValue
)
{
    return std::to_wstring(aValue);
}

template< typename CharT >
std::basic_string<CharT>
string_from_exp_num
(
    const CharT* const aAbbreviation,
    int aExp
)
{
    std::basic_string<CharT> theResult;
    if( aExp > 0 )
    {
        theResult += aAbbreviation;
        if( aExp > 1 )
        {
            theResult += power_char<CharT> + string_from_scalar<CharT,int>(aExp);
        }
    }

    return theResult;
}

template< typename CharT >
std::basic_string<CharT>
string_from_exp_den
(
    const CharT* const aAbbreviation,
    int aExp
)
{
    std::basic_string<CharT> theResult;
    if( aExp < 0 )
    {
        theResult += aAbbreviation;
        if( aExp < -1 )
        {
            theResult += power_char<CharT> + string_from_scalar<CharT,int>(-aExp);
        }
    }

    return theResult;
}

template
<
    typename CharT,
    typename Units
>
std::basic_string<CharT>
string_from_units
(
)
{
    auto theNum =
    string_from_exp_num( mass_abbrev<CharT>, Units::mass::value ) +
    string_from_exp_num( distance_abbrev<CharT>, Units::distance::value ) +
    string_from_exp_num( time_abbrev<CharT>, Units::time::value ) +
    string_from_exp_num( current_abbrev<CharT>, Units::current::value ) +
    string_from_exp_num( temperature_abbrev<CharT>, Units::temperature::value ) +
    string_from_exp_num( light_abbrev<CharT>, Units::light::value ) +
    string_from_exp_num( angle_abbrev<CharT>, Units::angle::value );
    if( theNum.empty() )
    {
        theNum = "1";
    }
    auto theDen =
    string_from_exp_den( mass_abbrev<CharT>, Units::mass::value ) +
    string_from_exp_den( distance_abbrev<CharT>, Units::distance::value ) +
    string_from_exp_den( time_abbrev<CharT>, Units::time::value ) +
    string_from_exp_den( current_abbrev<CharT>, Units::current::value ) +
    string_from_exp_den( temperature_abbrev<CharT>, Units::temperature::value ) +
    string_from_exp_den( light_abbrev<CharT>, Units::light::value ) +
    string_from_exp_den( angle_abbrev<CharT>, Units::angle::value );
    if( !theDen.empty() )
    {
        return theNum + '/' + theDen;
    }

    return theNum;
}

} // end of namespace string
} // end of namespace si

