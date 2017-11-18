#include "units.hpp"
#include "units-test.hpp"

// compile-time unit tests
namespace
{

using namespace si;

// is_units
static_assert( !is_units< int >, "" );
static_assert( is_units< scalar >, "" );
static_assert( is_units< mass >, "" );
static_assert( is_units< distance >, "" );
static_assert( is_units< time >, "" );
static_assert( is_units< current >, "" );
static_assert( is_units< const temperature >, "" );
static_assert( is_units< volatile luminous_intensity >, "" );
static_assert( is_units< const volatile angle >, "" );

// multiply_units
static_assert( std::is_same<units<4,6>, multiply_units<units<1,2>,units<3,4>>>::value, "" );

// power_units
static_assert( std::is_same<units<3,-6>, power_units<units<1,-2>,3>>::value, "" );

// reciprocal_units
static_assert( std::is_same<units<-1>, reciprocal_units<mass>>::value, "" );

// divide_units
static_assert( std::is_same<units<1,-1>, divide_units<mass,distance>>::value, "" );

// root_units
static_assert( std::is_same<units<2,-1>, root_units<units<4,-2>,2>>::value, "" );

} // end of anonymous namespace

void si::run_units_tests()
{
    // there are currently no run-time unit tests for units_t
}
