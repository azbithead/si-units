#include <iostream>
#include <type_traits>
#include "string/string-units.hpp"
#include "string-test.hpp"

namespace
{

#define assert( str1, str2 ) assertf( str1 == str2, __FILE__, __LINE__ )

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

#define assert_abbrev( aT, aAbbrev ) \
{ \
    { \
        assert(si::string::string_from(aT{}), aAbbrev<char>); \
    } \
    { \
        assert(si::string::wstring_from(aT{}), aAbbrev<wchar_t>); \
    } \
}

#define assert_literal( aT, str ) \
{ \
    { \
        assert(si::string::string_from(aT{}), str); \
    } \
    { \
        assert(si::string::wstring_from(aT{}), L##str); \
    } \
}

} // end of anonymous namespace

void si::run_string_tests()
{
    assert_abbrev( mass, string::mass_abbrev );
    assert_abbrev( length, string::length_abbrev );
    assert_abbrev( time, string::time_abbrev );
    assert_abbrev( current, string::current_abbrev );
    assert_abbrev( temperature, string::temperature_abbrev );
    assert_abbrev( luminous_intensity, string::luminous_intensity_abbrev );
    assert_abbrev( substance, string::substance_abbrev );
    assert_abbrev( angle, string::angle_abbrev );

    assert_literal( scalar, "");
    assert_literal( reciprocal_quantity<mass>, "1/kg");
    {
        using TestQuantity_t = power_quantity<mass,102>;
        assert_literal( TestQuantity_t, "kg\u00B9\u2070\u00B2");
    }
    {
        using TestQuantity_t = reciprocal_quantity<power_quantity<mass,2>>;
        assert_literal( TestQuantity_t, "1/kg\u00B2");
    }
    {
        using TestQuantity_t = divide_quantity<power_quantity<mass, 2>,power_quantity<time,4>>;
        assert_literal( TestQuantity_t, "kg\u00B2/s\u2074");
    }

    assert_literal(std::ratio<1>, "1");
    assert_literal(std::kilo, "1000");
    assert_literal(std::micro, "1/1000000");
    {
        using TestRatio_t = std::ratio<5,18>;
        assert_literal(TestRatio_t, "5/18");
    }

    {
        using theSuffixT = units_t<int,std::ratio<1>,scalar>;
        assert_literal(theSuffixT, "");
    }
    {
        using theSuffixT = units_t<int,std::milli,scalar>;
        assert_literal(theSuffixT, "1/1000");
    }
    {
        using theSuffixT = units_t<int,std::ratio<1>,mass>;
        assert_literal(theSuffixT, "kg");
    }
    {
        using theSuffixT = units_t<int,std::milli,luminous_intensity>;
        assert_literal(theSuffixT, "1/1000·cd");
    }
}
