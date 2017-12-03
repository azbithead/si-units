#include <iostream>
#include <sstream>
#include "units.hpp"
#include "units-test.hpp"

// compile-time unit tests
namespace
{

using namespace si;

template< typename T, typename U >
constexpr bool is_same_v = std::is_same<T, U>::value;

template< typename T, typename S, typename R, typename U >
constexpr bool is_q = is_same_v< T, const si::units_t<S, R, U> >;

using mm_t = units_t<int, std::milli, distance>;
using m_t = units_t<int, std::ratio<1>, distance>;

// is_units_t
static_assert( !is_units_t< int >, "" );
static_assert( is_units_t< m_t >, "" );
static_assert( is_units_t< const m_t >, "" );
static_assert( is_units_t< volatile m_t >, "" );
static_assert( is_units_t< const volatile m_t >, "" );

// gcd
static_assert( si::gcd<12,9> == 3, "" );
static_assert( si::gcd<7,5> == 1, "" );

// lcm
static_assert( si::lcm<4,6> == 12, "" );

// ratio_gcd
static_assert( is_same_v<si::ratio_gcd<std::ratio<2,3>,std::ratio<1,4>>,std::ratio<1,12>>, "" );

// units_cast
static_assert( units_cast<mm_t>( mm_t{5} ).value() == 5, "" );
static_assert( units_cast<m_t>( mm_t{5000} ).value() == 5, "" );
static_assert( units_cast<mm_t>( m_t{5} ).value() == 5000, "" );
static_assert( units_cast<m_t>( units_t<int, std::ratio<3,2>, distance>{2} ).value() == 3, "" );
static_assert( units_cast<seconds<>>( std::chrono::milliseconds{500} ).value() == 0.5, "" );

// duration_cast
static_assert( duration_cast<std::chrono::milliseconds>( minutes<>{0.5} ).count() == 30000, "" );

// is_ratio
static_assert( !si::is_ratio<int>, "" );
static_assert( si::is_ratio<std::ratio<1>>, "" );

// ctor and value()
static_assert( mm_t{}.value() == 0, "" );
static_assert( mm_t{2}.value() == 2, "" );

// quantity_t
static_assert( is_same_v<m_t::quantity_t, distance>, "" );

// storage_t
static_assert( is_same_v<m_t::value_t, int>, "" );

// ratio
static_assert( is_same_v<mm_t::interval_t, std::milli>, "" );

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
static_assert( theResult1.value() == 1001, "" );
static_assert( is_same_v<decltype( theResult1 ), const mm_t>, "" );

// subtract
constexpr auto theResult2 = m_t{ 1 } - mm_t{ 1 };
static_assert( theResult2.value() == 999, "" );
static_assert( is_same_v<decltype( theResult2 ), const mm_t>, "" );

// multiply by scalar
constexpr auto theResult3 = m_t{ 1 } * 2.1;
static_assert( theResult3.value() == 2.1, "" );
static_assert( is_q<decltype( theResult3 ), double, std::ratio<1>, distance>, "" );

constexpr auto theResult4 = 2 * m_t{ 1 };
static_assert( theResult4.value() == 2, "" );
static_assert( is_same_v<decltype( theResult4 ), const m_t>, "" );

// multiply
using s_t = units_t<int, std::ratio<1>, si::time>;
constexpr auto theResult5 = m_t{ 2 } * s_t{ 3 };
static_assert( theResult5.value() == 6, "" );
using meterseconds_t = multiply_quantity<distance, si::time>;
static_assert( is_q<decltype( theResult5 ), int, std::ratio<1>, meterseconds_t>, "" );

// divide by scalar
constexpr auto theResult6 = m_t{ 2 } / 2;
static_assert( theResult6.value() == 1, "" );
static_assert( is_same_v<decltype( theResult6 ), const m_t>, "" );

constexpr auto theResult7 = m_t{ 2 } / 2.0;
static_assert( theResult7.value() == 1.0, "" );
static_assert( is_q<decltype( theResult7 ), double, std::ratio<1>, distance>, "" );

// divide, same units
static_assert( ( m_t{ 2 } / mm_t{ 2 } ) == 1000, "" );

// divide, different units
using km_t = units_t<int,std::kilo,distance>;
using hours_t = units_t<int,std::ratio<60*60>,si::time>;
using mpers_t = divide_quantity<distance,si::time>;
constexpr auto theResult8 = km_t{ 6 } / hours_t{ 2 };
static_assert( theResult8.value() == 3, "" );
static_assert( is_q<decltype( theResult8 ), int, std::ratio<5,18>, mpers_t>, "" );

// divide scalar by
constexpr auto theResult9 = 1.0 / mm_t{2};
static_assert( theResult9.value() == 0.5, "" );
static_assert( is_q<decltype(theResult9), double, std::kilo, reciprocal_quantity<distance>>, "" );

// modulo by scalar
constexpr auto theResult10 = m_t{ 3 } % 2;
static_assert( theResult10.value() == 1, "" );
static_assert( is_same_v<decltype( theResult10 ), const m_t>, "" );

// modulo by quantity
constexpr auto theResult11 = m_t{ 5 } % s_t{ 2 };
static_assert( theResult11.value() == 1, "" );
static_assert( is_same_v<decltype( theResult10 ), const m_t>, "" );

// zero()
static_assert( m_t::zero().value() == 0, "" );

// min()
static_assert( m_t::min().value() == std::numeric_limits<m_t::value_t>::min(), "" );

// max()
static_assert( m_t::max().value() == std::numeric_limits<m_t::value_t>::max(), "" );

inline
void
assertf
(
    bool aInvariant,
    const char* aFilename,
    int aLineNumber
)
{
    if( !aInvariant )
    {
        std::cout
            << "assert failed at "
            << aFilename
            << ":"
            << aLineNumber
            << "\n";
    }
}

} // end of anonymous namespace

