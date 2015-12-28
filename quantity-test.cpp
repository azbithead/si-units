#include "quantity.hpp"

template< class T, class U >
constexpr bool is_same = std::is_same<T, U>::value;

int main( int aArgCount, char* aArgs[] )
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

    // operator < si::quantity
    static_assert( mm_t{ 999 } < m_t{ 1 }, "" );
    static_assert( m_t{ 1 } < m_t{ 2 }, "" );
    static_assert( !(m_t{ 1 } < m_t{ 1 }), "" );

    // add si::quantity to si::quantity
    static_assert( (m_t{ 1 } + mm_t{ 1 }).count() == 1001, "" );

    // subtract si::quantity from si::quantity
    static_assert( (m_t{ 1 } - mm_t{ 1 }).count() == 999, "" );

    // multiply si::quantity by scalar
    static_assert( ( m_t{ 1 } * 2.1 ).count() == 2.1, "" );
    static_assert( ( 2 * m_t{ 1 } ).count() == 2, "" );

#if 0
    // multiply si::quantity by si::quantity
    constexpr auto theResult1 = m_t{ 2 } * s_t{ 3 };
    static_assert( theResult1.count() == 6, "" );
    using ms_t = decltype( theResult1 );
    static_assert( is_same<ms_t::units, si::units<0,1,1>>, "" );
#endif

    // divide si::quantity by scalar
    static_assert( ( m_t{ 2 } / 2 ).count() == 1, "" );
    static_assert( ( m_t{ 2 } / 2.0 ).count() == 1.0, "" );

    // divide si::quantity by si::quantity, same units
    static_assert( ( m_t{ 2 } / mm_t{ 2 } ) == 1000, "" );

    // divide si::quantity by si::quantity, different units
    constexpr auto theResult2 = m_t{ 2 } / s_t{ 2 };
    static_assert( theResult2.count() == 1, "" );
    static_assert( is_same
    <
        decltype( theResult2 ),
        const si::quantity
        <
            int,
            std::ratio<1,1>,
            si::units<0,1,-1>
        >
    >, "" );

#if 0
    // modulo si::quantity by scalar
    constexpr theResult3 = m_t{ 3 } % 2;
    static_assert( theResult3.count() == 1, "" );
    using r3_t = decltype( theResult3 );
#endif
}
