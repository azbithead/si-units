#pragma once

#define STRING_CONST(aName, aString) \
template< typename CharT > constexpr const CharT* aName = nullptr; \
template<> inline constexpr const char* aName<char> = aString; \
template<> inline constexpr const wchar_t* aName<wchar_t> = L##aString; \
template<> inline constexpr const char16_t* aName<char16_t> = u##aString; \
template<> inline constexpr const char32_t* aName<char32_t> = U##aString

namespace si
{

STRING_CONST(divide_operator, "/");
STRING_CONST(multiply_operator, "·");

} // end of namespace si