#define assert( exp ) assertf( exp, __FILE__, __LINE__ )

#define assert_str_eq( str1, str2 ) assertf( str1 == str2, __FILE__, __LINE__ )

#define assert_abbrev( aT ) \
{ \
    { \
        assertf(si::string_from(aT{}) == si::abbrev<char,aT>, __FILE__, __LINE__); \
    } \
    { \
        assertf(si::wstring_from(aT{}) == si::abbrev<wchar_t,aT>, __FILE__, __LINE__); \
    } \
}

#define assert_literal( aT, str ) \
{ \
    { \
        assert_str_eq(si::string_from(aT{}), str); \
    } \
    { \
        assert_str_eq(si::wstring_from(aT{}), L##str); \
    } \
}

void si::run_units_tests()
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

    // hash
    {
    assert( std::hash<m_t>{}(m_t{3}) == std::hash<m_t::value_t>{}(3) );
    }

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

    assert_literal(std::ratio<1>, "");
    assert_literal(std::kilo, "10\u00B3");
    assert_literal(std::micro, "10\u207B\u2076");
    {
        using TestRatio_t = std::ratio<5,18>;
        assert_literal(TestRatio_t, "5/18");
    }

    {
        using theSuffixT = units_t<int,std::ratio<1>,none>;
        assert_literal(theSuffixT, "");
    }
    {
        using theSuffixT = units_t<int,std::milli,none>;
        assert_literal(theSuffixT, "10\u207B\u00B3");
    }
    {
        using theSuffixT = units_t<int,std::ratio<1>,mass>;
        assert_literal(theSuffixT, "kg");
    }
    {
        using theSuffixT = units_t<int,std::milli,luminous_intensity>;
        assert_literal(theSuffixT, "10\u207B\u00B3 cd");
    }

    {
        static constexpr auto theTestUnits = units_t<int,std::ratio<1>,none>{5};
        std::ostringstream theStream;
        theStream << theTestUnits;
        assert_str_eq(theStream.str(), "5");
    }

    {
        static constexpr auto theTestUnits = units_t<int,std::milli,none>{5};
        std::ostringstream theStream;
        theStream << theTestUnits;
        assert_str_eq(theStream.str(), "5·10\u207B\u00B3");
    }

    {
        static constexpr auto theTestUnits = units_t<int,std::milli,luminous_intensity>{5};
        std::ostringstream theStream;
        theStream << theTestUnits;
        assert_str_eq(theStream.str(), "5·10\u207B\u00B3 cd");
    }

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

    // exponentiate floating point
    {
    auto theValue = seconds<>{3.0};
    assert( exponentiate<2>(theValue) == (theValue * theValue) );
    }

    // exponentiate integer
    {
    auto theValue = seconds<std::ratio<1>,int>{2};
    assert( exponentiate<3>(theValue) == (theValue * theValue * theValue) );
    }

    static constexpr auto theHalfPi = radians<>{3.14159 / 2.0};

    // sin
    {
    assert( sine(theHalfPi) == scalar<>{std::sin(theHalfPi.value())} );
    }

    // cosine
    {
    assert( cosine(radians<>{}.zero()) == scalar<>{std::cos(0.0)} );
    }

    // tangent
    {
    assert( tangent(-theHalfPi) == scalar<>{std::tan(-theHalfPi.value())} );
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
