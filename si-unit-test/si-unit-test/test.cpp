#include "units-test.hpp"
#include "quantity-test.hpp"
#include "ratio-test.hpp"
#include "exponent-test.hpp"

int main(int argc, const char * argv[])
{
    using namespace si;

    run_ratio_tests();
    run_quantity_tests();
    run_units_tests();
    run_exponent_tests();

    return 0;
}
