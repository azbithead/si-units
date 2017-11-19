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
        assert(si::string::string_from_quantity<aQuantityT>() == aAbbrev<char>); \
    } \
    { \
        assert(si::string::wstring_from_quantity<aQuantityT>() == aAbbrev<wchar_t>); \
    } \
}

#define assert_quantity_string( aUnitsT, str ) \
{ \
    { \
        assert(si::string::string_from_quantity<aUnitsT>() == str); \
    } \
    { \
        assert(si::string::wstring_from_quantity<aUnitsT>() == L##str); \
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
        assert(si::string::string_from_units_suffix<aSuffix>() == str); \
    } \
    { \
        assert(si::string::wstring_from_units_suffix<aSuffix>() == L##str); \
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
        using TestUnits_t = power_quantity<mass,2>;
        assert_quantity_string( TestUnits_t, "kg\u00B2");
    }
    {
        using TestUnits_t = reciprocal_quantity<power_quantity<mass,2>>;
        assert_quantity_string( TestUnits_t, "1/kg\u00B2");
    }
    {
        using TestUnits_t = divide_quantity<power_quantity<mass, 2>,power_quantity<time,4>>;
        assert_quantity_string( TestUnits_t, "kg\u00B2/s\u2074");
    }

    assert_ratio_string(std::ratio<1>, "1");
    assert_ratio_string(std::kilo, "1000");
    assert_ratio_string(std::micro, "1/1000000");
    {
        using TestRatio_t = std::ratio<5,18>;
        assert_ratio_string(TestRatio_t, "5/18");
    }

    {
        using TestQuantity_t = units_t<int,std::ratio<1>,scalar>;
        assert_ratio_quantity_suffix_string(TestQuantity_t, "");
    }
    {
        using TestQuantity_t = units_t<int,std::milli,scalar>;
        assert_ratio_quantity_suffix_string(TestQuantity_t, "1/1000");
    }
    {
        using TestQuantity_t = units_t<int,std::ratio<1>,mass>;
        assert_ratio_quantity_suffix_string(TestQuantity_t, "kg");
    }
    {
        using TestQuantity_t = units_t<int,std::milli,luminous_intensity>;
        assert_ratio_quantity_suffix_string(TestQuantity_t, "1/1000·cd");
    }

    {
        auto theTestValue = meters<std::kilo>{5.0} / seconds<std::ratio<3600>>{2.0};
        assert(string::string_from_units(theTestValue) == std::to_string(2.5) + "·5/18·m/s");
    }
    {
        auto theTestValue = meters<std::kilo>{5.0} / seconds<std::ratio<3600>>{2.0};
        assert(string::wstring_from_units(theTestValue) == std::to_wstring(2.5) + L"·5/18·m/s");
    }
}
