#include <iostream>
#include "quantity.hpp"

// compile-time unit tests
namespace
{

using namespace si;

template< typename T, typename U >
constexpr bool is_same = std::is_same<T, U>::value;

template< typename T, typename S, typename R, typename U >
constexpr bool is_q = is_same< T, const si::quantity<S, R, U> >;

using mm_t = quantity<int, std::milli, meters>;
using m_t = quantity<int, std::ratio<1>, meters>;

// is_quantity
static_assert( !is_quantity< int >, "" );
static_assert( is_quantity< m_t >, "" );
static_assert( is_quantity< const m_t >, "" );
static_assert( is_quantity< volatile m_t >, "" );
static_assert( is_quantity< const volatile m_t >, "" );

// gcd
static_assert( gcd<12,9> == 3, "" );
static_assert( gcd<7,5> == 1, "" );

// lcm
static_assert( lcm<4,6> == 12, "" );

// ratio_gcd
static_assert( is_same<ratio_gcd<std::ratio<2,3>,std::ratio<1,4>>,std::ratio<1,12>>, "" );

// quantity_cast
static_assert( quantity_cast<mm_t>( mm_t{5} ).count() == 5, "" );
static_assert( quantity_cast<m_t>( mm_t{5000} ).count() == 5, "" );
static_assert( quantity_cast<mm_t>( m_t{5} ).count() == 5000, "" );
static_assert( quantity_cast<m_t>( quantity<int, std::ratio<3,2>, meters>{2} ).count() == 3, "" );

// is_ratio
static_assert( !is_ratio<int>, "" );
static_assert( is_ratio<std::ratio<1>>, "" );

// ctor and count()
static_assert( mm_t{}.count() == 0, "" );
static_assert( mm_t{2}.count() == 2, "" );

// units_t
static_assert( is_same<m_t::units_t, meters>, "" );

// storage_t
static_assert( is_same<m_t::storage_t, int>, "" );

// ratio_t
static_assert( is_same<mm_t::ratio_t, std::milli>, "" );

// ratio()
static_assert( mm_t{}.ratio().num == 1 && mm_t{}.ratio().den == 1000, "" );

// units()
static_assert( mm_t{}.units() == meters{}, "" );

// unary +
static_assert( +m_t{ 1 } == m_t{ 1 }, "" );

// unary -
static_assert( -m_t{ 1 } == m_t{ -1 }, "" );

// operator == quantity
static_assert( m_t{ 1 } == m_t{ 1 }, "" );
static_assert( m_t{ 1 } == mm_t{ 1000 }, "" );
static_assert( !(m_t{ 1 } == mm_t{ 1 }), "" );

// operator != quantity
static_assert( m_t{ 1 } != mm_t{ 1 }, "" );
static_assert( !(m_t{ 1 } != m_t{ 1 }), "" );

// operator < quantity
static_assert( mm_t{ 999 } < m_t{ 1 }, "" );
static_assert( m_t{ 1 } < m_t{ 2 }, "" );
static_assert( !(m_t{ 1 } < m_t{ 1 }), "" );

// operator > quantity
static_assert( m_t{ 1 } > mm_t{ 999 }, "" );
static_assert( m_t{ 2 } > m_t{ 1 }, "" );
static_assert( !(m_t{ 1 } > m_t{ 1 }), "" );

// operator <= quantity
static_assert( mm_t{ 999 } <= m_t{ 1 }, "" );
static_assert( m_t{ 1 } <= m_t{ 1 }, "" );
static_assert( !(m_t{ 2 } <= m_t{ 1 }), "" );

// operator >= quantity
static_assert( m_t{ 1 } >= mm_t{ 999 }, "" );
static_assert( m_t{ 2 } >= m_t{ 2 }, "" );
static_assert( !(m_t{ 1 } >= m_t{ 2 }), "" );

// add
constexpr auto theResult1 = m_t{ 1 } + mm_t{ 1 };
static_assert( theResult1.count() == 1001, "" );
static_assert( is_same<decltype( theResult1 ), const mm_t>, "" );

// subtract
constexpr auto theResult2 = m_t{ 1 } - mm_t{ 1 };
static_assert( theResult2.count() == 999, "" );
static_assert( is_same<decltype( theResult2 ), const mm_t>, "" );

// multiply by scalar
constexpr auto theResult3 = m_t{ 1 } * 2.1;
static_assert( theResult3.count() == 2.1, "" );
static_assert( is_q<decltype( theResult3 ), double, std::ratio<1>, meters>, "" );

constexpr auto theResult4 = 2 * m_t{ 1 };
static_assert( theResult4.count() == 2, "" );
static_assert( is_same<decltype( theResult4 ), const m_t>, "" );

// multiply
using s_t = quantity<int, std::ratio<1>, seconds>;
constexpr auto theResult5 = m_t{ 2 } * s_t{ 3 };
static_assert( theResult5.count() == 6, "" );
using meterseconds_t = multiply_units<meters, seconds>;
static_assert( is_q<decltype( theResult5 ), int, std::ratio<1>, meterseconds_t>, "" );

// divide by scalar
constexpr auto theResult6 = m_t{ 2 } / 2;
static_assert( theResult6.count() == 1, "" );
static_assert( is_same<decltype( theResult6 ), const m_t>, "" );

constexpr auto theResult7 = m_t{ 2 } / 2.0;
static_assert( theResult7.count() == 1.0, "" );
static_assert( is_q<decltype( theResult7 ), double, std::ratio<1>, meters>, "" );

// divide, same units
static_assert( ( m_t{ 2 } / mm_t{ 2 } ) == 1000, "" );

// divide, different units
using km_t = quantity<int,std::kilo,meters>;
using hours_t = quantity<int,std::ratio<60*60>,seconds>;
using mpers_t = divide_units<meters,seconds>;
constexpr auto theResult8 = km_t{ 6 } / hours_t{ 2 };
static_assert( theResult8.count() == 3, "" );
static_assert( is_q<decltype( theResult8 ), int, std::ratio<5,18>, mpers_t>, "" );

// divide scalar by
constexpr auto theResult9 = 1.0 / mm_t{2};
static_assert( theResult9.count() == 0.5, "" );
static_assert( is_q<decltype(theResult9), double, std::kilo, reciprocal_units<meters>>, "" );

// modulo by scalar
constexpr auto theResult10 = m_t{ 3 } % 2;
static_assert( theResult10.count() == 1, "" );
static_assert( is_same<decltype( theResult10 ), const m_t>, "" );

// zero()
static_assert( m_t::zero().count() == 0, "" );

// min()
static_assert( m_t::min().count() == std::numeric_limits<m_t::storage_t>::min(), "" );

// max()
static_assert( m_t::max().count() == std::numeric_limits<m_t::storage_t>::max(), "" );

void assertf( bool aInvariant, int aLineNumber )
{
    if( !aInvariant )
    {
        std::cout << "assert failed on line " << aLineNumber << "\n";
    }
}

} // end of anonymous namespace

