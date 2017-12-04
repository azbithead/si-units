#include <iostream>
#include "helpers.hpp"
#include "ratio.hpp"
#include "ratio-test.hpp"

// compile-time unit tests
namespace
{
using namespace si;

// is_ratio
static_assert( !is_ratio< int >, "" );
static_assert( is_ratio< std::ratio<1> >, "" );

// gcd
static_assert( si::gcd<12,9> == 3, "" );
static_assert( si::gcd<7,5> == 1, "" );

// lcm
static_assert( si::lcm<4,6> == 12, "" );

// ratio_gcd
static_assert
(
    std::is_same
    <
        si::ratio_gcd
        <
            std::ratio<2,3>,
            std::ratio<1,4>
        >,
        std::ratio<1,12>
    >::value,
    ""
);

static_assert
(
    std::is_same
    <
        si::ratio_sqrt
        <
            std::ratio<4>,
            std::nano
        >::type,
        std::ratio<2>
    >::value,
    ""
);

static_assert
(
    std::is_same
    <
        si::ratio_sqrt
        <
            std::ratio<5>,
            std::nano
        >::type,
        std::ratio<51841,23184>
    >::value,
    ""
);

} // end of anonymous namespace

void si::run_ratio_tests()
{
    using namespace si;

    assert_literal(std::ratio<1>, "");
    assert_literal(std::kilo, "10\u00B3");
    assert_literal(std::micro, "10\u207B\u2076");
    {
        using TestRatio_t = std::ratio<5,18>;
        assert_literal(TestRatio_t, "5/18");
    }
}
