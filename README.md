The si-quantity library provides type-safe calculations involving physical quantities that are measured in the base units of the International System of Units (SI):

meter - length (or distance)
kilogram - mass
second - time
ampere - electric current
kelvin - thermodynamic temperature
mole - amount of substance
candela - luminous intensity

si-quantity is a header-only library of template classes and functions that automatically convert quantities to the correct units when performing mathematical or relational operations. The library contains two classes/structs: si::units and si::quantity.

si::units

si::units is a struct template containing static constant members that represent the integer exponents associated with each base unit. All the exponents in si::units default to 0. An exponent equal to 0 effectively causes that base unit to not exist in the units of the type. Likewise an exponent of 2 indicates the corresponding unit is squared. For example, length squared is area. A negative exponent indicates the corresponding unit exists in the denominator of the units being represented by si::units.

The si::units examples provided below are intended to help the reader understand how the si::units type works. However, it is unlikely that clients of this library will need to write code that defines si::units types since those types are automatically created as needed when code performs mathematical or relational operations using si::quantity.

si::units example 1:

The time exponent in si::units is provided in the third template parameter. So, time units in si::units looks like this:

using time = si::units<0, 0, 1>;

si::units example 2:

The length or distance exponent is si::units is provided in the second template parameter. So, length units in si::units looks like this:

using length = si::units<0, 1>;

si::units example 3:

Speed is defined as length or distance divided by time. Expressed as si::units that would be:

using speed = si::divide_units<si::length, si::time>; // same as si::units<0, 1, -1>

si::quantity

si::quantity is a class template that takes three template parameters:

UNITS: si::units
VALUE: an arithmetic value, can be any C++ arithmetic type, i.e. std::is_arithmetic<VALUE> == true
RATIO: std::ratio representing the number of UNITS per VALUE in this quantity

si::quantity is a very lightweight class both in terms of memory usage and runtime performance. The memory usage of an si::quantity object is exactly equal to sizeof(VALUE). Only operations involving the VALUE component are done at runtime. All other operations are done at compile time.

si::quantity is type safe. Operations on si::quantity objects will not compile successfully if they involve incompatible units or would result in silent loss of precision. For example, an attempt to compare an object with mass units to an object with time units will not compile. Likewise, addition and subtraction only compiles for objects with the same units. However, multiplication and division is possible for objects with mixed units, resulting in a new object with the appropriate units.

Note that VALUE can be either an integral or floating point type. If an integral type is chosen, all mathematical and conversion operations will produce truncated results where appropriate just as they would in such operations on the raw VALUE type.

Convenience type aliases are defined for all of the SI base units and almost all SI derived units. These aliases are templates where VALUE defaults to double and RATIO defaults to std::ratio<1>. For example, since the SI base unit for mass is the kilogram, the following type alias is provided in the si namespace:

template< typename RATIO = std::ratio<1>, typename VALUE = double >
using kilograms = quantity<VALUE, RATIO, mass>;

si::quantity example 1:

A quantity of 50 millimeters has value = 50, ratio = 1/1000 and units with length exponent = 1.

A quantity can be thought of as the product of multiplying all of its components. In this example that would be 50 * (1/1000) * (meters^1). If we perform that math on a calculator, we see that 50 millimeters is equal to 0.05 meters, expressed in SI base units.

The declaration of a variable containing 50 millimeters could be written as:

auto theLength = si::meters<std::milli>{50.0};

where:

theLength.value() == 50.0 
&& theLength.ratio == std::milli{} // == std::ratio<1,1000>{}
&& theLength.units == si::length{} // == si::units<0, 1>{}

si::quantity example 2:

A quantity of 55 kilometers/hour has value = 55, ratio = 1000/3600 and units with length exponent = 1 and time exponent = -1.

Let’s break that down. The value component should be self evident. The ratio component has a numerator of 1000. That represents the kilo part of kilometers. The ratio denominator equals the number of seconds in one hour or 3600. The 3600 is in the denominator because “hour” is in the denominator of the desired units. Finally, length exponent is 1 as noted in example 1. The time exponent is -1, again because “hour” is in the denominator of the desired units.

Expressing this quantity as a product gives 55 * (1000/3600) * (meters^1*seconds^-1). Again, using a calculator, we find that 55 kilometers/hour is equal to approximately 15.28 meters/second. Note that we said “approximately” however this quantity will actually not lose any precision within the library due to the use of the ratio.

The easiest way to declare the variable containing 55 kilometers/hour is:

auto theSpeed = si::meters<std::kilo>{55.0} / si::hours<>{1.0};

where:

theSpeed.value() == 55.0
&& theSpeed.ratio == std::ratio<5, 18>{} // automatically reduced from std::ratio<1000, 3600>{}
&& theSpeed.units == si::units<0, 1, -1>{} // == si::length{} / si::time{}

si::quantity example 3:

Given the quantities in examples 1 and 2, we’d like to know how much time it takes to travel 50 millimeters if our speed is 55 kilometers/hour. So, we divide 50 millimeters by 55 kilometers/hour:

auto theTime = theLength / theSpeed;

where:

theTime.value == 0.90909090909090906
&& theTime.ratio == std::ratio<9, 2500>{} // == std::ratio<1, 1000>{} / std::ratio<5, 18>{}
&& theTime.units == si::time{} // == si::length{} / (si::length{} / si::time{})

You may be thinking that the value 0.909 * 9/2500 seconds is not very useful. It is actually correct and could be used in subsequent calculations, if desired. However, if we want to display this result to a user, we would probably want a value that is easier to understand. Looking at the components of this result, we can see that it is much less than a second. So, perhaps converting it to milliseconds would be good. We can use the si::quantity_cast template function to do that:

theMsecs = si::quantity_cast<si::seconds<std::milli>>(theTime);

where:

theMsecs.value == 3.2727272727272725
&& theMsecs.ratio == std::milli{}
&& theMsecs.units == si::time{}
