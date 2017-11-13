#include <iostream>
#include <type_traits>
#include "string/string-units.hpp"
#include "string-test.hpp"

namespace
{

#define assert( exp ) assertf( exp, __FILE__, __LINE__ )

constexpr inline void assertf( bool aInvariant, const char* aFilename, int aLineNumber )
{
    if( !aInvariant )
    {
        std::cout << "assert failed at " << aFilename << ":" << aLineNumber << "\n";
    }
}

#define m_assert_units_string( aUnitsT, str ) \
{ \
    { \
        constexpr auto aString = str; \
        using CharT = typename std::remove_cv<std::remove_pointer_t<decltype(aString)>>::type; \
        auto s = si::string::string_from_units<CharT,aUnitsT>(); \
        assert(s == aString); \
    } \
    { \
        constexpr auto aString = L##str; \
        using CharT = typename std::remove_cv<std::remove_pointer_t<decltype(aString)>>::type; \
        auto s = si::string::string_from_units<CharT,aUnitsT>(); \
        assert(s == aString); \
    } \
}

} // end of anonymous namespace

void si::run_string_tests()
{
    m_assert_units_string( scalar, "");
    m_assert_units_string( mass, "kg");
    m_assert_units_string( length, "m");
    m_assert_units_string( time, "s");
    m_assert_units_string( current, "A");
    m_assert_units_string( temperature, "K");
    m_assert_units_string( luminance, "cd");
    m_assert_units_string( substance, "mol");
    m_assert_units_string( angle, "rad");
    m_assert_units_string( reciprocal_units<mass>, "1/kg");
    {
        using TestUnits_t = power_units<mass,2>;
        m_assert_units_string( TestUnits_t, "kg\u00B2");
    }
    {
        using TestUnits_t = reciprocal_units<power_units<mass,2>>;
        m_assert_units_string( TestUnits_t, "1/kg\u00B2");
    }
    {
        using TestUnits_t = divide_units<power_units<mass, 2>,power_units<time,4>>;
        m_assert_units_string( TestUnits_t, "kg\u00B2/s\u2074");
    }
}
