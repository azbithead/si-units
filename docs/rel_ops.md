# operator==,!=,<,<=,>,>=([si::units_t](units_t.md))

```c++
template <typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
constexpr bool operator == // (1)
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
);

template <typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
constexpr bool operator != // (2)
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
);

template <typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
constexpr bool operator < // (3)
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
);

template <typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
constexpr bool operator > // (4)
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
);

template <typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
constexpr bool operator <= // (5)
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
);

template <typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
constexpr bool operator >= // (6)
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
)
```

Compares two `units_t`s.
1. Checks if `aLHS` and `aRHS` are equal, i.e. the value for the type common to both `units_t`'s are equal.
1. Checks if `aLHS` and `aRHS` are unequal, i.e. the value for the type common to both `units_t`'s are unequal.
1. Checks if `aLHS` is less than `aRHS`.
1. Checks if `aLHS` is greater than `aRHS`.
1. Checks if `aLHS` is less than or equal to `aRHS`.
1. Checks if `aLHS` is greater than or equal to `aRHS`.

## Parameters
aLHS - `units_t` on the left-hand side of the operator

aRHS - `units_t` on the right-hand side of the operator

## Return value
Assuming that CT = `std::common_type_t<si::units_t<V1, I1, Q>, si::units_t<V2, I2, Q>>`, then:
1. CT{aLHS}.value() == CT{aRHS}.value()
1. !(aLHS == aRHS)
1. CT{aLHS}.value() < CT{aRHS}.value()
1. aRHS < aLHS
1. !(aRHS < aLHS)
1. !(aLHS < aRHS)

## Example
```c++
#include <iostream>
#include "units.hpp"

int main()
{
    if( si::newtons<std::kilo>{2.5} == si::newtons<>{2500} )
        std::cout << "2.5 kilonewtons == 2500 newtons\n";
    else
        std::cout << "2.5 kilonewtons != 2500 newtons\n";

    if( si::seconds<>{61} > si::minutes<>{1} )
        std::cout << "61 seconds > 1 minute\n";
    else
        std::cout << "61 seconds <= 1 minute\n";
}
```
Output:
```
2.5 kilonewtons == 2500 newtons
61 seconds > 1 minute
```
