# [si::units_t](../units_t.md)::units_t

Function | Description
---------|----------
`constexpr units_t() = default;` | (1)
`constexpr units_t( const units_t& ) = default;` | (2)
`template <typename VALUE2>`<br>`constexpr explicit units_t( VALUE2 aValue );` | (3)
`template <typename VALUE2, typename INTERVAL2>`<br>`constexpr units_t( units_t<VALUE2, INTERVAL2, QUANTITY> aUnits );` | (4)

Constructs a new `units_t` from one of several data sources.
1. The default constructor is defaulted.
2. The copy constructor is defaulted.
3. Constructs a `units_t` with `aValue` intervals. Note that this constructor only participates in overload resolution if `VALUE2` (the argument type) is implicitly convertible to `value_t` (the type of this `units_t`'s intervals) and:
   * `std::is_floating_point<VALUE>::value` is `true`, or
   * `std::is_floating_point<VALUE2>::value` is `false`.
4. Constructs a `units_t` by converting `aUnits` to an appropriate value and interval, as if by calling `units_cast`. In order to prevent truncation during conversion, this constructor only participates in overload resolution if no overflow is induced by conversion and:
   * `std::is_floating_point<VALUE>::value` is `true`, or
   * `std::is_floating_point<VALUE2>::value` is `false`.

## Parameters
`aValue` - a unitless value
`aUnits` - a `units_t` to copy from

## Example
```c++
#include "units.hpp"

int main()
{
    // 1.0 hour, compile-time constant
    constexpr si::hours<> h{1};

    // 3 kilonewtons
    si::newtons<std::kilo,int> kn{3};

    // 55.0 kilometers/hour
    si::units_t<double, std::ratio<1000,3600>, si::divide_quantity<si::distance, si::time>> kph{55};
    // Easier 55.0 kilometers/hour
    constexpr auto kph2 = si::meters<std::kilo>{55} / h;

    // error: This units_t allows whole intervals only
//  si::kelvins<std::ratio<1>, int> k{3.5};

    // 3.5 ticks of a 30Hz clock
    si::seconds<std::ratio<1, 30>> hz30(3.5);

    // 3.0 milliamps
    si::amperes<std::milli> ma{3};

    // 3000.0 microamps constructed from 3 milliamps
    si::amperes<std::micro> ua = ma;

    // error: 1/1000000 is not divisible by 1/1000
//  si::amperes<std::milli,int> ma2 = ua;

    // ok, 3.0 milliamps
    si::amperes<std::milli> ma2 = ua;
}
```
