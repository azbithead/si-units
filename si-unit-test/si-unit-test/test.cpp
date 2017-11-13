#include <iostream>

#include "units-test.hpp"
#include "quantity-test.hpp"
#include "string-test.hpp"

int main(int argc, const char * argv[])
{
    using namespace si;

    run_units_tests();
    run_quantity_tests();
    run_string_tests();

    return 0;
}
