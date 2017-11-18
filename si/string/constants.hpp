#pragma once

namespace si
{
namespace string
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

template< typename CharT > constexpr const CharT* luminous_intensity_abbrev = nullptr;
template<> constexpr const char* luminous_intensity_abbrev<char> = "cd";
template<> constexpr const wchar_t* luminous_intensity_abbrev<wchar_t> = L"cd";

template< typename CharT > constexpr const CharT* substance_abbrev = nullptr;
template<> constexpr const char* substance_abbrev<char> = "mol";
template<> constexpr const wchar_t* substance_abbrev<wchar_t> = L"mol";

template< typename CharT > constexpr const CharT* angle_abbrev = nullptr;
template<> constexpr const char* angle_abbrev<char> = "rad";
template<> constexpr const wchar_t* angle_abbrev<wchar_t> = L"rad";

} // end of namespace string
} // end of namespace si
