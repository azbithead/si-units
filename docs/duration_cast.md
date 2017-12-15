# si::duration_cast

```c++
template<typename ToDurationT, typename ValueT, typename IntervalT>
constexpr ToDurationT duration_cast(units_t<ValueT, IntervalT, si::time> aUnits);
```

Converts a `units_t` having `quantity_t` equal to `si::time` to a `std::chrono::duration`.

## Parameters
aUnits - a `units_t` to convert

## Return value
A `std::chrono::duration` of type `ToDurationT`.

## Notes
The function does not participate in overload resolution unless `ToDurationT` is of type `std::chrono::duration`.

## See also
Function | Description
---------|------------
[`units_cast`](units_cast.md) | converts a `std::chrono::duration` to a `units_t`
