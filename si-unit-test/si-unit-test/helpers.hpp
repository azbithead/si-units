#pragma once
#include <iostream>

namespace si
{
namespace test
{

inline
void
assertf
(
    bool aInvariant,
    const char* aFilename,
    int aLineNumber
)
{
    if( !aInvariant )
    {
        std::cout
            << "assert failed at "
            << aFilename
            << ":"
            << aLineNumber
            << "\n";
    }
}

} // end of namespace test
} // end of namespace si

#define si_assert( exp ) si::test::assertf( exp, __FILE__, __LINE__ )

#define assert_str_eq( str1, str2 ) si_assert( str1 == str2 )

#define assert_literal( aT, str ) \
{ \
    { \
        assert_str_eq(si::basic_string_from<char>(aT{}), str); \
    } \
    { \
        assert_str_eq(si::basic_string_from<wchar_t>(aT{}), L##str); \
    } \
}
