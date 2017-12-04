#include "exponent.hpp"
#include "helpers.hpp"
#include "exponent-test.hpp"

// compile-time unit tests
namespace
{

using namespace si;

// multiply_exponent
static_assert( std::is_same<exponent_t<4>, multiply_exponent<exponent_t<1>,exponent_t<3>>>::value, "" );

// power_exponent
static_assert( std::is_same<exponent_t<-6>, power_exponent<exponent_t<-2>,3>>::value, "" );

// reciprocal_exponent
static_assert( std::is_same<exponent_t<-1>, reciprocal_exponent<exponent_t<1>>>::value, "" );

// divide_exponent
static_assert( std::is_same<exponent_t<3>, divide_exponent<exponent_t<5>,exponent_t<2>>>::value, "" );

// root_exponent
static_assert( std::is_same<exponent_t<2>, root_exponent<exponent_t<4>,2>>::value, "" );

} // end of anonymous namespace

void si::run_exponent_tests()
{
    using namespace si;

    assert_literal(exponent_t<2>,"\u00B2");
    assert_literal(exponent_t<-3>,"\u207B\u00B3");
}