#define assert( exp ) assertf( exp, __LINE__ )

void run_quantity_tests()
{
    using namespace si;

    // pre increment
    assert( ++m_t{ 1 } == m_t{ 2 } );

    // post increment
    {
    auto theValue = m_t{ 1 };
    assert( theValue++ == m_t{ 1 } );
    assert( theValue == m_t{ 2 } );
    }

    // pre decrement
    assert( --m_t{ 2 } == m_t{ 1 } );

    // post decrement
    {
    auto theValue = m_t{ 1 };
    assert( theValue-- == m_t{ 1 } );
    assert( theValue == m_t{ 0 } );
    }

    // add assign
    {
    auto theValue = m_t{ 1 };
    theValue += m_t{ 2 };
    assert( theValue == m_t{ 3 } );
    }

    // subtract assign
    {
    auto theValue = m_t{ 1 };
    theValue -= m_t{ 1 };
    assert( theValue == m_t{ 0 } );
    }

    // multiply assign
    {
    auto theValue = m_t{ 2 };
    theValue *= 2;
    assert( theValue == m_t{ 4 } );
    }

    // divide assign
    {
    auto theValue = m_t{ 2 };
    theValue /= 2;
    assert( theValue == m_t{ 1 } );
    }

    // modulo assign
    {
    auto theValue = m_t{ 3 };
    theValue %= 2;
    assert( theValue == m_t{ 1 } );
    theValue = m_t{ 3 };
    theValue %= m_t{ 2 };
    assert( theValue == m_t{ 1 } );
    }
}
