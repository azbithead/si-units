#pragma once
#include <string>

namespace si
{

template< typename T >
inline
std::string
string_from
(
    const T& aT
)
{
    return basic_string_from<char>(aT);
}

template< typename T >
inline
std::wstring
wstring_from
(
    const T& aT
)
{
    return basic_string_from<wchar_t>(aT);
}

} // end of namespace si
