#include <iostream>
#include "units-test.hpp"
#include "quantity-test.hpp"
#include "units-io.hpp"
#include "quantity.hpp"
#include "quantity-io.hpp"

int main(int argc, const char * argv[])
{
    run_units_tests();
    run_quantity_tests();

    return 0;
}
