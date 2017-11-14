#include <iostream>
#include <type_traits>
#include "string/string-quantity.hpp"
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

#define assert_units_abbrev( aUnitsT, aAbbrev ) \
{ \
    { \
        assert(si::string::string_from_units<aUnitsT>() == aAbbrev<char>); \
    } \
    { \
        assert(si::string::wstring_from_units<aUnitsT>() == aAbbrev<wchar_t>); \
    } \
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

#define assert_ratio_string( aRatio, str ) \
{ \
    { \
        assert(si::string::string_from_ratio<aRatio>() == str); \
    } \
    { \
        assert(si::string::wstring_from_ratio<aRatio>() == L##str); \
    } \
}

#define assert_ratio_quantity_suffix_string( aSuffix, str ) \
{ \
    { \
        assert(si::string::string_from_quantity_suffix<aSuffix>() == str); \
    } \
    { \
        assert(si::string::wstring_from_quantity_suffix<aSuffix>() == L##str); \
    } \
}

} // end of anonymous namespace

void si::run_string_tests()
{
    assert_units_abbrev( mass, string::mass_abbrev );
    assert_units_abbrev( length, string::length_abbrev );
    assert_units_abbrev( time, string::time_abbrev );
    assert_units_abbrev( current, string::current_abbrev );
    assert_units_abbrev( temperature, string::temperature_abbrev );
    assert_units_abbrev( luminance, string::luminance_abbrev );
    assert_units_abbrev( substance, string::substance_abbrev );
    assert_units_abbrev( angle, string::angle_abbrev );

    assert_units_string( scalar, "");
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

    assert_ratio_string(std::ratio<1>, "1");
    assert_ratio_string(std::kilo, "1000");
    assert_ratio_string(std::micro, "1/1000000");
    {
        using TestRatio_t = std::ratio<5,18>;
        assert_ratio_string(TestRatio_t, "5/18");
    }

    {
        using TestQuantity_t = quantity<int,std::ratio<1>,scalar>;
        assert_ratio_quantity_suffix_string(TestQuantity_t, "");
    }
    {
        using TestQuantity_t = quantity<int,std::milli,scalar>;
        assert_ratio_quantity_suffix_string(TestQuantity_t, "1/1000");
    }
    {
        using TestQuantity_t = quantity<int,std::ratio<1>,mass>;
        assert_ratio_quantity_suffix_string(TestQuantity_t, "kg");
    }
    {
        using TestQuantity_t = quantity<int,std::milli,luminance>;
        assert_ratio_quantity_suffix_string(TestQuantity_t, "1/1000·cd");
    }

    {
        auto theTestValue = meters<std::kilo>{5.0} / seconds<std::ratio<3600>>{2.0};
        assert(string::string_from_quantity(theTestValue) == "2.500000·5/18·m/s");
    }
    {
        auto theTestValue = meters<std::kilo>{5.0} / seconds<std::ratio<3600>>{2.0};
        assert(string::wstring_from_quantity(theTestValue) == L"2.500000·5/18·m/s");
    }
}
