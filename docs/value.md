# [si::units_t](units_t.md)::value

`constexpr value_t value() const;`

Returns the number of intervals for this `units_t`.

## Parameters
(none)

## Return value
The number of intervals for this `units_t`.

## Example
```c++
#include <iostream>
#include "units.hpp"

int main()
{
    // 3 kilonewtons
    si::newtons<std::kilo> kn{3};

    // 55.0 kilometers/hour
    constexpr auto kph = si::meters<std::kilo>{55} / si::hours<>{1};

    std::cout <<  "3 kN has " << kn.value() << " intervals\n"
              <<  "55 kph has " << kph.value() << " intervals\n";
}
```
Output:
```
3 kN has 3 intervals
55 kph has 55 intervals
```
