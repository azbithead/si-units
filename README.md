# si-units

The si-units C++ library provides type-safe calculations involving physical quantities that are measured in the units of the [International System of Units](https://en.wikipedia.org/wiki/International_System_of_Units).

si-units is a header-only library of template classes and functions that automatically convert quantities to the correct units when performing mathematical or relational operations. The library contains two classes/structs: [`si::quantity_t`](quantity_t.md) and [`si::units_t`](units_t.md).

## si::quantity_t

[`si::quantity_t`](quantity_t.md) (see si/quantity.hpp) is a struct template containing static constant members that represent the integer exponents of each type of quantity associated with an SI base or derived unit. All the exponents in [`si::quantity_t`](quantity_t.md) default to 0. An exponent equal to 0 effectively causes that quantity to not exist in the units of the type. Likewise an exponent of 2 indicates the corresponding quantity is squared. For example, length squared is area. A negative exponent indicates the corresponding quantity exists in the denominator of the quantity being represented by [`si::quantity_t`](quantity_t.md).

The [`si::quantity_t`](quantity_t.md) examples provided below are intended to help the reader understand how the [`si::quantity_t`](quantity_t.md) type works. However, it is unlikely that clients of this library will need to write code that defines [`si::quantity_t`](quantity_t.md) types since those types are automatically created as needed when code performs mathematical or relational operations using [`si::units_t`](units_t.md).

The library defines type aliases based on [`si::quantity_t`](quantity_t.md) for all quantities related to base SI units and most derived SI units.

### Example 1

The time exponent in [`si::quantity_t`](quantity_t.md) is provided in the third template parameter. So, a quantity of time in [`si::quantity_t`](quantity_t.md) looks like this:

```c++
using time = si::quantity_t<0, 0, 1>;
```

### Example 2

The length or distance exponent is [`si::quantity_t`](quantity_t.md) is provided in the second template parameter. So, a quantity of length in [`si::quantity_t`](quantity_t.md) looks like this:

```c++
using length = si::quantity_t<0, 1>;
```

### Example 3

Speed is defined as length or distance divided by time. Expressed as [`si::quantity_t`](quantity_t.md) that would be:

```c++
using speed = si::divide_quantity<si::length, si::time>; // same as si::quantity_t<0, 1, -1>
```

## si::units_t

[si::units_t](units_t.md)

[`si::units_t`](units_t.md) is a class template that takes three template parameters:

* QUANTITY: an [`si::quantity_t`](quantity_t.md)
* VALUE: an arithmetic value, can be any C++ arithmetic type, i.e. std::is_arithmetic\<VALUE\> == true
* INTERVAL: std::ratio representing the amount of QUANTITY per VALUE in these units

[`si::units_t`](units_t.md) is a very lightweight class both in terms of memory usage and runtime performance. The memory usage of an [`si::units_t`](units_t.md) object is exactly equal to sizeof(VALUE). Only operations involving the VALUE component are done at runtime. All other operations are done at compile time.

[`si::units_t`](units_t.md) is type safe. Operations on [`si::units_t`](units_t.md) objects will not compile successfully if they involve incompatible quantities or would result in silent loss of precision. For example, an attempt to compare an object with mass quantity to an object with time quantity will not compile. Likewise, addition and subtraction only compiles for objects with the same quantity type. However, multiplication and division is possible for objects with mixed quantity types, resulting in a new object with the appropriate quantity type.

Note that VALUE can be either an integral or floating point type. If an integral type is chosen, all mathematical and conversion operations will produce truncated results where appropriate just as they would in such operations on the raw VALUE type.

The library defines type aliases for all of the SI base units and almost all SI derived units. These aliases are templates where VALUE defaults to double and INTERVAL defaults to std::ratio<1>. For example, since the SI base unit for mass is the kilogram, the following type alias is provided in the si namespace:

```c++
template< typename INTERVAL = std::ratio<1>, typename VALUE = double >
using kilograms = si::units_t<VALUE, INTERVAL, mass>;
```

### Example 1

An [`si::units_t`](units_t.md) of 50 millimeters has value = 50, interval = 1/1000 and [`si::quantity_t`](quantity_t.md) with length exponent = 1.

An [`si::units_t`](units_t.md) can be thought of as the product of multiplying all of its components. In this example that would be 50 * (1/1000) * (length^1). If we perform that math on a calculator, we see that 50 millimeters is equal to 0.05 meters, expressed in SI base units.

The declaration of a variable containing 50 millimeters could be written as:

```c++
auto theLength = si::meters<std::milli>{50.0};
```

where:

```c++
theLength.value() == 50.0 
&& theLength.interval == std::milli{} // == std::ratio<1,1000>{}
&& theLength.quantity == si::length{} // == si::quantity_t<0, 1>{}
```

### Example 2

An [`si::units_t`](units_t.md) of 55 kilometers/hour has value = 55, interval = 1000/3600 and [`si::quantity_t`](quantity_t.md) with length exponent = 1 and time exponent = -1.

Let’s break that down. The value component should be self evident. The interval component has a numerator of 1000. That represents the kilo part of kilometers. The interval denominator equals the number of seconds in one hour or 3600. The 3600 is in the denominator because “hour” is in the denominator of the desired units. The length exponent is 1 as noted in example 1. The time exponent is -1, again because “hour” is in the denominator of the desired units.

Expressing this quantity as a product gives 55 * (1000/3600) * (length^1*time^-1). Again, using a calculator, we find that 55 kilometers/hour is equal to approximately 15.28 meters/second. Note that we said “approximately”, however this [`si::units_t`](units_t.md) will actually not lose any precision due to the use of the interval.

The easiest way to declare the variable containing 55 kilometers/hour is:

```c++
auto theSpeed = si::meters<std::kilo>{55.0} / si::hours<>{1.0};
```

where:

```c++
theSpeed.value() == 55.0
&& theSpeed.interval == std::ratio<5, 18>{} // automatically reduced from std::ratio<1000, 3600>{}
&& theSpeed.quantity == si::quantity_t<0, 1, -1>{} // == si::length{} / si::time{}
```

### Example 3

Given the variables defined in examples 1 and 2, we’d like to know how much time it takes to travel 50 millimeters if our speed is 55 kilometers/hour. So, we divide 50 millimeters by 55 kilometers/hour:

```c++
auto theTime = theLength / theSpeed;
```

where:

```c++
theTime.value == 0.90909090909090906
&& theTime.interval == std::ratio<9, 2500>{} // == std::ratio<1, 1000>{} / std::ratio<5, 18>{}
&& theTime.quantity == si::time{} // == si::length{} / (si::length{} / si::time{})
```

You may be thinking that the value 0.909 * 9/2500 seconds is not very useful. It is mathematically correct and could be used in subsequent calculations, if desired. However, if we want to display this result to a user, we would probably want a value that is easier to understand. Looking at the components of this result, we can see that it is much less than a second. So, perhaps converting it to milliseconds would be good. We can use the si::units_cast template function to do that:

```c++
auto theMsecs = si::units_cast<si::seconds<std::milli>>(theTime);
```

Or, here's an even easier way:

```c++
auto theMsecs = si::seconds<std::milli>{theLength / theSpeed};
```

where:

```c++
theMsecs.value == 3.2727272727272725
&& theMsecs.interval == std::milli{}
&& theMsecs.quantity == si::time{}
```
## Conversion to String

The library provides functions to convert [`si::quantity_t`](quantity_t.md) and [`si::units_t`](units_t.md) to strings. The functions are named "string_from" and "wstring_from" and return std::string and std::wstring respectively. Note that the string conversion functions for [`si::units_t`](units_t.md) only output the INTERVAL and QUANTITY components and not the VALUE component. Clients should use string conversion functions provided by the standard C++ library to convert the result of calling value() on an [`si::units_t`](units_t.md) object. When converting an [`si::quantity_t`](quantity_t.md), an abbreviation for a derived SI unit will be output when possible.

### Example

Running the code below with the variables declared in the [`si::units_t`](units_t.md) examples above:

```c++
std::cout << theLength.value() << " " << si::string_from(theLength) << "\n";
std::cout << theSpeed.value() << " " << si::string_from(theSpeed) << "\n";
std::cout << theTime.value() << " " << si::string_from(theTime) << "\n";
std::cout << theMsecs.value() << " " << si::string_from(theMsecs) << "\n";
```
Output:

```
50 10⁻³ m
55 5/18 m/s
0.909091 9/25·10⁻² s
3.27273 10⁻³ s
```

## Streaming

The library provides a function to stream [`si::units_t`](units_t.md) to any stream derived from std::ostream. Unlike string_from, this function does output the VALUE component.

### Example

Running the code below with the variables declared in the [`si::units_t`](units_t.md) examples above:

```c++
std::cout << theLength << "\n";
std::cout << theSpeed << "\n";
std::cout << theTime << "\n";
std::cout << theMsecs << "\n";
```
Output:

```
50·10⁻³ m
55·5/18 m/s
0.909091·9/25·10⁻² s
3.27273·10⁻³ s
```
