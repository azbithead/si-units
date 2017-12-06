# si::units_t
Defined in header "units.hpp"

```c++
template
<
	typename VALUE,
	typename INTERVAL,
	typename QUANTITY
>
class units_t;
```
Class template `si::units_t` represents a physical quantity measured in SI base units.

It consists of a value of type `VALUE`, an interval of type `INTERVAL` and a quantity of type `QUANTITY`. [`std::is_arithmetic<VALUE>::value`](http://en.cppreference.com/w/cpp/types/is_arithmetic) must evaluate to `true`. The interval is a compile-time rational constant representing one unit of quantity. `INTERVAL` must be of type [`std::ratio`](http://en.cppreference.com/w/cpp/numeric/ratio/ratio). Finally, `QUANTITY` must be of type [`si::quantity_t`](quantity_t.md).
	
The only data stored in a `units_t` is a value of type `VALUE`. If `VALUE` is floating point, then the `units_t` can represent fractions of intervals. `QUANTITY` is included as part of the `units_t`'s type, and is only used when converting between different `units_t`.

## Member types
Member type | Definition
------------|-----------
`quantity_t` | `QUANTITY`
`value_t` | `VALUE`
`interval_t` | `typename INTERVAL::type`

## Member objects
Member | Description
----------------------------------------|-----------------------------------------------------
`static constexpr interval_t interval` | equal to `INTERVAL{}`
`static constexpr quantity_t quantity` | equal to `QUANTITY{}`

## Member functions
Function | Description
---------|------------
[(constructor)](units_t.docs/ctor.md) | constructs new `units_t`
`operator=` | assigns the contents, defaulted
[`value`](units_t.docs/value.md) | returns the number of intervals
[`operator+`<br>`operator-`](units_t.docs/unary_plus_minus.md) | implements unary plus and unary minus
[`operator++`<br>`operator++(int)`<br>`operator--`<br>`operator--(int)`](units_t.docs/inc_dec.md) | increments or decrements value
`operator+=`<br>`operator-=`<br>`operator\*=`<br>`operator/=`<br>`operator%=` | implements compound assignment between two `units_t`
`zero` \[static] | returns the special `units_t` value zero
`min` \[static] | returns the special `units_t` value min
`max` \[static] | returns the special `units_t` value max

## Non-member functions
Function | Description
---------|------------
`std::common_type<si::units_t>` | specializes the [`std::common_type`](http://en.cppreference.com/w/cpp/types/common_type) trait
`operator+`<br>`operator-`<br>`operator\*`<br>`operator/`<br>`operator%` | implements arithmetic operations with `units_t` as arguments
`operator==`<br>`operator!=`<br>`operator<`<br>`operator<=`<br>`operator>`<br>`operator>=` | compares two `units_t``
`units_cast` | converts a `units_t` to another with a different `interval_t`
`duration_cast` | converts `seconds` to [`std::chrono::duration`](http://en.cppreference.com/w/cpp/chrono/duration)
`absolute` | returns the absolute value of a `units_t``
`floor` | converts a `units_t` to another, rounding down
`ceiling` | converts a `units_t` to another, rounding up
`round` | converts a `units_t` to another, rounding to nearest, ties to even
`truncate` | converts a `units_t` to another, rounding toward zero
`exponentiate` | computes the value of a `units_t` raised to a power
`sine` | computes the sine of a `radians`
`cosine` | computes the cosine of a `radians`
`tangent` | computes the tangent of a `radians`
`arc_sine` | returns `radians` given a sine scalar
`arc_cosine` | returns `radians` given a cosine scalar
`arc_tangent` | returns `radians` given a tangent scalar
`basic_string_from` | returns [`std::basic_string`](http://en.cppreference.com/w/cpp/string/basic_string) containing representations of `interval` and `quantity`
`string_from` | returns [`std::string`](http://en.cppreference.com/w/cpp/string/basic_string) containing representations of `interval` and `quantity`
`wstring_from` | returns [`std::wstring`](http://en.cppreference.com/w/cpp/string/basic_string) containing representations of `interval` and `quantity`
`operator <<` | outputs string representation to a `std::ostream`

## Helper types
Type | Definition
---------|------------
`scalar` | `units_t<VALUE, INTERVAL, none>`
`meters` | `units_t<VALUE, INTERVAL, length>`
`kilograms` | `units_t<VALUE, INTERVAL, mass>`
`seconds` | `units_t<VALUE, INTERVAL, time>`
`minutes` | `seconds<std::ratio<60>, VALUE>`
`hours` | `seconds<std::ratio<60*60>, VALUE>`
`days` | `seconds<std::ratio<24*60*60>, VALUE>`
`milliseconds` | `seconds<std::milli, VALUE>`
`microseconds` | `seconds<std::micro, VALUE>`
`nanoseconds` | `seconds<std::nano, VALUE>`
`amperes` | `units_t<VALUE, INTERVAL, current>`
`kelvins` | `units_t<VALUE, INTERVAL, temperature>`
`candelas` | `units_t<VALUE, INTERVAL, luminous_intensity>`
`radians` | `units_t<VALUE, INTERVAL, angle>`
`steradians` | `units_t<VALUE, INTERVAL, solid_angle>`
`hertz` | `units_t<VALUE, INTERVAL, frequency>`
`newtons` | `units_t<VALUE, INTERVAL, force>`
`coulombs` | `units_t<VALUE, INTERVAL, charge>`
`lux` | `units_t<VALUE, INTERVAL, illuminance>`
`pascals` | `units_t<VALUE, INTERVAL, pressure>`
`joules` | `units_t<VALUE, INTERVAL, energy>`
`watts` | `units_t<VALUE, INTERVAL, power>`
`volts` | `units_t<VALUE, INTERVAL, voltage>`
`farads` | `units_t<VALUE, INTERVAL, capacitance>`
`ohms` | `units_t<VALUE, INTERVAL, impedance>`
`siemens` | `units_t<VALUE, INTERVAL, conductance>`
`webers` | `units_t<VALUE, INTERVAL, magnetic_flux>`
`teslas` | `units_t<VALUE, INTERVAL, magnetic_flux_density>`
`henries` | `units_t<VALUE, INTERVAL, inductance>`
`lumens` | `units_t<VALUE, INTERVAL, luminous_flux>`

## Helper classes
Class | Description
------|------------
[`si::quantity_t`](quantity_t.md) | describes the SI quantities in a `units_t`
`std::hash<si::units_t>` | hash support for `units_t`

## Example
```c++
#include <iostream>
#include "units.hpp"

int main(int argc, const char * argv[])
{
    auto theLength = si::meters<std::milli>{50.0}; // 50 millimeters
    auto theSpeed = si::meters<std::kilo>{55.0} / si::hours<>{1.0}; // 55 kilometers per hour
    auto theTime = theLength / theSpeed;
    auto theMsecs = si::milliseconds<>{theLength / theSpeed};

    std::cout << theLength << "\n"
              << theSpeed << "\n"
              << theTime << "\n"
              << theMsecs << "\n";
}
```
Output:
```
50·10⁻³ m
55·5/18 m/s
0.909091·9/25·10⁻² s
3.27273·10⁻³ s
```
