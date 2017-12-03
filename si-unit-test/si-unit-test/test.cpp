#include "units-test.hpp"
#include "quantity-test.hpp"

int main(int argc, const char * argv[])
{
    using namespace si;

    run_quantity_tests();
    run_units_tests();

    return 0;
}
