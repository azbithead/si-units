# [si::units_t](units_t.md)::operator++, si::units_t::operator--

Function | Description
---------|----------
`constexpr units_t& operator++();` | (1)
`constexpr units_t operator++(int);` | (2)
`constexpr units_t& operator--();` | (3)
`constexpr units_t operator--(int);` | (4)

Increments or decrements the number of intervals for this `units_t`.
If value_ is a member variable holding the number of intervals in a `units_t` object,
1. Equivalent to `++value_; return *this;`
2. Equivalent to `return duration(value_++);`
3. Equivalent to `--value_; return *this;`
4. Equivalent to `return units_t(value_--);`

## Parameters
(none)

## Return value
1. a reference to this `units_t` object after modification
2. a reference to this `units_t` object before modification
3. a reference to this `units_t` object after modification
4. a reference to this `units_t` object before modification

## Example
```c++
#include <iostream>
#include "units.hpp"

int main()
{
    si::newtons<std::kilo> kn{3};
    si::newtons<> n = --kn;
    ++n;

    std::cout << n << "\n";
}
```
Output:
```
2001 N
```