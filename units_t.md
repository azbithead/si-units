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
## Non-member functions
## Helper types
