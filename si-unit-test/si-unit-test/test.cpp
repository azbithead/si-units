#include "units-test.hpp"
#include "quantity-test.hpp"
#include "string-test.hpp"
#include "math-test.hpp"

int main(int argc, const char * argv[])
{
    using namespace si;

    run_quantity_tests();
    run_units_tests();
    run_math_tests();
    run_string_tests();

    return 0;
}
