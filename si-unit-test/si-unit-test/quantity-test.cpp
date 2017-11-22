#include "units.hpp"
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
static_assert( is_quantity< time >, "" );
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

void si::run_quantity_tests()
{
    // there are currently no run-time unit tests for units_t
}
