#pragma once

#define STRING_CONST(aName, aString) \
template< typename CharT > constexpr const CharT* aName = nullptr; \
template<> constexpr const char* aName<char> = aString; \
template<> constexpr const wchar_t* aName<wchar_t> = L##aString; \
template<> constexpr const char16_t* aName<char16_t> = u##aString; \
template<> constexpr const char32_t* aName<char32_t> = U##aString

namespace si
{

STRING_CONST(one, "1");
STRING_CONST(space, " ");
STRING_CONST(divide_operator, "/");
STRING_CONST(multiply_operator, "·");
STRING_CONST(mass_abbrev, "kg");
STRING_CONST(length_abbrev, "m");
STRING_CONST(time_abbrev, "s");
STRING_CONST(current_abbrev, "A");
STRING_CONST(temperature_abbrev, "K");
STRING_CONST(luminous_intensity_abbrev, "cd");
STRING_CONST(substance_abbrev, "mol");
STRING_CONST(angle_abbrev, "rad");

constexpr auto digit_count = 10;

template< typename CharT > constexpr const CharT* superscript_digit[digit_count];
template<> constexpr const char* superscript_digit<char>[digit_count] =
{
    "\u2070",
    "\u00B9",
    "\u00B2",
    "\u00B3",
    "\u2074",
    "\u2075",
    "\u2076",
    "\u2077",
    "\u2078",
    "\u2079"
};
template<> constexpr const wchar_t* superscript_digit<wchar_t>[digit_count] =
{
    L"\u2070",
    L"\u00B9",
    L"\u00B2",
    L"\u00B3",
    L"\u2074",
    L"\u2075",
    L"\u2076",
    L"\u2077",
    L"\u2078",
    L"\u2079"
};
template<> constexpr const char16_t* superscript_digit<char16_t>[digit_count] =
{
    u"\u2070",
    u"\u00B9",
    u"\u00B2",
    u"\u00B3",
    u"\u2074",
    u"\u2075",
    u"\u2076",
    u"\u2077",
    u"\u2078",
    u"\u2079"
};
template<> constexpr const char32_t* superscript_digit<char32_t>[digit_count] =
{
    U"\u2070",
    U"\u00B9",
    U"\u00B2",
    U"\u00B3",
    U"\u2074",
    U"\u2075",
    U"\u2076",
    U"\u2077",
    U"\u2078",
    U"\u2079"
};

} // end of namespace si
