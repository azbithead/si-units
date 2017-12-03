#pragma once
#include <string>

namespace si
{

template< typename CharT, typename T >
struct basic_string_from_impl
{
    std::basic_string<CharT>
    operator()
    (
        const T& aT
    );
};

template<>
struct basic_string_from_impl<char, int>
{
    std::basic_string<char>
    operator()
    (
        int aInt
    )
    {
        return std::to_string(aInt);
    }
};

template<>
struct basic_string_from_impl<wchar_t, int>
{
    std::basic_string<wchar_t>
    operator()
    (
        int aInt
    )
    {
        return std::to_wstring(aInt);
    }
};

template<>
struct basic_string_from_impl<char, long>
{
    std::basic_string<char>
    operator()
    (
        long aInt
    )
    {
        return std::to_string(aInt);
    }
};

template<>
struct basic_string_from_impl<wchar_t, long>
{
    std::basic_string<wchar_t>
    operator()
    (
        long aInt
    )
    {
        return std::to_wstring(aInt);
    }
};

template< typename CharT, typename T >
inline
std::basic_string<CharT>
basic_string_from
(
    const T& aT
)
{
    return basic_string_from_impl<CharT, T>{}(aT);
}

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
