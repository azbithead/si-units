# si::units_cast

```c++
template <typename ToUnitsT, typename QuantityT, typename ValueT, typename IntervalT>
constexpr ToUnitsT units_cast(units_t<ValueT, IntervalT, QuantityT> aFromUnits); // (1)

template <typename ToUnitsT, typename RepT, typename PeriodT>
constexpr ToUnitsT units_cast(std::chrono::duration<RepT, PeriodT> aFromDuration); // (2)
```

1. Converts a `units_t` to a `units_t` having different `interval_t` and/or `value_t`.
1. Converts a `std::chrono::duration` to a `units_t` having `quantity_t` equal to `si::time`.

## Parameters
aFromUnits - a `units_t` to convert

aFromDuration - a `std::chrono::duration` to convert

## Return value
A `units_t` of type `ToUnitsT`.

## Notes
The function does not participate in overload resolution unless `ToUnitsT` is of type `units_t`.

Casting between floating-point `units_t` or between integer `units_t` where the source `IntervalT` is exactly divisible by the target `IntervalT` can be performed implicitly, no `units_cast` is needed.

Casting from a floating-point `units_t` to an integer `units_t` is subject to undefined behavior when the floating-point value is NaN, infinity, or too large to be representable by the target's integer type.

## Example
```c++
#include <iostream>
#include <chrono>
#include <ratio>
#include <thread>
#include "units.hpp"

void f()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    f();
    auto t2 = std::chrono::high_resolution_clock::now();

    // convert std::chrono::duration to fractional si milliseconds
    auto fp_ms = si::units_cast<si::milliseconds<>>(t2 - t1);

    // this cast is required to get integral milliseconds from fractional milliseconds
    auto int_ms = si::units_cast<si::seconds<std::milli,int>>(fp_ms);

    std::cout << "f() took " << fp_ms.value() << " ms, "
              << "or " << int_ms.value() << " whole milliseconds\n";
}
```
Output:
```
f() took 1001.55 ms, or 1001 whole milliseconds
```

## See also
Function | Description
---------|------------
`duration_cast` | converts `seconds` to [`std::chrono::duration`](http://en.cppreference.com/w/cpp/chrono/duration)
`floor` | converts a `units_t` to another, rounding down
`ceiling` | converts a `units_t` to another, rounding up
`round` | converts a `units_t` to another, rounding to nearest, ties to even
`truncate` | converts a `units_t` to another, rounding toward zero
