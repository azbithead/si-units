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
The struct template `si::quantity_t` contains static constant members that represent the integer exponents of each type of quantity associated with an SI base or derived unit. All the exponents in `si::quantity_t` default to 0. An exponent equal to 0 effectively causes that quantity to not exist in the units of the type. Likewise an exponent of 2 indicates the corresponding quantity is squared. For example, length squared is area. A negative exponent indicates the corresponding quantity exists in the denominator of the quantity being represented.
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
## Operations
The following template metafunctions produce new `si::quantity_t` types by performing operations on existing `si::quantity_t` types.

Operation | Description
----------|------------
_**multiply_quantity**_ | Product of multiplying two `si::quantity_t` types
_**divide_quantity**_ | Quotient of dividing two `si::quantity_t` types
_**reciprocal_quantity**_ | Reciprocal of an `si::quantity_t` type
_**power_quantity**_ | Raise an `si::quantity_t` to a power
_**root_quantity**_ | Take the root of an `si::quantity_t`
## Helper types
Type | Definition
-----|-----------
_**none**_ | quantity_t<>
_**mass**_ | quantity_t<1>
_**length**_ | quantity_t<0,1>
_**distance**_ | length
_**time**_ | quantity_t<0,0,1>
_**current**_ | quantity_t<0,0,0,1> // electric current
_**temperature**_ | quantity_t<0,0,0,0,1>
_**luminous_intensity**_ | quantity_t<0,0,0,0,0,1>
_**substance**_ | quantity_t<0,0,0,0,0,0,1> // amount of substance
_**angle**_ | quantity_t<0,0,0,0,0,0,0,1>
_**solid_angle**_ | power_quantity<angle,2>
_**frequency**_ | reciprocal_quantity\<time>
_**force**_ | divide_quantity<multiply_quantity<mass, length>, power_quantity<time,2>>
_**weight**_ | force
_**area**_ | power_quantity<length,2>
_**pressure**_ | divide_quantity<force, area>
_**stress**_ | pressure
_**energy**_ | multiply_quantity<force, length>
_**work**_ | energy
_**power**_ | divide_quantity<energy, time>
_**charge**_ | multiply_quantity<current, time>
_**voltage**_ | divide_quantity<power, current>
_**capacitance**_ | divide_quantity<charge, voltage>
_**impedance**_ | divide_quantity<voltage, current>
_**conductance**_ | reciprocal_quantity\<impedance>
_**magnetic_flux**_ | divide_quantity<energy, current>
_**magnetic_flux_density**_ | divide_quantity<magnetic_flux, area>
_**inductance**_ | divide_quantity<magnetic_flux, current>
_**luminous_flux**_ | multiply_quantity<luminous_intensity, solid_angle>
_**illuminance**_ | divide_quantity<luminous_intensity, area>
