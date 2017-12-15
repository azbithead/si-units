# si::units_t
Defined in header "units.hpp"

```c++
template
<
	typename ValueT,
	typename IntervalT,
	typename QuantityT
>
class units_t;
```
Class template `si::units_t` represents a physical quantity measured in SI base units.

It contains member data of type `ValueT` which must be an integer or floating point type, that is,  [`std::is_arithmetic<ValueT>::value`](http://en.cppreference.com/w/cpp/types/is_arithmetic) must evaluate to `true`.  `IntervalT` must be of type [`std::ratio`](http://en.cppreference.com/w/cpp/numeric/ratio/ratio) and represents one unit of quantity. Finally, `QuantityT` must be of type [`si::quantity_t`](quantity_t.md).
	
The only data stored in a `units_t` is a value of type `ValueT`. If `ValueT` is a floating point type, then the `units_t` can represent fractions of intervals. `QuantityT` and `IntervalT` are included as part of the `units_t`'s type, and are only used when converting between different `units_t`.

## Member types
Member type | Definition
------------|-----------
`quantity_t` | `QuantityT`
`value_t` | `ValueT`
`interval_t` | `typename IntervalT::type`

## Member objects
Member | Description
----------------------------------------|-----------------------------------------------------
`static constexpr interval_t interval` | equal to `IntervalT{}`
`static constexpr quantity_t quantity` | equal to `QuantityT{}`

## Member functions
Function | Description
---------|------------
[(constructor)](ctor.md) | constructs new `units_t`
`operator=` | assigns the contents, defaulted
[`value`](value.md) | returns the number of intervals
[`operator+`<br>`operator-`](unary_plus_minus.md) | implements unary plus and unary minus
[`operator++`<br>`operator++(int)`<br>`operator--`<br>`operator--(int)`](inc_dec.md) | increments or decrements value
[`operator+=`<br>`operator-=`<br>`operator*=`<br>`operator/=`<br>`operator%=`](comp_assign.md) | implements compound assignment between two `units_t`
[`zero`](zero.md) \[static] | returns the special `units_t` value zero
[`min`](min.md) \[static] | returns the special `units_t` value min
[`max`](max.md) \[static] | returns the special `units_t` value max

## Non-member functions
Function | Description
---------|------------
[`std::common_type<si::units_t>`](common_type.md) | specializes the [`std::common_type`](http://en.cppreference.com/w/cpp/types/common_type) trait
`operator+`<br>`operator-`<br>`operator*`<br>`operator/`<br>`operator%` | implements arithmetic operations with `units_t` as arguments
[`operator==`<br>`operator!=`<br>`operator<`<br>`operator<=`<br>`operator>`<br>`operator>=`](rel_ops.md) | compares two `units_t``
[`units_cast`](units_cast.md) | converts a `units_t` to another with a different `interval_t`
[`duration_cast`](duration_cast.md) | converts `seconds` to [`std::chrono::duration`](http://en.cppreference.com/w/cpp/chrono/duration)
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

## Operations
The following template metafunctions produce new `units_t` types by performing operations on `units_t` types.

Operation | Description
----------|------------
`multiply_units` |  Product of multiplying `units_t` types
`divide_units` |  Quotient of dividing two `units_t` types
`reciprocal_units` |  Reciprocal of a `units_t` type
`power_units` |  Raise a `units_t` to a power

## Helper types
Type | Definition
---------|------------
`scalar` | `units_t<ValueT, IntervalT, none>`
`meters` | `units_t<ValueT, IntervalT, length>`
`kilograms` | `units_t<ValueT, IntervalT, mass>`
`grams` | `units_t<ValueT, std::milli, mass>`
`seconds` | `units_t<ValueT, IntervalT, time>`
`minutes` | `seconds<std::ratio<60>, ValueT>`
`hours` | `seconds<std::ratio<60*60>, ValueT>`
`days` | `seconds<std::ratio<24*60*60>, ValueT>`
`milliseconds` | `seconds<std::milli, ValueT>`
`microseconds` | `seconds<std::micro, ValueT>`
`nanoseconds` | `seconds<std::nano, ValueT>`
`amperes` | `units_t<ValueT, IntervalT, current>`
`kelvins` | `units_t<ValueT, IntervalT, temperature>`
`candelas` | `units_t<ValueT, IntervalT, luminous_intensity>`
`radians` | `units_t<ValueT, IntervalT, angle>`
`steradians` | `units_t<ValueT, IntervalT, solid_angle>`
`hertz` | `units_t<ValueT, IntervalT, frequency>`
`newtons` | `units_t<ValueT, IntervalT, force>`
`coulombs` | `units_t<ValueT, IntervalT, charge>`
`lux` | `units_t<ValueT, IntervalT, illuminance>`
`pascals` | `units_t<ValueT, IntervalT, pressure>`
`joules` | `units_t<ValueT, IntervalT, energy>`
`watts` | `units_t<ValueT, IntervalT, power>`
`volts` | `units_t<ValueT, IntervalT, voltage>`
`farads` | `units_t<ValueT, IntervalT, capacitance>`
`ohms` | `units_t<ValueT, IntervalT, impedance>`
`siemens` | `units_t<ValueT, IntervalT, conductance>`
`webers` | `units_t<ValueT, IntervalT, magnetic_flux>`
`teslas` | `units_t<ValueT, IntervalT, magnetic_flux_density>`
`henries` | `units_t<ValueT, IntervalT, inductance>`
`lumens` | `units_t<ValueT, IntervalT, luminous_flux>`

## Helper classes
Class | Description
------|------------
[`si::quantity_t`](quantity_t.md) | describes the SI quantities in a `units_t`
`std::hash<si::units_t>` | hash support for `units_t`

## Literals
Defined in inline namespace `si::literals`.

Function | Description
---------|------------
`operator""m` | A `units_t` literal representing meters
`operator""kg` | A `units_t` literal representing kilograms
`operator""g` | A `units_t` literal representing grams
`operator""s` | A `units_t` literal representing seconds
`operator""min` | A `units_t` literal representing minutes
`operator""h` | A `units_t` literal representing hours
`operator""d` | A `units_t` literal representing days
`operator""ms` | A `units_t` literal representing milliseconds
`operator""us` | A `units_t` literal representing microseconds
`operator""ns` | A `units_t` literal representing nanoseconds
`operator""A` | A `units_t` literal representing amperes
`operator""K` | A `units_t` literal representing kelvins
`operator""cd` | A `units_t` literal representing candelas
`operator""rad` | A `units_t` literal representing radians
`operator""mol` | A `units_t` literal representing moles
`operator""sr` | A `units_t` literal representing steradians
`operator""Hz` | A `units_t` literal representing hertz
`operator""N` | A `units_t` literal representing newtons
`operator""Pa` | A `units_t` literal representing pascals
`operator""J` | A `units_t` literal representing joules
`operator""W` | A `units_t` literal representing watts
`operator""C` | A `units_t` literal representing coulombs
`operator""V` | A `units_t` literal representing volts
`operator""F` | A `units_t` literal representing farads
`operator""O` | A `units_t` literal representing ohms
`operator""S` | A `units_t` literal representing siemens
`operator""Wb` | A `units_t` literal representing webers
`operator""T` | A `units_t` literal representing teslas
`operator""H` | A `units_t` literal representing henries
`operator""lm` | A `units_t` literal representing lumens
`operator""lx` | A `units_t` literal representing lux

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
