#include "units.hpp"

// compile-time unit tests
namespace
{

using namespace si;

// is_units
static_assert( !is_units< int >, "" );
static_assert( is_units< scalar >, "" );
static_assert( is_units< kilograms >, "" );
static_assert( is_units< meters >, "" );
static_assert( is_units< seconds >, "" );
static_assert( is_units< amperes >, "" );
static_assert( is_units< const degreesk >, "" );
static_assert( is_units< volatile candelas >, "" );
static_assert( is_units< const volatile radians >, "" );

// multiply_units
static_assert( std::is_same<units<1,1>, multiply_units<kilograms,meters>>::value, "" );

// reciprocal_units
static_assert( std::is_same<units<-1>, reciprocal_units<kilograms>>::value, "" );

// divide_units
static_assert( std::is_same<units<1,-1>, divide_units<kilograms,meters>>::value, "" );

// exponentiate_units
static_assert( std::is_same<units<2,-2>, exponentiate_units<units<1,-1>,2>>::value, "" );

} // end of anonymous namespace

void run_units_tests()
{
    // there are currently no run-time unit tests for units_t
}
