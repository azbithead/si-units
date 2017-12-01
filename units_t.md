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

It consists of a value of type VALUE, an interval of type INTERVAL and a quantity of type QUANTITY. `std::is_arithmetic<VALUE>::value` must evaluate to `true`. The interval is a compile-time rational constant representing one unit of quantity. INTERVAL must be of type [`std::ratio`](http://en.cppreference.com/w/cpp/numeric/ratio/ratio). Finally, QUANTITY must be of type [`si::quantity_t`](quantity_t.md).
	
The only data stored in a `units_t` is a value of type VALUE. If VALUE is floating point, then the `units_t` can represent fractions of intervals. QUANTITY is included as part of the `units_t`'s type, and is only used when converting between different `units_t`.

## Member types
Member type | Definition
------------|-----------
_**quantity\_t**_ | QUANTITY
_**value\_t**_ | VALUE
_**interval\_t**_ | typename INTERVAL::type
## Member objects
Member | Description
----------------------------------------|-----------------------------------------------------
constexpr interval\_t _**interval**_ \[static] | equal to INTERVAL{}
constexpr quantity\_t _**quantity**_ \[static] | equal to QUANTITY{}

## Member functions
Function | Description
---------|------------
_**(constructor)**_ | constructs new `units_t`
_**operator=**_ | assigns the contents
_**value**_ | returns the value of intervals
_**operator+<br>operator-**_ | implements unary plus and unary minus
_**operator++<br>operator++(int)<br>operator--<br>operator--(int)**_ | increments or decrements value
_**operator+=<br>operator-=<br>operator\*=<br>operator/=<br>operator%=**_ | implements compound assignment between two `units_t`
_**zero [static]**_ | returns the special `units_t` value zero
_**min [static]**_ | returns the special `units_t` value min
_**max [static]**_ | returns the special `units_t` value max

## Non-member functions
Function | Description
---------|------------
_**std::common\_type\<si::units\_t\>**_ | specializes the std::common_type trait
_**operator+<br>operator-<br>operator\*<br>operator/<br>operator%**_ | implements arithmetic operations with units_t as arguments
_**operator==<br>operator!=<br>operator\<<br>operator\<=<br>operator\><br>operator\>=**_ | compares two units_t
_**units\_cast**_ | converts a units_t to another with a different interval_t
_**duration\_cast**_ | converts seconds to std::chrono::duration
_**absolute**_ | returns the absolute value of a units_t
_**floor**_ | converts a units_t to another, rounding down
_**ceiling**_ | converts a units_t to another, rounding up
_**round**_ | converts a units_t to another, rounding to nearest, ties to even
_**exponentiate**_ | computes the value of a units_t raised to a power
_**sine**_ | computes the sine of a radians
_**cosine**_ | computes the cosine of a radians
_**tangent**_ | computes the tangent of a radians
_**arc\_sine**_ | returns radians given a sine scalar
_**arc\_cosine**_ | returns radians given a cosine scalar
_**arc\_tangent**_ | returns radians given a tangent scalar

## Helper types
Type | Definition
---------|------------
_**scalar**_ | units\_t\<VALUE, INTERVAL, none\>
_**meters**_ | units\_t\<VALUE, INTERVAL, length\>
_**kilograms**_ | units\_t\<VALUE, INTERVAL, mass\>
_**seconds**_ | units\_t\<VALUE, INTERVAL, time\>
_**minutes**_ | seconds\<std::ratio\<60\>, VALUE\>
_**hours**_ | seconds\<std::ratio\<60*60\>, VALUE\>
_**days**_ | seconds\<std::ratio\<24*60*60\>, VALUE\>
_**milliseconds**_ | seconds\<std::milli, VALUE\>
_**microseconds**_ | seconds\<std::micro, VALUE\>
_**nanoseconds**_ | seconds\<std::nano, VALUE\>
_**amperes**_ | units\_t\<VALUE, INTERVAL, current\>
_**kelvins**_ | units\_t\<VALUE, INTERVAL, temperature\>
_**candelas**_ | units\_t\<VALUE, INTERVAL, luminous\_intensity\>
_**radians**_ | units\_t\<VALUE, INTERVAL, angle\>
_**steradians**_ | units\_t\<VALUE, INTERVAL, solid\_angle\>
_**hertz**_ | units\_t\<VALUE, INTERVAL, frequency\>
_**newtons**_ | units\_t\<VALUE, INTERVAL, force\>
_**coulombs**_ | units\_t\<VALUE, INTERVAL, charge\>
_**lux**_ | units\_t\<VALUE, INTERVAL, illuminance\>
_**pascals**_ | units\_t\<VALUE, INTERVAL, pressure\>
_**joules**_ | units\_t\<VALUE, INTERVAL, energy\>
_**watts**_ | units\_t\<VALUE, INTERVAL, power\>
_**volts**_ | units\_t\<VALUE, INTERVAL, voltage\>
_**farads**_ | units\_t\<VALUE, INTERVAL, capacitance\>
_**ohms**_ | units\_t\<VALUE, INTERVAL, impedance\>
_**siemens**_ | units\_t\<VALUE, INTERVAL, conductance\>
_**webers**_ | units\_t\<VALUE, INTERVAL, magnetic\_flux\>
_**teslas**_ | units\_t\<VALUE, INTERVAL, magnetic\_flux\_density\>
_**henries**_ | units\_t\<VALUE, INTERVAL, inductance\>
_**lumens**_ | units\_t\<VALUE, INTERVAL, luminous\_flux\>