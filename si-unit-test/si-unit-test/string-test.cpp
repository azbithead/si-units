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

#define assert_quantity_abbrev( aQuantityT, aAbbrev ) \
{ \
    { \
        assert(si::string::string_from(aQuantityT{}) == aAbbrev<char>); \
    } \
    { \
        assert(si::string::wstring_from(aQuantityT{}) == aAbbrev<wchar_t>); \
    } \
}

#define assert_quantity_string( aQuantityT, str ) \
{ \
    { \
        assert(si::string::string_from(aQuantityT{}) == str); \
    } \
    { \
        assert(si::string::wstring_from(aQuantityT{}) == L##str); \
    } \
}

#define assert_ratio_string( aRatio, str ) \
{ \
    { \
        assert(si::string::string_from(aRatio{}) == str); \
    } \
    { \
        assert(si::string::wstring_from(aRatio{}) == L##str); \
    } \
}

#define assert_units_suffix_string( aUnits, str ) \
{ \
    { \
        assert(si::string::string_from(aUnits{}) == str); \
    } \
    { \
        assert(si::string::wstring_from(aUnits{}) == L##str); \
    } \
}

} // end of anonymous namespace

void si::run_string_tests()
{
    assert_quantity_abbrev( mass, string::mass_abbrev );
    assert_quantity_abbrev( length, string::length_abbrev );
    assert_quantity_abbrev( time, string::time_abbrev );
    assert_quantity_abbrev( current, string::current_abbrev );
    assert_quantity_abbrev( temperature, string::temperature_abbrev );
    assert_quantity_abbrev( luminous_intensity, string::luminous_intensity_abbrev );
    assert_quantity_abbrev( substance, string::substance_abbrev );
    assert_quantity_abbrev( angle, string::angle_abbrev );

    assert_quantity_string( scalar, "");
    assert_quantity_string( reciprocal_quantity<mass>, "1/kg");
    {
        using TestQuantity_t = power_quantity<mass,2>;
        assert_quantity_string( TestQuantity_t, "kg\u00B2");
    }
    {
        using TestQuantity_t = reciprocal_quantity<power_quantity<mass,2>>;
        assert_quantity_string( TestQuantity_t, "1/kg\u00B2");
    }
    {
        using TestQuantity_t = divide_quantity<power_quantity<mass, 2>,power_quantity<time,4>>;
        assert_quantity_string( TestQuantity_t, "kg\u00B2/s\u2074");
    }

    assert_ratio_string(std::ratio<1>, "1");
    assert_ratio_string(std::kilo, "1000");
    assert_ratio_string(std::micro, "1/1000000");
    {
        using TestRatio_t = std::ratio<5,18>;
        assert_ratio_string(TestRatio_t, "5/18");
    }

    {
        using theSuffixT = typename units_t<int,std::ratio<1>,scalar>::suffix_t;
        assert_units_suffix_string(theSuffixT, "");
    }
    {
        using theSuffixT = typename units_t<int,std::milli,scalar>::suffix_t;
        assert_units_suffix_string(theSuffixT, "1/1000");
    }
    {
        using theSuffixT = typename units_t<int,std::ratio<1>,mass>::suffix_t;
        assert_units_suffix_string(theSuffixT, "kg");
    }
    {
        using theSuffixT = typename units_t<int,std::milli,luminous_intensity>::suffix_t;
        assert_units_suffix_string(theSuffixT, "1/1000·cd");
    }
}
