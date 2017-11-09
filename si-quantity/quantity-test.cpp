#include "quantity.hpp"

template< typename T, typename U >
constexpr bool is_same = std::is_same<T, U>::value;

template< typename T, typename S, typename R, typename U >
constexpr bool is_q = is_same< T, const si::quantity<S, R, U> >;

void run_quantity_tests()
{
    using mm_t = si::quantity<int, std::milli, si::meters>;
    using m_t = si::quantity<int, std::ratio<1>, si::meters>;
    using s_t = si::quantity<int, std::ratio<1>, si::seconds>;

    // si::is_quantity
    static_assert( !si::is_quantity< int >, "" );
    static_assert( si::is_quantity< m_t >, "" );
    static_assert( si::is_quantity< const m_t >, "" );
    static_assert( si::is_quantity< volatile m_t >, "" );
    static_assert( si::is_quantity< const volatile m_t >, "" );

    // si::gcd
    static_assert( si::gcd<12,9> == 3, "" );
    static_assert( si::gcd<7,5> == 1, "" );

    // si::lcm
    static_assert( si::lcm<4,6> == 12, "" );

    // si::ratio_gcd
    static_assert( is_same<si::ratio_gcd<std::ratio<2,3>,std::ratio<1,4>>,std::ratio<1,12>>, "" );

    // si::quantity_cast
    // num == 1, den == 1
    static_assert( si::quantity_cast<mm_t>( mm_t{5} ).count() == 5, "" );
    // num == 1, den == 1000
    static_assert( si::quantity_cast<m_t>( mm_t{5000} ).count() == 5, "" );
    // num == 1000, den == 1
    static_assert( si::quantity_cast<mm_t>( m_t{5} ).count() == 5000, "" );
    // num == 3, den == 2
    static_assert( si::quantity_cast<m_t>( si::quantity<int, std::ratio<3,2>, si::meters>{2} ).count() == 3, "" );

    // si::is_ratio
    static_assert( !si::is_ratio<int>, "" );
    static_assert( si::is_ratio<std::ratio<1>>, "" );

    // operator == si::quantity
    static_assert( m_t{ 1 } == m_t{ 1 }, "" );
    static_assert( m_t{ 1 } == mm_t{ 1000 }, "" );
    static_assert( !(m_t{ 1 } == mm_t{ 1 }), "" );

    // operator != si::quantity
    static_assert( m_t{ 1 } != mm_t{ 1 }, "" );
    static_assert( !(m_t{ 1 } != m_t{ 1 }), "" );

    // operator < si::quantity
    static_assert( mm_t{ 999 } < m_t{ 1 }, "" );
    static_assert( m_t{ 1 } < m_t{ 2 }, "" );
    static_assert( !(m_t{ 1 } < m_t{ 1 }), "" );

    // operator > si::quantity
    static_assert( m_t{ 1 } > mm_t{ 999 }, "" );
    static_assert( m_t{ 2 } > m_t{ 1 }, "" );
    static_assert( !(m_t{ 1 } > m_t{ 1 }), "" );

    // operator <= si::quantity
    static_assert( mm_t{ 999 } <= m_t{ 1 }, "" );
    static_assert( m_t{ 1 } <= m_t{ 1 }, "" );
    static_assert( !(m_t{ 2 } <= m_t{ 1 }), "" );

    // operator >= si::quantity
    static_assert( m_t{ 1 } >= mm_t{ 999 }, "" );
    static_assert( m_t{ 2 } >= m_t{ 2 }, "" );
    static_assert( !(m_t{ 1 } >= m_t{ 2 }), "" );

    // add si::quantity to si::quantity
    {
    constexpr auto theResult = m_t{ 1 } + mm_t{ 1 };
    static_assert( theResult.count() == 1001, "" );
    static_assert( is_same<decltype( theResult ), const mm_t>, "" );
    }

    // subtract si::quantity from si::quantity
    {
    constexpr auto theResult = m_t{ 1 } - mm_t{ 1 };
    static_assert( theResult.count() == 999, "" );
    static_assert( is_same<decltype( theResult ), const mm_t>, "" );
    }

    // multiply si::quantity by scalar
    {
    constexpr auto theResult = m_t{ 1 } * 2.1;
    static_assert( theResult.count() == 2.1, "" );
    static_assert( is_q<decltype( theResult ), double, std::ratio<1>, si::meters>, "" );
    }

    {
    constexpr auto theResult = 2 * m_t{ 1 };
    static_assert( theResult.count() == 2, "" );
    static_assert( is_same<decltype( theResult ), const m_t>, "" );
    }

    // multiply si::quantity by si::quantity
    {
    constexpr auto theResult = m_t{ 2 } * s_t{ 3 };
    static_assert( theResult.count() == 6, "" );
    using meterseconds_t = si::multiply_units<si::meters, si::seconds>;
    static_assert( is_q<decltype( theResult ), int, std::ratio<1>, meterseconds_t>, "" );
    }

    // divide si::quantity by scalar
    {
    constexpr auto theResult = m_t{ 2 } / 2;
    static_assert( theResult.count() == 1, "" );
    static_assert( is_same<decltype( theResult ), const m_t>, "" );
    }

    {
    constexpr auto theResult = m_t{ 2 } / 2.0;
    static_assert( theResult.count() == 1.0, "" );
    static_assert( is_q<decltype( theResult ), double, std::ratio<1>, si::meters>, "" );
    }

    // divide si::quantity by si::quantity, same units
    static_assert( ( m_t{ 2 } / mm_t{ 2 } ) == 1000, "" );

    // divide si::quantity by si::quantity, different units
    {
    using km_t = si::quantity<int,std::kilo,si::meters>;
    using hours_t = si::quantity<int,std::ratio<60*60>,si::seconds>;
    using mpers_t = si::divide_units<si::meters,si::seconds>;
    constexpr auto theResult = km_t{ 6 } / hours_t{ 2 };
    static_assert( theResult.count() == 3, "" );
    static_assert( is_q<decltype( theResult ), int, std::ratio<5,18>, mpers_t>, "" );
    }

    // divide scalar by si::quantity
    {
    constexpr auto theResult = 1.0 / mm_t{2};
    static_assert( theResult.count() == 0.5, "" );
    static_assert( is_q<decltype(theResult), double, std::kilo, si::reciprocal_units<si::meters>>, "" );
    }

    // modulo si::quantity by scalar
    {
    constexpr auto theResult = m_t{ 3 } % 2;
    static_assert( theResult.count() == 1, "" );
    static_assert( is_same<decltype( theResult ), const m_t>, "" );
    }
}
