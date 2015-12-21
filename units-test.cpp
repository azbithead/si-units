#include "units.hpp"

int main( int aArgCount, char* aArgs[] )
{
    // is_units
    static_assert( !is_units< int >, "" );
    static_assert( is_units< scalar >, "" );
    static_assert( is_units< kilograms >, "" );
    static_assert( is_units< meters >, "" );
    static_assert( is_units< seconds >, "" );
    static_assert( is_units< amperes >, "" );
    static_assert( is_units< degreesk >, "" );
    static_assert( is_units< candelas >, "" );
    static_assert( is_units< radians >, "" );
}
