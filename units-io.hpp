#pragma once
#include <ostream>
#include "units.hpp"

namespace
{

template< typename CharT >
constexpr CharT power_char = 0;

template<>
constexpr char power_char<char> = '^';

template<>
constexpr wchar_t power_char<wchar_t> = L'^';

template< typename CharT >
void
output_exp
(
    std::basic_ostream<CharT>& aStream,
    const CharT* const aAbbreviation,
    int aExp
)
{
    if( aExp != 0 )
    {
        aStream << aAbbreviation;
        if( aExp != 1 )
        {
            aStream << power_char<CharT> << aExp;
        }
    }
}

} // end of anonymous namespace

namespace si
{

template< typename CharT > constexpr CharT mass_abbrev[3] = {0,0,0};
template<> constexpr char mass_abbrev<char>[3] = "kg";
template<> constexpr wchar_t mass_abbrev<wchar_t>[3] = L"kg";

template< typename CharT > constexpr CharT length_abbrev[2] = {0,0};
template<> constexpr char length_abbrev<char>[2] = "m";
template<> constexpr wchar_t length_abbrev<wchar_t>[2] = L"m";

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

template
<
    typename CharT,
    int M,
    int L,
    int T,
    int C,
    int Tp,
    int Li,
    int A
>
std::basic_ostream<CharT>&
operator <<
(
    std::basic_ostream<CharT>& aStream,
    const si::units<M,L,T,C,Tp,Li,A>& aUnits
)
{
    output_exp( aStream, mass_abbrev<CharT>, aUnits.mass_exp() );
    output_exp( aStream, length_abbrev<CharT>, aUnits.length_exp() );
    output_exp( aStream, time_abbrev<CharT>, aUnits.time_exp() );
    output_exp( aStream, current_abbrev<CharT>, aUnits.current_exp() );
    output_exp( aStream, temperature_abbrev<CharT>, aUnits.temperature_exp() );
    output_exp( aStream, light_abbrev<CharT>, aUnits.light_exp() );
    output_exp( aStream, angle_abbrev<CharT>, aUnits.angle_exp() );

    return aStream;
}

} // end of namespace si
