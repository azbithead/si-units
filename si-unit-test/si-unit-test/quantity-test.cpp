#include <iostream>
#include "helpers.hpp"
#include "quantity.hpp"
#include "quantity-test.hpp"

// compile-time unit tests
namespace
{

using namespace si;

// is_quantity
static_assert( !is_quantity< int >, "" );
static_assert( is_quantity< none >, "" );
static_assert( is_quantity< mass >, "" );
static_assert( is_quantity< distance >, "" );
static_assert( is_quantity< si::time >, "" );
static_assert( is_quantity< current >, "" );
static_assert( is_quantity< const temperature >, "" );
static_assert( is_quantity< volatile luminous_intensity >, "" );
static_assert( is_quantity< const volatile angle >, "" );

// multiply_quantity
static_assert( std::is_same<quantity_t<4,6>, multiply_quantity<quantity_t<1,2>,quantity_t<3,4>>>::value, "" );

// power_quantity
static_assert( std::is_same<quantity_t<3,-6>, power_quantity<quantity_t<1,-2>,3>>::value, "" );

// reciprocal_quantity
static_assert( std::is_same<quantity_t<-1>, reciprocal_quantity<mass>>::value, "" );

// divide_quantity
static_assert( std::is_same<quantity_t<1,-1>, divide_quantity<mass,distance>>::value, "" );

// root_quantity
static_assert( std::is_same<quantity_t<2,-1>, root_quantity<quantity_t<4,-2>,2>>::value, "" );

} // end of anonymous namespace

#define assert_abbrev( aT ) \
{ \
    { \
        si::test::assertf(si::string_from(aT{}) == si::abbrev<char,aT>, __FILE__, __LINE__); \
    } \
    { \
        si::test::assertf(si::wstring_from(aT{}) == si::abbrev<wchar_t,aT>, __FILE__, __LINE__); \
    } \
}

void si::run_quantity_tests()
{
    using namespace si;

    assert_abbrev( mass );
    assert_abbrev( length );
    assert_abbrev( time );
    assert_abbrev( current );
    assert_abbrev( temperature );
    assert_abbrev( luminous_intensity );
    assert_abbrev( substance );
    assert_abbrev( angle );
    assert_abbrev( force );

    assert_literal( none, "");
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
}
