# si::quantity_t
Defined in header "quantity.hpp"
```c++
template
<
    std::intmax_t aMassExponent = 0,
    std::intmax_t aLengthExponent = 0,
    std::intmax_t aTimeExponent = 0,
    std::intmax_t aCurrentExponent = 0,
    std::intmax_t aTemperatureExponent = 0,
    std::intmax_t aLuminousIntensityExponent = 0,
    std::intmax_t aSubstanceExponent = 0,
    std::intmax_t aAngleExponent = 0
>
struct quantity_t;
```
The struct template `si::quantity_t` contain static constant members that represent the integer exponents of each type of quantity associated with an SI base or derived unit. All the exponents in `si::quantity_t` default to 0. An exponent equal to 0 effectively causes that quantity to not exist in the units of the type. Likewise an exponent of 2 indicates the corresponding quantity is squared. For example, length squared is area. A negative exponent indicates the corresponding quantity exists in the denominator of the quantity being represented.
## Member objects
Member | Definition
----------------------------------------|-----------------------------------------------------
constexpr std::intmax_t _**mass_exp**_ [static] | aMassExponent
constexpr std::intmax_t _**length_exp**_ [static] | aLengthExponent
constexpr std::intmax_t _**time_exp**_ [static] | aTimeExponent
constexpr std::intmax_t _**current_exp**_ [static] | aCurrentExponent
constexpr std::intmax_t _**temperature_exp**_ [static] | aTemperatureExponent
constexpr std::intmax_t _**luminous_intensity_exp**_ [static] | aLuminousIntensityExponent
constexpr std::intmax_t _**substance_exp**_ [static] | aSubstanceExponent
constexpr std::intmax_t _**angle_exp**_ [static] | aAngleExponent
## Operations on si::quantity_t
The following template metafunctions produce new `si::quantity_t` types by performing operations on existing `si::quantity_t` types.

Operation | Description
----------|------------
_**multiply_quantity**_ | Product of multiplying two `si::quantity_t` types
_**divide_quantity**_ | Quotient of dividing two `si::quantity_t` types
_**reciprocal_quantity**_ | Reciprocal of an `si::quantity_t` type
_**power_quantity**_ | Raise an `si::quantity_t` to a power
_**root_quantity**_ | Take the root of an `si::quantity_t`
