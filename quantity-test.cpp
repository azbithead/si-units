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
}
