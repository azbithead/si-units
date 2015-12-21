#include "quantity.hpp"

int main( int aArgCount, char* aArgs[] )
{
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
    static_assert( std::is_same<si::ratio_gcd<std::ratio<2,3>,std::ratio<1,4>>, std::ratio<1,12>>::value, "" );
}
