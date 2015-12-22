#include "quantity.hpp"

template< class T, class U >
constexpr bool is_same_v = std::is_same<T, U>::value;

int main( int aArgCount, char* aArgs[] )
{
    using mm_t = si::quantity<si::meters,int,std::milli>;
    using m_t = si::quantity<si::meters,int>;

    // si::is_quantity
    static_assert( !si::is_quantity< int >, "" );
    static_assert( si::is_quantity< si::quantity<si::meters,int> >, "" );
    static_assert( si::is_quantity< const si::quantity<si::meters,int> >, "" );
    static_assert( si::is_quantity< volatile si::quantity<si::meters,int> >, "" );
    static_assert( si::is_quantity< const volatile si::quantity<si::meters,int> >, "" );

    // si::gcd
    static_assert( si::gcd<12,9> == 3, "" );
    static_assert( si::gcd<7,5> == 1, "" );

    // si::lcm
    static_assert( si::lcm<4,6> == 12, "" );

    // si::ratio_gcd
    static_assert( is_same_v<si::ratio_gcd<std::ratio<2,3>,std::ratio<1,4>>,std::ratio<1,12>>, "" );

    // si::quantity_cast
    // num == 1, den == 1
    static_assert( si::quantity_cast<mm_t>( mm_t{5} ).count() == 5, "" );
    // num == 1, den == 1000
    static_assert( si::quantity_cast<m_t>( mm_t{5000} ).count() == 5, "" );
    // num == 1000, den == 1
    static_assert( si::quantity_cast<mm_t>( m_t{5} ).count() == 5000, "" );
    // num == 3, den == 2
    static_assert( si::quantity_cast<m_t>( si::quantity<si::meters, int, std::ratio<3,2>>{2} ).count() == 3, "" );

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

    // operator + si::quantity
    static_assert( (m_t{ 1 } + mm_t{ 1 }).count() == 1001, "" );

    // operator - si::quantity
    static_assert( (m_t{ 1 } - mm_t{ 1 }).count() == 999, "" );

    // scalar multiply si::quantity
    static_assert( ( m_t{ 1 } * 2.1 ).count() == 2.1, "" );
    static_assert( ( 2 * m_t{ 1 } ).count() == 2, "" );
}
