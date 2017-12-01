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
_**quantity_t**_ | QUANTITY
_**value_t**_ | VALUE
_**interval_t**_ | typename INTERVAL::type
## Member objects
Member | Description
----------------------------------------|-----------------------------------------------------
constexpr interval_t _**interval**_ \[static] | equal to INTERVAL{}
constexpr quantity_t _**quantity**_ \[static] | equal to QUANTITY{}

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
## Helper types
