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

    constexpr auto theTime = si::quantity<int, std::milli, si::seconds>{2};
    constexpr auto theDistance = si::quantity<double, std::ratio<1>, si::meters>{1};

    constexpr auto theSpeed = theDistance / theTime;

    std::wcout << si::string_from_quantity<wchar_t>(theSpeed) << L"\n";

    using KPH_t = si::quantity<double, std::ratio_divide<std::kilo, std::ratio<60*60>>, si::divide_units<si::meters, si::seconds>>;
    auto theKmPerHr = si::quantity_cast<KPH_t>(theSpeed);

    std::cout << si::string_from_quantity<char>(theKmPerHr) << "\n";

    return 0;
}
