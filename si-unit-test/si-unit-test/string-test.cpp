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

#define assert_units_string( aUnitsT, str ) \
{ \
    { \
        assert(si::string::string_from_units<aUnitsT>() == str); \
    } \
    { \
        assert(si::string::wstring_from_units<aUnitsT>() == L##str); \
    } \
}

} // end of anonymous namespace

void si::run_string_tests()
{
    assert_units_string( scalar, "");
    assert_units_string( mass, "kg");
    assert_units_string( length, "m");
    assert_units_string( time, "s");
    assert_units_string( current, "A");
    assert_units_string( temperature, "K");
    assert_units_string( luminance, "cd");
    assert_units_string( substance, "mol");
    assert_units_string( angle, "rad");
    assert_units_string( reciprocal_units<mass>, "1/kg");
    {
        using TestUnits_t = power_units<mass,2>;
        assert_units_string( TestUnits_t, "kg\u00B2");
    }
    {
        using TestUnits_t = reciprocal_units<power_units<mass,2>>;
        assert_units_string( TestUnits_t, "1/kg\u00B2");
    }
    {
        using TestUnits_t = divide_units<power_units<mass, 2>,power_units<time,4>>;
        assert_units_string( TestUnits_t, "kg\u00B2/s\u2074");
    }
}
