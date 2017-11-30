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

    // absolute
    {
    using int_meters = meters<std::ratio<1>,int>;
    assert( absolute(int_meters{-1}) == int_meters{1} );
    assert( absolute(meters<>{-1.0}) == meters<>{1.0} );
    }

    // floor
    {
    using int_meters = meters<std::ratio<1>,int>;
    using int_kilometers = meters<std::kilo,int>;
    assert( floor<int_kilometers>(int_meters{1500}) == int_kilometers{1} );
    assert( floor<meters<std::kilo>>(meters<>{1500.0}) == meters<std::kilo>{1.0} );
    }

    // ceiling
    {
    using int_meters = meters<std::ratio<1>,int>;
    using int_kilometers = meters<std::kilo,int>;
    assert( ceiling<int_kilometers>(int_meters{1500}) == int_kilometers{2} );
    assert( ceiling<meters<std::kilo>>(meters<>{1500.0}) == meters<std::kilo>{2.0} );
    }

    // round
    {
    using int_kilometers = meters<std::kilo,int>;
    assert( round<int_kilometers>(meters<>{1499.0}) == int_kilometers{1} );
    assert( round<int_kilometers>(meters<>{1501.0}) == int_kilometers{2} );
    assert( round<int_kilometers>(meters<>{1500.0}) == int_kilometers{2} );
    assert( round<int_kilometers>(meters<>{2500.0}) == int_kilometers{2} );
    }

    // square_root
    {
    auto theValue = square_root(seconds<std::mega>{4.0});
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

    static constexpr auto theHalfPi = radians<std::ratio<1,2>>{3.14159};

    // sin
    {
    assert( sine(theHalfPi) == scalar<>{std::sin(theHalfPi.expand())} );
    }

    // cosine
    {
    assert( cosine(radians<>{}.zero()) == scalar<>{std::cos(0.0)} );
    }

    // tangent
    {
    assert( tangent(-theHalfPi) == scalar<>{std::tan(-theHalfPi.expand())} );
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
