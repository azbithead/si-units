#include "units.hpp"

int main( int, char*[] )
{
    // si::is_units
    static_assert( !si::is_units< int >, "" );
    static_assert( si::is_units< si::scalar >, "" );
    static_assert( si::is_units< si::kilograms >, "" );
    static_assert( si::is_units< si::meters >, "" );
    static_assert( si::is_units< si::seconds >, "" );
    static_assert( si::is_units< si::amperes >, "" );
    static_assert( si::is_units< const si::degreesk >, "" );
    static_assert( si::is_units< volatile si::candelas >, "" );
    static_assert( si::is_units< const volatile si::radians >, "" );

    // multiply_units
    static_assert( std::is_same<si::units<1,1>, si::multiply_units<si::kilograms,si::meters>>::value, "" );

    // reciprocal_units
    static_assert( std::is_same<si::units<-1>, si::reciprocal_units<si::kilograms>>::value, "" );

    // divide_units
    static_assert( std::is_same<si::units<1,-1>, si::divide_units<si::kilograms,si::meters>>::value, "" );

    // exponentiate_units
    static_assert( std::is_same<si::units<2,-2>, si::exponentiate_units<si::units<1,-1>,2>>::value, "" );
}
