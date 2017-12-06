# [si::units_t](units_t.md)::operator+=, -=, *=, /=, %=

Function | Description
---------|----------
`constexpr units_t& operator+=(units_t u);` | (1)
`constexpr units_t& operator-=(units_t u);` | (2)
`constexpr units_t& operator*=(value_t rhs);` | (3)
`constexpr units_t& operator/=(value_t rhs);` | (4)
`constexpr units_t& operator%=(value_t rhs);` | (5)
`constexpr units_t& operator%=(units_t u);` | (6)

Performs compound assignments between two `units_t`'s with the same `interval_t` and `quantity_t` or between a `units_t` and a number of intervals.
If mValue is a member variable holding the number of intervals in this `units_t` object:
1. Equivalent to `mValue += u.value(); return *this;`
2. Equivalent to `mValue -= u.value(); return *this;`
3. Equivalent to `mValue *= rhs; return *this;`
4. Equivalent to `mValue /= rhs; return *this;`
5. Equivalent to `mValue %= rhs; return *this;`
6. Equivalent to `mValue %= u.value(); return *this;`

## Parameters
u - `units_t` on the right-hand side of the operator
rhs - number of intervals on the right-hand side of the operator

## Return value
A reference to this `units_t` object after modification

## Example
```c++
#include <iostream>
#include "units.hpp"

int main()
{
    si::joules<std::ratio<1>,int> j{11};
    j *= 2;
    using kjoules = si::joules<std::kilo,int>;
    j += kjoules{10}; // kilojoules implicitly convert to joules
    std::cout << j.value() << " joules equals "
              << si::units_cast<kjoules>(j).value()
              << " kilojoules plus ";
    j %= kjoules{1};
    std::cout << j.value() << " joules\n";
}
```
Output:
```
10022 joules equals 10 kilojoules plus 22 joules
```