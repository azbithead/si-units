# si::quantity_t
Defined in header "quantity.hpp"
```c++
template
<
    std::intmax_t MassExponent = 0,
    std::intmax_t LengthExponent = 0,
    std::intmax_t TimeExponent = 0,
    std::intmax_t CurrentExponent = 0,
    std::intmax_t TemperatureExponent = 0,
    std::intmax_t LuminousIntensityExponent = 0,
    std::intmax_t SubstanceExponent = 0,
    std::intmax_t AngleExponent = 0
>
struct quantity_t;
```
The struct template `quantity_t` contains static constant members that represent the integer exponents of each type of quantity associated with an SI base or derived unit. All the exponents in `quantity_t` default to 0. An exponent equal to 0 effectively causes that quantity to not exist in the units of the type. Likewise an exponent of 2 indicates the corresponding quantity is squared. For example, length squared is area. A negative exponent indicates the corresponding quantity exists in the denominator of the quantity being represented.
## Member objects
Member | Definition
----------------------------------------|-----------------------------------------------------
`static constexpr std::intmax_t mass_exp` | `MassExponent`
`static constexpr std::intmax_t length_exp` | `LengthExponent`
`static constexpr std::intmax_t time_exp` | `TimeExponent`
`static constexpr std::intmax_t current_exp` | `CurrentExponent`
`static constexpr std::intmax_t temperature_exp` | `TemperatureExponent`
`static constexpr std::intmax_t luminous_intensity_exp` | `LuminousIntensityExponent`
`static constexpr std::intmax_t substance_exp` | `SubstanceExponent`
`static constexpr std::intmax_t angle_exp` | `AngleExponent`

## Operations
The following template metafunctions produce new `quantity_t` types by performing operations on existing `quantity_t` types.

Operation | Description
----------|------------
`multiply_quantity` |  Product of multiplying two `quantity_t` types
`divide_quantity` |  Quotient of dividing two `quantity_t` types
`reciprocal_quantity` |  Reciprocal of an `quantity_t` type
`power_quantity` |  Raise an `quantity_t` to a power
`root_quantity` |  Take the root of an `quantity_t`

## Helper types
Type | Definition
-----|-----------
`none` | `quantity_t<>`
`mass` | `quantity_t<1>`
`length` | `quantity_t<0,1>`
`distance` | `length`
`time` | `quantity_t<0,0,1>`
`current` | `quantity_t<0,0,0,1> // electric current`
`temperature` | `quantity_t<0,0,0,0,1>`
`luminous_intensity` | `quantity_t<0,0,0,0,0,1>`
`substance` | `quantity_t<0,0,0,0,0,0,1> // amount of substance`
`angle` | `quantity_t<0,0,0,0,0,0,0,1>`
`solid_angle` | `power_quantity<angle,2>`
`frequency` | `reciprocal_quantity<time>`
`force` | `divide_quantity<multiply_quantity<mass, length>, power_quantity<time,2>>`
`weight` | `force`
`area` | `power_quantity<length,2>`
`pressure` | `divide_quantity<force, area>`
`stress` | `pressure`
`energy` | `multiply_quantity<force, length>`
`work` | `energy`
`power` | `divide_quantity<energy, time>`
`charge` | `multiply_quantity<current, time>`
`voltage` | `divide_quantity<power, current>`
`capacitance` | `divide_quantity<charge, voltage>`
`impedance` | `divide_quantity<voltage, current>`
`conductance` | `reciprocal_quantity<impedance>`
`magnetic_flux` | `divide_quantity<energy, current>`
`magnetic_flux_density` | `divide_quantity<magnetic_flux, area>`
`inductance` | `divide_quantity<magnetic_flux, current>`
`luminous_flux` | `multiply_quantity<luminous_intensity, solid_angle>`
`illuminance` | `divide_quantity<luminous_intensity, area>`

## Non-member functions
Function | Description
---------|------------
`basic_string_from` | returns `std::basic_string` containing representation of a `quantity_t`
`string_from` | returns `std::string` containing representation of a `quantity_t`
`wstring_from` | returns `std::wstring` containing representation of a `quantity_t`
