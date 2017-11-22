#include <iostream>
#include "math.hpp"
#include "math-test.hpp"

// compile-time unit tests
namespace
{

using namespace si;

constexpr inline void assertf( bool aInvariant, int aLineNumber )
{
    if( !aInvariant )
    {
        std::cout << "assert failed on line " << aLineNumber << "\n";
    }
}

} // end of anonymous namespace

#define assert( exp ) assertf( exp, __LINE__ )

void si::run_math_tests()
{
    using namespace si;

    // sqrt
    {
    auto theValue = sqrt(seconds<std::mega>{4.0});
    auto theExpectedResult = seconds<std::kilo>{2.0};
    assert( theValue == theExpectedResult );
    }

    // pow floating point
    {
    auto theValue = seconds<>{3.0};
    assert( pow<2>(theValue) == (theValue * theValue) );
    }

    // pow integer
    {
    auto theValue = seconds<std::ratio<1>,int>{2};
    assert( pow<3>(theValue) == (theValue * theValue * theValue) );
    }

    static constexpr auto theHalfPi = 3.14159 / 2.0;

    // sin
    {
    assert( sine(radians<>{theHalfPi}) == scalar<>{std::sin(theHalfPi)} );
    }

    // cosine
    {
    assert( cosine(radians<>{0.0}) == scalar<>{std::cos(0.0)} );
    }

    // tangent
    {
    assert( tangent(radians<>{-theHalfPi}) == scalar<>{std::tan(-theHalfPi)} );
    }

    // arc sine
    {
    assert( arc_sine(scalar<>{1.0}) == radians<>{std::asin(1.0)} );
    }

    // arc cosine
    {
    assert( arc_cosine(scalar<>{1.0}) == radians<>{std::acos(1.0)} );
    }

    // arc tangent
    {
    assert( arc_tangent(scalar<>{1.0}) == radians<>{std::atan(1.0)} );
    }
}
